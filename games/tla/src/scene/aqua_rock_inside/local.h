#ifndef AQUA_ROCK_INSIDE_LOCAL_H
#define AQUA_ROCK_INSIDE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"
#include "event_inline.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gActorPlacements[];
extern const struct Trigger gSceneEvents[];

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
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

extern u32 gFrameCount;

#include "map.h"

#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

struct PillarEntry {
  s32 x;
  s32 height;
  s32 z;
  s32 base;
  s16 kind;
  s16 state;
  s32 *position;
  u8 unk18[24];
};

struct PillarWork {
  u16 vramEntry;
  u16 count;
  struct PillarEntry entries[32];
};

struct BeamEntry {
  s32 x;
  s32 y;
  s32 z;
  s16 unk0c;
  s16 kind;
};

struct BeamWork {
  u16 vramEntry;
  u16 unk02;
  struct BeamEntry entries[2];
};

struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gData_020023c4[];
#define VENT_WORK ((struct VentWork *)(gData_020023c4 + 0x188))

struct VentRow {
  u8 unk00[8];
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

#define VENT_ROW(index) ((struct VentRow *)((u8 *)VENT_WORK + (index) * 20))

struct SceneEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[0xe];
  s16 value16a;
};

struct FieldSpriteFlags {
  u8 unk00[0x11];
  u8 flag0 : 1;
};

s32 Math_Cosine(s32 angle);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_02003c7c(const void *source, void *destination);
void Func_02003cc4(struct Actor *object, s32 mode);
void Func_02003ccc(struct Actor *object, const void *data);
void Func_02003cdc(void);
void Func_02003d54(void);
void Func_02003d64(s32 item, s32 mode);
void Func_02003d7c(void);
void Func_02003d84(void);
void Func_02003df4(s32 id, s32 a, s32 b);
void Func_02003e54(void);
void Func_02003e6c(void);
void Func_02003eb4(s32 a, s32 b);
void Func_02003ecc(s32 a);
void Func_02002bd4(struct Actor *object);
s32 Func_02002ed4(struct Actor *object);
void Func_02003ed4(void);

void Func_020006d0(void);
s32 Func_02000b24(s32 id, s32 value);
void Func_02000b5c(void);
void Func_02001a1c(void);
void Func_02001e78(s32 id);
void Func_020030f4(void);
void Func_02003460(s32 value);
void Func_020034fc(void);
void Func_02003628(void);
s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind);
s32 Func_020036b8(s32 a, s32 b);
void Func_0200389c(struct Actor *object);
void Func_02003994(struct Actor *object, s32 priority);
void Func_020039f0(s32 *position, s32 base);
s32 Func_02003bb8(s32 index, s32 value);
s32 Func_02003bcc(void);
s32 Func_02003bd8(s32 limit);

extern s32 gData_02004d84;
extern struct PillarWork gData_0200536c;
extern struct BeamWork gData_02005970;
extern const u8 gData_02003fd8[];
extern const u8 gData_020040dc[];
extern const u8 gData_0200411c[];
extern const u8 gData_02004128[];
extern const u8 gData_0200414c[];
extern const u8 gData_02004351[];
extern const u8 gData_020044c8[];

extern const u8 MsgCheckedTheColumn[];
extern const u8 MsgCheckedThePsynergy[];
extern const u8 MsgCheckedTheTabletIt[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x,
                                                             s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  Camera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void Func_02003d14_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

static __inline__ void Func_02003d1c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Func_02003d24_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Func_0802b450(a, b, c, d, e, f);
}

static __inline__ void *MemAlloc_(s32 size) {
  return Heap_Alloc(size);
}

#endif
