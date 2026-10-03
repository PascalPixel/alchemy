#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

#include "TYPES.H"
#include "BATTLE_SESSION.H"

/*
 * Shared 0x70-byte object used by the motion engine.
 * Only fields whose roles are established by multiple callers are named.
 */
struct MotionObject {
    u8 unknown_00[0x06];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x04];
    s32 scale_x;
    s32 scale_y; /* negated to draw the object upside down */
    u16 height;
    u8 unknown_22[0x02];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 vertical_motion_phase;
    s32 vertical_motion_strength;
    u8 unknown_4c[0x04];
    void *records;
    u8 record_storage_kind;
    u8 motion_flags;
    u8 unknown_56[0x02];
    u8 snap_to_target;
    u8 unknown_59;
    u8 auto_face_motion;
    u8 unknown_5b[0x15];
};

/* The fields battle uses at the start of a 24-byte animation entry. */
struct SpriteEntry {
    u8 unknown_00[5];
    u8 palette;
    u8 priority;
};

/* Raw linker spellings stay here; reconstructed APIs are used by C bodies. */
void ReleaseBattleObjectRecords(s32 object_id);
void ResetMotionRecordGroup(void *record_group);
void ResetBattleObjectRecordGroups(struct MotionObject *object);

#define MOTION_OBJECT_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))

typedef char MotionObject_size[
    sizeof(struct MotionObject) == 0x70 ? 1 : -1
];
typedef char MotionObject_scale_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, scale_x) == 0x18 ? 1 : -1
];
typedef char MotionObject_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, x) == 0x08 ? 1 : -1
];
typedef char MotionObject_velocity_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, velocity_x) == 0x24 ? 1 : -1
];
typedef char MotionObject_target_x_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, target_x) == 0x38 ? 1 : -1
];
typedef char MotionObject_motion_flags_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, motion_flags) == 0x55 ? 1 : -1
];
typedef char MotionObject_records_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, records) == 0x50 ? 1 : -1
];
typedef char MotionObject_record_storage_kind_offset[
    MOTION_OBJECT_OFFSET(struct MotionObject, record_storage_kind) == 0x54 ? 1 : -1
];
LAYOUT_SIZE_GUARD(SpriteEntry_PrefixSize, struct SpriteEntry, 0x08);
LAYOUT_OFFSET_GUARD(SpriteEntry_Palette, struct SpriteEntry, palette, 0x05);
LAYOUT_OFFSET_GUARD(SpriteEntry_Priority, struct SpriteEntry, priority, 0x06);

#undef MOTION_OBJECT_OFFSET

s32 ActivateBattleObjectSlot(s32 object_id);
struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 object_id);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);

#endif
