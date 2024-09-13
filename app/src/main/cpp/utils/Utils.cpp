#include "Utils.hpp"

namespace talesar::utils {

    void ThrowJavaException(JNIEnv *pEnv, const char *msg) {
        LOGE("Throw Java exception: %s", msg);
        jclass exceptionClass = pEnv->FindClass("java/lang/RuntimeException");
        if (!exceptionClass) {
            pEnv->ThrowNew(exceptionClass, msg);
        }
        pEnv->DeleteLocalRef(exceptionClass);
    }
}