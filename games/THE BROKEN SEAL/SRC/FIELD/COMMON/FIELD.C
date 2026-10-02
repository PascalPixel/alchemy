#include "GLOBAL_CELLS.H"
#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "IWRAM_CALL.H"
#include "MAP_RENDER_WORK.H"
#include "RAM_BUFFER.H"

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

/* The part of the saved game that holds the dash key setting. */
struct PlayerState {
    u8 unknown_000[0x21c];
    u16 dash_keys;
};

struct KeyMoveEventWork {
    u8 unknown_000[0x19c];
    u16 blocked_steps;
};

/* The sprite a world-map object draws: its OAM attribute words. */
struct WorldSprite {
    u8 unknown_00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 flip_x : 1;
    u16 flip_y : 1;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 unknown_0a[0x1c];
    u8 flags;
    u8 unknown_27[5];
    u8 *shadow;
};

extern struct PlayerState gGameState;
extern struct KeyMoveEventWork *gEventWork;
extern volatile u32 gKeysHeld;
extern u32 gKeysRepeat;
extern u8 gDebugMode;
extern const s16 Data_08013254[16];
extern const s32 Data_0801328c[16];
extern const u8 Data_08013274[];
void Vector_AddPolarOffset(s32 radius, s32 angle, struct Vec *position);
s32 Func_08011f54(u32 layer, s32 x, s32 z);
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectDispatch_ApplyArgumentToChildren(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_Initialize(void *, const void *);
s32 AnimationObjects_SelectAnimation(void *, s32);
s32 Field_CheckConfiguredKeys(void);
s32 FixedSqrt(s32);
struct ObjectRuntime *FieldObject_Create(s32, s32, s32, s32);

/* runtime/remap_bytes_by_table.c */
extern u8 Runtime_ByteRemapTable[];

/* The Japanese edition steers on the world map its own way, which stays in
   its scaffold for now. */
#if EDITION_INTERNATIONAL

/*
 * World-map player movement: pick the heading from the pad, probe the map
 * ahead and to both sides, fall back to eight neighbouring headings, then
 * steer the object, smooth the map rotation and drop footprint effects.
 * angle_offsets[28] uses only 8 entries and sizes the 92-byte frame.
 */
s32 Object_MoveOnWorldMap(struct ObjectRuntime *object)
{
    struct Vec position;
    struct Vec test_position;
    s16 angle_offsets[28]; /* the ROM frame keeps 28 slots; 8 are used */
    s32 collision;
    s32 angle;
    s32 mode;
    u16 direction;
    s32 angle_index;
    s32 count;
    s32 difference;
    s32 collision_kind;
    s32 square;
    s32 rate;
    struct MapRenderWork *work;
    s32 step;
    struct WorldSprite *sprite;
    struct ObjectRuntime *effect;
    struct WorldSprite *animation;

    mode = 2;
    collision = 0;
    if (gKeysHeld & gGameState.dash_keys) {
        object->speed_limit = 0x10000;
        object->acceleration = 0x14000;
        mode = 5;
    } else {
        object->speed_limit = 0x8000;
        object->acceleration = 0x4000;
    }
    if ((gKeysRepeat & 0x200) != 0)
        object->speed_limit = 0x40000;

    angle = Data_08013254[(gKeysHeld >> 4) & 15];
    direction = angle;
    if (direction == 0xffff) {
        collision |= 4;
        goto update_object;
    }

    collision = 0;
    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction, &position);
    if (gDebugMode != 0 && (gKeysHeld & 0x200) != 0)
        goto update_object;
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
        goto choose_direction;

    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction + 0x1000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction - 0x1000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction + 0x2000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    test_position.x = object->x;
    test_position.y = object->y;
    test_position.z = object->z;
    Vector_AddPolarOffset(0x70000, direction - 0x2000, &test_position);
    if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
        goto choose_direction;
    goto update_object;

choose_direction:
    count = 0;
    angle_offsets[count++] = direction + 0x1000;
    angle_offsets[count++] = direction - 0x1000;
    angle_offsets[count++] = direction + 0x2000;
    angle_offsets[count++] = direction - 0x2000;
    angle_offsets[count++] = direction + 0x3000;
    angle_offsets[count++] = direction - 0x3000;
    angle_offsets[count++] = direction + 0x4000;
    angle_offsets[count++] = direction - 0x4000;
    for (angle_index = 0; angle_index < count; angle_index++) {
        direction = angle_offsets[angle_index];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction, &position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction + 0x1000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction - 0x1000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction + 0x2000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        test_position.x = object->x;
        test_position.y = object->y;
        test_position.z = object->z;
        Vector_AddPolarOffset(0x70000, direction - 0x2000, &test_position);
        if (CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&test_position) != 0)
            continue;
        angle = (s16)direction;
        goto update_object;
    }
    collision |= 1;

update_object:
    if (gEventWork != NULL) {
        if ((collision & 3) != 0)
            gEventWork->blocked_steps++;
        else
            gEventWork->blocked_steps = 0;
    }
    if (collision != 0)
        ObjectDispatch_ApplyArgumentToChildren(object, 9);
    else
        ObjectDispatch_ApplyArgumentToChildren(object, mode);

    if (collision != 0) {
        object->target_x = -0x80000000;
        object->target_y = -0x80000000;
        object->target_z = -0x80000000;
        object->velocity_x = 0;
        object->velocity_z = 0;
        if ((collision & 3) != 0) {
            direction = angle;
            difference = (s16)(direction - object->angle);
            if (difference > 0x1000)
                difference = 0x1000;
            if (difference < -0x1000)
                difference = -0x1000;
            object->angle += difference;
        }
        object->action = 0;
        object->unknown_66 = 2;
        goto movement_done;
    }

    Object_SetMoveTarget(object, position.x, position.y, position.z);
    square = FixedSqrt(Iwram_MulQ16(object->velocity_x, object->velocity_x)
                       + Iwram_MulQ16(object->velocity_z, object->velocity_z));
    object->velocity_x = collision;
    object->velocity_z = collision;
    Vector_AddPolarOffset(square, (u16)angle, (struct Vec *)&object->velocity_x);
    if (object->action != 0)
        object->action--;

movement_done:
    work = gMapWork[0];
    rate = Data_0801328c[(gKeysHeld >> 4) & 15];
    step = (s16)(rate - work->rotation) / 8;
    if (step > 0x200)
        step = 0x200;
    if (step < -0x200)
        step = -0x200;
    if (step > -16 && step < 16)
        step = rate - work->rotation;
    work->rotation += step;

    if (object->animation_kind == 1) {
        sprite = object->animation;
        collision_kind = GetWorldMapCollision((struct WorldPosition *)&object->x);
        if (collision_kind == 9) {
            sprite->shadow[6] = 1;
            sprite->flags = 0;
        } else {
            sprite->shadow[6] = 9;
            sprite->flags = 1;
        }
        if (collision_kind == 6 && object->action == 0 && collision == 0) {
            effect = FieldObject_Create(24, object->x, object->y, object->z);
            if (effect != 0) {
                animation = effect->animation;
                ObjectDispatch_Initialize(effect, Data_08013274 + 12);
                effect->flags = collision;
                effect->terrain_id = 1;
                if (animation != 0) {
                    AnimationObjects_SelectAnimation(animation, 1);
                    animation->flags = collision;
                    animation->blend_mode = 1;
                    animation->priority = 2;
                }
                object->action = 10;
            }
        }
    }
    Field_CheckConfiguredKeys();
    object->step++;
    return 1;
}

#endif /* EDITION_INTERNATIONAL */

s32 Field_CheckConfiguredKeysAndCount(void *work)
{
    Field_CheckConfiguredKeys();
    FIELD_AT_OFFSET(work, u16 *, 4) = (u16)(FIELD_AT_OFFSET(work, u16 *, 4) + 1);
    return 1;
}

/*
 * Walks an object half a tile in the direction the pad is held. The step is
 * refused where the next cell is occupied or changes height; with L or R
 * held it is refused where the ground rises or falls away too far. A step
 * the object cannot take counts as a blocked step in the event work.
 *
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
s32 Object_MoveByKeys(struct ObjectRuntime *object)
{
    struct Vec pos;
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

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = Runtime_ByteRemapTable;
    n = cnt - 1;
    if (n != -1) {
        do {
            n -= 1;
            *p = tbl[*p];
            p += 1;
        } while (n != -1);
    }
}
