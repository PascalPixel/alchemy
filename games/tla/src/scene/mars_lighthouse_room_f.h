#ifndef TLA_MARS_LIGHTHOUSE_ROOM_F_ROOM_H
#define TLA_MARS_LIGHTHOUSE_ROOM_F_ROOM_H

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define U16_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct SceneWork {
  u8 unk000[0x166];
  u16 unk166;
  u8 unk168[0x44];
  s32 unk1ac;
  u8 unk1b0[0xb88];
  u8 unkd38;
};

struct MapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct MapLayers {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->eventWork)

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

s32 Func_020027ac(s32 index, s32 value);
s32 Func_020027dc(s32 index);
s32 Func_02004a8c(struct PushTarget *target);
void Func_02004d10(struct PushTarget target);
void Actor_StartScript(struct Actor *object, const void *script);
void Func_020055e0(void);
s32 Func_020055f8(s32 id);
void Func_02005600(void);
void ObjectTable_Snapshot(void);
struct Actor *Func_02005680(s32 *position, s32 mode);
void Func_02005688(void);
void Func_02005480(struct Actor *object, s32 mode);
void Actor_SetBlendMode(struct Actor *object, u32 value);
s32 Math_Cosine(s32 angle);

void Func_020001f4(s32 unused, s32 id);
void Func_0200215c(struct Actor *object, s32 part);
void Func_020002ec(void);
void Func_020003b4(void);
void Func_0200055c(s32 id);
void Func_02001448(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Func_020054e0_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                  s32 a5) {
  Map_CopyCellsRect(a0, a1, a2, a3, a4, a5);
}

static inline void Func_020054e8_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                  s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

static inline void Func_020054f0_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                  s32 a5) {
  MapGrid_CopyCellPictureCellsRect(a0, a1, a2, a3, a4, a5);
}

static inline void Func_02005598_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static inline struct Actor *Func_02005498_(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static inline void Vector_AddPolar(s32 length, s32 angle, s32 *position) {
  Vector_AddPolarOffsetFar(length, angle, position);
}

static inline void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) {
  FieldCamera_MoveWithinMapBounds(x, y, z, use_setter);
}

#endif
