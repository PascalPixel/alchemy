#ifndef ALCHEMY_FACING_OBJECT_H
#define ALCHEMY_FACING_OBJECT_H

#include "types.h"

struct FacingObject
{
    u8                   unk00[6];
    u16                  facing;
    s32                  positionX;
    u8                   unk0c[4];
    s32                  positionZ;
    u8                   unk14[0x46];
    u8                   facingFlags;
    u8                   unk5b[9];
    s16                  unk64;
    struct FacingObject *facingTarget;
};

#define FACING_OBJECT_OFFSET(field) ((u32)&(((struct FacingObject *)0)->field))
typedef char FacingObject_size[sizeof(struct FacingObject) == 0x6c ? 1 : -1];
typedef char FacingObject_facing_offset[FACING_OBJECT_OFFSET(facing) == 0x06 ? 1 : -1];
typedef char FacingObject_position_x_offset[FACING_OBJECT_OFFSET(positionX) == 0x08 ? 1 : -1];
typedef char FacingObject_position_z_offset[FACING_OBJECT_OFFSET(positionZ) == 0x10 ? 1 : -1];
typedef char FacingObject_facing_flags_offset[FACING_OBJECT_OFFSET(facingFlags) == 0x5a ? 1 : -1];
typedef char FacingObject_unknown_64_offset[FACING_OBJECT_OFFSET(unk64) == 0x64 ? 1 : -1];
typedef char FacingObject_facing_target_offset[FACING_OBJECT_OFFSET(facingTarget) == 0x68 ? 1 : -1];
#undef FACING_OBJECT_OFFSET

#endif
