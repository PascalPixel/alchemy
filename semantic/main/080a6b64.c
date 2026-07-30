typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

struct Page_080a6b64 {
    u8 padding00[8];
    s32 page_index;
    u8 padding0c[8];
    s32 item_count;
};

struct MenuState_080a6b64 {
    u8 padding000[0x1C8];
    u16 item_ids[40];
    u8 field218;
    u8 padding219;
    u8 owner_id;
    u8 padding21b[5];
    u16 flags;
};

struct Owner_080a6b64 {
    u8 padding00[0x3A];
    s16 threshold;
};

struct Item_080a6b64 {
    u8 padding00[9];
    u8 value;
};

typedef char Page_080a6b64_size[
    sizeof(struct Page_080a6b64) == 0x18 ? 1 : -1
];
typedef char Owner_080a6b64_size[
    sizeof(struct Owner_080a6b64) == 0x3C ? 1 : -1
];
typedef char Item_080a6b64_size[
    sizeof(struct Item_080a6b64) == 0x0C ? 1 : -1
];

void Func_08015070(void *, s32, s32, s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_080150a8(s32, s32, void *, s32, s32);
void Func_080150b8(s32);
void Func_08015270(void *);
struct Owner_080a6b64 *Func_08077008(s32);
struct Item_080a6b64 *Func_08077080(s32);
void Func_080a21b0(void *, s32, s32, s32, s32);
void Func_080a2324(s32, s32, void *, s32, s32);
s32 Func_080a735c(s32);

s32 Func_080a6b64(
    void *window,
    s32 unused_r1,
    const struct Page_080a6b64 *page)
{
    struct MenuState_080a6b64 *state;
    struct Owner_080a6b64 *owner;
    struct Item_080a6b64 *item;
    const u16 *entry;
    s32 offset;
    s32 item_id;
    s32 y;
    u8 visible;
    u8 index;

    (void)unused_r1;
    state = *(struct MenuState_080a6b64 **)0x03001F2C;

    Func_08015270(window);
    Func_08015070(window, 0, 0xB, 0x10, 0xB);
    if (state->flags & 2)
        Func_08015080(0xAE1, window, 0, 0x58);
    else
        Func_08015080(0xB89, window, 0, 0x58);

    offset = page->page_index * 5;
    visible = page->item_count - offset;
    if (visible > 5)
        visible = 5;

    Func_080a2324(5, offset, window, 0x70, 0x22);
    Func_080a21b0(
        window,
        page->item_count,
        5,
        page->page_index,
        0xF);
    Func_08015080(0xAED, window, 0x60, 0);

    index = 0;
    if (visible != 0) {
        entry = &state->item_ids[offset];
        do {
            owner = Func_08077008(state->owner_id);
            item_id = *entry & 0x3FFF;
            item = Func_08077080(item_id);

            if ((s32)item->value > owner->threshold)
                Func_080150b8(2);
            else if (Func_080a735c(item_id) != 0)
                Func_080150b8(4);
            else
                Func_080150b8(0xF);

            y = index * 0x10 + 8;
            Func_08015080(item_id + 0x333, window, 0x10, y);
            Func_080150a8(item->value, 2, window, 0x68, y);
            Func_080150b8(0xF);

            index++;
            entry++;
        } while (index < visible);
    }

    return 1;
}
