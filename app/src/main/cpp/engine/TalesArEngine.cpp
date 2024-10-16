#include "TalesArEngine.hpp"

#ifndef NDEBUG
bool VALIDATION_LAYERS_ENABLED = true;
#else
bool VALIDATION_LAYERS_ENABLED = false;
#endif

namespace talesar {
    const std::string APP_NAME = "TalesAR";
    const std::string ENGINE_NAME = "TalesAREngine_Vulkan";
    const std::vector<const char*> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};

    TalesArEngine::TalesArEngine(
        JNIEnv *pEnv,
        AAssetManager* pAssetManager,
        jobject surface
    ) : mJniEnv{pEnv},
        mAssetManager{pAssetManager},
        mWindowWrapper{pEnv, surface}
    {
        CreateInstance();
        CreateDebugMessenger();
        CreateSurface();
        CreatePhysicalDevice();
        SetQueueFamilyIndex();
        CreateLogicalDevice();
    }

    TalesArEngine::~TalesArEngine() {
        vkDestroyDevice(mLogicalDevice, nullptr);
        if (VALIDATION_LAYERS_ENABLED) {
            DestroyDebugMessenger();
        }
        vkDestroyInstance(mInstance, nullptr);
    }

    bool TalesArEngine::ValidationLayersSupported() {
        bool layerFound = false;
        uint32_t layerCount = 0;

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layer : VALIDATION_LAYERS) {
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layer, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
        }

        return layerFound;
    }

    std::vector<const char*> TalesArEngine::GetRequiredExtensions() {
        std::vector<const char*> extensions;

        extensions.emplace_back("VK_KHR_surface");
        extensions.emplace_back("VK_KHR_android_surface");
        extensions.emplace_back("VK_KHR_get_physical_device_properties2");

        if (VALIDATION_LAYERS_ENABLED) {
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    VkBool32 TalesArEngine::DebugMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData
    ) {
        const char validation[] = "Validation";
        const char performance[] = "Performance";
        const char error[] = "ERROR";
        const char warning[] = "WARNING";
        const char unknownType[] = "UNKNOWN_TYPE";
        const char unknownSeverity[] = "UNKNOWN_SEVERITY";
        const char* typeString = unknownType;
        const char* severityString = unknownSeverity;
        const char* messageIdName = pCallbackData->pMessageIdName;
        int32_t messageIdNumber = pCallbackData->messageIdNumber;
        const char* message = pCallbackData->pMessage;
        android_LogPriority priority = ANDROID_LOG_UNKNOWN;

        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            severityString = error;
            priority = ANDROID_LOG_ERROR;
        }
        else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            severityString = warning;
            priority = ANDROID_LOG_WARN;
        }
        if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            typeString = validation;
        }
        else if (messageTypes & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
            typeString = performance;
        }

        if (priority == ANDROID_LOG_ERROR) {
            LOGE(
                    "%s %s: [%s] Code %i : %s",
                    typeString,
                    severityString,
                    messageIdName,
                    messageIdNumber,
                    message
            );
        } else {
            LOGW(
                    "%s %s: [%s] Code %i : %s",
                    typeString,
                    severityString,
                    messageIdName,
                    messageIdNumber,
                    message
            );
        }

        return VK_FALSE;
    }

    void TalesArEngine::CreateDebugMessenger() {
        if (!VALIDATION_LAYERS_ENABLED) {
            return;
        }
        auto pfnCreateDebugUtilsMessengerEXT =
                reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                        vkGetInstanceProcAddr(
                                mInstance,
                                "vkCreateDebugUtilsMessengerEXT"
                        )
                );

        if (pfnCreateDebugUtilsMessengerEXT) {
            constexpr VkDebugUtilsMessageSeverityFlagsEXT kSeveritiesToLog =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
            constexpr VkDebugUtilsMessageTypeFlagsEXT kMessagesToLog =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            VkDebugUtilsMessengerCreateInfoEXT messengerInfo{
                    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                    .pNext = nullptr,
                    .flags = 0,
                    .messageSeverity = kSeveritiesToLog,
                    .messageType = kMessagesToLog,
                    .pfnUserCallback = &DebugMessengerCallback,
                    .pUserData = nullptr,
            };

            pfnCreateDebugUtilsMessengerEXT(
                    mInstance,
                    &messengerInfo,
                    nullptr,
                    &mDebugMessenger
            );
        }
    }

    void TalesArEngine::DestroyDebugMessenger() {
        auto pfnDestroyDebugUtilsMessengerEXT =
                reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                        vkGetInstanceProcAddr(
                                mInstance,
                                "vkDestroyDebugUtilsMessengerEXT"
                        )
                );
        if (pfnDestroyDebugUtilsMessengerEXT) {
            pfnDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
        }
    }

    void TalesArEngine::CreateInstance() {
        if (VALIDATION_LAYERS_ENABLED && !ValidationLayersSupported()) {
            LOGE("Validation layers requested, but not available!");
            LOGW("Disabling validation layers and proceeding without them.");
            VALIDATION_LAYERS_ENABLED = false;
        }

        std::vector<const char*> extensions = GetRequiredExtensions();
        const VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = APP_NAME.data(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = ENGINE_NAME.data(),
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_1,
        };
        const VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = VALIDATION_LAYERS_ENABLED ? static_cast<uint32_t>(VALIDATION_LAYERS.size()) : 0,
            .ppEnabledLayerNames = VALIDATION_LAYERS_ENABLED ? VALIDATION_LAYERS.data() : nullptr,
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        VK_CALL(vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance));
    }

    void TalesArEngine::CreateSurface() {
        VkAndroidSurfaceCreateInfoKHR createInfo{
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .flags = 0,
            .window = mWindowWrapper.GetWindow()
        };

        VK_CALL(vkCreateAndroidSurfaceKHR(mInstance, &createInfo, nullptr, &mSurface));
    }

    void TalesArEngine::CreatePhysicalDevice() {
        uint32_t gpuCount = 0;

        VK_CALL(vkEnumeratePhysicalDevices(mInstance, &gpuCount, nullptr));

        if (gpuCount == 0) {
            exception::ThrowJavaException(mJniEnv, "Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(gpuCount);

        VK_CALL(vkEnumeratePhysicalDevices(mInstance, &gpuCount, devices.data()));

        mPhysicalDevice = devices[0];
    }

    void TalesArEngine::SetQueueFamilyIndex() {
        uint32_t queueFamilyCount;

        vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);

        if (queueFamilyCount == 0) {
            exception::ThrowJavaException(mJniEnv, "No queue families found.");
        }

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            mPhysicalDevice,
            &queueFamilyCount,
            queueFamilies.data()
        );
        for (auto i = 0; i != queueFamilyCount; ++i) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                mQueueFamilyIndex = i;
                break;
            }
        }
        if (mQueueFamilyIndex == UINT32_MAX) {
            exception::ThrowJavaException(mJniEnv, "No graphics queue family found.");
        }
    }

    void TalesArEngine::CreateLogicalDevice() {
        std::vector<const char*> deviceExtensions;

        deviceExtensions.push_back("VK_KHR_swapchain");
        deviceExtensions.push_back("VK_KHR_sampler_ycbcr_conversion");
        deviceExtensions.push_back("VK_KHR_maintenance1");
        deviceExtensions.push_back("VK_KHR_bind_memory2");
        deviceExtensions.push_back("VK_KHR_get_memory_requirements2");
        deviceExtensions.push_back("VK_KHR_external_memory");
        deviceExtensions.push_back("VK_EXT_queue_family_foreign");
        deviceExtensions.push_back("VK_KHR_dedicated_allocation");

        float priority = 1.0f;

        const VkDeviceQueueCreateInfo queueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .flags = 0,
            .queueFamilyIndex = mQueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &priority
        };
        const VkPhysicalDeviceFeatures deviceFeatures{};

        const VkDeviceCreateInfo deviceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledExtensionCount = static_cast<uint32_t >(deviceExtensions.size()),
            .ppEnabledExtensionNames = deviceExtensions.data(),
            .pEnabledFeatures = &deviceFeatures,
        };

        VK_CALL(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice));
        vkGetDeviceQueue(mLogicalDevice, mQueueFamilyIndex, 0, &mGraphicsQueue);
    }
}