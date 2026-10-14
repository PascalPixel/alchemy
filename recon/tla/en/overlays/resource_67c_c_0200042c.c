// Near misses of overlay 67c, each with its remaining difference
// (alchemy diff, instructions that differ):
//   Func_0200042c: differ 4: the listing loads the pool word -0x10000 before object->y, twice
//   Func_02000a98: differ 2: the listing loads r0 (16) last for the first Func_02003688 call
//   Func_02000bd4: differ 2: the listing loads r1 (38) before r0 (12) for Func_02000b24
//   Func_0200107c: differ 20: the listing keeps 0 in r5 and 34 in r6, the draft the other way round, and loads object->x after movs r1
//   Func_02001170: differ 2: the listing loads r1 (37) before r0 (15) for the third Func_02000b24 call
//   Func_02001520: differ 2: for the y step the listing loads object->y before target->y
//   Func_0200156c: differ 2: the listing stores the decremented byte before it zero-extends it; storing first reloads the byte or branches
//   Func_02001ea0: differ 2: the listing loads the sprite attribute byte before lsls of 0x4000; with rotation stored first the zero for next_frame comes from the pool
//   Func_02001fcc: differ 4: the listing loads r1 before r0 (0) for the first Func_02003bb8 call of each case
//   Func_0200349c: differ 42: the listing keeps height in r4 and reuses r0 for the entry, no r5/r6 saved
//   Func_02003994: differ 4: the listing loads the sprite and its attribute byte one instruction later, around movs r3, #3 and ands
//   Func_02003ba4: differ 2: the listing loads the pool base before lsls of the index; the permuter matches all three only with base and row temporaries inside a one-pass block (not adopted)
//   Func_02003bb8: differ 2: the listing loads the pool base before lsls of the index
//   Func_02003c00: differ 2: the listing loads the pool base before lsls of the index
#include "../../../../games/tla/src/field/overlay_67c/local.h"

void Func_0200042c(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (id == 10) {
        s32 row = object->z >> 20;

        if (row == 12) {
            object->motion_flags = 0;
            object->y += -0x10000;
            GameFlag_SetBit(0x95a);
            Func_02003d1c(0, 12, 1, 1, 9, row);
            Func_02003d1c(0, 12, 1, 1, 13, row);
        }
    } else if (id == 11) {
        if (object->x >> 20 == 51) {
            object->motion_flags = 0;
            object->y += -0x10000;
            GameFlag_SetBit(0x95b);
        }
    }
}

void Func_02000a98(void)
{
    s32 i;

    Func_02003628();
    Func_02003688(16, 4, 16, 2);
    Func_02003688(22, 4, 16, 0);
    for (i = 5; i >= 0; i--) {
        Func_020036b8(33, 55);
        WaitFrames(15);
    }
    GameFlag_SetBit(0xa66);
}

void Func_02000bd4(void)
{
    s32 i;
    s32 result;

    if (GameFlag_Test(0x249)) {
        Func_02000b24(12, 38);
        Func_02003628();
        Func_02003688(12, 2, 45, 1);
        for (i = 3; i >= 0; i--)
            result = Func_020036b8(33, 55);
        GameFlag_SetBit(0x248);
        if (result == 1)
            GameFlag_SetBit(0xa64);
    }
}

void Func_0200107c(s32 unused, s32 id)
{
    if ((u32)(id - 10) <= 1) {
        struct ObjectRuntime *object = Object_GetById(10);

        object->motion_flags = 0;
        Func_02003d1c(8, 33, 1, 1, object->x >> 20, 34);
        object = Object_GetById(11);
        object->motion_flags = 0;
        Func_02003d1c(8, 33, 1, 1, object->x >> 20, 34);
    }
}

void Func_02001170(void)
{
    s32 i;
    s32 result;

    if (GameFlag_Test(0x24b)) {
        Func_02000b24(12, 38);
        Func_02000b24(13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Func_02003688(38, 2, 19, 1);
        for (i = 8; i >= 0; i--)
            result = Func_020036b8(33, 55);
        GameFlag_SetBit(0x24a);
        if (result == 1)
            GameFlag_SetBit(0xa65);
    }
}

s32 Func_02001520(struct ObjectRuntime *object)
{
    struct ObjectRuntime *target = object->linked_object;

    object->bounce = (target->x - object->x) / 10;
    object->unknown_4c = (0x100000 + (target->y - object->y)) / 10;
    object->gravity = (target->z - object->z) / 10;
    *((u8 *)object + 0x62) = 10;
    return 0;
}

s32 Func_0200156c(struct ObjectRuntime *object)
{
    u8 *steps;
    u8 remaining;

    object->x += object->bounce;
    object->y += object->unknown_4c;
    object->z += object->gravity;
    steps = (u8 *)object + 0x62;
    remaining = *steps - 1;
    *steps = remaining;
    return remaining != 0;
}

void Func_02001ea0(struct ObjectRuntime *object)
{
    s32 phase = gFrameCount & 15;

    if (phase == 0) {
        struct ObjectRuntime *child = Engine_ObjectCreate_(0x10d, object->x, object->y, object->z);

        if (child != 0) {
            struct FieldSprite *sprite = child->animation;

            Func_02003ccc(child, Data_020040dc);
            child->motion_flags = 0;
            child->terrain_id = 1;
            if (sprite != 0) {
                Animation_ApplyChildArgument(sprite, 2);
                sprite->next_frame = 0;
                sprite->rotation = 0x4000;
                sprite->priority = 2;
            }
        }
    }
}

void Func_02001fcc(void)
{
    s32 counter = Func_02003bcc();

    switch (counter) {
    case 0:
        Func_02003bb8(0, 1);
        Func_02003bb8(1, 1);
        Func_02003ecc(170);
        break;
    case 0x34bbf:
        Func_02003bb8(0, 0);
        Func_02003bb8(1, 0);
        Func_02003ecc(-1);
        break;
    }
    Func_02003bd8(0x34bc0);
}

void Func_0200349c(u32 index, s32 height)
{
    struct MapWork *map = Ram_HeapSlots->map_work;
    struct PillarWork *work = &Data_0200536c;

    if (index <= 31) {
        struct PillarEntry *entry = &work->entries[index];

        if (entry->x != 0) {
            *(s32 *)(Ram_MapCollision + 0x1e4) = ((height / 0x80000) & 0xff) << 8;
            map->layers[0].terrain_ids[(entry->x >> 20) + (entry->z >> 20) * 128] = 121;
            entry->height = height;
        }
    }
}

void Func_02003994(struct ObjectRuntime *object, s32 priority)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    object->priority_flags = 2;
    ((struct FieldSprite *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02003cc4(object, 2);
    Func_02003ccc(object, Data_020044c8);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

s32 Func_02003ba4(s32 index)
{
    u8 *base = (u8 *)VENT_WORK;
    struct VentRow *row = (struct VentRow *)(base + index * 20);

    return row->active;
}

void Func_02003bb8(s32 index, s32 active)
{
    u8 *base = (u8 *)VENT_WORK;
    struct VentRow *row = (struct VentRow *)(base + index * 20);

    row->active = active;
}

void Func_02003c00(s32 index, s32 value)
{
    u8 *base = (u8 *)VENT_WORK;
    struct VentRow *row = (struct VentRow *)(base + index * 20);

    row->value = value;
}
