typedef signed int s32; typedef unsigned int u32;

extern void Func_020098c8(s32);
extern void Func_0200965a(s32, s32, s32 *);
extern u32 Func_02009646(void);
extern u32 Func_0200965c(void);
extern void Func_02003e2e(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern void Func_02009674(s32);

void Func_02003c48(s32 x, s32 y, s32 z, s32 w)
{
    s32 desc[10];
    s32 tmp[3];
    u32 i;

    Func_020098c8(0xd8);
    i = 0;
    do {
        if ((i & 1) != 0) {
            desc[1] = 7;
            if ((i & 2) != 0)
                desc[1] = 5;
            desc[2] = 0x9999;
            desc[3] = 0x9999;
            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            Func_0200965a((6 - (i >> 1)) * 0x1999, w, tmp);
            {
                s32 a = x + ((6 - ((Func_02009646() * 6) >> 16)) << 16);
                s32 b = z + ((6 - ((Func_0200965c() * 6) >> 16)) << 16);

                Func_02003e2e(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
            }
        }
        Func_02009674(2);
        i++;
    } while (i <= 11);
}
