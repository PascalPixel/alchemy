#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "call.h"

extern const u8 gData_020062d4[];
extern const u8 gData_02006290[];

struct Actor *Actor_GetById(s32 id);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004fcc(s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 immediate);
void Func_02004a5c(s32 id, s32 x, s32 y, s32 z);
void Func_02004dac(struct Actor *object, const u8 *script);
void Func_02004d9c(struct Actor *object, s32 mode);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02005014(s32 cue);
void Func_02004e24(s32 x, s32 y, s32 z);
void Func_02004f34(s32 id, s32 value);
void Func_02004f54(void);
void FieldEvent_Wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void GameFlag_SetBit(s32 flag);

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
  leader = Actor_GetById(entryWork->currentOwner);
  gravity = leader->gravity;
  Func_02004e5c();
  Func_02004fcc(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  object = Actor_GetById(value);
  Func_02004a5c(value, leader->x, leader->y, leader->z);
  Func_02004dac(object, gData_020062d4);
  i = 15;
burst:
  object = Import_ActorCreate(255, leader->x, leader->y + 0x100000, leader->z);
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
    object->velocityX = Iwram_MulQ16(value << 3, Math_Sine(angle));
    object->acceleration = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    object->gravity = SPLIT_CONSTANT(0x5400, 122);
    object->bounce = 0x8000;
  }
  i--;
  if (i >= 0)
    goto burst;
  Func_02005014(SPLIT_CONSTANT(0x200, 78));
  Func_02005014(217);
  Call3((void (*)())Func_02004e24, 0x20000, 0x20000, 0xe666);
  Func_02004f34(gMainRoutineWork.currentOwner, 257);
  Func_02004d9c(leader, 40);
  FieldEvent_Wait(40);
  Func_02005014(204);
  leader->motionFlags = 3;
  leader->gravity = SPLIT_CONSTANT(0x5400, 122);
  for (value = 0; value < 180; value++) {
    wait(1);
    if (leader->y <= leader->terrainHeight)
      break;
  }
  Func_02005014(188);
  work = &gMainRoutineWork;
  leader->gravity = gravity;
  Func_02004f34(work->currentOwner, 0);
  Func_02004d9c(leader, 38);
  CallLr3((void (*)())Func_02004e24, 0x10000, 0x40000, 0x10000);
  wait(10);
  Call3((void (*)())Func_02004e24, -1, -1, 0xe666);
  wait(20);
  Camera_MoveWithinMapBounds(leader->x, leader->y, leader->z, 1);
  Func_02004f54();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02004d9c(leader, 1);
  leader->velocityY = 0x40000;
  wait(20);
  work->renderMode = 0;
  Func_02004e64();
  GameFlag_SetBit(flag);
}
