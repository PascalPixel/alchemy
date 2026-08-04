typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_080770d0(s32);
extern u8 *Func_0808a080(s32);
extern void Func_080000d8(u8 *);
extern void Func_0200118c(void);

void Func_020011d0(void)
{
    u8 *progress = *(u8 **)0x03001ebc;
    u8 *leader;
    u32 x;
    s32 z;

    Func_080770d0(0x241);
    Func_080770d0(0x240);
    leader = Func_0808a080(0);
    x = *(u32 *)(leader + 8);
    z = *(s32 *)(leader + 16);

    if (x >= 0x00680001 && x <= 0x00f00000 &&
        z > 0x00a00000 && z < 0x00f80000) {
        Func_080000d8((u8 *)0x02009241);
        *(s16 *)(progress + 386) = 91;
    }

    Func_0200118c();
    Func_080770d0(0x244);
}
