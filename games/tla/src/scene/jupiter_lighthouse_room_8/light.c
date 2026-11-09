#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002af0;
extern s16 gData_02002af2;
extern struct FieldActor *gData_02002af4[];
extern s16 gData_02002bf4;
extern s16 gData_02002bf6;

void *Heap_Alloc(s32 size);
void Func_02002118(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02001200(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_020013bc(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesA, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  VramCache_Load(gData_02002af0, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001200, TASK_PASS_VBLANK);
  gData_02002af2 = 0;
  gData_02002bf4 = 0;
  gData_02002bf6 = -1;
}

void Func_0200143c(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesB, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  Vram_Load(gData_02002af0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001200, TASK_PASS_VBLANK);
  gData_02002af2 = 0;
  gData_02002bf4 = 0;
  gData_02002bf6 = -1;
}

void Func_020014bc(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesC, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  Vram_Load(gData_02002af0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001200, TASK_PASS_VBLANK);
  gData_02002af2 = 0;
  gData_02002bf4 = 1;
  gData_02002bf6 = -1;
}
