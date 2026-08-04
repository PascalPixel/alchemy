#include "types.h"

s32 *Func_02001d42();
void Func_02001cf8();
void Func_02001cb6();

void Func_02000c08(void)
{
    s32 selector = 10;
    s32 remaining = 5;

    do {
        s32 *record;

        Func_02001cf8(Func_02001d42(selector), 0);
        record = Func_02001d4e(selector);
        record[17] = 0x1999;
        record[18] = 0;
        remaining--;
        record[3] = 0x00ff0000;
        selector++;
    } while (remaining >= 0);

    {
        s32 rank = 0xc80;

        Func_02001cb6(0x02008aa1, rank);
    }
}
