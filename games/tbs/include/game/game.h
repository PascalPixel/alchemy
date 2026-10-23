#ifndef GAME_H
#define GAME_H

/// @file
/// The item and summon tables and the game bank's routines.

#include "types.h"
#include "edition.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "item.h"
#include "character_ids.h"
#include "character.h"
#include "preset_table.h"
#include "farcall.h"

struct ItemDefinition;
struct SummonDefinition;

/// The 324 items by item id (items.tsv), read through Item_GetDirect
/// (Item_GetDirect in the overlays).
extern struct ItemDefinition gItem_DefinitionTable[];
/// The 16 summons (summons.tsv) by summon id, the numbers gSummon_OrderList
/// lists; read through SummonDefinition_Get.
extern const struct SummonDefinition gSummon_DefinitionTable[];

struct AbilityDefinition;
struct EnemyDefinition;
struct PresetValues;

/// The 519 abilities by ability id (abilities.tsv).
extern const struct AbilityDefinition gAbilityDefinitions[];
/// Each character's lighthouse group: 0 for Isaac, Garet, Felix and Jenna,
/// whose PP a lit Venus or Mars Lighthouse restores; 1 for the others, by
/// Mercury or Jupiter (Party_RestorePpOfLitElements).
extern const u8 gCharacter_ElementGroupTable[];
/// The experience each character needs for levels 1 to 99, 99 words a
/// character (Unit_GetExperienceForLevel).
extern u32 gCharacter_LevelExpTable[];
/// The Djinni sprite of each element, VENUS_DJINNI to JUPITER_DJINNI, whose
/// picture Djinn_ShowJoinedMessage shows as the portrait.
extern s16 gDjinn_ElementSprites[];
/// The class series by a character's strongest and second element, a 4 by 4
/// table read as [strongest * 4 + second] (Class_GetSeries).
extern s32 gClass_SeriesByElements[];
/// The 80 Djinn, 20 of each element in element order (djinn.tsv).
extern const struct DjinnDefinition gDjinn_DefinitionTable[];
/// Pairs of an element difference (power less resistance) and what it adds to
/// the multiplier in 256ths (256 = 1x), from 200 (+256) down to -200 (-256);
/// Battle_GetElementMultiplier interpolates and adds 256.
extern const s16 gElement_PowerResistCurve[5][2];
/// The 165 enemies (enemies.tsv).
extern const struct EnemyDefinition gEnemy_Definitions[];
/// The enemies' element presets (elempre.tsv): attack element, levels and
/// base power and resistance.
extern struct PresetValues gEnemy_ElementPresetTable[];
/// The game flags, 4096 bits in 512 bytes, saved with the game: flag n is bit
/// n & 7 of byte GAME_FLAG_BYTE(n).
extern u8 gGameFlagBytes[];
/// The party's equipment put aside while the Colosso takes it away: a
/// sentinel, four 15-slot inventories, the two shortcuts and the party order.
extern u16 gInventorySnapshot[];
/// How many of each rare item the shops may still sell, 0 to 99, by counter
/// number (gItem_ArtifactSlotTable; ItemCounter_Adjust).
extern u8 gItemCounters[128];
/// The 64-byte save stamp, in the first half of the save buffer, that the
/// Battle Arena writes into the current file alone.
extern u8 gSaveStamp[];
/// Each item's rare item counter number plus one, or 0 for an ordinary item
/// (Item_AdjustCounter).
extern u8 gItem_ArtifactSlotTable[];
/// Each party member's sprite, by character: Felix's is 35.
extern s16 gCharacter_SpriteTable[];
/// Each party member's sprite during the prologue (FLAG_CHILDHOOD):
/// ISAAC_CHILD, GARET_CHILD, FELIX_CHILD and JENNA_CHILD in place of their own.
extern s16 gCharacter_ChildSpriteTable[];
/// The summon ids in menu order (Summon_ListLearned).
extern u8 gSummon_OrderList[16];
/// Ends a list of summon ids (Summon_ListLearned).
#define SUMMON_END 32
s32 Audio_Check(void);

#endif
