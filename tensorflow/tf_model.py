import tensorflow as tf

IMAGE_WIDTH=256
IMAGE_HEIGHT=256
IMAGE_DEPTH=3

GRID_H = 7
GRID_W = 7
GRID_DEPTH=5
def model (image):
    # image=tf.placeholder(tf.uint8, shape=(None, 256, 256, 3))
    # answer_pos=tf.placeholder(tf.float32, shape=(None, 7, 7, 4))
    # answer_has=tf.placeholder(tf.float32, shape=(None, 7, 7, 1))
    image=(image - 128) / 256

    with tf.name_scope("conv_256x256_to_7x7"):
        conv1 = tf.keras.layers.Conv2D(64, (13, 13), strides=(4,4))
        conv1 = conv1(image)

        conv2 = tf.keras.layers.Conv2D(32, (7, 7), dilation_rate=(2,2))
        conv2 = conv2(conv1)

        conv3 = tf.keras.layers.Conv2D(32, (3, 3), strides=(2,2))
        conv3 = conv3(conv2)

        conv4 = tf.keras.layers.Conv2D(8, (3, 3), strides=(2,2))
        conv4 = conv4(conv3)

        conv5 = tf.keras.layers.Conv2D(5, (3, 3))
        conv5 = conv5(conv4)

        predict = tf.keras.layers.Conv2D(5, (3, 3))
        predict = predict(conv5)

    return predict

def process_label(label, img_w, img_h, threshold=0.4):
    idx    = tf.reshape(tf.range(GRID_W * GRID_H), [1, GRID_W, GRID_H])
    mask = tf.reshape(tf.greater(tf.sigmoid(label[...,0:1]), threshold), [1, GRID_W, GRID_H])
    label = tf.boolean_mask(label[...,1:5], mask)
    idx = tf.boolean_mask(idx, mask)
    return _process_box(label, idx, img_w, img_h)

def _process_box(predict, idx, img_w, img_h):
    step_w = img_w / float(GRID_W)
    step_h = img_h / float(GRID_H)

    box_x = tf.reshape(tf.cast(idx % GRID_W, tf.float32), [-1])
    box_y = tf.reshape(tf.cast(tf.cast(idx / GRID_W, tf.int32), tf.float32), [-1])

    predict_w = predict[:,2] * img_w
    predict_h = predict[:,3] * img_h
    predict_x = box_x * step_w + predict[:,0] * step_w - predict_w / 2
    predict_y = box_y * step_h + predict[:,1] * step_h - predict_h / 2

    return predict_x, predict_y, predict_w, predict_h

def loss(predict, labels):
    answer_has=labels[:,:,:,0:1]
    answer_pos=labels[:,:,:,1:5]

    mask = tf.cast(tf.reshape(answer_has, [-1, GRID_H, GRID_W]), tf.bool)

    answer_pos = tf.boolean_mask(answer_pos, mask)

    pred_has_box = predict[:,:,:,0:1]
    pred_box_xy  = tf.boolean_mask(predict[:,:,:,1:3], mask)
    pred_box_wh  = tf.boolean_mask(predict[:,:,:,3:5], mask)

    loss_xy      = tf.losses.mean_squared_error(pred_box_xy, answer_pos[:,0:2])
    loss_wh      = tf.losses.mean_squared_error(pred_box_wh, answer_pos[:,2:4])

    has_loss = tf.nn.sigmoid_cross_entropy_with_logits(labels=pred_has_box, logits=answer_has)
    has_loss = tf.minimum(has_loss, 3)

    has_obj_loss = has_loss * answer_has * tf.pow(1 - pred_has_box, 2)
    has_obj_loss = tf.reduce_mean(has_obj_loss, 0)
    has_obj_loss = tf.reduce_sum(has_obj_loss)

    no_obj_loss = has_loss * (1 - answer_has) * tf.pow(pred_has_box, 2)
    no_obj_loss = tf.reduce_mean(no_obj_loss, 0)
    no_obj_loss = tf.reduce_sum(no_obj_loss)

    return has_obj_loss, no_obj_loss, loss_xy, loss_wh