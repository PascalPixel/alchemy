typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_02000942(u16 *);
extern void Func_0200017e(u16, s32);

void Func_020000c0(s32 arg)
{
    u16 list[16];
    s32 i;
    s32 n;

    n = Func_02000942(list);
    for (i = 0; i < n; i++) {
        Func_0200017e(list[i], arg);
    }
}
