# Mesh Data Foundation

This module stores the basic topology of a polygon mesh.

- Vertices store local-space positions.
- Edges reference two distinct vertices.
- Faces store an ordered boundary of three or more unique vertex indices.
- MeshData owns the arrays and validates references before accepting new topology.

Indices are currently compact array indices. They are not persistent IDs and can change if future editing operations reorder or compact the arrays. Stable element handles, half-edge adjacency, normals, UVs and editing operations are planned as later geometry work.

The current implementation deliberately rejects invalid references and duplicate vertex indices in a face. It does not yet guarantee manifold topology or automatically create edges when a face is added.
