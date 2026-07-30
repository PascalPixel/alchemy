#include "types.h"

typedef struct {
    u32 allocation;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 link;
} MenuSprite_0802592c;

typedef struct {
    u8 pad00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
} MenuWindow_0802592c;

typedef struct {
    u8 pad00[0x30];
    s32 cursor;
    s32 page;
    s32 saved_cursor;
    u8 pad3c[0x10];
    s32 enabled;
} MenuState_0802592c;

typedef struct {
    u8 pad00[0x38];
    s16 limit;
} ActorState_0802592c;

typedef struct {
    u8 id;
    u8 flags;
    u8 category;
    u8 pad03[5];
    u8 icon;
    u8 cost;
} ActionInfo_0802592c;

#define GLOBAL_PTR(address, type) (*(type **)(address))
#define GLOBAL_WORD(address)      (*(volatile u32 *)(address))

void *Func_080040b4(s32);
s32 Func_080040d0(void *, s32);
void Func_08003dec(void *, s32);
void Func_08003f3c(void *);
void Func_080030f8(s32);
void Func_0800352c(void);
s32 Func_080022ec(s32, s32);
MenuWindow_0802592c *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(void *, s32);
void Func_08016498(void *);
void Func_08016738(void);
void Func_08017aa4(void *, void *, s32, s32);
void Func_08019000(MenuWindow_0802592c *, s32, s32, s32, s32);
void Func_0801965c(s32, void *, s32);
void Func_0801a3d0(s32, s32, s32, void *, s32);
void Func_0801e71c(s32);
void Func_0801e7c0(s32, MenuWindow_0802592c *, s32, s32);
void Func_0801e9d4(s32, s32, MenuWindow_0802592c *, s32, s32);
void Func_080218dc(MenuWindow_0802592c *, s32, s32, s32, s32);
void Func_08022768(s32, s32, s32, s32, s32);
void Func_080251d4(s32, s32);
ActorState_0802592c *Func_08077008(s32);
ActionInfo_0802592c *Func_08077080(s32);
void Func_080f9010(s32);

/*
 * Display and operate the five-entry action-selection menu.
 *
 * The routine owns its windows and six temporary sprites for the duration of
 * the modal loop.  arg1 is the action-id list and arg2 is its entry count.
 */
s32 Func_0802592c(s32 actor, u16 *actions, s32 action_count)
{
    MenuSprite_0802592c row_sprites[5];
    void *row_tiles[5];
    u8 description[52];
    MenuSprite_0802592c cursor_sprite;
    MenuState_0802592c *menu_state;
    ActorState_0802592c *actor_state;
    MenuWindow_0802592c *description_window;
    MenuWindow_0802592c *list_window;
    void *cursor_tiles;
    s32 page;
    s32 cursor;
    s32 saved_cursor;
    s32 previous_page = -1;
    s32 previous_cursor = -1;
    s32 visible_rows = 5;
    s32 cursor_y;
    s32 exit_value;
    s32 index;

    menu_state = GLOBAL_PTR(0x03001f34, MenuState_0802592c);
    page = menu_state->page;
    cursor = menu_state->cursor;
    saved_cursor = menu_state->saved_cursor;

    cursor_tiles = Func_080040b4(0x80);
    actor_state = Func_08077008(actor);
    description_window = Func_080162d4(0, 5, 30, 4, 42);
    list_window = Func_080162d4(9, 9, 21, 11, 6);

    for (index = 0; index < 5; index++) {
        row_sprites[index].attr0 = 0;
        row_sprites[index].attr1 = 0x4000;
        row_sprites[index].attr2 = 0;
        row_sprites[index].link = 0;
        row_sprites[index].attr0 = (u16)(((list_window->x * 8) + 8) & 0x1ff);
        row_sprites[index].attr1 =
            (u16)((row_sprites[index].attr1 & 0xfe00) |
                  ((((index * 2) + list_window->y) * 8 + 4) & 0x3ff));
    }

    for (index = 0; index < 5; index++) {
        s32 tile;
        row_tiles[index] = Func_080040b4(0x80);
        tile = Func_080040d0(row_tiles[index], -1);
        row_sprites[index].attr2 =
            (u16)((row_sprites[index].attr2 & 0xfc00) |
                  (tile & 0x3ff));
    }

    Func_080251d4(0xf018, 0x200);
    Func_080251d4(0xf018, 0x201);
    Func_080251d4(0xf019, 0x210);
    Func_080251d4(0xf019, 0x211);

    for (;;) {
        if (page != previous_page || cursor != previous_cursor) {
            GLOBAL_PTR(0x03001e8c, u8)[0xea6] = 1;
            Func_08022768(list_window->x + 1,
                          list_window->y + previous_cursor * 2 + 1,
                          list_window->width - 2, 1, 0xf);
            Func_08016738();

            if (action_count != 0) {
                Func_0801965c(
                    (actions[page + cursor] & 0x3fff) + 0x53a,
                    description, sizeof(description));
            } else {
                Func_0801965c(0xea6, description, sizeof(description));
            }
            Func_08017aa4(description, description_window, 0, 4);
            previous_cursor = cursor;

            if (page != previous_page) {
                Func_08016498(list_window);
                visible_rows = 0;
                while (visible_rows <= 4 &&
                       actions[page + visible_rows] != 0) {
                    u16 action = actions[page + visible_rows];
                    ActionInfo_0802592c *info = Func_08077080(action);
                    s32 row = visible_rows * 2;
                    s32 sprite_data;
                    s32 generated_tile;
                    u8 *global = GLOBAL_PTR(0x03001e8c, u8);

                    Func_08019000(list_window, 0xf01f, 11, row, 0);
                    Func_08019000(list_window, 0xf01e, 12, row, 0);
                    Func_0801a3d0(action & 0x3fff, 0,
                                  (s32)&row_tiles[visible_rows],
                                  &generated_tile, 1);
                    sprite_data = (s32)generated_tile;
                    row_sprites[visible_rows].attr2 =
                        (u16)((row_sprites[visible_rows].attr2 & 0xfc00) |
                              (sprite_data & 0x3ff));

                    if ((info->flags & 0x80) == 0)
                        Func_0801e71c(4);
                    else if (info->cost > actor_state->limit)
                        Func_0801e71c(2);
                    else if (GLOBAL_PTR(0x03001e8c, u8)[0xea7] != 0)
                        Func_0801e71c(9);

                    global[0xea6] = 5;
                    Func_0801e7c0(action + 0x333, list_window,
                                  16, visible_rows * 16);
                    Func_0801e9d4(info->cost, 2, list_window, 104,
                                  visible_rows * 16);
                    Func_0801e71c(15);
                    global[0xea6] = 15;

                    if (info->category != 4)
                        Func_08019000(list_window,
                                      info->category + 0x5001, 15, row, 0);
                    Func_080218dc(list_window, 16, row,
                                  info->icon == 0xff ? 11 : info->icon - 1, 0);
                    visible_rows++;
                }
                previous_page = page;
            }

            if (action_count > 5) {
                s32 page_count = Func_080022ec(action_count + 4, 5);
                for (index = 0; index < page_count; index++) {
                    s32 marker = index + 0xf301;
                    if (index == Func_080022ec(page, 5))
                        marker = index + 0xf30b;
                    Func_08019000(list_window, marker,
                                  list_window->width - page_count + index - 2,
                                  -1, 0);
                }
            }

            Func_08022768(list_window->x + 1,
                          list_window->y + cursor * 2 + 1,
                          list_window->width - 2, 1, 14);
            GLOBAL_PTR(0x03001e8c, u8)[0xea3] = 1;
            GLOBAL_PTR(0x03001e8c, u8)[0xea6] = 0;
        }

        if (action_count > 5) {
            s32 page_count;
            for (index = 0;
                 index < Func_080022ec(action_count + 4, 5);
                 index++) {
                s32 marker = index + 0xf301;
                if ((GLOBAL_WORD(0x03001e40) & 15) <= 11 &&
                    index == Func_080022ec(page, 5))
                    marker = index + 0xf30b;
                page_count = Func_080022ec(action_count + 4, 5);
                Func_08019000(list_window, marker,
                              list_window->width - page_count + index - 2,
                              -1, 0);
            }
            Func_08019000(list_window, 0xf334,
                          list_window->width - page_count - 3, -1, 0);
            Func_08019000(list_window, 0xf335,
                          list_window->width - 2, -1, 0);
            GLOBAL_PTR(0x03001e8c, u8)[0xea3] |=
                2 << ((list_window->y - 1) >> 2);
        }

        for (index = 0; index < visible_rows; index++)
            Func_08003dec(&row_sprites[index], 0xf0);

        cursor_y = (list_window->y + cursor * 2) * 8 + 20;
        cursor_sprite.attr0 = 0;
        cursor_sprite.attr1 = 0x4000;
        cursor_sprite.attr2 = 0;
        cursor_sprite.link = 0;
        cursor_sprite.attr2 =
            (u16)((cursor_sprite.attr2 & 0xfc00) |
                  (Func_080040d0(cursor_tiles, 0xf301) & 0x3ff));
        cursor_sprite.attr1 =
            (u16)((cursor_sprite.attr1 & 0xfe00) |
                  (((list_window->x * 8 - 4) +
                    ((GLOBAL_WORD(0x03001e40) & 4) >> 1) + 0x10a4) &
                   0x1ff));
        cursor_sprite.attr0 =
            (u16)((cursor_sprite.attr0 & 0xfffc) |
                  ((cursor_y - ((GLOBAL_WORD(0x03001e40) & 4) >> 2) +
                    0xf8) &
                   0xff));
        if (action_count != 0)
            Func_08003dec(&cursor_sprite, 0xf2);

        menu_state = GLOBAL_PTR(0x03001f34, MenuState_0802592c);
        menu_state->page = page;
        menu_state->cursor = cursor;
        menu_state->saved_cursor = saved_cursor;

        if (GLOBAL_WORD(0x03001c94) & 1) {
            if (action_count == 0) {
                exit_value = -1;
                break;
            }
            exit_value = page + cursor;
            if ((Func_08077080(actions[exit_value])->flags & 0x80) != 0)
                break;
        } else if (menu_state->enabled == 0 ||
                   (GLOBAL_WORD(0x03001c94) & 2)) {
            Func_080f9010(113);
            exit_value = -1;
            break;
        }

        if (action_count != 0) {
            u32 keys = GLOBAL_WORD(0x03001b04);
            if (keys & 0x80) {
                Func_080f9010(111);
                cursor++;
                if (cursor == 5 || page + cursor == action_count)
                    cursor = 0;
                saved_cursor = cursor;
            } else if (keys & 0x40) {
                Func_080f9010(111);
                cursor--;
                if (cursor < 0) {
                    s32 last_page = Func_080022ec(action_count - 1, 5) * 5;
                    cursor = page == last_page
                                 ? action_count - page - 1
                                 : 4;
                }
                saved_cursor = cursor;
            } else if (keys & 0x10) {
                Func_080f9010(111);
                Func_0800352c();
                if (page + 5 >= action_count) {
                    if (page != 0) {
                        page = 0;
                        cursor = saved_cursor;
                    }
                } else {
                    s32 last_page;
                    page += 5;
                    cursor = saved_cursor;
                    last_page = Func_080022ec(action_count - 1, 5) * 5;
                    if (page == last_page && cursor > action_count - page - 1)
                        cursor = action_count - page - 1;
                }
            } else if (keys & 0x20) {
                Func_080f9010(111);
                Func_0800352c();
                if (page != 0) {
                    page -= 5;
                    cursor = saved_cursor;
                } else {
                    page = Func_080022ec(action_count - 1, 5) * 5;
                    cursor = saved_cursor;
                    if (page != 0 && cursor > action_count - page - 1)
                        cursor = action_count - page - 1;
                }
            }
        }
        Func_080030f8(1);
    }

    Func_08016418(description_window, 1);
    Func_08016418(list_window, 1);
    Func_080030f8(1);
    for (index = 0; index < 5; index++)
        Func_08003f3c(row_tiles[index]);
    Func_08003f3c(cursor_tiles);
    Func_080030f8(1);
    return exit_value;
}
