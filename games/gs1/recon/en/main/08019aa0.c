#include "types.h"

extern u8 *Data_03001e8c;

s32 Func_08018038(s32);
void Func_080187ac(s32, s32 *, s32 *, s32 *, s32 *);
s32 Func_080162d4(s32, s32, s32, s32, s32);
s32 Func_080165d8(s32, s32, s32, s32, s32, s32);
void Func_08016418(s32, s32);
void Func_080030f8(s32);
s32 Func_08017364(void);
s32 Func_08017394(s32);

void Func_08019aa0(s32 p1, s32 flag, s32 baseY)
{
    u8 *base = Data_03001e8c;
    s32 callResult;
    s32 tableVal;

    callResult = Func_08018038(p1);
    tableVal = *(u16 *)(base + callResult * 2 + 3760);

    if (tableVal != 0) {
        s32 sp20 = 8, sp16 = 8, sp12, sp8;
        s32 x, y;
        s32 obj;

        Func_080187ac(p1, &sp20, &sp16, &sp12, &sp8);

        x = (30 - sp12) / 2;
        y = (15 - sp8) / 2 + baseY;

        if (flag != 0) {
            obj = Func_080162d4(x, y, sp12, sp8, 0);
        } else {
            obj = Func_080162d4(x, y, 0, 0, 2);
            *(s16 *)(obj + 8) = (s16)flag;
            *(s16 *)(obj + 10) = (s16)flag;
        }

        if (Func_080165d8(obj, callResult, 0, 0, 0, 0) == 0) {
            Func_08016418(obj, 1);
        } else {
            do {
                Func_080030f8(1);
            } while (Func_08017364() == 0);

            if (flag != 0) {
                Func_08016418(obj, 1);
            } else {
                Func_08016418(obj, 0);
                do {
                    Func_080030f8(1);
                } while (Func_08017394(obj) == 0);
            }

            *(u16 *)(base + 0x12f4) = 0;
            *(u16 *)(base + 0x12f6) = 0;
        }
    }
}
