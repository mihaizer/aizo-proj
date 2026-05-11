#!/usr/bin/env python3

import csv
import re
import subprocess
import time
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parent.parent
EXE = PROJECT_ROOT / "build" / "aizo_proj"
RAW_RESULTS = PROJECT_ROOT / "results" / "cocktail_singlelist_limit_scan.csv"
STATUS_RESULTS = PROJECT_ROOT / "results" / "cocktail_singlelist_limit_status.csv"
TIMEOUT_SECONDS = 120
SIZES = [100, 200, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7500, 10000]


def parse_avg_us(stdout_text):
    match = re.search(r"avg:\s+([0-9]+(?:\.[0-9]+)?)\s+us", stdout_text)
    if match is None:
        return ""
    return match.group(1)


def main():
    if not EXE.exists():
        raise SystemExit(f"ERROR: executable not found: {EXE}")

    RAW_RESULTS.parent.mkdir(parents=True, exist_ok=True)
    if RAW_RESULTS.exists():
        RAW_RESULTS.unlink()

    with STATUS_RESULTS.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.writer(handle)
        writer.writerow(["size", "status", "wall_time_sec", "avg_us", "note"])

        for size in SIZES:
            command = [
                str(EXE),
                "--benchmark",
                "--algorithm",
                "1",
                "--structure",
                "1",
                "--dataType",
                "0",
                "--distribution",
                "0",
                "--structureSize",
                str(size),
                "--iterations",
                "1",
                "--resultsFile",
                str(RAW_RESULTS),
            ]

            print(f"Starting size={size}...")
            start = time.monotonic()

            try:
                completed = subprocess.run(
                    command,
                    cwd=PROJECT_ROOT,
                    capture_output=True,
                    text=True,
                    timeout=TIMEOUT_SECONDS,
                    check=False,
                )
            except subprocess.TimeoutExpired:
                elapsed = time.monotonic() - start
                writer.writerow([size, "timeout", f"{elapsed:.2f}", "", f"exceeded {TIMEOUT_SECONDS} seconds"])
                print(f"Stopping at size={size}: run exceeded {TIMEOUT_SECONDS} seconds.")
                break

            elapsed = time.monotonic() - start

            if completed.returncode != 0:
                note = completed.stderr.strip() or completed.stdout.strip() or f"exit code {completed.returncode}"
                writer.writerow([size, "error", f"{elapsed:.2f}", "", note])
                print(f"Stopping at size={size}: command failed.")
                break

            avg_us = parse_avg_us(completed.stdout)
            writer.writerow([size, "ok", f"{elapsed:.2f}", avg_us, ""])
            print(f"Finished size={size} in {elapsed:.2f}s (avg {avg_us or 'n/a'} us).")

    print(f"Raw benchmark rows: {RAW_RESULTS}")
    print(f"Status summary: {STATUS_RESULTS}")


if __name__ == "__main__":
    main()
