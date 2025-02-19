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