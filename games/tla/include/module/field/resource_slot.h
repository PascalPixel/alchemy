#ifndef ALCHEMY_MODULE_FIELD_RESOURCE_SLOT_H
#define ALCHEMY_MODULE_FIELD_RESOURCE_SLOT_H

/// @file
/// What the resource_slot object defines for the code outside it.

#include "types.h"

struct Actor;
struct AnimationEntry;

struct MetadataSlotState {
  u8 unk00[0x0c];
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unk18[3];
  u8 count;
  u8 unk1c[0x0c];
  s32 slots[4];
};

struct ResourceObjectWork {
  u8 unk00[16];
  u8 resource; ///< VRAM cache index.
  u8 flags;    ///< Bit 0: do not return the VRAM cache entry.
  u8 unk12[22];
  void *children[4];
};

s32 Party_GetPpRatioFar(void);
extern char ResourceId_Map000;
extern const u8 Transform_UpdateVerticesCode[];
s32 MapGrid_IsClimbableCellFar(s32 x, s32 z, s32 kind);
extern const u16 gMaps[][6];
s32 Actor_GetCellIndexFar(struct Actor *object);
s32 FlagBlock_PlaceAllMarkingEdgesFar(struct Actor *object);
s32 Actor_CheckBlockedFar(s32 arg);
void SpriteAnimation_Reset(struct AnimationEntry *work);

#endif
