#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];

extern u16 ParticleStreams_CellOffsets[];
extern u16 BattleFx_GlintCellOffsets[];
extern u8 BattleFx_GlintCellWidths[];
extern u8 BattleFx_GlintCellHeights[];

/* Seven bytes for each mode: 0 for the swinging strike and 1 for the
   column, how many times it strikes, which particles fly (1 the wisps, 2 the
   motes that gather on the actor, 4 the glints, 8 the sparks, 16 a harder
   hit), the palette, how many frames one picture of the strike lasts, the
   column's blitter argument and how many frames the effect lasts. */
extern u8 TwelveMode_Records[];
/* The six pictures of the swinging strike: size, place in the sheet and how
   far the strike reaches towards the target. */
extern const u8 TwelveMode_StrikeWidths[];
extern const u8 TwelveMode_StrikeHeights[];
extern const u16 TwelveMode_StrikeOffsets[];
extern const u8 TwelveMode_StrikeReach[];
/* Which picture of the column each step of a strike shows. */
extern const u8 TwelveMode_ColumnStages[];
/* The flip a glint is drawn with, picked at random. */
extern const u8 TwelveMode_GlintDrawFlags[];

/* The rectangle blitters BattleEffect_LoadWork leaves in heap slots 46 and
   47. */
#define gBlitters ((DrawRectangle *)gWorkSlot)
/* The map cell buffer holds 128 motes for each target, then 512 sparks. */
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))
#define HI(v) (((s16 *)&(v))[1])

void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode);

void BattleFx_RunTwelveModeMode6(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 6);
}

void BattleFx_RunTwelveModeMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 3);
}

void BattleFx_RunTwelveModeMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 1);
}

void BattleFx_RunTwelveModeMode10(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 0xA);
}

void BattleFx_RunTwelveModeMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 5);
}

void BattleFx_RunTwelveModeMode9(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 9);
}

void BattleFx_RunTwelveModeMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 4);
}

void BattleFx_RunTwelveModeMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 0);
}

void BattleFx_RunTwelveModeMode8(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 8);
}

void BattleFx_RunTwelveModeMode7(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 7);
}

void BattleFx_RunTwelveModeMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 2);
}

void BattleFx_RunTwelveModeMode11(struct BattleEffectArgument *effect)
{
    BattleFx_RunTwelveMode(effect, 0xB);
}

/* Battle effect: a strike falls on the target again and again, either a
   blade swung across it or a column dropped onto it, as the mode's record
   says. Each hit shakes the target and wakes sparks; wisps rise from the
   ground, motes gather on the actor and glints flash around the target, each
   only in the modes whose record asks for it.

   The second blitter's slot address and the wisp sheet's place are kept in
   variables: the code loads both afresh at every use, as it does a value it
   holds in a variable and has no register for. */
void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode)
{
    /* FAKEMATCH: the existing relative heap-cell transport preserves load and literal ordering; independent typed slot loads change those instructions. */
    struct EffectPosition ground;
    struct EffectPosition target_position;
    struct EffectPosition alternate;
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 k;
    u8 *sheet;
    struct BattleCamera *camera;
    struct MotionObject *target;
    struct MotionObject *source;
    s32 resource;
    s32 i;
    s32 wisp_sheet = 0x3200;
    DrawRectangle *lower = &gBlitters[47];

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    camera = *(struct BattleCamera **)((u8 *)heap_cache -
        (HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_CAMERA) * sizeof(void *));
    work->effect = effect;
    if (mode == 8)
        BattleFx_BeginCanvasLayer(0);
    else
        BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work, 1, 0);
    if (TwelveMode_Records[mode * 7] == 0)
        Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work->sheet + 0xc80, 0, 0);
    else
        Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage, work->sheet + 0xc80, 0, 0);
    switch (TwelveMode_Records[mode * 7 + 3]) {
    case 0:
        resource = (s32)&ResourceId_BlueBeamSheet;
        break;
    case 1:
        resource = (s32)&ResourceId_LimePalette;
        break;
    case 2:
        resource = (s32)&ResourceId_PinkPalette;
        break;
    case 3:
        resource = (s32)&ResourceId_EmberStreakSheet;
        break;
    case 4:
        resource = (s32)&ResourceId_MarsDjinnSheet;
        break;
    case 5:
    default:
        resource = (s32)&ResourceId_LightningBoltSheet;
        break;
    }
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet + 0x3200, 1, 0);

    for (k = 0; k != 1; k++) {
        target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
        for (i = 0; i != 64; i++) {
            struct EffectStep *step = &work->particles[i];

            EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[k], &ground);
            step->x = (ground.x / 2) << 16;
            step->y = 0x500000;
            step->z = 0;
            step->velocity_x = ((Random16() & 255) - 128) << 9;
            step->velocity_y = ((Random16() & 255) - 128) << 9;
            step->velocity_z = 0;
            step->variant = -1;
        }
        for (i = 0; i != 128; i++) {
            struct EffectStep *step = &gMotes[k * 128 + i];

            step->x = target->x;
            step->y = 0x140000;
            step->z = target->z;
            step->velocity_x = ((Random16() & 255) - 128) << 11;
            step->velocity_y = ((Random16() & 255) - 128) << 11;
            step->velocity_z = ((Random16() & 255) - 128) << 11;
            step->variant = -1;
        }
        for (i = 0; i != 512; i++) {
            gSparks[i].x = target->x;
            gSparks[i].y = 0x140000;
            gSparks[i].z = target->z;
            if (TwelveMode_Records[mode * 7] == 1) {
                gSparks[i].velocity_x = ((Random16() & 255) - 128) << 11;
                gSparks[i].velocity_y = ((Random16() & 255) - 128) << 11;
                gSparks[i].velocity_z = ((Random16() & 255) - 128) << 11;
            } else {
                if (TwelveMode_Records[mode * 7 + 2] & 16) {
                    gSparks[i].velocity_x = (Random16() & 0x1ff) << 11;
                    gSparks[i].velocity_y = ((Random16() & 0x1ff) - 256) << 11;
                    gSparks[i].velocity_z = ((Random16() & 0x1ff) - 256) << 11;
                } else {
                    gSparks[i].velocity_x = (Random16() & 255) << 11;
                    gSparks[i].velocity_y = ((Random16() & 255) - 128) << 11;
                    gSparks[i].velocity_z = ((Random16() & 255) - 128) << 11;
                }
                if (gSparks[i].x > 0)
                    gSparks[i].velocity_x = -gSparks[i].velocity_x;
            }
            gSparks[i].variant = -1;
        }
    }

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_position);
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &alternate);
    target_position.y += (alternate.y - target_position.y) / 2;

    for (frame = 0; frame != TwelveMode_Records[mode * 7 + 6]; frame++) {
        s32 span = TwelveMode_Records[mode * 7 + 1] * TwelveMode_Records[mode * 7 + 4];
        s32 length = span * 4;
        s32 glint = 0;

        if (TwelveMode_Records[mode * 7] == 0) {
            if (frame < span * 6) {
                s32 picture = frame / TwelveMode_Records[mode * 7 + 4] % 6;

                if (work->effect->side == 1) {
                    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
                    gBlitters[46](canvas, work->sheet + TwelveMode_StrikeOffsets[picture],
                        target_position.x / 2 - (TwelveMode_StrikeReach[picture] / 2)
                            - TwelveMode_StrikeWidths[picture] + 8,
                        target_position.y - (TwelveMode_StrikeHeights[picture] / 2),
                        TwelveMode_StrikeWidths[picture], TwelveMode_StrikeHeights[picture]);
                } else {
                    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
                    gBlitters[46](canvas, work->sheet + TwelveMode_StrikeOffsets[picture],
                        target_position.x / 2 + (TwelveMode_StrikeReach[picture] / 2) - 8,
                        target_position.y - (TwelveMode_StrikeHeights[picture] / 2),
                        TwelveMode_StrikeWidths[picture], TwelveMode_StrikeHeights[picture]);
                }
                Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
                if (frame % (TwelveMode_Records[mode * 7 + 4] * 6)
                    == TwelveMode_Records[mode * 7 + 4] * 4) {
                    if (mode == 8) {
                        BattleEventRuntime_BeginPhaseFar(134);
                    } else {
                        AudioCommand_PlayFar(133);
                        BattleEventRuntime_BeginPhaseFar(133);
                    }
                    if (TwelveMode_Records[mode * 7 + 2] & 16) {
                        work->shake_frames = 8;
                        ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 12);
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
                        for (i = 0; i != 512; i++)
                            gSparks[i].variant = (Random16() & 15) + 15;
                    } else {
                        work->shake_frames = 4;
                        ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
                        for (i = 0; i != 32; i++) {
                            struct EffectStep *step = &gSparks[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 32 + i];

                            step->variant = (Random16() & 15) + 7;
                        }
                    }
                    for (i = 0; i != 8; i++) {
                        struct EffectStep *step = &work->particles[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i];

                        step->variant = 0;
                    }
                }
            }
            if ((u32)(frame - 12) <= 19)
                glint = 1;
        } else if (frame < length + 4) {
            s32 stage;

            if (frame < length) {
                s32 picture = frame / TwelveMode_Records[mode * 7 + 4];

                while (picture > 4)
                    picture -= 4;
                stage = TwelveMode_ColumnStages[picture];
            } else {
                stage = 3;
            }
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, TwelveMode_Records[mode * 7 + 5]);
            gBlitters[46](canvas, work->sheet + stage * 864 + 0xc80, target_position.x / 2 - 18, 56, 18, 48);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, TwelveMode_Records[mode * 7 + 5]);
            gBlitters[46](canvas, work->sheet + stage * 864 + 0xc80, target_position.x / 2, 56, 18, 48);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
            if (frame % (TwelveMode_Records[mode * 7 + 4] * 4)
                == TwelveMode_Records[mode * 7 + 4] * 3) {
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
                work->shake_frames = 4;
                if (frame > (TwelveMode_Records[mode * 7 + 1] * 4 - 4)
                    * TwelveMode_Records[mode * 7 + 4])
                    BattleEventRuntime_BeginPhaseFar(133);
                else
                    AudioCommand_PlayFar(133);
                for (i = 0; i != 64; i++) {
                    struct EffectStep *step = &gSparks[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 64 + i];

                    step->variant = (Random16() & 15) + 7;
                }
                for (i = 0; i != 8; i++) {
                    struct EffectStep *step = &work->particles[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i];

                    step->variant = 0;
                }
                for (i = 0; i != 16; i++)
                    gMotes[frame / (TwelveMode_Records[mode * 7 + 4] * 6) * 16 + i].variant = 0;
            }
            if (stage == 3)
                glint = 1;
        }

        if ((TwelveMode_Records[mode * 7 + 2] & 4) && glint) {
            for (i = 0; i != 3; i++) {
                s32 image = frame & 3;
                s32 angle = Random16() & 0xffff;
                s32 range = 31;
                s32 radius = Random16() & range;
                s32 x;
                s32 y;

                x = target_position.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17)
                    - (BattleFx_GlintCellWidths[image] / 2);
                y = target_position.y - (Trig_Cos(angle) * (radius + 4) >> 17)
                    - (BattleFx_GlintCellHeights[image] / 2);
                BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, TwelveMode_GlintDrawFlags[Random16() & 3] | 3, 3);
                (*lower)(canvas, work->sheet + BattleFx_GlintCellOffsets[image], x, y,
                    BattleFx_GlintCellWidths[image], BattleFx_GlintCellHeights[image]);
                Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            }
        }

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = gBlitters[46];
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
        draw[1] = *lower;
        for (k = 0; k != 1; k++) {
            source = GetBattleObjectSlotFar(work->effect->actor)->object;
            if (TwelveMode_Records[mode * 7 + 2] & 1) {
                for (i = 0; i != 64; i++) {
                    struct EffectStep *step = &work->particles[i];

                    if ((u32)step->variant <= 23) {
                        s32 picture = step->variant / 4;

                        draw[i & 1](canvas, work->sheet + picture * 1152 + wisp_sheet,
                            HI(step->x) - 12, HI(step->y) - 24, 24, 48);
                        EffectStep_AdvanceWithGravity2D(step, 62, -0x400);
                        step->variant++;
                    }
                }
            }
            if (TwelveMode_Records[mode * 7 + 2] & 2) {
                s32 size = 3;

                if (mode == 11)
                    size = 8;
                if (frame == 55)
                    ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
                if (frame == 90)
                    ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
                for (i = 0; i != 64; i++) {
                    struct EffectStep *step = &gMotes[i];

                    if (step->variant >= 0) {
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &position);
                        position.x >>= 1;
                        draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                            position.x - ((u32)size >> 1), position.y - size, size, size * 2);
                        EffectStep_AdvanceWithGravity3D(step, 60, 0);
                        step->variant++;
                        if (step->variant > 10) {
                            s32 dx = (source->x - step->x) >> 8;
                            s32 dy = (source->y - step->y + 0x140000) >> 8;
                            s32 dz = (source->z - step->z) >> 8;

                            step->velocity_x += dx;
                            step->velocity_y += dy;
                            step->velocity_z += dz;
                            if ((u32)(dx + 0xfff) <= 0x1ffe && (u32)(dz + 0xfff) <= 0x1ffe)
                                step->variant = -1;
                        }
                    }
                }
            }
            if (TwelveMode_Records[mode * 7 + 2] & 8) {
                for (i = 0; i != 128; i++) {
                    struct EffectStep *step = &gSparks[i];
                    s32 size = step->variant;

                    if (size >= 0) {
                        EffectPosition_ApplyBaseAndYOffset((s32 *)step, &position);
                        position.x >>= 1;
                        EffectStep_AdvanceWithGravity3D(step, 60, 0);
                        size >>= 3;
                        size++;
                        draw[1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                            position.x - size / 2, position.y - size, size, size * 2);
                        step->variant--;
                    }
                }
            }
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        if (TwelveMode_Records[mode * 7 + 2] & 16)
            Camera_ApplyShake(8, 8);
        else
            Camera_ApplyShake(2, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
