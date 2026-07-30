typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 Data_02000240[];
extern u8 Data_0200ee44[];
extern u8 Data_0200f120[];
extern u8 Data_0200f300[];
extern u8 Data_0200f3b4[];
extern u8 Data_0200f4f8[];
extern u8 Data_0200ef1c[];
extern u8 Value_000000b5;
extern u8 Value_000000b6;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;

u8 *Func_02002f8c(void)
{
    s32 off = 0x1c0;
    s32 v = *(s16 *)(Data_02000240 + off);

    if (v == (s32)&Value_000000b5) {
        return Data_0200ee44;
    }
    if (v == (s32)&Value_000000b6) {
        return Data_0200ef1c;
    }
    if (v == (s32)&Value_000000b7) {
        return Data_0200f120;
    }
    if (v == (s32)&Value_000000b8) {
        return Data_0200f300;
    }
    if (v == (s32)&Value_000000b9) {
        return Data_0200f3b4;
    }
    if (v == (s32)&Value_000000ba) {
        return Data_0200f4f8;
    }
    return Data_0200ef1c;
}
