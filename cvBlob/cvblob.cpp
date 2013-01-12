// Copyright (C) 2007 by Cristóbal Carnero Liñán
// grendel.ccl@gmail.com
//
// This file is part of Blob.
//
// Blob is free software: you can redistribute it and/or modify
// it under the terms of the Lesser GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Blob is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// Lesser GNU General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License
// along with Blob.  If not, see <http://www.gnu.org/licenses/>.
//

#include <cmath>
#include <iostream>
using namespace std;

#include "cvt.h"

namespace cvt
{

bool Blobs::getBlobs(const cv::Mat& image){
  vector<vector<cv::Point> > contours;
  vector<cv::Vec4i> hierarchy;
  Label currentLabel = 0;
  
    /// Find contours
  cv::findContours( image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    if( contours.size() == 0) return false; // nothing found, exit
    
    for(unsigned int i = 0; i < contours.size(); i++ ){
        Blob* blob = new Blob;
        
        cv::Moments m = cv::moments(contours[i],false); // moments
        Label l = currentLabel++;
        std::vector<cv::Point> contours_poly;
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly, 3, true );
        cv::Rect r = cv::boundingRect( cv::Mat(contours_poly) );

        blob->init(m,l,r);
        std::vector<cv::Point> v;
        for(unsigned int j=0;j< contours[i].size();++j) v.push_back(contours[i][j]);
        blob->contour.push_back(v);
        
        // need to push into map!!
        insert( pair<Label,Blob*>(l,blob) );
    }
    
    return true;
}

Label Blobs::largestBlob(void)
{
    Label label=0;
    unsigned int maxArea=0;

    for (Blobs::const_iterator it=begin(); it!=end(); ++it)
    {
        Blob *blob=(*it).second;

        if (blob->getArea() > maxArea)
        {
            label=blob->label;
            maxArea=blob->getArea();
        }
    }

    return label;
}

void Blobs::filterByArea(unsigned int minArea, unsigned int maxArea)
{
    Blobs::iterator it=begin();
    while(it!=end())
    {
        Blob *blob=(*it).second;
        if ((blob->getArea()<minArea)||(blob->getArea()>maxArea))
        {
            Blobs::iterator tmp=it;
            ++it;
            erase(tmp);
        }
        else
            ++it;
    }
}

void Blobs::filterByLabel(Label label)
{
    Blobs::iterator it=begin();
    while(it!=end())
    {
        Blob *blob=(*it).second;
        if (blob->label!=label)
        {
            delete blob;
            Blobs::iterator tmp=it;
            ++it;
            erase(tmp);
        }
        else
            ++it;
    }
}


// Returns radians
double Blob::angle(void)
{
    return 0.5*atan2(2.0*moment.mu11,(moment.mu20-moment.mu02));
}

}

ostream& operator<< (ostream& output, const cvt::Blob& b)
{
    output << b.label << ": " << b.getArea() << ", (" << b.centroid.x << ", " << b.centroid.y << "), [(" << b.bbox.x << ", " << b.bbox.y << ") - (" << b.bbox.width << ", " << b.bbox.height << ")]";

    return output;
}
