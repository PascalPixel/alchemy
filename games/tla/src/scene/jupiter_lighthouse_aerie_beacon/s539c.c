/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s539c's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_0200577c(void);
void Func_02005814(void);
void Func_0200581c(void);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) {
  Camera_MoveWithinMapBounds(x, y, z, use_setter);
}

static __inline__ void Func_020057ac_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_0200539c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Func_02005814();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_0200577c();
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

extern struct CellTarget Data_02006b74;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02005194(struct CellTarget *work,
                   struct CellTargetEntry *entry);
void Func_02005204(struct CellTarget *work,
                   struct CellTargetEntry *entry);

s32 Func_02005534(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &Data_02006b74;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02005194(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_02005204(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}
