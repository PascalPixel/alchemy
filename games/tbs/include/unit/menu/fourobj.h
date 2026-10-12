#ifndef ALCHEMY_UNIT_MENU_FOUROBJ_H
#define ALCHEMY_UNIT_MENU_FOUROBJ_H

#include "types.h"

void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1);
void FourObjectMotion_UpdateAllPositions(void);
void FourObjectMotion_UpdateBottomRow(void);
void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1);
s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative);
s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value);
void Menu_EnableAllItemObjects(void);

#endif
