typedef int s32;
typedef unsigned int u32;

extern s32 Func_0200238c(s32);
extern s32 Data_020098f8[];

s32 Func_0200115c(u32 a)
{
    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++) {
        if (Func_0200238c(base + i) != 0)
            return Data_020098f8[i];
    }
    return 0;
}
