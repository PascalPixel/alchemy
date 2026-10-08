#ifndef GAME_H
#define GAME_H

/* GAME */

#include "types.h"
#include "edition.h"
#include "btlsmn.h"
#include "battle_unit.h"
#include "item.h"
#include "chara.h"
#if !GAME_TLA
#include "character.h"
#include "preset_table.h"
#endif
#if GAME_TLA
#include "btltype.h"
#endif

struct ItemDefinition;
struct SummonDefinition;

extern struct ItemDefinition Item_DefinitionTable[];
extern const struct SummonDefinition Summon_DefinitionTable[];
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit);

#if !GAME_TLA
struct BtlAct;
struct EnemyDefinition;
struct PresetValues;

extern struct BtlAct BattleAction_DefinitionTable[];
extern const u8 Character_ElementGroupTable[];
extern u32 Character_LevelExpTable[];
extern s32 Character_StartingEquipOwnerIds[];
extern s16 Djinn_ElementSprites[];
extern s32 Owner_FourColumnValues[];
extern const u16 Djinn_DefinitionTable[];
extern const s16 Element_PowerResistCurve[5][2];
extern const struct EnemyDefinition Enemy_Definitions[];
extern struct PresetValues Enemy_ElementPresetTable[];
extern u16 gInventorySnapshot[];
extern u8 gItemCounters[128];
extern u8 gSaveStamp[];
extern u8 Item_ArtifactSlotTable[];
extern s16 Character_SpriteTable[];
extern s16 Character_ChildSpriteTable[];
extern u8 Summon_OrderList[16];
void Ability_PlayUseAnimation(s32 action);
s32 Audio_Check(void);
#ifndef OWN_BattleEffect_ApplyToTargets
s32 BattleEffect_ApplyToTargets(s32 effect_id, s32 source_id, s32 target_id, s32 fixed_scale);
#endif
s32 Curve_LookupScaledValue(s32 input, s32 halve);
s32 Djinn_CheckTurnBalance(s32 from, s32 to);
void Djinn_CountTurns(u8 *counts);
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 target);
s32 DjinnMenu_RunScreen(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
#ifndef OWN_Game_ResetForNewGameFar
void Game_ResetForNewGameFar(s32 mode);
#endif
void GameFlag_ClearBit(s32 flag);
void GameState_InitDefaults(void);
void Inventory_AddAndEquip(s32 owner, s32 target);
#ifndef OWN_Inventory_DiscardFar
s32 Inventory_DiscardFar(s32 owner, s32 slot);
#endif
s32 Inventory_RequestMode(s32, s32, s32, s32);
s32 Item_ClassifyUseAbility(s32 owner, s32 item);
void Item_PlayUseAnimation(u32 item);
s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1);
s32 Menu_RunConfirmSelectionFar(s32 value, s32 x, s32 y, s32 z);
s32 Menu_SetFirstObjectRowCoordinates(s32 arg0);
s32 Object_CallSpawnRoutineAtOrigin(s32 value);
void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value);
s32 Owner_DetermineClass(s32 character, const u8 *djinn);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
s32 Owner_GetResistanceValue(s32 owner, s32 index);
void Owner_InitRecords(void);
s32 Owner_LookupFourColumnTable(s32 row, s32 column);
s32 OwnerAction_RunCompareLoop(u32 mode);
void Party_Apply(s32, u16 *);
void Party_ApplyStatePreset(void);
s32 Party_CheckMemberValueTotal(s32 id);
void Party_Do(void *);
#ifndef OWN_Party_GetAverageLevel
u32 Party_GetAverageLevel(void);
#endif
#ifndef OWN_Party_LookupCharacterValueByFlag32
s32 Party_LookupCharacterValueByFlag32(u32 index);
#endif
s32 Shop_GetSelectionState(s32 owner, s32 slot);
s32 Shop_PickUnitItemFar(s32 *owner, s32 *slot);
void UiText_DecodeMessageFar(s32 message, u16 *buffer, s32 length);
s32 DjinnMenu_ShowOwnerStatus(void);
#endif

#if GAME_TLA
struct ActionDefinition;
struct BattleElementStats;

extern const u16 Battle_Effect90Classes[47];
extern const u16 Battle_Effect91Classes[29];
extern struct ActionDefinition BattleAction_DefinitionTable[];
extern const u16 Djinn_Definitions[][6];
extern const u8 Enemy_DefinitionTable[];
extern u8 GameFlagBytes[];
void BattlePlacement_UpdateTimedEntriesTwentyTimesFar(void);
s32 Djinn_AddToOwnerFar(s32, s32, s32);
void Event_ClearInvalidPackedValuesFar(s32);
s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
void Owner_CalculateBaseElements(s32 owner_no, struct BattleElementStats *elements);
s32 Owner_DetermineClass(s32 class_id, u32 *djinn, s32 item);
s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 *output);
void Owner_RefreshClassActions(s32);
void Owner_UpdateClassActions(s32 owner_no);
void *Resource_FarCall005(s32);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
#endif

#endif
