package com.mistersomov.talesar.common.facade

import android.app.Activity
import android.content.Context
import android.content.res.AssetManager

class JniFacade {
    companion object {
        private const val LIB_NAME = "talesar"

        init {
            System.loadLibrary(LIB_NAME)
        }

        @JvmStatic
        external fun createNativeApp(
            assetManager: AssetManager,
            context: Context,
            activity: Activity,
        ): Long

        @JvmStatic
        external fun destroyNativeApp(app: Long)

        @JvmStatic
        external fun onPause(app: Long)

        @JvmStatic
        external fun onResume(app: Long)
    }
}