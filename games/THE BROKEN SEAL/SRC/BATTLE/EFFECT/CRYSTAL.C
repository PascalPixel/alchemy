#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "IO_REG.H"
#include "BATTLE_PRESENTATION.H"
#include "MENU_LIST.H"

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];


void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    struct EffectStep *seed;
    struct EffectStep *point;
    struct EffectStep *sparks;
    struct EffectStep *spark;
    s32 x;
    s32 cell;
    s32 i;
    s32 j;

    sparks = (struct EffectStep *)gMapCellBuffer;
    cursor = (void **)(gWorkSlot + 39 * 4);
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    REG_BLDALPHA = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 1152);
    for (j = 0; j != Crystal_Counts[work->effect->variant * 2]; j++) {
        seed = &work->particles[j];
        x = Random16() & 31;
        seed->y = -0x400000;
        if (work->effect->side == 1) {
            x = ((Random16() & 31) + 80) << 16;
            seed->velocity_x = (Random16() & 63) << 12;
        } else {
            x = ((Random16() & 31) + 8) << 16;
            seed->velocity_x = -(Random16() & 63) << 12;
        }
        seed->x = x - seed->velocity_x * 18;
        seed->velocity_y = 0;
        seed->z = 0;
        seed->variant = j * 8;
    }

    for (frame = 0; frame != Crystal_Counts[work->effect->variant * 2 + 1]; frame++) {
        if (work->effect->variant == 2 && frame <= 103) {
            struct CameraWork *camera = *(struct CameraWork **)(gWorkSlot + 12 * 4);
            s32 speed;

            if (frame <= 95)
                speed = 192;
            else
                speed = 2496 - frame * 24;
            if (work->effect->side == 0)
                camera->scroll -= speed;
            else
                camera->scroll += speed;
        }
        if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 80)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == Crystal_Counts[work->effect->variant * 2 + 1] - 8) {
            work->transfer_mode = 3;
            work->transfer_value = 0x06060606;
        }
        if (frame <= Crystal_Counts[work->effect->variant * 2 + 1] - 8) {
            for (i = 0; i != Crystal_Counts[work->effect->variant * 2]; i++) {
                point = &work->particles[i];
                if (point->z == 1) {
                    for (j = 0; j != 16; j++) {
                        cell = (j % 5) * 3 + (sparks[i * 16 + j].variant / 96) % 3;
                        spark = &sparks[i * 16 + j];
                        draw[j <= 2](canvas,
                            work->sheet + Crystal_ShardOffsets[cell] + 2048,
                            ((s16 *)&spark->x)[1] - Crystal_ShardWidths[cell] / 2,
                            ((s16 *)&spark->y)[1] - Crystal_ShardHeights[cell] / 2,
                            Crystal_ShardWidths[cell], Crystal_ShardHeights[cell]);
                        EffectStep_AdvanceWithGravity2D(spark, 64, 0x2000);
                        spark->variant += spark->z;
                        if (spark->z > 1 && (frame & 1))
                            spark->z--;
                    }
                } else if (frame >= point->variant) {
                    draw[i & 1](canvas, work, ((s16 *)&point->x)[1] - 16, ((s16 *)&point->y)[1], 32, 64);
                    EffectStep_AdvanceWithGravity2D(point, 64, 0x10000);
                    if (point->y > 0x380000) {
                        point->z = 1;
                        point->y = 0x380000;
                        for (j = 0; j != 16; j++) {
                            spark = &sparks[i * 16 + j];
                            spark->x = ((Crystal_ShardStarts[j * 2] - 40) << 16) + point->x;
                            spark->y = Crystal_ShardStarts[j * 2 + 1] << 16;
                            spark->velocity_x = ((Random16() & 127) - 64) << 11;
                            spark->velocity_y = -(Random16() & 127) << 11;
                            if (i & 1) {
                                spark->velocity_x *= 2;
                                spark->velocity_y *= 2;
                            }
                            spark->z = 32;
                            spark->variant = 0;
                        }
                        work->shake_frames = 8;
                        AudioCommand_PlayFar(144);
                        for (j = 0; j != work->effect->count; j++)
                            ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 4);
                    }
                }
            }
        }
        Camera_ApplyShake(work->effect->variant * 2 + 4, work->effect->variant * 4 + 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
