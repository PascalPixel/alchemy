typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_020099d0[];
extern u8 Data_02009670[];
extern u8 Data_02009310[];

extern s32 Func_02000a34(s32);
extern s32 Func_02000a42(s32);

u8 *Func_02000064(void)
{
    if (Func_02000a34(0x950) != 0) {
        return Data_020099d0;
    }
    if (Func_02000a42(0x962) != 0) {
        return Data_02009670;
    }
    return Data_02009310;
}
