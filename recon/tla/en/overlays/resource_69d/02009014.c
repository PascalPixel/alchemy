#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gData_02001f10[];
extern u8 gData_02002072[];
extern u8 gData_020022a2[];
extern s16 gData_0200275c;
extern s16 gData_0200275e;
extern struct FieldActor *gData_02002760[];
extern s16 gData_02002860;
extern s16 gData_02002862;

void *MemAlloc(s32 size);
void Func_02001d68(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000e58(void);
struct FieldActor *Object_GetById(s32 id);

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  VramCache_Load(entry, size, source);
}

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  return MemAlloc(size);
}

void Func_02001014(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gData_02001f10, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000e58, 1152);
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
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000e58, 1152);
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
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000e58, 1152);
  gData_0200275e = 0;
  gData_02002860 = 1;
  gData_02002862 = -1;
}

void Func_0200119c(s32 id, s32 value) {
  struct FieldActor *object = Object_GetById(id);

  if (object != NULL) {
    s16 *count = &gData_0200275e;

    object->timer = value;
    gData_02002760[*count] = object;
    (*count)++;
  }
}

void Func_020011cc(s32 value) { gData_02002862 = value; }
