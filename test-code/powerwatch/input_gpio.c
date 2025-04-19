#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#define GPIO0_ADDR_START 0x000600000
#define GPIO0_ADDR_END 0x0006000FF

#define GPIO0_SIZE (GPIO0_ADDR_END - GPIO0_ADDR_START)
/*All 4 inputs on Bank2, so we need substract 32 from GPIO' number*/
// con0 con1 Output
// con2 con3 Inputs
#define gpio_con0 (1 << (41 - 32))
#define gpio_con1 (1 << (33 - 32))
#define gpio_con2 (1 << (36 - 32))
#define gpio_con3 (1 << (42 - 32))
/*Address  to access GPIO controller BANK 2 */
#define GPIO_DIR_OFFSET 0x000000038
#define GPIO_OUT_DATA_OFFSET 0x00000003C
#define GPIO_SET_DATA_OFFSET 0x000000040
#define GPIO_CLR_DATA_OFFSET 0x000000044
#define GPIO_IN_DATA_OFFSET 0x000000048
// GPIO Bank 0: 0:15, Bank 1: 16:31, Bank 2: 32:47, Bank 3: 48:63, Bank 4:
// 64:79, Bank 5: 80:86
void *gpioAddress;
uint32_t *gpio_setdataout_addr;
uint32_t *gpio_direction_addr;
uint32_t *gpio_cleardata_addr;
uint32_t *gpio_getdata_addr;
void delay(unsigned long ms) {
  clock_t start_ticks = clock();
  unsigned long millis_ticks = CLOCKS_PER_SEC / 1000;
  while (clock() - start_ticks < ms * millis_ticks) {
  }
}

int main() {
  int fd = open("/dev/mem", O_RDWR);
  int sample;
  gpioAddress = mmap(0, GPIO0_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                     GPIO0_ADDR_START);

  close(fd);

  gpio_setdataout_addr = gpioAddress + GPIO_SET_DATA_OFFSET;
  gpio_direction_addr = gpioAddress + GPIO_DIR_OFFSET;
  gpio_cleardata_addr = gpioAddress + GPIO_CLR_DATA_OFFSET;
  gpio_getdata_addr = gpioAddress + GPIO_IN_DATA_OFFSET;
  // Direction 0: Output 1: Input
  *gpio_direction_addr |= (gpio_con2 | gpio_con3);
  *gpio_direction_addr &= ~(gpio_con0 | gpio_con1);

  while (1) {

    *gpio_setdataout_addr |= gpio_con0;
    *gpio_setdataout_addr |= gpio_con1;
    delay(250);
    sample = (*gpio_getdata_addr);
    printf("INPUT = %x \n", ((sample & (gpio_con2 | gpio_con3) & 0x00000FFF)));
    delay(250);
    *gpio_cleardata_addr |= gpio_con0;
    *gpio_setdataout_addr |= gpio_con1;
    delay(250);
    sample = (*gpio_getdata_addr);
    printf("GPIO_CN2_3 = %x \n",
           ((sample & (gpio_con2 | gpio_con3) & 0x00000FFF)));
    delay(250);
    *gpio_setdataout_addr |= gpio_con0;
    *gpio_cleardata_addr |= gpio_con1;
    delay(250);
    sample = (*gpio_getdata_addr);
    printf("GPIO_CN2_3 = %x \n",
           ((sample & (gpio_con2 | gpio_con3) & 0x00000FFF)));
    delay(250);
    *gpio_cleardata_addr |= gpio_con0;
    *gpio_cleardata_addr |= gpio_con1;
    delay(250);
    sample = (*gpio_getdata_addr);
    printf("GPIO_CN2_3 = %x \n",
           ((sample & (gpio_con2 | gpio_con3) & 0x00000FFF)));
    delay(250);
  }
  return 0;
}
