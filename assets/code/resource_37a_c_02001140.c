typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;

extern s32 Func_02003aca(void);
extern void Func_02003c6e(s32);
extern void Func_02003b22(s32, s32, s32, s32, s32, s32);
extern void Func_02003b34(s32, s32, s32, s32, s32, s32);
extern void Func_02003bac(s32, s32, s32, s32, s32, s32);
extern void Func_02003b54(s32, s32, s32, s32, s32, s32);
extern void Func_02003b74(s32, s32, s32, s32, s32, s32);
extern void Func_02003b9a(s32, s32, s32, s32, s32, s32);
extern void Func_02003bc2(s32, s32, s32, s32, s32, s32);
extern s32 Func_02003bbe(void);
extern u16 Data_0200adec;
extern s32 Data_0200ade8;

void Func_02001140(void)
{
    if ((Func_02003aca() & 3) != 0) {
        u16 v = Data_0200adec;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003c6e(0xbb);
            a = 1;
            b = 5;
            Func_02003b22(0x2e, 0x3b, 41, 0x24, a, b);
            break;
        }
        case 1:
            Func_02003b34(0x2e, 0x3b, 41, 0x24, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Func_02003b54(0x2e, 0x3b, 41, 0x25, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Func_02003b74(0x2e, 0x3b, 41, 0x26, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003b9a(0x2e, 0x3b, 41, 0x27, v, v);
            Func_02003bac(0x2e, 0x3b, 41, 0x28, v, 5);
            break;
        case 0x55: {
            s32 a = 1;
            s32 b = 10;
            Func_02003bc2(0x2e, 0x31, 41, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200adec;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003bbe() * 40) >> 16) + 95) {
                *c = 0;
            }
        }
    }
}
