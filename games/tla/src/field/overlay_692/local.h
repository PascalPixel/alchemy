#ifndef OVERLAY_692_LOCAL_H
#define OVERLAY_692_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "frame.h"

struct MapLayer {
    u8 unknown_00[0x30];
    u8 *cells;
    u8 unknown_34[4];
};

struct MapLayers {
    u8 unknown_000[0x108];
    struct MapLayer layers[3];
};

extern u8 gMapCellBuffer[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));

extern const u8 Data_02005534[];

extern s32 Data_02006710;

void Func_02001190(s32 a, s32 b);
void Func_020024ac(void);
void Func_02004e30(s32 a, s32 b);
void Func_02004e90(s32 a);
void Func_02004ea8(s32 a, s32 b, s32 c);
void Func_02004934(s32 a, s32 b, s32 owner);
void Func_02004c98(struct ObjectRuntime *object, s32 a);
void Func_02004ca0(struct ObjectRuntime *object, const void *data);
void Func_02004df8(s32 id);

static __inline__ struct ObjectRuntime *Create(s32 kind, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return Engine_ObjectCreate(kind, x, y, z);
}

#endif
