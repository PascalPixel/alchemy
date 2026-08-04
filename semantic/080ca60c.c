#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec();
s32 Func_080022fc();
s32 Func_0800231c();
s32 Func_08002322();
void *Func_08002f40();
u32 Func_08004458();
void **Func_080b5098();

void Func_08002dd8(s32);
void Func_080030f8();
void Func_080041d8();
void Func_08004278();
void Func_080049ac();
void Func_080051d8();
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c();
void Func_080cd594(s32);
s32 Func_080cdbc0();
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524();
void Func_080e155c();
void Func_080e38b8();
void Func_080e3908();
void Func_080e3944();
void Func_080e396c();
void Func_080e3980();
void Func_080ed408();
void Func_080f9010();

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Nine `bl`
 * sites in this function land inside the 0x080072e4 bank; none is a call to a
 * function at the branch target, and each resolves through the register the
 * veneer entry branches on.
 *
 * 0x080ca718 -- `bl 0x080072f0` = __call_via_r3, r3 from this function's pool
 * = 0x03001388, the IWRAM-relocated word copy (destination, source, size).
 * r3 is an ARGUMENT register, so the draft's fourth argument WAS the callee;
 * the call takes three.
 *
 * The other eight are `bl 0x080072f4` = __call_via_r4. r4 is above r0-r3, so
 * no argument slot hides the callee and all six arguments the draft passes at
 * each of them are real.
 *
 * THIS FUNCTION USES BOTH RENDERER DISCIPLINES, AND THE DIFFERENCE IS THE
 * POINT. Slots 46 (0x03001f08) and 47 (0x03001f0c) are Func_080048b0's
 * allocator slots, published by Func_080ed408(id, ...) and released by
 * Func_08002dd8(id). Five sites RE-READ the slot at the point of use; three
 * go through a two-entry table parked in this frame. Reading the ROM:
 *
 *   Slot 46 is published FIVE times with DIFFERENT parameters -- 0x080ca9ca
 *   (7,7,7,2), 0x080caa70 (7,7,3,2), 0x080cac2c (7,7,3,table byte),
 *   0x080cac7c (7,7,7,a different table byte), 0x080caeb4 (7,7,3,2) -- and
 *   four sites re-read 0x03001f08 between two of them: 0x080caa02 after
 *   0x080ca9ca, 0x080caaa6 after 0x080caa70, 0x080cac62 after 0x080cac2c,
 *   0x080cac98 after 0x080cac7c. Each is publish, read, call, release.
 *
 *   Slot 47 is published at 0x080cae66 and again at 0x080caec8, and the site
 *   at 0x080cae88 falls between them -- inside the three-iteration loop that
 *   closes at 0x080cae9a, so the publish/read/call/release runs once per
 *   iteration with a fresh randomised parameter each time.
 *
 * SO THESE FIVE MUST NOT BE HOISTED. A `renderers[2]` local filled once at
 * the top would compile and would look exactly like the files where caching
 * is correct, and it would freeze a pointer the ROM refreshes on purpose.
 * The read is reproduced where the ROM does it.
 *
 * THE OTHER THREE ARE CACHED, AND THE ROM CACHES THEM. At 0x080ca962-0x080ca970
 * `mov r3, sp; adds r3, #100; str r3, [sp, #24]` parks the ADDRESS sp + 100.
 * After the last publish pair, 0x080caebe stores slot 46 into [sp, #100] and
 * 0x080caed2 stores slot 47 into [r2, #4] with r2 = sp + 100. That is a
 * two-element local array, and the three remaining sites index it:
 * 0x080caff4 (`[r0, #4]`) and 0x080cb0fe (`[r5, #4]`) are entry 1, and
 * 0x080caf42 is `ldr r4, [r4, r0]` with r0 the table base and r4 = (r8 & 1)
 * << 2 -- the particle counter's parity. No branch inside
 * 0x080caecc..0x080cb1a4 targets an address at or before 0x080caecc, checked
 * exhaustively, so no republication can reach those three sites.
 *
 * STRUCTURE CORRECTED at 0x080caf42. The draft called one renderer there;
 * the ROM alternates between the two on `var_r8_11 & 1`. The callee was the
 * only thing distinguishing the iterations and the draft could not see it.
 *
 * DEFECT CORRECTED in the draft's park. m2c wrote the slot-47 park as
 * `sp18[1]`, reading `str r3, [r2, #4]` as an offset from sp + 24 when r2
 * holds the CONTENTS of [sp, #24], which is sp + 100. The word written is
 * sp + 104, entry 1 of the table -- which is why the three cached sites read
 * back exactly what this store wrote.
 *
 * UNCERTAINTY, left standing: this settles which pointer each site calls and
 * when it is read. It settles nothing about what slots 46 and 47 CONTAIN --
 * the slot table unifies the addressing, never the contents.
 */
typedef void *(*WordCopy_080ca60c)(void *destination, const void *source,
                                   s32 size);
typedef void (*Renderer_080ca60c)(s32 target, void *source, s32 x, s32 y,
                                  u32 width, s32 height);

/*
 * Run the selector-driven combat visual from resource setup through its final
 * presentation frame.  The first particle bank supplies orbiting/spark
 * sprites, the second bank supplies the moving streams, and the descriptor
 * byte table at 0x080edf04 selects their rendering and sound phases.
 *
 * This owns the complete 0x080ca60c..0x080cb1a4 assembly row.  The original
 * uses BL for two local control-flow edges; `cleanup` and `frame_loop` express
 * those edges without inventing callees.
 */
s32 Func_080ca60c(s32 arg0, s32 arg1) {
    s32 sp8;
    s32 spC;
    s32 sp10;
    s32 sp14;
    s32 sp18[2];
    void *sp1C;
    s32 sp20;
    void *sp24;
    void *sp28;
    s32 sp2C;
    void **sp30;
    s32 sp34;
    s32 sp38[2];
    void *sp3C;
    void *sp40;
    u32 sp44;
    s32 sp48;
    s32 sp4C;
    void *sp50;
    s32 sp54;
    s32 sp58;
    s32 sp5C;
    s32 sp60;
    Renderer_080ca60c renderers[2];
    s32 sp6C[2];
    s32 sp78[2];
    s32 *var_r5_3;
    s32 temp_r0;
    s32 temp_r0_3;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 temp_r2_3;
    s32 temp_r2_4;
    s32 temp_r2_5;
    s32 temp_r3_6;
    s32 temp_r5;
    s32 temp_r5_10;
    s32 temp_r5_11;
    s32 temp_r5_12;
    s32 temp_r5_13;
    s32 temp_r5_14;
    s32 temp_r5_5;
    s32 temp_r5_6;
    s32 temp_r6_2;
    s32 temp_r6_4;
    s32 temp_r7;
    s32 temp_r7_2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r1;
    s32 var_r8;
    s32 var_r8_10;
    s32 var_r8_11;
    s32 var_r8_12;
    s32 var_r8_13;
    s32 var_r8_2;
    s32 var_r8_3;
    s32 var_r8_4;
    s32 var_r8_5;
    s32 var_r8_6;
    s32 var_r8_7;
    s32 var_r8_8;
    s32 var_r8_9;
    u16 temp_r0_4;
    u32 temp_r3_2;
    u32 temp_r3_3;
    u32 temp_r5_15;
    u32 temp_r6_3;
    u32 var_r3;
    u32 var_sl;
    u8 temp_r3;
    u8 temp_r4;
    u8 temp_r4_2;
    u8 temp_r4_3;
    u8 temp_r5_3;
    u8 temp_r5_7;
    u8 temp_r5_8;
    void *temp_r0_2;
    void *temp_r1;
    void *temp_r2;
    void *temp_r2_2;
    void *temp_r3_4;
    void *temp_r3_5;
    void *temp_r5_2;
    void *temp_r5_4;
    void *temp_r5_9;
    void *temp_r6;
    void *temp_r9;
    void *temp_sl;
    void *var_r5;
    void *var_r5_2;
    void *var_r5_4;
    void *var_r5_5;
    void *var_r6;
    void *var_r7;

    sp60 = arg1;
    temp_r1 = M2C_FIELD((void *)0x03001EEC, void **, 0);
    sp5C = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    sp50 = M2C_FIELD((void *)0x03001EEC, void **, 8);
    sp4C = *(s32 *)0x03001E80;
    M2C_FIELD(temp_r1, s32 *, 0x7828) = arg0;
    if (sp60 == 8) {
        Func_080cd594(0);
    } else {
        Func_080cd594(1);
    }
    *(s16 *)0x04000052 = 0x1010;
    Func_080e0524((void *)0x73, sp50, 0, 0);
    Func_080e0524((void *)0xCE, temp_r1, 1, 0);
    sp48 = sp60 * 8;
    if (M2C_FIELD((sp60 * 7), u8 *, 0x080EDF04) == 0) {
        Func_080e0524((void *)0xC5, temp_r1 + 0xC80, 0, 0);
    } else {
        Func_080e0524((void *)0xC6, temp_r1 + 0xC80, 0, 0);
    }
    temp_r3 = M2C_FIELD(((sp48 - sp60) + 3), u8 *, 0x080EDF04);
    switch (temp_r3) {
    case 0:
        var_r0 = 0xCC;
        break;
    case 1:
        var_r0 = 0xA0;
        break;
    case 2:
        var_r0 = 0xA1;
        break;
    case 3:
        var_r0 = 0xB4;
        break;
    case 4:
        var_r0 = 0x8D;
        break;
    default:
        var_r0 = 0xC4;
        break;
    }
    ((WordCopy_080ca60c)0x03001388)((void *)0x05000000, Func_08002f40(var_r0), 0x80);
    Func_080e0524((void *)0x9E, temp_r1 + 0x3200, 1, 0);
    sp54 = 0;
    sp10 = 0;
    spC = 0x24;
loop_16:
    temp_sl = *Func_080b5098(
        *(s16 *)((u8 *)(u32)M2C_FIELD(temp_r1, s32 *, 0x7828) + spC));
    var_r8 = 0;
    var_r5 = temp_r1 + 0x7080;
    do {
        Func_080e3980(
            *(s16 *)((u8 *)(u32)M2C_FIELD(temp_r1, s32 *, 0x7828) + spC),
            &sp44);
        temp_r3_2 = sp44;
        M2C_FIELD(var_r5, s32 *, 0) = (s32) (((s32) (temp_r3_2 + (temp_r3_2 >> 0x1F)) >> 1) << 0x10);
        M2C_FIELD(var_r5, s32 *, 4) = 0x500000;
        M2C_FIELD(var_r5, s32 *, 8) = 0;
        M2C_FIELD(var_r5, s32 *, 0xC) = (s32) (((u8) Func_08004458() - 0x80) << 9);
        var_r8 += 1;
        M2C_FIELD(var_r5, s32 *, 0x10) = (s32) (((u8) Func_08004458() - 0x80) << 9);
        M2C_FIELD(var_r5, s32 *, 0x14) = 0;
        M2C_FIELD(var_r5, s32 *, 0x18) = -1;
        var_r5 += 0x1C;
    } while (var_r8 != 0x40);
    var_r8_2 = 0;
    var_r5_2 = (void *)(sp10 + 0x02010000);
    do {
        M2C_FIELD(var_r5_2, s32 *, 0) = (s32) M2C_FIELD(temp_sl, s32 *, 8);
        M2C_FIELD(var_r5_2, s32 *, 4) = 0x140000;
        M2C_FIELD(var_r5_2, s32 *, 8) = (s32) M2C_FIELD(temp_sl, s32 *, 0x10);
        M2C_FIELD(var_r5_2, s32 *, 0xC) = (s32) (((u8) Func_08004458() - 0x80) << 0xB);
        M2C_FIELD(var_r5_2, s32 *, 0x10) = (s32) (((u8) Func_08004458() - 0x80) << 0xB);
        temp_r0 = (u8) Func_08004458() - 0x80;
        var_r8_2 += 1;
        M2C_FIELD(var_r5_2, s32 *, 0x18) = -1;
        M2C_FIELD(var_r5_2, s32 *, 0x14) = (s32) (temp_r0 << 0xB);
        var_r5_2 += 0x1C;
    } while (var_r8_2 != 0x80);
    temp_r0_2 = (void *)(sp48 - sp60);
    sp40 = temp_r0_2;
    sp3C = temp_r0_2 + 2;
    var_r6 = (void *)0x02013800;
    var_r8_3 = 0;
    do {
        M2C_FIELD(var_r6, s32 *, 0) = (s32) M2C_FIELD(temp_sl, s32 *, 8);
        M2C_FIELD(var_r6, s32 *, 4) = 0x140000;
        M2C_FIELD(var_r6, s32 *, 8) = (s32) M2C_FIELD(temp_sl, s32 *, 0x10);
        if (M2C_FIELD(sp40, u8 *, 0x080EDF04) == 1) {
            M2C_FIELD(var_r6, s32 *, 0xC) = (s32) (((u8) Func_08004458() - 0x80) << 0xB);
            M2C_FIELD(var_r6, s32 *, 0x10) = (s32) (((u8) Func_08004458() - 0x80) << 0xB);
            M2C_FIELD(var_r6, s32 *, 0x14) = (s32) (((u8) Func_08004458() - 0x80) << 0xB);
        } else {
            if (0x10 & M2C_FIELD(sp3C, u8 *, 0x080EDF04)) {
                M2C_FIELD(var_r6, s32 *, 0xC) = (s32) ((Func_08004458() & 0x1FF) << 0xB);
                M2C_FIELD(var_r6, s32 *, 0x10) = (s32) (((Func_08004458() & 0x1FF) + 0xFFFFFF00) << 0xB);
                var_r0_2 = (Func_08004458() & 0x1FF) + 0xFFFFFF00;
            } else {
                M2C_FIELD(var_r6, s32 *, 0xC) = (s32) ((u8) Func_08004458() << 0xB);
                M2C_FIELD(var_r6, s32 *, 0x10) = (s32) (((u8) Func_08004458() - 0x80) << 0xB);
                var_r0_2 = (u8) Func_08004458() - 0x80;
            }
            M2C_FIELD(var_r6, s32 *, 0x14) = (s32) (var_r0_2 << 0xB);
            if ((s32) M2C_FIELD(var_r6, s32 *, 0) > 0) {
                M2C_FIELD(var_r6, s32 *, 0xC) = (s32) (0 - M2C_FIELD(var_r6, s32 *, 0xC));
            }
        }
        var_r8_3 += 1;
        M2C_FIELD(var_r6, s32 *, 0x18) = -1;
        var_r6 += 0x1C;
    } while (var_r8_3 != 0x200);
    temp_r5 = sp54 + 1;
    sp10 += 0xE00;
    spC += 2;
    sp54 = temp_r5;
    if (temp_r5 != 1) {
        goto loop_16;
    }
    M2C_FIELD(temp_r1, s32 *, 0x7780) = 2;
    M2C_FIELD(temp_r1, s32 *, 0x7784) = 0x4B;
    Func_080041d8(0x080CD261, 0x480);
    temp_r2 = sp38;
    Func_080e396c((s32) M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s16 *, 0x24), temp_r2);
    Func_080e3980((s32) M2C_FIELD(M2C_FIELD(temp_r1, s32 *, 0x7828), s16 *, 0x24), sp78);
    temp_r1_2 = M2C_FIELD(sp38, s32 *, 4);
    temp_r3_3 = M2C_FIELD(sp78, s32 *, 4) - temp_r1_2;
    M2C_FIELD(sp38, s32 *, 4) = (s32) (temp_r1_2 + ((s32) (temp_r3_3 + (temp_r3_3 >> 0x1F)) >> 1));
    sp58 = 0;
    var_r1 = sp48 - sp60;
    if (M2C_FIELD((var_r1 + 6), u8 *, 0x080EDF04) == 0)
        goto cleanup;
    sp14 = sp4C + 0xC;
    sp2C = var_r1;
    sp30 = temp_r1 + 0x7828;
    sp28 = (void *)(var_r1 + 4);
    sp24 = (void *)(var_r1 + 2);
frame_loop:
    /*
     * r2 is deliberately live across the backwards BL edge in the original:
     * both entries load the selector-times-eight value immediately before
     * reaching this block.
     */
    temp_r2_2 = (void *)(sp48 - sp60);
    temp_r4 = M2C_FIELD((temp_r2_2 + 4), u8 *, 0x080EDF04);
    temp_r1_3 = M2C_FIELD((temp_r2_2 + 1), u8 *, 0x080EDF04) * temp_r4;
    sp34 = 0;
    temp_r0_3 = temp_r1_3 * 4;
    if (M2C_FIELD(temp_r2_2, u8 *, 0x080EDF04) != 0) {
        if (sp58 >= (s32) (temp_r0_3 + 4)) {

        } else {
            if (sp58 < temp_r0_3) {
                var_r0_3 = Func_080022ec(sp58, (s32) temp_r4);
                if (var_r0_3 > 4) {
                    do {
                        var_r0_3 -= 4;
                    } while (var_r0_3 > 4);
                }
                sp20 = (s32) M2C_FIELD(var_r0_3, u8 *, 0x080EDF76);
            } else {
                sp20 = 3;
            }
            temp_r1_4 = sp48 - sp60;
            temp_r3_4 = (void *)(temp_r1_4 + 5);
            Func_080ed408(0x2E, 7, 7, 3, (s32) M2C_FIELD(temp_r3_4, u8 *, 0x080EDF04));
            temp_r5_2 = (sp20 * 0x360) + temp_r1 + 0xC80;
            (*(Renderer_080ca60c *)0x03001F08)(sp5C, temp_r5_2, ((s32) (sp38[0] + (sp38[0] >> 0x1F)) >> 1) - 0x12, 0x38, 0x12U, 0x30);
            Func_08002dd8(0x2E);
            Func_080ed408(0x2E, 7, 7, 7, (s32) M2C_FIELD(temp_r3_4, u8 *, 0x080EDF04));
            (*(Renderer_080ca60c *)0x03001F08)(sp5C, temp_r5_2, (s32) (sp38[0] + (sp38[0] >> 0x1F)) >> 1, 0x38, 0x12U, 0x30);
            Func_08002dd8(0x2E);
            temp_r6 = (void *)(temp_r1_4 + 4);
            temp_r5_3 = M2C_FIELD(temp_r6, u8 *, 0x080EDF04);
            if (Func_080022fc(sp58, temp_r5_3 * 4) != (temp_r5_3 * 3)) {

            } else {
                Func_080d6888(M2C_FIELD(*sp30, s16 *, 0x24), 7, 5, 0, 8);
                M2C_FIELD(temp_r1, s32 *, 0x77A8) = 4;
                if (sp58 > (s32) (((M2C_FIELD((temp_r1_4 + 1), u8 *, 0x080EDF04) * 4) - 4) * M2C_FIELD(temp_r6, u8 *, 0x080EDF04))) {
                    Func_080b50e8(0x85);
                } else {
                    Func_080f9010(0x85);
                }
                var_r8_4 = 0;
                do {
                    temp_r5_4 = (void *)(((
                        Func_080022ec(sp58, M2C_FIELD((sp2C + 4), u8 *, 0x080EDF04) * 6)
                        << 6) + var_r8_4) * 0x1C + 0x02013800);
                    M2C_FIELD(temp_r5_4, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 7);
                    var_r8_4 += 1;
                } while (var_r8_4 != 0x40);
                var_r8_5 = 0;
                temp_r5_5 = M2C_FIELD(sp28, u8 *, 0x080EDF04) * 6;
                do {
                    M2C_FIELD(((((Func_080022ec(sp58, temp_r5_5) * 0x10) + var_r8_5) * 0x1C) + temp_r1 + 0x7080), s32 *, 0x18) = 0;
                    var_r8_5 += 1;
                } while (var_r8_5 != 8);
                var_r8_6 = 0;
                temp_r5_6 = M2C_FIELD(sp28, u8 *, 0x080EDF04) * 6;
                do {
                    M2C_FIELD(((((Func_080022ec(sp58, temp_r5_6) * 0x10) + var_r8_6) * 0x1C) + 0x02010000), s32 *, 0x18) = 0;
                    var_r8_6 += 1;
                } while (var_r8_6 != 0x10);
            }
            if (sp20 == 3) {
                sp34 = 1;
            }
        }
    } else {
        if (sp58 >= (s32) (temp_r1_3 * 6)) {

        } else {
            temp_r6_2 = Func_080022fc(Func_080022ec(sp58, (s32) temp_r4), 6);
            if (M2C_FIELD(*sp30, s32 *, 4) == 1) {
                Func_080ed408(0x2E, 7, 7, 7, 2);
                temp_r5_7 = M2C_FIELD(temp_r6_2, u8 *, 0x080EDF58);
                temp_r4_2 = M2C_FIELD(temp_r6_2, u8 *, 0x080EDF5E);
                (*(Renderer_080ca60c *)0x03001F08)(sp5C, M2C_FIELD((temp_r6_2 * 2), u16 *, 0x080EDF64) + temp_r1, ((((s32) (sp38[0] + (sp38[0] >> 0x1F)) >> 1) - ((u8) M2C_FIELD(temp_r6_2, u8 *, 0x080EDF70) >> 1)) - temp_r5_7) + 8, sp38[1] - (temp_r4_2 >> 1), (u32) temp_r5_7, (s32) temp_r4_2);
            } else {
                Func_080ed408(0x2E, 7, 7, 3, 2);
                temp_r4_3 = M2C_FIELD(temp_r6_2, u8 *, 0x080EDF5E);
                (*(Renderer_080ca60c *)0x03001F08)(sp5C, M2C_FIELD((temp_r6_2 * 2), u16 *, 0x080EDF64) + temp_r1, (((s32) (sp38[0] + (sp38[0] >> 0x1F)) >> 1) + ((u8) M2C_FIELD(temp_r6_2, u8 *, 0x080EDF70) >> 1)) - 8, sp38[1] - (temp_r4_3 >> 1), (u32) M2C_FIELD(temp_r6_2, u8 *, 0x080EDF58), (s32) temp_r4_3);
            }
            Func_08002dd8(0x2E);
            temp_r5_8 = M2C_FIELD(sp28, u8 *, 0x080EDF04);
            if (Func_080022fc(sp58, temp_r5_8 * 6) != (temp_r5_8 * 4)) {

            } else {
                if (sp60 == 8) {
                    Func_080b50e8(0x86);
                } else {
                    Func_080f9010(0x85);
                    Func_080b50e8(0x85);
                }
                if (0x10 & M2C_FIELD(sp24, u8 *, 0x080EDF04)) {
                    M2C_FIELD(temp_r1, s32 *, 0x77A8) = 8;
                    Func_080d6888(M2C_FIELD(*sp30, s16 *, 0x24), 7, 5, 0, 0xC);
                    Func_080b5088(M2C_FIELD(*sp30, s16 *, 0x24), 4);
                    var_r5_3 = (s32 *)0x02013818;
                    var_r8_7 = 0;
                    do {
                        var_r8_7 += 1;
                        *var_r5_3 = (Func_08004458() & 0xF) + 0xF;
                        var_r5_3 += 0x1C;
                    } while (var_r8_7 != 0x200);
                } else {
                    M2C_FIELD(temp_r1, s32 *, 0x77A8) = 4;
                    Func_080d6888(M2C_FIELD(*sp30, s16 *, 0x24), 7, 5, 0, 8);
                    var_r8_8 = 0;
                    do {
                        temp_r5_9 = (void *)(((
                            Func_080022ec(sp58, M2C_FIELD((sp2C + 4), u8 *, 0x080EDF04) * 6)
                            << 5) + var_r8_8) * 0x1C + 0x02013800);
                        M2C_FIELD(temp_r5_9, s32 *, 0x18) = (s32) ((0xF & Func_08004458()) + 7);
                        var_r8_8 += 1;
                    } while (var_r8_8 != 0x20);
                }
                var_r8_9 = 0;
                temp_r5_10 = M2C_FIELD(sp28, u8 *, 0x080EDF04) * 6;
                do {
                    temp_r3_5 = (((Func_080022ec(sp58, temp_r5_10) * 0x10) + var_r8_9) * 0x1C) + temp_r1 + 0x7080;
                    var_r8_9 += 1;
                    M2C_FIELD(temp_r3_5, s32 *, 0x18) = 0;
                } while (var_r8_9 != 8);
            }
        }
        if ((u32) (sp58 - 0xC) > 0x13U) {

        } else {
            sp34 = 1;
        }
    }
    if ((4 & M2C_FIELD(sp24, u8 *, 0x080EDF04)) && (sp34 != 0)) {
        var_r8_10 = 0;
        temp_r7 = sp58 & 3;
        do {
            temp_r0_4 = (u16) Func_08004458();
            sp8 = (s32) temp_r0_4;
            temp_r5_11 = 0x1F & Func_08004458();
            temp_r6_3 = M2C_FIELD(sp38, u32 *, 0);
            temp_r5_12 = temp_r5_11 + 4;
            temp_r6_4 = (((s32) (temp_r6_3 + (temp_r6_3 >> 0x1F)) >> 1) + ((s32) (temp_r5_12 * Func_08002322((s32) temp_r0_4)) >> 0x11)) - ((u8) M2C_FIELD(temp_r7, u8 *, 0x080EDECA) >> 1);
            temp_r5_13 = (M2C_FIELD(sp38, s32 *, 4) - ((s32) (temp_r5_12 * Func_0800231c((s32) temp_r0_4)) >> 0x11)) - ((u8) M2C_FIELD(temp_r7, u8 *, 0x080EDED0) >> 1);
            Func_080ed408(0x2F, 7, 7, 3 | M2C_FIELD((Func_08004458() & 3), u8 *, 0x080EDF7B), 3);
            (*(Renderer_080ca60c *)0x03001F0C)(sp5C, M2C_FIELD((temp_r7 * 2), u16 *, 0x080EDEBE) + temp_r1, temp_r6_4, temp_r5_13, (u32) M2C_FIELD(temp_r7, u8 *, 0x080EDECA), (s32) M2C_FIELD(temp_r7, u8 *, 0x080EDED0));
            Func_08002dd8(0x2F);
            var_r8_10 += 1;
        } while (var_r8_10 != 3);
    }
    Func_080049ac();
    Func_080051d8(sp4C, sp14);
    Func_080ed408(0x2E, 7, 7, 3, 2);
    renderers[0] = *(Renderer_080ca60c *)0x03001F08;
    Func_080ed408(0x2F, 7, 7, 3, 3);
    renderers[1] = *(Renderer_080ca60c *)0x03001F0C;
    sp54 = 0;
    sp1C = (void *)(sp2C + 2);
loop_83:
    temp_r9 = *Func_080b5098(M2C_FIELD(*sp30, s32 *, 8));
    if (1 & M2C_FIELD(sp1C, u8 *, 0x080EDF04)) {
        var_r8_11 = 0;
        var_r5_4 = temp_r1 + 0x7080;
        do {
            var_r3 = M2C_FIELD(var_r5_4, u32 *, 0x18);
            if (var_r3 <= 0x17U) {
                if ((s32) var_r3 < 0) {
                    var_r3 += 3;
                }
                renderers[var_r8_11 & 1](sp5C, (((s32) var_r3 >> 2) * 0x480) + temp_r1 + 0x3200, M2C_FIELD(var_r5_4, s16 *, 2) - 0xC, M2C_FIELD(var_r5_4, s16 *, 6) - 0x18, 0x18U, 0x30);
                Func_080e3908(var_r5_4, 0x3E, -0x400);
                M2C_FIELD(var_r5_4, u32 *, 0x18) = (u32) (M2C_FIELD(var_r5_4, u32 *, 0x18) + 1);
            }
            var_r8_11 += 1;
            var_r5_4 += 0x1C;
        } while (var_r8_11 != 0x40);
    }
    if (2 & M2C_FIELD(sp1C, u8 *, 0x080EDF04)) {
        var_sl = 3;
        if (sp60 == 0xB) {
            var_sl = 8;
        }
        if (sp58 == 0x37) {
            Func_080d6888((s16) M2C_FIELD(*sp30, s32 *, 8), 7, -1, -1, 0);
        }
        if (sp58 == 0x5A) {
            Func_080d6888((s16) M2C_FIELD(*sp30, s32 *, 8), 0, -1, -1, 0);
        }
        var_r5_5 = (void *)0x02010000;
        var_r8_12 = 0;
        temp_r7_2 = var_sl * 2;
        do {
            if ((s32) M2C_FIELD(var_r5_5, s32 *, 0x18) >= 0) {
                Func_080e3944((s32) var_r5_5, sp6C);
                temp_r2_3 = (s32) M2C_FIELD(sp6C, s32 *, 0) >> 1;
                M2C_FIELD(sp6C, s32 *, 0) = temp_r2_3;
                renderers[1](sp5C, sp50 + M2C_FIELD((temp_r7_2 - 2), u16 *, 0x080EDE48), temp_r2_3 - (var_sl >> 1), M2C_FIELD(sp6C, s32 *, 4) - var_sl, var_sl, temp_r7_2);
                Func_080e38b8(var_r5_5, 0x3C, 0);
                temp_r3_6 = M2C_FIELD(var_r5_5, s32 *, 0x18) + 1;
                M2C_FIELD(var_r5_5, s32 *, 0x18) = temp_r3_6;
                if (temp_r3_6 > 0xA) {
                    temp_r0_5 = (s32) (M2C_FIELD(temp_r9, s32 *, 8) - M2C_FIELD(var_r5_5, s32 *, 0)) >> 8;
                    M2C_FIELD(var_r5_5, s32 *, 0xC) = (s32) (M2C_FIELD(var_r5_5, s32 *, 0xC) + temp_r0_5);
                    M2C_FIELD(var_r5_5, s32 *, 0x10) = (s32) (M2C_FIELD(var_r5_5, s32 *, 0x10) + ((s32) ((M2C_FIELD(temp_r9, s32 *, 0xC) - M2C_FIELD(var_r5_5, s32 *, 4)) + 0x140000) >> 8));
                    temp_r2_4 = (s32) (M2C_FIELD(temp_r9, s32 *, 0x10) - M2C_FIELD(var_r5_5, s32 *, 8)) >> 8;
                    M2C_FIELD(var_r5_5, s32 *, 0x14) = (s32) (M2C_FIELD(var_r5_5, s32 *, 0x14) + temp_r2_4);
                    if (((u32) (temp_r0_5 + 0xFFF) <= 0x1FFEU) && ((u32) (temp_r2_4 + 0xFFF) <= 0x1FFEU)) {
                        M2C_FIELD(var_r5_5, s32 *, 0x18) = -1;
                    }
                }
            }
            var_r8_12 += 1;
            var_r5_5 += 0x1C;
        } while (var_r8_12 != 0x40);
    }
    if (8 & M2C_FIELD(sp1C, u8 *, 0x080EDF04)) {
        var_r7 = (void *)0x02013800;
        var_r8_13 = 0;
        do {
            temp_r5_14 = M2C_FIELD(var_r7, s32 *, 0x18);
            if (temp_r5_14 >= 0) {
                Func_080e3944((s32) var_r7, sp6C);
                M2C_FIELD(sp6C, s32 *, 0) = (s32) ((s32) M2C_FIELD(sp6C, s32 *, 0) >> 1);
                Func_080e38b8(var_r7, 0x3C, 0);
                temp_r5_15 = (temp_r5_14 >> 3) + 1;
                temp_r0_6 = temp_r5_15 * 2;
                renderers[1](sp5C, sp50 + M2C_FIELD((temp_r0_6 - 2), u16 *, 0x080EDE48), M2C_FIELD(sp6C, s32 *, 0) - ((s32) (temp_r5_15 + (temp_r5_15 >> 0x1F)) >> 1), M2C_FIELD(sp6C, s32 *, 4) - temp_r5_15, temp_r5_15, temp_r0_6);
                M2C_FIELD(var_r7, s32 *, 0x18) = (s32) (M2C_FIELD(var_r7, s32 *, 0x18) - 1);
            }
            var_r8_13 += 1;
            var_r7 += 0x1C;
        } while (var_r8_13 != 0x80);
    }
    temp_r2_5 = sp54 + 1;
    sp54 = temp_r2_5;
    if (temp_r2_5 != 1) {
        goto loop_83;
    }
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    if (0x10 & M2C_FIELD(sp24, u8 *, 0x080EDF04)) {
        Func_080e155c(8, 8U);
    } else {
        Func_080e155c(2, 4U);
    }
    Func_080cd52c();
    M2C_FIELD(temp_r1, s32 *, 0x7824) = 1;
    Func_080030f8(1U);
    temp_r0_7 = sp58 + 1;
    sp58 = temp_r0_7;
    if (temp_r0_7 != M2C_FIELD(((sp48 - sp60) + 6), u8 *, 0x080EDF04)) {
        goto frame_loop;
    }
cleanup:
    Func_08004278((void *)0x080CD261);
    return Func_080cdbc0();
}
