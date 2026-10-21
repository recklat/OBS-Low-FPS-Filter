# Low FPS Filter para OBS — paquete con compilación automatizada

Este paquete prepara el código del filtro y un flujo de GitHub Actions que intenta compilarlo en Windows usando la plantilla oficial `obsproject/obs-plugintemplate`.

## Cómo obtener el plugin compilado sin instalar Visual Studio en tu PC

1. Descomprime este ZIP.
2. En GitHub, crea un repositorio nuevo (puede ser privado).
3. Sube **el contenido** de esta carpeta al repositorio, conservando la ruta `.github/workflows/build.yml`.
4. Abre la pestaña **Actions** del repositorio.
5. Selecciona **Build Windows OBS plugin** y pulsa **Run workflow** (o espera a que termine la ejecución automática tras subir los archivos).
6. Cuando termine correctamente, abre la ejecución y descarga el artefacto `obs-low-fps-filter-windows-x64`.
7. Revisa el contenido del ZIP descargado y sigue la estructura de carpetas que incluye para copiar el plugin a la instalación de OBS.

## Importante

- Este entorno no ha ejecutado la compilación; el artefacto solo existirá si el flujo de GitHub Actions termina con éxito.
- La plantilla oficial descarga y compila sus dependencias. La versión de `libobs` usada depende del `buildspec.json` actual de la plantilla; no se garantiza compatibilidad binaria con OBS 32.2.2 hasta probarlo.
- El código del filtro descarta fotogramas intermedios para aproximar la frecuencia indicada. Es un prototipo y necesita pruebas reales en OBS antes de considerarlo estable.
- Si la compilación falla, abre el registro de Actions y comparte el error para corregir el proyecto.
