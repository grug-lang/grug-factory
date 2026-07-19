# grugtorio

TODO: Add intro

## Running

```sh
cmake -B build
cmake --build build
build/grugtorio
```

### grugtorio options

| Option                                 | Description                                 |
| -------------------------------------- | ------------------------------------------- |
| `--input-save [.grugtorio.json path]`  | Path to load a grugtorio save from.         |
| `--output-save [.grugtorio.json path]` | Path to write a grugtorio save to on exit.  |
| `--ticks [N]`                          | Number of ticks to simulate before exiting. |

### Running with AddressSanitizer and UBSan

```sh
cmake -B build -DENABLE_ASAN=ON
```

If you get leaks from Raylib then you should set this environment variable before configuring:

```sh
export LSAN_OPTIONS="suppressions=lsan.supp:print_suppressions=0"
```

### Running with valgrind

```sh
valgrind --leak-check=full build/grugtorio
```

## Testing

To run the test suite, execute:

```bash
python tests.py
```

You can also run tests with Valgrind by setting the `TEST_RUNNER` environment variable:

```bash
TEST_RUNNER="valgrind --leak-check=full" python tests.py
```

The `tests` directory its tests contain `TODO.md` files whenever their behavior is known to not match Factorio _yet_.
