#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080dd2c4, a sibling of
 * games/gs1/src/battle/effects/member_orbit/run.c (0x080ce85c) in the
 * 0x03001eec "battle work" subsystem family: same heap_cache(0x03001EEC)
 * cursor, the same M2C_FIELD(expr,type_ptr,offset) struct-field convention,
 * the same "object" argument struct (field 4 a mode flag, field 0x14 the
 * member count, field 0x24 the per-member id array stride 2) and the same
 * DrawRectangleFn rectangle_slot[2] pair sourced from Func_080ed408(46,...)
 * / Func_080ed408(47,...).
 *
 * Two differences from that template drive the extra bytes here:
 *
 *   1. This owner takes a second argument (`arg1`, the retained assembly's
 *      `str r1, [sp, #44]` in the very first instruction) that selects
 *      between two literal-pool constants passed to Func_080e0524 and
 *      between two parallel draw-table sets later on.  Both Value_ symbols
 *      follow the established spelling from Func_08002f40's callers: the
 *      reference loads 0x83 and 0x84 through the literal pool rather than
 *      `movs r0,#0x83`, which only an address-of expression reproduces.
 *
 *   2. Every `bl` to 0x080072f4 in the retained assembly is a call through
 *      the trampoline at games/gs1/asm/080072e4.s (0x080072e4 + 4*4, the r4
 *      slot): the fixed branch target is `bx r4`, and r4 is loaded with
 *      rectangle_slot[1 & idx] or rectangle[0]/rectangle[1] immediately
 *      before each call.  These are modeled as DrawRectangleFn calls
 *      through that traced pointer, not as calls to a fictional
 *      "Func_080072f4".
 *
 * Several signed-halving sites keep the raw unsigned byte/loop-counter
 * value alongside the sign-extended one (matching the note already
 * recorded in games/gs1/recon/en/main/080e7404.c for the analogous u16
 * case); those are spelled out with the shifted-word idiom rather than an
 * s8/s16 cast.  Plain nonnegative loop-counter halvings use `/ 2` per the
 * family's sign-extension-bug guidance.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_00000083;
extern u8 Value_00000084;

extern void *Data_03001e50[];

extern u8 Data_080eeb4b[];
extern u16 Data_080eeb4e[];
extern u8 Data_080eeb48[];
extern s8 Data_080eeb71[];
extern u16 Data_080eeb58[];
extern u8 Data_080eeb54[];
extern u8 Data_080eeb5e[];
extern u8 Data_080eeb61[];
extern s8 Data_080eeb79[];
extern u8 Data_080eeb80[];
extern u16 Data_080eeb88[];

void Func_080cd594(s32 mode);
void Func_080e0524(s32 id, void *work, s32 a, s32 b);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
s32 Func_08004458(void);
s32 Func_080022fc(s32 a, s32 b);
void Func_080b50e8(s32 id);
void Func_080041d8(void *callback, s32 interval);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080dd2c4(void *object, s32 arg1)
{
    void **heap_cache;
    void **cursor;
    void *work;
    s32 mode;
    void *draw_destination;
    s32 status;
    void *rectangle[2];
    void *second_rectangle;
    void **rectangle_slot;

    s32 sp0C;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    s32 sp20;
    s32 sp24;

    u8 *slot_cursor;
    s32 i;

    void *var_r6_121;
    s32 var_r9_120;
    s32 temp_r2_134;
    s32 temp_r5_140;
    s32 temp_r3_145;

    void *var_sl_273;
    s32 var_r9_230;
    s32 temp_r4_299;
    s32 var_r5_297;
    s32 temp_r3_298;
    u8 temp_r4_315;
    s8 temp_r3_335;
    u8 temp_r4_348;

    s32 var_r6_367;
    s32 var_r4_377;
    s32 var_r7_374;

    void *var_r5_420;
    s32 var_r6_421;

    s32 var_r9_470;
    void *var_r8_471;
    s32 temp_r2_474;
    s32 temp_r7_481;
    s32 var_sl_482;
    s8 temp_r5_490;
    s32 temp_r6_492;
    u8 temp_r0_496;
    s8 temp_r4_498;
    u8 temp_r0_514;
    s8 temp_r4_518;
    s32 temp_r3_538;

    mode = arg1;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(1);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    M2C_FIELD((void *)0x04000020, s16 *, 0x30) = 0;
    if (mode == 1) {
        Func_080e0524((s32)&Value_00000083, work, 1, 1);
    } else {
        Func_080e0524((s32)&Value_00000084, work, 1, 1);
    }
    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
        *(s32 *)0x04000028 = 0xFFFF9000;
    }
    status = Func_080ed408(46, 7, 7, 3, 1);
    rectangle[0] = Data_03001e50[46];
    status = Func_080ed408(47, 7, 7, 7, 1);
    second_rectangle = Data_03001e50[47];
    rectangle_slot = rectangle;
    rectangle_slot[1] = second_rectangle;

    sp20 = Data_080eeb5e[
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)] * 4 + 0x38;

    slot_cursor = (u8 *)0x02010018;
    i = 0;
    do {
        i += 1;
        M2C_FIELD(slot_cursor, s32 *, 0) = -1;
        slot_cursor += 28;
    } while (i != 0x400);

    var_r9_120 = 0;
    var_r6_121 = (u8 *)work + 0x7080;
    do {
        temp_r2_134 = (Data_080eeb61[var_r9_120] + (7 & Func_08004458())) - 4;
        M2C_FIELD(var_r6_121, s32 *, 4) = var_r9_120 / 2 + 0x6C;
        M2C_FIELD(var_r6_121, s32 *, 0) = temp_r2_134;
        temp_r5_140 = (63 & Func_08004458()) + 0x37;
        M2C_FIELD(var_r6_121, s32 *, 0x10) = temp_r5_140;
        temp_r3_145 = Data_080eeb4b[Func_080022fc(var_r9_120, 3)];
        if (temp_r3_145 < temp_r5_140) {
            M2C_FIELD(var_r6_121, s32 *, 0x10) = temp_r3_145;
        }
        M2C_FIELD(var_r6_121, s32 *, 0x18) = var_r9_120 * 4 + 8;
        var_r9_120 += 1;
        var_r6_121 += 28;
    } while (var_r9_120 != 16);

    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);

    sp24 = 0;
    if (sp20 != 0) {
        sp18 = sp20 - 64;
        sp14 = sp20 - 20;
        sp10 = sp20 - 4;
        do {
            if (sp24 == sp18) {
                Func_080b50e8(0x84);
            }
            if (sp24 >= sp14 && sp24 < sp10) {
                *(s16 *)0x04000050 = 0x3F44;
                *(s16 *)0x04000052 = ((sp20 - sp24) - 5) | 0x1000;
            }
            if (sp24 < sp10) {
                if (Data_080eeb5e[M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)]
                        != 0) {
                    sp0C = 8;
                    var_sl_273 = (u8 *)work + 0x7080;
                    var_r9_230 = 0;
                    do {
                        if (sp24 == ((var_r9_230 * 4) + 9)) {
                            M2C_FIELD(work, s32 *, 0x77A8) = 2;
                        }
                        if (sp24 > sp0C) {
                            var_r5_297 = (sp24 - sp0C) * 8;
                            temp_r3_298 = M2C_FIELD(var_sl_273, s32 *, 0x10);
                            temp_r4_299 = Func_080022fc(var_r9_230, 3);
                            if (var_r5_297 > temp_r3_298) {
                                var_r5_297 = temp_r3_298;
                            }
                            if (mode == 0) {
                                temp_r4_315 = Data_080eeb48[temp_r4_299];
                                ((DrawRectangleFn)
                                    rectangle_slot[1 & var_r9_230])(
                                    draw_destination,
                                    (u8 *)work + Data_080eeb4e[temp_r4_299],
                                    M2C_FIELD(var_sl_273, s32 *, 0)
                                        - (temp_r4_315 >> 1),
                                    M2C_FIELD(var_sl_273, s32 *, 4)
                                        - var_r5_297,
                                    temp_r4_315, var_r5_297);
                            } else {
                                temp_r3_335 = Data_080eeb71[7 & var_r9_230];
                                if (var_r5_297 > temp_r3_335) {
                                    var_r5_297 = temp_r3_335;
                                }
                                temp_r4_348 = Data_080eeb54[temp_r4_299];
                                ((DrawRectangleFn)
                                    rectangle_slot[1 & var_r9_230])(
                                    draw_destination,
                                    (u8 *)work + Data_080eeb58[temp_r4_299],
                                    M2C_FIELD(var_sl_273, s32 *, 0)
                                        - (temp_r4_348 >> 1),
                                    M2C_FIELD(var_sl_273, s32 *, 4)
                                        - var_r5_297,
                                    temp_r4_348, var_r5_297);
                            }
                        }
                        var_r6_367 = 0;
                        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s32 *, 0x14) != 0) {
                            var_r7_374 = sp0C + 4;
                            var_r4_377 = 0x24;
                            do {
                                if (sp24 == var_r7_374) {
                                    if (!(var_r9_230 & 1)) {
                                        Func_080f9010(0x85);
                                    }
                                    Func_080d6888(
                                        M2C_FIELD(
                                            M2C_FIELD(work, void **, 0x7828),
                                            s16 *, var_r4_377),
                                        7, 5, var_r6_367, 3);
                                }
                                var_r6_367 += 1;
                                var_r4_377 += 2;
                            } while (var_r6_367 != M2C_FIELD(
                                M2C_FIELD(work, void **, 0x7828), s32 *,
                                0x14));
                        } else {
                            var_r7_374 = sp0C + 4;
                        }
                        if ((sp24 == var_r7_374) || (sp24 == (sp0C + 8))) {
                            var_r5_420 = (void *)0x02010000;
                            var_r6_421 = 0;
                        loop_50:
                            if (var_r6_421 != 0x200) {
                                if (M2C_FIELD(var_r5_420, s32 *, 0x18)
                                        == -1) {
                                    M2C_FIELD(var_r5_420, s32 *, 0) =
                                        ((Func_08004458() & 0xF)
                                            + M2C_FIELD(var_sl_273, s32 *,
                                                0)) - 8;
                                    M2C_FIELD(var_r5_420, s32 *, 4) =
                                        (Func_08004458() & 0xF) + 0x50;
                                    M2C_FIELD(var_r5_420, s32 *, 0x18) = 0;
                                } else {
                                    var_r5_420 += 0x1C;
                                    var_r6_421 += 1;
                                    goto loop_50;
                                }
                            }
                        }
                        var_sl_273 += 0x1C;
                        sp0C += 4;
                        var_r9_230 += 1;
                    } while (var_r9_230 != Data_080eeb5e[M2C_FIELD(
                        M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)]);
                }
            }

            var_r9_470 = 0;
            var_r8_471 = (void *)0x02010000;
            do {
                temp_r2_474 = M2C_FIELD(var_r8_471, s32 *, 0x18);
                if (temp_r2_474 >= 0) {
                    temp_r7_481 = temp_r2_474 / 2;
                    var_sl_482 = 0x1E59;
                    if (mode != 0) {
                        var_sl_482 = 0xAFF;
                    }
                    temp_r5_490 = Data_080eeb79[temp_r7_481];
                    temp_r6_492 = temp_r7_481 * 2;
                    temp_r0_496 = Data_080eeb80[temp_r7_481];
                    temp_r4_498 = (s8)temp_r0_496;
                    ((DrawRectangleFn)rectangle[0])(
                        draw_destination,
                        (u8 *)work + Data_080eeb88[temp_r6_492]
                            + var_sl_482,
                        M2C_FIELD(var_r8_471, s32 *, 0) - temp_r5_490,
                        M2C_FIELD(var_r8_471, s32 *, 4)
                            - ((temp_r4_498
                                + (s32)((u32)(temp_r0_496 << 24) >> 31))
                                >> 1),
                        temp_r5_490, temp_r4_498);
                    temp_r0_514 = Data_080eeb80[temp_r7_481];
                    temp_r4_518 = (s8)temp_r0_514;
                    ((DrawRectangleFn)rectangle[1])(
                        draw_destination,
                        (u8 *)work + Data_080eeb88[temp_r6_492]
                            + var_sl_482,
                        M2C_FIELD(var_r8_471, s32 *, 0),
                        M2C_FIELD(var_r8_471, s32 *, 4)
                            - ((temp_r4_518
                                + (s32)((u32)(temp_r0_514 << 24) >> 31))
                                >> 1),
                        Data_080eeb79[temp_r7_481], temp_r4_518);
                    temp_r3_538 = M2C_FIELD(var_r8_471, s32 *, 0x18) + 1;
                    M2C_FIELD(var_r8_471, s32 *, 0x18) = temp_r3_538;
                    if (temp_r3_538 == 0xE) {
                        M2C_FIELD(var_r8_471, s32 *, 0x18) = -1;
                    }
                }
                var_r9_470 += 1;
                var_r8_471 += 0x1C;
            } while (var_r9_470 != 0x200);

            Func_080e155c(4, 4);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            sp24 += 1;
        } while (sp24 != sp20);
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
