package com.inker.anvil

internal object NativeEngine {
    init {
        System.loadLibrary("anvil_android")
    }

    external fun onSurfaceCreated()
    external fun onSurfaceChanged(width: Int, height: Int)
    external fun renderFrame()
}
