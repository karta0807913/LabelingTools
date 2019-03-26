import cv2
import tensorflow as tf
import tf_model
import sys
import numpy as np

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
label = tf_model.process_label(predict, img_width, img_height, 0.55)

saver = tf.train.Saver()
saver.restore(session, "./model/model.ckpt")

def predict_video(input_file, output_file):
    cap = cv2.VideoCapture(input_file)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    img_fps = cap.get(cv2.CAP_PROP_FPS)
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    print(" %s %s %s" % (img_fps, img_width, img_height))
    out = cv2.VideoWriter(output_file, fourcc, img_fps, (640,480))
    out = cv2.VideoWriter(output_file, fourcc, img_fps, (width, height))

    ret, frame = cap.read()
    if not ret:
        print("video read error")
        exit(1)

    ml = 15
    lc = 15
    last_predict = [[], [], [], []]
    while ret:
        p_frame = cv2.resize(frame, (tf_model.IMAGE_WIDTH, tf_model.IMAGE_HEIGHT))
        p_frame = p_frame.reshape((1, tf_model.IMAGE_WIDTH, tf_model.IMAGE_HEIGHT, tf_model.IMAGE_DEPTH))
        predict_label = session.run(label, {
            image: p_frame,
            img_width: width,
            img_height: height
        })
        if len(predict_label[0]) == 0:
            lc -= 1
            predict_label = last_predict
        else:
            lc = ml
            last_predict = predict_label
        if lc == 0:
            last_predict = [[], [], [], []]
        x, y, w, h = predict_label
        for i in range(len(x)):
            frame = cv2.rectangle(frame, (x[i], y[i]), (x[i]+w[i], y[i]+h[i]), 2)
        # cv2.imshow('frame',frame)
        # cv2.waitKey(1)
        out.write(frame)
        ret, frame = cap.read()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage " + sys.argv[0] + "input_file output_file")
    else:
        predict_video(sys.argv[1], sys.argv[2])