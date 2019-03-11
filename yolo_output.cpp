#ifdef USE_CAFFE_
#include "yolo_output.h"

using namespace std;

namespace yolo{
    void printRowData(std::vector<float> res, int splitScreenNum)
    {
        int counter = 0;
        for(auto iterator = res.begin(); iterator != res.end(); ++iterator)
        {
            std::cout << *iterator << ' ';
            if(++counter % splitScreenNum == 0){
                std::cout << std::endl;
                if(counter % splitScreenNum * splitScreenNum == 0){
                    std::cout << std::endl;
                    counter = 0;
                }
            }
        }
    }

    void printData(QList<DATA> dataList, std::string *classes)
    {
        for(auto iterator = dataList.begin(); iterator != dataList.end(); ++iterator)
        {
            float w = *iterator->w;
            float h = *iterator->h;
            float x = *iterator->x - w / 2;
            float y = *iterator->y - h / 2;
            w += x;
            h += y;
            if(x < 0)
                x = 0;
            if(y < 0)
                y = 0;
            std::cout <<
                classes[iterator->classIndex] << ' ' <<
                x << ' ' <<
                y << ' ' <<
                w << ' ' <<
                h << ' ' <<
                iterator->confidence << std::endl;
        }
        cout << endl;
    }

    bool sortByConfidence(const DATA &data1, const DATA &data2)
    {
        return (data1.confidence) > (data2.confidence);
    }

    float getTwoBoxArea(DATA data1, DATA data2)
    {
        float data1_w = *data1.w;
        float data1_h = *data1.h;
        float data1_x = *data1.x - data1_w / 2;
        float data1_y = *data1.y - data1_h / 2;
        if(data1_x < 0)
            data1_x = 0;
        if(data1_y < 0)
            data1_y = 0;

        float data2_w = *data2.w;
        float data2_h = *data2.h;
        float data2_x = *data2.x - data2_w / 2;
        float data2_y = *data2.y - data2_h / 2;
        if(data2_x < 0)
            data2_x = 0;
        if(data2_y < 0)
            data2_y = 0;

        float endx = std::max(data1_x + data1_w, data2_x + data2_w);
        float startx = std::min(data1_x, data2_x);
        float width = data1_w + data2_w - (endx - startx);

        float endy = std::max(data1_y + data1_h, data2_y + data2_h);
        float starty = std::min(data1_y, data2_y);
        float height = data1_h + data2_h - (endy - starty);

        if (width <= 0 || height <= 0)
            return 0;
        else
        {
            float area = width * height;
            float area1 = data1_w * data1_h;
            float area2 = data2_w * data2_h;
            return area / std::min(area1, area2);
        }
    }

    QList<DATA> getList(std::vector<float> &res, int classNumber,
                        int predictBoxNum, int splitScreenNum)
    {
        if((int)res.size() != splitScreenNum * splitScreenNum * (classNumber + 5 * predictBoxNum))
        {
            std::cout << "Error!!!, Yolo Output(" << (int)res.size()
                      << ") != Your Input Num("
                      << splitScreenNum * splitScreenNum * (classNumber + 5 * predictBoxNum)
                      << ")" << std::endl;
            exit(1);
        }
        QList<DATA> dataList;

        int offset_x = 0;
        int offset_y = 0;
        int powScreenNum = splitScreenNum * splitScreenNum;
        for(int row = 0; row < powScreenNum; ++row){
            DATA data;
            float max = 0;
            int index = 0;
            for(int classIndex = 0; classIndex < classNumber; ++classIndex)
            {
                if(max < res[classIndex * powScreenNum + row])
                {
                    max = res[classIndex * powScreenNum + row];
                    index = classIndex;
                }
            }


            for(int i = 0; i < predictBoxNum; ++i)
            {
                data.classIndex = index;
                data.confidence = res[(classNumber + i) * powScreenNum + row];

                int parIndex = classNumber + i * 4 + predictBoxNum;

                res[(parIndex + 0) * powScreenNum + row] += offset_x;
                res[(parIndex + 1) * powScreenNum + row] += offset_y;

                res[(parIndex + 0) * powScreenNum + row] /= splitScreenNum;
                res[(parIndex + 1) * powScreenNum + row] /= splitScreenNum;

                res[(parIndex + 2) * powScreenNum + row] *= res[(parIndex + 2) * powScreenNum + row];
                res[(parIndex + 3) * powScreenNum + row] *= res[(parIndex + 3) * powScreenNum + row];

                data.x = &res[(parIndex + 0) * powScreenNum + row];
                data.y = &res[(parIndex + 1) * powScreenNum + row];
                data.w = &res[(parIndex + 2) * powScreenNum + row];
                data.h = &res[(parIndex + 3) * powScreenNum + row];

                dataList.push_back(data);
            }

            if(++offset_x % splitScreenNum  == 0)
            {
                offset_x = 0;
                ++offset_y;
            }

        }

        return dataList;
    }


    void getPredictBoxes(QList<DATA> &dataList, float confidenceLimit, float twoBoxArea)
    {
        qSort(dataList.begin(), dataList.end(), sortByConfidence);

        for(int index = 0; index < dataList.length();)
        {
            if(dataList[index].confidence < confidenceLimit){ dataList.removeAt(index); continue; }
            for(int compare = index + 1; compare < dataList.length();)
            {
                float area = getTwoBoxArea(dataList[index], dataList[compare]);
                if(area > twoBoxArea){
                    dataList.removeAt(compare);
                    continue;
                }
                ++compare;
            }
            ++index;
        }
    }
}
#endif
