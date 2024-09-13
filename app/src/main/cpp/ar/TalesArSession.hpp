#ifndef TALESAR_TALESARSESSION_HPP
#define TALESAR_TALESARSESSION_HPP

#include <arcore_c_api.h>
#include <cstdint>
#include "utils/Utils.hpp"

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
        void OnResume(JNIEnv *pEnv, void *pContext);

    protected:
        TalesArSession() = default;

    private:
        static size_t IsArCoreSupportedAndUpToDate(JNIEnv *pEnv, void *pContext, void *pActivity);

        static TalesArSession *mInstance;
        ArSession *mSession{nullptr};
    };
}

#endif //TALESAR_TALESARSESSION_HPP