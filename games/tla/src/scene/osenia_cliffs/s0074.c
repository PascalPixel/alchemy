#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u32 gData_020001b0[];

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

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
    u8 *flags =
        (u8 *)((void *)Actor_GetById(gMainRoutineWork.currentOwner)) + 35;
    u32 value = *flags;
    u32 bit = 32;

    /* FAKEMATCH(barrier): the asm on the bit builds it in the register it ORs
     * into, as the native does; as *flags |= 32, the OR goes into the loaded
     * byte and 4 instructions differ. */
    __asm__("" : "+l"(bit));
    *flags = bit | value;
  }
  Func_02000190();
  {
    s32 a2 = 11;
    s32 a1 = 12;

    /* FAKEMATCH(barrier): the asm on the arguments loads them last to first, as
     * the native does; as Func_02000198(0, 12, 11), the first loads before the
     * second and 2 instructions differ. */
    asm volatile("" : "+l"(a2), "+l"(a1));
    Func_02000198(0, a1, a2);
  }
  Actor_SetModeById(12, 4);
  Actor_SetModeById(11, 4);
  {
    s32 flag = 0x104;
    s32 a0;

    a0 = 1;
    /* FAKEMATCH(barrier): the asm on the first argument keeps its load right
     * after the first half of flag + 0xff; as Func_020001a8(1, flag + 0xff, 13,
     * 14), it loads last. */
    asm volatile("" : "+l"(a0));
    Func_020001a8(a0, flag + 0xff, 13, 14);
  }
  return 0;
}
