#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 ObjectDispatch_SetSingleChildField26Far(void *, s32);

s32 Object_ResetAndClearField59(void *obj)
{
    ObjectDispatch_SetSingleChildField26Far(obj, 0);
    FIELD_AT_OFFSET(obj, s8 *, 0x59) = 0;
    return 0;
}

#include "OBJECT_RUNTIME.H"

u16 ArcTan2(s32, s32);
void *Object_GetById(u32);
void Object_SetCallback(void *, void *);
extern s16 gGameState[];
extern const u8 ObjectMotion_ActionKind1Script[];
extern const u8 ObjectMotion_ActionKind2Script[];
extern const u8 ObjectMotion_ActionKind3Script[];
extern const u8 ObjectMotion_ActionKind4Script[];
extern const u8 ObjectMotion_MoveTowardTargetScript[];
extern const u8 ObjectMotion_TurnTowardLinkedScript[];
extern const u8 ObjectMotion_ResetActionScript[];

s32 FixedPoint_Ratio(s32, s32);
s32 Object_SetPosition(s32, s32, s32, s32);
s32 Object_SetMode(s32, s32);

s32 ObjectMotion_MoveTowardTarget(s32 arg0)
{
    s32 object;
    void *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    object = arg0;
    target = *(void **)(object + 0x68);
    if (target != 0) {
        deltaX = *(s32 *)(target + 8) - *(s32 *)(object + 8);
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = *(s32 *)(target + 0x10) - *(s32 *)(object + 0x10);
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = ((s32 (*)(s32))0x030001d8)(cellX * cellX + cellY * cellY);
        arg0 = *(s16 *)(object + 0x64);
        if (distance >= arg0) {
            newX = *(s32 *)(object + 8) +
                FixedPoint_Ratio(cellX << 20, arg0);
            Object_SetPosition(object, newX, *(s32 *)(object + 0x0c),
                          *(s32 *)(object + 0x10) +
                              FixedPoint_Ratio(cellY << 20, arg0));
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
        current_angle = *(u16 *)((u8 *)object + 6);
        turn_step = target_angle - current_angle;
        if (turn_step != 0) {
            if (turn_step > 0x1000)
                turn_step = 0x1000;
            if (turn_step < -0x1000)
                turn_step = -0x1000;
            *(u16 *)((u8 *)object + 6) = (u16)(current_angle + turn_step);
        }
    }
    return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind)
{
    s32 index;

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
        index = 250;
        object->linked_object = Object_GetById(*(u32 *)&gGameState[index]);
        kind = (s32)ObjectMotion_TurnTowardLinkedScript;
        break;
    case 6:
        kind = (s32)ObjectMotion_ResetActionScript;
        break;
    default:
        break;
    }
    Object_SetCallback(object, (void *)kind);
}

#include "TYPES.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"

struct FacingRecord {
    u8 unknown_00[0x28];
    s16 *id;
};

struct FacingEntry {
    void *data;
    u8 unknown_04[2];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x3c];
    struct FacingRecord *record;
    u8 kind;
    u8 unknown_55[0x1b];
};


struct FacingEntry *Object_FindNearestFacingTarget(struct FacingEntry *self, s32 id)
{
    struct FacingEntry *entry;
    struct FacingEntry *found;
    struct FacingEntry *result;
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
    entry = *(struct FacingEntry **)ADDR_03001E64;
    for (cnt = 0; cnt < 64; cnt++, entry++) {
        if (entry->data == NULL)
            continue;
        if (entry == self)
            continue;
        if (entry->kind != 1)
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
        dist = ((s32 (*)(s32))0x030001d8)(dx *dx + dz *dz);
        if (dist >= best)
            continue;
        angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
        if (dist > 23) {
            turn = (s16)(angle - self->facing);
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
    if (*found->record->id != id)
        return NULL;
    return found;
}
