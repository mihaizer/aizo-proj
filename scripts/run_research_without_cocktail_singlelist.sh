#!/bin/bash

set -euo pipefail

# Wersja badan 3.0 bez kombinacji CocktailSort + SinglyLinkedList.
# Wyniki trafiaja do nowego pliku CSV i sa numerowane od task 1 do task 31.

EXE="./build/aizo_proj"
RESULTS_DIR="results"
RESULTS_FILE="$RESULTS_DIR/research_data_without_cocktail_singlelist.csv"
ITERATIONS=50
TOTAL_TASKS=31
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
echo "Starting Research Grade 3.0 automation without CocktailSort + SinglyLinkedList..."
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
run_test 1 1 0 0 0 5000 "Badanie A - CocktailSort on DynamicArray"
run_test 2 1 0 0 0 10000 "Badanie A - CocktailSort on DynamicArray"
run_test 3 1 0 0 0 25000 "Badanie A - CocktailSort on DynamicArray"
run_test 4 1 0 0 0 50000 "Badanie A - CocktailSort on DynamicArray"
run_test 5 2 0 0 0 5000 "Badanie A - MergeSort on DynamicArray"
run_test 6 2 0 0 0 10000 "Badanie A - MergeSort on DynamicArray"
run_test 7 2 0 0 0 25000 "Badanie A - MergeSort on DynamicArray"
run_test 8 2 0 0 0 50000 "Badanie A - MergeSort on DynamicArray"
run_test 9 2 1 0 0 5000 "Badanie A - MergeSort on SinglyLinkedList"
run_test 10 2 1 0 0 10000 "Badanie A - MergeSort on SinglyLinkedList"
run_test 11 2 1 0 0 25000 "Badanie A - MergeSort on SinglyLinkedList"
run_test 12 2 1 0 0 50000 "Badanie A - MergeSort on SinglyLinkedList"
run_test 13 3 0 0 0 5000 "Badanie A - InsertionSort on DynamicArray"
run_test 14 3 0 0 0 10000 "Badanie A - InsertionSort on DynamicArray"
run_test 15 3 0 0 0 25000 "Badanie A - InsertionSort on DynamicArray"
run_test 16 3 0 0 0 50000 "Badanie A - InsertionSort on DynamicArray"
run_test 17 3 1 0 0 5000 "Badanie A - InsertionSort on SinglyLinkedList"
run_test 18 3 1 0 0 10000 "Badanie A - InsertionSort on SinglyLinkedList"
run_test 19 3 1 0 0 25000 "Badanie A - InsertionSort on SinglyLinkedList"
run_test 20 3 1 0 0 50000 "Badanie A - InsertionSort on SinglyLinkedList"

echo
echo ">>> RUNNING BADANIE B: DISTRIBUTION IMPACT <<<"
run_test 21 2 0 0 0 25000 "Badanie B - MergeSort on DynamicArray (Random)"
run_test 22 2 0 0 1 25000 "Badanie B - MergeSort on DynamicArray (Ascending)"
run_test 23 2 0 0 2 25000 "Badanie B - MergeSort on DynamicArray (Asc50)"
run_test 24 2 0 0 3 25000 "Badanie B - MergeSort on DynamicArray (Descending)"
run_test 25 2 1 0 0 25000 "Badanie B - MergeSort on SinglyLinkedList (Random)"
run_test 26 2 1 0 1 25000 "Badanie B - MergeSort on SinglyLinkedList (Ascending)"
run_test 27 2 1 0 2 25000 "Badanie B - MergeSort on SinglyLinkedList (Asc50)"
run_test 28 2 1 0 3 25000 "Badanie B - MergeSort on SinglyLinkedList (Descending)"

echo
echo ">>> RUNNING BADANIE C: DATA TYPE IMPACT <<<"
run_test 29 2 0 0 0 25000 "Badanie C - MergeSort (int)"
run_test 30 2 0 1 0 25000 "Badanie C - MergeSort (float)"
run_test 31 2 0 5 0 25000 "Badanie C - MergeSort (uint)"

echo
echo "=========================================================="
echo "SUCCESS: Tasks from $START_FROM to $TOTAL_TASKS completed."
echo "Results saved in: $RESULTS_FILE"
echo "=========================================================="
