/*
 * Draft of Func_080d4330: sets the message window work's cursor and
 * scroll. 6 instructions differ, two swapped pairs in the second branch: the
 * listing loads the table address before it begins the field offset (ldr r2;
 * movs r1, #152) and reads the table byte before the offset is finished.
 * The table is read into a local before both stores: with the stores as two
 * plain statements the cursor is stored before the table read (16 differ).
 */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct MessageWindowWork {
    u8 unknown_0000[0x1384];
    u16 cursor;
    u16 scroll;
};

extern const u8 Data_080f330c[];

s32 ObjectTable_ReadActiveValue();
s32 Func_080d1eac();

void Func_080d4330(s32 id)
{
    struct MessageWindowWork *work = (struct MessageWindowWork *)Ram_HeapSlots->window_tiles;

    if (id == (s32)0x80000000) {
        work->cursor = 0;
        work->scroll = 0;
    } else {
        s32 cursor = Func_080d1eac(ObjectTable_ReadActiveValue(id));
        s32 scroll = Data_080f330c[gPartyState.event_mode];

        work->cursor = cursor;
        work->scroll = scroll;
    }
}
