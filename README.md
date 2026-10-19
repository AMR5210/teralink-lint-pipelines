# TeraLink linting & static-analysis pipelines

Drop-in GitHub Actions pipelines for TeraLink Python and C/C++ repositories. The tool choices
and standards come from the Software Design Document, section 4.4.

| Pipeline | Workflow | Config it uses |
|---|---|---|
| Python | `.github/workflows/python-lint.yml` | `pyproject.toml`, `requirements-lint.txt` |
| C/C++ | `.github/workflows/cpp-lint.yml` | `.clang-format`, `.clang-tidy`, `Doxyfile` |

Every check is its own workflow step, and **every step runs even if an earlier one fails**, so a
single push shows all problems at once.

## Adopting a pipeline in another repo

**Python:** copy `python-lint.yml` and `requirements-lint.txt`, and merge the `[tool.*]` tables from
`pyproject.toml` into the repo's own. Edit `PY_PATHS` at the top of the workflow (for example `src tests`).

**C/C++:** copy `cpp-lint.yml`, `.clang-format`, `.clang-tidy` and `Doxyfile`. Edit `CPP_SRC_DIRS`
(hand-written source and headers) and `CMAKE_SOURCE_DIR` at the top of the workflow.

Lines to edit are marked `EDIT PER REPOSITORY`. Optionally copy `.gitattributes` to keep LF line endings.
Then mark the jobs as required checks in branch protection.

**F Prime repos:** replace the workflow's "Configure" step with
`fprime-util generate -DCMAKE_EXPORT_COMPILE_COMMANDS=ON` and point `BUILD_DIR` at the build cache it
creates. Keep `CPP_SRC_DIRS` to hand-written code only. The `.clang-tidy` profile is much stricter
than F Prime's own, so it will flag autocoded `*Ac.cpp` files and the `fprime/` submodule.

## What each pipeline checks

**Python** (matrix: 3.10 and 3.13)

| Step | Tool | Enforces |
|---|---|---|
| Black | Black | formatting |
| Ruff | Ruff | PEP 8, anti-patterns, PEP 257 docstring presence, type-hint presence, complexity |
| Mypy | Mypy (`strict`) | static typing |
| Pydoclint | Pydoclint | docstrings match function signatures (Args/Returns/Raises) |
| Bandit | Bandit | security issues |

**C/C++** (Ubuntu 24.04, LLVM 18, C11 / C++17)

| Step | Tool | Enforces |
|---|---|---|
| ClangFormat | clang-format | formatting (same style as F Prime) |
| Build | gcc via CMake | zero warnings under `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror` |
| Clang-Tidy | clang-tidy | bugprone, CERT, HIC++, C++ Core Guidelines, static analyzer (see `.clang-tidy`) |
| Doxygen | doxygen | every file has an `@file` block and every public entity is documented; HTML uploaded as an artifact |

## Demo

`examples/` holds a `clean` and a `dirty` sample for each language. The `dirty` code is deliberately bad.
The workflows default to the `clean` code, so they pass out of the box.

To see each pipeline fail: **Actions tab, select the workflow, Run workflow**, and set the input
(Python: `paths` = `examples/python/dirty`; C/C++: `src-dirs` = `examples/cpp/dirty/src` and
`cmake-source-dir` = `examples/cpp/dirty`). Alternatively, open a PR that adds bad code.

Expected result on the `dirty` samples (all verified):

| Step | Result | Example findings |
|---|---|---|
| Black / ClangFormat | fail | quotes, spacing, brace placement |
| Ruff | fail, 27 findings | unused imports, bare `except`, mutable default arg, missing docstrings and annotations, `print` |
| Mypy | fail, 5 findings | wrong return type, wrong argument type, untyped defs |
| Pydoclint | fail | docstring lists `verbose`, omits `strict` and `cache`, no Returns section |
| Bandit | fail | MD5, `pickle`, `eval`, `shell=True`, hard-coded password |
| Build | fail | `-Wconversion`, unused variable |
| Clang-Tidy | fail, 17 findings | recursion, leaks, uninitialised read, C-style cast, `malloc`, magic numbers |
| Doxygen | fail | five undocumented functions (and a file with no `@file` block) |

## Known gaps

- **MISRA C++:2023 is not enforced.** The SDD names Parasoft for that and Coverity for deeper analysis.
  Both are commercial and need licences. `.clang-tidy` approximates MISRA and JPL intent with open-source
  checks and does not certify compliance. A Parasoft or Coverity step can be added once licences exist.
- **No test or coverage step.** The SDD's 80% target (`pytest-cov`, `gcovr`) is a separate task.
- **Tool versions are pinned** (`requirements-lint.txt`, `LLVM_VERSION`, action commit SHAs). Bump them
  deliberately, or enable Dependabot for the actions and pip pins.
- Third-party Python packages without type stubs need a per-module `ignore_missing_imports` override
  (example in `pyproject.toml`).
