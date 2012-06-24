/*
 * =====================================================================================
 *
 *       Filename:  testSpTableView.cpp
 *
 *    Description:  testSpLearn + Print the graph on paper/Fig.10(b) & Fig.14
 *                  Only accept 12x12 graph or it won't work.
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cv.h>
#include <highgui.h>
#include <list>
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

inline int max_binary(int lhs, int rhs) { return (lhs < rhs) ? rhs : lhs; }
inline int min_binary(int lhs, int rhs) { return (lhs > rhs) ? rhs : lhs; }
inline size_t max_binary(size_t lhs, size_t rhs) { return (lhs < rhs) ? rhs : lhs; }
inline size_t min_binary(size_t lhs, size_t rhs) { return (lhs > rhs) ? rhs : lhs; }

void prepareSpaces(const SpaceT *fulldataspace, 
            SpaceT **o_nodesspace, SpaceT ***o_subdataspaces);
void prepareAllocInfo(SpaceT const*const*inputsubspaces, data_t *inputdata, 
            size_t nodesnum, AllocInfoT **o_alinfo);
void splitImage(const data_t *indata, size_t nodenum, 
            SpaceT const*const*inputsubspaces, AllocInfoT *allocinfo);
void showImgCV(const data_t *src, size_t w, size_t h, const char *wnd_name = "htm");

void loadImage(const std::string& filename, data_t *out);
void initializeGnuplot(const LayerT *layer);
void outputGnuplot(const LayerT *layer);
void displayGnuplot();
void printNodeSpTable(const NodeT* node);
void printNodeTpSort(const NodeT* node);

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
    l1inputspacevec.dims = DIMS;
    l1inputspacevec.max = l1inputspacemax;
    l1inputspacevec.max[0] = INPUT_HEIGHT;
    l1inputspacevec.max[1] = INPUT_WIDTH;
    SpaceT l1inputspace(&l1inputspacevec);

    SpaceT *l1nodesspace;
    SpaceT **l1inputsubspaces;
    AllocInfoT *l1alinfo;

    data_t *l1input = new data_t[INPUT_HEIGHT * INPUT_WIDTH];
    assert(l1input && "Allocation failed.");
    prepareSpaces(&l1inputspace, &l1nodesspace, &l1inputsubspaces);
    std::cerr << l1inputsubspaces << '\n';
    //TODO
    //std::cerr << "{" << l1inputsubspaces->getLength(0) << "," << l1inputsubspaces->getLength(1) << "}\n";
    prepareAllocInfo(l1inputsubspaces, l1input, l1nodesspace->getSize(),
                &l1alinfo);
    size_t l1nodesnum = l1nodesspace->getSize();

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
        splitImage(input_data, l1nodesnum, l1inputsubspaces, l1alinfo);
        layer1->expose();
        outputGnuplot(layer1);
    }
    displayGnuplot();
    IntrospectionT is;
    //printNodeSpTable(is.getLayerNodes(layer1)[4]);
    layer1->forceConclude();
    printNodeTpSort(is.getLayerNodes(layer1)[4]);
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
                nowchar = imgdata[(i * widthstep + j * channels) + k];
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
            SpaceT **o_nodesspace, SpaceT ***o_subdataspaces)
{
    size_t nodessmax[2];
    VecT nodess;
    nodess.dims = 2;
    nodess.max = nodessmax;
    nodessmax[0] = 3;
    nodessmax[1] = 3;
    *o_nodesspace = new SpaceT(&nodess);

    SpaceT **spaces = (new SpaceT *[9]);
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

    startmax[1] = 0;
    startmax[0] = 0;
    spaces[0] = new SpaceT(&start, &size, fulldataspace);
    startmax[0] = 4;
    spaces[1] = new SpaceT(&start, &size, fulldataspace);
    startmax[0] = 8;
    spaces[2] = new SpaceT(&start, &size, fulldataspace);
    startmax[1] = 4;
    startmax[0] = 0;
    spaces[3] = new SpaceT(&start, &size, fulldataspace);
    startmax[0] = 4;
    spaces[4] = new SpaceT(&start, &size, fulldataspace);
    startmax[0] = 8;
    spaces[5] = new SpaceT(&start, &size, fulldataspace);
    startmax[1] = 8;
    startmax[0] = 0;
    spaces[6] = new SpaceT(&start, &size, fulldataspace);
    startmax[0] = 4;
    spaces[7] = new SpaceT(&start, &size, fulldataspace);
    startmax[0] = 8;
    spaces[8] = new SpaceT(&start, &size, fulldataspace);

}

//   Assume nodesnum == size(inputdata), where inputdata is where the formatted
// data is placed, allocate inputdata to nodes
void prepareAllocInfo(SpaceT const*const*inputsubspaces, data_t *inputdata, 
            size_t nodesnum, AllocInfoT **o_alinfo)
{
    data_t *nowtail = inputdata;
    AllocInfoT *alinfo = new AllocInfoT[nodesnum];
    assert(alinfo && "Allocation failed.");
    *o_alinfo = alinfo;
    for(size_t i = 0; i < nodesnum; ++i)
    {
        alinfo[i].pos = nowtail;
        alinfo[i].len = inputsubspaces[i]->getSize();
        nowtail += alinfo[i].len;
    }
}

// Reformat the origin integral data into what suits nodes to grab integrally
void splitImage(const data_t *indata, size_t nodesnum, 
            SpaceT const*const*inputsubspaces, AllocInfoT *allocinfo)
{
    for(size_t i = 0; i < nodesnum; ++i)
    {
        copyFromSpaceToSubSpace(indata, allocinfo[i].pos, inputsubspaces[i]);
    }
}

// Use the layer to output debug info.
void initializeGnuplot(const LayerT *layer)
{
    std::cout << "clear" << std::endl;
    std::cout << "set grid 0 " << std::endl;
    std::cout << "set key off" << std::endl;

}

void convertImgBinaryToCV(const data_t *src, IplImage *dst)
{
    if(!dst)
        return;
    CvSize img_size = cvGetSize(dst);
    char *now;
    char *row = dst->imageData;
    const data_t *nowsrc = src;
    for(int y = 0; y < img_size.height; ++y)
    {
        now = row;
        for(int x = 0; x < img_size.width; ++x)
        {
            *now = (*nowsrc) ? 0 : 255;
            ++now;
            ++nowsrc;
        }
        row += dst->widthStep;
    }
}

void printNodeSpTable(const NodeT* node)
{
    IntrospectionT is;
    const AdjMatT *adj_mat = is.getNodeSpAdjMat(node);    
    const SpPatternListT *ptlst = is.getNodeSpPatternList(node);

    // Structure of disp:
    //     col0 and row0: pattern
    //     other cols and rows: normalized weight of pat_col and pat_row
    //     surrounded by a weight boarder
    
    // Prepare
    const int BOARDER_WIDTH = 30;
    const size_t MAX_PATTERN_NUM = 12;
    const int PAT_SCALE_FACTOR = 8;
    const int PAT_BOARDER_WIDTH = 10;
    int pat_h = 4;
    int pat_w = 4;
    size_t pattern_num = ((*ptlst).size() > MAX_PATTERN_NUM) ? MAX_PATTERN_NUM 
                                                            : (*ptlst).size();
    int block_size = ((pat_h > pat_w) ? pat_h : pat_w) * PAT_SCALE_FACTOR 
                     + 2 * PAT_BOARDER_WIDTH;

    int disp_size = 2 * BOARDER_WIDTH + (1 + pattern_num) * block_size;
    IplImage *disp = cvCreateImage(cvSize(disp_size, disp_size), IPL_DEPTH_8U, 1);
    IplImage *pat_now = cvCreateImage(cvSize(pat_h, pat_w), IPL_DEPTH_8U, 1);
    IplImage *pat_bg = cvCreateImage(cvSize(block_size, block_size), IPL_DEPTH_8U, 1);
    IplImage *block_now = cvCreateImage(cvSize(block_size, block_size), 
                                               IPL_DEPTH_8U, 1);
    CvRect pat_bg_rect = cvRect((block_size - pat_w * PAT_SCALE_FACTOR) / 2,
                                (block_size - pat_h * PAT_SCALE_FACTOR) / 2,
                                pat_w * PAT_SCALE_FACTOR,
                                pat_h * PAT_SCALE_FACTOR);
    cvSet(disp, cvScalarAll(127));
    cvSet(pat_bg, cvScalarAll(127));
    cvRectangle(pat_bg, 
                cvPoint(pat_bg_rect.x - 1, pat_bg_rect.y - 1), 
                cvPoint(pat_bg_rect.x + pat_bg_rect.width, 
                        pat_bg_rect.y + pat_bg_rect.height),  
                cvScalarAll(0));

    // Find the largest weight
    float largest_weight = 0;
    {
        float now_weight;
        for(size_t i = 0; i < pattern_num; ++i)
        {
            for(size_t j = i; j < pattern_num; ++j)
            {
                now_weight = (*adj_mat)[i][j];
                if(now_weight > largest_weight)
                    largest_weight = now_weight;
            }
        }
    }

    // Draw pattern
    {
        CvRect tgt_rect_up = cvRect(BOARDER_WIDTH + block_size,
                                    BOARDER_WIDTH,
                                    block_size, block_size);
        CvRect tgt_rect_left = cvRect(BOARDER_WIDTH,
                                      BOARDER_WIDTH + block_size,
                                      block_size, block_size);
        for(size_t i = 0; i < pattern_num; ++i)
        {
            convertImgBinaryToCV(ptlst->getPattern(i), pat_now);
            cvSetImageROI(pat_bg, pat_bg_rect);
            cvResize(pat_now, pat_bg, CV_INTER_NN);
            cvResetImageROI(pat_bg);

            cvSetImageROI(disp, tgt_rect_up);
            cvCopy(pat_bg, disp);
            cvResetImageROI(disp);

            cvSetImageROI(disp, tgt_rect_left);
            cvCopy(pat_bg, disp);
            cvResetImageROI(disp);

            tgt_rect_up.x += block_size;
            tgt_rect_left.y += block_size;
        }
    }
    // Draw block
    {
        CvRect tgt_rect = cvRect(BOARDER_WIDTH + block_size,
                                 BOARDER_WIDTH + block_size,
                                 block_size, block_size);
        float now_weight;
        for(size_t i = 0; i < pattern_num; ++i)
        {
            tgt_rect.x = BOARDER_WIDTH + block_size;
            for(size_t j = 0; j < pattern_num; ++j)
            {
                now_weight = (*adj_mat)[i][j] / largest_weight;
                cvSet(block_now, cvScalarAll(255 * now_weight));
                cvSetImageROI(disp, tgt_rect);
                cvCopy(block_now, disp);
                cvResetImageROI(disp);
                tgt_rect.x += block_size;
            }
            tgt_rect.y += block_size;
        }
    }

    cvShowImage("SpTable", disp);
    cvWaitKey();
    cvReleaseImage(&disp);
    cvReleaseImage(&pat_now);
    cvReleaseImage(&pat_bg);
    cvReleaseImage(&block_now);
}

void printNodeTpSort(const NodeT* node)
{
    IntrospectionT is;
    size_t group_num = is.getNodeTpGroupNum(node);
    const SpPatternListT *pat_lst = is.getNodeSpPatternList(node);
    const size_t pat_w = 4;
    const size_t pat_h = 4;
    const size_t *sort_table = is.getNodeTpGroups(node); // pat->grpid
    const size_t MAX_COLS = 10;
    const size_t BOARDER_WIDTH = 30;
    const size_t PATTERN_RESIZE_FACTOR = 8;
    // Spacing between cols and rows within a group
    const size_t PATTERN_SPACING = 10; 
    // Spacing between groups
    const size_t GROUP_SPACING = 10;
    size_t block_w = pat_w * PATTERN_RESIZE_FACTOR + 2;
    size_t block_h = pat_h * PATTERN_RESIZE_FACTOR + 2;

    //   Make group table for drawing.
    //   Every group can has many rows, every of which can have at most
    // MAX_COLS patterns.
    size_t patnum = pat_lst->size();
    std::vector<size_t> group_sizes; // grp->grp_size
    std::vector<std::list<std::list<size_t> > > draw_table; // grp->row->patids
    std::list<std::list<size_t> > default_line(1, std::list<size_t>());
    group_sizes.resize(group_num, 0);
    draw_table.resize(group_num, default_line);
    size_t max_pat_in_grp = 0;
    for(size_t pat_id = 0; pat_id < patnum; ++pat_id)
    {
        size_t this_grp_id = sort_table[pat_id];
        max_pat_in_grp = max_binary(++group_sizes[this_grp_id], max_pat_in_grp);
        if(draw_table[this_grp_id].back().size() == MAX_COLS)
            draw_table[this_grp_id].push_back(std::list<size_t>());
        draw_table[this_grp_id].back().push_back(pat_id);
    }

    // Calculate the size of disp
    int disp_width = BOARDER_WIDTH * 2 + PATTERN_SPACING + 
        min_binary(MAX_COLS, max_pat_in_grp) * (PATTERN_SPACING + block_w);
    int disp_height = BOARDER_WIDTH * 2 - GROUP_SPACING;     // Initializing
    for(size_t grp_id = 0; grp_id < group_num; ++grp_id)
    {
        int row_num = draw_table[grp_id].size();
        disp_height += GROUP_SPACING + PATTERN_SPACING
            + row_num * (block_h + PATTERN_SPACING);
    }

    // Draw prepare
    IplImage *disp = cvCreateImage(cvSize(disp_width, disp_height), IPL_DEPTH_8U, 1);
    IplImage *pat_now = cvCreateImage(cvSize(pat_w, pat_h), IPL_DEPTH_8U, 1);
    IplImage *pat_bg = cvCreateImage(cvSize(block_w, block_h), IPL_DEPTH_8U, 1);
    cvSet(disp, cvScalarAll(255));  
    cvSet(pat_bg, cvScalarAll(0));  // Black boarder
    CvRect pat_bg_rect = cvRect(1, 1, block_w-2, block_h-2);
    CvRect disp_rect_now = cvRect(BOARDER_WIDTH + PATTERN_SPACING,
                BOARDER_WIDTH + PATTERN_SPACING, block_w, block_h);
    CvPoint grp_rect_p1 = cvPoint(BOARDER_WIDTH, BOARDER_WIDTH);
    CvPoint grp_rect_p2 = cvPoint(BOARDER_WIDTH, BOARDER_WIDTH);
    // Draw
    for(size_t grp_id = 0; grp_id < group_num; ++grp_id)
    {
        // Group boarder
        grp_rect_p2.x = grp_rect_p1.x + PATTERN_SPACING + 
            draw_table[grp_id].front().size() * (PATTERN_SPACING + block_w);
        grp_rect_p2.y = grp_rect_p1.y + PATTERN_SPACING + 
            draw_table[grp_id].size() * (PATTERN_SPACING + block_h);
        cvSetImageROI(disp, cvRect(grp_rect_p1.x, grp_rect_p1.y, 
                                grp_rect_p2.x - grp_rect_p1.x, 
                                grp_rect_p2.y - grp_rect_p1.y));
        cvSet(disp, cvScalarAll(127));
        cvResetImageROI(disp);
        cvRectangle(disp, grp_rect_p1, grp_rect_p2, cvScalarAll(0));        

        // Row
        std::list<std::list<size_t> >::iterator 
            row_cur = draw_table[grp_id].begin(),
            row_end = draw_table[grp_id].end();
        for(; row_cur != row_end; ++row_cur)
        {
            std::list<size_t>::iterator it_cur = row_cur->begin(),
                                          it_end = row_cur->end();
            for(; it_cur != it_end; ++it_cur)
            {
                convertImgBinaryToCV(pat_lst->getPattern(*it_cur), pat_now);

                cvSetImageROI(pat_bg, pat_bg_rect);
                cvResize(pat_now, pat_bg, CV_INTER_NN);
                cvResetImageROI(pat_bg);

                cvSetImageROI(disp, disp_rect_now);
                cvCopy(pat_bg, disp);
                cvResetImageROI(disp);

                disp_rect_now.x += PATTERN_SPACING + block_w;
            }
            disp_rect_now.x = BOARDER_WIDTH + PATTERN_SPACING;
            disp_rect_now.y += PATTERN_SPACING + block_h;
        }
        disp_rect_now.y += GROUP_SPACING + PATTERN_SPACING;
        grp_rect_p1.y = grp_rect_p2.y + GROUP_SPACING;
    }

    cvShowImage("TpSort", disp);
    cvWaitKey();
    cvReleaseImage(&disp);
    cvReleaseImage(&pat_now);
    cvReleaseImage(&pat_bg);
}

void showImgCV(const data_t *src, size_t w, size_t h, const char *wnd_name)
{
    IplImage *img = cvCreateImage(cvSize(h, w), IPL_DEPTH_8U, 1);
    IplImage *bigimg = cvCreateImage(cvSize(10*h, 10*w), IPL_DEPTH_8U, 1);
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
    std::cerr << wnd_name;
    cvShowImage(wnd_name, bigimg);
    cvReleaseImage(&img);
    cvReleaseImage(&bigimg);
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
