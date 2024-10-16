#include "TalesArApp.hpp"

namespace talesar {

    TalesArApp::TalesArApp(
        AAssetManager *pAssetManager,
        JNIEnv *pEnv,
        void *pContext,
        void *pActivity
    ) : mAssetManager{pAssetManager} {
        mSession.reset(TalesArSession::GetInstance());
        mSession->OnCreate(pEnv, pContext, pActivity);
    }

    TalesArApp::~TalesArApp() {

    }

    void TalesArApp::OnSurfaceCreated(JNIEnv *pEnv, jobject surface) {
        if (mAssetManager) {
            mEngine.reset(new TalesArEngine(pEnv, mAssetManager, surface));
        }
    }

    void TalesArApp::OnPause(JNIEnv *pEnv) {
        if (mSession) {
            mSession->OnPause(pEnv);
        }
    }

    void TalesArApp::OnResume(JNIEnv* pEnv) {
        if (mSession) {
            mSession->OnResume(pEnv);
        }
    }

}