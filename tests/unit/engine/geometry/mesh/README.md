# Mesh Data and Editing Tests

Run from the repository root:

```sh
cmake -S tests/unit/engine/geometry/mesh -B build/mesh-tests
cmake --build build/mesh-tests --config Debug
ctest --test-dir build/mesh-tests --output-on-failure
```

- `test_mesh_data` covers stable IDs, lookup, vertex position changes, validation and clear semantics.
- `test_mesh_editing` covers edge splitting and insertion of the new vertex into adjacent face boundaries in both winding directions.

These checks are also included in the repository-wide GitHub Actions workflow.
