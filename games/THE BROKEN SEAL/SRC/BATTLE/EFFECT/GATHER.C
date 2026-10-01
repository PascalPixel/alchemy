#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];

#define MOTES_PER_MEMBER 128

/* Battle effect: 128 motes leave every affected unit in random directions,
   one unit every twenty frames, and the first 32 of each unit are drawn
   while they are pulled towards the acting unit, where they vanish. */
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw;
    void *sheet;
    s32 facing;
    struct EffectPosition screen;
    s32 alternate;
    s32 member;
    s32 frame;
    s32 i;

    heap_cache = &gWorkSlot[39];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    facing = *(s32 *)((u8 *)gWorkSlot + 12 * 4);
    if (effect->variant == 0)
        alternate = 0;
    else
        alternate = 1;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Iwram_CopyWords((void *)BG_PLTT,
        Resource_GetTableEntry(alternate == 0
            ? (s32)&ResourceId_IceBlockSheet : (s32)&ResourceId_BlastSheet), 128);

    for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;

    for (member = 0; member != work->effect->count; member++) {
        s32 *object;
        s32 height;

        object = *GetBattleObjectSlotFar(work->effect->actors[member]);
        height = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
        for (i = 0; i != MOTES_PER_MEMBER; i++) {
            struct EffectStep *mote =
                &((struct EffectStep *)Ram_MapCellBuffer)[member * MOTES_PER_MEMBER + i];

            mote->x = object[2];
            mote->y = object[3] + height;
            mote->z = object[4];
            mote->velocity_x = ((Random16() & 0xff) - 128) << 10;
            mote->velocity_y = ((Random16() & 0xff) - 128) << 10;
            mote->velocity_z = ((Random16() & 0xff) - 128) << 10;
            mote->variant = 0;
        }
    }

    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw = (DrawRectangle)gWorkSlot[46];
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(142);

    for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
        s32 *target;
        s32 height;

        target = *GetBattleObjectSlotFar(work->effect->actor);
        height = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
        if (frame == 64)
            BattleEventRuntime_BeginPhaseFar(133);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame == 40)
            ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
        if (frame == work->effect->count * 20 + 52)
            ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
        for (member = 0; member != work->effect->count; member++) {
            if (frame == member * 20)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 42);
            if (frame > member * 20) {
                for (i = 0; i != 32; i++) {
                    struct EffectStep *mote =
                        &((struct EffectStep *)Ram_MapCellBuffer)[member * MOTES_PER_MEMBER + i];

                    if (mote->variant >= 0) {
                        s32 size;

                        EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
                        screen.x >>= 1;
                        size = 6;
                        draw(canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                            screen.x - size / 2, screen.y - size, size, size * 2);
                        EffectStep_AdvanceWithGravity3D(mote, 62, 0);
                        if (frame > member * 20 + i + 10) {
                            s32 dx;
                            s32 dy;
                            s32 dz;

                            dx = (target[2] - mote->x) >> 8;
                            dy = (target[3] + height - mote->y) >> 8;
                            dz = (target[4] - mote->z) >> 8;
                            mote->velocity_x += dx;
                            mote->velocity_y += dy;
                            mote->velocity_z += dz;
                            if ((dx > -0x1000 && dx < 0x1000) && (dz > -0x1000 && dz < 0x1000))
                                mote->variant = -1;
                        }
                    }
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
