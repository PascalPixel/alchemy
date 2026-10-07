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
#include "ANIMSPR.H"
#include "FIELDOBJ.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "OBJDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFFECT_RUNTIME.H"

#include "MAP_SCROLL.H"

#define WaitFrames ((s32 (*)(s32))WaitFrames)

#define gScanlinePitch ((u8 (*))&gScanlinePitch)


struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

s16 *BattleAction_FindDescriptor(s16 action);

struct FieldActor *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Object_SetMode(struct FieldActor *object, s32 mode);
void Audio_PlayCue(s32 cue);
void Battle_WaitMode0(s32 state);

void Map_SetViewBounds(s32 first, s32 second, s32 third, s32 fourth)
{
    struct MapScrollWork *work = gMapWork[0];

    work->min_x = first;
    work->min_y = second;
    work->max_x = third;
    work->max_y = fourth;
}

s32 BattleFx_StepRatioTransition(void)
{
    u8 *work;
    s16 *duration;
    s32 *from;
    s16 *step;
    s32 offset;
    s32 delta;

    work = gMapWork[0];
    if ((*(u8 **)(Runtime_AllocateBlock(27, 0xccc) + 480))[91] != 0)
        return;
    duration = (s16 *)(work + 0x358);
    if (*duration == 0)
        return;
    from = (s32 *)(work + 0x350);
    delta = *(s32 *)(work + 0x354) - *from;
    step = (s16 *)(work + 0x35a);
    (*step)++;
    offset = *from + __divsi3(delta * *step, *duration);
    *(s32 *)(work + 0x34c) = Iwram_MulQ16(*(s32 *)(work + 0x348), offset);
    *(u32 *)gScanlinePitch = *(u16 *)(work + 0x118) + 1;
    if (*step == *duration) {
        *duration = 0;
        Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
    }
}

void BattleFx_ScheduleRatioTransition(s32 arg0, s32 arg1)
{
    struct Work_080936a0 *state = gMapWork[0];
    s32 handle;
    s32 result;

    handle = (s32)Runtime_AllocateBlock(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    {
        s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14;
        result = ratio(arg0, 0x10000);
    }
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition, 0xc94);
}

void Event_WaitForZoomTransition(void)
{
    s32 frames;
    u8 *work = gMapWork[0];

    if (((struct EventWork *)Runtime_AllocateBlock(0x1b, 0xccc))->mode == 3) {
        frames = 0;
        if (*(s16 *)&((struct PerspectiveWork *)work)->zoom_transition_frames != 0) {
            do {
                WaitFrames(1);
                frames++;
            } while (frames <= 0x12b && *(s16 *)&((struct PerspectiveWork *)work)->zoom_transition_frames != 0);
        }
    }
}

s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xc) = FIELD_AT_OFFSET(link, s32 *, 0xc)
            + ((s8)Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(FIELD_AT_OFFSET(obj, s16 *, 0x66)))->box_x << 16)
            + 0x80000;
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}

void BattleFx_SpawnLinked(
    s32 resource_id,
    s32 flags,
    s32 state)
{
    struct FieldActor *resource;

    if ((flags & 0xff) == 6) {
        Audio_PlayCue(110);
    }

    resource = ObjectTable_Get(resource_id);
    if (resource != 0) {
        struct FieldActor *object =
            Object_CreateFar(21, resource->x.fixed, resource->y.fixed, resource->z.fixed);

        if (object != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattleFx_LinkedObjectScript);
            Object_SetMode(object, flags & 15);
            object->motion_flags = 0;
            object->timer = 0;
            object->work = resource_id;
            object->update = (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
            object->sprite->flags = 0;
            *(struct FieldActor **)&object->unknown_68[0] = resource;

            if ((flags & 0x100) != 0) {
                s32 mask = 13;
                u8 visual_flags = ((u8 *)&((struct AnimationObject *)object->sprite)->part[0])[9];

                mask = -mask;
                mask &= visual_flags;
                mask |= 4;
                ((u8 *)&((struct AnimationObject *)object->sprite)->part[0])[9] = mask;
            } else {
                s32 copied_flags = 12;
                u8 source_flags = ((u8 *)&((struct AnimationObject *)resource->sprite)->part[0])[9];
                u8 flags;
                s32 clear_mask = 13;

                copied_flags &= source_flags;
                flags = ((u8 *)&((struct AnimationObject *)object->sprite)->part[0])[9];
                clear_mask = -clear_mask;
                clear_mask &= flags;
                clear_mask |= copied_flags;
                ((u8 *)&((struct AnimationObject *)object->sprite)->part[0])[9] = clear_mask;
            }
        }
        Battle_WaitMode0(state);
    }
}
