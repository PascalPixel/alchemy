#include "types.h"

extern void *Data_03001ebc;
extern u8 Data_02000240[];

void Func_080f9010(s32);
void Func_080901c0(s32, s32);
void Func_080030f8(s32);

void Func_080941e0(void)
{
    void *state = Data_03001ebc;
    s32 mode;
    s32 i;

    Func_080f9010(((s16 *)Data_02000240)[247]);
    Func_080f9010(288);
    Func_080f9010(147);

    mode = *(s16 *)(state + 414);
    if (mode == 3) {
        *(u16 *)0x050001e6 = 0x7fff;
        Func_080901c0(0x050001e6, 16);
        *(u16 *)(state + 454) = 0;
        Func_080030f8(16);

        {
            s32 a = 0x7800;
            s32 b = 960;
            s32 c = 30;
            for (i = 0; i < 16; i++) {
                *(u16 *)0x04000008 = a | b | c;
                Func_080030f8(1);
                a += 0x401;
                b -= 64;
                c -= 2;
            }
        }
    } else {
        *(u16 *)0x05000000 = 0x7fff;
        Func_080901c0(0x05000000, 16);
        *(u16 *)(state + 454) = 0;
        Func_080030f8(16);

        {
            s32 a = 0x7800;
            s32 b = 960;
            s32 c = 30;
            for (i = 15; i >= 0; i--) {
                *(u16 *)0x0400000a = a | b | c;
                Func_080030f8(1);
                a += 0x401;
                b -= 64;
                c -= 2;
            }
        }
    }
}
