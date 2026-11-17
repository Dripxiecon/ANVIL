package com.inker.anvil

import android.app.Activity
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.view.ViewGroup
import android.widget.FrameLayout
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : Activity() {
    private lateinit var surfaceView: GLSurfaceView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        surfaceView = GLSurfaceView(this).apply {
            setEGLContextClientVersion(3)
            preserveEGLContextOnPause = false
            setRenderer(AnvilSurfaceRenderer())
            renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        }

        setContentView(
            FrameLayout(this).apply {
                addView(
                    surfaceView,
                    FrameLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT
                    )
                )
            }
        )
    }

    override fun onResume() {
        super.onResume()
        if (::surfaceView.isInitialized) surfaceView.onResume()
    }

    override fun onPause() {
        if (::surfaceView.isInitialized) surfaceView.onPause()
        super.onPause()
    }

    private class AnvilSurfaceRenderer : GLSurfaceView.Renderer {
        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            NativeEngine.onSurfaceCreated()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            NativeEngine.onSurfaceChanged(width, height)
        }

        override fun onDrawFrame(gl: GL10?) {
            NativeEngine.renderFrame()
        }
    }
}
