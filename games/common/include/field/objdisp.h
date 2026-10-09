#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

#include "types.h"

struct DispatchObject;

void *ObjectDispatch_FindFreeObject(void);
void  ObjectDispatch_Release(struct DispatchObject *object);
void  ObjectDispatch_InitializeFar(struct DispatchObject *object, const s32 *script);

#endif
