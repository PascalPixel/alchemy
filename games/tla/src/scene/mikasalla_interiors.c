/// @file
/// Overlay 0x65e's scene: its code, then its tables, in the order the ROM has them.

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
#include "actor.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gData_02000840[];
extern struct ActorPlacement gData_02000960[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2)
    return (const struct ActorPlacement *)gData_02000960;
  return (const struct ActorPlacement *)gData_02000840;
}

void Event_Start();
void Event_End();

s32 Func_02000078(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors18);
  Event_ShowMessage(8, 0);
  Event_End();
  return 0;
}

s32 Func_020000a0(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors32);
  Event_ShowMessage(9, 0);
  Event_End();
  return 0;
}

extern struct Trigger gData_02000a20[];
extern struct Trigger gData_02000bac[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2)
    return (const struct Trigger *)gData_02000bac;
  return (const struct Trigger *)gData_02000a20;
}

void Func_0200063c();
void Func_02000644();
void Func_0200064c();

void Func_020000f4(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03);
  Event_OpenMessage(10, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_0200812c;
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId(((s32)MsgMikasallaInteriors03 + 1));
  goto L_02008138;
L_0200812c:
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(((s32)MsgMikasallaInteriors03 + 2));
L_02008138:
  Event_ShowMessage(10, 0);
  Event_End();
  return;
}

void Func_0200014c(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_0200063c(13, actor);
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors11);
    Event_ShowMessage(actor, 0);
  }
}

void Func_0200019c(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_0200064c(4, actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors14);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_020001fc(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(
            FieldParty_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17 + 2);
    }
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_02000288(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors29);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_020002e4(s32 actor) {
  u16 facing;

  facing = (Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void Func_02000454(void);

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom1) {
    if (gMainRoutineWork.entrance == 5)
      Func_02000454();
    else if (!Event_GetFlag(0x821) ||
             (Event_GetFlag(0xa39) && !Event_GetFlag(0x341)) ||
             Event_GetFlag(34))
      Actor_SetPosition(18, 0, 0);
    else
      Actor_SetSpriteColor(Actor_Get(18), 4);
    return 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 1) {
      Actor_Get(8)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(8, 2);
    } else if (entrance == 5) {
      Event_ClearFlag(0x12f);
      Actor_Get(9)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(9, 2);
    } else if (entrance == 4) {
      Event_ClearFlag(0x12f);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000454(void) {
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Event_ClearFlag(0x12f);
  return;
}

void Func_020004dc(void);

void Func_02000470(void) {
  if (Event_GetFlag(0xa39)) {
    FieldEvent_SetMessageId(Event_GetFlag(0x340) ? (s32)MsgMikasallaInteriors20
                                            : (s32)MsgMikasallaInteriors21);
    Event_ShowMessage(18, 0);
  } else {
    Func_020004dc();
  }
}

void Func_020004b0(void) {
  s32 result0;

  result0 = Event_GetFlag(0xa39);
  if (result0 != 0)
    goto L_020084c6;
  Func_020004dc();
  goto L_020084d4;
L_020084c6:
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors22);
  Event_ShowMessage(18, 0);
L_020084d4:
  return;
}

void Func_020004dc(void) {
  s32 result0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors19);
  Actor_ShowEmote(18, 0x100, 60);
  Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 60);
  Actor_SetAnimation(18, 4);
  FieldEvent_Wait(15);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(18, 0x108, 60);
  Event_OpenMessage(18, 0);
  result0 =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result0 != 0)
    goto L_02008568;
  Event_ShowMessage(18, 0);
  Event_SetFlag(0x340);
  goto L_02008582;
L_02008568:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessage(18, 0);
L_02008582:
  Event_SetFlag(0xa39);
  Event_SetFlag(0x341);
  Event_End();
  return;
}

#include "objcmd.h"

extern u32 gData_02000654[];
extern u32 gData_0200070c[];

// clang-format off
u32 gData_02000654[] = {
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_ACCEL, 0x1999),
    SC_SET(SW_SPEED, 0x4ccc),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(600), 0, PIXELS(88)),
    SC_SYNC,
    SC_WAIT(20),
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE(PIXELS(616), 0, PIXELS(104)),
    SC_SYNC,
    SC_WAIT(20),
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gData_0200070c[] = {
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_ACCEL, 0x1999),
    SC_SET(SW_SPEED, 0x4ccc),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(360), 0, PIXELS(88)),
    SC_SYNC,
    SC_WAIT(20),
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE(PIXELS(376), 0, PIXELS(104)),
    SC_SYNC,
    SC_WAIT(20),
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(57),
    SCENE_EXIT_IF(1, 55, 1, -1),
    SCENE_EXIT_IF(2, 55, 2, -1),
    SCENE_EXIT_IF(3, 55, 3, -1),
    SCENE_EXIT_IF(4, 55, 4, -1),
    SCENE_EXIT_IF(5, 132, 2, -1),
    SCENE_EXITS(58),
    SCENE_EXIT_IF(1, 55, 5, -1),
    SCENE_EXIT_IF(5, 154, 9, -1),
    SCENE_EXIT_IF(4, 139, 8, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02000840[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(152), FACING_EAST, 1, 0},
    {72, CONDITION_ALWAYS, (s32)gData_02000654, PIXELS(616), 0, PIXELS(104), FACING_EAST, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(136), FACING_NORTH, 3, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(120), FACING_EAST, 1, 0},
    {76, CONDITION_ALWAYS, (s32)gData_0200070c, PIXELS(360), 0, PIXELS(88), FACING_NORTH, 1, 0},
    {130, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {199, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(392), FACING_EAST, 1, 0},
    {209, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(408), FACING_EAST, 1, 0},
    {76, CONDITION_ALWAYS, 2, PIXELS(664), 0, PIXELS(376), FACING_NORTH, 1, 0},
    {239, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(376), FACING_NORTHWEST, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02000960[] = {
    {201, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(147), FACING_SOUTH, 1, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(466), FACING_SOUTH, 1, 0},
    {71, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(520), FACING_NORTH, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(536), FACING_NORTH, 1, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(472), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(152), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(472), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct Trigger gData_02000a20[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors01)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors02)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_020000f4)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors07)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors08)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_0200014c)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors13)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_0200019c)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors23)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors24)},
    {0, 18, CONDITION_ALWAYS, EVENT(Func_02000470)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors04)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors05)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors06)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors09)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors10)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors12)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors15)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors16)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors25)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors26)},
    {0x8d15, 18, CONDITION_ALWAYS, EVENT(Func_020004b0)},
    {0x173, 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors35))},
    {0x173, 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors36))},
    {0x173, 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors37))},
    {0x173, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMikasallaInteriors38))},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02000bac[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_020001fc)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000288)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors30)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors31)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020002e4)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_020001fc)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors18)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors32)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors33)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors34)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgMikasallaInteriors28)},
    {0x8d15, 13, CONDITION_ALWAYS, EVENT(Func_02000078)},
    {0x8d15, 14, CONDITION_ALWAYS, EVENT(Func_020000a0)},
    {TRIGGERS_END},
};
