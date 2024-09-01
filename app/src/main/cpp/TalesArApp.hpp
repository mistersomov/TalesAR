#ifndef TALESAR_TALESARAPP_HPP
#define TALESAR_TALESARAPP_HPP

#include <jni.h>
#include <android/asset_manager.h>

namespace talesar {

    class TalesArApp {

    public:
        explicit TalesArApp(AAssetManager *pAssetManager);
        ~TalesArApp();

    private:
        AAssetManager *mAssetManager;
    };

}

#endif //TALESAR_TALESARAPP_HPP