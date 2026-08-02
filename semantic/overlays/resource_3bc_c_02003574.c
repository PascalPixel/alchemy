typedef signed short s16;
typedef signed int s32;

/* resource_3bc signed transition-step owner, complete 204-byte span. */

extern void Func_02002e54(s32 mode);
extern void Func_020033d8(s32 style, s32 variant);
extern void Func_080000c0(s32 frames);
extern void Func_0808a010(s32 frames);
extern void Func_0808a1e8(s32 actor, s32 cue, s32 frames);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 cue);
extern s32 Func_080f9048(void);

void Func_02003574(s32 step)
{
    s32 actor;
    s32 magnitude = step < 0 ? -step : step;

    Func_080f9010(0xf7);
    Func_0808a360();
    Func_0808a370();
    *(s16 *)(0x0200da50 + 26) = (s16)(step * 60);
    *(s16 *)(0x0200cce2 + 26) = (s16)(magnitude * 60);

    if (step < 0) {
        Func_0808a010(30);
        Func_080f9010(0x56);
        Func_02002e54(8);
        Func_020033d8(3, 1);
        Func_0808a010(magnitude * 60 + 60);
        actor = 0;
    } else {
        Func_0808a010(30);
        Func_080f9010(step + 0x5a);
        Func_02002e54(4);
        Func_020033d8(3, 0);
        Func_0808a010(step * 60 + 60);
        actor = 8;
    }

    Func_0808a1e8(actor, 0x105, 0);
    goto check_transition;
wait_transition:
    Func_080000c0(1);
check_transition:
    if (Func_080f9048() != 0)
        goto wait_transition;

    Func_080f9010(0x13);
    Func_0808a010(30);
    Func_080f9010(0x121);
    Func_0808a368();
    Func_0808a370();
}
