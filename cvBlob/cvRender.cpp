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
//#include <iostream>
using namespace std;


#include "cvRender.h"

namespace cvb // namespace_start
{

  /*void cvCentralMoments(CvBlob *blob, const IplImage *img)
  {
    CV_FUNCNAME("cvCentralMoments");
    __CV_BEGIN__;
    if (!blob->centralMoments)
    {
      CV_ASSERT(img&&(img->depth==IPL_DEPTH_LABEL)&&(img->nChannels==1));

      //cvCentroid(blob); // Here?

      blob->u11=blob->u20=blob->u02=0.;

      // Only in the bounding box
      int stepIn = img->widthStep / (img->depth / 8);
      int img_width = img->width;
      int img_height = img->height;
      int img_offset = 0;
      if(0 != img->roi)
      {
	img_width = img->roi->width;
	img_height = img->roi->height;
	img_offset = img->roi->xOffset + (img->roi->yOffset * stepIn);
      }

      CvLabel *imgData=(CvLabel *)img->imageData + (blob->miny * stepIn) + img_offset;
      for (unsigned int r=blob->miny;
	  r<blob->maxy;
	  r++,imgData+=stepIn)
	for (unsigned int c=blob->minx;c<blob->maxx;c++)
	  if (imgData[c]==blob->label)
	  {
	    double tx=(c-blob->centroid.x);
	    double ty=(r-blob->centroid.y);
	    blob->u11+=tx*ty;
	    blob->u20+=tx*tx;
	    blob->u02+=ty*ty;
	  }

      blob->centralMoments = true;
    }
    __CV_END__;
  }*/

  void cvRenderBlob(const IplImage *imgLabel, CvBlob *blob, IplImage *imgSource, IplImage *imgDest, unsigned short mode, CvScalar const &color, double alpha)
  {
    CV_FUNCNAME("cvRenderBlob");
    __CV_BEGIN__;

    CV_ASSERT(imgLabel&&(imgLabel->depth==IPL_DEPTH_LABEL)&&(imgLabel->nChannels==1));
    CV_ASSERT(imgDest&&(imgDest->depth==IPL_DEPTH_8U)&&(imgDest->nChannels==3));

    if (mode&CV_BLOB_RENDER_COLOR)
    {
      int stepLbl = imgLabel->widthStep/(imgLabel->depth/8);
      int stepSrc = imgSource->widthStep/(imgSource->depth/8);
      int stepDst = imgDest->widthStep/(imgDest->depth/8);
      int imgLabel_width = imgLabel->width;
      int imgLabel_height = imgLabel->height;
      int imgLabel_offset = 0;
      int imgSource_width = imgSource->width;
      int imgSource_height = imgSource->height;
      int imgSource_offset = 0;
      int imgDest_width = imgDest->width;
      int imgDest_height = imgDest->height;
      int imgDest_offset = 0;
      if(imgLabel->roi)
      {
	imgLabel_width = imgLabel->roi->width;
	imgLabel_height = imgLabel->roi->height;
	imgLabel_offset = (imgLabel->nChannels * imgLabel->roi->xOffset) + (imgLabel->roi->yOffset * stepLbl);
      }
      if(imgSource->roi)
      {
	imgSource_width = imgSource->roi->width;
	imgSource_height = imgSource->roi->height;
	imgSource_offset = (imgSource->nChannels * imgSource->roi->xOffset) + (imgSource->roi->yOffset * stepSrc);
      }
      if(imgDest->roi)
      {
	imgDest_width = imgDest->roi->width;
	imgDest_height = imgDest->roi->height;
	imgDest_offset = (imgDest->nChannels * imgDest->roi->xOffset) + (imgDest->roi->yOffset * stepDst);
      }

      CvLabel *labels = (CvLabel *)imgLabel->imageData + imgLabel_offset + (blob->miny * stepLbl);
      unsigned char *source = (unsigned char *)imgSource->imageData + imgSource_offset + (blob->miny * stepSrc);
      unsigned char *imgData = (unsigned char *)imgDest->imageData + imgDest_offset + (blob->miny * stepDst);

      for (unsigned int r=blob->miny; r<blob->maxy; r++, labels+=stepLbl, source+=stepSrc, imgData+=stepDst)
	for (unsigned int c=blob->minx; c<blob->maxx; c++)
	{
	  if (labels[c]==blob->label)
	  {
	    imgData[imgDest->nChannels*c+0] = (unsigned char)((1.-alpha)*source[imgSource->nChannels*c+0]+alpha*color.val[0]);
	    imgData[imgDest->nChannels*c+1] = (unsigned char)((1.-alpha)*source[imgSource->nChannels*c+1]+alpha*color.val[1]);
	    imgData[imgDest->nChannels*c+2] = (unsigned char)((1.-alpha)*source[imgSource->nChannels*c+2]+alpha*color.val[2]);
	  }
	}
    }

    if (mode)
    {
      if (mode&CV_BLOB_RENDER_TO_LOG)
      {
	std::clog << "Blob " << blob->label << std::endl;
	std::clog << " - Bounding box: (" << blob->minx << ", " << blob->miny << ") - (" << blob->maxx << ", " << blob->maxy << ")" << std::endl;
	std::clog << " - Bounding box area: " << (1 + blob->maxx - blob->minx) * (1 + blob->maxy - blob->miny) << std::endl;
	std::clog << " - Area: " << blob->area << std::endl;
	std::clog << " - Centroid: (" << blob->centroid.x << ", " << blob->centroid.y << ")" << std::endl;
	std::clog << std::endl;
      }

      if (mode&CV_BLOB_RENDER_TO_STD)
      {
	std::cout << "Blob " << blob->label << std::endl;
	std::cout << " - Bounding box: (" << blob->minx << ", " << blob->miny << ") - (" << blob->maxx << ", " << blob->maxy << ")" << std::endl;
	std::cout << " - Bounding box area: " << (1 + blob->maxx - blob->minx) * (1 + blob->maxy - blob->miny) << std::endl;
	std::cout << " - Area: " << blob->area << std::endl;
	std::cout << " - Centroid: (" << blob->centroid.x << ", " << blob->centroid.y << ")" << std::endl;
	std::cout << std::endl;
      }

      if (mode&CV_BLOB_RENDER_BOUNDING_BOX)
	cvRectangle(imgDest, cvPoint(blob->minx, blob->miny), cvPoint(blob->maxx-1, blob->maxy-1), CV_RGB(255., 0., 0.));

      if (mode&CV_BLOB_RENDER_ANGLE)
      {
	double angle = cvAngle(blob);

	double x1,y1,x2,y2;
	double lengthLine = MAX(blob->maxx-blob->minx, blob->maxy-blob->miny)/2.;

	x1=blob->centroid.x-lengthLine*cos(angle);
	y1=blob->centroid.y-lengthLine*sin(angle);
	x2=blob->centroid.x+lengthLine*cos(angle);
	y2=blob->centroid.y+lengthLine*sin(angle);
	cvLine(imgDest,cvPoint(int(x1),int(y1)),cvPoint(int(x2),int(y2)),CV_RGB(0.,255.,0.));
      }

      if (mode&CV_BLOB_RENDER_CENTROID)
      {
	cvLine(imgDest,cvPoint(int(blob->centroid.x)-3,int(blob->centroid.y)),cvPoint(int(blob->centroid.x)+3,int(blob->centroid.y)),CV_RGB(0.,0.,255.));
	cvLine(imgDest,cvPoint(int(blob->centroid.x),int(blob->centroid.y)-3),cvPoint(int(blob->centroid.x),int(blob->centroid.y)+3),CV_RGB(0.,0.,255.));
      }
    }

    __CV_END__;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Based on http://en.wikipedia.org/wiki/HSL_and_HSV

  /// \def _HSV2RGB_(H, S, V, R, G, B)
  /// \brief Color translation between HSV and RGB.
inline void HSV2RGB(double H, double S, double V, double R, double G, double B){
    double _h = H/60.; 
    int _hf = (int)floor(_h); 
    int _hi = ((int)_h)%6; 
    double _f = _h - _hf; 
    
    double _p = V * (1. - S); 
    double _q = V * (1. - _f * S); 
    double _t = V * (1. - (1. - _f) * S); 
    
    switch (_hi) 
    { 
      case 0: 
	      R = 255.*V; G = 255.*_t; B = 255.*_p; 
      break; 
      case 1: 
	      R = 255.*_q; G = 255.*V; B = 255.*_p; 
      break; 
      case 2: 
	      R = 255.*_p; G = 255.*V; B = 255.*_t; 
      break; 
      case 3: 
	      R = 255.*_p; G = 255.*_q; B = 255.*V; 
      break; 
      case 4: 
	      R = 255.*_t; G = 255.*_p; B = 255.*V; 
      break; 
      case 5: 
	      R = 255.*V; G = 255.*_p; B = 255.*_q; 
      break; 
    } 
  }
  ///////////////////////////////////////////////////////////////////////////////////////////////////


  typedef std::map<CvLabel, CvScalar> Palete;
  
  void cvRenderBlobs(const IplImage *imgLabel, CvBlobs &blobs, IplImage *imgSource, IplImage *imgDest, unsigned short mode, double alpha)
  {
    CV_FUNCNAME("cvRenderBlobs");
    __CV_BEGIN__;
    {

      CV_ASSERT(imgLabel&&(imgLabel->depth==IPL_DEPTH_LABEL)&&(imgLabel->nChannels==1));
      CV_ASSERT(imgDest&&(imgDest->depth==IPL_DEPTH_8U)&&(imgDest->nChannels==3));

      Palete pal;
      if (mode&CV_BLOB_RENDER_COLOR)
      {

	unsigned int colorCount = 0;
	for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
	{
	  CvLabel label = (*it).second->label;

	  double r, g, b;

	  HSV2RGB((double)((colorCount*77)%360), .5, 1., r, g, b);
	  colorCount++;

	  pal[label] = CV_RGB(r, g, b);
	}
      }

      for (CvBlobs::iterator it=blobs.begin(); it!=blobs.end(); ++it)
	cvRenderBlob(imgLabel, (*it).second, imgSource, imgDest, mode, pal[(*it).second->label], alpha);

    }
    __CV_END__;
  }

  void cvSaveImageBlob(const char *filename, IplImage *img, CvBlob const *blob)
  {
    CvRect roi = cvGetImageROI(img);
    cvSetImageROItoBlob(img, blob);
    cvSaveImage(filename, img);
    cvSetImageROI(img, roi);
  }

/////////// cvtrack.cpp

  CvFont *defaultFont = NULL;

  void cvRenderTracks(CvTracks const tracks, IplImage *imgSource, IplImage *imgDest, unsigned short mode, CvFont *font)
  {
    CV_FUNCNAME("cvRenderTracks");
    __CV_BEGIN__;

    CV_ASSERT(imgDest&&(imgDest->depth==IPL_DEPTH_8U)&&(imgDest->nChannels==3));

    if ((mode&CV_TRACK_RENDER_ID)&&(!font))
    {
      if (!defaultFont)
      {
	font = defaultFont = new CvFont;
	cvInitFont(font, CV_FONT_HERSHEY_DUPLEX, 0.5, 0.5, 0, 1);
	// Other fonts:
	//   CV_FONT_HERSHEY_SIMPLEX, CV_FONT_HERSHEY_PLAIN,
	//   CV_FONT_HERSHEY_DUPLEX, CV_FONT_HERSHEY_COMPLEX,
	//   CV_FONT_HERSHEY_TRIPLEX, CV_FONT_HERSHEY_COMPLEX_SMALL,
	//   CV_FONT_HERSHEY_SCRIPT_SIMPLEX, CV_FONT_HERSHEY_SCRIPT_COMPLEX
      }
      else
	font = defaultFont;
    }

    if (mode)
    {
      for (CvTracks::const_iterator it=tracks.begin(); it!=tracks.end(); ++it)
      {
	if (mode&CV_TRACK_RENDER_ID)
	  if (!it->second->inactive)
	  {
	    stringstream buffer;
	    buffer << it->first;
	    cvPutText(imgDest, buffer.str().c_str(), cvPoint((int)it->second->centroid.x, (int)it->second->centroid.y), font, CV_RGB(0.,255.,0.));
	  }

	if (mode&CV_TRACK_RENDER_BOUNDING_BOX){
	  if (it->second->inactive)
	    cvRectangle(imgDest, cvPoint(it->second->minx, it->second->miny), cvPoint(it->second->maxx-1, it->second->maxy-1), CV_RGB(0., 0., 50.));
	  else
	    cvRectangle(imgDest, cvPoint(it->second->minx, it->second->miny), cvPoint(it->second->maxx-1, it->second->maxy-1), CV_RGB(0., 0., 255.));
    }
    
	if (mode&CV_TRACK_RENDER_TO_LOG)
	{
	  clog << "Track " << it->second->id << endl;
	  if (it->second->inactive)
	    clog << " - Inactive for " << it->second->inactive << " frames" << endl;
	  else
	    clog << " - Associated with blob " << it->second->label << endl;
	  clog << " - Lifetime " << it->second->lifetime << endl;
	  clog << " - Active " << it->second->active << endl;
	  clog << " - Bounding box: (" << it->second->minx << ", " << it->second->miny << ") - (" << it->second->maxx << ", " << it->second->maxy << ")" << endl;
	  clog << " - Centroid: (" << it->second->centroid.x << ", " << it->second->centroid.y << ")" << endl;
	  clog << endl;
	}

	if (mode&CV_TRACK_RENDER_TO_STD)
	{
	  cout << "Track " << it->second->id << endl;
	  if (it->second->inactive)
	    cout << " - Inactive for " << it->second->inactive << " frames" << endl;
	  else
	    cout << " - Associated with blobs " << it->second->label << endl;
	  cout << " - Lifetime " << it->second->lifetime << endl;
	  cout << " - Active " << it->second->active << endl;
	  cout << " - Bounding box: (" << it->second->minx << ", " << it->second->miny << ") - (" << it->second->maxx << ", " << it->second->maxy << ")" << endl;
	  cout << " - Centroid: (" << it->second->centroid.x << ", " << it->second->centroid.y << ")" << endl;
	  cout << endl;
	}
      }
    }

    __CV_END__;
  }

/////////////// cvcolor.cpp

  CvScalar cvBlobMeanColor(CvBlob const *blob, IplImage const *imgLabel, IplImage const *img)
  {
    CV_FUNCNAME("cvBlobMeanColor");
    __CV_BEGIN__;
    {
      CV_ASSERT(imgLabel&&(imgLabel->depth==IPL_DEPTH_LABEL)&&(imgLabel->nChannels==1));
      CV_ASSERT(img&&(img->depth==IPL_DEPTH_8U)&&(img->nChannels==3));

      int stepLbl = imgLabel->widthStep/(imgLabel->depth/8);
      int stepImg = img->widthStep/(img->depth/8);
      int imgLabel_width = imgLabel->width;
      int imgLabel_height = imgLabel->height;
      int imgLabel_offset = 0;
      int img_width = img->width;
      int img_height = img->height;
      int img_offset = 0;
      if(imgLabel->roi)
      {
	imgLabel_width = imgLabel->roi->width;
	imgLabel_height = imgLabel->roi->height;
	imgLabel_offset = (imgLabel->nChannels * imgLabel->roi->xOffset) + (imgLabel->roi->yOffset * stepLbl);
      }
      if(img->roi)
      {
	img_width = img->roi->width;
	img_height = img->roi->height;
	img_offset = (img->nChannels * img->roi->xOffset) + (img->roi->yOffset * stepImg);
      }

      CvLabel *labels = (CvLabel *)imgLabel->imageData + imgLabel_offset;
      unsigned char *imgData = (unsigned char *)img->imageData + img_offset;

      double mb = 0;
      double mg = 0;
      double mr = 0;
      double pixels = (double)blob->area;

      for (unsigned int r=0; r<(unsigned int)imgLabel_height; r++, labels+=stepLbl, imgData+=stepImg)
	for (unsigned int c=0; c<(unsigned int)imgLabel_width; c++)
	{
	  if (labels[c]==blob->label)
	  {
	    mb += ((double)imgData[img->nChannels*c+0])/pixels; // B
	    mg += ((double)imgData[img->nChannels*c+1])/pixels; // G
	    mr += ((double)imgData[img->nChannels*c+2])/pixels; // R
	  }
	}

      /*double mb = 0;
      double mg = 0;
      double mr = 0;
      double pixels = (double)blob->area;
      for (unsigned int y=0; y<imgLabel->height; y++)
	for (unsigned int x=0; x<imgLabel->width; x++)
	{
	  if (cvGetLabel(imgLabel, x, y)==blob->label)
	  {
	    CvScalar color = cvGet2D(img, y, x);
	    mb += color.val[0]/pixels;
	    mg += color.val[1]/pixels;
	    mr += color.val[2]/pixels;
	  }
	}*/

      return cvScalar(mr, mg, mb);
    }
    __CV_END__;
  }

////////////////////// cvcontour.cpp

  void cvRenderContourChainCode(CvContourChainCode const *contour, IplImage const *img, CvScalar const &color)
  {
    CV_FUNCNAME("cvRenderContourChainCode");
    __CV_BEGIN__;
    {
      CV_ASSERT(img&&(img->depth==IPL_DEPTH_8U)&&(img->nChannels==3));

      int stepDst = img->widthStep/(img->depth/8);
      int img_width = img->width;
      int img_height = img->height;
      int img_offset = 0;

      if(img->roi)
      {
	img_width = img->roi->width;
	img_height = img->roi->height;
	img_offset = (img->nChannels * img->roi->xOffset) + (img->roi->yOffset * stepDst);
      }

      unsigned char *imgData = (unsigned char *)img->imageData + img_offset;

      unsigned int x = contour->startingPoint.x;
      unsigned int y = contour->startingPoint.y;

      for (CvChainCodes::const_iterator it=contour->chainCode.begin(); it!=contour->chainCode.end(); ++it)
      {
	imgData[img->nChannels*x+img->widthStep*y+0] = (unsigned char)(color.val[0]); // Blue
	imgData[img->nChannels*x+img->widthStep*y+1] = (unsigned char)(color.val[1]); // Green
	imgData[img->nChannels*x+img->widthStep*y+2] = (unsigned char)(color.val[2]); // Red

	x += cvChainCodeMoves[*it][0];
	y += cvChainCodeMoves[*it][1];
      }
    }
    __CV_END__;
  }

  void cvRenderContourPolygon(CvContourPolygon const *contour, IplImage *img, CvScalar const &color)
  {
    CV_FUNCNAME("cvRenderContourPolygon");
    __CV_BEGIN__;
    {
      CV_ASSERT(img&&(img->depth==IPL_DEPTH_8U)&&(img->nChannels==3));

      CvContourPolygon::const_iterator it=contour->begin();

      if (it!=contour->end())
      {
	unsigned int fx, x, fy, y;
	fx = x = it->x;
	fy = y = it->y;

	for (; it!=contour->end(); ++it)
	{
	  cvLine(img, cvPoint(x, y), cvPoint(it->x, it->y), color, 1);
	  x = it->x;
	  y = it->y;
	}

	cvLine(img, cvPoint(x, y), cvPoint(fx, fy), color, 1);
      }
    }
    __CV_END__;
  }

/////////////////////////////// cvlabel.cpp

  const char movesE[4][3][4] = { { {-1, -1, 3, CV_CHAINCODE_UP_LEFT   }, { 0, -1, 0, CV_CHAINCODE_UP   }, { 1, -1, 0, CV_CHAINCODE_UP_RIGHT   } },
				 { { 1, -1, 0, CV_CHAINCODE_UP_RIGHT  }, { 1,  0, 1, CV_CHAINCODE_RIGHT}, { 1,  1, 1, CV_CHAINCODE_DOWN_RIGHT } },
				 { { 1,  1, 1, CV_CHAINCODE_DOWN_RIGHT}, { 0,  1, 2, CV_CHAINCODE_DOWN }, {-1,  1, 2, CV_CHAINCODE_DOWN_LEFT  } },
				 { {-1,  1, 2, CV_CHAINCODE_DOWN_LEFT }, {-1,  0, 3, CV_CHAINCODE_LEFT }, {-1, -1, 3, CV_CHAINCODE_UP_LEFT    } }
  };

  const char movesI[4][3][4] = { { { 1, -1, 3, CV_CHAINCODE_UP_RIGHT   }, { 0, -1, 0, CV_CHAINCODE_UP   }, {-1, -1, 0, CV_CHAINCODE_UP_LEFT    } },
				 { {-1, -1, 0, CV_CHAINCODE_UP_LEFT    }, {-1,  0, 1, CV_CHAINCODE_LEFT }, {-1,  1, 1, CV_CHAINCODE_DOWN_LEFT  } },
				 { {-1,  1, 1, CV_CHAINCODE_DOWN_LEFT  }, { 0,  1, 2, CV_CHAINCODE_DOWN }, { 1,  1, 2, CV_CHAINCODE_DOWN_RIGHT } },
				 { { 1,  1, 2, CV_CHAINCODE_DOWN_RIGHT }, { 1,  0, 3, CV_CHAINCODE_RIGHT}, { 1, -1, 3, CV_CHAINCODE_UP_RIGHT   } }
  };


  unsigned int cvLabel (IplImage const *img, IplImage *imgOut, CvBlobs &blobs)
  {
    CV_FUNCNAME("cvLabel");
    __CV_BEGIN__;
    {
      CV_ASSERT(img&&(img->depth==IPL_DEPTH_8U)&&(img->nChannels==1));
      CV_ASSERT(imgOut&&(imgOut->depth==IPL_DEPTH_LABEL)&&(imgOut->nChannels==1));

      unsigned int numPixels=0;

      cvSetZero(imgOut);

      CvLabel label=0;
      //cvReleaseBlobs(blobs);
      blobs.clear();

      unsigned int stepIn = img->widthStep / (img->depth / 8);
      unsigned int stepOut = imgOut->widthStep / (imgOut->depth / 8);
      unsigned int imgIn_width = img->width;
      unsigned int imgIn_height = img->height;
      unsigned int imgIn_offset = 0;
      unsigned int imgOut_width = imgOut->width;
      unsigned int imgOut_height = imgOut->height;
      unsigned int imgOut_offset = 0;
      if(img->roi)
      {
	imgIn_width = img->roi->width;
	imgIn_height = img->roi->height;
	imgIn_offset = img->roi->xOffset + (img->roi->yOffset * stepIn);
      }
      if(imgOut->roi)
      {
	imgOut_width = imgOut->roi->width;
	imgOut_height = imgOut->roi->height;
	imgOut_offset = imgOut->roi->xOffset + (imgOut->roi->yOffset * stepOut);
      }

      unsigned char *imgDataIn = (unsigned char *)img->imageData + imgIn_offset;
      CvLabel *imgDataOut = (CvLabel *)imgOut->imageData + imgOut_offset;

#define imageIn(X, Y) imgDataIn[(X) + (Y)*stepIn]
#define imageOut(X, Y) imgDataOut[(X) + (Y)*stepOut]

      CvLabel lastLabel = 0;
      CvBlob *lastBlob = NULL;

      for (unsigned int y=0; y<imgIn_height; y++)
      {
	for (unsigned int x=0; x<imgIn_width; x++)
	{
	  if (imageIn(x, y))
	  {
	    bool labeled = imageOut(x, y);

	    if ((!imageOut(x, y))&&((y==0)||(!imageIn(x, y-1))))
	    {
	      labeled = true;

	      // Label contour.
	      label++;
	      CV_ASSERT(label!=CV_BLOB_MAX_LABEL);

	      imageOut(x, y) = label;
	      numPixels++;

	      // XXX This is not necessary at all. I only do this for consistency.
	      if (y>0)
		imageOut(x, y-1) = CV_BLOB_MAX_LABEL;

	      CvBlob *blob = new CvBlob;
	      blob->label = label;
	      blob->area = 1;
	      blob->minx = x; blob->maxx = x;
	      blob->miny = y; blob->maxy = y;
	      blob->m10=x; blob->m01=y;
	      blob->m11=x*y;
	      blob->m20=x*x; blob->m02=y*y;
	      blob->internalContours.clear();
	      blobs.insert(CvLabelBlob(label,blob));

              lastLabel = label;
	      lastBlob = blob;

	      blob->contour.startingPoint = cvPoint(x, y);

	      unsigned char direction=1;
	      unsigned int xx = x;
	      unsigned int yy = y;


	      bool contourEnd = false;

	      do
	      {
		for (unsigned int numAttempts=0; numAttempts<3; numAttempts++)
		{
		  bool found = false;

		  for (unsigned char i=0; i<3; i++)
		  {
		    int nx = xx+movesE[direction][i][0];
		    int ny = yy+movesE[direction][i][1];
		    if ((nx<imgIn_width)&&(nx>=0)&&(ny<imgIn_height)&&(ny>=0))
		    {
		      if (imageIn(nx, ny))
		      {
			found = true;

			blob->contour.chainCode.push_back(movesE[direction][i][3]);

			xx=nx;
			yy=ny;

			direction=movesE[direction][i][2];
			break;
		      }
		      else
		      {
			imageOut(nx, ny) = CV_BLOB_MAX_LABEL;
		      }
		    }
		  }

		  if (!found)
		    direction=(direction+1)%4;
		  else
		  {
		    if (imageOut(xx, yy) != label)
		    {
		      imageOut(xx, yy) = label;
		      numPixels++;

		      if (xx<blob->minx) blob->minx = xx;
		      else if (xx>blob->maxx) blob->maxx = xx;
		      if (yy<blob->miny) blob->miny = yy;
		      else if (yy>blob->maxy) blob->maxy = yy;

		      blob->area++;
		      blob->m10+=xx; blob->m01+=yy;
		      blob->m11+=xx*yy;
		      blob->m20+=xx*xx; blob->m02+=yy*yy;
		    }

		    break;
		  }
		  // fixed assignment and if statement - kjw
		  contourEnd = ((xx==x) && (yy==y) && (direction==1));
		  if (contourEnd)
		    break;
		}
	      }
	      while (!contourEnd);

	    }

	    if ((y+1<imgIn_height)&&(!imageIn(x, y+1))&&(!imageOut(x, y+1)))
	    {
	      labeled = true;

	      // Label internal contour
	      CvLabel l;
	      CvBlob *blob = NULL;

	      if (!imageOut(x, y))
	      {
		/*if (!imageOut(x-1, y))
		{
		  cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		  continue;
		}*/

		l = imageOut(x-1, y);

		imageOut(x, y) = l;
		numPixels++;

                if (l==lastLabel)
                  blob = lastBlob;
                else
                {
                  blob = blobs.find(l)->second;
                  lastLabel = l;
                  lastBlob = blob;
                }
		blob->area++;
		blob->m10+=x; blob->m01+=y;
		blob->m11+=x*y;
		blob->m20+=x*x; blob->m02+=y*y;
	      }
	      else
	      {
		l = imageOut(x, y);

                if (l==lastLabel)
                  blob = lastBlob;
                else
                {
                  blob = blobs.find(l)->second;
                  lastLabel = l;
                  lastBlob = blob;
                }
	      }

	      // XXX This is not necessary (I believe). I only do this for consistency.
	      imageOut(x, y+1) = CV_BLOB_MAX_LABEL;

	      CvContourChainCode *contour = new CvContourChainCode;
	      contour->startingPoint = cvPoint(x, y);

	      unsigned char direction = 3;
	      unsigned int xx = x;
	      unsigned int yy = y;

	      do
	      {
		for (unsigned int numAttempts=0; numAttempts<3; numAttempts++)
		{
		  bool found = false;

		  for (unsigned char i=0; i<3; i++)
		  {
		    int nx = xx+movesI[direction][i][0];
		    int ny = yy+movesI[direction][i][1];
		    if (imageIn(nx, ny))
		    {
		      found = true;

		      contour->chainCode.push_back(movesI[direction][i][3]);

		      xx=nx;
		      yy=ny;

		      direction=movesI[direction][i][2];
		      break;
		    }
		    else
		    {
		      imageOut(nx, ny) = CV_BLOB_MAX_LABEL;
		    }
		  }

		  if (!found)
		    direction=(direction+1)%4;
		  else
		  {
		    if (!imageOut(xx, yy))
		    {
		      imageOut(xx, yy) = l;
		      numPixels++;

		      blob->area++;
		      blob->m10+=xx; blob->m01+=yy;
		      blob->m11+=xx*yy;
		      blob->m20+=xx*xx; blob->m02+=yy*yy;
		    }

		    break;
		  }
		}
	      }
	      while (!(xx==x && yy==y));

	      blob->internalContours.push_back(contour);
	    }

	    //else if (!imageOut(x, y))
	    if (!labeled)
	    {
	      // Internal pixel
	      CvLabel l = imageOut(x-1, y);

	      imageOut(x, y) = l;
	      numPixels++;

	      CvBlob *blob = NULL;
              if (l==lastLabel)
                blob = lastBlob;
              else
              {
                blob = blobs.find(l)->second;
                lastLabel = l;
                lastBlob = blob;
              }
	      blob->area++;
	      blob->m10+=x; blob->m01+=y;
	      blob->m11+=x*y;
	      blob->m20+=x*x; blob->m02+=y*y;
	    }
	  }
	}
      }

      for (CvBlobs::iterator it=blobs.begin(); it!=blobs.end(); ++it)
      {
	cvCentroid((*it).second);

        (*it).second->u11 = (*it).second->m11 - ((*it).second->m10*(*it).second->m01)/(*it).second->m00;
        (*it).second->u20 = (*it).second->m20 - ((*it).second->m10*(*it).second->m10)/(*it).second->m00;
        (*it).second->u02 = (*it).second->m02 - ((*it).second->m01*(*it).second->m01)/(*it).second->m00;

        double m00_2 = (*it).second->m00 * (*it).second->m00;

        (*it).second->n11 = (*it).second->u11 / m00_2;
        (*it).second->n20 = (*it).second->u20 / m00_2;
        (*it).second->n02 = (*it).second->u02 / m00_2;

        (*it).second->p1 = (*it).second->n20 + (*it).second->n02;

        double nn = (*it).second->n20 - (*it).second->n02;
        (*it).second->p2 = nn*nn + 4.*((*it).second->n11*(*it).second->n11);
      }

      return numPixels;

    }
    
    return 0; // added return statement - check if right - kjw
    __CV_END__;
  }

  void cvFilterLabels(IplImage *imgIn, IplImage *imgOut, const CvBlobs &blobs)
  {
    CV_FUNCNAME("cvFilterLabels");
    __CV_BEGIN__;
    {
      CV_ASSERT(imgIn&&(imgIn->depth==IPL_DEPTH_LABEL)&&(imgIn->nChannels==1));
      CV_ASSERT(imgOut&&(imgOut->depth==IPL_DEPTH_8U)&&(imgOut->nChannels==1));

      int stepIn = imgIn->widthStep / (imgIn->depth / 8);
      int stepOut = imgOut->widthStep / (imgOut->depth / 8);
      int imgIn_width = imgIn->width;
      int imgIn_height = imgIn->height;
      int imgIn_offset = 0;
      int imgOut_width = imgOut->width;
      int imgOut_height = imgOut->height;
      int imgOut_offset = 0;
      if(imgIn->roi)
      {
	imgIn_width = imgIn->roi->width;
	imgIn_height = imgIn->roi->height;
	imgIn_offset = imgIn->roi->xOffset + (imgIn->roi->yOffset * stepIn);
      }
      if(imgOut->roi)
      {
	imgOut_width = imgOut->roi->width;
	imgOut_height = imgOut->roi->height;
	imgOut_offset = imgOut->roi->xOffset + (imgOut->roi->yOffset * stepOut);
      }

      char *imgDataOut=imgOut->imageData + imgOut_offset;
      CvLabel *imgDataIn=(CvLabel *)imgIn->imageData + imgIn_offset;

      for (unsigned int r=0;r<(unsigned int)imgIn_height;r++,
	  imgDataIn+=stepIn,imgDataOut+=stepOut)
      {
	for (unsigned int c=0;c<(unsigned int)imgIn_width;c++)
	{
	  if (imgDataIn[c])
	  {
	    if (blobs.find(imgDataIn[c])==blobs.end()) imgDataOut[c]=0x00;
	    else imgDataOut[c]=(char)0xff;
	  }
	  else
	    imgDataOut[c]=0x00;
	}
      }
    }
    __CV_END__;
  }


  CvLabel cvGetLabel(IplImage const *img, unsigned int x, unsigned int y)
  {
    CV_FUNCNAME("cvGetLabel");
    __CV_BEGIN__;
    {
      CV_ASSERT(img&&(img->depth==IPL_DEPTH_LABEL)&&(img->nChannels==1));

      int step = img->widthStep / (img->depth / 8);
      int img_width = 0;
      int img_height= 0;
      int img_offset = 0;
      if(img->roi)
      {
        img_width = img->roi->width;
        img_height = img->roi->height;
        img_offset = img->roi->xOffset + (img->roi->yOffset * step);
      }
      else
      {
        img_width = img->width;
        img_height= img->height;
      }

      CV_ASSERT((x>=0)&&(x<img_width)&&(y>=0)&&(y<img_height));

      return ((CvLabel *)(img->imageData + img_offset))[x + y*step];
    }
    __CV_END__;
  }




} // namespace_stop





