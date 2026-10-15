//--------------------------------------------------------------------------
// release_battle_object_records.c
// バトル：オブジェクトの絵の解放
//--------------------------------------------------------------------------

#include "types.h"
#include "motobj.h"
#include "resource.h"

//--------------------------------------------------------------------------
// オブジェクトが持つ絵をすべて解放し、絵なしにもどす
// no : オブジェクト番号
//--------------------------------------------------------------------------
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
      obj->recordStorageKind = 0; // 絵なし
      obj->records = NULL;
    }
  }
}
