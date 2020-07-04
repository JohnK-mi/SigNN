#!/bin/bash
<<<<<<< HEAD
# Follow this guide to get any version of bazel: https://docs.bazel.build/versions/master/install-ubuntu.html
=======
# Follow this guide to get bazel 1.2.1: https://docs.bazel.build/versions/master/install-ubuntu.html
>>>>>>> I don't know which bazel to compile with
bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/multi_hand_tracking:multi_hand_tracking_cpu