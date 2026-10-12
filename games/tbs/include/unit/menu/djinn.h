#ifndef ALCHEMY_UNIT_MENU_DJINN_H
#define ALCHEMY_UNIT_MENU_DJINN_H

#include "types.h"

struct OwnerActionSlot;

s32 Menu_ReservedStatusZero(void);
s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1);
s32 Menu_SetFirstObjectRowCoordinates(s32 arg0);
s32 ItemMenu_Run(void);
s32 DjinnMenu_RunScreen(void);
void Graphics_AdjustPaletteBank(s32 d);
s32 Menu_OpenBackdropScreen(void);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b, u16 *out, s32 *first_count, s32 *second_count);
s32 Menu_ComputeEntryValues(void *tbl);
/* DjinnMenu_DrawElementList is called before its definition with no prototype in scope. */
s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette);
s32 DjinnMenu_ShowHelp(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element);
s32 Menu_RunPairedEntryAction(s32 mode, s32 param);
s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 owner, s32 give, s32 take, s32 mode, s32 page, s32 side);

#endif
