#include "types.h"

struct Window_08020bd8 {
    u8 unknown_00[0x0c];
    u16 tile_x;
    u16 tile_y;
};

struct Sprite_08020bd8 {
    u8 unknown_00[5];
    s8 field_05;
    u8 field_06;
    u8 unknown_07;
    u8 field_08;
    u8 unknown_09[6];
    s8 field_0f;
    u8 unknown_10[4];
    s8 field_14;
    u8 unknown_15;
    u16 field_16;
    u8 unknown_18;
    u8 flags_19;
};

void *Func_08077008(s32);
void Func_0800479c(void);
struct Window_08020bd8 *Func_080162d4(s32, s32, s32, s32, s32);
s32 Func_08019d2c(s32);
void Func_08019da8(s32, s32, s32, s32);
void Func_080209d0(struct Window_08020bd8 *, const void *);
void Func_0801e41c(struct Window_08020bd8 *, s32, s32, s32, s32);
void Func_08020b64(struct Window_08020bd8 *, const u8 *);
s32 Func_08004080(void);
void Func_08003fa4(s32, s32, const void *);
struct Sprite_08020bd8 *Func_0801eadc(
    s32, u32, struct Window_08020bd8 *, s32, s32);
void Func_080b0038(struct Sprite_08020bd8 **, s32, s32);
s32 Func_08020b14(const u8 *);
void Func_08020a60(struct Window_08020bd8 *, s32, s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_080b0030(struct Sprite_08020bd8 **, s32, s32, s32);
void Func_080b0020(struct Sprite_08020bd8 **);
void Func_080b0028(struct Sprite_08020bd8 **);
void Func_080f9010(s32);
void Func_08016478(struct Window_08020bd8 *);
void Func_08016418(struct Window_08020bd8 *, s32);
void Func_08019e48(s32);

static u32 *const gFrameCounter_08020bd8 = (u32 *)0x03001800;
static u32 *const gInputPressed_08020bd8 = (u32 *)0x03001b04;
static u32 *const gInputRepeat_08020bd8 = (u32 *)0x03001c94;
static u8 **const gTilemap_08020bd8 = (u8 **)0x03001e8c;

s32 Func_08020bd8(s32 combatant)
{
    struct Window_08020bd8 *grid_window;
    struct Window_08020bd8 *list_window;
    struct Sprite_08020bd8 *cursor_sprite = 0;
    struct Sprite_08020bd8 *list_sprite = 0;
    u8 selected[15];
    u8 *saved = Func_08077008(combatant);
    u8 *tilemap = *gTilemap_08020bd8;
    u8 *write;
    s32 selected_count = 0;
    s32 x = 18;
    s32 y = 5;
    s32 move_cursor = 1;
    s32 move_list = 1;
    s32 result = 0;
    s32 i;
    volatile s32 refresh_path = 0;

    Func_0800479c();
    grid_window = Func_080162d4(3, 6, 24, 9, 2);
    list_window = Func_080162d4(8, 3, 8, 3, 2);
    Func_08019da8(Func_08019d2c(combatant), 0, 3, 1);
    Func_080209d0(grid_window, (const void *)0x08073864);
    Func_0801e41c(grid_window, 18, 0, 18, 7);
    tilemap[0xea3] = 1;

    for (i = 0; i < 15; i++) {
        selected[i] = saved[i];
        if (saved[i] != 0)
            selected_count++;
    }
    selected[14] = 0;
    write = selected + selected_count;
    Func_08020b64(list_window, saved);

    i = Func_08004080();
    if (i <= 0x5f) {
        Func_08003fa4(i, 0x80, (const void *)0x080310a4);
        cursor_sprite = Func_0801eadc(i, 0x40000000, grid_window, 0, 0);
        Func_080b0038(&cursor_sprite,
                      grid_window->tile_x * 8 + 0x8c,
                      grid_window->tile_y * 8 + 0x34);
    }

    i = Func_08004080();
    if (i <= 0x5f) {
        Func_08003fa4(i, 0x80, (const void *)0x080317e4);
        list_sprite = Func_0801eadc(i, 0x40000000, grid_window, 0, 0);
        list_sprite->field_0f = -1;
        list_sprite->flags_19 &= (u8)~0x0c;
        Func_080b0038(&list_sprite, Func_08020b14(selected) + 0x46, 0x16);
    }

    *(volatile u32 *)0x040000d4 = 0x050001e0;
    *(volatile u32 *)0x040000d8 = 0x050001c0;
    *(volatile u32 *)0x040000dc = 0x84000008;
    *(volatile u16 *)0x050001c8 = 0x6318;

    for (;;) {
        s32 cursor_width = x == 18 && (y == 4 || y == 5) ? 3 : 1;
        u32 pressed;

        Func_08020a60(grid_window, x, y, cursor_width, 1, 14);
        Func_080030f8(1);
        Func_08020a60(grid_window, x, y, cursor_width, 1, 15);

        if (move_cursor) {
            move_cursor = 0;
            Func_080b0030(&cursor_sprite,
                          (grid_window->tile_x + x) * 8 - 7,
                          (grid_window->tile_y + y) * 8 + 15, 3);
        }
        if (move_list) {
            move_list = 0;
            Func_080b0030(
                &list_sprite, Func_08020b14(selected) + 0x46, 0x16, 3);
        }

        Func_080b0020(&cursor_sprite);
        Func_080b0028(&list_sprite);
        if (list_sprite != 0) {
            s32 phase = (*gFrameCounter_08020bd8 >> 1) & 7;
            const s8 *wave = (const s8 *)0x080371f6;
            list_sprite->field_16 =
                (list_sprite->field_16 & 0xfe00) |
                ((list_sprite->field_06 + wave[phase]) & 0x1ff);
            list_sprite->field_14 =
                list_sprite->field_08 + wave[(phase + 5) & 7];
        }

        pressed = *gInputPressed_08020bd8;
        if (pressed & 0x40) {
            Func_080f9010(0x6f);
            move_cursor = 1;
            y--;
            if (x != 18) {
                if (y < 0)
                    y = 5;
            } else {
                y = y == 3 ? 5 : 4;
            }
        }
        if (pressed & 0x80) {
            Func_080f9010(0x6f);
            move_cursor = 1;
            y++;
            if (x != 18) {
                if (y == 6)
                    y = 0;
            } else {
                y = y == 6 ? 4 : 5;
            }
        }
        if (pressed & 0x20) {
            Func_080f9010(0x6f);
            move_cursor = 1;
            x--;
            if (x < 0)
                x = (u32)(y - 4) <= 1 ? 18 : 16;
            else if (x == 5 || x == 11 || x == 17)
                x--;
        }
        if (pressed & 0x10) {
            Func_080f9010(0x6f);
            move_cursor = 1;
            x++;
            if (x == 19)
                x = 0;
            else if (x == 5 || x == 11 || x == 17)
                x++;
            if (x == 18 && (u32)(y - 4) > 1)
                x = 0;
        }
        if (*gInputRepeat_08020bd8 & 8) {
            Func_080f9010(0x6f);
            move_cursor = 1;
            x = 18;
            y = 5;
        }

        if (pressed & 2) {
            Func_080f9010(0x71);
            goto delete_last;
        }
        if (!(pressed & 1))
            continue;

        Func_080f9010(0x70);
        if (x == 18) {
            if (y == 5) {
                if (selected_count == 0) {
                    refresh_path = 1;
                    list_sprite->field_05 = 13;
                    Func_08016478(list_window);
                    Func_08020b64(list_window, saved);
                    refresh_path = 3;
                    Func_080030f8(10);
                } else {
                    for (i = 0; i < 15; i++)
                        saved[i] = selected[i];
                }
                break;
            }
            if (y == 4)
                goto delete_last;
            continue;
        }

        if (selected_count != 5) {
            s32 tile_x = grid_window->tile_x + x + 1;
            s32 tile_y = grid_window->tile_y + y + 1;

            *write++ = tilemap[(tile_y * 32 + tile_x) * 2];
            *write = 0;
            selected_count++;
            if (selected_count == 5) {
                x = 18;
                y = 5;
                move_cursor = 1;
            }
            Func_08016478(list_window);
            Func_08020b64(list_window, selected);
            move_list = 1;
        }
        continue;

delete_last:
        if (selected_count != 0) {
            refresh_path = 2;
            selected_count--;
            *--write = 0;
            Func_08016478(list_window);
            Func_08020b64(list_window, selected);
            refresh_path = 4;
            move_list = 1;
            continue;
        }
        result = -1;
        break;
    }

    Func_08016418(grid_window, 2);
    Func_08016418(list_window, 2);
    Func_08019e48(combatant);
    Func_080030f8(1);
    return result;
}
