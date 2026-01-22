## Propósito
Breve descripción de qué contiene esta carpeta y qué rol cumple
dentro del proyecto.

## Contenido


- [grid_search.ipynb](/scr/utils/grid_search.ipynb)
- [map_heat.ipynb](/scr/utils/map_heat.ipynb)
- [Untitled.ipynb](/scr/utils/Untitled.ipynb)


### Notebooks

- [grid_search.ipynb](/scr/utils/grid_search.ipynb):
	- - Define un conjunto de **hiperparámetros candidatos** para uno o más modelos (p. ej. LR, SVM).
	- Entrena múltiples configuraciones del modelo usando **validación cruzada**.
	- Evalúa cada configuración con métricas de desempeño.
	- **Selecciona la combinación óptima de hiperparámetros** según el criterio de evaluación.
	- Guarda o reporta los mejores parámetros encontrados.
	Mejorar el rendimiento y la estabilidad de los modelos mediante una **optimización sistemática de hiperparámetros**.
- [map_heat.ipynb](/scr/utils/map_heat.ipynb):
	-  Carga los datos procesados relevantes.
	- Calcula métricas de relación (por ejemplo, correlaciones entre variables o retardos temporales).
	- Construye un **heatmap** que permite identificar visualmente:
	    - variables más relacionadas,
	    - intensidades de correlación,
	    - posibles patrones temporales o espaciales.
	- Exporta o muestra la visualización para análisis exploratorio.
	Facilitar la **interpretación visual** de dependencias entre variables y apoyar la selección de características y el análisis exploratorio previo al modelado.
- [Untitled.ipynb](/scr/utils/Untitled.ipynb):
	-  Carga uno o más datasets del pipeline.
    
	- Realiza inspecciones preliminares, cálculos auxiliares y/o visualizaciones rápidas.
    
	- Permite validar hipótesis, verificar transformaciones o explorar relaciones antes de integrarlas en notebooks formales.
	


