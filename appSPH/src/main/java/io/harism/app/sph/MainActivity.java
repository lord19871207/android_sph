package io.harism.app.sph;

import android.app.Activity;
import android.opengl.GLES30;
import android.os.Bundle;
import android.view.Choreographer;

import io.harism.app.sph.gles.EglCore;
import io.harism.app.sph.gles.GlRenderer;
import io.harism.app.sph.gles.GlSurfaceView;

public class MainActivity extends Activity {

    private final Choreographer.FrameCallback frameCallback = new Choreographer.FrameCallback() {
        @Override
        public void doFrame(long frameTimeNanos) {
            glSurfaceView.renderFrame(frameTimeNanos);
            choreographer.postFrameCallback(this);
        }
    };

    private Choreographer choreographer;
    private GlSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        choreographer = Choreographer.getInstance();

        glSurfaceView = new GlSurfaceView(this);
        glSurfaceView.setEglContext(EglCore.VERSION_GLES3, EglCore.FLAG_DEPTH_BUFFER);
        glSurfaceView.setGlRenderer(new Renderer());

        setContentView(glSurfaceView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        choreographer.postFrameCallback(frameCallback);
    }

    @Override
    protected void onPause() {
        super.onPause();
        choreographer.removeFrameCallback(frameCallback);
    }

    private class Renderer implements GlRenderer {
        @Override
        public void onRenderFrame() {
            JniWrapper.onRenderFrame();
        }

        @Override
        public void onSurfaceCreated() {
            JniWrapper.onSurfaceCreated();
        }

        @Override
        public void onSurfaceChanged(int width, int height) {
            JniWrapper.onSurfaceChanged(width, height);
        }

        @Override
        public void onSurfaceReleased() {
            JniWrapper.onSurfaceReleased();
        }
    }
}
