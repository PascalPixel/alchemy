#include "types.h"

void Func_08016478(void);
void Func_0801e41c(s32, s32, s32, s32, s32);
void Func_0801e8b0(void *, s32, s32, s32);
s32 Func_0801e940(s32, s32, s32, s32);
void Func_0801e9d4(s32, s32, s32, s32, s32);
void Func_0801e7c0(s32, s32, s32, s32);
s32 Func_0801f680(s32, void *);
void Func_0801ea08(s32, s32, s32, s32, s32);

void Func_08020198(s32 surface, u8 *st)
{
    u32 buf[4];
    s32 extra;

    if (surface != 0) {
        Func_08016478();
        Func_0801e41c(surface, 0, 4, 13, 4);
        Func_0801e8b0(st + 16, surface, 0, 0);
        extra = 0;
        Func_0801e940(0x080371e0, surface, 72, 0);
        Func_0801e9d4(st[28], 2, surface, 80, extra);
        Func_0801e7c0(st[29] + 0x741, surface, 0, 16);
        Func_0801e7c0(9, surface, 0, 32);
        Func_0801e940(Func_0801f680(*(s32 *)(st + 32), buf), surface, 48, 40);
        extra = 48;
        Func_0801ea08(*(s32 *)(st + 36), 6, surface, 0, extra);
        Func_0801e7c0(0xc88, surface, 48, 48);
    }
}
