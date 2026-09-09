#include "types.h"

/*
 * resource_3bb: look up an object by arg0, then scan the first 15 halfword
 * entries of its table at offset 0xd8 for one equal to arg1, calling a handler
 * with each matching index.  The owner at 0x02002380 is 60 bytes and carries
 * no literal pool.  The three callees are identified by call shape only, and
 * the table's role is inferred from this scan alone.
 */

extern u16 *Func_0200625c(s32 no);
extern void Func_0200626e(s32 no, s32 val);
extern void Func_0200628a(s32 no, s32 index);

void OverlayObject_NotifyMatchingEntries(s32 no, s32 val)
{
    u16 *tbl = Func_0200625c(no);
    s32 i;

    Func_0200626e(no, val);

    tbl = (u16 *)((char *)tbl + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (tbl[i] == val) {
            Func_0200628a(no, i);
        }
    }
}
