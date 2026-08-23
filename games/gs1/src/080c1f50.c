#include "types.h"
#include "gs1_edition.h"

u8 *Runtime_GetObject(s32);

s32 Func_080c1f50(s32 arg0) {
    u8 *ptr;
    s32 i;
    s32 idx;
    s32 idx2;
    u8 val;
    u8 marker;

    i = 0;
    marker = LIST_MARKER_CHAR;
loop_i:
    ptr = Runtime_GetObject(i + 0x80);
    val = ptr[298];
    if (val != 1) goto next_i;
    if (ptr[296] != arg0) goto next_i;
    idx = 0;
    if (ptr[0] != 0) goto loop_idx;
    ptr[0] = marker;
    ptr[val] = idx;
    return;
loop_idx:
    idx++;
    if (idx > 13) return;
    val = ptr[idx];
    if (val != 0) goto loop_idx;
    idx2 = idx + 1;
    ptr[idx] = marker;
    ptr[idx2] = val;
    return;
next_i:
    i++;
    if (i <= 5) goto loop_i;
}
