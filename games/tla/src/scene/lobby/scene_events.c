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

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "input.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "serial.h"
#include "text/msg_ids.h"
#include "event_inline.h"

s32 Func_02000f6c(s32 id);
void Func_02000fbc(s32 actor);
void Func_0200191c(void);
void Func_0200197c(void);
void Func_020019e4(s32 value, s32 digits);
void Func_020019ec(void);
void Func_02001a14(void);
void Func_02001a1c(void);
s32 Func_02001a84(s32 scene, s32 entrance);

s32 Func_020013ec(void) {
  Func_020019ec();
  FieldMessage_Show(MsgBattleResultsWere, 1);
}

s32 Func_02001400(void) {
  Func_020019ec();
  FieldMessage_Show(MsgMonsterBattleResults, 1);
}

/// Draws value, at most 999, as three digits from right to left.
void Func_02001414(s32 value) {
  s32 i;

  if (value > 999)
    value = 999;
  i = 0;
  // A goto loop; as ☀️'s for loop, loop.c keeps the stacked 1 in r7 (differ
  // 12).
next:
  Map_CopyCellPictureIndicesRect(27, value % 10, 16 - i, 8, 1, 1);
  value /= 10;
  i++;
  if (i <= 2)
    goto next;
  Func_0200197c();
}

#include "ram_buffer.h"

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
  u8 unk16c[0x40];
  s32 startTransition;
};

extern s32 gData_020022b8;
extern s32 gData_020022bc;
extern u16 gData_02003a74[];
extern u8 gAutoSleepEnabled;

s32 Func_02000134(void);
void Func_02000044(void);
void Func_02001914(void);
void Func_02001944(void *func, s32 mask);
void Func_02001994(s32 entry);
void Func_02001a04(s32 value);
void Sound_LoadPresetParameters(s32 preset);
void LinkLobby_WriteSlotValue(s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void GameFlag_SetByte(s32 flag, s32 value);

static inline s32 Flag_GetByte(s32 flag) {
  /* FAKEMATCH(wrapper): Flag_GetByte forwarded inline; as direct calls to
   * GameFlag_GetByte, 60 of 438 instructions differ in tla-en. */
  return GameFlag_GetByte(flag);
}

static inline void Flag_SetByte(s32 flag, s32 value) {
  /* FAKEMATCH(wrapper): Flag_SetByte forwarded inline; as direct calls to
   * GameFlag_SetByte, 17 of 438 instructions differ in tla-en. */
  GameFlag_SetByte(flag, value);
}

static inline void Map_CopyCells(s32 x, s32 y, s32 width, s32 height, s32 destX,
                                 s32 destY) {
  /* FAKEMATCH(wrapper): Map_CopyCells forwarded inline; as direct calls to
   * Map_CopyCellAttributeRect, 6 of 438 instructions differ in tla-en. */
  Map_CopyCellAttributeRect(x, y, width, height, destX, destY);
}

s32 Func_0200145c(void) {
  s32 i;
  s32 wins;
  u32 score;
  struct LobbyEventWork *work;
  void *func;

  Event_SetFlag(0x17e);
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gData_020022bc = 0;
  gData_020022b8 = 0;
  ((struct LobbyEventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x201;
  Sound_LoadPresetParameters(2);
  Func_02001414(gMainRoutineWork.linkTallies[6]);
  Map_CopyCells(11, 11, 1, 1, 13, 10);
  LinkLobby_WriteSlotValue(4);
  wait(1);
  Func_02001994(5);
  {
    u16 *name = gData_02003a74;

    name[4] = 'T';
    name[5] = 'A';
    name[6] = 'L';
    name[7] = 'K';
  }
  for (i = 0; i < 8; i++) {
    s32 flag = 0x2f0 + i;

    Event_ClearFlag(flag);
    if (Func_02000f6c(i))
      Event_SetFlag(flag);
  }
  if (gMainRoutineWork.entrance == 8) {
    s32 msg;
    s32 v;

    Func_02001a14();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(5);
    gMainRoutineWork.linkTallies[0]++;
    gMainRoutineWork.linkTallies[3]++;
    wins = (s8)GameFlag_GetByte(0x3f8);
    msg = wins * 2 + 2;
    if (msg > 14)
      msg = 14;
    v = Flag_GetByte(1000);
    if (v == 2) {
      GameFlag_SetByte(1000, 0);
      wins++;
      msg++;
    } else {
      Flag_SetByte(1000, v + 1);
    }
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)MsgNotBadWould + msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      if (wins > 90)
        wins = 90;
      GameFlag_SetByte(0x3f8, wins);
    } else {
      Event_ClearFlag(0x173);
      GameFlag_SetByte(0x3f8, -1);
      /* FAKEMATCH(temporary): one scalar holds the row address, then its score;
       * as the row read directly in entrances 8 and 9, 36 of 438 instructions
       * differ in tla-en. */
      score = (u32)&gMainRoutineWork.linkTallies[3];
      Func_020019e4(*(u16 *)score, 5);
      score = *(u16 *)score;
      if (gMainRoutineWork.linkTallies[2] < score) {
        gMainRoutineWork.linkTallies[2] = score;
        Event_SetMessageId((s32)MsgYouWonBattle);
        FieldMessage_ShowAtActor(8, 0);
        Func_02001400();
      } else {
        Event_SetMessageId((s32)MsgYouWonBattlePlease);
        FieldMessage_ShowAtActor(8, 0);
      }
      LinkLobby_WriteSlotValue(0);
    }
    Func_02001a1c();
  } else if (gMainRoutineWork.entrance == 9) {
    gMainRoutineWork.linkTallies[1]++;
    Func_02001a14();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(5);
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    /* FAKEMATCH(temporary): one scalar holds the row address, then its score;
     * as the row read directly in entrances 8 and 9, 36 of 438 instructions
     * differ in tla-en. */
    score = (u32)&gMainRoutineWork.linkTallies[3];
    Func_020019e4(*(u16 *)score, 5);
    score = *(u16 *)score;
    if (gMainRoutineWork.linkTallies[2] < score) {
      gMainRoutineWork.linkTallies[2] = score;
      Event_SetMessageId((s32)MsgYouWonBattle);
      FieldMessage_ShowAtActor(8, 0);
      Func_02001400();
    } else {
      Event_SetMessageId((s32)MsgYouWonBattleI);
      FieldMessage_ShowAtActor(8, 0);
    }
    gMainRoutineWork.linkTallies[3] = 0;
    Event_ClearFlag(0x173);
    GameFlag_SetByte(0x3f8, -1);
    LinkLobby_WriteSlotValue(0);
    Func_02001a1c();
  } else if (gMainRoutineWork.entrance == 10) {
    s32 v;

    Func_02001a14();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if (Event_GetFlag(1000)) {
      work = Ram_HeapSlots->eventWork;
      Event_ClearFlag(1000);
      work->raisedTrigger = 2;
      Event_ClearFlag(0x304);
      wait(20);
      Func_02000044();
      LinkLobby_WriteSlotValue(0);
      LinkLobby_WriteSlotValue(4);
    } else {
      gMainRoutineWork.linkTallies[4]++;
      v = gMainRoutineWork.linkTallies[7] + 1;
      gMainRoutineWork.linkTallies[7] = v;
      /* FAKEMATCH(temporary): the other score branches' address slot reused; as
       * the row read directly, 8 of 438 instructions differ in tla-en. */
      score = (u32)&gMainRoutineWork.linkTallies[6];
      if (*(u16 *)score < (u16)v)
        *(u16 *)score = v;
      Func_02001414(*(u16 *)score);
      Func_020013ec();
      Event_SetFlag(0x304);
      Event_SetFlag(0x305);
    }
    Func_02001a1c();
  } else if (gMainRoutineWork.entrance == 11) {
    s32 v;

    Func_02001a14();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if ((v = Event_GetFlag(0x173)) == 0) {
      gMainRoutineWork.linkTallies[5]++;
      gMainRoutineWork.linkTallies[7] = v;
      Func_020013ec();
    }
    Event_SetFlag(0x304);
    Event_ClearFlag(0x305);
    Func_02001a1c();
  } else {
    Func_02001914();
    Event_ClearFlag(0x172);
    GameFlag_SetByte(0x3f8, -1);
    if (gMainRoutineWork.unk244[6] != 0) {
      Func_02001a14();
      Event_ResolvePendingActions(0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
      Event_SetMessageId((s32)MsgNoteYouCan);
      Event_ShowMessageAndWait(8, 0);
      Func_02001a1c();
    }
    gMainRoutineWork.unk244[6] = 0;
    gAutoSleepEnabled = 0;
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
  }
  func = Func_02000134;
  Task_Add(func, 0x480);
  Func_02001944(func, 1);
  if (gMainRoutineWork.entrance != 8 || !Event_GetFlag(0x173)) {
    Func_02001a04(1);
    DjinnRecovery_CountDownTwenty();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "edition.h"

void Func_020002d8(void);
void Func_02000344(void);
void Func_02000864(void);
void Func_02000bb0(void);
void Func_02000da0(void);
void Func_02000e4c(void);
void Func_02000f50(void);
void Func_02001130(void);
void Func_02001210(void);
void Func_0200122c(void);
void Func_02001304(void);
void Func_02001388(void);

/// Edition-specific link handshake values (each word is four ASCII characters).
// clang-format off
s32 gLinkLobby_SlotValues[] = {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    0x32674d43, 0x33323130, 0x31434241, 0x32454443, 0x33474645, 0x434d4753,
#elif (LANGUAGE == LANGUAGE_GERMAN)
    0x47357343, 0x32313033, 0x43424131, 0x45444332, 0x47464533, 0x43534747,
#elif (LANGUAGE == LANGUAGE_SPANISH)
    0x47733743, 0x32313033, 0x43424131, 0x45444332, 0x47464533, 0x43535347,
#elif (LANGUAGE == LANGUAGE_FRENCH)
    0x32475363, 0x32313033, 0x43424131, 0x45444332, 0x47464533, 0x4353474d,
#elif (LANGUAGE == LANGUAGE_ITALIAN)
    0x31494773, 0x33323130, 0x31434241, 0x32454443, 0x33474645, 0x53474943,
#else
    0x43314773, 0x33323130, 0x31434241, 0x32454443, 0x33474645, 0x434d4753,
#endif
};

u8 gLinkLobby_SlotColumns[] = {
    0, 1, 1, 1, 1, 0,
};

u16 gData_02001b02[] = {
    709, 68, 74, 727, 1, 2, 3, 4,
    5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 55, 76, 49,
    50, 51, 52, 56, 57, 53, 54, 58,
    59, 60, 61, 62, 63, 64, 65, 70,
    71, 72, 73, 67, 75, 700, 701, 702,
    703, 704, 705, 706, 707, 708, 710, 720,
    721, 722, 723, 724, 725, 726, 728, 729,
    730, 741, 742, 743, 750, 751, 752, 753,
    754,
};

struct SceneEntrance gSceneEntrances[] = {
    {10, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {11, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {8, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
    {9, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
    {1, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {0, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {190, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(204), FACING_SOUTH, 0, 1},
    {187, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(204), FACING_SOUTH, 1, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(88), FACING_SOUTH, 1, 0},
    {192, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(272), FACING_WEST, 1, 0},
    {344, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(380), FACING_SOUTH, 1, 0},
    {86, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(252), FACING_SOUTHEAST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(284), FACING_EAST, 1, 0},
    {242, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(384), FACING_SOUTH, 1, 0},
    {191, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(216), FACING_WEST, 1, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(216), FACING_EAST, 1, 0},
    {232, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(384), FACING_EAST, 1, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(64), 0, PIXELS(88), FACING_SOUTH, 1, 0},
    {234, CONDITION_ALWAYS, 2, PIXELS(184), 0, PIXELS(256), FACING_NORTH, 1, 0},
    {196, CONDITION_ALWAYS, 1, PIXELS(64), 0, PIXELS(376), FACING_NORTH, 1, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_SOUTH, 1, 0},
    {223, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(376), FACING_EAST, 1, 0},
    {192, CONDITION_ALWAYS, 8, PIXELS(360), 0, PIXELS(296), FACING_EAST, 1, 0},
    {0, 0x1000, 1, PIXELS(324), 0, PIXELS(216), FACING_SOUTHWEST, 1, 0},
    {1, 0x1001, 1, PIXELS(296), 0, PIXELS(168), FACING_NORTHEAST, 1, 0},
    {2, 0x1002, 1, PIXELS(352), 0, PIXELS(152), FACING_NORTH, 1, 0},
    {3, 0x1003, 1, PIXELS(352), 0, PIXELS(108), FACING_SOUTH, 1, 0},
    {6, 0x1006, 1, PIXELS(376), 0, PIXELS(136), FACING_WEST, 1, 0},
    {7, 0x1007, 1, PIXELS(328), 0, PIXELS(88), FACING_SOUTH, 1, 0},
    {5, 0x1005, 1, PIXELS(328), 0, PIXELS(136), FACING_EAST, 1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02001ee0[] = {
    0xffff00be, 1, 0x980000, 0, 0xcc0000, 0x1004000,
    0xffff00bb, 1, 0x1180000, 0, 0xcc0000, 0x14000,
    0xffff00c9, 1, 0x1300000, 0, 0xa80000, 0x10000,
    0xffff00c0, 1, 0xc00000, 0, 0x1100000, 0x10000,
    344, 1, 0x1480000, 0, 0x880000, 0x10000,
    0xffff0056, 1, 0x400000, 0, 0x5c0000, 0x14000,
    0xffff0078, 1, 0x280000, 0, 0x740000, 0x10000,
    0xffff00f2, 1, 0x1000000, 0, 0x1800000, 0x14000,
    0xffff00bf, 1, 0x680000, 0, 0xf80000, 0x14000,
    0xffff00ba, 1, 0x1480000, 0, 0xf80000, 0x14000,
    0x100e8, 1, 0x1600000, 0, 0x6c0000, 0x14000,
    0x200e7, 1, 0x1600000, 0, 0x9c0000, 0x1c000,
    0x300ea, 1, 0x1780000, 0, 0x880000, 0x18000,
    0xffff00c4, 1, 0x400000, 0, 0x880000, 0x1c000,
    0xffff00bc, 1, 0xf80000, 0, 0xb80000, 0x14000,
    0xffff00df, 1, 0x1480000, 0, 0x580000, 0x14000,
    0xffff00bb, 1, 0x480000, 0, 0x1200000, 0x10000,
    0xffff00c0, 8, 0x1680000, 0, 0x1280000, 0x10000,
    0x10000000, 1, 0x1480000, 0, 0x880000, 0x10000,
    0x10010001, 1, 0x1600000, 0, 0x6c0000, 0x14000,
    0x10020002, 1, 0x1600000, 0, 0x9c0000, 0x1c000,
    0x10030003, 1, 0x1780000, 0, 0x880000, 0x18000,
    0x10060006, 1, 0x980000, 0, 0x900000, 0x18000,
    0x10070007, 1, 0x680000, 0, 0x900000, 0x10000,
    0x10050005, 1, 0x800000, 0, 0x7c0000, 0x14000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {0, 0, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 1, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 2, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 3, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 5, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 6, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 7, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000bb0)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000da0)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000f50)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgWelcomeToBattle)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgHmItComes)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_0200122c)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_0200122c)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_02000e4c)},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_02001304)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgInTheLinked)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgSoYouWin)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgYouCouldUse)},
    {0, 20, CONDITION_ALWAYS, MSGNO(MsgShouldIEnter)},
    {0, 21, CONDITION_ALWAYS, MSGNO(MsgMonsterBattleRules)},
    {0, 22, CONDITION_ALWAYS, EVENT(Func_02001388)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgSometimesYourClass)},
    {0, 24, CONDITION_ALWAYS, EVENT(Func_02000fbc)},
    {0, 25, CONDITION_ALWAYS, MSGNO(MsgIDonTHaveThe)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02000864)},
    {2, 1, CONDITION_ALWAYS, EVENT(Func_02001210)},
    {6, 1, CONDITION_ALWAYS, EVENT(Func_020002d8)},
    {6, 2, CONDITION_ALWAYS, EVENT(Func_02000344)},
    {TRIGGERS_END},
};
