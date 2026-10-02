#ifndef ALCHEMY_CHARACTER_H
#define ALCHEMY_CHARACTER_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct CharacterDefinition {
    u8 unknown_00[0x50];
    s16 hp[6];
    s16 pp[6];
    u16 attack[6];
    u16 defense[6];
    u16 agility[6];
    u8 luck[6];
    u8 element_bonus[4];
    u8 starting_level;
    u8 unknown_097;
    u16 items[13];
    u8 unknown_0b2[2];
};

struct ClassDefinition {
    s32 series;
    u8 required[4];
    u8 hp;
    u8 pp;
    u8 attack;
    u8 defense;
    u8 agility;
    u8 luck;
    u8 unknown_0e[0x46];
};

struct DjinnDefinition {
    u8 unknown_00[4];
    s8 hp;
    s8 pp;
    s8 attack;
    s8 defense;
    s8 agility;
    s8 luck;
    u8 unknown_0a[2];
};

struct EnemyDefinition {
    u8 unknown_00[15];
    u8 level;
    u16 hp;
    u16 pp;
    u16 attack;
    u16 defense;
    u16 agility;
    u8 luck;
    u8 turns;
    u8 hp_regen;
    u8 pp_regen;
    u8 unknown_1e[2];
    u32 rewards;
    u8 unknown_24[4];
    u16 items[4];
    u8 counts[4];
    u8 element_preset;
    s8 target_strategy;
    u8 unknown_36[0x16];
    u16 coins;
    s16 item;
    s16 item_chance;
    u16 experience;
};

struct LevelUpResult {
    s16 level;
    u16 ability;
    u16 hp;
    u16 pp;
    u16 attack;
    u16 defense;
    u16 agility;
    u16 luck;
};

LAYOUT_SIZE_GUARD(CharacterDefinition_Size, struct CharacterDefinition, 0xb4);
LAYOUT_OFFSET_GUARD(CharacterDefinition_Hp, struct CharacterDefinition, hp, 0x50);
LAYOUT_OFFSET_GUARD(CharacterDefinition_StartingLevel, struct CharacterDefinition, starting_level, 0x96);
LAYOUT_OFFSET_GUARD(CharacterDefinition_Items, struct CharacterDefinition, items, 0x98);
LAYOUT_SIZE_GUARD(ClassDefinition_Size, struct ClassDefinition, 0x54);
LAYOUT_OFFSET_GUARD(ClassDefinition_Hp, struct ClassDefinition, hp, 8);
LAYOUT_SIZE_GUARD(DjinnDefinition_Size, struct DjinnDefinition, 0xc);
LAYOUT_SIZE_GUARD(EnemyDefinition_Size, struct EnemyDefinition, 0x54);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Rewards, struct EnemyDefinition, rewards, 0x20);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Coins, struct EnemyDefinition, coins, 0x4c);
LAYOUT_OFFSET_GUARD(EnemyDefinition_Experience, struct EnemyDefinition, experience, 0x52);
LAYOUT_SIZE_GUARD(LevelUpResult_Size, struct LevelUpResult, 0x10);

extern struct CharacterDefinition Character_DefinitionTable[];
extern struct ClassDefinition Class_DefinitionTable[];
struct CharacterDefinition *Owner_GetRecordStride180(s32 index);
struct ClassDefinition *Owner_GetRecordStride84(s32 index);
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *result);
struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(s32 owner, struct LevelUpResult *result);
struct LevelUpResult *OwnerAction_CheckLevelThreshold(s32 owner, struct LevelUpResult *result);
void *Trade_GetOfferState(s32 which);

#endif
