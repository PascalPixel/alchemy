#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

#include "unit/game/game.h"
#include "types.h"
#include "owner_value.h"
#include "static_assert.h"

struct BattleUnit;

struct CharacterDefinition {
  u8 unk00[0x50];
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
  u8 elementBonus[4];
  u8 startingLevel;
  u8 unk097;
  u16 items[13];
  u8 unk0b2[2];
};

struct ClassAbility {
  u8 ability;
  u8 level;
  u8 unk02[2];
};

struct ClassDefinition {
  long series;
  u8 required[4];
  u8 hp;
  u8 pp;
  u8 atk;
  u8 def;
  u8 agi;
  u8 luck;
  u8 unk0e[2];
  struct ClassAbility abilities[16];
  u8 unk50[4];
};

struct DjinnDefinition {
  u16 abilityId;
  u8 unk02[2];
  s8 hp;
  s8 pp;
  s8 atk;
  s8 def;
  s8 agi;
  s8 luck;
  u8 unk0a[2];
};

struct EnemyDefinition {
  u8 unk00[15];
  u8 level;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 turns;
  u8 hpRegen;
  u8 ppRegen;
  u8 unk1e[2];
  u32 rewards;
  u8 unk24[4];
  u16 items[4];
  u8 counts[4];
  u8 elementPreset;
  s8 targetStrategy;
  s8 actionStrategy;
  u8 itemActions;
  u16 actions[8];
  u8 unk48[4];
  u16 coins;
  s16 item;
  s16 itemChance;
  u16 experience;
};

struct LevelUpResult {
  s16 level;
  u16 ability;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u16 luck;
};

STATIC_ASSERT(sizeof(struct CharacterDefinition) == 0xb4);
STATIC_ASSERT(offsetof(struct CharacterDefinition, hp) == 0x50);
STATIC_ASSERT(offsetof(struct CharacterDefinition, startingLevel) == 0x96);
STATIC_ASSERT(offsetof(struct CharacterDefinition, items) == 0x98);
STATIC_ASSERT(sizeof(struct ClassDefinition) == 0x54);
STATIC_ASSERT(offsetof(struct ClassDefinition, hp) == 8);
STATIC_ASSERT(sizeof(struct ClassAbility) == 4);
STATIC_ASSERT(offsetof(struct ClassDefinition, abilities) == 0x10);
STATIC_ASSERT(sizeof(struct DjinnDefinition) == 0xc);
STATIC_ASSERT(offsetof(struct DjinnDefinition, abilityId) == 0);
STATIC_ASSERT(offsetof(struct DjinnDefinition, unk02) == 2);
STATIC_ASSERT(offsetof(struct DjinnDefinition, hp) == 4);
STATIC_ASSERT(sizeof(struct EnemyDefinition) == 0x54);
STATIC_ASSERT(offsetof(struct EnemyDefinition, rewards) == 0x20);
STATIC_ASSERT(offsetof(struct EnemyDefinition, actionStrategy) == 0x36);
STATIC_ASSERT(offsetof(struct EnemyDefinition, itemActions) == 0x37);
STATIC_ASSERT(offsetof(struct EnemyDefinition, actions) == 0x38);
STATIC_ASSERT(offsetof(struct EnemyDefinition, coins) == 0x4c);
STATIC_ASSERT(offsetof(struct EnemyDefinition, experience) == 0x52);
STATIC_ASSERT(sizeof(struct LevelUpResult) == 0x10);

extern struct CharacterDefinition gCharacter_DefinitionTable[];
extern struct ClassDefinition gClass_DefinitionTable[];

#endif
