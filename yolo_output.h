#ifndef YOLO_OUTPUT_H
#define YOLO_OUTPUT_H

#ifdef USE_CAFFE_
#include <iostream>
#include <vector>
#include <QList>

namespace yolo{
    struct DATA{
        float confidence;

        float   *x,
                *y,
                *w,
                *h;

        int classIndex;

        bool operator==(const DATA &data)
        {
            return this->confidence == data.confidence;
        }
    };

    void printRowData(std::vector<float> res, int splitScreenNum); // if you want to see the row data, call this

    QList<DATA> getList(std::vector<float> &res, int classNumber,
                        int predictBoxNum, int splitScreenNum); // net->forward(), call this to get the row predict box
    void printData(QList<DATA> dataList, std::string *classes); // print all row predict box

    void getPredictBoxes(QList<DATA> &dataList, float confidenceLimit, float twoBoxArea); // get final predict box

    bool sortByConfidence(const DATA &data1, const DATA &data2); //sort function
    float getTwoBoxArea(DATA data1, DATA data2); //IOU function

}
#else
namespace yolo {

}
#endif
#endif // YOLO_OUTPUT_H
