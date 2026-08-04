typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Func_0808a080(s32);
extern void Func_080000d8(u8 *);

void Func_02001240(void)
{
    u8 *progress = *(u8 **)0x03001ebc;
    u8 *leader = Func_0808a080(0);
    u32 x = *(u32 *)(leader + 8);
    s32 z = *(s32 *)(leader + 16);

    if (*(s16 *)(0x02000240 + 588) == 0 &&
        x >= 0x00900000 && x <= 0x00b00000 &&
        z >= 0x00a80000 && z < 0x00b00000) {
        Func_080000d8((u8 *)0x02009241);
        *(s16 *)(progress + 386) = 91;
    }
}
