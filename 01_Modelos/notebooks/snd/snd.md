## Propósito
Contiene 3 scripts , aquí se encuentran los scripts que grafican y procesan los datos de nivel 


## Contenido

- [snd_preprocessing.ipynb](/notebooks/snd/snd_preprocessing.ipynb)
- [snd_processed_plot.ipynb](/notebooks/snd/snd_processed_plot.ipynb)
- [snd_utc_to_local.ipynb](/notebooks/snd/snd_utc_to_local.ipynb)
## Archivos relevantes

### Datos
- sin datos

### Notebooks
-  [snd_preprocessing.ipynb](/notebooks/snd/snd_preprocessing.ipynb)
	- Se carga [snd.csv](/data/interim/snd.csv) 
	- salida [snd_processed.csv](/data/processed/snd_processed.csv)
- [snd_processed_plot.ipynb](/notebooks/snd/snd_processed_plot.ipynb):
	- Se cargan [snd_processed.csv](/data/processed/snd_processed.csv) y  se grafica
- [snd_utc_to_local.ipynb](/notebooks/snd/snd_utc_to_local.ipynb):
	- Pasamos de utc a horario local 
	- salida [snd_local_processed.csv](/data/processed/snd_local_processed.csv)
 

## Entradas
- [snd.csv](/data/interim/snd.csv) 
## Salidas
- [snd_processed.csv](/data/processed/snd_processed.csv)
- [snd_local_processed.csv](/data/processed/snd_local_processed.csv)