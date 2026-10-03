#ifndef ALCHEMY_WINDOW_H
#define ALCHEMY_WINDOW_H

#include "TYPES.H"
#include "TBS_EDITION.H"

/* One window record, as UiWindow_Create returns it; positions and sizes are
   in tiles. */
struct UiWindow {
    s32 unknown_00;
    s32 unknown_04;
    u16 width;                  /* 0x08 */
    u16 height;                 /* 0x0a */
    u16 x;                      /* 0x0c */
    u16 y;                      /* 0x0e */
    u16 unknown_10;
    u16 unknown_12;
    u16 state;                  /* 0x14 */
    u16 flags;                  /* 0x16 */
    u16 frame;                  /* 0x18 */
    s16 duration;               /* 0x1a */
    u16 previous_x;             /* 0x1c */
    u16 previous_y;             /* 0x1e */
    u16 previous_width;         /* 0x20 */
    u16 previous_height;        /* 0x22 */
};

/* The flag bytes of the window and text work behind gWindowWork[0]. The
   Japanese work is 0x90 bytes longer before them (TBS_EDITION.H). */
struct UiRenderWork {
    u8 unknown_000[RENDER_DIRTY_OFS];
    u8 dirty;                   /* rows of tiles to upload, one bit per four */
    u8 mode;
    u8 menu_state;
    u8 menu_busy;               /* holds the upload while a page is redrawn */
    u8 level;                   /* text colour of the next string */
};

extern u8 *gWindowWork[];
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_Finalize(struct UiWindow *window, s32 release);

#endif
