#include "SCENE.H"
#include "INVENTORY.H"
#include "GAME_FLAGS.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "PARTY_STATE.H"

extern struct PartyState gGameState;
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

        owner = Owner_GetState(gGameState.active_owners[n]);
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
                        GameFlag_SetBit(0x167);
                    }
                }
            }
        }
    }
}
