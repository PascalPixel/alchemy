typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Value_000000b5;
extern u8 Value_000000b6;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Data_0200e8ec[];
extern u8 Data_0200e904[];
extern u8 Data_0200e9c4[];
extern u8 Data_0200eb74[];
extern u8 Data_0200ec04[];
extern u8 Data_0200ec64[];
extern u8 Data_0200ecf4[];

extern void Func_02005d78(u8 *);

u8 *Func_02000e88(void)
{
    u8 *p;
    s16 *table;
    s16 v;

    table = Data_02000240;
    v = table[224];
    if (v == (s32) (u32) &Value_000000b5) {
        return Data_0200e904;
    }
    if (v == (s32) (u32) &Value_000000b6) {
        p = Data_0200e9c4;
    } else if (v == (s32) (u32) &Value_000000b7) {
        p = Data_0200eb74;
    } else if (v == (s32) (u32) &Value_000000b8) {
        p = Data_0200ec04;
    } else if (v == (s32) (u32) &Value_000000b9) {
        p = Data_0200ec64;
    } else if (v == (s32) (u32) &Value_000000ba) {
        p = Data_0200ecf4;
    } else {
        goto no_match;
    }
    Func_02005d78(p);
    return p;

no_match:
    return Data_0200e8ec;
}
