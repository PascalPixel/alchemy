typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;

extern s32 Func_020039a6(void);
extern void Func_02003b4a(s32);
extern void Func_020039fe(s32, s32, s32, s32, s32, s32);
extern void Func_02003a10(s32, s32, s32, s32, s32, s32);
extern void Func_02003a88(s32, s32, s32, s32, s32, s32);
extern void Func_02003a30(s32, s32, s32, s32, s32, s32);
extern void Func_02003a50(s32, s32, s32, s32, s32, s32);
extern void Func_02003a76(s32, s32, s32, s32, s32, s32);
extern void Func_02003a9e(s32, s32, s32, s32, s32, s32);
extern s32 Func_02003a9a(void);
extern u16 Data_0200ade0;
extern s32 Data_0200ade8;

void Func_0200101c(void)
{
    if ((Func_020039a6() & 3) != 0) {
        u16 v = Data_0200ade0;
        switch (v) {
        case 0: {
            s32 a;
            s32 b;
            Func_02003b4a(0xbb);
            a = 1;
            b = 5;
            Func_020039fe(0x30, 0x3b, 31, 0x24, a, b);
            break;
        }
        case 1:
            Func_02003a10(0x30, 0x3b, 31, 0x24, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x25, v, 5);
            break;
        case 2:
            v = 1;
            Func_02003a30(0x30, 0x3b, 31, 0x25, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x26, v, 5);
            break;
        case 3:
            v = 1;
            Func_02003a50(0x30, 0x3b, 31, 0x26, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x27, v, 5);
            break;
        case 4: {
            s32 val = 2;
            s32 *p = &Data_0200ade8;
            *p = val;
        }
            v = 1;
            Func_02003a76(0x30, 0x3b, 31, 0x27, v, v);
            Func_02003a88(0x30, 0x3b, 31, 0x28, v, 5);
            break;
        case 0x5f: {
            s32 a = 1;
            s32 b = 10;
            Func_02003a9e(0x30, 0x31, 31, 0x24, a, b);
            break;
        }
        }
        {
            u16 *c = &Data_0200ade0;
            u32 w;
            *c = *c + 1;
            w = *c;
            if (w > ((u32)(Func_02003a9a() * 40) >> 16) + 105) {
                *c = 0;
            }
        }
    }
}
