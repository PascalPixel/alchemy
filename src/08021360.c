typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern s16 Data_08037206[];
extern s16 Data_08037216[];
s32 Func_080770c0(s32);

s32 Func_08021360(u32 index)
{
    if (index > 8) {
        return 0;
    }
    if (Func_080770c0(32) == 0) {
        return Data_08037206[index];
    }
    return Data_08037216[index];
}
