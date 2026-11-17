# ANVIL

A performance-conscious 3D modeling, animation and rendering application.

## Current status

The repository now contains the initial Android-first foundation scaffold:

- Kotlin Android application entry point.
- OpenGL ES 3.0 surface creation.
- C++ native bridge built with CMake.
- Architecture and technical decision documents.

This is a foundation target, not yet a modeling application or a feature-complete renderer.

## Platform strategy

ANVIL is Android-first and cross-platform-ready. Shared engine systems must remain independent of Android APIs. Android lifecycle and platform integration stay in `platform/android`.

## Build

Open the repository in a compatible Android Studio installation with Android SDK, NDK and CMake installed. The Android application module is `app/android`.

The project currently expects JDK 17, Android Gradle Plugin 8.13.2, Kotlin 2.2.20, Android SDK 36 and CMake 3.22.1. A Gradle wrapper has not yet been committed.

## Performance target

The Galaxy A05 is the primary early test device. The engineering goal is 30–60 FPS where workload and hardware allow, with stability and predictable memory use taking priority over visual effects.
