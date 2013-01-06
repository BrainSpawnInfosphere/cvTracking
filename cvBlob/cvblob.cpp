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

#include <cmath>
#include <iostream>
using namespace std;

#include "cvblob.h"

namespace cvb
{

  CvLabel cvLargestBlob(const CvBlobs &blobs)
  {
    CvLabel label=0;
    unsigned int maxArea=0;

    for (CvBlobs::const_iterator it=blobs.begin();it!=blobs.end();++it)
    {
      CvBlob *blob=(*it).second;

      if (blob->area > maxArea)
      {
		label=blob->label;
		maxArea=blob->area;
      }
    }

    return label;
  }

  void cvFilterByArea(CvBlobs &blobs, unsigned int minArea, unsigned int maxArea)
  {
    CvBlobs::iterator it=blobs.begin();
    while(it!=blobs.end())
    {
      CvBlob *blob=(*it).second;
      if ((blob->area<minArea)||(blob->area>maxArea))
      {
	//cvReleaseBlob(blob);

	CvBlobs::iterator tmp=it;
	++it;
	blobs.erase(tmp);
      }
      else
	++it;
    }
  }

  void cvFilterByLabel(CvBlobs &blobs, CvLabel label)
  {
    CvBlobs::iterator it=blobs.begin();
    while(it!=blobs.end())
    {
      CvBlob *blob=(*it).second;
      if (blob->label!=label)
      {
	delete blob;
	CvBlobs::iterator tmp=it;
	++it;
	blobs.erase(tmp);
      }
      else
	++it;
    }
  }


  // Returns radians
  double cvAngle(CvBlob *blob)
  {
    CV_FUNCNAME("cvAngle");
    __CV_BEGIN__;

    return .5*atan2(2.*blob->u11,(blob->u20-blob->u02));

    __CV_END__;
  }

}

ostream& operator<< (ostream& output, const cvb::CvBlob& b)
{
  output << b.label << ": " << b.area << ", (" << b.centroid.x << ", " << b.centroid.y << "), [(" << b.minx << ", " << b.miny << ") - (" << b.maxx << ", " << b.maxy << ")]";

  return output;
}
