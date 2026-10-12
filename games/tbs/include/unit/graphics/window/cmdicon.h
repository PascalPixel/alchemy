#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_CMDICON_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_CMDICON_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

void AffineEffect_UpdateFrame(void);
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32 initial);
s32 Menu_SelectResource(s32 start, s32 goal);
void Menu_LoadResourceSlot(s32 slot, s32 index);
void Menu_AppendResourceEntry(s32 no);
void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base);
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base);
s32 Menu_SelectTopEntry(s32 sel);
s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1);
s32 Menu_SelectSaveSlotAction(void);
void Menu_DrawModeLabel(void);
void Menu_DrawModeIndicator(void);
s32 Menu_SelectResourceLayout(s32 mode);
s32 Menu_SelectEntry11To14(s32 sel);
s32 Menu_SelectEntry19To1c(s32 sel);
s32 Menu_RunConfirmSelection(s32 low, s32 mes, s32 w, s32 sel);
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel);
s32 Menu_SelectEntry20To21(s32 sel);

#endif
