/*
 * =====================================================================================
 *
 *       Filename:  image_sequence.cpp
 *
 *    Description:  Generate a sequence of images based on a given image
 *                  in a particular motion method. Output a list of images
 *                  and the file containing the list of their name.
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>
#include <fstream>
#include <sstream>
#include <unistd.h>     // Used for getcwd

#define EXIT_FILENOTFOUNT(filename) {  \
    exit(-1);  }

std::string imageFilename(const char* origin_filename, int number, 
                            int width = 0, const char* postfix = ".bmp")
{
    std::ostringstream filename_stream;
    filename_stream << origin_filename << ".";
    if(width != 0)
    {
        filename_stream.width(width);
        filename_stream.fill('0');
    }
    filename_stream << number << postfix;
    if(filename_stream)
        return filename_stream.str();

    assert(0);
    return std::string();
}

//   Given the image file \var src with the size of height*width*channels,
// copy it to \var dst with an offset of \var height_offset on the 
// height dimension. Fill the rest part with (255, 255, 255)
//
//   The image is allocated in the form of [h, [w, [c]]], 
//   Take (h, w, c) == (5, 10, 3) as example:
//   [0, 0, 0]  [0, 0, 1]  [0, 0, 2], 
//   [0, 1, 0]     ...        ...
//      ...        ...        ...
//   [0, 9, 0]  [0, 9, 1]  [0, 9, 2],
//   [1, 0, 0]     ...        ...
//      ...        ...     [4, 9, 2].
void imgScrollDown(
            const char*     src, 
            char*           dst, 
            int             width,
            int             height,
            int             channels,
            int             height_offset,
            int             widthstep = 0   // width * channels default
            )
{
    if(widthstep == 0)
        widthstep = width * channels;
    int offset = height_offset;
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
        for (int k=0;k<channels;k++)
        {
            int idpre = (i-offset)*widthstep+j*channels+k;
            int id = i*widthstep+j*channels+k;
            if (idpre<0||idpre>height*widthstep-1)
              dst[id] = 255;
            else
              dst[id] = src[idpre];
        }

}

// Count the width to represent x in decimal base
int decWidth(int x)
{
    int w = 0;
    while(x != 0)
    {
        ++w;
        x /= 10;
    }
    return w;
}

std::string strGetFileName(const std::string& src)
{
    size_t found = src.find_last_of("/\\");
    // found = -1 if not found, -1 + 1 = 0
    return src.substr(found + 1);
}

int main ( int argc, char *argv[] )
{
    std::cerr << "Image sequence generator v0.0.1\n" 
                 "Used only in HTM07 project.\n";
    if(argc != 2)
    {
        std::cerr << "\nUsage: " << argv[0] << " <source file>\n";
    }

    // Get current full path (no final '/')
    std::string tgt_dir;
    {
#define CWD_MAX 300
        char current_dir[CWD_MAX];
        if(getcwd(current_dir, CWD_MAX-1) == NULL)
        {
            std::cerr << "Error: Failed to get current path.\nQuit.\n";
            exit(-1);
        }
        tgt_dir = current_dir;
    }
    // Create target directory
    tgt_dir += "/img_seq/";
    std::string mkdir_cmd = "mkdir -p ";
    mkdir_cmd += tgt_dir;
    int sys_ret;
    sys_ret = system(mkdir_cmd.c_str());

    if(sys_ret == -1)
    {
        std::cerr << "Error: Failed to create directory \"" << tgt_dir 
                  << "\".\nQuit.\n";
        exit(-1);
    }

    // Load source image file
    const char* src_relpath = argv[1];
    IplImage *origin = cvLoadImage(src_relpath, true);
    if(!origin)
    {
        std::cerr << "Error: File \"" << src_relpath << "\" not found.\nQuit.\n";
        exit(-1);
    }

    // Create list file
    std::string listfile_name = tgt_dir + "list.txt";
    std::ofstream listfile(listfile_name.c_str());
    if(!listfile)
    {
        std::cerr << "Error: Failed to create \"" << listfile_name.c_str() 
                  << "\".\nQuit.\n";
        exit(-1);
    }

    // Generate image sequence
    CvSize origin_size = cvGetSize(origin);
    int height = origin_size.height;
    IplImage *img_this = cvCreateImage(origin_size, origin->depth, 
                                        origin->nChannels);
    std::string src_filename = strGetFileName(src_relpath);
    int number_width = decWidth(2*height);
    std::string filename0;
    for(int i = -height; i < height; ++i)
    {
        imgScrollDown(origin->imageData, img_this->imageData, 
                    origin_size.width, origin_size.height,
                    origin->nChannels, i, origin->widthStep);
        std::string imgfilename = imageFilename(src_filename.c_str(), 
                    i+height, number_width);
        imgfilename = tgt_dir + imgfilename;
        cvSaveImage(imgfilename.c_str(), img_this);
        listfile << imgfilename.c_str() << std::endl;
        if (i==0)
        {
            filename0 = imgfilename;
        }
    }
    
    listfile << "q" << std::endl;
    listfile << filename0.c_str() << std::endl;
    listfile << "q" << std::endl;
    listfile.close();
    cvReleaseImage(&img_this);
    cvReleaseImage(&origin);

    std::cerr << "Done.\n";
    return 0;
}		
