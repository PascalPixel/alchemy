// Draft, differ 6 of 19, skeleton gap 3 (tla-en, 2026-11-01)
// Measure with --symbol: Func_02001014, Func_02001094, Func_02001114 and
// Func_020011cc are differ 0; the figures above are Func_0200119c.
// Func_0200119c: the count's address loads before the timer store, and the
// table load and the shift then follow it out of place. In sched2 the two ldr
// loads have no dependence on the store, so only a scheduling barrier keeps
// them after it. Plain forms all stay at gap 3 or worse: globals or pointers,
// late or early, a cast store sharing the count's alias set. An increment read
// first or a post-increment index merges the two reads (18 instructions).
// With `asm volatile("")` or a `do { } while (0)` around the timer store it is
// differ 0, as the matched lightcue.c twins in games/tla/src/scene are, each
// with a tagged device. Adopting this needs that device: one-pass is the milder.

#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gData_02001f10[];
extern u8 gData_02002072[];
extern u8 gData_020022a2[];
extern s16 gData_0200275c;
extern s16 gData_0200275e;
extern struct FieldActor *gData_02002760[];
extern s16 gData_02002860;
extern s16 gData_02002862;

void *Heap_Alloc(s32 size);
void Func_02001d68(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void Heap_Release(void *block);
void Func_02000e58(void);
struct FieldActor *Actor_GetById(s32 id);

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  VramCache_Load(entry, size, source);
}

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  return Heap_Alloc(size);
}

void Func_02001014(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gData_02001f10, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 0;
  gData_02002862 = -1;
}

void Func_02001094(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gData_02002072 + 1, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 0;
  gData_02002862 = -1;
}

void Func_02001114(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gData_020022a2, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 1;
  gData_02002862 = -1;
}

void Func_0200119c(s32 id, s32 value) {
  struct FieldActor *object = Actor_GetById(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_0200275e;
    gData_02002760[*count] = object;
    (*count)++;
  }
}

void Func_020011cc(s32 value) { gData_02002862 = value; }
