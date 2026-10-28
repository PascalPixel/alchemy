#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"

extern u32 gData_020001b0[];

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

void GameFlag_SetBit(s32 flag);
void Task_Add(void (*func)(void), s32 priority);
void *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02000190(void);
void Func_02000198(s32 a, s32 b, s32 c);
void Func_020001a8(s32 a, s32 flag, s32 b, s32 c);
void Func_020000fc(void);

s32 Func_02000074(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;

  map->mode = 1;
  gData_020001b0[0] = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 256;
  Task_Add(Func_020000fc, 0x480);
  {
    u8 *flags = (u8 *)Actor_GetById(gMainRoutineWork.currentOwner) + 35;
    u32 value = *flags;
    u32 bit = 32;

    /* FAKEMATCH: the native builds the bit in the register it ORs into; a plain
     * |= ORs into the loaded byte (differ 4). */
    __asm__("" : "+l"(bit));
    *flags = bit | value;
  }
  Func_02000190();
  {
    s32 a2 = 11;
    s32 a1 = 12;

    /* FAKEMATCH: the native loads the arguments last to first; the plain call
     * loads the first before the second (differ 2). */
    asm volatile("" : "+l"(a2), "+l"(a1));
    Func_02000198(0, a1, a2);
  }
  Actor_SetModeById(12, 4);
  Actor_SetModeById(11, 4);
  {
    s32 flag = 0x104;
    s32 a0;

    a0 = 1;
    /* FAKEMATCH: and this forced first argument keeps its load right after that
     * half; unforced it loads last. */
    asm volatile("" : "+l"(a0));
    Func_020001a8(a0, flag + 0xff, 13, 14);
  }
  return 0;
}
