/// @file
/// What the Mars Lighthouse rooms with flame throwers share: the flag bytes of
/// the crack the leader falls through, and the routines they call.
#ifndef ALCHEMY_MARS_LIGHTHOUSE_ENTRANCE_ROOM_H
#define ALCHEMY_MARS_LIGHTHOUSE_ENTRANCE_ROOM_H

#include "fixed_math.h"
#include "types.h"
#include "flags.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "field/actor.h"
#include "field/flag_call.h"
#include "field/event_inline.h"
#include "field/map_grid.h"

/// The cell the leader fell through, its column and row, a byte of area flags
/// each: MarsLighthouseEntrance_FallThroughCrack keeps them, and the star room
/// below lands him under it.
#define FLAG_BYTE_FALL_COLUMN FLAG_AREA_TEMP_40
#define FLAG_BYTE_FALL_ROW FLAG_AREA_TEMP_48

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

s32 FlameThrower_GetFlags(s32 index);
void Actor_StartScript(struct Actor *actor, const void *script);
void ActorTable_Snapshot(void);
FIXED Trig_Cos(s32 angle);

/// The event commands MapGrid_CopyCellAttributes, MapGrid_CopyCellsLayered,
/// Actor_SetPositionFX, FieldActor_Create and FieldCamera_MoveToFX, under the
/// names the entrance's events still call them by; they compile alike.
static inline void Map_CopyCellAttributeRect_(s32 srcX, s32 srcY, s32 width,
                                              s32 height, s32 dstX, s32 dstY) {
  Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

static inline void MapGrid_CopyCellTilesCellsRect_(s32 srcX, s32 srcY,
                                                   s32 width, s32 height,
                                                   s32 dstX, s32 dstY) {
  MapGrid_CopyCellTilesCellsRect(srcX, srcY, width, height, dstX, dstY);
}

static inline void Actor_SetHorizontalPositionWithTerrain_(s32 id, FIXED x,
                                                           FIXED z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static inline struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static inline void FieldCamera_MoveWithinMapBounds_(FIXED x, FIXED y, FIXED z,
                                                    s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

#endif
