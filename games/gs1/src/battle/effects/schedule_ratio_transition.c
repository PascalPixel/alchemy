#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "metadata_lookup.h"
#include "battle_effect_runtime.h"
#include "fixed_math.h"

/* battle/effects/misc/schedule_ratio_transition.c */
/*
 * Record a ratio-driven transition on the battle effect work block and
 * schedule its callback.
 *
 * The ratio helper is reached through a call-via-register veneer, so it is a
 * typed indirect call rather than a call to a function at the branch target.
 * It takes exactly two arguments, numerator and denominator; r2 is never set
 * at the call site, so a third argument must not be added.
 */
#include "types.h"
#include "scene.h"
#include "global_cells.h"

typedef s32 (*ArmRatio)(s32 numerator, s32 denominator);

struct Work_080936a0 {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void ScheduleCallbackAfterFrames(const void *arg0, s32 arg1);
extern u8 gRom;

void BattleFx_ScheduleRatioTransition(s32 arg0, s32 arg1)
{
    struct Work_080936a0 *state = *(struct Work_080936a0 **)ADDR_03001E70;
    s32 handle;
    s32 result;

    handle = Runtime_AllocateBlock(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    {
        ArmRatio ratio = (ArmRatio)0x0300013c;
        result = ratio(arg0, 0x10000);
    }
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    ScheduleCallbackAfterFrames(&gRom, 0xc94);
}

/* event/wait_for_display_field358_clear.c */
void WaitFrames(s32);
void Event_WaitForDisplayField358Clear(void)
{
  s32 cnt;
  void *work;
  u8 *p;
  work = *((void **)ADDR_03001E70);
  if ((*((s16 *)(((u8 *)Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E))) == 3)
  {
    cnt = 0;
    if ((*((s16 *)(((u8 *)work) + 0x358))) != 0)
    {
 do { loop_3: WaitFrames(1); cnt += 1; if (cnt <= 0x12B) { if ((*((s16 *)((p = (u8 *)work) + 0x358))) != 0) { goto loop_3; } } } while (0);
    }
  }
}

/* battle/effects/objects/fx_copy_linked_object_position.c */
/* battle/effects/objects/copy_linked_object_position.c */
s16 *Battle_Run(s16);

s32 BattleFx_CopyLinkedObjectPosition(void *obj)
{
    void *link;

    link = FIELD_AT_OFFSET(obj, void **, 0x68);
    if (link != NULL) {
        FIELD_AT_OFFSET(obj, s8 *, 0x55) = 0;
        FIELD_AT_OFFSET(obj, s32 *, 8) = (s32)FIELD_AT_OFFSET(link, s32 *, 8);
        FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32)(FIELD_AT_OFFSET(link, s32 *, 0xC) + (FIELD_AT_OFFSET(BattleFx_CopyLinkedObjectPosition(*Battle_Run(FIELD_AT_OFFSET(obj, s16 *, 0x66))), s8 *, 8) << 0x10) + 0x80000);
        FIELD_AT_OFFSET(obj, s32 *, 0x14) = (s32)FIELD_AT_OFFSET(link, s32 *, 0x14);
        FIELD_AT_OFFSET(obj, s32 *, 0x10) = (s32)FIELD_AT_OFFSET(link, s32 *, 0x10);
    }
    return 0;
}

/* battle/effects/objects/spawn_linked_resource_object.c */
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

struct BattleEffectLinkedObject *Battle_Run(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);

extern const u8 gRomLinkedObjectPosition[];

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
            Battle_Run(21, resource->x, resource->y, resource->z);

        if (object != 0) {
            ObjectDispatch_InitializeFar(object, gRomLinkedObjectPosition);
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

/* object/reset_and_clear_field59.c */
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
extern const u8 gRomAndClearField59[];
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
        kind = (s32)gRomAndClearField59;
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
