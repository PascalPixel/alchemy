#include "types.h"

extern u8 *Data_03001e8c;
extern volatile u32 Data_03001b04;

s32 Func_08019da8(s32, s32, s32, s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);
s32 Func_080022fc(s32);
void Func_08016478(s32);
s32 Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
void Func_0801ea08(s32, s32, s32, s32, s32);
void Func_0801e7c0(s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_08016418(s32, s32);

void Func_0802977c(void)
{
    u8 *base = Data_03001e8c;
    s32 flag = 1;
    s32 windowHandle;
    s32 obj;
    s32 count1 = 0, count2 = 0, total;
    s32 index = 0;
    s32 i;

    windowHandle = Func_08019da8(0, 0, 10, 5);
    obj = Func_080162d4(10, 10, 14, 3, 2);

    for (i = 0; *(s16 *)(0x080367e4 + i * 4) != -1; i++) {
        count1++;
    }
    for (i = 0; *(s16 *)(0x0803680c + i * 4) != -1; i++) {
        count2++;
    }
    total = count1 + count2;

    for (;;) {
        u32 keys = Data_03001b04;

        if (keys & 0x20) {
            flag = 1;
            index--;
        }
        if (keys & 0x10) {
            flag = 1;
            index++;
        }
        if (keys & 0x200) {
            flag = 1;
            index -= 10;
        }
        if (keys & 0x100) {
            flag = 1;
            index += 10;
        }

        if ((keys & 1) == 0 && (keys & 2) == 0) {
            if (flag) {
                s32 sp16, sp20;
                s32 y;

                flag = 0;
                index = Func_080022fc(index + total);

                Func_08016478(obj);

                if (index < count1) {
                    y = *(s16 *)(0x080367e4 + index * 4 + 2);
                } else {
                    y = *(s16 *)(0x0803680c + (index - count1) * 4 + 2) + 128;
                }

                sp20 = *(u16 *)(base + 0x12f2);
                Func_0801a4fc(0, index, &sp20, &sp16, 15, 1);
                Func_0801ea08(index, 2, obj, 0, 0);
                Func_0801e7c0(index + 0xdd2, obj, 24, 0);
            }

            Func_080030f8(1);
        } else {
            Func_08016418(obj, 2);
            Func_08016418(windowHandle, 2);
            Func_080030f8(1);
            break;
        }
    }
}
