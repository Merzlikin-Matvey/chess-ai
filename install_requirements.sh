#!/bin/bash

# Установим необходимые пакеты
if ! command -v unzip &> /dev/null
then
    echo "Установка unzip"
    sudo apt-get install unzip
    if [ $? -eq 0 ]; then
        echo "Установка unzip прошла успешно"
    else
        echo "Ошибка при установке unzip"
        exit 1
    fi
fi

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

if [ -d "dependencies" ]; then
    echo "Удаление существующей директории dependencies"
    rm -rf dependencies

    if [ $? -eq 0 ]; then
        echo "Удаление директории dependencies прошло успешно"
    else
        echo "Ошибка при удалении директории dependencies"
        exit 1
    fi
fi

# Чтение списка библиотек файла dependencies.json
repositories=($(jq -r '.repositories[]' dependencies.json))

installed_repos=()

for repo in "${repositories[@]}"; do
    IFS='/' read -r -a array <<< "$repo"
    server_name="${array[0]}"
    author_name="${array[1]}"
    repo_name="${array[2]}"
    branch_name="${array[3]}"

    dir_path="dependencies/$author_name/$repo_name"

    mkdir -p $dir_path
    if [[ "$server_name" == "github" ]]; then
        curl -L https://github.com/$author_name/$repo_name/archive/refs/heads/$branch_name.zip --output $dir_path/$repo_name.zip
    else
        echo "Загрузка $repo_name с gitlab"
        echo https://gitlab.com/$author_name/$repo_name/-/archive/$branch_name/$repo_name-$branch_name.zip
        curl -L https://gitlab.com/$author_name/$repo_name/-/archive/$branch_name/$repo_name-$branch_name.zip --output $dir_path/$repo_name.zip
    fi

    unzip -q $dir_path/$repo_name.zip -d $dir_path
    rm $dir_path/$repo_name.zip

    if [ $? -eq 0 ]; then
        echo "Установка $repo_name прошла успешно"
        installed_repos+=("$author_name/$repo_name")
    else
        echo "Ошибка при установке $repo_name"
        exit 1
    fi
done

echo "Установленные репозитории:"
for repo in "${installed_repos[@]}"; do
    echo $repo
done


# Запуск скрипта installer_requirements.sh в каждой из установленных директорий, если он существует
for repo in "${installed_repos[@]}"; do
    IFS='/' read -r -a array <<< "$repo"
    author_name="${array[0]}"
    repo_name="${array[1]}"
    branch_name="${array[2]}"

    dir_path="dependencies/$author_name/$repo_name/$branch_name"

    if [ -f "$dir_path/requirements.sh" ]; then
        echo "Запуск requirements.sh в $repo_name"
        chmod +x $dir_path/requirements.sh
        $dir_path/requirements.sh
        if [ $? -eq 0 ]; then
            echo "Скрипт requirements.sh в $repo_name успешно выполнен"
        else
            echo "Ошибка при выполнении requirements.sh в $repo_name"
            exit 1
        fi
    fi
done
