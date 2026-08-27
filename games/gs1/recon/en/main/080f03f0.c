#include "types.h"

void Func_080040e8(void);
void Func_080041d8(s32 target, s32 duration);
void Func_080f037c(s32 addr);
void Func_080f0254(s32 arg);
void Func_080f0678(void);
void Func_080030f8(s32 frames);
u32 Func_080f02b0(u32 value, s32 flag);
void Func_0800479c(void);
void Func_08004760(void);

s32 Func_080f03f0(void)
{
    s32 outer;
    s32 inner;
    s32 base;
    u32 *table = (u32 *)0x080f0a5c;

    *(u8 *)0x03001d18 = 0;
    *(u8 *)0x03001f58 = 0;
    *(u8 *)0x03001ac4 = 0;
    *(u8 *)0x03001d08 = 0;

    Func_080040e8();
    Func_080041d8(0x080f03c1, 1152);

    *(u16 *)0x04000000 = 64;
    Func_080f037c(0x06007800);
    Func_080f037c(0x0600f800);
    Func_080f0254(0);
    Func_080f0254(1);

    *(u16 *)0x0400000c = 0x1f8a;
    *(u16 *)0x0400000e = 0x1f8a;
    *(u16 *)0x04000000 = 0x1c40;
    *(u16 *)0x00002844 = 0x0f83;
    Func_080f0678();
    Func_080030f8(300);

    for (outer = 0; outer <= 32; outer++) {
        s32 toggle = outer & 1;
        s32 flag = toggle ^ 1;

        Func_080f02b0(table[outer], flag);

        base = 0xf00;
        for (inner = 0; inner <= 16; inner++) {
            if (toggle != 0) {
                *(u16 *)0x04000050 = (inner << 8) | (16 - inner);
            } else {
                *(u16 *)0x04000050 = base | inner;
            }
            Func_080030f8(4);
            base -= 256;
        }
        Func_080030f8(0x0000010b);
    }

    *(u16 *)0x04000050 = 0;
    *(u16 *)0x04000000 = 0x1040;
    Func_0800479c();
    Func_08004760();
    *(u8 *)0x03001d18 = 1;

    return 0;
}
