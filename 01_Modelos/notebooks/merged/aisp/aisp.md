## Propósito
Se realizan la correlación de los datos para la estación aisp.

## Contenido


- [aisp_nivel.ipynb](/notebooks/merged/aisp/aisp_nivel.ipynb)
- [aisp_nivel_merged_cross_correlation.ipynb](/notebooks/merged/aisp/aisp_nivel_merged_cross_correlation.ipynb)
- [aisp_nivel_merged_plot.ipynb](/notebooks/merged/aisp/aisp_nivel_merged_plot.ipynb)

## Archivos relevantes

### Datos
- sin datos
### Notebooks
- [aisp_nivel.ipynb](/notebooks/merged/aisp/aisp_nivel.ipynb)
	- Se cargan [nivel_processed.csv](/data/processed/nivel_processed.csv) y [aisp_processed.csv](/data/processed/aisp_processed.csv) 
	- Fusiona (merge) las copias usando el índice temporal como clave
	- Usa `inner join` para mantener solo las fechas que existen en ambos conjuntos
	- Guarda el DataFrame fusionado como [aisp_nivel_merged.csv](/data/external/aisp_nivel_merged.csv) en `external`
- [aisp_nivel_merged_cross_correlation.ipynb](/notebooks/merged/aisp/aisp_nivel_merged_cross_correlation.ipynb)
	- Se lee [aisp_nivel_merged.csv](/data/external/aisp_nivel_merged.csv)
	-  Calcula la correlación cruzada entre nivel y AISP estandarizados
	- Genera desfases (lags) y normaliza la correlación
	- Convierte los lags a minutos (multiplicando por 10)
	- Determinar cuántos minutos de retraso existe entre los cambios en AISP (precipitación) y el nivel del río.
- [aisp_nivel_merged_plot.ipynb](/notebooks/merged/aisp/aisp_nivel_merged_plot.ipynb)

## Flujo de uso
Cómo se usan estos archivos en conjunto.
[aisp_nivel.ipynb](/notebooks/merged/aisp/aisp_nivel.ipynb)-------------- [aisp_nivel_merged_cross_correlation.ipynb](/notebooks/merged/aisp/aisp_nivel_merged_cross_correlation.ipynb) --------------  [aisp_nivel_merged_plot.ipynb](/notebooks/merged/aisp/aisp_nivel_merged_plot.ipynb)

## Entradas
- [nivel_processed.csv](/data/processed/nivel_processed.csv) y [aisp_processed.csv](/data/processed/aisp_processed.csv) 
## Salidas
-  [aisp_nivel_merged.csv](/data/external/aisp_nivel_merged.csv)

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
