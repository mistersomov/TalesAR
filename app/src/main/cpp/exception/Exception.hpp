#ifndef TALESAR_EXCEPTION_HPP
#define TALESAR_EXCEPTION_HPP

#include <jni.h>

#include "logging/Log.hpp"

namespace talesar::exception {
    /** Throw a Java exception.
     @param pEnv the JNIEnv.
     @param msg the message of this exception.
     */
    void ThrowJavaException(JNIEnv *pEnv, const char *msg);

    /** Throw a NoSuchElementException.
     @param pEnv the JNIEnv.
     @param msg the message of this exception.
     */
    void ThrowJavaNoSuchElementException(JNIEnv *pEnv, const char *msg);
}

#endif //TALESAR_EXCEPTION_HPP