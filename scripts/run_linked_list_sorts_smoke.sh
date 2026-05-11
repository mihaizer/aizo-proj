#!/bin/bash

set -euo pipefail

# Quick smoke benchmark for the honest singly linked list sort implementations.
# Defaults cover only the algorithms changed in this branch:
#   2 - Merge sort
#   3 - Insertion sort

EXE="${EXE:-./build/aizo_proj}"
RESULTS_DIR="${RESULTS_DIR:-results}"
RESULTS_FILE="${RESULTS_FILE:-$RESULTS_DIR/linked_list_new_sorts_smoke.csv}"
ITERATIONS="${ITERATIONS:-1}"
DATA_TYPE="${DATA_TYPE:-0}"      # 0 - int
STRUCTURE="${STRUCTURE:-1}"      # 1 - single linked list
DISTRIBUTION="${DISTRIBUTION:-0}" # 0 - random
CLEAR_RESULTS="${CLEAR_RESULTS:-1}"

# Keep the same size set as the existing grade-3 complexity script.
SIZES=(${SIZES:-5000 10000 25000 50000})
ALGORITHMS=(${ALGORITHMS:-2 3})

mkdir -p "$RESULTS_DIR"

if [[ "$CLEAR_RESULTS" == "1" ]]; then
    rm -f "$RESULTS_FILE"
fi

if [[ ! -x "$EXE" ]]; then
    echo "ERROR: executable $EXE was not found. Build the project first."
    echo "Hint: cmake --build build"
    exit 1
fi

if [[ "$ITERATIONS" != "1" ]]; then
    echo "WARNING: ITERATIONS=$ITERATIONS; default smoke mode uses one iteration."
fi

algorithm_name() {
    case "$1" in
        1) echo "cocktail" ;;
        2) echo "merge" ;;
        3) echo "insertion" ;;
        *) echo "unknown-$1" ;;
    esac
}

echo "=========================================================="
echo "Starting linked-list smoke benchmarks"
echo "Executable: $EXE"
echo "Algorithms: ${ALGORITHMS[*]}"
echo "Sizes: ${SIZES[*]}"
echo "Iterations per command: $ITERATIONS"
echo "Results file: $RESULTS_FILE"
echo "=========================================================="

for algorithm in "${ALGORITHMS[@]}"; do
    for size in "${SIZES[@]}"; do
        label="$(algorithm_name "$algorithm") sort, single linked list, size=$size"
        echo
        echo ">>> Running: $label"

        "$EXE" \
            --benchmark \
            --algorithm "$algorithm" \
            --structure "$STRUCTURE" \
            --dataType "$DATA_TYPE" \
            --distribution "$DISTRIBUTION" \
            --structureSize "$size" \
            --iterations "$ITERATIONS" \
            --resultsFile "$RESULTS_FILE"
    done
done

echo
echo "=========================================================="
echo "SUCCESS: linked-list smoke benchmarks completed."
echo "Results saved in: $RESULTS_FILE"
echo "=========================================================="
