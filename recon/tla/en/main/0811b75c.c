// Draft, differ 2 of 290, skeleton gap 1 (tla-en, 2026-11-01)
// Left: after the metadata call the native reads ldrb; mov r2, sl; lsrs;
// mov r1, r8; strh; str; here mov r2, sl comes after mov r1, r8, and the
// skeleton gap is that one row. Only those two copies differ.
// What sched2 shows (--rtl dumps, rank_for_schedule): reload's rotation r1,
// r2, r3 proves the native's RTL order is height store, record store, then
// the next pointer, so the copy of sl is later in the RTL than the copy of r8
// and has to win on rank. Plain, the record store loses to the split chain
// (movs #12; add; mov sl), which outranks it through the anti dependence on
// r3 (6, tagged below). With a boundary after the record store the two copies
// tie in priority and dependents and LUID picks r8's first (4). The block
// below makes lsrs the boundary, so the copy of sl cannot fill the load's
// stall. Its record store is ahead of the chain, which is what is kept.
// This looks like the shared-fact family "a split-constant chain outranks
// stores": a model question under K2, out of reach of this draft.
// Tried 1 November, one edit at a time: the plain statements (6); an asm or a
// one-pass block on records++ after the record store (4); the block from the
// metadata call on, with or without boxY (6); a copy of records into a local
// before the block, any placement or with its own block (2 or 4, cprop folds
// it); an empty asm holding that copy in a low register (36 to 101: regmove
// merges it, or the pointer loses or swaps r8 and sl).
// Not to retry: storing the record first swaps the reload registers (8).
// Earlier: records = records + 1, &records[1], boxY / 2 (6 each); records++
// then records[-1] (91); a one-pass block around the store alone (4); four
// minutes of permute (best 4). The Broken Seal's twin in tbs-en schedules
// these rows a third way (mov r1, r8 before the shift, mov r2, sl after it).
// The one-pass device below dates from 30 October and does not reach 0.
// From The Broken Seal's BattleObject_SpawnObjectsForList
// (games/tbs/src/battle/battle/objects.c): this game releases the unlisted
// places only when asked (the third argument), skips a fading actor, has
// seven two-part sprites, sets the body's palette before its priority and
// has a sprite record with its scale at 12.
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
    if ((battleObject->resource & 0xfff) == 561 ||
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
        /* FAKEMATCH(one-pass): the height and the first record stored in a
         * once-only block; as the two plain statements, 6 of 290
         * instructions differ in tla-en. */
        do {
          object->height = boxY >> 1;
          *records = record;
        } while (0);
        records++;
      }
      record->flags = 0;
      record = ResourceObject_CreateFar(spriteId + 0x2001);
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
            (battleObject = (struct TlaBattleObj *)BattleObject_Get(spriteId)) != 0 &&
            (object = battleObject->object) != 0)
          BattleObject_UpdatePose(spriteId);
        unitIndex++;
        if (unitIndex >= 14 || (spriteId = unitIds[unitIndex]) == 255)
          break;
      }
    }
  }
}
