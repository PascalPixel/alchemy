#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern s32 gData_02005af0;
struct Actor *Func_02000624(struct Actor *origin, s32 id);
void Func_020006ac(struct Actor *object, s32 dx, s32 dz, s32 velocity);
void Func_020056b4(struct Actor *object);
void Func_020056bc(void);

void Func_02000c38(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
    struct Actor *found = Func_02000624(actor, 359);
    s32 dx = *speed << 20;

    if (found != NULL) {
      u16 facing = *(u16 *)((u8 *)found + 6);

      /* FAKEMATCH: ⚓️ reads the facing before it builds the half turn */
      asm volatile("");
      if (facing == 0x8000)
        dx = -dx;
      facing = *(u16 *)((u8 *)actor + 6);
      if ((facing == 0 && dx > 0) || (facing == 0x8000 && dx < 0))
        Func_020006ac(actor, dx, 0, 0x40000);
    }
  }
}

void Func_02000c8c(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
    struct Actor *found = Func_02000624(actor, 360);
    s32 dz = *speed << 20;
    u16 facing;

    if (found == NULL) {
      found = Func_02000624(actor, 361);
      dz = -dz;
      if (found == NULL)
        return;
    }
    facing = *(u16 *)((u8 *)actor + 6);
    /* FAKEMATCH: ⚓️ reads the facing before it builds the quarter turn */
    asm volatile("");
    if ((facing == 0x4000 && dz > 0) || (facing == 0xc000 && dz < 0))
      Func_020006ac(actor, 0, dz, 0x40000);
  }
}

void Func_02000cf0(struct Actor *object) {
  Func_020056b4(object);
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Actor_SetHorizontalPositionWithTerrain(39, 164 << 17, 148 << 17);
  Event_Wait(30);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Event_Wait(15);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 39);
  Func_080d47b4(gMainRoutineWork.currentOwner, 257, 30);
  Camera_FollowActor(39, 1);
  Event_Wait(30);
  Actor_GetById(39)->priorityFlags |= 0x20;
  Actor_SetModeById(39, 1);
  Actor_WalkToAndWait(39, 320, 240);
  Actor_WalkToAndWait(39, 304, 216);
  Actor_WalkToAndWait(39, 280, 216);
  Actor_SetPositionAndReset(39, 280, 184);
  Event_Wait(20);
  Actor_SetAngleToward(39, gMainRoutineWork.currentOwner, 0);
  Event_Wait(10);
  Sound_Play(262);
  {
    s32 a = 256, b = 30;

    /* FAKEMATCH: the native sets the id last; called plainly or through an
     * inline wrapper the id is set before the 30 (one reordered pair, differ
     * 2). */
    asm volatile("" : "+l"(a), "+l"(b));
    Func_080d47b4(39, a, b);
  }
  Actor_WalkToAndWait(39, 344, 184);
  Actor_SetPositionAndReset(39, 344, 200);
  Event_Wait(2);
  Actor_ArmCallback(39, FACING_WEST, 0);
  Event_Wait(15);
  Sound_Play(152);
  Actor_GetById(39)->velocityY = PIXELS(4);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 312, 200);
  Sound_Play(152);
  Actor_GetById(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 312, 168);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_WalkToAndWait(39, 328, 152);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_Wait(15);
  Sound_Play(152);
  Actor_GetById(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 360, 152);
  Event_Wait(15);
  Sound_Play(152);
  Actor_GetById(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_WalkToAndWait(39, 360, 184);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_WalkToAndWait(39, 376, 184);
  Actor_WalkToAndWait(39, 392, 200);
  Actor_SetPositionAndReset(39, 408, 184);
  Event_Wait(15);
  Sound_Play(152);
  Actor_GetById(39)->velocityY = PIXELS(6);
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_WalkToAndWait(39, 440, 184);
  Actor_SetHorizontalPositionWithTerrain(39, 0, 0);
  GameFlag_SetBit(0xa6c);
  Func_020056bc();
}
