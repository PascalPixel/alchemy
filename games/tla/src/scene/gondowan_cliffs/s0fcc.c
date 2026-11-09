#include "types.h"
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "event_inline.h"

struct GondowanCliffsWork {
  s16 entry;
  u16 value;
  u8 unk004[0x1c0];
  void *block;
};

extern u8 gData_020023c4[];
extern const u8 gField_Tiles64D[];

void *Heap_AllocEwram(s32 size);
void Func_02001184(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02000be4(void);
void Func_02000dac(void);

void Func_02000fcc(s32 unused, s32 value) {
  struct GondowanCliffsWork *work =
      (struct GondowanCliffsWork *)(gData_020023c4 + 0x188);
  void *buffer;

  if (!Event_GetFlag(0x109))
    Call2((void (*)())Iwram_ClearWordsEntry, (s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Func_02001184(gField_Tiles64D, buffer);
  work->entry = Resource_FindFreeEntry();
  Vram_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(0x3c0);
  work->value = value;
  FieldTask_AddFrame((s32)Func_02000be4, 0x48d);
  FieldTask_AddFrame((s32)Func_02000dac, 0x480);
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x40;
}
