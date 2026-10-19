#include "keystate.h"
#include "unit/scene/tolbi_spring.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/shop/shop.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "global_cells.h"
#include "edition.h"
#include "field_service.h"
#include "game_state.h"
#include "inventory.h"
#include "menu_list.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "iowrite.h"
#include "frame.h"
#include "fixed_math.h"
#include "item_ids.h"
#include "map.h"
#include "flags.h"
#include "songs.h"
#include "common/colosso_story_flags.h"

/// Forward declarations.
static void TolbiSpring_RiseAndFadeIn(void);
static void TolbiSpring_OfferLuckyWheels(s32 spoken);
static s32 TolbiSpring_RunSpringGame(void);
static s32 TolbiSpring_SelectLuckyWheelsPrize(s32 category);
static void TolbiSpring_InitializeTossSimulation(void);
static s32 TolbiSpring_RunToss(s32 tossKind);

enum SpringTossKind { SPRING_TOSS_COIN, SPRING_TOSS_MEDAL };

/// The game room's actors, numbered in the order of its placement table from 8.
enum TolbiSpringGameRoomActor {
  TOLBI_SPRING_WHEELS_ATTENDANT = 8,
  TOLBI_SPRING_DICE_ATTENDANT = 9,
  TOLBI_SPRING_WHEELS_PRIZE_SIGN = 14,
  TOLBI_SPRING_WHEELS_RULES_SIGN = 15,
  TOLBI_SPRING_SOLDIER = 17, ///< "poor soldiers like us"; gone after Colosso
};

/// The spring's own actors, numbered the same way.
enum TolbiSpringActor {
  TOLBI_SPRING_MEDAL_SEEKER = 27, ///< "Do you have a Lucky Medal?"
  TOLBI_SPRING_SPRING = 80,       ///< searched to toss a coin or medal
};

// FLAG_SCENE_TEMP_00 is set once the spring has risen and faded in.

#define SPRING_CENTER_X PIXELS(120)
#define SPRING_CENTER_Z PIXELS(71)

struct SpringTossMotion {
  u8 unk00[2];
  s16 launchFrame;
  struct Vector3 trail[4];
  struct Vector3 shadow;
  struct Vector3 velocity;
  s32 attractionFrames;
};

struct SpringObstacle {
  struct Vector3 position;
  s16 heading;
  s16 pauseOrPhase;
  s16 speedMode;
  s16 reactionFrames;
  s16 hitCooldownFrames;
  u8 unk16[2];
};

/// Write the spring scene's blend control register.
static __inline__ void Io_SetBlendControl(s32 value) { REG_BLDCNT = value; }

/// Write the spring scene's blend coefficients.
static __inline__ void Io_SetBlendAlpha(s32 value) { REG_BLDALPHA = value; }

/// Return the spring and game-room entrances.
const struct SceneEntrance *TolbiSpring_GetEntrances(void) {
  return gTolbiSpring_Entrances;
}

/// These rooms have no region table.
const struct SceneRegion *TolbiSpring_GetRegions(void) { return NULL; }

/// Return the spring and game-room exit connections.
const u32 *TolbiSpring_GetExits(void) { return gTolbiSpring_Exits; }

/// Select the actors for the spring or its nearby games.
const struct ScenePlacement *TolbiSpring_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_TolbiSpring2) {
    return gTolbiSpringPlacements2;
  }
  return gTolbiSpringPlacementsOther;
}

/// Ask whether the party has a Lucky Medal.
static void TolbiSpring_AskAboutLuckyMedal(s32 actorId) {
  s32 question = MsgTolbiLuckyMedal;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

/// Ask whether this is the party's first Lucky Dice game.
static void TolbiSpring_AskAboutFirstDiceGame(s32 actorId) {
  s32 question = MsgTolbiYerFirstTime;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

/// Reveal the spring once, then position the leader to toss into it.
static void TolbiSpring_TossCoinOrMedal(void) {
  s32 leader = gGameState.selectedActor;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    TolbiSpring_RiseAndFadeIn();
  }
  Engine_EventBegin();
  Engine_ActorWalkToAndWait(leader, 120, 152);
  Engine_ActorFaceDirection(leader, FACING_SOUTH, 0);
  TolbiSpring_RunSpringGame();
  Engine_EventEnd();
}

/// Read the Lucky Wheels rules sign.
static void TolbiSpring_ReadLuckyWheelsRules(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiLuckyWheelsRulesPullLever);
  Engine_EventOpenMessage(-1, 0);
  Engine_EventEnd();
}

/// Read the Lucky Wheels prize sign.
static void TolbiSpring_ReadLuckyWheelsPrizes(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiLuckyWheelsPrizesPrizesDetermined);
  Engine_EventOpenMessage(-1, 0);
  Engine_EventEnd();
}

/// Start the Lucky Wheels attendant's offer.
static void TolbiSpring_TalkToWheelsAttendant(void) {
  TolbiSpring_OfferLuckyWheels(0);
}

/// Select interactions for the spring or the game attendants.
const struct SceneEvent *TolbiSpring_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_TolbiSpring2) {
    return gTolbiSpringEvents2;
  }
  return gTolbiSpringEventsOther;
}

/// Prepare the spring or settle rewards from the nearby games.
s32 TolbiSpring_Initialize(void) {
  s32 coinChange;
  s8 *wonPrizes;
  s32 prizeIndex;
  s32 item;

  if (gGameState.scene == (s32)&SceneId_TolbiSpring2) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Io_Write16(0x3f42, &REG_BLDCNT);
    Io_Write16(0x80c, &REG_BLDALPHA);
    Engine_ActorSetAnimation(24, 2);
    Engine_ActorSetAnimation(25, 2);
    Object_GetById(24)->scaleX = -Q16_ONE;
    Object_GetById(25)->scaleX = -Q16_ONE;
    Object_GetById(24)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(25)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_EventOpenScreen();
    if (gGameState.entrance == 1) {
      TolbiSpring_InitializeTossSimulation();
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00)) {
        Io_SetBlendControl(0x3f42);
        Io_SetBlendAlpha(0x1000);
      }
    }
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER)) {
      Engine_ActorSetPosition(TOLBI_SPRING_SOLDIER, 0, 0);
    }
    gOamCopyEnabled = 1;
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.entrance == 10) {
      Engine_ActorSetChildValue(TOLBI_SPRING_WHEELS_ATTENDANT, 1);
      Engine_ActorSetChildValue(TOLBI_SPRING_DICE_ATTENDANT, 2);
    }
    // Lucky Dice returns here to report the change in the party's coins.
    if (gGameState.entrance == 13 &&
        !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_EventBegin();
      Engine_ActorSetChildValue(TOLBI_SPRING_WHEELS_ATTENDANT, 1);
      Engine_ActorSetChildValue(TOLBI_SPRING_DICE_ATTENDANT, 2);
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      Engine_EventWait(10);
      Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 112);
      Engine_EventWait(20);
      coinChange = gGameState.coins - *(s32 *)gSceneState;
      if (coinChange > 0) {
        if (coinChange > 19999) {
          Engine_AudioPlayCue(MUS_093);
        } else if (coinChange > 4999) {
          Engine_AudioPlayCue(MUS_092);
        } else {
          Engine_AudioPlayCue(MUS_091);
        }
        Engine_EventWait(20);
        Engine_EventSetMessage(MsgTolbiYaWonNumber);
        UiWork_PushValueSlot(coinChange, 5);
        Engine_EventShowMessage(TOLBI_SPRING_DICE_ATTENDANT, 0);
        AudioCommand_WaitForStateByteClear();
      } else if (coinChange < 0) {
        Engine_EventSetMessage(MsgTolbiYaLostNumber);
        UiWork_PushValueSlot(-coinChange, 5);
        Engine_EventShowMessage(TOLBI_SPRING_DICE_ATTENDANT, 0);
      }
      Engine_EventEnd();
    }
    // Lucky Wheels returns item categories for the attendant to award.
    if (gGameState.entrance == 12 &&
        !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      wonPrizes = gGameState.wonPrizes;
      Engine_EventBegin();
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      Engine_EventWait(10);
      if (wonPrizes[0] == -1) {
        TolbiSpring_OfferLuckyWheels(1);
      } else if (wonPrizes[0] != -2) {
        Engine_EventSetMessage(MsgTolbiCongratulations);
        Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
        if (*wonPrizes != -1) {
          for (prizeIndex = 0; wonPrizes[prizeIndex] != -1; prizeIndex++) {
            if (prizeIndex == 0) {
              Engine_EventSetMessage(MsgTolbiWonItemGo);
            } else {
              Engine_EventSetMessage(MsgTolbiWonItemGo2);
            }
            item = TolbiSpring_SelectLuckyWheelsPrize(wonPrizes[prizeIndex]);
            UiWork_PushValueSlot(item, 2);
            Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
            Engine_ItemShowFound(item, 3);
            Engine_PartyGiveItem(item, 0);
            Engine_EventWait(10);
            Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
            Engine_EventWait(30);
          }
        }
        gGameState.wonPrizes[0] = -2;
        Engine_EventSetMessage(MsgTolbiComeAgain);
        Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      }
      Engine_EventEnd();
    }
  }
  return 0;
}

/// Display one frame of the spring's opening or closing prize animation.
static inline void TolbiSpring_ShowPrizeFrame(s32 sourceColumn, s32 sourceRow) {
  Map_CopyCellsTo(sourceColumn, sourceRow, 70, 0, 3, 8);
  Engine_AudioPlayCue(SE_CAST);
  Engine_EventWait(8);
}

/// Animate the spring's prize reveal, give the item, and close it.
static void TolbiSpring_RevealPrize(s32 item) {
  Engine_EventBegin();
  Engine_EventWait(30);
  Engine_AudioPlayCue(SE_148);
  Engine_EventWait(100);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(40);

  Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
  Engine_EventWait(3);
  TolbiSpring_ShowPrizeFrame(85, 20);
  TolbiSpring_ShowPrizeFrame(88, 20);
  TolbiSpring_ShowPrizeFrame(91, 20);
  TolbiSpring_ShowPrizeFrame(94, 20);
  TolbiSpring_ShowPrizeFrame(97, 20);
  TolbiSpring_ShowPrizeFrame(100, 20);

  TolbiSpring_ShowPrizeFrame(79, 29);
  TolbiSpring_ShowPrizeFrame(82, 29);
  TolbiSpring_ShowPrizeFrame(85, 29);
  TolbiSpring_ShowPrizeFrame(88, 29);
  TolbiSpring_ShowPrizeFrame(91, 29);
  TolbiSpring_ShowPrizeFrame(94, 29);
  TolbiSpring_ShowPrizeFrame(97, 29);
  Map_CopyCellsTo(100, 29, 70, 0, 3, 8);
  Engine_AudioPlayCue(SE_CAST);

  Engine_EventWait(70);
  Engine_AudioPlayCue(SE_RECOVERY);
  Engine_ItemShowFound(item, 3);
  Engine_PartyGiveItem(item, 0);
  Engine_EventWait(20);

  TolbiSpring_ShowPrizeFrame(97, 29);
  TolbiSpring_ShowPrizeFrame(94, 29);
  TolbiSpring_ShowPrizeFrame(91, 29);
  TolbiSpring_ShowPrizeFrame(88, 29);
  TolbiSpring_ShowPrizeFrame(85, 29);
  TolbiSpring_ShowPrizeFrame(82, 29);
  TolbiSpring_ShowPrizeFrame(100, 20);
  TolbiSpring_ShowPrizeFrame(97, 20);
  TolbiSpring_ShowPrizeFrame(94, 20);
  TolbiSpring_ShowPrizeFrame(91, 20);
  TolbiSpring_ShowPrizeFrame(88, 20);
  TolbiSpring_ShowPrizeFrame(85, 20);
  TolbiSpring_ShowPrizeFrame(82, 20);
  TolbiSpring_ShowPrizeFrame(79, 20);
  Engine_EventEnd();
}

/// Raise the spring's displayed layer and blend it into view.
static void TolbiSpring_RiseAndFadeIn(void) {
  u8 *springLayer = gMapWork[0];
  const u16 *alphaSteps;
  s32 stepIndex;

  Engine_AudioPlayCue(SE_216);
  // This value belongs to layer 1; its individual field remains unnamed.
  springLayer += offsetof(struct MapState, layers[1]) + 4;
  for (stepIndex = 0; stepIndex < 16; stepIndex++) {
    *(s32 *)(springLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
  }
  alphaSteps = gTolbiSpring_AlphaSteps;
  for (stepIndex = 0; stepIndex < 8; stepIndex++) {
    Io_Write16(0x3f42, &REG_BLDCNT);
    Io_Write16(*alphaSteps++, &REG_BLDALPHA);
    Engine_TaskWait(8);
  }
}

/// Check tickets and inventory space before sending the party to Lucky Wheels.
static void TolbiSpring_OfferLuckyWheels(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(ITEM_GAME_TICKET);
  s32 room;

  ObjectTable_Snapshot();
  if (spoken == 0) {
    s32 message = MsgTolbiStepRightTry;

    Engine_EventSetMessage(message);
    Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (tickets == 0) {
      Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    Engine_EventSetMessage(message + 2);
    UiWork_PushValueSlot(tickets, 5);
    Engine_EventOpenMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Engine_EventSetMessage(message + 4);
      Engine_EventOpenMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    } else if (room <= 6) {
      Engine_EventSetMessage(message + 5);
      Engine_EventOpenMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    }
    if (room <= 6 && Engine_EventChooseYesNo(0, 0) != 0) {
      Engine_EventSetMessage(MsgTolbiGetRidItems);
      Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Engine_EventSetMessage(MsgTolbiBadComeBack);
      Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    Engine_EventSetMessage(MsgTolbiBadLikeUse);
    Engine_EventOpenMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      Engine_EventSetMessage(MsgTolbiComeAgain);
      Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
  }
  Engine_EventSetMessage(MsgTolbiStepRightEnjoy);
  Engine_EventShowMessage(TOLBI_SPRING_WHEELS_ATTENDANT, 0);
  Event_RequestDestination(508, 0);
  Event_SetNextDestination((s32)&SceneId_TolbiSpring1, 12);
}

/// Offer Lucky Dice and save the coin total for the return visit.
static void TolbiSpring_OfferLuckyDice(void) {
  u32 cost;
  u32 coins;

  cost = Party_GetAverageLevel() * 10;
  Engine_EventBegin();
  coins = gGameState.coins;
  if (coins < cost) {
    Engine_EventSetMessage(MsgTolbiYaDontEnough);
    Engine_EventOpenMessage(TOLBI_SPRING_DICE_ATTENDANT, 0);
    return;
  }
  *(u32 *)gSceneState = coins;
  ObjectTable_Snapshot();
  Engine_EventSetMessage(MsgTolbiBuddyWannaTry);
  UiWork_PushValueSlot(cost, 5);
  Engine_EventOpenMessage(TOLBI_SPRING_DICE_ATTENDANT, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventShowMessage(TOLBI_SPRING_DICE_ATTENDANT, 0);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 128);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 152);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Engine_EventWait(20);
    Event_RequestDestination(0x1fd, 0);
    Event_SetNextDestination((s32)&SceneId_TolbiSpring1, 13);
  } else {
    gEventWork->message++;
    Engine_EventShowMessage(TOLBI_SPRING_DICE_ATTENDANT, 0);
  }
  Engine_EventEnd();
}

#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_JAPANESE
#define ASK_WIDTH 17
#else
#define ASK_WIDTH 19
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define NUMBER_X 56
#define TOSS_X 22
#define TOSS_WIDTH 7
#elif LANGUAGE == LANGUAGE_ENGLISH
#define NUMBER_X 72
#define TOSS_X 20
#define TOSS_WIDTH 9
#else
#define NUMBER_X 88
#define TOSS_X 19
#define TOSS_WIDTH 10
#endif

/// Choose a coin or medal, run its toss, and award the resulting prize.
static s32 TolbiSpring_RunSpringGame(void) {
  s32 choice = 0;
  s32 text;
  s32 cancel = -1;
  s32 coins;
  s32 medals;
  struct UiWindow *window;
  s32 result;
  s32 totalWeight;
  s32 prizeIndex;
  s32 prizeRoll;
  s32 message;
  struct GameState *state;

  Event_Begin();
  Battle_ResetEffectCounter();
  message = MsgTolbiThrow;
#if TBS_PATCH_1
  state = &gGameState;
prompt:
  coins = state->coins;
  medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
  Event_SetMessage(message);
  Event_OpenMessage(-1, 0);
  window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
  {
    text = MsgTolbiCoins;
    UiText_DrawCharacterAtOffset(text, window, 0, 0);
    UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
    UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
    UiText_DrawNumberInWindow(medals, 6, window, NUMBER_X, 8);
  }
  choice = Menu_SelectCoinsOrMedals(choice);
  UiWork_Finalize(window, 2);
  UiWork_FinalizePendingCore();
  if (choice == cancel) {
    Event_End();
    return 0;
  }
  if (choice == SPRING_TOSS_COIN) {
    if (coins == 0) {
      Event_SetMessage(message + 1);
      Event_ShowMessage(-1, 0);
      Task_Wait(1);
      goto prompt;
    }
  } else if (choice == SPRING_TOSS_MEDAL) {
    if (medals == 0) {
      Event_SetMessage(message + 2);
      Event_ShowMessage(-1, 0);
      Task_Wait(1);
      goto prompt;
    }
    if (PartyInventory_CountFreeSlots() == 0) {
      Event_SetMessage(message + 4);
      Event_OpenMessage(-1, 0);
      if (Event_ChooseYesNo(0, 0) != 0) {
        goto end;
      }
    }
  }
  window = UiWindow_Create(TOSS_X, 15, TOSS_WIDTH, 4, 2);
  {
    text = MsgTolbiToss;
    UiText_DrawCharacterAtOffset(text, window, 0, 0);
    UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
  }
  Task_Wait(5);
  Event_PlayCue(SE_116);
  for (;;) {
    if (gKeysPressed & KEY_A) {
      Event_PlayCue(SE_MENU_CONFIRM);
      result = 0;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Event_PlayCue(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    Task_Wait(1);
  }
  UiWork_Finalize(window, 2);
#else
  /* FAKEMATCH: whole marker ids swap message/label registers; keep message in
   * r10 and label in r9. 2026-10-11: writing the marker as text + 2 matches in
   * tbs-ja (the pool word becomes MsgTolbiCoins+2) but the id gate refuses an
   * id with an addend (O3), so both barriers stay. */
  asm("" : "+r"(message));
  for (;;) {
    text = MsgTolbiCoins;
    /* FAKEMATCH: keep the coin label in r9; otherwise the repeated marker takes
     * fp and moves state into r9. */
    asm("" : "+r"(text));
    state = &gGameState;
    coins = state->coins;
    medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
    Event_SetMessage(message);
    Event_OpenMessage(-1, 0);
    window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
    UiText_DrawCharacterAtOffset(text, window, 0, 0);
    UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
    UiText_DrawCharacterAtOffset(MsgTolbiCounterMarker, window, 104, 0);
    UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
    UiText_DrawNumberInWindow(medals, 6, window, NUMBER_X, 8);
    UiText_DrawCharacterAtOffset(MsgTolbiCounterMarker, window, 104, 8);
    choice = Menu_SelectCoinsOrMedals(choice);
    UiWork_Finalize(window, 2);
    UiWork_FinalizePendingCore();
    if (choice == cancel) {
      Event_End();
      return 0;
    }
    if (choice == SPRING_TOSS_COIN) {
      if (coins == 0) {
        Event_SetMessage(message + 1);
        Event_ShowMessage(-1, 0);
        Task_Wait(1);
        continue;
      }
    } else if (choice == SPRING_TOSS_MEDAL) {
      if (medals == 0) {
        Event_SetMessage(message + 2);
        Event_ShowMessage(-1, 0);
        Task_Wait(1);
        continue;
      }
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessage(message + 4);
        Event_OpenMessage(-1, 0);
        if (Event_ChooseYesNo(0, 0) != 0) {
          Event_End();
          return 0;
        }
      }
    }
    window = UiWindow_Create(TOSS_X, 15, TOSS_WIDTH, 4, 2);
    result = MsgTolbiToss;
    UiText_DrawCharacterAtOffset(result, window, 0, 0);
    UiText_DrawCharacterAtOffset(result + 1, window, 0, 8);
    Task_Wait(5);
    Event_PlayCue(SE_116);
    for (;;) {
      if (gKeysPressed & KEY_A) {
        Event_PlayCue(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Event_PlayCue(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      Task_Wait(1);
    }
    UiWork_Finalize(window, 2);
    break;
  }
#endif
  // Charge the chosen token only after the player confirms the toss.
  if (result != cancel) {
    if (choice == SPRING_TOSS_COIN) {
      Party_AdjustCoins(-1);
    } else if (choice == SPRING_TOSS_MEDAL) {
      PartyInventory_Remove(ITEM_LUCKY_MEDAL);
    }
    result = TolbiSpring_RunToss(choice);
    if (choice == SPRING_TOSS_COIN) {
      if (result != 4) {
#ifdef UBFIX
        Party_AdjustCoins(gTolbiSpring_CoinPrizes[result]);
#else
        // UB: the original view indexes beyond its one-element array.
        // Plain arrays change register allocation and add four bytes in tbs-en.
        Party_AdjustCoins(((struct { u16 coins[1]; } *)gTolbiSpring_CoinPrizes)
                              ->coins[result]);
#endif
        Event_PlayCue(MUS_091);
#ifdef UBFIX
        UiWork_PushValueSlot(gTolbiSpring_CoinPrizes[result], 5);
#else
        // UB: retain the same native one-element view for this second read.
        UiWork_PushValueSlot(
            ((struct { u16 coins[1]; } *)gTolbiSpring_CoinPrizes)
                ->coins[result],
            5);
#endif
        Event_SetMessage(MsgTolbiWonNumberCoin);
        Event_ShowMessage(-1, 0);
      } else {
        Event_PlayCue(SE_MENU_CANCEL);
        Event_Wait(10);
      }
    } else {
      // The prize ring limits the weighted pool of medal rewards.
      totalWeight = 0;
      prizeIndex = 0;
      if (prizeIndex < result * 3 + 3) {
        do {
          u8 prizeWeight = ((u8 *)state)[prizeIndex + 285];
          totalWeight += (s8)prizeWeight;
          prizeIndex++;
        } while (prizeIndex < result * 3 + 3);
      }
      prizeRoll = (u32)(Random_Next() * totalWeight) >> 16;
      for (prizeIndex = 0; prizeIndex < 15; prizeIndex++) {
        prizeRoll -= state->prizeWeights[prizeIndex];
        if (prizeRoll < 0) {
          break;
        }
      }
      if (prizeIndex == 15) {
        prizeIndex = 14;
      }
      // Reveal the selected item, then reduce its chance on later tosses.
#ifdef UBFIX
      TolbiSpring_RevealPrize(gTolbiSpring_PrizeItems[prizeIndex]);
#else
      // UB: use the whole item array in modern C; preserve the native view.
      TolbiSpring_RevealPrize(
          ((struct { s32 prize[1]; } *)gTolbiSpring_PrizeItems)
              ->prize[prizeIndex]);
#endif

#if TBS_PATCH_1
      /* FAKEMATCH: the pointer pair keeps the native prize-weight base
       * recomputation in r0 after the call. Without it, the earlier
       * roulette base survives in r5 (280 -> 278 instructions, 42 rows
       * differ in the international editions). */
      state++;
      state--;
      {

        /* FAKEMATCH: keeps the slot in r1 and state + 1 in r0. 2026-10-11:
         * plain forms share state + 1 with the roulette's peeled first
         * iteration through GCSE PRE (struct field 42/96, s8 pointer 57/107,
         * direct global 38/100 rows in EN/JA); the ++/-- pair breaks the
         * sharing, but then global allocation gives the later-defined pointer
         * r1 before the slot (differ 10) or, pointer first, sched2 keeps adds
         * r0, #1 ahead of adds r1, r6, r3 by RTL order (differ 2). */
        register s32 slot asm("r1") = prizeIndex + 284;
        s8 *prizeWeight = (s8 *)state + 1;

        if (prizeWeight[slot] > 1) {
          prizeWeight[slot] /= 2;
        }
      }
#else
      {
        /* FAKEMATCH: keeps state + 1 in r0 as the native does. */
        register s8 *prizeWeight asm("r0");
        s32 slot = prizeIndex + 284;

        /* FAKEMATCH: the native recomputes state + 1 after the call. */
        __asm__("" : "=r"(prizeWeight) : "0"(state));
        prizeWeight++;
        if (prizeWeight[slot] > 1) {
          prizeWeight[slot] /= 2;
        }
      }
#endif
    }
  }
#if TBS_PATCH_1
end:
#endif
  Event_End();
  return 0;
}

struct Half {
  u16 v;
};

/// Choose an item variant in the prize category, avoiding its last variant.
static s32 TolbiSpring_SelectLuckyWheelsPrize(s32 category) {
  s32 previousVariant;
  s32 variant;

  if (category < 0) {
    return 0;
  }

  if (category == 5) {
    category = ((unsigned int)(Engine_RandomNext() * 5) >> 16);
  }

  previousVariant = gGameState.wonPrizes[8 + category];

  variant = (previousVariant +
             (s32)((unsigned int)(Engine_RandomNext() * 2) >> 16) + 4) %
            3;

  gGameState.wonPrizes[8 + category] = variant;

  return gTolbiSpring_LuckyWheelsPrizeItems[category * 3 + variant];
}

/// Copy three coordinates and set the actor's facing, child value and colors.
static void TolbiSpring_SetActorAppearance(s32 actorId, const s32 *coordinate,
                                           s32 facing, s32 colorVariant,
                                           s32 childValue) {
  struct FieldActor *actor = Object_GetById(actorId);
  struct AnimationObject *sprite;
  u32 entryCount;

  if (actor != 0) {
    struct Half zero;

    actor->x.fixed = *coordinate++;
    actor->y.fixed = *coordinate++;
    actor->z.fixed = *coordinate;
    actor->facing = facing;
    zero.v = 0;
    actor->motionFlags = zero.v;
    actor->sprite->flags = zero.v;
    ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)actor,
                                     childValue);
  }
  // BUG: the original still dereferences actor here when lookup returned NULL.
  sprite = actor->sprite;
  entryCount = sprite->count;
  if (entryCount != 0) {
    u32 invalidFrame = 255;
    struct AnimationEntry **entries = sprite->entries;
    u32 entriesLeft = entryCount;

    do {
      struct AnimationEntry *entry = *entries++;

      if (entry->param != colorVariant) {
        entry->param = colorVariant;
        {
          u32 frame = entry->frame;

          // Invalidate the cached frame so the new colors are applied.
          frame |= invalidFrame;
          entry->frame = frame;
        }
      }
    } while (--entriesLeft != 0);
  }
}

/// Set whether the specified actor draws its sprite.
static void TolbiSpring_SetSpriteStorageKind(s32 actorId, s32 storageKind) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    u8 *spriteStorageKind = &actor->spriteStorageKind;

    *spriteStorageKind = storageKind;
  }
}

static struct SpringTossMotion sToss;

static s32 sTossKind;

static s32 sTossUnknown[3];

static struct SpringObstacle sObstacles[4];

static s32 sTossFrame;
static s32 sTossEnded;
static s32 sTossResult;

/// Advance the toss, obstacles, collisions and displayed trail each frame.
static void TolbiSpring_UpdateTossSimulation(void) {
  struct SpringTossMotion *toss;
  struct SpringObstacle *obstacle;
  s32 index;
  s32 height;
  s32 edgeInset;
  s32 motionStep;
  s32 speedMode;
  s32 cycleFrame;
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
  /* FAKEMATCH: never used; the reference's frame keeps twelve bytes that no
   * instruction touches. */
  struct Vector3 spare;

  toss = &sToss;
  // Retain the previous three positions for the visible toss trail.
  index = 3;
  do {
    toss->trail[index].x = toss->trail[index - 1].x;
    toss->trail[index].y = toss->trail[index - 1].y;
    toss->trail[index].z = toss->trail[index - 1].z;
    index--;
  } while (index != 0);

  // Delay motion until the leader's launch animation has played.
  if (toss->launchFrame > 31) {
    toss->trail[0].x += toss->velocity.x;
    height = toss->trail[0].y + toss->velocity.y;
    toss->trail[0].y = height;
    toss->trail[0].z += toss->velocity.z;

    if (height <= 0) {
      toss->trail[0].y = 0;
      if (toss->velocity.y != 0) {
        toss->velocity.y = 0;
        if (sTossKind == SPRING_TOSS_MEDAL) {
          Object_SetMode(Object_GetById(17), 1);
        } else {
          Object_SetMode(Object_GetById(12), 1);
        }
      }

      // Pull the floating token toward the prize rings, then let it slow down.
      if (toss->attractionFrames > 0) {
        s32 deltaX;
        s32 deltaZ;
        s32 distance;

        deltaX = (SPRING_CENTER_X - toss->trail[0].x) >> 8;
        deltaZ = (SPRING_CENTER_Z - toss->trail[0].z) >> 8;
        // BUG: an exact center position leaves a zero divisor below.
        distance = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        toss->velocity.x += ((PIXELS(1) / 10) * deltaX) / distance;
        toss->velocity.z += ((PIXELS(1) / 10) * deltaZ) / distance;
        toss->velocity.x = toss->velocity.x * 253 / 256;
        toss->velocity.z = toss->velocity.z * 253 / 256;
        toss->attractionFrames = toss->attractionFrames - 1;
      } else {
        toss->velocity.x = toss->velocity.x * 220 / 256;
        toss->velocity.z = toss->velocity.z * 220 / 256;
        if (toss->velocity.x > -PIXELS(1) / 64 &&
            toss->velocity.x < PIXELS(1) / 64) {
          toss->velocity.x = 0;
        }
        if (toss->velocity.z > -PIXELS(1) / 64 &&
            toss->velocity.z < PIXELS(1) / 64) {
          toss->velocity.z = 0;
        }
        if (toss->velocity.x == 0 && toss->velocity.z == 0) {
          if (sTossKind == SPRING_TOSS_MEDAL) {
            Object_SetMode(Object_GetById(17), 2);
            TolbiSpring_SetSpriteStorageKind(15, 0);
            TolbiSpring_SetSpriteStorageKind(14, 0);
            TolbiSpring_SetSpriteStorageKind(13, 0);
          } else {
            Object_SetMode(Object_GetById(12), 2);
            TolbiSpring_SetSpriteStorageKind(10, 0);
            TolbiSpring_SetSpriteStorageKind(9, 0);
            TolbiSpring_SetSpriteStorageKind(8, 0);
          }
          {
            s32 deltaX = (SPRING_CENTER_X - toss->trail[0].x) >> 16;
            s32 deltaZ = (SPRING_CENTER_Z - toss->trail[0].z) >> 16;
            s32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

            // Classify squared pixel distance into the five prize rings.
            sTossEnded = 1;
            if (distanceSquared <= 224) {
              sTossResult = 0;
            } else if (distanceSquared <= 624) {
              sTossResult = 1;
            } else if (distanceSquared <= 1088) {
              sTossResult = 2;
            } else if (distanceSquared <= 1680) {
              sTossResult = 3;
            } else {
              sTossResult = 4;
            }
          }
        }
      }

      // Narrow the bounds near the spring's sloping corners.
      minX = PIXELS(48);
      maxX = PIXELS(192);
      minZ = PIXELS(24);
      maxZ = PIXELS(120);
      if (toss->trail[0].z < PIXELS(42)) {
        edgeInset = (PIXELS(42) - toss->trail[0].z) * 42 / 18;
        minX = PIXELS(48) + edgeInset;
        if (minX > PIXELS(90)) {
          minX = PIXELS(90);
        }
        maxX = PIXELS(192) - edgeInset;
        if (maxX < PIXELS(150)) {
          maxX = PIXELS(150);
        }
      }
      if (toss->trail[0].z > PIXELS(102)) {
        edgeInset = (toss->trail[0].z * 42 - PIXELS(102) * 42) / 18;
        minX = PIXELS(48) + edgeInset;
        if (minX > PIXELS(90)) {
          minX = PIXELS(90);
        }
        maxX = PIXELS(192) - edgeInset;
        if (maxX < PIXELS(150)) {
          maxX = PIXELS(150);
        }
      }
      if (toss->trail[0].x < PIXELS(90)) {
        edgeInset = (PIXELS(90) - toss->trail[0].x) * 18 / 42;
        minZ = PIXELS(24) + edgeInset;
        if (minZ > PIXELS(42)) {
          minZ = PIXELS(42);
        }
        maxZ = PIXELS(120) - edgeInset;
        if (maxZ < PIXELS(102)) {
          maxZ = PIXELS(102);
        }
      }
      if (toss->trail[0].x > PIXELS(150)) {
        edgeInset = (toss->trail[0].x * 18 - PIXELS(150) * 18) / 42;
        minZ = PIXELS(24) + edgeInset;
        if (minZ > PIXELS(42)) {
          minZ = PIXELS(42);
        }
        maxZ = PIXELS(120) - edgeInset;
        if (maxZ < PIXELS(102)) {
          maxZ = PIXELS(102);
        }
      }

      // Reflect outward motion at the boundary, halving the reflected speed.
      if (toss->trail[0].x < minX) {
        toss->trail[0].x = minX;
        if (toss->velocity.x < 0) {
          toss->velocity.x = -toss->velocity.x / 2;
        }
      }
      if (toss->trail[0].x > maxX) {
        toss->trail[0].x = maxX;
        if (toss->velocity.x > 0) {
          toss->velocity.x = -toss->velocity.x / 2;
        }
      }
      if (toss->trail[0].z < minZ) {
        toss->trail[0].z = minZ;
        if (toss->velocity.z < 0) {
          toss->velocity.z = -toss->velocity.z / 2;
        }
      }
      if (toss->trail[0].z > maxZ) {
        toss->trail[0].z = maxZ;
        if (toss->velocity.z > 0) {
          toss->velocity.z = -toss->velocity.z / 2;
        }
      }
    } else {
      toss->velocity.y -= PIXELS(1) / 4;
    }
  }

  // Two obstacles slide across X; two follow timed elliptical paths.
  index = 0;
  do {
    obstacle = &sObstacles[index];

    if (obstacle->reactionFrames > 0) {
      obstacle->reactionFrames = obstacle->reactionFrames - 1;
    }
    if (obstacle->hitCooldownFrames > 0) {
      obstacle->hitCooldownFrames = obstacle->hitCooldownFrames - 1;
    }

    if (index <= 1) {
      speedMode = obstacle->speedMode;
      motionStep = PIXELS(1);
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        if (index == 0) {
          Object_SetMode(Object_GetById(18), 3);
        } else {
          Object_SetMode(Object_GetById(19), 3);
        }
      } else {
        if (index == 0) {
          Object_SetMode(Object_GetById(18), 1);
        } else {
          Object_SetMode(Object_GetById(19), 1);
        }
        if (obstacle->pauseOrPhase == 0) {
          if (obstacle->heading == 0) {
            obstacle->position.x = obstacle->position.x + motionStep;
          } else {
            obstacle->position.x = obstacle->position.x - motionStep;
          }
          if (obstacle->position.x <= PIXELS(64)) {
            obstacle->heading = 0;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
          if (obstacle->position.x > (PIXELS(176) - 1)) {
            obstacle->heading = 1;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
        } else {
          obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
        }
      }
    } else if (index == 2) {
      speedMode = obstacle->speedMode;
      motionStep = -64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        Object_SetMode(Object_GetById(20), 3);
      } else {
        Object_SetMode(Object_GetById(20), 2);
        obstacle->position.x =
            Engine_MathSin(obstacle->heading) * 48 + PIXELS(112);
        obstacle->position.z =
            Engine_MathCos(obstacle->heading) * 40 + PIXELS(72);
        cycleFrame = (u16)obstacle->heading + motionStep;
        obstacle->heading = cycleFrame;
        obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
      }
    } else {
      cycleFrame = obstacle->pauseOrPhase & 511;
      speedMode = obstacle->speedMode;
      motionStep = 64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        Object_SetMode(Object_GetById(21), 3);
      } else if (cycleFrame <= 383) {
        obstacle->position.x =
            Engine_MathSin(obstacle->heading) * 52 + PIXELS(112);
        obstacle->position.z =
            Engine_MathCos(obstacle->heading) * 24 + PIXELS(72);
        obstacle->heading = obstacle->heading + motionStep;
        Object_SetMode(Object_GetById(21), 2);
      } else {
        Object_SetMode(Object_GetById(21), 3);
      }
      obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
    }

    // A nearby obstacle redirects the token and shortens its attraction time.
    if (obstacle->hitCooldownFrames == 0 && toss->trail[0].y == 0) {
      s32 deltaX;
      s32 deltaZ;
      s32 distanceSquared;

      deltaX = (obstacle->position.x - toss->trail[0].x) >> 16;
      deltaZ = (obstacle->position.z - toss->trail[0].z) >> 16;
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && toss->attractionFrames > 30) {
        s32 speed;
        s32 nextMode;

        speed = PIXELS(3);
        if (index <= 1) {
          if (obstacle->heading == 0) {
            if (toss->velocity.x < speed) {
              toss->velocity.x = speed;
              toss->attractionFrames = toss->attractionFrames - 100;
            }
          } else {
            if (toss->velocity.x > -speed) {
              toss->velocity.x = -speed;
              toss->attractionFrames = toss->attractionFrames - 100;
            }
          }
        } else {
          s32 distance;

          // BUG: coincident positions leave a zero divisor below.
          distance = Iwram_Sqrt(distanceSquared);
          toss->velocity.x = -deltaX * speed / distance;
          toss->velocity.z = -deltaZ * speed / distance;
          toss->attractionFrames = toss->attractionFrames - 100;
        }
        Engine_AudioPlayCue(SE_301);
        nextMode = (obstacle->speedMode + 1) % 3;
        obstacle->reactionFrames = 36;
        obstacle->speedMode = nextMode;
        obstacle->hitCooldownFrames = 30;
      }
    }

    // Apply the obstacle's position, orientation and speed-dependent colors.
    switch (index) {
    case 0:
      TolbiSpring_SetActorAppearance(
          18, Vector3_Coordinates(&obstacle->position), 0,
          gTolbiSpring_SliderFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 1:
      TolbiSpring_SetActorAppearance(
          19, Vector3_Coordinates(&obstacle->position), 0,
          gTolbiSpring_SliderFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 2:
      TolbiSpring_SetActorAppearance(
          20, Vector3_Coordinates(&obstacle->position),
          0x8000 - obstacle->heading,
          gTolbiSpring_CircleFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 3:
      TolbiSpring_SetActorAppearance(
          21, Vector3_Coordinates(&obstacle->position),
          0xffff - obstacle->heading,
          gTolbiSpring_CircleFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    }

    index++;
  } while (index != 4);

  // Draw the token, its ground-level shadow and the three trailing copies.
  toss->shadow.x = toss->trail[0].x;
  toss->shadow.y = 0;
  toss->shadow.z = toss->trail[0].z;

  if (sTossKind == SPRING_TOSS_MEDAL) {
    TolbiSpring_SetActorAppearance(17, Vector3_Coordinates(&toss->trail[0]), 0,
                                   0, 16);
    TolbiSpring_SetActorAppearance(16, Vector3_Coordinates(&toss->shadow), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(15, Vector3_Coordinates(&toss->trail[1]), 0,
                                   0, 16);
    TolbiSpring_SetActorAppearance(14, Vector3_Coordinates(&toss->trail[2]), 0,
                                   0, 16);
    TolbiSpring_SetActorAppearance(13, Vector3_Coordinates(&toss->trail[3]), 0,
                                   0, 16);
    Object_SetMode(Object_GetById(15), 4);
    Object_SetMode(Object_GetById(14), 4);
    Object_SetMode(Object_GetById(13), 4);
  } else {
    TolbiSpring_SetActorAppearance(12, Vector3_Coordinates(&toss->trail[0]), 0,
                                   0, 16);
    TolbiSpring_SetActorAppearance(11, Vector3_Coordinates(&toss->shadow), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(10, Vector3_Coordinates(&toss->trail[1]), 0,
                                   0, 16);
    TolbiSpring_SetActorAppearance(9, Vector3_Coordinates(&toss->trail[2]), 0,
                                   0, 16);
    TolbiSpring_SetActorAppearance(8, Vector3_Coordinates(&toss->trail[3]), 0,
                                   0, 16);
    Object_SetMode(Object_GetById(10), 4);
    Object_SetMode(Object_GetById(9), 4);
    Object_SetMode(Object_GetById(8), 4);
  }

  if (toss->launchFrame != -1) {
    toss->launchFrame = toss->launchFrame + 1;
  }
}

/// Reset four obstacles and schedule the spring simulation.
static void TolbiSpring_InitializeTossSimulation(void) {
  struct SpringTossMotion *toss = &sToss;
  s32 obstacleIndex = 0;
  const u8 *pixelX;
  const u16 *headings;
  struct SpringObstacle *obstacle;
  const u8 *pixelZ;

  pixelX = gTolbiSpring_ActorPixelX;
  obstacle = sObstacles;
  headings = gTolbiSpring_ActorHeadings;
  pixelZ = gTolbiSpring_ActorPixelZ;

  do {
    obstacle->position.x = *pixelX << 16;
    obstacle->position.z = *pixelZ << 16;
    obstacle->position.y = 0;
    obstacle->heading = *headings;
    obstacle->pauseOrPhase = 0;
    obstacle->speedMode = 0;
    obstacle->reactionFrames = 0;
    obstacle->hitCooldownFrames = 0;

    obstacleIndex++;
    pixelX++;
    pixelZ++;
    headings++;
    obstacle++;
  } while (obstacleIndex != 4);

  toss->trail[0].x = -PIXELS(30);
  toss->trail[0].y = 0;
  toss->trail[0].z = PIXELS(100);
  toss->velocity.x = 0;
  toss->velocity.y = 0;
  toss->velocity.z = 0;
  toss->attractionFrames = 0;

  Object_SetMode(Object_GetById(20), 2);
  Object_SetMode(Object_GetById(21), 2);

  {
    s32 budget = 0xc83;
    void (*task)(void) = TolbiSpring_UpdateTossSimulation;

    FieldScheduler_AddCallback(task, budget);
  }
}

/// Launch the chosen coin or medal and wait for its final prize ring.
static s32 TolbiSpring_RunToss(s32 tossKind) {
  struct SpringTossMotion *toss;

  toss = &sToss;
  toss->trail[0].y = 0;
  toss->trail[1].y = 0;
  toss->trail[2].y = 0;
  toss->trail[3].y = 0;
  sTossKind = tossKind;
  sTossEnded = 0;
  toss->launchFrame = -1;
  for (sTossFrame = 0;; sTossFrame++) {
    if (sTossFrame == 50) {
      Engine_AudioPlayCue(SE_300);
    }
    // Release the token on the leader's throw frame.
    if (sTossFrame == 16) {
      Engine_ActorSetAnimation(gGameState.selectedActor, 29);
      toss->launchFrame = 0;
      toss->velocity.x = 13 * PIXELS(1) / 10;
      toss->velocity.y = PIXELS(4);
      toss->velocity.z = -PIXELS(2);
      toss->trail[0].x = PIXELS(120);
      toss->trail[0].y = PIXELS(16);
      toss->trail[0].z = PIXELS(152);
      toss->attractionFrames = 300;
      if (sTossKind == SPRING_TOSS_MEDAL) {
        Object_SetMode(Object_GetById(16), 3);
        Object_SetMode(Object_GetById(17), 0);
        TolbiSpring_SetSpriteStorageKind(15, 1);
        TolbiSpring_SetSpriteStorageKind(14, 1);
        TolbiSpring_SetSpriteStorageKind(13, 1);
      } else {
        Object_SetMode(Object_GetById(11), 3);
        Object_SetMode(Object_GetById(12), 0);
        TolbiSpring_SetSpriteStorageKind(10, 1);
        TolbiSpring_SetSpriteStorageKind(9, 1);
        TolbiSpring_SetSpriteStorageKind(8, 1);
      }
    }
    Engine_TaskWait(1);
    if (sTossEnded == 1) {
      break;
    }
  }
  return sTossResult;
}

const struct SceneEntrance gTolbiSpring_Entrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 120, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 128, 0, 176, FACING_NORTH, 0, 8, 8, 248, 200, 0},
    {11, CONDITION_ALWAYS, 96, 0, 440, FACING_NORTH, 0, 8, 288, 248, 456, 0},
    {12, CONDITION_ALWAYS, 144, 0, 390, FACING_NORTH, 0, 8, 288, 248, 456, 0},
    {13, CONDITION_ALWAYS, 120, 0, 152, FACING_WEST, 0, 8, 8, 248, 200, 0},
    {SCENE_TABLE_END},
};

const u32 gTolbiSpring_Exits[] = {
    SCENE_EXITS(189),
    SCENE_EXIT_IF(1, 135, 20, 0xffffffff),
    SCENE_EXIT(0, 137, 0),
    SCENE_EXIT_IF(1, 135, 13, 0xffffffff),
    SCENE_EXIT_IF(2, 135, 5, 0xffffffff),
    SCENE_EXITS_END,
};

// clang-format off
const struct ScenePlacement gTolbiSpringPlacements2[] = {
    { 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x18d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x112, CONDITION_ALWAYS, 7,
        PIXELS(176), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x112, CONDITION_ALWAYS, 7,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x112, CONDITION_ALWAYS, 7,
        PIXELS(176), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x112, CONDITION_ALWAYS, 7,
        PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(48), 0, PIXELS(136), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(24), 0, PIXELS(72), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(96), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiSpringPlacementsOther[] = {
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(360), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(82), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(82), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(128), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(96), 0, PIXELS(168), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(104), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(96), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(160), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(152), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(80), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gTolbiSpringEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, TOLBI_SPRING_WHEELS_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_TalkToWheelsAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SPRING_WHEELS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringHmmIWonderIfTheseKids) },
    { EVENT_TALK, TOLBI_SPRING_DICE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_OfferLuckyDice) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringReadTheSignIfYaDont) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_AskAboutFirstDiceGame) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWhoaGettingTripleDigitsIsReal) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringIDontWantToStopPlaying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SPRING_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWinningOrLosingIsPureLuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTheRulesForLuckyDiceAint) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringSometimesPeopleWhoHaveNeverPlayed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringThinkingWontHelpYouWinThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringMyLuckHasRunOutFor) },
    { EVENT_TALK, TOLBI_SPRING_WHEELS_PRIZE_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_ReadLuckyWheelsPrizes) },
    { EVENT_TALK, TOLBI_SPRING_WHEELS_RULES_SIGN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_ReadLuckyWheelsRules) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringLuckyDiceThrowTheDiceOn) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringLuckyDiceIfTwoNumbersMatch) },
    { EVENT_TALK, TOLBI_SPRING_SOLDIER, FLAG_BABI_SAVED, MSGNO(MsgTolbiSpringYeahThatDiceGameLooksKinda) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SPRING_SOLDIER, FLAG_BABI_SAVED, MSGNO(MsgTolbiSpringILikePlayingLuckyDiceBut) },
    { EVENT_TALK, TOLBI_SPRING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringOhThereAreAlwaysScalpersAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SPRING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTheresNoWayPoorSoldiersLike) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiSpringEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiTossLuckyMedal) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiFaceAwayTolbi) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTolbiSpringHasMysteriousPowersStand) },
    { EVENT_TALK, TOLBI_SPRING_MEDAL_SEEKER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_AskAboutLuckyMedal) },
    { EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTooBadIOnlyHaveOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringIGetNervousWhenIToss) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_SPRING_MEDAL_SEEKER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWhyWontAnyoneGiveMeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringThisIsALuckyMedalSo) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_SPRING_SPRING, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiSpring_TossCoinOrMedal) },
    { SCENE_EVENTS_END },
};

/// BLDALPHA steps.
const u16 gTolbiSpring_AlphaSteps[] = {
    0x90a, 0xa08, 0xb06, 0xc04, 0xd03, 0xe02, 0xf01, 0x1000,
};

const u32 gTolbiSpring_PrizeItems[] = {
    23, 124, 81, 152, 37, 108, 171, 142,
    48, 160, 133, 97, 183, 186, 189,
};

const u16 gTolbiSpring_CoinPrizes[] = {
    20, 10, 2, 1, 0, 0,
};

const u32 gTolbiSpring_LuckyWheelsPrizeItems[] = {
    250, 251, 252, 256, 257, 258, 262, 263,
    264, 183, 182, 181, 189, 186, 188,
};

const u8 gTolbiSpring_SliderFrames[] = {
    0, 1, 9,
};

const u8 gTolbiSpring_CircleFrames[] = {
    0, 3, 4,
};

const u8 gTolbiSpring_ActorPixelX[] = {
    80, 160, 80, 72,
};

const u8 gTolbiSpring_ActorPixelZ[] = {
    32, 104, 68, 72,
};

const u16 gTolbiSpring_ActorHeadings[] = {
    FACING_EAST, 0x1, FACING_EAST, FACING_WEST,
};
// clang-format on
