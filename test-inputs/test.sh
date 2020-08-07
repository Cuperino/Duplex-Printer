#!/bin/bash
# gs -sOutputFile=- -q -sstdout=%stderr -dNOPAUSE -dBATCH -sDEVICE=ps2write -sColorConversionStrategy=Gray - < 0printerplusplus-capture.ps | psselect -q -p1-5 | psnup -q -d1 -nup 2 > out
gs -sOutputFile=- -q -sstdout=%stderr -dNOPAUSE -dBATCH -sDEVICE=ps2write -sColorConversionStrategy=Gray - < 0printerplusplus-capture.ps | psselect -q -p1-5  | psnup -q -d1 -n 2 | psselect -q -r -o | psblank 1 > out
