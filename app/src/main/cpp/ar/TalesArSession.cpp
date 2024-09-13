#include "TalesArSession.hpp"

namespace talesar {

    TalesArSession* TalesArSession::mInstance = nullptr;

    TalesArSession::~TalesArSession() {
        ArSession_destroy(mSession);
    }

    TalesArSession *TalesArSession::GetInstance() {
        if (mInstance == nullptr) {
            mInstance = new TalesArSession();
        }
        return mInstance;
    }

    void TalesArSession::OnCreate(JNIEnv *pEnv, void *pContext, void *pActivity) {
        IsArCoreSupportedAndUpToDate(pEnv, pContext, pActivity);
        AR_CALL(
            ArSession_create(pEnv, pContext, &mSession) == AR_SUCCESS,
            pEnv,
            "Failed to create ArSession"
        )

        ArConfig* config;
        ArConfig_create(mSession, &config);

        AR_CALL(ArSession_configure(mSession, config) == AR_SUCCESS, pEnv, "Failed to configure ArSession")

        ArConfig_destroy(config);
    }

    void TalesArSession::OnPause(JNIEnv *pEnv) {
        AR_CALL(ArSession_pause(mSession) == AR_SUCCESS, pEnv, "Failed to pause ArSession")
    }

    void TalesArSession::OnResume(JNIEnv *pEnv, void *pContext) {
        AR_CALL(
            ArSession_resume(mSession) == AR_SUCCESS,
            pEnv,
            "Failed to resume ArSession"
        )
    }

    size_t TalesArSession::IsArCoreSupportedAndUpToDate(JNIEnv *pEnv, void *pContext, void *pActivity) {
        ArAvailability availability;
        ArCoreApk_checkAvailability(pEnv, pContext, &availability);

        switch(availability) {
            case AR_AVAILABILITY_SUPPORTED_INSTALLED:
                return true;
            case AR_AVAILABILITY_SUPPORTED_APK_TOO_OLD:
            case AR_AVAILABILITY_SUPPORTED_NOT_INSTALLED:
                ArInstallStatus status;
                AR_CALL(
                    ArCoreApk_requestInstall(pEnv, pActivity, true, &status) == AR_SUCCESS,
                    pEnv,
                    "Failed to install ARCore"
                )
                return false;
            default:
                return false;
        }
    }

}