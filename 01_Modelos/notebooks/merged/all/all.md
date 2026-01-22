## Propósito
Se crea el conjunto de datos [sil_snd_aisp_local_nivel_merged.csv](/data/external/sil_snd_aisp_local_nivel_merged.csv) y se realizan el cross correlation y mutual information.

## Contenido
- [(Local)sil_snd_aisp_nivel.ipynb](/notebooks/merged/all/(Local)sil_snd_aisp_nivel.ipynb)
- [(Local)sil_snd_aisp_nivel_merged_plot.ipynb](/notebooks/merged/all/(Local)sil_snd_aisp_nivel_merged_plot.ipynb)
- [Cross_correlation.ipynb](/notebooks/merged/all/Cross_correlation.ipynb)
- [Mutual_information.ipynb](/notebooks/merged/all/Mutual_information.ipynb)
- [sil_snd_aisp_nivel.ipynb](/notebooks/merged/all/sil_snd_aisp_nivel.ipynb)
- [sil_snd_aisp_nivel_merged_plot.ipynb](/notebooks/merged/all/sil_snd_aisp_nivel_merged_plot.ipynb)
## Archivos relevantes

### Datos
- sin datos

### Notebooks
- [(Local)sil_snd_aisp_nivel.ipynb](/notebooks/merged/all/(Local)sil_snd_aisp_nivel.ipynb):
	- Se cargan [nivel_processed.csv](/data/processed/nivel_processed.csv) ,[sil_processed.csv](/data/processed/sil_processed.csv) , [snd_local_processed.csv](/data/processed/snd_local_processed.csv), [aisp_local_processed.csv](data/processed/aisp_local_processed.csv)
	-  DataFrame con 4 variables combinadas: nivel, sil, snd y aisp
	-  Solo mantiene fechas que existen en los 4 conjuntos de datos (inner join)
	- resultado [sil_snd_aisp_local_nivel_merged.csv](/data/external/sil_snd_aisp_local_nivel_merged.csv)
- [(Local)sil_snd_aisp_nivel_merged_plot.ipynb](/notebooks/merged/all/(Local)sil_snd_aisp_nivel_merged_plot.ipynb):
	- se realizan gráficos , introduciendo fecha y hora.
- [Cross_correlation.ipynb](/notebooks/merged/all/Cross_correlation.ipynb):
	- Lee 3 archivos CSV fusionados:  [aisp_local_nivel_merged.csv](/data/external/aisp_local_nivel_merged.csv) , [snd_local_nivel_merged.csv](/data/external/snd_local_nivel_merged.csv) , [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv) 
	- Define función `cross_correlation()` que:
		- Calcula correlación cruzada normalizada
		- Filtra delays entre 0-80 minutos (pasos de 10)
		- Identifica el lag de máxima correlación absoluta
	- Aplica correlación cruzada a 3 pares:
		- AISP vs Nivel
		- SND vs Nivel
		- SIL vs Nivel
	- Compara simultáneamente la respuesta temporal del nivel del río a precipitaciones de 3 estaciones diferentes.
- [Mutual_information.ipynb](/notebooks/merged/all/Mutual_information.ipynb):
	-  Lee 3 archivos CSV fusionados:  [aisp_local_nivel_merged.csv](/data/external/aisp_local_nivel_merged.csv) , [snd_local_nivel_merged.csv](/data/external/snd_local_nivel_merged.csv) , [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv) 
	- Define delays de 0 a 80 minutos en pasos de 10
	- Convierte delays a pasos temporales (÷10)
	- Cálculo de Información Mutua (para cada estación):
		- **AISP:** Desplaza valores, calcula MI con nivel, identifica máximo
		- **SND:** Mismo proceso
		- **SIL:** Mismo proceso
		- Almacena delay óptimo y valor MI máximo de cada estación
	 - Compara qué estación meteorológica tiene mayor información mutua con el nivel del río y cuál es su delay óptimo (captura relaciones lineales y no lineales).

	 **Diferencia clave vs código anterior:** Este usa Mutual Information en lugar de correlación cruzada, siendo más robusto para detectar relaciones no lineales.
- [sil_snd_aisp_nivel.ipynb](/notebooks/merged/all/sil_snd_aisp_nivel.ipynb):
	- Es lo mismo que el codigo [sil_snd_aisp_nivel.ipynb](/notebooks/merged/all/sil_snd_aisp_nivel.ipynb) pero sin los conjuntos de datos utc a local , asi que no usamos este código, solo se usa para comparación.
- [sil_snd_aisp_nivel_merged_plot.ipynb](/notebooks/merged/all/sil_snd_aisp_nivel_merged_plot.ipynb)

## Flujo de uso
[(Local)sil_snd_aisp_nivel.ipynb](/notebooks/merged/all/(Local)sil_snd_aisp_nivel.ipynb) ---------[Cross_correlation.ipynb](/notebooks/merged/all/Cross_correlation.ipynb) ------------- [Mutual_information.ipynb](/notebooks/merged/all/Mutual_information.ipynb)

## Entradas
-  [aisp_local_nivel_merged.csv](/data/external/aisp_local_nivel_merged.csv) , [snd_local_nivel_merged.csv](/data/external/snd_local_nivel_merged.csv) , [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv) 

## Salidas
- [sil_snd_aisp_local_nivel_merged.csv](/data/external/sil_snd_aisp_local_nivel_merged.csv) 

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
