## Propósito
Se realiza la correlación cruzada y la información mutua para la estación SIL

## Contenido


- [sil_nivel.ipynb](/notebooks/merged/sil/sil_nivel.ipynb)
- [sil_nivel_merged_cross_correlation.ipynb](/notebooks/merged/sil/sil_nivel_merged_cross_correlation.ipynb)
- [sil_nivel_merged_mutal_information.ipynb](/notebooks/merged/sil/sil_nivel_merged_mutal_information.ipynb)
- [sil_nivel_merged_plot.ipynb](/notebooks/merged/sil/sil_nivel_merged_plot.ipynb)

## Archivos relevantes

### Datos
- sin datos

### Notebooks
-  [sil_nivel.ipynb](/notebooks/merged/sil/sil_nivel.ipynb)
- [sil_nivel_merged_cross_correlation.ipynb](/notebooks/merged/sil/sil_nivel_merged_cross_correlation.ipynb)
- [sil_nivel_merged_mutal_information.ipynb](/notebooks/merged/sil/sil_nivel_merged_mutal_information.ipynb)
- [sil_nivel_merged_plot.ipynb](/notebooks/merged/sil/sil_nivel_merged_plot.ipynb)


## Flujo de uso
-  [sil_nivel.ipynb](/notebooks/merged/sil/sil_nivel.ipynb):
	- Se cargan [nivel_processed.csv](/data/processed/nivel_processed.csv) y [sil_processed.csv](/data/processed/sil_processed.csv) salida [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv)
- [sil_nivel_merged_cross_correlation.ipynb](/notebooks/merged/sil/sil_nivel_merged_cross_correlation.ipynb):
	- Igual que los anteriores scripts se realiza la correlación cruzada usando el mismo método 
- [sil_nivel_merged_mutal_information.ipynb](/notebooks/merged/sil/sil_nivel_merged_mutal_information.ipynb):
	- Se realiza la información mutua
- [sil_nivel_merged_plot.ipynb](/notebooks/merged/sil/sil_nivel_merged_plot.ipynb)
	- gráfico

## Entradas
-  [nivel_processed.csv](/data/processed/nivel_processed.csv) y [sil_processed.csv](/data/processed/sil_processed.csv)

## Salidas
- [sil_nivel_merged.csv](/data/external/sil_nivel_merged.csv)

## Observaciones
- Supuestos
- Limitaciones
- Notas importantes
