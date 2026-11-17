# ANVIL Project Structure

This document defines the initial modular layout and the rules every contribution must follow. It is a target structure, not a claim that every module already exists.

## Repository layout

```text
ANVIL/
├── app/
│   └── android/                 # Android application entry point and app lifecycle
├── core/
│   ├── project/                 # Project model, project settings and project lifecycle
│   ├── scene/                   # Scene graph, objects and collections
│   ├── math/                    # Shared vectors, matrices, transforms and geometry math
│   ├── assets/                  # Asset identities, metadata and asset references
│   ├── serialization/           # Versioned project save/load formats
│   └── events/                  # Typed events and message contracts
├── editor/
│   ├── workspace/               # Workspace layout and workspace state
│   ├── viewport/                # Viewport interaction and camera controls
│   ├── panels/                  # Independent editor panels
│   ├── tools/                   # Modeling and editing tool orchestration
│   ├── gizmos/                  # Transform and editing gizmos
│   └── command_menu/            # Searchable Surgical Command Menu
├── engine/
│   ├── geometry/                # Mesh data, topology and mesh operations
│   ├── rendering/               # Render graph, frame scheduling and GPU resources
│   ├── materials/               # PBR material model and texture channels
│   ├── nodes/                   # Node graph model, evaluation and compilation
│   ├── depth/                   # ANVIL DEPTH and PBR Depth Fusion
│   ├── sculpting/               # INKSCULPT and adaptive local detail
│   ├── rigging/                 # Armatures, bones and skinning
│   ├── animation/               # Timeline, keyframes, curves and playback
│   ├── bone_physics/            # Wiggle Bones simulation and baking
│   ├── collision/               # Collision shapes, detection and response
│   └── simulation/
│       └── inkflow/             # INKFLOW liquid systems and caches
├── platform/
│   └── android/                 # Android-specific input, storage and device services
├── assets/
│   ├── materials/
│   ├── icons/
│   └── test_scenes/
├── tests/
│   ├── unit/
│   ├── integration/
│   └── performance/
└── docs/
    ├── architecture/
    └── development/
```

Platform-specific implementations must stay behind clear interfaces so the core engine is not tied to Android APIs. Additional platform targets can be added without moving feature logic into the app entry point.

## Non-negotiable architecture rules

1. **One responsibility per file.** A file should have one clear reason to change. Split large classes and files before they become difficult to navigate.
2. **Feature ownership.** Each feature lives in its own module and folder. Feature-specific models, systems, UI, tests and documentation stay together or in clearly named subfolders.
3. **No god files or god classes.** App entry points coordinate startup only. They must not contain renderer, geometry, simulation, editor or business logic.
4. **Explicit module boundaries.** Modules communicate through documented interfaces and typed data contracts. Avoid hidden cross-module dependencies and circular imports.
5. **Core stays platform-independent.** Android APIs belong in platform adapters, not in shared geometry, scene, material or simulation logic.
6. **Tests follow the feature.** Every substantial module gets unit tests; interactions between modules get integration tests; performance-sensitive systems get benchmarks.
7. **Document public contracts.** Explain module responsibilities, important data formats, threading assumptions and ownership of GPU/CPU resources.
8. **No dumping ground folders.** Avoid vague folders such as `misc`, `stuff` or `helpers`. Shared utilities must have a specific, justified owner.
9. **Keep files reviewable.** Prefer small, cohesive files. If a file grows into multiple responsibilities, split it. Do not split code into meaningless one-line fragments just to chase a line-count target.
10. **Measure performance.** Expensive work must be profiled on representative hardware. Do not add background threads, caches or GPU work without clear ownership and lifecycle rules.

## Dependency direction

- `app` and editor UI may depend on stable core and engine interfaces.
- Feature modules may depend on shared core contracts and explicitly approved lower-level engine modules.
- `core` must not depend on editor UI or Android application code.
- Platform adapters implement interfaces; shared feature logic must not call platform APIs directly.
- Avoid circular dependencies. If two modules need each other, move their shared contract to a lower-level module.

## Change checklist

Before merging a feature, verify:
- Its code is in the correct feature module.
- No unrelated feature logic was added to the app entry point.
- Public interfaces and dependencies are clear.
- Tests and performance checks cover the new behaviour.
- Documentation is updated when module responsibilities or data contracts change.
