#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

#include "unit/game/character/chara.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "types.h"
#include "ownerval.h"
#include "layout_guard.h"

struct BtlUnit;

struct CharacterDefinition
{
	u8	unknown_00[0x50];
	s16 hp[6];
	s16 ep[6];
	u16 atk[6];
	u16 def[6];
	u16 agi[6];
	u8	luck[6];
	u8	element_bonus[4];
	u8	starting_level;
	u8	unknown_097;
	u16 items[13];
	u8	unknown_0b2[2];
};

struct ClassAbility
{
	u8 ability;
	u8 level;
	u8 unknown_02[2];
};

struct ClassDefinition
{
	long				series;
	u8					required[4];
	u8					hp;
	u8					ep;
	u8					atk;
	u8					def;
	u8					agi;
	u8					luck;
	u8					unknown_0e[2];
	struct ClassAbility abilities[16];
	u8					unknown_50[4];
};

struct DjinnDefinition
{
	u8 unknown_00[4];
	s8 hp;
	s8 ep;
	s8 atk;
	s8 def;
	s8 agi;
	s8 luck;
	u8 unknown_0a[2];
};

struct EnemyDefinition
{
	u8	unknown_00[15];
	u8	level;
	u16 hp;
	u16 ep;
	u16 atk;
	u16 def;
	u16 agi;
	u8	luck;
	u8	turns;
	u8	hp_regen;
	u8	ep_regen;
	u8	unknown_1e[2];
	u32 rewards;
	u8	unknown_24[4];
	u16 items[4];
	u8	counts[4];
	u8	element_preset;
	s8	target_strategy;
	s8	action_strategy;
	u8	item_actions;
	u16 actions[8];
	u8	unknown_48[4];
	u16 coins;
	s16 item;
	s16 item_chance;
	u16 experience;
};

struct LevelUpResult
{
	s16 level;
	u16 ability;
	u16 hp;
	u16 ep;
	u16 atk;
	u16 def;
	u16 agi;
	u16 luck;
};

LAYOUT_SIZE_GUARD(CharacterDefinition_Size, struct CharacterDefinition, 0xb4);
LAYOUT_OFFSET_GUARD(CharacterDefinition_Hp, struct CharacterDefinition, hp, 0x50);
LAYOUT_OFFSET_GUARD(
	CharacterDefinition_StartingLevel, struct CharacterDefinition, starting_level, 0x96);
LAYOUT_OFFSET_GUARD(CharacterDefinition_Items, struct CharacterDefinition, items, 0x98);
LAYOUT_SIZE_GUARD(ClassDefinition_Size, struct ClassDefinition, 0x54);
LAYOUT_OFFSET_GUARD(ClassDefinition_Hp, struct ClassDefinition, hp, 8);
LAYOUT_SIZE_GUARD(ClassAbility_Size, struct ClassAbility, 4);
LAYOUT_OFFSET_GUARD(ClassDefinition_Abilities, struct ClassDefinition, abilities, 0x10);
LAYOUT_SIZE_GUARD(DjinnDefinition_Size, struct DjinnDefinition, 0xc);
LAYOUT_SIZE_GUARD(EnemyDefinition_Size, struct EnemyDefinition, 0x54);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Rewards, struct EnemyDefinition, rewards, 0x20);
LAYOUT_OFFSET_GUARD(EnemyDefinition_ActionStrategy, struct EnemyDefinition, action_strategy, 0x36);
LAYOUT_OFFSET_GUARD(EnemyDefinition_ItemActions, struct EnemyDefinition, item_actions, 0x37);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Actions, struct EnemyDefinition, actions, 0x38);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Coins, struct EnemyDefinition, coins, 0x4c);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Experience, struct EnemyDefinition, experience, 0x52);
LAYOUT_SIZE_GUARD(LevelUpResult_Size, struct LevelUpResult, 0x10);

extern struct CharacterDefinition Character_DefinitionTable[];
extern struct ClassDefinition	  Class_DefinitionTable[];

#endif
