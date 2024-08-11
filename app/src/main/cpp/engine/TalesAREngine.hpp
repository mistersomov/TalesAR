#ifndef TALESAR_TALESARENGINE_HPP
#define TALESAR_TALESARENGINE_HPP

#include "logging/AndroidLog.h"

#include <android/asset_manager.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <optional>

namespace talesar::engine {
    class TalesAREngine {
    public:
        explicit TalesAREngine(AAssetManager* assetManager);
        ~TalesAREngine();

        void Init();

    private:
        void CreateInstance();

    private:
        AAssetManager* mAssetManager;
        VkInstance mInstance;
    };
}

#endif