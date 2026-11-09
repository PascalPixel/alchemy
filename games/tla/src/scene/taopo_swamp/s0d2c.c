#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_inline.h"

extern u8 gData_02002a04[];
void Func_02002898(void);
void Func_020028a0(void);
s32 Math_Cosine(s32 angle);
u32 Random16Far(void);
void Func_02000c14(void);
void Func_02000ca0(void);
void Func_02000c24(void);
void Func_02000cec(void);
void Func_02002800(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

void Func_02000d2c(void) {
  struct Actor *rock = Actor_GetById(19);
  struct Actor *clod;
  s32 i, row, cosine;
  s32 (*mul)(s32, s32);
  u32 angle, speed, scale;
  s32 turn = -256;

  Func_02002898();
  Event_ResolvePendingActions(0);
  for (i = 9; i >= 0; i--) {
    wait(1);
    ((struct FieldSprite *)rock->animation)->rotation += turn;
    rock->x -= Math_Cosine(((struct FieldSprite *)rock->animation)->rotation) / 2;
    rock->targetX = 0x80000000;
  }
  rock->update = Func_02000c14;
  Actor_SetSpeed(19, 0x20000, 0x10000);
  Actor_SetDestination(19, 492, 246);
  rock->gravity = 0xcccc;
  rock->motionFlags = 3;
  rock->terrainId = 0;
  Actor_WaitForMoveById(19);
  rock->velocityY = 0;
  rock->y = rock->terrainHeight;
  Actor_SetPosition(19, 0x1e00000, 0xf80000);
  ((struct FieldSprite *)rock->animation)->rotation = 0;
  Sound_Play(188);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  Sound_Play(141);
  MapGrid_SetShake(-1, -1, 0xe666);
  Actor_SetPosition(10, 0x1e00000, 0xf80000);
  Actor_SetDestination(10, 440, 232);
  Actor_GetById(10)->velocityY = 0xa0000;
  rock->update = 0;
  i = 15;
burst:
  clod = ((struct Actor *)Import_ActorCreate(255, rock->x, rock->y + 0x100000, rock->z));
  if (clod == 0)
    goto next;
  angle = Random16Far();
  speed = Random16Far();
  scale = (Random16Far() >> 2) + 0x1000;
  Func_02002800(clod, gData_02002a04);
  Actor_SetSingleChildFlags(clod, 0);
  clod->velocityY = (angle << 2) + 0x20000;
  cosine = Math_Cosine(angle);
  mul = Iwram_MulQ16;
  speed <<= 3;
  clod->velocityZ = mul(speed, cosine);
  clod->velocityX = mul(speed, Math_Sine(angle));
  clod->acceleration = 0;
  clod->gravity = 0x547a;
  clod->scaleX = scale;
  clod->scaleY = scale;
  clod->bounce = 0x8000;
next:
  i--;
  if (i >= 0)
    goto burst;
  Map_WaitForShakeToSettle();
  Task_AddFrame(Func_02000ca0, 0x480);
  wait(40);
  FieldCamera_SetSpeed(0xe666, 0x1ccc);
  FieldCamera_MoveTo(0x1500000, -1, 0x1180000, 1);
  i = 11;
  row = 114;
drain:
  MapGrid_CopyCells(50, row, 13, 5, 18, 15);
  wait(8);
  row -= 5;
  i--;
  if (i > 0)
    goto drain;
  Task_AddFrame(Func_02000c24, 0x480);
  wait(60);
  Task_AddFrame(Func_02000cec, 0x480);
  wait(24);
  Event_Wait(120);
  Func_020028a0();
  GameFlag_SetBit(0x330);
}
