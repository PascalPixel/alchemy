#include "CANVAS.H"
#include "RUNTIME_MEM.H"
/* Draft. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern u8 gWorkSlot[];
extern u16 ParticleStreams_CellOffsets[];

extern u8 Volley_CometHeights[];
/* Five bytes for each mode: how many shots it throws at a unit, how many
   frames a shot flies, how many frames lie between two shots and between
   two units, and how many sparks a hit throws. */
extern u8 Volley_Modes[];
extern u8 Volley_FlashWidths[];
extern u8 Volley_FlashHeights[];
extern u8 Volley_FlashSteps[];
extern u8 Volley_FlashRises[];
extern u16 Volley_FlashOffsets[];
extern u16 Volley_PelletOffsets[];

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define HI(v) (((s16 *)&(v))[1])
#define SLOT47 (*(DrawRectangle *)(gWorkSlot + 47 * 4))

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind, s32 side,
    s32 mode, s32 *x, s32 *y);
s32 Battle_GetObjectTableValueFar(s32 unit);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    s32 screen_x;
    s32 screen_y;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 i;
    s32 j;
    s32 frame;
    struct BlitterPair draw;
    struct BattleCamera *camera;
    u8 *sheet;
    s32 cooldown;
    s32 total;
    s32 kind;
    s32 shots;
    s32 steps;
    s32 stagger;
    s32 delay;
    s32 height;
    struct MotionObject *source;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    sheet = heap_cache[2];
    cooldown = 0;
    work->effect = effect;
    kind = effect->variant;
    if (mode == 10)
        BattleFx_BeginCanvasLayer(0);
    else
        BattleFx_BeginCanvasLayer(1);
    if (work->effect->unknown_001c == 1)
        BattleFx_PrepareCanvasEffect(effect, 1, work->effect->side, 2, &screen_x, &screen_y);
    if (mode == 5) {
        if (work->effect->side == 1)
            BattleEffect_LoadWork(46, 7, 7, 7, 2);
        else
            BattleEffect_LoadWork(46, 7, 7, 3, 2);
    } else {
        if (work->effect->side == 1)
            BattleEffect_LoadWork(46, 7, 7, 7, 3);
        else
            BattleEffect_LoadWork(46, 7, 7, 3, 3);
    }
    draw.upper = ((DrawRectangle *)gWorkSlot)[46];
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    if (mode == 0 || mode == 5 || mode == 8) {
        if (mode == 5)
            kind = 2;
        if (mode == 8)
            kind = 0;
        if (kind == 0)
            Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA, work->sheet + 0x1000, 1, 1);
        else if (kind == 1)
            Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB, work->sheet + 0x1000, 1, 1);
        else
            Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC, work->sheet + 0x1000, 1, 1);
        if (mode == 5)
            Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
        Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet, work->sheet + 0x2000, 1, 0);
        if (mode == 5)
            Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
        work->transfer_mode = 2;
        work->transfer_value = 75;
    } else if (mode == 1) {
        Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work, 1, 1);
        *(u16 *)0x04000050 = 0;
        work->transfer_mode = 1;
        work->transfer_value = 0;
    } else if (mode == 2) {
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
        Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage, work, 0, 0);
        work->transfer_mode = 2;
        work->transfer_value = 50;
    } else {
        if ((u32)(mode - 3) <= 1 || mode == 11) {
            Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work, 1, 1);
        } else if (mode == 6) {
            Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work, 1, 1);
        } else {
            Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work, 1, 1);
            Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet, work->sheet + 0x65c0, 1, 0);
        }
        work->transfer_mode = 2;
        work->transfer_value = 50;
    }
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    shots = Volley_Modes[mode * 5];
    steps = Volley_Modes[mode * 5 + 1];
    stagger = Volley_Modes[mode * 5 + 2];
    delay = Volley_Modes[mode * 5 + 3];
    height = Battle_GetObjectTableValueFar(work->effect->actor);
    if (work->effect->count * shots > 63)
        work->effect->count = 1;

    for (i = 0; i != work->effect->count; i++) {
        struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[i])->object;
        s32 target_height = Battle_GetObjectTableValueFar(work->effect->actors[i]);

        for (j = 0; j != shots; j++) {
            struct EffectStep *shot = &work->particles[i * shots + j];

            shot->x = source->x;
            if (mode == 7)
                shot->y = source->y + ((Random16() & 15) << 16) + 0x3a0000;
            else if (mode == 10)
                shot->y = source->y + height / 2;
            else if (mode == 6)
                shot->y = source->y + height / 2;
            else if (mode == 9)
                shot->y = source->y + height / 2 + ((16 - (Random16() & 31)) << 16);
            else if ((u32)(mode - 3) <= 1)
                shot->y = source->y + height / 2 + ((16 - (Random16() & 31)) << 16);
            else if (mode == 11)
                shot->y = source->y + height / 2 + ((32 - (Random16() & 63)) << 16);
            else if (mode == 5)
                shot->y = source->y + height / 2;
            else
                shot->y = source->y + height;
            shot->z = source->z;
            shot->velocity_x = (target->x - shot->x) / steps;
            if (mode == 7)
                shot->velocity_y = (target->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) / steps;
            else if (mode == 8)
                shot->velocity_y = (target->y + ((Random16() & 7) << 16) - shot->y + 0x160000) / steps;
            else if (mode == 9)
                shot->velocity_y = (target->y + ((64 - (Random16() & 63)) << 16) - shot->y) / steps;
            else if (mode == 10)
                shot->velocity_y = (target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) / steps;
            else if ((u32)(mode - 3) <= 1 || mode == 11 || mode == 5)
                shot->velocity_y = 0;
            else if (mode == 6)
                shot->velocity_y = (target->y + target_height / 2 + ((Random16() & 15) << 16) - shot->y) / steps;
            else
                shot->velocity_y = (target->y + target_height - ((Random16() & 15) << 16) - shot->y) / steps;
            shot->velocity_z = (target->z - shot->z) / steps;
            shot->variant = 0;
        }
    }
    for (i = 0; i != 1024; i++)
        gMotes[i].variant = 0;
    if (mode == 6)
        total = work->effect->count * delay + stagger * shots + 32;
    else
        total = work->effect->count * delay + stagger * shots + 16;

    for (frame = 0; frame != total; frame++) {
        s32 index;

        if (cooldown > 0)
            cooldown--;
        if (mode == 6) {
            if (frame == 4)
                AudioCommand_PlayFar(136);
            if (frame == 32)
                BattleEventRuntime_BeginPhaseFar(134);
        } else if (mode == 7) {
            if (frame == 48)
                BattleEventRuntime_BeginPhaseFar(133);
        } else if (mode != 5) {
            if (frame == 16)
                BattleEventRuntime_BeginPhaseFar(133);
        }
        if (work->effect->unknown_001c == 1) {
            s32 x = (-Trig_Sin(frame << 11) << 2 >> 16) + screen_x / 2 - 10;
            s32 y = (Trig_Cos(frame << 11) << 1 >> 16) + screen_y - 24;

            if (frame > 69)
                y = y - frame * 2 + 138;
            if (work->effect->side == 1)
                BattleEffect_LoadWork(47, 7, 7, 7, 3);
            else
                BattleEffect_LoadWork(47, 7, 7, 3, 3);
            draw.lower = ((DrawRectangle *)gWorkSlot)[47];
            if (frame <= 3)
                draw.lower(canvas, work->sheet + 0x65c0, x, y, 20, 40);
            draw.lower(canvas, work->sheet + 0x65c0, x, y, 20, 40);
            Runtime_ReleaseHeapBlock(47);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        index = 0;
        for (i = 0; i != work->effect->count; i++) {
            struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[i])->object;

            for (j = 0; j != shots; j++) {
                if (j * stagger + i * delay < frame) {
                    struct EffectStep *shot = &work->particles[shots * i + j];

                    EffectPosition_ApplyBaseAndYOffset((s32 *)shot, &position);
                    position.x >>= 1;
                    shot->x += shot->velocity_x;
                    shot->y += shot->velocity_y;
                    shot->z += shot->velocity_z;
                    if (mode == 6) {
                        s32 spawned = 0;
                        s32 k;

                        for (k = 512; k != 1024; k++) {
                            struct EffectStep *mote = &gMotes[k];

                            if (mote->variant == 0) {
                                mote->x = position.x << 16;
                                mote->y = position.y << 16;
                                mote->velocity_x = ((Random16() & 255) - 128) << 11;
                                mote->velocity_y = ((Random16() & 255) - 128) << 11;
                                mote->variant = (Random16() & 7) + 32;
                                spawned++;
                                if (spawned == 2)
                                    break;
                            }
                        }
                    }
                    if (shot->variant == 0
                        && ((target->x < 0 && shot->x < 0) || (target->x >= 0 && shot->x >= 0))
                        && abs(shot->x) >= abs(target->x)) {
                        s32 spawned = 0;
                        struct EffectStep *flash = &gMotes[index];
                        s32 shift;
                        s32 k;

                        shot->variant = 1;
                        if (mode == 5) {
                            BattleEventRuntime_BeginPhaseFar(134);
                        } else if (mode != 6 && cooldown == 0) {
                            cooldown = 8;
                            AudioCommand_PlayFar(132);
                        }
                        if (mode == 2) {
                            s32 kick = ((Random16() & 31) + 32) << 12;

                            if (Random16() & 1)
                                shot->velocity_y += kick;
                            else
                                shot->velocity_y -= kick;
                            shot->velocity_x = -shot->velocity_x;
                        }
                        flash->variant = 1;
                        flash->x = position.x;
                        flash->y = position.y;
                        flash->z = 0;
                        if (mode != 7)
                            work->shake_frames = 2;
                        ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
                        if (mode != 7 && mode != 9 && mode != 10) {
                            if (mode == 5)
                                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
                            else
                                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 5);
                        }
                        shift = 12 - (mode != 5);
                        for (k = 100; k != 512; k++) {
                            struct EffectStep *mote = &gMotes[k];

                            if (mote->variant == 0) {
                                mote->x = position.x << 16;
                                mote->y = position.y << 16;
                                mote->velocity_x = ((Random16() & 255) - 128) << shift;
                                mote->velocity_y = ((Random16() & 255) - 128) << shift;
                                mote->variant = (Random16() & 7) + 16;
                                spawned++;
                                if (spawned == Volley_Modes[mode * 5 + 4])
                                    break;
                            }
                        }
                    }
                    if (mode == 0 || mode == 5 || mode == 8) {
                        if (work->effect->side == 1)
                            BattleEffect_LoadWork(47, 7, 7, 7, 2);
                        else
                            BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        SLOT47(canvas, work->sheet + 0x1000, position.x - 16,
                            position.y - Volley_CometHeights[kind], 32, Volley_CometHeights[kind]);
                        Runtime_ReleaseHeapBlock(47);
                        if (work->effect->side == 1)
                            BattleEffect_LoadWork(47, 7, 7, 15, 2);
                        else
                            BattleEffect_LoadWork(47, 7, 7, 11, 2);
                        SLOT47(canvas, work->sheet + 0x1000, position.x - 16,
                            position.y, 32, Volley_CometHeights[kind]);
                        Runtime_ReleaseHeapBlock(47);
                    } else if (mode == 1) {
                        if (abs(shot->x) <= abs(target->x))
                            draw.upper(canvas, work->sheet + frame % 6 * 768,
                                position.x - 16, position.y - 12, 32, 24);
                    } else if (mode == 7 || mode == 9 || mode == 10) {
                        draw.upper(canvas, work->sheet + Volley_PelletOffsets[j & 3],
                            position.x - 4, position.y - 4, 8, 8);
                    } else if (mode == 2) {
                        draw.upper(canvas, work->sheet + j % 6 * 128, position.x - 4, position.y - 8, 8, 16);
                    } else if (mode == 3) {
                        draw.upper(canvas, work->sheet + 96, position.x - 9, position.y - 7, 18, 13);
                    } else if (mode == 4) {
                        draw.upper(canvas, work, position.x - 6, position.y - 4, 12, 8);
                    } else if (mode == 11) {
                        draw.upper(canvas, work->sheet + 330, position.x - 15, position.y - 12, 29, 23);
                    } else {
                        draw.upper(canvas, work, position.x - 20, position.y - 32, 40, 64);
                    }
                }
                index++;
            }
        }
        if (mode == 0 || mode == 5 || mode == 8) {
            for (i = 0; i != work->effect->count * shots; i++) {
                struct EffectStep *flash = &gMotes[i];

                if (flash->variant == 1) {
                    s32 step = flash->z / 2;

                    if (!(i & 1)) {
                        BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        SLOT47(canvas, work->sheet + Volley_FlashOffsets[step],
                            flash->x - Volley_FlashWidths[step], flash->y - Volley_FlashRises[step],
                            Volley_FlashWidths[step], Volley_FlashHeights[step]);
                        Runtime_ReleaseHeapBlock(47);
                        BattleEffect_LoadWork(47, 7, 7, 15, 2);
                        SLOT47(canvas, work->sheet + Volley_FlashOffsets[step],
                            flash->x, flash->y + Volley_FlashSteps[step] - Volley_FlashRises[step],
                            Volley_FlashWidths[step], Volley_FlashHeights[step]);
                        Runtime_ReleaseHeapBlock(47);
                    } else {
                        BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        SLOT47(canvas, work->sheet + Volley_FlashOffsets[step] + 0x128a,
                            flash->x - Volley_FlashRises[step], flash->y - Volley_FlashWidths[step],
                            Volley_FlashHeights[step], Volley_FlashWidths[step]);
                        Runtime_ReleaseHeapBlock(47);
                        BattleEffect_LoadWork(47, 7, 7, 15, 2);
                        SLOT47(canvas, work->sheet + Volley_FlashOffsets[step] + 0x128a,
                            flash->x + Volley_FlashSteps[step] - Volley_FlashRises[step], flash->y,
                            Volley_FlashHeights[step], Volley_FlashWidths[step]);
                        Runtime_ReleaseHeapBlock(47);
                    }
                    flash->z++;
                    if (flash->z == 12)
                        flash->variant = 0;
                }
            }
        }
        for (i = 100; i != 512; i++) {
            struct EffectStep *mote = &gMotes[i];

            if (mote->variant > 0) {
                s32 size = (mote->variant >> 3) + 1;
                s32 tall = size * 2;

                draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                    HI(mote->x) - size / 2, HI(mote->y) - size, size, tall);
                EffectStep_AdvanceWithGravity2D(mote, 60, 0x1000);
                if (mote->y > 0x700000)
                    mote->velocity_y = -mote->velocity_y / 2;
                mote->variant--;
            }
        }
        for (i = 512; i != 1024; i++) {
            struct EffectStep *mote = &gMotes[i];

            if (mote->variant > 0) {
                s32 size = (mote->variant >> 4) + 1;
                s32 tall = size * 2;

                draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                    HI(mote->x) - size / 2, HI(mote->y) - size, size, tall);
                EffectStep_AdvanceWithGravity2D(mote, 60, -0x4000);
                mote->variant--;
            }
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
