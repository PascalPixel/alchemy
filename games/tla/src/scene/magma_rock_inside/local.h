#ifndef MAGMA_ROCK_INSIDE_LOCAL_H
#define MAGMA_ROCK_INSIDE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "frame.h"

struct MapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct MapLayers {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

extern u8 gMapCellBuffer[];

extern char SceneId_Ov692Room1;
extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room4;
extern char SceneId_Ov692Room5;
extern char SceneId_Ov692Room6;
extern char SceneId_Ov692Room7;
extern char SceneId_Ov692Room8;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Task_Add(void (*callback)(void), s32 priority);
void Task_RemoveFar(void (*callback)(void));

extern const u8 gData_02005534[];

extern s32 gData_02006710;

void Func_02001190(s32 a, s32 b);
void Func_020024ac(void);
void Func_02004e30(s32 a, s32 b);
void Func_02004e90(s32 a);
void Func_02004ea8(s32 a, s32 b, s32 c);
void Func_02004934(s32 a, s32 b, s32 owner);
void Func_02004c98(struct ObjectRuntime *object, s32 a);
void Func_02004ca0(struct ObjectRuntime *object, const void *data);
void Func_02004df8(s32 id);

static __inline__ struct ObjectRuntime *Create(s32 kind, s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Import_ActorCreate(kind, x, y, z);
}

void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);

static __inline__ void ObjectMotion_ResetAndSetPositionInMode2_(s32 id, s32 x,
                                                                s32 z) {
  /* FAKEMATCH: forwarding through the inline sets x between the two halves of
   * z, as the native does; called directly, z is built whole first (differ 2).
   */
  ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

#endif
