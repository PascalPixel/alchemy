#ifndef ALCHEMY_FIELD_RESOURCE_SLOT_H
#define ALCHEMY_FIELD_RESOURCE_SLOT_H

/// @file
/// What the resource_slot object defines for the code outside it.

#include "types.h"
#include "resource_ids.h"

struct Actor;
struct SpriteAnimation;

struct MetadataSlotState {
  u8 unused[12]; ///< no code names it
  s32 scale;
  u8 unusedAfterScale[4]; ///< no code names it
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unusedAfterOffsetY[3]; ///< no code names it
  u8 count;
  u8 unusedAfterCount[12]; ///< no code names it
  s32 slots[4];
};

struct ResourceObjectWork {
  u8 unused[16];           ///< no code names it
  u8 resource;             ///< VRAM cache index.
  u8 flags;                ///< Bit 0: do not return the VRAM cache entry.
  u8 unusedAfterFlags[22]; ///< no code names it
  void *children[4];
};

s32 Party_GetPpRatioFar(void);
extern const u8 Matrix_UpdateVerticesCode[];
s32 MapGrid_IsClimbableCellFar(s32 x, s32 z, s32 kind);
s32 Actor_GetCellIndexFar(struct Actor *object);
s32 FlagBlock_PlaceAllMarkingEdgesFar(struct Actor *object);
s32 Actor_CheckBlockedFar(s32 arg);
void SpriteAnimation_Reset(struct SpriteAnimation *work);

#endif
