#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cvt.h>
#include <cvRender.h>
using namespace cvt;

int main()
{
	Tracks tracks;
	// vector<vector<Point> > contours;
	// vector<Vec4i> hierarchy;

	cv::namedWindow("red_object_tracking", CV_WINDOW_AUTOSIZE);

	// open default camera and capture images
	cv::VideoCapture capture(0);
	if(!capture.isOpened()) {
		std::cout<<"Couldn't open camera"<<std::endl;
		exit(-1);
	}
	capture.set(CV_CAP_PROP_FRAME_WIDTH,320);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);

	cv::Mat img; // camera
	cv::Mat segmented; // blob
	cv::Mat frame = cv::Mat(img); // drawing
	cv::Mat hsv;

	capture >> img;
	cv::Mat hue(img.rows,img.cols,CV_8UC1);

	// morphological operators
	cv::Mat er_mat = getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));
	cv::Mat di_mat = getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));

	std::cout<<"Press ESC to quit"<<std::endl;

	while (capture.isOpened())
	{
		capture >> img;
		img.copyTo(frame);  // save a copy for later

		// smooth image
		cv::blur(img,img,cv::Size(3,3));

		cvtColor(img,hsv,CV_BGR2HSV);

		// Green: RGB --> HSV[0:180, 0:255, 0:255]
		cv::inRange(hsv,
					// cv::Scalar(70,150,50),
					// cv::Scalar(75,255,255),
					cv::Scalar(29, 86, 6),
					cv::Scalar(64, 255, 255),
					hsv);

		int fromTo[] = {0,0};
		mixChannels(&hsv,1,&hue,1,fromTo,1);

		// remove noise and close holes
		cv::dilate(hue,hue,di_mat); // remove noise
		cv::erode(hue,hue,er_mat); // remove holes

		cv::imshow("1. hue after clean-up",hue);

		// find and track blobs
		// Blobs blobs;
		// blobs.getBlobs(hue); //<- [fixme] changes image ... why?
		// blobs.filterByArea(50, 10000);
		// Render(frame, blobs);
		// cvUpdateTracks(blobs, tracks, 200., 5);
		// Render(frame, tracks);
		using namespace cv;
		using namespace std;
		RNG rng(12345);
		int thresh = 100;
		int max_thresh = 255;
		cv::Mat canny_output;
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::Canny( hue, canny_output, thresh, thresh*2, 3 );
		cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

		// vector<Moments> mu(contours.size() );
		// for( size_t i = 0; i < contours.size(); i++ ){
		// 	mu[i] = moments( contours[i], false );
		// }
		// vector<Point2f> mc( contours.size() );
		//
		// for( size_t i = 0; i < contours.size(); i++ ){
		// 	mc[i] = Point2f(
		// 		static_cast<float>(mu[i].m10/mu[i].m00),
		// 		static_cast<float>(mu[i].m01/mu[i].m00)
		// 	);
		// }
		//
		// Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
		// for( size_t i = 0; i< contours.size(); i++ ){
		// 	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		// 	drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
		// 	circle( drawing, mc[i], 4, color, -1, 8, 0 );
		// }
		// imshow( "Contours", drawing );

		cv::imshow("2. hue after blob stuff",canny_output);

		cv::imshow("red_object_tracking", frame);

		char key = cv::waitKey(10);

		switch (key)
		{
		case 27: // exit
			exit(0);
			break;
		case 'p': // print blobs
			// std::cout<< "---[ Blobs(" << blobs.size() << ") ]-----"<<std::endl;
			// for (Blobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
			// {
			// 	std::cout << *it->second << std::endl;
			// }
			// std::cout<< "--------------------------------------------"<<std::endl;
			break;
		}
	}

	return 0;
}
