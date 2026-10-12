#ifndef TLA_OVERLAY_6A3_OV6A3_H
#define TLA_OVERLAY_6A3_OV6A3_H

#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Func_020040b4(void);
struct ObjectRuntime *Func_020048fc(void);

#endif
