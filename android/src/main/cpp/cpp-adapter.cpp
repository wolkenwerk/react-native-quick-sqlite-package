#include "bindings.h"
#include "logs.h"
#include <ReactCommon/CallInvokerHolder.h>
#include <fbjni/fbjni.h>
#include <jni.h>
#include <jsi/jsi.h>
#include <typeinfo>

struct QuickSQLiteBridge : jni::JavaClass<QuickSQLiteBridge> {
  static constexpr auto kJavaDescriptor =
      "Lcom/margelo/rnquicksqlite/QuickSQLiteBridge;";

  static void registerNatives() {
    javaClassStatic()->registerNatives(
        {// initialization for JSI
         makeNativeMethod("installNativeJsi",
                          QuickSQLiteBridge::installNativeJsi),
         makeNativeMethod("clearStateNativeJsi",
                          QuickSQLiteBridge::clearStateNativeJsi)
      });
  }

private:
  static void installNativeJsi(
      jni::alias_ref<jni::JObject> thiz, jlong jsiRuntimePtr,
      jni::alias_ref<react::CallInvokerHolder::javaobject> jsCallInvokerHolder,
      jni::alias_ref<jni::JString> docPath,
      jni::alias_ref<jni::JString> cachePath){
    auto jsiRuntime = reinterpret_cast<jsi::Runtime *>(jsiRuntimePtr);
    auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
    std::string docPathString = docPath->toStdString();
    std::string cachePathString = cachePath->toStdString();


   osp::install(*jsiRuntime, jsCallInvoker, docPathString.c_str(), cachePathString.c_str());
  }

  static void clearStateNativeJsi(jni::alias_ref<jni::JObject> thiz) {
    osp::clearState();
  }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *) {
  return jni::initialize(vm, [] { QuickSQLiteBridge::registerNatives(); });
}
