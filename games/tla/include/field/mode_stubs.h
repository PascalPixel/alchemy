#ifndef ALCHEMY_FIELD_MODE_STUBS_H
#define ALCHEMY_FIELD_MODE_STUBS_H

/* The stub calls into the field mode module, as the other modules' code calls
   them. */

#include "types.h"

struct ACTOR;
struct FIELD_PARTICLE;
/* Returns actor actorId, or NULL if it does not exist. */
struct ACTOR *fdActorGetByIdStub(Sint32 actorId);
/* The main routine: runs one mode after another and never returns. */
void gmMainRoutineRunStub(Sint32 start);
/* The script command "wait for screen": waits as long as the transition
   runs. */
void fdScriptWaitTransitionTicksStub(void);
/* The script command "open screen": starts the script's screen transition. */
void fdScriptStartScreenTransitionStub(void);
/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPosStub(struct FIELD_PARTICLE *effect, Sint32 x, Sint32 z);
/* Reports an unfinished target only for effects that stop on arrival. */
Sint32 fdParticleHasPendingTargetStub(struct FIELD_PARTICLE *effect);
/* Updates one field effect sprite: its age, callback, motion and picture. */
void fdParticleUpdateStub(void *effect);
/* Starts a field effect sprite with a sprite of the given kind at (x, z). */
void fdParticleInitStub(
	struct FIELD_PARTICLE *effect, Sint32 kind, Sint32 x, Sint32 z);
/* Gives a field effect sprite its per-tick callback, starting at state 0. */
void fdParticleSetCallbackStub(
	struct FIELD_PARTICLE *effect, void (*callback)(struct FIELD_PARTICLE *));
/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimationStub(struct FIELD_PARTICLE *effect, Sint32 mode);
/* Makes the script leave for entrance of scene. */
void fdScriptSetDestinationStub(Sint32 scene, Sint32 entrance);
/* Ends every lasting Psynergy. */
void fdScriptEndAllLastingPsynergyStub(void);
/* Returns the voice of sprite id's messages, the first of its blip sounds, or 0
   for none or with the Speech option off. */
Sint32 fdSpeakerGetVoiceStub(Uint16 id);
/* Frees a field effect sprite and its sprite, clearing it to zero. */
void fdEffectClearOwnedSlotStub(struct FIELD_PARTICLE *effect);
/* Returns the battle background chosen for the current scene. */
Sint32 fdBattleBackgroundGetSelectedStub(void);

#endif
