#include "types.h"

/* Scene state helper for overlay resource_3b1. */

s32 Func_0200c876();
void Func_0200c890();
s32 Func_0200c896();
void Func_0200c8b0();
void Func_0200c8b0_a();
void Func_0200c8b8();

/*
 * Runs two first-match linear scans over indices 0 to 8, each breaking on its
 * first hit and calling a per-element handler, then cross-pairs the miss
 * counts: the count from scanning `a` indexes into `b`, and the count from
 * scanning `b` indexes into `a`.  Each callee is named for its own call site,
 * because every call reaches its target through its own local veneer and two
 * of the sites share one veneer.
 */
void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (Func_0200c876(p)!= 0) {
            Func_0200c890(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Func_0200c896(p)!= 0) {
            Func_0200c8b0(p);
            break;
        }
        cnt_b++;
    }

    Func_0200c8b0_a(b + cnt_a);
    Func_0200c8b8(a + cnt_b);
}
