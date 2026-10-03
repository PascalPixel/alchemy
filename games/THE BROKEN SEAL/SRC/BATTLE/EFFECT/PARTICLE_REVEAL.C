#include "RESOURCE.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BATTLE_PRESENTATION.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void BattleFx_EndCanvasLayer(void);
void Audio_PlayCue(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern u8 PuffArc_CellWidths[];
extern u8 PuffArc_CellHeights[];
extern u8 PuffArc_CellBiasY[];
extern u16 PuffArc_CellSourceOffsets[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: the small Venus djinn sheet swings in on an arc while ten
   blue flame columns rise one after another, four frames apart; each column
   throws sixteen motes into the map cell buffer as it appears, which fall
   under gravity while their life in variant runs out, and makes every
   affected unit react. */
void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    void *sheet;
    struct EffectStep *step;
    struct EffectStep *mote;
    s32 i;
    s32 j;
    s32 angle;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    BattleEffect_LoadWork(46, 7, 7, 11, 2);
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw[0] = heap_cache[7];
    draw[1] = heap_cache[8];
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, (u8 *)work + 0x2f8, 1, 0);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != 32; i++) {
        step = &work->particles[i];
        step->x = Random16() & 63;
        step->y = 104;
    }
    for (i = 0; i != 512; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;
    Audio_PlayCue(141);
    frame = 0;
    angle = 0x8000;
    do {
        if (frame <= 79) {
            s32 x = (Trig_Sin(angle) * 24) >> 16;
            s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

            draw[1](canvas, work, x + 22, y + 29, 20, 38);
        }
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(133);
        for (i = 0; i != 10; i++) {
            step = &work->particles[i];
            if (frame >= i * 4 + 16) {
                draw[0](canvas, (u8 *)work + 0x9e0, step->x - 17, step->y - 32, 34, 65);
                if (frame == i * 4 + 16) {
                    for (j = 0; j != 16; j++) {
                        s32 direction;
                        s32 speed;

                        mote = &((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j];
                        direction = (Random16() & 0x7fff) + 0x4000;
                        speed = (Random16() & 0x1ff) + 256;
                        mote->x = step->x << 16;
                        mote->y = (step->y + 16) << 16;
                        mote->velocity_x = (Trig_Sin(direction) * speed) >> 7;
                        mote->velocity_y = (Trig_Cos(direction) * speed) >> 6;
                        mote->variant = (Random16() & 15) + 32;
                    }
                    if (i & 1)
                        Audio_PlayCue(133);
                    work->shake_frames = 4;
                    for (j = 0; j != work->effect->count; j++) {
                        ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
                    }
                }
                step->y -= 12;
            }
        }
        for (i = 0; i != 512; i++) {
            mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];
            if (mote->variant != -1) {
                s32 size = mote->variant / 16 + 2;

                draw[1](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    HI(mote->x) - size / 2, HI(mote->y) - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(mote, 62, 0x2000);
                mote->variant--;
            }
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        angle -= 0x800;
        frame++;
    } while (frame != 96);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/* Battle effect: a glow orb animates for 24 frames, then the small Mars
   djinn sheet slides across as a 20-pixel strip (frames 20-31, its palette
   copied in at frame 20); from frame 32 nine puffs open along a shallow arc
   (the puff records at work->particles keep their frame in variant). Each
   puff seeded sixteen motes in the map cell buffer, which fall under gravity
   from frame 40 on, a burst every two frames, swaying on an angle kept in z.
   At frame 38 every affected unit reacts. */
void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *sheet;
    DrawRectangle callbacks[2];
    DrawRectangle *draw;
    struct EffectStep *step; /* walks the puffs, then the motes it draws */
    struct EffectStep *mote;
    s32 burst_offset;
    s32 member;
    s32 frame;
    s32 i;
    s32 curtain_y;
    s32 angle_mask;
    s32 speed_mask;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    draw = callbacks;
    BattleFx_FetchRectangleBlitters(0, draw);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, (u8 *)work + 0x320, 1, 1);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    burst_offset = 0;
    member = 0;
    step = work->particles;
    do {
        s32 angle = member << 11;

        step->x = (Trig_Sin(angle) * 24) >> 16;
        step->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
        if (member & 1)
            step->x = 32 - step->x;
        else
            step->x += 32;
        step->variant = -(member * 2);
        i = 0;
        /* FAKEMATCH: the two random masks sit in locals set in this order,
           ahead of the mote pointer; that order gives 0xffff r9 and 127 fp
           and keeps the reload registers of the reference. */
        speed_mask = 127; /* FAKEMATCH: mask local, see above */
        angle_mask = 0xffff; /* FAKEMATCH: mask local, see above */
        mote = (struct EffectStep *)(Ram_MapCellBuffer + burst_offset);
        do {
            mote->x = (((Random16() & 15) + step->x) - 8) << 16;
            mote->y = ((Random16() & 7) + 96) << 16;
            mote->velocity_x = ((Random16() & speed_mask) - 64) << 11;
            mote->velocity_y = ((Random16() & speed_mask) - 64) << 10;
            mote->z = Random16() & angle_mask;
            mote->velocity_z = Random16() & angle_mask;
            i++;
            mote++;
        } while (i != 16);
        step++;
        member++;
        burst_offset += 16 * sizeof(struct EffectStep);
    } while (member != 9);

    Audio_PlayCue(0x88);
    frame = 0;
    curtain_y = -172;
    do {
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(0x85);
        if (frame <= 23)
            callbacks[0](canvas, (u8 *)work + 0x320 + (frame / 4) * 0x640, 40, 20, 40, 40);
        if (frame == 20)
            Iwram_CopyWords((void *)BG_PLTT,
                Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
        if (frame >= 20 && frame <= 31) {
            if (frame > 23)
                draw[1](canvas, work, 146 - frame * 4, curtain_y, 20, 40);
            else
                callbacks[0](canvas, work, 50, 20, 20, 40);
        }
        if (frame == 32) {
            Audio_PlayCue(0x91);
            work->shake_frames = 8;
            Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work, 1, 1);
        }
        if (frame > 31) {
            member = 0;
            step = work->particles;
            do {
                if (step->variant >= 0 && step->variant <= 47) {
                    s32 cell = step->variant / 8;
                    u32 width;

                    callbacks[0](canvas, (u8 *)work + PuffArc_CellSourceOffsets[cell],
                        step->x - ((width = PuffArc_CellWidths[cell]) >> 1),
                        step->y + PuffArc_CellBiasY[cell],
                        width, PuffArc_CellHeights[cell]);
                }
                member++;
                step->variant++;
                step++;
            } while (member != 9);
        }
        step = (struct EffectStep *)Ram_MapCellBuffer;
        member = 0;
        do {
            if (frame >= (member / 16) * 2 + 40) {
                s32 size;
                s32 x;
                s32 angle;

                size = (member & 1) + 3;
                x = HI(step->x) + ((Trig_Sin(step->z) * 4) >> 16);
                draw[1](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    x - ((u32)size >> 1), HI(step->y) - size, size, size * 2);
                EffectStep_AdvanceWithGravity2D(step, 64, -0x2000);
                angle = step->z;
                step->z = angle + 0x800;
                if (step->z > 0xffff)
                    step->z = angle + 0x800 - 0xffff;
            }
            member++;
            step++;
        } while (member != 144);

        if (frame == 38) {
            for (member = 0; member != work->effect->count; member++) {
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 16);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[member], 6);
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        curtain_y += 8;
        frame++;
    } while (frame != 112);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);

/*
 * Battle-presentation sub-effect at 0x080e0c84.
 *
 * Confirmed member of the 0x03001eec "battle work" subsystem family
 * documented in games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C (owner
 * 080ce85c) and games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/PUFF_ARC.C (owner
 * 080d9fc8): same heap_cache=(void**)0x03001EEC / cursor / work / canvas
 * prologue, same the +0x7828 field=object republish, same
 * BattleFx_BeginCanvasLayer(0)/Scheduler_AddOrUpdateCallback(0x080CD261,0x480)/Scheduler_RemoveCallback(0x080CD261)/
 * Runtime_ReleaseHeapBlock(id)/BattleFx_EndCanvasLayer() bracket, and the same
 * BattleFx_FetchRectangleBlitters(flag, DrawRectangleFn callbacks[2]) two-word blit-routine
 * resolver already established in recon/tbs/en/main/080e01e4.c.
 *
 * Unlike member_orbit's single 64-frame per-member sprite loop, this owner
 * runs a 64-slot randomly-seeded particle pool (fixed-point x/y plus a
 * sin/cos velocity pair) alongside the 64-frame animation loop, and reads
 * every particle's fixed-point position back through the upper halfword of
 * its s32 field -- the same +2 / +6 halfword
 * idiom already confirmed in 080e01e4.c.
 *
 * Every `Func_080072f4`/`Func_08007314` call site is an indirect call
 * through the value the reference loads into r4/r12 immediately before the
 * `bl`, not a real function -- both addresses fall inside the
 * container-built `_call_via_rN` bank at 0x080072e4 (r4 slot at
 * +0x10, ip/r12 slot at +0x30). All such call sites here go through
 * `routine[]`, a two-entry DrawRectangleFn array BattleFx_FetchRectangleBlitters fills.
 *
 * All three Resource_LoadAndDecompress id arguments are loaded from the reference's
 * literal pool rather than built with a `movs` immediate: each is a row of
 * the resource directory, and `(s32)&ResourceId_X` forces the same pool load
 * even though the row numbers would otherwise fit an 8-bit `movs` immediate.
 */

typedef struct {
    s32 x;
    s32 y;
    s32 rot;
    s32 vx;
    s32 vy;
    s32 unk14;
    s32 unk18;
} Particle;

/* The EWRAM scratch buffer holds the effect's particle records. */
extern Particle gMapCellBuffer[];
#define PARTICLE_COUNT 64

extern u8 ParticleReveal_CellWidths[];
extern u8 ParticleReveal_CellHeights[];
extern u16 ParticleReveal_CellSourceOffsets[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_PrepareCanvasEffect(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void BattleFx_FetchRectangleBlitters(s32 flag, DrawRectangleFn *out_callbacks);
void EffectPosition_ApplyAlternateStepAndYOffset(s16 a, s32 *out_pair);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 b);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_EndCanvasLayer(void);

void BattleFx_RunParticleReveal(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    s32 spawn[3];
    s32 screen_y;
    s32 screen_x;
    DrawRectangleFn routine[2];
    Particle *p;
    s32 i;
    s32 frame;
    s32 clamp;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    (*(void **)((u8 *)(work) + (0x7828))) = object;
    BattleFx_BeginCanvasLayer(0);
    BattleFx_PrepareCanvasEffect(object, 1,
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))), 2,
        &screen_x, &screen_y);
    BattleFx_FetchRectangleBlitters(
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))), routine);
    Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work, 1, 1);
    (*(s32 *)((u8 *)(work) + (0x7780))) = 2;
    (*(s32 *)((u8 *)(work) + (0x7784))) = 75;
    {
        s32 interval;
        void *callback;

        interval = 0x480;
        callback = (void *)BattlePresentation_ProcessPendingGraphicsTransfer;
        Scheduler_AddOrUpdateCallback(callback, interval);
    }
    EffectPosition_ApplyAlternateStepAndYOffset(
        (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), spawn);

    for (i = 0; i != PARTICLE_COUNT; i++) {
        s32 angle;
        s32 amp;

        angle = (Random16() & 0x7FFF) + 0x4000;
        amp = (Random16() & 0x1FF) + 0x80;
        gMapCellBuffer[i].x =
            ((spawn[0] / 2 + (Random16() & 0xF)) - 8) << 16;
        gMapCellBuffer[i].y = (spawn[1] + 8) << 16;
        gMapCellBuffer[i].vx = (Trig_Sin(angle) * amp) >> 9;
        gMapCellBuffer[i].vy = (Trig_Cos(angle) * amp) >> 6;
        gMapCellBuffer[i].rot = Random16() & 0x7F;
        gMapCellBuffer[i].unk14 = Random16() & 0x7F;
        gMapCellBuffer[i].unk18 = (Random16() & 0xF) + 32;
    }

    for (frame = 0; frame != 64; frame++) {
        if (frame > 47) {
            (*(s16 *)((u8 *)((void *)0x04000052) + (0))) = (64 - frame) | 0x1000;
        }
        if (frame == 1) {
            Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, (u8 *)work + 0x400, 1, 1);
            Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, (u8 *)work + 0x65C0, 1, 0);
        }

        if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x1C))) == 1) {
            s32 orbit_angle;
            s32 x;
            s32 y;

            orbit_angle = frame << 11;
            x = (((-Trig_Sin(orbit_angle)) << 2) >> 16)
                + screen_x / 2 - 10;
            y = ((Trig_Cos(orbit_angle) << 1) >> 16) + screen_y - 22;
            if (frame > 0x45) {
                y = (y - frame * 2) + 0x8A;
            }
            routine[1](canvas, (u8 *)work + 0x65C0, x, y, 20, 40);
            if (frame <= 3) {
                routine[1](canvas, (u8 *)work + 0x65C0, x, y, 20, 40);
            }
        }

        for (i = 0, p = gMapCellBuffer; i != PARTICLE_COUNT; i++, p++) {
            if (frame >= i / 4 + 4) {
                s32 index;
                s32 w;
                s32 h;

                index = (p->rot / 128) & 3;
                routine[i & 1](
                    canvas, (u8 *)work + 0x400 + ParticleReveal_CellSourceOffsets[index],
                    (*(s16 *)((u8 *)(p) + (2))) - (w = ParticleReveal_CellWidths[index]) / 2,
                    (*(s16 *)((u8 *)(p) + (6))) - (h = ParticleReveal_CellHeights[index]) / 2,
                    w, h);
                EffectStep_AdvanceWithGravity3D(p, 0x3F, 0x1000);
            }
        }

        if (frame == 8) {
            (*(s32 *)((u8 *)(work) + (0x77A8))) = frame;
            BattleEventRuntime_BeginPhaseFar(0x86);
            ObjectGroup_UpdateMembers(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))),
                7, 5, 0, 16);
            BattleMotion_ApplyVariantMotionFar(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), 3);
        }

        clamp = frame * 4;
        if (clamp > 32) {
            clamp = 32;
        }
        if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 0) {
            for (i = 0; i != 5; i++) {
                routine[0](canvas, work, (i << 5) - (frame / 4 & 31),
                    120 - clamp, 32, 32);
            }
        } else {
            for (i = 0; i != 5; i++) {
                routine[0](canvas, work, ((i << 5) + (frame / 4 & 31)) - 32,
                    120 - clamp, 32, 32);
            }
        }

        Camera_ApplyShake(4, 8);
        ObjectGroup_TickMemberTimers();
        (*(s32 *)((u8 *)(work) + (0x7824))) = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(0x2F);
    Runtime_ReleaseHeapBlock(0x2E);
    BattleFx_EndCanvasLayer();
}

extern DrawRectangle gWorkSlot[];
void BattleFx_StepPaletteToResource(s32 resource_id);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);

/* The rock chips: four flip settings, then nine cells with their sizes,
   sheet offsets and places on the rock. */
extern u8 RockToss_ChipFlips[];
extern u8 RockToss_ChipWidths[];
extern u8 RockToss_ChipHeights[];
extern u16 RockToss_ChipCells[];
extern u8 RockToss_ChipX[];
extern u8 RockToss_ChipY[];

/* Battle effect: the small Venus djinn swings in while six rocks leave the
   acting unit one after another, two frames apart, each with four chips
   that change shape and flip every other frame. Sixteen frames after it
   leaves, a rock is pulled towards the target and lands on it. */
void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 i;
    s32 frame;
    DrawRectangle draw[2];
    struct BattleCamera *camera;
    struct MotionObject *target;
    struct MotionObject *actor;
    struct EffectPosition position;
    s32 screen_x;
    s32 screen_y;
    s32 j;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    BattleFx_PrepareCanvasEffect(effect, 0, work->effect->side, 2, &screen_x, &screen_y);
    *(volatile u16 *)0x04000052 = 0x1010;
    if (work->effect->side == 1)
        BattleEffect_LoadWork(46, 7, 7, 7, 2);
    else
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = gWorkSlot[46];
    Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, (u8 *)work + 0x65c0, 1, 1);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    actor = GetBattleObjectSlotFar(work->effect->actor)->object;
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0; i != 8; i++) {
        struct EffectStep *rock = &work->particles[i];

        rock->x = actor->x;
        rock->y = 0x420000;
        rock->z = actor->z;
        rock->velocity_x = (i * 0x500000) >> 5;
        rock->velocity_y = (s32)(((Random16() & 127) - 64) << 16) >> 6;
        rock->velocity_z = (s32)(((Random16() & 255) - 127) << 16) >> 5;
        if (rock->x > 0)
            rock->velocity_x = -rock->velocity_x;
        rock->variant = 1;
    }
    for (frame = 0; frame != 96; frame++) {
        if (frame > 16)
            BattleFx_StepPaletteToResource((s32)&ResourceId_PortalSheet);
        if (work->effect->unknown_001c == 1) {
            s32 angle = frame << 11;
            s32 x = ((-Trig_Sin(angle) << 2) >> 16) + screen_x / 2 - 10;
            s32 y = ((Trig_Cos(angle) << 1) >> 16) + screen_y - 22;

            if (frame > 16)
                y = y - frame * 2 + 32;
            if (work->effect->side == 1)
                BattleEffect_LoadWork(47, 7, 7, 7, 3);
            else
                BattleEffect_LoadWork(47, 7, 7, 3, 3);
            if (frame <= 3)
                gWorkSlot[47](canvas, (u8 *)work + 0x65c0, x, y, 20, 40);
            Runtime_ReleaseHeapBlock(47);
            draw[0](canvas, (u8 *)work + 0x65c0, x, y, 20, 40);
        }
        if ((frame & 1) == 0) {
            for (i = 0; i != 32; i++) {
                struct EffectStep *chip = &work->particles[32 + i];

                chip->velocity_x = Random16() % 6 + 3;
                chip->velocity_y = RockToss_ChipFlips[Random16() & 3];
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        for (i = 0; i != 6; i++) {
            struct EffectStep *rock = &work->particles[i];

            if (rock->variant == 1) {
                if (frame > i * 2) {
                    s32 x;
                    s32 y;

                    EffectPosition_ApplyBaseAndYOffset(&rock->x, &position);
                    position.x >>= 1;
                    x = position.x - 12;
                    y = position.y - 24;
                    draw[0](canvas, work, x, y, 24, 48);
                    if ((frame & 3) <= 1)
                        draw[0](canvas, (u8 *)work + RockToss_ChipCells[1],
                            x + RockToss_ChipX[1], y + RockToss_ChipY[1],
                            RockToss_ChipWidths[1], RockToss_ChipHeights[1]);
                    else
                        draw[0](canvas, (u8 *)work + RockToss_ChipCells[2],
                            x + RockToss_ChipX[2], y + RockToss_ChipY[2],
                            RockToss_ChipWidths[2], RockToss_ChipHeights[2]);
                    for (j = 0; j != 4; j++) {
                        struct EffectStep *chip = &work->particles[32 + i * 4 + j];
                        s32 chip_x;
                        s32 chip_y;

                        BattleEffect_LoadWork(47, 7, 7, chip->velocity_y, 2);
                        draw[1] = gWorkSlot[47];
                        if (chip->velocity_y & 4)
                            chip_x = x - RockToss_ChipWidths[chip->velocity_x]
                                - RockToss_ChipX[chip->velocity_x] + 24;
                        else
                            chip_x = x + RockToss_ChipX[chip->velocity_x];
                        if (chip->velocity_y & 8)
                            chip_y = y - RockToss_ChipHeights[chip->velocity_x]
                                - RockToss_ChipY[chip->velocity_x] + 48;
                        else
                            chip_y = y + RockToss_ChipY[chip->velocity_x];
                        draw[1](canvas, (u8 *)work + RockToss_ChipCells[chip->velocity_x],
                            chip_x, chip_y, RockToss_ChipWidths[chip->velocity_x],
                            RockToss_ChipHeights[chip->velocity_x]);
                        Runtime_ReleaseHeapBlock(47);
                    }
                    rock->x += rock->velocity_x;
                    rock->y += rock->velocity_y;
                    rock->z += rock->velocity_z;
                }
                if (frame > i * 2 + 16) {
                    rock->velocity_x += (target->x - rock->x) >> 8;
                    rock->velocity_y += (0x140000 - rock->y) >> 8;
                    rock->velocity_z += (target->z - rock->z) >> 8;
                    if (frame < i * 2 + 85) {
                        rock->velocity_x = rock->velocity_x * 60 / 64;
                        rock->velocity_y = rock->velocity_y * 60 / 64;
                        rock->velocity_z = rock->velocity_z * 60 / 64;
                    }
                    if (rock->y < 0x140000) {
                        work->shake_frames = 8;
                        rock->variant = 0;
                        Audio_PlayCue(134);
                        ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
                    }
                }
            }
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
