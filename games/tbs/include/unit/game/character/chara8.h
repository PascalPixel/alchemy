#ifndef ALCHEMY_UNIT_GAME_CHARACTER_CHARA8_H
#define ALCHEMY_UNIT_GAME_CHARACTER_CHARA8_H

#include "types.h"

void Inventory_AddAndEquip(s32 owner, s32 target);
void InventorySnapshot_Save(void);
void InventorySnapshot_Restore(void);

#endif
