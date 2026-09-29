#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001f2c[];
s16 Djinn_ListOwnerEntries(void *, s32, s32);

/* menu/core/compute_entry_values.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Menu_ComputeEntryValues(void *tbl)
{
    void *state;
    s32 i;
    void *p;
    u16 *src;
    s8 *dst;
    s16 v;
    s32 cnt;

    state = *(void **)((u32)&Data_03001f2c);
    i = 0;
    if (i < FIELD_AT_OFFSET(state, u8, 0x219)) {
        dst = (s8 *)tbl + 0xA0;
        src = (u16 *)((u8 *)state + 0x208);
        p = tbl;
        do {
            v = Djinn_ListOwnerEntries(p, *src, -1);
            cnt = FIELD_AT_OFFSET(state, u8, 0x219);
            i += 1;
            *dst = v;
            src += 1;
            dst += 1;
            p = (u8 *)p + 0x14;
        } while (i < cnt);
    }
}
