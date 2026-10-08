#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "BTLDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "MENU_LIST.H"


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

#include "FIXED_MATH.H"
#include "RESOURCE.H"
#include "MOTOBJ.H"
#include "B5_CONTEXT.H"
#include "IO_REG.H"



#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)


struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect)
{
    struct EffectPosition ground;
    struct BattleEffectWork *work;
    void *canvas;
    struct BlitterPair draw;
    u8 *sheet;
    void *palette;
    s32 frame;
    s32 i;

    work = (struct BattleEffectWork *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
    canvas = (void *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
    sheet = (u8 *)Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    work->fade_frames = 24;
    work->fade_step = 0;
    REG_BLDALPHA = 0x100c;
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
    switch (work->effect->kind) {
    case 0:
        palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
        break;
    case 1:
        palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
        break;
    case 2:
        palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
        break;
    default:
        palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
        break;
    }
    {
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)0x05000000, palette, 128);
    }

    for (i = 0; i != 128; i++) {
        gFlecks[i].y = 0x800000;
        gFlecks[i].x = Random16() & 0xffff;
        gFlecks[i].z = (Random16() & 0x1ff) + 0x400;
        gFlecks[i].variant = -i;
    }
    for (i = 0; i != 64; i++) {
        work->particles[i].x = Random16() & 0xffff;
        work->particles[i].y = (Random16() & 31) + 16;
        work->particles[i].variant = (i & 15) + 16;
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 3);
    draw.upper = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
    AudioCommand_PlayFar(140);

    for (frame = 0; frame != 56; frame++) {
        EffectPosition_ApplyStepAndYOffset(effect->actor, &ground);
        REG_BG2X = (64 - ground.x) << 8;
        if (frame > 49)
            REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
        if (frame == 26) {
            AudioCommand_PlayFar(212);
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, -1, 0, 20);
        }
        if (frame >= 28 && frame <= 48)
            draw.upper(canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40, ground.y - 24, 48, 48);
        if (frame >= 0 && frame <= 14) {
            s32 picture = frame / 3 % 5;
            DrawRectangle *slot = &((DrawRectangle *)Ram_WorkSlot)[47];

            for (i = 0; i != 4; i++) {
                BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, CornerSparks_DrawFlags[i] | 3, 2);
                draw.lower = *slot;
                draw.lower(canvas, work->sheet + (picture << 10), CornerSparks_X[i] + 32,
                    ground.y + CornerSparks_Y[i] - 32, 32, 32);
                Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            }
        }
        if (frame >= 0) {
            for (i = 0; i != 64; i++) {
                struct EffectStep *mote = &work->particles[i];

                if (mote->variant >= 0 && mote->y > 0) {
                    s32 size = (mote->variant >> 3) + 1;
                    s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
                    s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

                    if (size <= 0)
                        size = 1;
                    draw.upper(canvas, sheet + BattleFx6_FlareCells[size - 1],
                        x - size, y - size, size * 2, size * 2);
                    mote->y -= 2;
                    mote->variant--;
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

void BattlePresentation_DrawStreaks(void)
{
    struct BattleEffectWork *work = *(void **)gBattleFxWork;
    s32 frame;
    s32 i;
    struct Streak *streak;
    struct StreakPoint point;
    struct EffectPosition head;
    struct EffectPosition tail;

    frame = (work->frame)++;
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
    work->transfer_pending = 1;
}

void BattlePresentation_PrepareScene(s32 kind)
{
    struct BattleEffectWork *work;
    void *palette;
    s32 id;

    work = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
    Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
    BattleFx_BeginCanvasLayer(0);
    work->fade_frames = 24;
    REG_BG2PA = 0x100;
    REG_BLDALPHA = 0x1010;
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
    work->frame = 0;
    work->transfer_mode = 3;
    work->transfer_value = 0x06060606;
    Scheduler_AddOrUpdateCallback(BattlePresentation_DrawStreaks, 0xC80);
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
}

void BattleFx_ScheduleCallbacksAndReleaseBlocks(void)
{
    Scheduler_RemoveCallback(BattlePresentation_DrawStreaks);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    {
        s32 (*transfer)(void *, s32) = Iwram_ClearWords;

        transfer((void *)0x06004000, 0x4000);
    }
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
}

void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx)
{
    BattleFx_RunTwoResource(efx, 0);
}

void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx)
{
    BattleFx_RunTwoResource(efx, 1);
}



void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
    struct BattleEffectWork *work;
    void *canvas;
    struct MotionObject *object;
    struct EffectPosition position;
    /* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
    DrawRectangle draw[2];
    s32 frames;
    s32 frame;
    s32 cell;
    void **heap;
    void **p;

    heap = (void **)gBattleFxWork;
    p = heap;
    work = *p++;
    canvas = *p;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    if (work->effect->side == 1) {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
        draw[0] = (DrawRectangle)heap[7];
    } else {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 1);
        draw[0] = (DrawRectangle)heap[7];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
    if (mode == 0) {
        void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
        /* FAKEMATCH: needed to match: the copier address is set before the costly 0x05000000 operand, as the reference loads it */
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)0x05000000, palette, 128);
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    if (work->effect->side == 0)
        REG_BG2X = (16 - position.x) << 8;
    else
        REG_BG2X = (112 - position.x) << 8;
    frames = 74;
    if (mode != 1)
        frames = 48;
    for (frame = 0; frame != frames; frame++) {
        cell = frame / 4;
        if (cell <= 5) {
            if (cell <= 3)
                draw[0](canvas, work->sheet + TwoResource_CellSourceOffsets[cell],
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
                AudioCommand_PlayFar(0x86);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
            }
            work->shake_frames = 8;
        }
        if (mode == 1) {
            if (frame == 13) {
                object->velocity_y = 0xc0000;
                object->vertical_motion_strength = 0x7851;
                object->vertical_motion_phase = 0x4000;
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
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
