#include "room.h"

extern u16 gData_020068a2;
void *MemAlloc(s32 size);
void MemFree(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_02000d18(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

/// Fills 0x100 bytes with colour 1, loads them into a VRAM block of their own
/// and starts Func_02000d18.
void Func_02000df0(void) {
  u32 *buffer = MemAlloc(0x100);
  /* FAKEMATCH: ⚓️ keeps the fill pointer in r1 and the count in r2;
   * unpinned, the pointer takes r2 and 10 instructions differ in tla-en. */
  register u32 *word asm("r1") = buffer;
  s32 count = 63;

  // A goto loop: as a for loop loop.c moves the fill word out of it.
fill:
  *word++ = 0x11111111;
  if (--count >= 0)
    goto fill;
  gData_020068a2 = Resource_FindFreeEntry();
  /* FAKEMATCH: the native stores the entry before it starts the size, as
   * overlay 67c's Func_02003278 does; without the barrier the size's first half
   * comes first. */
  __asm__("");
  VramCache_Load(gData_020068a2, 0x100, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000d18, 0x480);
}
