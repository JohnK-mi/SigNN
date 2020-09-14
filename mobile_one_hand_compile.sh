export ANDROID_HOME=/home/python/Android/Sdk/
export ANDROID_NDK_HOME=/home/python/Android/Sdk/ndk/21.3.6528147/
bazel build -c opt --config=android_arm64 mediapipe/examples/android/src/java/com/google/mediapipe/apps/signnonehand:signnonehand
adb install bazel-bin/mediapipe/examples/android/src/java/com/google/mediapipe/apps/signnonehand/signnonehand.apk