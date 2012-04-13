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

#define EXIT_FILENOTFOUNT(filename) {  \
    std::cerr << "Error: File \"" << (filename) << "\" not found.\nQuit.\n"; \
    exit(-1);  }

std::string imageFilename(const char* origin_filename, int number, 
                            int width = 0, const char* postfix = ".bmp")
{
    std::ostringstream filename_stream;
    filename_stream << origin_filename << ".";
    if(width != 0)
    {
        filename_stream.width(width);
    }
    filename_stream << number << postfix;
    if(filename_stream)
        return filename_stream.str();

    assert(0);
    return std::string();
}

//   Given the image file \var src with the size of height*width*depth,
// copy it to \var dst with an offset of \var height_offset on the 
// height dimension. Fill the rest part with (255, 255, 255)
//
//   The image is allocated in the form of [h, w, d], i.e.
//   [0, 0, 0]       [0, 0, 1]       [0, 0, 2], 
//   [0, 1, 0]       ...             ...
//   ...             ...             ...
//   [0, width-1, 0] [0, width-1, 1] [0, width-1, 2],
//   [1, 0, 0]       ...             ...
//   ...             ...             [height-1, width-1, 2].
void imgScrollDown(
            const char*     src, 
            char*           dst, 
            int             width,
            int             height,
            int             depth,
            int             height_offset
            )
{
    for (int i=0;i<height;i++)
      for (int j=0;j<width;j++)
        for (int k=0;k<depth;k++)
        {
            int idpre = (i-offset)*width*depth+j*depth+k;
            int id = i*width*depth+j*depth+k;
            if (idpre<0||idpre>height*width*depth-1)
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

int main ( int argc, char *argv[] )
{
    std::cerr << "Image sequence generator v0.0.1\n" 
                 "Used only in HTM07 project.\n"
                 "\n";
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <source file>\n";
    }

    // Create target directory
    std::string tgt_dir = "./img_seq/";
    std::string mkdir_cmd = "mkdir -p ";
    mkdir_cmd += tgt_dir;
    int sys_ret;
    sys_ret = system(mkdir_cmd.c_str());

    if(sys_ret == -1)
    {
        std::cerr << "Error: Failed to create directory \"" << tgt_dir 
                  << "\".\nQuit.\n";
        return -1;
    }

    // Load source image file
    const char* src_file = argv[1];
    IplImage *origin = cvLoadImage(src_file, true);
    if(!origin)
    {
        EXIT_FILENOTFOUNT(src_file);
    }

    std::string listfile_name = tgt_dir + "list.txt";
    std::ofstream listfile(listfile_name.c_str());
    if(!listfile)
    {
        EXIT_FILENOTFOUNT(listfile_name.c_str());
    }

    // Generate image sequence
    CvSize origin_size = cvGetSize(origin);
    int height = origin_size.height;
    IplImage *img_this = cvCreateImage(origin_size, origin->depth, 
                                        origin->nChannels);
    int number_width = decWidth(2*height);
    for(int i = -height; i < height; ++i)
    {
        imgScrollDown(origin->imageData, img_this->imageData, 
                    origin_size.width, origin_size.height,
                    origin->depth, i);
        std::string imgfilename = imageFilename(src_file, i+height, 
                    number_width);
        cvSaveImage(imgfilename.c_str(), img_this);
        listfile << imgfilename.c_str() << std::endl;
    }

    listfile.close();
    cvReleaseImage(&img_this);
    cvReleaseImage(&origin);
    return 0;
}		
