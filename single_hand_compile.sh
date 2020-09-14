#!/bin/bash
# Follow this guide to get any version of bazel: https://docs.bazel.build/versions/master/install-ubuntu.html
bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/hand_tracking:hand_tracking_cpu