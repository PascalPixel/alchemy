/// @file
/// The scene code of overlay 0x65d, in ROM order.

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
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gData_02000794[];

s16 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_02000794;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gData_02000fa0[];
extern struct ActorPlacement gData_020010c0[];

void *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom1)
    return gData_02000fa0;
  return gData_020010c0;
}

#include "mikasalla/local.h"
#include "event_inline.h"

struct WarpEntry {
  s32 map;
  u16 x;
  u16 z;
};

extern struct WarpEntry gData_0200073c[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_0200009c(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 id;
  s16 *choice;
  s32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    u8 *actor = (u8 *)Actor_GetById(id);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      s32 zero = 0;

      *flags = zero;
    }
  }
  choice = (s16 *)(work + 340);
  index = *choice;
  Sound_Play(158);
  index--;
  MapGrid_AnimateCells(gData_0200073c[index].map, gData_0200073c[index].x,
                 gData_0200073c[index].z);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  if (index != 4) {
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(*choice);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_0200014c(void) { Func_080d7788(18, 2, 8); }

extern struct Trigger gData_020012ac[];
extern struct Trigger gData_020010d8[];

struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gData_020012ac;
  return gData_020010d8;
}

void Func_02000188(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgFieldAreYouLookingForSomeone;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Event_End();
}

void Func_020001e0(void) {
  Map_CopyCellPictureIndicesRect(5, 49, 5, 41, 1, 1);
  MapGrid_CopyCellAttributes(4, 48, 3, 2, 4, 40);
  GameFlag_SetBit(0x85b);
}

void Func_02000218(void) {
  if (Event_GetFlag(0x200) == 0) {
    Func_02000620(0);
    Func_02000620(1);
    Func_02000620(2);
    Map_CopyCellPictureIndicesRect(56, 46, 30, 46, 16, 16);
    Map_CopyCellPictureIndicesRect(56, 174, 30, 174, 16, 16);
    MapGrid_CopyCells(28, 23, 16, 16, 15, 23);
    MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
    Map_CopyCellsRect(28, 90, 4, 4, 17, 89);
    Map_CopyCellsRect(28, 90, 2, 1, 17, 91);
    Map_CopyCellsRect(28, 90, 2, 2, 19, 91);
    Map_CopyCellsRect(28, 92, 2, 2, 15, 92);
    Map_CopyCellsRect(30, 92, 2, 2, 17, 92);
    GameFlag_SetBit(0x200);
  }
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  struct Actor *object;
  u8 flags;

  object = Actor_GetById(16);
  flags = object->priorityFlags & ~1;
  // Once-only: the once-only block holds the store below gMainRoutineWork's
  // address load, which sched2 otherwise puts above the ands (differ 2 with the
  // plain statement).
  do {
    party = &gMainRoutineWork;
  } while (0);
  object->priorityFlags = flags;
  Actor_GetById(party->currentOwner)->priorityFlags |= 0x20;
  if (party->scene == (s32)&SceneId_Ov65dRoom2) {
    if (GameFlag_Test(0x200)) {
      Func_02000620(0);
      Func_02000620(1);
      Func_02000620(2);
      Map_CopyCellPictureIndicesRect(56, 46, 30, 46, 16, 16);
      Map_CopyCellPictureIndicesRect(56, 174, 30, 174, 16, 16);
      Map_CopyCellsRect(28, 23, 16, 16, 15, 23);
      MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
      Map_CopyCellsRect(28, 90, 4, 4, 17, 89);
      Map_CopyCellsRect(28, 90, 2, 1, 17, 91);
      Map_CopyCellsRect(28, 90, 2, 2, 19, 91);
      MapGrid_CopyCells(28, 92, 2, 2, 15, 92);
      Map_CopyCellsRect(30, 92, 2, 2, 17, 92);
    }
  } else if (GameFlag_Test(0x85b)) {
    Func_020001e0();
  }
  return 0;
}

void Func_02000438(void) {
  s32 sound;
  s32 drop;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65dRoom1) {
    sound = 12;
    drop = 17;
  } else {
    sound = 3;
    drop = 9;
  }
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, drop);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(10);
  Sound_Play(123);
  Import_EventSetExitRequest(sound);
  Event_End();
}

void Func_020004ec(void) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  id = 4;
  Actor_CommitPositionAndActivate(id, 16, 0);
  Func_02000438();
}

void Func_02000514(void) {
  s32 id, x, z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * distances and their negation; as no asm, 2 of 18 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_CommitPositionAndActivate(id, -x, -z);
  Func_02000438();
}

void Func_02000544(void) {
  s32 id, a;

  Event_Start();
  Event_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * angle byte and its shift; as no asm, 2 of 11 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_ArmCallback(id, a << 8, 0);
  Func_02000438();
}

void Func_02000564(void) {
  s32 x = ((s16 *)&Actor_GetById(4)->x)[1];

  Event_Start();
  Event_ResolvePendingActions(0);
  if (x % 16 <= 7) {
    Actor_WalkByAndWait(4, -4, -4);
    Actor_WalkByAndWait(4, -4, -8);
  } else {
    Actor_WalkByAndWait(4, 4, -4);
    Actor_WalkByAndWait(4, 4, -8);
  }
  Actor_WalkToAndWait(4, 424, 99);
  Func_02000438();
}

void Func_020005d4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -4);
  Actor_WalkToAndWait(4, 424, 99);
  Func_02000438();
}

#include "text/msg_ids.h"
#include "objcmd.h"

extern s16 gData_020006f8[];
extern s16 gData_0200070e[];
extern s16 gData_02000724[];
extern s32 gData_02000850[];
extern s32 gData_02000bf8[];

// clang-format off
s16 gData_020006f8[] = {
    81, 50, 1, 2, 6, 80, 50, 1,
    2, 6, -1,
};

s16 gData_0200070e[] = {
    79, 50, 1, 2, 6, 78, 50, 1,
    2, 6, -1,
};

s16 gData_02000724[] = {
    78, 52, 2, 2, 6, 78, 53, 2,
    2, 6, -1, 0,
};

struct WarpEntry gData_0200073c[] = {
    {(s32)gData_020006f8, 84, 29},
    {(s32)gData_020006f8, 77, 34},
    {(s32)gData_0200070e, 86, 38},
    {(s32)gData_020006f8, 75, 41},
    {(s32)gData_02000724, 69, 27},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 584, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_02000794[] = {
    68, 28, 404, 76, 36, 412, -1, 1,
    244, 28, 100, 252, 36, 108, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(55),
    SCENE_EXIT_IF(1, 57, 1, -1),
    SCENE_EXIT_IF(2, 57, 2, -1),
    SCENE_EXIT_IF(3, 57, 3, -1),
    SCENE_EXIT_IF(4, 57, 4, -1),
    SCENE_EXIT_IF(5, 58, 1, -1),
    SCENE_EXIT_IF(6, 2, 14, -1),
    SCENE_EXIT_IF(7, 55, 9, -1),
    SCENE_EXIT_IF(8, 55, 10, -1),
    SCENE_EXIT_IF(9, 55, 7, -1),
    SCENE_EXIT_IF(10, 55, 8, -1),
    SCENE_EXIT_IF(11, 56, 4, -1),
    SCENE_EXIT_IF(12, 56, 1, -1),
    SCENE_EXITS(56),
    SCENE_EXIT_IF(1, 55, 12, -1),
    SCENE_EXIT_IF(2, 56, 3, -1),
    SCENE_EXIT_IF(3, 56, 2, -1),
    SCENE_EXIT_IF(4, 55, 11, -1),
    SCENE_EXITS_END,
};

s32 gData_02000850[] = {
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_SPEED, 0x9999),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    // A half turn; FACING_WEST names a heading, not a turn.
    SC_ADD(SW_FACING, -0x8000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ARG(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    // A half turn; FACING_WEST names a heading, not a turn.
    SC_ADD(SW_FACING, -0x8000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gData_02000bf8[] = {
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_SPEED, 0x4ccc),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(680)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    // A half turn; FACING_WEST names a heading, not a turn.
    SC_ADD(SW_FACING, -0x8000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ARG(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(680)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    // A half turn; FACING_WEST names a heading, not a turn.
    SC_ADD(SW_FACING, -0x8000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct ActorPlacement gData_02000fa0[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(552), FACING_SOUTH, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(600), FACING_SOUTH, 3, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(504), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 2, PIXELS(264), 0, PIXELS(680), FACING_NORTH, 0, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(664), FACING_SOUTH + FACING_STEP, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(680), FACING_NORTHEAST, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(632), FACING_NORTHWEST, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(616), FACING_SOUTHEAST, 1, 0},
    {203, CONDITION_ALWAYS, (s32)gData_02000850, PIXELS(72), 0, PIXELS(648), FACING_EAST, 0, 0},
    {81, CONDITION_ALWAYS, (s32)gData_02000bf8, PIXELS(120), 0, PIXELS(680), FACING_WEST, 0, 0},
    {245, 0x60, 1, PIXELS(312), 0, PIXELS(168), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020010c0[] = {
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct Trigger gData_020010d8[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_0200009c)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Func_020004ec)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Func_02000514)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFieldItSBeenALong)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000188)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldIWentOutToInvestigate)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouReGoingDeeper)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgFieldInTheHeartOfOsenia)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWantToGo)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouLookAcrossThe)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldATownCalledGarohLies)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgFieldBukBukBuk)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldBahBaaaaaaa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFieldWhyWouldAnyoneTravelAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFieldILikeThisTimeOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFieldOseniaWasBigAlreadyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFieldNoWayIDLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFieldAirSRockHasStood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFieldItSWeirdThatYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFieldItSBitterColdIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFieldThePeopleInGarohAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgFieldTheGroundHereIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgFieldOhYeahMmmIJust)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_0200014c)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldItSCompletelyFullOf))},
    {0x50008805, 50, CONDITION_ALWAYS, EVENT(Func_020001e0)},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_020012ac[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Func_020004ec)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Func_02000514)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(Func_02000564)},
    {0xc602, 34, CONDITION_ALWAYS, EVENT(Func_020005d4)},
    {0x50008a05, 60, 0x200, EVENT(Func_02000218)},
    {TRIGGERS_END},
};
