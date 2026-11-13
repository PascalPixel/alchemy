#ifndef APOJII_ISLANDS_LOCAL_H
#define APOJII_ISLANDS_LOCAL_H

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"
#include "text/msg_ids.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
s32 VramCache_FindFreeEntry(void);
void Irq_SetHandler(s32 a, s32 b, void *callback);
void Actor_StartScript(struct Actor *object, const void *data);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Field_TryJumpForward(void);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Shop_Run(s32 a, s32 b);
void Inn_CheckIn(s32 a, s32 b);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                       s32 *params);
void Func_020003e8(void);
void Func_02000540(void);
void Func_020006bc(void);
void Func_02000770(struct Actor *object);
void Actor_FlickerChild(struct Actor *object);
void Func_02000974(void);
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Scroll_StartHBlank(void);
void Func_02001054(void);
void Func_020010b4(void);
void Func_020010d8(struct Actor *object);
s32 Actor_EmitMist(struct Actor *object);
void Leader_RiseFromWater(void);
void Func_020015f8(void);
void Func_020017bc(void);
void Func_020017f8(u32 base, s32 a, s32 b);
void GroundParticles_Draw(void);
void Func_02001ad8(void);
void Func_02001ba0(s32 id);

extern s32 gData_02002a98;
extern u16 gData_02002a9c;
extern u16 gData_02002a9e;
struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[128];
};

extern struct CellMarkers gData_02002aa0;
extern u32 gData_02001ee8[];
extern u32 gWaterDropScript[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  FieldCamera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void Func_02001d04_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_02001e0c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

#endif
