#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

#include "TYPES.H"

struct DispatchObject;

void *ObjectDispatch_FindFreeObject(void);
void ObjectDispatch_Release(struct DispatchObject *object);
void ObjectDispatch_InitializeFar(struct DispatchObject *object, u32 value);

#endif
