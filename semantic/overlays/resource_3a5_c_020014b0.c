typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern void Func_02001054(void);
extern s32 Func_080770c0();
extern void Func_080770c8();
extern void Func_02001c78();
extern u8 *Func_0808a080();
extern void Func_0808a100();
extern void Func_0808a148();
extern void Func_0808a010();
extern void Func_0808a130();
extern void Func_0808a1f0();
extern void Func_08000128();
extern void Func_0808a090();
extern void Func_0808a0e0();
extern void Func_0808a0e8();
extern void Func_080f9010();
extern void Func_0808a138();
extern void Func_0808a0d0();

/* Run the scene-0x90a finale and hand actor 8 its closing pose. */
void Func_020014b0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    u8 *actor;
    s32 position[3] = { 0, 0, 0 };

    Func_02001054();
    if (Func_080770c0(0x90a) != 0)
        return;
    if (Func_080770c0(0x200) != 0)
        return;

    Func_080770c8(0x200);
    Func_02001c78(1);
    *(u16 *)(workspace + 0xcba) = 600;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 36) = 0;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 44) = 0;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 56) = (s32)0x80000000;
    actor = Func_0808a080(0);
    *(s32 *)(actor + 64) = (s32)0x80000000;
    Func_0808a100(0, 1);
    Func_0808a148(0, 8, 0);
    Func_0808a010(40);
    Func_0808a130(0, 2);
    Func_0808a1f0(0, 0x102);
    Func_0808a010(40);
    actor = Func_0808a080(0);
    actor[90] &= (u8)~1;
    actor = Func_0808a080(0);
    Func_08000128((void *)0xfff00000, *(u16 *)(actor + 6), position);
    Func_0808a090(0, 0x20000, 0x10000);
    Func_0808a100(0, 2);
    Func_0808a0e0(0, position[0] >> 16, position[2] >> 16);
    Func_0808a0e8(0);
    Func_0808a010(2);
    actor = Func_0808a080(0);
    actor[90] |= 1;
    Func_0808a010(30);
    Func_080f9010(148);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a090(8, 0x28000, 0x28000);
    Func_0808a0d0(8, 168, 104);
    Func_0808a090(8, 0x10000, 0x4000);
    Func_0808a0d0(8, 168, 92);
    *(u16 *)(workspace + 0xcba) = 600;
    Func_02001c78(0);
}
