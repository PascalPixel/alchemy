#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

#include "types.h"
#include "static_assert.h"
#include "battle_session.h"
#include "edition.h"

union FieldObject;

/// An effect object that moves and draws one sprite: its position, scale,
/// velocity and target, and its per-frame update. It is one of the object
/// slots the field's FieldActor also describes.
struct MotionObject {
  u8 unk00[0x06]; ///< not named by TLA code through this view
  u16 angle;      ///< 1/65536 turns
  s32 x;          ///< 16.16 fixed-point pixels
  s32 y;          ///< 16.16 fixed-point pixels
  s32 z;          ///< 16.16 fixed-point pixels
  u8 unk14[0x04]; ///< FieldActor's terrainHeight, the ground's y
  s32 scaleX;     ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;     ///< 16.16 fixed point, 0x10000 full size
  u16 height;     ///< not named by TLA code through this view
  u8 unk22[0x02]; ///< not named by TLA code through this view
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Horizontal speed gained each frame towards the target, or lost braking
  /// without one, in 16.16 fixed-point pixels per frame.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; 0x80000000 for no target
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  /// With motionFlags bit 1, the 16.16 share of velocityY kept on a bounce.
  s32 verticalMotionPhase;
  /// Gravity: taken from velocityY each frame above the ground with
  /// motionFlags bit 1, in 16.16 fixed-point pixels per frame.
  s32 verticalMotionStrength;
  u8 unk4c[0x04]; ///< not named by TLA code through this view
  void *records;  ///< one sprite, or a table of them (recordStorageKind)
  /// bits: 3..0 how records holds the sprites
  u8 recordStorageKind;
  /// bits: 1 falls under gravity and bounces | 0 follows the terrain
  u8 motionFlags;
  u8 unk56[0x02];    ///< not named by TLA code through this view
  u8 snapToTarget;   ///< nonzero: lands exactly on the target
  u8 unk59;          ///< FieldActor's collisionFlags
  u8 autoFaceMotion; ///< bit 0 turns the object to face where it moves
  u8 unk5b[0x11];    ///< not named by TLA code through this view
  void (*update)(union FieldObject *object);
};

struct SpriteEntry {
  u8 unk00[5];
  u8 palette;
  u8 priority;
};

void ReleaseBattleObjectRecords(s32 objectId);
void ResetMotionRecordGroup(void *record_group);
void ResetBattleObjectRecordGroups(struct MotionObject *object);

STATIC_ASSERT(sizeof(struct MotionObject) == 0x70);
STATIC_ASSERT(offsetof(struct MotionObject, scaleX) == 0x18);
STATIC_ASSERT(offsetof(struct MotionObject, x) == 0x08);
STATIC_ASSERT(offsetof(struct MotionObject, velocityX) == 0x24);
STATIC_ASSERT(offsetof(struct MotionObject, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct MotionObject, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct MotionObject, records) == 0x50);
STATIC_ASSERT(offsetof(struct MotionObject, recordStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct MotionObject, update) == 0x6c);
STATIC_ASSERT(sizeof(struct SpriteEntry) == 0x08);
STATIC_ASSERT(offsetof(struct SpriteEntry, palette) == 0x05);
STATIC_ASSERT(offsetof(struct SpriteEntry, priority) == 0x06);

s32 BattleObject_MarkForRelease(s32 objectId);
struct BattleObj *BattleObjGet(s32 objectId);
struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);

#endif
