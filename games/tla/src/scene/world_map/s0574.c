#include "types.h"
#include "edition.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_02004d68(s32 owner);
void Func_02004c20(void);
void Func_02004db8(s32 value);
void Func_02004e30(s32 cue);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b48(struct Actor *object);
void Func_02004b50(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004b58(struct Actor *object);
void Func_02004c88(s32 owner, s32 a, s32 b);
void Func_02004dc8(void);
void Func_02004dc0(void);
void Func_02004c28(void);
void Func_02004df8(s32 a, s32 b);

void Func_02000574(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  struct Actor *ship;
  s32 kind, dx, dz, distance;

  if (GameFlag_Test(0x8de) == 0)
    return;
  kind = Func_02004d68(gMainRoutineWork.currentOwner);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ship = Actor_GetById(8);
  dx = (leader->x - ship->x) >> 16;
  dz = (leader->z - ship->z) >> 16;
  distance = dx * dx + dz * dz;
  if (kind != 8 && distance > 255)
    return;
  Func_02004c20();
  Func_02004db8(0);
  gMainRoutineWork.renderMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  GameFlag_SetBit(0x176);
  gMainRoutineWork.savedX = ship->x;
  gMainRoutineWork.savedZ = ship->z;
  /* FAKEMATCH(one-pass): the angle's store in a once-only block, which
   * builds its address before the angle's load; as the plain store, 2 of
   * 149 instructions differ in tla-en. */
  do {
    gMainRoutineWork.unk280 = ship->angle;
  } while (0);
  Func_02004e30(78);
  Func_02004b18(leader, 6);
  wait(6);
  Func_02004e30(152);
  Actor_SetSingleChildFlags(leader, 0);
  Func_02004b18(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x40000;
  leader->terrainHeight = 0x40000;
  Func_02004b48(leader);
  Func_02004b50(leader, ship->x, 0, ship->z);
  Func_02004b58(leader);
  Func_02004b18(leader, 6);
  wait(6);
  leader->terrainHeight = 0;
  Func_02004c88(gMainRoutineWork.currentOwner, 0, 0);
  Func_02004dc8();
  Func_02004dc0();
  Func_02004c28();
  *(s16 *)work->unk15c = 0;
#if LANGUAGE != LANGUAGE_JAPANESE
  work->exit = 0;
#endif
  Func_02004df8(0x10000, 30);
}
