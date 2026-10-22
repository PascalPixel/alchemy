// Near miss, differ 41 of 273 (151 before), from The Broken Seal's
// BattleObject_UpdateSpritePriorities (games/tbs/src/battle/battle/
// objects.c): this game lists both sides at once, reads each object through
// BattleObject_Get inlined and keeps it, stops when the battle work's byte at
// 0x866 is set, and marks a guarding unit's sprite (flag 2) as it sets the
// side priority. The two-part loops take each part into its own pointer
// before the shared record (151 to 47), and the counts declared before the
// priorities give the native's spill slots (41). Left: the battle work's
// load before sub sp (the prologue pattern), the motion record copied to r1
// here where the native keeps it in r0 (one extra row, and the effect and
// priority in r0 and r2 for r2 and r1), and the pool rows those shift.
// Tried: the priority set either way and as a conditional (41, 43), the
// effect read twice (41), five minutes of permute (only a rewrite that
// changes the comparison).
#include "types.h"
#include "motobj.h"
#include "animspr.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_party.h"
#include "battle_unit.h"

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
  struct SpriteEntry *effectEntry;
  s16 active;
  u8 fading;
  u8 visible;
};

struct TlaSpriteRecord {
  struct AnimationSpritePart image;
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 unk15[4];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct SpriteEntry *entries[1];
};

struct BattleUnit *Unit_Get(s32 unitId);
s32 BattleStatusIcon_Cycle(struct TlaBattleObj *slot);

static inline struct TlaBattleObj *BattleObject_GetInline(s32 unitId) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct TlaBattleObj *objects = (struct TlaBattleObj *)battle->slots;
  s32 placementIndex;

  if (unitId > 7)
    unitId -= 120;
  placementIndex = unitId;
  if (battle->placement[placementIndex] == 0xff)
    return 0;
  return &objects[battle->placement[placementIndex]];
}

void Func_0811b9fc(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 unitIds[14];
  struct MotionObject *objects[14];
  s32 partyCount;
  s32 enemyCount;
  s32 partyPriority, enemyPriority;
  s32 unitIndex;
  s32 recordIndex;
  struct MotionObject *object;
  struct TlaSpriteRecord *record;
  struct TlaSpriteRecord **records;

  partyCount = BattleParty_ListActorIds(1, unitIds);
  enemyCount = BattleParty_ListActorIds(2, &unitIds[partyCount]);
  unitIndex = 0;
  if (unitIds[unitIndex] != 255) {
    for (;;) {
      struct TlaBattleObj *slot;

      object = 0;
      slot = BattleObject_GetInline(unitIds[unitIndex]);
      if (slot != 0) {
        object = slot->object;
        BattleStatusIcon_Cycle(slot);
        if (slot->effectEntry != 0) {
          struct TlaSpriteRecord *record = GetMotionRecord(object, 0);

          if (record != 0) {
            struct SpriteEntry *effect;
            s32 effectPriority = 0;

            if (object->y != 0)
              effectPriority = 9;
            effect = slot->effectEntry;
            if (effect->priority != effectPriority) {
              effect->priority = effectPriority;
              record->dirty = 1;
            }
          }
        }
      }
      objects[unitIndex] = object;
      unitIndex++;
      if (unitIndex >= 14 || unitIds[unitIndex] == 255)
        break;
    }
  }

  if (((u8 *)work)[0x866] != 0)
    return;

  if ((s16)((struct BattleCamera *)Ram_HeapSlots->cameraWork)->yaw >= 0) {
    partyPriority = 1;
    enemyPriority = 2;
  } else {
    partyPriority = 2;
    enemyPriority = 1;
  }

  for (unitIndex = 0; unitIndex < partyCount; unitIndex++) {
    object = objects[unitIndex];
    if (object != 0) {
      switch (object->recordStorageKind & 15) {
      case 1:
        record = object->records;
        record->image.priority = partyPriority;
        if (Unit_Get(unitIds[unitIndex])->guardLevel == 4)
          record->flags |= 2;
        else
          record->flags &= ~2;
        break;
      case 2:
        records = object->records;
        for (recordIndex = 0; recordIndex < 4; recordIndex++) {
          struct TlaSpriteRecord *part = *records++;

          if (part != 0) {
            record = part;
            record->image.priority = partyPriority;
          }
        }
        break;
      }
    }
  }

  for (unitIndex = partyCount; unitIndex < partyCount + enemyCount;
       unitIndex++) {
    object = objects[unitIndex];
    if (object != 0) {
      switch (object->recordStorageKind & 15) {
      case 1:
        record = object->records;
        record->image.priority = enemyPriority;
        if (Unit_Get(unitIds[unitIndex])->guardLevel == 4)
          record->flags |= 2;
        else
          record->flags &= ~2;
        break;
      case 2:
        records = object->records;
        for (recordIndex = 0; recordIndex < 4; recordIndex++) {
          struct TlaSpriteRecord *part = *records++;

          if (part != 0) {
            record = part;
            record->image.priority = enemyPriority;
          }
        }
        break;
      }
    }
  }
}
