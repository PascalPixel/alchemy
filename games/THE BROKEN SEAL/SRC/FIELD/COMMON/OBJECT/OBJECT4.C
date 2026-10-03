#include "FACING_OBJECT.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "OBJECT_DISPATCH.H"
#include "ANIMSPR.H"
#include "SCRIPT_MOTION.H"

s32 ArcTan2(s32, s32);
void Battle_WaitMode0(s32);
s32 WaitFrames(s32 frames);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second);

extern u8 *gEventWork;
void Object_Destroy(void *);
void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *);
void ObjectGroup_SetChildValue(struct DispatchObject *, s32);
extern u32 gFrameCount;
extern u8 ObjectGroup_BlinkChildValues[];

void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait)
{
    struct ObjectRuntime *first;
    struct ObjectRuntime *second;

    first = ObjectTable_Get(first_id);
    second = ObjectTable_Get(second_id);
    if (first != 0 && second != 0) {
        first->angle = ArcTan2(second->z - first->z,
            second->x - first->x);
        Battle_WaitMode0(wait);
    }
}

void Object_LinkPair(s32 first_id, s32 second_id, s32 wait)
{
    void *first = ObjectTable_Get(first_id);
    void *second = ObjectTable_Get(second_id);

    if (first != NULL && second != NULL) {
        FacingObject_TurnPairToFaceEachOther(first, second);
        Battle_WaitMode0(wait);
    }
}

/* Turns two objects toward each other by at most 0x1000 per frame, for up
   to sixty frames, until both face along the line between them. */
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first, struct FacingObject *raw_second)
{
    struct ObjectRuntime *first = (struct ObjectRuntime *)raw_first;
    struct ObjectRuntime *second = (struct ObjectRuntime *)raw_second;
    s32 toward;
    s32 away;
    s32 frame;
    s32 turning;
    s32 delta;

    if (first == NULL || second == NULL)
        return;
    toward = (u16)ArcTan2(second->z - first->z,
        second->x - first->x);
    away = toward + 0x8000;
    for (frame = 0; frame < 60; frame++) {
        turning = 2;
        delta = (s16)(toward - first->angle);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            first->angle += delta;
        } else {
            turning = 1;
        }
        delta = (s16)(away - second->angle);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            second->angle += delta;
        } else {
            turning--;
        }
        if (turning == 0)
            break;
        WaitFrames(1);
    }
}

void ObjectTable_DestroyById(s32 index)
{
    void *object = ObjectTable_Get(index);
    u8 *base = gEventWork;
    s32 offset;

    if (object != 0) {
        Object_Destroy(object);
        offset = index * 4;
        offset += 20;
        *(s32 *)(base + offset) = 0;
    }
}

void ObjectTable_DestroyNoOp(void)
{
}

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
    s32 flags;
    struct DispatchObject *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL) {
        flags = 0x100 & value;
        if (flags != 0) {
            ((struct ScriptMotionObject *)object)->hook =
                (void (*)(struct ScriptMotionObject *))ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        ((struct ScriptMotionObject *)object)->hook = NULL;
        ObjectGroup_SetChildValue(object, value);
    }
}

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
    if ((object->kind & 0xf) == 1) {
        u8 child_value;
        struct AnimationObject *container;
        u8 child_count;

        child_value = ObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
        container = object->target.child;
        child_count = container->count;
        if (child_count != 0) {
            struct AnimationEntry **entries = container->entries;
            s32 remaining = child_count;
            do {
                struct AnimationEntry *entry = *entries++;
                if (entry != 0 && entry->script != 0) {
                    entry->param = child_value;
                }
                remaining--;
            } while (remaining != 0);
        }
        container->dirty = 1;
    }
}

void ObjectGroup_SetChildValue(struct DispatchObject *object, s32 value)
{
    if ((object->kind & 0xf) == 1) {
        struct AnimationObject *container = object->target.child;
        u8 raw_count = container->count;

        if (raw_count != 0) {
            struct AnimationEntry **entry = container->entries;
            u32 count = raw_count;
            do {
                struct AnimationEntry *item = *entry++;
                if (item != NULL && item->script != 0) {
                    item->param = value;
                }
                count--;
            } while (count != 0);
        }
        container->dirty = 1;
    }
}
