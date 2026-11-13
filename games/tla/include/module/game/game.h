#ifndef ALCHEMY_MODULE_GAME_GAME_H
#define ALCHEMY_MODULE_GAME_GAME_H

/// @file
/// What the game object defines for the code outside it.

#include "types.h"
#include "battle/battle_unit.h"
#include "owner_state.h"

struct LevelUpResult;

void Unit_Calculate(s32 owner);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 itemId);
extern const s32 gClass_SeriesByElements[4][4];
s32 Unit_KnowsPsynergy(s32 owner, s32 value);
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains);
void MainRoutineWork_InitializeDefaults(void);
extern const u8 Data_080b1f2c[4][5];
void Party_InitializeCharacters(void);
s32 Party_AddMember(s32 owner);
s32 Unit_LearnPsynergy(s32 owner, s32 ability);
s32 Party_RemoveMember(s32 owner);
void Djinn_ActivateOfferedForParty(void);
extern const u16 Data_080b1284[6];
void Func_080ae358(void);
s16 Unit_AddPp(s32 owner, s32 delta);
s32 PartyInventory_Remove(s32 item_id);
s32 PartyInventory_CountItem(s32 itemId);
s32 PartyInventory_CountFreeSlots(void);
u32 Party_GetAverageLevel(void);
s32 PartyInventory_FindRoom(s32 itemId);
s32 Inventory_GetQuantity(s32 owner, s32 slot);
s32 Party_AdjustCoins(s32 amount);
void Party_SetLeaderJenna(void);
void Party_SetLeaderFelix(void);

#endif
