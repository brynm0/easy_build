Dead simple & hacky build system. Returns 0 if you should compile a translation unit, 1 if not.

Windows online

Dependent on BLib https://github.com/brynm0/BLib

Compiled on my machine using the following command
  cl easy_build.cpp -MDd -W1 -arch:AVX2 -wd4556 -wd4805 -nologo -Gm- -GR- -EHsc- -O2 -Oi -FC -Z7 -I "W:\BLib" -Fmeasy_build.map /link -STACK:0xFFFFFF,0x100000 -PROFILE -NODEFAULTLIB:msvcrt.lib -incremental:no -opt:ref user32.lib shell32.lib gdi32.lib winmm.lib  /PDB:easy_build_%random%.pdb
