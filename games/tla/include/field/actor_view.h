#ifndef ALCHEMY_ACTOR_VIEW_H
#define ALCHEMY_ACTOR_VIEW_H

/* Starting an actor's sprite animations and setting its sprite's colour mode
   and priority. */

#include "types.h"
#include "system.h"
#include "field/actor.h"

struct SPR;
struct SPR_ANIMATION;

struct FIELD_SPR;

void fdSprSetAnimationColModesStub(struct SPR *sprite, Uint32 colorMode);

void fdActorSetSprFlagsStub(ACTOR *actor, Uint32 flags);
void fdActorSetSprColModeStub(ACTOR *actor, Uint32 colorMode);

extern const Sint32 Actor_CameraScript[];
extern const Sint32 ActorScript_EndScript[];

#endif
