#include "types.h"

void Func_080072f0(s32 source, s32 destination, s32 size, s32 work);
void Func_08005534(s32 source, s32 destination, s32 size);
void Func_080054e4(void *source, void *destination, u32 size);
void Func_08005490(s32 source, s32 mode, s32 destination, s32 size);
void Func_0800543c(s32 source, s32 mode, s32 destination, s32 size);

void Func_080cd260(void)
{
    u8 *state = *(u8 **)0x03001eec;
    s32 work = *(s32 *)0x03001ef0;
    s32 mode;

    if (*(s32 *)(state + 0x7824) != 1) {
        (*(s32 *)(state + 0x7820))++;
        return;
    }

    mode = *(s32 *)(state + 0x7780);
    switch (mode) {
    case 0:
        Func_080072f0(0x06004000, work, 0x4000, 0x03001388);
        break;
    case 1:
        Func_080072f0(0x06004000, work, 0x4000, 0x03001388);
        Func_080072f0(
            work, 0x4000, *(s32 *)(state + 0x7784), 0x03000168);
        break;
    case 2:
        if (*(s32 *)(state + 0x7784) == 0x32)
            Func_08005534(work, 0x06004000, 0x4000);
        else
            Func_080054e4((void *)work, (void *)0x06004000, 0x4000);
        break;
    case 3:
        Func_08005490(
            work, *(s32 *)(state + 0x7784), 0x06004000, 0x4000);
        break;
    case 4:
        Func_0800543c(
            work, *(s32 *)(state + 0x7784), 0x06004000, 0x4000);
        break;
    }

    *(s32 *)(state + 0x7824) = 0;
    *(s32 *)(state + 0x7820) = 1;
}
