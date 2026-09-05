#include "types.h"

#define Runtime_RemapBytesByTable Func_0800f9cc

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern u8 Data_080097b8[];

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt) {
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = Data_080097b8;
    n = cnt - 1;
    if (n != -1) {
        do {
            n -= 1;
            *p = tbl[*p];
            p += 1;
        } while (n != -1);
    }
}
