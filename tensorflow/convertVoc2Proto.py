import xml.etree.ElementTree as ET
import google.protobuf.text_format
import data_pb2
import sys
import os

def make_proto(input_dir, output_dir):
    os.makedirs(output_dir, exist_ok=True)
    folders = os.listdir(input_dir)
    xml_list = []
    for xml in folders:
        print(xml)
        tree = ET.parse(os.path.join(input_dir, xml))
        root = tree.getroot()
        label = data_pb2.PictureData()
        size = root.find("size")
        label.height = int(size.find("height").text)
        label.width = int(size.find("width").text)
        for obj in root.findall("object"):
            rect = label.object_parameter.add()
            box = obj.find("bndbox")
            rect.tag.append(obj.find("name").text)
            rect.xmin = int(box.find("xmin").text)
            rect.ymin = int(box.find("ymin").text)
            rect.xmax = int(box.find("xmax").text)
            rect.ymax = int(box.find("ymax").text)

        proto = os.path.splitext(xml)[0] + ".prototxt"
        open(os.path.join(output_dir, proto), "w").write(google.protobuf.text_format.MessageToString(label))
    return xml_list

if __name__ == "__main__":

    if len(sys.argv) != 3:
        print("usage " + sys.argv[0] + " root_folder out_folder")
    else:
        make_proto(sys.argv[1], sys.argv[2])
