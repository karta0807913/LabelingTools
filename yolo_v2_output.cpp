#ifdef USE_CAFFE_
#include "yolo_v2_output.h"
#include <iostream>
namespace yolo_v2{

    float sigmoid(double x)
    {
        return 1 / (1 + exp(-x));
    }

    template<typename Dtype>
    Dtype Overlap(Dtype x1, Dtype w1, Dtype x2, Dtype w2) {
      Dtype left = std::max(x1, x2);
      Dtype right = std::min(x1 + w1, x2 + w2);
      return right - left;
    }

    bool DATA::operator<(DATA data)
    {
        return this->confidence > data.confidence;
    }

    float getTwoBoxArea(DATA data1, DATA data2)
    {
        float width = Overlap(data1.x, data1.w,
                          data2.x, data2.w);

        float height = Overlap(data1.y, data1.h,
                          data2.y, data2.h);

        if (width <= 0 || height <= 0)
            return 0;
        else
        {
            float area = width * height;
            float area1 = data1.w * data1.h;
            float area2 = data2.w * data2.h;
            return area / ((area1 + area2) - area);
        }
    }
}
#endif
