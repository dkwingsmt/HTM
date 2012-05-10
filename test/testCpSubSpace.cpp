/*
 * =====================================================================================
 *
 *       Filename:  testTpSort.cpp
 *
 *    Description:  test tp group sort.
 *
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <cv.h>
#include <highgui.h>
#include "Space.h"
#include "SpatialPooler.h"

using namespace htm07;

void showImgCV(const data_t *src, size_t w, size_t h, const char *wnd_name)
{
    IplImage *img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    //IplImage *bigimg = cvCreateImage(cvSize(10*w, 10*h), IPL_DEPTH_8U, 1);
    if(!img)
        return;
    char *now;
    char *row = img->imageData;
    const data_t *nowsrc = src;
    for(size_t y = 0; y < h; ++y)
    {
        now = row;
        for(size_t x = 0; x < w; ++x)
        {
            *now = (*nowsrc) ? 0 : 255;
            ++now;
            ++nowsrc;
        }
        row += img->widthStep;
    }
    /*
    row = bigimg->imageData;
    for(size_t y = 0; y < 10*h; ++y)
    {
        now = row;
        for(size_t x = 0; x < 10*w; ++x)
        {
            *now = img->imageData[(y/10)*img->widthStep + (x/10)];
            ++now;
        }
        row += bigimg->widthStep;
    }
    */
    std::cerr << wnd_name;
    cvShowImage(wnd_name, img);
    cvReleaseImage(&img);
    //cvReleaseImage(&bigimg);
}

//   Load a image with given name, allocate a new data_t array,
// and store the image to the array in the standard format.
void loadImage(const std::string& filename, data_t **o_out, 
            int* o_height, int* o_width)
{
    IplImage *img = cvLoadImage(filename.c_str(), 0);
    if(!img)
    {
        return;
    }
    int height = img->height;
    int width = img->width;
    int channels = img->nChannels;
    int widthstep = img->widthStep;
    data_t *out = new data_t[height * width];
    *o_out = out;
    *o_height = height;
    *o_width = width;

    int i, j, k;
    int nowid = 0;
    unsigned nowdata;
    unsigned char nowchar;
    const char* imgdata = img->imageData;
    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width; ++j)
        {
            nowdata = 0;
            for(k = 0; k < channels; ++k)
            {
                nowchar = imgdata[i * widthstep + j * channels + k];
                nowdata += nowchar;
            }
            out[nowid] = (nowdata < 128); 
            ++nowid;
        }
    }

    cvReleaseImage(&img);
}

bool readVar(char *argv[], size_t *o_stw, size_t *o_sth, 
            size_t *o_cutw, size_t *o_cuth)
{
    size_t stw;
    size_t sth;
    size_t cutw;
    size_t cuth;
    std::istringstream issstw;
    std::istringstream isssth;
    std::istringstream isscutw;
    std::istringstream isscuth;
    issstw.str(argv[2]);
    issstw >> stw;
    isssth.str(argv[3]);
    isssth >> sth;
    isscutw.str(argv[4]);
    isscutw >> cutw;
    isscuth.str(argv[5]);
    isscuth >> cuth;
    if(!issstw || !isssth || !isscutw || !isscuth)
    {
        return false;
    }
    *o_stw = stw;
    *o_sth = sth;
    *o_cutw = cutw;
    *o_cuth = cuth;
    return true;
}

int main (int argc, char *argv[])
{
    if(argc != 6)
    {
        std::cerr << "Useage: " << argv[0] << " <Source Image Name> "
            "<StartWidth> <StartHeight> <CutWidth> <CutHeight>";
        return 0;
    }
    data_t *origin = NULL;
    int width;
    int height;
    loadImage(argv[1], &origin, &height, &width);

    size_t stw;
    size_t sth;
    size_t cutw;
    size_t cuth;
    if(!origin || !(readVar(argv, &stw, &sth, &cutw, &cuth))
        || (stw + cutw > width) || (sth + cuth > height))
    {
        std::cerr << "Useage: " << argv[0] << " <Source Image Name> "
            "<StartWidth> <StartHeight> <CutWidth> <CutHeight>";
        return 0;
    }

    size_t fullmax[2] = {height, width};
    VecT fullvec;
    fullvec.dims = 2;
    fullvec.max = fullmax;

    SpaceT fullspace(&fullvec);

    size_t startmax[2] = {stw, sth};
    VecT startvec;
    startvec.dims = 2;
    startvec.max = startmax;

    size_t cutmax[2] = {cutw, cuth};
    VecT cutvec;
    cutvec.dims = 2;
    cutvec.max = cutmax;

    SpaceT cutspace(&startvec, &cutvec, &fullspace);

    data_t *cutimg = new data_t[cutw * cuth];
    copyFromSpaceToSubSpace(origin, cutimg, &cutspace);

    showImgCV(origin, width, height, "Origin");
    showImgCV(cutimg, cutw, cuth, "Cut");

    cvWaitKey();

    delete[] origin;
    delete[] cutimg;

    return 0;
}		
