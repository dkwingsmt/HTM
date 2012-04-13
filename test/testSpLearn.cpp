/*
 * =====================================================================================
 *
 *       Filename:  testSpLearn.cpp
 *
 *    Description:  Input certain pictures to Sp. Test if sp is correctly 
 *                  working when in learning state.
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cv.h>
#include <highgui.h>
#include "Layer.h"
#include "Common.h"

using namespace htm07;

#define DIMS            2
#define INPUT_HEIGHT    100
#define INPUT_WIDTH     100
#define NODE_HEIGHT     4
#define NODE_WIDTH      4

std::string strip(std::string instr, const char* charset)
{
    std::string::size_type first = instr.find_first_not_of(charset);
    return ( first == std::string::npos )
        ? std::string()
        : instr.substr(first, instr.find_last_not_of(charset) - first + 1);
}

void loadImage(const std::string& filename, data_t **p_out);
void introspectionGnuplot(LayerT *layer);

//   Load picture sequence by reading continuous file name of pictures
// ended by '\n'. 
//   Input a single "q\n" to stop.
//   Meet any error (e.g. file not found) and it's stop.
int main ( int argc, char *argv[] )
{
    VecT layer1size;
    initializeVec(&layer1size, DIMS);
    layer1size.max[0] = INPUT_HEIGHT;
    layer1size.max[1] = INPUT_WIDTH;
    VecT layer1nodesize;
    initializeVec(&layer1nodesize, DIMS);
    layer1nodesize.max[0] = NODE_HEIGHT;
    layer1nodesize.max[1] = NODE_WIDTH;
    LayerT *layer1 = new LayerT(&layer1size, &layer1nodesize);

    data_t *input_data = NULL;

    std::string infilename;
    while(1)
    {
        std::getline(std::cin, infilename);
        infilename = strip(infilename, " \t\n\r");
        if(infilename.size() == 0)
            continue;
        else if(infilename == "q")
            break;

        loadImage(infilename, &input_data);
        if(input_data == NULL)
        {
            std::cerr << "Error: File \"" << infilename << "\" not found.\n";
            break;
        }
        layer1->expose(input_data);
        introspectionGnuplot(layer1);
    }

    return 0;
}		

//   Load a image with given name, allocate a new data_t array,
// and store the image to the array in the standard format.
void loadImage(const std::string& filename, data_t **p_out)
{
    IplImage *img = cvLoadImage(filename.c_str(), true);
    if(!img)
    {
        *p_out = NULL;
        return;
    }
    int height = img->height;
    int width = img->width;
    int channels = img->nChannels;
    int widthstep = img->widthStep;

    data_t *out = new data_t[height*width*widthstep];
    assert(out && "Allocation failed.");

    int i, j, k;
    int nowid = 0;
    unsigned nowdata;
    unsigned char nowchar;
    const char* imgdata = img->imageData;
    std::cerr << '\n';
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
            //std::cerr << (nowdata);
            // 0x0 if white, 0x1 otherwise; gave a bit of tolerance
            out[nowid] = (nowdata < 0x2f0); 
            std::cerr << out[nowid];
            ++nowid;
        }
        std::cerr << '\n';
    }

    *p_out = out;
}

// Use the layer to output debug info.
void introspectionGnuplot(LayerT *layer)
{

}
