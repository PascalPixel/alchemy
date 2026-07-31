typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02009a9c[];
extern u8 Data_020097a8[];
extern u8 Data_020094a8[];

extern s32 Func_020011fc(s32);
extern s32 Func_0200120a(s32);

u8 *Func_020004b0(void)
{
    if (Func_020011fc(0x950) != 0) {
        return Data_02009a9c;
    }
    if (Func_0200120a(0x962) != 0) {
        return Data_020097a8;
    }
    return Data_020094a8;
}
