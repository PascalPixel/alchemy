#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "WINDOW.H"

/* The runtime links a sprite's three hardware attributes behind its next
   pointer before copying them to OAM. */
struct SelectionSprite {
    struct SelectionSprite *next;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 mosaic : 1;
    u8 colors : 1;
    u8 shape : 2;
    u16 x : 9;
    u16 param : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
};

/* A list or path entry, also used for the side arrows, cursor, and enlarged
   icon. Cursor scale is its initialized flag; other entries animate scale. */
struct SelectionNode {
    struct SelectionNode *prev;
    struct SelectionNode *next;
    u16 base;
    u16 kind;
    u16 slot;
    u16 tile;
    s16 x;
    s16 y;
    s16 dx;
    s16 dy;
    s16 target_x;
    s16 target_y;
    s16 home_x;
    s16 home_y;
    u16 message;
    s16 scale;
    s16 scale_step;
    s16 scale_end;
    struct SelectionSprite sprite;
};

/* UiGlyph_ResetWorkState allocates all sixteen records together: two side
   arrows, seven free list entries, five path entries, cursor, enlarged icon.
   The lists link those records; the sixteen pending choices remain separate. */
struct SelectionScreen {
    struct SelectionNode records[16];
    u16 matrix[4];
    struct SelectionNode *head;
    struct SelectionNode *path;
    struct UiWindow *window;
    u16 kinds[16];
    u16 bases[16];
    u16 count;
    u16 base_x;
    u16 base_y;
    u16 unknown_39a;
    u16 top;
    u16 cursor_index;
    u16 vertical_scroll;
    u16 status;
    u16 path_top[5];
    u16 path_cursor[5];
    u16 locked;
    u8 unknown_3ba[42];
};

LAYOUT_SIZE_GUARD(SelectionSprite_Size, struct SelectionSprite, 0x0c);
LAYOUT_SIZE_GUARD(SelectionNode_Size, struct SelectionNode, 0x34);
LAYOUT_OFFSET_GUARD(SelectionNode_Message, struct SelectionNode, message, 0x20);
LAYOUT_OFFSET_GUARD(SelectionNode_Sprite, struct SelectionNode, sprite, 0x28);
LAYOUT_OFFSET_GUARD(SelectionScreen_Head, struct SelectionScreen, head, 0x348);
LAYOUT_OFFSET_GUARD(SelectionScreen_Kinds, struct SelectionScreen, kinds, 0x354);
LAYOUT_OFFSET_GUARD(SelectionScreen_CursorIndex, struct SelectionScreen, cursor_index, 0x39e);
LAYOUT_OFFSET_GUARD(SelectionScreen_Locked, struct SelectionScreen, locked, 0x3b8);
LAYOUT_SIZE_GUARD(SelectionScreen_Size, struct SelectionScreen, 0x3e4);

#endif
