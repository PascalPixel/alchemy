/// @file
/// Battle: release an object's graphics.

#include "types.h"
#include "motobj.h"
#include "resource.h"

/// Release all graphics owned by the object and mark it as having none.
/// no: object ID.
void ReleaseBattleObjectRecords(s32 no) {
  struct BattleObj *slot;
  struct MotionObject *obj;
  s32 i;
  void *rec;

  slot = BattleObjGet(no);
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
