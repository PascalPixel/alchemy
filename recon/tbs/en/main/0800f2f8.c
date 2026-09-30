/* 2026-09-30 (Mercury): EXACT, 1252 of 1252 bytes with stock agscc, no
   FAKEMATCH. It sits between FIELD/COMMON modules (after the 0800ebec
   listing, before CHECK_CONFIGURED_KEYS_AND_COUNT), so its module is Mars's
   to choose; compile it under #if defined(TBS_EDITION_EN) until the other
   editions adopt theirs. angle_offsets[28] uses only 8 entries and sizes
   the 92-byte frame; Data_08013274 + 12 wants its own label at 0x08013280. */
/*
 * World-map player movement: pick the heading from the pad, probe the map
 * ahead and to both sides, fall back to eight neighbouring headings, then
 * steer the object, smooth the map rotation and drop footprint effects.
 * Exact: 1252 of 1252 bytes.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "MAP.H"
#include "IWRAM_CALL.H"
#include "MAP_RENDER_WORK.H"

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
extern u8 *gEventWork;
extern u32 gKeysHeld;
extern u32 gKeysRepeat;
extern u8 gDebugMode;
extern const s16 Data_08013254[16];
extern const s32 Data_0801328c[16];
extern const u8 Data_08013274[];

void Vector_AddPolarOffset(s32 distance, s32 angle, struct Vec *position);
s32 CheckWorldMapCollisionRange(s32, struct WorldPosition *position);
void Object_SetMoveTarget(struct ObjectRuntime *, s32, s32, s32);
void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void ObjectDispatch_Initialize(void *, const void *);
s32 AnimationObjects_SelectAnimation(void *, s32);
s32 Field_CheckConfiguredKeys(void);
s32 FixedSqrt(s32);
struct ObjectRuntime *FieldObject_Create(s32, s32, s32, s32);
s32 GetWorldMapCollision(struct WorldPosition *);

s32 Func_0800f2f8(struct ObjectRuntime *object)
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
    if (gEventWork != 0) {
        if ((collision & 3) != 0)
            (*(u16 *)(gEventWork + 0x19c))++;
        else
            *(u16 *)(gEventWork + 0x19c) = 0;
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
    work = gMapWork;
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
