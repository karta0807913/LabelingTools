#ifndef YOLO_V2_OUTPUT_H
#define YOLO_V2_OUTPUT_H

#ifdef USE_CAFFE_
#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>

namespace yolo_v2{
    class DATA{
    public:
        float confidence;

        float   x,
                y,
                w,
                h;

        int classIndex;

        bool operator<(DATA data);

    private:
        bool operator>(DATA data);
    };

    float sigmoid(double x);
    float getTwoBoxArea(DATA data1, DATA data2); //IOU function

    class BASE_YOLO_2_FUNCTOR {
    public:
        static void run(std::vector<DATA> &ans, std::deque<DATA> &boxDataList, float twoBoxArea, float confidenceLimit){
            std::vector<bool> flags(boxDataList.size());
            for(unsigned int compare = 0; compare < boxDataList.size(); ++compare){
                if(flags[compare])
                    continue;
                for(unsigned int target = 0; target < boxDataList.size(); ++target){
                    float box_iou = getTwoBoxArea(boxDataList[compare], boxDataList[target]);
                    if(flags[target] ||
                            box_iou > twoBoxArea ||
                            (box_iou != 0 && boxDataList[compare].classIndex == boxDataList[target].classIndex)){
                        flags[target] = true;
                        continue;
                    }
                }
                ans.push_back(boxDataList[compare]);
            }
        }
        BASE_YOLO_2_FUNCTOR() = delete;

    };

    class SOFT_NMS {
    public:
        static void run(std::vector<DATA> &ans, std::deque<DATA> &boxDataList, float twoBoxArea, float confidenceLimit){
            std::vector<bool> flags(boxDataList.size());
            for(unsigned int compare = 0; compare < boxDataList.size(); ++compare){
                if(flags[compare])
                    continue;
                for(unsigned int target = 0; target < boxDataList.size(); ++target){
                    auto rescore= [](float confidence, float iou, float twoBoxArea){
                        return confidence*(float)exp((iou*iou/twoBoxArea)*-1);
                    };

                    float box_iou = getTwoBoxArea(boxDataList[compare], boxDataList[target]);
                    if(flags[target] ||
                        (box_iou != 0 && rescore(boxDataList[target].confidence, box_iou, twoBoxArea) < confidenceLimit))
                    {
                        flags[target] = true;
                    }
                }
                ans.push_back(boxDataList[compare]);
            }
        }
        SOFT_NMS() = delete;
    };

    template<int OBJECT_NUM_, int SIDE_, typename FUN = BASE_YOLO_2_FUNCTOR>
    std::vector<DATA> getResult(std::vector<float> &res, std::vector<float> &bias, int classNumber,
                              float confidenceLimit, float twoBoxArea){

        if((int)res.size() != (OBJECT_NUM_) * (classNumber + 5) * SIDE_ * SIDE_){
            std::cout << "vector Number (" << res.size() << ") != request Number (" << OBJECT_NUM_ * (classNumber + 5) * SIDE_ * SIDE_ << ")" << std::endl;
            exit(1);
        }
        if(OBJECT_NUM_ > bias.size() / 2){
            std::cout << "OBJECT NUMBER (" << OBJECT_NUM_ << ") > bias number / 2 (" << bias.size() / 2 << ")" << std::endl;
            exit(1);
        }
        DATA data;
        std::vector<DATA> ans;
        std::deque<DATA> boxDataList;
        const int screen_size = SIDE_ * SIDE_;
        const int box_size = (5 + classNumber) * screen_size;

        for(int side_y = 0; side_y < SIDE_; ++side_y){
            for(int side_x = 0; side_x < SIDE_; ++side_x){
                for(int obj_index = 0; obj_index < OBJECT_NUM_; ++obj_index){
                    int box_index = side_y * SIDE_ + side_x + obj_index * box_size;
                    data.x = ((res[box_index + 0 * screen_size]) + side_x) / SIDE_;
                    data.y = ((res[box_index + 1 * screen_size]) + side_y) / SIDE_;
                    data.w = bias[2 * obj_index + 0] * exp(res[box_index + 2 * screen_size]);
                    data.h = bias[2 * obj_index + 1] * exp(res[box_index + 3 * screen_size]);
                    data.confidence = res[box_index + 4 * screen_size];
                    float biggestNum = 0;
                    for(int classIndex = 0; classIndex < classNumber; ++classIndex){
                        if(biggestNum < res[box_index + (5 + classIndex) * screen_size]){
                            data.classIndex = classIndex;
                            biggestNum = res[box_index + (5 + classIndex) * screen_size];
                        }
                    }
                    if(data.confidence > confidenceLimit)
                        boxDataList.push_back(data);
                }
            }
        }
        std::sort(boxDataList.begin(), boxDataList.end());
        FUN::run(ans, boxDataList, twoBoxArea, confidenceLimit);
        return ans;
    }
}
#else
namespace yolo_v2 {

}
#endif //USE_CAFFE_

#endif // YOLO_V2_OUTPUT_H
