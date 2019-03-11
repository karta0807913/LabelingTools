#include "mainwindow.h"
#include <QApplication>
#include <QTcpSocket>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();

	//QTcpSocket socket(&w);
	//socket.connectToHost("127.0.0.1", 14333);
	//unsigned int width = 1;
	//unsigned int height = 1;

	//cv::Mat img = cv::imread("D:\\gitProject\\LabelingTools\\tensorflow\\test.jpg", cv::IMREAD_COLOR);
	//if (socket.waitForConnected()) {
	//	socket.write((const char *)&width, sizeof(unsigned int) / sizeof(const char));
	//	socket.write((const char *)&height, sizeof(unsigned int) / sizeof(const char));
	//	socket.write((const char *)img.data, img.cols * img.rows * img.elemSize());
	//	socket.flush();
	//}
	//if (socket.waitForReadyRead()) {
	//	QByteArray data = socket.readAll();
	//	auto float_data = (float *)data.data();
	//	for (int i = 0; i < data.length() / sizeof(float) / 4; ++i) {
	//		std::cout << float_data[i * 4 + 0] << std::endl;
	//		std::cout << float_data[i * 4 + 1] << std::endl;
	//		std::cout << float_data[i * 4 + 2] << std::endl;
	//		std::cout << float_data[i * 4 + 3] << std::endl;
	//	}
	//}
}
