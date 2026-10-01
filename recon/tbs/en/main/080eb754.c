/* Draft (2026-10-02, slice-11), rewritten on the shared effect structs with
   no address constants. Battle effect in two timed loops: 120 frames that
   sweep nine spawned objects across the screen behind a growing ring and a
   64-record fire pool, then 96 frames in which the objects rise and each
   affected unit bursts into 32 sparks as the row passes it.
   Remaining difference: see the score line `make drafts` prints; nothing
   here has been fitted yet beyond the statement order of the listing. */
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "MAP_SCROLL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

#define REG16(address) (*(volatile u16 *)(address))

/* 320 burst records fill the map cell buffer, 32 for each affected unit. */
#define BURST ((struct EffectStep *)Ram_MapCellBuffer)

/* The nine spawned objects, kept in the work block. */
#define OBJECTS(work) ((struct FxObject **)((u8 *)(work) + 0x77d8))

struct FxObject {
    u8 unknown_00[9];
    u8 flags;
};

/* A projected placement: a 16.16 position and two words around it. */
struct FxPlacement {
    s32 x;
    s32 scale;
    s32 y;
    s32 unknown_0c;
};

struct FxPair {
    s32 a;
    s32 b;
};

struct FxControl {
    u8 unknown_00[16];
    s32 active;
};

extern void *gBattleFxWork[];
extern void *gWorkSlot[];
extern s32 gProjection[];
extern u32 gKeysRepeat;
extern u16 ParticleStreams_CellOffsets[];
/* Each object's column and row in the formation. */
extern const u8 ObjectRow_Columns[];
extern const u8 ObjectRow_Rows[];
extern const struct FxPair ObjectRow_SweepPair;
extern const struct FxPair ObjectRow_RisePair;

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32 layer, s32 mode);
void BattleFx_SpawnObjects(s32 count, s32 animation, s32 mode);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleBackground_LoadFar(s32 layer, s32 resource_id, s32 mode);
void BattleEffect_SetupBlendedDisplay(void);
void Object_ApplyProjectedPlacementFar(struct FxObject *object, struct FxPlacement *placement, struct FxPair *pair, s32 mode);
void ResourceObject_ReleaseFar(struct FxObject *object);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode, s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Audio_PlayCue(s32 cue);

void Unnamed_080eb754(struct BattleEffectArgument *effect)
{
    void **cursor;
    struct FxPlacement place;
    struct EffectPosition unit;
    u8 jitter_row[16];
    u8 hit_row[14];
    struct FxPlacement place2;
    void *canvas;
    struct BattleEffectWork *work;
    DrawRectangle blit;
    u8 *sheet;
    s32 base_x;
    s32 grow;
    s32 saved_x;
    struct FxControl *control;
    s32 shift;
    s32 wave;
    s32 frame;
    s32 i;

    cursor = gBattleFxWork + 1;
    canvas = cursor[0];
    work = cursor[-1];
    sheet = cursor[1];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    BattlePres_ConfigureEffectDisplay();
    REG16(0x0400000c) = 0x784;
    REG16(0x05000000) = 0;
    REG16(0x05000002) = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_WipeCanvas(1, 0);
    BattleFx_SpawnObjects(9, 0x175, 1);
    gProjection[4] = 240;
    BattleFx_SelectLivingTargets(work->effect);
    REG16(0x04000048) = 0x2737;
    REG16(0x04000040) = 0xca;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
    BattleEffect_WipeCanvas(1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work, 1, 1);
    REG16(0x04000000) = 0x7741;
    REG16(0x04000020) = 0x80;
    REG16(0x04000052) = 0x100e;
    REG16(0x04000050) = 0x3f44;

    base_x = 0;
    grow = 0;
    saved_x = gBgScroll[1].x;
    control = cursor[4];
    shift = 0;
    work->transfer_mode = 1;
    work->transfer_value = base_x;
    control->active = 1;

    i = 0;
    do {
        struct EffectStep *fire = &work->particles[i];

        fire->x = (Random16() & 31) + 16;
        fire->y = ((Random16() & 31) + 48) << 16;
        fire->velocity_y = ((Random16() & 31) - 16) << 16;
        fire->variant = (u32)Random16() % 48 + 2;
        i++;
    } while (i != 64);

    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    blit = gWorkSlot[46];
    REG16(0x0400000c) = 0x786;

    for (frame = 0; frame != 120; frame++) {
        wave = 0;
        if (frame == 0) {
            Audio_PlayCue(136);
        }
        if (frame == 26) {
            Audio_PlayCue(141);
        }
        if (frame == 40) {
            Audio_PlayCue(154);
        }
        if (frame == 72) {
            Audio_PlayCue(154);
        }
        if (frame == 104) {
            Audio_PlayCue(154);
        }
        if ((gKeysRepeat & 3) != 0 && frame > 16) {
            break;
        }

        if ((u32)(frame - 24) <= 31) {
            shift++;
        }
        if (shift > 24) {
            shift = 24;
        }
        if (frame <= 135) {
            gBgScroll[1].x -= shift;
            grow += shift;
        }

        if (frame <= 149) {
            struct FxPair pair = ObjectRow_SweepPair;
            s32 lift;

            lift = 0;
            if (frame > 103) {
                lift = frame * 16 - 1664;
            }
            if ((u32)(frame - 8) <= 23) {
                base_x = base_x + shift - 8;
            }
            if (frame > 7) {
                s32 amp;
                s32 ang;

                amp = 96;
                if (frame <= 104) {
                    amp = 32;
                }
                ang = (frame * 1024 - 0x2000) & 0xFFFF;
                if (ang > 0x8000) {
                    ang -= 0x8000;
                }
                wave = (Trig_Sin(ang) * amp) >> 16;
                if ((frame & 31) == 8) {
                    work->shake_frames = 4;
                }
            }
            place.unknown_0c = 0;
            place.scale = 255 << 16;
            i = 0;
            do {
                place.x = ((base_x + ObjectRow_Columns[i] - lift) << 16) + (224 << 16);
                place.y = ((ObjectRow_Rows[i] - wave) << 16) + (144 << 15);
                Object_ApplyProjectedPlacementFar(OBJECTS(work)[i], &place, &pair, 0);
                i++;
            } while (i != 9);
        }

        /* The ring: up to 64 cells on an ellipse that widens with the scroll. */
        if (frame <= 26) {
            s32 size = grow + 4;
            s32 count = frame * 8;

            if (size > 10) {
                size = 10;
            }
            if (count > 64) {
                count = 64;
            }
            for (i = 0; i != count; i++) {
                s32 ang = i << 10;
                s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow;
                s32 y = (Trig_Cos(ang) * (grow * 12 + 48)) >> 16;

                blit(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                    x + 96 - size / 2, y + 64 - size, size, size * 2);
            }
        }

        if (frame == 24) {
            work->transfer_mode = 2;
            work->transfer_value = 50;
        }
        if (frame == 28) {
            REG16(0x0400000c) = 0x784;
        }

        if (frame > 17) {
            i = 0;
            do {
                struct EffectStep *fire = &work->particles[i];

                if (fire->variant == 0) {
                    s32 size = i % 3 + 1;

                    blit(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        fire->x, HI(fire->y) - size, size, size * 2);
                    fire->x += 2;
                    fire->y += fire->velocity_y;
                    fire->velocity_y = fire->velocity_y * 48 / 64;
                } else {
                    fire->variant--;
                }
                if (fire->x > 128 || fire->variant == 1) {
                    fire->x = (Random16() & 31) + base_x + 172;
                    fire->y = ((Random16() & 31) - wave + 56) << 16;
                    fire->velocity_y = ((Random16() & 31) - 16) << 15;
                }
                i++;
            } while (i != 48);
        }

        if (frame > 31) {
            s32 rise = (frame - 32) / 2;
            s32 y;

            if (rise > 40) {
                rise = 40;
            }
            y = 0;
            i = 0;
            do {
                blit(canvas, (u8 *)work + (Random16() & 3) * 0x600, 120 - rise, y, 48, 32);
                i++;
                y += 18;
            } while (i != 6);
        }

        if (work->shake_frames > 0) {
            work->shake_frames--;
            gBgScroll[1].y = (Random16() & 7) + 28;
        } else {
            gBgScroll[1].y = 32;
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    gBgScroll[1].x = saved_x;
    control->active = 0;
    BattleEffect_SetupBlendedDisplay();
    REG16(0x04000040) = 0xf0;

    i = 0;
    do {
        struct FxObject *object = OBJECTS(work)[i];

        object->flags |= 12;
        i++;
    } while (i != 9);

    {
    s32 row_x;

    row_x = 224;
    i = 0;
    do {
        hit_row[i] = 0;
        i++;
    } while (i != 14);
    i = 0;
    do {
        jitter_row[i] = Random16() & 31;
        i++;
    } while (i != 16);
    i = 0;
    do {
        BURST[i].variant = 0;
        i++;
    } while (i != 160 << 1);

    work->transfer_mode = 2;
    work->transfer_value = 75;
    REG16(0x0400000c) = 0x784;
    REG16(0x04000052) = 0x1010;

    frame = 0;
    do {
        if (frame <= 23) {
            struct FxPair pair2 = ObjectRow_RisePair;
            s32 ang;
            s32 rise;

            row_x -= 16;
            if (frame <= 8) {
                ang = frame * 2048 + 0x4000;
                if (ang > 0x8000) {
                    ang -= 0x8000;
                }
                rise = Trig_Sin(ang) << 6;
            } else {
                ang = frame * 2048 + 0x4000;
                if (ang > 0x8000) {
                    ang -= 0x8000;
                }
                rise = Trig_Sin(ang) << 5;
            }
            rise >>= 16;
            place2.unknown_0c = 0;
            place2.scale = 255 << 16;
            i = 0;
            do {
                place2.x = (row_x + ObjectRow_Columns[i]) << 16;
                place2.y = ((ObjectRow_Rows[i] - rise) << 16) + (144 << 15);
                Object_ApplyProjectedPlacementFar(OBJECTS(work)[i], &place2, &pair2, 0);
                i++;
            } while (i != 9);
        }

        if (frame == 8) {
            work->shake_frames = frame;
            Audio_PlayCue(145);
        }
        if (frame == 11) {
            Audio_PlayCue(145);
        }
        if (frame == 46) {
            Audio_PlayCue(137);
        }

        /* Each unit bursts once, when the rising row has passed it. */
        for (i = 0; i != work->effect->count; i++) {
            if (hit_row[i] == 0) {
                EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i], &unit);
                if (unit.x > row_x) {
                    s32 k;

                    hit_row[i] = 1;
                    k = 0;
                    do {
                        struct EffectStep *burst = &BURST[i * 32 + k];
                        s32 up;

                        burst->x = unit.x << 15;
                        burst->y = (unit.y - 16) << 16;
                        burst->velocity_x = ((Random16() & 255) - 128) << 10;
                        up = (Random16() & 255) - 192;
                        burst->velocity_y = up << 11;
                        burst->x += burst->velocity_x * 4;
                        burst->y += up << 13;
                        burst->variant = (Random16() & 15) + 8;
                        k++;
                    } while (k != 32);
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
                    Audio_PlayCue(134);
                }
            }
        }

        i = 0;
        do {
            struct EffectStep *burst = &BURST[i];

            if (burst->variant > 0) {
                blit(canvas, sheet + ParticleStreams_CellOffsets[2],
                    HI(burst->x) - 1, HI(burst->y) - 3, 3, 6);
                burst->x += burst->velocity_x;
                burst->y += burst->velocity_y;
                burst->variant--;
            }
            i++;
        } while (i != 192);

        if (frame == 48) {
            Audio_PlayCue(136);
        }
        if (frame > 40) {
            s32 slide = (frame - 40) * 12;
            s32 y;

            work->transfer_mode = 0;
            work->transfer_value = 75;
            y = -8;
            i = 0;
            do {
                blit(canvas, (u8 *)work + (Random16() & 3) * 0x600,
                    jitter_row[i] - slide + 120, y, 48, 32);
                i++;
                y += 8;
            } while (i != 16);
        }
        if (frame > 64) {
            work->transfer_mode = 2;
        }
        if (frame == 58) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, -1, 0);
            }
        }

        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 96);
    }

    BattleEventRuntime_BeginPhaseFar(134);
    i = 0;
    do {
        ResourceObject_ReleaseFar(OBJECTS(work)[i]);
        i++;
    } while (i != 9);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
