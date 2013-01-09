// Copyright (C) 2007 by Cristóbal Carnero Liñán
// grendel.ccl@gmail.com
//
// This file is part of cvBlob.
//
// cvBlob is free software: you can redistribute it and/or modify
// it under the terms of the Lesser GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// cvBlob is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// Lesser GNU General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License
// along with cvBlob.  If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>
#include <string>


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


#include <cvblob.h>
#include <cvRender.h>
using namespace cvb;

int main()
{
  CvTracks tracks;

  cv::namedWindow("test_tracking", CV_WINDOW_AUTOSIZE);
  
  std::string file = "../test/EnterExitCrossingPaths2front_blobs.mpeg";
  cv::VideoCapture capture;
  bool good = capture.open(file.c_str());
  
  if(!good){
    std::cout << "Couldn't load " << file << std::endl;
    exit(-1);
  }

  cv::Mat frame;
  cv::Mat grey;
  
  capture >> frame;

  while (capture.isOpened())
  {
    cvtColor(frame, grey, CV_RGB2GRAY);
    cv::threshold(grey, grey, 100, 200, CV_THRESH_BINARY);

    CvBlobs blobs;
    blobs.getBlobs(grey);
    blobs.cvFilterByArea(500, 1000);

    cvUpdateTracks(blobs, tracks, 5., 10);
    //cvUpdateTracks(blobs, tracks, 10., 5);

    cvRenderBlobs(frame, blobs);
    cvRenderTracks(frame, tracks);

    cv:imshow("test_tracking", frame);

    if ((cvWaitKey(10)&0xff)==27)
      break;
      
    capture >> frame;
  }

  return 0;
}
