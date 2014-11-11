#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_core249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_imgproc249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_highgui249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_objdetect249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_contrib249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_features2d249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_flann249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_gpu249d.lib")
//#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_haartraining_engined.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_legacy249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_ts249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_video249d.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_nonfree249d.lib")
#else
//Releaseモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_core249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_imgproc249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_highgui249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_objdetect249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_contrib249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_features2d249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_flann249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_gpu249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_haartraining_engined.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_legacy249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_ts249.lib")
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_video249.lib")
#endif

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include<fstream>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)

#if OPENCV_VERSION_CODE>=OPENCV_VERSION(2,4,0)
#include <opencv2/nonfree/features2d.hpp>
#endif


 

using namespace std;
using namespace cv;

class serch_img
{
public:
	Mat img;
	vector<KeyPoint> keypoints;
	Mat descriptors;
	String path;
	String name;
	void output_txt();
	void show_window();//画像を出すための関数

};

serch_img SURFDECT(serch_img);



int main(int argc, char* argv[])
{
	
	serch_img test;
	test.path = "C:\\opencv\\samples\\c\\lena.jpg";
	test.name = "lena";
	test = SURFDECT(test);
	test.output_txt();
	test.show_window();
	return 0;
}

//特徴点検出用関数
serch_img SURFDECT(serch_img test)
{
	initModule_nonfree();
	test.img = imread(test.path);

	cv::Mat gray_img;
	cv::cvtColor(test.img, gray_img, CV_BGR2GRAY);
	cv::normalize(gray_img, gray_img, 0, 255, cv::NORM_MINMAX);

	
	std::vector<cv::KeyPoint>::iterator itk;
	cv::Mat descriptors;
	
	// SURF 検出器に基づく特徴点検出
	// hessianThreshold=4500, 
	cv::SurfFeatureDetector detector(4500);
	cv::Scalar color(100, 255, 50);
	detector.detect(gray_img, test.keypoints);
	// SURF に基づくディスクリプタ抽出器
	cv::SurfDescriptorExtractor extractor;
	extractor.compute(gray_img, test.keypoints, test.descriptors);
	for (itk = test.keypoints.begin(); itk != test.keypoints.end(); ++itk) {
		cv::circle(test.img, itk->pt, 1, color, -1);
		cv::circle(test.img, itk->pt, itk->size, color, 1, CV_AA);
		if (itk->angle >= 0) {
			cv::Point pt2(itk->pt.x + cos(itk->angle)*itk->size, itk->pt.y + sin(itk->angle)*itk->size);
			cv::line(test.img, itk->pt, pt2, color, 1, CV_AA);
		}
	}
	
	

	
	
	return test;
}

void serch_img::output_txt()
{
	String imgname = name + ".txt";
	ofstream ofs1(imgname);
	// 64次元の特徴量 x keypoint数
	for (int i = 0; i<descriptors.rows; ++i) {
		cv::Mat d(descriptors, cv::Rect(0, i, descriptors.cols, 1));
		ofs1 << i << ": " << d << std::endl;
	}

}

void serch_img::show_window()
{
	cv::namedWindow(name, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow(name, img);
	waitKey(0);
}