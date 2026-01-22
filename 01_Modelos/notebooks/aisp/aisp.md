
## Propósito
Esta carpeta contiene los notebooks asociados al análisis y procesamiento
de los datos **==AISP==** (Aeropuerto Internacional Silvio Petirossi)

## Contenido
Descripción general de los tipos de archivos que se encuentran aquí.

- [aisp_preprocessing.ipynb](/notebooks/aisp/aisp_preprocessing.ipynb)
- [aisp_processed_plot.ipynb](/notebooks/aisp/aisp_processed_plot.ipynb)
- [aisp_utc_to_local.ipynb](/notebooks/aisp/aisp_utc_to_local.ipynb)

## Archivos relevantes

### Datos 
Datos que se utilizaron en los scripts
- [aisp.csv](/data/interim/aisp.csv)
-  [aisp_processed](/data/processed/aisp_processed.csv)
- [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv)

### Notebooks
 - [aisp_preprocessing.ipynb](/notebooks/aisp/aisp_preprocessing.ipynb) — Se carga el conjunto de datos aisp de ==`interim`== - [aisp.csv](/data/interim/aisp.csv) , se **normaliza la dimensión temporal** del dataset AISP y lo deja listo para análisis temporal.  Resultado [aisp_processed](/data/processed/aisp_processed.csv)
-  [aisp_processed_plot.ipynb](/notebooks/aisp/aisp_processed_plot.ipynb) —  Gráfico usando matplotlib 
-  [aisp_utc_to_local.ipynb](/notebooks/aisp/aisp_utc_to_local.ipynb) —   Pasar utc a horario local America/Asuncion , salida [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv)


## Flujo de uso
Cómo se usan estos archivos en conjunto.

1. Se cargan los datos desde `interim` [aisp.csv](/data/interim/aisp.csv)
2. realizamos normalización temporal se tiene   [aisp_processed](/data/processed/aisp_processed.csv)
3. Se realiza el cambio de hora , la salida final es [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv)

## Entradas
-  [aisp.csv](/data/interim/aisp.csv)

## Salidas
- [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv)

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
