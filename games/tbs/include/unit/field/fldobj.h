#ifndef ALCHEMY_UNIT_FIELD_FLDOBJ_H
#define ALCHEMY_UNIT_FIELD_FLDOBJ_H

#include "types.h"

struct DispatchObject;
struct ObjectRuntime;

s32 Map_GetScreenRelativePosition(struct ObjectRuntime *obj, s32 *out);
void ObjectSystem_Initialize(s32 mode);
void ResourceTable_ReservedNoOpC0C4(void);
void ResourceTable_ReservedNoOp2(void);
void *ObjectDispatch_FindFreeObject(void);
void ObjectDispatch_Release(struct DispatchObject *obj);
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z);
void ObjectDispatch_Initialize(struct DispatchObject *obj, const s32 *script);
void ObjectDispatch_ApplyPairToChildren(struct DispatchObject *obj, s32 no, s32 steps);
void ObjectDispatch_SetChildRotation(struct DispatchObject *obj, u32 rot);
void Animation_SetIndexAndInitObjects(void *raw, s32 no);
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *obj, s32 no);
void ObjectDispatch_InitFromTable5WithArgument(struct DispatchObject *obj, s32 arg);
void ObjectDispatch_InitFromTable0(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable1(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable2(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable3(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable6(struct DispatchObject *obj);
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *obj, s32 arg);
void ObjectDispatch_WaitForValue16(void *raw);
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *obj, u32 flags);
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *obj, u32 mode);
void ObjectDispatch_SetChildDisplayFlag1(struct DispatchObject *obj, u32 on);
void Animation_ApplyChildValues(struct DispatchObject *obj, u32 val);
void Graphics_EnableObjLayerAndCallbacks(void);
void ObjectDispatch_StopCallbacksAndHideLayers(void);
s32 ObjectDispatch_ReturnTrue(void);
void ObjectSystem_UpdateCamera(void);
s32 ObjectCamera_ReturnTrue(void);
void ObjectSystem_UpdateCameraFixed(void);

#endif
