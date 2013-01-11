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
using namespace std;

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cvt.h>
#include <cvRender.h>
using namespace cvt;
using namespace std;

int main()
{
  cv::Mat img = cv::imread("../test/test.png", 1);
  cv::Mat draw;
  img.copyTo(draw);
  //cv::Mat draw(img.rows, img.cols, CV_8UC3);
  //cv::Mat roi = img(cv::Rect(100, 100, 800, 500));
  cv::Mat grey;
  cv::cvtColor(img, grey, CV_BGR2GRAY);
  cv::threshold(grey, grey, 100, 255, CV_THRESH_BINARY);
  
  

  Blobs blobs;
  
  //exit(0);
  
  blobs.getBlobs(grey);
  
  cout<<"blobs: "<<endl;
  
  Blobs::iterator it=blobs.begin();
    while(it!=blobs.end())
    {
        Blob *blob=(*it).second;
        cout<<*blob<<endl;
        ++it;
    }
  
  Render(draw, blobs);

  cv::namedWindow("test", 1);
  cv::imshow("test", draw);
  cv::waitKey(0);

  return 0;
}
