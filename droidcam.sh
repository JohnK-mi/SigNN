command=$(<$1)
command=${command:5}
start=sudo LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so
eval $start+$command