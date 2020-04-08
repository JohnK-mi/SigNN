import os
import sys

import tensorflow as tf
import numpy as np

import subprocess

MODEL_RELATIVE_PATH = "signn_model/model.tflite"

model_abs_path = os.path.join(os.getcwd(), MODEL_RELATIVE_PATH)


def loadTFModel(path):
    # Load TFLite model and allocate tensors.
    interpreter = tf.lite.Interpreter(model_path=path)
    interpreter.allocate_tensors()

    # # Get input and output tensors.


    return interpreter

def invokeModel(interpreter, data):
    # Test model on random input data.
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    input_shape = input_details[0]['shape']
    input_data = np.array(np.random.random_sample(input_shape), dtype=np.float32)
    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()
    # The function `get_tensor()` returns a copy of the tensor data.
    # Use `tensor()` in order to get a pointer to the tensor.
    output_data = interpreter.get_tensor(output_details[0]['index'])
    return output_data.flatten()

def pythonprint(*args):
    print("[SigNN] ", *args)


def inerpretResult(data):
    POTENTIAL_OUTPUTS = [
        "A", "B", "C", "D", "E", "F", "G", "H", "I",  "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y"
    ]

    # return POTENTIAL_OUTPUTS(np.argmax(data))
    # print(data.shape)
    # print(data.flatten())
    # print(data.flatten().shape)
    # data = data.tolist()
    # print(data)
    highest_val = 0
    highest_index = 0
    for i in range(len(data)):
        if data[i] > highest_val:
            highest_val = data[i]
            highest_index = i
    return POTENTIAL_OUTPUTS[highest_index], highest_val


model = loadTFModel(model_abs_path)
pythonprint("Starting signn")
while True:
    for line in sys.stdin:
        line = line[:-1]
        if not line or "[SigNN]" in line[:8]:
            continue
        
        result = invokeModel(model, np.fromiter(line.split(" "), dtype=np.float32))
        pythonprint(inerpretResult(result))

pythonprint("END")
