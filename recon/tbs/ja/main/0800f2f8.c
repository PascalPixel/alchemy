/* DRAFT: Japanese world-map movement at 0800f2f8, 1092 bytes including pools.
   It projects three points at radius 0x80000, tests their combined collision,
   then tries six headings. Six angle slots are used; no frame padding.
   Approved agscc produces a 68-byte frame versus the ROMs 100-byte frame.
   2026-10-02: initial score 940; naming calls from the current Japanese
   linked build gives 540 (11 register-only, 15 stack-only, 7 operand,
   2 reordered, 2 deleted). Data_08013274 and Data_0801328c have no Japanese
   labels; Data_08013254 is typed Thumb code. The 32 unread frame bytes
   remain unexplained. */
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

s32 Object_MoveOnWorldMap(struct ObjectRuntime *object)
{
    struct Vec position;
    struct Vec left_position;
    struct Vec right_position;
    s16 angle_offsets[6];
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
    Vector_AddPolarOffset(0x80000, direction, &position);
    if (gDebugMode != 0 && (gKeysHeld & 0x200) != 0)
        goto update_object;
    left_position.x = object->x;
    left_position.y = object->y;
    left_position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction + 0x1555, &left_position);
    right_position.x = object->x;
    right_position.y = object->y;
    right_position.z = object->z;
    Vector_AddPolarOffset(0x80000, direction - 0x1555, &right_position);
    if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
         | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&left_position)
         | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&right_position)) == 0)
    goto update_object;

choose_direction:
    count = 0;
    angle_offsets[count++] = direction + 0x1000;
    angle_offsets[count++] = direction - 0x1000;
    angle_offsets[count++] = direction + 0x2000;
    angle_offsets[count++] = direction - 0x2000;
    angle_offsets[count++] = direction + 0x3000;
    angle_offsets[count++] = direction - 0x3000;
    for (angle_index = 0; angle_index < count; angle_index++) {
        direction = angle_offsets[angle_index];
        position.x = object->x;
        position.y = object->y;
        position.z = object->z;
        Vector_AddPolarOffset(0x80000, direction, &position);
        left_position.x = object->x;
        left_position.y = object->y;
        left_position.z = object->z;
        Vector_AddPolarOffset(0x80000, direction + 0x1555, &left_position);
        right_position.x = object->x;
        right_position.y = object->y;
        right_position.z = object->z;
        Vector_AddPolarOffset(0x80000, direction - 0x1555, &right_position);
        if ((CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&position)
                 | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&left_position)
                 | CheckWorldMapCollisionRange((s32)object, (struct WorldPosition *)&right_position)) != 0)
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

