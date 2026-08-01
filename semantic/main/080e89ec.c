typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. This file is
 * still a raw m2c draft; the veneer pass below is the only thing corrected.
 *
 * All eight `bl 0x080072f4` sites are __call_via_r4, so each is an indirect
 * call through r4 rather than a call to a function at 0x080072f4.
 *
 * THE RENDERER PAIR ARRIVES AS AN OUT-PARAMETER. At 0x080e8ac2 r1 = sp + 48,
 * parked in [sp, #16], and `Func_080cef64(alternate, sp + 48)` at 0x080e8aca
 * fills sp+48 with allocator slot 46 and sp+52 with slot 47. The `alternate`
 * argument here is runtime state -- it is read from a scene record -- but that
 * does NOT make the callee runtime-dependent: the byte-exact src/080cef64.c
 * takes the same two slots in BOTH of its branches, and `alternate` only
 * changes the arguments it passes to Func_080ed408. Worth stating, because a
 * runtime value feeding the setup call is exactly the shape that looks like it
 * should defeat resolution and does not.
 *
 * WHY THE RESOLVER CALLED ALL EIGHT "memory". Six read `ldr r4, [sp, #48]`
 * and two read `ldr r4, [rN, #4]` where rN was just loaded from [sp, #16].
 * The second form is renderers[1]; the tool cannot know rN is a table base, so
 * it refuses and reports an indirect memory load. **A `[rN, #4]` load whose rN
 * is the parked table base is entry 1.** Resolve rN first.
 *
 * The two entry-1 sites, pinned by ARGUMENT AGREEMENT and not by order:
 *
 * - 0x080e8cca, r4 = [r2, #4] with r2 = [sp, #16]. r2 = 0x38 at 0x080e8cc8.
 *   That is the `0x38` statement, whose neighbour at 0x080e8ca4 is otherwise
 *   identical but sets r2 = 0x30 and reads r4 straight from [sp, #48].
 * - 0x080e9018, r4 = [r0, #4] with r0 = [sp, #16]. y is `[r6, #4]` plus a
 *   0x080EDEAB byte-table read and height is a 0x080EDEA5 read: the
 *   `var_r6_7` statement.
 *
 * TWO ADJACENT CALLS, DIFFERENT RENDERERS. The 0x30 and 0x38 statements read
 * as the same call twice with one changed coordinate. They are entry 0 and
 * entry 1. The draft could not see it, because the callee load looks dead.
 *
 * The remaining six sites all read `ldr r4, [sp, #48]` -- entry 0 -- and the
 * six remaining C statements account for them exactly.
 *
 * ARITY: six at every site. r4 is above the argument registers, so no
 * argument slot holds the callee.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents. This file
 * resolved because its table lives in the caller's own frame; the memory sites
 * still open in this audit read a callee out of a heap record whose contents
 * depend on what ran before, and those SHOULD end as written uncertainties
 * rather than names. A page of bounded uncertainties is this job going right,
 * not a reconstruction giving up.
 */
typedef void (*Renderer_080e89ec)(
    s32 target, void *source, s32 x, s32 y, u32 width, s32 height);

void Func_08002dd8(s32);
void Func_080b5028(s32, s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080d6888(s16, s32, s32, s32, s32);

/*
 * Run a staged battle transition with scanline darkening and particle arcs.
 *
 * The scene builds two moving particle banks, animates a twenty-entry trail,
 * then releases a larger projected burst while updating character and camera
 * state through the final cleanup frame.
 */
void Func_080e89ec(void *arg0) {
    u32 sp8;
    void **spC;
    u32 *sp10;
    void *sp14;
    s32 sp18;
    u32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;
    s32 sp38[3];
    s32 sp44[40];
    Renderer_080e89ec renderers[2];
    s16 var_r3_5;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r3_2;
    s32 temp_r3_4;
    s32 temp_r3_5;
    s32 temp_r3_7;
    s32 temp_r3_8;
    s32 temp_r4_2;
    s32 temp_r5;
    s32 var_r0;
    s32 var_r2_2;
    s32 var_r3;
    s32 var_r3_2;
    s32 var_r3_3;
    s32 var_r3_4;
    s32 var_r3_6;
    s32 var_r3_7;
    s32 var_r3_8;
    s32 var_r6;
    s32 var_r6_2;
    s32 var_r6_3;
    s32 var_r6_4;
    s32 var_r6_5;
    s32 var_r7;
    s32 var_r7_2;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r7_5;
    s8 *var_r1;
    u32 *temp_r1_2;
    u32 temp_r3_3;
    u32 temp_r3_6;
    u32 temp_r3_9;
    u32 temp_r4;
    u8 *var_r2;
    u8 temp_r3;
    u8 temp_r5_2;
    void *temp_fp;
    void *temp_r1;
    void *var_r4;
    void *var_r5;
    void *var_r5_2;
    void *var_r5_3;
    void *var_r6_6;
    void *var_r6_7;

    temp_r1 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp2C = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp24 = 0;
    sp1C = 0;
    sp18 = 0;
    sp14 = M2C_FIELD((void *)0x03001EEC, void **, 8);
    M2C_FIELD(temp_r1, void **, 0x7828) = arg0;
    Func_080cd594(0);
    *(s16 *)0x04000052 = 0x1010;
    Func_080e0524((void *)0xC2, temp_r1, 1, 1);
    var_r7 = 1;
    var_r4 = temp_r1 + 0x3A8;
    do {
        var_r6 = 0;
        var_r1 = var_r4 + 0xFFFFF1F0;
        var_r2 = temp_r1 + 0x1680;
loop_2:
        temp_r3 = *var_r2;
        var_r2 += 1;
        if (var_r7 > 0xA) {
            var_r3 = (temp_r3 - (var_r7 * 4)) + 0x28;
            if (var_r3 < 0) {
                var_r3 = 0;
            }
            *var_r1 = (s8) var_r3;
        }
        var_r6 += 1;
        var_r1 += 1;
        if (var_r6 != 0x3A8) {
            goto loop_2;
        }
        var_r7 += 1;
        var_r4 += 0x3A8;
    } while (var_r7 != 0x14);
    Func_080e0524((void *)0x73, sp14, 0, 0);
    Func_080e0524((void *)0xB4, temp_r1 + 0x3C00, 1, 1);
    Func_080e0524((void *)0x7D, (void *)0x02010000, 1, 0);
    temp_r1_2 = (u32 *)renderers;
    sp10 = temp_r1_2;
    Func_080cef64(M2C_FIELD(M2C_FIELD(temp_r1, void **, 0x7828), s32 *, 4), temp_r1_2);
    M2C_FIELD(temp_r1, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r1, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    var_r6_2 = 0;
    var_r5 = temp_r1 + 0x7160;
    do {
        if (M2C_FIELD(M2C_FIELD(temp_r1, void **, 0x7828), s32 *, 4) == 0) {
            var_r3_2 = 0xFFC80000;
        } else {
            var_r3_2 = 0x380000;
        }
        M2C_FIELD(var_r5, s32 *, 0) = var_r3_2;
        M2C_FIELD(var_r5, s32 *, 4) = 0;
        M2C_FIELD(var_r5, s32 *, 8) = 0;
        M2C_FIELD(var_r5, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5, s32 *, 0x10) = (s32) ((Func_08004458() & 0x3F) << 0xD);
        var_r6_2 += 1;
        M2C_FIELD(var_r5, s32 *, 0x14) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5, s32 *, 0x18) = 1;
        var_r5 += 0x1C;
    } while (var_r6_2 != 0x28);
    var_r6_3 = 0;
    var_r5_2 = temp_r1 + 0x75C0;
    do {
        if (M2C_FIELD(M2C_FIELD(temp_r1, void **, 0x7828), s32 *, 4) == 0) {
            var_r3_3 = 0xFFC80000;
        } else {
            var_r3_3 = 0x380000;
        }
        M2C_FIELD(var_r5_2, s32 *, 0) = var_r3_3;
        M2C_FIELD(var_r5_2, s32 *, 4) = 0x140000;
        M2C_FIELD(var_r5_2, s32 *, 8) = 0;
        M2C_FIELD(var_r5_2, s32 *, 0xC) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5_2, s32 *, 0x10) = (s32) ((Func_08004458() & 0x3F) << 0xC);
        var_r6_3 += 1;
        M2C_FIELD(var_r5_2, s32 *, 0x14) = (s32) (((Func_08004458() & 0x3F) - 0x20) << 0xE);
        M2C_FIELD(var_r5_2, s32 *, 0x18) = 0;
        var_r5_2 += 0x1C;
    } while (var_r6_3 != 0x10);
    var_r6_4 = -0x4000;
    var_r7_2 = 0;
    var_r5_3 = temp_r1 + 0x7080;
    do {
        if (M2C_FIELD(M2C_FIELD(temp_r1, void **, 0x7828), s32 *, 4) == 1) {
            var_r3_4 = ((s32) (Func_08002322(var_r6_4) * 0x18) >> 0x10) + 0x58;
        } else {
            var_r3_4 = ((s32) (0 - (Func_08002322(var_r6_4) * 0x18)) >> 0x10) + 0x10;
        }
        M2C_FIELD(var_r5_3, s32 *, 0) = var_r3_4;
        M2C_FIELD(var_r5_3, s32 *, 4) = (s32) (((s32) (Func_0800231c(var_r6_4) * 0x10) >> 0x10) + 0x28);
        temp_r3_2 = 0 - (var_r7_2 * 2);
        var_r7_2 += 1;
        M2C_FIELD(var_r5_3, s32 *, 0x18) = temp_r3_2;
        var_r6_4 += 0x1000;
        var_r5_3 += 0x1C;
    } while (var_r7_2 != 8);
    sp20 = Func_08002f40(0xD3);
    sp28 = 0;
    spC = temp_r1 + 0x7828;
loop_24:
    temp_fp = *(void **)0x03001E80;
    if (sp28 == 0x53) {
        Func_080b50e8(0x86);
    }
    if (sp28 == 0) {
        Func_080f9010(0x88);
    }
    if (sp28 == 0x32) {
        Func_080f9010(0x88);
    }
    if (M2C_FIELD(*spC, s32 *, 4) == 0) {
        if (sp28 <= 0x3F) {
            var_r3_5 = M2C_FIELD(temp_fp, u16 *, 0x36) + 0xFFFFFF00;
            goto block_35;
        }
    } else if (sp28 <= 0x3F) {
        var_r3_5 = M2C_FIELD(temp_fp, u16 *, 0x36) + 0x100;
block_35:
        M2C_FIELD(temp_fp, s16 *, 0x36) = var_r3_5;
    }
    Func_080b5028(0, 0, 0, 0x64);
    if (sp28 <= 0x11) {
        temp_r0 = Func_080022ec(sp28, 3);
        temp_r0_2 = temp_r0 * 2;
        renderers[0](sp2C, M2C_FIELD(temp_r0_2, u16 *, 0x080EDEB2) + temp_r1 + 0x3C00, 0x30, M2C_FIELD(temp_r0, u8 *, 0x080EDEAB) + 0x3C, (u32) M2C_FIELD(temp_r0, u8 *, 0x080EDE9F), (s32) M2C_FIELD(temp_r0, u8 *, 0x080EDEA5));
        renderers[1](sp2C, M2C_FIELD(temp_r0_2, u16 *, 0x080EDEB2) + temp_r1 + 0x3C00, 0x38, M2C_FIELD(temp_r0, u8 *, 0x080EDEAB) + 0x3C, (u32) M2C_FIELD(temp_r0, u8 *, 0x080EDE9F), (s32) M2C_FIELD(temp_r0, u8 *, 0x080EDEA5));
    }
    temp_r3_3 = sp28 - 0x12;
    sp8 = temp_r3_3;
    if (temp_r3_3 <= 0x28U) {
        if (sp28 == 0x12) {
            sp1C = (M2C_FIELD(sp20, s8 *, 0) << 8) + M2C_FIELD(sp20, u8 *, 1);
            sp18 = (M2C_FIELD(sp20, s8 *, 2) << 8) + M2C_FIELD(sp20, u8 *, 3) + 0x10;
            sp20 += 4;
        } else {
            sp1C += M2C_FIELD(sp20, s8 *, 0);
            sp18 += (s8) M2C_FIELD(sp20, u8 *, 1);
            sp20 += 2;
        }
    }
    if ((u32) (sp28 - 0x4E) <= 0x28U) {
        if (sp28 == 0x4E) {
            sp1C = -0x38U;
            sp18 = 0x30;
        } else {
            sp18 -= 0x10;
        }
    }
    var_r6_5 = 0x13;
    var_r7_3 = 0x9C;
    do {
        temp_r3_4 = var_r6_5 + 0x12;
        if ((sp28 > temp_r3_4) && (sp28 <= (s32) (temp_r3_4 + 0x41))) {
            temp_r0_3 = var_r6_5 * 8;
            M2C_FIELD(sp44, s32 *, temp_r0_3) = M2C_FIELD(sp44, s32 *, temp_r0_3 - 8);
            temp_r5 = M2C_FIELD(sp44, s32 *, temp_r0_3 - 4);
            M2C_FIELD(sp44, s32 *, var_r7_3) = temp_r5;
            if (var_r6_5 > 0xA) {
                renderers[0](sp2C, (var_r6_5 * 0x3A8) + temp_r1 + 0xFFFFF1F0, M2C_FIELD(sp44, s32 *, temp_r0_3), temp_r5, 0x18U, 0x27);
            } else {
                renderers[0](sp2C, temp_r1 + 0x1680, M2C_FIELD(sp44, s32 *, temp_r0_3), temp_r5, 0x18U, 0x27);
            }
        }
        var_r6_5 -= 1;
        var_r7_3 -= 8;
    } while (var_r6_5 != 0);
    Func_080049ac();
    Func_080051d8((s32) temp_fp, (s32) (temp_fp + 0xC));
    if (sp8 <= 0x41U) {
        if (M2C_FIELD(*spC, s32 *, 4) == 1) {
            var_r3_6 = 0x40 - ((s32) (sp1C + (sp1C >> 0x1F)) >> 1);
        } else {
            var_r3_6 = ((s32) (sp1C + (sp1C >> 0x1F)) >> 1) + 0x40;
        }
        M2C_FIELD(sp38, s32 *, 0) = var_r3_6;
        temp_r3_5 = 0x3C - sp18;
        M2C_FIELD(sp38, s32 *, 4) = temp_r3_5;
        temp_r3_6 = (temp_r3_5 - M2C_FIELD(sp44, s32 *, 4)) - 0x18;
        var_r0 = (s32) (temp_r3_6 + (temp_r3_6 >> 0x1F)) >> 1;
        if (var_r0 > 2) {
            var_r0 = 2;
        }
        if (var_r0 < -2) {
            var_r0 = -2;
        }
        temp_r2 = sp24 + var_r0;
        sp24 = temp_r2;
        if (temp_r2 > 8) {
            sp24 = 8;
        }
        if (sp24 < -8) {
            sp24 = -8;
        }
        var_r3_7 = sp24;
        if (var_r3_7 < 0) {
            var_r3_7 += 3;
        }
        temp_r2_2 = M2C_FIELD(sp38, s32 *, 0);
        M2C_FIELD(sp44, s32 *, 0) = (s32) (temp_r2_2 - 0xC);
        temp_r3_7 = M2C_FIELD(sp38, s32 *, 4);
        M2C_FIELD(sp44, s32 *, 4) = (s32) (temp_r3_7 - 0x14);
        renderers[0](sp2C, (((var_r3_7 >> 2) + 2) * 0x480) + temp_r1, temp_r2_2 - 0x12, temp_r3_7 - 0x16, 0x18U, 0x30);
    }
    if (sp28 == 0x53) {
        M2C_FIELD(temp_r1, s32 *, 0x77A8) = 8;
        Func_080d6888(M2C_FIELD(*spC, s16 *, 0x24), 7, 5, 0, 8);
        Func_080b5088(M2C_FIELD(*spC, s16 *, 0x24), 1);
    }
    if (sp28 > 0x53) {
        var_r7_4 = 0;
        var_r6_6 = temp_r1 + 0x7160;
        do {
            if ((s32) M2C_FIELD(var_r6_6, s32 *, 4) >= 0) {
                Func_080e3944((s32) var_r6_6, sp38);
                M2C_FIELD(sp38, s32 *, 0) = (s32) ((s32) M2C_FIELD(sp38, s32 *, 0) >> 1);
                var_r3_8 = M2C_FIELD(sp38, s32 *, 8);
                if (var_r3_8 <= 0x9F) {
                    var_r3_8 = 0xA0;
                    M2C_FIELD(sp38, s32 *, 8) = 0xA0;
                }
                if (var_r3_8 > 0x31F) {
                    M2C_FIELD(sp38, s32 *, 8) = 0x31F;
                    var_r3_8 = 0x31F;
                }
                var_r2_2 = var_r3_8 - 0xA0;
                if (var_r2_2 < 0) {
                    var_r2_2 += 0x3F;
                }
                temp_r4 = 9 - (var_r2_2 >> 6);
                if (var_r7_4 > 0x2F) {
                    temp_r3_8 = M2C_FIELD(var_r6_6, s32 *, 0x18);
                    if (temp_r3_8 <= 0xB) {
                        renderers[0](sp2C, (void *)((((s32) (temp_r3_8 + ((u32) temp_r3_8 >> 0x1F)) >> 1) << 0xB) + 0x02010000), M2C_FIELD(sp38, s32 *, 0) - 0x10, M2C_FIELD(sp38, s32 *, 4) - 0x20, 0x20U, 0x40);
                        M2C_FIELD(var_r6_6, s32 *, 0x18) = (s32) (M2C_FIELD(var_r6_6, s32 *, 0x18) + 1);
                    }
                } else {
                    temp_r0_4 = temp_r4 * 2;
                    renderers[0](sp2C, sp14 + M2C_FIELD((temp_r0_4 - 2), u16 *, 0x080EDE48), M2C_FIELD(sp38, s32 *, 0) - ((s32) (temp_r4 + (temp_r4 >> 0x1F)) >> 1), M2C_FIELD(sp38, s32 *, 4) - temp_r4, temp_r4, temp_r0_4);
                }
                temp_r1_3 = M2C_FIELD(var_r6_6, s32 *, 0x10);
                M2C_FIELD(var_r6_6, s32 *, 0) = (s32) (M2C_FIELD(var_r6_6, s32 *, 0) + M2C_FIELD(var_r6_6, s32 *, 0xC));
                M2C_FIELD(var_r6_6, s32 *, 4) = (s32) (M2C_FIELD(var_r6_6, s32 *, 4) + temp_r1_3);
                M2C_FIELD(var_r6_6, s32 *, 8) = (s32) (M2C_FIELD(var_r6_6, s32 *, 8) + M2C_FIELD(var_r6_6, s32 *, 0x14));
                M2C_FIELD(var_r6_6, s32 *, 0x10) = (s32) (temp_r1_3 + 0xFFFFE000);
            }
            var_r7_4 += 1;
            var_r6_6 += 0x1C;
        } while (var_r7_4 != 0x38);
    }
    if (sp28 == 0x32) {
        M2C_FIELD(temp_r1, s32 *, 0x77A8) = 0xC;
        Func_080d6888(M2C_FIELD(*spC, s16 *, 0x24), 7, 5, 0, 8);
    }
    if (sp28 > 0x31) {
        var_r7_5 = 0;
        var_r6_7 = temp_r1 + 0x7080;
        do {
            temp_r3_9 = M2C_FIELD(var_r6_7, u32 *, 0x18);
            if (temp_r3_9 <= 0xBU) {
                temp_r4_2 = (s32) (temp_r3_9 + (temp_r3_9 >> 0x1F)) >> 1;
                temp_r5_2 = M2C_FIELD(temp_r4_2, u8 *, 0x080EDE9F);
                renderers[1](sp2C, M2C_FIELD((temp_r4_2 * 2), u16 *, 0x080EDEB2) + temp_r1 + 0x3C00, M2C_FIELD(var_r6_7, s32 *, 0) - (temp_r5_2 >> 1), M2C_FIELD(var_r6_7, s32 *, 4) + M2C_FIELD(temp_r4_2, u8 *, 0x080EDEAB), (u32) temp_r5_2, (s32) M2C_FIELD(temp_r4_2, u8 *, 0x080EDEA5));
            }
            var_r7_5 += 1;
            M2C_FIELD(var_r6_7, u32 *, 0x18) = (u32) (M2C_FIELD(var_r6_7, u32 *, 0x18) + 1);
            var_r6_7 += 0x1C;
        } while (var_r7_5 != 8);
    }
    Func_080e155c(8, 8U);
    Func_080cd52c();
    M2C_FIELD(temp_r1, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    temp_r1_4 = sp28 + 1;
    sp28 = temp_r1_4;
    if (temp_r1_4 != 0x96) {
        goto loop_24;
    }
    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}
