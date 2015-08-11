#ifndef _DEBUG_H
#define _DEBUG_H

typedef struct {
    uint8_t flag[4];
    uint8_t buffer[64];
} debug_log_t;

debug_log_t log = { .flag = "DBG"};

#endif // _DEBUG_H
