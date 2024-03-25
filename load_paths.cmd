@echo off
setlocal enabledelayedexpansion

REM Папки, которые надо создать и добавить в json
set directories=saves
set names=SAVES_DIR

REM Создадим settings.json
echo { > settings.json

REM Создание папок и получение их абсолютных путей
for %%i in (%directories%) do (
    if not exist %%i (
        mkdir %%i
    )
    REM Получение абсолютного пути к только что созданной директории
    cd /D %%i
    for /f "delims=" %%j in ('cd') do set "abs_path=%%j"
    cd /D ..
    REM Замена всех обратных слешей на двойные обратные слеши
    set "abs_path=!abs_path:\=\\!"
    echo    "!names!": "!abs_path!" >> settings.json
)

echo } >> settings.json