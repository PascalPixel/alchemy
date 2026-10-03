#include "TYPES.H"
#include "OBJDISP.H"
#include "IWRAM_CALL.H"
#include "OBJECT_RUNTIME.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "GAME_STATE.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "FIELD_SPRITE.H"
#include "ANIMSPR.H"

struct LinkedEffectObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct FieldSprite *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct LinkedEffectObject *resource;
    s32 (*callback)(void *);
};

LAYOUT_SIZE_GUARD(LinkedEffectObject_Size, struct LinkedEffectObject, 0x70);
LAYOUT_OFFSET_GUARD(LinkedEffectObject_Counter, struct LinkedEffectObject, counter, 0x64);
LAYOUT_OFFSET_GUARD(LinkedEffectObject_Resource, struct LinkedEffectObject, resource, 0x68);

void *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Object_Destroy(void *object);
s32 BattleFx_CopyLinkedObjectPosition(void *);
extern const u8 Data_0809fd38[];

extern struct ObjectRuntime *gObjectSlots;
void ObjectDispatch_SetSingleChildField26Far(void *, s32);
s32 ArcTan2(s32, s32);
struct ObjectRuntime *Object_GetById(u32);
extern const u8 ObjectMotion_ActionKind1Script[];
extern const u8 ObjectMotion_ActionKind2Script[];
extern const u8 ObjectMotion_ActionKind3Script[];
extern const u8 ObjectMotion_ActionKind4Script[];
extern const u8 ObjectMotion_MoveTowardTargetScript[];
extern const u8 ObjectMotion_TurnTowardLinkedScript[];
extern const u8 ObjectMotion_ResetActionScript[];
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_SetMode(struct ObjectRuntime *, s32);

void BattleFx_ConfigureLinkedObject(s32 id, s32 flags)
{
    struct LinkedEffectObject *object = ObjectTable_Get(id);
    struct LinkedEffectObject *child;
    struct FieldSprite *visual;
    s32 mode;

    child = 0;
    visual = 0;

    if (object == 0)
        return;

    if ((flags & 3) != 0) {
        if ((flags & 3) == 2 || object->resource == 0) {
            child = Object_CreateFar(209, object->x, object->y, object->z);
        }
    } else {
        child = object->resource;
        if (child == 0)
            return;
        Object_Destroy(child);
        /* FAKEMATCH: reuse the null visual value for the child link. */
        object->resource = (struct LinkedEffectObject *)visual;
        return;
    }

    if (child == 0)
        return;

    mode = flags & 3;
    switch (mode) {
    case 1:
        Object_SetMode((struct ObjectRuntime *)child, 1);
        object->resource = child;
        child->counter = 1;
        break;
    case 2:
        Object_SetMode((struct ObjectRuntime *)child, 2);
        ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)Data_0809fd38);
        child->counter = 1;
        break;
    }

    child->resource_id = id;
    child->value_55 = 0;
    child->callback = BattleFx_CopyLinkedObjectPosition;
    visual = child->visual;
    visual->flags = 0;
    child->resource = object;

    /* Sprite priority is bits2-3 of the third attribute's high byte. */
    if (flags & 0x100) {
        s32 mask = 13;
        u8 visual_flags = ((u8 *)visual)[9];

        mask = -mask;
        mask &= visual_flags;
        mask |= 4;
        ((u8 *)visual)[9] = mask;
    } else {
        s32 copied_flags = 12;
        u8 source_flags = ((u8 *)object->visual)[9];
        u8 destination_flags;
        s32 clear_mask = 13;

        copied_flags &= source_flags;
        destination_flags = ((u8 *)visual)[9];
        clear_mask = -clear_mask;
        clear_mask &= destination_flags;
        clear_mask |= copied_flags;
        ((u8 *)visual)[9] = clear_mask;
    }
}

s32 Object_ResetAndClearField59(void *obj)
{
    ObjectDispatch_SetSingleChildField26Far(obj, 0);
    ((struct ScriptObjectRuntime *)obj)->flags_59 = 0;
    return 0;
}

s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object)
{
    s32 step;
    struct ObjectRuntime *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    target = object->linked_object;
    if (target != 0) {
        deltaX = target->x - object->x;
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = target->z - object->z;
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
        step = object->action;
        if (distance >= step) {
            newX = object->x +
                (cellX << 20) / step;
            Object_SetPosition(object, newX, object->y,
                          object->z +
                              __divsi3(cellY << 20, step));
            Object_SetMode(object, 2);
        } else {
            Object_SetMode(object, 1);
        }
    }
    return 1;
}

s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object)
{
    s16 turn_step;
    u16 current_angle;
    s32 target_angle;
    struct ObjectRuntime *target;

    target = object->linked_object;
    if (target != NULL) {
        object->action_flags &= 0xfe;
        target_angle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
        current_angle = object->angle;
        turn_step = target_angle - current_angle;
        if (turn_step != 0) {
            if (turn_step > 0x1000)
                turn_step = 0x1000;
            if (turn_step < -0x1000)
                turn_step = -0x1000;
            object->angle = (u16)(current_angle + turn_step);
        }
    }
    return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind)
{
    switch ((u32)(kind - 1)) {
    case 0:
        kind = (s32)ObjectMotion_ActionKind1Script;
        break;
    case 1:
        kind = (s32)ObjectMotion_ActionKind2Script;
        break;
    case 2:
        kind = (s32)ObjectMotion_ActionKind3Script;
        break;
    case 3:
        kind = (s32)ObjectMotion_ActionKind4Script;
        break;
    case 4:
        kind = (s32)ObjectMotion_MoveTowardTargetScript;
        break;
    case 5:
        object->linked_object = Object_GetById(gGameState.selected_actor);
        kind = (s32)ObjectMotion_TurnTowardLinkedScript;
        break;
    case 6:
        kind = (s32)ObjectMotion_ResetActionScript;
        break;
    default:
        break;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)kind);
}

struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id)
{
    struct ObjectRuntime *entry;
    struct ObjectRuntime *found;
    s32 cnt;
    s32 best;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 turn;

    found = NULL;
    best = 40;
    entry = gObjectSlots;
    for (cnt = 0; cnt < 64; cnt++, entry++) {
        if (entry->script == NULL)
            continue;
        if (entry == self)
            continue;
        if (entry->animation_kind != 1)
            continue;
        dy = entry->y - self->y;
        if (dy >= 0) {
            if (dy > 0x2fffff)
                continue;
        } else {
            if (self->y - entry->y > 0x2fffff)
                continue;
        }
        dx = (entry->x - self->x) / 0x10000;
        dz = (entry->z - self->z) / 0x10000;
        dist = Iwram_Sqrt(dx *dx + dz *dz);
        if (dist >= best)
            continue;
        angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
        if (dist > 23) {
            turn = (s16)(angle - self->angle);
            if (turn < -0x2fff)
                continue;
            if (turn > 0x2fff)
                continue;
        }
        found = entry;
        best = dist;
    }
    if (found == NULL)
        return NULL;
    if (((struct AnimationObject *)found->animation)->entries[0]->anim_id != id)
        return NULL;
    return found;
}
