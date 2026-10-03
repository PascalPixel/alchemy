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
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);

extern u16 PaletteRamp_ShardCells[];
extern u8 PaletteRamp_ShardWidths[];
extern u8 PaletteRamp_ShardHeights[];

void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode);

/* Mode entries of the palette ramp effect. */

void BattleFx_RunPaletteRampMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunPaletteRamp(effect, 0);
}

void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunPaletteRamp(effect, 1);
}

void BattleFx_RunPaletteRampMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunPaletteRamp(effect, 2);
}

void BattleFx_RunPaletteRampMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RunPaletteRamp(effect, 3);
}

/* The ramp image and its seven darker copies, one after another. */
#define RAMP_IMAGE 0x2580
#define RAMP_SIZE 696

/* Battle effect: an emblem (a dagger in mode 0, a shield otherwise) flashes
   over each affected unit in turn, four frames apart, and then two dozen
   shards fly out of it, drawn from an image that darkens as they go. */
void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 j;
    s32 x_offset;
    DrawRectangle *draw;
    struct BattleCamera *camera;
    s32 origin[3];
    struct EffectPosition screen;
    s32 point[3];
    DrawRectangle callbacks[2];
    s32 i;
    s32 palette;

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    if (mode == 0)
        Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work, 1, 1);
    else
        Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work, 1, 1);
    if (mode == 0)
        palette = (s32)&ResourceId_PinkBurstSheet;
    else if (mode == 1)
        palette = (s32)&ResourceId_GlowOrbSheet;
    else
        palette = (s32)&ResourceId_PinkBurstSheet;
    Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage, (u8 *)work + RAMP_IMAGE, 0, 0);

    for (i = 1; i != 8; i++) {
        for (j = 0; j != RAMP_SIZE; j++) {
            s32 level = ((u8 *)work + RAMP_IMAGE)[j];

            if (level > 64 - i * 7)
                level = 64 - i * 7;
            if (level < 0)
                level = 0;
            ((u8 *)work + RAMP_IMAGE)[i * RAMP_SIZE + j] = level;
        }
    }

    if (work->effect->side == 1) {
        REG_BG2X = -0x7000;
        x_offset = -112;
    } else {
        REG_BG2X = 0;
        x_offset = 0;
    }

    for (i = 0; i != 512; i++) {
        struct EffectStep *shard = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 radius = 192;
        s32 angle;

        angle = Random16() & 0xffff;
        shard->x = 0;
        if (mode == 0) {
            shard->y = ((i & 31) / 4 * 6 - 10) << 16;
            shard->z = (i % 4 * 2 - 2) << 16;
        } else {
            shard->y = ((i & 31) / 4 * 6 - 10) << 16;
            shard->z = (i % 4 * 8 - 16) << 16;
        }
        if (work->effect->side == 1)
            shard->velocity_x = 0x20000;
        else
            shard->velocity_x = -0x20000;
        shard->velocity_y = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
        shard->velocity_z = (Trig_Sin(angle) * radius) >> 6;
        shard->variant = Random16() & 0xff;
    }

    BattleFx_FetchRectangleBlitters(work->effect->side, draw = callbacks);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != work->effect->count * 4 + 64; frame++) {
        camera = gCameraWork;
        if (frame == 72)
            BattleEventRuntime_BeginPhaseFar(0);
        for (j = 0; j != work->effect->count; j++) {
            struct MotionObject *object;
            s32 time;
            s32 x;
            s32 y;

            time = frame - j * 4;
            object = GetBattleObjectSlotFar(work->effect->actors[j])->object;
            if (time > 0) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                point[0] = object->x;
                point[1] = 160 << 13;
                point[2] = object->z;
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                SceneTransform_ApplyPosition(point);
                origin[0] = 0;
                origin[1] = 0;
                origin[2] = 0;
                EffectPosition_ApplyBaseAndYOffset(origin, &screen);
                x = screen.x + x_offset;
                y = screen.y;
                if (mode == 0) {
                    if (time <= 26)
                        callbacks[0](canvas, (u8 *)work + time / 4 % 7 * 960,
                            x - 12, y - 20, 24, 40);
                } else {
                    if (time <= 23)
                        draw[1](canvas, (u8 *)work + time / 4 % 6 * 1600,
                            x - 20, y - 20, 40, 40);
                }
                if (time == 24)
                    Audio_PlayCue(143);
                if (time >= 24 && time <= 60) {
                    s32 level;

                    level = 0;
                    if (time > 28) {
                        level = (time - 24) / 4;
                        if (level > 7)
                            level = 7;
                    }
                    for (i = 0; i != 24; i++) {
                        struct EffectStep *shard =
                            &((struct EffectStep *)Ram_MapCellBuffer)[j * 32 + i];
                        s32 cell;

                        cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
                        EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &screen);
                        x = screen.x + x_offset;
                        y = screen.y;
                        callbacks[0](canvas,
                            (u8 *)work + (level * RAMP_SIZE + PaletteRamp_ShardCells[cell]) + RAMP_IMAGE,
                            x, y, PaletteRamp_ShardWidths[cell], PaletteRamp_ShardHeights[cell]);
                        EffectStep_AdvanceWithGravity3D(shard, 60, 0);
                    }
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
