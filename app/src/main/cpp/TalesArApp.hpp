#ifndef TALESAR_TALESARAPP_HPP
#define TALESAR_TALESARAPP_HPP

#include "ar/TalesArSession.hpp"
#include "engine/TalesArEngine.hpp"

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
        TalesArEngine *mEngine;
    };

}

#endif //TALESAR_TALESARAPP_HPP