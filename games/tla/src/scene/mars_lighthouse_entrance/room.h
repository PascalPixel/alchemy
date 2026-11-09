#ifndef TLA_MARS_LIGHTHOUSE_ENTRANCE_ROOM_H
#define TLA_MARS_LIGHTHOUSE_ENTRANCE_ROOM_H

#include "types.h"
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

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
  u8 unk0a[6];
  u8 unk10;
  u8 unk11;
  u16 unk12;
};

struct Box {
  s32 word[6];
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->eventWork)

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
u32 Random16Far(void);

s32 Func_0200246c(s32 index, s32 value);
s32 Func_0200249c(s32 index);
void Func_02004bd0(struct Actor *object, const void *script);
void Func_02004ca8(void);
void Func_02004cb0(void);
void Func_02004d20(void);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004db0(void);
void Func_02004bc0(struct Actor *object, s32 a1);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
s32 Math_Cosine(s32 angle);

void Func_02001e1c(struct Actor *object, s32 part);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Func_02004c28_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                  s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

static inline void Func_02004c30_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4,
                                  s32 a5) {
  Func_0802b450(a0, a1, a2, a3, a4, a5);
}

static inline void Func_02004cd8_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static inline struct Actor *Func_02004bd8_(s32 kind, s32 x, s32 y,
                                                   s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static inline void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) {
  Camera_MoveWithinMapBounds(x, y, z, use_setter);
}

#endif
