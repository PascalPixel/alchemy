#ifndef TLA_OVERLAY_686_OV686_H
#define TLA_OVERLAY_686_OV686_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "field_sprite.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
s32 Random16Far(void);

#endif
