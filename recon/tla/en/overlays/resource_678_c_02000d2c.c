// Draft, differ 225 of 260: a Taopo Swamp scene inside an event: actor 19 rocks
// over for ten frames and crashes down to (492, 246) with the screen shaking
// (cues 188 and 141), actor 10 is thrown up beside it, sixteen clods of
// type 255 burst out in random directions, the camera moves, the swamp's
// cells (Func_02002838) drain row by row over eleven steps while
// Func_02000ca0, Func_02000c24 and Func_02000cec run in turn, and flag
// 0x330 is set. Left: the native's loops count down in r11 with their tests at the bottom (the clod loop keeps IwramMulQ16 in r10), keeps the rock in r9, and orders the clods' stores otherwise.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "iwram_call.h"

extern u8 gData_02002a04[];
struct Actor *Actor_GetById(u32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
void Func_020028a0(void);
void Func_02002870(void);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u16 Random16Far(void);
void Func_02000c14(void);
void Func_02000ca0(void);
void Func_02000c24(void);
void Func_02000cec(void);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPosition(u32 id, s32 x, s32 z);
void Actor_WaitForMoveById(u32 id);
void Func_020028d0(s32 id, s32 x, s32 z);
void Func_020029b0(s32 cue);
void Func_02002868(s32 a, s32 b, s32 c);
struct Actor *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Task_Add(void (*func)(void), s32 priority);
void Func_02002918(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02002838(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void GameFlag_SetBit(s32 flag);

void Func_02000d2c(void) {
  struct Actor *rock = Actor_GetById(19);
  struct Actor *clod;
  s32 i, angle, speed, scale, row;

  Func_02002898();
  Func_02002968(0);
  for (i = 9; i >= 0; i--) {
    wait(1);
    ((struct FieldSprite *)rock->animation)->rotation += -256;
    rock->x -= Math_Cosine(((struct FieldSprite *)rock->animation)->rotation) / 2;
    rock->targetX = 0x80000000;
  }
  rock->update = Func_02000c14;
  Actor_SetSpeedParameters(19, 0x20000, 0x10000);
  Actor_ResetAndSetPosition(19, 492, 246);
  rock->gravity = 0xcccc;
  rock->motionFlags = 3;
  rock->terrainId = 0;
  Actor_WaitForMoveById(19);
  rock->velocityY = 0;
  rock->y = rock->terrainHeight;
  Func_020028d0(19, 0x1e00000, 0xf80000);
  ((struct FieldSprite *)rock->animation)->rotation = 0;
  Func_020029b0(188);
  Func_02002868(0x50000, 0x50000, 0x10000);
  Func_020029b0(141);
  Func_02002868(-1, -1, 0xe666);
  Func_020028d0(10, 0x1e00000, 0xf80000);
  Actor_ResetAndSetPosition(10, 440, 232);
  Actor_GetById(10)->velocityY = 0x50000;
  rock->update = 0;
  for (i = 15; i >= 0; i--) {
    clod = Func_02002808(255, rock->x, rock->y + 0x100000, rock->z);
    if (clod == 0)
      continue;
    angle = Random16Far();
    speed = Random16Far();
    scale = (Random16Far() >> 2) + 0x1000;
    Func_02002800(clod, gData_02002a04);
    Actor_SetSingleChildFlags(clod, 0);
    clod->velocityY = (angle << 2) + 0x20000;
    speed <<= 3;
    clod->velocityZ = Iwram_MulQ16(speed, Math_Cosine(angle));
    clod->acceleration = 0;
    clod->gravity = 0x547a;
    clod->velocityX = Iwram_MulQ16(speed, Math_Sine(angle));
    clod->scaleX = scale;
    clod->scaleY = scale;
    clod->bounce = 0x8000;
  }
  Func_02002870();
  Task_Add(Func_02000ca0, 0x480);
  wait(40);
  Func_02002918(0xe666, 0x1ccc);
  Camera_MoveWithinMapBounds(0x1500000, -1, 0x1180000, 1);
  row = 114;
  for (i = 11; i > 0; i--) {
    Func_02002838(50, row, 13, 5, 18, 15);
    wait(8);
    row -= 5;
  }
  Task_Add(Func_02000c24, 0x480);
  wait(60);
  Task_Add(Func_02000cec, 0x480);
  wait(24);
  FieldEvent_Wait(120);
  Func_020028a0();
  GameFlag_SetBit(0x330);
}
