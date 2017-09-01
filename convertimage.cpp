#include "convertimage.h"

QImage CvMat2QImage(cv::Mat &image)
{
    QImage qtImg;
    int nChannel=image.channels();

    if( !image.empty() && image.depth() == CV_8U )
    {
        if(nChannel == 3)    // RGB image
        {
            cvtColor(image,image,CV_BGR2RGB);
            qtImg = QImage((const uchar*)(image.data),  //(const unsigned char*)
                         image.cols,image.rows,
                         image.cols*image.channels(),   //new add
                         QImage::Format_RGB888);
        }
        else if(nChannel==4||nChannel==1)
        {
            qtImg = QImage((const unsigned char*)image.data,image.cols,image.rows,QImage::Format_ARGB32);
        }
        else                     // gray image
        {
            qtImg = QImage((const uchar*)(image.data),
                         image.cols,image.rows,
                         image.cols*image.channels(),    //new add
                         QImage::Format_Indexed8);
        }
    }
    return qtImg;
}

QImage IplImage2QImage( IplImage *image )
{
    QImage qtImg;
    if (image && image->depth == IPL_DEPTH_8U && cvGetSize(image).width > 0 )
    {
        if(image->nChannels == 1){
            qtImg = QImage( (const unsigned char *)(image->imageData),
                            image->width,
                            image->height,
                            QImage::Format_Indexed8 );
        }
        else{
            cvCvtColor( image, image, CV_BGR2RGB );
            qtImg = QImage( (const unsigned char *)(image->imageData),
                            image->width,
                            image->height,
                            QImage::Format_RGB888 );
        }
    }
    return qtImg;
}

cv::Mat QImage2CvMat( QImage &image )
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

IplImage *QImage2IplImage( QImage &image )
{
    IplImage *cvImage = NULL;
    switch (image.format()){
    case QImage::Format_RGB888:{
        cvImage = cvCreateImageHeader( cvSize(image.width(),
                                       image.height()), 8, 3 );
        cvSetData( cvImage, image.bits(), image.bytesPerLine() );
        cvCvtColor( cvImage, cvImage, CV_RGB2BGR );
        return cvImage;
    }
    case QImage::Format_Indexed8:{
        cvImage = cvCreateImageHeader( cvSize(image.width(),
                                       image.height()), 8, 1 );
        cvSetData( cvImage, image.bits(), image.bytesPerLine() );
        return cvImage;
    }
    default:
        break;
    }
    return cvImage;
}

cv::Mat fillUpImageToSqure(cv::Mat &img)
{
    cv::Mat reFilledImage;

//    if(img.rows > img.cols)
//    {
//        cv::Mat fillupImg(img.rows, img.rows - img.cols, img.type());
//        fillupImg.setTo(cv::Scalar(0,0,0));
//        cv::hconcat(img, fillupImg, reFilledImage);
//    }
//    else if(img.rows < img.cols)
//    {
//        cv::Mat fillupImg(img.cols - img.rows, img.cols, img.type());
//        fillupImg.setTo(cv::Scalar(0,0,0));
//        cv::vconcat(img, fillupImg, reFilledImage);
//    }
//    else
//    {
//        reFilledImage = img;
//    }

//    if(img.rows == img.cols)
//    {
//        reFilledImage = img;
//    }
//    else
//    {
//        int cols;
//        int rows;


//        cols = (img.rows > img.cols)? (img.rows - img.cols) : 0;
//        rows = (img.cols > img.rows)? (img.cols - img.rows) : 0;

//        cols /= 2;
//        rows /= 2;

//        cv::copyMakeBorder(img, reFilledImage, rows, rows,
//                           cols,  cols, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

//        cv::Point point(0,0);
//        for(int y = 0; y < rows; ++y)
//        {
//            for(int x = 0; x < reFilledImage.cols; ++x)
//            {
//                point.x = x;
//                point.y = y;
//                cv::Vec3b &colorTop = reFilledImage.at<cv::Vec3b>(point);
//                colorTop[0] = qrand() % 255;
//                colorTop[1] = qrand() % 255;
//                colorTop[2] = qrand() % 255;

//                point.x = x;
//                point.y = reFilledImage.rows - 1 - y;
//                cv::Vec3b &colorButton = reFilledImage.at<cv::Vec3b>(point);
//                colorButton[0] = qrand() % 255;
//                colorButton[1] = qrand() % 255;
//                colorButton[2] = qrand() % 255;
//            }
//        }

//        for(int y = 0; y < reFilledImage.rows; ++y)
//        {
//            for(int x = 0; x < cols; ++x)
//            {
//                point.x = x;
//                point.y = y;
//                cv::Vec3b &colorTop = reFilledImage.at<cv::Vec3b>(point);
//                colorTop[0] = qrand() % 255;
//                colorTop[1] = qrand() % 255;
//                colorTop[2] = qrand() % 255;

//                point.x = reFilledImage.cols - 1 - x;
//                point.y = y;
//                cv::Vec3b &colorButton = reFilledImage.at<cv::Vec3b>(point);
//                colorButton[0] = qrand() % 255;
//                colorButton[1] = qrand() % 255;
//                colorButton[2] = qrand() % 255;
//            }
//        }
//    }

    reFilledImage = img;

    return reFilledImage;
}
