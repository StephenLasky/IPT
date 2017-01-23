//
//  IPT.h
//  IPT2
//
//  Created by Stephen Lasky on 1/17/17.
//  Copyright © 2017 Stephen Lasky. All rights reserved.
//

#ifndef IPT_h
#define IPT_h

#include <stdio.h>

/* takes and opens file and mallocs the contents into a buf */
/* returns size of buf on success, -1 on failure */
/* NOTE: buf must be freed after use */
long file_to_buf(char fname[], char **buf);

/* define data types */
struct pt_logfile_header {
    unsigned int magic;
    unsigned int version;
};
enum pt_logitem_kind {
    PT_LOGITEM_BUFFER,
    PT_LOGITEM_PROCESS,
    PT_LOGITEM_THREAD,
    PT_LOGITEM_IMAGE,
    PT_LOGITEM_XPAGE,
};
/* logitem header */
struct pt_logitem_header {
    enum pt_logitem_kind kind;
    unsigned int size;
};
/* type process */
struct pt_logitem_process {
    struct pt_logitem_header header;
    unsigned long tgid;                 // process id
    unsigned long cmd_size;             // the size of command string followed to launch this process
};
/* type thread */
struct pt_logitem_thread {
    struct pt_logitem_header header;
    unsigned long tgid;                 // process id
    unsigned long pid;                  // thread id
};
/* type xpage */
struct pt_logitem_xpage {
    struct pt_logitem_header header;
    unsigned long tgid; // process id
    unsigned long base; // the base address where executable pages were mapped
    unsigned long size; // the total size of the executable pages
};
/* type buffer */
struct pt_logitem_buffer {
    struct pt_logitem_header header;
    unsigned long tgid; // process id
    unsigned long pid; // thread id
    unsigned long sequence; // a per-thread sequence number
    unsigned long size; // the total size of PT packets followed
};


#endif /* IPT_h */

