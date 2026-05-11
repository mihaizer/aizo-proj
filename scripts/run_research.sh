#!/bin/bash

# Skrypt automatyzujący badania do projektu AiZO (Ocena 3.0)
# Wszystkie wyniki trafiają do jednego pliku CSV.

EXE="./build/aizo_proj"
RESULTS_DIR="results"
RESULTS_FILE="$RESULTS_DIR/research_data.csv"
ITERATIONS=50 

mkdir -p $RESULTS_DIR
rm -f $RESULTS_FILE

# Licznik zadań (24 z A + 8 z B + 3 z C = 35 zadań)
TOTAL_TASKS=35
CURRENT_TASK=0

echo "=========================================================="
echo "Starting Research Grade 3.0 automation..."
echo "Results will be saved to: $RESULTS_FILE"
echo "Total tasks to complete: $TOTAL_TASKS"
echo "=========================================================="

run_test() {
    CURRENT_TASK=$((CURRENT_TASK + 1))
    local algo=$1
    local struct=$2
    local type=$3
    local dist=$4
    local size=$5
    local label=$6

    echo -e "\n[$CURRENT_TASK/$TOTAL_TASKS] Starting: $label"
    echo "Parameters: Size=$size, Dist=$dist, Type=$type"
    
    # Start stoper
    start_time=$(date +%s)

    # Uruchamiamy program i pozwalamy mu pisać do konsoli, żeby było widać, że żyje
    $EXE --benchmark --algorithm $algo --structure $struct --dataType $type --distribution $dist --structureSize $size --iterations $ITERATIONS --resultsFile $RESULTS_FILE

    # Koniec stoper
    end_time=$(date +%s)
    duration=$((end_time - start_time))

    echo ">>> [OK] Task $CURRENT_TASK finished in $duration seconds."
}

# --- BADANIE A: Algorytmy i Struktury ---
echo -e "\n>>> RUNNING BADANIE A: COMPLEXITY ANALYSIS <<<"
for algo_name in "Cocktail:1" "Merge:2" "Insertion:3"; do
    IFS=":" read -r name algo_id <<< "$algo_name"
    for struct_name in "Array:0" "List:1"; do
        IFS=":" read -r sname struct_id <<< "$struct_name"
        for size in 5000 10000 25000 50000; do
            run_test $algo_id $struct_id 0 0 $size "Badanie A - $name Sort on $sname"
        done
    done
done

# --- BADANIE B: Rozkłady (Distributions) ---
echo -e "\n>>> RUNNING BADANIE B: DISTRIBUTION IMPACT <<<"
for struct_name in "Array:0" "List:1"; do
    IFS=":" read -r sname struct_id <<< "$struct_name"
    for dist_name in "Random:0" "Ascending:1" "Asc50:2" "Descending:3"; do
        IFS=":" read -r dname dist_id <<< "$dist_name"
        run_test 2 $struct_id 0 $dist_id 25000 "Badanie B - MergeSort on $sname ($dname)"
    done
done

# --- BADANIE C: Typy Danych (Data Types) ---
echo -e "\n>>> RUNNING BADANIE C: DATA TYPE IMPACT <<<"
for type_name in "int:0" "float:1" "uint:5"; do
    IFS=":" read -r tname type_id <<< "$type_name"
    run_test 2 0 $type_id 0 25000 "Badanie C - MergeSort ($tname)"
done

echo -e "\n=========================================================="
echo "SUCCESS: All $TOTAL_TASKS tasks completed!"
echo "Final results saved in: $RESULTS_FILE"
echo "=========================================================="
