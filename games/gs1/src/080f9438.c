#include "types.h"

void Func_080fa2a0(void);
extern u8 Data_02003000;
extern u8 Data_02003004;
extern u16 Data_02003008;
extern u16 Data_0200300c;
extern u16 Data_02003010;
extern u8 Data_02003014;
extern u16 Data_02003020;
extern u16 Data_02003030;
extern u16 Data_02003034;
extern u16 Data_02003038;
extern u8 Data_0200303c;
extern u8 Data_02003040;

void Func_080f9438(void)
{
    s16 *item;
    s32 count;

    Func_080fa2a0();
    Data_0200303c = 0xff;
    Data_02003000 = 0;
    Data_02003034 = 0x100;
    Data_02003008 = 0x100;
    Data_02003010 = 4;
    Data_02003030 = 0x100;
    Data_02003038 = 0x100;
    Data_0200300c = 4;
    Data_02003014 = 0;
    Data_02003040 = 0;
    item = &Data_02003020;
    Data_02003004 = 0;
    count = 7;
    do {
        count--;
        *item = 0;
        item += 1;
    } while (count >= 0);
}
