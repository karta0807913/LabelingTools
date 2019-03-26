import tensorflow as tf

IMAGE_WIDTH=416
IMAGE_HEIGHT=416
IMAGE_DEPTH=3

GRID_H = 13
GRID_W = 13
GRID_DEPTH=5
def model (image):
    # image=tf.placeholder(tf.uint8, shape=(None, 256, 256, 3))
    # answer_pos=tf.placeholder(tf.float32, shape=(None, 7, 7, 4))
    # answer_has=tf.placeholder(tf.float32, shape=(None, 7, 7, 1))
    image=(image - 128) / 256

    def make_scope(_input, name, conv_fitter):
        conv = tf.keras.layers.Conv2D(conv_fitter, (3, 3), padding="same", name=name+"_conv")
        conv = conv(_input)
        leaky = tf.keras.layers.LeakyReLU(name=name+"_leaky")
        leaky = leaky(conv)
        pool = tf.keras.layers.MaxPool2D(name=name+"_pool")
        return pool(leaky)

    with tf.name_scope("conv_416x416_to_13x13"):
        image=make_scope(image, "layer1", 16)
        image=make_scope(image, "layer2", 32)
        image=make_scope(image, "layer3", 64)
        image=make_scope(image, "layer4", 128)
        image=make_scope(image, "layer5", 256)

        conv6 = tf.keras.layers.Conv2D(256, (3, 3), padding="same", name="layer6_conv")
        conv6 = conv6(image)
        leaky6 = tf.keras.layers.LeakyReLU(name="layer6_leaky")
        leaky6 = leaky6(conv6)
        pool6 = tf.keras.layers.MaxPool2D(padding="same", strides=(1,1), name="layer6_pool")
        pool6 = pool6(leaky6)

        conv7 = tf.keras.layers.Conv2D(128, (3, 3), padding="same", name="layer7_conv")
        conv7 = conv7(pool6)
        leaky7 = tf.keras.layers.LeakyReLU(name="layer7_leaky")
        leaky7 = leaky7(conv7)


        conv8 = tf.keras.layers.Conv2D(64, (3, 3), padding="same", name="layer8_conv")
        conv8 = conv8(leaky7)
        leaky8 = tf.keras.layers.LeakyReLU(name="layer8_leaky")
        leaky8 = leaky8(conv8)

        conv9 = tf.keras.layers.Conv2D(5, (3, 3), padding="same", name="layer9_conv")
        conv9 = conv9(leaky8)

    return conv9

def process_label(label, img_w, img_h, threshold=0.4, batch_size=1):
    idx   = tf.reshape(
        tf.tile(
            tf.range(GRID_W * GRID_H), [batch_size]
        ), [batch_size, GRID_W, GRID_H])
    mask  = tf.reshape(tf.greater(tf.sigmoid(label[...,0:1]), threshold), [batch_size, GRID_W, GRID_H])
    label = tf.boolean_mask(label[...,1:5], mask)
    idx   = tf.boolean_mask(idx, mask)
    return _process_box(label, idx, img_w, img_h)

def _process_box(predict, idx, img_w, img_h):
    step_w = img_w / float(GRID_W)
    step_h = img_h / float(GRID_H)

    box_x = tf.reshape(tf.cast(idx / GRID_W, tf.float32), [-1])
    box_y = tf.reshape(tf.cast(tf.cast(idx % GRID_W, tf.int32), tf.float32), [-1])

    predict_w = tf.exp(predict[:,2]) * img_w
    predict_h = tf.exp(predict[:,3]) * img_h
    predict_x = box_x * step_w + predict[:,0] * step_w - predict_w / 2
    predict_y = box_y * step_h + predict[:,1] * step_h - predict_h / 2

    return predict_x, predict_y, predict_w, predict_h

def loss(predict, labels):
    answer_has=labels[:,:,:,0:1]
    answer_pos=labels[:,:,:,1:5]

    reshaped = tf.reshape(answer_has, [-1, GRID_H, GRID_W])
    mask = tf.cast(reshaped, tf.bool)

    answer_pos = tf.boolean_mask(answer_pos, mask)

    pred_has_box = predict[:,:,:,0:1]
    pred_box_xy  = tf.boolean_mask(predict[:,:,:,1:3], mask)
    pred_box_w   = tf.exp(tf.boolean_mask(predict[:,:,:,3:4], mask))
    pred_box_h   = tf.exp(tf.boolean_mask(predict[:,:,:,4:5], mask))

    loss_xy      = tf.losses.mean_squared_error(pred_box_xy, answer_pos[:,0:2])
    loss_w       = tf.losses.mean_squared_error(pred_box_w, answer_pos[:,2:3])
    loss_h       = tf.losses.mean_squared_error(pred_box_h, answer_pos[:,3:4])

    has_loss = tf.nn.sigmoid_cross_entropy_with_logits(labels=pred_has_box, logits=answer_has)

    has_obj_loss = has_loss * answer_has * tf.pow(1 - tf.sigmoid(pred_has_box), 5)
    has_obj_loss = tf.reduce_mean(has_obj_loss, 0)
    has_obj_loss = tf.reduce_sum(has_obj_loss)

    no_obj_loss = has_loss * (1 - answer_has) * tf.pow(tf.sigmoid(pred_has_box), 5)
    no_obj_loss = tf.reduce_mean(no_obj_loss, 0)
    no_obj_loss = tf.reduce_sum(no_obj_loss)

    return has_obj_loss, no_obj_loss, loss_xy, loss_w + loss_h