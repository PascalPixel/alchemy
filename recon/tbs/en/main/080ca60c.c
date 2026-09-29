#include "TYPES.H"
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect at 0x080ca60c, part of the 0x03001eec
 * "battle work" subsystem already partly recovered in
 * recon/tbs/en/main/080ca1fc.c, 080cb1a4.c, 080e01e4.c and
 * games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C.
 *
 * The owner is one function: the two `bl` sites at 0x080ca956 and
 * 0x080cb176 target addresses inside this same extent (0x080cb17a and
 * 0x080ca978) and are the compiler's long-branch form, which is why the
 * epilogue returns through `pop {r0}; bx r0` instead of popping pc.
 *
 * Semantic summary: the second argument selects a seven-byte record in the
 * 0x080edf04 table that drives the whole effect -- record[0] picks between
 * two layouts, record[1]*record[4] scales the timeline, record[2] is a bit
 * mask enabling the per-frame passes, record[3] chooses the palette
 * resource, record[5] is a blit parameter and record[6] is the frame count.
 *
 * Setup opens the resources through Resource_LoadAndDecompress, copies a palette to
 * 0x05000000 through the generic word-copy helper (_call_via_r3 taking the
 * 0x03001388 routine as a trailing callback argument -- _call_via_r3 is the
 * r3 slot of the _call_via_rN trampoline at recon/tbs/raw/080072e4.s,
 * modeled as a direct call with the real callee as a trailing argument per
 * that trampoline's established convention), then seeds three particle
 * pools: 64 slots at work+0x7080, 128 slots at 0x02010000 and 512 slots at
 * 0x02013800.
 *
 * The frame loop then draws a staged foreground element (record[0] == 0
 * uses the Math_Div/Math_Mod phase tables at 0x080edf58..0x080edf76,
 * record[0] != 0 uses a 27-cell strip inside the work buffer), retriggers
 * the three pools at fixed phase boundaries, optionally scatters three
 * decorations, and finally redraws the pools through the display-kind 46/47
 * routines cached in gWorkSlot[].
 *
 * Uncertain: the roles of Data_080edf04's individual record bytes are read
 * off the arithmetic only; the two one-iteration `do/while (... != 1)` loops
 * are reproduced as written because the reference emits their induction
 * variables and their stack slots, but their original bound is not
 * recoverable from the bytes.
 *
 * Residual: this draft emits the complete 2968-byte extent with the same
 * instruction count, the same frame size (156), the same local-array offsets
 * (blit 100, screen 108, alt_pos 120, base_pos 132, seed_pos 144) and the
 * same control flow, but it is not byte-exact.  What remains is register and
 * spill-slot allocation: the reference keeps the 0x1FF particle mask in a
 * high register (r9, costing an extra `mov` per use) where this candidate
 * keeps it in a low register, and several compiler temporaries land in
 * different stack slots, which then perturbs the local instruction order.
 * The five parameter/local slots the prologue writes (96, 92, 80, 76 and the
 * arrays) already agree.
 * 2026-09-29 slice 4: alchemy permute cannot parse this draft, because
 * M2C_FIELD takes a type as a macro argument. Preprocessed, it scores
 * 17,152 with 39 symbols the linked build does not define, too far for a
 * 10-minute search, so none was run.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((s8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);

extern void *gWorkSlot[];
extern const u16 ParticleStreams_CellOffsets[];
extern const u16 Data_080edebe[];
extern const u8 Data_080edeca[];
extern const u8 Data_080eded0[];
extern const u8 Data_080edf04[];
extern const u8 Data_080edf58[];
extern const u8 Data_080edf5e[];
extern const u16 Data_080edf64[];
extern const u8 Data_080edf70[];
extern const u8 Data_080edf76[];
extern const u8 Data_080edf7b[];

extern u8 Value_00000073;
extern u8 Value_0000008d;
extern u8 Value_0000009e;
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000b4;
extern u8 Value_000000c4;
extern u8 Value_000000c5;
extern u8 Value_000000c6;
extern u8 Value_000000cc;
extern u8 Value_000000ce;

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
void _call_via_r3(void *dest, void *src, s32 size, WordCopyFn copier);
void **GetBattleObjectSlotFar(s32 member_id);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 member_id, s32 *out);
void EffectPosition_ApplyStepAndYOffset(s32 member_id, s32 *out);
u32 Random16(void);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
s32 Math_Div(s32 numerator, s32 denominator);
s32 Math_Mod(s32 numerator, s32 denominator);
void Runtime_ReleaseHeapBlock(s32 id);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Audio_PlayCue(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 flag);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void EffectStep_AdvanceWithGravity2D(s32 *particle, s32 a, s32 b);
void EffectPosition_ApplyBaseAndYOffset(const s32 *source, s32 *screen);
void EffectStep_AdvanceWithGravity3D(s32 *particle, s32 a, s32 b);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Scheduler_RemoveCallback(void *callback);
s32 BattleFx_EndCanvasLayer(void);

void BattleFx_RunTwelveMode(void *object, s32 kind)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    s32 frame;
    s32 n;
    void *extra_target;
    s32 facing;
    void *member_object;
    s32 palette_id;
    s32 count;
    s32 phase_index;
    s32 cell;
    s32 i;
    s32 width;
    s32 height;
    s32 size;
    s32 *particle;
    s32 pass;
    s32 hit;
    s32 idx;
    s32 look_target;
    s32 pool_off;
    s32 member_off;
    DrawRectangleFn blit[2];
    s32 seed_pos[3];
    s32 base_pos[3];
    s32 alt_pos[3];
    s32 screen[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    facing = *(s32 *)((s8 *)heap_cache - 108);
    M2C_FIELD(work, void **, 0x7828) = object;

    if (kind == 8) {
        BattleFx_BeginCanvasLayer(0);
    } else {
        BattleFx_BeginCanvasLayer(1);
    }

    *(u16 *)0x04000052 = 0x1010;

    Resource_LoadAndDecompress((s32)&Value_00000073, extra_target, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_000000ce, work, 1, 0);

    if (Data_080edf04[kind * 7] == 0) {
        Resource_LoadAndDecompress((s32)&Value_000000c5, (s8 *)work + (200 << 4), 0, 0);
    } else {
        Resource_LoadAndDecompress((s32)&Value_000000c6, (s8 *)work + (200 << 4), 0, 0);
    }

    switch (Data_080edf04[kind * 7 + 3]) {
    case 0:
        palette_id = (s32)&Value_000000cc;
        break;
    case 1:
        palette_id = (s32)&Value_000000a0;
        break;
    case 2:
        palette_id = (s32)&Value_000000a1;
        break;
    case 3:
        palette_id = (s32)&Value_000000b4;
        break;
    case 4:
        palette_id = (s32)&Value_0000008d;
        break;
    case 5:
    default:
        palette_id = (s32)&Value_000000c4;
        break;
    }
    _call_via_r3((void *)(160 << 19), Resource_GetTableEntry(palette_id), 128,
        (WordCopyFn)0x03001388);

    Resource_LoadAndDecompress((s32)&Value_0000009e, (s8 *)work + (200 << 6), 1, 0);

    n = 0;
    member_off = 36;
    pool_off = 0;
    do {
        member_object = *GetBattleObjectSlotFar(
            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, member_off));

        particle = (s32 *)((s8 *)work + (225 << 7));
        i = 0;
        do {
            EffectPosition_ApplyAlternateStepAndYOffset(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
                    member_off),
                seed_pos);
            particle[0] = (seed_pos[0] / 2) << 16;
            particle[1] = 160 << 15;
            particle[2] = 0;
            particle[3] = ((s32)(Random16() & 0xFF) - 128) << 9;
            particle[4] = ((s32)(Random16() & 0xFF) - 128) << 9;
            particle[5] = 0;
            particle[6] = -1;
            i++;
            particle += 7;
        } while (i != 64);

        particle = (s32 *)(0x02010000 + pool_off);
        i = 0;
        do {
            particle[0] = M2C_FIELD(member_object, s32 *, 8);
            particle[1] = 160 << 13;
            particle[2] = M2C_FIELD(member_object, s32 *, 16);
            particle[3] = ((s32)(Random16() & 0xFF) - 128) << 11;
            particle[4] = ((s32)(Random16() & 0xFF) - 128) << 11;
            particle[5] = ((s32)(Random16() & 0xFF) - 128) << 11;
            particle[6] = -1;
            i++;
            particle += 7;
        } while (i != 128);

        particle = (s32 *)0x02013800;
        i = 0;
        do {
            particle[0] = M2C_FIELD(member_object, s32 *, 8);
            particle[1] = 160 << 13;
            particle[2] = M2C_FIELD(member_object, s32 *, 16);
            if (Data_080edf04[kind * 7] == 1) {
                particle[3] = ((s32)(Random16() & 0xFF) - 128) << 11;
                particle[4] = ((s32)(Random16() & 0xFF) - 128) << 11;
                particle[5] = ((s32)(Random16() & 0xFF) - 128) << 11;
            } else {
                if ((Data_080edf04[kind * 7 + 2] & 16) != 0) {
                    particle[3] = (s32)(Random16() & 0x1FF) << 11;
                    particle[4] =
                        ((s32)(Random16() & 0x1FF) - 256) << 11;
                    particle[5] =
                        ((s32)(Random16() & 0x1FF) - 256) << 11;
                } else {
                    particle[3] = (s32)(Random16() & 0xFF) << 11;
                    particle[4] = ((s32)(Random16() & 0xFF) - 128) << 11;
                    particle[5] = ((s32)(Random16() & 0xFF) - 128) << 11;
                }
                if (particle[0] > 0) {
                    particle[3] = -particle[3];
                }
            }
            particle[6] = -1;
            i++;
            particle += 7;
        } while (i != 512);

        pool_off += 3584;
        member_off += 2;
        n++;
    } while (n != 1);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    EffectPosition_ApplyStepAndYOffset(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36), base_pos);
    EffectPosition_ApplyAlternateStepAndYOffset(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36), alt_pos);
    base_pos[1] += (alt_pos[1] - base_pos[1]) / 2;

    look_target = facing + 12;

    for (frame = 0; frame != Data_080edf04[kind * 7 + 6]; frame++) {
        count = Data_080edf04[kind * 7 + 1] * Data_080edf04[kind * 7 + 4];
        hit = 0;

        if (Data_080edf04[kind * 7] == 0) {
            if (frame < count * 6) {
                cell = Math_Mod(
                    Math_Div(frame, Data_080edf04[kind * 7 + 4]), 6);

                if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4)
                        == 1) {
                    BattleEffect_LoadWork(46, 7, 7, 7, 2);
                    ((DrawRectangleFn)gWorkSlot[46])(draw_destination,
                        (s8 *)work + Data_080edf64[cell],
                        ((base_pos[0] / 2) - (Data_080edf70[cell] >> 1)
                            - Data_080edf58[cell]) + 8,
                        base_pos[1] - (Data_080edf5e[cell] >> 1),
                        Data_080edf58[cell], Data_080edf5e[cell]);
                } else {
                    BattleEffect_LoadWork(46, 7, 7, 3, 2);
                    ((DrawRectangleFn)gWorkSlot[46])(draw_destination,
                        (s8 *)work + Data_080edf64[cell],
                        ((base_pos[0] / 2) + (Data_080edf70[cell] >> 1)) - 8,
                        base_pos[1] - (Data_080edf5e[cell] >> 1),
                        Data_080edf58[cell], Data_080edf5e[cell]);
                }
                Runtime_ReleaseHeapBlock(46);

                if (Math_Mod(frame, Data_080edf04[kind * 7 + 4] * 6)
                        == Data_080edf04[kind * 7 + 4] * 4) {
                    if (kind == 8) {
                        BattleEventRuntime_BeginPhaseFar(134);
                    } else {
                        Audio_PlayCue(133);
                        BattleEventRuntime_BeginPhaseFar(133);
                    }

                    if ((Data_080edf04[kind * 7 + 2] & 16) != 0) {
                        M2C_FIELD(work, s32 *, 0x77A8) = 8;
                        ObjectGroup_UpdateMembers(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, 36),
                            7, 5, 0, 12);
                        BattleMotion_ApplyVariantMotionFar(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, 36),
                            4);
                        particle = (s32 *)0x02013800;
                        i = 0;
                        do {
                            particle[6] = (s32)(Random16() & 15) + 15;
                            i++;
                            particle += 7;
                        } while (i != 512);
                    } else {
                        M2C_FIELD(work, s32 *, 0x77A8) = 4;
                        ObjectGroup_UpdateMembers(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s16 *, 36),
                            7, 5, 0, 8);
                        i = 0;
                        do {
                            particle = (s32 *)(0x02013800
                                + ((Math_Div(frame,
                                        Data_080edf04[kind * 7 + 4] * 6) * 32)
                                      + i) * 28);
                            particle[6] = (s32)(Random16() & 15) + 7;
                            i++;
                        } while (i != 32);
                    }

                    i = 0;
                    do {
                        particle = (s32 *)(((s8 *)work + (225 << 7))
                            + ((Math_Div(frame,
                                    Data_080edf04[kind * 7 + 4] * 6) * 16)
                                  + i) * 28);
                        particle[6] = 0;
                        i++;
                    } while (i != 8);
                }
            }
        } else if (frame < (count * 4) + 4) {
            if (frame >= count * 4) {
                idx = 3;
            } else {
                phase_index =
                    Math_Div(frame, Data_080edf04[kind * 7 + 4]);
                while (phase_index > 4) {
                    phase_index -= 4;
                }
                idx = Data_080edf76[phase_index];
            }

            BattleEffect_LoadWork(46, 7, 7, 3, Data_080edf04[kind * 7 + 5]);
            ((DrawRectangleFn)gWorkSlot[46])(draw_destination,
                ((s8 *)work + (200 << 4)) + (idx * 27) * 32,
                (base_pos[0] / 2) - 18, 56, 18, 48);
            Runtime_ReleaseHeapBlock(46);

            BattleEffect_LoadWork(46, 7, 7, 7, Data_080edf04[kind * 7 + 5]);
            ((DrawRectangleFn)gWorkSlot[46])(draw_destination,
                ((s8 *)work + (200 << 4)) + (idx * 27) * 32,
                base_pos[0] / 2, 56, 18, 48);
            Runtime_ReleaseHeapBlock(46);

            if (Math_Mod(frame, Data_080edf04[kind * 7 + 4] * 4)
                    == Data_080edf04[kind * 7 + 4] * 3) {
                ObjectGroup_UpdateMembers(
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                    7, 5, 0, 8);
                M2C_FIELD(work, s32 *, 0x77A8) = 4;

                if (frame > (Data_080edf04[kind * 7 + 1] * 4 - 4)
                        * Data_080edf04[kind * 7 + 4]) {
                    BattleEventRuntime_BeginPhaseFar(133);
                } else {
                    Audio_PlayCue(133);
                }

                i = 0;
                do {
                    particle = (s32 *)(0x02013800
                        + ((Math_Div(frame,
                                Data_080edf04[kind * 7 + 4] * 6) * 64)
                              + i) * 28);
                    particle[6] = (s32)(Random16() & 15) + 7;
                    i++;
                } while (i != 64);

                i = 0;
                do {
                    particle = (s32 *)(((s8 *)work + (225 << 7))
                        + ((Math_Div(frame,
                                Data_080edf04[kind * 7 + 4] * 6) * 16)
                              + i) * 28);
                    particle[6] = 0;
                    i++;
                } while (i != 8);

                i = 0;
                do {
                    particle = (s32 *)(0x02010000
                        + ((Math_Div(frame,
                                Data_080edf04[kind * 7 + 4] * 6) * 16)
                              + i) * 28);
                    particle[6] = 0;
                    i++;
                } while (i != 16);
            }

            if (idx == 3) {
                hit = 1;
            }
        }

        if ((Data_080edf04[kind * 7 + 2] & 4) != 0 && hit != 0) {
            i = 0;
            do {
                s32 angle;
                s32 radius;
                s32 x;
                s32 y;

                angle = (s32)(Random16() & 0xFFFF);
                radius = (s32)(Random16() & 31) + 4;
                x = ((base_pos[0] / 2)
                        + ((radius * Trig_Sin(angle)) >> 17))
                    - (Data_080edeca[frame & 3] >> 1);
                y = (base_pos[1] - ((radius * Trig_Cos(angle)) >> 17))
                    - (Data_080eded0[frame & 3] >> 1);
                BattleEffect_LoadWork(47, 7, 7,
                    3 | Data_080edf7b[Random16() & 3], 3);
                ((DrawRectangleFn)gWorkSlot[47])(draw_destination,
                    (s8 *)work + Data_080edebe[frame & 3], x, y,
                    Data_080edeca[frame & 3], Data_080eded0[frame & 3]);
                Runtime_ReleaseHeapBlock(47);
                i++;
            } while (i != 3);
        }

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, look_target);

        BattleEffect_LoadWork(46, 7, 7, 3, 2);
        blit[0] = (DrawRectangleFn)gWorkSlot[46];
        BattleEffect_LoadWork(47, 7, 7, 3, 2);
        blit[1] = (DrawRectangleFn)gWorkSlot[47];

        pass = 0;
        do {
            member_object = *GetBattleObjectSlotFar(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));

            if ((Data_080edf04[kind * 7 + 2] & 1) != 0) {
                particle = (s32 *)((s8 *)work + (225 << 7));
                i = 0;
                do {
                    if ((u32)particle[6] <= 23) {
                        blit[i & 1](draw_destination,
                            ((s8 *)work + (200 << 6))
                                + ((particle[6] / 4) * 9) * 128,
                            M2C_FIELD(particle, s16 *, 2) - 12,
                            M2C_FIELD(particle, s16 *, 6) - 24, 24, 48);
                        EffectStep_AdvanceWithGravity2D(particle, 62, -1024);
                        particle[6] = particle[6] + 1;
                    }
                    i++;
                    particle += 7;
                } while (i != 64);
            }

            if ((Data_080edf04[kind * 7 + 2] & 2) != 0) {
                size = 3;
                if (kind == 11) {
                    size = 8;
                }
                if (frame == 55) {
                    ObjectGroup_UpdateMembers(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8),
                        7, -1, -1, 0);
                }
                if (frame == 90) {
                    ObjectGroup_UpdateMembers(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8),
                        0, -1, -1, 0);
                }

                particle = (s32 *)0x02010000;
                i = 0;
                do {
                    if (particle[6] >= 0) {
                        EffectPosition_ApplyBaseAndYOffset(particle, screen);
                        screen[0] = screen[0] >> 1;
                        blit[1](draw_destination,
                            (s8 *)extra_target + ParticleStreams_CellOffsets[size - 1],
                            screen[0] - (s32)((u32)size >> 1),
                            screen[1] - size, size, size * 2);
                        EffectStep_AdvanceWithGravity3D(particle, 60, 0);
                        particle[6] = particle[6] + 1;
                        if (particle[6] > 10) {
                            s32 dx;
                            s32 dy;
                            s32 dz;

                            dx = (M2C_FIELD(member_object, s32 *, 8)
                                     - particle[0]) >> 8;
                            dy = ((M2C_FIELD(member_object, s32 *, 12)
                                      - particle[1]) + (160 << 13)) >> 8;
                            dz = (M2C_FIELD(member_object, s32 *, 16)
                                     - particle[2]) >> 8;
                            particle[3] = particle[3] + dx;
                            particle[4] = particle[4] + dy;
                            particle[5] = particle[5] + dz;
                            if ((u32)(dx + 0xFFF) <= 0x1FFE
                                && (u32)(dz + 0xFFF) <= 0x1FFE) {
                                particle[6] = -1;
                            }
                        }
                    }
                    i++;
                    particle += 7;
                } while (i != 64);
            }

            if ((Data_080edf04[kind * 7 + 2] & 8) != 0) {
                particle = (s32 *)0x02013800;
                i = 0;
                do {
                    if (particle[6] >= 0) {
                        width = (particle[6] >> 3) + 1;
                        EffectPosition_ApplyBaseAndYOffset(particle, screen);
                        screen[0] = screen[0] >> 1;
                        EffectStep_AdvanceWithGravity3D(particle, 60, 0);
                        height = width * 2;
                        blit[1](draw_destination,
                            (s8 *)extra_target + ParticleStreams_CellOffsets[width - 1],
                            screen[0] - (width / 2), screen[1] - width,
                            width, height);
                        particle[6] = particle[6] - 1;
                    }
                    i++;
                    particle += 7;
                } while (i != 128);
            }

            pass++;
        } while (pass != 1);

        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);

        if ((Data_080edf04[kind * 7 + 2] & 16) != 0) {
            Camera_ApplyShake(8, 8);
        } else {
            Camera_ApplyShake(2, 4);
        }

        ObjectGroup_TickMemberTimers();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    BattleFx_EndCanvasLayer();
}
