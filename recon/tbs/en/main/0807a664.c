/* Draft, not exact: 316 of 316 bytes, 3 differing instructions (score
   220), all in the packing loop's test. The listing loads each slot with
   ldrh into r2, tests it with lsls r3, r2, #16 and stores r2: a halfword
   register compared as a halfword, with the step of the read pointer
   between. Testing and storing *src (one load, CSE) here gives ldrh and a
   plain register copy for cmp instead. u16/s16 item variables give ldrsh
   with the lsls (combine folds the load into the compare's extension); a
   volatile read keeps ldrh but adds an asrs. Settled: the read pointer r4,
   write pointer r1, the shortcuts as gGameState fields, and the inventory
   pointer reused for the active owners (which puts that address in r0
   before the second save step). Twin of InventorySnapshot_Restore
   (GAME/INVENTORY/RESTORE_SNAPSHOT.C). */
#include "TYPES.H"
#include "ITEM.H"
#include "GAME_STATE.H"

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

struct OwnerEquipment {
    u8 unknown_000[0xd8];
    u16 inventory[15];          /* 0xd8 */
};

extern u16 gInventorySnapshot[];

struct OwnerEquipment *Owner_GetState(s32 owner);
void Owner_RefreshDerivedData(s32 owner);
void Owner_RecalculateStats(s32 owner);
void Inventory_AddAndEquip(s32 owner, s32 item);
void GameFlag_SetBit(s32 flag);
void Owner_RefreshActiveRatios(s32 mode);

/* Saves the party's inventories once, with the two Psynergy shortcuts and
   the first four active owners, then leaves each member only the items of
   type 6, packed to the front, and hands the first member item 16. */
void Func_0807a664(void)
{
    u16 *save;
    struct OwnerEquipment *st;
    u16 *src;
    u16 *dst;
    u16 *inventory;
    s32 owner;
    s32 i;
    s32 n;
    s16 first;
    s16 second;

    save = gInventorySnapshot;
    if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
        *save++ = INVENTORY_SNAPSHOT_SENTINEL;
        first = gGameState.first_shortcut;
        second = gGameState.second_shortcut;
        for (owner = 0; owner < 4; owner++) {
            st = Owner_GetState(owner);
            for (n = 0; n < 15; n++)
                *save++ = st->inventory[n];
            for (n = 0; n < 15; n++) {
                if (Item_GetDirect(st->inventory[n])->type != 6)
                    st->inventory[n] = 0;
            }
            n = 0;
            inventory = st->inventory;
            src = inventory;
            dst = inventory;
            for (i = 0; i < 15; i++) {
                if (*src != 0) {
                    *dst++ = *src;
                    n++;
                }
                src++;
            }
            for (; n < 15; n++)
                inventory[n] = 0;
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
        }
        *save++ = first;
        *save++ = second;
        inventory = (u16 *)gGameState.active_owners;
        save[0] = inventory[0];
        save[1] = inventory[1];
        Inventory_AddAndEquip(0, 16);
        GameFlag_SetBit(0x952);
    }
    Owner_RefreshActiveRatios(1);
}
