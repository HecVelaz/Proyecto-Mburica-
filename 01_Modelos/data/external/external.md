# Carpeta `external`

## Propósito
Contiene los datos procesados generados a partir de los archivos en *==**`processed`***==.

## Proceso general
- Se realizaron combinaciones a los conjuntos de datos de==**`processed`***== 
## El procesamiento incluyó:
- utilizando la función de python Merged , how= inner , se realizaron intersecciones con los conjuntos de datos de processed y esto dio los archivos generados a continuación
- Además un conjunto de datos de la detección de picos en formato csv

## Archivos generados

- ==`aisp_local_nivel_merged.csv`==  
- `aisp_nivel_merged.csv`  
- `peak_detection_info.csv`  
- `peak_nivel_detection.csv`  
- `sil_nivel_merged.csv`  
- ==`sil_snd_aisp_local_nivel_merged.csv`==  
- `sil_snd_aisp_nivel_merged.csv`  
- `snd_aisp_merged.csv`  
- ==`snd_aisp_local_merged.csv`==  
- ==`snd_local_nivel_merged.csv`== 
## Scripts relacionado
-  `aisp_nivel.ipynb`.
-  ==`aisp_local_nivel.ipynb`.==
-  ==`(local)sil_snd_aisp_nivel.ipynb`.==
- `sil_snd_aisp_nivel.ipynb`.
-  `sil_nivel.ipynb`.
-  `snd_nivel.ipynb`.
- `snd_aisp.ipynb`.
- ==`snd_local_nivel.ipynb`==


## Observaciones
- Las combinaciones que tienen "local" son los conjuntos de datos que pasaron de horario utc a local , éstos son los datos que se utilizaron realmente para el modelo.