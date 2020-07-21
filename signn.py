import os
import sys
from scipy.stats import zscore

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

import subprocess

MODEL_RELATIVE_PATH = "signn_model/model.tflite"

model_abs_path = os.path.join(os.getcwd(), MODEL_RELATIVE_PATH)

# hi


def plot(hand, word, save=False):
    assert isinstance(save, bool)
    COLORS = ['red', 'blue', 'green', 'purple', 'black']
    x = [max(xx for xx in hand[::2]) - x for x in hand[::2]]
    y = [max(yy for yy in hand[1::2]) - y for y in hand[1::2]]

    text = [z for z in range(0, len(x))]
    assert len(x) == len(y), "Uneven x and y coordnates. {} x and {} y".format(len(x), len(y))
    plt.scatter(x, y)
    plt.xlabel('x')
    plt.ylabel('y')
    plt.axis('square')
    plt.title(word)

    for i, txt in enumerate(text):
        plt.annotate(txt, (x[i], y[i]))

    for finger, color in zip(range(1, 21, 4), COLORS):
        plt.plot(x[finger:finger+4], y[finger:finger+4], 'ro-', color=color)

    if save:
        plt.savefig(os.path.join("image_means", word + '.png'))
    else:
        plt.show()
    plt.close()

def loadTFModel(path):
    # Load TFLite model and allocate tensors.
    interpreter = tf.lite.Interpreter(model_path=path)
    interpreter.allocate_tensors()

    # # Get input and output tensors.


    return interpreter

def dataToZscoredata(data):
    x_values = zscore(data[::2])
    y_values = zscore(data[1::2])
    full_values = np.zeros(shape=(42))
    for i in range(len(x_values)):
        full_values[i*2] = (x_values[i])
        full_values[i*2 + 1] = (y_values[i])
    return full_values

def invokeModel(interpreter, data):
    # Test model on random input data.
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    input_shape = input_details[0]['shape']
    input_data = np.array(np.zeros(shape=input_shape), dtype=np.float32)
    for x in range(len(data)):
        input_data[0][x] = data[x]
    # pythonprint("INPUT SHAPE:", input_shape)
    # pythonprint(np.random.random_sample(input_shape))
    # pythonprint(input_data)
    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()
    # The function `get_tensor()` returns a copy of the tensor data.
    # Use `tensor()` in order to get a pointer to the tensor.
    output_data = interpreter.get_tensor(output_details[0]['index'])
    return output_data.flatten()

def invokeModelPythonZscores(interpreter, data):
    # Z = (x - u) / o
    full_values = dataToZscoredata(data)
    # plot(data, "Original Data")
    # plot(full_values, "Z scores")
    return invokeModel(interpreter, full_values)

def pythonprint(*args):
    print("[SigNN] ", *args)


def inerpretResult(data):
    POTENTIAL_OUTPUTS = [
        "A", "B", "C", "D", "E", "F", "G", "H", "I",  "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y"
    ]


    highest_val = 0
    highest_index = 0
    for i in range(len(data)):
        if data[i] > highest_val:
            highest_val = data[i]
            highest_index = i

    return POTENTIAL_OUTPUTS[highest_index], highest_val


if __name__ == "__main__":
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
