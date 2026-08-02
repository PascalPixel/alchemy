typedef unsigned char u8;
typedef signed int s32;

extern void Func_0808a018();
extern u8 *Func_0808a080();
extern void Func_0808a0f0();
extern void Func_0808a090();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a360();
extern void Func_0808a370();
extern void Func_0808a138();
extern void Func_0808a010();
extern void Func_0808a0c8();
extern void Func_0808a098();
extern void Func_0808a0e8();
extern void Func_0808a1b8();
extern void Func_0808a1e8();
extern void Func_0808a220();
extern void Func_0808a0d0();
extern void Func_0808a218();
extern void Func_080091c0();
extern void Func_0808a020();

/* Run the complete actor-3/actor-8 exchange and its two repeated beats. */
void Func_02001730(void)
{
    u8 *actor;
    s32 pass;

    Func_0808a018();
    actor = Func_0808a080(12); *(s32 *)(actor + 24) = -0x10000;
    actor = Func_0808a080(13); *(s32 *)(actor + 24) = -0x10000;
    actor = Func_0808a080(14); *(s32 *)(actor + 24) = -0x10000;
    Func_0808a0f0(3, 0x880000, 0xb80000);
    Func_0808a0f0(0, 0x880000, 0x1280000);
    Func_0808a0f0(8, 0x880000, 0x980000);
    Func_0808a090(3, 0x18000, 0xc000);
    Func_0808a090(8, 0x18000, 0xc000);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x880000, -1, 0xb80000, 0);
    Func_0808a360();
    Func_0808a370();
    Func_0808a138(3, 1);
    actor = Func_0808a080(8); actor[90] &= (u8)~1;
    Func_0808a010(20);

    for (pass = 0; pass < 2; pass++) {
        Func_0808a0c8(3, 152, 168);
        Func_0808a010(10);
        Func_0808a098(8, (void *)0x0200a8c8);
        Func_0808a0e8(3);
        Func_0808a1b8(3, 0xc000, 30);
        Func_0808a138(3, 1);
        actor = Func_0808a080(3); actor[90] &= (u8)~1;
        Func_0808a0c8(3, 136, 184);
        Func_0808a010(10);
        Func_0808a098(8, (void *)0x0200a820);
        Func_0808a0e8(3);
        actor = Func_0808a080(3); actor[90] |= 1;
        Func_0808a010(30);
        Func_0808a0c8(3, 120, 168);
        Func_0808a010(5);
        Func_0808a098(8, (void *)0x0200a874);
        Func_0808a0e8(3);
        Func_0808a1b8(3, 0xc000, 30);
        Func_0808a138(3, 1);
        Func_0808a010(15);
        actor = Func_0808a080(3); actor[90] &= (u8)~1;
        Func_0808a0c8(3, 136, 184);
        Func_0808a010(15);
        Func_0808a098(8, (void *)0x0200a820);
        Func_0808a0e8(3);
        Func_0808a138(3, 1);
        actor = Func_0808a080(3); actor[90] |= 1;
    }

    Func_0808a010(20);
    Func_0808a1e8(3, 0x102, 60);
    actor = Func_0808a080(3); *(void **)(actor + 108) = (void *)0x02008d59;
    Func_0808a220(0, 1);
    Func_0808a010(30);
    Func_0808a0d0(0, 136, 264);
    Func_0808a218();
    Func_080091c0(0, 0, 3, 3, 7, 9);
    Func_0808a020();
}
