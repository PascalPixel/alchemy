#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"

extern char SceneId_Ov676Room1;

struct TileSprite {
  u8 unk00[18];
  u16 tile;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_02002450(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002458(struct Actor *object);
void Func_02002470(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020025a0(s32 cue);
void Func_020023c8(void);

void Func_020022f8(void) {
  s32 step;
  s32 i;
  struct Actor *object = Actor_GetById(9);
  struct TileSprite *sprite = object->animation;

  GameFlag_SetBit(0x300);
  object->acceleration = 0x18000;
  object->speedLimit = 0x80000;
  Func_020025a0(227);
  Call4((void (*)())Func_02002450, (s32)object, 0xb00000, 0x280000, 0xb80000);
  step = 0;
  for (i = 15; i >= 0; i--) {
    sprite->tile -= step;
    wait(1);
    step += 40;
  }
  Call4((void (*)())Func_02002450, (s32)object, 0xa80000, 0x100000, 0xb80000);
  step = 400;
  for (i = 25; i >= 0; i--) {
    sprite->tile -= step;
    wait(1);
    step += 40;
  }
  Func_02002458(object);
  FieldEvent_Wait(2);
  Func_020025a0(240);
  i = 0;
  sprite->tile = i;
  object->x = 0xa80000;
  object->z = 0xc00000;
  object->y = i;
  object->velocityY = i;
  object->velocityX = i;
  Call6((void (*)())Func_02002470, 8, 12, 1, 1, 10, 12);
}
