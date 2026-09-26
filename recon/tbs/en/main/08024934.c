/* Not-yet-C: complete 2124-byte summon picker, including literal pools.
 * Standby Djinn counts arrive in argument three. Available summons are
 * ordered with affordable entries first, four per page. Returns an id or
 * -1. Candidate 2192 / reference 2124 bytes, 689 aligned halfword edits.
 * The 368-byte frame is four bytes short; drawn_page spills instead of
 * living in r9, and the first sprite loop hoists its 0x1ff mask into a
 * short-range halfword pool. Scoping that counter changed its register
 * but not the frame; explicit full-width OAM masks moved pools without
 * recovering the first-loop register roles (2174 bytes / 776 edits).
 * All call arities and byte/halfword sprite accesses follow the listing. */
#include "TYPES.H"
#include "BATTLE_SUMMON.H"

struct SummonWindow {
    u8 unknown_00[8];
    u16 width, height, x, y;
};

struct SummonNavigation {
    u8 unknown_00[0x30];
    s32 row, page, preferred_row;
    u8 unknown_3c[0x10];
    s32 active;
    u8 unknown_50[0x88];
    s32 automatic, delay;
};

struct SummonRender {
    u8 unknown_000[0xea3];
    u8 dirty;
    u8 unknown_ea4[2];
    u8 busy;
};

struct SummonGlobals {
    struct SummonRender *render;
    u8 unknown_04[0xa4];
    struct SummonNavigation *navigation;
};

union SummonSprite {
    struct { void *next; u32 attr01, attr23; } word;
    struct {
        void *next;
        u8 y, flags;
        u16 x : 9;
        u16 flags06 : 5;
        u16 size : 2;
        u16 tile : 10;
        u16 flags08 : 2;
        u16 palette : 4;
        u16 affine;
    } attr;
};

extern struct SummonGlobals Data_03001e8c;
extern struct SummonNavigation *Data_03001f34;
extern volatile u32 Data_03001c94, Data_03001b04, Data_03001e40;
extern u8 Data_080310a4[];
extern u8 Value_0000053a, Value_00000333, Value_00005001;
extern u8 Value_0000f301, Value_0000f30b, Value_0000f334, Value_0000f335;
extern u8 Value_0000fffc;

void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void Runtime_PushSlotEntry(union SummonSprite *sprite, s32 priority);
void Resource_ResetEntry(s32 handle);
s32 Resource_LoadIntoFreeSlot(s32 kind);
s32 Resource_GetBuffer(s32 handle, const void *source);
struct SummonWindow *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWork_Finalize(struct SummonWindow *, s32);
void RenderOutput_RedrawSavedRect(struct SummonWindow *);
void Ui_FillVramBlockPattern(void);
s32 UiText_CopyMessageString(s32 message, s16 *text, s32 size);
void UiText_RenderWideStringAtOffset(s16 *, struct SummonWindow *, s32, s32);
void UiWindow_SetTilemapEntry(struct SummonWindow *, s32, s32, s32, s32);
void Func_08018efc(struct SummonWindow *, s32, s32, s32, s32);
void Ability_LoadGlyph(s32, s32, s32 *, s32 *, s32);
void UiWork_SetParamNibble(s32 colour);
void UiText_DrawCharacterAtOffset(s32, struct SummonWindow *, s32, s32);
void Ui_SetRectHighlight(s32, s32, s32, s32, s32);
s32 Func_080771d8(u8 *ids);
void Audio_PlayCue(s32 cue);

s32 Func_08024934(s32 unused0, s32 unused1, const u8 *standby)
{
    union SummonSprite cursor;
    union SummonSprite sprites[4];
    u8 available[33];
    u8 ordered[33];
    s16 text[64];
    s32 handles[4];
    u8 visible[4];
    s32 glyph;
    struct SummonRender *render;
    struct SummonWindow *description, *counts, *window;
    struct SummonNavigation *nav;
    const struct SummonDefinition *summon;
    union SummonSprite *sprite;
    const u8 *required, *supply;
    u8 *src, *dst;
    s32 *handle;
    s32 drawn_row, drawn_page, row, page, preferred;
    s32 cursor_handle, count, used, mask, index, element, col;
    s32 id, tile, affordable, cursor_x, cursor_y;
    s32 pressed, repeated, result;

    render = Data_03001e8c.render;
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = (s16)Resource_LoadIntoFreeSlot(128);
    description = UiWindow_Create(0, 4, 30, 4, 42);
    counts = UiWindow_Create(20, 8, 10, 3, 6);
    mask = 0;
    page = Data_03001e8c.navigation->page;
    row = Data_03001e8c.navigation->row;
    preferred = Data_03001e8c.navigation->preferred_row;
    window = UiWindow_Create(13, 11, 17, 9, 6);
    sprite = sprites;
    index = 0;
    do {
        sprite->word.attr01 = 0x40000000;
        sprite->word.attr23 = 0;
        sprite->attr.x = window->x * 8 + 8;
        sprite->attr.y = (index * 2 + window->y) * 8 + 4;
        index++;
        sprite++;
    } while (index <= 3);
    handle = handles;
    sprite = sprites;
    index = 3;
    do {
        s32 slot = Resource_LoadIntoFreeSlot(128);
        *handle++ = slot;
        sprite->attr.tile = Resource_GetBuffer(slot, (void *)-1);
        sprite++;
    } while (--index >= 0);

    count = Func_080771d8(available);
    used = 0;
    for (src = available + count - 1; src >= available; src--) {
        id = *src;
        required = Func_080771e0(id)->djinn_required;
        supply = standby;
        element = 0;
        while (*required <= *supply) {
            element++;
            if (element > 3)
                break;
            required++;
            supply++;
        }
        if (element == 4) {
            ordered[used++] = id;
            *src = 32;
        }
    }
    dst = ordered + used;
    src = available;
    for (index = count; index > 0; index--) {
        id = *src++;
        if (id != 32) {
            *dst++ = id;
            used++;
        }
    }
    ordered[used] = 32;

    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render->busy = 1;
            Ui_SetRectHighlight(window->x + 1, window->y + drawn_row * 2 + 1,
                window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            summon = Func_080771e0(ordered[page + row]);
            UiText_CopyMessageString(summon->name_message_id + (s32)&Value_0000053a, text, 52);
            UiText_RenderWideStringAtOffset(text, description, 0, 4);
            mask = 0;
            drawn_row = row;
            required = summon->djinn_required;
            for (element = 0; element <= 3; element++) {
                if (*required++ != 0)
                    mask |= 1 << element;
            }
            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                col = 1;
                for (element = 0; element <= 3; element++) {
                    UiWindow_SetTilemapEntry(counts, element + (s32)&Value_00005001, element * 2, 0, 0);
                    Func_08018efc(counts, standby[element] + 48, col, 0, 0);
                    col += 2;
                }
                index = 0;
                while (index <= 3 && (id = ordered[page + index]) != 32) {
                    summon = Func_080771e0(id);
                    required = summon->djinn_required;
                    supply = standby;
                    element = 0;
                    while (*required <= *supply) {
                        element++;
                        if (element > 3)
                            break;
                        required++;
                        supply++;
                    }
                    affordable = element == 4;
                    Ability_LoadGlyph(summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
                    sprites[index].attr.tile = glyph;
                    if (!affordable)
                        UiWork_SetParamNibble(2);
                    UiText_DrawCharacterAtOffset(Func_080771e0(id)->name_message_id + (s32)&Value_00000333,
                        window, 16, index * 16);
                    required = summon->djinn_required;
                    col = 13;
                    for (element = 0; element <= 3; element++) {
                        if (*required != 0) {
                            UiWindow_SetTilemapEntry(window, element + (s32)&Value_00005001, col, index * 2, 0);
                            Func_08018efc(window, *required + 48, col + 1, index * 2, 0);
                            col += 2;
                        }
                        required++;
                    }
                    UiWork_SetParamNibble(15);
                    visible[index++] = 1;
                }
                while (index <= 3)
                    visible[index++] = 0;
                drawn_page = page;
            }
            if (count > 4) {
                for (index = 0; index < (count + 3) / 4; index++) {
                    tile = index + (s32)&Value_0000f301;
                    if (index == page / 4)
                        tile = index + (s32)&Value_0000f30b;
                    UiWindow_SetTilemapEntry(window, tile,
                        window->width - (count + 3) / 4 + index - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->busy = 0;
        }
        sprite = sprites;
        src = visible;
        for (index = 0; index <= 3; index++) {
            if (*src++ != 0)
                Runtime_PushSlotEntry(sprite, 240);
            sprite++;
        }
        cursor_x = window->x * 8 - 2;
        cursor_y = (row * 2 + window->y) * 8 + 20;
        cursor.word.attr01 = 0x40000000;
        cursor.word.attr23 = 0;
        cursor.attr.tile = Resource_GetBuffer((u16)cursor_handle, Data_080310a4);
        cursor.attr.x = cursor_x + ((Data_03001e40 & 4) >> 1) + (s32)&Value_0000fffc;
        cursor.attr.y = cursor_y - ((Data_03001e40 & 4) >> 2) + 248;
        Runtime_PushSlotEntry(&cursor, 242);
        affordable = Data_03001e40 & 8;
        for (element = 0; element <= 3; element++) {
            tile = 15 - (affordable != 0);
            if ((mask & (1 << element)) == 0)
                tile = 15;
            Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
        }
        if (count > 4) {
            for (index = 0; index < (count + 3) / 4; index++) {
                tile = index + (s32)&Value_0000f301;
                if ((Data_03001e40 & 15) <= 11 && index == page / 4)
                    tile = index + (s32)&Value_0000f30b;
                UiWindow_SetTilemapEntry(window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
            }
            UiWindow_SetTilemapEntry(window, (s32)&Value_0000f334, window->width - (count + 3) / 4 - 3, -1, 0);
            UiWindow_SetTilemapEntry(window, (s32)&Value_0000f335, window->width - 2, -1, 0);
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }
        nav = Data_03001f34;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred;
        pressed = Data_03001c94;
        repeated = Data_03001b04;
        if (nav->automatic != 0) {
            repeated = 0;
            pressed = 0;
            if (nav->delay == 0) {
                nav->delay = 120;
                repeated = 1;
                pressed = 1;
            } else {
                nav->delay--;
            }
        }
        if (pressed & 1) {
            result = ordered[page + row];
            break;
        }
        if (Data_03001f34->active == 0 || (pressed & 2)) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        if (repeated & 128) {
            Audio_PlayCue(111);
            row++;
            if (row == 4 || page + row == count)
                row = 0;
            preferred = row;
        } else if (repeated & 64) {
            Audio_PlayCue(111);
            row--;
            if (row < 0) {
                if (page == ((count - 1) / 4) * 4)
                    row = count - page - 1;
                else
                    row = 3;
            }
            preferred = row;
        } else if (repeated & 16) {
            Audio_PlayCue(111);
            Runtime_SetMainState19();
            if (page + 4 >= count) {
                if (page != 0) {
                    row = preferred;
                    page = 0;
                }
            } else {
                page += 4;
                row = preferred;
                if (page == ((count - 1) / 4) * 4) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        } else if (repeated & 32) {
            Audio_PlayCue(111);
            Runtime_SetMainState19();
            if (page != 0) {
                row = preferred;
                page -= 4;
            } else {
                page = ((count - 1) / 4) * 4;
                row = preferred;
                if (page != 0) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        }
        WaitFrames(1);
    }
    WaitFrames(1);
    handle = handles;
    index = 3;
    do {
        Resource_ResetEntry(*handle++);
    } while (--index >= 0);
    Resource_ResetEntry((u16)cursor_handle);
    UiWork_Finalize(counts, 1);
    UiWork_Finalize(description, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    return result;
}
