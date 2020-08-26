# Example call: "bash droidcam.sh run_gpu.sh". This will run the gpu script with droidcam
command=$(<$1)
command=${command:5}
start="sudo LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libv4l/v4l2convert.so"
command="${start} ${command}"
eval $command