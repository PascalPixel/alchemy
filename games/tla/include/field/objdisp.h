#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

#include "types.h"
#include "edition.h"

struct ActorView;

void *Actor_FindFreeObject(void);
void Actor_Release(struct ActorView *object);
void Actor_StartScriptFar(struct ActorView *object, const s32 *script);

#endif
