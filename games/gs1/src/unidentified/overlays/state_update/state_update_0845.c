#include "types.h"

s32 *Func_02001d8a();
void Func_02001d40();
void Func_02001dc8();
void Func_02001d08();

void Func_02000c50(void)
{
    s32 selector = 11;
    s32 index = 0;

    do {
        s32 *record;

        Func_02001d40(Func_02001d8a(selector), 0);
        record = Func_02001d96(selector);
        record[17] = 0x1999;
        record[18] = 0;
        record[3] = 0x00ff0000;
        Func_02001dc8(index + 11, 1);
        index++;
        selector++;
    } while (index <= 3);

    {
        s32 rank = 0xc80;

        Func_02001d08(0x020089c1, rank);
    }
}
