#ifndef ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H
#define ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H

/// @file
/// The far calls into the field sequence bank, as the other banks' code calls
/// them.

#include "types.h"

struct Actor;
struct FieldParticle;
struct Actor *Actor_GetByIdFar(s32 id);
void MainRoutine_RunFar(s32 mode);
void Event_WaitTransitionFramesFar(void);
void Event_StartScreenTransitionFar(void);
void FieldParticle_SetPositionFar(struct FieldParticle *, s32, s32);
/// Reports an unfinished target only for effects that stop on arrival.
s32 FieldParticle_HasPendingTargetFar(struct FieldParticle *);
void FieldParticle_UpdateFar(void *slot);
/// Starts slot effect as a particle of resource object kind at (x, z).
void FieldParticle_InitializeFar(struct FieldParticle *effect, s32 mode, s32 x,
                                 s32 z);
void FieldParticle_SetCallbackFar(struct FieldParticle *effect,
                                  void (*callback)(struct FieldParticle *));
void FieldParticle_SetActorModeFar(struct FieldParticle *effect, s32 value);
void Event_SetDestinationFar(s32 scene, s32 entrance);
void Event_ResolveAllPendingActionsFar(void);
/// Returns the voice sound of sprite id's messages, its definition's value plus
/// 0x100, or 0 for none (0xff) or with option 3 (gMainRoutineWork.speech) off.
s32 Speaker_GetVoiceFar(u16 id);
void FieldEffect_ClearOwnedSlotFar(struct FieldParticle *);
s32 BattleBackground_GetSelectedFar(void);

#endif
