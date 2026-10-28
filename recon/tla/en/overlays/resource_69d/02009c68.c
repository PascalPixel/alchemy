// Near miss of overlay 69d's Func_02001c68 (698's Func_020018bc and 6ad's
// Func_02003fc8 are the same routine): differ 10 of 79 in two places. The
// native loads gMainRoutineWork's address before it spills the third argument (ldr
// r3, =gMainRoutineWork; str r2, [sp, #0]), and it shifts and stores the new x after
// it has stored z and started the follower's address (str r3, [r6, #16]; movs
// r3, #230; lsls r3, r3, #1; add r3, r10; lsls r2, r2, #20; adds r2, r2, r1;
// ldr r1, [r3]; str r2, [r6, #8]; str r2, [r1, #8]). Tried: the follower loaded
// first (the stores then come before its address), the sums as locals with z
// stored first (the two flag bytes swap r7 and r8), the sums added into the
// parameters (worse). GameFlag_GetByte must return a word here: as u8 the
// results are masked.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s16 gData_02002864[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001dd0(void);
void Func_02001a20(void);

void Func_02001c68(s32 x, s32 z, s32 value) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  void *work = Ram_HeapSlots->eventWork;
  s32 stepX = GameFlag_GetByte(gData_02002864[1]);
  s32 stepZ = GameFlag_GetByte(gData_02002864[1] + 8);

  if (stepX != 0 || stepZ != 0) {
    struct Actor *follower;

    GameFlag_SetByte(gData_02002864[1], 0);
    GameFlag_SetByte(gData_02002864[1] + 8, 0);
    object->x = ((stepX + x) << 20) + 0x80000;
    object->z = ((stepZ + z) << 20) + 0x80000;
    follower = (struct Actor *)WORD_AT(work, 460);
    follower->x = object->x;
    follower->z = object->z;
    Func_02001dd0();
    Func_02001a20();
    WORD_AT(Ram_HeapSlots->eventWork, 428) = value;
  }
}
