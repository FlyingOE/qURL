/ @see curl.c
\d .curl

DLL:`:qURL

/ verbose flag
verbose:DLL 2:(`set_verbose;1)

/ HTTP GET
/q) .curl.cget
cget:DLL 2:(`cget;1)

/ HTTP POST
/q) .curl.cpost
cpost:DLL 2:(`cpost;2)

\
__EOD__
/=============================================================================