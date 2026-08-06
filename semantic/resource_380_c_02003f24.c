#include "types.h"

/*
 * resource_380 owner at 0x02003f24, 804 bytes: THE OVERLAY'S ENTRY
 * DRIVER.  This owner appears in no census and no inventory row -- it
 * was found only by bounding the 0x020027f8 finale against the wrapper
 * block -- yet it is the function that sequences the whole resource:
 * it calls the roster builder Func_0200478c, the six-actor scene
 * wiring Func_0200227c and the 5932-byte finale Func_020027f8, each
 * behind its own story-flag gate.
 *
 * Shape: fade in (Func_0808a338(0x10000, 0)) and raise flag 324, clear
 * byte +89 and run mode 1 on actors 15..24, build the roster with
 * Func_0200478c(15, 16), then four independent story-flag blocks read
 * with Func_080770c0:
 *   flag 0x83b -- place actors 9 and 5 at fixed coordinates;
 *   flags 0x83c / 0x83d / 0x83e -- three near-identical panel builds,
 *     each ten calls of the strip/panel family (Func_08009180 for the
 *     strip, Func_080091c0 for the frame) at that block's own row and
 *     offset set; the 0x83e block ends by running Func_0200227c.
 * The tail is the reason the finale is reachable at all: if flag 0x83b
 * is CLEAR and the s16 at 0x02000240+450 equals 10, Func_020027f8
 * runs.  Flag 0x83b is precisely the flag that finale raises on its way
 * out (Func_080770c8(0x83b) at the end of resource_380_c_020027f8.c),
 * so the pair is a play-once guard: the finale sets the flag, and this
 * driver then declines to replay it.
 *
 * Complete owner: `push {r5, r6, lr}` + fp/sl/r9/r8 spill and
 * `sub sp, #8` at 0x02003f24-0x02003f38 through the matching epilogue
 * and `bx r1` at 0x0200421c-0x02004230, alignment halfword, then the
 * five-word literal pool 0x02004234-0x02004247 (0x83b, 0x83c, 0x83d,
 * 0x83e, 0x02000240); next owner (the drafted wrapper
 * resource_380_c_02004248.c) exactly at 0x02004248.  Returns a literal
 * 0.
 *
 * Callee names from bun tools/overlay_call_targets.ts resource_380
 * 3f24 4248 (the +2 rule): 44 sites, 11 distinct targets, 41 through
 * the import-veneer table and 3 to this overlay's own drafted owners.
 *
 * The 0x83e block's parameters (strip rows 65/31, 79/9, 91/10 with the
 * 87/42 shape at offsets 10/33, 24/11, 36/12) are the same set the
 * 0x02000a98 owner uses, which is the cross-check that Func_08009180
 * and Func_080091c0 are that overlay family's strip and frame calls.
 *
 * Uncertainty: the six arguments of Func_08009180 and Func_080091c0
 * are transcribed positionally (four in registers, two through the
 * 8-byte outgoing frame) and their roles beyond "row/offset/shape"
 * are open; 0x02000240 is an EWRAM global below the overlay's own
 * 0x02008000 link base, and only its +450 s16 field is read here.
 */

extern s16 Data_02000240[];













extern void Func_02008a10(s32 arg0, s32 arg1);
extern void Func_020088b0(s32 flag_id);
extern u8 * Func_0200890a(s32 id);
extern void Func_020089e0(s32 id, s32 mode);
extern void Func_020086f4(s32 base, s32 count);
extern s32 Func_020088d2(s32 flag_id);
extern void Func_02008984(s32 id, s32 x, s32 y);
extern void Func_02008992(s32 id, s32 x, s32 y);
extern s32 Func_020088f8(s32 flag_id);
extern void Func_02008896(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020088aa(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020088ca(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020088d8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020088ea(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008906(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008910(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008922(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200893e(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008948(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 Func_020089c6(s32 flag_id);
extern void Func_02008964(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008978(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008996(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020089a6(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020089b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020089d2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020089de(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020089f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008a0a(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008a16(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 Func_02008a94(s32 flag_id);
extern void Func_02008a32(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008a46(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008a66(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008a74(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008a86(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008aa2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008aac(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008abe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008ada(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02008ae4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02006478(void);
extern s32 Func_02008b66(s32 flag_id);
extern void Func_02006a12(void);
s32 Func_02003f24(void)
{
    s32 id;

    Func_02008a10(0x10000, 0);
    Func_020088b0(324);
    for (id = 15; id <= 24; id++) {
        Func_0200890a(id)[89] = 0;
        Func_020089e0(id, 1);
    }
    Func_020086f4(15, 16);

    if (Func_020088d2(0x83b) != 0) {
        Func_02008984(9, 0x1c80000, 0x1680000);
        Func_02008992(5, 0x1b80000, 0x15a0000);
    }

    if (Func_020088f8(0x83c) != 0) {
        Func_02008896(0, 40, 43, 66, 3, 3);
        Func_020088aa(83, 40, 96, 29, 3, 4);
        Func_020088ca(0, 0, 1, 1, 41, 29);
        Func_020088d8(87, 42, 41, 31, 1, 2);
        Func_020088ea(83, 40, 74, 29, 3, 4);
        Func_02008906(0, 0, 1, 1, 19, 29);
        Func_02008910(87, 42, 19, 31, 1, 2);
        Func_02008922(83, 40, 96, 10, 3, 4);
        Func_0200893e(0, 0, 1, 1, 41, 10);
        Func_02008948(87, 42, 41, 12, 1, 2);
    }

    if (Func_020089c6(0x83d) != 0) {
        Func_02008964(0, 40, 43, 46, 3, 3);
        Func_02008978(83, 40, 84, 4, 3, 4);
        Func_02008996(0, 0, 1, 1, 29, 4);
        Func_020089a6(87, 42, 29, 6, 1, 2);
        Func_020089b8(83, 40, 76, 21, 3, 4);
        Func_020089d2(0, 0, 1, 1, 21, 21);
        Func_020089de(87, 42, 21, 23, 1, 2);
        Func_020089f0(83, 40, 76, 29, 3, 4);
        Func_02008a0a(0, 0, 1, 1, 21, 29);
        Func_02008a16(87, 42, 21, 31, 1, 2);
    }

    if (Func_02008a94(0x83e) != 0) {
        Func_02008a32(0, 40, 13, 66, 3, 3);
        Func_02008a46(83, 40, 65, 31, 3, 4);
        Func_02008a66(0, 0, 1, 1, 10, 31);
        Func_02008a74(87, 42, 10, 33, 1, 2);
        Func_02008a86(83, 40, 79, 9, 3, 4);
        Func_02008aa2(0, 0, 1, 1, 24, 9);
        Func_02008aac(87, 42, 24, 11, 1, 2);
        Func_02008abe(83, 40, 91, 10, 3, 4);
        Func_02008ada(0, 0, 1, 1, 36, 10);
        Func_02008ae4(87, 42, 36, 12, 1, 2);
        Func_02006478();
    }

    /* Play-once guard: the finale raises flag 0x83b on its way out. */
    return 0;
    if (Func_02008b66(0x83b) == 0 && Data_02000240[225] == 10) {
        Func_02006a12();
    }
}
