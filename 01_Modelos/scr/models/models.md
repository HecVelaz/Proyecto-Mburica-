## Propósito
Scripts de los modelos aplicados

## Contenido


- [LR_model.ipynb](/scr/models/LR_model.ipynb)
- [LR_model_features_windows.ipynb](/scr/models/LR_model_features_windows.ipynb)
- [SVM_model.ipynb](/scr/models/SVM_model.ipynb)
- [SVM_model_features_windows.ipynb](/scr/models/SVM_model_features_windows.ipynb)
- [SVM_model_true_event_plot.ipynb](/scr/models/SVM_model_true_event_plot.ipynb)
- [Multilinear_Regression_Analysis.png](/scr/models/Multilinear_Regression_Analysis.png)
- [Raw_Correlations_Rainfall_Peak.png](/scr/models/Raw_Correlations_Rainfall_Peak.png)
- [Raw_Correlations_Rainfall_Peak_v2.png](/scr/models/Raw_Correlations_Rainfall_Peak_v2.png)
- [Regresion.png](/scr/models/Regresion.png)
- [Regresion_v2.png](/scr/models/Regresion_v2.png)

## Archivos relevantes

### Datos
- [Multilinear_Regression_Analysis.png](/scr/models/Multilinear_Regression_Analysis.png)
- [Raw_Correlations_Rainfall_Peak.png](/scr/models/Raw_Correlations_Rainfall_Peak.png)
- [Raw_Correlations_Rainfall_Peak_v2.png](/scr/models/Raw_Correlations_Rainfall_Peak_v2.png)
- [Regresion.png](/scr/models/Regresion.png)
- [Regresion_v2.png](/scr/models/Regresion_v2.png)

### Notebooks
- [LR_model.ipynb](/scr/models/LR_model.ipynb)
- [LR_model_features_windows.ipynb](/scr/models/LR_model_features_windows.ipynb)
- [SVM_model.ipynb](/scr/models/SVM_model.ipynb)
- [SVM_model_features_windows.ipynb](/scr/models/SVM_model_features_windows.ipynb)
- [SVM_model_true_event_plot.ipynb](/scr/models/SVM_model_true_event_plot.ipynb)

## Flujo de uso
- [LR_model.ipynb](/scr/models/LR_model.ipynb):
	- ### Modelo de Regresión Logística (`LR_model.ipynb`)

	Este notebook entrena y evalúa un **modelo de regresión logística** para clasificar eventos binarios
	(señales/eventos críticos). El modelo:

	- Carga los datos de entrenamiento con las características extraídas.
	- Separa las variables predictoras de la variable objetivo.
	- Ajusta un modelo de regresión logística para maximizar la probabilidad de clasificación correcta.
	- Evalúa el rendimiento con métricas estándar y gráficos.
	- Permite predecir la clase de nuevos eventos basados en sus características.

	La regresión logística es un modelo de clasificación que estima la probabilidad de pertenecer a una categoría usando una función sigmoidea entre 0 y 1 (clasificación binaria).
- [LR_model_features_windows.ipynb](/scr/models/LR_model_features_windows.ipynb):
	- Este notebook entrena y evalúa un modelo de regresión logística utilizando características creadas en ventanas de tiempo
	basadas en retardos y acumulados de señales (por ejemplo, SIL). El objetivo es analizar cómo distintas ventanas de características temporales afectan el rendimiento del modelo para clasificar eventos.
    - Se compara el rendimiento del modelo con distintas configuraciones de entradas, y se reportan métricas relevantes de clasificación para identificar qué configuraciones de “feature windows” son más informativas.
- [SVM_model.ipynb](/scr/models/SVM_model.ipynb):
	- - Carga el dataset de características y la variable objetivo (evento / no evento).
	- Prepara los datos para clasificación (selección de features, separación X–y).
	- Entrena un **clasificador SVM** para aprender la frontera de decisión entre clases.
	- Evalúa el rendimiento del modelo mediante métricas estándar de clasificación.
	- Analiza la capacidad del SVM para distinguir eventos críticos frente a no críticos.
	Compara el desempeño del **SVM** frente a otros modelos (p. ej. regresión logística) en la detección de eventos, especialmente en espacios de características potencialmente no lineales.
- [SVM_model_features_windows.ipynb](/scr/models/SVM_model_features_windows.ipynb):
	-  Carga datos con _features_ construidas en distintas ventanas temporales previas al evento.
	- Define variables predictoras y la etiqueta de evento.    
	- Entrena un **clasificador SVM** con estas características temporales
	- Evalúa el rendimiento del modelo y **analiza el impacto de las ventanas de tiempo** en la capacidad de clasificación.
	Evalua si la incorporación de **ventanas temporales de características** mejora la detección de eventos frente a un SVM con variables estáticas.
- [SVM_model_true_event_plot.ipynb](/scr/models/SVM_model_true_event_plot.ipynb):
	- gráfico

