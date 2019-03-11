#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include <QtGui/QImage>

QImage  Mat2QImage( cv::Mat &image );
cv::Mat QImage2Mat( QImage &image );
cv::Mat fillUpImageToSqure(cv::Mat &img);

#endif // CONVERTIMAGE_H
