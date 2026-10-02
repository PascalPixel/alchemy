#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "OBJDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFFECT_RUNTIME.H"

struct State_080935b0 {
    u8 filler0[0xEC];
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern u8 Data_03001af4[];
u8 *Runtime_AllocateBlock(s32 kind, s32 size);
s32 BattleFx_StepRatioTransition(void);

struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};


s32 WaitFrames(s32 frames);
s16 *BattleAction_FindDescriptor(s16 action);
void *Resource_GetMetadataRecordFar(s16 id);

struct BattleEffectVisual {
    u8 unknown_00[9];
    u8 flags;
    u8 unknown_0a[28];
    u8 value_26;
};

struct BattleEffectResource {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct BattleEffectVisual *visual;
};

struct BattleEffectLinkedObject {
    u8 unknown_00[80];
    struct BattleEffectVisual *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct BattleEffectResource *resource;
    void (*callback)(void);
};

struct BattleEffectLinkedObject *Object_CreateFar(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);
void Object_SetMode(struct BattleEffectLinkedObject *object, s32 mode);
void Audio_PlayCue(s32 cue);
void Battle_WaitMode0(s32 state);
extern const u8 BattleFx_LinkedObjectScript[];
s32 BattleFx_CopyLinkedObjectPosition(void *obj);

void Map_SetWorkFourValues(s32 first, s32 second, s32 third, s32 fourth)
{
    struct State_080935b0 *work = gMapWork[0];

    work->first = first;
    work->second = second;
    work->third = third;
    work->fourth = fourth;
}

/* Eases the ratio at work + 0x34c from the start to the end value over the
   transition's duration, one step per frame, then unschedules itself.
   Declared int: the reference returns through r1, with no value. */
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
    *(u32 *)Data_03001af4 = *(u16 *)(work + 0x118) + 1;
    if (*step == *duration) {
        *duration = 0;
        Scheduler_RemoveCallback((u32)(BattleFx_StepRatioTransition));
    }
}

/*
 * Record a ratio-driven transition on the battle effect work block and
 * schedule its callback.
 */

/* the transition callback, Thumb address */
void BattleFx_ScheduleRatioTransition(s32 arg0, s32 arg1)
{
    struct Work_080936a0 *state = gMapWork[0];
    s32 handle;
    s32 result;

    handle = Runtime_AllocateBlock(27, 0xccc);
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
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_StepRatioTransition), 0xc94);
}

/* Wait (at most 300 frames) for the display work's pending flag at +0x358 to clear. */
void Event_WaitForDisplayField358Clear(void)
{
    s32 frames;
    u8 *work = gMapWork[0];

    if (*(s16 *)((u8 *)Runtime_AllocateBlock(0x1b, 0xccc) + 0x19e) == 3) {
        frames = 0;
        if (*(s16 *)(work + 0x358) != 0) {
            do {
                WaitFrames(1);
                frames++;
            } while (frames <= 0x12b && *(s16 *)(work + 0x358) != 0);
        }
    }
}

/* Snap an effect object onto the object it is linked to, raised by the
   linked descriptor's height. */
s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xc) = FIELD_AT_OFFSET(link, s32 *, 0xc)
            + (FIELD_AT_OFFSET(Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 16)
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
    struct BattleEffectResource *resource;

    if ((flags & 0xff) == 6) {
        Audio_PlayCue(110);
    }

    resource = ObjectTable_Get(resource_id);
    if (resource != 0) {
        struct BattleEffectLinkedObject *object =
            Object_CreateFar(21, resource->x, resource->y, resource->z);

        if (object != 0) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_LinkedObjectScript);
            Object_SetMode(object, flags & 15);
            object->value_55 = 0;
            object->counter = 0;
            object->resource_id = resource_id;
            object->callback = BattleFx_CopyLinkedObjectPosition;
            object->visual->value_26 = 0;
            object->resource = resource;

            if ((flags & 0x100) != 0) {
                s32 mask = 13;
                u8 visual_flags = object->visual->flags;

                mask = -mask;
                mask &= visual_flags;
                mask |= 4;
                object->visual->flags = mask;
            } else {
                s32 copied_flags = 12;
                u8 source_flags = resource->visual->flags;
                u8 flags;
                s32 clear_mask = 13;

                copied_flags &= source_flags;
                flags = object->visual->flags;
                clear_mask = -clear_mask;
                clear_mask &= flags;
                clear_mask |= copied_flags;
                object->visual->flags = clear_mask;
            }
        }
        Battle_WaitMode0(state);
    }
}
