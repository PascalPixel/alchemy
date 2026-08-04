#include "layout_guard.h"

struct Page_080a9598 {
    u8 padding00[8];
    s32 page_index;
    u8 padding0c[8];
    s32 item_count;
};

struct MenuState_080a9598 {
    u8 padding000[0x1C8];
    u16 item_ids[40];
    u8 show_items;
    u8 padding219;
    u8 owner_id;
};

struct Owner_080a9598 {
    u8 padding00[0x3C];
    u16 field3c;
    u16 field3e;
};

LAYOUT_OFFSET_GUARD(
    Page080a9598_PageIndex,
    struct Page_080a9598,
    page_index,
    0x08);
LAYOUT_OFFSET_GUARD(
    Page080a9598_ItemCount,
    struct Page_080a9598,
    item_count,
    0x14);
LAYOUT_OFFSET_GUARD(
    MenuState080a9598_ItemIds,
    struct MenuState_080a9598,
    item_ids,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    MenuState080a9598_ShowItems,
    struct MenuState_080a9598,
    show_items,
    0x218);
LAYOUT_OFFSET_GUARD(
    MenuState080a9598_OwnerId,
    struct MenuState_080a9598,
    owner_id,
    0x21a);
LAYOUT_OFFSET_GUARD(
    Owner080a9598_Field3c,
    struct Owner_080a9598,
    field3c,
    0x3c);

void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015090(void *, s32, s32, s32);
void Func_080150b0(u16, s32, s32, s32, s32);
struct Owner_080a9598 *Func_08077008(s32);
void Func_080a21b0(s32, s32, s32, s32, s32);
void Func_080a2324(s32, s32, s32, s32, s32);

s32 Func_080a9598(
    s32 window,
    s32 unused_r1,
    const struct Page_080a9598 *page)
{
    struct MenuState_080a9598 *state;
    struct Owner_080a9598 *owner;
    const u16 *item;
    s32 offset;
    u8 visible;
    u8 index;

    (void)unused_r1;
    state = *(struct MenuState_080a9598 **)0x03001F2C;
    owner = Func_08077008(state->owner_id);

    Func_08015068(window, 0x80, 8, 0xE0, 0x60);

    offset = page->page_index * 5;
    visible = page->item_count - offset;
    if (visible > 5)
        visible = 5;

    Func_080a2324(5, offset, window, 0x77, 0x34);
    Func_080a21b0(
        window,
        page->item_count,
        5,
        page->page_index,
        0x1C);

    if (state->show_items == 0) {
        Func_08015080(0xAD7, window, 0x78, 8);
    } else {
        index = 0;
        if (visible != 0) {
            item = &state->item_ids[page->page_index * 5];
            do {
                Func_08015080(
                    (*item & 0x1FF) + 0x182,
                    window,
                    0x80,
                    index * 0x10 + 8);
                index++;
                item++;
            } while (index < visible);
        }
    }

    Func_08015090(owner, window, 0x28, 0);
    Func_08015080(0xAF7, window, 0x20, 0x10);
    Func_08015080(0xAF8, window, 0x20, 0x18);
    Func_080150b0(owner->field3c, 3, window, 0x50, 0x10);
    Func_080150b0(owner->field3e, 3, window, 0x50, 0x18);
    return 1;
}
