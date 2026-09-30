#include "FACING_OBJECT.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "OBJECT_DISPATCH.H"

struct ObjectPairPosition {
    u8 unknown_00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
};

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
    struct ObjectPairPosition *first;
    struct ObjectPairPosition *second;

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
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second)
{
    s32 toward;
    s32 away;
    s32 frame;
    s32 turning;
    s32 delta;

    if (first == NULL || second == NULL)
        return;
    toward = (u16)ArcTan2(second->position_z - first->position_z,
        second->position_x - first->position_x);
    away = toward + 0x8000;
    for (frame = 0; frame < 60; frame++) {
        turning = 2;
        delta = (s16)(toward - first->facing);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            first->facing += delta;
        } else {
            turning = 1;
        }
        delta = (s16)(away - second->facing);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            second->facing += delta;
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
            *(void (**)(struct DispatchObject *))((u8 *)object + 0x6c) =
                ObjectGroup_ApplyIndexedChildValue;
            return;
        }
        *(s32 *)((u8 *)object + 0x6c) = flags;
        ObjectGroup_SetChildValue(object, value);
    }
}

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
    if ((object->kind & 0xf) == 1) {
        u8 child_value;
        u8 *container;
        u8 child_count;

        child_value = ObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
        container = object->target.child;
        child_count = *(container + 0x27);
        if (child_count != 0) {
            u8 **entries = (u8 **)(container + 0x28);
            s32 remaining = child_count;
            do {
                u8 *entry = *entries++;
                if (entry != 0 && *(u32 *)(entry + 0x10) != 0) {
                    *(entry + 5) = child_value;
                }
                remaining--;
            } while (remaining != 0);
        }
        *(container + 0x25) = 1;
    }
}

void ObjectGroup_SetChildValue(struct DispatchObject *object, s32 value)
{
    if ((object->kind & 0xf) == 1) {
        u8 *container = object->target.child;
        u8 raw_count = container[0x27];

        if (raw_count != 0) {
            void **entry = (void **)(container + 0x28);
            u32 count = raw_count;
            do {
                void *item = *entry++;
                if (item != NULL && *(s32 *)((u8 *)item + 0x10) != 0) {
                    *((s8 *)item + 5) = value;
                }
                count--;
            } while (count != 0);
        }
        container[0x25] = 1;
    }
}
