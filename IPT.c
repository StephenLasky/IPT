//
//  IPT.c
//  IPT2
//
//  Created by Stephen Lasky on 1/17/17.
//  Copyright © 2017 Stephen Lasky. All rights reserved.
//

#include "IPT.h"

/* takes and opens file and mallocs the contents into a buf */
/* returns size of buf on success, -1 on failure */
/* NOTE: buf must be freed after use */
long file_to_buf(char fname[], char **buf) {
    /* set up file information */
    FILE *fp;
    long int size;
    fp = fopen(fname, "r");
    if (!fp) {
        printf("file_to_buf: %s failed to open.\n", fname);
        return -1;
    }
    
    /* figure out the size/end of the file */
    fseek(fp, 0L, SEEK_END);
    size = (long int)ftell(fp);
    rewind(fp);
    
    /* malloc size for buffer */
    *buf = (char*)calloc(1, size+1);
    if (!buf) {
        printf("file_to_buf: malloc failed\n");
        fclose(fp);
        return -1;
    }
    
    /* copy file contents into buffer */
    if (1 != fread(*buf, size, 1, fp)) {
        printf("file_to_buf: read into file failed. result[%zu]\n", fread(buf,size,1,fp));
        printf("file_to_buf: attempted to use fname %s\n", fname);
        fclose(fp);
        return -1;
    }
    
    /* free relevant data structures */
    fclose(fp);
    
    /* return success */
    return size;
}


