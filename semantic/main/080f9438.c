typedef unsigned char u8;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

void Func_080fa2a0(void);
extern u8 Data_02003000;
extern u8 Data_02003004;
extern s16 Data_02003008;
extern s16 Data_0200300c;
extern s16 Data_02003010;
extern u8 Data_02003014;
extern s16 Data_02003020;
extern s16 Data_02003030;
extern s16 Data_02003034;
extern s16 Data_02003038;
extern u8 Data_0200303c;
extern u8 Data_02003040;
extern u32 Value_00000000;
extern u32 Value_00000004;
extern u32 Value_00000100;

void Func_080f9438(void)
{
    s16 *item;
    s32 count;

    Func_080fa2a0();
    Data_0200303c = 0xff;
    Data_02003000 = 0;
    Data_02003034 = (u32)&Value_00000100;
    Data_02003008 = (u32)&Value_00000100;
    Data_02003010 = (u32)&Value_00000004;
    Data_02003030 = (u32)&Value_00000100;
    Data_02003038 = (u32)&Value_00000100;
    Data_0200300c = (u32)&Value_00000004;
    Data_02003014 = 0;
    Data_02003040 = 0;
    item = &Data_02003020;
    Data_02003004 = 0;
    count = 7;
    do {
        count--;
        *item = (u32)&Value_00000000;
        item += 2;
    } while (count >= 0);
}
