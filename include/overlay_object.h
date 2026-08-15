#ifndef ALCHEMY_OVERLAY_OBJECT_H
#define ALCHEMY_OVERLAY_OBJECT_H

#include "types.h"

/* Observed nested record used by the small overlay-object setup wrappers. */
struct OverlayObjectRecord {
    u8 unknown_00[9];
    u8 flags;
};

/* Shared prefix touched by the ten matching overlay setup wrappers. */
struct OverlayObject {
    u8 unknown_00[0x50];
    struct OverlayObjectRecord *record;
    u8 unknown_54;
    u8 unknown_55;
    u8 unknown_56[3];
    u8 unknown_59;
};

#define OVERLAY_OBJECT_OFFSET(field) ((u32)&(((struct OverlayObject *)0)->field))
typedef char OverlayObjectRecord_flags_offset[
    ((u32)&(((struct OverlayObjectRecord *)0)->flags)) == 0x09 ? 1 : -1
];
typedef char OverlayObject_record_offset[
    OVERLAY_OBJECT_OFFSET(record) == 0x50 ? 1 : -1
];
typedef char OverlayObject_unknown_55_offset[
    OVERLAY_OBJECT_OFFSET(unknown_55) == 0x55 ? 1 : -1
];
typedef char OverlayObject_unknown_59_offset[
    OVERLAY_OBJECT_OFFSET(unknown_59) == 0x59 ? 1 : -1
];
#undef OVERLAY_OBJECT_OFFSET

#endif
