#include "types.h"

#define Ui_LoadEntryForKind Func_08021b80

s32 Func_0801a4fc(u32, s32, s32 *, s32 *, s32, s32);
s32 GameFlag_IsSet(s32);

s32 Ui_LoadEntryForKind(u32 kind, s32 value)
{
    s32 out;
    s32 cur;
    u32 no;

    no = kind;
    cur = value;
    if (no > 7U) {
        no = 0;
    }
    if (GameFlag_IsSet(0x20) != 0) {
        switch (no) {
        case 0:
            no = 0x38;
            break;
        case 1:
            no = 0x39;
            break;
        }
    }
    Func_0801a4fc(no, 0, &cur, &out, 0xE, 1);
    return out;
}
