/* Draft, differ 95 of 168: a rolling ride object's step (overlays 6ab, 6ac and 6ad hold copies). Left: the native keeps the leader in r4, saving it around __divsi3 (caller-save is profitable only with five references, this has four), so gPartyState takes r10 and the event work r9; here the leader takes r10 and the event work r11. The slot loop in the native loads the first id into r0 and reuses it as the call's argument. Measured: the direction as a ternary (pool load of a halfword 0xffff) or a u16 local (negs). */
#include "../../../../games/tla/src/scene/mars_entrance/room.h"

struct RideSlot {
    u16 id;
    u16 blocked;
};

struct RideWork {
    u8 unk00[4];
    u16 landed;
    u16 blocked;
    u8 unk08[2];
    u16 value0a;
    u8 unk0c[0x48];
    struct RideSlot slots[4];
    u8 unk64[4];
    u16 value68;
    u16 direction;
};

extern u8 Data_020023c4[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_02001a1c(struct ObjectRuntime *a, struct ObjectRuntime *b);
s32 Func_02001a80(struct ObjectRuntime *a, struct ObjectRuntime *b);

void Func_02001ae0(struct ObjectRuntime *object)
{
    struct RideWork *work = (struct RideWork *)(Data_020023c4 + 0x188);
    struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
    u8 *event = Ram_HeapSlots->eventWork;
    s32 dx = object->bounce;
    s32 dz;
    s32 i;
    s32 direction;
    struct RideSlot *slot;

    object->x += dx;
    object->y += object->gravity;
    dz = object->unk4c;
    object->z += dz;
    object->bounce = dx - dx / 18;
    object->unk4c = dz - dz / 16;
    object->scaleX += object->speedLimit;
    object->scaleY += object->acceleration;
    ((struct SpriteParts *)object->animation)->unk12 += object->action;
    if (leader->y == leader->terrainHeight && Func_02001a1c(object, leader) && gPartyState.renderMode == 0) {
        if (object->unk4c != 0) {
            work->value68 = 0;
            {
                u16 *cell = &work->direction;

                if (object->unk4c > 0)
                    direction = 1;
                else
                    direction = 0xffff;
                *cell = direction;
            }
        }
        object->priorityFlags = 2;
        {
            s32 speed = object->unk4c;

            object->unk4c = 0;
            object->z -= speed;
            object->gravity += speed >> 2;
        }
        work->landed = 1;
        *(u16 *)(event + 340) = work->value0a;
    }
    slot = work->slots;
    for (i = 0; i < 4 && slot->id != 0; i++, slot++) {
        struct ObjectRuntime *other = Object_GetById(slot->id);

        if (other->y == other->terrainHeight && Func_02001a80(object, other)) {
            if (slot->blocked == 0) {
                s32 speed;

                object->priorityFlags = 2;
                speed = object->unk4c;
                object->unk4c = 0;
                object->z -= speed;
                object->gravity += speed >> 2;
            } else {
                work->blocked = 1;
            }
        }
    }
}
