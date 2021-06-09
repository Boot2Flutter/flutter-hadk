# flutter-hadk
A light-weight Flutter Engine Embedder based on HADK ,which for Android devices that runs without any java code

# 1.Build by android-ndk-toolchain
build isuse fixed by: https://www.jianshu.com/p/f84dbf8ec147
```
cd build

cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_NATIVE_API_LEVEL=21

make
```