typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_0808a080(s32 actor);
extern void Func_080000d0(const void *callback, s32 value);
extern void Func_020017ac(void);
extern void Func_0200141c(void);
extern void Func_02001308(void);

/* Enable the staged actors and install their common presentation velocity. */
void Func_02002290(void)
{
    u8 *actor;

    actor = Func_0808a080(8);
    actor[89] = 1;
    actor = Func_0808a080(9);
    actor[89] = 1;
    actor = Func_0808a080(10);
    actor[89] = 1;
    actor = Func_0808a080(11);
    actor[89] = 1;

    actor = Func_0808a080(8);
    *(s32 *)(actor + 24) = 0xb333;
    actor = Func_0808a080(9);
    *(s32 *)(actor + 24) = 0xb333;
    actor = Func_0808a080(10);
    *(s32 *)(actor + 24) = 0xb333;
    actor = Func_0808a080(11);
    *(s32 *)(actor + 24) = 0xb333;
    actor = Func_0808a080(12);
    *(s32 *)(actor + 24) = 0xb333;
    Func_080000d0((const void *)Func_020017ac, 3200);
    Func_080000d0((const void *)Func_0200141c, 3200);
    Func_080000d0((const void *)Func_02001308, 3200);
    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000052 = 0x0607;
}
