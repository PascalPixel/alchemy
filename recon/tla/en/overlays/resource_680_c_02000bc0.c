// Draft, differ 163 of 218: a pillar the party pushed off a ledge drops onto
// the floor below with sixteen dust puffs, a shake and the leader riding it
// down (gravity lowered until it lands, at most 180 frames), then sets its
// flag. Overlay 681's Func_02004af0 is a copy with small changes.
// Measured against its copies: overlay 681's Func_02004af0 (differ 163 of 218).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"

extern const u8 gData_020016cc[];
extern const u8 gData_02001688[];

struct Actor *Actor_GetById(s32 id);
void Func_02000e74(void);
void Func_02000ef4(s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 follow);
void Func_02000b2c(s32 id, s32 x, s32 y, s32 z);
void Func_02000e2c(struct Actor *object, const u8 *script);
struct Actor *Func_02000e34(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02000f24(s32 cue);
void Func_02000e54(s32 x, s32 y, s32 z);
void Func_02000ebc(s32 id, s32 animation);
void Func_02000e24(struct Actor *object, s32 mode);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Func_02000ecc(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02000e7c(void);
void GameFlag_SetBit(s32 flag);

void Func_02000bc0(s32 id, s32 flag) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 gravity = leader->gravity;
  struct Actor *pillar;
  s32 i;

  Func_02000e74();
  Func_02000ef4(0);
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  pillar = Actor_GetById(id);
  Func_02000b2c(id, leader->x, leader->y, leader->z);
  Func_02000e2c(pillar, gData_020016cc);
  i = 15;
dust:
  {
    struct Actor *dust =
        Func_02000e34(255, leader->x, leader->y + 0x100000, leader->z);

    if (dust != 0) {
      s32 angle;
      s32 speed;
      s32 scale;

      Func_02000e2c(dust, gData_02001688);
      angle = Random16Far();
      speed = Random16Far();
      scale = (Random16Far() >> 1) + 0x2000;
      speed <<= 3;
      dust->velocityY = Iwram_MulQ16(speed, Math_Cosine(angle));
      dust->velocityX = Iwram_MulQ16(speed, Math_Sine(angle));
      dust->acceleration = 0;
      dust->scaleX = scale;
      dust->scaleY = scale;
      dust->gravity = 0x547a;
      dust->bounce = 0x8000;
    }
  }
  if (--i >= 0)
    goto dust;
  Func_02000f24(0x24e);
  Func_02000f24(217);
  Func_02000e54(0x20000, 0x20000, 0xe666);
  Func_02000ebc(gMainRoutineWork.currentOwner, 0x101);
  Func_02000e24(leader, 40);
  FieldEvent_Wait(40);
  Func_02000f24(204);
  leader->motionFlags = 3;
  leader->gravity = 0x547a;
  i = 0;
  goto fall;
next:
  i++;
fall:
  if (i <= 179) {
    wait(1);
    if (leader->y > leader->terrainHeight)
      goto next;
  }
  Func_02000f24(188);
  leader->gravity = gravity;
  Func_02000ebc(gMainRoutineWork.currentOwner, 0);
  Func_02000e24(leader, 38);
  Func_02000e54(0x10000, 0x40000, 0x10000);
  wait(10);
  Func_02000e54(-1, -1, 0xe666);
  wait(20);
  Camera_MoveWithinMapBounds(leader->x, leader->y, leader->z, 1);
  Func_02000ecc();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02000e24(leader, 1);
  leader->velocityY = 0x40000;
  wait(20);
  gMainRoutineWork.renderMode = 0;
  Func_02000e7c();
  GameFlag_SetBit(flag);
}
