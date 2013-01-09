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

#if (defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__))
    #include <opencv.h>
    #include <highgui.h>
#else
    #include <opencv2/opencv.hpp>
    #include <opencv2/highgui/highgui.hpp>
#endif

#include <cvblob.h>
#include <cvRender.h>
using namespace cvb;
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
  
  

  CvBlobs blobs;
  
  //exit(0);
  
  blobs.getBlobs(grey);
  
  cout<<"blobs: "<<endl;
  
  CvBlobs::iterator it=blobs.begin();
    while(it!=blobs.end())
    {
        CvBlob *blob=(*it).second;
        cout<<*blob<<endl;
        ++it;
    }
  
  //exit(0);

  cvRenderBlobs(draw, blobs);

  //unsigned int i = 0;

  // Render contours:
  //for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
  //{
    //cvRenderBlob(labelImg, (*it).second, img, imgOut);
    
    //CvScalar meanColor = cvBlobMeanColor((*it).second, labelImg, img);
    //cout << "Mean color: r=" << (unsigned int)meanColor.val[0] << ", g=" << (unsigned int)meanColor.val[1] << ", b=" << (unsigned int)meanColor.val[2] << endl;

    //CvContourPolygon *polygon = cvConvertChainCodesToPolygon(&(*it).second->contour);

    //CvContourPolygon *sPolygon = cvSimplifyPolygon(polygon, 10.);
    //CvContourPolygon *cPolygon = cvPolygonContourConvexHull(sPolygon);

    //cvRenderContourChainCode(&(*it).second->contour, imgOut);
    //cvRenderContourPolygon(sPolygon, imgOut, CV_RGB(0, 0, 255));
    //cvRenderContourPolygon(cPolygon, imgOut, CV_RGB(0, 255, 0));

    //delete cPolygon;
    //delete sPolygon;
    //delete polygon;

    // Render internal contours:
    //for (CvContoursChainCode::const_iterator jt=(*it).second->internalContours.begin(); jt!=(*it).second->internalContours.end(); ++jt)
      //cvRenderContourChainCode((*jt), imgOut);

    //stringstream filename;
    //filename << "blob_" << setw(2) << setfill('0') << i++ << ".png";
    //cvSaveImageBlob(filename.str().c_str(), imgOut, (*it).second);
  //}

  cv::namedWindow("test", 1);
  cv::imshow("test", draw);
  cv::waitKey(0);

  return 0;
}
