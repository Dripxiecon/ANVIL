# ANVIL Technical Decisions

## Platform strategy

**Decision:** Android-first, cross-platform-ready.

The Galaxy A05 is the first reference device. Shared engine code must not depend on Android APIs. Android-specific lifecycle, input, storage and graphics-surface integration belong in platform adapters.

## Initial technology choices

- Android application layer: Kotlin.
- Native engine language: C++.
- Native build system: CMake.
- Initial mobile graphics API: OpenGL ES 3.0.
- Android minimum API: 26 (provisional; validate device coverage before release).
- Rendering backend access: through engine-facing interfaces so another backend can be introduced without rewriting feature systems.

These are foundation choices, not claims that every target device supports every future rendering feature at the desired quality.

## Performance policy

- The Galaxy A05 is the primary early profiling target.
- Aim for 30–60 FPS where scene complexity and hardware permit; never promise a fixed frame rate.
- Track frame time, memory use, thermal state and GPU resource lifetime.
- Prefer graceful quality reduction over crashes or uncontrolled memory growth.
- Expensive systems must have measurable budgets and lower-cost preview paths.

## Build policy

- Keep the Android app module thin.
- Keep native engine code separate from Android JNI and lifecycle code.
- Do not put feature implementation into the app entry point.
- Add tests with each engine subsystem.
- Record exact toolchain versions and update them deliberately.

## Current scope

The first scaffold proves that the Android app can create an OpenGL ES 3 context and call a small native bridge. It is not yet a modeling application, renderer, or production-ready engine.
