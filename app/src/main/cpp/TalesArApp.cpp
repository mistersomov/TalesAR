#include "TalesArApp.hpp"

namespace talesar {

    TalesArApp::TalesArApp(AAssetManager *pAssetManager) : mAssetManager{pAssetManager},
                                                           mSession{nullptr}
    {

    }

    TalesArApp::~TalesArApp() {
        delete mSession;
    }

    void TalesArApp::OnPause(JNIEnv *pEnv) {
        if (mSession != nullptr) {
            mSession->OnPause(pEnv);
        }
    }

    void TalesArApp::OnResume(JNIEnv* pEnv, void* pContext, void* pActivity) {
        if (mSession == nullptr) {
            mSession = TalesArSession::GetInstance();
            mSession->OnCreate(pEnv, pContext, pActivity);
        }
        mSession->OnResume(pEnv, pContext);
    }

}