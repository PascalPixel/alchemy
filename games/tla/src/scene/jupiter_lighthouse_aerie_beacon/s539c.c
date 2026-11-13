/// @file
/// Scene code of overlay 0x6a5, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_aerie_beacon.h"
#include "module/scene/scene.h"

void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) {
  Camera_MoveWithinMapBounds(x, y, z, use_setter);
}

static __inline__ void Func_020057ac_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Teleporter_DropIn(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (i = 0; i <= 16; i++) {
    s32 angle = i << 12;

    velocity.x = Math_Cosine(angle);
    velocity.y = 0;
    velocity.z = Math_Sin(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
    Func_02004bd4(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                  velocity.z, 0x1090001, &options);
  }
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  Func_020057ac_(0x50000, 0x50000, 0x10000);
  Func_020057ac_(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_0200581c();
}

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_SinkEvent(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_TELEPORTERS_STEP(Teleporters_Step, gTeleporters, Teleporter_Uncover, Teleporter_SinkEvent)

DEFINE_TELEPORTER_PLACE_ARRIVAL(Teleporter_PlaceArrival, gTeleporters, Teleporter_DropIn)

#include "scene_tables.h"

// clang-format off
u32 gData_020059fc[] = {
    29, 27, 28, 64,
};
// clang-format on

u32 gData_02005a0c[] = {
    0,
    64,
};

u32 gData_02005a14[] = {
    4,
    64,
};

u32 gData_02005a1c[] = {
    64,
};

u32 gData_02005a20[] = {
    64,
};
