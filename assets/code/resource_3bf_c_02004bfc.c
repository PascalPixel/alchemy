typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_0000242e;
extern u8 Value_00002430;

s32 Func_0200a212(s32);
void Func_0200a2f4();
void Func_0200a30c(s32, s32);
void Func_0200a24a(void);
s32 Func_0200a23a(s32);
void Func_0200a2f0(s32, s32, s32);
void Func_0200a2ae(s32);
void Func_0200a2ac(s32);
void Func_0200a272(s32);
void Func_0200a33a(s32);
void Func_0200a352(s32, s32);
void Func_0200a380(s32, s32, s32);
void Func_0200a356(s32);
void Func_0200a36e(s32, s32);
void Func_0200a2a4(s32);
void Func_0200a32c(s32, s32);
void Func_0200a2b2(s32);
void Func_0200a37a(s32);
void Func_0200a392();
void Func_0200a3bc();
s32 Func_0200a3a2(s32, s32);
s32 Func_0200a302(s32, s32);
void Func_0200a3ac(s32);
void Func_0200a2ea(s32);
void Func_0200a3c2(s32);
void Func_0200a3d2(s32, s32);
void Func_0200a300(s32);
void Func_0200a324(void);

void Func_02004bfc(void)
{
    if (Func_0200a212(0x226)) {
        Func_0200a2f4(0x2434);
        Func_0200a30c(20, 0);
        return;
    }
    Func_0200a24a();
    Func_0200a2f4(20, 0, 0);
    if (Func_0200a23a(0x227) == 0) {
        Func_0200a2f0(20, 4, 0);
        Func_0200a2ae(20);
        Func_0200a2ac(20);
        Func_0200a272(20);
        {
            u8 *t = &Value_0000242e;

            Func_0200a33a((s32)t);
            Func_0200a352(20, 0);
            t += 1;
            Func_0200a380(20, 258, 30);
            Func_0200a356((s32)t);
        }
        Func_0200a36e(20, 0);
        Func_0200a2a4(30);
        Func_0200a32c(20, 4);
        Func_0200a2b2(30);
    }
    {
        u8 *t = &Value_00002430;

        Func_0200a37a((s32)t);
        Func_0200a392(20, 0);
        Func_0200a3bc(20, 0x101, 40);
        Func_0200a392((s32)(t + 1));
        Func_0200a3a2(20, 0);
        if (Func_0200a302(0, 0) == 0) {
            Func_0200a3ac((s32)(t + 2));
            Func_0200a3bc(20, 0);
            Func_0200a2ea(0x226);
        } else {
            Func_0200a3c2((s32)(t + 3));
            Func_0200a3d2(20, 0);
        }
    }
    Func_0200a300(0x227);
    Func_0200a324();
}
