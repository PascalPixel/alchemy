/* 2026-09-24: hand-written, 18 differing halfwords, register swap only:
   the reference keeps the key-repeat address in r6 and the mode pointer in
   r0; here they are the other way round. */
#include "TYPES.H"

extern volatile u32 Data_03001b04;
void Func_08028ef0(s32 arg, s32 value, s16 *sub);

s32 Func_08029094(s32 arg, s16 value, s16 *sub, s16 *mode)
{
    if (Data_03001b04 & 1)
        return -1;
    if (Data_03001b04 & 2)
        return -2;
    if ((Data_03001b04 & 0x80) || (Data_03001b04 & 0x40)) {
        *mode ^= 1;
    } else if (Data_03001b04 & 0x10) {
        if (*mode == 0) {
            value++;
        } else {
            (*sub)++;
            if (*sub > 99)
                *sub = 0;
        }
        if (value > 200)
            value = 0;
        Func_08028ef0(arg, value, sub);
    } else if (Data_03001b04 & 0x20) {
        if (*mode == 0) {
            value--;
        } else {
            (*sub)--;
            if (*sub < 0)
                *sub = 99;
        }
        if (value < 0)
            value = 200;
        Func_08028ef0(arg, value, sub);
    } else if (Data_03001b04 & 0x100) {
        if (*mode == 0) {
            *sub = 0;
            value += 10;
        } else {
            *sub += 10;
            if (*sub > 99)
                *sub -= 99;
        }
        if (value > 200)
            value = 0;
        Func_08028ef0(arg, value, sub);
    } else if (Data_03001b04 & 0x200) {
        if (*mode == 0) {
            *sub = 0;
            value -= 10;
        } else {
            *sub -= 10;
            if (*sub < 0)
                *sub += 99;
        }
        if (value < 0)
            value = 200;
        Func_08028ef0(arg, value, sub);
    }
    return value;
}
