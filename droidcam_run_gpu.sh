sudo GLOG_logtostderr=1 LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so bazel-bin/mediapipe/examples/desktop/hand_tracking/hand_tracking_gpu --calculator_graph_config_file=mediapipe/graphs/hand_tracking/hand_tracking_mobile.pbtxt