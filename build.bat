@echo off

set BLibPath= "W:\BLib"
set HashMapPath= "W:\flat_hash_map"

REM -wd4201 -wd4100 -wd4189 -wd4552 -wd4553
set commonCompilerFlags=  -W1 -arch:AVX2 -wd4556 -wd4805 -nologo -Gm- -GR- -EHsc- -O2 -Oi -DSSE_WIDTH_8 -DPRINTINPUT=0 -DWIN32=1 -DDEBUG=1 -DPOOL_WIN32=1 -FC -Z7 -I %BLibPath% -I %HashMapPath%
set commonLinkerFlags= -STACK:0xFFFFFF,0x100000 -PROFILE -NODEFAULTLIB:msvcrt.lib -incremental:no -opt:ref user32.lib shell32.lib gdi32.lib winmm.lib

del *.pdb > NUL 2> NUL

rem win32
cl easy_build.cpp -MDd %commonCompilerFlags% -Fmeasy_build.map /link %commonLinkerFlags%  /PDB:easy_build_%random%.pdb
rem cuda
rem nvcc -o "W:\AgentBasedFramework\NearestSearch.dll" --shared "W:\AgentBasedFramework\Cuda\NearestSearch.cu" -I "W:\Blib" -I "W:\AgentBasedFramework\Code" -O0 -g
