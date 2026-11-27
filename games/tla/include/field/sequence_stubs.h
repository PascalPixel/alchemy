#ifndef ALCHEMY_FIELD_SEQUENCE_STUBS_H
#define ALCHEMY_FIELD_SEQUENCE_STUBS_H

/* @file */
/* The stub calls into the field sequence module, as the other modules' code */
/* calls them. */

#include "types.h"

struct ACTOR;
struct FIELD_PARTICLE;
/* Returns actor actorId, or NULL if it does not exist. */
struct ACTOR *fdActorGetByIdStub(s32 actorId);
/* The main routine: runs one sequence after another and never returns. */
void gmMainRoutineRunStub(s32 mode);
/* The event command "wait for screen": waits as long as the transition runs. */
void fdEventWaitTransitionFramesStub(void);
/* The event command "open screen": starts the event's screen transition. */
void fdEventStartScrTransitionStub(void);
/* Places a field effect sprite at (x, z), at rest with no target. */
void fdParticleSetPosStub(struct FIELD_PARTICLE *effect, s32 x, s32 z);
/* Reports an unfinished target only for effects that stop on arrival. */
s32 fdParticleHasPendingTargetStub(struct FIELD_PARTICLE *effect);
/* Updates one field effect sprite: its age, callback, motion and picture. */
void fdParticleUpdateStub(void *effect);
/* Starts a field effect sprite with a sprite of the given kind at (x, z). */
void fdParticleInitStub(struct FIELD_PARTICLE *effect, s32 kind, s32 x, s32 z);
/* Gives a field effect sprite its per-frame callback, starting at state 0. */
void fdParticleSetCallbackStub(struct FIELD_PARTICLE *effect,
							   void (*callback)(struct FIELD_PARTICLE *));
/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimationStub(struct FIELD_PARTICLE *effect, s32 mode);
/* Makes the event leave for entrance of scene. */
void fdEventSetDestinationStub(s32 scene, s32 entrance);
/* Ends every lasting Psynergy. */
void fdEventEndAllLastingPsynergyStub(void);
/* Returns the voice of sprite id's messages, the first of its blip sounds, or
 */
/* 0 for none or with the Speech option off. */
s32 fdSpeakerGetVoiceStub(u16 id);
/* Frees a field effect sprite and its sprite, clearing it to zero. */
void fdEffectClearOwnedSlotStub(struct FIELD_PARTICLE *effect);
/* Returns the battle background chosen for the current scene. */
s32 btBackgroundGetSelectedStub(void);

#endif
