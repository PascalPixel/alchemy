#ifndef ALCHEMY_FACING_OBJECT_H
#define ALCHEMY_FACING_OBJECT_H

#include "types.h"
#include "layout_guard.h"

struct FacingObject {
  u8 unk00[6];
  u16 facing;
  s32 positionX;
  u8 unk0c[4];
  s32 positionZ;
  u8 unk14[0x46];
  u8 facingFlags;
  u8 unk5b[9];
  s16 unk64;
  struct FacingObject *facingTarget;
};

LAYOUT_SIZE_GUARD(FacingObject_size, struct FacingObject, 0x6c);
LAYOUT_OFFSET_GUARD(FacingObject_facing_offset, struct FacingObject, facing,
                    0x06);
LAYOUT_OFFSET_GUARD(FacingObject_position_x_offset, struct FacingObject,
                    positionX, 0x08);
LAYOUT_OFFSET_GUARD(FacingObject_position_z_offset, struct FacingObject,
                    positionZ, 0x10);
LAYOUT_OFFSET_GUARD(FacingObject_facing_flags_offset, struct FacingObject,
                    facingFlags, 0x5a);
LAYOUT_OFFSET_GUARD(FacingObject_unknown_64_offset, struct FacingObject, unk64,
                    0x64);
LAYOUT_OFFSET_GUARD(FacingObject_facing_target_offset, struct FacingObject,
                    facingTarget, 0x68);

#endif
