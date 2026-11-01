// Draft, differ 16 of 260, skeleton gap 2 (tla-en, 2026-11-01)
// A Taopo Swamp scene inside an event: actor 19 rocks over for ten frames and
// crashes down to (492, 246) with the screen shaking (cues 188 and 141), actor
// 10 is thrown up beside it, sixteen clods of type 255 burst out in random
// directions, the camera moves, the swamp's cells (Func_02002838) drain row by
// row over eleven steps while Func_02000ca0, Func_02000c24 and Func_02000cec
// run in turn, and flag 0x330 is set.
// Left: angle and mul trade r8 and r10, and "movs r0, #15" comes before the
// store to rock->update where the listing has it after (the one skeleton
// pair). The registers follow local-alloc's priorities: mul must rank below
// angle. Setting mul right after Math_Cosine ranks it between speed and
// angle; a longer life flips it, as a long long temporary for mul's first
// result does (differ 2), but that is a forced temporary, not the source.
// Moving i = 15 before the store worsens it (differ 22). The labelled loops
// keep the loop pass from hoisting 18, 15 and mul out of them, and the call.h
// forwarding keeps each call's constants its own.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

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
u32 Random16Far(void);
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
  s32 i, row, cosine;
  s32 (*mul)(s32, s32);
  u32 angle, speed, scale;
  s32 turn = -256;

  Func_02002898();
  Func_02002968(0);
  for (i = 9; i >= 0; i--) {
    wait(1);
    ((struct FieldSprite *)rock->animation)->rotation += turn;
    rock->x -= Math_Cosine(((struct FieldSprite *)rock->animation)->rotation) / 2;
    rock->targetX = 0x80000000;
  }
  rock->update = Func_02000c14;
  Call3((void (*)())Actor_SetSpeedParameters, 19, 0x20000, 0x10000);
  Call3((void (*)())Actor_ResetAndSetPosition, 19, 492, 246);
  rock->gravity = 0xcccc;
  rock->motionFlags = 3;
  rock->terrainId = 0;
  Actor_WaitForMoveById(19);
  rock->velocityY = 0;
  rock->y = rock->terrainHeight;
  Call3((void (*)())Func_020028d0, 19, 0x1e00000, 0xf80000);
  ((struct FieldSprite *)rock->animation)->rotation = 0;
  Func_020029b0(188);
  Call3((void (*)())Func_02002868, 0x50000, 0x50000, 0x10000);
  Func_020029b0(141);
  Call3((void (*)())Func_02002868, -1, -1, 0xe666);
  Call3((void (*)())Func_020028d0, 10, 0x1e00000, 0xf80000);
  Call3((void (*)())Actor_ResetAndSetPosition, 10, 440, 232);
  Actor_GetById(10)->velocityY = 0xa0000;
  rock->update = 0;
  i = 15;
burst:
  clod = Func_02002808(255, rock->x, rock->y + 0x100000, rock->z);
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
  Func_02002870();
  Call2((void (*)())Task_Add, (s32)Func_02000ca0, 0x480);
  wait(40);
  Call2((void (*)())Func_02002918, 0xe666, 0x1ccc);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1500000, -1, 0x1180000, 1);
  i = 11;
  row = 114;
drain:
  Call6((void (*)())Func_02002838, 50, row, 13, 5, 18, 15);
  wait(8);
  row -= 5;
  i--;
  if (i > 0)
    goto drain;
  Call2((void (*)())Task_Add, (s32)Func_02000c24, 0x480);
  wait(60);
  Call2((void (*)())Task_Add, (s32)Func_02000cec, 0x480);
  wait(24);
  FieldEvent_Wait(120);
  Func_020028a0();
  GameFlag_SetBit(0x330);
}
