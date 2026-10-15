/* Draft, differ 95 of 168: a rolling ride object's step (overlays 6ab, 6ac and 6ad hold copies). Left: the native keeps the leader in r4, saving it around __divsi3 (caller-save is profitable only with five references, this has four), so gPartyState takes r10 and the event work r9; here the leader takes r10 and the event work r11. The slot loop in the native loads the first id into r0 and reuses it as the call's argument. Measured: the direction as a ternary (pool load of a halfword 0xffff) or a u16 local (negs). */
#include "../../../../games/tla/src/field/overlay_6aa/room.h"

struct RideSlot {
    u16 id;
    u16 blocked;
};

struct RideWork {
    u8 unknown_00[4];
    u16 landed;
    u16 blocked;
    u8 unknown_08[2];
    u16 value_0a;
    u8 unknown_0c[0x48];
    struct RideSlot slots[4];
    u8 unknown_64[4];
    u16 value_68;
    u16 direction;
};

extern u8 Data_020023c4[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02001a1c(struct ObjectRuntime *a, struct ObjectRuntime *b);
s32 Func_02001a80(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02001ae0(struct ObjectRuntime *object)
{
    struct RideWork *work = (struct RideWork *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    u8 *event = Ram_HeapSlots->event_work;
    s32 dx = object->bounce;
    s32 dz;
    s32 i;
    s32 direction;
    struct RideSlot *slot;

    object->x += dx;
    object->y += object->gravity;
    dz = object->unknown_4c;
    object->z += dz;
    object->bounce = dx - dx / 18;
    object->unknown_4c = dz - dz / 16;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    ((struct SpriteParts *)object->animation)->unknown_12 += object->action;
    if (leader->y == leader->terrain_height && Func_02001a1c(object, leader) && gPartyState.render_mode == 0) {
        if (object->unknown_4c != 0) {
            work->value_68 = 0;
            {
                u16 *cell = &work->direction;

                if (object->unknown_4c > 0)
                    direction = 1;
                else
                    direction = 0xffff;
                *cell = direction;
            }
        }
        object->priority_flags = 2;
        {
            s32 speed = object->unknown_4c;

            object->unknown_4c = 0;
            object->z -= speed;
            object->gravity += speed >> 2;
        }
        work->landed = 1;
        *(u16 *)(event + 340) = work->value_0a;
    }
    slot = work->slots;
    for (i = 0; i < 4 && slot->id != 0; i++, slot++) {
        struct ObjectRuntime *other = Object_GetById(slot->id);

        if (other->y == other->terrain_height && Func_02001a80(object, other)) {
            if (slot->blocked == 0) {
                s32 speed;

                object->priority_flags = 2;
                speed = object->unknown_4c;
                object->unknown_4c = 0;
                object->z -= speed;
                object->gravity += speed >> 2;
            } else {
                work->blocked = 1;
            }
        }
    }
}
