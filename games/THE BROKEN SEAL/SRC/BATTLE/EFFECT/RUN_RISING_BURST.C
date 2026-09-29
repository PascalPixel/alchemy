#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"

extern u8 gBattleFxWork[];
extern const u16 RisingBurst_SparkCells[];
extern const u16 RisingBurst_SparkSizes[];
extern u8 Value_00000051;
extern u8 Value_00000073;
extern u8 Value_000000c0;

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 flag, DrawRectangleFn *out_callbacks);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: sixteen sparks burst up from the target while the loaded
   pillar blitter draws two columns beside it from frame 8, growing for a
   while and then sinking again; at frame 10 the event phase starts and the
   target reacts. A spark appears every other frame, shrinks through seven
   cells as it ages and falls under gravity. The pillar blitter is kept in
   draw[0] as well as called through its heap cell. */
void BattleFx_RunRisingBurst(struct BattleEffectArgument *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *pillars;
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

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    pillars = heap_cache[2];
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress((s32)&Value_00000073, pillars, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_00000051, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_000000c0, (u8 *)work + 0x460, 1, 0);
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
                        BattleEffect_LoadWork(46, 7, 7, 3, kind);
                    else
                        BattleEffect_LoadWork(46, 7, 7, 7, kind);
                    draw[0] = ((DrawRectangleFn *)gBattleFxWork)[7];
                    ((DrawRectangleFn *)gBattleFxWork)[7](canvas, work, x, 112 - height, 14, height);
                    Runtime_ReleaseHeapBlock(46);
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
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
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
