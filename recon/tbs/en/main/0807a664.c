/* Draft, not exact: 316 of 316 bytes, score 220 (was 470), 11 register
   names and the packing loop's test. What closed the rest: the counter of the
   first two loops is the count of kept slots (one variable, r5), the packing
   loop has its own counter, and it reads each slot once into a word. Left:
   the listing tests that word as 16 bits (lsls #16, cmp) after stepping the
   read pointer, and keeps the read pointer in r4 and the write pointer in
   r1; here the test is on the whole register. It loads the address of the
   active owners before stepping the save pointer and keeps it in r0. This
   is the twin of InventorySnapshot_Restore (GAME/INVENTORY/RESTORE_SNAPSHOT.C).
   The zero written to a slot is a pool load only because the slots are u16:
   no symbol is needed. */
#include "TYPES.H"
#include "ITEM.H"
#include "GAME_STATE.H"

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

struct OwnerEquipment {
    u8 unknown_000[0xd8];
    u16 inventory[15];          /* 0xd8 */
};

struct ShortcutState {
    u8 unknown_000[0x220];
    s16 psynergy_shortcuts[2];  /* 0x220 */
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
    u16 *owners;

    save = gInventorySnapshot;
    if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
        *save++ = INVENTORY_SNAPSHOT_SENTINEL;
        first = ((struct ShortcutState *)&gGameState)->psynergy_shortcuts[0];
        second = ((struct ShortcutState *)&gGameState)->psynergy_shortcuts[1];
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
                u32 item = *src++;

                if ((u16)item != 0) {
                    *dst++ = item;
                    n++;
                }
            }
            for (; n < 15; n++)
                inventory[n] = 0;
            Owner_RefreshDerivedData(owner);
            Owner_RecalculateStats(owner);
        }
        *save++ = first;
        *save++ = second;
        owners = (u16 *)gGameState.active_owners;
        save[0] = owners[0];
        save[1] = owners[1];
        Inventory_AddAndEquip(0, 16);
        GameFlag_SetBit(0x952);
    }
    Owner_RefreshActiveRatios(1);
}
