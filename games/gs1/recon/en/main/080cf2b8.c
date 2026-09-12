#include "types.h"

#define BattleEffect_RunMemberBeam Func_080cf2b8

/*
 * Battle-presentation sub-effect at 0x080cf2b8, a two-argument entry
 * (effect object, variant) in the same family as the adopted
 * games/gs1/src/battle/effects/member_orbit/run.c (0x080ce85c) and
 * games/gs1/src/battle/presentation/prepare_scene.c (0x080ccaec).
 *
 * The owner republishes the effect object at work + 0x7828, asks
 * Func_080de2f8 for a pair of screen coordinates when state field 28 is 1,
 * loads three palettes into palette RAM through the IWRAM word-copy kernel
 * at 0x03001388 and their trailing 128-byte tile payloads into the kind-39
 * work block at +0, +0x2710 and +0x65c0, seeds 512 twenty-eight-byte
 * particles at 0x02010000, sets the BG2 affine scale and reference point,
 * then runs member_count * 16 + 116 frames.
 *
 * Each frame regenerates the kind-46/47 rectangle blitters through
 * Func_080ed408 (their block pointers are read back from the heap-block
 * table at 0x03001e50, entries 46 and 47 -- the same table whose entries 39
 * and 40 are the work block and the canvas read here through 0x03001eec),
 * optionally draws the 40x40 orbiting sprite, then walks the listed party
 * members twice: once to advance and draw up to eight particles per member,
 * and once to draw the member's growing bar plus a column of one-pixel
 * slices.
 *
 * Uncertain / left as raw offsets:
 *  - Data_080ee0a2 (8 bytes), Data_080ee0aa (3 halfwords), Data_080ee0b0 and
 *    Data_080ee0b3 (3 bytes each) are one contiguous unnamed table block at
 *    0x080ee0a2..0x080ee0b5; their roles here are "sprite cell index",
 *    "tile offset inside the work block", "width" and "height".
 *  - state fields 4, 20, 28 and the s16 member-id array at 36 follow the
 *    same kind-39 state layout member_orbit/run.c uses; no header owns it.
 *  - The second argument of BattleFx_FetchRectangleBlitters is the
 *    two-entry blitter array; only entry 1 is consumed by the first draw,
 *    entry 0 by the later ones.
 *  - The two identical 40x40 draws for frame <= 3 are the reference's own
 *    behaviour, not a transcription slip.
 *
 * Measured residual (1464 candidate bytes against 1492 reference bytes, 709
 * differing halfwords, independently re-measured): every reference branch,
 * loop, call target, call argument and store is represented block for block,
 * and the pool constants (0x03001eec, 0x03001e50, 0x03001e80, 0x03001388,
 * 0x02010000, 0x7824, 0x7828, 0x2710, 0x65c0, -0x10000, 0x080cd261 and the
 * four 0x080ee0xx table bases) are the reference's own.  Stack slot offsets
 * do NOT line up: the candidate frame is 112 bytes against the reference's
 * 116, so every sp-relative slot sits four bytes lower.  That, and nearly
 * the whole halfword difference, follows from one global-allocation tie
 * broken the other way: the
 * reference keeps `work` in fp for the entire body and spills `frame` to
 * sp+44, while this source makes the compiler keep `frame` in fp and spill
 * `work`.  Every `add rN, fp` / `ldr rN, [rBase, rIndex]` the reference uses
 * for a work-block field therefore reloads a base and adds it here, and the
 * frame is 112 bytes instead of 116 because one fewer pseudo is spilled.
 * Retyping `work` as `u8 *` (removing every cast on its offsets) was tried
 * and changed nothing: 1464 bytes, 709 differing halfwords, identical
 * topology.  Nothing evidenced in the reference distinguishes the two
 * lifetimes at source level, so the swap is left as the stated residual
 * rather than chased with contrived spellings.
 */
#define FIELD_AT_OFFSET(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_0000007b;
extern u8 Value_00000091;
extern u8 Value_00000093;
extern u8 Value_000000b1;

extern const u8 Data_080ee0a2[];
extern const u16 Data_080ee0aa[];
extern const u8 Data_080ee0b0[];
extern const u8 Data_080ee0b3[];

void Func_080cd594(s32 mode);
void Func_080de2f8(
    void *object, s32 variant, s32 flag, s32 unk, s32 *out_x, s32 *out_y);
void *Func_08002f40(s32 id);
u32 Func_08005340(const void *source, void *destination);
u32 Func_08004458(void);
void Func_080e396c(s32 member_id, s32 *out);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void BattleFx_FetchRectangleBlitters(s32 flag, void **pair);
void Runtime_ReleaseHeapBlock(s32 id);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void **Func_080b5098(s32 member_id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *record);
void Func_080e3944(void *source, s32 *screen);
s32 Func_080022ec(s32 value, s32 divisor);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080030f8(s32 frames);
void Scheduler_RemoveCallback(void *callback);
s32 Func_080cdbc0(void);

void BattleEffect_RunMemberBeam(void *object, s32 variant)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    void *state;
    u8 *blocks;
    s32 *particle;
    void *rectangle[2];
    void **rect;
    s32 record[3];
    s32 record2[3];
    s32 screen[3];
    s32 tmp[3];
    s32 out_x;
    s32 out_y;
    s32 y_offset;
    s32 facing;
    s32 facing12;
    s32 status;
    s32 flag;
    s32 id;
    s32 frame;
    s32 member;
    s32 member2;
    s32 bank;
    s32 id_ofs;
    s32 window2;
    s32 i;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    FIELD_AT_OFFSET(work, void **, 0x7828) = object;
    Func_080cd594(0);
    state = FIELD_AT_OFFSET(work, void **, 0x7828);
    if (FIELD_AT_OFFSET(state, s32 *, 28) == 1) {
        Func_080de2f8(object, variant,
            FIELD_AT_OFFSET(state, s32 *, 4)
                ^ FIELD_AT_OFFSET(state, s32 *, 28),
            0, &out_x, &out_y);
    }

    palette = Func_08002f40((s32)&Value_0000007b);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Func_08005340((u8 *)palette + 128, work);

    palette = Func_08002f40((s32)&Value_000000b1);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Func_08005340((u8 *)palette + 128, (u8 *)work + 0x2710);

    if (variant == 0) {
        id = (s32)&Value_00000093;
    } else {
        id = (s32)&Value_00000091;
    }
    palette = Func_08002f40(id);
    status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
    status = Func_08005340((u8 *)palette + 128, (u8 *)work + 0x65c0);

    particle = (s32 *)0x02010000;
    for (i = 0; i != 512; i++) {
        particle[0] = 0;
        particle[1] = 0x500000;
        particle[2] = (s32)((Func_08004458() & 31) - 32) << 14;
        particle[6] = (s32)(Func_08004458() & 255);
        particle = (s32 *)((u8 *)particle + 28);
    }

    FIELD_AT_OFFSET((void *)0x04000020, s16 *, 0) = 0x100;
    state = FIELD_AT_OFFSET(work, void **, 0x7828);
    if (FIELD_AT_OFFSET(state, s32 *, 20) == 1) {
        Func_080e396c(FIELD_AT_OFFSET(state, s16 *, 36), tmp);
        y_offset = 64 - tmp[0];
    } else if (FIELD_AT_OFFSET(state, s32 *, 4) == 1) {
        y_offset = -112;
    } else {
        y_offset = 0;
    }
    FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = y_offset << 8;
    FIELD_AT_OFFSET(work, s32 *, 0x7780) = 2;
    FIELD_AT_OFFSET(work, s32 *, 0x7784) = 50;
    status = Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    rect = rectangle;
    for (frame = 0;
            frame != (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                s32 *, 20) << 4) + 116;
            frame++) {
        facing = *(s32 *)0x03001E80;
        if (frame == 64) {
            Func_080f9010(212);
        }
        if (frame == 80) {
            Func_080b50e8(0);
        }
        flag = FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
            s32 *, 28);
        if (flag == 1) {
            s32 angle;
            s32 x;
            s32 y;

            angle = frame << 11;
            x = (((-Func_08002322(angle)) * 20) >> 16) + out_x + y_offset - 20;
            y = ((Func_0800231c(angle) * 4) >> 16) + out_y;
            BattleFx_FetchRectangleBlitters(
                FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                    s32 *, 4) ^ flag,
                rect);
            y -= 24;
            if (frame > 32) {
                y = (y - (frame * 2)) + 64;
            }
            ((DrawRectangleFn)rect[1])(
                canvas, (u8 *)work + 0x65c0, x, y, 40, 40);
            if (frame <= 3) {
                ((DrawRectangleFn)rect[1])(
                    canvas, (u8 *)work + 0x65c0, x, y, 40, 40);
            }
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
        }

        status = Func_080ed408(46, 7, 7, 3, 2);
        blocks = (u8 *)0x03001E50;
        rectangle[0] = *(void **)(blocks + 184);
        status = Func_080ed408(47, 7, 7, 7, 2);
        rect[1] = *(void **)(blocks + 188);

        member = 0;
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                s32 *, 20) != 0) {
            facing12 = facing + 12;
            bank = 0;
            do {
                void *member_ptr;
                s32 window;
                s32 k;

                member_ptr = *Func_080b5098(
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                        s16 *, (member * 2) + 36));
                window = member << 4;
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(facing, facing12);
                record[0] = FIELD_AT_OFFSET(member_ptr, s32 *, 8);
                record[1] = 0;
                record[2] = FIELD_AT_OFFSET(member_ptr, s32 *, 16);
                SceneTransform_ApplyPosition(record);
                if (frame > window) {
                    u8 *p;

                    p = (u8 *)0x02010000 + bank;
                    for (k = 0; k != 8; k++) {
                        if (frame > (k << 3) + window
                                && FIELD_AT_OFFSET(p, s32 *, 4) > 0x80000) {
                            s32 wobble;
                            s32 shape;

                            Func_080e3944(p, screen);
                            screen[0] = screen[0] + y_offset;
                            wobble = (Func_08002322(
                                FIELD_AT_OFFSET(p, s32 *, 24) << 10) << 4)
                                >> 16;
                            if ((k & 1) != 0) {
                                screen[0] = screen[0] - wobble;
                            } else {
                                screen[0] = screen[0] + wobble;
                            }
                            shape = Data_080ee0a2[
                                (FIELD_AT_OFFSET(p, s32 *, 24) / 8) & 7];
                            ((DrawRectangleFn)rect[k & 1])(canvas,
                                (u8 *)work + ((shape * 9) << 6) + 0x2710,
                                screen[0] - 12, screen[1] - 12, 24, 24);
                            FIELD_AT_OFFSET(p, s32 *, 4) =
                                FIELD_AT_OFFSET(p, s32 *, 4) - 0x10000;
                            FIELD_AT_OFFSET(p, s32 *, 24) =
                                FIELD_AT_OFFSET(p, s32 *, 24) + 1;
                        }
                        p += 28;
                    }
                }
                bank += 0x700;
                member++;
            } while (member
                != FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                    s32 *, 20));
        }

        member2 = 0;
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                s32 *, 20) != 0) {
            id_ofs = 36;
            window2 = 72;
            do {
                s32 base;

                base = member2 << 4;
                if (frame >= window2) {
                    void **statep;
                    void *member_ptr;
                    s32 gap;
                    s32 q;
                    s32 thick;
                    s32 j;

                    statep = (void **)((u8 *)work + 0x7828);
                    member_ptr = *Func_080b5098(
                        FIELD_AT_OFFSET(*statep, s16 *, id_ofs));
                    Render_ResetTransformState();
                    Graphics_PrepareTransferInIwramWork(facing, facing + 12);
                    if (frame == window2) {
                        Func_080d6888(FIELD_AT_OFFSET(*statep, s16 *, id_ofs),
                            1, -1, -1, 0);
                    }
                    gap = window2 + 16;
                    if (frame == gap) {
                        Func_080d6888(FIELD_AT_OFFSET(*statep, s16 *, id_ofs),
                            0, -1, -1, 0);
                    }
                    record2[0] = FIELD_AT_OFFSET(member_ptr, s32 *, 8);
                    record2[1] = 0;
                    record2[2] = FIELD_AT_OFFSET(member_ptr, s32 *, 16);
                    Func_080e3944(record2, screen);
                    screen[0] = screen[0] + y_offset;
                    if (frame < base + 104) {
                        q = frame / 4;
                        thick = 6;
                        if (frame > gap) {
                            thick = 6 - Func_080022ec((frame - base) - 88, 3);
                        }
                        if (q > 2) {
                            q = (q & 1) + 1;
                        }
                        if (frame < base + 100) {
                            s32 w;
                            s32 h;

                            w = Data_080ee0b0[q];
                            h = Data_080ee0b3[q];
                            ((DrawRectangleFn)rectangle[0])(canvas,
                                (u8 *)work + Data_080ee0aa[q],
                                screen[0] - w, (screen[1] - h) + 8, w, h);
                            h = Data_080ee0b3[q];
                            w = Data_080ee0b0[q];
                            ((DrawRectangleFn)rect[1])(canvas,
                                (u8 *)work + Data_080ee0aa[q],
                                screen[0], (screen[1] - h) + 8, w, h);
                        }
                        for (j = 0; j != screen[1]; j++) {
                            ((DrawRectangleFn)rectangle[0])(canvas,
                                (u8 *)work + 5, screen[0] - thick, j,
                                thick, 1);
                            ((DrawRectangleFn)rect[1])(canvas,
                                (u8 *)work + 5, screen[0], j, thick, 1);
                        }
                    }
                }
                id_ofs += 2;
                window2 += 16;
                member2++;
            } while (member2
                != FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                    s32 *, 20));
        }

        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        FIELD_AT_OFFSET(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Func_080cdbc0();
}
