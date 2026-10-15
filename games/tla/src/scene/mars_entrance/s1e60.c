#include "room.h"

extern const u8 Data_020056a0[];

void Func_02001ae0(void);
void Func_02001cbc(void);
void Func_02001f90(struct ObjectRuntime *object);

// Stops the object and sets its sprite part, then restarts its script at
// Data_020056a0. Overlays 6ab, 6ac and 6ad have the same function.
void Func_02001e1c(struct ObjectRuntime *object, s32 part)
{
    s32 zero = 0;

    object->motion_flags = zero;
    object->collision_flags = zero;
    {
        s32 three = 3;

        /* FAKEMATCH: an empty asm holding a 3 builds the part mask before the sprite loads, as the native does; plain, sched2 ranks the sprite load above the mask (differ 4). The asm ends CSE's memory of constants, so the zero is a variable. */
        asm volatile("" : : "r"(three));
        ((struct SpriteParts *)object->animation)->part = part;
    }
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004bd0(object, Data_020056a0);
    object->action = zero;
    object->speed_limit = zero;
    object->acceleration = zero;
}

void Func_02001e60(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02004bd8_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = -speed;
        Func_02001e1c(object, 3);
        object->update = Func_02001ae0;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02004bc0(object, 2);
    }
}

void Func_02001ea8(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02004bd8_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = speed;
        Func_02001e1c(object, 3);
        object->update = Func_02001cbc;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02004bc0(object, 2);
    }
}

void Func_02001ef0(s32 *position, s32 speed, s32 mode)
{
    struct ObjectRuntime *object = Func_02004bd8_(366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = -speed;
        Func_02001e1c(object, 3);
        object->update = Func_02001cbc;
        if (mode)
            object->unknown_5c[0] = 2;
        Func_02004bc0(object, 2);
    }
}

s32 Func_02001f38(struct ObjectRuntime *a, struct ObjectRuntime *b)
{
    s32 dy;

    if (ABS(a->x - b->x) < 0xc0000) {
        dy = a->y - b->y;
        if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)
            return 1;
    }
    return 0;
}

void Func_02001f90(struct ObjectRuntime *object)
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
    if (leader->y == leader->terrain_height && Func_02001f38(object, leader)) {
        object->z -= object->unknown_4c;
        object->gravity += object->unknown_4c >> 2;
        object->unknown_4c = 0;
    }
}

void Func_0200202c(struct ObjectRuntime *object, s32 unused)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004bc0(object, 2);
    Func_02004bd0(object, Data_020056a0);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

void Func_0200206c(s32 *position, s32 speed)
{
    struct ObjectRuntime *object = Func_02004bd8_(285, position[0], position[1], position[2] + 0xfffe0000);
    s32 spin;

    if (object != NULL) {
        object->bounce = Math_Cosine((u16)(Random16Far() * 3 >> 4) + 0x3000);
        object->gravity = (u16)Random16Far();
        object->unknown_4c = (u16)(Random16Far() << 1) + speed;
        spin = (u16)Random16Far() + 0xffff8000;
        object->action = spin;
        Func_0200202c(object, 2);
        object->update = (void (*)(void))Func_02001f90;
        Animation_SetStateFlags(object, 1);
    }
}
