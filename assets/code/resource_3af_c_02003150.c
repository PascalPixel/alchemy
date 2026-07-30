typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_0200744e(s32);

s32 Func_02003150(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
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
        if (Func_0200744e(v) != 0) return id;
        v++;
        id++;
    }
    return 0;
}
