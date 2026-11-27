#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

/* @file */
/* Finding a free actor, releasing one and starting its script. */

#include "types.h"
#include "edition.h"

struct ACTOR;

void fdActorStartScriptStub(struct ACTOR *actor, const s32 *script);

#endif
