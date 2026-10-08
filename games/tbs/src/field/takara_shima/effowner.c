#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#include "staged_actor.h"
#include "field_effect.h"

enum {
    ITEM_NUT = 181
};

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

struct S_02000474 {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    u8 filler00[0x24];
    s32 callback;
};

typedef union {
    s32 w;
    s16 h[2];
} RecWord;

struct S_02001b14 { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

void SetEffectOwnerMode(struct FieldActor *object, s32 priority)
{
    struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)object->sprite;

    sprite->priority = priority;
}

struct FieldActor *SpawnMode14Effect(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        ((struct FieldSpritePriority *)object->sprite)->priority = 0;
        object->motion_flags = 0;
        object->collision_flags = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 14);
        Engine_ObjectSetBlendMode(object, 1);
        return object;
    }
    return 0;
}

struct FieldActor *SpawnMode15Effect(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *object = Engine_ObjectCreate(kind, x, y, z);

    if (object != 0) {
        ((struct FieldSpritePriority *)object->sprite)->priority = 1;
        object->motion_flags = 0;
        object->collision_flags = 8;
        Engine_ActorSetSpriteFlags(object, 0);
        ObjectGroup_SetChildValue(object, 15);
        object->priority_flags = (object->priority_flags & ~ACTOR_PRIORITY_AUTOMATIC) | ACTOR_PRIORITY_UNDERFOOT;
        return object;
    }
    return 0;
}
