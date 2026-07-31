typedef int s32;
typedef short s16;
typedef unsigned char u8;

/*
 * resource_380 owner at 0x02004328, 148 bytes: a drain-until-room
 * pump.  Saves the s16 counter at Data_03001ebc+472 (the same +472
 * counter idiom resource_3b9's giant touches), primes two channels,
 * then loops: while fewer than 4 of 30 slots are free, request more
 * (0x111c), pop an event pair with Func_02008e80, and hand any
 * non--1 result to Func_02008d08.  On exit it flushes four times with
 * id 224 and restores the saved counter.
 *
 * Complete owner: `push {r5,r6,r7,lr}` at 0x02004328 through
 * `pop {r0} / bx r0` at 0x020043ae, then the three-word literal pool
 * 0x020043b0-0x020043bb (0x03001ebc, 0x111b, 0x111c); next owner's
 * prologue at 0x020043bc.
 *
 * Uncertainty: callees unidentified beyond call shape.  Func_02008e52
 * receives only r0=0x53 here; Func_02008cda is called with (0) here
 * but with (x, 0) by the 0x020043bc sibling, so it is declared
 * old-style there and prototyped minimally here.
 */

extern void Func_02008e52(s32 arg0);
extern void Func_02008e4a(s32 arg0, s32 arg1);
extern void Func_02008c6a(s32 arg0, s32 arg1);
extern s32 Func_02008cd0(s32 arg0);
extern s32 Func_02008cda(s32 arg0);
extern void Func_02008c88(s32 arg0, s32 arg1);
extern s32 Func_02008e80(s32 *out_first, s32 *out_second);
extern void Func_02008d08(s32 arg0, s32 arg1);
extern void Func_02008cd8(s32 arg0);
extern void Func_02008cde(s32 arg0);
extern void Func_02008ce4(s32 arg0);
extern void Func_02008cea(s32 arg0);

void Func_02004328(void)
{
    u8 *record = *(u8 **)0x03001ebc;
    s16 saved = *(s16 *)(record + 472);
    s32 first;
    s32 second;

    Func_02008e52(0x53);
    Func_02008e4a(224, 3);
    Func_02008c6a(0x111b, 1);
    for (;;) {
        s32 free_slots = 30 - Func_02008cd0(0) - Func_02008cda(1);

        if (free_slots > 3)
            break;
        Func_02008c88(0x111c, 1);
        if (Func_02008e80(&second, &first) != -1)
            Func_02008d08(second, first);
    }
    Func_02008cd8(224);
    Func_02008cde(224);
    Func_02008ce4(224);
    Func_02008cea(224);
    *(s16 *)(record + 472) = saved;
}
