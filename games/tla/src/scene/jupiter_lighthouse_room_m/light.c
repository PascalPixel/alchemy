#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02005a80;
extern s16 gData_02005a82;
extern struct FieldActor *gData_02005a84[];
extern s16 gData_02005b84;
extern s16 gData_02005b86;

void *Heap_Alloc(s32 size);
void Func_0200468c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_0200377c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02003938(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02005a84, 64);
  Func_0200468c(gField_CachedTilesA, buffer);
  gData_02005a80 = Resource_FindFreeEntry();
  VramCache_Load(gData_02005a80, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_0200377c, TASK_PASS_VBLANK);
  gData_02005a82 = 0;
  gData_02005b84 = 0;
  gData_02005b86 = -1;
}

void Func_020039b8(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02005a84, 64);
  Func_0200468c(gField_CachedTilesB, buffer);
  gData_02005a80 = Resource_FindFreeEntry();
  Vram_Load(gData_02005a80, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_0200377c, TASK_PASS_VBLANK);
  gData_02005a82 = 0;
  gData_02005b84 = 0;
  gData_02005b86 = -1;
}

void Func_02003a38(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02005a84, 64);
  Func_0200468c(gField_CachedTilesC, buffer);
  gData_02005a80 = Resource_FindFreeEntry();
  Vram_Load(gData_02005a80, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_0200377c, TASK_PASS_VBLANK);
  gData_02005a82 = 0;
  gData_02005b84 = 1;
  gData_02005b86 = -1;
}
