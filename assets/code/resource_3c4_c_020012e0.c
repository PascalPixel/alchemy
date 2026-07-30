typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];

extern void Func_02004402(u8 *);

u8 *Func_020012e0(void)
{
    u8 *table;

    if (Data_02000240[224] == (s32)&Value_000000ac) {
        table = Data_0200b8f4;
    } else {
        table = Data_0200ba74;
    }
    Func_02004402(table);
    return table;
}
