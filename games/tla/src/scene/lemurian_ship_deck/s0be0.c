#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "scene_ids.h"

extern const u8 gData_020020dc[];

struct ObjectRuntime *Object_GetById(s32 id);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020020c4(const void *data);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000210(void);
void Func_02000b84(void);

struct MapMode {
    u8 unk00[23];
    struct MapModeBits { u8 low : 2; u8 mode : 2; } __attribute__((packed)) bits;
};

static __inline__ void AddCallback(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

s32 Func_02000be0(void)
{
    struct PartyState *party;
    struct MapMode *map = (struct MapMode *)Ram_HeapSlots->mapWork;
    struct MapModeBits bits = map->bits;

    bits.mode = 1;
    /* FAKEMATCH: the mode goes through a one-byte copy with the loop note before its store: the native loads the party state address between the or and the store. As map->bits.mode = 1 the load comes right after the store (differ 2), and with the note after that store two rows early. */
    do { } while (0);
    party = &gPartyState;
    map->bits = bits;
    if (party->scene == (s32)&SceneId_Ov657Room1) {
        Func_02000b84();
    } else {
        *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
        do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block after the store; without it the ROM differs (measured by make compare-tla-en). */
        Object_GetById(party->currentOwner)->priorityFlags |= 0x20;
        Object_SetModeById(8, 1);
        Func_020020c4(gData_020020dc);
        AddCallback(Func_02000188, 0x480);
        if (party->entrance == 99)
            Func_02000210();
        else
            AddCallback(Func_020001cc, 0x480);
    }
    return 0;
}
