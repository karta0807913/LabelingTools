#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QtGui/QImage>

QImage  CvMat2QImage( cv::Mat &image );
QImage  IplImage2QImage( IplImage *image );
cv::Mat QImage2CvMat( QImage &image );
IplImage *QImage2IplImage( QImage &image );
cv::Mat fillUpImageToSqure(cv::Mat &img);

#endif // CONVERTIMAGE_H
