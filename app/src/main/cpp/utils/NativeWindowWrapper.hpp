#ifndef TALESAR_NATIVEWINDOWWRAPPER_HPP
#define TALESAR_NATIVEWINDOWWRAPPER_HPP

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <memory>

#include "exception/Exception.hpp"

namespace talesar {
    class NativeWindowWrapper {
    public:
        explicit NativeWindowWrapper(JNIEnv* pEnv, jobject surface) {
            mGlobalRef = pEnv->NewGlobalRef(surface);
            ANativeWindow* pWindow = ANativeWindow_fromSurface(pEnv, mGlobalRef);

            if (!pWindow) {
                exception::ThrowJavaException(pEnv, "Failed to create native window from surface");
            }
            mWindow.reset(pWindow);
        }
        ~NativeWindowWrapper() {
            if (mGlobalRef) {
                mGlobalRef = nullptr;
            }
        }

        ANativeWindow* GetWindow() const noexcept {
            return mWindow.get();
        }

    private:
        struct Deleter {
            void operator()(ANativeWindow* pWindow) {
                ANativeWindow_release(pWindow);
            }
        };
        jobject mGlobalRef{nullptr};
        std::unique_ptr<ANativeWindow, Deleter> mWindow;
    };
}

#endif //TALESAR_NATIVEWINDOWWRAPPER_HPP