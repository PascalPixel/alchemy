typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Value_00000013;
extern u8 Value_00000010;

extern void Func_02001236(void);
extern void Func_0200133a(void);

s32 Func_020008f4(void)
{
    s32 scenario = Data_02000240[224];

    if (scenario == (s32)&Value_00000013) {
        Func_02001236();
    } else if (scenario == (s32)&Value_00000010) {
        Func_0200133a();
    }
    return 0;
}
