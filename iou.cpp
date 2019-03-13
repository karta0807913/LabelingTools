#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <google/protobuf/text_format.h>

#include "netforward.h"
#include "yolo_v2_output.h"
#include <data.pb.h>

const int predictBoxNum = 5;
const int splitScreenNum = 13;

const int classNumber = 8;
const float twoBoxArea = .3f;
const float confidenceLimit = .3f;

std::vector<float> bias = {
                            0.661646461603,
                            0.476997742485,
                            0.13816690521,
                            0.168037784958,
                            0.0469252103135,
                            0.0704064485569,
                            0.249637180906,
                            0.369454411045,
                            0.334808151587,
                            0.813230779006
                            };

const std::vector<std::string> classes = { 
                            "cap" ,
                            "cap_mask" ,
                            "cap_sunglasses" ,
                            "cap_sunglasses_mask" ,
                            "helmet" ,
                            "mask" ,
                            "sunglasses" ,
                            "uncovered"
                            };

const char protoFilePath[]  =
        "/media/2.0TB_Disk/Jay/jay_yolov2/yolo_voc.prototxt";
const char caffeModelPath[] =
        "/media/2.0TB_Disk/Jay/faceDB_0404/model_iter_680000.caffemodel";
const char meanFilePath[]   =
        "/media/2.0TB_Disk/Jay/faceDB_0404/lmdb/faceDB_CAT10_train.binaryproto";

typedef yolo_v2::SOFT_NMS YOLO_FUNCTOR;

using namespace std;
using namespace yolo_v2;

inline bool pathToPictureData(const string &path, RectData::PictureData *pictureData) {
    fstream file(path, fstream::in);
    char c[1024];
    if(!file.is_open())
        return false;
    string tmp;
    while(file) {
        file.read(c, 1024);
        tmp += c;
    }
    google::protobuf::TextFormat::ParseFromString(tmp, pictureData);
    return true;
}

int main(int argc, char**argv) {
    if(argc != 2) {
        cout << "a.out path/to/list" << endl;
    }
    fstream list(argv[1], fstream::in);
    string line;
    map<string, int> classMap;
    for(int index = 0; index < classes.size(); ++index) {
        classMap[classes[index]] = index;
    }
    NetForward net(protoFilePath, caffeModelPath, meanFilePath, caffe::TEST);
    float allAvgIou = 0;
    float allAvgRecall = 0;
    int counter = 0;
    while(getline(list, line)) {
        int pos=line.find_last_of(' ');

        cv::Mat img = cv::imread(line.substr(0, pos));
        if(img.empty()) {
            cout << "can't open image " << line.substr(0, pos) << endl;
            continue;
        }

        RectData::PictureData pictureData;
        if(!pathToPictureData(line.substr(pos + 1), &pictureData)) {
            cout << "can't open prototxt " << line.substr(pos) << endl;
            continue;
        }
        
        if(img.cols != pictureData.width() || img.rows != pictureData.height()) {
            cout << line << endl;
            cout << "img and prototxt may not match, skip" << endl;
            continue;
        }
        
        std::vector<DATA> trueLabel;
        for(google::protobuf::RepeatedPtrField<RectData::ObjectParam>::const_iterator iterator = pictureData.object_parameter().begin();
            iterator != pictureData.object_parameter().end(); ++iterator)
        {
            map<string, int>::iterator index = classMap.find(iterator->tag().Get(0));
            if(index == classMap.end()) {
                cout << "can't find clas " << iterator->tag().Get(0) << endl;
                return 1;
            }
            DATA data;
            data.x = iterator->xmin();
            data.y = iterator->ymin();
            data.w = iterator->xmax();
            data.h = iterator->ymax();
            data.classIndex  = index->second;
            trueLabel.push_back(data);
        }
        std::vector<float> res = net.forward(img);
        std::vector<DATA> finalRes = 
                getResult<predictBoxNum, splitScreenNum, YOLO_FUNCTOR>(res, bias, 
                classNumber, confidenceLimit, twoBoxArea);
        float avgIou = 0;
        float recall = 0;
        for(auto trueBox = trueLabel.begin(); trueBox != trueLabel.end(); ++trueBox) {
            float bestIOU = 0;
            for(int index = 0; index < finalRes.size(); ++index) {
                float iou = getTwoBoxArea(*trueBox, finalRes[index]);
                if(iou > .5 && finalRes[index].classIndex == trueBox->classIndex) {
                    ++recall;
                    bestIOU = iou;
                    finalRes.erase(finalRes.begin() + index);
                }
                if(iou > bestIOU) {
                    bestIOU = iou;
                }
            }
            avgIou += bestIOU;
        }
        avgIou /= trueLabel.size();
        recall /= trueLabel.size();
        cout << "avg iou is " << avgIou <<
                " recall is " << recall << endl;
        counter += 1;
        allAvgIou += avgIou;
        allAvgRecall += recall;
    } 
    allAvgRecall /= counter;
    allAvgIou /= counter;
    cout << "all avg iou is " << allAvgIou <<
            " all avg recall is " << allAvgRecall << endl;
}
