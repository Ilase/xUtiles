#!/bin/bash

# Проверка на наличие аргумента версии
if [ -z "$1" ]; then
  echo "Usage: $0 <version>"
  exit 1
fi

VERSION=$1
BUILD_DIR="builds/v$VERSION"

# Создание папки сборки
mkdir -p $BUILD_DIR

# Переход в папку сборки и запуск CMake
cd $BUILD_DIR
cmake ../../

echo "Build directory for version $VERSION created at $BUILD_DIR"
