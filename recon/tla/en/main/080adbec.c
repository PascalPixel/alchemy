#include "scene.h"
#include "inventory.h"
#include "game_flags.h"
#include "types.h"
#include "resource.h"
#include "party_state.h"

struct ItemUses {
    u8 unk00[24];
    struct {
        u8 kind;
        u8 unk01[3];
    } use[4];
};

void GameFlag_ClearBit(s32 flag);
s32 GameFlag_SetBit(s32 flag);

void GameFlag_RefreshLureCap(void)
{
    s32 count;
    s32 n;

    GameFlag_ClearBit(0x167);
    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        struct OwnerInventoryState *owner;
        s32 i;

        owner = UnitGet(gPartyState.activeOwners[n]);
        for (i = 0; i < 15; i++) {
            if (owner->inventory[i] & 0x200) {
                struct ItemUses *uses;
                s32 j;

                uses = (struct ItemUses *)Item_GetDirect(owner->inventory[i]);
                for (j = 0; j < 4; j++) {
                    if (uses->use[j].kind == 27) {
                        GameFlag_SetBit(0x167);
                    }
                }
            }
        }
    }
}
