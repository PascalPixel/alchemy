#include "types.h"
#include "iwram_call.h"
#include "call.h"
#include "object_runtime.h"

struct YampiDesertRideWork {
  s16 entry;
  u16 value;
  u8 unk004[0x1c0];
  void *block;
};

extern u8 gData_020023c4[];
extern const u8 gField_Tiles64D[];

s32 GameFlag_Test(s32 flag);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Func_02003f70(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Scheduler_AddOrUpdateCallback();
void Func_020039b8(void);
void Func_02003b80(void);
s32 Func_020041f0(void);
struct ObjectRuntime *Object_GetById(s32 id);

void Func_02003da0(s32 unused, s32 value) {
  struct YampiDesertRideWork *work =
      (struct YampiDesertRideWork *)(gData_020023c4 + 0x188);
  void *buffer;

  if (!GameFlag_Test(0x109))
    Call2((void (*)())Iwram_ClearWordsEntry, (s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Runtime_BumpAllocateAlternatePool, 0x800);
  Func_02003f70(gField_Tiles64D, buffer);
  work->entry = Resource_FindFreeEntry();
  Call3((void (*)())VramCache_Load, work->entry, 0x800, (s32)buffer);
  MemFree(buffer);
  work->block = Runtime_BumpAllocateAlternatePool(0x3c0);
  work->value = value;
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020039b8, 0x48d);
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02003b80, 0x480);
  Object_GetById(Func_020041f0())->priorityFlags |= 0x40;
}
