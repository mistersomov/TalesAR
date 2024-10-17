package com.mistersomov.talesar.logging

import com.google.androidgamesdk.BuildConfig
import timber.log.Timber
import timber.log.Timber.DebugTree

object Logger {
    private const val TAG = "TalesARTag"
    private var isRegistered = false

    fun register() {
        if (!isRegistered && BuildConfig.DEBUG) {
            Timber.plant(DebugTree())
            isRegistered = true
        }
    }

    fun info(message: String) {
        Timber
            .tag(makeTag())
            .i(message = message)
    }

    fun error(message: String) {
        Timber
            .tag(makeTag())
            .e(message = message)
    }

    fun error(throwable: Throwable) {
        Timber
            .tag(makeTag())
            .e(t = throwable)
    }

    private fun makeTag(): String =
        getCaller(Thread.currentThread()).let {
            "$TAG (${it.fileName}:${it.lineNumber})"
        }

    private fun getCaller(thread: Thread): StackTraceElement {
        val className = Logger::class.java.name

        return thread.stackTrace.firstOrNull {
            !it.className.startsWith(className, ignoreCase = true)
        } ?: thread.stackTrace.lastOrNull() ?: throw IllegalStateException("Unable to determine caller")
    }
}