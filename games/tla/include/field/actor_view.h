#ifndef ALCHEMY_ACTOR_VIEW_H
#define ALCHEMY_ACTOR_VIEW_H

/* @file */
/* Starting an actor's sprite animations and setting its sprite's colour mode */
/* and priority. */

#include "types.h"
#include "system.h"
#include "field/actor.h"

struct SPR;
struct SPR_ANIMATION;

struct FIELD_SPR;

void fdSprSetAnimationColModesStub(struct SPR *sprite, u32 colorMode);

void fdActorSetSprFlagsStub(ACTOR *actor, u32 flags);
void fdActorSetSprColModeStub(ACTOR *actor, u32 colorMode);

extern const s32 Actor_CameraScript[];
extern const s32 ActorScript_EndScript[];

#endif
