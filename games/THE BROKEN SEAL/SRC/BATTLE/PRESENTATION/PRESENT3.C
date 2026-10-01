#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "BATTLE_PRESENTATION.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

extern u8 gBattleFxWork[];
u32 Random16(void);
void Render_ResetTransformState(void);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

struct Streak {
    s32 head;
    s32 tail;
    s32 unused_08;
    s32 pitch;
    s32 yaw;
    s32 roll;
    s32 unused_18;
};

struct StreakPoint {
    s32 distance;
    s32 y;
    s32 z;
};

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode);
s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);
void *Resource_GetTableEntry(s32 id);
void BattlePresentation_DrawStreaks(void);

/*
 * Frame callback that BattlePresentation_PrepareScene schedules at 0xc80.
 * On its first frame it seeds 256 streaks with a random length and a random
 * roll, pitch and yaw; every frame after that it draws the first 64 (one more
 * every four frames) as three lines from tail to head, both projected around
 * the screen point (64, 80), and pulls each streak four units closer to that
 * point. The line colour brightens as the tail reaches the centre.
 */
void BattlePresentation_DrawStreaks(void)
{
    u8 *work = *(u8 **)gBattleFxWork;
    s32 frame;
    s32 i;
    struct Streak *streak;
    struct StreakPoint point;
    struct EffectPosition head;
    struct EffectPosition tail;

    frame = (*(s32 *)(work + 0x778c))++;
    if (frame == 0) {
        for (i = 0; i != 256; i++) {
            struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
            s32 r = Random16() & 15;

            seed->head = r + 48;
            seed->tail = r + 40;
            seed->pitch = Random16() & 0xffff;
            seed->yaw = Random16() & 0xffff;
            seed->roll = Random16() & 0xffff;
        }
    }
    point.y = 0;
    point.z = 0;
    i = 0;
    for (; i != 64; i++) {
        streak = &((struct Streak *)Ram_MapCellBuffer)[i];
        if (frame > i / 4 && streak->head > 0) {
            s32 fade;

            Render_ResetTransformState();
            SceneTransform_ApplyRoll(streak->roll);
            SceneTransform_ApplyPitch(streak->pitch);
            SceneTransform_ApplyYaw(streak->yaw);
            point.distance = streak->head;
            EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &head);
            head.x += 64;
            head.y += 80;
            point.distance = streak->tail;
            EffectPosition_ApplyBaseAndYOffset((s32 *)&point, &tail);
            tail.x += 64;
            tail.y += 80;
            streak->tail -= 4;
            streak->head -= 4;
            if (streak->tail < 0)
                streak->tail = 0;
            fade = -streak->tail / 2;
            BattleFx_DrawClippedCanvasLine(tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
            BattleFx_DrawClippedCanvasLine(tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
            BattleFx_DrawClippedCanvasLine(tail.x, tail.y, head.x, head.y, fade + 56);
        }
    }
    *(s32 *)(work + 0x7824) = 1;
}

/* battle/presentation/misc/prepare.c */
/*
 * Battle presentation setup at 0x080ccaec.  Allocate the kind-39 and kind-40
 * work blocks, reset the presentation, write the BG2PA identity scale and the
 * blend coefficients, stream the palette selected by the caller's kind into
 * palette RAM through the IWRAM word-copy kernel, seed three work-block
 * fields, then schedule two frame callbacks.  The work-block offsets are
 * taken by position and are not verified.
 */

/*
 * ResourceId_ rows are small resource numbers the reference loads from its
 * literal pool rather than materializing, so they must not be folded into
 * constants.  Battle_Apply is declared as returning s32 although the result
 * is unused: the value-returning call form keeps the call the last setter of
 * r0, which is what places each callback's pool load after the order
 * argument's shift.
 */
void BattlePresentation_PrepareScene(s32 kind)
{
    u8 *work;
    void *palette;
    s32 id;

    work = (u8 *)Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);
    BattleFx_BeginCanvasLayer(0);
    *(s32 *)(work + 0x77b4) = 24;
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000052 = 0x1010;
    switch (kind) {
    case 0:
        id = (s32)&ResourceId_YellowPaletteB;
        break;
    case 1:
        id = (s32)&ResourceId_CrystalSheet;
        break;
    case 2:
        id = (s32)&ResourceId_EmberStreakSheet;
        break;
    case 3:
        id = (s32)&ResourceId_VioletPaletteD;
        break;
    case 4:
    default:
        id = (s32)&ResourceId_DustPuffSheet;
        break;
    }
    palette = Resource_GetTableEntry(id);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    *(s32 *)(work + 0x778c) = 0;
    *(s32 *)(work + 0x7780) = 3;
    *(s32 *)(work + 0x7784) = 0x06060606;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_DrawStreaks, 0xC80);
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
}

void BattleFx_ScheduleCallbacksAndReleaseBlocks(void)
{
    Scheduler_RemoveCallback((u32)BattlePresentation_DrawStreaks);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    {
        s32 (*transfer)(void *, s32) = Iwram_ClearWords;

        transfer((void *)0x06004000, 0x4000);
    }
    Scheduler_RemoveCallback((u32)Palette_StepFadeTransfer);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
}

/* battle/effects/two_resource/run_mode0.c */
void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx)
{
    BattleFx_RunTwoResource(efx, 0);
}

/* battle/effects/two_resource/run_mode1.c */
void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx)
{
    BattleFx_RunTwoResource(efx, 1);
}

#if defined(TBS_EDITION_EN)
/* The other editions keep this function in their scaffolds for now. */

void **GetBattleObjectSlotFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 variant);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
s32 BattleFx_EndCanvasLayer(void);

extern u8 TwoResource_CellWidths[];
extern u8 TwoResource_CellHeights[];
extern u16 TwoResource_CellSourceOffsets[];
extern u8 TwoResource_CellX[];
extern s8 TwoResource_CellBiasY[];

/* Plays a six-cell sheet whose first four cells load into the work block
   and last two into the map cell buffer, four frames a cell, while the
   screen shakes; mode 1 runs 74 frames and launches the target, other
   modes 48. */
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2]; /* FAKEMATCH: only draw[0] is used; the pair is the reference's spare frame word */
    s32 *object;
    struct EffectPosition position;
    s32 frames;
    s32 frame;
    s32 cell;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000020 = 0x100;
    if (work->effect->side == 1) {
        BattleEffect_LoadWork(46, 7, 7, 3, 1);
        draw[0] = (DrawRectangle)heap_cache[46 - 39];
    } else {
        BattleEffect_LoadWork(46, 7, 7, 7, 1);
        draw[0] = (DrawRectangle)heap_cache[46 - 39];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
    if (mode == 0) {
        void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
        /* FAKEMATCH: the copier address is set before the costly 0x05000000 operand, as the reference loads it */
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)0x05000000, palette, 128);
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    object = *GetBattleObjectSlotFar(work->effect->actors[0]);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    if (work->effect->side == 0)
        *(s32 *)0x04000028 = (16 - position.x) << 8;
    else
        *(s32 *)0x04000028 = (112 - position.x) << 8;
    frames = 74;
    if (mode != 1)
        frames = 48;
    for (frame = 0; frame != frames; frame++) {
        cell = frame / 4;
        if (cell <= 5) {
            if (cell <= 3)
                draw[0](canvas, (u8 *)work + TwoResource_CellSourceOffsets[cell],
                    TwoResource_CellX[cell + work->effect->side * 6], TwoResource_CellBiasY[cell] + 32,
                    TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
            else
                draw[0](canvas, Ram_MapCellBuffer + TwoResource_CellSourceOffsets[cell],
                    TwoResource_CellX[cell + work->effect->side * 6], TwoResource_CellBiasY[cell] + 32,
                    TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
        }
        if (frame == 8) {
            if (mode == 0) {
                BattleEventRuntime_BeginPhaseFar(0x85);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
            } else {
                Audio_PlayCue(0x86);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
            }
            work->shake_frames = 8;
        }
        if (mode == 1) {
            if (frame == 13) {
                object[10] = 0xc0000;
                object[18] = 0x7851;
                object[17] = 0x4000;
            }
            if (frame == 65) {
                work->shake_frames = 4;
                BattleEventRuntime_BeginPhaseFar(0x86);
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
#endif
