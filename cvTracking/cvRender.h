
#ifndef __CVT_RENDER_H__
#define __CVT_RENDER_H__


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


#include "cvt.h"

namespace cvt // namespace_start
{
// Blobs
void Render(cv::Mat& image, Blob *blob,  cv::Scalar color);
void Render(cv::Mat& image, Blobs& blobs);

// Tracks
void Render(cv::Mat& image, Tracks& tracks, cv::Scalar color);
void Render(cv::Mat& image, Tracks& tracks);

} // namespace_end

#endif
