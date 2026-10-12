#include "types.h"
#include "ram_buffer.h"

struct Ov681EventWork {
    u8 unknown_000[0x15a];
    u16 unknown_15a;
};

extern const u8 MsgCheckedTheAltar[];

void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 position);
s32 PartyInventory_FindOwner(s32 item);

void Func_02001fd8(void)
{
    const u8 *message = MsgCheckedTheAltar;
    const u8 *next;
    s32 owner;

    UiText_ShowPositionedMessageAndWait(message, 1);
    owner = PartyInventory_FindOwner(451);
    next = message + 2;
    if (owner != -1) {
        struct Ov681EventWork *work = Ram_HeapSlots->event_work;

        /* FAKEMATCH: as overlay 680's Func_02000314, a one-pass block keeps the work pointer load before the message arguments (one reordered instruction, differ 2). */
        do {
        } while (0);
        UiText_ShowPositionedMessageAndWait(next, 1);
        work->unknown_15a = 1;
    } else {
        UiText_ShowPositionedMessageAndWait(next, 1);
    }
}
