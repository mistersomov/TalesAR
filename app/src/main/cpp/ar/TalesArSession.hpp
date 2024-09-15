#ifndef TALESAR_TALESARSESSION_HPP
#define TALESAR_TALESARSESSION_HPP

#include <arcore_c_api.h>
#include <cstdint>

#include "exception/Exception.hpp"

#ifndef AR_CALL
#define AR_CALL(condition, env, msg, ...)                           \
    if (!(condition)) {                                             \
        LOGE("*** AR_CALL FAILED at %s:%d", __FILE__, __LINE__);    \
        talesar::exception::ThrowJavaException(env, msg);           \
    }
#endif

namespace talesar {
    /**
     * A wrapper class for ArSession.
     * This class is a singleton.
     * It should be used to initialize and destroy ArSession.
     */
    class TalesArSession {
    public:
        ~TalesArSession();

        static TalesArSession *GetInstance();

        void OnCreate(JNIEnv *pEnv, void *pContext, void *pActivity);
        void OnPause(JNIEnv *pEnv);
        void OnResume(JNIEnv *pEnv);

    protected:
        TalesArSession() = default;

    private:
        static size_t IsArCoreSupportedAndUpToDate(JNIEnv *pEnv, void *pContext, void *pActivity);

        void SetCameraConfig(JNIEnv *pEnv) const;

        static TalesArSession *mInstance;
        ArSession *mSession{nullptr};
    };
}

#endif //TALESAR_TALESARSESSION_HPP