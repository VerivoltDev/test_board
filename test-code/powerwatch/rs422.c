#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

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
  printf("Estado CTS actual: %s\n", (status & TIOCM_CTS) ? "ALTO" : "BAJO");

  // Aplicar los cambios a RTS
  if (ioctl(fd, TIOCMSET, &status)) {
    perror("Error en ioctl(TIOCMSET)");
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int fd;
  char *portname;
  int speed = B9600; // Velocidad por defecto: 9600 baudios

  if (argc < 2) {
    fprintf(stderr, "Uso: %s <dispositivo_serial>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  portname = argv[1];

  // Abrir el puerto serial
  fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    perror("Error al abrir el puerto serial");
    exit(EXIT_FAILURE);
  }

  // Configurar el puerto serial
  if (configure_serial_port(fd, speed) < 0) {
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Puerto serial %s configurado a %d baudios\n", portname, speed);

  // Menú de opciones
  while (1) {
    printf("\nOpciones:\n");
    printf("1. Enviar dato\n");
    printf("2. Leer dato\n");
    printf("3. Configurar RTS/CTS\n");
    printf("4. Salir\n");
    printf("Seleccione una opción: ");

    int option;
    scanf("%d", &option);
    getchar(); // Limpiar el buffer de entrada

    switch (option) {
    case 1: {
      // Enviar dato
      char buffer[256];
      printf("Ingrese el dato a enviar: ");
      fgets(buffer, sizeof(buffer), stdin);

      int n = write(fd, buffer, strlen(buffer));
      if (n < 0) {
        perror("Error al escribir en el puerto serial");
      } else {
        printf("Enviados %d bytes: %s", n, buffer);
      }
      break;
    }
    case 2: {
      // Leer dato
      char buf[256];
      printf("Leyendo datos... (presione Ctrl+C para cancelar)\n");

      int n = read(fd, buf, sizeof(buf) - 1);
      if (n < 0) {
        perror("Error al leer del puerto serial");
      } else if (n == 0) {
        printf("No se recibieron datos\n");
      } else {
        buf[n] = '\0';
        printf("Recibidos %d bytes: %s", n, buf);
      }
      break;
    }
    case 3: {
      // Configurar RTS/CTS
      int rts, cts;
      printf("Ingrese estado para RTS (0=BAJO, 1=ALTO): ");
      scanf("%d", &rts);
      printf("Ingrese estado para CTS (0=BAJO, 1=ALTO - solo lectura): ");
      scanf("%d", &cts);

      if (set_rts_cts(fd, rts, cts) == 0) {
        printf("RTS configurado a %s\n", rts ? "ALTO" : "BAJO");
      }
      break;
    }
    case 4:
      // Salir
      close(fd);
      printf("Saliendo...\n");
      exit(EXIT_SUCCESS);
    default:
      printf("Opción no válida\n");
    }
  }

  return 0;
}
