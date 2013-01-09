
#include "cvRender.h"
#include "cvblob.h"
#include <stdio.h>
#include <sstream>


namespace cvb // namespace_start
{

/** 
 * add: contour option, cm option
 */
void cvRenderBlob(cv::Mat& image, CvBlob *blob, const cv::Scalar color){
    cv::drawContours( image, blob->contour, 0, color, 2); //CV_FILLED); //2, 8, cv::Vec4i(-1,-1,-1,-1), 0, Point() );

    cv::circle( image, blob->centroid, 4, color, -1, 8, 0 );
}

void cvRenderBlobs(cv::Mat& image, CvBlobs &blobs){
    cv::Scalar color = cv::Scalar( 0, 255, 0 );
    CvBlobs::iterator it;
    for ( it=blobs.begin(); it != blobs.end(); it++ ) cvRenderBlob(image, (*it).second, color);
}

int font = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;

void cvRenderTrack(cv::Mat& image, CvTrack* track, cv::Scalar color){
    cv::Rect box(track->minx,track->miny,track->maxx-track->minx,track->maxy-track->miny);
    cv::rectangle(image,box,color);
    
    cv::Point orig(track->minx,track->miny);
    //cv::Point orig(track->minx + box.width/2,track->miny+box.height/2);
    std::stringstream ss;
    std::string id;
    ss << track->id;
    id = ss.str();
    cv::putText(image,id.c_str(),orig,font,0.5,color);
}

void cvRenderTracks(cv::Mat& image, CvTracks& tracks){
    cv::Scalar color = cv::Scalar( 0, 255, 255 );
    std::map<CvID, CvTrack *>::iterator it;
    for ( it=tracks.begin(); it != tracks.end(); it++ ) cvRenderTrack(image, (*it).second, color);
}

} // namespace_end


