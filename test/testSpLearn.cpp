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
#include "Introspection.h"

using namespace htm07;

#define DIMS            2
#define INPUT_HEIGHT    12
#define INPUT_WIDTH     12
#define NODES_HEIGHT    3
#define NODES_WIDTH     3

std::string strip(const std::string& instr, const char* charset)
{
    std::string::size_type first = instr.find_first_not_of(charset);
    return ( first == std::string::npos )
        ? std::string()
        : instr.substr(first, instr.find_last_not_of(charset) - first + 1);
}

void loadImage(const std::string& filename, data_t *out);
void initializeGnuplot(const LayerT *layer);
void outputGnuplot(const LayerT *layer);
void displayGnuplot();

//   Load picture sequence by reading continuous file name of pictures
// ended by '\n'. 
//   Input a single "q\n" to stop.
//   Meet any error (e.g. file not found) and it's stop.
int main ( int argc, char *argv[] )
{
    data_t *input_data = new data_t[INPUT_HEIGHT * INPUT_WIDTH];
    assert(input_data && "Allocation failed.");

    VecT layer1nodes;
    initializeVec(&layer1nodes, DIMS);
    layer1nodes.max[0] = NODES_HEIGHT;
    layer1nodes.max[1] = NODES_WIDTH;
    SpaceT l1nodesspace(&layer1nodes);

    AllocInfoT l1alinfo[NODES_HEIGHT * NODES_WIDTH];
    l1alinfo[0].pos = input_data + 0;
    l1alinfo[0].len = 16;
    l1alinfo[1].pos = input_data + 4;
    l1alinfo[1].len = 16;
    l1alinfo[2].pos = input_data + 8;
    l1alinfo[2].len = 16;
    l1alinfo[3].pos = input_data + 0 + 48;
    l1alinfo[3].len = 16;
    l1alinfo[4].pos = input_data + 4 + 48;
    l1alinfo[4].len = 16;
    l1alinfo[5].pos = input_data + 8 + 48;
    l1alinfo[5].len = 16;
    l1alinfo[6].pos = input_data + 0 + 96;
    l1alinfo[6].len = 16;
    l1alinfo[7].pos = input_data + 4 + 96;
    l1alinfo[7].len = 16;
    l1alinfo[8].pos = input_data + 8 + 96;
    l1alinfo[8].len = 16;

    LayerT *layer1 = new LayerT(&layer1nodes, l1alinfo);

    initializeGnuplot(layer1);
    std::string infilename;
    while(1)
    {
        system("read n1");
        std::getline(std::cin, infilename);
        std::cerr << infilename << "\n"; // TODO:Debug
        if(!std::cin)
            break;
        infilename = strip(infilename, " \t\n\r");
        if(infilename.size() == 0)
            continue;
        else if(infilename == "q")
            break;

        loadImage(infilename, input_data);
        if(input_data == NULL)
        {
            std::cerr << "Error: File \"" << infilename << "\" not found.\n";
            break;
        }
        layer1->expose();
        outputGnuplot(layer1);
    }
    displayGnuplot();
    while(1);

    return 0;
}		

//   Load a image with given name, allocate a new data_t array,
// and store the image to the array in the standard format.
void loadImage(const std::string& filename, data_t *out)
{
    IplImage *img = cvLoadImage(filename.c_str(), true);
    if(!img)
    {
        return;
    }
    int height = img->height;
    int width = img->width;
    int channels = img->nChannels;
    int widthstep = img->widthStep;

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
            // 0x0 if white, 0x1 otherwise; gave a bit of tolerance
            out[nowid] = (nowdata < 0x2f0); 
            ++nowid;
        }
    }

    cvReleaseImage(&img);
}

// Use the layer to output debug info.
void initializeGnuplot(const LayerT *layer)
{
    std::cout << "clear" << std::endl;
    std::cout << "set grid 0 " << std::endl;
    std::cout << "set key off" << std::endl;

}

void outputGnuplot(const LayerT *layer)
{
    IntrospectionT is;
    size_t nodenum = is.getLayerNodeNum(layer);
    const SpaceT *nodes_space = layer->nodesSpace();

    size_t xmax = nodes_space->getTotalLength(0);
    size_t ymax = nodes_space->getTotalLength(1);
    std::cout << "splot [0:" << xmax << "] [0:" << ymax << "] [0:10] "
              << " '-' " << std::endl;
    for(size_t i = 0; i < nodenum; ++i)
    {
        size_t x = nodes_space->getTotalCoord(i, 0);
        size_t y = nodes_space->getTotalCoord(i, 1);
        size_t centernum = is.getLayerNodeCenterNum(layer, i);
        std::cout << x << ' ' << y << ' ' << centernum << std::endl;
    }
}

void displayGnuplot()
{
    std::cout << "e" << std::endl;
}
