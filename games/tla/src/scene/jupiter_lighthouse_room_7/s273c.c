/// @file
/// Scene code of overlay 0x6a0, in ROM order, from s273c's offset up to the
/// next object the overlay links, and the
/// overlay's data.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"

void Func_02002b6c(void);
void Func_02002afc(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02001f74(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02002b74(void);

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which keeps
   * each -1 in its own register; as a direct call, 8 of 169 instructions
   * differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void Func_02002b3c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): the calls forwarded through the inline, which keeps
   * each call's constants in their own registers; as direct calls, 33 of
   * 169 instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

void Func_0200273c(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 always;

  Func_02002b6c();
  FieldEvent_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_02002afc();
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
  Func_02002b3c_(0x50000, 0x50000, 0x10000);
  Func_02002b3c_(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_02002b74();
}

extern struct CellTarget gData_02003704;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002534(struct CellTarget *work, struct CellTargetEntry *entry);
void Func_020025a4(struct CellTarget *work, struct CellTargetEntry *entry);

s32 Func_020028d4(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &gData_02003704;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        Func_02002534(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      Func_020025a4(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

void Func_02002984(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;
  s32 dx = FieldGameFlag_GetByte(gData_02003704.flagByte);
  s32 dz = FieldGameFlag_GetByte(gData_02003704.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gData_02003704.flagByte, 0);
    GameFlag_SetByte(gData_02003704.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->marker;
    marker->x = leader->x;
    marker->z = leader->z;
    Func_02002afc();
    Func_0200273c();
    ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = effect;
  }
}

#include "scene_tables.h"

// clang-format off
s16 gData_02002c64[] = {
    8, 16, 10, 16, 11, 16, 12, 16,
    14, 16, 15, 16, 10, 18, 12, 18,
    13, 18, 8, 20, 10, 20, 13, 20,
    15, 20, -1, 0,
};
// clang-format on
