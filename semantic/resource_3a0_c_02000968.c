typedef unsigned char u8;
typedef signed int s32;
extern void Func_0808a018(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_080000d0(s32 effect, s32 duration);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a1e0(s32 actorId, s32 mode);
extern void Func_0808a020(void);
void Func_02000968(void)
{
    s32 x;
    s32 z;

    Func_0808a018();
    Func_0808a080(20)[0x23] &= (u8)~2;
    Func_0808a080(20)[0x55] = 0;
    x = *(s32 *)(Func_0808a080(20) + 8) >> 20;
    z = *(s32 *)(Func_0808a080(20) + 16) >> 20;
    Func_080091c0(3, 17, 1, 1, x, z);
    Func_080000d0(0x02008325, 0xc80);
    Func_080770c8(0x201);
    Func_0808a1e0(20, 2);
    Func_0808a020();
}
