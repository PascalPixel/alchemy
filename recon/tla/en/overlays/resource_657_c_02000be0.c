/* Near miss, differ 2 of 62: the native loads the party state address (ldr r5) right before the map mode store (strb); with the pointer local first and a one-pass block after the store this form loads it two instructions early, and without them right after the store. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "scene_ids.h"

extern const u8 Data_020020dc[];

struct ObjectRuntime *Object_GetById(s32 id);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020020c4(const void *data);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000210(void);
void Func_02000b84(void);

struct MapMode {
    u8 unknown_00[23];
    u8 low : 2;
    u8 mode : 2;
};

static __inline__ void AddCallback(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

s32 Func_02000be0(void)
{
    struct PartyState *party = &gPartyState;

    ((struct MapMode *)Ram_HeapSlots->map_work)->mode = 1;
    do { } while (0);
    if (party->scene == (s32)&SceneId_Ov657Room1) {
        Func_02000b84();
    } else {
        *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x100;
        do { } while (0);
        Object_GetById(party->current_owner)->priority_flags |= 0x20;
        Object_SetModeById(8, 1);
        Func_020020c4(Data_020020dc);
        AddCallback(Func_02000188, 0x480);
        if (party->entrance == 99)
            Func_02000210();
        else
            AddCallback(Func_020001cc, 0x480);
    }
    return 0;
}
