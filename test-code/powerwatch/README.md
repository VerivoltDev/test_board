
```bash
gpiodetect # ver bloques de gpio disponibles
```

## Particiones y tipo de montaje

1. Verificar cómo está montado el filesystem:

```bash
mount | grep -e "/lib/firmware" -e "/ "
```

2. Remontar en modo lectura/escritura:

```bash
mount -o remount,rw /
```

3. Si falla el remount:

    Causa común: El filesystem está corrupto o el hardware (ej. eMMC, SSD) tiene errores.
    Verifica con:

```bash
dmesg | grep -i "error\|read-only\|filesystem"
```

4. Alternativa temporal:

Si no puedes remontar, prueba a escribir en otro directorio (ej. /tmp/) y luego copia el archivo a /lib/firmware/ con cp (a veces funciona aunque el filesystem sea ro):

```bash
touch /tmp/help && cp /tmp/help /lib/firmware/
```
