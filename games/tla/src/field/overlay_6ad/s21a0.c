#include "room.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
    u8 unknown_00[9];
    u8 unknown_09_low : 2;
    u8 part : 2;
    u8 unknown_0a[8];
    u16 unknown_12;
};

extern const u8 Data_02005d8c[];

void Func_02001e20(void);
void Func_02001ffc(void);
void Func_020022d0(struct ObjectRuntime *object);

void Func_020021a0(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02005498_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = -speed;
        Func_0200215c(object, 3);
        object->update = Func_02001e20;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02005480(object, 2);
    }
}

void Func_020021e8(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02005498_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = speed;
        Func_0200215c(object, 3);
        object->update = Func_02001ffc;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02005480(object, 2);
    }
}

void Func_02002230(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02005498_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = -speed;
        Func_0200215c(object, 3);
        object->update = Func_02001ffc;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02005480(object, 2);
    }
}

s32 Func_02002278(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    s32 dy;

    if (ABS(a->x - b->x) < 0xc0000) {
        dy = a->y - b->y;
        if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)
            return 1;
    }
    return 0;
}

void Func_020022d0(struct ObjectRuntime *object)
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
    if (leader->y == leader->terrain_height && Func_02002278(object, leader)) {
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 2;
        object->unknown_4c = 0;
    }
}

void Func_0200236c(struct ObjectRuntime *object, s32 unused)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02005480(object, 2);
    Func_02005490(object, Data_02005d8c);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

void Func_020023ac(s32 *position, s32 speed)
{
    struct ObjectRuntime *object = Func_02005498_(285, position[0], position[1], position[2] + 0xfffe0000);
    s32 spin;

    if (object != NULL) {
        object->bounce = Math_Cosine((u16)(Random16Far() * 3 >> 4) + 0x3000);
        object->gravity = (u16)Random16Far();
        object->unknown_4c = (u16)(Random16Far() << 1) + speed;
        spin = (u16)Random16Far() + 0xffff8000;
        object->action = spin;
        Func_0200236c(object, 2);
        object->update = (void (*)(void))Func_020022d0;
        Animation_SetStateFlags(object, 1);
    }
}
