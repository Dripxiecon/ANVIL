#include <jni.h>
#include <GLES3/gl3.h>

#include <algorithm>

namespace {

int surface_width = 1;
int surface_height = 1;

}  // namespace

extern "C" JNIEXPORT void JNICALL
Java_com_inker_anvil_NativeEngine_onSurfaceCreated(JNIEnv*, jobject) {
    glClearColor(0.035F, 0.045F, 0.065F, 1.0F);
    glDisable(GL_DEPTH_TEST);
}

extern "C" JNIEXPORT void JNICALL
Java_com_inker_anvil_NativeEngine_onSurfaceChanged(
    JNIEnv*,
    jobject,
    jint width,
    jint height
) {
    surface_width = std::max(1, static_cast<int>(width));
    surface_height = std::max(1, static_cast<int>(height));
    glViewport(0, 0, surface_width, surface_height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_inker_anvil_NativeEngine_renderFrame(JNIEnv*, jobject) {
    glClear(GL_COLOR_BUFFER_BIT);
}
