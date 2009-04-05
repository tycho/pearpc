#!/usr/bin/python 
# Copyright (c) 2004 Marco Lange.

import sys 

GRANULARITY = 516096 

imagename = sys.argv[1]
imagesize = int(sys.argv[2])
if imagesize % GRANULARITY != 0: 
    imagesize = ((imagesize / GRANULARITY) + 1) * GRANULARITY 

print "Using image size:", imagesize 

imagefile = open(imagename, "wb+")
imagefile.seek(imagesize-1)
imagefile.write("\x00")
imagefile.close
