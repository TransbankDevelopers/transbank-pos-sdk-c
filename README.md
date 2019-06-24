# Transbank POS - SDK en C

## Desarrollo

Descarga e instala la dependencia **libserialport** ([instrucciones](https://sigrok.org/wiki/Libserialport)) `git clone git://sigrok.org/libserialport`

### Controladores (Dependiendo del adaptador/puerto serial que tengas)

- USB Serial **Chipset CH340** para Windows: <http://www.wch.cn/download/CH341SER_EXE.html>
- USB Serial **Chipset CH340** para macOS: <https://blog.sengotta.net/signed-mac-os-driver-for-winchiphead-ch340-serial-bridge/>
- USB Serial **Chipset Prolific** para macOS: <http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=229&pcid=41>

### Windows 10

- Visual Studio 2017.

- [msys2 - mingw-w64](http://www.msys2.org/) sigue las instrucciones en la web:
  - Instala mingw
    - 32bits: ```pacman -S mingw-w64-i686-toolchain```
    - 64bits: ```pacman -S mingw-w64-x86_64-toolchain```
  - Install mingw cmocka
    - 32bits: ```pacman -S mingw32/mingw-w64-i686-cmocka```
    - 64bits: ```pacman -S mingw64/mingw-w64-x86_64-cmocka```
- Swig (puedes usar [Chocolatey](https://chocolatey.org/))

### macOS

Estas instrucciones asumen que tienes instalado [homebrew](https://brew.sh/).

- `brew install automake`
- `brew install autoconf`
- `brew install libtool`
- `brew install swig`
- `brew install cmocka`

### Unix

- build-esentials
- automake
- autoconf
- libtool
- swig
- cmocka

### Construir el Proyecto

#### Build

```bash
make build example=main
```

### Ejecutar ejemplos

```bash
make run example=main
```

### Debug

```bash
make debug example=main
```

### Instalación

```bash
cp build/transbank.dll /ruta/en/tu/path
```

### Test

Estamos usando [cmocka](https://cmocka.org) para ejecutar los test unitarios.
Una vez tengas **cmocka** instalado en tu maquina, debes ejecutar:

```bash
make cmocka-test
```

### Generar una nueva versión

Para generar una nueva versión se debe crear un nuevo pull request que contenga:

1. Incrementar el número de versión en el archivo `version.rc` siguiendo la guía de SemVer.
2. Actualizar `Changelog.md` con los nuevos cambios/modificaciones incluidas en esta nueva versión.
3. Generar la DLL y el Wrapper (`make windows-wrapper`).

Luego de mezclar el Pull Request:

1. Crear inmediatamente un release en GitHub.
2. Incluir en el Release de GitHub la DLL creada.
3. Incluir también `libserialport-0.dll`.
