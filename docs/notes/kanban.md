# Kanban

## To Do

### Project scaffolding

- tags: [setup, build, dependency]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Подготовить базовую структуру проекта: `src`, `include`, `lib`, `scripts`, `docs`, `CMakeLists.txt`, `.gitignore` и минимальный `main.cpp` с `--help`.
```

### Parameter library integration

- tags: [dependency, mac, parameters]
- priority: high
- workload: Normal
- defaultExpanded: false
```md
Скачать и собрать библиотеку `parameters.cpp/h` из репозитория преподавателя под Mac ARM, чтобы проект корректно работал без несовместимого `.so`.
```

### Custom dynamic array

- tags: [data-structures, memory]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Реализовать шаблонный динамический массив на ручной памяти через `new[]` и `delete[]` с базовыми операциями доступа и изменения.
```

### Custom linked list

- tags: [data-structures, memory]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Реализовать шаблонный связный список с узлами `Node`, поддержкой добавления, удаления и доступа к элементам.
```

### Memory leak tests

- tags: [testing, quality, memory]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Проверить создание и удаление больших объёмов данных, чтобы убедиться, что нет утечек памяти, и прогнать это через `Leaks` или `AddressSanitizer`.
```

### Sorting validation

- tags: [algorithms, sorting, validation]
- priority: medium
- workload: Easy
- defaultExpanded: false
```md
Сделать `isSorted`, чтобы автоматически проверять, что результат сортировки действительно упорядочен по возрастанию.
```

### Sorting algorithms

- tags: [algorithms, sorting]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Реализовать набор шаблонных сортировок: QuickSort, ShellSort и ещё один алгоритм на выбор, чтобы закрыть требования по разным вариантам.
```

### QuickSort pivots

- tags: [algorithms, sorting]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Добавить выбор пивота для QuickSort: случайный, средний и крайний элемент, если нужна версия под более высокий балл.
```

### Benchmark pipeline

- tags: [benchmarking, timing, io]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Настроить замер времени через `std::chrono`, чтение и запись входных данных, а также режим benchmark с прогоном `N` раз и расчётом средней, минимальной и максимальной длительности.
```

### CSV logging

- tags: [benchmarking, logging, csv]
- priority: high
- workload: Normal
- defaultExpanded: false
```md
Сделать дозапись результатов в `.csv` после выполнения сортировки, чтобы не мешать самому алгоритму.
```

### Data generator

- tags: [automation, scripts, input]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Скриптом генерировать тестовые файлы со случайными, отсортированными, обратно отсортированными и наполовину отсортированными данными.
```

### Test runner

- tags: [automation, scripts, benchmarking]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Сделать раннер, который запускает бинарник много раз для каждого типа входных данных и собирает итоговую статистику.
```

### Report prep

- tags: [report, documentation]
- priority: medium
- workload: Easy
- defaultExpanded: false
```md
Подготовить анализ сложности алгоритмов, описание плюсов и минусов, а также упоминание, что код взят из репозитория преподавателя.
```

### Charts

- tags: [report, visualization]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Построить графики зависимости времени выполнения от размера входных данных.
```

### Warning-free build

- tags: [build, quality]
- priority: high
- workload: Easy
- defaultExpanded: false
```md
Финально проверить, что проект собирается без предупреждений и проходит все ключевые проверки.
```

### Badanie A: size impact

- tags: [research, benchmarking, report]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Проверить влияние размера входного множества на время работы алгоритма. Это эпик для нескольких запусков на сильно разных размерах, например 5k, 10k, 25k, 50k или 10k, 25k, 50k, 100k.
```

### Badanie B: distribution impact

- tags: [research, benchmarking, report]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Проверить влияние распределения элементов на результат алгоритма для одного выбранного размера из Badanie A: случайное, по убыванию, по возрастанию и наполовину отсортированное.
```

### Badanie C: data type impact

- tags: [research, benchmarking, report]
- priority: high
- workload: Hard
- defaultExpanded: false
```md
Проверить влияние типа данных на время работы алгоритма для того же размера, что и в Badanie B: `int` и другие выбранные типы.
```

### Badanie Omega: non-linear structures

- tags: [research, data-structures, report]
- priority: medium
- workload: Normal
- defaultExpanded: false
```md
Сравнить нелинейные структуры данных с линейными и описать, где какая структура даёт выигрыш.
```

## In Progress

## Done
