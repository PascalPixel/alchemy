/// @file
/// Battle: release an object's graphics.

#include "types.h"
#include "motobj.h"
#include "resource.h"

/// Releases all graphics the object owns and marks it as having none.
/// no: object ID.
void ReleaseBattleObjectRecords(s32 no) {
  struct BattleObj *slot;
  struct MotionObject *obj;
  s32 i;
  void *rec;

  slot = BattleObject_Get(no);
  if (slot != NULL) {
    obj = slot->object;
    if (obj != NULL) {
      slot->animationEntry = 0;
      slot->effectEntry = 0;
      i = 0;
      while ((rec = GetMotionRecord(obj, i)) != NULL) {
        ResourceObject_ReleaseFar(rec);
        i += 1;
      }
      obj->recordStorageKind = 0; // No graphics.
      obj->records = NULL;
    }
  }
}
