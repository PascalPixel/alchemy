// Draft, differ 277 of 405: a Magma Rock event: sound 107 with the screen shaken,
// the camera to (776, 272), and for 128 steps of 3 frames effects rain over a
// 32 by 32 area (sound 567 every 8th) while objects 13, 20, 14, 19, 15, 18,
// 16 and 17 arrive one by one at their places from (776, 248), each falling in
// on its update (Func_02001d04); object 12 comes last with the camera and the
// shaking eased; then it settles, sound 288, flag 2552 is set and the camera
// returns to the leader.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "field_effect.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_02004e90(s32 value);
void Func_02004da0(s32 id, s32 x, s32 z);

extern const s32 gData_02005050[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004d18(void);
void Func_02004f30(s32 sound);
void Func_02004d10(s32 a, s32 b, s32 c);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02004e20(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02001d04(void);

static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Func_02004da0(id, 194 << 18, 248 << 16);
  Actor_ResetAndSetPosition(id, x, z);
  Actor_GetById(id)->velocityY = 0x60000;
  Actor_GetById(id)->gravity = 0x8000;
  Actor_GetById(id)->action = 0;
  Actor_GetById(id)->update = Func_02001d04;
}

void Func_02001d64(void) {
  struct EffectOptions options;
  s32 position[3];
  s32 i;

  Func_02004d40();
  Func_02004e90(0);
  Func_02004f30(107);
  Func_02004d10(0x30000, 0x30000, 0x10000);
  FieldEvent_Wait(30);
  Camera_MoveWithinMapBounds(194 << 18, -1, 136 << 17, 1);
  Func_02004e20();
  options.type = 403;
  options.priority = 2;
  options.palette = 10;
  options.script = gData_02005050;
  options.startScaleY = 0x18000;
  options.startScaleX = 0x18000;
  position[1] = 0;
  for (i = 0; i != 128; i++) {
    position[0] = ((Random16Far() & 31) << 16) + (190 << 18);
    position[2] = ((Random16Far() & 31) << 16) + (128 << 17);
    Func_0200015c(position[0], position[1], position[2], 0, 0x10000, 0,
                  0x3b0000, &options);
    if ((i & 7) == 0)
      Func_02004f30(567);
    if (i == 32) {
      Arrive(13, 744, 216);
    } else if (i == 40) {
      Arrive(20, 776, 328);
    } else if (i == 48) {
      Arrive(14, 856, 232);
    } else if (i == 56) {
      Arrive(19, 696, 312);
    } else if (i == 64) {
      Arrive(15, 728, 280);
    } else if (i == 72) {
      Arrive(18, 824, 280);
    } else if (i == 80) {
      Arrive(16, 824, 312);
    } else if (i == 88) {
      Arrive(17, 776, 232);
    } else if (i == 112) {
      Func_02004da0(12, 194 << 18, 248 << 16);
      Actor_ResetAndSetPosition(12, 696, 232);
      Camera_MoveWithinMapBounds(174 << 18, -1, 232 << 16, 1);
      Actor_GetById(12)->velocityY = 0x68000;
      Actor_GetById(12)->gravity = 0x8000;
      Actor_GetById(12)->action = 0;
      Actor_GetById(12)->update = Func_02001d04;
      Func_02004d10(-1, -1, 0xe666);
    }
    wait(3);
  }
  Actor_WaitForMoveById(12);
  Func_02004d18();
  Func_02004f30(288);
  FieldEvent_Wait(40);
  GameFlag_SetBit(2552);
  Camera_FollowActor(gAdventure.currentOwner, 1);
  Func_02004e20();
  wait(10);
  Func_02004d48();
}
