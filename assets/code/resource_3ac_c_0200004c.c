typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Data_02008c98[];
extern u8 Data_02008a64[];
extern u8 Data_02008824[];
extern s32 Func_020004fe(s32);

u8 *Func_0200004c(void)
{
    s16 *table = Data_02000240;

    if (table[225] == 10) {
        return Data_02008c98;
    }
    if (Func_020004fe(0x941) != 0) {
        return Data_02008a64;
    }
    return Data_02008824;
}
