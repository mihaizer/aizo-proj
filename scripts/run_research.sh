#!/bin/bash

set -euo pipefail

# Skrypt automatyzujacy badania do projektu AiZO (Ocena 3.0).
# Wszystkie wyniki trafiaja do jednego pliku CSV.
# Kazdy test jest wpisany jawnie jako osobna linia run_test, zeby latwo wznowic od wybranego miejsca.

EXE="./build/aizo_proj"
RESULTS_DIR="results"
RESULTS_FILE="$RESULTS_DIR/research_data.csv"
ITERATIONS=50
TOTAL_TASKS=35
START_FROM="${START_FROM:-1}"
CLEAR_RESULTS="${CLEAR_RESULTS:-0}"

mkdir -p "$RESULTS_DIR"

if [[ "$CLEAR_RESULTS" == "1" ]]; then
    rm -f "$RESULTS_FILE"
fi

if [[ ! -x "$EXE" ]]; then
    echo "ERROR: executable $EXE was not found. Build the project first."
    exit 1
fi

if [[ ! "$START_FROM" =~ ^[0-9]+$ ]] || (( START_FROM < 1 || START_FROM > TOTAL_TASKS )); then
    echo "ERROR: START_FROM must be a number from 1 to $TOTAL_TASKS."
    exit 1
fi

echo "=========================================================="
echo "Starting Research Grade 3.0 automation..."
echo "Results file: $RESULTS_FILE"
echo "Total tasks: $TOTAL_TASKS"
echo "Start from task: $START_FROM"
echo "Clear results first: $CLEAR_RESULTS"
echo "=========================================================="

run_test() {
    local task_id=$1
    local algo=$2
    local struct=$3
    local type=$4
    local dist=$5
    local size=$6
    local label=$7

    if (( task_id < START_FROM )); then
        echo "[$task_id/$TOTAL_TASKS] Skipping: $label"
        return
    fi

    echo
    echo "[$task_id/$TOTAL_TASKS] Starting: $label"
    echo "Parameters: Size=$size, Dist=$dist, Type=$type"

    local start_time
    local end_time
    local duration
    start_time=$(date +%s)

    "$EXE" \
        --benchmark \
        --algorithm "$algo" \
        --structure "$struct" \
        --dataType "$type" \
        --distribution "$dist" \
        --structureSize "$size" \
        --iterations "$ITERATIONS" \
        --resultsFile "$RESULTS_FILE"

    end_time=$(date +%s)
    duration=$((end_time - start_time))

    echo ">>> [OK] Task $task_id finished in $duration seconds."
}

echo
echo ">>> RUNNING BADANIE A: COMPLEXITY ANALYSIS <<<"
run_test 1 1 0 0 0 5000 "Badanie A - Cocktail Sort on Array"
run_test 2 1 0 0 0 10000 "Badanie A - Cocktail Sort on Array"
run_test 3 1 0 0 0 25000 "Badanie A - Cocktail Sort on Array"
run_test 4 1 0 0 0 50000 "Badanie A - Cocktail Sort on Array"
run_test 5 1 1 0 0 5000 "Badanie A - Cocktail Sort on List"
run_test 6 1 1 0 0 10000 "Badanie A - Cocktail Sort on List"
run_test 7 1 1 0 0 25000 "Badanie A - Cocktail Sort on List"
run_test 8 1 1 0 0 50000 "Badanie A - Cocktail Sort on List"
run_test 9 2 0 0 0 5000 "Badanie A - Merge Sort on Array"
run_test 10 2 0 0 0 10000 "Badanie A - Merge Sort on Array"
run_test 11 2 0 0 0 25000 "Badanie A - Merge Sort on Array"
run_test 12 2 0 0 0 50000 "Badanie A - Merge Sort on Array"
run_test 13 2 1 0 0 5000 "Badanie A - Merge Sort on List"
run_test 14 2 1 0 0 10000 "Badanie A - Merge Sort on List"
run_test 15 2 1 0 0 25000 "Badanie A - Merge Sort on List"
run_test 16 2 1 0 0 50000 "Badanie A - Merge Sort on List"
run_test 17 3 0 0 0 5000 "Badanie A - Insertion Sort on Array"
run_test 18 3 0 0 0 10000 "Badanie A - Insertion Sort on Array"
run_test 19 3 0 0 0 25000 "Badanie A - Insertion Sort on Array"
run_test 20 3 0 0 0 50000 "Badanie A - Insertion Sort on Array"
run_test 21 3 1 0 0 5000 "Badanie A - Insertion Sort on List"
run_test 22 3 1 0 0 10000 "Badanie A - Insertion Sort on List"
run_test 23 3 1 0 0 25000 "Badanie A - Insertion Sort on List"
run_test 24 3 1 0 0 50000 "Badanie A - Insertion Sort on List"

echo
echo ">>> RUNNING BADANIE B: DISTRIBUTION IMPACT <<<"
run_test 25 2 0 0 0 25000 "Badanie B - MergeSort on Array (Random)"
run_test 26 2 0 0 1 25000 "Badanie B - MergeSort on Array (Ascending)"
run_test 27 2 0 0 2 25000 "Badanie B - MergeSort on Array (Asc50)"
run_test 28 2 0 0 3 25000 "Badanie B - MergeSort on Array (Descending)"
run_test 29 2 1 0 0 25000 "Badanie B - MergeSort on List (Random)"
run_test 30 2 1 0 1 25000 "Badanie B - MergeSort on List (Ascending)"
run_test 31 2 1 0 2 25000 "Badanie B - MergeSort on List (Asc50)"
run_test 32 2 1 0 3 25000 "Badanie B - MergeSort on List (Descending)"

echo
echo ">>> RUNNING BADANIE C: DATA TYPE IMPACT <<<"
run_test 33 2 0 0 0 25000 "Badanie C - MergeSort (int)"
run_test 34 2 0 1 0 25000 "Badanie C - MergeSort (float)"
run_test 35 2 0 5 0 25000 "Badanie C - MergeSort (uint)"

echo
echo "=========================================================="
echo "SUCCESS: Tasks from $START_FROM to $TOTAL_TASKS completed."
echo "Results saved in: $RESULTS_FILE"
echo "=========================================================="
