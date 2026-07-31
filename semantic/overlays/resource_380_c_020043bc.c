typedef int s32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;

/*
 * resource_380 owner at 0x020043bc, 224 bytes: a 32-tick spawn burst.
 * Builds a 16-byte parameter block on the stack (1, 5, descriptor
 * 0x6666, weight 0x30000, id halfword 284 at +24), then ticks 32
 * times: every even tick it derives a spawn position from two resident
 * counters (x = base_x + (Func_02008c84()*24 masked to its high half)
 * - 0xc0000, y = base_y + (Func_02008c9c()<<5 masked likewise)
 * + 0x200000) and fires the in-overlay spawner Func_0200458e with
 * fixed velocity constants (-0x40000, 0x1b0000); on tick 20 it nudges
 * channel 14 by 256.  Afterwards it settles channel 14 and hands off.
 *
 * Complete owner: `push {r5,r6,r7,lr}` + r8/sl spill at 0x020043bc
 * through `pop {r0} / bx r0` at 0x0200448e, then the literal pool
 * 0x02004490-0x0200449b (0x6666, 0xfff40000, 0xfffc0000); next owner's
 * prologue at 0x0200449c (the drafted resource_380_c_0200449c.c).
 *
 * Uncertainty: callees unidentified beyond call shape; the context
 * object from Func_02008d86(14) supplies base coordinates at +8/+12
 * and a third value at +16 passed straight through.  Func_02008cda and
 * Func_02008d7c consume the result of the preceding call in r0 in the
 * original (register reuse); that dataflow is kept explicit here.
 */

extern s32 Func_02008d86(s32 channel);
extern void Func_02008ee6(s32 arg0);
extern s32 Func_02008d94(s32 channel);
extern s32 Func_02008cda();
extern void Func_02008d96(s32 arg0);
extern s32 Func_02008c84(void);
extern s32 Func_02008c9c(void);
extern void Func_0200458e(
    s32 x, s32 y, s32 arg2, s32 arg3,
    s32 velocity_x, s32 flag, s32 velocity_y, void *parameters);
extern void Func_02008ea6(s32 channel, s32 amount);
extern void Func_02008eb8(s32 channel, s32 arg1);
extern s32 Func_02008e36(s32 channel);
extern void Func_02008d7c();

void Func_020043bc(void)
{
    s32 parameters[7];
    u32 tick;
    s32 context = Func_02008d86(14);

    Func_02008ee6(190);
    Func_02008cda(Func_02008d94(14), 0);

    parameters[0] = 1;
    parameters[1] = 5;
    parameters[2] = 0x6666;
    parameters[3] = 0x30000;
    *(u16 *)((u8 *)parameters + 24) = 284;

    for (tick = 0; tick <= 31; tick++) {
        Func_02008d96(1);
        if ((tick & 1) == 0) {
            s32 x = *(s32 *)(context + 8)
                + (u32)((Func_02008c84() * 24) & 0xffff0000)
                + 0xfff40000;
            s32 y = *(s32 *)(context + 12)
                + (u32)((Func_02008c9c() << 5) & 0xffff0000)
                + 0x200000;

            Func_0200458e(x, y, *(s32 *)(context + 16), 0,
                0xfffc0000, 0, 0x1b0000, parameters);
        }
        if (tick == 20)
            Func_02008ea6(14, 256);
    }
    Func_02008eb8(14, 0);
    Func_02008d7c(Func_02008e36(14), 1);
}
