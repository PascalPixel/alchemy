#ifndef ALCHEMY_FACING_OBJECT_H
#define ALCHEMY_FACING_OBJECT_H

#include "types.h"

/* Shared target-facing controller used by the small scene-object overlays. */
struct FacingObject {
    u8 unknown_00[6];
    u16 facing;
    s32 position_x;
    u8 unknown_0c[4];
    s32 position_z;
    u8 unknown_14[0x46];
    u8 facing_flags;
    u8 unknown_5b[9];
    /* This selector is passed to the resolver by resource 378; its identity is unknown. */
    s16 unknown_64;
    struct FacingObject *facing_target;
};

#define FACING_OBJECT_OFFSET(field) ((u32)&(((struct FacingObject *)0)->field))
typedef char FacingObject_size[sizeof(struct FacingObject) == 0x6c ? 1 : -1];
typedef char FacingObject_facing_offset[
    FACING_OBJECT_OFFSET(facing) == 0x06 ? 1 : -1
];
typedef char FacingObject_position_x_offset[
    FACING_OBJECT_OFFSET(position_x) == 0x08 ? 1 : -1
];
typedef char FacingObject_position_z_offset[
    FACING_OBJECT_OFFSET(position_z) == 0x10 ? 1 : -1
];
typedef char FacingObject_facing_flags_offset[
    FACING_OBJECT_OFFSET(facing_flags) == 0x5a ? 1 : -1
];
typedef char FacingObject_unknown_64_offset[
    FACING_OBJECT_OFFSET(unknown_64) == 0x64 ? 1 : -1
];
typedef char FacingObject_facing_target_offset[
    FACING_OBJECT_OFFSET(facing_target) == 0x68 ? 1 : -1
];
#undef FACING_OBJECT_OFFSET

#endif
