#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "TBS_EDITION.H"

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
 * gLinkCountdownWork so that reopening the menu resumes where it left off.
 * The same cell's +0x4c flag ends the loop from outside.
 *
 * Remaining difference (384 instructions, nearly all register choice): the
 * listing keeps `page` on the stack and gives r8 and r9 to short-lived values
 * of the entry-drawing loop (the action record, i * 2), because its loop pass
 * moves nothing but &glyph out of that loop and reduces no induction value
 * there, where this draft hoists render + RENDER_TEXT_COLOR_OFS and reduces
 * three. agscc's loop dump (-dL) counts 132 insns in this draft's drawing
 * loop and 25 in the first sprite loop; the listing's loops must count more
 * (or hold a label between the two text-colour stores), as inlined helpers
 * would give. The first sprite loop likewise leaves 0x1ff inside the loop.
 * What already agrees: gWindowWork as the base both work pointers come from,
 * the sprite words written through a union (so window->x is reread each
 * pass), index-form tile stores, and the frame layout.
 * Message 2279, shown for an empty list, has no name in the catalogs yet.
 */

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

/* A queued sprite: the list link, then its OAM attributes as fields. */
struct MenuSprite {
    struct MenuSprite *next;
    union {
        struct {
            u32 attr01;
            u32 attr23;
        } word;
        struct {
            u16 y : 8;
            u16 affine : 2;
            u16 mode : 2;
            u16 mosaic : 1;
            u16 colors : 1;
            u16 shape : 2;
            u16 x : 9;
            u16 affine_index : 5;
            u16 size : 2;
            u16 tile : 10;
            u16 priority : 2;
            u16 palette : 4;
            u16 unused;
        } f;
    } oam;
};

extern volatile s32 gKeyState;
extern volatile s32 gKeysRepeat;
extern volatile u32 gFrameCount;
extern u8 Resource_FixedBlockBTiles[];
extern u8 gWindowWork[];
extern struct BattleMenuNav *gLinkCountdownWork;

extern u8 MsgAbilityName;
extern u8 MsgAbilityDescription;

void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void Runtime_PushSlotEntry(struct MenuSprite *entry, s32 slot);
void Resource_ResetEntry(s32 handle);
s32 Resource_LoadIntoFreeSlot(s32 kind);
s32 Resource_GetBuffer(s32 handle, s32 source);
s32 __divsi3(s32 dividend, s32 divisor);
struct UiWindowWork *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 style);
void UiWork_Finalize(struct UiWindowWork *window, s32 release);
void RenderOutput_RedrawSavedRect(struct UiWindowWork *window);
s32 UiText_CopyMessageString(s32 message, s16 *buffer, s32 count);
void UiText_RenderWideStringAtOffset(s16 *buffer, struct UiWindowWork *window, s32 x, s32 y);
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
void Ui_SetRectHighlight(s32 x, s32 y, s32 w, s32 h, s32 value);
void Vram_CopyTile(s32 source, s32 destination);
void Ui_FillVramBlockPattern(void);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
struct BattleAction *Ability_GetData(s32 action);
void AudioCommand_PlayFar(s32 cue);

s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count)
{
    u8 **globals;
    u8 *render;
    s32 drawn_page;
    s32 page;
    s32 drawn_row;
    s32 cursor_handle;
    struct BattleUnit *unit;
    struct UiWindowWork *message_window;
    s32 icon_count;
    s32 preferred_row;
    struct MenuSprite *sprite;
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
    struct MenuSprite cursor;
    struct MenuSprite *spr;
    s16 buffer[64];
    s32 handles[PAGE_ROWS];
    struct MenuSprite sprites[PAGE_ROWS];
    s32 glyph;

    globals = (u8 **)gWindowWork;
    render = globals[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    unit = Owner_GetStateFar(unit_id);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = PAGE_ROWS;
    nav = (struct BattleMenuNav *)globals[42];
    page = nav->page;
    row = nav->row;
    preferred_row = nav->preferred_row;
    window = UiWindow_Create(9, 9, 21, 11, 6);

    for (i = 0; i <= 4; i++) {
        sprite = &sprites[i];
        sprite->oam.word.attr01 = 0x40000000;
        sprite->oam.word.attr23 = 0;
        sprite->oam.f.x = window->x * 8 + 8;
        sprite->oam.f.y = (i * 2 + window->y) * 8 + 4;
    }

    handle = handles;
    for (i = 0; i < PAGE_ROWS; i++) {
        s32 slot;

        slot = Resource_LoadIntoFreeSlot(128);
        *handle++ = slot;
        sprites[i].oam.f.tile = Resource_GetBuffer(slot, -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render[RENDER_MENU_BUSY_OFS] = 1;
            Ui_SetRectHighlight(
                window->x + 1,
                window->y + drawn_row * 2 + 1,
                window->width - 2,
                1,
                15);
            Ui_FillVramBlockPattern();
            if (count != 0) {
                UiText_CopyMessageString(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)&MsgAbilityDescription,
                    buffer,
                    52);
            } else {
                UiText_CopyMessageString(2279, buffer, 52);
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
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
                    sprites[i].oam.f.tile = glyph;

                    if ((action->target_flags & 0x80) == 0) {
                        UiWork_SetParamNibble(4);
                    } else if (action->pp_cost > unit->pp) {
                        UiWork_SetParamNibble(2);
                    } else if (unit->psy_seal != 0) {
                        UiWork_SetParamNibble(9);
                    }

                    render[RENDER_TEXT_COLOR_OFS] = 5;
                    UiText_DrawCharacterAtOffset(
                        entry + (s32)&MsgAbilityName, window, 16, i * 16);
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
                while (i < (pages = __divsi3(count + 4, PAGE_ROWS))) {
                    tile = i + 0xf301;
                    if (i == __divsi3(page, PAGE_ROWS)) {
                        tile = i + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(
                        window, tile, window->width - pages + i - 2, -1, 0);
                    i++;
                }
            }

            Ui_SetRectHighlight(
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
            while (i < (pages = __divsi3(count + 4, PAGE_ROWS))) {
                tile = i + 0xf301;
                if ((gFrameCount & 15) <= 11) {
                    if (i == __divsi3(page, PAGE_ROWS)) {
                        tile = i + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - __divsi3(count + 4, PAGE_ROWS) + i - 2,
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
        spr = &cursor;
        spr->oam.word.attr01 = 0x40000000;
        spr->oam.word.attr23 = 0;
        spr->oam.f.tile =
            Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
        spr->oam.f.x = cursor_x + ((gFrameCount & 4) >> 1) - 4;
        spr->oam.f.y = cursor_y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(spr, 242);
        }

        nav = gLinkCountdownWork;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred_row;

        if ((gKeyState & 1) != 0) {
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
        } else if (nav->active == 0 || (gKeyState & 2) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0) {
            if ((gKeysRepeat & 128) != 0) {
                AudioCommand_PlayFar(111);
                row++;
                if (row == PAGE_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((gKeysRepeat & 64) != 0) {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0) {
                    if (page == __divsi3(count - 1, PAGE_ROWS) * PAGE_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = 4;
                    }
                }
                preferred_row = row;
            } else if ((gKeysRepeat & 16) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + PAGE_ROWS >= count) {
                    if (page != 0) {
                        row = preferred_row;
                        page = 0;
                    }
                } else {
                    page += PAGE_ROWS;
                    row = preferred_row;
                    if (page ==
                        __divsi3(count - 1, PAGE_ROWS) * PAGE_ROWS) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            } else if ((gKeysRepeat & 32) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0) {
                    row = preferred_row;
                    page -= PAGE_ROWS;
                } else {
                    page = __divsi3(count - 1, PAGE_ROWS) * PAGE_ROWS;
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
