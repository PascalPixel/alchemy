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

// Far call to Func_080219cc. Table order varies: English and Japanese use
// the 0x08020370 slot; other editions use the slot named Func_08020340.
// Both jump to Func_080219cc.
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_JAPANESE)
void Func_08020370(void *record);
#define Func_080219ccFar Func_08020370
#else
void Func_08020340(void *record);
#define Func_080219ccFar Func_08020340
#endif
s32 WaitFrames(s32 frames);
void ResourceMetadata_ReleaseSlotFar(void *record, s32 slot);

// Return the borrowed buffer and release the motion graphics slot.
void Func_0811e7dc(struct Work0811e7dc *work) {
  struct MotionObject *object = BattleObjGet(work->objectId)->object;

  if (work->buffer != NULL) {
    Func_080219ccFar(GetMotionRecord(object, 0));
    WaitFrames(1);
    MemFree(work->buffer);
    work->buffer = NULL;
    {
      struct Record0811e7dc *record = GetMotionRecord(object, 0);

      record->flags = 0x20 | record->flags;
    }
    ResourceMetadata_ReleaseSlotFar(GetMotionRecord(object, 0), 0);
  }
}
