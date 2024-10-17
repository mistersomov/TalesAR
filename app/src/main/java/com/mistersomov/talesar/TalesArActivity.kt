package com.mistersomov.talesar

import android.os.Bundle
import android.view.SurfaceHolder
import android.view.View
import android.widget.Toast
import android.widget.Toast.LENGTH_SHORT
import com.google.androidgamesdk.GameActivity
import com.mistersomov.talesar.common.facade.JniFacade
import com.mistersomov.talesar.common.helper.CameraPermissionHelper.hasCameraPermission
import com.mistersomov.talesar.common.helper.CameraPermissionHelper.launchPermissionSettings
import com.mistersomov.talesar.common.helper.CameraPermissionHelper.requestCameraPermission
import com.mistersomov.talesar.common.helper.CameraPermissionHelper.shouldShowRequestPermissionRationale
import com.mistersomov.talesar.logging.Logger

class TalesArActivity : GameActivity() {
    companion object {
        private const val CAMERA_PERMISSION_DENIED_TEXT =
            "Camera permission is needed to run this application"
    }

    private var nativeApplication: Long = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Logger.register()
        try {
            nativeApplication = JniFacade.createNativeApp(
                assetManager = assets,
                context = applicationContext,
                activity = this,
            )
        } catch (e: Exception) {
            Logger.error(e)
            return
        }
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        super.surfaceCreated(holder)
        try {
            JniFacade.onSurfaceCreated(
                app = nativeApplication,
                surface = holder.surface,
            )
        } catch (e: Exception) {
            Logger.error(e)
            return
        }
    }

    override fun onPause() {
        super.onPause()
        try {
            JniFacade.onPause(app = nativeApplication)
        } catch (e: Exception) {
            Logger.error(e)
            return
        }
    }

    override fun onResume() {
        super.onResume()
        if (!hasCameraPermission()) {
            requestCameraPermission()
            return
        }
        try {
            JniFacade.onResume(app = nativeApplication)
        } catch (e: Exception) {
            Logger.error(e)
            return
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        synchronized(this) {
            JniFacade.destroyNativeApp(app = nativeApplication)
            nativeApplication = 0;
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray,
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (!hasCameraPermission()) {
            Toast.makeText(
                this,
                CAMERA_PERMISSION_DENIED_TEXT,
                LENGTH_SHORT
            ).show()
            if (!shouldShowRequestPermissionRationale()) {
                // Permission denied with checking "Do not ask again".
                launchPermissionSettings()
            }
            finish()
        }
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            hideSystemUi()
        }
    }

    private fun hideSystemUi() {
        val decorView = window.decorView
        decorView.systemUiVisibility = (View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_FULLSCREEN)
    }
}