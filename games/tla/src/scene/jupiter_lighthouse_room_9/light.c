#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_0200265c;
extern s16 gData_0200265e;
extern struct FieldActor *gData_02002660[];
extern s16 gData_02002760;
extern s16 gData_02002762;

void *Heap_Alloc(s32 size);
void Func_02001b28(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02000c18(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000dd4(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002660, 64);
  Func_02001b28(gField_CachedTilesA, buffer);
  gData_0200265c = Resource_FindFreeEntry();
  VramCache_Load(gData_0200265c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000c18, TASK_PASS_VBLANK);
  gData_0200265e = 0;
  gData_02002760 = 0;
  gData_02002762 = -1;
}

void Func_02000e54(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002660, 64);
  Func_02001b28(gField_CachedTilesB, buffer);
  gData_0200265c = Resource_FindFreeEntry();
  Vram_Load(gData_0200265c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000c18, TASK_PASS_VBLANK);
  gData_0200265e = 0;
  gData_02002760 = 0;
  gData_02002762 = -1;
}

void Func_02000ed4(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002660, 64);
  Func_02001b28(gField_CachedTilesC, buffer);
  gData_0200265c = Resource_FindFreeEntry();
  Vram_Load(gData_0200265c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000c18, TASK_PASS_VBLANK);
  gData_0200265e = 0;
  gData_02002760 = 1;
  gData_02002762 = -1;
}
