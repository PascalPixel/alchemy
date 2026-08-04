typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_381 owner at 0x020010c4, complete 664-byte span through the
 * alignment halfword at 0x02001326 and thirteen-word literal pool
 * 0x02001328-0x0200135b.  The next owner starts at 0x0200135c.
 *
 * This is one owner, not five: 0x02001266, 0x02001292, 0x020012ba and
 * 0x020012de are four calls through the relocated IWRAM 16.16 multiplier
 * at 0x03000118 (`mov ip, pc; bx r9`).  The sole function return is the
 * matching high-register unwind and bx r0 at 0x02001314-0x02001324.
 * tools/m2c_guard.ts therefore refuses this span as a seed; the source below
 * was reconstructed from the full control-flow walk and m2c is used only as
 * the documented truncation cross-check.
 *
 * The owner first derives a ten-frame scale setting, advances the sixteen
 * formation-slot states, and resets actors whose state reaches 19.  Unless
 * the global gate is closed, it then finds the first inactive slot, creates
 * one randomly oriented actor around the fixed formation centre, applies a
 * second random offset, starts mode 0, and arms a thirty-frame countdown.
 *
 * Call targets were independently resolved from each stored BL displacement
 * with the overlay's target-minus-two rule.  There is no skeleton twin in the
 * reviewed semantic corpus (`overlay_twins --semantic --unconverted`).
 */

extern u8 *Data_03001ec4;
extern volatile s32 Data_03001e40;
extern s32 Data_0200bb00;
extern s32 Data_0200bb68;
extern s32 Data_0200bac0[16];
extern u8 Data_0200ba00[];

typedef s32 (*ProjectionHelper)(s32 value, s32 scale);
extern s32 Func_03000118(s32 value, s32 scale);
extern s32 Func_030003e0(s32 value, s32 divisor);
extern s32 Func_030003f0(s32 dividend, s32 divisor);

extern s32 Func_080000f8(void);
extern s32 Func_08000118(s32 angle);
extern s32 Func_08000120(s32 angle);
extern void Func_08009080(u8 *actor, s32 mode);
extern void Func_08009098(u8 *actor, const void *script);
extern void Func_08009150(u8 *actor, s32 x, s32 y, s32 z);
extern void Func_080091e0(u8 *actor, s32 mode);
extern void Func_080091f0(s32 x, s32 y, s32 z);
extern s32 Func_080770c0(s32 flagId);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a160(u8 *actor, s32 mode);
extern void Func_080f9010(s32 soundId);

void Func_020010c4(void)
{
    ProjectionHelper project = (ProjectionHelper)0x03000118;
    u8 *scene = Data_03001ec4;
    s32 scale = Func_030003f0(Data_0200bb00, 10);
    u32 slot;

    if (scale != 0) {
        *(s32 *)(scene + 0x40c) = 0;
        Func_080091f0(scale << 16, scale << 16, 0x10000);
    } else {
        *(s32 *)(scene + 0x40c) = 1;
        Func_080091f0(-1, -1, 0xe666);
    }

    if (Data_0200bb00 != 0)
        Data_0200bb00 -= 3;

    for (slot = 0; slot < 16; slot++) {
        s32 state = Data_0200bac0[slot];

        if (state != 0) {
            u8 *actor = Func_0808a080((s32)slot + 16);

            if (*(u32 *)(actor + 56) == 0x80000000u &&
                *(u32 *)(actor + 64) == 0x80000000u) {
                state++;
                Data_0200bac0[slot] = state;
                if (state == 2)
                    Func_08009080(actor, 3);

                if (state == 19) {
                    *(s32 *)(actor + 8) = 0;
                    *(s32 *)(actor + 12) = 0;
                    *(s32 *)(actor + 16) = 0;
                    *(s32 *)(actor + 36) = 0;
                    *(s32 *)(actor + 40) = 0;
                    *(s32 *)(actor + 44) = 0;
                    *(u32 *)(actor + 56) = 0x80000000u;
                    *(u32 *)(actor + 60) = 0x80000000u;
                    *(u32 *)(actor + 64) = 0x80000000u;
                    Func_0808a160(actor, 15);
                } else if (state == 20) {
                    Data_0200bac0[slot] = 0;
                }
            }
        }
    }

    if (Data_0200bb68 == 999 ||
        (Data_03001e40 & Data_0200bb68) != 0)
        return;

    for (slot = 0; slot < 16; slot++) {
        s16 angle;
        u8 *actor;
        s32 first;
        s32 second;
        s32 randomScale;
        s32 x;
        s32 y;

        /* This order is observable: the random draw precedes actor lookup. */
        angle = (s16)Func_030003e0(Func_080000f8(), 0xffff);
        actor = Func_0808a080((s32)slot + 16);
        if (Data_0200bac0[slot] != 0)
            continue;

        if (Func_080770c0(0x246) == 0)
            Func_080f9010(0xf6);

        Data_0200bac0[slot] = 1;
        actor[85] = 0;
        *(s32 *)(actor + 48) = 0x80000;
        *(s32 *)(actor + 52) = 0x10000;
        Func_080091e0(actor, 0);
        (*(u8 **)(actor + 80))[9] =
            (u8)(((*(u8 **)(actor + 80))[9] & (u8)~12) | 4);
        Func_08009080(actor, 2);
        Func_08009098(actor, Data_0200ba00);

        first = Func_08000120((u16)angle);
        randomScale = (s32)((((u32)Func_080000f8() << 8) >> 16) << 16);
        *(s32 *)(actor + 8) =
            project(first, randomScale + 0x01000000) + 0x01450000;
        *(s32 *)(actor + 12) = 0;

        second = Func_08000118((u16)angle);
        randomScale = (s32)((((u32)Func_080000f8() << 8) >> 16) << 16);
        *(s32 *)(actor + 16) =
            project(second, randomScale + 0x01000000) + 0x012e0000;

        first = Func_08000120((u16)angle);
        randomScale = (Func_080000f8() & 63) << 16;
        x = project(first, randomScale + 0x80000) + 0x01450000;

        second = Func_08000118((u16)angle);
        randomScale = (Func_080000f8() & 63) << 16;
        y = project(second, randomScale + 0x80000) + 0x011e0000;

        Func_08009150(actor, x, y, 0);
        Func_0808a160(actor, 0);
        Data_0200bb00 = 30;
        return;
    }
}
