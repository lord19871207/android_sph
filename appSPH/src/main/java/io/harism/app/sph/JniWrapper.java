package io.harism.app.sph;

public class JniWrapper {

    static {
        System.loadLibrary("SPH");
    }

    public static native void onSurfaceCreated();

    public static native void onSurfaceChanged(int width, int height);

    public static native void onRenderFrame();

    public static native void onSurfaceReleased();

}
