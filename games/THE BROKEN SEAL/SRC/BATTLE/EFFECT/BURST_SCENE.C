#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "TRANSFORM.H"

extern u8 gBattleFxWork[];

void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind, s32 side,
    s32 anchor, s32 *out_x, s32 *out_y);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames, s32 speed);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void Render_ResetTransformState(void);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u8 BurstScene_Records[];
extern u16 ParticleStreams_CellOffsets[];

void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant)
{
    struct EffectPosition actor_position;
    struct EffectPosition target_position;
    struct EffectPosition position;
    s32 screen_x;
    s32 screen_y;
    DrawRectangle draw[2];
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    u8 *sheet;
    struct BattleCamera *camera;
    struct MotionObject *target;
    u8 *palette;
    s32 resource;
    s32 row;
    s32 column;
    s32 i;
    s32 j;

    work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
    canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
    sheet = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    REG_BLDALPHA = 0x1010;
    if (work->effect->prelude == 1)
        BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side, 2, &screen_x, &screen_y);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1, 0);
    for (row = 0; row != 288; row++) {
        for (column = 0; column != 40; column++) {
            s32 source = row * 40 + column;
            s32 offset = row * 20 + column / 2 + 0x5100;

            work->sheet[offset] = work->sheet[source];
        }
    }
    if (BurstScene_Records[variant * 7] == 0)
        Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1, 1);
    else
        Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, Ram_MapCellBuffer + 0x5e00, 1, 0);
    switch (BurstScene_Records[variant * 7 + 1]) {
    case 0:
        resource = (s32)&ResourceId_MarsDjinnSheet;
        break;
    case 1:
        resource = (s32)&ResourceId_LimePalette;
        break;
    case 2:
        resource = (s32)&ResourceId_BlueArcSheetB;
        break;
    default:
        resource = (s32)&ResourceId_EmberStreakSheet;
        break;
    }
    palette = Resource_GetTableEntry(resource);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    WaitFrames(1);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_position);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0; i != 768; i++) {
        struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        mote->x = target->x;
        mote->y = target->y + 0x190000;
        mote->z = target->z;
        mote->velocity_x = (Random16() & 255) << 12;
        mote->velocity_y = ((Random16() & 255) - 127) << 12;
        mote->velocity_z = ((Random16() & 255) - 127) << 12;
        if (mote->x > 0)
            mote->velocity_x = -mote->velocity_x;
        mote->variant = -1;
    }
    BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);
    for (frame = 0; frame != BurstScene_Records[variant * 7 + 5]; frame++) {
        s32 bursts = BurstScene_Records[variant * 7 + 2];

        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &actor_position);
        actor_position.x /= 2;
        if (work->effect->side == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 15, 2);
        }
        draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        for (i = 0; i != bursts; i++) {
            s32 start = i * BurstScene_Records[variant * 7 + 4];

            if (frame >= start && frame < start + 6) {
                s32 step = frame - start;

                if ((i & 3) <= 1 || BurstScene_Records[variant * 7] == 1) {
                    if (work->effect->side == 0)
                        draw[i & 1](canvas, work->sheet + step * 3456,
                            target_position.x / 2 - 16, target_position.y - 40, 48, 72);
                    else
                        draw[i & 1](canvas, work->sheet + step * 3456,
                            target_position.x / 2 - 32, target_position.y - 40, 48, 72);
                } else {
                    if (work->effect->side == 0)
                        draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
                            target_position.x / 2 - 16, actor_position.y - 8, 48, 16);
                    else
                        draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
                            target_position.x / 2 - 32, actor_position.y - 8, 48, 16);
                }
            }
            if (frame == start + 2) {
                if (BurstScene_Records[variant * 7 + 6] == 1)
                    Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
                if (i == bursts - 1) {
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
                    work->shake_frames = 8;
                    BattleEventRuntime_BeginPhaseFar(134);
                } else {
                    if (i & 1)
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 7);
                    work->shake_frames = 4;
                    Audio_PlayCue(134);
                }
                for (j = 0; j != BurstScene_Records[variant * 7 + 3]; j++)
                    ((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j].variant = (Random16() & 7) + 15;
            }
            if (frame >= start + 2 && frame < start + 14)
                draw[0](canvas, work->sheet + ((frame - start - 2) / 2) * 960 + 0x5100,
                    target_position.x / 2 - 10, target_position.y - 24, 20, 48);
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
        draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        for (i = 0; i != 512; i++) {
            struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            s32 size = mote->variant;

            if (size > 0) {
                EffectPosition_ApplyBaseAndYOffset(&mote->x, &position);
                position.x /= 2;
                size >>= 3;
                size++;
                draw[(i / 2) & 1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
                if (mote->y < 0x80000)
                    mote->velocity_y = -mote->velocity_y / 2;
                mote->variant--;
            }
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

struct WallScale {
    s32 x;
    s32 y;
};

struct AnimationObject *GetBattleEffectObject(s32 kind);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *object, s32 animation);
void Object_ApplyProjectedPlacementFar(struct AnimationObject *object, s32 *position,
    struct WallScale *scale, s32 mode);
void AudioCommand_PlayFar(s32 cue);

extern u16 RisingWall_RowSheetOffsets[];
extern u8 RisingWall_RowWidths[];
extern u16 RisingWall_MoundSheetOffsets[];
extern u8 RisingWall_MoundWidths[];
extern u8 RisingWall_MoundHeights[];
extern s8 RisingWall_Bg2Shifts[][3];
extern u8 RisingWall_LateRockColumns[];
extern const struct WallScale RisingWall_UnitScale;

#define HI(v) (((s16 *)&(v))[1])

void BattlePres_RunRisingWall(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    u8 *sheet;
    s32 bias;
    s32 offset;
    struct WallScale scale;
    s32 position[4];
    u8 *source;
    u16 *palette;
    void *colors = (void *)0x05000000;
    s32 written;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    if (work->effect->variant == 0)
        REG_BG2PA = 204;
    else if (work->effect->variant == 1)
        REG_BG2PA = 170;
    if (work->effect->side == 1) {
        bias = 8;
        if (work->effect->variant == 0)
            offset = 40;
        else if (work->effect->variant == 1)
            offset = 36;
        else
            offset = 40;
    } else {
        bias = -16;
        offset = -12;
    }
    REG_BG2X =
        RisingWall_Bg2Shifts[work->effect->side][work->effect->variant] << 8;
    Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer, 1, 0);
    Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
    palette = (u16 *)0x05000002;
    for (i = 0; i != 63; i++) {
        s32 color = *palette;
        s32 blue = ((u16)color >> 10) & 31;
        s32 green = ((u16)color >> 5) & 31;
        s32 red = 31;

        red &= color;
        blue -= 8;
        green -= 8;
        red -= 8;

        if (blue < 0)
            blue = 0;
        if (green < 0)
            green = 0;
        if (red < 0)
            red = 0;
        *palette++ = (blue << 10) | (green << 5) | red;
    }
    source = Ram_MapCellBuffer;
    written = 0;
    for (j = 0; j != 9; j++) {
        for (i = 0; i != 32; i++) {
            for (k = 0; k != RisingWall_RowWidths[j]; k++, written++)
                work->sheet[written] = source[k];
        }
        source += RisingWall_RowWidths[j];
    }
    for (j = 0; j != 32; j++) {
        for (i = 0; i != 3; i++) {
            for (k = 0; k != 48; k++, written++)
                work->sheet[written] = source[k];
        }
        source += 48;
    }
    for (i = 0; i != 1008; i++)
        work->sheet[written++] = *source++;
    for (i = 0; i != 3; i++) {
        for (k = 0; k != RisingWall_MoundWidths[i] * RisingWall_MoundHeights[i]; k++)
            work->sheet[written++] = *source++;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    for (i = 0; i != 11; i++) {
        struct AnimationObject *object = GetBattleEffectObject(390);

        work->objects[i] = object;
        if (object != 0) {
            object->flags = 0;
            AnimationObjects_SelectAnimationFar(object, i / 4);
            ((struct AnimationObject *)work->objects[i])->part[0].priority = 1;
        }
    }
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    draw[0] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
    draw[1] = (BattleEffectDrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    REG_BLDCNT = 0x3f46;
    REG_BLDALPHA = 0x1010;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != 11; i++) {
        struct EffectStep *rock = &work->particles[10 + i];

        rock->x = (Random16() & 15) + 88;
        rock->y = 128;
        rock->z = (Random16() & 15) + 2;
        rock->velocity_x = i;
        rock->velocity_y = 1;
        rock->velocity_z = 0x8000;
        rock->variant = 44 - i * 4;
    }
    for (i = 0; i != 6; i++)
        work->particles[16 + i].x = RisingWall_LateRockColumns[i];
    for (i = 0; i != 256; i++) {
        struct EffectStep *dust = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        dust->x = ((Random16() & 63) + bias + 32) << 16;
        dust->y = ((Random16() & 7) + 96) << 16;
        dust->velocity_y = ((Random16() & 63) + 32) << 13;
        dust->variant = Random16() & 31;
    }
    REG_BG2CNT = 0x785;
    work->shake_frames = 250;
    for (frame = 0; frame != 192; frame++) {
        if (frame == 0)
            AudioCommand_PlayFar(212);
        if (frame == 40)
            AudioCommand_PlayFar(141);
        if (frame == 96)
            AudioCommand_PlayFar(145);
        if (frame == 120)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame <= 81) {
            s32 cell = frame / 4;

            if (cell > 2)
                cell = (cell & 1) + 1;
            draw[0](canvas, work->sheet + RisingWall_MoundSheetOffsets[cell], bias - RisingWall_MoundWidths[cell] + 64,
                116 - RisingWall_MoundHeights[cell], RisingWall_MoundWidths[cell], RisingWall_MoundHeights[cell]);
            draw[1](canvas, work->sheet + RisingWall_MoundSheetOffsets[cell], bias + 64,
                116 - RisingWall_MoundHeights[cell], RisingWall_MoundWidths[cell], RisingWall_MoundHeights[cell]);
        }
        if (frame >= 12 && frame <= 87) {
            s32 row = (frame - 64) / 3;

            if (row < 0)
                row = 0;
            if (row > 7)
                row = 7;
            for (i = 0; i != 4; i++) {
                draw[0](canvas, work->sheet + RisingWall_RowSheetOffsets[row], bias - RisingWall_RowWidths[row] + 64,
                    i * 32 - 12, RisingWall_RowWidths[row], 32);
                draw[1](canvas, work->sheet + RisingWall_RowSheetOffsets[row], bias + 64,
                    i * 32 - 12, RisingWall_RowWidths[row], 32);
            }
        }
        if (frame >= 160 && frame <= 183) {
            s32 row = 7 - (frame - 160) / 3;

            if (row < 0)
                row = 0;
            if (row > 7)
                row = 7;
            for (i = 0; i != 4; i++) {
                draw[0](canvas, work->sheet + RisingWall_RowSheetOffsets[row], bias - RisingWall_RowWidths[row] + 64,
                    i * 32 - 12, RisingWall_RowWidths[row], 32);
                draw[1](canvas, work->sheet + RisingWall_RowSheetOffsets[row], bias + 64,
                    i * 32 - 12, RisingWall_RowWidths[row], 32);
            }
        }
        if (frame >= 88 && frame <= 159) {
            s32 width = 48;

            draw[0](canvas, &work->sheet[0x13c0], bias + 16, 0, width, 96);
            draw[1](canvas, &work->sheet[0x13c0], bias + 64, 0, width, 96);
            draw[0](canvas, &work->sheet[0x25c0], bias + 16, 96, width, 21);
            draw[1](canvas, &work->sheet[0x25c0], bias + 64, 96, width, 21);
        }
        if (frame > 87) {
            for (i = 0; i != 64; i++) {
                struct EffectStep *dust = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                if (dust->variant == 0) {
                    s32 size = (i & 3) + 5;

                    draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
                    dust->y -= dust->velocity_y;
                    if (dust->y < 0 && frame <= 159)
                        dust->y = 0x600000;
                } else {
                    dust->variant--;
                }
            }
        }
        if (frame > 4) {
            s32 rocks = 6;

            if (frame > 71)
                rocks = 11;
            for (k = 0; k != rocks; k++) {
                struct EffectStep *rock = &work->particles[10 + k];
                s32 slot;

                if (rock->variant == 0) {
                    scale = RisingWall_UnitScale;
                    if (frame > 71) {
                        scale.x = (k << 12) + 0x8000;
                        scale.x += work->effect->variant << 14;
                    } else {
                        scale.x = 0x8000;
                    }
                    scale.y = scale.x;
                    position[3] = 0;
                    position[0] = (rock->x + offset * 2) << 16;
                    position[1] = 0x2000000 - (rock->y << 16);
                    position[2] = 0x2000000;
                    slot = (frame / 2 + k) % 11;
                    if (slot != -1)
                        Object_ApplyProjectedPlacementFar(work->objects[slot], position, &scale, 0);
                    rock->y -= rock->z;
                    rock->velocity_x += rock->velocity_y;
                    if (rock->velocity_x > 12)
                        rock->velocity_x -= 12;
                    if (rock->y < 0) {
                        if (frame > 159) {
                            rock->variant = -1;
                        } else {
                            if (frame > 87) {
                                rock->z = (Random16() & 7) + 8;
                                if (work->effect->variant == 0)
                                    rock->x = Random16() % 96 + 42;
                                else if (work->effect->variant == 1)
                                    rock->x = Random16() % 112 + 34;
                                else
                                    rock->x = Random16() % 160 + 10;
                            }
                            rock->y = 128;
                            rock->variant = 8;
                        }
                    }
                } else {
                    rock->variant--;
                }
            }
        }
        if (frame <= 158) {
            for (i = 0; i != work->effect->count; i++) {
                if (frame > 85) {
                    if (frame % 12 == 0)
                        ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 6);
                    if ((frame & 3) == 0)
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
                }
            }
        }
        if (frame < 90 || frame > 160)
            Camera_ApplyShake(2, 2);
        else
            Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    for (i = 0; i != 11; i++)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    BattleFx_EndCanvasLayer();
}
