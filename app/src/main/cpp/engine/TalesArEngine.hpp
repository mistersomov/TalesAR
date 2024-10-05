#ifndef TALESAR_TALESARENGINE_HPP
#define TALESAR_TALESARENGINE_HPP

#include <android/asset_manager.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <optional>
#include <cassert>

#include "exception/Exception.hpp"

#define VK_CALL(func)                                                             \
    do {                                                                          \
        VkResult status = (func);                                                 \
        if (VK_SUCCESS != status) {                                               \
          LOGE("==== Vulkan error %d. File[%s], line[%d]", status,                \
                              __FILE__, __LINE__);                                \
          assert(false);                                                          \
        }                                                                         \
    } while(0)

namespace talesar {
    class TalesArEngine {
    public:
        explicit TalesArEngine(JNIEnv *pEnv, AAssetManager* pAssetManager);
        ~TalesArEngine();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData
        );

    private:
        bool ValidationLayersSupported();
        std::vector<const char*> GetRequiredExtensions();

        void CreateDebugMessenger();
        void DestroyDebugMessenger();

        void CreateInstance();
        void CreatePhysicalDevice();

    private:
        JNIEnv* mJniEnv;
        AAssetManager* mAssetManager;

        VkDebugUtilsMessengerEXT mDebugMessenger;

        VkInstance mInstance;
        VkPhysicalDevice mPhysicalDevice{VK_NULL_HANDLE};
    };
}

#endif