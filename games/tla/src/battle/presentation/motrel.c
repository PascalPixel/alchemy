#include "types.h"
#include "motobj.h"
#include "mem.h"
#include "edition.h"

struct Work0811e7dc {
  u8 unk00[0x2c];
  void *buffer;
  u8 objectId;
};

struct Record0811e7dc {
  u8 unk00[26];
  u8 flags;
};

// Far call to Func_080219cc.
void Func_08020370(void *record);
#define Func_080219ccFar Func_08020370
s32 wait(s32 frames);
void ResourceMetadata_ReleaseSlotFar(void *record, s32 slot);

/// Frees the borrowed buffer and releases the motion graphics slot.
void Func_0811e7dc(struct Work0811e7dc *work) {
  struct MotionObject *object = BattleObject_Get(work->objectId)->object;

  if (work->buffer != NULL) {
    Func_080219ccFar(GetMotionRecord(object, 0));
    wait(1);
    Heap_Release(work->buffer);
    work->buffer = NULL;
    {
      struct Record0811e7dc *record = GetMotionRecord(object, 0);

      record->flags = 0x20 | record->flags;
    }
    ResourceMetadata_ReleaseSlotFar(GetMotionRecord(object, 0), 0);
  }
}
