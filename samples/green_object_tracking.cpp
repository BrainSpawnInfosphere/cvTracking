#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cvt.h>
#include <cvRender.h>
using namespace cvt;

int main()
{
    Tracks tracks;

    cv::namedWindow("red_object_tracking", CV_WINDOW_AUTOSIZE);

    // open default camera and capture images
    cv::VideoCapture capture(0);
    if(!capture.isOpened()) {
        std::cout<<"Couldn't open camera"<<std::endl;
        exit(-1);
    }

    cv::Mat img; // camera
    cv::Mat segmented; // blob
    cv::Mat frame = cv::Mat(img); // drawing
    cv::Mat hsv;

    capture >> img;
    cv::Mat hue(img.rows,img.cols,CV_8UC1);
    
    // morphological operators
    cv::Mat er_mat = getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));
    cv::Mat di_mat = getStructuringElement(cv::MORPH_RECT,cv::Size(7,7));

    while (capture.isOpened())
    {
        capture >> img;
        img.copyTo(frame);

        // smooth image
        cv::blur(img,img,cv::Size(3,3));

        cvtColor(img,hsv,CV_BGR2HSV);

        // Green chapstick: RGB: 25 95 55 --> HSV[0:360,0:1,0:1]: 146 .74 .36
        cv::inRange(hsv,
                    cv::Scalar(70,150,50),
                    cv::Scalar(75,255,255),
                    hsv);

        int fromTo[] = {0,0};
        mixChannels(&hsv,1,&hue,1,fromTo,1);

        // remove noise and close holes
        cv::dilate(hue,hue,di_mat); // remove noise
        cv::erode(hue,hue,er_mat); // remove holes

        cv::imshow("hue",hue);

        // find and track blobs
        Blobs blobs;
        blobs.getBlobs(hue); //<- [fixme] changes image ... why?
        blobs.filterByArea(50, 10000);
        Render(frame, blobs);
        cvUpdateTracks(blobs, tracks, 200., 5);
        Render(frame, tracks);

        cv::imshow("red_object_tracking", frame);

        char key = cv::waitKey(10);

        switch (key)
        {
        case 27: // exit
            exit(0);
            break;
        case 'p': // print blobs
            std::cout<< "---[ Blobs(" << blobs.size() << ") ]-----"<<std::endl;
            for (Blobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
            {
                std::cout << *it->second << std::endl;
            }
            std::cout<< "--------------------------------------------"<<std::endl;
            break;
        }
    }

    return 0;
}
