from __future__ import annotations

from pathlib import Path

import numpy as np
from tqdm import tqdm


SIZES = (10, 100, 10_000, 25_000, 50_000, 100_000)
DISTRIBUTIONS = ("random", "ascending", "descending", "half_sorted")
DTYPES = ("int", "float", "unsigned_int", "char")


def repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def test_data_root() -> Path:
    return repo_root() / "test_data"


def create_numeric_data(
    rng: np.random.Generator,
    size: int,
    dtype_name: str,
) -> np.ndarray:
    if dtype_name == "int":
        return rng.integers(
            np.iinfo(np.int32).min,
            np.iinfo(np.int32).max + 1,
            size=size,
            dtype=np.int64,
        )

    if dtype_name == "float":
        return rng.uniform(-1_000_000.0, 1_000_000.0, size=size).astype(np.float64)

    if dtype_name == "unsigned_int":
        return rng.integers(
            1,
            np.iinfo(np.uint32).max + 1,
            size=size,
            dtype=np.uint64,
        )

    raise ValueError(f"Unsupported numeric dtype: {dtype_name}")


def create_char_data(rng: np.random.Generator, size: int) -> np.ndarray:
    codes = rng.integers(32, 127, size=size, dtype=np.uint8)
    return np.char.decode(codes.view("S1"), "ascii")


def apply_distribution(data: np.ndarray, distribution: str) -> np.ndarray:
    if distribution == "random":
        return data

    if distribution == "ascending":
        return np.sort(data, kind="quicksort")

    if distribution == "descending":
        return np.sort(data, kind="quicksort")[::-1]

    if distribution == "half_sorted":
        midpoint = data.size // 2
        first_half = np.sort(data[:midpoint], kind="quicksort")
        result = data.copy()
        result[:midpoint] = first_half
        return result

    raise ValueError(f"Unsupported distribution: {distribution}")


def format_for_output(data: np.ndarray, dtype_name: str) -> tuple[np.ndarray, str]:
    if dtype_name == "float":
        return data.astype(np.float64, copy=False), "%.17g"

    if dtype_name == "char":
        return data.astype(str, copy=False), "%s"

    return data, "%s"


def write_file(path: Path, data: np.ndarray, fmt: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="\n") as handle:
        handle.write(f"{data.size}\n")
        np.savetxt(handle, data, fmt=fmt)


def generate_one_file(
    output_dir: Path,
    dtype_name: str,
    distribution: str,
    size: int,
    rng: np.random.Generator,
) -> Path:
    if dtype_name == "char":
        data = create_char_data(rng, size)
    else:
        data = create_numeric_data(rng, size, dtype_name)

    data = apply_distribution(data, distribution)
    data, fmt = format_for_output(data, dtype_name)

    output_path = output_dir / dtype_name / f"{distribution}_{size}.txt"
    write_file(output_path, data, fmt)
    return output_path


def generate_all() -> None:
    root = test_data_root()
    root.mkdir(parents=True, exist_ok=True)

    total_files = len(DTYPES) * len(DISTRIBUTIONS) * len(SIZES)
    rng = np.random.default_rng()

    with tqdm(total=total_files, desc="Generating test data", unit="file") as progress:
        for dtype_name in DTYPES:
            for distribution in DISTRIBUTIONS:
                for size in SIZES:
                    generate_one_file(root, dtype_name, distribution, size, rng)
                    progress.update(1)


def main() -> None:
    generate_all()


if __name__ == "__main__":
    main()
