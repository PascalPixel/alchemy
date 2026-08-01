/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);
typedef struct {
    u32 allocation;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 link;
} MenuSprite_08023178;

typedef struct {
    u8 pad00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
} MenuWindow_08023178;

typedef struct {
    s8 cursor[3];
    u8 pad03;
    s32 displayed_value;
    s32 previous_value;
    s32 mode;
    s32 limit;
    s32 icon_x;
    s32 icon_y;
} MenuState_08023178;

#define BYTE_AT(p, n)   (*(u8 *)((u8 *)(p) + (n)))
#define SBYTE_AT(p, n)  (*(s8 *)((u8 *)(p) + (n)))
#define HALF_AT(p, n)   (*(u16 *)((u8 *)(p) + (n)))
#define SHALF_AT(p, n)  (*(s16 *)((u8 *)(p) + (n)))
#define WORD_AT(p, n)   (*(u32 *)((u8 *)(p) + (n)))
#define PTR_AT(a, type) (*(type **)(a))
#define WORD_GLOBAL(a)  (*(volatile u32 *)(a))
#define APPEND_ROW(k)   do { if (row_count < 8) row_kinds[row_count++] = (k); } while (0)

void *Func_080040b4(s32);
s32 Func_080040d0(void *, s32);
void Func_08003dec(void *, s32);
void Func_08003f3c(void *);
void Func_080030f8(s32);
void Func_08002df0(void *);
void *Func_08004938(s32);
MenuWindow_08023178 *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(void *, s32);
void Func_08016498(void *);
void Func_08016738(void);
void Func_08017248(s32, s32, s32, s32, s32);
void Func_08017aa4(void *, void *, s32, s32);
void Func_08019000(MenuWindow_08023178 *, s32, s32, s32, s32);
void Func_0801965c(s32, void *, s32);
void Func_08019908(s32, s32);
void Func_0801e318(void);
void Func_0801e41c(MenuWindow_08023178 *, s32, s32, s32, s32);
void Func_0801e7c0(s32, MenuWindow_08023178 *, s32, s32);
void Func_0801e8b0(const void *, MenuWindow_08023178 *, s32, s32);
void Func_0801ea08(s32, s32, MenuWindow_08023178 *, s32, s32);
void Func_0801f200(s32);
s32 Func_08021ab0(s32, s32);
s32 Func_08021b80(u32, s32);
void Func_08022768(s32, s32, s32, s32, s32);
void *Func_08077008(s32);
void Func_08077010(s32);
s32 Func_080771f8(s32, s32);
s32 Func_08077258(s32, s32);
void *Func_08077290(s32);
void *Func_080b5098(s32);
void Func_080f9010(s32);

/*
 * Display the modal status/details screen for one party member.
 *
 * `members` is an optional 0xff-terminated list.  0xfe entries are separators;
 * the initial selection is the entry equal to `member`.  The routine owns both
 * windows and all twelve temporary sprites until the modal loop terminates.
 */
s32 Func_08023178(u16 *members, s32 member_count, u32 member)
{
    MenuSprite_08023178 cursor_sprite;
    MenuSprite_08023178 row_sprites[11];
    void *row_tiles[11];
    s8 row_kinds[11];
    MenuState_08023178 state = {{0, 0, 0}, 0, 0, 0, 0, 0, 0, 0};
    void *actor;
    void *actor_snapshot;
    void *cursor_tiles;
    void *row_tile;
    MenuWindow_08023178 *main_window;
    MenuWindow_08023178 *key_window;
    u32 input;
    s32 selected_list_index = 0;
    s32 row_count = 0;
    s32 redraw = 1;
    s32 running = 1;
    s32 i;

    actor = PTR_AT(0x03001f34, void);
    actor_snapshot = Func_080040b4(0x200);
    cursor_tiles = Func_08077290(-1);
    row_tile = Func_080040b4(0x80);

    {
        void *display = PTR_AT(0x03001fdc, void);
        void *old_window = (void *)WORD_AT(display, 68);
        WORD_AT(display, 72) = 1;
        if (old_window != 0) {
            Func_08016418(old_window, 0);
            WORD_AT(display, 68) = 0;
        }
    }
    Func_080f9010(112);

    for (i = 0; i < 11; i++) {
        row_tiles[i] = Func_080040b4(0x80);
        row_kinds[i] = -1;
    }

    if (member_count != 0 && members != 0) {
        for (i = 0; i <= 5 && members[i] != 0xff; i++) {
            if (members[i] != 0xfe && members[i] == member) {
                selected_list_index = i;
                break;
            }
        }
    }

    main_window = Func_080162d4(0, 0, 30, 20, 6);
    key_window = Func_080162d4(0, 14, 30, 6, 10);
    Func_0801e318();

refresh:
    input = WORD_GLOBAL(0x03001b04);
    cursor_sprite.allocation = (u32)actor_snapshot;
    cursor_sprite.attr0 = 0x400;
    cursor_sprite.attr1 = 0x400;
    cursor_sprite.attr2 =
        (u16)Func_08021b80(BYTE_AT(actor, 0x128), (s32)actor_snapshot);
    cursor_sprite.attr0 = (cursor_sprite.attr0 & 0x03ff) | 0x8000;
    cursor_sprite.attr1 = (cursor_sprite.attr1 & 0x01ff) | 0x0400;
    cursor_sprite.attr2 = (cursor_sprite.attr2 & 0x03ff) | 0xc000;
    BYTE_AT(&cursor_sprite, 4) = 24;
    BYTE_AT(&cursor_sprite, 9) =
        (BYTE_AT(&cursor_sprite, 9) & 15) | 0xe0;
    Func_08003dec(&cursor_sprite, 240);

    if (state.displayed_value <= 9) {
        state.mode = 0;
        state.limit = cursor_tiles != 0 ? 9 : 7;
    } else if (state.displayed_value <= 13) {
        state.mode = 1;
        state.limit = 4;
    } else if (state.displayed_value <= 21) {
        state.mode = 2;
        state.limit = 0;
    }

    Func_08022768(main_window->x, main_window->width,
                  main_window->y, main_window->height, 15);

    if (row_count != 0) {
        s32 cursor = state.cursor[state.mode];

        if (state.mode <= 1) {
            if (input & 0x80) {
                input = 0;
                Func_080f9010(111);
                if (++cursor >= state.limit) {
                    cursor = 0;
                    if (state.mode == 1) {
                        state.mode = 2;
                        cursor = state.cursor[2];
                    }
                }
            } else if (input & 0x40) {
                input = 0;
                Func_080f9010(111);
                if (--cursor < 0) {
                    cursor = state.limit - 1;
                    if (state.mode == 1) {
                        state.mode = 2;
                        cursor = state.cursor[2];
                    }
                }
            } else if (input & 0x31) {
                input = 0;
                Func_080f9010(111);
                state.mode ^= 2;
                if (WORD_GLOBAL(0x03001b04) & 1)
                    cursor = state.cursor[state.mode];
                else
                    cursor = (WORD_GLOBAL(0x03001b04) & 0x20)
                                 ? row_count - 1 : 0;
            }
        } else if (state.mode == 2) {
            s32 movement_sound = 0;
            if (cursor >= state.limit)
                cursor = state.limit - 1;
            if (cursor < 0) {
                state.mode = 0;
                cursor = state.cursor[0];
            } else if (input & 0x10) {
                input = 0;
                if (++cursor >= state.limit) {
                    state.mode = 0;
                    cursor = state.cursor[0];
                }
                movement_sound = 1;
            } else if (input & 0x20) {
                input = 0;
                if (--cursor < 0) {
                    state.mode = 0;
                    cursor = state.cursor[0];
                }
                movement_sound = 1;
            } else if (input & 0xc1) {
                input = 0;
                state.mode = 0;
                cursor = state.cursor[0];
                Func_080f9010(111);
            }
            if (movement_sound)
                Func_080f9010(111);
        }
        state.cursor[state.mode] = (s8)cursor;
    }

    if (state.mode == 0) {
        const u8 *entry =
            (const u8 *)0x08037328 +
            (state.cursor[0] + (cursor_tiles == 0 ? 9 : 0)) * 8;
        state.displayed_value = entry[0];
        state.icon_x = entry[1];
        state.icon_y = entry[2];
        Func_08022768(main_window->x + entry[3] + 1,
                      main_window->y + entry[4] + 1, entry[5], 1, 14);
    } else {
        const u8 *entry;
        if (state.mode == 1)
            entry = (const u8 *)0x080373a8 + state.cursor[1] * 4;
        else
            entry = (const u8 *)0x080373b8 + state.cursor[2] * 4;
        state.displayed_value = entry[0];
        state.icon_x = entry[1];
        state.icon_y = entry[2];
    }
    if (state.previous_value != state.displayed_value) {
        state.previous_value = state.displayed_value;
        redraw = 2;
    }

    state.icon_x *= 8;
    state.icon_y *= 8;
    actor = Func_080b5098(member);

    row_sprites[10].allocation = WORD_AT(WORD_AT(actor, 0), 80);
    row_sprites[10].attr0 = 0x0400;
    row_sprites[10].attr1 = 0x0470;
    row_sprites[10].attr2 =
        HALF_AT((void *)row_sprites[10].allocation, 8) & 0x03ff;
    Func_08003dec(&row_sprites[10], 240);

    row_sprites[9].allocation = (u32)row_tile;
    row_sprites[9].attr0 = 0x400;
    row_sprites[9].attr1 =
        (u16)((main_window->x * 8 + state.icon_x + 16 -
               ((WORD_GLOBAL(0x03001e40) & 4) >> 2)) & 0x1ff);
    row_sprites[9].attr2 =
        (u16)(Func_080040d0(row_tile, 0x080310a4) & 0x03ff);
    BYTE_AT(&row_sprites[9], 4) =
        (u8)(main_window->y * 8 + state.icon_y + 16 -
             ((WORD_GLOBAL(0x03001e40) & 4) >> 2));
    BYTE_AT(&row_sprites[9], 7) =
        (BYTE_AT(&row_sprites[9], 7) & 0xc0) | 0x10;
    Func_08003dec(&row_sprites[9], 241);

    if (redraw != 0) {
        u8 *data = Func_08077008(member);
        s32 y;
        s32 x;

        Func_0801e318();
        Func_08016738();
        if (redraw & 1) {
            Func_08016498(main_window);
            Func_08017248(key_window->x, key_window->width,
                          key_window->y, key_window->height, 0);
            Func_0801e41c(main_window, 0, 14, 29, 14);
            Func_0801e8b0(data, main_window, 0, 0);
            Func_0801e8b0((const void *)0x080373dc,
                          main_window, 56, 0);
            Func_0801ea08(data[15], 2, main_window, 72, 0);
            Func_0801e7c0(0x8ba, main_window, 0, 8);
            Func_0801ea08(WORD_AT(data, 292), 8, main_window, 40, 8);
            Func_0801e8b0((const void *)0x080373e0,
                          main_window, 40, 24);
            Func_0801ea08(SHALF_AT(data, 56), 4,
                          main_window, 56, 24);
            Func_0801e8b0((const void *)0x080373e4,
                          main_window, 88, 24);
            Func_0801ea08(SHALF_AT(data, 52), 4,
                          main_window, 96, 24);
            Func_0801e8b0((const void *)0x080373e0,
                          main_window, 40, 32);
            Func_0801ea08(SHALF_AT(data, 58), 4,
                          main_window, 56, 32);
            Func_0801e8b0((const void *)0x080373e4,
                          main_window, 88, 32);
            Func_0801ea08(SHALF_AT(data, 54), 4,
                          main_window, 96, 32);
            Func_0801e7c0(0x8b0, main_window, 136, 16);
            Func_0801ea08(HALF_AT(data, 60), 3,
                          main_window, 184, 16);
            Func_0801e7c0(0x8b1, main_window, 136, 24);
            Func_0801ea08(HALF_AT(data, 62), 3,
                          main_window, 184, 24);
            Func_0801e7c0(0x8b2, main_window, 136, 32);
            Func_0801ea08(HALF_AT(data, 64), 3,
                          main_window, 184, 32);
            Func_0801e7c0(0x8b3, main_window, 136, 40);
            Func_0801ea08(data[66], 3, main_window, 184, 40);
            Func_0801e7c0(data[0x129] + 0x741,
                          main_window, 0, 48);
            if (cursor_tiles != 0)
                Func_0801e7c0(0x8b9, main_window, 0, 72);
            Func_0801e7c0(0x8b5, main_window, 0, 80);
            Func_0801e7c0(0x8b6, main_window, 0, 88);
            Func_0801e7c0(0x8b7, main_window, 0, 96);

            for (i = 0, x = 40, y = 72; i < 4;
                 i++, x += 32, y += 32) {
                Func_08019000(main_window,
                              ((const u16 *)0x080373d8)[i],
                              cursor_tiles == 0 ? 9 : 8, 7 + i * 4, 0);
                if (cursor_tiles != 0) {
                    Func_0801ea08(data[284 + i], 1,
                                  main_window, x, 72);
                    Func_0801e8b0((const void *)0x080373e0,
                                  main_window, y, 72);
                    Func_0801ea08(data[280 + i], 1,
                                  main_window, y + 8, 72);
                }
                Func_0801ea08(Func_080771f8(member, i), 2,
                              main_window, x, 80);
                Func_0801ea08(SHALF_AT(data, 72 + i * 4), 3,
                              main_window, x, 88);
                Func_0801ea08(SHALF_AT(data, 74 + i * 4), 3,
                              main_window, x, 96);
            }

            row_count = 0;
            if (SHALF_AT(data, 56) == 0)
                APPEND_ROW(16);
            if (SBYTE_AT(data, 304) != 0)
                APPEND_ROW(15);
            if (BYTE_AT(data, 0x131))
                APPEND_ROW(8);
            if (SBYTE_AT(data, 0x13d) == 1)
                APPEND_ROW(1);
            if (SBYTE_AT(data, 0x13d) == 2)
                APPEND_ROW(2);
            if (BYTE_AT(data, 0x13b))
                APPEND_ROW(4);
            if (BYTE_AT(data, 0x133))
                APPEND_ROW(3);
            if (BYTE_AT(data, 316))
                APPEND_ROW(5);
            if (BYTE_AT(data, 320))
                APPEND_ROW(7);
            if (BYTE_AT(data, 312))
                APPEND_ROW(6);
            if (BYTE_AT(data, 306)) {
                if (SBYTE_AT(data, 0x135) > 0)
                    APPEND_ROW(9);
                if (SBYTE_AT(data, 0x135) < 0)
                    APPEND_ROW(10);
            }
            if (BYTE_AT(data, 308)) {
                if (SBYTE_AT(data, 0x137) > 0)
                    APPEND_ROW(11);
                if (SBYTE_AT(data, 0x137) < 0)
                    APPEND_ROW(12);
            }
            if (BYTE_AT(data, 310)) {
                if (SBYTE_AT(data, 0x141) > 0)
                    APPEND_ROW(13);
                if (SBYTE_AT(data, 0x141) < 0)
                    APPEND_ROW(14);
            }
            if (SBYTE_AT(data, 0x147) > 0)
                APPEND_ROW(17);
            if (SBYTE_AT(data, 0x147) < 0)
                APPEND_ROW(18);

            if (row_count == 0)
                APPEND_ROW(0);
            for (i = row_count; i < 11; i++)
                row_kinds[i] = -1;
            for (i = 0; i < row_count; i++)
                Func_08021ab0(row_kinds[i], (s32)row_tiles[i]);
            if (row_kinds[0] == 0)
                Func_0801e7c0(SHALF_AT(data, 56) != 0 ? 0x8bd : 0x8be,
                              main_window, 112, 0);
        }

        {
            void *text = Func_08004938(0x100);
            s32 description_id;
            s32 prepared_description = 0;
            if (state.displayed_value > 13) {
                s32 kind = row_kinds[state.displayed_value - 14];
                s32 delta = 0;
                void *snapshot;
                u8 old_60;
                u8 old_62;
                u8 old_64;

                if (kind == 0 && SHALF_AT(data, 56) == 0)
                    kind = 16;
                snapshot = Func_08004938(0x14c);
                ((WordCopy)0x03001388)(snapshot, data, 0x14c);
                old_60 = BYTE_AT(data, 60);
                old_62 = BYTE_AT(data, 62);
                old_64 = BYTE_AT(data, 64);
                BYTE_AT(data, 0x13d) = 0;
                BYTE_AT(data, 0x13b) = 0;
                BYTE_AT(data, 0x133) = 0;
                Func_08077010(member);
                switch (kind) {
                case 8:  delta = BYTE_AT(data, 0x131); break;
                case 9:  delta = old_60 - BYTE_AT(data, 60); break;
                case 10: delta = BYTE_AT(data, 60) - old_60; break;
                case 11: delta = old_62 - BYTE_AT(data, 62); break;
                case 12: delta = BYTE_AT(data, 62) - old_62; break;
                case 13: delta = old_64 - BYTE_AT(data, 64); break;
                case 14: delta = BYTE_AT(data, 64) - old_64; break;
                case 15: delta = SBYTE_AT(data, 0x147) * 20; break;
                default: break;
                }
                ((WordCopy)0x03001388)(data, snapshot, 0x14c);
                Func_08002df0(snapshot);
                Func_08019908(delta, 5);
                description_id = kind + 0x8d2;
                prepared_description = 1;
            } else if (state.displayed_value == 2 && data[15] <= 98) {
                s32 value = Func_08077258(member, data[15] + 1);
                value -= WORD_AT(data, 292);
                Func_08019908(value, 5);
                description_id = 0x8bf;
                prepared_description = 1;
            } else {
                description_id = state.displayed_value + 0x8c0;
                Func_0801965c(description_id, text, 0x80);
            }
            if (prepared_description)
                Func_0801965c(description_id, text, 0x80);
            Func_08017aa4(text, key_window, 0, 4);
            Func_08002df0(text);
            BYTE_AT(PTR_AT(0x03001c94, void), 0xea3) = 1;
        }
        redraw = 0;
    }

    for (i = 0; i < 11; i++) {
        row_sprites[i].allocation = (u32)row_tiles[i];
        row_sprites[i].attr0 = 0x400;
        row_sprites[i].attr1 = (u16)((112 + i * 15) & 0x1ff);
        row_sprites[i].attr2 =
            ((const u16 *)0x03001b10)[(u32)row_tiles[i] * 2 + 1] >> 5;
        BYTE_AT(&row_sprites[i], 4) = (u8)(main_window->y * 8 + 8);
        if (row_kinds[i] > 0)
            Func_08003dec(&row_sprites[i], 240);
    }

    if (WORD_AT(PTR_AT(0x03001e74, void), 76) != 0 &&
        !(WORD_GLOBAL(0x03001b04) & 2)) {
        if (member_count != 0) {
            if (input & 0x100) {
                if (++selected_list_index >= member_count)
                    selected_list_index = 0;
                member = members[selected_list_index];
                redraw = 1;
                Func_080f9010(111);
            } else if (input & 0x200) {
                if (--selected_list_index < 0)
                    selected_list_index = member_count - 1;
                member = members[selected_list_index];
                redraw = 1;
                Func_080f9010(111);
            }
        }
        Func_080030f8(1);
        goto refresh;
    }

    for (i = 0; i < 11; i++)
        Func_08003f3c(row_tiles[i]);
    Func_08003f3c(actor_snapshot);
    Func_08003f3c(row_tile);
    Func_080030f8(1);
    Func_0801e318();
    Func_08016418(main_window, 1);
    Func_08016418(key_window, 1);
    Func_0801f200(BYTE_AT(PTR_AT(0x03001f34, void), 65));
    WORD_AT(PTR_AT(0x03001ff4, void), 72) = 0;
    Func_080030f8(1);
    (void)running;
    return 0;
}
