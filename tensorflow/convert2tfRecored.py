import tensorflow as tf
import numpy as np
import google.protobuf.text_format
import data_pb2
import cv2
import sys
import os

def get_data_list(root_folder):
    folders = os.listdir(root_folder)
    image_list = []
    proto_list = []
    for folder in folders:
        folder = os.path.join(root_folder, folder)

        images = os.listdir(folder + "/image")
        for image in images:
            image_path = folder + "/image/" + image
            image_filename = os.path.basename(image_path)
            image_name = os.path.splitext(image_filename)[0]

            proto_path = folder + "/proto/" + image_name + ".prototxt"

            image_list.append(image_path)
            proto_list.append(proto_path)

    return image_list, proto_list

def make_record(root_folder, out_filename, img_w=256, img_h=256, grid_w=7, grid_h=7):
    images, labels = get_data_list(root_folder)

    with tf.python_io.TFRecordWriter(out_filename) as writer:
        for image_file, label_file in zip(images, labels):
            img = cv2.imread(image_file)
            img = cv2.resize(img, (img_w, img_h))

            label = data_pb2.PictureData()
            label_martix=np.zeros([grid_w, grid_h, 5], dtype=np.float32)
            google.protobuf.text_format.Merge(open(label_file, 'r').read(), label)

            step_w_size = float(label.width) / grid_w
            step_h_size = float(label.height) / grid_h
            for rect_data in label.object_parameter:
                label_w  = (rect_data.xmax - rect_data.xmin)
                label_h  = (rect_data.ymax - rect_data.ymin)

                x_center = rect_data.xmax - label_w / 2
                y_center = rect_data.ymax - label_h / 2

                float_x = x_center / step_w_size
                float_y = y_center / step_h_size

                grid_x = int(float_x)
                grid_y = int(float_y)

                label_martix[grid_x, grid_y, 0] = 1.0 #have box or not
                label_martix[grid_x, grid_y, 1] = float(float_x) - grid_x # box pos x
                label_martix[grid_x, grid_y, 2] = float(float_y) - grid_y # box pos y
                label_martix[grid_x, grid_y, 3] = float(label_w) / label.width # box width
                label_martix[grid_x, grid_y, 4] = float(label_h) / label.height # box height

                image_raw = img.tostring()
                label_raw = label_martix.tostring()

                example = tf.train.Example(features=tf.train.Features(feature={
                    "image": tf.train.Feature(bytes_list=tf.train.BytesList(value=[image_raw])),
                    "label": tf.train.Feature(bytes_list=tf.train.BytesList(value=[label_raw]))
                }))

                writer.write(example.SerializeToString())

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage " + sys.argv[0] + " root_folder out_filename")

    else:
        make_record(sys.argv[1], sys.argv[2])
