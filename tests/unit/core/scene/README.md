# Core Scene Tests

Run from the repository root:

```sh
cmake -S tests/unit/core/scene -B build/scene-tests
cmake --build build/scene-tests --config Debug
ctest --test-dir build/scene-tests --output-on-failure
```

Coverage includes object creation and lookup, parent/child hierarchy changes, cycle rejection, collection membership, and cleanup when objects are removed.

These tests have not yet been run in CI or on an Android device.
