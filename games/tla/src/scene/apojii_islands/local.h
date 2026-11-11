#ifndef APOJII_ISLANDS_LOCAL_H
#define APOJII_ISLANDS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"
#include "event_inline.h"

extern const u32 gSceneExits[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Random16Far(void);
s32 Resource_FindFreeEntry(void);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Map_GetTerrainHeight(s32 x, s32 z);
void Func_02001bf4(void *callback);
void Func_02001bfc(void *callback);
void Func_02001c0c(s32 a, s32 b, void *callback);
void Func_02001c24(const void *source, void *destination);
void Func_02001c6c(struct Actor *object, s32 mode);
void Func_02001c74(struct Actor *object, const void *data);
s32 Func_02001c9c(struct Actor *object, s32 *position);
void Func_02001cc4(void);
void Func_02001ccc(void);
void Func_02001d2c(s32 id, s32 a, s32 b);
void Func_02001d8c(void);
void Func_02001d9c(void);
void Func_02001de4(void);
void Func_02001dec(s32 a);
void Func_02001df4(void);
void Func_02001dfc(void);
void Func_02001e04(s32 a, s32 b, s32 c, s32 d);
void Func_02001e0c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_02001e1c(s32 a, s32 b);
void Func_02001e24(s32 a, s32 b);

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);
void Func_020003e8(void);
void Func_02000540(void);
void Func_020006bc(void);
void Func_02000770(struct Actor *object);
void Func_02000780(struct Actor *object);
void Func_02000974(void);
void Func_02000ef8(void);
void Func_02000f58(void);
void Func_02000f98(void);
void Func_02001054(void);
void Func_020010b4(void);
void Func_020010d8(struct Actor *object);
s32 Func_020013d8(struct Actor *object);
void Func_02001500(void);
void Func_020015f8(void);
void Func_020017bc(void);
void Func_020017f8(s32 a, s32 b, s32 c);
void Func_02001940(void);
void Func_02001ad8(void);
void Func_02001ba0(s32 id);

extern s32 gData_02002a98;
extern u16 gData_02002a9c;
extern u16 gData_02002a9e;
extern u16 gData_02002aa0[];
extern const u8 gData_02001ee8[];
extern const u8 gData_02001ef4[];
extern const u8 gField_Tiles679[];
extern u32 gFrameCount;

extern u8 MsgWhatWhatJust;
extern u8 MsgWhyDoesMy;
extern u8 MsgICanTEver;
extern u8 MsgWowYouLook;
extern u8 MsgYouGuysAren;
extern u8 MsgItDBe;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x,
                                                             s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  Camera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void Func_02001d04_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_02001e0c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02001e0c(a, b, c, d, e, f, g);
}

#endif
