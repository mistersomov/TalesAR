#ifndef TALESAR_LOG_HPP
#define TALESAR_LOG_HPP

#include <android/log.h>

#ifndef LOGI
#define LOGI(...) \
  __android_log_print(ANDROID_LOG_INFO, "TalesAR_TAG", __VA_ARGS__)
#endif  // LOGI

#ifndef LOGE
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, "TalesAR_TAG", __VA_ARGS__)
#endif  // LOGE

#endif //TALESAR_LOG_HPP
