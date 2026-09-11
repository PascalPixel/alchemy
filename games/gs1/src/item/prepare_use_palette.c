#include "types.h"
#include "scene.h"

/* item/prepare_use_palette.c */
void Item_PrepareUsePalette(void)
{
    Item_Run();
    Item_Do(13);
}

void Item_UseNoOpCallback(void)
{
}

/* item/use.c */
struct ItemOwner {
    u8 padding_000[0xd8];
    u16 items[15];
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
    u8 padding_208[0x10];
    u8 entry_count;
};

extern struct ItemUseWork *gIw;

struct ItemOwner *Runtime_GetObject(s32);
struct ItemData *Item_GetData(s32);

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
    struct ItemUseWork *work;
    s32 result;
    s32 item_id;
    struct ItemOwner *owner;
    struct ItemData *item;

    owner = Runtime_GetObject(owner_id);
    work = gIw;
    item_id = 0x1ff & owner->items[slot];
    item = Item_GetData(item_id);
    result = Item_SetMode(
        0x3fff & item->use_ability,
        owner_id,
        target_id,
        1);
    if (result != -1) {
        item = Item_GetData(owner->items[slot]);
        if (item->kind == 1) {
            Item_Apply(owner_id, slot);
            work->entry_count =
                Item_Place(owner, work->entries, 0);
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

s32 Item_Run(void)
{
    return 1;
}
