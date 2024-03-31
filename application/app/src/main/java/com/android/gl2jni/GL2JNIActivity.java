/*
package com.android.gl2jni;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class GL2JNILib {
    static { System.loadLibrary("gl2jni");}
    static native void init(int width, int height);
    static native void step();
}

class GL2JNIView extends GLSurfaceView {

    public GL2JNIView(Context context) {
        super(context);
        setEGLContextFactory(new ContextFactory());
        setRenderer(new Renderer());
    }
    static class ContextFactory implements GLSurfaceView.EGLContextFactory {
        int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        @Override
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            return context;
        }

        @Override
        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }
    static class Renderer implements GLSurfaceView.Renderer {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GL2JNILib.init(width, height);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            GL2JNILib.init(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            GL2JNILib.step();
        }
    }
}

public class GL2JNIActivity extends Activity {
    GL2JNIView mView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new GL2JNIView(getApplication());
        setContentView(mView);
    }
}
*/

package com.android.gl2jni;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class GL2JNILib {
    static {System.loadLibrary("gl2jni");}
    static native void setup(int width, int height);
    static native void tick();
}

class GL2JNIView extends GLSurfaceView {

    public GL2JNIView(Context context) {
        super(context);
        setEGLContextFactory(new ContextFactory());
        setRenderer(new Renderer());
    }
    private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
        @Override
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            int[] attrib_list = {0x3098, 2, EGL10.EGL_NONE };
            return egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
        }
        @Override
        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }
    static class Renderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {}
        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) { GL2JNILib.setup(width,height); }
        @Override
        public void onDrawFrame(GL10 gl) {
            GL2JNILib.tick();
        }

    }
}

public class GL2JNIActivity extends Activity {
    GL2JNIView mView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new GL2JNIView(getApplication());
        setContentView(mView);
    }
}