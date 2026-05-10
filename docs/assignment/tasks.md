# Tasks List

## PR 1 - feature/dynamic-array-cleanup
- [x] Start from the `Dynamic-array` branch.
- [x] Move `DynamicArray` template implementations from `.h` to `.tpp`.
- [x] Keep `DynamicArray.h` as the public declaration file.
- [x] Change `pushBack()` capacity growth from `*2` to `+1`.
- [x] Preserve manual `new[]`/`delete[]` memory management and copy support.
- [x] Confirm the branch builds cleanly.

## PR 2 - feature/parameters-v12
- [x] Download the professor `for-students` repository from GitLab.
- [x] Check the provided `AiZO-P1-sortingAlgorithms` build files and source.
- [x] Copy `Parameters.h` v1.2 into `include/` and `lib/`.
- [x] Build and install a local macOS `libparameters.dylib` v1.2 into `lib/`.
- [x] Confirm `--distribution` parsing through the installed library.
- [x] Confirm the branch builds cleanly.

## PR 3 - feature/cli-skeleton
- [x] Replace the temporary `DynamicArray` demo in `main.cpp`.
- [x] Keep command-line parsing inside the professor `Parameters` library.
- [x] Add project help notes for the grade 3.0 scope.
- [x] Validate supported grade 3.0 algorithms, structures and data types.
- [x] Validate benchmark distribution values from `Parameters v1.2`.
- [x] Return clear errors for options outside the grade 3.0 scope.
- [x] Confirm the branch builds cleanly.
- [x] Smoke-test valid and invalid CLI examples.

## Next
- [ ] PR 4: add input/output file handling and custom ascending verification.
