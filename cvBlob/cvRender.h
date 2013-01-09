
#ifndef __CVB_RENDER_H__
#define __CVB_RENDER_H__


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "cvblob.h"

namespace cvb // namespace_start
{
// Blobs
void cvRenderBlob(const CvBlob *blob, cv::Mat& image, cv::Scalar color);
void cvRenderBlobs(cv::Mat& image, CvBlobs& blobs);

// Tracks
void cvRenderTracks(cv::Mat& image, CvTracks& tracks, cv::Scalar color);
void cvRenderTracks(cv::Mat& image, CvTracks& tracks);

} // namespace_end

#endif
