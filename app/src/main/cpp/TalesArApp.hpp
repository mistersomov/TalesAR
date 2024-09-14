#ifndef TALESAR_TALESARAPP_HPP
#define TALESAR_TALESARAPP_HPP

#include <android/asset_manager.h>

#include "ar/TalesArSession.hpp"

namespace talesar {

    class TalesArApp {

    public:
        explicit TalesArApp(
            AAssetManager *pAssetManager,
            JNIEnv *pEnv,
            void *pContext,
            void *pActivity
        );
        ~TalesArApp();

        void OnPause(JNIEnv *pEnv);
        void OnResume(JNIEnv *pEnv);

    private:
        AAssetManager *mAssetManager;
        TalesArSession *mSession;
    };

}

#endif //TALESAR_TALESARAPP_HPP