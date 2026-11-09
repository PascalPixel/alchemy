#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002428;
extern s16 gData_0200242a;
extern struct FieldActor *gData_0200242c[];
extern s16 gData_0200252c;
extern s16 gData_0200252e;

void *Heap_Alloc(s32 size);
void Func_02001a6c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02000b5c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000d18(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesA, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  VramCache_Load(gData_02002428, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e = -1;
}

void Func_02000d98(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesB, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  Vram_Load(gData_02002428, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e = -1;
}

void Func_02000e18(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesC, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  Vram_Load(gData_02002428, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 1;
  gData_0200252e = -1;
}
