
`[[peaks]]`
## Propósito
Contiene 2 scripts , aquí se encuentran los scripts que grafican y procesan los datos de nivel 


## Contenido

- [nivel_preprocessing.ipynb](/notebooks/nivel/nivel_preprocessing.ipynb)
- [nivel_processed_plot.ipynb](/notebooks/nivel/nivel_processed_plot.ipynb)
## Archivos relevantes

### Datos
- sin datos

### Notebooks
- [nivel_preprocessing.ipynb](/notebooks/nivel/nivel_preprocessing.ipynb)
	- Se carga [nivel.csv](/data/interim/nivel.csv) 
	- salida [nivel_processed.csv](/data/processed/nivel_processed.csv)
	- salida [peak_detection_info.csv](/data/external/peak_detection_info.csv) 
- [nivel_processed_plot.ipynb](/notebooks/nivel/nivel_processed_plot.ipynb):
	- Se cargan  [nivel_processed.csv](/data/processed/nivel_processed.csv) y [peak_detection_info.csv](/data/external/peak_detection_info.csv) se grafica

## Entradas
- [nivel.csv](/data/interim/nivel.csv) 

## Salidas
- [nivel_processed.csv](/data/processed/nivel_processed.csv)
- [peak_detection_info.csv](/data/external/peak_detection_info.csv) 


