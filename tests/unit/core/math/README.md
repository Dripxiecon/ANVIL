# Core Math Tests

These tests are intentionally standalone so the platform-independent math layer can be checked without an Android SDK or an emulator.

## Run

From the repository root:

```sh
cmake -S tests/unit/core/math -B build/math-tests
cmake --build build/math-tests --config Debug
ctest --test-dir build/math-tests --output-on-failure
```

A C++20 compiler and CMake 3.22 or newer are required.

## Coverage

- `test_vector3`: arithmetic, dot product, cross product and normalization, including the zero vector.
- `test_quaternion`: axis-angle construction and vector rotation.
- `test_transform`: point transformation and inverse transformation with translation, rotation and non-uniform scale.
- `test_matrix4`: column-major matrix layout, transform composition and point/direction conversion.

The tests use assertions and return a non-zero exit code if a check fails. They have not yet been run in CI or on the target Android device.
