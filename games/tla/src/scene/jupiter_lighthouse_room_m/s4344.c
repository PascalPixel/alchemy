/// @file
/// Scene code of overlay 0x6a3, in ROM order, from s4344's offset up to the next object the overlay links, and the overlay's data.

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
#include "module/scene/jupiter_lighthouse_room_m.h"
#include "module/scene/scene.h"

void Func_02003b7c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_02004344(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  CamBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_02003b7c(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

/* VIEW(s4344): routine GameFlag_SetByte as void GameFlag_SetByte(s32 flag, s32 value); game_flags.h has u32 GameFlag_SetByte(u32 flag, u8 value). */
extern void GameFlag_SetByte__s4344(s32 flag, s32 value) __asm__("GameFlag_SetByte");

s32 Func_020044dc(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &Data_02005b88;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_0200413c(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_020041ac(work, entry);
      GameFlag_SetByte__s4344(work->flagByte, x);
      GameFlag_SetByte__s4344(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void Func_0200458c(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(Data_02005b88.flagByte);
  s32 dz = FieldGameFlag_GetByte(Data_02005b88.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte__s4344(Data_02005b88.flagByte, 0);
    GameFlag_SetByte__s4344(Data_02005b88.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Map_CenterOnFollowedPoint();
    Func_02004344();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "scene_tables.h"
#include "objcmd.h"

// clang-format off
u32 gData_020049cc[] = {
    1, 39, 1, 17,
};

u32 gData_020049dc[] = {
    1, 39, 1, 17,
};

u32 gData_020049ec[] = {
    1, 39, 1, 17,
};

u32 gData_020049fc[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

u32 gData_02004a18[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

u32 gData_02004a34[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

u32 gData_02004a50[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gData_02004a6c[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004a90[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004ab4[] = {
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(699), PIXELS(170)),
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004ae8[] = {
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(679), PIXELS(170)),
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004b1c[] = {
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(659), PIXELS(170)),
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};
// clang-format on
