#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"


extern const s32 gData_02005050[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e20(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001d04(void);

static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, 194 << 18, 248 << 16);
  Actor_ResetAndSetPosition(id, x, z);
  Actor_GetById(id)->velocityY = 0xc0000;
  Actor_GetById(id)->gravity = 0x8000;
  Actor_GetById(id)->action = 0;
  Actor_GetById(id)->update = Func_02001d04;
}

void Func_02001d64(void) {
  s32 position[3];
  struct EffectOptions options;
  s32 i;

  Func_02004d40();
  Event_ResolvePendingActions(0);
  Sound_Play(107);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  Event_Wait(30);
  Camera_MoveWithinMapBounds(194 << 18, -1, 136 << 17, 1);
  Func_02004e20();
  options.type = 403;
  options.priority = 2;
  options.palette = 10;
  options.script = gData_02005050;
  options.startScaleY = 0x18000;
  options.startScaleX = 0x18000;
  position[1] = 0;
  i = 0;
loop:
  position[0] = ((Random16Far() & 31) << 16) + (190 << 18);
  position[2] = ((Random16Far() & 31) << 16) + (128 << 17);
  Func_0200015c(position[0], position[1], position[2], 0, 0x10000, 0, 0x3b0000,
                &options);
  if ((i & 7) == 0)
    Sound_Play(567);
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
    Actor_SetPosition(12, 194 << 18, 248 << 16);
    Actor_SetDestination(12, 696, 232);
    FieldCamera_MoveTo(174 << 18, -1, 232 << 16, 1);
    Actor_GetById(12)->velocityY = 0xd0000;
    Actor_GetById(12)->gravity = 0x8000;
    Actor_GetById(12)->action = 0;
    Actor_GetById(12)->update = Func_02001d04;
    MapGrid_SetShake(-1, -1, 0xe666);
  }
  wait(3);
  i++;
  if (i != 128)
    goto loop;
  FieldActor_WaitForMoveById(12);
  MapGrid_WaitForShakeToSettle();
  Sound_Play(288);
  Event_Wait(40);
  Event_SetFlag(2552);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004e20();
  wait(10);
  Func_02004d48();
}
