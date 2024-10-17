#include "Exception.hpp"

namespace talesar::exception {
    void ThrowJavaException(JNIEnv *pEnv, const char *msg) {
        jclass exceptionClass = pEnv->FindClass("java/lang/RuntimeException");
        if (exceptionClass != NULL) {
            pEnv->ThrowNew(exceptionClass, msg);
        }
        pEnv->DeleteLocalRef(exceptionClass);
    }
}