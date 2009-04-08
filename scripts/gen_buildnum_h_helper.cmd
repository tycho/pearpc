@echo off
set PATH=C:\cygwin\bin;%PATH%
bash.exe "%1/scripts/gen_buildnum_h.sh" "%1/src/build_number.h"
