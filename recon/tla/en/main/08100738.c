#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_TYPES.H"

struct ItemOwner {
    u8 padding_000[0x10];
    u16 stat_10;
    u16 stat_12;
    u8 padding_014[4];
    u16 stat_18;
    u16 stat_1a;
    u16 stat_1c;
    u8 stat_1e;
    u8 padding_01f[0x15];
    s16 max_hp;                     /* 0x034 */
    s16 max_pp;
    s16 hp;
    s16 pp;
    u8 padding_03c[0x9c];
    u16 items[15];                  /* 0x0d8 */
    u8 padding_0f6[0x3b];
    s8 poison;                      /* 0x131 */
};

struct ItemData {
    u8 padding_00[0x0c];
    u8 kind;
    u8 padding_0d[0x1b];
    u16 use_ability;
};

struct ItemUseWork {
    u8 padding_000[0x1c8];
    u16 entries[32];
    u16 targets[8];                 /* 0x208, the party list */
    u8 entry_count;                 /* 0x218 */
    u8 target_count;                /* 0x219 */
    u8 padding_21a[0x40];
    s16 result_code;                /* 0x25a */
};

extern struct ItemUseWork *gMenuWork;

struct ItemOwner *Owner_GetStateFar(s32);
struct ItemData *Item_Get(s32);
u8 Inventory_RemoveFar(s32, s32);
u32 ItemMenu_Collect(struct ItemOwner *, u16 *, s32);
s32 BattleEffect_ApplyToTargets(s32, s32, s32, s32);

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
    struct ItemUseWork *work;
    s32 result;
    s32 item_id;
    struct ItemOwner *owner;
    struct ItemData *item;

    owner = Owner_GetStateFar(owner_id);
    work = gMenuWork;
    item_id = 0x1ff & owner->items[slot];
    item = Item_Get(item_id);
    result = BattleEffect_ApplyToTargets(
        0x3fff & item->use_ability,
        owner_id,
        target_id,
        1);
    if (result != -1) {
        item = Item_Get(owner->items[slot]);
        if (item->kind == 1) {
            Inventory_RemoveFar(owner_id, slot);
            work->entry_count =
                ItemMenu_Collect(owner, work->entries, 0);
        }
        if (item->kind == 4) {
            if (item_id == 0xb8)
                item_id = 0xb9;
            owner->items[slot] = item_id;
        }
        result = 0;
    }
    return result;
}
