#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

struct Point2D {
    s32 x;
    s32 y;
};

struct BattleObject {
    u8 unknown_00[40];
    s32 unknown_28;
    u8 unknown_2c[28];
    s32 unknown_48;
};

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];
extern volatile u32 gKeysRepeat;
extern u16 ParticleStreams_CellOffsets[];
/* Three x and y pairs: where each ember column stands. */
extern u8 EmberColumns_Columns[];
/* By ember index modulo four: how fast it is pulled down. */
extern s32 EmberColumns_Gravity[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleFx_SpawnObjects(s32 count, s32 kind, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void AudioCommand_PlayFar(s32 value);
void BattleFx_PlaceFormationObjects(s32 channel, s32 x, s32 y);
void BattleEffect_RunImpactBurst(s32 channel, s32 x, s32 y);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
struct BattleObject **GetBattleObjectSlotFar(s32 id);
void ResourceObject_ReleaseFar(void *object);
s32 BattleFx_EndCanvasLayer(void);

/*
 * A scrolling band of spray rises while a focus point glides in from the
 * right and settles; from frame 28 embers burst around it, and for 48 frames
 * three columns of fire climb while sixteen more embers join each frame.
 * Embers grow, fall under a per-lane pull, and die once they drop past the
 * ground. A held button skips the middle of the effect. Afterwards the
 * impact burst plays at the focus point and the spawned objects are freed.
 */
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void **cursor;
    void *canvas;
    struct BattleEffectWork *work;
    u8 *graphics;
    u8 *palette;

    cursor = (void **)(gWorkSlot + 40 * 4);
    canvas = cursor[0];
    work = cursor[-1];
    graphics = cursor[1];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000052 = 0x1010;
    BattleFx_FetchRectangleBlitters(0, draw);
    palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
    palette += 128;
    Resource_DecodeType01(palette, (u8 *)work + 0x6e4);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    {
        struct Point2D focus;
        struct Point2D velocity;
        s32 rise;
        s32 frame;
        s32 k;
        struct EffectStep *point;
        struct EffectStep *spark;
        struct EffectStep *sparks;
        s32 *life;

        rise = 0;
        focus.x = 256 << 16;
        focus.y = 88 << 16;
        velocity.x = -0x100000;
        velocity.y = -0x40000;
        for (k = 0; k != 64; k++)
            work->particles[k].variant = -1;
        for (k = 0; k != 16; k++) {
            point = &work->particles[24 + k];
            point->x = Random16() & 127;
            point->y = (Random16() & 7) + 56;
            point->variant = -(s32)(Random16() & 15);
        }
        life = &((struct EffectStep *)gMapCellBuffer)->variant;
        for (k = 0; k != 1024; k++) {
            *life = -1;
            life += 7;
        }
        sparks = (struct EffectStep *)gMapCellBuffer;
        BattleFx_SelectLivingTargets(work->effect);
        WaitFrames(1);
        BattleFx_SpawnObjects(12, 380, 2);

        for (frame = 0; frame != 124; frame++) {
            if ((gKeysRepeat & 3) && frame > 32 && frame <= 97)
                frame = 98;
            if (frame == 120)
                BattleEventRuntime_BeginPhaseFar(134);
            if (frame <= 15)
                rise += 2;
            if (frame <= 99) {
                focus.x += velocity.x;
                focus.y += velocity.y;
                velocity.x = velocity.x * 58 / 64;
                velocity.y = velocity.y * 56 / 64;
                if (focus.x <= 0x77ffff)
                    velocity.x += 0x8000;
            }
            BattleFx_PlaceFormationObjects(1, focus.x, focus.y);
            if (frame == 28) {
                for (k = 0; k != 256; k++) {
                    spark = &sparks[k];
                    if (spark->variant == -1) {
                        s32 radius = Random16() & 63;
                        s32 angle = Random16() & 0xffff;

                        spark->x = ((Trig_Sin(angle) * radius) >> 3) + 0x200000;
                        spark->y = ((Trig_Cos(angle) * radius) >> 2) + 0x600000;
                        spark->velocity_x = ((s32)(Random16() & 63) - 32) << 14;
                        spark->velocity_y = (-(s32)(Random16() & 63) - 8) << 13;
                        spark->variant = 0;
                    }
                }
            }
            if (frame >= 32 && frame < 80) {
                s32 spawned = 0;

                for (k = 0; k != 1024; k++) {
                    spark = &sparks[k];
                    if (spark->variant == -1) {
                        s32 radius = Random16() & 63;
                        s32 angle = Random16() & 0xffff;

                        spark->x = ((Trig_Sin(angle) * radius) >> 3) + 0x200000;
                        spark->y = ((Trig_Cos(angle) * radius) >> 2) + 0x600000;
                        spark->velocity_x = ((s32)(Random16() & 63) - 32) << 14;
                        spark->velocity_y = (-(s32)(Random16() & 63) - 8) << 13;
                        spark->variant = 0;
                        spawned++;
                        if (spawned == 16)
                            break;
                    }
                }
            }
            if (frame == 0)
                AudioCommand_PlayFar(164);
            if (frame == 32)
                AudioCommand_PlayFar(145);
            if (frame == 80)
                AudioCommand_PlayFar(144);
            if (frame >= 32 && frame < 80) {
                for (k = 0; k != 3; k++) {
                    s32 span = (frame * 16 - 256 + k * 25) % 104;

                    draw[0](canvas, (u8 *)work + 0x6e4, EmberColumns_Columns[k * 2] - 17,
                        EmberColumns_Columns[k * 2 + 1] - span - 104, 34, 104);
                    draw[0](canvas, (u8 *)work + 0x6e4, EmberColumns_Columns[k * 2] - 17,
                        EmberColumns_Columns[k * 2 + 1] - span, 34, span);
                }
            }
            if (frame <= 95) {
                for (k = 0; k != 5; k++)
                    draw[0](canvas, work, k * 32 + ((frame / 4) & 31) - 32, 120 - rise, 32, 32);
            }
            for (k = 0; k != 1024; k++) {
                spark = &sparks[k];
                if (spark->variant >= 0) {
                    s32 size = k % 3 + 2;

                    if (spark->velocity_y > 0)
                        size += 2;
                    if (frame > 68 && size <= 5)
                        size = 6;
                    if (frame > 70 && size <= 6)
                        size = 7;
                    if (frame > 72 && size <= 7)
                        size = 8;
                    if (frame > 74 && size <= 8)
                        size = 9;
                    if (frame > 76)
                        size = 10;
                    draw[spark->velocity_y > 0](canvas, graphics + ParticleStreams_CellOffsets[size - 1],
                        ((s16 *)&spark->x)[1] - size / 2, ((s16 *)&spark->y)[1] - size, size, size * 2);
                    spark->x += spark->velocity_x;
                    spark->y += spark->velocity_y;
                    if (frame > 80)
                        spark->velocity_y += -0x8000;
                    else
                        spark->velocity_y += EmberColumns_Gravity[k & 3];
                    spark->velocity_x = spark->velocity_x * 62 / 64;
                    spark->velocity_y = spark->velocity_y * 62 / 64;
                    spark->variant++;
                    if (spark->velocity_y > 0 && ((s16 *)&spark->y)[1] > 104)
                        spark->variant = -1;
                }
            }
            if (frame <= 79) {
                for (k = 0; k != work->effect->count; k++) {
                    if (frame > 29) {
                        s32 phase = frame % 12;

                        if (phase == 0) {
                            struct BattleObject *object = *GetBattleObjectSlotFar(work->effect->actors[k]);

                            ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, -1, 0);
                            object->unknown_28 = 0x48000;
                            object->unknown_48 = 0xab85;
                        }
                        if (phase == 6)
                            ObjectGroup_UpdateMembers(work->effect->actors[k], 0, 5, -1, 0);
                    }
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        }

        Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        BattleEffect_RunImpactBurst(1, focus.x, focus.y);
        for (k = 0; k != 12; k++)
            ResourceObject_ReleaseFar(((void **)((u8 *)work + 0x77d8))[k]);
    }
    BattleFx_EndCanvasLayer();
}
