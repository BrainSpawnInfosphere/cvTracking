
#ifndef __CVBLOB_RENDER_H__
#define __CVBLOB_RENDER_H__


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "cvblob.h"

namespace cvb // namespace_start
{

void cvRenderBlob(const CvBlob *blob, cv::Mat *image, cv::Scalar color);

void cvRenderBlobs(const cv::Mat *image, CvBlobs &blobs);

} // namespace_end

#endif
