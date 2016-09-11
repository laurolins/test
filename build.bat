set OPTIONS=-FC -Zi -EHsc
REM -Wall -WX -wd4214 -wd4820 -wd4127 -wd4255 -wd4201 -wd4668 -wd4710 -wd4711 -wd4706 -wd4996 -wd4189 -wd4100

set DEBUG=%OPTIONS% -DCHECK_ASSERTIONS -Od
set RELEASE=%OPTIONS% -O2

cl %DEBUG% test.c -Fe:test.exe -Fm:test.map /link -PDB:test.pdb
