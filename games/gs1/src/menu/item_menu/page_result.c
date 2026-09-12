#include "scene.h"
#include "item_menu.h"
#include "global_cells.h"
#include "types.h"
#include "gs1_edition.h"

/* menu/item_menu/page_result.c */
/* item_menu/page_result.c */
s32 Runtime_GetObject(s32);
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

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
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

    limit = ItemMenu_Count(LoadByte(entries, offset));
    encoded = Runtime_GetObject(LoadByte(entries, offset));
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    quotient = FixedPoint_Ratio(value, 5);
    remainder = Modulo(value, 5);
    groups = FixedPoint_Ratio(limit, 5);
    if (Modulo(limit, 5) != 0) {
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

/* item_menu/draw/detail.c */
extern void *gIw;
extern u8 Value_00000075;

s32 ItemMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 combined;
    s32 off;
    s32 row;

    page = *(s32 *)(state + 8);
    menu = gIw;
    combined = page * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    UiWindow_Commit(*(s32 *)(menu + 44));
    WaitFrames(1);

    combined = *(s32 *)(state + 24);
    off = combined * 2 + 456;
    if (*(u16 *)((char *)menu + off) != 0) {
        s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&Value_00000075;
        UiText_DrawAt(masked, *(s32 *)(menu + 44), 0, 0);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Sys_SetRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 14);
        } else {
            Sys_SetRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}

/* item_menu/draw/name_page.c */
#if defined(GS1_EDITION_JA)
#define PAGE_X  120
#define ENTRY_X 32
#else
#define PAGE_X  116
#define ENTRY_X 24
#endif

extern u8 Value_00000182;

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void UiWindow_Commit(s32 window);

s32 ItemMenu_DrawNamePage(
    s32 window,
    s32 unused,
    const struct MenuResult *state)
{
    struct ItemMenuState *menu =
        *(struct ItemMenuState **)ADDR_03001F2C;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    (void)unused;

    UiWindow_Commit(window);
    Sys_SetRange(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        item_id = &menu->items[first_entry];
        do {
            UiText_DrawAt(
                (item_id[0] & 0x1ff) + (s32)&Value_00000182,
                menu->item_window,
                ENTRY_X,
                row * 16 + 8
            );
            row++;
            item_id++;
        } while (visible_count > row);
    }

    return 1;
}

s32 ItemMenu_NamePageOk(void)
{
    return 1;
}

/* menu/selection/open_confirm_prompt.c */
#define FIELD(ptr, type, offset) (*(type *)((u8 *)(ptr) + (offset)))

struct MenuObjectControl {
    u8 padding00[4];
    u16 suspended;
};

extern struct MenuObjectControl *gIwOpenConfirmPrompt;

s32 Runtime_AllocateHeapBlock(s32 kind, s32 size);

void WaitFrames(s32 frames);

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_CancelSoundReset(void);

void Menu_EnsureCancelSound(void);
s32 Ability_GetData(s32 flags);

/*
 * Open a modal menu screen and run its blocking interaction body.
 *
 * The address of gIwOpenConfirmPrompt, not the pointer it holds, is the base for the
 * two fixed-address fields at +0x24 and +0x54; only "suspended" is reached
 * through the pointer itself.  The field read at +0x178 is named by position
 * only and is not otherwise confirmed.
 */
s32 Menu_OpenConfirmPrompt(void)
{
    void *state = (void *)Runtime_AllocateHeapBlock(0x37, 0xa70);
    s32 high;
    s32 unused;
    s32 low;
    s32 result;

    gIwOpenConfirmPrompt->suspended = 1;
    Menu_SetMode(0, 0, 30, 20);
    WaitFrames(1);
    Menu_Do(0);
    FIELD(state, u8, 0x219) = (u8)Menu_Check((const u16 *)((u8 *)state + 0x208));
    Menu_SetMode2(0, 3, 0, 7);
    FIELD(state, s32, 0x10c) = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Menu_unk2_2(14);
    Menu_unk3_2(0x06002500);
    Menu_CancelSoundReset();
    result = Menu_Place(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        void *target = FIELD(&gIwOpenConfirmPrompt, void *, 0x54);
        u16 flags;
        Ability_GetData(0x3fff & FIELD(state, u16, 0x178));
        flags = (u16)(low | (high << 10));
        FIELD(target, u16, 0x17e) = flags;
    }
    Menu_unk4_2(FIELD(state, s32, 0x24));
    FIELD(FIELD(&gIwOpenConfirmPrompt, void *, 0x24), u8, 0xea6) = 1;
    Menu_Run();
    Menu_SetMode(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(0x37);
    gIwOpenConfirmPrompt->suspended = 0;
    WaitFrames(1);
    Menu_SetMode3(0, 0, 30, 20);
    FIELD(FIELD(&gIwOpenConfirmPrompt, void *, 0x24), u8, 0xea6) = 0;
    Menu_unk2_4();
    return result;
}
