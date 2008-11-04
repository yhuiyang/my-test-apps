#!/bin/bash
# put some text in the splash screen (like version etc...)
# this does only work with linux (you need the ImageMagick tool)

VERS=0.6

convert \
    -font @/home/jb/.fonts/captainpodd.ttf \
    -pointsize 44 -fill orange \
    -draw "text 24,185 'wxterm'" \
    -font @/home/jb/.fonts/luxisr.ttf \
    -pointsize 20 -fill white \
	-draw "text 240,185 '$VERS'" \
    -draw "text 24,216 'RS232 + GPIB Terminal'" \
    -pointsize 16 \
    -draw "text 24,240 '2000 - 2006 (c) IFTOOLS GbR" \
    -draw "text 24,260 'http://www.iftools.com ~ Serial Tools, Analyser and more...'" \
	splash.jpg splashvers.jpg

convert splashvers.jpg splashvers.png
b2h -o ../src/about.cpp -H ../src/about.h -s -n about splashvers.jpg

# DO NOT DELETE


