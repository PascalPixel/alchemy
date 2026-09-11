#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"

/* object/reset_and_clear_field59.c */
s32 Object_ResetAndClearField59(void *obj)
{
    Obj_Apply(obj, 0);
    FIELD_AT_OFFSET(obj, s8 *, 0x59) = 0;
    return 0;
}

/* object/motion/aim/move_toward_target.c */
/*
 * Step an object toward its linked target, or change its mode when the
 * remaining distance is too small.
 */

/*
 * Obj_SetMode names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee. The callee
 * takes one argument and returns one; it is fed a sum of squares and its
 * result used as a length, which reads as a square root but is not
 * established.
 */

s32 FixedPoint_Ratio(s32, s32);
s32 Object_SetPosition(s32, s32, s32, s32);
s32 Object_SetMode(s32, s32);

/*
 * The routine address is held in a local and passed as the trailing
 * argument -- that local is the callee load. The parameter is then reused
 * to hold the halfword at +0x64.
 */
s32 Motion_MoveTowardTarget(s32 arg0)
{
    s32 squareRoot = 0x030001d8;
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
        distance = Obj_SetMode(cellX *cellX + cellY *cellY,
                                 (s32)target, cellY, squareRoot);
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

/* object/motion/aim/turn_toward_linked_target.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 ArcTan2(s32, s32);

s32 Motion_TurnTowardLinkedTarget(void *object)
{
    s16 turn_step;
    u16 current_angle;
    s32 target_angle;
    void *linked_target;

    linked_target = FIELD_AT_OFFSET(object, void *, 0x68);
    if (linked_target != NULL) {
        FIELD_AT_OFFSET(object, u8, 0x5A) = (u8)(0xFE & FIELD_AT_OFFSET(object, u8, 0x5A));
        target_angle = (u16)ArcTan2(FIELD_AT_OFFSET(linked_target, s32, 0x10) - FIELD_AT_OFFSET(object, s32, 0x10), FIELD_AT_OFFSET(linked_target, s32, 8) - FIELD_AT_OFFSET(object, s32, 8));
        current_angle = FIELD_AT_OFFSET(object, u16, 6);
        turn_step = target_angle - current_angle;
        if (turn_step != 0) {
            if (turn_step > 0x1000) {
                turn_step = 0x1000;
            }
            if (turn_step < -0x1000) {
                turn_step = -0x1000;
            }
            FIELD_AT_OFFSET(object, u16, 6) = (u16)(current_angle + turn_step);
        }
    }
    return 1;
}

/* object/motion/act/set_action_callback.c */
extern s16 gCell[];
extern s32 Object_GetById(u32);
extern void Object_SetCallback(void *, void *);
extern const u8 gRom[];
extern const u8 gRom2[];
extern const u8 gRom3[];
extern const u8 gRom4[];
extern const u8 gRom5[];
extern const u8 gRom6[];
extern const u8 gRom7[];

void Motion_SetActionCallback(s8 *object, s32 kind)
{
    s32 index;
    switch ((u32)(kind - 1)) {
    case 0:
        kind = (s32)gRom;
        break;
    case 1:
        kind = (s32)gRom2;
        break;
    case 2:
        kind = (s32)gRom3;
        break;
    case 3:
        kind = (s32)gRom4;
        break;
    case 4:
        kind = (s32)gRom5;
        break;
    case 5:
        index = 250;
        *(s32 *)(object + 0x68) = Object_GetById(*(u32 *)&gCell[index]);
        kind = (s32)gRom6;
        break;
    case 6:
        kind = (s32)gRom7;
        break;
    default:
        break;
    }
    Object_SetCallback(object, (void *)kind);
}

/* object/facing/find_nearest_target.c */
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

s32 ArcTan2(s32, s32);

struct FacingEntry *Obj_Run(struct FacingEntry *self, s32 id)
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
