#!/bin/bash

# Установим необходимые пакеты
if ! command -v jq &> /dev/null
then
    echo "Установка jq"
    sudo apt-get install jq
    if [ $? -eq 0 ]; then
        echo "Установка jq прошла успешно"
    else
        echo "Ошибка при установке jq"
        exit 1
    fi
fi

# Папки, которые надо создать и добавить в json
directories=("saves")
names=("SAVES_DIR")
real_directories=()

# Для каждой папки найдем ее абсолютный путь
for dir in "${directories[@]}"; do
    real_directories+=($(realpath $dir))
done


# Создадим нужные папки
for dir in "${directories[@]}"; do
    if [ ! -d "$dir" ]; then
        echo "Создание директории $dir"
        mkdir -p $dir
        if [ $? -eq 0 ]; then
            echo "Создание директории $dir прошло успешно"
        else
            echo "Ошибка при создании директории $dir"
            exit 1
        fi
    fi
done

# Создадим пустой settings.json, если его нет

if [ ! -f "settings.json" ]; then
    echo "Создание файла settings.json"
    echo "{}" > settings.json
    if [ $? -eq 0 ]; then
        echo "Создание файла settings.json прошло успешно"
    else
        echo "Ошибка при создании файла settings.json"
        exit 1
    fi
fi

# Добавим в settings.json пути до папок и их переменные
for ((i=0; i<${#directories[@]}; i++)); do
    if ! grep -q "${names[$i]}" settings.json; then
        jq ".${names[$i]} = \"${real_directories[$i]}\"" settings.json > tmp.$$.json && mv tmp.$$.json settings.json
        if ! [ $? -eq 0 ]; then
          echo "Ошибка при добавлении в settings.json пути до ${directories[$i]}"
          exit 1
        fi
    fi
done

echo "Пути до директорий добавлены в settings.json"




