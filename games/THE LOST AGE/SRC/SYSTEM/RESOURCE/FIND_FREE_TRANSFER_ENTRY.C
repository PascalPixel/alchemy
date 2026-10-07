#include "TYPES.H"
#include "SCENE.H"
#include "SELECT.H"
#include "RAM_BUFFER.H"

s32 Resource_FindFreeTransferEntry(s32 kind)
{
    s32 state;
    s32 off;
    s32 i;
    s32 j;
    u32 ret;
    u16 *q;
    u16 *p;
    u32 v;

    state = (s32)Ram_HeapSlots->menu_work;
    if (kind != 0) {
        i = 0;
        p = &((struct SelectionScreen *)state)->records[9].kind;
        off = 0;
loop_2:
        if (*p == 0) {
            return state + off + 468;
        }
        i++;
        p += 26;
        off += 52;
        if (i == 5) {
            goto block_10;
        }
        goto loop_2;
    }
    j = 0;
    ret = state + 104;
    q = &((struct SelectionScreen *)state)->records[2].kind;
loop_7:
    v = *q;
    q += 26;
    if (v == 0) {
        return ret;
    }
    ret += 52;
    j += 1;
    if (j == 7) {
block_10:
        return 0;
    }
    goto loop_7;
}
