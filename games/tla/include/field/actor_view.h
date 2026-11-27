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

void fdSprSetAnimationColModesStub(struct SPR *spr, Uint32 colMode);

void fdActorSetSprFlagsStub(ACTOR *actor, Uint32 flg);
void fdActorSetSprColModeStub(ACTOR *actor, Uint32 colMode);

extern const Sint32 ActorCameraScript[];
extern const Sint32 ActorScriptEndScript[];

#endif
