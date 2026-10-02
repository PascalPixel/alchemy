#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

#include "TYPES.H"
#include "TBS_EDITION.H"

/* The flags the menus set in the window work. */
struct UiWork {
    u8 unknown_000[RENDER_DIRTY_OFS];
    unsigned dirty : 8;
    u8 unknown_mode[RENDER_MENU_BUSY_OFS - RENDER_DIRTY_OFS - 1];
    unsigned menu_busy : 8;
};

/* Window and text services the menus reach through the far-call table at
   0x08015000. */

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 UiWork_IsCompleteFar(void);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

#endif
