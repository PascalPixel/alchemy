#include "types.h"

extern const s32 Data_080fb794[3];

s32 Func_080022fc(s32 dividend, s32 divisor);
u32 Func_080037d4(u32 index);
void Func_080030f8(s32 frames);
void Func_080f9080(s32 request);

void Func_080f92fc(void)
{
    s32 sounds[3];
    s32 mode = 2;
    s32 selection = 0;
    s32 cooldown = 20;

    sounds[0] = Data_080fb794[0];
    sounds[1] = Data_080fb794[1];
    sounds[2] = Data_080fb794[2];
    *(volatile s32 *)0x03007804 = 0;

    for (;;) {
        if (cooldown != 0)
            cooldown--;

        if (*(volatile s32 *)0x03007804 != 0) {
            *(volatile s32 *)0x03007804 = 0;
            cooldown = 20;
        }

        if ((*(volatile u32 *)0x03000b5c & 4) != 0) {
            mode = Func_080022fc(mode + 1, 5);
            Func_080037d4(mode);
        }

        if ((*(volatile u32 *)0x03000b5c & 0x100) != 0)
            sounds[selection] += 10;

        if ((*(volatile u32 *)0x03000b5c & 0x200) != 0)
            sounds[selection] -= 10;

        if ((*(volatile u32 *)0x03000b5c & 0x10) != 0)
            sounds[selection]++;

        if ((*(volatile u32 *)0x03000b5c & 0x20) != 0)
            sounds[selection]--;

        if ((*(volatile u32 *)0x03000b5c & 0x40) != 0 &&
            selection > 0) {
            selection--;
        }

        if ((*(volatile u32 *)0x03001b04 & 0x80) != 0 &&
            selection <= 1) {
            selection++;
        }

        if ((*(volatile u32 *)0x03001b04 & 1) != 0)
            Func_080f9080(sounds[selection]);

        if ((*(volatile u32 *)0x03001b04 & 2) != 0)
            Func_080f9080(19);

        if ((*(volatile u32 *)0x03001b04 & 8) != 0)
            Func_080f9080(17);

        if ((*(volatile u32 *)0x03001b04 & 4) != 0)
            Func_080f9080(0x121);

        Func_080030f8(1);
    }
}
