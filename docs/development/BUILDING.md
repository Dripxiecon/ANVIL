# Building the ANVIL Android foundation

## Required tools

- Android Studio with Android SDK Platform 36 installed.
- JDK 17.
- Android NDK installed through SDK Manager.
- CMake 3.22.1 installed through SDK Manager.
- Android Gradle Plugin 8.13.2 and Kotlin plugin 2.2.20 are declared by the project.

## Open and sync

1. Clone the repository.
2. Open the repository root in Android Studio.
3. Install the SDK, NDK and CMake versions requested by the project if Android Studio reports they are missing.
4. Allow Gradle sync to finish.
5. Select the `android` application configuration and build the debug APK.

The Gradle wrapper is not included yet. Use an Android Studio installation with a compatible Gradle distribution, or add and verify the wrapper as a separate foundation task before relying on command-line builds.

## Device smoke test

Install the debug APK on an OpenGL ES 3.0-capable Android device. The current test is intentionally small: the app should open to a dark clear-color surface without crashing. This confirms only the Android activity, EGL context, JNI boundary and native library load path.

## What this does not test

- Mesh creation or editing.
- Camera controls or editor UI.
- PBR materials, lighting or shadows.
- GPU memory budgets.
- Sustained frame rate or thermal stability.
- Cross-platform builds.

Those systems need their own modules, tests and profiling before they can be considered implemented.
