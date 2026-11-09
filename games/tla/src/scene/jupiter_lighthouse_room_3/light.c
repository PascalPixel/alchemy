#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002d2c;
extern s16 gData_02002d2e;
extern struct FieldActor *gData_02002d30[];
extern s16 gData_02002e30;
extern s16 gData_02002e32;

void *Heap_Alloc(s32 size);
void Func_020022a0(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02001388(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001544(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002d30, 64);
  Func_020022a0(gField_CachedTilesA, buffer);
  gData_02002d2c = Resource_FindFreeEntry();
  VramCache_Load(gData_02002d2c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001388, TASK_PASS_VBLANK);
  gData_02002d2e = 0;
  gData_02002e30 = 0;
  gData_02002e32 = -1;
}

void Func_020015c4(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002d30, 64);
  Func_020022a0(gField_CachedTilesB, buffer);
  gData_02002d2c = Resource_FindFreeEntry();
  VramCache_Load(gData_02002d2c, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001388, TASK_PASS_VBLANK);
  gData_02002d2e = 0;
  gData_02002e30 = 0;
  gData_02002e32 = -1;
}

void Func_02001644(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002d30, 64);
  Func_020022a0(gField_CachedTilesC, buffer);
  gData_02002d2c = Resource_FindFreeEntry();
  VramCache_Load(gData_02002d2c, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001388, TASK_PASS_VBLANK);
  gData_02002d2e = 0;
  gData_02002e30 = 1;
  gData_02002e32 = -1;
}
