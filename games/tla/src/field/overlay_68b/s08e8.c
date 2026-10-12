#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

extern u8 MsgAndTheOthers;

struct ObjectRuntime *Object_GetById(s32 id);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Func_02001d7c(s32 value);
void Func_02001bb4(s32 value);

void Func_020008e8(void)
{
    s32 y = Object_GetById(gPartyState.current_owner)->y;

    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the owner's y before it builds the limit (2 differ without it). */
    if (y < 0x5a0000) {
        UiText_ShowPositionedMessageAndWait((s32)&MsgAndTheOthers, 1);
        Func_02001d7c(126);
        Func_02001bb4(1);
    } else {
        FIELD_AT(Ram_HeapSlots->event_work, u16, 346) = 1;
    }
}
