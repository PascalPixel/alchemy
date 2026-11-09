#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"

extern const u8 gData_020062d4[];
extern const u8 gData_02006290[];

void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004a5c(s32 id, s32 x, s32 y, s32 z);
void Func_02004dac(struct Actor *object, const u8 *script);
void Func_02004d9c(struct Actor *object, s32 mode);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Func_02004f54(void);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);

void Func_02004af0(s32 id, s32 flag) {
  struct MainRoutineWork *entryWork = &gMainRoutineWork;
  struct Actor *leader;
  s32 gravity;
  s32 value;
  struct Actor *object;
  s32 i;
  struct MainRoutineWork *work;

  /* FAKEMATCH(barrier): the form kept; as plain gMainRoutineWork.currentOwner
   * with no asm, 2 of 218 instructions differ in tla-en. */
  __asm__("" : "=r"(entryWork) : "0"(entryWork));
  /* FAKEMATCH(barrier): the form kept; as plain code with no asm here, 2 of 218
   * instructions differ in tla-en. */
  __asm__("");
  /* FAKEMATCH(temporary): the form kept; as id used directly from entry, 4 of
   * 218 instructions differ in tla-en. */
  value = id;
  leader = Actor_Get(entryWork->currentOwner);
  gravity = leader->gravity;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object = Actor_Get(value);
  Func_02004a5c(value, leader->x, leader->y, leader->z);
  Func_02004dac(object, gData_020062d4);
  i = 15;
burst:
  object = (FieldActor_Create(
      255, leader->x, leader->y + 0x100000, leader->z));
  if (object != NULL) {
    u32 angle;
    u32 size;
    s32 scale;

    Func_02004dac(object, gData_02006290);
    angle = Random16Far();
    value = Random16Far();
    size = Random16Far();
    scale = (size >> 1) + 0x2000;
    object->velocityY = Iwram_MulQ16(value << 3, Math_Cosine(angle));
    object->velocityX = Iwram_MulQ16(value << 3, Math_Sin(angle));
    object->acceleration = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    object->gravity = 0x547a;
    object->bounce = 0x8000;
  }
  i--;
  if (i >= 0)
    goto burst;
  Event_PlaySound(590);
  Event_PlaySound(217);
  MapGrid_SetShake(0x20000, 0x20000, 0xe666);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 257);
  Func_02004d9c(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(204);
  leader->motionFlags = 3;
  leader->gravity = 0x547a;
  for (value = 0; value < 180; value++) {
    System_Wait(1);
    if (leader->y <= leader->terrainHeight)
      break;
  }
  Event_PlaySound(188);
  work = &gMainRoutineWork;
  leader->gravity = gravity;
  Actor_SetAttachedEffect(work->currentOwner, 0);
  Func_02004d9c(leader, 38);
  MapGrid_SetShake(0x10000, 0x40000, 0x10000);
  System_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  System_Wait(20);
  FieldCamera_MoveTo(leader->x, leader->y, leader->z, 1);
  Func_02004f54();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02004d9c(leader, 1);
  leader->velocityY = 0x40000;
  System_Wait(20);
  work->renderMode = 0;
  Func_02004e64();
  Event_SetFlag(flag);
}
