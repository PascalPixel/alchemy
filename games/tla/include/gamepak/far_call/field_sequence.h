#ifndef ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H
#define ALCHEMY_FAR_CALL_FIELD_SEQUENCE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct Effect_080b2f4c;

struct Actor *Actor_GetByIdFar(s32 id);

void MainRoutine_RunFar(s32 mode);

void Event_WaitTransitionFramesFar(void);
void Event_StartScreenTransitionFar(void);

void FieldParticle_SetPositionFar(struct Effect_080b2f4c *, s32, s32);
s32 FieldParticle_HasPendingTargetFar(struct Effect_080b2f4c *);
void FieldParticle_UpdateFar(void *slot);
void FieldParticle_InitializeFar(struct Effect_080b2f4c *effect, s32 mode,
                                 s32 x, s32 z);
void FieldParticle_SetCallbackFar(struct Effect_080b2f4c *effect,
                                  void (*callback)(struct Effect_080b2f4c *));
void FieldParticle_SetActorModeFar(struct Effect_080b2f4c *effect, s32 value);

void Event_SetDestinationFar(s16 primary, s16 secondary);

#endif
