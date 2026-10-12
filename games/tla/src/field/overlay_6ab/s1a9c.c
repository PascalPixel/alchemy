#include "ov6ab.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
    u8 unknown_00[0x12];
    u16 unknown_12;
};

s32 Func_02001a44(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02001a9c(struct ObjectRuntime *object)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 dx = object->bounce;
    s32 dz;
    struct SpriteParts *parts;
    s32 rate;
    s32 scale;

    object->x += dx;
    object->y += object->gravity;
    dz = object->unknown_4c;
    object->z += dz;
    object->bounce = dx - dx / 18;
    object->unknown_4c = dz - dz / 16;
    /* FAKEMATCH: ⚓️ stores the Z speed before it loads the scale rate */
    asm volatile("");
    rate = object->speed_limit;
    scale = object->scale_x;
    parts = object->animation;
    object->scale_x = scale + rate;
    object->scale_y += object->acceleration;
    /* FAKEMATCH: ⚓️ stores the Y scale before it addresses the action */
    asm volatile("");
    parts->unknown_12 += object->action;
    if (leader->y == leader->terrain_height && Func_02001a44(object, leader)) {
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 2;
        object->unknown_4c = 0;
    }
}
