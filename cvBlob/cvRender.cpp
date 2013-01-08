
#include "cvRender.h"
#include "cvblob.h"

namespace cvb // namespace_start
{

void cvRenderBlob(cv::Mat& image, CvBlob *blob, const cv::Scalar color){
    cv::drawContours( image, blob->contour, 0, color, CV_FILLED); //2, 8, cv::Vec4i(-1,-1,-1,-1), 0, Point() );
    cv::circle( image, blob->centroid, 4, color, -1, 8, 0 );
}

void cvRenderBlobs(cv::Mat image, CvBlobs &blobs){
    cv::Scalar color = cv::Scalar( 0, 255, 0 );
    std::map<CvLabel,CvBlob*>::iterator it;
    for ( it=blobs.begin(); it != blobs.end(); it++ ) cvRenderBlob(image, (*it).second, color);
}

} // namespace_end


