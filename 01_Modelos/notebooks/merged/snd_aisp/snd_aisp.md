## Propósito
Ésta carpeta contiene los notebooks asociados al análisis y procesamiento
de los datos SND(Secretaría Nacional de deportes), este conjunto de datos si se realizaron los cambios de horario utc a horario local para snd y para aisp.


## Contenido
- [snd_aisp.ipynb](/notebooks/merged/snd_aisp/snd_aisp.ipynb)
- [(local)aisp_snd_merged_plot.ipynb](/notebooks/merged/snd_aisp/(local)aisp_snd_merged_plot.ipynb)
- [aisp_snd_merged_plot.ipynb](/notebooks/merged/snd_aisp/aisp_snd_merged_plot.ipynb)

## Archivos relevantes

### Datos
- sin datos

### Notebooks
- [snd_aisp.ipynb](/notebooks/merged/snd_aisp/snd_aisp.ipynb)
- [(local)aisp_snd_merged_plot.ipynb](/notebooks/merged/snd_aisp/(local)aisp_snd_merged_plot.ipynb)
- [aisp_snd_merged_plot.ipynb](/notebooks/merged/snd_aisp/aisp_snd_merged_plot.ipynb)


## Flujo de uso
- [snd_aisp.ipynb](/notebooks/merged/snd_aisp/snd_aisp.ipynb):
	- Se cargan los conjuntos de datos [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv) , [snd_local_processed.csv](/data/processed/snd_local_processed.csv), [aisp_processed.csv](/data/processed/aisp_processed.csv) , [snd_processed.csv](/data/processed/snd_processed.csv). Se tiene como salidas 
	  [snd_aisp_merged.csv](/data/external/snd_aisp_merged.csv), [snd_aisp_local_merged.csv](/data/external/snd_aisp_local_merged.csv)
- [(local)aisp_snd_merged_plot.ipynb](/notebooks/merged/snd_aisp/(local)aisp_snd_merged_plot.ipynb):
	- grafico con horario local
- [aisp_snd_merged_plot.ipynb](/notebooks/merged/snd_aisp/aisp_snd_merged_plot.ipynb):
	- grafico sin horario local


## Entradas
- [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv) , [snd_local_processed.csv](/data/processed/snd_local_processed.csv), [aisp_processed.csv](/data/processed/aisp_processed.csv) , [snd_processed.csv](/data/processed/snd_processed.csv)

## Salidas
-  [snd_aisp_merged.csv](/data/external/snd_aisp_merged.csv), [snd_aisp_local_merged.csv](/data/external/snd_aisp_local_merged.csv)

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
