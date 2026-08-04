#include "types.h"

extern u8 Data_02000240[];

s32 Func_08077148(void);
u8 *Func_08077008(s32);
s32 Func_080022ec(s32, s32);
void Func_08077118(s32, s32);
void Func_08091220(void *, s32);
void Func_08091254(s32);
void Func_080f9010(s32);

s32 Func_0808c3a4(void)
{
    s32 result = 0;
    s32 count = Func_08077148();

    if (result < count) {
        s32 offset = 252;
        u8 *entry;
        s32 remaining;

        offset <<= 1;
        entry = Data_02000240 + offset;
        remaining = count;

        do {
            u8 *object = Func_08077008(*entry);
            s32 amount;

            switch ((s8)object[0x131]) {
            case 1:
                amount = -Func_080022ec(*(s16 *)(object + 0x34) + 10, 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -Func_080022ec(*(s16 *)(object + 0x34) + 5, 10);
                if (amount == 0)
                    amount = -1;
                if (result <= 1)
                    result = 2;
                break;
            default:
                amount = 0;
                break;
            }

            remaining--;
            Func_08077118(*entry, amount);
            entry++;
        } while (remaining != 0);
    }

    if (result != 0) {
        Func_08091220((void *)0x1ff, 0);
        Func_08091254(4);
        Func_080f9010(133);
    }

    return result;
}
