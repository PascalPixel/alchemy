#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_ids.h"
#include "event_inline.h"

extern const u8 gData_020020dc[];

void Func_020020c4(const void *data);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000210(void);
void Func_02000b84(void);

struct MapMode {
  u8 unk00[23];
  struct MapModeBits {
    u8 low : 2;
    u8 mode : 2;
  } __attribute__((packed)) bits;
};

static __inline__ void AddTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so the arguments are set up in their own order and each call
   * keeps its constants; as a direct call, the costly constant is built first
   * or shared, 18 of 62 instructions differ in tla-en. */
  Task_Add(func, priority);
}

s32 Func_02000be0(void) {
  struct MainRoutineWork *party;
  struct MapMode *map = (struct MapMode *)Ram_HeapSlots->mapWork;
  struct MapModeBits bits = map->bits;

  /* FAKEMATCH(temporary): the mode bits changed in a one-byte copy, bits, and
   * stored back; as map->bits.mode = 1 with no empty block, 2 of 62
   * instructions differ in tla-en. */
  bits.mode = 1;
  /* FAKEMATCH(dead): an empty once-only block before the copy's store, so
   * gMainRoutineWork's address loads between the or and the store; as no block,
   * 12 of 62 instructions differ in tla-en. */
  do {
  } while (0);
  party = &gMainRoutineWork;
  map->bits = bits;
  if (party->scene == (s32)&SceneId_Ov657Room1) {
    Func_02000b84();
  } else {
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
    /* FAKEMATCH(dead): an empty once-only block, which ends the scheduling
     * block after the store; as no block, 4 of 62 instructions differ in
     * tla-en. */
    do {
    } while (0);
    Actor_Get(party->currentOwner)->priorityFlags |= 0x20;
    Actor_SetAnimation(8, 1);
    Func_020020c4(gData_020020dc);
    AddTask(Func_02000188, 0x480);
    if (party->entrance == 99)
      Func_02000210();
    else
      AddTask(Func_020001cc, 0x480);
  }
  return 0;
}
