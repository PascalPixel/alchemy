#include "TYPES.H"

extern u8 Runtime_ByteRemapTable[];

/* Replaces each of the CNT bytes at BUF with its entry in the remap table. */
void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = Runtime_ByteRemapTable;
    n = cnt - 1;
    if (n != -1) {
        do {
            u32 index = *p;

            n -= 1;
            /* FAKEMATCH: the plain order counts down after the table load. */
            __asm__("");
            *p = tbl[index];
            p += 1;
        } while (n != -1);
    }
}
