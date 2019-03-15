import data_pb2
import cv2
import numpy as np

def compute_otherwise(min_val, max_val, length):
    new_max_val = length - min_val
    new_min_val = length - max_val
    return new_min_val, new_max_val

def flip_left_right(image, label):
    return flip_label(image, label, 1)

def flip_up_down(image, label):
    return flip_label(image, label, 0)

def flip_label(image, label, i):
    new_label = data_pb2.PictureData()
    image = cv2.flip(image, i)
    new_label.width = label.width
    new_label.height = label.height
    for rect_data in label.object_parameter:
        new_data = new_label.object_parameter.add()
        if i == 1:
            new_data.xmin, new_data.xmax = compute_otherwise(rect_data.xmin, rect_data.xmax, label.width)
            new_data.ymin = rect_data.ymin
            new_data.ymax = rect_data.ymax
        else:
            new_data.ymin, new_data.ymax = compute_otherwise(rect_data.ymin, rect_data.ymax, label.height)
            new_data.xmin = rect_data.xmin
            new_data.xmax = rect_data.xmax
        for tag in rect_data.tag:
            new_data.tag.append(tag)
    return image, new_label

def make_yolo_format(label, grid_w, grid_h):
    label_matrix=np.zeros([grid_w, grid_h, 5], dtype=np.float32)
    step_w_size = float(label.width) / grid_w
    step_h_size = float(label.height) / grid_h
    for rect_data in label.object_parameter:
        label_w  = float(rect_data.xmax - rect_data.xmin)
        label_h  = float(rect_data.ymax - rect_data.ymin)

        x_center = rect_data.xmax - label_w / 2
        y_center = rect_data.ymax - label_h / 2

        float_x = x_center / step_w_size
        float_y = y_center / step_h_size

        grid_x = int(float_x)
        grid_y = int(float_y)

        label_matrix[grid_x, grid_y, 0] = 1.0 #have box or not
        label_matrix[grid_x, grid_y, 1] = float(float_x) - grid_x # box pos x
        label_matrix[grid_x, grid_y, 2] = float(float_y) - grid_y # box pos y
        label_matrix[grid_x, grid_y, 3] = float(label_w) / label.width # box width
        label_matrix[grid_x, grid_y, 4] = float(label_h) / label.height # box height
    return label_matrix
