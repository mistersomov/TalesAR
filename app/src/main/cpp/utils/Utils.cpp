#include "Utils.hpp"

namespace talesar::utils {

    void ThrowJavaException(JNIEnv *env, const char *msg) {
        LOGE("Throw Java exception: %s", msg);
        jclass exceptionClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClass, msg);
    }
}