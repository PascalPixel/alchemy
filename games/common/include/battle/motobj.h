#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

#include "types.h"
#include "battle_session.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct MotionObject
{
    u8    unk00[0x06];
    u16   angle;
    s32   x;
    s32   y;
    s32   z;
    u8    unk14[0x04];
    s32   scaleX;
    s32   scaleY;
    u16   height;
    u8    unk22[0x02];
    s32   velocityX;
    s32   velocityY;
    s32   velocityZ;
    s32   speedLimit;
    s32   acceleration;
    s32   targetX;
    s32   targetY;
    s32   targetZ;
    s32   verticalMotionPhase;
    s32   verticalMotionStrength;
    u8    unk4c[0x04];
    void *records;
    u8    recordStorageKind;
    u8    motionFlags;
    u8    unk56[0x02];
    u8    snapToTarget;
    u8    unk59;
    u8    autoFaceMotion;
    u8    unk5b[0x11];
    void (*update)();
};

struct SpriteEntry
{
    u8 unk00[5];
    u8 palette;
    u8 priority;
};

#if GAME_TLA
void ReleaseBattleObjectRecords(s32 objectId);
void ResetMotionRecordGroup(void *record_group);
void ResetBattleObjectRecordGroups(struct MotionObject *object);
#endif

#define MOTION_OBJECT_OFFSET(type, field) ((u32) & (((type *)0)->field))

typedef char MotionObject_size[sizeof(struct MotionObject) == 0x70 ? 1 : -1];
typedef char
    MotionObject_scale_x_offset[MOTION_OBJECT_OFFSET(struct MotionObject, scaleX) == 0x18 ? 1 : -1];
typedef char MotionObject_x_offset[MOTION_OBJECT_OFFSET(struct MotionObject, x) == 0x08 ? 1 : -1];
typedef char MotionObject_velocity_x_offset
    [MOTION_OBJECT_OFFSET(struct MotionObject, velocityX) == 0x24 ? 1 : -1];
typedef char MotionObject_target_x_offset[MOTION_OBJECT_OFFSET(struct MotionObject, targetX) == 0x38
                                              ? 1
                                              : -1];
typedef char MotionObject_motion_flags_offset
    [MOTION_OBJECT_OFFSET(struct MotionObject, motionFlags) == 0x55 ? 1 : -1];
typedef char MotionObject_records_offset[MOTION_OBJECT_OFFSET(struct MotionObject, records) == 0x50
                                             ? 1
                                             : -1];
typedef char MotionObject_record_storage_kind_offset
    [MOTION_OBJECT_OFFSET(struct MotionObject, recordStorageKind) == 0x54 ? 1 : -1];
typedef char
    MotionObject_update_offset[MOTION_OBJECT_OFFSET(struct MotionObject, update) == 0x6c ? 1 : -1];
LAYOUT_SIZE_GUARD(SpriteEntry_PrefixSize, struct SpriteEntry, 0x08);
LAYOUT_OFFSET_GUARD(SpriteEntry_Palette, struct SpriteEntry, palette, 0x05);
LAYOUT_OFFSET_GUARD(SpriteEntry_Priority, struct SpriteEntry, priority, 0x06);

#undef MOTION_OBJECT_OFFSET

#if GAME_TLA
s32               ActivateBattleObjectSlot(s32 objectId);
struct BattleObj *BattleObjGet(s32 objectId);
#endif
#if GAME_TLA
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
#endif
#if GAME_TLA
void *GetMotionRecord(struct MotionObject *object, s32 record_index);
#endif

#endif
