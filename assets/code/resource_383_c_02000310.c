typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern s32 Func_02005042(s32);

u8 *Func_02000310(void)
{
    s16 *table = Data_02000240;
    s32 value = table[225];
    s32 low = 15;

    if (value <= 17) {
        if (value >= low) {
            return Data_0200e1fc;
        }
    }
    if (Func_02005042(0x855) != 0) {
        return Data_0200e250;
    }
    return Data_0200de30;
}
