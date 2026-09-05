#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
void Func_080bb65c(void);
s32 Func_080bb938();
s32 Func_080bbb0c(void *, s32);
void Func_080c10e8(u16 *, s32);

s32 BattlePresentation_RunActorEntries(void *tbl) {
    s32 i;
    s8 n;

    Func_080c10e8(0, 0);
    n = FIELD_AT_OFFSET(tbl, s8 *, 1);
    if (n == 0) {
        Func_080bb65c();
    } else {
        i = 0;
        if (i < (s32) n) {
            do {
                Func_080bbb0c(tbl, i);
                Func_080bb938();
                i += 1;
            } while (i < (s32) FIELD_AT_OFFSET(tbl, s8 *, 1));
        }
    }
    return WaitFrames(1);
}
