#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesD[];
extern s16 gData_02001cdc;
extern s16 gData_02001cde;
extern struct FieldActor *gData_02001ce0[];
extern s16 gData_02001de0;
extern s16 gData_02001de2;
extern s16 gData_02001de4;

void *Heap_Alloc(s32 size);
void Func_02000c24(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02000858(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000a98(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02001ce0, 64);
  Func_02000c24(gField_CachedTilesA, buffer);
  gData_02001cdc = Resource_FindFreeEntry();
  VramCache_Load(gData_02001cdc, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000858, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2 = -1;
}

void Func_02000b18(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWords(gData_02001ce0, 64);
  Func_02000c24(gField_CachedTilesD, buffer);
  gData_02001cdc = Resource_FindFreeEntry();
  VramCache_Load(gData_02001cdc, 0x1800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000858, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2 = -1;
  gData_02001de4 = 0;
}
