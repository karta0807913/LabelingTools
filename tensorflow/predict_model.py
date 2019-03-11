import tensorflow as tf
import tf_model
import cv2
import socket as socket_module
import numpy as np

socket = socket_module.socket(socket_module.AF_INET, socket_module.SOCK_STREAM)

socket.bind(("127.0.0.1", 14333))
socket.listen(1)

session = tf.Session()
image = tf.placeholder(tf.float32, [
    1,
    tf_model.IMAGE_WIDTH,
    tf_model.IMAGE_HEIGHT,
    tf_model.IMAGE_DEPTH
])

img_width = tf.placeholder(tf.float32, shape=[])
img_height = tf.placeholder(tf.float32, shape=[])
predict = tf_model.model(image)
label = tf_model.process_label(predict, img_width, img_height, 0.6)

saver = tf.train.Saver()
saver.restore(session, "./model/model.ckpt")

while True:
    connection, client_address = socket.accept()
    width = connection.recv(4)
    height = connection.recv(4)
    data = b""
    while True:
        tmp = connection.recv(256*256*2)
        data += tmp
        if len(tmp) == 0:
            data = b""
            break
        print("get data size %d" % len(data))
        if len(data) < 256*256*3:
            continue
        elif len(data) > 256*256*3:
            print("wrong data size")
            data = b""
            break
        else:
            break
    if len(data) == 0:
        continue

    width = np.frombuffer(width, dtype=np.uint32)
    height = np.frombuffer(height, dtype=np.uint32)
    data = np.frombuffer(data, dtype=np.uint8)
    data = data.reshape((1, 256, 256, 3))

    result = session.run(label,{
        image: data,
        img_width: width[0],
        img_height: height[0]
    })
    data = b""
    for i in result:
        data += i.tostring()
    connection.send(data)
    connection.close()
