#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
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

/* One slot of the cache, addressed whole. */
#define WORK_SLOT(kind) (*(void **)((u8 *)gWorkSlot + (kind) * 4))

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind,
    s32 side, s32 narrow, s32 *x, s32 *y);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void ObjectGroup_TickMemberTimers(void);

extern u16 BattleFx6_FlareCells[];

void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode);

/* Mode entries of battle effect series B. */
void BattleFx_RunSeriesBMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesB(effect, 0);
}

void BattleFx_RunSeriesBMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesB(effect, 1);
}

void BattleFx_RunSeriesBMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesB(effect, 2);
}

void BattleFx_RunSeriesBMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesB(effect, 3);
}

void BattleFx_RunSeriesBMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesB(effect, 5);
}

void BattleFx_RunSeriesBMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesB(effect, 4);
}

/* Battle effect: a djinni hovers over the acting side while flares close in
   on every affected unit, eight frames apart, and a burst and the mode's
   own picture play over the unit; six modes. */
void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode)
{
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    s32 member;
    s32 shift;
    s32 flags;
    s32 frame;
    s32 i;
    struct EffectPosition base;
    s32 rider[3];
    struct EffectPosition screen;
    s32 point[3];
    s32 origin_x;
    s32 origin_y;

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    if (work->effect->unknown_001c == 1) {
        if (mode == 3)
            BattleFx_PrepareCanvasEffect(effect, 0, work->effect->side ^ 1, 1,
                &origin_x, &origin_y);
        else if (mode == 2 || mode == 4)
            BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side ^ 1, 1,
                &origin_x, &origin_y);
        else
            BattleFx_PrepareCanvasEffect(effect, 2, work->effect->side ^ 1, 1,
                &origin_x, &origin_y);
        origin_x = origin_x * 4 / 5;
    }
    REG_BG2PA = 0xcc;
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, (u8 *)work + 0x60e, 1, 1);
    /* FAKEMATCH: forwarded through Call3, each palette copy loads its routine
       before it shifts the destination, as the reference does. */
    if (mode == 3 || mode == 5) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
            (u8 *)work + 0x2b8e, 1, 1);
        if (mode == 3)
            Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT,
                (s32)Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet), 128);
        else
            Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT,
                (s32)Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    } else if (mode == 4) {
        Resource_LoadAndDecompress((s32)&ResourceId_WispSheet, (u8 *)work + 0x2b8e, 1, 1);
    } else {
        void *palette;

        if (mode == 0)
            Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                (u8 *)work + 0x2b8e, 1, 0);
        else
            Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                (u8 *)work + 0x2b8e, 1, 0);
        if (mode == 0)
            palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        else if (mode == 2 || mode == 4)
            palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
        else if (mode == 1)
            palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        else
            palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
        Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT, (s32)palette, 128);
    }
    if (mode == 3)
        Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
            (u8 *)work + 0x65c0, 1, 0);
    else if (mode == 2 || mode == 4)
        Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
            (u8 *)work + 0x65c0, 1, 0);
    else
        Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
            (u8 *)work + 0x65c0, 1, 0);

    for (i = 0; i != 512; i++) {
        struct EffectStep *flare = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        flare->x = (Random16() % 200 - 100) << 14;
        flare->y = (Random16() % 200 - 100) << 15;
        flare->z = (Random16() % 200 - 100) << 14;
        flare->variant = 0;
    }

    if (work->effect->count == 1) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
        shift = -base.x * 4 / 5 + 64;
    } else {
        if (work->effect->side == 1)
            shift = -64;
        else
            shift = 0;
    }
    REG_BG2X = shift << 8;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (work->effect->side == 1)
        flags = 7;
    else
        flags = 3;
    Audio_PlayCue(142);

    for (frame = 0; frame != work->effect->count * 8 + 108; frame++) {
        s32 facing = (s32)WORK_SLOT(12);

        if (frame == 80)
            BattleEventRuntime_BeginPhaseFar(0);
        if (work->effect->unknown_001c == 1) {
            s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
            s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

            BattleEffect_LoadWork(46, 7, 7, flags ^ 4, 2);
            draw[0] = (DrawRectangle)gWorkSlot[46];
            BattleEffect_LoadWork(47, 7, 7, flags ^ 4, 3);
            draw[1] = (DrawRectangle)gWorkSlot[47];
            if (frame > 32)
                y = y - frame * 2 + 64;
            draw[0](canvas, (u8 *)work + 0x65c0, x, y, 40, 40);
            if (frame <= 3)
                draw[1](canvas, (u8 *)work + 0x65c0, x, y, 40, 40);
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
        }

        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *object =
                GetBattleObjectSlotFar(work->effect->actors[member])->object;
            s32 height = Battle_GetObjectTableValueFar(work->effect->actors[member]) * 2 / 3;

            if (frame == member * 8 + 80)
                Audio_PlayCue(212);
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
            point[0] = object->x;
            point[1] = height;
            point[2] = object->z;
            SceneTransform_ApplyPosition(point);
            if (frame == member * 8 + 48)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 16);
            if (frame > member * 8) {
                BattleEffect_LoadWork(46, 7, 7, flags, 2);
                draw[0] = (DrawRectangle)gWorkSlot[46];
                BattleEffect_LoadWork(47, 7, 7, flags, 3);
                draw[1] = (DrawRectangle)gWorkSlot[47];
                if (mode == 0) {
                    SceneTransform_ApplyPitch(-frame << 10);
                } else if (mode == 1) {
                } else if (mode == 2) {
                    SceneTransform_ApplyYaw(frame << 10);
                } else {
                    SceneTransform_ApplyYaw(frame << 10);
                    SceneTransform_ApplyRoll(frame << 10);
                }
                for (i = 0; i != 32; i++) {
                    struct EffectStep *flare =
                        &((struct EffectStep *)Ram_MapCellBuffer)[member * 64 + i];

                    if (frame > member * 8 + i) {
                        s32 xx = (flare->x >> 8) * (flare->x >> 8);
                        s32 yy = (flare->y >> 8) * (flare->y >> 8);
                        s32 zz = (flare->z >> 8) * (flare->z >> 8);
                        s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

                        if (distance != 0) {
                            s32 size;

                            EffectPosition_ApplyBaseAndYOffset((s32 *)flare, &screen);
                            screen.x = screen.x * 4 / 5 + shift;
                            if (screen.depth < 314)
                                screen.depth = 314;
                            if (screen.depth > 634)
                                screen.depth = 634;
                            size = 6 - (screen.depth - 314) / 64;
                            draw[1](canvas, (u8 *)work + BattleFx6_FlareCells[size - 1],
                                screen.x - size, screen.y - size, size * 2, size * 2);
                            flare->x -= flare->x / distance;
                            flare->y -= flare->y / distance;
                            flare->z -= flare->z / distance;
                        }
                    }
                }
                Runtime_ReleaseHeapBlock(47);
                Runtime_ReleaseHeapBlock(46);
            }
            rider[0] = 0;
            rider[1] = 0;
            rider[2] = 0;
            EffectPosition_ApplyBaseAndYOffset(rider, &screen);
            screen.x = screen.x * 4 / 5 + shift;
            if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
                s32 cel = (frame - member * 8 - 52) / 4 % 6;

                BattleEffect_LoadWork(46, 7, 7, flags, 2);
                draw[0] = (DrawRectangle)gWorkSlot[46];
                draw[0](canvas, (u8 *)work + cel * 1600 + 0x60e,
                    screen.x - 20, screen.y - 20, 40, 40);
                Runtime_ReleaseHeapBlock(46);
            }
            if (mode == 0) {
                if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
                    s32 cel = (frame - member * 8 - 80) / 4 % 7;

                    BattleEffect_LoadWork(46, 7, 7, flags, 2);
                    draw[0] = (DrawRectangle)gWorkSlot[46];
                    draw[0](canvas, (u8 *)work + cel * 960 + 0x2b8e,
                        screen.x - 12, screen.y - 20, 24, 40);
                    Runtime_ReleaseHeapBlock(46);
                }
            } else if (mode == 3 || mode == 5) {
                if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
                    s32 cel = (frame - member * 8 - 80) / 4 % 6;

                    BattleEffect_LoadWork(46, 7, 7, flags, 2);
                    draw[0] = (DrawRectangle)gWorkSlot[46];
                    draw[0](canvas, (u8 *)work + cel * 2048 + 0x2b8e,
                        screen.x - 16, screen.y - 32, 32, 64);
                    Runtime_ReleaseHeapBlock(46);
                }
            } else if (mode == 4) {
                if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
                    s32 cel = (frame - member * 8 - 80) / 2 % 6;

                    BattleEffect_LoadWork(46, 7, 7, flags, 2);
                    draw[0] = (DrawRectangle)gWorkSlot[46];
                    BattleEffect_LoadWork(47, 7, 7, flags | 8, 2);
                    draw[1] = (DrawRectangle)gWorkSlot[47];
                    draw[0](canvas, (u8 *)work + cel * 2048 + 0x2b8e,
                        screen.x - 32, screen.y - 24, 64, 32);
                    draw[1](canvas, (u8 *)work + cel * 2048 + 0x2b8e,
                        screen.x - 32, screen.y + 8, 64, 32);
                    Runtime_ReleaseHeapBlock(47);
                    Runtime_ReleaseHeapBlock(46);
                }
            } else {
                if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
                    s32 cel = (frame - member * 8 - 80) / 4 % 6;

                    BattleEffect_LoadWork(46, 7, 7, flags, 3);
                    draw[0] = (DrawRectangle)gWorkSlot[46];
                    draw[0](canvas, (u8 *)work + cel * 1600 + 0x2b8e,
                        screen.x - 20, screen.y - 20, 40, 40);
                    Runtime_ReleaseHeapBlock(46);
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
