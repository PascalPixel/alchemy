/* Near miss, differ 2 of 36 (ported from The Broken Seal's
   Ui_SetRenderResultFromObject). Plain, the second branch builds the result's
   offset (movs r1, #152) before it reads the event mode and the table, which
   the native reads first (differ 6). The one-pass block below (permute's
   best, 3 min) fixes that but ends at the first result's store, which the
   native schedules after the second offset's add (adds r1, #2). Blocks around
   the reads alone, around the stores, or with the table and mode in locals
   differ 4 to 12. */
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "window.h"

s32 ObjectTable_ReadActiveValue(s32 key);
s32 Func_080d1eac(s32 id);
extern const u8 Data_080f330c[];

void Func_080d4330(s32 key)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    u8 value;
    s32 id;

    if (key == 0x80000000) {
        *(s16 *)&work->result[0] = (value = 0);
        *(s16 *)&work->result[1] = value;
    } else {
        do {
            id = Func_080d1eac(ObjectTable_ReadActiveValue(key));
            value = Data_080f330c[gPartyState.event_mode];
            *(s16 *)&work->result[0] = id;
        } while (0);
        *(s16 *)&work->result[1] = value;
    }
}
