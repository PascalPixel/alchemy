#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080c972c, part of the 0x03001eec
 * "battle work" family whose field-offset layout and calling shapes are
 * established by games/gs1/src/battle/effects/member_orbit/run.c
 * (main:080ce85c) and games/gs1/src/battle/effects/puff_arc/run.c
 * (main:080d9fc8).  The family-matcher's closest template is member_orbit
 * (score 7767/10000), but this owner is 1332 bytes against the template's
 * 724 and is NOT the same effect: it draws a ring of 64 randomly-placed
 * "wave" markers around the caller's screen position (Data_080ededc[layers]
 * gives the per-kind ring radius, ring count, debris-spawn count and total
 * frame budget), animates each ring cell through a two-cell sprite as it
 * expands outward, and once a cell's distance crosses a threshold it pops
 * into a batch of the kind's debris count, added to a persistent 512-slot
 * fixed-point particle pool at 0x02010140 that every frame ages, draws and
 * hands to Func_080e3908 for physics -- independent of the ring loop
 * itself and still running on frames where the kind's ring count is zero.
 *
 * Shaped from an m2c decompile of the target's own retained assembly
 * (games/gs1/asm/080c972c.s) rather than the member_orbit template, since
 * the two functions share only field offsets and callees, not structure.
 * `Func_080072f4` is not a real symbol: it is the r4 entry of the
 * _call_via_rN trampoline bundle at games/gs1/asm/080072e4.s
 * (0x080072e4 + 4*4), so every call through it is modeled as a direct call
 * through the DrawRectangleFn cached from Func_080ed408's heap slot
 * (sp10/sp14 below, matching the reference's own sp+16/sp+20 spill slots),
 * as in puff_arc/run.c and games/gs1/recon/en/main/080ccc38.c.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 resource_id, void *destination, s32 flag_a, s32 flag_b);
void Func_080c9048(void);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
u32 Func_08004458(void);
void Func_080b50e8(s32 cue);
void Func_080f9010(s32 cue);
void Func_080d6888(s32 actor, s32 b, s32 c, s32 d, s32 e);
void Func_080e3908(void *particle, s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

/* Per-"layers"-kind table, four bytes per kind: [0] active ring count,
   [1] debris particles spawned per pop, [2] ring radius step, [3] total
   frame budget for the effect. */
extern const u8 Data_080ededc[];

/* Sprite offset within the work block, keyed by clamped particle age
   (1..6), read as the byte pair at age*2-2. */
extern const u16 Data_080ede5c[];

void Func_080c972c(void *object) {
    void *temp_r1_17;
    void **heap_cache;
    void **cursor;
    s32 sp8;
    s32 spC;
    DrawRectangleFn sp10;
    s32 sp14;
    s32 sp18;
    s32 sp1C;
    s16 *var_r1_165;
    s16 *var_r1_221;
    s32 *var_r3_93;
    s32 temp_r0_112;
    s32 temp_r0_581;
    s32 temp_r0_590;
    s32 temp_r1_268;
    s32 temp_r1_629;
    s32 temp_r3_176;
    s32 temp_r3_230;
    s32 temp_r3_436;
    s32 temp_r5_360;
    s32 temp_r6_355;
    s32 var_r0_133;
    s32 var_r2_349;
    s32 var_r2_536;
    s32 var_r3_289;
    s32 var_r3_354;
    s32 var_r3_390;
    s32 var_r3_534;
    s32 var_r4_412;
    s32 var_r4_503;
    s32 var_r4_585;
    s32 var_r6_507;
    s32 var_r8_107;
    s32 var_r8_340;
    s32 var_r8_578;
    s32 var_r8_89;
    s32 temp_r1_124;
    s32 temp_r3_362;
    s32 var_r8_163;
    s32 var_r8_218;
    void *temp_r2_115;
    void *temp_r6_437;
    const void *var_r1_535;
    void *var_r5_108;
    void *var_r5_577;
    void *var_r7_347;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    temp_r1_17 = *cursor++;
    sp1C = (s32) *cursor;
    M2C_FIELD(temp_r1_17, void **, 0x7828) = object;
    Func_080cd594(0x2001);
    *(s16 *)0x04000020 = 0x100;
    Func_080e0524(0xCC, (u8 *)temp_r1_17 + 0x604, 1, 1);
    Func_080e0524(0x76, temp_r1_17, 0, 0);
    Func_080c9048();
    *(s16 *)0x04000050 = 0x3F44;
    *(s16 *)0x04000048 = 0x3337;
    Func_080ed408(0x2E, 7, 7, 2, 2);
    sp10 = (DrawRectangleFn)heap_cache[7];
    Func_080ed408(0x2F, 7, 7, 2, 3);
    var_r8_89 = 0;
    sp14 = (s32) heap_cache[8];
    var_r3_93 = (s32 *)0x02010158;
    do {
        var_r8_89 += 1;
        *var_r3_93 = -1;
        var_r3_93 += 7;
    } while (var_r8_89 != 0x200);
    var_r8_107 = 0;
    var_r5_108 = (u8 *)temp_r1_17 + 0x7080;
    do {
        temp_r0_112 = Func_08004458() & 0x3F;
        temp_r2_115 = M2C_FIELD(temp_r1_17, void **, 0x7828);
        temp_r1_124 = 0 - ((var_r8_107 * Data_080ededc[(M2C_FIELD(temp_r2_115, s32 *, 0x18) * 4) + 2]) + 0x10);
        if (M2C_FIELD(temp_r2_115, s32 *, 4) == 1) {
            var_r0_133 = (temp_r0_112 + (temp_r1_124 / 2)) - 0x30;
        } else {
            var_r0_133 = (temp_r0_112 - (temp_r1_124 / 2)) + 0x48;
        }
        M2C_FIELD(var_r5_108, s32 *, 0) = (s32) (var_r0_133 * 8);
        M2C_FIELD(var_r5_108, s32 *, 4) = (s32) (temp_r1_124 * 8);
        var_r8_107 += 1;
        M2C_FIELD(var_r5_108, s32 *, 0x18) = -1;
        var_r5_108 = (u8 *)var_r5_108 + 0x1C;
    } while (var_r8_107 != 0x40);
    if (M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 4) == 0) {
        var_r8_163 = 0;
        var_r1_165 = (s16 *)0x02010000;
        do {
            if ((u32) (var_r8_163 - 8) <= 0x5FU) {
                temp_r3_176 = var_r8_163 / 2;
                *var_r1_165 = (s16) (((0x34 - temp_r3_176) << 8) | (0xB4 - temp_r3_176));
            } else if (var_r8_163 <= 0x87) {
                *var_r1_165 = 0x80;
            } else {
                *var_r1_165 = 0x100;
            }
            var_r8_163 += 1;
            var_r1_165 += 2;
        } while (var_r8_163 != 0xA0);
    } else {
        var_r8_218 = 0;
        var_r1_221 = (s16 *)0x02010000;
        do {
            if ((u32) (var_r8_218 - 8) <= 0x5FU) {
                temp_r3_230 = var_r8_218 / 2;
                *var_r1_221 = (s16) (((temp_r3_230 + 0x3C) << 8) | (temp_r3_230 + 0xBC));
            } else if (var_r8_218 <= 0x87) {
                *var_r1_221 = 0x70F0;
            } else {
                *var_r1_221 = 0x100;
            }
            var_r8_218 += 1;
            var_r1_221 += 2;
        } while (var_r8_218 != 0xA0);
    }
    Func_080041d8((void *)0x080C91A5, 0x480);
    temp_r1_268 = M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18);
    if (temp_r1_268 == 0) {
        M2C_FIELD(temp_r1_17, s32 *, 0x7780) = 1;
        M2C_FIELD(temp_r1_17, s32 *, 0x7784) = temp_r1_268;
    } else {
        if (temp_r1_268 == 1) {
            M2C_FIELD(temp_r1_17, s32 *, 0x7780) = 2;
            var_r3_289 = 0x32;
        } else {
            M2C_FIELD(temp_r1_17, s32 *, 0x7780) = 2;
            var_r3_289 = 0x4B;
        }
        M2C_FIELD(temp_r1_17, s32 *, 0x7784) = var_r3_289;
    }
    Func_080041d8((void *)0x080CD261, 0x480);
    sp18 = 0;
    if (Data_080ededc[(M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4) + 3] == 0) {

    } else {
loop_33:
        if (sp18 == (Data_080ededc[(M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4) + 3] - 0x40)) {
            Func_080b50e8(0x84);
        }
        var_r8_340 = 0;
        if (Data_080ededc[M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4] == 0) {

        } else {
            var_r7_347 = (u8 *)temp_r1_17 + 0x7080;
loop_38:
            var_r2_349 = M2C_FIELD(var_r7_347, s32 *, 0);
            if (var_r2_349 < 0) {
                var_r2_349 += 7;
            }
            var_r3_354 = M2C_FIELD(var_r7_347, s32 *, 4);
            temp_r6_355 = var_r2_349 / 8;
            if (var_r3_354 < 0) {
                var_r3_354 += 7;
            }
            temp_r5_360 = var_r3_354 / 8;
            temp_r3_362 = M2C_FIELD(var_r7_347, s32 *, 0x18);
            if (temp_r3_362 != -1) {
                if ((u32) temp_r3_362 <= 3U) {
                    var_r3_534 = temp_r5_360;
                    var_r1_535 = (u8 *)temp_r1_17 + 0x844;
                    var_r2_536 = temp_r6_355;
                    sp10(sp1C, var_r1_535, var_r2_536, var_r3_534, 24, 24);
                } else if (temp_r3_362 <= 7) {
                    var_r3_534 = temp_r5_360 - 9;
                    var_r2_536 = temp_r6_355 - 9;
                    var_r1_535 = (u8 *)temp_r1_17 + 0xA84;
                    sp10(sp1C, var_r1_535, var_r2_536, var_r3_534, 42, 42);
                }
                if (M2C_FIELD(var_r7_347, s32 *, 0x18) <= 0xE) {
                    M2C_FIELD(var_r7_347, s32 *, 0x18) = M2C_FIELD(var_r7_347, s32 *, 0x18) + 1;
                }
            } else {
                sp10(sp1C, (u8 *)temp_r1_17 + 0x604, temp_r6_355, temp_r5_360, 0x18, 0x18);
                if (M2C_FIELD(var_r7_347, s32 *, 4) <= 0x27F) {
                    if (M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 4) == 0) {
                        var_r3_390 = M2C_FIELD(var_r7_347, s32 *, 0) - 0x20;
                    } else {
                        var_r3_390 = M2C_FIELD(var_r7_347, s32 *, 0) + 0x20;
                    }
                    M2C_FIELD(var_r7_347, s32 *, 0) = var_r3_390;
                    M2C_FIELD(var_r7_347, s32 *, 4) = (s32) (M2C_FIELD(var_r7_347, s32 *, 4) + 0x40);
                } else {
                    M2C_FIELD(var_r7_347, s32 *, 0x18) = 0;
                    var_r4_412 = 0;
                    if (Data_080ededc[(M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4) + 1] != 0) {
                        spC = temp_r6_355 + 0xC;
                        do {
                            temp_r3_436 = ((var_r8_340 * Data_080ededc[(M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4) + 1]) + var_r4_412) * 0x1C;
                            temp_r6_437 = (u8 *)temp_r3_436 + 0x02010140;
                            M2C_FIELD((void *)0x02010140, s32 *, temp_r3_436) = (s32) (spC << 0x10);
                            M2C_FIELD(temp_r6_437, s32 *, 4) = (s32) (temp_r5_360 << 0x10);
                            sp8 = var_r4_412;
                            M2C_FIELD(temp_r6_437, s32 *, 0xC) = (s32) ((Func_08004458() - 0x80) << 9);
                            if (M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) == 2) {
                                M2C_FIELD(temp_r6_437, s32 *, 0x10) = (s32) (((0x1FF & Func_08004458()) + 0xFFFFFE80) << 0xA);
                            } else {
                                sp8 = var_r4_412;
                                M2C_FIELD(temp_r6_437, s32 *, 0x10) = (s32) ((Func_08004458() - 0xFF) << 0xA);
                            }
                            M2C_FIELD(temp_r6_437, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 0x10);
                            var_r4_412 = sp8 + 1;
                        } while (var_r4_412 != Data_080ededc[(M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4) + 1]);
                    }
                    if (!(3 & var_r8_340)) {
                        Func_080f9010(0x84);
                    }
                    var_r4_503 = 0;
                    if (M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x14) != 0) {
                        var_r6_507 = 0x24;
                        do {
                            sp8 = var_r4_503;
                            Func_080d6888((s32) M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s16 *, var_r6_507), 7, 5, var_r4_503, 2);
                            var_r4_503 += 1;
                            var_r6_507 += 2;
                        } while (var_r4_503 != M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x14));
                    }
                }
            }
            var_r8_340 += 1;
            var_r7_347 = (u8 *)var_r7_347 + 0x1C;
            if (var_r8_340 != Data_080ededc[M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4]) {
                goto loop_38;
            }
        }
        var_r5_577 = (void *)0x02010140;
        var_r8_578 = 0;
        do {
            temp_r0_581 = M2C_FIELD(var_r5_577, s32 *, 0x18);
            if (temp_r0_581 != -1) {
                var_r4_585 = temp_r0_581 + 1;
                if (var_r4_585 > 6) {
                    var_r4_585 = 6;
                }
                temp_r0_590 = var_r4_585 * 2;
                temp_r6_437 = (void *) ((const u8 *)Data_080ede5c + temp_r0_590 - 2);
                ((DrawRectangleFn) sp14)(sp1C, *(const u16 *)temp_r6_437 + (u8 *)temp_r1_17, M2C_FIELD(var_r5_577, s16 *, 2) - var_r4_585, M2C_FIELD(var_r5_577, s16 *, 6) - var_r4_585, temp_r0_590, temp_r0_590);
                Func_080e3908(var_r5_577, 0x3C, 0x2000);
                M2C_FIELD(var_r5_577, s32 *, 0x18) = (s32) (M2C_FIELD(var_r5_577, s32 *, 0x18) - 1);
            }
            var_r8_578 += 1;
            var_r5_577 = (u8 *)var_r5_577 + 0x1C;
        } while (var_r8_578 != 0x200);
        Func_080cd52c();
        M2C_FIELD(temp_r1_17, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        temp_r1_629 = sp18 + 1;
        sp18 = temp_r1_629;
        if (temp_r1_629 != Data_080ededc[(M2C_FIELD(M2C_FIELD(temp_r1_17, void **, 0x7828), s32 *, 0x18) * 4) + 3]) {
            goto loop_33;
        }
    }
    Func_08004278((void *)0x080C91A5);
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
    Func_080c9048();
}
