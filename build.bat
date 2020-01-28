@echo off
set BLibPath= "W:\BLib"
set commonCompilerFlags=-W1 -arch:AVX2 -wd4556 -wd4805 -nologo -Gm- -GR- -EHsc- -Od -Oi -DSSE_WIDTH_8 -DPRINTINPUT=0 -DWIN32=1 -DDEBUG=1 -DPOOL_WIN32=1 -FC -Z7 -I %BLibPath% 
set commonLinkerFlags=-STACK:0xFFFFFF,0x100000 -PROFILE -NODEFAULTLIB:msvcrt.lib -incremental:no -opt:ref user32.lib shell32.lib gdi32.lib winmm.lib
del *.pdb > NUL 2> NUL
cl easy_build.cpp -MDd %commonCompilerFlags% -Fmeasy_build.map /link %commonLinkerFlags%  /PDB:easy_build_%random%.pdb
