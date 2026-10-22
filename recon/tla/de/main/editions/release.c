#include "types.h"
#include "motobj.h"

void ResourceObject_ReleaseFar(void *);
void ReleaseBattleObjectRecords(s32 objectId) {
  struct BattleObj *slot;
  struct MotionObject *object;
  s32 recordIndex;
  void *record;

  slot = BattleObject_Get(objectId);
  if (slot != NULL) {
    object = slot->object;
    if (object != NULL) {
      slot->runtime_word_20 = 0;
      slot->runtime_word_24 = 0;
      recordIndex = 0;
      while ((record = GetMotionRecord(object, recordIndex)) != NULL) {
        ResourceObject_ReleaseFar(record);
        recordIndex += 1;
      }
      object->recordStorageKind = (s8)record;
      object->records = record;
    }
  }
}
