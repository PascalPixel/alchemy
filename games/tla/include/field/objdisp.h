#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

/// @file
/// Finding a free actor, releasing one and starting its script.

#include "types.h"
#include "edition.h"

struct Actor;

void *Actor_FindFree(void);
void Actor_Release(struct Actor *actor);
void Actor_StartScriptFar(struct Actor *actor, const s32 *script);

#endif
