#!/bin/bash

for l in "de" "en"; do
    ZIPFILES="wxterm_$l.hhc
        wxterm_$l.hhk
	   wxterm_$l.hhp
	   intro_$l.html
	   operation_$l.html
	   faq_$l.html
	   images/break.gif
	   images/configure.gif
	   images/connect.gif
	   images/disconnect.gif
	   images/filenew.gif
	   images/fileopen.gif
	   images/filesaveas.gif
	   images/quit.gif
	   images/refresh.gif
	   images/view_ascii.gif
	   images/view_hex.gif"
    echo $ZIPFILES
    zip wxterm_$l.zip $ZIPFILES
done
