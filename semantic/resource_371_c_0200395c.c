typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 Data_02000240[];
extern s32 Data_03001e40;
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 enabled);
extern s32 Func_080000f8(void);
extern s32 Func_030003e0(s32 state, s32 mode);

/* Start the diagonal camera step selected by the current transition phase. */
void Func_0200395c(void)
{
    u8 *actor;
    s32 x;
    s32 y;
    s32 phase;
    s32 dx;
    s32 dy;

    actor = Func_0808a080(*(s32 *)(Data_02000240 + 500));
    x = *(s16 *)(actor + 10);
    y = *(s16 *)(actor + 18);
    if (Func_030003e0(Data_03001e40, 3) != 0) {
        return;
    }

    phase = (Func_080000f8() << 2) >> 16;
    if (phase <= 2) {
        dx = phase == 0 ? -1 : 1;
        dy = phase == 1 ? -1 : 1;
        Func_0808a210((x + dx) << 16, -1, (y + dy) << 16, 1);
    } else if (phase == 3) {
        Func_0808a210((x - 1) << 16, -1, (y - 1) << 16, 1);
    }
}
