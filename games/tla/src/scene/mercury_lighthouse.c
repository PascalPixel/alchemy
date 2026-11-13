/// @file
/// The scene code of overlay 0x6b2, in ROM order.

#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "actor.h"
#include "scene_tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "task.h"
#include "io_reg.h"

#include "objcmd.h"
#include "frame.h"
#include "event_work.h"
#include "module/scene/mercury_lighthouse.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

s32 Func_02000038(struct Actor *object) {
  if ((gData_020007e4 >> 1) & 1)
    Actor_SetSpriteColor(object, 15);
  else
    Actor_SetSpriteColor(object, 7);
  return 0;
}

void Func_02000060(struct Actor *object) {
  u8 *timer = (u8 *)object + 98;
  u8 *state;

  if (*timer != 0) {
    (*timer)--;
    return;
  }
  *timer = ((Random16Far() * 80) >> 16) + 40;
  state = (u8 *)object + 99;
  switch (*state) {
  case 0:
    object->facing = FACING_WEST;
    *state = 1;
    break;
  case 1:
    object->facing = FACING_NORTHWEST + FACING_STEP;
    *state = 0;
    break;
  case 2:
    object->facing = 0;
    *state = 3;
    break;
  case 3:
    object->facing = FACING_NORTH + FACING_STEP;
    *state = 2;
    break;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Actor_StartScript(struct Actor *object, const void *script);

void Func_020000f4(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 phase;
  s32 x;
  s32 z;

  Actor_SetSpritePriority(8, 3);
  Actor_SetSpritePriority(9, 3);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  phase = gFrameCount & 15;
  if (phase == 0) {
    object = ((struct Actor *)Import_ActorCreate(0x2a2, 0, 0, 0));
    if (object != NULL) {
      sprite = object->sprite;
      x = Random16Far() * 40 + 0x32c0000;
      /* FAKEMATCH(temporary): z is built before the motion flag and x are
       * stored, as in the native; as object->z = 0x2b80000, 12 of 76
       * instructions differ in tla-en. */
      z = 0x2b80000;
      object->motionFlags = phase;
      object->x = x;
      object->y = PIXELS(160);
      object->z = z;
      Actor_SetSingleChildFlags(object, 0);
      object->priorityFlags &= ~1;
      sprite->priority = 2;
      Actor_StartScript(object, gData_02000618);
      Func_02000540(object, 1);
    }
  }
  gData_020007e4++;
}

void Func_020001b0(void) {
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  wait(1);
  gData_020007e4 = 0;
  Task_Add(Func_020000f4, TASK_PASS_VBLANK);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSpritePriority(9, 2);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Func_080dc978(13, 14);
  Event_SetMessageId(MsgImilVoicesDisbeliefReply);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Actor_ShowEmote(8, 256, 0);
  Actor_ShowEmote(8, 256, 20);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_Jump(8, 4, 10);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(11, 257, 0);
  Actor_ShowEmote(12, 257, 40);
  Actor_FaceDirection(11, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Actor_SetAnimation(9, 3);
  Event_ShowMessage(0x8009, 0);
  Actor_FaceDirectionAndWait(12, FACING_EAST);
  Event_ShowMessage(0x800c, 0);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(0x400b, 0);
  actor = Actor_GetById(11);
  *((u8 *)actor + 98) = ((Random16Far() * 80) >> 16) + 40;
  *((u8 *)actor + 99) = 0;
  actor->update = (void (*)(void))Func_02000060;
  actor = Actor_GetById(12);
  {
    /* FAKEMATCH(temporary): the timer's + 40 comes before its address, as in
     * the native; as a direct store, 2 of 303 instructions differ in tla-en. */
    u8 timer = ((Random16Far() * 80) >> 16) + 40;

    *((u8 *)actor + 98) = timer;
  }
  *((u8 *)actor + 99) = 2;
  actor->update = (void (*)(void))Func_02000060;
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Actor_SetAnimation(8, 4);
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Event_ShowMessage(0x8009, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Event_ShowMessage(0x8009, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(1, 0);
  Func_02000608();
  Actor_Jump(9, 2, 0);
  FieldActor_ShowEmote(9, 263, 20);
  Event_ShowMessage(0x8009, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(3, 0);
  Func_02000608();
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(0x8008, 0);
  Func_080dc978(13, 14);
  Event_ShowMessage(6, 0);
  Func_02000608();
  Event_Wait(20);
  *(u16 *)BG_PLTT = 0;
  wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 16;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(0xa3c);
  Import_EventSetExitRequest(3);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.entrance == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(201);
    BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(20);
    Import_EventSetExitRequest(9);
  } else {
    Func_020001b0();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 gData_02000618[] = {
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_FREE,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(309),      SCENE_EXIT(2, 309, 2), SCENE_EXIT(3, 297, 5),
    SCENE_EXIT(9, 309, 2), SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    {191, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(696), FACING_SOUTH + FACING_STEP, 2, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {190, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTHWEST + FACING_STEP, 2, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(906), 0, PIXELS(744), FACING_WEST, 2, 0},
    {194, CONDITION_ALWAYS, 1, PIXELS(782), 0, PIXELS(762), FACING_EAST, 2, 0},
    {3, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {1, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(780), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGERS_END},
};
