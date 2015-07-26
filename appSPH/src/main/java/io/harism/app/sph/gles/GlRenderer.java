package io.harism.app.sph.gles;

public interface GlRenderer {

    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onRenderFrame();

    void onSurfaceReleased();

}
