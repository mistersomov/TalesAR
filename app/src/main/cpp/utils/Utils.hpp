#ifndef TALESAR_UTILS_HPP
#define TALESAR_UTILS_HPP

#include <android/log.h>
#include <jni.h>

#ifndef LOGI
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, "TalesAR_TAG", __VA_ARGS__)
#endif  // LOGI

#ifndef LOGE
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "TalesAR_TAG", __VA_ARGS__)
#endif  // LOGE

#ifndef AR_CALL
#define AR_CALL(condition, env, msg, ...)                           \
    if (!(condition)) {                                             \
        LOGE("*** AR_CALL FAILED at %s:%d", __FILE__, __LINE__);    \
        talesar::utils::ThrowJavaException(env, msg);               \
    }
#endif

namespace talesar::utils {
    /** Throw a Java exception.
     @param pEnv the JNIEnv.
     @param msg the message of this exception.
     */
    void ThrowJavaException(JNIEnv *pEnv, const char *msg);
}

#endif //TALESAR_UTILS_HPP