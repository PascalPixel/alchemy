#ifndef ALCHEMY_UNIT_FIELD_COMMON_EFFECT_CREATE_H
#define ALCHEMY_UNIT_FIELD_COMMON_EFFECT_CREATE_H

#include "types.h"

struct FieldActor;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type);

#endif
