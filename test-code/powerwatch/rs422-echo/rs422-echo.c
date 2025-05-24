#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

char debug;

void delay(unsigned long ms) {
  // clock_t start_ticks = clock();
  // unsigned long millis_ticks = CLOCKS_PER_SEC / 1000;
  // while (clock() - start_ticks < ms * millis_ticks) {
  // }
  usleep(ms * 1000);
}

// Función para configurar el puerto serial
int configure_serial_port(int fd, int speed) {
  struct termios tty;

  if (tcgetattr(fd, &tty) < 0) {
    perror("Error en tcgetattr");
    return -1;
  }

  // Configurar velocidad de entrada/salida
  cfsetospeed(&tty, (speed_t)speed);
  cfsetispeed(&tty, (speed_t)speed);

  // Configurar 8N1
  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;

  // Habilitar lectura e ignorar control de línea
  tty.c_cflag |= (CLOCAL | CREAD);

  // Deshabilitar control de flujo por hardware
  tty.c_cflag &= ~CRTSCTS;

  // Configuración para modo no canónico
  tty.c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tty.c_oflag &= ~OPOST;

  // Configurar tiempo de espera
  tty.c_cc[VMIN] = 1;
  tty.c_cc[VTIME] = 10;

  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    perror("Error en tcsetattr");
    return -1;
  }

  return 0;
}

// Función para establecer el estado de los pines RTS y CTS
int set_rts_cts(int fd, int rts_state, int cts_state) {
  int status;

  // Obtener el estado actual de los pines de control
  if (ioctl(fd, TIOCMGET, &status)) {
    perror("Error en ioctl(TIOCMGET)");
    return -1;
  }

  // Establecer RTS (Request To Send)
  if (rts_state) {
    status |= TIOCM_RTS;
  } else {
    status &= ~TIOCM_RTS;
  }

  // CTS es un pin de entrada, no podemos establecerlo directamente
  // Solo podemos leerlo. Esta parte es para mostrar el estado.
  if (debug) {
    printf("Estado CTS actual: %s\n", (status & TIOCM_CTS) ? "ALTO" : "BAJO");
  }

  // Aplicar los cambios a RTS
  if (ioctl(fd, TIOCMSET, &status)) {
    perror("Error en ioctl(TIOCMSET)");
    return -1;
  }

  return 0;
}
// rs422-echo
// /dev/ttyS3 (serialport) [1]
// 250 (delay_ms) [2]
// 10 (loops) [3]
// any (debug) [4]

int main(int argc, char *argv[]) {
  srand(time(NULL)); // generar semilla para aleatorios
  int fd;
  char *portname;
  int speed = B9600; // Velocidad por defecto: 9600 baudios
  if (argc < 4) {
    // fprintf(stderr, "Uso: %s <dispositivo_serial>\n", argv[0]);
    printf("How to use?:\n");
    printf("%s serialport delay_ms loops debug\n", argv[0]);
    printf("Example:\n");
    printf("%s /dev/ttyS3 250 10 d\n", argv[0]);
    printf("%s /dev/ttyS3 100 20\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  portname = argv[1];

  int delay_ms = 250;
  int loops = 10;
  if (argc >= 3) {
    delay_ms = atoi(argv[2]);
    loops = atoi(argv[3]);
  }

  // Activate debug with 4 argument
  if (argc == 5) {
    debug = 1;
  } else {
    debug = 0;
  }
  // Abrir el puerto serial
  fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    perror("Error opening serial port");
    exit(EXIT_FAILURE);
  }

  // Configurar el puerto serial
  if (configure_serial_port(fd, speed) < 0) {
    close(fd);
    exit(EXIT_FAILURE);
  }
  set_rts_cts(fd, 0, 0);
  int ret;
  int count = 0;
  int i = 0;
  char msg2send[12] = "rs422echo0\n";
  for (i = 0; i < loops; i++) {
    char number = rand() % 10;
    msg2send[9] = 48 + number; // ASCCI int 48 = char 0
    ret = write(fd, msg2send, strlen(msg2send));
    if (debug) {
      printf("Sent %d bytes: %s", ret, msg2send);
    }
    delay(delay_ms);
    char msg2received[16];
    memset(msg2received, 0, sizeof(msg2received));
    ret = read(fd, msg2received, sizeof(msg2received) - 1);
    if (ret > 0) {
      msg2received[ret] = '\0'; // Añadir terminador nulo
    }
    if (debug) {
      printf("Received %d bytes: %s", ret, msg2received);
    }
    ret = strcmp(msg2send, msg2received);
    if (debug) {
      printf("Difference: %d\n", ret);
    }
    if (ret == 0) {
      count += 1;
    }
  }
  if (debug) {
    printf("Expected: %d\n", loops);
    printf("Asserts: %d\n", count);
  }
  printf("%d\n", count);
  close(fd);
  exit(EXIT_SUCCESS);
}
