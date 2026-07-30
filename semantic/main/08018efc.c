#include "types.h"

struct OamCommand_08018efc {
    struct OamCommand_08018efc *next;
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 affine;
};

struct SpriteNode_08018efc {
    struct SpriteNode_08018efc *next;
    u8 unknown_04;
    u8 kind;
    s16 x;
    s16 y;
    u8 unknown_0a[4];
    u8 oam_slot;
    u8 unknown_0f;
    struct OamCommand_08018efc oam;
};

struct DisplayState_08018efc {
    u8 unknown_000[0x698];
    struct SpriteNode_08018efc sprites[64];
    u8 unknown_d98[0x51e];
    u16 marker_resource;
};

struct WindowFrame_08018efc {
    u8 unknown_00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

struct SpriteNode_08018efc *Func_08015e8c(void);
s32 Func_08004080(void);
void Func_08016584(
    struct WindowFrame_08018efc *window,
    struct SpriteNode_08018efc *sprite);

/*
 * Place one interior window tile.  Mode one instead creates the OAM marker
 * anchored to the window's lower-right corner.
 */
void Func_08018efc(
    struct WindowFrame_08018efc *window,
    u32 tile,
    u32 column,
    u32 row,
    s32 mode)
{
    struct DisplayState_08018efc *display =
        *(struct DisplayState_08018efc **)0x03001e8c;

    if (row > (u32)(window->height - 2) ||
        column > (u32)(window->width - 2)) {
        return;
    }

    if (mode == 1) {
        struct SpriteNode_08018efc *sprite = Func_08015e8c();

        if (sprite != 0) {
            u16 attr1;
            u8 attr0;

            sprite->kind = 2;
            if (display->marker_resource == 0x63) {
                display->marker_resource = (u16)Func_08004080();
            }

            attr1 = (sprite->oam.attr1 & 0xfe00) |
                (((window->x + window->width - 2) * 8 + 4) & 0x01ff);
            attr0 =
                ((window->y + (u8)window->height - 2) * 8) - 1;
            sprite->oam.attr1 = attr1;
            ((u8 *)&sprite->oam.attr0)[0] = attr0;
            sprite->x = attr1 & 0x01ff;
            sprite->y = attr0;
            sprite->next = 0;
            sprite->oam_slot =
                (u8)((sprite - display->sprites) * 4);

            if (sprite->kind == 0) {
                sprite->kind = (u8)mode;
            }
            Func_08016584(window, sprite);
        }
    } else if (tile <= 0xff) {
        u32 tile_index =
            ((u8)window->y + row + 1) * 32 +
            window->x + column + 1;

        if (tile_index < 0x280) {
            ((u16 *)display)[tile_index] = (u16)tile | 0xf000;
        }
    }
}
