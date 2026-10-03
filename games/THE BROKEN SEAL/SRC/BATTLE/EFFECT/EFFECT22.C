#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE_IDS.H"
#include "FIXED_MATH.H"
#include "EFFECT_STEP.H"
#include "B5_CONTEXT.H"

extern u8 gMapCellBuffer[];
extern BattleEffectDrawRectangle Data_03001e50[];
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames, s32 speed);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void Audio_PlayCue(s32 cue);

extern const u16 RisingBurst_SparkCells[];
extern const u16 RisingBurst_SparkSizes[];
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant);

/* Mode entries of the column reveal effect. */
void BattleFx_RunRevealColumnMode1(s32 arg0)
{
    BattleFx_RunRevealColumn(arg0, 1);
}

void BattleFx_RunRevealColumnMode2(s32 arg0)
{
    BattleFx_RunRevealColumn(arg0, 2);
}

/* Battle effect: load one of three column pictures (by variant) into the
   work canvas and gMapCellBuffer, move the actor towards its
   target, then for 21 frames blit the four 120x120 quarters in turn, flood
   the canvas with colour 0x3f for frames 16..19, start event phase 134 at
   frame 18 and shake the target at frame 20, with the camera shaking
   throughout. */
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *canvas;
    BattleEffectDrawRectangle draw[2];
    s32 frame;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    work = *heap_cache++;
    canvas = *heap_cache;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000050 = 0;
    if (variant == 0) {
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, gMapCellBuffer, 1, 1);
    } else if (variant == 1) {
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, gMapCellBuffer, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, gMapCellBuffer, 1, 1);
    }
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (variant == 1)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x80000);
    else
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    WaitFrames(16);
    if (work->effect->side == 1)
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 0);
    else
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 0);
    draw[0] = Data_03001e50[46];
    Audio_PlayCue(212);
    for (frame = 0; frame != 21; frame++) {
        if (frame <= 3)
            draw[0](canvas, work, 0, 0, 120, 120);
        else if (frame <= 7)
            draw[0](canvas, (u8 *)work + 0x3840, 0, 0, 120, 120);
        else if (frame <= 11)
            draw[0](canvas, gMapCellBuffer, 0, 0, 120, 120);
        else if (frame <= 15)
            draw[0](canvas, gMapCellBuffer + 0x3840, 0, 0, 120, 120);
        if (frame >= 16 && frame <= 19)
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if (frame == 18)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 20) {
            work->shake_frames = 8;
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

/* Battle effect: sixteen sparks burst up from the target while the loaded
   pillar blitter draws two columns of the lime column sheet beside it from
   frame 8, growing for a while and then sinking again; at frame 10 the event
   phase starts and the target reacts. A spark appears every other frame,
   shrinks through seven cells of the blast sheet as it ages and falls under
   gravity. The pillar blitter is kept in draw[0] as well as called through
   its heap cell. */
void BattleFx_RunRisingBurst(struct BattleEffectArgument *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *sprites;
    DrawRectangleFn draw[2];
    struct EffectPosition position;
    struct EffectStep *spark;
    struct EffectStep *p;
    s32 frame;
    s32 i;
    s32 height;
    s32 kind;
    s32 x;
    u8 *source;
    s32 spread;
    s32 angle;
    s32 cell;
    s32 size;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    sprites = heap_cache[2];
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, (u8 *)work + 0x460, 1, 0);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &position);
    *(volatile u32 *)0x04000028 = (64 - position.x) << 8;
    for (i = 0, spark = work->particles; i != 16; i++) {
        spread = (Random16() & 0x1ff) + 128;
        angle = Random16() & 0xffff;
        spark->x = 0x400000;
        spark->y = 0x700000;
        spark->velocity_x = (Trig_Sin(angle) * spread) >> 8;
        spark->velocity_y = (Trig_Cos(angle) * spread) >> 9;
        spark->variant = Random16() & 7;
        spark++;
    }
    work->shake_frames = 8;
    for (frame = 0; frame != 54; frame++) {
        if (frame == 10) {
            work->shake_frames = 8;
            BattleEventRuntime_BeginPhaseFar(212);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
        }
        if (frame > 7) {
            if (frame <= 31)
                height = frame * 12 - 96;
            else
                height = 272 - frame * 6;
            if (height > 0) {
                if (height > 80) {
                    height = 80;
                    kind = 2;
                } else {
                    kind = 3;
                }
                for (i = 0, x = 50; i != 2; i++, x += 14) {
                    if (i == 0)
                        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, kind);
                    else
                        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, kind);
                    draw[0] = ((DrawRectangleFn)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER]);
                    ((DrawRectangleFn)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER])(canvas, work, x, 112 - height, 14, height);
                    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
                }
            }
        }
        BattleFx_FetchRectangleBlitters(work->effect->side, draw);
        for (i = 0, p = work->particles; i != 16; i++) {
            if (frame >= i / 2 + 8 && p->variant <= 28) {
                s32 px;
                s32 py;

                cell = p->variant / 3;
                px = HI(p->x);
                py = HI(p->y);
                if (cell > 6)
                    cell = 6;
                source = (u8 *)work + RisingBurst_SparkCells[cell] + 0x460;
                size = RisingBurst_SparkSizes[cell];
                draw[0](canvas, source, px - size / 2, py - size / 2, size, size);
                p->variant++;
                EffectStep_AdvanceWithGravity2D(p, 62, -0x2000);
            }
            p++;
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        if (frame <= 7)
            Camera_ApplyShake(2, 2);
        else
            Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
