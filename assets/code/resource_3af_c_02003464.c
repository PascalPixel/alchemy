typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_02007764(s32);

extern s32 Data_0200db08[];

s32 Func_02003464(u32 sel)
{
    s32 v = 0;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Func_02007764(v + i) != 0) return Data_0200db08[i];
    }
    return 0;
}
