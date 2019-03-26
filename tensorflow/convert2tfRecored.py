import tensorflow as tf
import numpy as np
import google.protobuf.text_format
import crop_util
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

def make_record(root_folder, out_filename, img_w=416, img_h=416, grid_w=13, grid_h=13):
    images, labels = get_data_list(root_folder)

    count = 0
    with tf.python_io.TFRecordWriter(out_filename) as writer:
        for image_file, label_file in zip(images, labels):
            try:
                img = cv2.imread(image_file)
                img = cv2.resize(img, (img_w, img_h))

                label = data_pb2.PictureData()
                google.protobuf.text_format.Merge(open(label_file, 'r').read(), label)

                label_matrix = crop_util.make_yolo_format(label, grid_w, grid_h)
                image_raw = img.tostring()
                label_raw = label_matrix.tostring()

                example = tf.train.Example(features=tf.train.Features(feature={
                    "image": tf.train.Feature(bytes_list=tf.train.BytesList(value=[image_raw])),
                    "label": tf.train.Feature(bytes_list=tf.train.BytesList(value=[label_raw]))
                }))

                writer.write(example.SerializeToString())


                img_r = np.random.random(img_w * img_h * 3) * 4 - 2
                img_r = np.reshape(img_r, (img_w, img_h, 3))
                img_r = img + img_r
                img_r = np.maximum(img_r, 0)
                img_r = np.minimum(img_r, 255)
                img_r = np.cast[np.int8](img_r)
                image_raw = img_r.tostring()
                example = tf.train.Example(features=tf.train.Features(feature={
                    "image": tf.train.Feature(bytes_list=tf.train.BytesList(value=[image_raw])),
                    "label": tf.train.Feature(bytes_list=tf.train.BytesList(value=[label_raw]))
                }))
                writer.write(example.SerializeToString())

                img_u, label_u = crop_util.flip_up_down(img, label)
                label_matrix = crop_util.make_yolo_format(label_u, grid_w, grid_h)
                image_raw = img_u.tostring()
                label_raw = label_matrix.tostring()

                example = tf.train.Example(features=tf.train.Features(feature={
                    "image": tf.train.Feature(bytes_list=tf.train.BytesList(value=[image_raw])),
                    "label": tf.train.Feature(bytes_list=tf.train.BytesList(value=[label_raw]))
                }))
                writer.write(example.SerializeToString())

                img_l, label_l = crop_util.flip_left_right(img, label)
                label_matrix = crop_util.make_yolo_format(label_l, grid_w, grid_h)
                image_raw = img_l.tostring()
                label_raw = label_matrix.tostring()

                example = tf.train.Example(features=tf.train.Features(feature={
                    "image": tf.train.Feature(bytes_list=tf.train.BytesList(value=[image_raw])),
                    "label": tf.train.Feature(bytes_list=tf.train.BytesList(value=[label_raw]))
                }))
                writer.write(example.SerializeToString())

                count += 1
                print("processed %s" % image_file)
            except Exception as e:
                print(e)
        print("total %d data" % count )

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage " + sys.argv[0] + " root_folder out_filename")

    else:
        make_record(sys.argv[1], sys.argv[2])
