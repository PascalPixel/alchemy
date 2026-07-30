typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 *Func_02007692(s32);
extern void Func_02007778(s32, s32);
extern void Func_02007640(u8 *, s32);

void Func_02003380(s32 a)
{
    u8 *p = Func_02007692(a);

    if (p != 0) {
        Func_02007778(a, 3);
        Func_02007640(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}
