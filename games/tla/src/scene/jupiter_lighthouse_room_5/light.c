#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02003444;
extern s16 gData_02003446;
extern struct FieldActor *gData_02003448[];
extern s16 gData_02003548;
extern s16 gData_0200354a;

void *Heap_Alloc(s32 size);
void Func_020027b8(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02001888(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001a44(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003448, 64);
  Func_020027b8(gField_CachedTilesA, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  VramCache_Load(gData_02003444, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 0;
  gData_0200354a = -1;
}

void Func_02001ac4(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02003448, 64);
  Func_020027b8(gField_CachedTilesB, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  Vram_Load(gData_02003444, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 0;
  gData_0200354a = -1;
}

void Func_02001b44(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02003448, 64);
  Func_020027b8(gField_CachedTilesC, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  Vram_Load(gData_02003444, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 1;
  gData_0200354a = -1;
}
