
#include "cvRender.h"
#include "cvt.h"
#include <stdio.h>
#include <sstream>


namespace cvt // namespace_start
{

/** 
 * add: contour option, cm option
 */
void Render(cv::Mat& image, Blob *blob, const cv::Scalar color){
    cv::drawContours( image, blob->contour, 0, color, 2); //CV_FILLED); //2, 8, cv::Vec4i(-1,-1,-1,-1), 0, Point() );

    cv::circle( image, blob->centroid, 4, color, -1, 8, 0 );
}

void Render(cv::Mat& image, Blobs &blobs){
    cv::Scalar color = cv::Scalar( 0, 255, 0 );
    Blobs::iterator it;
    for ( it=blobs.begin(); it != blobs.end(); it++ ) Render(image, (*it).second, color);
}

int font = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;

void Render(cv::Mat& image, Track* track, const cv::Scalar color){
    cv::Rect box = track->bbox;
    cv::rectangle(image,box,color);
    
    cv::Point orig(track->bbox.x,track->bbox.y);
    //cv::Point orig(track->minx + box.width/2,track->miny+box.height/2);
    std::stringstream ss;
    std::string id;
    ss << track->id;
    id = ss.str();
    cv::putText(image,id.c_str(),orig,font,0.5,color);
}

void Render(cv::Mat& image, Tracks& tracks){
    cv::Scalar color = cv::Scalar( 0, 255, 255 );
    std::map<CvID, Track *>::iterator it;
    for ( it=tracks.begin(); it != tracks.end(); it++ ) Render(image, (*it).second, color);
}

} // namespace_end


