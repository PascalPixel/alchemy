typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

extern u8 *Data_03001ebc;

extern void Func_02004204();
extern void Func_0200433e();
extern void Func_02004352();
extern void Func_02004218();
extern void Func_02003292();
extern void Func_020042d0();
extern s32 Func_020041fe();
extern void Func_020032a2();
extern void Func_020032c0();
extern void Func_0200422e();
extern void Func_0200426a();

void Func_0200160c(void)
{
    Func_02004204();
    *(s32 *)((u8 *)Data_03001ebc + 448) = 513;

    Func_0200433e();
    Func_02004352();

    Func_02004218(20);
    Func_02003292(17, 160 << 7);
    Func_020042d0(0x206e);

    if (Func_020041fe(0x8a4) != 0) {
        *(u16 *)((u8 *)Data_03001ebc + 472) =
            (u16)(*(u16 *)((u8 *)Data_03001ebc + 472) + 1);
    }

    Func_020032a2(17);
    Func_020032c0(17, 192 << 6);
    Func_0200422e(0x8a3);

    Func_0200426a();
}
