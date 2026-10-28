#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "motobj.h"
#include "battle_action.h"
#include "battle_work.h"
#include "ram_buffer.h"
#include "fixed_point_position.h"

#define Actor_Destroy ((void (*)(void *))Actor_Destroy)

void ResetBattleObjectRecordGroups(struct MotionObject *object) {
  s32 storageKind;
  s32 remaining;
  void **recordGroups;
  void *recordGroup;

  if (object != NULL) {
    storageKind = object->recordStorageKind & 0xF;
    switch (storageKind) {
    case 1:
      ResetMotionRecordGroup(object->records);
      return;
    case 2:
      recordGroups = object->records;
      remaining = 3;
      do {
        recordGroup = *recordGroups++;
        if (recordGroup != NULL) {
          ResetMotionRecordGroup(recordGroup);
        }
        remaining -= 1;
      } while (remaining >= 0);
      break;
    }
  }
}

static __inline__ struct BattleObj *BattleObject_FindSlot(s32 id) {
  u8 *work = (u8 *)Ram_HeapSlots->battleWork;
  struct BattleObj *slots = ((struct BattleWork *)work)->slots;

  if (id > 7)
    id -= 0x78;
  id += 0x74;
  if (work[id] == 0xff)
    return NULL;
  return &slots[work[id]];
}

s32 BattleObject_MarkForRelease(s32 objectId) {
  struct BattleObj *slot;

  BattleMotion_ReleaseObjectSlotByValue(objectId);
  slot = BattleObject_FindSlot(objectId);
  slot->active = 1;
  return 0;
}

void BattleMotion_DestroyAllSlotObjects(void) {
  s32 no;
  s32 i;
  struct BattleObj *slot;

  i = 0;
  do {
    no = i + 0x78;
    if (i <= 7) {
      no = i;
    }
    slot = BattleObject_FindSlot(no);
    if ((slot != NULL) && (slot->active != 0)) {
      Actor_Destroy(slot->object);
      slot->object = NULL;
      slot->active = 0;
    }
    i += 1;
  } while (i <= 0xD);
}

/// Writes a battle object's anchor to out, at height 0.
s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out) {
  struct BattleWork *state = Ram_HeapSlots->battleWork;
  /* FAKEMATCH: the return value, pinned to r0, is set between the two stores
   * and the last load, which a memory barrier holds back; as plain C, 2
   * instructions differ in tla-en. */
  register s32 result asm("r0");

  out->x = state->slots[no].anchorX;
  out->y = 0;
  result = 0;
  /* FAKEMATCH: the barrier of the pin above. */
  __asm__("" : "+l"(result) : : "memory");
  out->z = state->slots[no].anchorZ;
  return result;
}
