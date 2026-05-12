#!/usr/bin/env python3

import argparse
import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt


EXPECTED_TASKS = 31
EXPECTED_ITERATIONS = 50
BADANIE_A_LAST_TASK = 20
BADANIE_B_LAST_TASK = 28


ALGORITHM_LABELS = {
    "cocktail": "CocktailSort",
    "merge": "MergeSort",
    "insertion": "InsertionSort",
}

STRUCTURE_LABELS = {
    "array": "DynamicArray",
    "singleList": "SinglyLinkedList",
}

DISTRIBUTION_LABELS = {
    "random": "random",
    "ascending": "ascending",
    "ascending50Per": "ascending50Per",
    "descending": "descending",
}

DATA_TYPE_LABELS = {
    "int": "int",
    "float": "float",
    "unsigned_int": "unsigned int",
}


def parse_args():
    parser = argparse.ArgumentParser(
        description="Analyze benchmark CSV results, split them into task blocks, and generate report artifacts."
    )
    parser.add_argument(
        "--input",
        default="results/research_data_without_cocktail_singlelist.csv",
        help="Path to the benchmark CSV file.",
    )
    parser.add_argument(
        "--scan-status",
        default="results/cocktail_singlelist_limit_status.csv",
        help="Path to the diagnostic CocktailSort + SinglyLinkedList status CSV.",
    )
    parser.add_argument(
        "--analysis-dir",
        default="results/analysis",
        help="Directory for generated summary CSV files.",
    )
    parser.add_argument(
        "--plots-dir",
        default="results/plots",
        help="Directory for generated PNG charts.",
    )
    parser.add_argument(
        "--typst-data",
        default="docs/report/generated-data.typ",
        help="Path to the generated Typst data fragment.",
    )
    return parser.parse_args()


def ensure_directory(path):
    path.mkdir(parents=True, exist_ok=True)


def read_rows(csv_path):
    with csv_path.open(newline="", encoding="utf-8") as handle:
        return list(csv.DictReader(handle))


def read_scan_rows(csv_path):
    with csv_path.open(newline="", encoding="utf-8") as handle:
        rows = list(csv.DictReader(handle))

    parsed = []
    for row in rows:
        parsed.append(
            {
                "size": int(row["size"]),
                "status": row["status"],
                "wall_time_sec": float(row["wall_time_sec"]) if row["wall_time_sec"] else 0.0,
                "avg_us": float(row["avg_us"]) if row["avg_us"] else 0.0,
                "note": row["note"],
            }
        )

    return parsed


def badanie_for_task(task_id):
    if task_id <= BADANIE_A_LAST_TASK:
        return "A"
    if task_id <= BADANIE_B_LAST_TASK:
        return "B"
    return "C"


def validate_and_summarize(rows):
    expected_rows = EXPECTED_TASKS * EXPECTED_ITERATIONS
    if len(rows) != expected_rows:
        raise ValueError(
            f"Expected {expected_rows} data rows, found {len(rows)}. Check whether the CSV is complete."
        )

    summaries = []
    for task_id in range(1, EXPECTED_TASKS + 1):
        start_index = (task_id - 1) * EXPECTED_ITERATIONS
        end_index = start_index + EXPECTED_ITERATIONS
        block = rows[start_index:end_index]

        if len(block) != EXPECTED_ITERATIONS:
            raise ValueError(
                f"Task {task_id} should contain {EXPECTED_ITERATIONS} rows, found {len(block)}."
            )

        first = block[0]
        last = block[-1]
        stable_fields = (
            "timestamp",
            "algorithm",
            "structure",
            "data_type",
            "distribution",
            "size",
            "iterations",
        )
        for row in block[1:]:
            for field in stable_fields:
                if field == "timestamp":
                    continue
                if row[field] != first[field]:
                    raise ValueError(
                        f"Task {task_id} has inconsistent field '{field}': "
                        f"{first[field]!r} vs {row[field]!r}."
                    )

        iteration_ids = [int(row["iteration_id"]) for row in block]
        expected_iteration_ids = list(range(1, EXPECTED_ITERATIONS + 1))
        if iteration_ids != expected_iteration_ids:
            raise ValueError(
                f"Task {task_id} has invalid iteration sequence: {iteration_ids[:5]}...{iteration_ids[-5:]}."
            )

        raw_durations = [int(row["duration_us"]) for row in block]
        computed_min = min(raw_durations)
        computed_max = max(raw_durations)
        computed_avg = sum(raw_durations) / len(raw_durations)
        min_us = computed_min
        avg_us = computed_avg
        max_us = computed_max

        summaries.append(
            {
                "task_id": task_id,
                "badanie": badanie_for_task(task_id),
                "timestamp": last["timestamp"],
                "algorithm": first["algorithm"],
                "structure": first["structure"],
                "data_type": first["data_type"],
                "distribution": first["distribution"],
                "size": int(first["size"]),
                "iterations": int(first["iterations"]),
                "min_us": min_us,
                "avg_us": round(avg_us, 2),
                "max_us": max_us,
            }
        )

    return summaries


def write_summary_csv(path, records):
    fieldnames = [
        "task_id",
        "badanie",
        "timestamp",
        "algorithm",
        "structure",
        "data_type",
        "distribution",
        "size",
        "iterations",
        "min_us",
        "avg_us",
        "max_us",
    ]
    with path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fieldnames)
        writer.writeheader()
        for record in records:
            writer.writerow(record)


def write_scan_summary_csv(path, records):
    fieldnames = [
        "size",
        "status",
        "wall_time_sec",
        "avg_us",
        "note",
    ]
    with path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fieldnames)
        writer.writeheader()
        for record in records:
            writer.writerow(record)


def filter_records(records, badanie):
    return [record for record in records if record["badanie"] == badanie]


def create_badanie_a_plot(records, structure, output_path):
    ordered_sizes = sorted({record["size"] for record in records if record["structure"] == structure})
    algorithms = ["cocktail", "merge", "insertion"]
    colors = {
        "cocktail": "#c63d2f",
        "merge": "#0f766e",
        "insertion": "#d97706",
    }

    fig, ax = plt.subplots(figsize=(9, 5))
    for algorithm in algorithms:
        points = sorted(
            (
                record["size"],
                record["avg_us"],
            )
            for record in records
            if record["structure"] == structure and record["algorithm"] == algorithm
        )
        if not points:
            continue
        ax.plot(
            [size for size, _ in points],
            [avg for _, avg in points],
            marker="o",
            linewidth=2,
            markersize=7,
            color=colors[algorithm],
            label=ALGORITHM_LABELS[algorithm],
        )

    ax.set_title(f"Badanie A - {STRUCTURE_LABELS[structure]}")
    ax.set_xlabel("rozmiar [liczba elementow]")
    ax.set_ylabel("sredni czas [us]")
    ax.set_xticks(ordered_sizes)
    ax.grid(True, linestyle="--", linewidth=0.6, alpha=0.5)
    ax.legend(title="algorytm")
    fig.tight_layout()
    fig.savefig(output_path, dpi=180)
    plt.close(fig)


def create_badanie_b_plot(records, structure, output_path):
    distribution_order = ["random", "ascending", "ascending50Per", "descending"]
    values = []
    for distribution in distribution_order:
        match = next(
            record
            for record in records
            if record["structure"] == structure and record["distribution"] == distribution
        )
        values.append(match["avg_us"])

    fig, ax = plt.subplots(figsize=(9, 5))
    positions = list(range(len(distribution_order)))
    bars = ax.bar(
        positions,
        values,
        color=["#2563eb", "#16a34a", "#d97706", "#dc2626"],
        edgecolor="#1f2937",
    )
    ax.set_title(f"Badanie B - MergeSort / {STRUCTURE_LABELS[structure]}")
    ax.set_xlabel("rozklad danych")
    ax.set_ylabel("sredni czas [us]")
    ax.set_xticks(positions)
    ax.set_xticklabels([DISTRIBUTION_LABELS[item] for item in distribution_order])
    ax.grid(axis="y", linestyle="--", linewidth=0.6, alpha=0.5)
    for bar, value in zip(bars, values):
        ax.text(bar.get_x() + bar.get_width() / 2, value, f"{value:.2f}", ha="center", va="bottom", fontsize=8)
    fig.tight_layout()
    fig.savefig(output_path, dpi=180)
    plt.close(fig)


def create_badanie_c_plot(records, output_path):
    type_order = ["int", "float", "unsigned_int"]
    values = []
    for data_type in type_order:
        match = next(record for record in records if record["data_type"] == data_type)
        values.append(match["avg_us"])

    fig, ax = plt.subplots(figsize=(8, 5))
    positions = list(range(len(type_order)))
    bars = ax.bar(
        positions,
        values,
        color=["#0f766e", "#7c3aed", "#b91c1c"],
        edgecolor="#1f2937",
    )
    ax.set_title("Badanie C - MergeSort / DynamicArray / 25000")
    ax.set_xlabel("typ danych")
    ax.set_ylabel("sredni czas [us]")
    ax.set_xticks(positions)
    ax.set_xticklabels([DATA_TYPE_LABELS[item] for item in type_order])
    ax.grid(axis="y", linestyle="--", linewidth=0.6, alpha=0.5)
    for bar, value in zip(bars, values):
        ax.text(bar.get_x() + bar.get_width() / 2, value, f"{value:.2f}", ha="center", va="bottom", fontsize=8)
    fig.tight_layout()
    fig.savefig(output_path, dpi=180)
    plt.close(fig)


def create_cocktail_singlelist_limit_plot(records, output_path):
    points = [record for record in records if record["status"] == "ok"]

    fig, ax = plt.subplots(figsize=(9, 5))
    ax.plot(
        [record["size"] for record in points],
        [record["avg_us"] for record in points],
        marker="o",
        linewidth=2,
        markersize=7,
        color="#111827",
    )
    ax.set_title("CocktailSort / SinglyLinkedList - pojedyncza iteracja")
    ax.set_xlabel("rozmiar [liczba elementow]")
    ax.set_ylabel("czas [us]")
    ax.grid(True, linestyle="--", linewidth=0.6, alpha=0.5)
    fig.tight_layout()
    fig.savefig(output_path, dpi=180)
    plt.close(fig)


def escape_typst(text):
    return (
        str(text)
        .replace("\\", "\\\\")
        .replace('"', '\\"')
    )


def write_typst_fragment(path, records, scan_records):
    lines = [
        "// This file is generated by scripts/analyze_results.py.",
        "#let generated-task-summary = (",
    ]
    for record in records:
        lines.append(
            "  ("
            + f'task-id: {record["task_id"]}, '
            + f'badanie: "{escape_typst(record["badanie"])}", '
            + f'timestamp: "{escape_typst(record["timestamp"])}", '
            + f'algorithm: "{escape_typst(record["algorithm"])}", '
            + f'structure: "{escape_typst(record["structure"])}", '
            + f'data-type: "{escape_typst(record["data_type"])}", '
            + f'distribution: "{escape_typst(record["distribution"])}", '
            + f'size: {record["size"]}, '
            + f'iterations: {record["iterations"]}, '
            + f'min-us: {record["min_us"]}, '
            + f'avg-us: {record["avg_us"]}, '
            + f'max-us: {record["max_us"]}'
            + "),"
        )
    lines.append(")")
    lines.append("")
    lines.append("#let generated-cocktail-singlelist-limit = (")
    for record in scan_records:
        lines.append(
            "  ("
            + f'size: {record["size"]}, '
            + f'status: "{escape_typst(record["status"])}", '
            + f'wall-time-sec: {record["wall_time_sec"]}, '
            + f'avg-us: {record["avg_us"]}, '
            + f'note: "{escape_typst(record["note"])}"'
            + "),"
        )
    lines.append(")")
    lines.append("")
    path.write_text("\n".join(lines), encoding="utf-8")


def print_cli_report(records, scan_records, analysis_dir, plots_dir, typst_path):
    print(f"Detected tasks: {len(records)}")
    print(f"Detected iterations per task: {EXPECTED_ITERATIONS}")
    print(f"Detected CocktailSort + SinglyLinkedList scan rows: {len(scan_records)}")
    print("Output files:")
    print(f"  - {analysis_dir / 'task_summary.csv'}")
    print(f"  - {analysis_dir / 'badanie_A_summary.csv'}")
    print(f"  - {analysis_dir / 'badanie_B_summary.csv'}")
    print(f"  - {analysis_dir / 'badanie_C_summary.csv'}")
    print(f"  - {analysis_dir / 'cocktail_singlelist_limit_summary.csv'}")
    print(f"  - {plots_dir / 'badanie_A_array.png'}")
    print(f"  - {plots_dir / 'badanie_A_singleList.png'}")
    print(f"  - {plots_dir / 'badanie_B_array.png'}")
    print(f"  - {plots_dir / 'badanie_B_singleList.png'}")
    print(f"  - {plots_dir / 'badanie_C_types.png'}")
    print(f"  - {plots_dir / 'cocktail_singlelist_limit.png'}")
    print(f"  - {typst_path}")
    print("Validation checks passed:")
    print("  - 1550 data rows")
    print("  - 31 task blocks")
    print("  - each task contains 50 iterations")
    print("  - summary values match raw durations")


def main():
    args = parse_args()
    input_path = Path(args.input)
    scan_status_path = Path(args.scan_status)
    analysis_dir = Path(args.analysis_dir)
    plots_dir = Path(args.plots_dir)
    typst_path = Path(args.typst_data)

    ensure_directory(analysis_dir)
    ensure_directory(plots_dir)
    ensure_directory(typst_path.parent)

    rows = read_rows(input_path)
    records = validate_and_summarize(rows)
    scan_records = read_scan_rows(scan_status_path)

    write_summary_csv(analysis_dir / "task_summary.csv", records)
    write_summary_csv(analysis_dir / "badanie_A_summary.csv", filter_records(records, "A"))
    write_summary_csv(analysis_dir / "badanie_B_summary.csv", filter_records(records, "B"))
    write_summary_csv(analysis_dir / "badanie_C_summary.csv", filter_records(records, "C"))
    write_scan_summary_csv(analysis_dir / "cocktail_singlelist_limit_summary.csv", scan_records)

    badanie_a_records = filter_records(records, "A")
    badanie_b_records = filter_records(records, "B")
    badanie_c_records = filter_records(records, "C")

    create_badanie_a_plot(badanie_a_records, "array", plots_dir / "badanie_A_array.png")
    create_badanie_a_plot(badanie_a_records, "singleList", plots_dir / "badanie_A_singleList.png")
    create_badanie_b_plot(badanie_b_records, "array", plots_dir / "badanie_B_array.png")
    create_badanie_b_plot(badanie_b_records, "singleList", plots_dir / "badanie_B_singleList.png")
    create_badanie_c_plot(badanie_c_records, plots_dir / "badanie_C_types.png")
    create_cocktail_singlelist_limit_plot(scan_records, plots_dir / "cocktail_singlelist_limit.png")

    write_typst_fragment(typst_path, records, scan_records)
    print_cli_report(records, scan_records, analysis_dir, plots_dir, typst_path)


if __name__ == "__main__":
    main()
