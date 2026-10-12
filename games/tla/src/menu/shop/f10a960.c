#include "types.h"
#include "ram_buffer.h"

extern u8 MsgReviveService[];
extern u8 MsgCurePoisonService[];
extern u8 MsgExorcismService[];
extern u8 MsgRemoveCurseService[];

s32 Shop_MsgByMode(s32 value)
{
    s8 *shop;
    s8 mode;

    /* FAKEMATCH: the one-pass block ends the scheduling block: the native loads the shop work before it builds the offset of the service kind (one reordered row, differ 2 without). */
    do {
        shop = Ram_HeapSlots->menu_runtime;
    } while (0);
    mode = shop[0x507];
    if (mode == 1) {
        value += (u32)MsgCurePoisonService - (u32)MsgReviveService;
    }
    if (mode == 2) {
        value += (u32)MsgExorcismService - (u32)MsgReviveService;
    }
    if (mode == 3) {
        value += (u32)MsgRemoveCurseService - (u32)MsgReviveService;
    }
    return value;
}
