package com.mistersomov.talesar.common.facade

import android.app.Activity
import android.content.Context
import android.content.res.AssetManager
import dalvik.annotation.optimization.FastNative

class JniFacade {
    companion object {
        private const val LIB_NAME = "talesar"

        init {
            System.loadLibrary(LIB_NAME)
        }

        @JvmStatic
        @FastNative
        external fun createNativeApp(assetManager: AssetManager): Long

        @JvmStatic
        @FastNative
        external fun destroyNativeApp(app: Long)

        @JvmStatic
        @FastNative
        external fun onPause(app: Long)

        @JvmStatic
        @FastNative
        external fun onResume(app: Long, context: Context, activity: Activity)
    }
}