#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];
extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern u8 ParticleField_Counts[][3];
extern u16 ParticleField_PuffCells[];
extern u16 ParticleField_PuffSizes[];

void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode);

/* Mode entries of the particle field effect. */

void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunParticleField(effect, 1);
}

void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunParticleField(effect, 0);
}

/* Per variant: how many motes fly, how many puffs open, how long it runs. */
enum {
    FIELD_MOTES,
    FIELD_PUFFS,
    FIELD_FRAMES
};

/* Battle effect: a blast of motes and opening puffs thrown from one side of
   the screen across the field. Mode 1 runs it over a grey ramp palette and
   leaves the affected units still. */
void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    /* FAKEMATCH: a blitter pair of which only draw[0] is used; the unused
       second slot is the spare word in the reference frame. */
    DrawRectangle draw[2];
    s32 variant;
    struct EffectPosition screen;
    s32 frame;
    s32 i;

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work, 1, 0);
    if (mode == 1) {
        for (i = 0; i != 64; i++) {
            s32 level = i / 2;

            BG_PLTT[i] = (level << 10) | (level << 5) | level;
        }
        variant = 1;
    } else {
        Iwram_CopyWords((void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet), 128);
        variant = work->effect->variant;
    }

    for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (work->effect->side == 1)
            puff->x = 200 << 14;
        else
            puff->x = -(200 << 14);
        puff->y = 0;
        puff->z = 0;
        puff->velocity_x = ((Random16() & 63) - 32) << 13;
        puff->velocity_y = ((Random16() & 63) + 16) << 12;
        puff->velocity_z = ((Random16() & 63) - 32) << 13;
        puff->variant = 0;
    }
    for (i = 0; i != 1024; i++) {
        struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        if (work->effect->side == 1)
            mote->x = 200 << 14;
        else
            mote->x = -(200 << 14);
        mote->y = 0;
        mote->z = 0;
        mote->velocity_x = ((Random16() & 63) - 32) << 13;
        mote->velocity_y = ((Random16() & 31) + 8) << 13;
        mote->velocity_z = ((Random16() & 63) - 32) << 13;
        mote->variant = 0;
    }

    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != ParticleField_Counts[variant][FIELD_FRAMES]; frame++) {
        struct BattleCamera *camera;

        camera = gCameraWork;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame == 2)
            Audio_PlayCue(144);
        if (frame == ParticleField_Counts[variant][FIELD_FRAMES] - 48)
            BattleEventRuntime_BeginPhaseFar(133);

        for (i = 0; i != ParticleField_Counts[variant][FIELD_MOTES]; i++) {
            struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            if (mote->y >= 0) {
                s32 size;

                EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
                screen.x >>= 1;
                screen.x += work->effect->side * 32 - 16;
                if (screen.depth < 160)
                    screen.depth = 160;
                if (screen.depth > 799)
                    screen.depth = 799;
                size = 9 - (screen.depth - 160) / 64;
                draw[0](canvas,
                    (u8 *)work + (ParticleStreams_CellOffsets[size - 1] + (i & 1) * 770) + 0x3200,
                    screen.x - size / 2, screen.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(mote, 64, -0x2000);
            }
        }

        if (frame > 2) {
            for (i = 0; i != ParticleField_Counts[variant][FIELD_PUFFS]; i++) {
                struct EffectStep *puff = &work->particles[i];

                if (i < frame && puff->y >= 0) {
                    EffectPosition_ApplyBaseAndYOffset((s32 *)puff, &screen);
                    screen.x >>= 1;
                    screen.x += work->effect->side * 32 - 16;
                    if (puff->variant >= 0 && puff->variant <= 20) {
                        s32 cel = puff->variant / 3;
                        u32 width;

                        draw[0](canvas, (u8 *)work + ParticleField_PuffCells[cel],
                            screen.x - ((width = ParticleField_PuffSizes[cel]) >> 1),
                            screen.y - (width >> 1), width, width);
                    }
                    if (puff->variant <= 20)
                        puff->variant++;
                    EffectStep_AdvanceWithGravity3D(puff, 64, -0x2000);
                }
            }
        }

        if (mode == 0) {
            for (i = 0; i != work->effect->count; i++) {
                if (frame == i + 6) {
                    ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
                }
            }
        } else {
            for (i = 0; i != work->effect->count; i++) {
                if (frame == i + 6)
                    ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 10);
            }
        }
        if (frame == 2)
            work->shake_frames = 6;
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
