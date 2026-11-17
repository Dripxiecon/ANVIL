# Mesh Data Foundation

This module stores the basic topology of a polygon mesh.

- Vertices store local-space positions and stable 64-bit IDs.
- Edges reference two distinct vertex IDs and have their own stable IDs.
- Faces store an ordered boundary of three or more unique vertex IDs and have their own stable IDs.
- MeshData owns compact storage arrays and ID-to-slot lookup tables.
- Vertex positions can be updated without changing their IDs.

IDs are not array offsets. Adding more elements or allowing the vectors to reallocate does not change an existing element's ID. IDs are not reused after `clear()`, so a stale handle cannot silently point to a newly-created element in the same MeshData instance.

The current implementation rejects invalid references and duplicate vertex IDs in a face. It supports edge splitting, polygon face extrusion, and a first centroid-based inset operation. Extrusion translates a face along a caller-provided offset; inset moves each boundary vertex toward the face centroid by a fraction between 0 and 1. This inset is intentionally a simple proportional inset, not yet a constant-distance offset suitable for every concave or non-planar polygon.\n\nThe mesh does not yet guarantee manifold topology, automatically create boundary edges when a face is added, or support deletion/compaction. Editing operations currently assume valid input topology and are not yet fully transactional if memory allocation throws. Those guarantees need explicit topology and rollback rules before more destructive editing operations are added.

## Tests

Run from the repository root:

```sh
cmake -S tests/unit/engine/geometry/mesh -B build/mesh-tests
cmake --build build/mesh-tests --config Debug
ctest --test-dir build/mesh-tests --output-on-failure
```

The repository-wide workflow also builds and runs the math, scene and mesh test targets.
