#include "types.h"

struct Entry_080b17e4 {
    s16 value;
    u8 kind;
};

void *Func_08077018(s32);
s32 Func_08077028(s32, s32);
s32 Func_08077228(s32, u8);
s32 Func_08077230(s32);
s32 Func_08077298(s16);
void Func_080b0574(s32 arg0);
void Func_080b10cc(void);
s32 Func_080b1868(s32, s32);
s32 Func_080b196c(s32 arg0, s32 arg1);
s32 Func_080f9010(s32);

void Func_080b17e4(s32 owner, s32 entry_id, s32 count) {
    s32 temp_r9;
    s32 var_r5;
    s32 var_sl;
    void *temp_r0;

    var_r5 = count;
    temp_r0 = Func_08077018(entry_id);
    var_sl = 0;
    temp_r9 = Func_08077228(
        owner, ((struct Entry_080b17e4 *)temp_r0)->kind);
    Func_080f9010(0x65);
    if (var_sl < var_r5) {
        do {
            var_sl = Func_08077028(owner, entry_id);
            Func_08077230(0 - ((struct Entry_080b17e4 *)temp_r0)->value);
            var_r5 -= 1;
            Func_08077298(((struct Entry_080b17e4 *)temp_r0)->value);
            Func_080b10cc();
        } while (var_r5 != 0);
    }
    Func_080b0574(0xCA1);
    if (Func_080b1868(owner, var_sl) != 0) {
        Func_080b196c(owner, temp_r9);
    }
}
