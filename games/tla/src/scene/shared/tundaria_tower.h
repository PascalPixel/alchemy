/// @file
/// Declarations Tundaria Tower shares with Dehkan Plateau and Magma Rock's
/// inside, whose pillars and rubble it copies: the field calls their overlays
/// make, some through their own import rows.
#ifndef ALCHEMY_TUNDARIA_TOWER_H
#define ALCHEMY_TUNDARIA_TOWER_H

#include "fixed_math.h"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/field_effect.h"

#include "field/scene_tables.h"

void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 dstX, s32 dstY,
                                        s32 width, s32 height);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY);
/// Stops the leader where it stands (Leader_StopMoving, through the overlay's
/// own import row).
void Import_LeaderStopMoving(void);
void Event_ResolvePendingActions(s32 skip);

void Rock_Spawn(s32 x, s32 y, s32 z, s32 frames);

void MapGrid_SetScreenShake(FIXED horizontal, FIXED vertical, FIXED decay);
void PaletteFade_SetTarget(s32 color, s32 mode);
void PaletteFade_SetSource(s32 color, s32 mode);

/// PaletteFade_SetSource, under the name the tower's fades call it by.
static inline void TundariaTower_FadeFrom(s32 color, s32 mode) {
  PaletteFade_SetSource(color, mode);
}

#endif
