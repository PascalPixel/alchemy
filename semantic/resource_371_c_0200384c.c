typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02000240[];
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 enabled);
extern void Func_0808a238(const void *script, s32 mode);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080091e0(u8 *actor, s32 mode);
extern void Func_080000c0(s32 frames);
extern void Func_080770c8(s32 flag);
extern void Func_080f9010(s32 cue);

/* Move actors 54 and the selected actor together for sixty ticks. */
void Func_0200384c(void)
{
    u8 *selected;
    u8 *actor54;
    s32 ticks;

    selected = Func_0808a080(*(s32 *)(Data_02000240 + 500));
    actor54 = Func_0808a080(54);
    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080f9010(219);
    Func_080091e0(selected, 0);

    actor54[85] = 0;
    selected[85] = 0;
    *(s32 *)(selected + 40) = 0;
    selected[97] = 1;
    actor54[97] = 1;
    for (ticks = 60; ticks != 0; ticks--) {
        *(s32 *)(selected + 40) += 0x3333;
        *(s32 *)(actor54 + 40) += 0x3333;
        Func_080000c0(1);
    }

    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
    Func_080770c8(0x122);
    Func_0808a238((const void *)2, 27);
}
