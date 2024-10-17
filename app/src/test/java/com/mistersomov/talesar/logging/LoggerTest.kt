package com.mistersomov.talesar.logging

import org.junit.jupiter.api.AfterEach
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertTrue
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.api.Test
import timber.log.Timber

internal class LoggerTest {
    private val logMessages = mutableListOf<Pair<String, String>>()

    @BeforeEach
    fun setUp() {
        Timber.plant(object : Timber.Tree() {
            override fun log(priority: Int, tag: String?, message: String, t: Throwable?) {
                if (tag != null && message.isNotBlank()) {
                    logMessages.add(Pair(tag, message))
                }
            }
        })
    }

    @AfterEach
    fun teardown() {
        Timber.uprootAll()
        logMessages.clear()
    }

    @Test
    fun `getCaller returns correct caller`() {
        // action
        Logger.info("Test message")
        val (tag, message) = logMessages.first()

        // assert
        assertTrue {
            tag.contains("TalesARTag")
        }
        assertEquals("Test message", message)
    }

    @Test
    fun `getCaller skips Logger class entries and returns caller outside Logger`() {
        // mock
        fun sampleCaller() {
            Logger.error("Error message")
        }

        // action
        sampleCaller()

        // assert
        assertTrue(logMessages.isNotEmpty(), "No log messages were captured.")
        val (tag, message) = logMessages.first()
        assertEquals("Error message", message, "Logged message does not match.")
    }

    @Test
    fun `getCaller handles stack trace with no suitable caller by returning last element`() {
        // mock
        val thread = Thread.currentThread()

        // action
        val caller = Logger::class.java.getDeclaredMethod("getCaller", Thread::class.java)
            .apply { isAccessible = true }
            .invoke(Logger, thread) as StackTraceElement

        // assert
        assertTrue(caller is StackTraceElement, "Expected a StackTraceElement to be returned.")
    }

}