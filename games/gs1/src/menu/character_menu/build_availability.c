#include "types.h"
#include "scene.h"
#include "menu_result.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "battle_types.h"
#include "psynergy_menu.h"
#include "owner_state.h"
#include "item_menu.h"
#include "a9_motion.h"
#include "layout_guard.h"
#include "object_factory.h"

/* menu/character_menu/build_availability.c */
/* character_menu/build_availability.c */
/* character_menu/build_availability.c */
u8 *Runtime_GetObject(s32 owner);

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    u8 *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = Runtime_GetObject(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (*(s16 *)(state + 56) == 0 && requested == 1) {
        output[0] = requested;
        count = 1;
    }

    mode = *(s8 *)(state + 0x131);
    if (mode != 0) {
        if (mode == 1)
            output[1] = mode;
        else
            output[2] = 1;
        count++;
    }
    if (*(s8 *)(state + 304) != 0) {
        output[3] = 1;
        count++;
    }
    if (state[320] != 0) {
        output[4] = 1;
        count++;
    }
    return count;
}

/* menu/core/build_page_result.c */
/* menu/core/build_page_result.c */
#if defined(GS1_EDITION_JA)
#define GROUP_LEN 6
#else
#define GROUP_LEN 5
#endif

extern u8 gIw[];
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)ADDR_03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = Runtime_GetObject(LoadByte(entries, offset));
    limit = LoadByte(base, 0x218);
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = FixedPoint_Ratio(value, GROUP_LEN);
    remainder = Modulo(value, GROUP_LEN);
    groups = FixedPoint_Ratio(limit, GROUP_LEN);
    if (Modulo(limit, GROUP_LEN) != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}

/* menu/core/build_pattern_tiles.c */
struct TileMask {
    u32 word0;
    u32 word1;
};

typedef void (*FillProc)(void *dst, s32 size, u32 word);

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask MaskTbl[];

#define FillWords ((FillProc)0x03000168)

void Menu_BuildPatternTiles(void)
{
    u32 *vram = (u32 *)0x06005000;
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], MaskTbl[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], MaskTbl[mi].word1);
            }
        }
    }
}

/* psynergy_menu/psynergy_menu_draw_range.c */
/* psynergy_menu/draw_range.c */
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

void PsynergyMenu_DrawRange(
    s32 window, s32 x, s32 y, s32 range, s32 unused)
{
    s32 n;
    s32 tile;

    (void)unused;

    n = range * 2;
    tile = n + 0xf281;
    /* Keep the empty zero path; GCC emits the reference branch shape. */
    if (!n) {
    }
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + 0xf280, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

/* psynergy_menu/draw_list_page.c */
extern u8 Value_00000aed;
extern u8 Value_00000aef;
extern u8 Value_00000333;
extern u8 Value_00000741;
extern u8 StrLv;

#define MsgPP Value_00000aed
#define MsgNoPsy Value_00000aef
#define MsgAction Value_00000333
#define MsgClass Value_00000741

#define PSY_LIST_OFS 0x1c8
#define ACT_ID_MASK 0x3fff
#define OWNER_LEVEL_OFS 15
#define OWNER_CLASS_MSG_OFS 0x129

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(
    s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
void UiText_DrawStringAtOffsetFar(u8 *, void *, s32, s32);
void UiText_DrawStringInWindowFar(u8 *, s32, s32, s32);
void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void PsynergyMenu_DrawRange(s32, s32, s32, s32, s32);
struct BattleAction *Ability_GetData(s32 action);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = PsyMenuWork;
    u8 *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = Runtime_GetObject(menu->owner_ids[0]);

    UiWindow_Commit(window);

    first = res->page * 5;
    rows = (u8)(res->entry_count - first);
    if (rows > 5) {
        rows = 5;
    }

    Menu_SetPageIcons(5, first, window, 80, 58);
    Menu_DrawPageIndicator(window, res->entry_count, 5, res->page, 28);

    UiText_DrawAt((s32)&MsgPP, window, 176, 0);

    row = 0;
    if (rows > row) {
        ofs = (s32)(first * 2) + PSY_LIST_OFS;
        do {
            struct BattleAction *act;
            s32 msg;
            s32 y;
            s32 range;
            act = Ability_GetData(
                ACT_ID_MASK & *(u16 *)(ofs + (s32)menu));
            msg = (*(u16 *)(ofs + (s32)menu) & ACT_ID_MASK) +
                (s32)&MsgAction;
            y = row * 16 + 16;

            UiText_DrawAt(msg, window, 88, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, 176, y);

            range = act->range;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + 2, range, 0);

            row++;
            ofs += 2;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawAt((s32)&MsgNoPsy, window, 96, 17);
    }

    UiText_DrawStringAtOffsetFar(owner, (void *)window, 40, 0);
    UiText_DrawAt(
        owner[OWNER_CLASS_MSG_OFS] + (s32)&MsgClass, window, 0, 32);
    UiText_DrawStringInWindowFar(&StrLv, window, 0, 48);
    UiNumber_DrawAt(owner[OWNER_LEVEL_OFS], 2, window, 24, 48);

    return 1;
}

/* psynergy_menu/draw_prepared_psynergy_icons.c */
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gIw;

    OwnerState_GetFar(owner_id);
    Sys_Run();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

/* shop/draw/item_page.c */
/* shop/draw/item_page.c */
/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 gVal[];
extern u8 gVal2[];

s32 Shop_DrawItemPage(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec5;
    s32 record;
    s32 r10;
    s32 v10;
    s32 v5;
    s32 base6_0;
    s32 v6;
    s32 base5_af7;

    p9 = *(s32 *)gIw;
    rec5 = Runtime_GetObject(*(u8 *)((0x21a + p9)));
    Menu_SetRange(a0, 128, 8, 224, 96);
    p8 = ((*(s32 *)(a2 + 8) << 2) + *(s32 *)(a2 + 8));
    p10 = ((u32)(s32)((s32)(*(s32 *)(a2 + 20) - (s32)p8) << 24) >> 24);
    v10 = p10;
    if ((u32)p10 > 5) {
        v10 = 5;
    }
    Menu_SetPageIcons(5, p8, a0, 119, 52);
    Menu_DrawPageIndicator(a0, *(s32 *)(a2 + 20), 5, *(s32 *)(a2 + 8), 28);
    if (*(u8 *)((0x218 + p9)) == 0) {
        Sys_Run(0xad7, a0, 120, 8);
        v6 = r10;
    } else {
        v6 = 0;
        base6_0 = 0;
        /* The entry guard compares the count variable, not a literal zero:
           folded to a literal it becomes `!= 0` and a `beq`, where the
           original keeps the unsigned `bls`. */
        if ((u32)v10 > base6_0) {
            v5 = ((s32)(((s32)p8 << 1) + p9) + 0x1c8);
            do {
                UiText_DrawAt(((0x1ff & *(u16 *)(v5)) + (s32)gVal), a0, 128, ((base6_0 << 4) + 8));
                base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
                v5 = (v5 + 2);
            } while ((u32)v10 > base6_0);
            v6 = base6_0;
        }
    }
    UiText_DrawStringAtOffsetFar(rec5, a0, 40, 0);
    base5_af7 = (s32)gVal2;
    UiText_DrawAt(base5_af7, a0, 32, 16);
    UiText_DrawAt((base5_af7 + 1), a0, 32, 24);
    UiNumber_DrawAt(*(u16 *)(rec5 + 60), 3, a0, 80, 16);
    UiNumber_DrawAt(*(u16 *)(rec5 + 62), 3, a0, 80, 24);
    p9b = base5_af7 + 1;
    p10b = v6;
    p11 = a0;
    return 1;
}

/* menu/move_grid_cursor.c */
/* menu/input/move_grid_cursor.c */
s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2)
{
    s32 row;
    s32 col;

    col = *arg0;
    row = *arg1;
    switch (arg2) {
    case 0x40:
        row -= 1;
        if (row < 0) {
            row = 5;
        }
        if (row <= 3) {
            if (row == 3) {
                if (col <= 4) {
                    col = 0;
                } else {
                    goto set_one;
                }
            } else if (col > 1) {
set_one:
                col = 1;
            }
            if ((row == 3) && (col == 1)) {
                row = 2;
            }
        }
        break;
    case 0x80:
        row += 1;
        if (row > 5) {
            row = 0;
        }
        if ((row == 3) && (col == 1)) {
            row = 4;
        }
        if (row == 4) {
            goto set_zero;
        }
        break;
    case 0x20:
        col -= 1;
        if (row == 3) {
            col += 1;
        } else if (row > 3) {
            if (col < 0) {
                col = 7;
            }
        } else if (col < 0) {
            col = 1;
        }
        break;
    case 0x10:
        col += 1;
        if (row == 3) {
            col -= 1;
        } else if (row > 3) {
            if (col > 7) {
                goto set_zero;
            }
        } else if (col > 1) {
            goto set_zero;
        }
        break;
set_zero:
        col = 0;
        break;
    }
    *arg0 = col;
    *arg1 = row;
    return (row * 9) + col;
}

/* menu/status/reserved_status_one.c */
s32 Menu_ReservedStatusOne(void)
{
    return 1;
}

/* menu/item_menu/draw/category.c */
extern u8 Value_00000b24;

extern void UiText_DrawAt(void *, s32, s32, s32);

void WaitFrames(s32);

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct ItemMenuState *menu = gIw;
    u8 *items;

    OwnerState_GetFar(owner_id);
    ItemMenu_PosCategory();
    ItemMenu_HideAllIcons();
    UiText_DrawAt(&Value_00000b24, window, 0, 0);
    UiText_DrawAt(&Value_00000b24 + 1, window, 0, 32);
    UiText_DrawAt(&Value_00000b24 + 2, window, 0, 16);
    UiText_DrawAt(&Value_00000b24 + 3, window, 0, 48);
    items = (u8 *)menu->items;
    Sys_Apply(window, items);
    if (mode == 0) {
        WaitFrames(1);
        ItemMenu_DrawIcons((u16 *)items, 1);
        Sys_CheckCategory(items);
    }
}

/* menu/entry/place_entry_objects_in_grid.c */
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
    UiIcon_PrepareObject(obj);
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
            UiIcon_PrepareObject(object);
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
            UiIcon_PrepareObject(object);
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
    FunctionHead_080a22f4();
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
