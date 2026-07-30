typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Value_00000024;
extern u8 Value_00000027;
extern u8 Data_0200b098[];
extern u8 Data_0200b368[];
extern u8 Data_0200b080[];

s32 Func_02002c10(s32);
void Func_0200029e(s32);

s32 Func_020001dc(void) {
    s32 v = Data_02000240[224];
    if (v == (s32)&Value_00000024) {
        if (Func_02002c10(0x845) == 0) {
            Func_0200029e((s32)Data_0200b098);
        }
        return (s32)Data_0200b098;
    }
    if (v == (s32)&Value_00000027) {
        return (s32)Data_0200b368;
    }
    return (s32)Data_0200b080;
}
