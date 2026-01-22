## Propósito
Ésta carpeta contiene los notebooks asociados al análisis y procesamiento
de los datos SND(Secretaría Nacional de deportes), este conjunto de datos no se pasó a horario local. Solo se usaron para comparar.


## Contenido


- [snd_nivel.ipynb](/notebooks/merged/snd/snd_nivel.ipynb)
- [snd_nivel_merged_cross_correlation.ipynb](/notebooks/merged/snd/snd_nivel.ipynb)
- [snd_nivel_merged_cross_correlation.ipynb](/notebooks/merged/snd/snd_nivel_merged_cross_correlation.ipynb)
- [snd_nivel_merged_plot.ipynb](/notebooks/merged/snd/snd_nivel_merged_plot.ipynb)

## Archivos relevantes

### Datos
- sin datos

### Notebooks
- [snd_nivel.ipynb](/notebooks/merged/snd/snd_nivel.ipynb)
- [snd_nivel_merged_cross_correlation.ipynb](/notebooks/merged/snd/snd_nivel.ipynb)
- [snd_nivel_merged_cross_correlation.ipynb](/notebooks/merged/snd/snd_nivel_merged_cross_correlation.ipynb)
- [snd_nivel_merged_plot.ipynb](/notebooks/merged/snd/snd_nivel_merged_plot.ipynb)


## Flujo de uso
- [snd_nivel.ipynb](/notebooks/merged/snd/snd_nivel.ipynb):
	- Se cargan [nivel_processed.csv](/data/processed/nivel_processed.csv) y [snd_processed.csv](/data/processed/snd_processed.csv)
	- Se crea el nuevo conjunto de datos `snd_nivel_merged.csv`
- [snd_nivel_merged_cross_correlation.ipynb](/notebooks/merged/snd/snd_nivel.ipynb):
	- Se realiza la correlación cruzada 
- [snd_nivel_merged_cross_correlation.ipynb](/notebooks/merged/snd/snd_nivel_merged_cross_correlation.ipynb):
	- Se realiza el mutual information
- [snd_nivel_merged_plot.ipynb](/notebooks/merged/snd/snd_nivel_merged_plot.ipynb):
	- Se realiza el ploteo


## Entradas
- [nivel_processed.csv](/data/processed/nivel_processed.csv) y [snd_processed.csv](/data/processed/snd_processed.csv)

## Salidas
- `snd_nivel_merged.csv`

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
