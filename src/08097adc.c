typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

extern void Func_08004278(s32);
extern s32 Func_08015360(s32, s32);
extern u8 Data_02000240[];

void Func_08097adc(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001E8C;
    Func_08004278(0x08097869);
    *(volatile s16 *)0x050001E2 = 0x7FFF;
    *(s16 *)0x050001E6 = 0;
    *(volatile s16 *)0x050001F6 = 0x294A;
    *(volatile s16 *)0x050001F8 = 0x5294;
    Func_08015360(Data_02000240[0x205], Data_02000240[0x206]);
    *((u8 *)temp_r5 + 0xEA4) = 0;
}
