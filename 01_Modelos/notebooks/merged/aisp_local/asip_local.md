## Propósito
La diferencia con la carpeta de arriba `aisp` es que se trabaja con el conjunto de datos [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv) , donde éste tiene la fecha utc a horario local . Además se realiza el mutual information. 
## Contenido

- [aisp_local_nivel.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel.ipynb)
- [aisp_local_nivel_merged_cross_correlation.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_cross_correlation.ipynb)
- [aisp_local_nivel_merged_mutal_information.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_mutal_information.ipynb)
- [aisp_local_nivel_merged_plot.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_plot.ipynb)

## Archivos relevantes

### Datos
- sin datos

### Notebooks
- [aisp_local_nivel.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel.ipynb):
	- Se cargan [nivel_processed.csv](/data/processed/nivel_processed.csv) y [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv)
	- Fusiona (merge) las copias usando el índice temporal como clave
	- Usa `inner join` para mantener solo las fechas que existen en ambos conjuntos
	- Guarda el DataFrame fusionado como  [aisp_local_nivel_merged.csv](/data/external/aisp_local_nivel_merged.csv) `external`
- [aisp_local_nivel_merged_cross_correlation.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_cross_correlation.ipynb):
	- Se lee  [aisp_local_nivel_merged.csv](/data/external/aisp_local_nivel_merged.csv)
	-  Calcula la correlación cruzada entre nivel y AISP estandarizados
	- Genera desfases (lags) y normaliza la correlación
	- Convierte los lags a minutos (multiplicando por 10)
	- Determinar cuántos minutos de retraso existe entre los cambios en AISP (precipitación) y el nivel del río.
- [aisp_local_nivel_merged_mutal_information.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_mutal_information.ipynb):
	-  Evalúa información mutua (MI) para delays de 0 a 80 min (pasos de 10)
	- Desplaza AISP temporalmente y calcula MI con nivel
	- Identifica el delay con máxima MI
	- Visualiza con gráfico Plotly simple
	- Repite el mismo proceso pero con datos sin estandarizar
	- Usa el mismo rango de delays (0-80 min)
	- Calcula información mutua para cada desplazamiento
	- Gráfico más grande (1200x1200 px)
	- Fuentes en negrita y tamaño 20-24
	- Marca el punto de máxima MI en rojo
	- Líneas más gruesas y marcadores más visibles
	-  Crea DataFrame con delays y valores MI
	- Renombra delays como "sil0", "sil10", etc.
	- Muestra tabla con información mutua por delay
	- Encontrar el desfase temporal óptimo entre AISP y nivel del río usando información mutua (captura relaciones no lineales).
- [aisp_local_nivel_merged_plot.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_plot.ipynb):

## Flujo de uso
Cómo se usan estos archivos en conjunto.
[aisp_local_nivel.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel.ipynb) ---[aisp_local_nivel_merged_cross_correlation.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_cross_correlation.ipynb) ---- [aisp_local_nivel_merged_mutal_information.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_mutal_information.ipynb) ---- [aisp_local_nivel_merged_plot.ipynb](/notebooks/merged/aisp_local/aisp_local_nivel_merged_plot.ipynb):

## Entradas
- [nivel_processed.csv](/data/processed/nivel_processed.csv) y [aisp_local_processed.csv](/data/processed/aisp_local_processed.csv)

## Salidas
-  [aisp_local_nivel_merged.csv](/data/external/aisp_local_nivel_merged.csv)
## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
