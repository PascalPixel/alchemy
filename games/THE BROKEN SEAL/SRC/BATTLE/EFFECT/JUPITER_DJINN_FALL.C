#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void BattleFx_EndCanvasLayer(void);
void AudioCommand_PlayFar(s32 cue);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: the small Jupiter djinn sheet swings in on a shrinking arc
   while eight flameballs fall one after another; each one that reaches the
   ground bursts into thirty-two motes in the map cell buffer, which fall
   under gravity while their life in variant runs out, and makes every
   affected unit react. */
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *sheet;
    DrawRectangle callbacks[2];
    DrawRectangle *draw;
    struct EffectStep *step;
    struct EffectStep *mote;
    s32 frame;
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
    draw = callbacks;
    BattleFx_FetchRectangleBlitters(0, draw);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet, (u8 *)work + 0x320, 1, 0);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != 32; i++) {
        step = &work->particles[i];
        step->x = (Random16() & 63) + 64;
        step->y = (Random16() & 63) - 80;
    }
    for (i = 0; i != 512; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;
    AudioCommand_PlayFar(171);
    frame = 0;
    angle = 0x8000;
    do {
        if (frame == 56)
            BattleEventRuntime_BeginPhaseFar(133);
        if (frame <= 95) {
            s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
            s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

            callbacks[0](canvas, work, x + 86, y + 28, 20, 40);
        }
        for (i = 0; i != 8; i++) {
            step = &work->particles[i];
            if (frame >= i * 4 + 8 && step->y <= 95) {
                callbacks[0](canvas, (u8 *)work + 0x320, step->x - 20, step->y - 32, 40, 64);
                step->x -= 6;
                step->y += 12;
                if (step->y > 95) {
                    for (j = 0; j != 32; j++) {
                        s32 direction;
                        s32 speed;

                        mote = &((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j];
                        direction = Random16() & 0xffff;
                        speed = (Random16() & 0x1ff) + 256;
                        mote->x = step->x << 16;
                        mote->y = step->y << 16;
                        mote->velocity_x = (Trig_Sin(direction) * speed) >> 7;
                        mote->velocity_y = (Trig_Cos(direction) * speed) >> 6;
                        mote->variant = (Random16() & 15) + 32;
                    }
                    AudioCommand_PlayFar(133);
                    work->shake_frames = 4;
                    for (j = 0; j != work->effect->count; j++) {
                        ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 6);
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
                    }
                }
            }
        }
        for (i = 0; i != 512; i++) {
            mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];
            if (mote->variant != -1) {
                s32 size = mote->variant / 16 + 1;

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
