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

#ifdef SWIG
%module cvblob
%{
#include "cvblob.h"
%}
#endif

#ifndef CVBLOB_H
#define CVBLOB_H

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <limits>

#if (defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__))
    #include <cv.h>
#else
    #include <opencv2/opencv.hpp>
#endif

#ifndef __CV_BEGIN__
#define __CV_BEGIN__ __BEGIN__
#endif
#ifndef __CV_END__
#define __CV_END__ __END__
#endif

namespace cvb // namespace_start
{

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Contours

  // Chain code:
  //        7 0 1
  //        6   2
  //        5 4 3
#define CV_CHAINCODE_UP		0 ///< Up.
#define CV_CHAINCODE_UP_RIGHT	1 ///< Up and right.
#define CV_CHAINCODE_RIGHT	2 ///< Right.
#define CV_CHAINCODE_DOWN_RIGHT	3 ///< Down and right.
#define CV_CHAINCODE_DOWN	4 ///< Down.
#define CV_CHAINCODE_DOWN_LEFT	5 ///< Down and left.
#define CV_CHAINCODE_LEFT	6 ///< Left.
#define CV_CHAINCODE_UP_LEFT	7 ///< Up and left.

  /// \brief Move vectors of chain codes.
  /// \see CV_CHAINCODE_UP
  /// \see CV_CHAINCODE_UP_LEFT
  /// \see CV_CHAINCODE_LEFT
  /// \see CV_CHAINCODE_DOWN_LEFT
  /// \see CV_CHAINCODE_DOWN
  /// \see CV_CHAINCODE_DOWN_RIGHT
  /// \see CV_CHAINCODE_RIGHT
  /// \see CV_CHAINCODE_UP_RIGHT
  const char cvChainCodeMoves[8][2] = { { 0, -1},
                                        { 1, -1},
					{ 1,  0},
					{ 1,  1},
					{ 0,  1},
					{-1,  1},
					{-1,  0},
					{-1, -1}
                                      };

  /// \brief Direction.
  /// \see CV_CHAINCODE_UP
  /// \see CV_CHAINCODE_UP_LEFT
  /// \see CV_CHAINCODE_LEFT
  /// \see CV_CHAINCODE_DOWN_LEFT
  /// \see CV_CHAINCODE_DOWN
  /// \see CV_CHAINCODE_DOWN_RIGHT
  /// \see CV_CHAINCODE_RIGHT
  /// \see CV_CHAINCODE_UP_RIGHT
  typedef unsigned char CvChainCode;

  /// \brief Chain code.
  /// \see CvChainCode
  typedef std::list<CvChainCode> CvChainCodes;

  /// \brief Chain code contour.
  /// \see CvChainCodes
  class CvContourChainCode
  {
    public:
    CvContourChainCode(void){;}
    
    ~CvContourChainCode(void){
        chainCode.clear();
    }
    
    CvPoint startingPoint; ///< Point where contour begin.
    CvChainCodes chainCode; ///< Polygon description based on chain codes.
  };

  typedef std::list<CvContourChainCode *> CvContoursChainCode; ///< List of contours (chain codes type).

  /// \brief Polygon based contour.
  typedef std::vector<CvPoint> CvContourPolygon;

  /// \fn CvContourPolygon *cvConvertChainCodesToPolygon(CvContourChainCode const *cc)
  /// \brief Convert a chain code contour to a polygon.
  /// \param cc Chain code contour.
  /// \return A polygon.
  /// \see CvContourChainCode
  /// \see CvContourPolygon
  CvContourPolygon *cvConvertChainCodesToPolygon(CvContourChainCode const *cc);

  /// \fn double cvContourPolygonArea(CvContourPolygon const *p)
  /// \brief Calculates area of a polygonal contour.
  /// \param p Contour (polygon type).
  /// \return Area of the contour.
  double cvContourPolygonArea(CvContourPolygon const *p);

  /// \fn double cvContourChainCodePerimeter(CvContourChainCode const *c)
  /// \brief Calculates perimeter of a chain code contour.
  /// \param c Contour (chain code type).
  /// \return Perimeter of the contour.
  double cvContourChainCodePerimeter(CvContourChainCode const *c);

  /// \fn double cvContourPolygonPerimeter(CvContourPolygon const *p)
  /// \brief Calculates perimeter of a polygonal contour.
  /// \param p Contour (polygon type).
  /// \return Perimeter of the contour.
  double cvContourPolygonPerimeter(CvContourPolygon const *p);

  /// \fn double cvContourPolygonCircularity(const CvContourPolygon *p)
  /// \brief Calculates the circularity of a polygon (compactness measure).
  /// \param p Contour (polygon type).
  /// \return Circularity: a non-negative value, where 0 correspond with a circumference.
  double cvContourPolygonCircularity(const CvContourPolygon *p);

  /// \fn CvContourPolygon *cvSimplifyPolygon(CvContourPolygon const *p, double const delta=1.)
  /// \brief Simplify a polygon reducing the number of vertex according the distance "delta".
  /// Uses a version of the Ramer-Douglas-Peucker algorithm (http://en.wikipedia.org/wiki/Ramer-Douglas-Peucker_algorithm).
  /// \param p Contour (polygon type).
  /// \param delta Minimun distance.
  /// \return A simplify version of the original polygon.
  CvContourPolygon *cvSimplifyPolygon(CvContourPolygon const *p, double const delta=1.);

  /// \fn CvContourPolygon *cvPolygonContourConvexHull(CvContourPolygon const *p)
  /// \brief Calculates convex hull of a contour.
  /// Uses the Melkman Algorithm. Code based on the version in http://w3.impa.br/~rdcastan/Cgeometry/.
  /// \param p Contour (polygon type).
  /// \return Convex hull.
  CvContourPolygon *cvPolygonContourConvexHull(CvContourPolygon const *p);

  /// \fn void cvWriteContourPolygonCSV(const CvContourPolygon& p, const std::string& filename)
  /// \brief Write a contour to a CSV (Comma-separated values) file.
  /// \param p Polygon contour.
  /// \param filename File name.
  void cvWriteContourPolygonCSV(const CvContourPolygon& p, const std::string& filename);

  /// \fn void cvWriteContourPolygonSVG(const CvContourPolygon& p, const std::string& filename, const CvScalar& stroke=cvScalar(0,0,0), const CvScalar& fill=cvScalar(255,255,255))
  /// \brief Write a contour to a SVG file (http://en.wikipedia.org/wiki/Scalable_Vector_Graphics).
  /// \param p Polygon contour.
  /// \param filename File name.
  /// \param stroke Stroke color (black by default).
  /// \param fill Fill color (white by default).
  void cvWriteContourPolygonSVG(const CvContourPolygon& p, const std::string& filename, const CvScalar& stroke=cvScalar(0,0,0), const CvScalar& fill=cvScalar(255,255,255));

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Blobs

  /// \brief Type of label.
  /// \see IPL_DEPTH_LABEL
  typedef unsigned int CvLabel;
  //typedef unsigned char CvLabel;

  /// \def IPL_DEPTH_LABEL
  /// \brief Size of a label in bits.
  /// \see CvLabel
#define IPL_DEPTH_LABEL (sizeof(cvb::CvLabel)*8)

  /// \def CV_BLOB_MAX_LABEL
  /// \brief Max label number.
  /// \see CvLabel.
#define CV_BLOB_MAX_LABEL std::numeric_limits<CvLabel>::max()
  
  /// \brief Type of identification numbers.
  typedef unsigned int CvID;

  /// \brief Struct that contain information about one blob.
  class CvBlob
  {
  public:
    CvBlob(void){;}
    
    ~CvBlob(void){
        // clear() calls destructors
        /*
        for (CvContoursChainCode::iterator jt=internalContours.begin(); jt!=internalContours.end(); ++jt)
          {
            CvContourChainCode *contour = *jt;
            if (contour)
              delete contour;
          }
          */
          internalContours.clear();
      }
    
    CvLabel label; ///< Label assigned to the blob.
    
    union
    {
      unsigned int area; ///< Area (moment 00).
      unsigned int m00; ///< Moment 00 (area).
    };
    
    unsigned int minx; ///< X min.
    unsigned int maxx; ///< X max.
    unsigned int miny; ///< Y min.
    unsigned int maxy; ///< y max.
    
    CvPoint2D64f centroid; ///< Centroid.
    
    double m10; ///< Moment 10.
    double m01; ///< Moment 01.
    double m11; ///< Moment 11.
    double m20; ///< Moment 20.
    double m02; ///< Moment 02.
    
    double u11; ///< Central moment 11.
    double u20; ///< Central moment 20.
    double u02; ///< Central moment 02.

    double n11; ///< Normalized central moment 11.
    double n20; ///< Normalized central moment 20.
    double n02; ///< Normalized central moment 02.

    double p1; ///< Hu moment 1.
    double p2; ///< Hu moment 2.

    CvContourChainCode contour;           ///< Contour.
    CvContoursChainCode internalContours; ///< Internal contours.
  };
  
  /// \var typedef std::map<CvLabel,CvBlob *> CvBlobs
  /// \brief List of blobs.
  /// A map is used to access each blob from its label number.
  /// \see CvLabel
  /// \see CvBlob
  //typedef std::map<CvLabel,CvBlob *> CvBlobs;
  class CvBlobs : public std::map<CvLabel,CvBlob *> {
  public:
    CvBlobs(void){;}
    ~CvBlobs(void){
        // clear() calls CvBlob's destructor
        /*
        for (CvBlobs::iterator it=begin(); it!=end(); ++it)
        {
          //cvReleaseBlob((*it).second);
          delete (*it).second;
        }
        */
        clear();
    }
  };

  /// \var typedef std::pair<CvLabel,CvBlob *> CvLabelBlob
  /// \brief Pair (label, blob).
  /// \see CvLabel
  /// \see CvBlob
  typedef std::pair<CvLabel,CvBlob *> CvLabelBlob;
  

  /// \fn inline void cvReleaseBlobs(CvBlobs &blobs)
  /// \brief Clear blobs structure.
  /// \param blobs List of blobs.
  /// \see CvBlobs
  /*
  inline void cvReleaseBlobs(CvBlobs &blobs)
  {
    for (CvBlobs::iterator it=blobs.begin(); it!=blobs.end(); ++it)
    {
      //cvReleaseBlob((*it).second);
      delete (*it).second;
    }
    blobs.clear();
  }
  */

  /// \fn CvLabel cvLargestBlob(const CvBlobs &blobs)
  /// \brief Find largest blob (biggest area).
  /// \param blobs List of blobs.
  /// \return Label of the largest blob or 0 if there are no blobs.
  /// \see cvLabel
  CvLabel cvLargestBlob(const CvBlobs &blobs);

  inline CvLabel cvGreaterBlob(const CvBlobs &blobs)
  {
    return cvLargestBlob(blobs);
  }

  /// \fn void cvFilterByArea(CvBlobs &blobs, unsigned int minArea, unsigned int maxArea)
  /// \brief Filter blobs by area.
  /// Those blobs whose areas are not in range will be erased from the input list of blobs.
  /// \param blobs List of blobs.
  /// \param minArea Minimun area.
  /// \param maxArea Maximun area.
  void cvFilterByArea(CvBlobs &blobs, unsigned int minArea, unsigned int maxArea);

  /// \fn void cvFilterByLabel(CvBlobs &blobs, CvLabel label)
  /// \brief Filter blobs by label.
  /// Delete all blobs except those with label l.
  /// \param blobs List of blobs.
  /// \param label Label to leave.
  void cvFilterByLabel(CvBlobs &blobs, CvLabel label);

  /// \fn inline CvPoint2D64f cvCentroid(CvBlob *blob)
  /// \brief Calculates centroid.
  /// Centroid will be returned and stored in the blob structure.
  /// \param blob Blob whose centroid will be calculated.
  /// \return Centroid.
  /// \see CvBlob
  inline CvPoint2D64f cvCentroid(CvBlob *blob)
  {
    return blob->centroid=cvPoint2D64f(blob->m10/blob->area, blob->m01/blob->area);
  }

  /// \fn double cvAngle(CvBlob *blob)
  /// \brief Calculates angle orientation of a blob.
  /// \param blob Blob.
  /// \return Angle orientation in radians.
  /// \see CvBlob
  double cvAngle(CvBlob *blob);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Aux
  
  /// \fn double cvDotProductPoints(CvPoint const &a, CvPoint const &b, CvPoint const &c)
  /// \brief Dot product of the vectors ab and bc.
  /// \param a First point.
  /// \param b Middle point.
  /// \param c Last point.
  /// \return Dot product of ab and bc.
  double cvDotProductPoints(CvPoint const &a, CvPoint const &b, CvPoint const &c);
  
  /// \fn double cvCrossProductPoints(CvPoint const &a, CvPoint const &b, CvPoint const &c)
  /// \brief Cross product of the vectors ab and bc.
  /// \param a Point.
  /// \param b Point.
  /// \param c Point.
  /// \return Cross product of ab and bc.
  double cvCrossProductPoints(CvPoint const &a, CvPoint const &b, CvPoint const &c);

  /// \fn double cvDistancePointPoint(CvPoint const &a, CvPoint const &b)
  /// \brief Distance between two points.
  /// \param a Point.
  /// \param b Point.
  /// \return Distance.
  double cvDistancePointPoint(CvPoint const &a, CvPoint const &b);

  /// \fn double cvDistanceLinePoint(CvPoint const &a, CvPoint const &b, CvPoint const &c, bool isSegment=true)
  /// \brief Distance between line ab and point c.
  /// \param a First point of the segment.
  /// \param b Second point of the segment.
  /// \param c Point.
  /// \param isSegment If false then the distance will be calculated from the line defined by the points a and b, to the point c.
  /// \return Distance between ab and c.
  double cvDistanceLinePoint(CvPoint const &a, CvPoint const &b, CvPoint const &c, bool isSegment=true);
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Tracking

  /// \brief Struct that contain information about one track.
  /// \see CvID
  /// \see CvLabel
  class CvTrack
  {
  public:
    CvTrack(void){;}
    ~CvTrack(void){;}
    
    CvID id; ///< Track identification number.

    CvLabel label; ///< Label assigned to the blob related to this track.

    unsigned int minx; ///< X min.
    unsigned int maxx; ///< X max.
    unsigned int miny; ///< Y min.
    unsigned int maxy; ///< y max.
    
    CvPoint2D64f centroid; ///< Centroid.

    unsigned int lifetime; ///< Indicates how much frames the object has been in scene.
    unsigned int active; ///< Indicates number of frames that has been active from last inactive period.
    unsigned int inactive; ///< Indicates number of frames that has been missing.
  };

  /// \var typedef std::map<CvID, CvTrack *> CvTracks
  /// \brief List of tracks.
  /// \see CvID
  /// \see CvTrack
  //typedef std::map<CvID, CvTrack *> CvTracks;
  class CvTracks : public std::map<CvID, CvTrack *> {
  public:
    CvTracks(void){;}
    ~CvTracks(void){
        // clear calls CvTrack's destructor
        /*
        for (CvTracks::iterator it=begin(); it!=end(); it++)
        {
          CvTrack *track = (*it).second;
          if (track) delete track;
        }
        */
    
        clear();
    }
  };

  /// \var typedef std::pair<CvID, CvTrack *> CvIDTrack
  /// \brief Pair (identification number, track).
  /// \see CvID
  /// \see CvTrack
  typedef std::pair<CvID, CvTrack *> CvIDTrack;


  /// \fn cvUpdateTracks(CvBlobs const &b, CvTracks &t, const double thDistance, const unsigned int thInactive, const unsigned int thActive=0)
  /// \brief Updates list of tracks based on current blobs.
  /// Tracking based on:
  /// A. Senior, A. Hampapur, Y-L Tian, L. Brown, S. Pankanti, R. Bolle. Appearance Models for
  /// Occlusion Handling. Second International workshop on Performance Evaluation of Tracking and
  /// Surveillance Systems & CVPR'01. December, 2001.
  /// (http://www.research.ibm.com/peoplevision/PETS2001.pdf)
  /// \param b List of blobs.
  /// \param t List of tracks.
  /// \param thDistance Max distance to determine when a track and a blob match.
  /// \param thInactive Max number of frames a track can be inactive.
  /// \param thActive If a track becomes inactive but it has been active less than thActive frames, the track will be deleted.
  /// \see CvBlobs
  /// \see Tracks
  void cvUpdateTracks(CvBlobs const &b, CvTracks &t, const double thDistance, const unsigned int thInactive, const unsigned int thActive=0);

} // namespace_end

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::CvBlob& b)
/// \brief Overload operator "<<" for printing blob structure.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvb::CvBlob& b);

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::CvContourPolygon& p)
/// \brief Overload operator "<<" for printing polygons in CSV format.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvb::CvContourPolygon& p);

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::CvTrack& t)
/// \brief Overload operator "<<" for printing track structure.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvb::CvTrack& t);

#endif
