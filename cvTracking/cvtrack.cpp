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


#include "cvt.h"

using namespace std;
namespace cvt // namespace_start
{

double distantBlobTrack(Blob const *b, Track const *t)
{
    double d1;
    if (b->centroid.x<t->bbox.x)
    {
        if (b->centroid.y<t->bbox.y)
            d1 = MAX(t->bbox.x - b->centroid.x, t->bbox.y - b->centroid.y);
        else if (b->centroid.y>(t->bbox.y+t->bbox.height))
            d1 = MAX(t->bbox.x - b->centroid.x, b->centroid.y - (t->bbox.y+t->bbox.height));
        else // if (t->bbox.y < b->centroid.y)&&(b->centroid.y < (t->bbox.y+t->bbox.height))
            d1 = t->bbox.x - b->centroid.x;
    }
    else if (b->centroid.x>(t->bbox.x+t->bbox.width))
    {
        if (b->centroid.y<t->bbox.y)
            d1 = MAX(b->centroid.x - (t->bbox.x+t->bbox.width), t->bbox.y - b->centroid.y);
        else if (b->centroid.y>(t->bbox.y+t->bbox.height))
            d1 = MAX(b->centroid.x - (t->bbox.x+t->bbox.width), b->centroid.y - (t->bbox.y+t->bbox.height));
        else
            d1 = b->centroid.x - (t->bbox.x+t->bbox.width);
    }
    else // if (t->bbox.x =< b->centroid.x) && (b->centroid.x =< (t->bbox.x+t->bbox.width))
    {
        if (b->centroid.y<t->bbox.y)
            d1 = t->bbox.y - b->centroid.y;
        else if (b->centroid.y>(t->bbox.y+t->bbox.height))
            d1 = b->centroid.y - (t->bbox.y+t->bbox.height);
        else
            return 0.;
    }

    double d2;
    if (t->centroid.x<b->bbox.x)
    {
        if (t->centroid.y<b->bbox.y)
            d2 = MAX(b->bbox.x - t->centroid.x, b->bbox.y - t->centroid.y);
        else if (t->centroid.y>(b->bbox.y+b->bbox.height))
            d2 = MAX(b->bbox.x - t->centroid.x, t->centroid.y - (b->bbox.y+b->bbox.height));
        else // if (b->bbox.y < t->centroid.y)&&(t->centroid.y < (b->bbox.y+b->bbox.height))
            d2 = b->bbox.x - t->centroid.x;
    }
    else if (t->centroid.x>(b->bbox.x+b->bbox.width))
    {
        if (t->centroid.y<b->bbox.y)
            d2 = MAX(t->centroid.x - (b->bbox.x+b->bbox.width), b->bbox.y - t->centroid.y);
        else if (t->centroid.y>(b->bbox.y+b->bbox.height))
            d2 = MAX(t->centroid.x - (b->bbox.x+b->bbox.width), t->centroid.y - (b->bbox.y+b->bbox.height));
        else
            d2 = t->centroid.x - (b->bbox.x+b->bbox.width);
    }
    else // if (b->bbox.x =< t->centroid.x) && (t->centroid.x =< b->maxx)
    {
        if (t->centroid.y<b->bbox.y)
            d2 = b->bbox.y - t->centroid.y;
        else if (t->centroid.y>(b->bbox.y+b->bbox.height))
            d2 = t->centroid.y - (b->bbox.y+b->bbox.height);
        else
            return 0.;
    }

    return MIN(d1, d2);
}

// Access to matrix
#define C(blob, track) close[((blob) + (track)*(nBlobs+2))]
// Access to accumulators
#define AB(label) C((label), (nTracks))
#define AT(id) C((nBlobs), (id))
// Access to identifications
#define IB(label) C((label), (nTracks)+1)
#define IT(id) C((nBlobs)+1, (id))
// Access to registers
#define B(label) blobs.find(IB(label))->second
#define T(id) tracks.find(IT(id))->second

void getClusterForTrack(unsigned int trackPos, CvID *close, unsigned int nBlobs, unsigned int nTracks, Blobs const &blobs, Tracks const &tracks, list<Blob*> &bb, list<Track*> &tt);

void getClusterForBlob(unsigned int blobPos, CvID *close, unsigned int nBlobs, unsigned int nTracks, Blobs const &blobs, Tracks const &tracks, list<Blob*> &bb, list<Track*> &tt)
{
    for (unsigned int j=0; j<nTracks; j++)
    {
        if (C(blobPos, j))
        {
            tt.push_back(T(j));

            unsigned int c = AT(j);

            C(blobPos, j) = 0;
            AB(blobPos)--;
            AT(j)--;

            if (c>1)
            {
                getClusterForTrack(j, close, nBlobs, nTracks, blobs, tracks, bb, tt);
            }
        }
    }
}

void getClusterForTrack(unsigned int trackPos, CvID *close, unsigned int nBlobs, unsigned int nTracks, Blobs const &blobs, Tracks const &tracks, list<Blob*> &bb, list<Track*> &tt)
{
    for (unsigned int i=0; i<nBlobs; i++)
    {
        if (C(i, trackPos))
        {
            bb.push_back(B(i));

            unsigned int c = AB(i);

            C(i, trackPos) = 0;
            AB(i)--;
            AT(trackPos)--;

            if (c>1)
            {
                getClusterForBlob(i, close, nBlobs, nTracks, blobs, tracks, bb, tt);
            }
        }
    }
}

void cvUpdateTracks(Blobs const &blobs, Tracks &tracks, const double thDistance, const unsigned int thInactive, const unsigned int thActive)
{
    CV_FUNCNAME("cvUpdateTracks");
    __CV_BEGIN__;

    unsigned int nBlobs = blobs.size();
    unsigned int nTracks = tracks.size();

    // Proximity matrix:
    // Last row/column is for ID/label.
    // Last-1 "/" is for accumulation.
    CvID *close = new unsigned int[(nBlobs+2)*(nTracks+2)]; // XXX Must be same type than Label.

    try
    {
        // Inicialization:
        unsigned int i=0;
        for (Blobs::const_iterator it = blobs.begin(); it!=blobs.end(); ++it, i++)
        {
            AB(i) = 0;
            IB(i) = it->second->label;
        }

        CvID maxTrackID = 0;

        unsigned int j=0;
        for (Tracks::const_iterator jt = tracks.begin(); jt!=tracks.end(); ++jt, j++)
        {
            AT(j) = 0;
            IT(j) = jt->second->id;
            if (jt->second->id > maxTrackID)
                maxTrackID = jt->second->id;
        }

        // Proximity matrix calculation and "used blob" list inicialization:
        for (i=0; i<nBlobs; i++)
            for (j=0; j<nTracks; j++) {
                C(i, j) = (distantBlobTrack(B(i), T(j)) < thDistance);
                if (C(i, j))
                {
                    AB(i)++;
                    AT(j)++;
                }
            }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Detect inactive tracks
        for (j=0; j<nTracks; j++)
        {
            unsigned int c = AT(j);

            if (c==0)
            {
                //cout << "Inactive track: " << j << endl;

                // Inactive track.
                Track *track = T(j);
                track->inactive++;
                track->label = 0;
            }
        }

        // Detect new tracks
        for (i=0; i<nBlobs; i++)
        {
            unsigned int c = AB(i);

            if (c==0)
            {
                //cout << "Blob (new track): " << maxTrackID+1 << endl;
                //cout << *B(i) << endl;

                // New track.
                maxTrackID++;
                Blob *blob = B(i);
                Track *track = new Track;
                track->id = maxTrackID;
                track->label = blob->label;
                track->bbox = blob->bbox;
                track->centroid = blob->centroid;
                track->lifetime = 0;
                track->active = 0;
                track->inactive = 0;
                tracks.insert(CvIDTrack(maxTrackID, track));
            }
        }

        // Clustering
        for (j=0; j<nTracks; j++)
        {
            unsigned int c = AT(j);

            if (c)
            {
                list<Track*> tt;
                tt.push_back(T(j));
                list<Blob*> bb;

                getClusterForTrack(j, close, nBlobs, nTracks, blobs, tracks, bb, tt);

                // Select track
                Track *track;
                unsigned int area = 0;
                for (list<Track*>::const_iterator it=tt.begin(); it!=tt.end(); ++it)
                {
                    Track *t = *it;

                    unsigned int a = ((t->bbox.x+t->bbox.width)-t->bbox.x)*((t->bbox.y+t->bbox.height)-t->bbox.y);
                    if (a>area)
                    {
                        area = a;
                        track = t;
                    }
                }

                // Select blob
                Blob *blob;
                area = 0;
                //cout << "Matching blobs: ";
                for (list<Blob*>::const_iterator it=bb.begin(); it!=bb.end(); ++it)
                {
                    Blob *b = *it;

                    //cout << b->label << " ";

                    if (b->getArea()>area)
                    {
                        area = b->getArea();
                        blob = b;
                    }
                }
                //cout << endl;

                // Update track
                //cout << "Matching: track=" << track->id << ", blob=" << blob->label << endl;
                track->label = blob->label;
                track->centroid = blob->centroid;
                track->bbox = blob->bbox;
                if (track->inactive)
                    track->active = 0;
                track->inactive = 0;

                // Others to inactive
                for (list<Track*>::const_iterator it=tt.begin(); it!=tt.end(); ++it)
                {
                    Track *t = *it;

                    if (t!=track)
                    {
                        //cout << "Inactive: track=" << t->id << endl;
                        t->inactive++;
                        t->label = 0;
                    }
                }
            }
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        for (Tracks::iterator jt=tracks.begin(); jt!=tracks.end();)
            if ((jt->second->inactive>=thInactive)||((jt->second->inactive)&&(thActive)&&(jt->second->active<thActive)))
            {
                delete jt->second;
                tracks.erase(jt++);
            }
            else
            {
                jt->second->lifetime++;
                if (!jt->second->inactive)
                    jt->second->active++;
                ++jt;
            }
    }
    catch (...)
    {
        delete[] close;
        throw; // TODO: OpenCV style.
    }

    delete[] close;

    __CV_END__;
}


} // namespace_end
