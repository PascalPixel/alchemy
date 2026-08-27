#include "types.h"
#include "global_cells.h"

extern void *Data_03001f2c;

s32 Func_08077290(s32);
void Func_080a10d0(s32 *, s32, s32, s32, s32, s32);
void Func_08004278(void (*)(void));
void Func_080a19a0(void);
void Func_080a1ac0(s32, s32);
void *Func_08077008(s32);
void Func_080a8604(s32, s32, s32);
s8 Func_080a8b10(u8 *, s32, s32);
s32 Modulo(s32, s32);
s32 Func_080770c0(s32);
void Func_08015270(s32);
void Func_08015278(s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_080041d8(const void *, s32);
void Func_080a1804(void *, s32);
void Func_080a9d84(void);
void Func_080a847c(s32, s32, u8 *, s32);
void Func_080a8508(s32, s32, u8 *);
void Func_080a8578(s32, s32, s32);
void Func_080a1a40(s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);

s32 Func_080a8114(void)
{
    void *menu;
    s32 result;
    s32 step;
    s32 hasAlt;
    s32 done;
    s32 roundMode;
    u8 *ownerIdPtr;
    u8 entriesBuf[8];
    s32 sl;
    s32 quantity;
    s32 changed;
    s8 flagByte;
    s32 rawFlag;

    menu = Data_03001f2c;
    sl = 0;
    result = 0;
    quantity = 0;
    rawFlag = Func_08077290(-1);
    roundMode = (u32)(-rawFlag | rawFlag) >> 31;

    Func_080a10d0((s32 *)((u8 *)menu + 44), 0, 0, 30, 5, 2);
    Func_08004278(Func_080a19a0);

    {
        s16 *reset = (s16 *)((u8 *)menu + 0x242);
        s32 i = 3;
        do {
            i -= 1;
            *reset = 0x68;
            reset -= 1;
        } while (i >= 0);
    }

    done = 0;
    Func_080a1ac0(-10, 88);
    ownerIdPtr = (u8 *)menu + 0x21a;

    for (;;) {
        if (done != 0)
            break;
        if (Func_080770c0(336) != 0)
            break;

        Func_08077008(*ownerIdPtr);
        Func_080a8604(*(s32 *)((u8 *)menu + 36), *ownerIdPtr, 1);
        flagByte = Func_080a8b10(entriesBuf, 1, *ownerIdPtr);
        hasAlt = 0;
        step = flagByte;
        if (flagByte != 0) {
            hasAlt = 1;
        } else {
            step = 1;
        }
        changed = 1;

        for (;;) {
            if (Func_080770c0(336) != 0)
                break;

            if (changed != 0) {
                changed = 0;
                sl = (sl + 2) % 2;
                if (sl != 0) {
                    Func_08015270(*(s32 *)((u8 *)menu + 44));
                    if (roundMode != 0) {
                        quantity = (quantity + 8) % 8;
                    } else {
                        quantity = Modulo(quantity + 7, 7);
                    }
                } else {
                    quantity = Modulo(quantity + step, step);
                    Func_08015270(*(s32 *)((u8 *)menu + 44));
                    if (hasAlt == 0) {
                        Func_08015080(0xb06, *(s32 *)((u8 *)menu + 36), 0x50,
                            -0x18);
                        Func_08015080(0xb07, *(s32 *)((u8 *)menu + 36), 0,
                            -0x18);
                    }
                }
                Func_080a847c(sl, quantity, entriesBuf, 0);
                Func_08015278(*(s32 *)((u8 *)menu + 44));
                Func_080030f8(1);
                if (sl == 0) {
                    Func_080a8508(
                        *(s32 *)((u8 *)menu + 44), quantity, entriesBuf);
                } else {
                    Func_080a8578(
                        *(s32 *)((u8 *)menu + 44), quantity, roundMode);
                }
            }

            *((u8 *)(*(void **)((u8 *)menu + 20)) + 5) = 1;
            if (sl == 0) {
                Func_080a1a40(-10, quantity * 16 + 88);
            } else if (quantity <= 3) {
                Func_080a1a40(24, quantity * 8 + 48);
            } else {
                Func_080a1a40(48, quantity * 8 + 80);
            }
            Func_080030f8(1);
            if (*(volatile u32 *)ADDR_03001B04 & 0xf0) {
                Func_080a847c(sl, quantity, entriesBuf, 1);
            }

            if (*(volatile u32 *)ADDR_03001C94 & 1) {
                Func_080f9010(112);
                done = 1;
                result = 1;
            } else if (*(volatile u32 *)ADDR_03001C94 & 2) {
                Func_080f9010(113);
                done = 1;
                result = -1;
            } else {
                if (*(volatile u32 *)ADDR_03001B04 & 0x40) {
                    Func_080f9010(111);
                    changed = 1;
                    quantity -= 1;
                }
                if (*(volatile u32 *)ADDR_03001B04 & 0x80) {
                    Func_080f9010(111);
                    changed = 1;
                    quantity += 1;
                }
                if (*(volatile u32 *)ADDR_03001B04 & 0x10) {
                    Func_080f9010(111);
                    changed = 1;
                    sl += 1;
                }
                if (*(volatile u32 *)ADDR_03001B04 & 0x20) {
                    Func_080f9010(111);
                    changed = 1;
                    sl -= 1;
                }
                if (*(volatile u32 *)ADDR_03001B04 & 0x100 ||
                    *(volatile u32 *)ADDR_03001B04 & 0x200) {
                    s32 page;
                    s32 newPage;
                    u8 count;
                    u16 tableVal;

                    Func_080f9010(111);
                    page = *((s8 *)menu + 28);
                    if (*(volatile u32 *)ADDR_03001B04 & 0x100) {
                        newPage = page + 1;
                    } else {
                        newPage = page - 1;
                    }
                    count = *((u8 *)menu + 0x219);
                    newPage = Modulo(newPage + count, count);
                    tableVal =
                        *(u16 *)((u8 *)menu + newPage * 2 + 0x208);
                    *(s32 *)((u8 *)menu + 8) = tableVal;
                    *ownerIdPtr = (u8)tableVal;
                    *((s8 *)menu + 28) = (s8)newPage;
                    Func_080a1804(menu, tableVal);
                } else {
                    continue;
                }
            }
            break;
        }
    }

    Func_08015278(*(s32 *)((u8 *)menu + 44));
    Func_08015270(*(s32 *)((u8 *)menu + 44));
    Func_08015068(*(s32 *)((u8 *)menu + 36), 0x40, 0x38, 0xe0, 0x60);
    Func_080041d8((const void *)Func_080a19a0, 0xc80);

    {
        s16 *reset = (s16 *)((u8 *)menu + 0x242);
        s32 i = 3;
        do {
            i -= 1;
            *reset = 0x80;
            reset -= 1;
        } while (i >= 0);
    }

    Func_080a9d84();
    return result;
}
