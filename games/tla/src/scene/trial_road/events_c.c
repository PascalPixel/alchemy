#include "trial_road.h"

#include "scene_call.h"
#include "text/msg_ids.h"

struct EventWork15a {
    u8 unknown_000[0x15a];
    u16 value;
};

void Func_02005880(void);
void Func_02005888(void);
void Func_020059e8(s32 a);

void Func_020005b4(s32 id)
{
    s32 flag;

    if (id == 9 || id == 11)
        flag = 0x285;
    else
        flag = 0x286;
    if (!GameFlag_Test(flag)) {
        Func_02005880();
        Func_020059e8(0);
        UiText_ShowPositionedMessageAndWait((s32)MsgTrialRoadChestBlocked, 1);
        Func_02005888();
    } else {
        UiText_ShowPositionedMessageAndWait((s32)MsgTrialRoadPlaceItemPrompt, 13);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 1) == 0)
            ((struct EventWork15a *)Ram_HeapSlots->event_work)->value = 1;
    }
}

void Func_02000628(void)
{
}

void Func_0200062c(s32 id)
{
    struct TrialRoadMapWork *map = (struct TrialRoadMapWork *)Ram_HeapSlots->map_work;
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        u8 *cells = map->layers[0].cells;
        u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        cell[3] = bit | cell[3];
    }
}

void Func_02000660(s32 id)
{
    struct TrialRoadMapWork *map = (struct TrialRoadMapWork *)Ram_HeapSlots->map_work;
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        u8 *cells = map->layers[0].cells;
        u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);

        cell[3] &= 0x7f;
    }
}

s32 Func_02000694(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner = object->linked_object;

    object->x = owner->x;
    object->y = owner->y + (OBJECT_BYTE(object, 99) << 16);
    object->z = owner->z;
    object->terrain_height = owner->terrain_height;
    return 1;
}

extern const u8 Data_02005a7c[];

void Func_020006b4(s32 id)
{
    struct ObjectRuntime *owner = Object_GetById(id);
    struct ObjectRuntime *object;

    if (owner != NULL) {
        object = Object_Create(489, owner->x, owner->y, owner->z);
        if (object != NULL) {
            ObjectDispatch_SetSingleChildFlags(object, 0);
            Func_020057d0(object, Data_02005a7c);
            object->linked_object = owner;
            /* FAKEMATCH: stores the owner link before it copies the object for the priority flags (differ 2 without it; an explicit flag pointer is differ 27). */
            do { } while (0);
            object->priority_flags |= 0x80;
            object->motion_flags = 0;
            object->collision_flags = 0;
            OBJECT_BYTE(object, 99) = 16;
            owner->linked_object = object;
        }
    }
}
