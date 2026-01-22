# Carpeta `data`

## Propósito
En ésta carpeta se encuentran  4 carpetas , raw , interim, processed y external
Esta carpeta contiene **los datos base del proyecto**.
Todo el procesamiento almacenados aquí.
## Contenido
Descripción general de los tipos de archivos que se encuentran aquí.

- [[raw]]
-  [[interim]]
- [[processed]]
- [[external]]
## Archivos relevantes

### Datos
- `raw.csv`             — Datos sin procesar , los datos almacenados aquí están sin nigún cambio
- `interim.csv`      — El siguiente paso , los datos raw se analizan y se guardan en external , aquí se cambian los nombres de las columnas 
- `processed.csv`  —siguiente paso los datos de la carpeta ==`interim`== fueron pre procesados con el objetivo de unificar la resolución temporal y estabilizar la precisión numérica.
- `external.csv`    — Último paso ,  se realizaron combinaciones a los conjuntos de datos de==**`processed`***== 
### Notebooks
-  Sin scripts en ésta carpeta

## Flujo de uso
Cómo se usan estos archivos en conjunto.
flujo del trabajo
`raw` ----- `interim` ----- `processed` ----- `external`
## Entradas
- `raw`
## Salidas
- `external`
## Observaciones
- Supuestos
- Limitaciones
- Notas importantes