#ifndef ALCHEMY_OVERLAY_OBJECT_H
#define ALCHEMY_OVERLAY_OBJECT_H

#include "types.h"

/* Observed nested record used by the small overlay-object setup wrappers. */
struct OverlayObjectRecord {
    u8 unknown_00[9];
    u8 flags;
};

/*
 * Shared prefix evidenced by the overlay setup wrappers and the object
 * orientation update. Fields between observed offsets deliberately remain
 * unnamed.
 */
struct OverlayObject {
    u8 unknown_00[6];
    u16 angle;
    s32 coordinate_08;
    u8 unknown_0c[4];
    s32 coordinate_10;
    u8 unknown_14[0x3c];
    struct OverlayObjectRecord *record;
    u8 unknown_54;
    u8 unknown_55;
    u8 unknown_56[3];
    u8 unknown_59;
    u8 unknown_5a;
    u8 unknown_5b[13];
    struct OverlayObject *linked_object;
};

#define OVERLAY_OBJECT_OFFSET(field) ((u32)&(((struct OverlayObject *)0)->field))
typedef char OverlayObjectRecord_flags_offset[
    ((u32)&(((struct OverlayObjectRecord *)0)->flags)) == 0x09 ? 1 : -1
];
typedef char OverlayObject_record_offset[
    OVERLAY_OBJECT_OFFSET(record) == 0x50 ? 1 : -1
];
typedef char OverlayObject_angle_offset[
    OVERLAY_OBJECT_OFFSET(angle) == 0x06 ? 1 : -1
];
typedef char OverlayObject_coordinate_08_offset[
    OVERLAY_OBJECT_OFFSET(coordinate_08) == 0x08 ? 1 : -1
];
typedef char OverlayObject_coordinate_10_offset[
    OVERLAY_OBJECT_OFFSET(coordinate_10) == 0x10 ? 1 : -1
];
typedef char OverlayObject_unknown_55_offset[
    OVERLAY_OBJECT_OFFSET(unknown_55) == 0x55 ? 1 : -1
];
typedef char OverlayObject_unknown_59_offset[
    OVERLAY_OBJECT_OFFSET(unknown_59) == 0x59 ? 1 : -1
];
typedef char OverlayObject_unknown_5a_offset[
    OVERLAY_OBJECT_OFFSET(unknown_5a) == 0x5a ? 1 : -1
];
typedef char OverlayObject_linked_object_offset[
    OVERLAY_OBJECT_OFFSET(linked_object) == 0x68 ? 1 : -1
];
#undef OVERLAY_OBJECT_OFFSET

#endif
