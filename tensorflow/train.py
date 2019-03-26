import tensorflow as tf
import tf_model
import matplotlib.pyplot as plt

def _parser_(serialized_example):
    features={'image': tf.FixedLenFeature([], tf.string),
              'label': tf.FixedLenFeature([], tf.string)}
    example = tf.parse_single_example(serialized_example, features)
    image_raw = tf.decode_raw(example['image'], tf.uint8)
    label_raw = tf.decode_raw(example["label"], tf.float32)
    image = tf.cast(
        tf.reshape(
            image_raw,
            [
                tf_model.IMAGE_HEIGHT,
                tf_model.IMAGE_WIDTH,
                tf_model.IMAGE_DEPTH
            ]
        ), tf.float32)
    label = tf.reshape(
        label_raw,
        [
            tf_model.GRID_H,
            tf_model.GRID_W,
            tf_model.GRID_DEPTH
        ]
    )
    return image, label

def preprocess2iterator(dataset, batch_size):
    dataset = dataset.shuffle(1000*batch_size)
    dataset = dataset.map(_parser_)
    batch = dataset.batch(batch_size)
    return batch.make_initializable_iterator()

filename="./data/out.recored"
test_filename = "./data/test.recored"
batch_size=100

session = tf.Session()

dataset = tf.data.TFRecordDataset(filename)
test_dataset = tf.data.TFRecordDataset(test_filename)
iterator = preprocess2iterator(dataset, 50)

handle = session.run(iterator.string_handle())

input_handle = tf.placeholder(tf.string)
input_iterator = tf.data.Iterator.from_string_handle(
    input_handle,
    iterator.output_types,
    iterator.output_shapes
)

image, label = input_iterator.get_next()
predict = tf_model.model(image)
obj_loss, no_obj_loss, loss_xy, loss_wh = tf_model.loss(predict, label)
optmz = tf.train.AdamOptimizer(3e-4)
train_op = optmz.minimize(obj_loss + no_obj_loss * 0.7 + 2 * (loss_xy + loss_wh))

test_iterator = preprocess2iterator(test_dataset, 1)
test_handle = session.run(test_iterator.string_handle())

init=tf.global_variables_initializer()
session.run(init)
session.run(iterator.initializer)
session.run(test_iterator.initializer)

loss_collection = []
def train(filename, batch_size, times, show_step, test_step=0):
    for i in range(times):
        try:
            losses = session.run([obj_loss, no_obj_loss, loss_xy, loss_wh, train_op], {input_handle: handle})
            if (i % show_step) == 0:
                loss_collection.append([i, losses[0], losses[1], losses[2], losses[3]])
                print("step: %d\nobj_loss: %f, no_obj_loss: %f, \nloss_xy: %f, loss_wh: %f\n" % (i, losses[0], losses[1], losses[2], losses[3]))

            if not test_step < 1 and i % test_step == 0:
                try:
                    session.run([obj_loss, no_obj_loss, loss_xy, loss_wh, train_op], {input_handle: test_handle})
                    print("test:result: obj_loss: %f, no_obj_loss: %f, \nloss_xy: %f, loss_wh: %f\n" % (losses[0], losses[1], losses[2], losses[3]))
                except tf.errors.OutOfRangeError as e:
                    session.run(test_iterator.initializer)
        except tf.errors.OutOfRangeError as e:
            session.run(iterator.initializer)

    los = session.run(tf.transpose(loss_collection))
    plt.plot(los[0], los[1], "--o")
    plt.plot(los[0], los[2], "--o")
    plt.plot(los[0], los[3], "--o")
    plt.plot(los[0], los[4], "--o")
    plt.ylim(top=1)
    plt.ylim(bottom=0)
    plt.show()

def test():
    loss, img, pp, ll = session.run([[obj_loss, no_obj_loss, loss_xy, loss_wh], image, predict, label], {input_handle: test_handle})
    session.run(tf_model.process_label(pp, 1080, 720, 0.6))
    session.run(tf_model.process_label(ll, 1080, 720, 0.6))

if __name__ == "__main__":
    saver = tf.train.Saver()
    # saver.restore(session, "./model/model.ckpt")
    train(filename, 50, 4000, 10, 1)
    saver.save(session, "./model/model.ckpt")
