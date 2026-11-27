#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

/* Each enemy's battle sprite and how it attacks and falls, the summons, and the
   enemies called into a battle. */

#include "static_assert.h"

#include "types.h"
#include "edition.h"

/* A class's plain attack (BATTLE_SPR_DEFINITION.attackEffect), which
   btEffectRunByIdRange plays: btEffectRunAttackSlash's beam from the attacker
   and the target's recoil, or the canvas burst. */
typedef enum ATTACK_EFFECT {
	ATTACK_EFFECT_SLASH = 1,	/* a blue arc; the target is nudged */
	ATTACK_EFFECT_CLAW,			/* three blue arcs; the target is nudged */
	ATTACK_EFFECT_THRUST,		/* a beam reaching further each picture */
	ATTACK_EFFECT_WAVE,			/* a wave, played backwards */
	ATTACK_EFFECT_RISING_SLASH, /* the arc upside down; the target hops */
	/* As ATTACK_EFFECT_RISING_SLASH, drawn 16 pixels lower. */
	ATTACK_EFFECT_RISING_SLASH_LOW,
	ATTACK_EFFECT_HURL,	 /* no beam; the target is hurled (KNOCKBACK_HURL) */
	ATTACK_EFFECT_SHOVE, /* no beam; the target is pushed to 150 percent */
	ATTACK_EFFECT_BLOW,	 /* no beam; the target is nudged */
	ATTACK_EFFECT_BURST	 /* btEffectRunBallAttack's small ball */
} ATTACK_EFFECT;

/* A fallen class's cry (BATTLE_SPR_DEFINITION.defeatSound): BattlePlayback
   plays SE_ENEMY_CRY plus it, or one less (never below SE_ENEMY_CRY) when the
   playback's flags are set. */
typedef enum DEFEAT_SND {
	DEFEAT_SOUND_ENEMY_CRY,	   /* SE_ENEMY_CRY */
	DEFEAT_SOUND_BEAST_CRY,	   /* SE_BEAST_CRY */
	DEFEAT_SOUND_BOSS_ROAR,	   /* SE_BOSS_ROAR */
	DEFEAT_SOUND_MONSTER_ROAR, /* SE_MONSTER_ROAR */
	DEFEAT_SOUND_UNUSED_CRY	   /* SE_UNUSED_DEFEAT_CRY; no class has it */
} DEFEAT_SND;

/* An enemy class's battle sprite: its animation, size, attack effect, palette
   and defeat-sound kind. */
typedef struct BATTLE_SPR_DEFINITION {
	Uint16 animation; /* the class's battle sprite */
	/* The byte at 2. bits: 7..5 attackHop | 4..1 attackEffect | 0 isSmall */
	Uint8 isSmall : 1; /* 1 for a small enemy, which takes one sprite slot */
	Uint8 attackEffect : 4; /* ATTACK_EFFECT; 0 reads as ATTACK_EFFECT_SLASH */
	Uint8 attackHop : 3;	/* the attack-hop height */
	/* The byte at 3. bits: 7..5 defeatSound | 4..1 palette | 0 heavyImpact */
	Uint8 heavyImpact : 1; /* 1 for an enemy whose beam blow lands heavily */
	Uint8 palette : 4;	   /* the sprite's palette */
	Uint8 defeatSound : 3; /* DEFEAT_SND; above 4 reads as none */
	/* The height a battle effect is drawn at over the enemy, in pixels; 0 for
	   its size's default. */
	Uint8 effectHeight;
	Uint8 reserved[3]; /* 0 in every row */
} BATTLE_SPR_DEFINITION;

STATIC_ASSERT(sizeof(BATTLE_SPR_DEFINITION) == 8);
/* Each enemy's battle sprite and how it attacks and falls, by enemy id. */
extern BATTLE_SPR_DEFINITION BattleSprites[];

/* A summon: its name and the Djinn of each element it needs. */
typedef struct SUMMON_DEFINITION {
	Uint16 abilityId;
	Uint8  reserved[2];		 /* 0 in every row; read by no code */
	Uint8  djinnRequired[4]; /* standby Djinn needed, by element */
} SUMMON_DEFINITION;

/* Returns a summon's definition, or NULL past the last. */
const SUMMON_DEFINITION *gmSummonDefinitionGetNearStub(Sint32 summonId);
Sint32					 btEnemyGetFreeUnitId(void);
Sint32					 btEnemyHasSpriteRoom(Sint32 classId);

Sint32		  btFormationGetRandomMember(Sint32 recordId);
Sint32		  btEnemyClaimNameLetter(Sint32 classId, Sint32 n);
UNREAD_RESULT btEnemyReleaseNameLetter(Sint32 unitId);
Sint32		  btEnemyLetterFirstOfKind(Sint32 classId);
void		  btPlacementRelayoutEnemies(void);

#endif
