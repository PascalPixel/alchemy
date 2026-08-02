typedef unsigned char u8;
typedef signed int s32;

extern void Func_0808a018(void);
extern void Func_080f9010(s32);
extern void Func_0808a338(s32, s32);
extern void Func_0808a330(s32, s32);
extern void Func_0808a348(s32);
extern void Func_080000c0(s32);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a158(s32, s32);
extern u8 *Func_0808a080(s32);
extern void Func_080091e0(u8 *, s32);
extern void Func_0808a208(s32, s32);
extern void Func_080000d0(void (*)(void), s32);
extern void Func_0808a010(s32);
extern void Func_0808a248(s32);
extern void Func_080770c8(s32);
extern void Func_0808a020(void);
extern void Func_020031fc(void);

/* Stage and time the transition driven by the following owner callback. */
void Func_02003130(void)
{
    Func_0808a018();
    Func_080f9010(141);
    Func_0808a338(0, 0);
    Func_0808a330(0, 0);
    Func_0808a348(1);
    Func_080000c0(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Func_0808a360();
    Func_0808a370();
    Func_0808a158(0, 15);
    Func_080091e0(Func_0808a080(0), 0);
    Func_080000c0(1);
    Func_0808a208(0x40000, 0x10000);
    Func_080000d0(Func_020031fc, 2000);
    Func_0808a338(0, 0);
    Func_0808a330(0x10004, 1);
    Func_0808a330(0x10000, 2);
    Func_0808a348(40);
    Func_0808a010(240);
    Func_0808a330(0, 0);
    Func_0808a348(80);
    Func_080000c0(90);
    Func_0808a248(109);
    Func_080770c8(282);
    Func_0808a020();
}
