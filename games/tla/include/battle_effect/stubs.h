#ifndef ALCHEMY_BATTLE_EFFECT_STUBS_H
#define ALCHEMY_BATTLE_EFFECT_STUBS_H

/* The stub calls into the battle effect module, as the other modules' code
   calls them. */

#include "types.h"

struct BATTLE_PRESENTATION_STATE;
/* Runs a battle effect from the second range of kinds: the casting impacts, the
   plain attacks' beams and their critical hits. */
void btEffectRunByIdRangeStub(struct BATTLE_PRESENTATION_STATE *state);
/* Runs an ordinary battle effect from BattleEffectHandlers, with the sprite
   sheet, state and canvas it lends. */
void btEffectRunFromTableStub(struct BATTLE_PRESENTATION_STATE *state);
/* A pillar of light in an element's colours stands at a unit's feet while motes
   spiral up around it. */
void btEffectRunElementPillarStub(struct BATTLE_PRESENTATION_STATE *state);
/* The Doom Dragon's change of form once a form is beaten; formation says which
   (FORMATION_DOOM_DRAGON_A to C). */
void btEffectRunFormChangeStub(
	struct BATTLE_PRESENTATION_STATE *state, Sint32 formation);
/* Sparks in an element's colours flare at four corners around a unit and motes
   spiral in, then a burst of light plays over it. */
void btEffectRunElementBurstStub(struct BATTLE_PRESENTATION_STATE *state);
/* Starts the element streaks, which then draw by themselves while the battle
   runs on. */
void btEffectStartElementStreaksStub(Sint32 element);
/* Ends the element streaks: removes their events, clears their canvas in VRAM
   and frees what they took. */
void btEffectEndElementStreaksStub(void);
/* Loads a blitter for a 128 by 128 canvas into a heap slot, the flags and the
   mode packed into one word: mode << 4 | flags. */
Sint32 btEffectLoadBlitterStub(Sint32 slot, Sint32 modeAndFlg);

#endif
