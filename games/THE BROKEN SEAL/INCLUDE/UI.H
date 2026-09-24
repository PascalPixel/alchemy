#ifndef ALCHEMY_UI_H
#define ALCHEMY_UI_H

#include "TYPES.H"

/* Window and text services the menus reach through the far-call table at
   0x08015000. */

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 UiWork_IsCompleteFar(void);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

#endif
