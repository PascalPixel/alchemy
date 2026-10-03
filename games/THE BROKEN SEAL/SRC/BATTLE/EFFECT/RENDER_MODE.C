#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];
extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames, s32 speed);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 BattleFx_GlintCellOffsets[];
extern u8 BattleFx_GlintCellWidths[];
extern u8 BattleFx_GlintCellHeights[];
extern u8 RenderMode_GlintFlips[];

/* The modes whose particles close in on the unit; the others rise. */
#define CONVERGES(mode) \
    ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);

/* Mode entries of the battle render effect. */
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect)
{
    if (effect->variant == 0) {
        BattleFx_RenderMode(effect, 0);
        return;
    }
    BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 2);
}

void BattleFx_RenderMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 3);
}

void BattleFx_RenderMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 4);
}

void BattleFx_RenderMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 5);
}

void BattleFx_RenderMode7(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 7);
}

void BattleFx_RenderMode6(struct BattleEffectArgument *effect)
{
    BattleFx_RenderMode(effect, 6);
}

/* Battle effect: a cloud of particles around each affected unit, eight
   frames apart, in one of eight modes that choose the picture, how many
   particles there are and whether they close in on the unit or rise. */
void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 total;
    s32 member;
    s32 count;
    struct EffectPosition screen;
    s32 point[3];
    struct EffectPosition base;
    DrawRectangle draw[2];
    void *palette;
    s32 *row;
    s32 i;

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    count = 16;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work, 1, 1);
    if (mode == 0) {
        palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    } else if (mode == 1) {
        palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    } else if (mode == 2) {
        palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
    } else if (mode == 3) {
        palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
    } else if (mode == 4) {
        palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    } else if (mode == 5) {
        Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work, 1, 0);
        palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
    } else if (mode == 7) {
        count = 24;
        Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work, 1, 0);
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    } else {
        count = 32;
        Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work, 1, 0);
        palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
    }
    /* FAKEMATCH: forwarded through Call3, the palette copy loads its routine
       before it shifts the destination, as the reference does. */
    Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT, (s32)palette, 128);
    if (mode == 4)
        Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work, 1, 1);
    if (mode == 3)
        Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, (u8 *)work + 0x2580, 1, 0);

    for (i = 0; i != 512; i++) {
        struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        if (CONVERGES(mode)) {
            mote->x = ((Random16() & 0xff) - 127) << 15;
            mote->y = ((Random16() & 0xff) - 127) << 14;
            mote->z = ((Random16() & 0xff) - 127) << 15;
        } else {
            mote->x = ((Random16() & 0xff) - 127) << 13;
            mote->y = ((Random16() & 0xff) - 255) << 13;
            mote->z = ((Random16() & 0xff) - 127) << 13;
        }
        mote->variant = 0;
    }

    if (CONVERGES(mode))
        total = work->effect->count * 8 + 64;
    else
        total = work->effect->count * 8 + 32;
    if (!(mode <= 1 || mode == 3))
        Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(142);

    for (frame = 0; frame != total; frame++) {
        struct BattleCamera *camera = gCameraWork;

        if (mode == 7) {
            if (frame == total - 46)
                BattleMotion_ApproachTargetFar(work->effect->actor,
                    work->effect->actors[0], 16, 0);
            if (frame == total - 32) {
                BattleEventRuntime_BeginPhaseFar(134);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
                work->shake_frames = 8;
            }
        } else if (frame == total - 32) {
            BattleEventRuntime_BeginPhaseFar(133);
        }
        row = work->bg2_x;
        for (i = 0; i != 160; i++)
            *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *object =
                GetBattleObjectSlotFar(work->effect->actors[member])->object;

            if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
                EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &base);
                for (i = 0; i != 2; i++) {
                    s32 angle;
                    s32 radius;
                    s32 x;
                    s32 y;

                    angle = Random16() & 0xffff;
                    radius = (Random16() & 31) + 4;
                    x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17)
                        - (BattleFx_GlintCellWidths[frame & 3] >> 1);
                    y = base.y - ((Trig_Cos(angle) * radius) >> 16)
                        - (BattleFx_GlintCellHeights[frame & 3] >> 1);
                    BattleEffect_LoadWork(47, 7, 7, 3 | RenderMode_GlintFlips[Random16() & 3], 2);
                    ((DrawRectangle)gWorkSlot[47])(canvas,
                        (u8 *)work + BattleFx_GlintCellOffsets[frame & 3] + 0x2580,
                        x, y + 16,
                        BattleFx_GlintCellWidths[frame & 3],
                        BattleFx_GlintCellHeights[frame & 3]);
                    Runtime_ReleaseHeapBlock(47);
                }
            }
            BattleEffect_LoadWork(46, 7, 7, 3, 3);
            draw[0] = (DrawRectangle)gWorkSlot[46];
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            draw[1] = (DrawRectangle)gWorkSlot[47];
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            point[0] = object->x;
            point[1] = 160 << 13;
            point[2] = object->z;
            SceneTransform_ApplyPosition(point);
            if (frame > member * 8) {
                SceneTransform_ApplyYaw(frame << 9);
                if (mode <= 1 || mode == 4)
                    SceneTransform_ApplyPitch(frame << 9);
                for (i = 0; i != count; i++) {
                    struct EffectStep *mote =
                        &((struct EffectStep *)Ram_MapCellBuffer)[member * 64 + i];
                    s32 distance;

                    if (frame > member * 8 + i) {
                        s32 xx = (mote->x >> 8) * (mote->x >> 8);
                        s32 yy = (mote->y >> 8) * (mote->y >> 8);
                        s32 zz = (mote->z >> 8) * (mote->z >> 8);

                        distance = Iwram_Sqrt(xx + yy + zz) >> 9;
                        if (distance != 0 && mote->variant <= 23) {
                            s32 cel = mote->variant / 4;

                            EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
                            screen.x >>= 1;
                            if (mode == 5 || mode == 7)
                                draw[1](canvas, (u8 *)work + cel * 1600,
                                    screen.x - 20, screen.y - 20, 40, 40);
                            else if (mode == 6)
                                draw[1](canvas, (u8 *)work + 0xc00,
                                    screen.x - 6, screen.y - 12, 12, 24);
                            else if (mode == 4)
                                draw[1](canvas, work,
                                    screen.x - 11, screen.y - 21, 22, 42);
                            else
                                draw[(i & 3) != 0](canvas, (u8 *)work + cel * 1152,
                                    screen.x - 12, screen.y - 24, 24, 48);
                            if (DECAYS(mode)) {
                                mote->x -= mote->x / distance;
                                mote->y -= mote->y / distance;
                                mote->z -= mote->z / distance;
                            } else {
                                mote->y += 0x10000;
                            }
                            mote->variant++;
                            if (mote->variant == 24) {
                                if (DECAYS(mode)) {
                                    mote->variant = 0;
                                } else {
                                    mote->x = ((Random16() & 0xff) - 127) << 13;
                                    mote->y = ((Random16() & 0xff) - 255) << 12;
                                    mote->z = ((Random16() & 0xff) - 127) << 13;
                                }
                            }
                        }
                    }
                }
            }
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
            if (frame == member * 8 + 16) {
                s32 delay = total - frame;

                if (delay > 31)
                    delay = 31;
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, delay);
            }
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    if (!(mode <= 1 || mode == 3))
        Scheduler_RemoveCallback((u32)BattleFx_ArmBg2AffineHBlankDma);
    BattleFx_EndCanvasLayer();
}
