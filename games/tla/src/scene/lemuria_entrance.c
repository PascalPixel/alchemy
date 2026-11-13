/// @file
/// Overlay 0x688's scene: its code, then its tables, in the order the ROM has them.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "event_work.h"
#include "call.h"

struct EventWorkRoute {
  u8 unk000[0x1ac];
  s32 route;
  u8 unk1b0[4];
  s32 countdown;
};

struct EventWork1c4 {
  u8 unk000[0x1c4];
  s16 message;
};

struct AnimationRecord26 {
  u8 unk00[0x1a];
  u8 frame;
};

extern u8 MsgFieldAddressCompanion;
extern u32 gData_020005e4[];

s32 BattleEffect_GetResourceId(s32 id);
void Text_GetMessageSize(s32 id, s32 *a, s32 *b, s32 *c, s32 *d);
void Message_OpenWindow(s32 id, s32 x, s32 y, u32 flags);
s32 Message_AllTyped(void);
void Window_Close(s32 work, s32 mode);
void Message_CloseFinishedWindows(void);
s32 Func_0200053c(s32 a, s32 b, s32 c, s32 d);
void Func_0200055c(void);
void Func_02000564(void);
void Func_02000310(s32 kind, s32 frames);

s32 Func_02000038(struct Actor *object) {
  ((struct AnimationRecord26 *)object->sprite)->frame = 0;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000060(s32 arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  party->ownerOverride = 0;
  Event_ClearFlag(0x960);
  Event_PlaySound(123);
  party->currentOwner = 8;
  ((struct EventWorkRoute *)Ram_HeapSlots->eventWork)->route = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->currentOwner = 4;
  FieldEvent_SetExitRequest(arg);
}

void Func_020000b0(s32 arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  party->ownerOverride = 0;
  Event_SetFlag(0x960);
  Event_PlaySound(123);
  party->currentOwner = 8;
  ((struct EventWorkRoute *)Ram_HeapSlots->eventWork)->route = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->currentOwner = 4;
  FieldEvent_SetExitRequest(arg);
}

void Func_02000100(void) {
  Event_SetFlag(0x909);
  Actor_SetPosition(19, 0, 0);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_02000120(void) {
  if (Event_GetFlag(0x800) != 0) {
    Func_0200055c();
    FieldEvent_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    {
      struct MainRoutineWork *state = &gMainRoutineWork;

      state->renderMode = 6;
      state->ownerOverride = 1;
    }
    Actor_SetAnimation(8, 7);
    Func_02000564();
  } else {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->currentOwner = 4;
    party->renderMode = 0;
    party->ownerOverride = 0;
  }
}

// The scene's setup: places its actors, then plays what the entrance it was
// reached by calls for.
extern char SceneId_Ov688;

void Func_020005a4(s32 scene, s32 entrance);
void Func_020003cc(void);

/// The scene's setup: places its actors, then plays what the entrance it was
/// reached by calls for.
s32 Scene_Initialize(void) {
  s32 *screen = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  s32 effect = 0x204;

  *screen = effect;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_SetSingleChildFlags(Actor_Get(19), 0);
  Event_SetFlag(0x907);
  Event_ClearFlag(0x9b4);
  if (Event_GetFlag(0x909))
    Actor_SetPosition(19, 0, 0);
  Actor_SetSingleChildFlags(Actor_Get(9), 0);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  Actor_SetSingleChildFlags(Actor_Get(13), 0);
  Actor_SetSingleChildFlags(Actor_Get(14), 0);
  Actor_SetSingleChildFlags(Actor_Get(15), 0);
  Actor_SetSingleChildFlags(Actor_Get(16), 0);
  Actor_SetSingleChildFlags(Actor_Get(17), 0);
  Actor_SetSingleChildFlags(Actor_Get(18), 0);
  if (gMainRoutineWork.entrance == 90) {
    gMainRoutineWork.ownerOverride = 1;
    Func_020005a4((s32)&SceneId_Ov688, 1);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5) {
    gMainRoutineWork.currentOwner = 8;
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gMainRoutineWork.currentOwner = 4;
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  }
  /* FAKEMATCH(wrapper): forwarded, so that the flag number is built for each call; as
   * Event_GetFlag(0x96b), 15 of 153 instructions differ in tla-en. */
  if (gMainRoutineWork.entrance == 3 && Event_GetFlag(0x96b)) {
    Event_ClearFlag(0x96b);
    Func_020003cc();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000310(s32 kind, s32 frames) {
  // This routine's own view of the event work. A union: the read of width
  // keeps its place after this store.
  struct EventWork1c4 {
    u8 unk000[0x1c4];
    union {
      s16 message;
      s32 word;
    } u;
  };
  struct EventWork1c4 *work = Ram_HeapSlots->eventWork;
  s32 b;
  s32 a;
  s32 height;
  s32 width;
  s32 resource;
  s32 id;
  s32 x;

  width = 0;
  height = 0;
  id = 56;
  if (kind != 8)
    id = kind;
  resource = BattleEffect_GetResourceId(id);
  if (kind == 7) {
    width = 10;
    height = 10;
    Text_GetMessageSize(work->u.message, &width, &height, &a, &b);
    x = width + a - 5;
  } else {
    width = 5;
    height = 10;
    x = 5;
  }
  Call4((void (*)())Message_OpenWindow, work->u.message++, width, height,
        (resource << 16) | 2);
  x = Func_0200053c(id, 0, x, 5);
  while (Message_AllTyped() == 0)
    System_Wait(1);
  FieldEvent_Wait(frames);
  Window_Close(x, 2);
  Message_CloseFinishedWindows();
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                 s32 acceleration) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, so the id is
   * set between the speed's last add and the call; as a direct call to
   * Actor_SetSpeedParameters, 2 of 113 instructions differ in tla-en. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_020003cc(void) {
  struct EventWork *work;
  u8 saved;

  saved = gMainRoutineWork.eventMode;
  gMainRoutineWork.eventMode = 2;
  Func_0200055c();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeedParameters_(8, 0x451e, 0x228f);
  Actor_SetScript(8, gData_020005e4);
  FieldEvent_Wait(80);
  FieldEvent_SetMessageId((s32)&MsgFieldAddressCompanion);
  Func_02000310(6, 90);
  Func_02000310(7, 90);
  Func_02000310(6, 180);
  Func_02000310(7, 90);
  Func_02000310(6, 250);
  Func_02000310(7, 180);
  Func_02000310(5, 250);
  Func_02000310(8, 90);
  Func_02000310(7, 180);
  Func_02000310(8, 180);
  Func_02000310(7, 180);
  Func_02000310(6, 90);
  Func_02000310(7, 250);
  Func_02000310(5, 250);
  Func_02000310(7, 250);
  Func_02000310(6, 90);
  Func_02000310(5, 90);
  Func_02000310(8, 90);
  Func_02000310(7, 250);
  Func_02000310(4, 90);
  Func_02000310(7, 250);
  FieldActor_RefreshSelectorById(8);
  FieldEvent_Wait(10);
  work = Ram_HeapSlots->eventWork;
  work->delay = 30;
  work->screenEffect = 0x204;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
  gMainRoutineWork.eventMode = saved;
  Func_02000564();
}

#include "objcmd.h"

extern s32 gData_02000728[];

// clang-format off
u32 gData_020005e4[] = {
    SC_MOVE(PIXELS(728), 0, PIXELS(160)),
    SC_SYNC,
    SC_MOVE(PIXELS(608), 0, PIXELS(152)),
    SC_SYNC,
    SC_MOVE(PIXELS(488), 0, PIXELS(152)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), 0, PIXELS(152)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), 0, PIXELS(112)),
    SC_SYNC,
    SC_MOVE(PIXELS(336), 0, PIXELS(88)),
    SC_SYNC,
    SC_MOVE(PIXELS(320), 0, PIXELS(88)),
    SC_SYNC,
    SC_MOVE(PIXELS(288), 0, PIXELS(120)),
    SC_SYNC,
    SC_MOVE(PIXELS(288), 0, PIXELS(176)),
    SC_SYNC,
    SC_MOVE(PIXELS(272), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(272), 0, PIXELS(280)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(280)),
    SC_SYNC,
    SC_MOVE(PIXELS(384), 0, PIXELS(320)),
    SC_SYNC,
    SC_MOVE(PIXELS(384), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(416), 0, PIXELS(440)),
    SC_SYNC,
    SC_MOVE(PIXELS(416), 0, PIXELS(552)),
    SC_SYNC,
    SC_END,
};
// clang-format on

s32 gData_02000728[] = {
    SC_CALL(Func_02000038),
    SC_ARG(7),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(199),
    SCENE_EXIT_IF(1, 2, 45, -1),
    SCENE_EXIT_IF(2, 199, 4, -1),
    SCENE_EXIT_IF(3, 200, 1, -1),
    SCENE_EXIT_IF(4, 199, 2, -1),
    SCENE_EXIT_IF(5, 200, 1, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {8, CONDITION_ALWAYS, (s32)gData_02000728, PIXELS(14), 0, PIXELS(4),
     FACING_NORTH, 2, 0},
    {342, CONDITION_ALWAYS, 1, PIXELS(636), 0, PIXELS(112), FACING_WEST, 2, 0},
    {342, CONDITION_ALWAYS, 1, PIXELS(668), 0, PIXELS(112), FACING_EAST, 2, 1},
    {342, CONDITION_ALWAYS, 1, PIXELS(668), 0, PIXELS(216), FACING_EAST, 2, 1},
    {342, CONDITION_ALWAYS, 1, PIXELS(732), 0, PIXELS(216), FACING_WEST, 2, 1},
    {11, CONDITION_ALWAYS, 1, PIXELS(736), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {11, CONDITION_ALWAYS, 1, PIXELS(640), 0, PIXELS(208), FACING_SOUTH, 2, 1},
    {11, CONDITION_ALWAYS, 1, PIXELS(704), 0, PIXELS(208), FACING_SOUTH, 2, 1},
    {11, CONDITION_ALWAYS, 1, PIXELS(52), 0, PIXELS(796), FACING_WEST, 2, 0},
    {11, CONDITION_ALWAYS, 1, PIXELS(52), 0, PIXELS(828), FACING_WEST, 2, 1},
    {11, CONDITION_ALWAYS, 1, PIXELS(52), 0, PIXELS(860), FACING_WEST, 2, 1},
    {424, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(160), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 4, CONDITION_ALWAYS, 4},
    {2, 3, CONDITION_ALWAYS, EVENT(Func_02000060)},
    {2, 5, CONDITION_ALWAYS, EVENT(Func_020000b0)},
    {0x9815, 19, CONDITION_ALWAYS, EVENT(Func_02000100)},
    {TRIGGERS_END},
};
