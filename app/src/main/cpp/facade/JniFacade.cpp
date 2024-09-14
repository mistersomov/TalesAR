#include "JniFacade.hpp"

#include <android/asset_manager_jni.h>

#include "TalesArApp.hpp"

#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_mistersomov_talesar_common_facade_JniFacade_##method_name

extern "C" {
namespace {

    JavaVM *gVm = nullptr;

    inline jlong jptr(talesar::TalesArApp *pApp) {
        return reinterpret_cast<jlong>(pApp);
    }

    inline talesar::TalesArApp *native(jlong ptr) {
        return reinterpret_cast<talesar::TalesArApp*>(ptr);
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    gVm = vm;
    return JNI_VERSION_1_6;
}

JNI_METHOD(jlong, createNativeApp)
(JNIEnv *env, jclass, jobject j_asset_manager, jobject context, jobject activity) {
    AAssetManager *asset_manager = AAssetManager_fromJava(env, j_asset_manager);
    return jptr(new talesar::TalesArApp(asset_manager, env, context, activity));
}

JNI_METHOD(void, destroyNativeApp)
(JNIEnv *env, jclass clazz, jlong app) {
    delete native(app);
}

JNI_METHOD(void, onPause)
(JNIEnv *env, jclass clazz, jlong app) {
    native(app)->OnPause(env);
}

JNI_METHOD(void, onResume)
(JNIEnv *env, jclass clazz, jlong app) {
    native(app)->OnResume(env);
}
}