# grugtorio

TODO: Add intro

## Running

```sh
cmake -B build
cmake --build build
build/grugtorio
```

### Running with AddressSanitizer and UBSan

```sh
cmake -B build -DENABLE_ASAN=ON
```

If you get leaks from Raylib then you should set this environment variable before configuring:

```sh
export LSAN_OPTIONS="suppressions=lsan.supp:print_suppressions=0"
```
