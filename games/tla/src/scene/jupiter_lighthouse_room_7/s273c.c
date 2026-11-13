/// @file
/// Scene code of overlay 0x6a0, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_7.h"
#include "module/scene/scene.h"

void Func_02001f74(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

void Teleporter_DropIn(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 always;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  always = 1;
  i = 0;
  // A goto loop: as a do-while loop, 4 of 169 instructions differ in tla-en.
loop:
  /* FAKEMATCH(dead): the body tested on a flag that is always set; without
   * the test, 4 of 169 instructions differ in tla-en. */
  if (always) {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_02001f74(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
  }
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

extern struct CellTarget gTeleporters;

void GameFlag_SetByte(s32 flag, s32 value);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover,
                        Teleporter_Sink)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters,
                                Teleporter_DropIn)

#include "scene_tables.h"

// clang-format off
s16 gData_02002c64[] = {
    8, 16, 10, 16, 11, 16, 12, 16,
    14, 16, 15, 16, 10, 18, 12, 18,
    13, 18, 8, 20, 10, 20, 13, 20,
    15, 20, -1, 0,
};
// clang-format on
