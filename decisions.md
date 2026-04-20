# ABI

- `Parameters` собираем как macOS shared library `libparameters.dylib`, не как Linux-style `.so`.
- Целевая архитектура сейчас `arm64` only.
- Для сборки используем Apple Clang из Xcode Command Line Tools.
- Публичная граница библиотеки сейчас C++-only API из `Parameters.h`, без `extern "C"` слоя.
- Библиотеку и код-потребитель стараемся собирать одним toolchain, чтобы не ловить ABI-проблемы.

# Placement

- Исходники библиотеки остаются в `for-students-master/sem_2025-2026/AiZO-P1-sortingAlgorithms`.
- Готовую библиотеку копируем в корневую папку проекта `lib/`.
- Заголовок `Parameters.h` копируем в корневую папку проекта `include/`.
- Системный `/lib` не используем.

# CMake

- Основной проект пока не подключает `libparameters.dylib`; это можно добавить отдельно после установки библиотеки в `lib/`.
- Для подключения библиотеки из `CMake` предпочтительно использовать путь внутри репозитория, а не системные директории.
- Для локального теста подключаем `libparameters.dylib` к `aizo_proj` напрямую из `lib/` и добавляем `BUILD_RPATH` на эту папку.
