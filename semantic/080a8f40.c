#include "types.h"

struct Page_080a8f40 {
    u8 padding00[8];
    s32 page_index;
    u8 padding0c[8];
    s32 item_count;
};

struct MenuState_080a8f40 {
    u8 padding000[0x1C8];
    u16 item_ids[40];
    u8 show_items;
    u8 padding219;
    u8 owner_id;
};

struct Owner_080a8f40 {
    u8 padding00[0x0F];
    u8 field0f;
    u8 padding010[0x119];
    u8 field129;
};

struct Item_080a8f40 {
    u8 padding00[8];
    u8 category;
    u8 value;
};

typedef char Page_080a8f40_size[
    sizeof(struct Page_080a8f40) == 0x18 ? 1 : -1
];
typedef char Owner_080a8f40_size[
    sizeof(struct Owner_080a8f40) == 0x12C ? 1 : -1
];
typedef char Item_080a8f40_size[
    sizeof(struct Item_080a8f40) == 0x0C ? 1 : -1
];

void Func_08015080(s32, void *, s32, s32);
void Func_08015090(void *, void *, s32, s32);
void Func_08015098(const u8 *, void *, s32, s32);
void Func_080150a8(s32, s32, void *, s32, s32);
void Func_080150b0(s32, s32, void *, s32, s32);
void Func_08015270(void *);
struct Owner_080a8f40 *Func_08077008(s32);
struct Item_080a8f40 *Func_08077080(s32);
void Func_080a21b0(void *, s32, s32, s32, s32);
void Func_080a2324(s32, s32, void *, s32, s32);
void Func_080a8cc0(void *, s32, s32, s32, s32);

s32 Func_080a8f40(
    void *window,
    s32 unused_r1,
    const struct Page_080a8f40 *page)
{
    struct MenuState_080a8f40 *state;
    struct Owner_080a8f40 *owner;
    struct Item_080a8f40 *item;
    const u16 *entry;
    s32 offset;
    s32 item_id;
    s32 icon;
    s32 y;
    u8 visible;
    u8 index;

    (void)unused_r1;
    state = *(struct MenuState_080a8f40 **)0x03001F2C;
    owner = Func_08077008(state->owner_id);
    Func_08015270(window);

    offset = page->page_index * 5;
    visible = page->item_count - offset;
    if (visible > 5)
        visible = 5;

    Func_080a2324(5, offset, window, 0x50, 0x3A);
    Func_080a21b0(
        window,
        page->item_count,
        5,
        page->page_index,
        0x1C);
    Func_08015080(0xAED, window, 0xB0, 0);

    index = 0;
    if (visible != 0) {
        entry = &state->item_ids[offset];
        do {
            item_id = *entry & 0x3FFF;
            item = Func_08077080(item_id);
            y = index * 0x10 + 0x10;

            Func_08015080(item_id + 0x333, window, 0x58, y);
            Func_080150a8(item->value, 2, window, 0xB0, y);

            if (item->category == 0xFF)
                icon = 0xB;
            else
                icon = item->category - 1;
            Func_080a8cc0(window, 0x19, index * 2 + 2, icon, 0);

            index++;
            entry++;
        } while (index < visible);
    }

    if (state->show_items == 0)
        Func_08015080(0xAEF, window, 0x60, 0x11);

    Func_08015090(owner, window, 0x28, 0);
    Func_08015080(owner->field129 + 0x741, window, 0, 0x20);
    Func_08015098((const u8 *)0x080AF22C, window, 0, 0x30);
    Func_080150b0(owner->field0f, 2, window, 0x18, 0x30);
    return 1;
}
