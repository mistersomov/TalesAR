package com.mistersomov.talesar.common.helper

import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager.PERMISSION_GRANTED
import android.net.Uri
import android.provider.Settings.ACTION_APPLICATION_DETAILS_SETTINGS
import androidx.core.content.ContextCompat

object CameraPermissionHelper {
    private const val CAMERA_PERMISSION = android.Manifest.permission.CAMERA
    private const val CAMERA_PERMISSION_REQUEST_CODE = 0
    private const val PACKAGE = "package"

    /** Check to see we have the necessary permissions for this app. */
    fun Activity.hasCameraPermission(): Boolean =
        ContextCompat.checkSelfPermission(this, CAMERA_PERMISSION) == PERMISSION_GRANTED;

    /** Check to see we have the necessary permissions for this app, and ask for them if we don't. */
    fun Activity.requestCameraPermission() {
        requestPermissions(arrayOf(CAMERA_PERMISSION), CAMERA_PERMISSION_REQUEST_CODE)
    }

    /** Check to see if we need to show the rationale for this permission. */
    fun Activity.shouldShowRequestPermissionRationale(): Boolean =
        shouldShowRequestPermissionRationale(CAMERA_PERMISSION)

    /** Launch Application Setting to grant permission. */
    fun Activity.launchPermissionSettings() {
        val intent = Intent(ACTION_APPLICATION_DETAILS_SETTINGS)
        intent.data = Uri.fromParts(PACKAGE, packageName, null)
        startActivity(intent)
    }
}