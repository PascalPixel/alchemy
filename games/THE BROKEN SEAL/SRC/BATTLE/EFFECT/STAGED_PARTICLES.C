#include "PROJECT.H"
#include "TRANSFORM.H"
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "B5_CONTEXT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "MAP_SCROLL.H"

extern u8 gTransitionWork[];
extern u8 gMapCellBuffer[];
extern volatile u32 gKeysRepeat;



/* The battle presentation block in heap slot 44. */
struct BattlePresentationWork {
    u8 unknown_00[16];
    s32 scroll_enabled;
};

typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_AdvanceScrollOnInterval(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32 mode, s32 layer);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);
void BattleFx_SpawnObjects(s32 count, s32 kind, s32 variant);
void BattleBackground_LoadFar(s32 layer, s32 resource, s32 mode);
void BattleEffect_SetupBlendedDisplay(void);
void Render_ResetTransformState(void);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
void Object_ApplyProjectedPlacementFar(void *object, s32 *position, const Scale *scale, s32 mode);
void Audio_PlayCue(s32 cue);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern u16 PuffArc_CellSourceOffsets[];
extern u8 PuffArc_CellWidths[];
extern u8 PuffArc_CellBiasY[];
extern u8 PuffArc_CellHeights[];
extern const Scale StagedParticles_UnitScale;

/* The nine scene objects the effect work keeps. */
#define OBJECTS ((void **)((u8 *)work + 0x77d8))
/* The spark trails and their saved transforms in the map cell buffer. */
#define TRAILS ((struct EffectStep *)Ram_MapCellBuffer)
#define MATRICES (Ram_MapCellBuffer + 0x3800)

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: nine scene objects follow a recorded path across a violet
   sky while sixteen slashes appear one after another, eight frames apart;
   each gathers twenty-four sparks for sixteen frames, flashes through five
   slash pictures and falls away. A or B skips the rest. Thirty-two slashes
   then rain down on the targets and burst into embers. */
void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect)
{
    s32 point[3];
    struct EffectPosition screen;
    s32 position[4];
    struct EffectPosition target;
    DrawRectangle draw[2];
    void **cache;
    struct BattlePresentationWork *presentation;
    void *canvas;
    struct BattleEffectWork *work;
    s8 *path;
    s32 i;
    s32 j;
    s32 frame;
    void *sheet;
    s32 scroll_x;
    s32 path_x;
    s32 path_y;
    s32 row;
    s32 column;

    cache = (void **)gTransitionWork;
    presentation = cache[0];
    canvas = cache[-4];
    work = cache[-5];
    sheet = cache[-3];
    scroll_x = gBgScroll[1].x;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2000);
    *(volatile u16 *)0x04000020 = 0x100;
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_WipeCanvas(0, 0);
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(9, 370, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_SlashSheet, work, 1, 1);
    Iwram_CopyWords((void *)0x05000000,
        Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    path = Resource_GetTableEntry((s32)&ResourceId_StagedParticlePath);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw[0] = cache[2];
    /* FAKEMATCH: the one-pass block keeps the table address after the second read. */
    do {
        draw[1] = cache[3];
    } while (0);
    gProjection.center_y = 240;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
    *(s32 *)((u8 *)work + 0x7790) = 0;
    *(s32 *)((u8 *)work + 0x7794) = 4;
    *(s32 *)((u8 *)work + 0x7798) = -1;
    *(s32 *)((u8 *)work + 0x779c) = 0;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_AdvanceScrollOnInterval, 0x480);
    presentation->scroll_enabled = 1;
    BattleEffect_WipeCanvas(0, 1);
    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000050 = 0x3f44;
    path_x = 0;
    path_y = 0;
    for (i = 0; i != 16; i++) {
        struct EffectStep *slash = &work->particles[i];

        slash->x = (Random16() % 96 + 12) << 16;
        slash->y = ((Random16() & 63) + 32) << 16;
        slash->velocity_x = 0;
        slash->velocity_y = 0;
        slash->variant = 0;
        for (j = 0; j != 24; j++) {
            TRAILS[i * 24 + j].x = (Random16() & 15) + 48;
            Render_ResetTransformState();
            SceneTransform_ApplyRoll(Random16() & 0xffff);
            SceneTransform_ApplyPitch(Random16() & 0xffff);
            SceneTransform_ApplyYaw(Random16() & 0xffff);
            Graphics_SaveTransferWork(MATRICES + (i * 24 + j) * 48);
        }
    }
    work->transfer_mode = 2;
    work->transfer_value = 50;
    *(volatile u16 *)0x0400000c = 0x784;
    for (frame = 0; frame != 220 && !(gKeysRepeat & 3); frame++) {
        if (frame <= 209) {
            if (frame == 0) {
                path_x = (path[0] << 8) + (u8)path[1];
                path_y = (path[2] << 8) + (u8)path[3];
                path += 4;
            } else {
                path_x += path[0];
                path_y += path[1];
                path += 2;
            }
            position[3] = 0;
            position[1] = 0xff0000;
            for (row = 0; row != 3; row++) {
                for (column = 0; column != 3; column++) {
                    position[0] = (path_x + 80 + column * 32) << 16;
                    position[2] = (64 - path_y + row * 32) << 16;
                    Object_ApplyProjectedPlacementFar(OBJECTS[row * 3 + column], position,
                        &StagedParticles_UnitScale, 0);
                }
            }
        }
        point[1] = 0;
        point[2] = 0;
        if (frame == 48) {
            work->fade_frames = 24;
            work->fade_step = 0;
        }
        for (i = 0; i != 16; i++) {
            s32 start = i * 8 + 64;

            if (frame >= start) {
                struct EffectStep *slash = &work->particles[i];
                s32 x = HI(slash->x);
                s32 y = HI(slash->y);

                if (frame == i * 8 + 84)
                    Audio_PlayCue(212);
                if (frame >= i * 8 + 85) {
                    slash->x += slash->velocity_x;
                    slash->y += slash->velocity_y;
                    slash->velocity_x -= 0x10000;
                    slash->velocity_y += 0x20000;
                    draw[0](canvas, (u8 *)work + 0x16ac, x + 4, y - 40, 16, 21);
                    draw[0](canvas, (u8 *)work + 0x17fc, x - 16, y - 19, 29, 35);
                    draw[0](canvas, (u8 *)work + 0x1bf3, x - 20, y + 16, 21, 24);
                } else if (frame >= i * 8 + 80) {
                    switch (frame - start - 16) {
                    case 0:
                        draw[0](canvas, work, x - 7, y - 14, 14, 28);
                        break;
                    case 1:
                        draw[0](canvas, (u8 *)work + 392, x - 11, y - 22, 23, 44);
                        break;
                    case 2:
                        draw[0](canvas, (u8 *)work + 0x57c, x - 4, y - 31, 20, 30);
                        draw[0](canvas, (u8 *)work + 0x7d4, x - 16, y - 1, 22, 33);
                        break;
                    case 3:
                        draw[0](canvas, (u8 *)work + 0xaaa, x + 1, y - 38, 18, 27);
                        draw[0](canvas, (u8 *)work + 0xc90, x - 11, y - 11, 22, 22);
                        draw[0](canvas, (u8 *)work + 0xe74, x - 19, y + 11, 19, 28);
                        break;
                    case 4:
                        draw[0](canvas, (u8 *)work + 0x1088, x + 4, y - 40, 16, 23);
                        draw[0](canvas, (u8 *)work + 0x11f8, x - 10, y - 17, 23, 28);
                        draw[0](canvas, (u8 *)work + 0x147c, x - 20, y + 11, 20, 28);
                        break;
                    }
                } else {
                    for (j = 0; j != 24; j++) {
                        struct EffectStep *trail = &TRAILS[i * 24 + j];

                        if (trail->x > 0) {
                            Graphics_LoadTransferWork(MATRICES + (i * 24 + j) * 48);
                            point[0] = trail->x;
#if defined(TBS_EDITION_JA)
                            {
                                /* FAKEMATCH: naming the result pointer keeps the point address in a register across this loop, as the Japanese build has it. */
                                struct EffectPosition *out = &screen;

                                EffectPosition_ApplyBaseAndYOffset(point, out);
                            }
                            screen.x = (screen.x >> 1) + x;
                            screen.y = screen.y + y;
#else
                            EffectPosition_ApplyBaseAndYOffset(point, &screen);
                            screen.x = (screen.x >> 1) + x;
                            /* The localized releases draw the trail sixteen pixels lower. */
                            screen.y = screen.y + y + 16;
#endif
                            trail->x -= 4;
                            draw[1](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[4],
                                screen.x - 2, screen.y - 5, 5, 10);
                        }
                    }
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattleFx_AdvanceScrollOnInterval);
    presentation->scroll_enabled = 0;
    gBgScroll[1].x = scroll_x;
    BattleEffect_SetupBlendedDisplay();
    for (i = 0; i != 9; i++)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)OBJECTS[i]);
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000000 = 0x7741;
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, Ram_MapCellBuffer, 1, 0);
    for (i = 0; i != 32; i++) {
        struct EffectStep *slash = &work->particles[i];
        s32 member = i % 6;

        if (member < work->effect->count) {
            EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &target);
            slash->y = -((Random16() & 31) + 40);
            slash->x = target.x / 2 + (80 - slash->y) / 2;
        } else {
            slash->x = (Random16() & 63) + 80;
            slash->y = -((Random16() & 31) + 40);
        }
        slash->variant = -1;
    }
    for (frame = 0; frame != 88; frame++) {
        for (i = 0; i != 24; i++) {
            struct EffectStep *slash = &work->particles[i];

            if (frame >= i * 2 || frame > 40) {
                if (slash->variant >= 0) {
                    if (slash->variant <= 23) {
                        s32 cell = slash->variant / 4;
                        u32 width;

                        draw[i & 1](canvas, Ram_MapCellBuffer + PuffArc_CellSourceOffsets[cell],
                            slash->x - ((width = PuffArc_CellWidths[cell]) >> 1) - 8,
                            slash->y + PuffArc_CellBiasY[cell] - 40,
                            width, PuffArc_CellHeights[cell]);
                        if (slash->variant <= 11) {
                            draw[0](canvas, (u8 *)work + 0x16ac, slash->x + 4, slash->y - 40, 16, 21);
                            draw[0](canvas, (u8 *)work + 0x17fc, slash->x - 16, slash->y - 19, 29, 35);
                        }
                    }
                    slash->variant++;
                } else {
                    s32 height = 24;

                    if (slash->y > 56)
                        height = height - slash->y + 56;
                    draw[0](canvas, (u8 *)work + 0x16ac, slash->x + 4, slash->y - 40, 16, 21);
                    draw[0](canvas, (u8 *)work + 0x17fc, slash->x - 16, slash->y - 19, 29, 35);
                    if (height > 0)
                        draw[0](canvas, (u8 *)work + 0x1bf3, slash->x - 20, slash->y + 16, 21, height);
                    slash->x -= 6;
                    slash->y += 12;
                    if (slash->y > 79) {
                        s32 member;

                        slash->variant = 0;
                        work->shake_frames = 2;
                        Audio_PlayCue(134);
                        member = i % 6;
                        if (member < work->effect->count) {
                            ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 8);
                            BattleMotion_ApplyVariantMotionFar(work->effect->actors[member], 1);
                        }
                    }
                }
            }
        }
        Camera_ApplyShake(4, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
