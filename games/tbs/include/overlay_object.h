#ifndef ALCHEMY_OVERLAY_OBJECT_H
#define ALCHEMY_OVERLAY_OBJECT_H

/// @file
/// Overlay objects and their records.

#include "types.h"
#include "static_assert.h"

struct OverlayObjectRecord {
  u8 unk00[9];
  u8 flags;
};

/// A field object as a Mercury Lighthouse beacon spark sees it: its position
/// and target, its angle round the flame and the flame it is linked to.
struct OverlayObject {
  u8 unk00[6];
  u16 angle;        ///< 1/65536 turns
  s32 coordinate08; ///< 16.16 fixed-point pixels
  u8 unk0c[4];
  s32 coordinate10; ///< 16.16 fixed-point pixels
  u8 unk14[0x1c];
  s32 field30;
  u8 unk34[4];
  s32 coordinate38; ///< 16.16 fixed-point pixels
  u8 unk3c[4];
  s32 coordinate40; ///< 16.16 fixed-point pixels
  u8 unk44[0x0c];
  struct OverlayObjectRecord *record;
  u8 unk54;
  u8 unk55;
  u8 unk56[3];
  u8 unk59;
  u8 steeringFlags;
  u8 unk5b[9];
  u16 angle64; ///< 1/65536 turns
  u8 unk66[2];
  struct OverlayObject *linkedObject;
};

STATIC_ASSERT(offsetof(struct OverlayObjectRecord, flags) == 0x09);
STATIC_ASSERT(offsetof(struct OverlayObject, record) == 0x50);
STATIC_ASSERT(offsetof(struct OverlayObject, angle) == 0x06);
STATIC_ASSERT(offsetof(struct OverlayObject, coordinate08) == 0x08);
STATIC_ASSERT(offsetof(struct OverlayObject, coordinate10) == 0x10);
STATIC_ASSERT(offsetof(struct OverlayObject, field30) == 0x30);
STATIC_ASSERT(offsetof(struct OverlayObject, coordinate38) == 0x38);
STATIC_ASSERT(offsetof(struct OverlayObject, coordinate40) == 0x40);
STATIC_ASSERT(offsetof(struct OverlayObject, unk55) == 0x55);
STATIC_ASSERT(offsetof(struct OverlayObject, unk59) == 0x59);
STATIC_ASSERT(offsetof(struct OverlayObject, steeringFlags) == 0x5a);
STATIC_ASSERT(offsetof(struct OverlayObject, angle64) == 0x64);
STATIC_ASSERT(offsetof(struct OverlayObject, linkedObject) == 0x68);

#endif
