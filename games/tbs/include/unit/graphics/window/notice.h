#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_NOTICE_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_NOTICE_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct AbilityListEntry;

void UiTimedNotice_Create(void);
void UiTimedNotice_Tick(void);
void UiTimedNotice_CloseIfActive(void);
s32 Item_CallHandler48(s32 arg0, s32 arg1);
s32 Item_ReturnTrue(void);
void Party_AdjustPaletteGlowPhase(s32 arg0);
void Debug_SelectAbilityPair(void);
s32 Object_CollectResources(struct AbilityListEntry *output);
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index, const struct AbilityListEntry *entries);
void Menu_RunSelectionHook(void);

#endif
