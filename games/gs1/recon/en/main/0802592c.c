#include "types.h"
#include "battle_types.h"
#include "gs1_edition.h"

/*
 * Battle action (Psynergy) selection loop.
 *
 * Draws one page of at most five entries from `actions` into a 21x11 window,
 * with a description line in a second full-width window, a page indicator
 * when the list is longer than one page, five entry glyph sprites and a
 * blinking cursor sprite.  It then blocks, one frame at a time, handling the
 * four direction keys plus confirm and cancel, and returns the selected index
 * into `actions` or -1 when the caller cancelled.
 *
 * The paging state (page top, row within the page, and the row the player
 * prefers when moving horizontally) lives in the shared menu-navigation cell
 * at 0x03001f34 so that reopening the menu resumes where it left off.  The
 * same cell's +0x4c flag ends the loop from outside; UiText_ShowLocalizedMessage
 * AndWait (games/gs1/src/ui/text/msg/show_localized_message_and_wait.c) polls the
 * identical pair of "0x03001c94 & 3" and "+0x4c" conditions.
 *
 * Uncertain, and left as read from the reference:
 *  - The message-name text is fetched with the raw table word, while the glyph
 *    and the availability test mask it with 0x3fff first.  Both spellings are
 *    reproduced literally rather than made consistent.
 *  - Render-work byte 0xea7 has no established macro; it is written next to
 *    UiWork_SetParamNibble and evidently selects the same text colour.
 *  - The 0x0000f018/0x0000f019 arguments to Func_080251d4 are tilemap-shaped
 *    words whose low ten bits that helper uses as a tile index.
 */

#define BattleMenu_RunActionSelection Func_0802592c

#define ACTION_ID_MASK 0x3fff
#define PAGE_ROWS 5

/* Render-work byte adjacent to the menu-busy flag; unestablished elsewhere. */
#define RENDER_TEXT_COLOR_OFS (RENDER_MENU_BUSY_OFS + 1)

struct UiWindowWork {
    u8 unknown_00[8];
    u16 width;                  /* 0x08 */
    u8 unknown_0a[2];
    u16 x;                      /* 0x0c */
    u16 y;                      /* 0x0e */
};

/* Shared menu-navigation cell; only the fields this owner touches are named. */
struct BattleMenuNav {
    u8 unknown_00[0x30];
    s32 row;                    /* 0x30 */
    s32 page;                   /* 0x34 */
    s32 preferred_row;          /* 0x38 */
    u8 unknown_3c[0x10];
    s32 active;                 /* 0x4c */
};

/*
 * Slot-list sprite record: a link word followed by the four object attribute
 * halfwords.  The two views share one storage so the whole attribute pair can
 * be cleared with a word store before the packed fields are written, which is
 * the shape Runtime_PushSlotEntry consumes.
 */
union MenuSprite {
    struct {
        void *next;
        u32 attr01;
        u32 attr23;
    } word;
    struct {
        void *next;
        u8 y;                   /* 0x04 */
        u8 field_05;
        u16 x : 9;              /* 0x06 */
        u16 field_06_9 : 5;
        u16 size : 2;
        u16 tile : 10;          /* 0x08 */
        u16 field_08_a : 2;
        u16 palette : 4;
        u16 field_0a;
    } attr;
};

extern volatile s32 Data_03001c94;
extern volatile s32 Data_03001b04;
extern volatile u32 Data_03001e40;
extern u8 Data_080310a4[];

extern u8 Value_00000333;
extern u8 Value_0000053a;
extern u8 Value_000008e7;

#define ActionNameMessage Value_00000333
#define ActionTextMessage Value_0000053a
#define NoActionMessage Value_000008e7

#define Math_Div Func_080022ec

#define RenderWork (*(u8 **)0x03001e8c)
#define MenuNav (*(struct BattleMenuNav **)0x03001f34)

void WaitFrames(s32 frames);
void Func_0800352c(void);
void Runtime_PushSlotEntry(union MenuSprite *entry, s32 slot);
void Resource_ResetEntry(s32 handle);
s32 Resource_LoadIntoFreeSlot(s32 kind);
s32 Resource_GetBuffer(s32 handle, s32 source);
s32 Math_Div(s32 dividend, s32 divisor);
struct UiWindowWork *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 style);
void UiWork_Finalize(struct UiWindowWork *window, s32 release);
void Func_08016498(struct UiWindowWork *window);
s32 Func_0801965c(s32 message, s16 *buffer, s32 count);
void Func_08017aa4(s16 *buffer, struct UiWindowWork *window, s32 x, s32 y);
void UiWindow_SetTilemapEntry(
    struct UiWindowWork *window, s32 tile, s32 x, s32 y, s32 layer);
void Ability_LoadGlyph(s32 action, s32 base, s32 *source, s32 *tile, s32 reuse);
void UiWork_SetParamNibble(s32 param);
void UiText_DrawCharacterAtOffset(
    s32 message, struct UiWindowWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffset(
    s32 value, s32 digits, struct UiWindowWork *window, s32 x, s32 y);
void UiWindow_DrawThreeTileColumn(
    struct UiWindowWork *window, s32 x, s32 y, s32 range, s32 layer);
void Func_08022768(s32 x, s32 y, s32 w, s32 h, s32 value);
void Func_080251d4(s32 source, s32 destination);
void Ui_FillVramBlockPattern(void);
struct BattleUnit *Runtime_GetObject(s32 unit_id);
struct BattleAction *Ability_GetData(s32 action);
void Audio_PlayCue(s32 cue);

s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count)
{
    u8 *render;
    s32 drawn_page;
    s32 page;
    s32 drawn_row;
    s32 cursor_handle;
    struct BattleUnit *unit;
    struct UiWindowWork *message_window;
    s32 icon_count;
    s32 preferred_row;
    union MenuSprite *sprite;
    s32 *handle;
    struct BattleMenuNav *nav;
    struct BattleAction *action;
    struct UiWindowWork *window;
    s32 row;
    s32 cursor_x;
    s32 cursor_y;
    s32 entry;
    s32 pages;
    s32 tile;
    s32 range;
    s32 result;
    s32 i;
    union MenuSprite cursor;
    s16 buffer[64];
    s32 handles[PAGE_ROWS];
    union MenuSprite sprites[PAGE_ROWS];
    s32 glyph;

    render = RenderWork;
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    unit = Runtime_GetObject(unit_id);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = PAGE_ROWS;
    nav = MenuNav;
    page = nav->page;
    row = nav->row;
    preferred_row = nav->preferred_row;
    window = UiWindow_Create(9, 9, 21, 11, 6);

    for (i = 0; i <= 4; i++) {
        sprites[i].word.attr01 = 0x40000000;
        sprites[i].word.attr23 = 0;
        sprites[i].attr.x = window->x * 8 + 8;
        sprites[i].attr.y = (i * 2 + window->y) * 8 + 4;
    }

    handle = handles;
    sprite = sprites;
    i = 4;
    do {
        s32 slot;

        slot = Resource_LoadIntoFreeSlot(128);
        *handle++ = slot;
        sprite->attr.tile = Resource_GetBuffer(slot, -1);
        sprite++;
    } while (--i >= 0);

    Func_080251d4(0xf018, 0x200);
    Func_080251d4(0xf018, 0x201);
    Func_080251d4(0xf019, 0x210);
    Func_080251d4(0xf019, 0x211);

    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render[RENDER_MENU_BUSY_OFS] = 1;
            Func_08022768(
                window->x + 1,
                window->y + drawn_row * 2 + 1,
                window->width - 2,
                1,
                15);
            Ui_FillVramBlockPattern();
            if (count != 0) {
                Func_0801965c(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)&ActionTextMessage,
                    buffer,
                    52);
            } else {
                Func_0801965c((s32)&NoActionMessage, buffer, 52);
            }
            Func_08017aa4(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                Func_08016498(window);
                for (i = 0; i < PAGE_ROWS; i++) {
                    entry = actions[page + i];
                    if (entry == 0) {
                        break;
                    }
                    action = Ability_GetData(entry);
                    UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
                    UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
                    Ability_LoadGlyph(
                        entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
                    sprites[i].attr.tile = glyph;

                    if ((action->target_flags & 0x80) == 0) {
                        UiWork_SetParamNibble(4);
                    } else if (action->pp_cost > unit->pp) {
                        UiWork_SetParamNibble(2);
                    } else if (unit->psy_seal != 0) {
                        UiWork_SetParamNibble(9);
                    }

                    render[RENDER_TEXT_COLOR_OFS] = 5;
                    UiText_DrawCharacterAtOffset(
                        entry + (s32)&ActionNameMessage, window, 16, i * 16);
                    UiText_DrawNumberAtOffset(
                        action->pp_cost, 2, window, 104, i * 16);
                    UiWork_SetParamNibble(15);
                    render[RENDER_TEXT_COLOR_OFS] = 15;

                    if (action->damage_class != 4) {
                        UiWindow_SetTilemapEntry(
                            window, action->damage_class + 0x5001, 15, i * 2, 0);
                    }

                    range = action->range;
                    if (range == 255) {
                        range = 11;
                    } else {
                        range--;
                    }
                    UiWindow_DrawThreeTileColumn(window, 16, i * 2, range, 0);
                }
                icon_count = i;
                drawn_page = page;
            }

            if (count > PAGE_ROWS) {
                i = 0;
                while (i < (pages = Math_Div(count + 4, PAGE_ROWS))) {
                    tile = i + 0xf301;
                    if (i == Math_Div(page, PAGE_ROWS)) {
                        tile = i + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(
                        window, tile, window->width - pages + i - 2, -1, 0);
                    i++;
                }
            }

            Func_08022768(
                window->x + 1,
                window->y + row * 2 + 1,
                window->width - 2,
                1,
                14);
            render[RENDER_DIRTY_OFS] = 1;
            render[RENDER_MENU_BUSY_OFS] = 0;
        }

        if (count > PAGE_ROWS) {
            i = 0;
            while (i < (pages = Math_Div(count + 4, PAGE_ROWS))) {
                tile = i + 0xf301;
                if ((Data_03001e40 & 15) <= 11) {
                    if (i == Math_Div(page, PAGE_ROWS)) {
                        tile = i + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - Math_Div(count + 4, PAGE_ROWS) + i - 2,
                    -1,
                    0);
                i++;
            }
            UiWindow_SetTilemapEntry(
                window, 0xf334, window->width - pages - 3, -1, 0);
            UiWindow_SetTilemapEntry(
                window, 0xf335, window->width - 2, -1, 0);
            render[RENDER_DIRTY_OFS] |=
                2 << ((u32)(window->y - 1) >> 2);
        }

        for (i = 0; i < icon_count; i++) {
            Runtime_PushSlotEntry(&sprites[i], 240);
        }

        cursor_x = window->x * 8 - 4;
        cursor_y = (row * 2 + window->y) * 8 + 20;
        cursor.word.attr01 = 0x40000000;
        cursor.word.attr23 = 0;
        cursor.attr.tile =
            Resource_GetBuffer(cursor_handle, (s32)Data_080310a4);
        cursor.attr.x = cursor_x + ((Data_03001e40 & 4) >> 1) - 4;
        cursor.attr.y = cursor_y - ((Data_03001e40 & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(&cursor, 242);
        }

        nav = MenuNav;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred_row;

        if ((Data_03001c94 & 1) != 0) {
            if (count != 0) {
                result = page + row;
                action = Ability_GetData(actions[result]);
                if ((action->target_flags & 0x80) != 0) {
                    break;
                }
            } else {
                result = -1;
                break;
            }
        } else if (nav->active == 0 || (Data_03001c94 & 2) != 0) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }

        if (count != 0) {
            if ((Data_03001b04 & 128) != 0) {
                Audio_PlayCue(111);
                row++;
                if (row == PAGE_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((Data_03001b04 & 64) != 0) {
                Audio_PlayCue(111);
                row--;
                if (row < 0) {
                    if (page == Math_Div(count - 1, PAGE_ROWS) * PAGE_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = 4;
                    }
                }
                preferred_row = row;
            } else if ((Data_03001b04 & 16) != 0) {
                Audio_PlayCue(111);
                Func_0800352c();
                if (page + PAGE_ROWS >= count) {
                    if (page != 0) {
                        row = preferred_row;
                        page = 0;
                    }
                } else {
                    page += PAGE_ROWS;
                    row = preferred_row;
                    if (page ==
                        Math_Div(count - 1, PAGE_ROWS) * PAGE_ROWS) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            } else if ((Data_03001b04 & 32) != 0) {
                Audio_PlayCue(111);
                Func_0800352c();
                if (page != 0) {
                    row = preferred_row;
                    page -= PAGE_ROWS;
                } else {
                    page = Math_Div(count - 1, PAGE_ROWS) * PAGE_ROWS;
                    row = preferred_row;
                    if (page != 0) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            }
        }
        WaitFrames(1);
    }

    UiWork_Finalize(message_window, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    handle = handles;
    i = 4;
    do {
        Resource_ResetEntry(*handle++);
    } while (--i >= 0);
    Resource_ResetEntry(cursor_handle);
    WaitFrames(1);
    return result;
}
