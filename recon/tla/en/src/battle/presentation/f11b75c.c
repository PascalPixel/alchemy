#include "types.h"
#include "motobj.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "sprite_metadata.h"
#include "task.h"

struct TlaBattleObj {
  struct MotionObject *object;
  u16 resource;
  u16 overlay;
  u16 animation;
  u16 effect;
  s32 anchorX;
  s32 anchorZ;
  s32 palette;
  s32 scale;
  u16 activeConditions;
  u8 selectedCondition;
  s8 cycleTimer;
  void *animationEntry;
  void *effectEntry;
  s16 active;
  u8 fading;
  u8 visible;
};

struct TlaSpriteRecord {
  u8 unk00[12];
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 unk15[5];
  u8 flags;
  u8 unk1b[13];
  struct SpriteEntry *entries[1];
};

s32 BattlePlacement_ContainsId(s16 *unitIds, s32 id);
s32 BattleUnit_BuildStatusFlags(s32 id, struct TlaBattleObj *slot);
struct TlaSpriteRecord *ResourceObject_CreateFar(s32 resource);
struct SpriteEntry *ResourceMetadata_RegisterFar(struct TlaSpriteRecord *record,
                                                 s32 sprite);
void Animation_SetWorkEntryFar(struct SpriteEntry *entry, s32 value);
void BattleObject_UpdatePose(s32 unitId);

void Func_0811b75c(s16 *unitIds, s32 refreshPoses, s32 releaseOthers) {
  s32 unitIndex;
  s32 unitId;
  struct TlaBattleObj *battleObject;
  struct MotionObject *object;
  struct TlaSpriteRecord *record;
  struct SpriteEntry *entry;
  struct TlaSpriteRecord **records;
  s32 spriteId;
  s32 releasedUnitId;
  u8 boxY;

  if (releaseOthers) {
    for (unitIndex = 0; unitIndex < 14; unitIndex++) {
      if (!BattlePlacement_ContainsId(unitIds, unitIndex)) {
        releasedUnitId = unitIndex + 120;
        if (unitIndex <= 7)
          releasedUnitId = unitIndex;
        ReleaseBattleObjectRecords(releasedUnitId);
      }
    }
  }
  if (gTasksRunning == 0)
    wait(1);

  for (unitIndex = 0; unitIndex < 14 && (unitId = unitIds[unitIndex]) != 255;
       unitIndex++) {
    if (unitId == 254)
      continue;
    battleObject = (struct TlaBattleObj *)BattleObject_Get(unitId);
    if (battleObject == 0)
      continue;
    if (battleObject->fading != 0)
      continue;
    BattleUnit_BuildStatusFlags(unitId, battleObject);
    object = battleObject->object;
    if (object == 0)
      continue;
    if (object->recordStorageKind != 0)
      continue;
    if ((battleObject->resource & 0xfff) == SPLIT_CONSTANT(0x132, 255) ||
        (battleObject->resource & 0xfff) == 631 ||
        (battleObject->resource & 0xfff) == 638 ||
        (battleObject->resource & 0xfff) == 585 ||
        (battleObject->resource & 0xfff) == 579 ||
        (battleObject->resource & 0xfff) == 581 ||
        (battleObject->resource & 0xfff) == 583) {
      records = (struct TlaSpriteRecord **)Ram_HeapSlots->objectSystem->unk08 +
                Ram_HeapSlots->objectSystem->count;
      spriteId = battleObject->resource;
      object->recordStorageKind = 2;
      object->records = records;
      Iwram_ClearWords(records, 16);
      record = ResourceObject_CreateFar(spriteId);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        boxY = Resource_GetMetadataRecordFarFar(spriteId)->boxY;
        object->height = boxY >> 1;
        *records = record;
        records++;
      }
      record->flags = 0;
      record = ResourceObject_CreateFar(spriteId + SPLIT_CONSTANT(0x2000, 1));
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        *records = record;
      }
      record->flags = 0;
    } else {
      record = ResourceObject_CreateFar(battleObject->resource);
      if (record != 0) {
        object->recordStorageKind = 1;
        object->records = record;
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        entry = record->entries[0];
        entry->palette = battleObject->palette;
        entry->priority = 1;
        if ((spriteId = battleObject->overlay) != 0) {
          entry = ResourceMetadata_RegisterFar(record, spriteId);
          entry->priority = 1;
        }
        if ((spriteId = battleObject->animation) != 0) {
          entry = ResourceMetadata_RegisterFar(record, spriteId);
          battleObject->animationEntry = entry;
          Animation_SetWorkEntryFar(entry, 0);
          entry->priority = 3;
        }
        if ((spriteId = battleObject->effect) != 0) {
          if (record->width == 32)
            spriteId = 687;
          entry = ResourceMetadata_RegisterFar(record, spriteId);
          battleObject->effectEntry = entry;
          entry->priority = 0;
          record->flags = 0;
        }
      }
    }
    BattleObject_UpdatePose(unitId);
  }

  if (refreshPoses) {
    unitIndex = 0;
    if ((spriteId = unitIds[0]) != 255) {
      for (;;) {
        if (unitIds[unitIndex] != 254 &&
            (battleObject =
                 (struct TlaBattleObj *)BattleObject_Get(spriteId)) != 0 &&
            (object = battleObject->object) != 0)
          BattleObject_UpdatePose(spriteId);
        unitIndex++;
        if (unitIndex >= 14 || (spriteId = unitIds[unitIndex]) == 255)
          break;
      }
    }
  }
}
