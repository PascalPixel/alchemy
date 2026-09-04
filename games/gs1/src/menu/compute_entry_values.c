#include "types.h"
#include "global_cells.h"

#define Menu_ComputeEntryValues Func_080aaf58

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s16 Func_080ac8fc(void *, s32, s32);

s32 Menu_ComputeEntryValues(void *tbl) {
    void *state;
    s32 i;
    void *p;
    u16 *src;
    s8 *dst;
    s16 v;
    s32 cnt;

    state = *(void **)ADDR_03001F2C;
    i = 0;
    if (i < FIELD_AT_OFFSET(state, u8, 0x219)) {
        dst = (s8 *)tbl + 0xA0;
        src = (u16 *)((u8 *)state + 0x208);
        p = tbl;
        do {
            v = Func_080ac8fc(p, *src, -1);
            cnt = FIELD_AT_OFFSET(state, u8, 0x219);
            i += 1;
            *dst = v;
            src += 1;
            dst += 1;
            p = (u8 *)p + 0x14;
        } while (i < cnt);
    }
}
