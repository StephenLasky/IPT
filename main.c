//
//  main.c
//  IPT2
//
//  Created by Stephen Lasky on 1/17/17.
//  Copyright © 2017 Stephen Lasky. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IPT.h"

#define INPUT_FILE_NAME "aout.log"
#define MAGIC 0x51C0FFEE                // 4 bytes
#define VERSION 1

int main(int argc, const char * argv[]) {
    /* move raw trace into buf */
    char * buf;
    long buf_size = file_to_buf(INPUT_FILE_NAME, &buf);
    char * buf_o = buf;                                     // marks the position of the original buffere positions
    
    /* set up the header */
    struct pt_logfile_header header;
    memcpy((void *)&header, (void *)buf, sizeof(struct pt_logfile_header));
    if (header.magic != MAGIC)  // warn if magic numbers are not the same
        printf("main: WARNING: header.magic != MAGIC\n");
    buf += sizeof(struct pt_logfile_header);    // move the buffer
    
    /* begin parsing the raw trace from the buffer */
    struct pt_logitem_header logitem_header;    // temporary logitem header
    while (buf-buf_o < buf_size) {
        /* set up header for iteration */
        memcpy((void *)&logitem_header, (void *)buf, sizeof(struct pt_logitem_header));
        
//        printf("DEBUG:\tcurrent buf location: %lx\n", buf-buf_o);
//        printf("DEBUG:\tlogitem header kind: %d\n", logitem_header.kind);
//        printf("DEBUG:\thex at buf: %x\n", (unsigned)buf);
        
        /* detect PROCESS */
        if (logitem_header.kind == PT_LOGITEM_PROCESS) {
            struct pt_logitem_process * new_logitem_process = (struct pt_logitem_process * )malloc(sizeof(struct pt_logitem_process));
            memcpy(new_logitem_process, buf, sizeof(struct pt_logitem_process));
            buf += sizeof(struct pt_logitem_process);
            
            /* print info */
            printf("Process: tgid=%lu cmd_size=%lu", new_logitem_process->tgid, new_logitem_process->cmd_size);
            unsigned long i;
            printf(" cmd=");
            for (i=0; i<new_logitem_process->cmd_size; i++) {
                printf("%c", buf[i]);
            }
            printf("\n");
            buf += new_logitem_process->cmd_size;
        }
        /* detect THREAD */
        else if (logitem_header.kind == PT_LOGITEM_THREAD) {
            struct pt_logitem_thread * new_logitem_thread = (struct pt_logitem_thread *)malloc(sizeof(struct pt_logitem_thread));
            memcpy(new_logitem_thread, buf, sizeof(struct pt_logitem_thread));
            buf += sizeof(struct pt_logitem_thread);
            
            /* print info */
            printf("Thread: tgid=%lu pid=%lu\n", new_logitem_thread->tgid, new_logitem_thread->pid);
        }
        /* detect XPAGE */
        else if (logitem_header.kind == PT_LOGITEM_XPAGE) {
            int alloc_size = sizeof(struct pt_logitem_xpage);
            struct pt_logitem_xpage * new_logitem_xpage = (struct pt_logitem_xpage *)malloc(alloc_size);
            memcpy(new_logitem_xpage, buf, alloc_size);
            buf += alloc_size;
            
            /* add xpage */
            // TODO: NEED TO ADD XPAGE CODE HERE: pt_add_xpage
            
            /* print info */
            printf("xpage: tgid=%lu base=%lx size=%lx\n", new_logitem_xpage->tgid, new_logitem_xpage->base, new_logitem_xpage->size);
            buf += new_logitem_xpage->size;
        }
        /* detect BUFFER */
        else if (logitem_header.kind == PT_LOGITEM_BUFFER) {
            int alloc_size = sizeof(struct pt_logitem_buffer);
            struct pt_logitem_buffer * new_logitem_buffer = (struct pt_logitem_buffer *)malloc(alloc_size);
            memcpy(new_logitem_buffer, buf, alloc_size);
            buf += alloc_size;
            
            /* print info */
            printf("buffer: pid=%lu, size=%lu\n", new_logitem_buffer->pid, new_logitem_buffer->size);
            buf += new_logitem_buffer->size;
        }
        /* detect SOMETHING ELSE */
        else {
            printf("main: warning: format not recognized; exiting loop...\n");
            break;
        }
    }

	return 0;
}

