/* 2026-09-30 (Mercury): EXACT, 472 of 472 bytes with stock agscc, no
   FAKEMATCH. It follows FIELD/COMMON/CHECK_CONFIGURED_KEYS_AND_COUNT (and
   the 0800f7f2 pad), so its module is Mars's to choose; compile it under
   #if defined(TBS_EDITION_EN) until the other editions adopt theirs. */
/* Exact: 472 bytes, code and pool.
 * What placed the reference's registers (all from reload order and
 * local-alloc ties, no compiler steering):
 * - motion = 12 comes before the key lookup, so its constant is the first
 *   reload (r0) and the halfword load's scratch follows in r2.
 * - The no-key case is its own small THEN block, blocked = 4. ifcvt case 1
 *   hoists it above the branch after the angle's zero extension, so its
 *   constant takes r3 and 0xffff takes r2; the round robin then gives 14,
 *   15, 10 and the zeroed position r3, r0, r2, r3.
 * - Both ground heights share one variable, which spans blocks, so the
 *   difference ties to object->y (r3) instead of the call result.
 * - The refusal tail is written once (goto refused), as the reference's
 *   single tail shows; two tails merged later by cross-jumping shift the
 *   reload round robin (the -0x80000 constant would take r0, not r1).
 * - The zeroed position and the projection call are written out in the
 *   body, which puts the 0x80000 argument ahead of the angle in sched2.
 * The frame keeps the 68 unused bytes above the position; delta_z captures
 * the displacement before the two facing stores (both sign tests use it).
 */
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "RAM_BUFFER.H"

struct KeyMoveEventWork {
    u8 unknown_000[0x19c];
    u16 blocked_steps;
};

struct KeyMovePosition {
    s32 x;
    s32 y;
    s32 z;
};

extern volatile u32 gKeysHeld;
extern struct KeyMoveEventWork *gEventWork;
extern const s16 Data_08013254[16];

void Vector_AddPolarOffset(s32 radius, s32 angle, struct KeyMovePosition *position);
s32 Func_08011f54(u32 layer, s32 x, s32 z);
void ObjectDispatch_ApplyArgumentToChildren(struct ObjectRuntime *object, s32 value);
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

/*
 * Walks an object half a tile in the direction the pad is held. The step is
 * refused where the next cell is occupied or changes height; with L or R
 * held it is refused where the ground rises or falls away too far. A step
 * the object cannot take counts as a blocked step in the event work.
 */
s32 Object_MoveByKeys(struct ObjectRuntime *object)
{
    struct KeyMovePosition pos;
    u8 unused[68];
    struct MapCell *from;
    struct MapCell *to;
    u16 angle;
    s32 motion;
    s32 blocked;
    s32 delta_z;
    s32 height;

    object->speed_limit = 0x8000;
    object->acceleration = 0x4000;
    motion = 12;
    angle = Data_08013254[(gKeysHeld >> 4) & 15];
    if (angle == 0xffff) {
        blocked = 4;
    } else {
        motion = 14;
        if ((angle & 0xf000) != 0) {
            motion = 15;
            if ((angle & 0xf000) != 0x8000)
                motion = 10;
        }
        blocked = 0;
        pos.x = 0;
        pos.y = 0;
        pos.z = 0;
        Vector_AddPolarOffset(0x80000, angle, &pos);
        pos.x += object->x;
        delta_z = pos.z;
        if (delta_z < 0)
            object->angle = 0xc000;
        if (delta_z > 0)
            object->angle = 0x4000;
        pos.y = object->y - pos.z;
        pos.z = object->z;
        from = &((struct MapCell *)Ram_MapCellBuffer)
            [object->x / 0x100000 + (pos.z / 0x100000) * 128];
        to = &((struct MapCell *)Ram_MapCellBuffer)
            [pos.x / 0x100000 + (pos.z / 0x100000) * 128];
        if (CheckMapPositionCellOccupied((struct WorldPosition *)&pos) != 0
            || from->collision_code != to->collision_code) {
            blocked = 4;
            motion = 12;
        } else if (gKeysHeld & 0x40) {
            height = Func_08011f54(object->terrain_id, pos.x, pos.z - 0x100000);
            if (height - object->y < 0x100000)
                goto refused;
        } else if (gKeysHeld & 0x80) {
            height = Func_08011f54(object->terrain_id, pos.x, pos.z);
            if (height - object->y > -0x80000) {
refused:
                blocked = 1;
                motion = 12;
            }
        }
    }
    if (gEventWork != NULL) {
        if (blocked & 3)
            gEventWork->blocked_steps++;
        else
            gEventWork->blocked_steps = 0;
    }
    ObjectDispatch_ApplyArgumentToChildren(object, motion);
    if (blocked != 0) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
    } else {
        Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    }
    object->step++;
    return 1;
}
