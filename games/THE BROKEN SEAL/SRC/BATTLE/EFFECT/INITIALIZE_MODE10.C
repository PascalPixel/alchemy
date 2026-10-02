#include "TYPES.H"
#include "RESOURCE.H"
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

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];
extern u8 gBattleFxWork[];
extern s32 gCameraWork[];
extern volatile u32 gKeysRepeat;

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_SpawnObjects(s32 count, s32 kind, s32 variant);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
struct BattleEffectTargetArgument;
void BattleFx_SelectLivingTargets(struct BattleEffectTargetArgument *argument);
void BattleFx_PlaceFormationObjects(s32 channel, s32 x, s32 y);
void BattleEffect_RunImpactBurst(s32 channel, s32 x, s32 y);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void *GetMotionRecordFar(struct MotionObject *object, s32 index);
s32 Object_InitializeMode(void *object, s32 mode);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void AudioCommand_PlayFar(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32 x, s32 y);

/* Three pairs of poses for the two middle objects, one pair every 24
   frames. */
extern u8 ThornVines_ObjectPoses[];
/* The pictures in the sheet, each kind with where it starts and how large
   it is: five thorns, three leaves, five chips, five sparks and seven
   embers. */
extern u16 ThornVines_ThornOffsets[];
extern u8 ThornVines_ThornWidths[];
extern u8 ThornVines_ThornHeights[];
extern u16 ThornVines_LeafOffsets[];
extern u8 ThornVines_LeafSizes[];
extern u16 ThornVines_ChipOffsets[];
extern u8 ThornVines_ChipWidths[];
extern u8 ThornVines_ChipHeights[];
extern u16 ThornVines_SparkOffsets[];
extern u8 ThornVines_SparkSizes[];
extern u16 ThornVines_EmberOffsets[];
extern u16 ThornVines_EmberSizes[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])


/* Battle effect: vines grow up through the ground under the enemy side,
   thorns scatter, the picture dissolves through a shuffled dither, and
   rings burst on every affected unit. */
void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 height;
    s32 speed;
    s32 phase;
    s32 rise;
    s32 i;
    s32 j;
    u8 *cells;
    s32 point[3];
    struct EffectPosition screen;
    DrawRectangle draw[2];

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_SpawnObjects(8, 0x177, 1);

    cells = Ram_MapCellBuffer;
    for (i = 0; i != 1024; i++)
        cells[i] = i & 127;
    for (i = 0; i != 8; i++) {
        for (j = 0; j != 128; j++) {
            s32 a = Random16() & 127;
            s32 b = Random16() & 127;
            u8 *to = &cells[i * 128 + b];
            u8 *from = &cells[i * 128 + a];
            u8 t = *to;

            *to = *from;
            *from = t;
        }
    }

    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)gWorkSlot[47];
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    REG_BG2X = -0x2000;

    for (i = 0; i != 16; i++) {
        struct EffectStep *vine = &work->particles[i];

        if (i <= 4) {
            vine->x = i * 20;
            vine->y = (Random16() & 7) + 104;
        } else {
            vine->x = i * 20 - 90;
            vine->y = (Random16() & 7) + 108;
        }
        vine->velocity_y = (Random16() & 7) + 4;
        vine->variant = (Random16() & 15) + 16;
    }
    for (i = 0; i != 16; i++) {
        struct EffectStep *spark = &work->particles[16 + i];

        spark->x = (Random16() & 63) + 32;
        spark->y = (Random16() & 31) + 64;
        spark->variant = -(Random16() & 7) - 8;
    }
    for (i = 0; i != 16; i++) {
        struct EffectStep *thorn = &work->particles[32 + i];

        thorn->x = 0x800000;
        thorn->y = 0x400000;
        thorn->velocity_x = -((Random16() & 255) + 200) << 9;
        thorn->velocity_y = 0;
        thorn->variant = 0;
    }
    BattleFx_SelectLivingTargets((struct BattleEffectTargetArgument *)work->effect);

    height = -0x400000;
    speed = 0;
    for (frame = 0; frame != 366; frame++) {
        s32 facing = gCameraWork[0];

        if ((gKeysRepeat & 3) != 0 && frame > 190 && frame <= 285) {
            Iwram_ClearWords(canvas, 0x4000);
            frame = 286;
        }
        if (frame == 224)
            work->transfer_mode = 0;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame == 31) {
            work->shake_frames = 8;
            AudioCommand_PlayFar(157);
            for (i = 0; i != work->effect->count; i++)
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
        }
        if (frame == 72)
            AudioCommand_PlayFar(136);
        if (frame == 140)
            AudioCommand_PlayFar(156);
        speed += 0x4000;
        height += speed;
        if (height > 0x400000)
            height = 0x400000;
        BattleFx_PlaceFormationObjects(2, 0x800000, height);
        if (frame >= 48 && frame <= 96) {
            s32 pose = (frame - 48) / 24 % 3;

            Object_InitializeMode(work->objects[3], ThornVines_ObjectPoses[pose * 2]);
            Object_InitializeMode(work->objects[4], ThornVines_ObjectPoses[pose * 2 + 1]);
        }
        if (frame >= 72 && frame <= 127) {
            for (i = 0; i != 16; i++) {
                struct EffectStep *thorn = &work->particles[32 + i];

                if (frame >= i + 72 && thorn->y <= 0x67ffff) {
                    s32 cel = (frame + i) / 4 % 5;
                    s32 x = HI(thorn->x);
                    s32 y = thorn->y >> 16;
                    u8 *cell = (u8 *)work + ThornVines_ThornOffsets[cel];
                    u32 width = ThornVines_ThornWidths[cel];
                    u32 tall;

                    x -= width >> 1;
                    tall = ThornVines_ThornHeights[cel];
                    y -= tall >> 1;
                    draw[0](canvas, cell, x, y, width, tall);
                    EffectStep_AdvanceWithGravity2D(thorn, 64, 0x1000);
                }
            }
        }
        if (frame == 128) {
            for (i = 0; i != 48; i++) {
                struct EffectStep *chip = &work->particles[16 + i];

                chip->x = (Random16() % 96) << 16;
                chip->y = ((Random16() & 7) + 88) << 16;
                chip->velocity_x = ((Random16() & 255) - 128) << 11;
                chip->velocity_y = -(Random16() & 255) << 11;
                chip->variant = -(Random16() & 15) - 16;
            }
            REG_BG2X = 0;
        }

        phase = frame - 128;
        if (phase >= 0 && phase <= 96) {
            rise = phase;
            if (rise > 80)
                rise = 80;
            else
                work->shake_frames = 2;
            for (i = 0; i != 10; i++) {
                struct EffectStep *vine = &work->particles[i];

                if (rise > vine->variant) {
                    u8 *sheet = (u8 *)work;
                    s32 top;
                    s32 wrap;
                    s32 k;

                    if (i > 5)
                        sheet = (u8 *)work + 0x6c0;
                    top = (rise - vine->variant) * vine->velocity_y;
                    for (wrap = top; wrap > 184; wrap -= 64)
                        ;
                    if (wrap <= 119)
                        draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
                    for (k = 0; k != 3; k++) {
                        s32 y = vine->y - wrap + k * 64;
                        s32 skip = 0;
                        s32 rows = 64;

                        if (y >= -64) {
                            if (y < 0) {
                                skip = -y * 24;
                                rows = y + 64;
                                y = 0;
                            }
                            if (y + rows > vine->y)
                                rows -= y + rows - vine->y;
                            draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
                        }
                    }
                    if ((i & 1) != 0) {
                        s32 y = ((vine->y - top) & 127) - 16;
                        s32 kind = i % 3;
                        s32 full = ThornVines_LeafSizes[kind];
                        s32 rows = full;

                        if (y + rows > vine->y)
                            rows -= y + rows - vine->y;
                        if (rows > 0)
                            draw[i & 1](canvas, (u8 *)work + ThornVines_LeafOffsets[kind],
                                vine->x + 8, y, full, rows);
                    }
                }
            }
        }
        if (phase >= 0 && phase <= 95) {
            for (i = 0; i != 32; i++) {
                struct EffectStep *chip = &work->particles[32 + i];

                if (chip->variant >= 0) {
                    s32 cel = i % 5;

                    draw[0](canvas, (u8 *)work + ThornVines_ChipOffsets[cel],
                        HI(chip->x), HI(chip->y), ThornVines_ChipWidths[cel], ThornVines_ChipHeights[cel]);
                    chip->x += chip->velocity_x;
                    chip->y += chip->velocity_y;
                    chip->velocity_y += 0x4000;
                    if ((u32)chip->y > 0x780000 && frame <= 159) {
                        chip->x = (Random16() % 96) << 16;
                        chip->y = ((Random16() & 7) + 88) << 16;
                        chip->velocity_x = ((Random16() & 255) - 128) << 11;
                        chip->velocity_y = -(Random16() & 255) << 11;
                    }
                }
                chip->variant++;
            }
        }

        if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
            u16 *color = (u16 *)BG_PLTT;

            for (i = 0; i != 64; i++) {
                s32 red = *color & 31;
                s32 green = (*color >> 5) & 31;
                s32 blue = (*color >> 10) & 31;
                s32 grey = (red + green + blue) / 3;

                if (red > grey)
                    red--;
                if (red < grey)
                    red++;
                if (green > grey)
                    green--;
                if (green < grey)
                    green++;
                if (blue > grey)
                    blue--;
                if (blue < grey)
                    blue++;
                *color++ = (blue << 10) | (green << 5) | red;
            }
        }

        if (phase >= 0 && phase <= 172) {
            for (i = 0; i != work->effect->count; i++) {
                struct MotionObject *object =
                    GetBattleObjectSlotFar(work->effect->actors[i])->object;

                point[0] = object->x;
                point[1] = object->y;
                point[2] = object->z;
                EffectPosition_ApplyBaseAndYOffset(point, &screen);
                for (j = 0; j != 6; j++) {
                    struct EffectStep *spark = &work->particles[16 + i * 6 + j];

                    if (spark->variant == 0) {
                        spark->x = screen.x + (Random16() & 15) - 8;
                        spark->y = screen.y + (Random16() & 15) - 40;
                    }
                    if ((u32)spark->variant <= 4) {
                        u8 *cell = (u8 *)work + ThornVines_SparkOffsets[spark->variant];
                        u32 size = ThornVines_SparkSizes[spark->variant];
                        u32 half = size >> 1;

                        draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
                    }
                    spark->variant++;
                    if (frame <= 199 && spark->variant == 5)
                        spark->variant = -(Random16() & 7);
                }
            }
        }

        if (frame > 232) {
            s32 row = frame * 2 - 496;

            for (i = 0; i != 32; i++) {
                for (j = 0; j != 4; j++) {
                    s32 y = row + i;

                    if ((u32)y <= 127) {
                        s32 x = cells[((y & 7) * 32 + i) * 4 + j];

                        ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8
                            + (x & 7)] = 0;
                    }
                }
            }
            row++;
            for (i = 0; i != 32; i++) {
                for (j = 0; j != 4; j++) {
                    s32 y = row + i;

                    if ((u32)y <= 127) {
                        s32 x = cells[((y & 7) * 32 + i) * 4 + j];

                        ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8
                            + (x & 7)] = 0;
                    }
                }
            }
        }

        if (frame >= 161 && frame <= 223) {
            for (i = 0; i != work->effect->count; i++) {
                if (frame > i * 8 + 160) {
                    struct BattleObjectSlot *slot =
                        GetBattleObjectSlotFar(work->effect->actors[i]);
                    struct MotionObject *object = slot->object;
                    void *record;
                    s32 n;

                    object->y += 0x80000;
                    if (object->y > 0x800000)
                        object->y = 0x800000;
                    object->vertical_motion_strength = 0;
                    for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
                        Object_InitializeMode(record, 5);
                }
            }
        }
        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 5 + 286) {
                struct MotionObject *object =
                    GetBattleObjectSlotFar(work->effect->actors[i])->object;

                object->y = 0x600000;
                object->vertical_motion_strength = 0xab85;
            }
            if (frame == i * 5 + 302) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
                AudioCommand_PlayFar(134);
                work->shake_frames = 8;
            }
        }

        if (frame == 302) {
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
            Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work, 1, 0);
            Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, (u8 *)work + 0x1680, 1, 1);
            BattleEffect_LoadWork(46, 7, 7, 3, 2);
            draw[0] = (DrawRectangle)gWorkSlot[46];
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            draw[1] = (DrawRectangle)gWorkSlot[47];
            REG_BLDCNT = 0x3f46;
            REG_BG2PA = 0x80;
            REG_BG2X = 0;
            work->transfer_mode = 2;
            work->transfer_value = 75;
            Scheduler_AddOrUpdateCallback(
                (s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
            for (i = 0; i != work->effect->count; i++) {
                struct MotionObject *object =
                    GetBattleObjectSlotFar(work->effect->actors[i])->object;

                for (j = 0; j != 10; j++) {
                    struct EffectStep *ember = &work->particles[i * 10 + j];

                    ember->x = object->x;
                    ember->y = 0x140000;
                    ember->z = object->z;
                    ember->velocity_x = Trig_Sin(j * 0x3334) << 2;
                    ember->velocity_y = (Random16() & 0x7fff) + 0x10000;
                    ember->velocity_z = Trig_Cos(j * 0x3334) << 2;
                    ember->variant = 0;
                }
            }
        }
        if (frame > 301) {
            for (i = 0; i != work->effect->count; i++) {
                s32 start = i * 4 + 302;

                if (frame >= start && frame < i * 4 + 314) {
                    s32 cel = (frame - start) / 2;
                    struct MotionObject *object =
                        GetBattleObjectSlotFar(work->effect->actors[i])->object;

                    point[0] = object->x;
                    point[1] = 0;
                    point[2] = object->z;
                    EffectPosition_ApplyBaseAndYOffset(point, &screen);
                    screen.x >>= 1;
                    draw[0](canvas, (u8 *)work + cel * 480,
                        screen.x - 20, screen.y - 24, 20, 24);
                    draw[1](canvas, (u8 *)work + cel * 480,
                        screen.x, screen.y - 24, 20, 24);
                }
                if (frame >= start + 6) {

                    for (j = 0; j != 5; j++) {
                        struct EffectStep *ember = &work->particles[i * 10 + j];

                        EffectPosition_ApplyBaseAndYOffset((s32 *)ember, &screen);
                        screen.x >>= 1;
                        if ((u32)ember->variant <= 26) {
                            s32 cel = 6;
                            s32 offset = ThornVines_EmberOffsets[cel];
                            u32 size = ThornVines_EmberSizes[cel];

                            draw[0](canvas, (u8 *)work + offset,
                                screen.x - (size >> 1), screen.y - (size >> 1), size, size);
                        }
                        EffectStep_AdvanceWithGravity2D(ember, 60, 0x1000);
                        ember->variant++;
                    }
                }
            }
        }

        if (frame <= 127)
            Camera_ApplyShake(4, 16);
        else if (frame <= 301)
            Camera_ApplyShake(2, 2);
        else
            Camera_ApplyShake(4, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleEventRuntime_BeginPhaseFar(134);
    BattleEffect_RunImpactBurst(2, 0x800000, height);
    for (i = 0; i != 8; i++)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    BattleFx_EndCanvasLayer();
}

/* Battle effect mode 10: rocks burst from the ground around the targets
   while nine sprite objects rise in a 3x3 block, then boulders rain down.
   Phase one runs 288 frames (A or B skips it after frame 16); phase two
   runs 146. */
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "IO_REG.H"
extern u8 gMapCellBuffer[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BuildWindowEdgeTable(void);

void WaitFrames(s32);
u32 Random16(void);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattleFx_SpawnObjects(s32, s32, s32);
void Render_ResetTransformState(void);
void AnimationObjects_SelectAnimationFar(void *, s32);
void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEventRuntime_BeginPhaseFar(s32);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *, s32, s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void Camera_ApplyShake(s32, s32);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);

extern volatile u32 gKeysRepeat;

struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};
extern struct Cells03001ad0 gBgScroll;

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};
extern struct Cells03001ce0 gProjection;


typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

extern const Scale BattleFx10_UnitScale;
extern const u8 BattleFx10_Points[][2];
extern const s8 BattleFx10_ShakeOffsets[];
extern const u16 BattleFx10_RockCells[];
extern const u8 BattleFx10_RockWidths[];
extern const u8 BattleFx10_RockHeights[];
extern const u8 BattleFx10_Animations[];
extern const u8 BattleFx10_DebrisWidths[];
extern const u8 BattleFx10_DebrisHeights[];
extern const u16 BattleFx10_DebrisCells[];
extern const u8 BattleFx10_SprayWidths[];
extern const u8 BattleFx10_SprayHeights[];
extern const u16 BattleFx10_SprayCells[];
extern const u16 BattleFx10_BoulderCells[];
extern const u8 BattleFx10_BoulderWidths[];
extern const u8 BattleFx10_BoulderHeights[];
extern const u8 BattleFx10_FallWidths[];
extern const u8 BattleFx10_FallHeights[];
extern const u16 BattleFx10_FallCells[];
extern const u16 BattleFx_PuffCells[];
extern const u8 BattleFx_PuffSizes[];

/* The effect's work block (heap slot 39). */
struct Mode10Work {
    u8 sheet[0x7080];
    struct EffectStep sparks[64];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake;
    u8 unknown_77ac[0x2c];
    void *objects[9];
    u8 unknown_77fc[0x28];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define TRAILS ((struct EffectStep *)(gMapCellBuffer + 0xe00))
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_InitializeMode10(struct BattleEffectArgument *efx)
{
    s32 pts[32][2];
    s32 pos[4];
    void *dst;
    struct Mode10Work *work;
    s32 frame;
    DrawRectangle blit46;
    DrawRectangle blit47;
    u8 *aux;
    s32 step;
    s32 row;
    u32 *cache;
    s32 count;
    s32 i;
    s32 j;
    s32 ox;
    s32 oy;
    s32 y;
    s32 t;
    struct EffectStep *p;
    struct EffectStep *q;
    Scale scale;

    cache = (u32 *)((u8 *)gWorkSlot + 40 * 4);
    dst = (void *)cache[40 - 40];
    work = (struct Mode10Work *)cache[39 - 40];
    aux = (u8 *)cache[41 - 40];
    step = 1;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    *(u16 *)0x04000048 = 0x2137;
    BattleEffect_WipeCanvas(1, 0);
    *(u16 *)0x04000040 = 0xf0f0;
    Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, aux, 0, 0);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    blit46 = (DrawRectangle)cache[46 - 40];
    BattleEffect_LoadWork(47, 7, 7, 3, 1);
    blit47 = (DrawRectangle)cache[47 - 40];
    gProjection.unk10 = 240;
    BattleFx_SelectLivingTargets((struct BattleEffectTargetArgument *)work->effect);
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
    BattleFx_SpawnObjects(9, 0x174, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
    work->transfer_mode = 2;
    work->transfer_value = 50;

    for (i = 0; i != 64; i++) {
        PARTICLES[i].x = (Random16() & 63) + 32;
        PARTICLES[i].y = (Random16() & 31) + 120;
        PARTICLES[i].variant = -1;
    }

    for (i = 0; i != 32; i++) {
        pts[i][0] = BattleFx10_Points[i][0];
        pts[i][1] = BattleFx10_Points[i][1];
    }

    Audio_PlayCue(141);

    /* FAKEMATCH: the rock count resets inside the loop test, after the
       frame limit and before the skip keys are tested. */
    for (frame = 0; frame != 288 && (count = 16, !(gKeysRepeat & 3) || frame <= 16); frame++) {
        if (frame >= 0 && frame < 16) {
            u16 *phase = (u16 *)gMapCellBuffer;
            if (frame == 1) {
                u8 *noise = Ram_MapCellBuffer + 2;
                s32 n;
                for (n = 0; n != 128; n++) {
                    noise[n] = Random16() & 63;
                }
                *phase = 0;
                Scheduler_AddOrUpdateCallback((s32)BattleFx_BuildWindowEdgeTable, 0x480);
            }
            *phase += step;
            step += 3;
            if (frame == 15) {
                Scheduler_RemoveCallback((s32)BattleFx_BuildWindowEdgeTable);
            }
        }

        if (frame > 103) {
            count = 0;
        } else if (frame > 63) {
            count = 6;
        } else if (frame > 31) {
            count = 10;
        }

        if (frame <= 167) {
            ox = (Random16() & 3) - 1;
            oy = (Random16() & 3) - 1;
            gBgScroll.unk04 = ox;
            gBgScroll.unk06 = oy + 32;
        } else {
            ox = 0;
            oy = 0;
            gBgScroll.unk04 = 0;
            gBgScroll.unk06 = 32;
        }
        if (frame >= 176 && frame < 180) {
            ox = -BattleFx10_ShakeOffsets[frame - 176];
            oy = BattleFx10_ShakeOffsets[frame - 176];
            gBgScroll.unk04 = ox;
            gBgScroll.unk06 = oy + 32;
        }

        for (i = 0; i != count; i++) {
            s32 n = i % 3;
            blit46(dst, work->sheet + BattleFx10_RockCells[n],
                BattleFx10_Points[i][0] - ox, BattleFx10_Points[i][1] - BattleFx10_RockHeights[n] - oy,
                BattleFx10_RockWidths[n], BattleFx10_RockHeights[n]);
        }

        scale = BattleFx10_UnitScale;
        if (frame == 174) {
            for (i = 0; i != 9; i++) {
                ((struct { u8 pad[9]; u8 tile_hi : 2; u8 priority : 2; u8 palette : 4; } *)work->objects[i])->priority = 0;
            }
        }
        if (frame > 208) {
            AnimationObjects_SelectAnimationFar(work->objects[2], BattleFx10_Animations[(frame / 4) & 3]);
        }

        scale.x = 0x10000;
        scale.y = 0x10000;
        pos[3] = 0;
        pos[1] = 0xff0000;
        for (row = 0; row != 3; row++) {
            for (j = 0; j != 3; j++) {
                pos[0] = (j * 32 + 144 - ox) << 16;
                pos[2] = (row * 32 + 76 - oy) << 16;
                Object_ApplyProjectedPlacementFar(work->objects[row * 3 + j], pos, &scale, 0);
            }
        }

        t = frame - 160;
        if (t >= 0 && t < 158) {
            s32 u = frame - 208;
            s32 sx = 80;
            s32 sy = 8;
            if (frame <= 175) {
                sx = 96 - t;
                sy = t * 4 - 56;
            } else if (frame > 208) {
                sy = u / 4 + 8;
            }
            blit47(dst, work->sheet + 0xc46, sx, sy, 24, 48);
        }

        if (frame == 32) {
            Audio_PlayCue(134);
        }
        if (frame == 64) {
            Audio_PlayCue(134);
        }
        if (frame == 104) {
            Audio_PlayCue(134);
        }
        if (frame == 176) {
            Audio_PlayCue(134);
        }
        if (frame == 226) {
            Audio_PlayCue(145);
        }
        Render_ResetTransformState();

        if (frame == 32) {
            for (i = 0; i != 32; i++) {
                PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
                PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
                PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
                PARTICLES[i].velocity_y = ((-Random16() & 127) - 64) << 11;
                PARTICLES[i].variant = (Random16() & 15) + 32;
            }
        }
        if (frame == 64) {
            for (i = 0; i != 32; i++) {
                PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
                PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
                PARTICLES[i + 32].velocity_x = ((Random16() & 127) - 63) << 12;
                PARTICLES[i + 32].velocity_y = ((-Random16() & 31) - 32) << 13;
                PARTICLES[i + 32].variant = (Random16() & 15) + 32;
            }
        }
        if (frame == 104) {
            for (i = 0; i != 32; i++) {
                PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
                PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
                PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
                PARTICLES[i].velocity_y = ((-Random16() & 31) - 32) << 13;
                PARTICLES[i].variant = (Random16() & 15) + 32;
            }
        }

        if (frame >= 32 && frame < 208) {
            for (i = 0; i != 64; i++) {
                if (PARTICLES[i].variant >= 0) {
                    s32 n;
                    if (frame > 191) {
                        n = i % 7 + 4;
                    } else {
                        n = i & 3;
                    }
                    blit47(dst, work->sheet + BattleFx10_DebrisCells[n], HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                        BattleFx10_DebrisWidths[n], BattleFx10_DebrisHeights[n]);
                    PARTICLES[i].x += PARTICLES[i].velocity_x;
                    PARTICLES[i].y += PARTICLES[i].velocity_y;
                    PARTICLES[i].velocity_y += 0x2000;
                }
            }
        }

        if (frame > 223) {
            if (frame == 224) {
                for (i = 0; i != 128; i++) {
                    PARTICLES[i].x = 0x480000;
                    PARTICLES[i].y = 0x380000;
                    PARTICLES[i].velocity_x = (-(Random16() & 127) - 64) << 11;
                    PARTICLES[i].velocity_y = ((Random16() & 127) + 16) << 11;
                    PARTICLES[i].variant = Random16();
                }
            }
            for (i = 0; i != 128; i++) {
                if (frame >= i / 4 + 224) {
                    s32 n = i % 3;
                    if ((i & 1) == 0) {
                        blit46(dst, work->sheet + BattleFx10_SprayCells[n], HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                            BattleFx10_SprayWidths[n], BattleFx10_SprayHeights[n]);
                    }
                    PARTICLES[i].x += PARTICLES[i].velocity_x;
                    PARTICLES[i].y += PARTICLES[i].velocity_y;
                    if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
                        PARTICLES[i].x = 0x480000;
                        PARTICLES[i].y = 0x380000;
                    }
                    PARTICLES[i].variant++;
                }
            }
            if (frame == 228) {
                for (i = 0; i != 128; i++) {
                    p = &PARTICLES[i];
                    q = &TRAILS[i];
                    q->x = p->x;
                    q->y = p->y;
                    q->variant = 0;
                }
            }
            for (i = 0, q = TRAILS, p = PARTICLES; i != 128; i++, q++, p++) {
                if (frame >= i + 228) {
                    s32 n = q->variant / 2 % 9;
                    s32 sz = BattleFx_PuffSizes[n];
                    blit46(dst, aux + BattleFx_PuffCells[n], HI(q->x) - sz / 2, HI(q->y) - sz / 2, sz, sz);
                    if (++q->variant == 18) {
                        q->x = p->x;
                        q->y = p->y;
                        q->variant = 0;
                    }
                }
            }
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    for (i = 0; i != 9; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }
    BattleEffect_SetupBlendedDisplay();
    *(u16 *)0x04000052 = 0x1010;
    Audio_PlayCue(0x121);

    for (i = 0; i != 64; i++) {
        work->sparks[i].x = ((Random16() & 127) + 64) << 16;
        work->sparks[i].z = -0x100000;
        work->sparks[i].y = (-(Random16() & 127) - 64) << 16;
        work->sparks[i].velocity_x = (-(Random16() & 63) - 127) << 12;
        work->sparks[i].velocity_y = ((Random16() & 63) + 127) << 12;
        work->sparks[i].variant = 0;
    }
    for (i = 0; i != 64; i++) {
        PARTICLES[i].x = Random16() & 127;
        PARTICLES[i].y = (Random16() & 63) + i / 2;
        PARTICLES[i].variant = -i / 2;
    }

    for (frame = 0; frame != 146; frame++) {
        count = 0;
        if (frame == 96) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        if (frame == 16) {
            work->shake = 32;
        }
        if (frame > 16) {
            count = (frame - 16) / 2;
            if (count > 16) {
                count = 16;
            }
        }
        if (frame >= 9 && frame < 72 && (frame & 3) == 0) {
            Audio_PlayCue(132);
        }
        if (frame == 72) {
            Audio_PlayCue(145);
        }
        if (frame > 64) {
            blit46(dst, work->sheet + 0x14f9, (64 - frame) * 7 + 88, frame * 14 - 992, 40, 80);
        }
        if (frame <= 71) {
            for (i = 0; i != count; i++) {
                s32 n = i % 3;
                blit46(dst, work->sheet + BattleFx10_BoulderCells[n],
                    BattleFx10_Points[i][0] - 56, BattleFx10_Points[i][1] - BattleFx10_BoulderHeights[n],
                    BattleFx10_BoulderWidths[n], BattleFx10_BoulderHeights[n]);
            }
        }
        if (frame == 72) {
            for (i = 0; i != 64; i++) {
                work->sparks[i].x = (BattleFx10_Points[i & 15][0] - 56) << 16;
                work->sparks[i].y = BattleFx10_Points[i & 15][1] << 16;
                work->sparks[i].velocity_x = ((Random16() & 127) - 63) << 13;
                work->sparks[i].velocity_y = (-(Random16() & 31) - 16) << 14;
            }
            if (frame == 72) {
                work->shake = 4;
            }
        }
        if (frame > 71) {
            count = 32;
            if (frame != 72) {
                count = 64;
            }
            for (i = 0; i != count; i++) {
                q = &work->sparks[i];
                y = HI(q->y);
                if (y <= 135) {
                    s32 n = i % 3;
                    blit46(dst, work->sheet + BattleFx10_FallCells[n], HI(q->x), y - BattleFx10_FallHeights[n],
                        BattleFx10_FallWidths[n], BattleFx10_FallHeights[n]);
                    EffectStep_AdvanceWithGravity2D(q, 64, 0x10000);
                    if (HI(q->y) > 120 && q->velocity_y > 0x80000) {
                        q->velocity_y = -q->velocity_y / 4;
                        q->y = 0x780000;
                        work->shake = 1;
                    }
                }
            }
        }
        if (frame <= 71) {
            for (i = 0; i != 64; i++) {
                blit46(dst, work->sheet, HI(work->sparks[i].x) - 12, HI(work->sparks[i].y) - 24, 24, 48);
                work->sparks[i].x += work->sparks[i].velocity_x;
                work->sparks[i].y += work->sparks[i].velocity_y;
                if ((work->sparks[i].y >> 16) > 120 && frame <= 47) {
                    work->sparks[i].x = ((Random16() & 127) + 64) << 16;
                    work->sparks[i].y = -0x100000;
                }
            }
        }

        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 8 + 32) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
            }
        }

        if (frame > 72) {
            for (i = 0; i != 64; i++) {
                if ((u32)PARTICLES[i].variant < 18) {
                    s32 n = PARTICLES[i].variant / 2 % 9;
                    s32 sz = BattleFx_PuffSizes[n];
                    blit46(dst, aux + BattleFx_PuffCells[n], PARTICLES[i].x - sz / 2, PARTICLES[i].y - sz / 2, sz, sz);
                }
                if (++PARTICLES[i].variant == 18 && frame <= 127) {
                    PARTICLES[i].x = Random16() & 127;
                    PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
                    PARTICLES[i].variant = 0;
                }
            }
        }

        if (frame >= 72 && frame < 80) {
            Camera_ApplyShake(8, 8);
        } else {
            Camera_ApplyShake(2, 2);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

void *Resource_GetTableEntry(s32 id);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern s32 IceShardBursts_Gravities[];

#define SHARDS_PER_MEMBER 128

/* Battle effect: every affected unit in turn, twenty frames apart, bursts
   into 128 ice shards. Each shard starts on a random ring around the unit,
   flies up and outwards and falls under one of four gravities until it has
   dropped below the ground line. The shards of unit n lie n * 128 records
   into the map cell buffer. */
void BattleFx_RunIceShardBursts(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw;
    void *sheet;
    s32 facing;
    s32 point[3];
    struct EffectPosition screen;
    s32 member;
    s32 frame;
    s32 i;

    heap_cache = (void **)((u8 *)gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    facing = *(s32 *)((u8 *)gWorkSlot + 12 * 4);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw = (DrawRectangle)heap_cache[7];

    for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;

    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(facing, facing + 12);

    for (member = 0; member != work->effect->count; member++) {
        void *object;
        s32 offset;
        s32 height;
        struct EffectStep *shard;

        offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
        object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
        height = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
        point[0] = *(s32 *)((u8 *)object + 8);
        point[1] = height;
        point[2] = *(s32 *)((u8 *)object + 16);
        EffectPosition_ApplyBaseAndYOffset(point, &screen);
        screen.x >>= 1;
        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != SHARDS_PER_MEMBER; i++) {
            s32 radius;
            s32 angle;
            s32 lift;

            radius = Random16() & 0xff;
            angle = Random16() & 0xffff;
            shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
            shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
            shard->velocity_x = (128 - (Random16() & 0xff)) << 9;
            lift = Random16() & 0xff;
            shard->variant = 0;
            shard->velocity_y = (-lift - 128) << 10;
            shard++;
        }
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != work->effect->count * 20 + 56; frame++) {
        if (frame == 32)
            BattleEventRuntime_BeginPhaseFar(0);
        for (member = 0; member != work->effect->count; member++) {
            s32 offset;

            offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
            if (frame == member * 20) {
                Audio_PlayCue(143);
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 20);
            }
            if (frame > member * 20) {
                struct EffectStep *shard;

                for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
                     i != SHARDS_PER_MEMBER; i++) {
                    if (shard->variant >= 0) {
                        s32 size;

                        size = i % 3 + 1;
                        draw(canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                            HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
                        EffectStep_AdvanceWithGravity2D(shard, 62, IceShardBursts_Gravities[i & 3]);
                        shard->variant++;
                        if (shard->velocity_y > 0 && HI(shard->y) > 112)
                            shard->variant = -1;
                    }
                    shard++;
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
