// save pic
//
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <malloc.h>

long           columns;
unsigned char *image;

void SavePic(int x, int y, unsigned char *texels, char *filename)
   {
    unsigned short w, h, vo, ho, run;
    int            wide, high, fn, col = 0, row = 0, start = 0;
    long          *columns, isize = 0;
    unsigned char *image, *p;
    unsigned char  colbuff[128];

    wide = x;
    high = y;
    w = x;
    h = y;

    // allocate space for columns offsets
    columns = (long *)malloc(sizeof(long)*x);
    memset(columns, 0, sizeof(long)*x);

    // Worst case scenario is if every other texel
    // is 0 (blank). This means that every column
    // has y/2 runs of 1 texels. It also means that
    // we have a column offset and a length for each
    // of those runs.  Making it three bytes for each
    // texel painted or 50% larger.
    image = (unsigned char *)malloc(x*y*4);
    memset(image, 0, (x*y*4));

    p = image;
    columns[0] = 8;
    for (col = 0; col < x; col++)
       {
        run = 0;
        start = 0;
        columns[col] = ((sizeof(long)*x)+8)+(p - image);
        for (row = 0; row < y; row++)
           {
            if (texels[(row*x)+col] == 0)
               {
                if (run != 0)
                   {
                    *p = start;
                    p++;
                    *p = (unsigned char)run;
                    p += 2;
                    memcpy(p, colbuff, run);
                    p += (run + 1);
                   }
                run = 0;
                start = row + 1;
               }
            else
               {
                colbuff[run] = texels[(row*x)+col];
                run++;
               }
            vo = run;
           }
        if (run != 0)
           {
            *p = start;
            p++;
            *p = (unsigned char)run;
            p += 2;
            memcpy(p, colbuff, run);
            p += (run + 1);
           }
        *p = 0xff;
        p++;
       }

    // set the vertical and horizontal offsets to 0
    vo = ho = 0;

    // open the file to save the "lump" in
    fn = open(filename, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 0666);
    write(fn, &w, sizeof(short));
    write(fn, &h, sizeof(short));
    write(fn, &vo, sizeof(short));
    write(fn, &ho, sizeof(short));
    write(fn, columns, sizeof(long)*x);
    write(fn, image, ((p-image)+1));
    // close the output file
    close(fn);

    // free the output image
    free(image);
    // free the memory used to hold the column offsets
    free(columns);
   }