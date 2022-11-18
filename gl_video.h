// gl_video.h
// Data type and function declarations for gl_video.c

#include <windows.h>
#include "doomtype.h"

#define GL_MAX_EXT     256                    // How many extensions will we allow?

typedef enum { gl_2d, gl_3d } glmode_t;
dboolean StartUpOpenGL( HWND hWnd );
void OGL_Error( int, char * );
void GetGLInfo( HWND );

