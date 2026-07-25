typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000b0;
extern u8 Value_000000af;
extern u8 Value_000000ae;
extern u8 Data_0200b270[];
extern u8 Data_0200b330[];
extern u8 Data_0200b4f8[];
extern u8 Data_0200b558[];

s32 Func_02000f58(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000b0) {
        return (s32)Data_0200b270;
    }
    if (v == (s32)&Value_000000af) {
        return (s32)Data_0200b330;
    }
    if (v == (s32)&Value_000000ae) {
        return (s32)Data_0200b4f8;
    }
    return (s32)Data_0200b558;
}
