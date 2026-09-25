#include "TYPES.H"

/* The party member's record: the item ids of its fifteen inventory slots. */
struct Member_387 {
    u8 pad[216];
    u16 items[15];
};

struct Item_387 {
    u8 pad[2];
    u16 flags;
    u8 pad4[8];
    u8 equippable;
};

struct Member_387 *Engine_OwnerGetState();
s32 Main_08077028(s32 member, s32 item);
struct Item_387 *Engine_DebugGetItem();
void Main_080772b0(s32 member, s32 slot);
void Main_08077050(s32 member, s32 slot);

/* Give member 2 item 65, dropping an item to make room while the inventory
 * is full (the last slot is cleared after a thousand tries), then equip every
 * slot holding it. */
void GomaIriguchi_GiveShamansRod(void)
{
    struct Member_387 *member;
    struct Item_387 *item;
    s32 tries;
    s32 i;
    s32 id;

    id = 65;
    member = Engine_OwnerGetState(2);
    tries = 0;
retry:
    if (++tries > 1000)
        member->items[14] = 0;
    if (Main_08077028(2, id) == -1) {
        for (i = 0; i <= 14; i++) {
            item = Engine_DebugGetItem(member->items[i]);
            if (((u8 *)item)[2] == 1) {
                Main_080772b0(2, i);
                goto retry;
            }
        }
        for (i = 0; i <= 14; i++) {
            item = Engine_DebugGetItem(member->items[i]);
            if ((item->flags & 0x8ff) == 0 && item->equippable == 1) {
                Main_080772b0(2, i);
                goto retry;
            }
        }
        goto retry;
    }
    for (i = 0; i <= 14; i++) {
        if (member->items[i] == id)
            Main_08077050(2, i);
    }
}
