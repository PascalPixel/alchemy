#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "frame.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "system/vector3.h"

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02001fcc();
void Func_02001fd4();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02001fcc();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fd4();
}

extern u8 gData_02002368[];
extern u8 gData_02002378[];

void Func_020000c8(s32 kind) {
  const u8 *data = gData_02002368;

  if (kind == 3)
    data += 8;
  if (kind == 16)
    data = gData_02002378;
  Func_02000054(data, kind, 0);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern s16 gData_02002608[];

static inline u32 Frame_Mod6(u32 frame) {
  /* FAKEMATCH(wrapper): the inline keeps gFrameCount's address out of a
   * callee-saved register; as gFrameCount % 6, 16 of 35 instructions differ in
   * tla-en. */
  return frame % 6;
}

/// Every sixth frame rotates colours 1-14 of background palette 12 by one
/// place; every frame sets its colour 15 from a twelve-step cycle.
void Func_020000f4(void) {
  /* FAKEMATCH(pin): the colour stays in r1; as a plain local, 4 of 35
   * instructions differ in tla-en. */
  register s32 color asm("r1");

  if (Frame_Mod6(gFrameCount) == 0) {
    volatile u16 *palette;
    /* FAKEMATCH(pin): the saved colour takes r1 and leaves r0 to the index; as
     * a plain local, 12 of 35 instructions differ in tla-en. */
    register s32 saved asm("r1");
    s32 index;

    /* FAKEMATCH(pin): the read stays in r3; as a plain local, 6 of 35
     * instructions differ in tla-en. */
    register s32 raw asm("r3") = *(volatile u16 *)&BG_PLTT_COLOR(12, 14);

    /* FAKEMATCH(barrier): the barrier keeps the sign extension apart from
     * the read; without it, 6 of 35 instructions differ in tla-en. */
    asm("" : "+r"(raw));
    saved = (s16)raw;
    index = 14;
  loop:
    palette = (volatile u16 *)&BG_PLTT_COLOR(12, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto loop;
    *(volatile u16 *)&BG_PLTT_COLOR(12, 1) = saved;
  }
  color = gData_02002608[(gFrameCount >> 2) % 12];
  *(volatile u16 *)&BG_PLTT_COLOR(12, 15) = color;
}

void Func_020000f4(void);

/// The room's setup: it schedules the palette cycle Func_020000f4.
s32 Func_02000158(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Task_AddFrame(Func_020000f4, 0xc80);
  FieldActor_SetSpritePriority(8, 3);
  if (gMainRoutineWork.entrance == 6)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(0x96a)) {
    Actor_SetPosition(7, 0, 0);
    Actor_SetPosition(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02001f9c(struct Actor *object);

void Func_020001d8(void) {
  struct Actor *object = Actor_Get(13);
  struct Actor *first;
  struct Actor *second;

  Actor_SetAnimation(13, 5);
  FieldEvent_Wait(28);
  Event_PlaySound(112);
  first =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  first->speedLimit = 0x20000;
  first->acceleration = 0x20000;
  first->motionFlags = 0;
  FieldActor_SetMoveTarget(first, first->x + PIXELS(6), first->y + PIXELS(8),
                      first->z);
  second =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  second->speedLimit = 0x20000;
  second->acceleration = 0x20000;
  second->motionFlags = 0;
  FieldActor_SetMoveTarget(second, first->x + PIXELS(8), first->y - PIXELS(4),
                      first->z);
  Actor_WaitForMove(first);
  Func_02001f9c(first);
  Actor_WaitForMove(second);
  Func_02001f9c(second);
  FieldEvent_Wait(30);
}

extern u8 gData_0200308c[];
extern u8 Data_02003098[];
extern u8 Data_02002fd7[];
extern u8 gData_02002fe8[];
extern u8 Data_02003090[];
extern u8 Data_02003024[];
extern u8 Data_02003060[];
extern u8 Data_02003030[];
s32 __modsi3();
s32 Func_020020d4();
s32 Func_0200210c();

void Func_020002b4(void) {
  struct Vector3 position;
  s8 mode = gData_0200308c[0];

  if (mode == 1) {
    s32 *time = (s32 *)Data_02003098;
    s32 t = *time;
    s32 level = Data_02002fd7[(t >> 3) & 15];

    {
      s32 value = level << 8;
      /* FAKEMATCH(dead): an empty block holds the shift first; with no
       * block, 2 of 96 instructions differ in tla-en. */
      do {
      } while (0);
      value |= 16 - level;

      REG_BLDALPHA = value;
    }
    /* FAKEMATCH(dead): the step in a once-only block holds it after the
     * store; as the plain statement, 2 of 96 instructions differ in tla-en. */
    do {
      t++;
    } while (0);
    *time = t;
  } else if (mode == 2) {
    s32 *time = (s32 *)Data_02003098;
    s32 step = (*time >> 2) % 28;

    {
      u16 *table = (u16 *)gData_02002fe8;
      /* FAKEMATCH(temporary): the offset in a local puts the table first in the
       * load; as table[step], 2 of 96 instructions differ in tla-en. */
      s32 at = step * 2;
      s32 value = *(u16 *)((u8 *)table + at);

      REG_BLDALPHA = value;
    }
    if (step == 0) {
      *(s32 *)Data_02003090 = 1;
      *(s32 *)Data_02003024 = step;
      Event_PlaySound(153);
    }
    if (step == 14) {
      *(s32 *)Data_02003090 = 0;
      *(s32 *)Data_02003024 = 1;
      Event_PlaySound(153);
    }
    (*time)++;
  }
  position.x = 0x3400000;
  position.y = 0;
  position.z = 0x1c00000;
  Func_020020d4(&position);
  if (*(s32 *)Data_02003090 != 0) {
    ((struct Actor *)Data_02003060)->y = position.x;
    ((struct Actor *)Data_02003060)->z = position.z;
    Func_0200210c(Data_02003060);
  }
  if (*(s32 *)Data_02003024 != 0) {
    ((struct Actor *)Data_02003030)->y = position.x;
    ((struct Actor *)Data_02003030)->z = position.z;
    Func_0200210c(Data_02003030);
  }
}
