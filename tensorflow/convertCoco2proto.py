import json
import sys
import os
import data_pb2
import google.protobuf.text_format

def convert(json_file, output_dir):
    os.makedirs(output_dir, exist_ok=True)
    jj = json.load(open(json_file, "r"))
    images = {}
    for image in jj["images"]:
        label = data_pb2.PictureData()
        label.height = image["width"]
        label.width = image["width"]
        file_name = image["file_name"]
        image_filename = os.path.basename(file_name)
        image_name = os.path.splitext(image_filename)[0]
        images[image["id"]] = [label, image_name + ".prototxt"]

    for annotation in jj["annotations"]:
        label = images[annotation["image_id"]][0]
        bbox = annotation["bbox"]
        box = label.object_parameter.add()
        box.tag.append(str(annotation["category_id"]))
        box.xmin = int(bbox[0])
        box.ymin = int(bbox[1])
        box.xmax = int(bbox[0] + bbox[2])
        box.ymax = int(bbox[1] + bbox[3])

    for img_id in images:
        label, proto = images[img_id]
        open(os.path.join(output_dir, proto), "w").write(google.protobuf.text_format.MessageToString(label))

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("usage %s <json> <out>" % sys.argv[0])
        exit(1)
    convert(sys.argv[1], sys.argv[2])

