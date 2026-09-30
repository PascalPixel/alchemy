/* Near miss: score 220. ⚓️ reads the party's owners from gPartyState. After
   Item_GetDirect it reloads the spilled index and offset first and then
   forms the uses pointer (adds r5, r0, #0; adds r5, #24); this draft copies
   the result before the reloads. Pointer, index and struct forms and 60 s
   of permuting all keep the copy early. */
#include "SCENE.H"
#include "INVENTORY.H"
#include "GAME_FLAGS.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "PARTY_STATE.H"

struct ItemUses {
    u8 unknown_00[24];
    struct {
        u8 kind;
        u8 unknown_01[3];
    } use[4];
};

void GameFlag_ClearBit(s32 flag);
s32 GameFlag_SetBit(s32 flag);

/* game_flags/refresh_lure_cap.c */

void GameFlag_RefreshLureCap(void)
{
    s32 count;
    s32 n;

    GameFlag_ClearBit(0x167);
    count = Party_CountActiveOwners();
    for (n = 0; n < count; n++) {
        struct OwnerInventoryState *owner;
        s32 i;

        owner = Owner_GetState(gPartyState.active_owners[n]);
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
