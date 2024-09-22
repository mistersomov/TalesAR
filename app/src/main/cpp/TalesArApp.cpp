#include "TalesArApp.hpp"

namespace talesar {

    TalesArApp::TalesArApp(
        AAssetManager *pAssetManager,
        JNIEnv *pEnv,
        void *pContext,
        void *pActivity
    ) : mAssetManager{pAssetManager} {
        mSession = TalesArSession::GetInstance();
        mSession->OnCreate(pEnv, pContext, pActivity);
        mEngine = new TalesArEngine(pAssetManager);
    }

    TalesArApp::~TalesArApp() {
        delete mSession;
    }

    void TalesArApp::OnPause(JNIEnv *pEnv) {
        if (mSession != nullptr) {
            mSession->OnPause(pEnv);
        }
    }

    void TalesArApp::OnResume(JNIEnv* pEnv) {
        if (mSession != nullptr) {
            mSession->OnResume(pEnv);
        }
    }

}