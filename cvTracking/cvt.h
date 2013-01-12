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

#ifndef __CV_TRACK_H__
#define __CV_TRACK_H__

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

namespace cvt // namespace_start
{

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Blobs

  /// \brief Type of label.
  /// \see IPL_DEPTH_LABEL
  typedef unsigned int Label;
  
  /// \brief Type of identification numbers.
  typedef unsigned int CvID;

  /// \brief Struct that contain information about one blob.
  class Blob
  {
  public:
    
    ~Blob(void){
        // clear() calls destructors
          contour.clear();
      }
      
      void init(cv::Moments& mu, Label l, cv::Rect& r){
        centroid = cv::Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
        moment = mu;
        label = l;
        bbox = r;
        //minx = r.x;
        //maxx = r.x+r.width;
        //miny = r.y;
        //maxy = r.y+r.height;
      }
    
    Label label; ///< Label assigned to the blob.
    
    /// \brief Calculates angle orientation of a blob.
    /// \param blob Blob.
    /// \return Angle orientation in radians.
    /// \see Blob
    double angle(void);
    inline double getArea(void) const {return moment.m00;}
    
    // make this a cv::Rect
    //unsigned int minx; ///< X min.
    //unsigned int maxx; ///< X max.
    //unsigned int miny; ///< Y min.
    //unsigned int maxy; ///< y max.
    cv::Rect bbox;
    
    cv::Point2f centroid; ///< Centroid.
    std::vector< std::vector<cv::Point> > contour; ///< Contour for drawing
    cv::Moments moment; 
    
  };
  
  //////////////////////////////////////////////////////////////////////////////
  /// \var typedef std::map<Label,Blob *> Blobs
  /// \brief List of blobs.
  /// A map is used to access each blob from its label number.
  /// \see Label
  /// \see Blob
  class Blobs : public std::map<Label,Blob*> {
  public:
    Blobs(void){;}
    
    ~Blobs(void){
        // clear() calls Blob's destructor
        clear();
    }
    
    bool getBlobs(const cv::Mat& image);
    
    /// \brief Find largest blob (biggest area).
    /// \return Label of the largest blob or 0 if there are no blobs.
    /// \see Label
    Label largestBlob(void);
    
    /// \fn void cvFilterByArea(Blobs &blobs, unsigned int minArea, unsigned int maxArea)
    /// \brief Filter blobs by area.
    /// Those blobs whose areas are not in range will be erased from the input list of blobs.
    /// \param blobs List of blobs.
    /// \param minArea Minimun area.
    /// \param maxArea Maximun area.
    void filterByArea(unsigned int minArea, unsigned int maxArea);
    
    /// \fn void cvFilterByLabel(Blobs &blobs, Label label)
    /// \brief Filter blobs by label.
    /// Delete all blobs except those with label l.
    /// \param blobs List of blobs.
    /// \param label Label to leave.
    void filterByLabel(Label label);
  
  };
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Tracking

  /// \brief Struct that contain information about one track.
  /// \see CvID
  /// \see Label
  class Track
  {
  public:
    Track(void){;}
    ~Track(void){;}
    
    CvID id; ///< Track identification number.

    Label label; ///< Label assigned to the blob related to this track.

    // Rect!!!
    //unsigned int minx; ///< X min.
    //unsigned int maxx; ///< X max.
    //unsigned int miny; ///< Y min.
    //unsigned int maxy; ///< y max.
    cv::Rect bbox;
    cv::Point2f centroid; ///< Centroid.

    unsigned int lifetime; ///< Indicates how much frames the object has been in scene.
    unsigned int active; ///< Indicates number of frames that has been active from last inactive period.
    unsigned int inactive; ///< Indicates number of frames that has been missing.
  };

  /// \brief List of tracks.
  /// \see CvID
  /// \see Track
  class Tracks : public std::map<CvID, Track *> {
  public:
    Tracks(void){;}
    
    ~Tracks(void){
        // clear calls Track's destructor
        clear();
    }
    
  };

/// \fn cvUpdateTracks(Blobs const &b, Tracks &t, const double thDistance, const unsigned int thInactive, const unsigned int thActive=0)
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
/// \see Blobs
/// \see Tracks
void cvUpdateTracks(Blobs const &b, Tracks &t, const double thDistance, const unsigned int thInactive, const unsigned int thActive=0);

/// \brief Pair (identification number, track).
/// \see CvID
/// \see Track
typedef std::pair<CvID, Track *> CvIDTrack;


} // namespace_end

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::Blob& b)
/// \brief Overload operator "<<" for printing blob structure.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvt::Blob& b);

/// \fn std::ostream& operator<< (std::ostream& output, const cvb::Track& t)
/// \brief Overload operator "<<" for printing track structure.
/// \return Stream.
std::ostream& operator<< (std::ostream& output, const cvt::Track& t);

#endif
