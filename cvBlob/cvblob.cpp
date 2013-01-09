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

#include <cmath>
#include <iostream>
using namespace std;

#include "cvblob.h"

namespace cvb
{

//CvLabel CvBlobs::currentLabel = 0;

bool CvBlobs::getBlobs(const cv::Mat& image){
  vector<vector<cv::Point> > contours;
  vector<cv::Vec4i> hierarchy;
  CvLabel currentLabel = 0;
  
    /// Find contours
  cv::findContours( image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    if( contours.size() == 0) return false; // nothing found, exit
    
    for(unsigned int i = 0; i < contours.size(); i++ ){
        CvBlob* blob = new CvBlob;
        
        cv::Moments m = cv::moments(contours[i],false); // moments
        CvLabel l = currentLabel++;
        std::vector<cv::Point> contours_poly;
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly, 3, true );
        cv::Rect r = cv::boundingRect( cv::Mat(contours_poly) );

        blob->init(m,l,r);
        std::vector<cv::Point> v;
        for(unsigned int j=0;j< contours[i].size();++j) v.push_back(contours[i][j]);
        blob->contour.push_back(v);
        
        // need to push into map!!
        insert( pair<CvLabel,CvBlob*>(l,blob) );
    }
    
    return true;
}

CvLabel CvBlobs::cvLargestBlob(void)
{
    CvLabel label=0;
    unsigned int maxArea=0;

    for (CvBlobs::const_iterator it=begin(); it!=end(); ++it)
    {
        CvBlob *blob=(*it).second;

        if (blob->getArea() > maxArea)
        {
            label=blob->label;
            maxArea=blob->getArea();
        }
    }

    return label;
}

void CvBlobs::cvFilterByArea(unsigned int minArea, unsigned int maxArea)
{
    CvBlobs::iterator it=begin();
    while(it!=end())
    {
        CvBlob *blob=(*it).second;
        if ((blob->getArea()<minArea)||(blob->getArea()>maxArea))
        {
            CvBlobs::iterator tmp=it;
            ++it;
            erase(tmp);
        }
        else
            ++it;
    }
}

void CvBlobs::cvFilterByLabel(CvLabel label)
{
    CvBlobs::iterator it=begin();
    while(it!=end())
    {
        CvBlob *blob=(*it).second;
        if (blob->label!=label)
        {
            delete blob;
            CvBlobs::iterator tmp=it;
            ++it;
            erase(tmp);
        }
        else
            ++it;
    }
}


// Returns radians
double CvBlob::cvAngle(void)
{
    CV_FUNCNAME("cvAngle");
    __CV_BEGIN__;

    return 0.5*atan2(2.0*moment.mu11,(moment.mu20-moment.mu02));

    __CV_END__;
}

}

ostream& operator<< (ostream& output, const cvb::CvBlob& b)
{
    output << b.label << ": " << b.getArea() << ", (" << b.centroid.x << ", " << b.centroid.y << "), [(" << b.minx << ", " << b.miny << ") - (" << b.maxx << ", " << b.maxy << ")] contours:"<<b.contour.size();

    return output;
}
