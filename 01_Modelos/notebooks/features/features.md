## Propósito
Aqui están los scripts relacionados a los features para nuestro modelo , eventos criticos , duración de los eventos , etc

## Contenido

- [`categorize_critical_event.ipynb`](/notebooks/features/categorize_critical_event.ipynb)
- [`categorize_critical_event_by_data_extraction.ipynb`](/notebooks/features/categorize_critical_event_by_data_extraction.ipynb)
- [`data_extraction.ipynb`](/notebooks/features/data_extraction.ipynb)
- [`duration_event.ipynb`](/notebooks/features/duration_event.ipynb)
- [`level_duration_event_plot.ipynb`](/notebooks/features/level_duration_event_plot.ipynb)
-  [event_duration_counts.csv](/notebooks/features/event_duration_counts.csv)
-  [global_duration_counts.csv](/notebooks/features/global_duration_counts.csv)
## Archivos relevantes

### Datos
-  [event_duration_counts.csv](/notebooks/features/event_duration_counts.csv)
-  [global_duration_counts.csv](/notebooks/features/global_duration_counts.csv)

### Notebooksw
- [`categorize_critical_event.ipynb`](/notebooks/features/categorize_critical_event.ipynb)
- [`categorize_critical_event_by_data_extraction.ipynb`](/notebooks/features/categorize_critical_event_by_data_extraction.ipynb)
- [`data_extraction.ipynb`](/notebooks/features/data_extraction.ipynb)
- [`duration_event.ipynb`](/notebooks/features/duration_event.ipynb)
- [`level_duration_event_plot.ipynb`](/notebooks/features/level_duration_event_plot.ipynb)
## Flujo de uso
- [`categorize_critical_event.ipynb`](/notebooks/features/categorize_critical_event.ipynb):
	- Se cargan los datos [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv) y [peak_nivel_detection.csv](/data/external/peak_nivel_detection.csv)
	- Se validan ventanas temporales previas al evento.
	- Se extraen valores y acumulados de SIL como **features**.
	- Se etiqueta si el pico es **crítico** según un umbral.
	- Se genera un CSV final listo para **clustering**. `data_for_clustering_2.csv.csv`
- [`categorize_critical_event_by_data_extraction.ipynb`](/notebooks/features/categorize_critical_event_by_data_extraction.ipynb)
	- Se carga  `data_extraction.csv` y  **se  transforma una señal continua (`global_peak`) en una variable de evento (verdadero/falso)** mediante un criterio de umbral, preparando los datos para análisis de eventos, clasificación o detección de ocurrencias.
- [`data_extraction.ipynb`](/notebooks/features/data_extraction.ipynb)
	- Se cargan[sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv)  y [peak_nivel_detection.csv](/data/external/peak_nivel_detection.csv)
	- Se **normalizan las columnas temporales** de ambos conjuntos y se establece la fecha como **índice temporal** en el dataset combinado, dejando los datos listos para operaciones basadas en tiempo.
	- Para **cada pico detectado**, se valida que existan datos SIL disponibles en una **ventana temporal previa** (hasta 120 minutos antes, en pasos de 10 minutos).
	- Para los eventos válidos, se **extraen características temporales**:
		- El valor del pico global (`global_peak`).
		- Valores de SIL en distintos **retardos temporales** respecto al pico.
		- **Características acumuladas** de SIL calculadas sobre ventanas de tiempo definidas.
	- Todas las características se agrupan en una **tabla de variables** donde cada fila representa un evento y sus atributos asociados.
	- El resultado se guarda como un **nuevo dataset estructurado** (`data_extraction.csv`), listo para análisis posteriores, modelado o detección de eventos.
	- En síntesis, el código **convierte eventos detectados en un conjunto de características temporales alineadas**, preparando los datos para análisis avanzado o aprendizaje automático.
- [`duration_event.ipynb`](/notebooks/features/duration_event.ipynb)
	- Se cargan [peak_detection_info.csv](/data/external/peak_detection_info.csv) y`data_for_clustering_1.csv`
	- Se **limpian y normalizan** dos datasets (duración de eventos y picos).
	- Se **unen por coincidencia de pico global** (merge interno tras redondeo).
	- Se genera un **dataset final** evento–pico y se guarda en CSV `duration_event_and_peak.csv`.
	- Se **analizan distribuciones** de duraciones con histogramas.
	- Se **cuentan duraciones** (evento y global) y se **asocian fechas**, exportando los conteos a CSV `peaks_counts_dates.csv.csv` 
- [`level_duration_event_plot.ipynb`](/notebooks/features/level_duration_event_plot.ipynb)

## Entradas
- [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv)  y [peak_detection_info.csv](/data/external/peak_detection_info.csv)
## Salidas
- `data_for_clustering_2.csv.csv`
- `data_extraction.csv`
-    [peak_detection_info.csv](/data/external/peak_detection_info.csv) 
- `duration_event_and_peak.csv`
- `peaks_counts_dates.csv.csv` 


## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
