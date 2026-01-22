# Carpeta `processed`

## Propósito
Contiene los datos procesados generados a partir de los archivos en ==**`interim`***==

## Proceso general
- Los datos de originales  de ==**`interim`***== fueron pre procesados con el objetivo de
unificar la resolución temporal y estabilizar la precisión numérica.
## El procesamiento incluyó:
- Conversión y validación de la columna temporal.
- Redondeo de las marcas de tiempo a resolución de minutos.
- Reducción de la precisión del valor de nivel para minimizar ruido numérico.
- Cambio de zona horaria para snd y aisp  utc a local.


## Archivos generados

- `nivel_processed.csv`  
- ==`aisp_processed.csv`==  
- ==`aisp_local_processed.csv`==  
- `sil_processed.csv`  
- ==`snd_processed.csv`==  
- ==`snd_local_processed.csv`==  
## Scripts relacionado
-  `nivel_preprocessing.ipynb`.
-  `aisp_preprocessing.ipynb`.
-  `sil_preprocessing.ipynb`.
-  `snd_preprocessing.ipynb`.
-  `snd_utc_to_local.ipynb`
-  `aisp_utc_to_local.ipynb`

## Observaciones
- ==`aisp_local_processed.csv`==  y ==`snd_local_processed.csv`==  se obtuvieron como `aisp_preprocessing.ipynb`. y  `snd_preprocessing.ipynb` pero con la diferencia que se cambio la zona horaria .
  