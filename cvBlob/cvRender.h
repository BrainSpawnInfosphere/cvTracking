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

/// \file cvblob.h
/// \brief OpenCV Blob header file.


#ifndef __CVBLOB_RENDER_H__
#define __CVBLOB_RENDER_H__

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <limits>

#if (defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__))
    #include <opencv.h>
    #include <highgui.h>
#else
    #include <opencv2/opencv.hpp>
    #include <opencv2/highgui/highgui.hpp>
#endif

#include "cvblob.h"

namespace cvb // namespace_start
{


  /// \fn void cvRenderContourChainCode(CvContourChainCode const *contour, IplImage const *img, CvScalar const &color=CV_RGB(255, 255, 255))
  /// \brief Draw a contour.
  /// \param contour Chain code contour.
  /// \param img Image to draw on.
  /// \param color Color to draw (default, white).
  /// \see CvContourChainCode
void cvRenderContourChainCode(CvContourChainCode const *contour, IplImage const *img, CvScalar const &color=CV_RGB(255, 255, 255));
  

  /// \fn void cvRenderContourPolygon(CvContourPolygon const *contour, IplImage *img, CvScalar const &color=CV_RGB(255, 255, 255))
  /// \brief Draw a polygon.
  /// \param contour Polygon contour.
  /// \param img Image to draw on.
  /// \param color Color to draw (default, white).
  /// \see CvContourPolygon
void cvRenderContourPolygon(CvContourPolygon const *contour, IplImage *img, CvScalar const &color=CV_RGB(255, 255, 255));

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Blobs

  /// \fn unsigned int cvLabel (IplImage const *img, IplImage *imgOut, CvBlobs &blobs);
  /// \brief Label the connected parts of a binary image.
  /// Algorithm based on paper "A linear-time component-labeling algorithm using contour tracing technique" of Fu Chang, Chun-Jen Chen and Chi-Jen Lu.
  /// \param img Input binary image (depth=IPL_DEPTH_8U and num. channels=1).
  /// \param imgOut Output image (depth=IPL_DEPTH_LABEL and num. channels=1).
  /// \param blobs List of blobs.
  /// \return Number of pixels that has been labeled.
unsigned int cvLabel (IplImage const *img, IplImage *imgOut, CvBlobs &blobs);

  //IplImage *cvFilterLabel(IplImage *imgIn, CvLabel label);

  /// \fn void cvFilterLabels(IplImage *imgIn, IplImage *imgOut, const CvBlobs &blobs)
  /// \brief Draw a binary image with the blobs that have been given.
  /// \param imgIn Input image (depth=IPL_DEPTH_LABEL and num. channels=1).
  /// \param imgOut Output binary image (depth=IPL_DEPTH_8U and num. channels=1).
  /// \param blobs List of blobs to be drawn.
  /// \see cvLabel
void cvFilterLabels(IplImage *imgIn, IplImage *imgOut, const CvBlobs &blobs);

  /// \fn CvLabel cvGetLabel(IplImage const *img, unsigned int x, unsigned int y)
  /// \brief Get the label value from a labeled image.
  /// \param img Label image.
  /// \param x X coordenate.
  /// \param y Y coordenate.
  /// \return Label value.
  /// \see CvLabel
CvLabel cvGetLabel(IplImage const *img, unsigned int x, unsigned int y);

  /// \fn cvSaveImageBlob(const char *filename, IplImage *img, CvBlob const *blob)
  /// \brief Save the image of a blob to a file.
  /// The function uses an image (that can be the original pre-processed image or a processed one, or even the result of cvRenderBlobs, for example) and a blob structure.
  /// Then the function saves a copy of the part of the image where the blob is.
  /// \param filename Name of the file.
  /// \param img Image.
  /// \param blob Blob.
  /// \see CvBlob
  /// \see cvRenderBlob
  void cvSaveImageBlob(const char *filename, IplImage *img, CvBlob const *blob);
  
#define CV_BLOB_RENDER_COLOR            0x0001 ///< Render each blog with a different color. \see cvRenderBlobs
#define CV_BLOB_RENDER_CENTROID         0x0002 ///< Render centroid. \see cvRenderBlobs
#define CV_BLOB_RENDER_BOUNDING_BOX     0x0004 ///< Render bounding box. \see cvRenderBlobs
#define CV_BLOB_RENDER_ANGLE            0x0008 ///< Render angle. \see cvRenderBlobs
#define CV_BLOB_RENDER_TO_LOG           0x0010 ///< Print blob data to log out. \see cvRenderBlobs
#define CV_BLOB_RENDER_TO_STD           0x0020 ///< Print blob data to std out. \see cvRenderBlobs

  /// \fn void cvRenderBlob(const IplImage *imgLabel, CvBlob *blob, IplImage *imgSource, IplImage *imgDest, unsigned short mode=0x000f, CvScalar const &color=CV_RGB(255, 255, 255), double alpha=1.)
  /// \brief Draws or prints information about a blob.
  /// \param imgLabel Label image (depth=IPL_DEPTH_LABEL and num. channels=1).
  /// \param blob Blob.
  /// \param imgSource Input image (depth=IPL_DEPTH_8U and num. channels=3).
  /// \param imgDest Output image (depth=IPL_DEPTH_8U and num. channels=3).
  /// \param mode Render mode. By default is CV_BLOB_RENDER_COLOR|CV_BLOB_RENDER_CENTROID|CV_BLOB_RENDER_BOUNDING_BOX|CV_BLOB_RENDER_ANGLE.
  /// \param color Color to render (if CV_BLOB_RENDER_COLOR is used).
  /// \param alpha If mode CV_BLOB_RENDER_COLOR is used. 1.0 indicates opaque and 0.0 translucent (1.0 by default).
  /// \see CV_BLOB_RENDER_COLOR
  /// \see CV_BLOB_RENDER_CENTROID
  /// \see CV_BLOB_RENDER_BOUNDING_BOX
  /// \see CV_BLOB_RENDER_ANGLE
  /// \see CV_BLOB_RENDER_TO_LOG
  /// \see CV_BLOB_RENDER_TO_STD
  void cvRenderBlob(const IplImage *imgLabel, CvBlob *blob, IplImage *imgSource, IplImage *imgDest, unsigned short mode=0x000f, CvScalar const &color=CV_RGB(255, 255, 255), double alpha=1.);

  /// \fn void cvRenderBlobs(const IplImage *imgLabel, CvBlobs &blobs, IplImage *imgSource, IplImage *imgDest, unsigned short mode=0x000f, double alpha=1.)
  /// \brief Draws or prints information about blobs.
  /// \param imgLabel Label image (depth=IPL_DEPTH_LABEL and num. channels=1).
  /// \param blobs List of blobs.
  /// \param imgSource Input image (depth=IPL_DEPTH_8U and num. channels=3).
  /// \param imgDest Output image (depth=IPL_DEPTH_8U and num. channels=3).
  /// \param mode Render mode. By default is CV_BLOB_RENDER_COLOR|CV_BLOB_RENDER_CENTROID|CV_BLOB_RENDER_BOUNDING_BOX|CV_BLOB_RENDER_ANGLE.
  /// \param alpha If mode CV_BLOB_RENDER_COLOR is used. 1.0 indicates opaque and 0.0 translucent (1.0 by default).
  /// \see CV_BLOB_RENDER_COLOR
  /// \see CV_BLOB_RENDER_CENTROID
  /// \see CV_BLOB_RENDER_BOUNDING_BOX
  /// \see CV_BLOB_RENDER_ANGLE
  /// \see CV_BLOB_RENDER_TO_LOG
  /// \see CV_BLOB_RENDER_TO_STD
  void cvRenderBlobs(const IplImage *imgLabel, CvBlobs &blobs, IplImage *imgSource, IplImage *imgDest, unsigned short mode=0x000f, double alpha=1.);

  /// \fn void cvSetImageROItoBlob(IplImage *img, CvBlob const *blob)
  /// \brief Set the ROI of an image to the bounding box of a blob.
  /// \param img Image.
  /// \param blob Blob.
  /// \see CvBlob
  inline void cvSetImageROItoBlob(IplImage *img, CvBlob const *blob)
  {
    cvSetImageROI(img, cvRect(blob->minx, blob->miny, blob->maxx-blob->minx, blob->maxy-blob->miny));
  };

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Color

  /// \fn CvScalar cvBlobMeanColor(CvBlob const *blob, IplImage const *imgLabel, IplImage const *img)
  /// \brief Calculates mean color of a blob in an image.
  /// \param blob Blob.
  /// \param imgLabel Image of labels.
  /// \param img Original image.
  /// \return Average color.
  CvScalar cvBlobMeanColor(CvBlob const *blob, IplImage const *imgLabel, IplImage const *img);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Tracking

#define CV_TRACK_RENDER_ID            0x0001 ///< Print the ID of each track in the image. \see cvRenderTracks
#define CV_TRACK_RENDER_BOUNDING_BOX  0x0002 ///< Draw bounding box of each track in the image. \see cvRenderTracks
#define CV_TRACK_RENDER_TO_LOG        0x0010 ///< Print track info to log out. \see cvRenderTracks
#define CV_TRACK_RENDER_TO_STD        0x0020 ///< Print track info to log out. \see cvRenderTracks

  /// \fn void cvRenderTracks(CvTracks const tracks, IplImage *imgSource, IplImage *imgDest, unsigned short mode=0x00ff, CvFont *font=NULL)
  /// \brief Prints tracks information.
  /// \param tracks List of tracks.
  /// \param imgSource Input image (depth=IPL_DEPTH_8U and num. channels=3).
  /// \param imgDest Output image (depth=IPL_DEPTH_8U and num. channels=3).
  /// \param mode Render mode. By default is CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX.
  /// \param font OpenCV font for print on the image.
  /// \see CV_TRACK_RENDER_ID
  /// \see CV_TRACK_RENDER_BOUNDING_BOX
  /// \see CV_TRACK_RENDER_TO_LOG
  /// \see CV_TRACK_RENDER_TO_STD
  void cvRenderTracks(CvTracks const tracks, IplImage *imgSource, IplImage *imgDest, unsigned short mode=0x000f, CvFont *font=NULL);
  
} // namespace_end

#endif
