#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct DispatchObject;

void *ObjectDispatch_FindFreeObject(void);
void  ObjectDispatch_Release(struct DispatchObject *object);
#if GAME_TLA
void  ObjectDispatch_InitializeFar(struct DispatchObject *object, const s32 *script);
#endif

#endif
