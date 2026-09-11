#include "types.h"
#include "scene.h"

extern u8 gRom[];

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = gRom;
    n = cnt - 1;
    if (n != -1) {
        do {
            n -= 1;
            *p = tbl[*p];
            p += 1;
        } while (n != -1);
    }
}
