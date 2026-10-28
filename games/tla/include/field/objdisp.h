#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

#include "types.h"
#include "edition.h"

struct DispatchObject;

void *Actor_FindFreeObject(void);
void Actor_Release(struct DispatchObject *object);
void Actor_StartScriptFar(struct DispatchObject *object, const s32 *script);

#endif
