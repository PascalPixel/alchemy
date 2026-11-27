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

void fdSpriteSetAnimationColorModesStub(struct SPR *sprite, Uint32 colorMode);

void fdActorSetSpriteFlagsStub(ACTOR *actor, Uint32 flags);
void fdActorSetSpriteColorModeStub(ACTOR *actor, Uint32 colorMode);

extern const Sint32 ActorCameraScript[];
extern const Sint32 ActorScriptEndScript[];

#endif
