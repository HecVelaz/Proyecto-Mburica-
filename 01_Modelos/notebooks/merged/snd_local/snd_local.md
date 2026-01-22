## Propósito
Se realizan el cross correlation y el mutual information con sus gráficos correspondientes , usando conjunto de datos snd y nivel en horario local 

## Contenido

- [snd_local_merged_cross_correlation.ipynb](/notebooks/merged/snd_local/snd_local_merged_cross_correlation.ipynb)
- [snd_local_merged_plot.ipynb](/notebooks/merged/snd_local/snd_local_merged_plot.ipynb)
- [snd_local_nivel.ipynb](/notebooks/merged/snd_local/snd_local_nivel.ipynb)
- [snd_local_nivel_merged_mutal_information.ipynb](/notebooks/merged/snd_local/snd_local_nivel_merged_mutal_information.ipynb)
## Archivos relevantes

### Datos
- sin datos
## Flujo de uso
Cómo se usan estos archivos en conjunto.

- [snd_local_merged_cross_correlation.ipynb](/notebooks/merged/snd_local/snd_local_merged_cross_correlation.ipynb)
	- Se calcula el cross correlation con snd en horario local
- [snd_local_merged_plot.ipynb](/notebooks/merged/snd_local/snd_local_merged_plot.ipynb)
	- gráfico del cross correlation
- [snd_local_nivel.ipynb](/notebooks/merged/snd_local/snd_local_nivel.ipynb)
	- Se cargan [nivel_processed.csv](/data/processed/nivel_processed.csv) y [snd_local_processed.csv](/data/processed/snd_local_processed.csv)
	- Salida [snd_local_nivel_merged.csv](/data/external/snd_local_nivel_merged.csv)
- [snd_local_nivel_merged_mutal_information.ipynb](/notebooks/merged/snd_local/snd_local_nivel_merged_mutal_information.ipynb):
	- Se calcula el mutual information del conjunto snd con nivel 
## Entradas
- [nivel_processed.csv](/data/processed/nivel_processed.csv) y [snd_local_processed.csv](/data/processed/snd_local_processed.csv)

## Salidas
- [snd_local_nivel_merged.csv](/data/external/snd_local_nivel_merged.csv)

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
