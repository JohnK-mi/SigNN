import os

import tensorflow as tf
import numpy as np

from multiprocessing import Queue

MODEL_RELATIVE_PATH = "signn_model/model.tflite"

model_abs_path = os.path.join(os.getcwd(), MODEL_RELATIVE_PATH)


def loadTFModel(path):
    # Load TFLite model and allocate tensors.
    interpreter = tf.lite.Interpreter(model_path=path)
    interpreter.allocate_tensors()

    # # Get input and output tensors.
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    return interpreter

def invokeModel(interpreter, data):
    # Test model on random input data.
    input_shape = input_details[0]['shape']
    input_data = np.array(np.random.random_sample(input_shape), dtype=np.float32)
    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()
    # The function `get_tensor()` returns a copy of the tensor data.
    # Use `tensor()` in order to get a pointer to the tensor.
    output_data = interpreter.get_tensor(output_details[0]['index'])
    return output_data



def runmediapipe(droidcam=False):
    command = "sudo GLOG_logtostderr=0 bazel-bin/mediapipe/examples/desktop/multi_hand_tracking/multi_hand_tracking_cpu --calculator_graph_config_file=mediapipe/graphs/hand_tracking/multi_hand_tracking_desktop_console_logger.pbtxt"
    if droidcam:
        

for line in sys.stdin:
    print("pythonline", line)

# model = loadTFModel(model_abs_path)

