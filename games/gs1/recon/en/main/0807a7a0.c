#include "game_flags.h"
#include "owner_state.h"
#include "psynergy_menu.h"

extern u16 Data_02001078[];

#define InventorySnapshot_Restore Func_0807a7a0

void InventorySnapshot_Restore(u32 sentinel)
{
    s32 owner = 0;
    u16 *source = Data_02001078;

    sentinel = *source;
    source++;
    if (sentinel == 0x6774) {
        do {
            u16 *inventory = (u16 *)((u8 *)OwnerState_Get(owner) + 0xD8);
            s32 remaining = 14;

            do {
                *inventory++ = *source++;
                remaining--;
            } while (remaining >= 0);
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
            owner++;
        } while (owner <= 3);

        Data_02000240.psynergy_shortcuts[0] = *source++;
        Data_02000240.psynergy_shortcuts[1] = *source++;
        *(u16 *)((u8 *)&Data_02000240 + 0x1F8) = *source++;
        *(u16 *)((u8 *)&Data_02000240 + 0x1FA) = *source;
        Data_02001078[0] = 0;
        GameFlag_Clear(0x952);
    }
}
