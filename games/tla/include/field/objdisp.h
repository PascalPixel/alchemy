#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

#include "types.h"
#include "edition.h"

struct Actor;
struct Actor;

void *Actor_FindFree(void);
void Actor_Release(struct Actor *object);
void Actor_StartScriptFar(struct Actor *object, const s32 *script);

#endif
