#ifndef ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD5_H
#define ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD5_H

#include "types.h"

struct ObjectRuntime;

void ObjectDispatch_SetUpdateValue(void *raw, s32 value);
u32 Field_StoreAssignedKeyValue(u32 value);
s32 Field_CheckConfiguredKeys(void);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
s32 FieldObject_UpdatePlayerControl(struct ObjectRuntime *object);
s32 Object_MoveOnWorldMap(struct ObjectRuntime *object);
s32 Field_CheckConfiguredKeysAndCount(void *work);
s32 Object_MoveByKeys(struct ObjectRuntime *object);

#endif
