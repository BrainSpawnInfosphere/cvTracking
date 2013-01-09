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

#ifndef CVBLOB_H
#define CVBLOB_H

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <limits>

#include <opencv2/opencv.hpp>

#ifndef __CV_BEGIN__
#define __CV_BEGIN__ __BEGIN__
#endif
#ifndef __CV_END__
#define __CV_END__ __END__
#endif

namespace cvb // namespace_start
{

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Blobs

  /// \brief Type of label.
  /// \see IPL_DEPTH_LABEL
  typedef unsigned int CvLabel;
  
  /// \brief Type of identification numbers.
  typedef unsigned int CvID;

  /// \brief Struct that contain information about one blob.
  class CvBlob
  {
  public:
    
    ~CvBlob(void){
        // clear() calls destructors
          contour.clear();
      }
      
      void init(cv::Moments& mu, CvLabel l, cv::Rect& r){
        centroid = cv::Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
        moment = mu;
        label = l;
        
        minx = r.x;
        maxx = r.x+r.width;
        miny = r.y;
        maxy = r.y+r.height;
      }
    
    CvLabel label; ///< Label assigned to the blob.
    
    /// \brief Calculates angle orientation of a blob.
    /// \param blob Blob.
    /// \return Angle orientation in radians.
    /// \see CvBlob
    double cvAngle(void);
    inline double getArea(void) const {return moment.m00;}
    
    // make this a cv::Rect
    unsigned int minx; ///< X min.
    unsigned int maxx; ///< X max.
    unsigned int miny; ///< Y min.
    unsigned int maxy; ///< y max.
    
    cv::Point2f centroid; ///< Centroid.
    std::vector< std::vector<cv::Point> > contour; ///< Contour for drawing
    cv::Moments moment; 
    
  };
  
  //////////////////////////////////////////////////////////////////////////////
  /// \var typedef std::map<CvLabel,CvBlob *> CvBlobs
  /// \brief List of blobs.
  /// A map is used to access each blob from its label number.
  /// \see CvLabel
  /// \see CvBlob
  class CvBlobs : public std::map<CvLabel,CvBlob*> {
  public:
    CvBlobs(void){;}
    
    ~CvBlobs(void){
        // clear() calls CvBlob's destructor
        clear();
    }
    
    bool getBlobs(const cv::Mat& image);
    
    /// \brief Find largest blob (biggest area).
    /// \return Label of the largest blob or 0 if there are no blobs.
    /// \see cvLabel
    CvLabel cvLargestBlob(void);
    
    /// \fn void cvFilterByArea(CvBlobs &blobs, unsigned int minArea, unsigned int maxArea)
    /// \brief Filter blobs by area.
    /// Those blobs whose areas are not in range will be erased from the input list of blobs.
    /// \param blobs List of blobs.
    /// \param minArea Minimun area.
    /// \param maxArea Maximun area.
    void cvFilterByArea(unsigned int minArea, unsigned int maxArea);
    
    /// \fn void cvFilterByLabel(CvBlobs &blobs, CvLabel label)
    /// \brief Filter blobs by label.
    /// Delete all blobs except those with label l.
    /// \param blobs List of blobs.
    /// \param label Label to leave.
    void cvFilterByLabel(CvLabel label);
  
  };

  /// \var typedef std::pair<CvLabel,CvBlob *> CvLabelBlob
  /// \brief Pair (label, blob).
  /// \see CvLabel
  /// \see CvBlob
  //typedef std::pair<CvLabel,CvBlob *> CvLabelBlob;
  
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

    // Rect!!!
    unsigned int minx; ///< X min.
    unsigned int maxx; ///< X max.
    unsigned int miny; ///< Y min.
    unsigned int maxy; ///< y max.
    
    cv::Point2f centroid; ///< Centroid.

    unsigned int lifetime; ///< Indicates how much frames the object has been in scene.
    unsigned int active; ///< Indicates number of frames that has been active from last inactive period.
    unsigned int inactive; ///< Indicates number of frames that has been missing.
  };

  /// \brief List of tracks.
  /// \see CvID
  /// \see CvTrack
  class CvTracks : public std::map<CvID, CvTrack *> {
  public:
    CvTracks(void){;}
    
    ~CvTracks(void){
        // clear calls CvTrack's destructor
        clear();
    }
    
  };

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

/// \brief Pair (identification number, track).
/// \see CvID
/// \see CvTrack
typedef std::pair<CvID, CvTrack *> CvIDTrack;


} // namespace_end

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::CvBlob& b)
/// \brief Overload operator "<<" for printing blob structure.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvb::CvBlob& b);

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::CvTrack& t)
/// \brief Overload operator "<<" for printing track structure.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvb::CvTrack& t);

#endif
