# Changelog

Todos los cambios notables a este proyecto serán documentados en este archivo.

El formato está basado en [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
y este proyecto adhiere a [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [3.0.0] - 2019-10-29

### Changed

- `Sale`
    - `Ticket` data type from `int` to `string`. (It has to be 6 characters)

- Fix a bug when no option was selected in the sale or it was canceled from the pos.

Also, improves the way wrappepr is generated in makefile.

## [2.0.0] - 2019-06-26

### Added

- `Sales Detail` method.

### Changed

- `LoadKeys`
  - `Terminal ID` data type from `int` to `string`.
- `Refund`
  - `Terminal ID` data type from `int` to `string`.
  - `Authorization Code` data type from `int` to `string`.

## [1.4.0] - 2019-06-18

### Added

- Función para Anulación.
- Función para Última Venta.
- Función para Totales.

## [1.0.0] - 2019-05-13

### Added

- Función para listar puertos seriales.
- Función para POLL de POS.
- Función para Cambio a POS Normal.
- Función para Carga de Llaves.
- Función para Cierre.
- Función para Venta sin mensajes intermedios.
