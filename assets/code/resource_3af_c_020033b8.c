typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_02006514(s32);
extern s32 Func_02006832(s32);
extern s32 Func_02006532(s32);
extern s32 Func_02006850(s32);
extern s32 Func_0200654e(s32);
extern s32 Func_0200686c(s32);
extern s32 Func_02006572(s32);
extern s32 Func_02006890(s32);
extern s32 Func_02006588(s32);
extern s32 Func_020068a6(s32);

extern s32 Data_0200db70[];

void Func_020033b8(void)
{
    {
        s32 *p = Data_0200db70;
        u32 i = 0;
        s32 t = 3;

        for (i = 0; i < 4; i++) {
            if (Func_02006514(i) != 0) p[i] = Func_02006832(i);
            else p[i] = t;
        }
    }
    if (Func_02006532(0) != 0) Data_0200db70[0] = Func_02006850(0);
    else Data_0200db70[0] = 3;
    if (Func_0200654e(2) != 0) Data_0200db70[1] = Func_0200686c(2);
    else Data_0200db70[1] = 3;
    {
        s32 *q = Data_0200db70;
        s32 t = 3;

        q[2] = t;
        q[3] = t;
        if (Func_02006572(1) != 0) q[4] = Func_02006890(1);
        else q[4] = t;
    }
    if (Func_02006588(3) != 0) Data_0200db70[5] = Func_020068a6(3);
    else Data_0200db70[5] = 3;
    Data_0200db70[6] = 3;
    Data_0200db70[7] = 3;
}
