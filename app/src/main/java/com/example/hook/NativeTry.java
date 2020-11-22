package com.example.hook;

public class NativeTry {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("inlinehook");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public static native void testTargetHook();

    public static native void testLibcHook();
}
