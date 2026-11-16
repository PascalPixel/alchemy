#ifndef ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H
#define ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H

/// @file
/// The far calls into the field sequence bank, as the other banks' code calls
/// them.

#include "types.h"

struct Actor;
struct FieldParticle;
/// Returns the actor with actor id id, or NULL.
struct Actor *Actor_GetByIdFar(s32 id);
/// The main routine: runs one sequence after another and never returns.
void MainRoutine_RunFar(s32 mode);
/// The event command "wait for screen": waits as long as the transition runs.
void Event_WaitTransitionFramesFar(void);
/// The event command "open screen": starts the event's screen transition.
void Event_StartScreenTransitionFar(void);
/// Places a field effect sprite at (x, z), at rest with no target.
void FieldParticle_SetPositionFar(struct FieldParticle *, s32, s32);
/// Reports an unfinished target only for effects that stop on arrival.
s32 FieldParticle_HasPendingTargetFar(struct FieldParticle *);
/// Updates one field effect sprite: its age, callback, motion and picture.
void FieldParticle_UpdateFar(void *slot);
/// Starts slot effect as a particle of resource object kind at (x, z).
void FieldParticle_InitializeFar(struct FieldParticle *effect, s32 mode, s32 x,
                                 s32 z);
/// Gives a field effect sprite its per-frame callback, starting at state 0.
void FieldParticle_SetCallbackFar(struct FieldParticle *effect,
                                  void (*callback)(struct FieldParticle *));
/// Sets the animation of a field effect sprite.
void FieldParticle_SetActorModeFar(struct FieldParticle *effect, s32 value);
/// Makes the event leave for entrance of scene.
void Event_SetDestinationFar(s32 scene, s32 entrance);
/// Ends every lasting Psynergy.
void Event_ResolveAllPendingActionsFar(void);
/// Returns the voice sound of sprite id's messages, its definition's value plus
/// 0x100, or 0 for none (0xff) or with option 3 (gMainRoutineWork.speech) off.
s32 Speaker_GetVoiceFar(u16 id);
/// Frees a field effect sprite and its sprite, clearing it to zero.
void FieldEffect_ClearOwnedSlotFar(struct FieldParticle *);
/// Returns the battle background chosen for the current scene.
s32 BattleBackground_GetSelectedFar(void);

#endif
