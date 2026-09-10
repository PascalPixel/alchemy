#include "types.h"

extern volatile s32 Data_03001b04;
extern volatile s32 Data_03001c94;
extern u8 Data_00000182[];
extern u8 Data_00000ad4[];
extern u8 Data_00000b2c[];

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32 frames);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08077018(s32);
s32 Func_080770c0(s32);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
void Func_080f9010(s32 cue);

s32 Func_080a524c(s32 a0)
{
    volatile s32 *pad;
    s32 win;
    s32 slot;
    s32 text;
    s32 label;
    s32 sel;
    s32 changed;

    win = Func_08015010(13, 3, 17, 10, 2);
    slot = a0 & 0x1ff;
    Func_08077018(slot);
    Func_08015080(slot + (s32)Data_00000182, win, 24, 0);
    text = (s32)Data_00000ad4;
    Func_08015080(text, win, 0, 16);
    text++;
    Func_08015080(text, win, 0, 24);
    label = (s32)Data_00000b2c;
    Func_08015080(label, win, 24, 40);
    label++;
    Func_08015080(label, win, 24, 56);
    sel = 1;
    changed = 1;
    Func_080a1ac0(104, 86);
    for (;;) {
        if (Func_080770c0(0x150) != 0) {
            break;
        }
        if (changed) {
            changed = 0;
            sel = Func_080022fc(sel + 2, 2);
        }
        if (Data_03001c94 & 1) {
            Func_080f9010(112);
            break;
        }
        if (Data_03001c94 & 2) {
            Func_080f9010(113);
            sel = 1;
            break;
        }
        Func_080a1a40(104, (sel << 4) + 70);
        pad = &Data_03001b04;
        if (*pad & 64) {
            sel -= 1;
            changed = 1;
            Func_080f9010(111);
        }
        if (*pad & 128) {
            sel += 1;
            changed = 1;
            Func_080f9010(111);
        }
        Func_080030f8(1);
    }
    if (Func_080770c0(0x150) != 0) {
        sel = 1;
    }
    Func_08015018(win, 1);
    return sel;
}
