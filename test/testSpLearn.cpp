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

void prepareSpaces(const SpaceT *fulldataspace, 
            SpaceT **o_nodesspace, SpaceT **o_subdataspaces);
void prepareAllocInfo(const SpaceT *nodesspaces, data_t *inputdata, 
            size_t nodesnum, AllocInfoT **o_alinfo);
void splitImage(const data_t *indata, const SpaceT *nodesspaces, 
            AllocInfoT *allocinfo);

void loadImage(const std::string& filename, data_t *out);
void initializeGnuplot(const LayerT *layer);
void outputGnuplot(const LayerT *layer);
void displayGnuplot();

//   Load picture sequence by reading continuous file name of pictures
// ended by '\n'. 
//   Input a single "q\n" to stop.
//   Meet any error (e.g. file not found) and it stops.
int main ( int argc, char *argv[] )
{
    data_t *input_data = new data_t[INPUT_HEIGHT * INPUT_WIDTH];
    assert(input_data && "Allocation failed.");

    size_t l1inputspacemax[DIMS];
    VecT l1inputspacevec;
    l1inputspacevec.max = l1inputspacemax;
    l1inputspacevec.max[0] = NODES_HEIGHT;
    l1inputspacevec.max[1] = NODES_WIDTH;
    SpaceT l1inputspace(&l1inputspacevec);

    SpaceT *l1nodesspace;
    SpaceT *l1inputsubspaces;
    AllocInfoT *l1alinfo;

    data_t *l1input = new data_t[INPUT_HEIGHT * INPUT_WIDTH];
    assert(l1input && "Allocation failed.");
    prepareSpaces(&l1inputspace, &l1nodesspace, &l1inputsubspaces);
    prepareAllocInfo(l1inputsubspaces, l1input, l1nodesspace->getSize(),
                &l1alinfo);

    LayerT *layer1 = new LayerT(*l1nodesspace, l1alinfo);

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
        splitImage(input_data, l1inputsubspaces, l1alinfo);
        layer1->expose();
        outputGnuplot(layer1);
    }
    displayGnuplot();
    while(1);

    delete []input_data;
    delete []l1input;
    delete l1nodesspace;
    delete []l1inputsubspaces;
    delete layer1;

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

// TODO(mt): generalize it 
//   Split the full data into subspaces for nodes
//   Input is the space for full data, output is the space for nodes and 
// array of subspaces from the fulldataspace 
void prepareSpaces(const SpaceT *fulldataspace, 
            SpaceT **o_nodesspace, SpaceT **o_subdataspaces)
{
    size_t nodessmax[2];
    VecT nodess;
    nodess.dims = 2;
    nodess.max = nodessmax;
    nodessmax[0] = 3;
    nodessmax[1] = 3;
    *o_nodesspace = new SpaceT(&nodess);

    SpaceT *spaces = (SpaceT *)(new void *[9]);
    *o_subdataspaces = spaces;

    size_t startmax[2];
    VecT start;
    start.dims = 2;
    start.max = startmax;
    startmax[0] = 0;
    startmax[1] = 0;

    size_t sizemax[2];
    VecT size;
    size.dims = 2;
    size.max = sizemax;
    sizemax[0] = 4;
    sizemax[1] = 4;

    startmax[0] = 0;
    startmax[1] = 0;
    new (spaces + 0) SpaceT(&start, &size, fulldataspace);
    startmax[1] = 4;
    new (spaces + 1) SpaceT(&start, &size, fulldataspace);
    startmax[1] = 8;
    new (spaces + 2) SpaceT(&start, &size, fulldataspace);
    startmax[0] = 1;
    startmax[1] = 0;
    new (spaces + 3) SpaceT(&start, &size, fulldataspace);
    startmax[1] = 4;
    new (spaces + 4) SpaceT(&start, &size, fulldataspace);
    startmax[1] = 8;
    new (spaces + 5) SpaceT(&start, &size, fulldataspace);
    startmax[0] = 2;
    startmax[1] = 0;
    new (spaces + 6) SpaceT(&start, &size, fulldataspace);
    startmax[1] = 4;
    new (spaces + 7) SpaceT(&start, &size, fulldataspace);
    startmax[1] = 8;
    new (spaces + 8) SpaceT(&start, &size, fulldataspace);

}

//   Assume nodesnum == size(inputdata), where inputdata is where the formatted
// data is placed, allocate inputdata to nodes
void prepareAllocInfo(const SpaceT *nodesspaces, data_t *inputdata, 
            size_t nodesnum, AllocInfoT **o_alinfo)
{
    data_t *nowtail = inputdata;
    AllocInfoT *alinfo = new AllocInfoT[nodesnum];
    assert(alinfo && "Allocation failed.");
    *o_alinfo = alinfo;
    for(size_t i = 0; i < nodesnum; ++i)
    {
        alinfo[i].pos = nowtail;
        alinfo[i].len = nodesspaces->getSize();
        nowtail += alinfo[i].len;
    }
}

// Reformat the origin integral data into what suits nodes to grab integrally
void splitImage(const data_t *indata, const SpaceT *nodesspaces, 
            AllocInfoT *allocinfo)
{
    size_t nodesnum = nodesspaces->getSize();
    for(size_t i = 0; i < nodesnum; ++i)
    {
        copyFromSpaceToSubSpace(indata, allocinfo->pos, nodesspaces);
    }
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
