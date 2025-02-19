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

GapBuffer::~GapBuffer()
{

    if (buffer) {
        free(buffer);
    }

};

void GapBuffer::ExpandBuffer(unsigned int size)
{   

    // Check to see that we actually need to increase the buffer
    // since BufferSize doesn't include the gap.
    if ( ( (bufend - buffer) + size) > BufferSize() ) {

        char *origbuffer = buffer;

        int NewBufferSize = (bufend - buffer) + size  + GAP_SIZE;
        
        buffer = (char *) realloc(buffer, NewBufferSize);

        point += buffer - origbuffer;
        bufend += buffer - origbuffer;
        gapstart += buffer - origbuffer;
        gapend += buffer - origbuffer;
    }

}


void GapBuffer::MoveGapToPoint()
{


    if (point == gapstart) {
        return;
    }

    if (point == gapend) {
        point = gapstart;
        return;
    }

    // Move gap towards the left 
    if (point < gapstart) {
        // Move the point over by gapsize.        
        CopyBytes(point + (gapend-gapstart), point, gapstart - point);
        gapend -= (gapstart - point);
        gapstart = point;
    } else {
        // Since point is after the gap, find distance
        // between gapend and point and that's how
        // much we move from gapend to gapstart.
        CopyBytes(gapstart, gapend, point - gapend);
        gapstart += point - gapend;
        gapend = point;
        point = gapstart;
    }
}