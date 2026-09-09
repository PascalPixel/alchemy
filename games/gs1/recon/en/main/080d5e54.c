#include "types.h"
#include "effect_step.h"

#define BattleEffect_RunSparkTravel Func_080d5e54

/*
 * Battle-presentation sub-effect at 0x080d5e54, from the same 0x03001eec
 * "battle work" subsystem family already recovered in
 * games/gs1/recon/en/main/080d85d0.c, 080d5c48.c, 080d82b0.c and
 * games/gs1/src/battle/effects/member_orbit/run.c.  The heap-cache triple
 * (kind 39 work block, kind 40 draw destination, kind 41 graphics block),
 * the work + 0x7828 state republication, the Value_ absolute-literal
 * spelling, the 0x02010000 particle pool and the 0x03001E80 render-context
 * word are all the conventions recorded for those owners.
 *
 * The sequence has two phases.
 *
 * Phase one (98 frames) seeds 64 EffectStep records at 0x02010000 with a
 * random horizontal heading and speed and a random launch height, moves the
 * camera position from the acting member toward the first listed target over
 * frames 30..69 in forty equal steps, and then, once per frame, walks the
 * first 32 of those records - the reference seeds 64 but only ever draws
 * records 0..31, and the remaining 32 are simply left where phase two
 * re-seeds them.  A spark is drawn once the frame counter passes its index
 * and while its variant field is still zero, so the burst opens one particle
 * per frame.  Each drawn spark is rendered through the depth-scaled cell
 * table Data_080ede48 (nine sizes, one 64-unit depth band each, clamped to
 * the 250..634 band), advanced by the shared gravity helper, and after
 * another thirty frames pulled back toward the origin by a 1/256
 * acceleration.  From frame 83 a single 20x34 sprite drawn from the work
 * block tracks a sine sweep.
 *
 * Phase two re-seeds all 512 records with a random position in a box about
 * the pool origin - the seeding loop is flat and carries no member index; the
 * per-member placement comes from the transform rebuilt for each member
 * before its own 64-record slice (stride 0x700) is drawn.  It then runs
 * member_count * 8 + 72 frames.  Each frame draws the same 20x34 sprite at
 * the travelling target position, then for every listed member gathers that
 * member's 64 sparks inward: distance is the IWRAM integer square root at
 * 0x030001d8 of the sum of the squared 1/256 components, the spark is drawn
 * with the Data_080ede84/Data_080ede96 cell pair chosen by
 * (index * 4 + frame) % 9, and each component is reduced by
 * component / distance so the cloud converges on the member.
 *
 * Uncertain: the roles of the effect-state fields at offsets 4, 8, 20 and 36
 * are inherited from the sibling owner games/gs1/recon/en/main/080dab74.c,
 * which reads them as direction, source id, member count and the member id
 * array.  Their use here is consistent with that but does not independently
 * establish it, so the accesses stay written as offsets.  The sound and
 * member-update ids (212, 142, 126 and the 7/0 pair) are left as raw
 * constants because nothing here shows what they select.
 *
 * Residual (draft, not adopted; measured, not estimated).  candidate = 1712,
 * reference = 1712, topology equal, differing_halfwords = 577,
 * wrong_instructions = 572.  Under the tool's own smart alignment 297
 * instructions are present on both sides but differ, and 99 on each side do
 * not pair up at all, so this is not a pure register renaming: the local
 * instruction order moves as well.
 *
 * The one cause behind all of it is a global register-allocation divergence.
 * The reference spills the second party member's context pointer to the
 * frame and keeps the address of the camera position vector in fp, the frame
 * counter in r9 and the render-context word in r5; this source makes the
 * opposite trade, putting that context pointer in fp and re-deriving the
 * vector addresses, which needs two fewer spill slots.  The candidate frame
 * is therefore 164 bytes against the reference's 172, every array sits eight
 * bytes lower, and the scalar spill slots are not merely shifted - the two
 * sources spill a different set of values, so the slot numbers do not
 * correspond one for one.  Around that shift the scheduler also reorders a
 * handful of loads in the phase-one damping block and in the phase-two
 * gather, where the reference has one more free register to pipeline with.
 *
 * Eight source hypotheses have been tried against this axis.  Two helped and
 * are kept: giving each loop its own counter and cursor local, and binding
 * the gather cell's source pointer before its size.  Six were measured and
 * reverted: an explicit negated frame and negated spin accumulator (1724
 * bytes, 789 halfwords), a plain for-loop over the phase-two frame counter
 * (1708 bytes, 694 halfwords), a per-phase frame counter, a separate cursor
 * for the phase-two seeding loop, an explicit negated-spin accumulator alone,
 * and a different declaration order.  The axis is closed on present evidence;
 * reopening it needs allocator dumps (alchemy build allocator 080d5e54, then
 * psynergy inspect allocator), not another source spelling.
 *
 * Call targets carry the names "alchemy inspect" resolves and that the owner
 * register already holds.  The names it prints for Func_080e0524
 * (load_and_decompress), Func_08004458 (random_16), Func_080d6888
 * (update_members), Func_080e3944 (apply_base_and_y_offset) and Func_080e38b8
 * (advance_with_gravity_3d) are not registered symbols, so those keep their
 * Func_ spelling here, as the sibling owners do.  Func_080072f4 and
 * Func_080072f0 are never named in this source: they are the compiler
 * runtime's call-via-r4 and call-via-r3 thunks and are reached only as typed
 * indirect calls.
 */

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* The dossier evidence for 080d82b0 records that this compiler CSEs a named
   work + 0x7828 local across the whole enclosing block and emits a spill the
   reference does not have, so the access is re-expanded textually instead. */
#define STATE M2C_FIELD(work, void **, 0x7828)

/* Six drawn arguments: destination, source cell, x, y, width, height.  Both
   entries come back from BattleEffect_FetchRectangleBlitters and are reached through the compiler
   runtime's bx r4 thunk, so they are typed indirect calls. */
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

/* Relocated IWRAM integer square root, reached through the bx r3 thunk. */
typedef s32 (*SqrtFn)(s32 value);

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_00000073;
extern u8 Value_00000092;
extern u8 Value_000000ba;

/* Depth-scaled spark cells: one source offset per size 1..9. */
extern const u16 Data_080ede48[];

/* Gather-cell pair: source offset and square edge, nine animation cels. */
extern const u16 Data_080ede84[];
extern const u8 Data_080ede96[];

void Func_080cd594(s32 mode);
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b);
void BattleEffect_FetchRectangleBlitters(s32 alternate, void **output);
void **Func_080b5098(s32 member_id);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_080022fc(s32 value, s32 divisor);
void Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Render_ResetTransformState(void);
void Func_080049e8(void);
void Func_08004a5c(void);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Func_08004c6c(s32 angle);
void SceneTransform_ApplyPosition(s32 *position);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e3944(void *source, s32 *screen);
void Func_080e38b8(void *source, s32 a, s32 b);
void Func_080f9010(s32 id);
void Func_080030f8(s32 frames);
void Runtime_ReleaseHeapBlock(s32 id);
s32 Func_080cdbc0(void);

s32 BattleEffect_RunSparkTravel(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *graphics;
    void *actor;
    void *goal;
    struct EffectStep *step;
    struct EffectStep *spark_step;
    struct EffectStep *grain_step;
    s32 frame;
    s32 i;
    s32 slot;
    s32 grain;
    s32 member;
    s32 id_offset;
    s32 pool_offset;
    s32 facing;
    s32 heading;
    s32 spin;
    /* This compiler lays the frame out with the two-word blitter pair first
       and the three-word vectors after it in reverse declaration order, so
       the declarations below are ordered to reproduce the reference frame:
       blit, view, record, screen, spark, delta, target, pos. */
    void *blit[2];
    s32 pos[3];
    s32 target[3];
    s32 delta[3];
    s32 spark[3];
    s32 screen[3];
    s32 record[3];
    s32 view[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    graphics = heap_cache[2];
    STATE = object;
    Func_080cd594(0);
    Func_080e0524((s32) &Value_00000092, work, 1, 1);
    Func_080e0524((s32) &Value_00000073, graphics, 0, 0);
    BattleEffect_FetchRectangleBlitters(M2C_FIELD(STATE, s32 *, 4) ^ 1, blit);
    actor = *Func_080b5098(M2C_FIELD(STATE, s32 *, 8));
    goal = *Func_080b5098(M2C_FIELD(STATE, s16 *, 36));

    step = (struct EffectStep *)0x02010000;
    i = 0;
    do {
        s32 speed;

        heading = Func_08004458() & 0xFFFF;
        speed = Func_08004458() & 0xFF;
        step->x = 0;
        step->y = ((Func_08004458() & 31) + 20) << 16;
        step->z = 0;
        step->velocity_x = (Func_08002322(heading) * (speed + 128)) >> 5;
        step->velocity_y = 0;
        step->velocity_z = (Func_0800231c(heading) * (speed + 128)) >> 5;
        step->variant = 0;
        i++;
        step++;
    } while (i != 64);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    pos[0] = M2C_FIELD(actor, s32 *, 8);
    pos[1] = 0;
    pos[2] = M2C_FIELD(actor, s32 *, 16);
    target[0] = M2C_FIELD(goal, s32 *, 8);
    target[1] = 0x5A0000;
    target[2] = 0;
    delta[0] = Func_080022ec(target[0] - pos[0], 40);
    delta[1] = Func_080022ec(target[1] - pos[1], 40);
    delta[2] = Func_080022ec(target[2] - pos[2], 40);

    frame = 0;
    do {
        facing = *(s32 *)0x03001E80;
        if (frame == 8) {
            Func_080f9010(212);
        }
        if (frame == 80) {
            Func_080f9010(142);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame >= 30 && frame <= 69) {
            pos[0] += delta[0];
            pos[1] += delta[1];
            pos[2] += delta[2];
        }
        SceneTransform_ApplyPosition(pos);
        if (frame == 0) {
            Func_080d6888(M2C_FIELD(STATE, s32 *, 8), 7, -1, -1, 0);
        }
        if (frame == 24) {
            Func_080d6888(M2C_FIELD(STATE, s32 *, 8), 0, -1, -1, 0);
        }

        spin = frame << 8;
        spark_step = (struct EffectStep *)0x02010000;
        slot = 0;
        do {
            if (frame > slot && spark_step->variant == 0) {
                s32 scale;

                Func_080049e8();
                switch (slot & 3) {
                case 0:
                    SceneTransform_ApplyYaw(spin);
                    break;
                case 1:
                    SceneTransform_ApplyPitch(-spin);
                    break;
                case 2:
                    Func_08004c6c(-spin);
                    break;
                case 3:
                    SceneTransform_ApplyPitch(-spin);
                    Func_08004c6c(-spin);
                    break;
                }
                Func_080e3944(spark_step, screen);
                screen[0] = screen[0] >> 1;
                Func_08004a5c();
                if (screen[2] < 250) {
                    screen[2] = 250;
                }
                if (screen[2] > 634) {
                    screen[2] = 634;
                }
                scale = 9 - (screen[2] - 250) / 64;
                ((DrawRectangleFn)blit[0])(
                    draw_destination,
                    (u8 *)graphics + Data_080ede48[scale - 1],
                    screen[0] - scale / 2,
                    screen[1] - scale,
                    scale,
                    scale * 2);
                Func_080e38b8(spark_step, 60, 0);
                if (frame > slot + 30) {
                    spark_step->velocity_x += -spark_step->x >> 8;
                    spark_step->velocity_y += -spark_step->y >> 8;
                    spark_step->velocity_z += -spark_step->z >> 8;
                }
            }
            slot++;
            spin += frame << 5;
            spark_step++;
        } while (slot != 32);

        if (frame > 82) {
            spark[0] = 0;
            spark[1] = Func_08002322(frame << 10) << 2;
            spark[2] = 0;
            Func_080e3944(spark, screen);
            screen[0] = screen[0] >> 1;
            ((DrawRectangleFn)blit[1])(
                draw_destination, work,
                screen[0] - 10, screen[1] - 17, 20, 34);
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 98);

    step = (struct EffectStep *)0x02010000;
    i = 0;
    do {
        step->x = ((Func_08004458() & 255) - 127) << 15;
        step->y = ((Func_08004458() & 127) + 64) << 15;
        step->z = ((Func_08004458() & 255) - 127) << 15;
        step->variant = 0;
        i++;
        step++;
    } while (i != 512);

    Func_080e0524((s32) &Value_000000ba, graphics, 0, 0);

    if (M2C_FIELD(STATE, s32 *, 20) * 8 + 72 != 0) {
        frame = 0;
        do {
            facing = *(s32 *)0x03001E80;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
            if (frame >= M2C_FIELD(STATE, s32 *, 20) * 8 + 40) {
                target[1] += 0x40000;
            }
            record[0] = target[0];
            record[1] = target[1];
            record[2] = target[2] + Func_08002322(frame << 11) * 40;
            Func_080e3944(record, view);
            view[0] = view[0] >> 1;
            ((DrawRectangleFn)blit[0])(
                draw_destination, work,
                view[0] - 10, view[1] - 17, 20, 34);

            member = 0;
            if (M2C_FIELD(STATE, s32 *, 20) != 0) {
                id_offset = 36;
                pool_offset = 0;
                do {
                    void *member_object;
                    s32 stagger;

                    member_object = *Func_080b5098(
                        M2C_FIELD(STATE, s16 *, id_offset));
                    stagger = member * 8;
                    Render_ResetTransformState();
                    Graphics_PrepareTransferInIwramWork(facing, facing + 12);
                    pos[0] = M2C_FIELD(member_object, s32 *, 8);
                    pos[1] = 0x280000;
                    pos[2] = M2C_FIELD(member_object, s32 *, 16);
                    SceneTransform_ApplyPosition(pos);
                    if (frame == stagger + 30) {
                        Func_080f9010(126);
                    }
                    if (frame == stagger + 40) {
                        Func_080d6888(M2C_FIELD(STATE, s16 *, id_offset),
                            7, -1, -1, 0);
                    }
                    if (frame == stagger + 64) {
                        Func_080d6888(M2C_FIELD(STATE, s16 *, id_offset),
                            0, -1, -1, 0);
                    }
                    if (frame > stagger) {
                        SceneTransform_ApplyYaw((frame - stagger) << 9);
                        grain_step = (struct EffectStep *)
                            ((u8 *)0x02010000 + pool_offset);
                        grain = 0;
                        do {
                            if (frame > stagger + grain / 2) {
                                s32 dx;
                                s32 dy;
                                s32 dz;
                                s32 distance;

                                dx = grain_step->x >> 8;
                                dy = grain_step->y >> 8;
                                dz = grain_step->z >> 8;
                                distance = ((SqrtFn)0x030001d8)(
                                    dx * dx + dy * dy + dz * dz) >> 9;
                                if (distance != 0) {
                                    s32 cel;
                                    const u8 *src;
                                    s32 size;
                                    u32 half;

                                    Func_080e3944(grain_step, view);
                                    view[0] = view[0] >> 1;
                                    if (view[2] < 314) {
                                        view[2] = 314;
                                    }
                                    if (view[2] > 634) {
                                        view[2] = 634;
                                    }
                                    cel = Func_080022fc(grain * 4 + frame, 9);
                                    src = (u8 *)graphics + Data_080ede84[cel];
                                    size = Data_080ede96[cel];
                                    half = (u32)size >> 1;
                                    ((DrawRectangleFn)blit[1])(
                                        draw_destination, src,
                                        view[0] - half,
                                        view[1] - half,
                                        size, size);
                                    grain_step->x -= Func_080022ec(
                                        grain_step->x, distance);
                                    grain_step->y -= Func_080022ec(
                                        grain_step->y, distance);
                                    grain_step->z -= Func_080022ec(
                                        grain_step->z, distance);
                                }
                            }
                            grain++;
                            grain_step++;
                        } while (grain != 64);
                    }
                    id_offset += 2;
                    pool_offset += 0x700;
                    member++;
                } while (member != M2C_FIELD(STATE, s32 *, 20));
            }

            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != M2C_FIELD(STATE, s32 *, 20) * 8 + 72);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    return Func_080cdbc0();
}
