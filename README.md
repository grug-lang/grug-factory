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
valgrind build/grugtorio
```
