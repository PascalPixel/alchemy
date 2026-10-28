#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct ClockWork {
  u8 *tiles;
  u8 *digits;
  u8 unk08[120];
  s32 tileEntry;
  s32 timer;
  u8 ringing;
  u8 mode;
};

extern u8 gData_020023c4[];
void Func_02005854(s32 sound);
void Func_02005090(void);
s32 Math_Cosine(s32 angle);
void Task_RemoveFar(void (*callback)(void));
s32 Task_Add(void (*callback)(void), s32 order);
void Func_02005210(void);
void Func_0200525c(void);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);
void *Heap_AllocEwram(s32 size);
void *Resource_GetTableEntry(s32 index);
void Func_020055cc(const void *data, void *buffer);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 handle, s32 size, void *source);
s32 GameFlag_Test(s32 flag);
extern char ResourceId_ClockDigits;

void Func_020052a4(s32 mode) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  u8 *tiles;

  if (!GameFlag_Test(0x109)) {
    Iwram_ClearWords(clock, sizeof(struct ClockWork));
    clock->timer = 3600;
    clock->ringing = 1;
  }
  tiles = Heap_AllocEwram(0x400);
  Func_020055cc(Resource_GetTableEntry((s32)&ResourceId_ClockDigits), tiles);
  clock->tiles = tiles;
  clock->digits = tiles + 0x2c0;
  clock->tileEntry = Resource_FindFreeEntry();
  VramCache_Load(clock->tileEntry, 320, NULL);
  clock->mode = mode;
  if (mode)
    Task_Add(Func_02005210, 1152);
  else
    Task_Add(Func_0200525c, 1152);
}

void Func_0200533c(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (clock->mode)
    Task_RemoveFar(Func_02005210);
  else
    Task_RemoveFar(Func_0200525c);
}

void Func_02005364(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

  if (clock->mode)
    Task_Add(Func_02005210, 1152);
  else
    Task_Add(Func_0200525c, 1152);
  clock->timer = 3600;
}

s32 Func_020053a0(void) {
  return ((struct ClockWork *)(gData_020023c4 + 0x288))->timer;
}

void Func_020053ac(void) {
  ((struct ClockWork *)(gData_020023c4 + 0x288))->timer = 3600;
}
