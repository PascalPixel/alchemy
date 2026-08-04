typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 */

extern void Func_02002e54(s32 mode);
extern void Func_020033d8(s32 style, s32 variant);
extern void Func_080000c0(s32 frames);
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a4f0(void);
extern void Func_080f9010(s32 cue);
extern s32 Func_080f9048(void);

void Func_02003468(s32 mode)
{
    if (mode == 0) {
        Func_0808a018();
        Func_0808a360();
        Func_0808a370();
        Func_0808a010(30);
        Func_080f9010(0x59);
        Func_02002e54(0);
        Func_020033d8(1, 0);
        Func_0808a010(120);
        Func_0808a020();
        return;
    }

    Func_080f9010(0xf7);
    Func_0808a018();
    Func_0808a360();
    Func_0808a370();
    *(s16 *)(0x0200d9a6 + 30) = (s16)(mode * 60);
    Func_0808a010(30);
    Func_080f9010(mode + 0x5a);
    Func_02002e54(mode);
    Func_020033d8(1, 0);
    Func_0808a010(120);

    goto check_transition;
wait_transition:
    Func_080000c0(1);
check_transition:
    if (Func_080f9048() != 0)
        goto wait_transition;

    Func_080f9010(0x121);
    Func_02002e54(5);
    Func_020033d8(2, 0);
    Func_080f9010(0xec);
    Func_0808a010(60);
    Func_020033d8(2, 1);
    Func_080f9010(0xec);
    Func_0808a010(60);
    Func_02002e54(6);
    Func_020033d8(2, 0);
    Func_080f9010(0xec);
    Func_0808a010(60);
    Func_02002e54(7);
    Func_020033d8(4, 0);
    Func_080f9010(0xed);
    Func_0808a4f0();
    Func_0808a020();
    Func_080770c8(0x123);
}
