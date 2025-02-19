#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "gap_buffer.h"

using namespace std;

GapBuffer::GapBuffer(int gsize) : GAP_SIZE(gsize), buffer(NULL)
{
    InitBuffer(GAP_SIZE);

};

GapBuffer::GapBuffer(FILE *file, int gsize) : GAP_SIZE(gsize)
{
    struct stat buf;
    
    fstat(fileno(file), &buf);
    long file_size = buf.st_size;
    InitBuffer(file_size + GAP_SIZE);    
    MoveGapToPoint();
    ExpandGap( (int)file_size );
    unsigned int amount = fread(gapstart, 1, file_size, file);

    gapstart += amount;
};

GapBuffer::GapBuffer(const GapBuffer& tb)
{
     GAP_SIZE = tb.GAP_SIZE;
     
     buffer = (char *) malloc(tb.bufend - tb.buffer);
     
     strcpy(buffer, tb.buffer);

     bufend = buffer + (tb.bufend - tb.buffer);
     gapstart = buffer + (tb.gapstart - tb.buffer);
     gapend = gapstart + (tb.gapend - tb.gapstart);
     point = buffer + (tb.point - tb.buffer);
    
}
