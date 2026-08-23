#include "inventory.h"
#include "game_flags.h"

void Func_08077c10(void)
{
    s32 count;
    s32 n;

    GameFlag_Clear(0x167);
    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        struct OwnerInventoryState *owner;
        s32 i;

        owner = OwnerState_Get(Data_02000240.active_owners[n]);
        for (i = 0; i < 15; i++) {
            if (owner->inventory[i] & 0x200) {
                u8 *record;
                s32 j;

                record = (u8 *)Item_GetDirect(owner->inventory[i]) + 24;
                for (j = 0; j < 4; j++) {
                    u8 kind;

                    kind = *record;
                    record += 4;
                    if (kind == 27) {
                        GameFlag_Set(0x167);
                    }
                }
            }
        }
    }
}
