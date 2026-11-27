#ifndef ALCHEMY_BATTLE_SPRITES_H
#define ALCHEMY_BATTLE_SPRITES_H

/* Summons and the battle sprite definitions of the enemies. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "stubs.h"

/* A unit's row of BattleSprites: an enemy's battle sprite, size, attack effect,
   palette and defeat-sound kind. */
typedef struct BATTLE_SPR_DEFINITION {
	Uint16 animation;	/* the enemy's battle sprite */
	Uint8  isSmall : 1; /* 1 for a small enemy, which takes one sprite slot */
	/* The beam its attack plays, an BEAM_KIND (battle_effect_beams.c); 0 reads
	   as 1. */
	Uint8 attackEffect : 4;
	/* how high it hops as it runs up to attack, in pixels */
	Uint8 attackHop : 3;
	Uint8 heavyImpact : 1; /* 1 for an enemy whose beam blow lands heavily */
	Uint8 palette : 4;	   /* the sprite's palette */
	Uint8 defeatSound : 3; /* the defeat-sound kind; above 4 reads as none */
	/* The height a battle effect is drawn at over the enemy, in pixels; 0 for
	   its size's default. */
	Uint8 effectHeight;
	Uint8 reserved[3]; /* 0 in every row */
} BATTLE_SPR_DEFINITION;

STATIC_ASSERT(sizeof(BATTLE_SPR_DEFINITION) == 8);
/* The units' battle sprite rows, by unit id 0 to 171: the eight characters',
   then the enemies' from ENEMY_ID_FIRST (battle_sprites.c), read by the
   getters in battle_sprite.c. */
extern const BATTLE_SPR_DEFINITION BattleSprites[];

/* A summon: its name's message and the Djinn of each element it needs. */
typedef struct SUMMON_DEFINITION {
	/* The summon's ability id: its name is message MsgAbilityName plus it, and
	   its low 14 bits pick the icon. */
	Uint16 abilityId;
	Uint8  reserved[2];		 /* 0 in every row; read by no code */
	Uint8  djinnRequired[4]; /* standby Djinn needed, by element */
} SUMMON_DEFINITION;

#endif
