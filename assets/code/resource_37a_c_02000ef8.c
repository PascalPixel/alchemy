typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;

extern s32 Func_02003882(void);
extern void Func_02003a26(s32);
extern void Func_020038da(s32, s32, s32, s32, s32, s32);
extern void Func_020038ec(s32, s32, s32, s32, s32, s32);
extern void Func_02003964(s32, s32, s32, s32, s32, s32);
extern void Func_0200390c(s32, s32, s32, s32, s32, s32);
extern void Func_0200392c(s32, s32, s32, s32, s32, s32);
extern void Func_02003952(s32, s32, s32, s32, s32, s32);
extern void Func_0200397a(s32, s32, s32, s32, s32, s32);
extern s32 Func_02003976(void);
extern u16 Data_0200addc;
extern s32 Data_0200ade8;

void Func_02000ef8(void)
{
    if ((Func_02003882() & 3) != 0) {
        u16 v = Data_0200addc;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003a26(0xbb);
            a = 1;
            b = 5;
            Func_020038da(0x2f, 0x3b, 42, 0x21, a, b);
            break;
        }
        case 1:
            Func_020038ec(0x2f, 0x3b, 42, 0x21, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x22, v, 5);
            break;
        case 2:
            v = 1;
            Func_0200390c(0x2f, 0x3b, 42, 0x22, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x23, v, 5);
            break;
        case 3:
            v = 1;
            Func_0200392c(0x2f, 0x3b, 42, 0x23, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x24, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003952(0x2f, 0x3b, 42, 0x24, v, v);
            Func_02003964(0x2f, 0x3b, 42, 0x25, v, 5);
            break;
        case 0x5a: {
            s32 a = 1;
            s32 b = 10;
            Func_0200397a(0x2f, 0x31, 42, 0x21, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200addc;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003976() * 40) >> 16) + 100) {
                *c = 0;
            }
        }
    }
}
