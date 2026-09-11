#include "a9_motion.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "object_factory.h"

/* menu/entry/place_entry_objects_in_grid.c */
/* menu/entry/place_entry_objects_in_grid.c */
/* menu/entry/place_entry_objects_in_grid.c */
void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct Object080a9bd8 *obj;
    struct Object080a9bd8 **tbl;

    i = 0;
    tbl =
        (struct Object080a9bd8 **)(*(s32 *)ADDR_03001F2C + 0x48);
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_Run(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}

/* menu/entry/place_entry_object_in_grid.c */
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

void Menu_PlaceEntryObjectInGrid(struct Object080a9bd8 *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (s16)((FixedPoint_Ratio(no, phase) * 0x10) + origin_y);
    obj->x =
        (s16)((Modulo(no, phase) * 0x10) + origin_x);
    Menu_Do(obj);
}

/* menu/item_menu/pos_category.c */
struct Object_080a9cbc {
    u8 padding[6];
    s16 value1;
    s16 value2;
};

struct State_080a9cbc {
    u8 padding[72];
    struct Object_080a9cbc *objects[32];
};

LAYOUT_OFFSET_GUARD(
    Object_080a9cbc_value1_offset, struct Object_080a9cbc, value1, 6);
LAYOUT_OFFSET_GUARD(
    State_080a9cbc_objects_offset, struct State_080a9cbc, objects, 72);
LAYOUT_SIZE_GUARD(State_080a9cbc_size, struct State_080a9cbc, 200);

extern struct State_080a9cbc *gIw;

void ItemMenu_PosCategory(void)
{
    struct State_080a9cbc *state = gIw;
    s32 value1 = 248;
    struct Object_080a9cbc **entry = state->objects;
    s32 value2 = 168;
    s32 remaining = 31;

    do {
        struct Object_080a9cbc *object = *entry++;

        if (object != 0) {
            object->value1 = value1;
            object->value2 = value2;
            Sys_Do(object);
        }
        remaining--;
    } while (remaining >= 0);
}

/* menu/entry/create_eight_entry_objects.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Menu_CreateEightEntryObjects(s32 resource)
{
    void **slot;
    void *obj;
    s32 i;
    u8 *state;
    s32 param;

    state = *(u8 **)ADDR_03001F2C;
    i = 0;
    param = 0xA8;
    slot = (void **)(state + 0xC8);
    do {
        obj = Menu_RunCreateEightEntryObjects(2, i, resource, 0xF8, param);
        i += 1;
        *slot = obj;
        slot += 1;
    } while (i <= 7);
    return 1;
}

/* menu/item_menu/reset_category.c */
struct Object_080a9d84 {
    u8 padding[6];
    s16 value1;
    s16 value2;
    u8 padding2[5];
    u8 flag;
};

struct State_080a9d84 {
    u8 padding[200];
    struct Object_080a9d84 *objects[32];
};

LAYOUT_OFFSET_GUARD(
    Object_080a9d84_flag_offset, struct Object_080a9d84, flag, 15);
LAYOUT_OFFSET_GUARD(
    State_080a9d84_objects_offset, struct State_080a9d84, objects, 200);
LAYOUT_SIZE_GUARD(State_080a9d84_size, struct State_080a9d84, 328);

extern struct State_080a9d84 *gIw;

void ItemMenu_ResetCategory(void)
{
    struct State_080a9d84 *state = gIw;
    s32 index;

    for (index = 0; index < 5; index++) {
        struct Object_080a9d84 *object = state->objects[index];

        if (object != 0) {
            object->value1 = 248;
            object->value2 = 168;
            object->flag = 240;
            Sys_Do(object);
        }
    }
}

/* menu/character_menu/update_selection_icons.c */
struct Entry_080a9dc4 {
    u8 padding[14];
    u8 value;
};

struct State_080a9dc4 {
    u8 padding[200];
    struct Entry_080a9dc4 *entries[5];
};

extern struct State_080a9dc4 *gIw;

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled)
{
    struct State_080a9dc4 *state = gIw;
    s32 index = 0;

    do {
        if (enabled[index] != 0) {
            s32 kind;
            switch (index) {
            case 0: kind = 16; break;
            case 1: kind = 1; break;
            case 2: kind = 2; break;
            case 3: kind = 15; break;
            case 4: kind = 7; break;
            default: kind = 0; break;
            }
            Sys_SetMode(8, kind, state->entries[index]->value, 0);
        }
        index++;
    } while (index <= 4);
    return 1;
}

/* item/prepare_use_palette.c */
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
