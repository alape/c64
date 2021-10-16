@echo off

set CC65_HOME=c:\cc65
set VICE_HOME=D:\Users\Algirdas\Desktop\GTK3VICE-3.5-win64\bin
set CC65_INC=%CC65_HOME%\include
set path=%PATH%;%CC65_HOME%\bin;%VICE_HOME%

cl65 -O -t c64 -o %1.prg %1