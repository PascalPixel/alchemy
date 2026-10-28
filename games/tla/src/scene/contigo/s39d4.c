#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

struct ContigoEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

extern const u8 gData_02004a1c[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetMovementTypeById(s32 actor, const void *script);
void Func_02001d50(void);
void Func_020040d4(s32 actor, s32 x, s32 z);

void Func_020039d4(void) {
  ((struct ContigoEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x209;
  if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9d0) == 0)
      Func_02001d50();
  } else if (gMainRoutineWork.entrance == 21) {
    GameFlag_ClearBit(0x12f);
  } else {
    Actor_SetModeById(17, 2);
    Actor_SetSingleChildFlags(Actor_GetById(17), 0);
    if (GameFlag_Test(0x9b2) != 0)
      Actor_SetMovementTypeById(11, gData_02004a1c);
    if (GameFlag_Test(0x9d0) != 0) {
      Func_020040d4(24, 0, 0);
      Func_020040d4(25, 0, 0);
      Func_020040d4(26, 0, 0);
      Func_020040d4(27, 0, 0);
    }
  }
}
