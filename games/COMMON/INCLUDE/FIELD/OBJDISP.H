#ifndef ALCHEMY_OBJDISP_H
#define ALCHEMY_OBJDISP_H

struct DispatchObject;

void *ObjectDispatch_FindFreeObject(void);
void ObjectDispatch_Release(struct DispatchObject *object);

#endif
