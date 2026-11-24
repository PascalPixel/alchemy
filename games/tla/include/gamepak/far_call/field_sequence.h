#ifndef ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H
#define ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H

/// @file
/// The far calls into the field sequence bank, as the other banks' code calls
/// them.

#include "types.h"

struct Actor;
struct FieldParticle;
/// Returns actor actorId, or NULL if it does not exist.
struct Actor *Actor_GetByIdFar(s32 actorId);
/// The main routine: runs one sequence after another and never returns.
void MainRoutine_RunFar(s32 mode);
/// The event command "wait for screen": waits as long as the transition runs.
void Event_WaitTransitionFramesFar(void);
/// The event command "open screen": starts the event's screen transition.
void Event_StartScreenTransitionFar(void);
/// Places a field effect sprite at (x, z), at rest with no target.
void FieldParticle_SetPositionFar(struct FieldParticle *effect, s32 x, s32 z);
/// Reports an unfinished target only for effects that stop on arrival.
s32 FieldParticle_HasPendingTargetFar(struct FieldParticle *effect);
/// Updates one field effect sprite: its age, callback, motion and picture.
void FieldParticle_UpdateFar(void *effect);
/// Starts a field effect sprite with a sprite of the given kind at (x, z).
void FieldParticle_InitializeFar(struct FieldParticle *effect, s32 kind, s32 x,
                                 s32 z);
/// Gives a field effect sprite its per-frame callback, starting at state 0.
void FieldParticle_SetCallbackFar(struct FieldParticle *effect,
                                  void (*callback)(struct FieldParticle *));
/// Sets the animation of a field effect sprite.
void FieldParticle_SetAnimationFar(struct FieldParticle *effect, s32 mode);
/// Makes the event leave for entrance of scene.
void Event_SetDestinationFar(s32 scene, s32 entrance);
/// Ends every lasting Psynergy.
void Event_EndAllLastingPsynergyFar(void);
/// Returns the voice of sprite id's messages, the first of its blip sounds, or
/// 0 for none or with the Speech option off.
s32 Speaker_GetVoiceFar(u16 id);
/// Frees a field effect sprite and its sprite, clearing it to zero.
void FieldEffect_ClearOwnedSlotFar(struct FieldParticle *effect);
/// Returns the battle background chosen for the current scene.
s32 BattleBackground_GetSelectedFar(void);

#endif
