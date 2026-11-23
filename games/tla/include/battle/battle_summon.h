#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

/// @file
/// Each enemy's battle sprite and how it attacks and falls, the summons, and
/// the enemies called into a battle.

#include "static_assert.h"

#include "types.h"
#include "edition.h"

/// A class's plain attack (BattleSpriteDefinition.attackEffect), which
/// BattleEffect_RunByIdRange plays: BattleEffect_RunAttackSlash's beam from
/// the attacker and the target's recoil, or the canvas burst.
enum AttackEffect {
  ATTACK_EFFECT_SLASH = 1,    ///< a blue arc; the target is nudged
  ATTACK_EFFECT_CLAW,         ///< three blue arcs; the target is nudged
  ATTACK_EFFECT_THRUST,       ///< a beam reaching further each picture
  ATTACK_EFFECT_WAVE,         ///< a wave, played backwards
  ATTACK_EFFECT_RISING_SLASH, ///< the arc upside down; the target hops
  /// As ATTACK_EFFECT_RISING_SLASH, drawn 16 pixels lower.
  ATTACK_EFFECT_RISING_SLASH_LOW,
  ATTACK_EFFECT_HURL,  ///< no beam; the target is hurled (KNOCKBACK_HURL)
  ATTACK_EFFECT_SHOVE, ///< no beam; the target is pushed to 150 percent
  ATTACK_EFFECT_BLOW,  ///< no beam; the target is nudged
  ATTACK_EFFECT_BURST  ///< BattleEffect_RunCanvasModeByCount's small ball
};

/// A fallen class's cry (BattleSpriteDefinition.defeatSound): BattlePlayback
/// plays SE_ENEMY_CRY plus it, or one less (never below SE_ENEMY_CRY) when the
/// playback's flags are set.
enum DefeatSound {
  DEFEAT_SOUND_ENEMY_CRY,    ///< SE_ENEMY_CRY
  DEFEAT_SOUND_BEAST_CRY,    ///< SE_BEAST_CRY
  DEFEAT_SOUND_BOSS_ROAR,    ///< SE_BOSS_ROAR
  DEFEAT_SOUND_MONSTER_ROAR, ///< SE_MONSTER_ROAR
  DEFEAT_SOUND_UNUSED_CRY    ///< SE_UNUSED_DEFEAT_CRY; no class has it
};

/// An enemy class's battle sprite: its animation, size, attack effect,
/// palette and defeat-sound kind.
struct BattleSpriteDefinition {
  u16 animation; ///< the class's battle sprite
  /// The byte at 2.
  /// bits: 7..5 attackHop | 4..1 attackEffect | 0 isSmall
  ///@{
  u8 isSmall : 1;      ///< 1 for a small enemy, which takes one sprite slot
  u8 attackEffect : 4; ///< enum AttackEffect; 0 reads as ATTACK_EFFECT_SLASH
  u8 attackHop : 3;    ///< the attack-hop height
  ///@}
  /// The byte at 3.
  /// bits: 7..5 defeatSound | 4..1 palette | 0 heavyImpact
  ///@{
  u8 heavyImpact : 1; ///< 1 for an enemy whose beam blow lands heavily
  u8 palette : 4;     ///< the sprite's palette
  u8 defeatSound : 3; ///< enum DefeatSound; above 4 reads as none
  ///@}
  /// The height a battle effect is drawn at over the enemy, in pixels; 0 for
  /// its size's default.
  u8 effectHeight;
  u8 reserved[3]; ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct BattleSpriteDefinition) == 8);
/// Each enemy's battle sprite and how it attacks and falls, by enemy id.
extern struct BattleSpriteDefinition gBattleSprites[];

/// A summon: its name and the Djinn of each element it needs.
struct SummonDefinition {
  u16 abilityId;
  u8 reserved[2];      ///< 0 in every row; read by no code
  u8 djinnRequired[4]; ///< standby Djinn needed, by element
};

/// Returns a summon's definition, or NULL past the last.
const struct SummonDefinition *SummonDefinition_Get(s32 summonId);
s32 Enemy_GetFreeUnitId(void);
s32 Enemy_HasSpriteRoom(s32 classId);

const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId);
s32 BattleFormation_GetRandomMember(s32 recordId);
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 n);
UNREAD_RESULT BattleEnemy_ReleaseNameLetter(s32 unitId);
s32 BattleEnemy_LetterFirstOfKind(s32 classId);
void BattlePlacement_RelayoutEnemies(void);

#endif
