/// @file
/// Lemuria's spring: the party drinks from it to restore its HP and PP, and
/// tosses coins or Lucky Medals into it as in Tolbi's spring game, past the
/// moving obstacles, for coins or for items.

#include "battle/battle.h"
#include "sprite_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "system/io_write.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"

/// The spring's actors, as The Broken Seal numbers Tolbi's spring, by the
/// rows of gActorPlacements.
///
/// The toss draws the coin or the medal with its shadow and three trailing
/// copies, which LEMURIA_SPRING_COIN_TRAIL and LEMURIA_SPRING_MEDAL_TRAIL
/// number.
enum LemuriaSpringActorId {
  ACTOR_LEMURIA_SPRING_COIN_SHADOW = ACTOR_FIRST_PLACED + 3,
  ACTOR_LEMURIA_SPRING_COIN, ///< SPRITE_SPRING_COIN
  ACTOR_LEMURIA_SPRING_MEDAL_SHADOW = ACTOR_LEMURIA_SPRING_COIN + 4,
  ACTOR_LEMURIA_SPRING_MEDAL, ///< SPRITE_SPRING_LUCKY_MEDAL
  /// The four obstacles that move in the spring during a toss: two crabs,
  /// then two turtles.
  ACTOR_LEMURIA_SPRING_CRAB_A,
  ACTOR_LEMURIA_SPRING_CRAB_B,
  ACTOR_LEMURIA_SPRING_TURTLE_A,
  ACTOR_LEMURIA_SPRING_TURTLE_B,
  ACTOR_LEMURIA_SPRING_MEDAL_SIGN, ///< east of the spring
  ACTOR_LEMURIA_SPRING_COIN_SIGN,  ///< west of the spring
  /// The tufts of grass at the foot of each sign.
  ACTOR_LEMURIA_SPRING_EAST_SIGN_GRASS,
  ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS,
  /// "If you face away..."
  ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY,
  ACTOR_LEMURIA_SPRING_MAN_WITH_PACK,
  /// "I remember an outsider who came..."
  ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER,
  ACTOR_LEMURIA_SPRING_PRIZE ///< the item won, shown as it is given
};

#define ACTOR_LEMURIA_SPRING_FIRST_OBSTACLE ACTOR_LEMURIA_SPRING_CRAB_A
/// The actor that draws toss->trail[n] of a tossed coin or medal, n from 1.
#define LEMURIA_SPRING_COIN_TRAIL(n) (ACTOR_LEMURIA_SPRING_COIN_SHADOW - (n))
#define LEMURIA_SPRING_MEDAL_TRAIL(n) (ACTOR_LEMURIA_SPRING_MEDAL_SHADOW - (n))
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "field/field_bank.h"
#include "game/game_bank.h"
#include "scene/scene.h"
#include "scene/shared.h"
#include "window/window.h"
#include "graphics/text_render.h"
#include "graphics/uiflow.h"
#include "system/main_routine_work.h"
#include "version.h"
#include "edition.h"
#include "system/input.h"
#include "field/actor.h"
#include "field/call.h"
#include "system/iwram_call.h"
#include "system/vector3.h"
#include "system/task.h"

s32 LemuriaSpring_RunSpringGame(void);

extern u16 gLemuriaSpring_CoinPrizes[];
extern u32 gLemuriaSpring_MedalPrizes[];

s32 LemuriaSpring_RunToss();

void LemuriaSpring_GivePrize(s32 a0);

void SummonTablet_PlayRunes(s32 targetId);

s32 ItemDisplay_ShowItem();

extern u8 gLemuriaSpring_FrontObstacleAnimations[];
extern u8 gLemuriaSpring_BackObstacleAnimations[];
extern u8 gLemuriaSpring_ObstacleStartX[];
extern u8 gLemuriaSpring_ObstacleStartZ[];

extern u16 gLemuriaSpring_ObstacleHeadings[];

void LemuriaSpring_SetSpriteStorageKind(s32 actor, s32 active);

s32 Party_RestoreHpPp();
s32 Class_GetSeries();

#include "text/msg_ids.h"

extern struct SpringTossMotion gLemuriaSpring_Toss;
extern struct SpringObstacle gLemuriaSpring_Obstacles[];
extern s32 gLemuriaSpring_TossDone;
extern s32 gLemuriaSpring_TossResult;
void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern s32 gLemuriaSpring_TossFrame;
extern s32 gLemuriaSpring_TossChoice;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// The party drinks from the spring and its HP and PP are restored.
/// "Felix and the others drank of the spring. Their HP and PP were fully
/// replenished!"
void LemuriaSpring_DrinkFromSpring(void) {
  // "Felix and the others drank of the spring. Their HP and PP were fully..."
  Message_ShowCentered((s32)MsgAndTheOthers, 1);
  Event_PlaySound(SE_RECOVERY);
  Party_RestoreHpPp(1);
}

extern u16 gLemuriaSpring_AlphaSteps[];

/// Raises the spring's displayed layer and blends it into view, as Tolbi's
/// does.
void LemuriaSpring_RiseAndFadeIn(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  s32 *layer;
  s32 i;

  Event_PlaySound(SE_BURST);
  layer = (s32 *)(map + 0x178);
  for (i = 0; i < 16; i++) {
    layer[3] -= px(1);
    wait(4);
  }
  for (i = 0; i < 8; i++) {
    Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
               &REG_BLDCNT);
    Io_Write16(gLemuriaSpring_AlphaSteps[i], &REG_BLDALPHA);
    wait(8);
  }
}

/// Searching the spring lets the party toss a coin into it. The spring rises
/// into view the first time in the visit (flag 0x200), as Tolbi's does.
void LemuriaSpring_TossCoin(void) {
  s32 actor = FIELD_AT(&gMainRoutineWork, s32, 532);

  if (!Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    Event_SetFlag(FLAG_SCENE_TEMP_00);
    LemuriaSpring_RiseAndFadeIn();
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(actor, 120, 152);
  Actor_FaceDirection(actor, FACING_SOUTH, 0);
  LemuriaSpring_RunSpringGame();
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void LemuriaSpring_InitializeTossSimulation(void);

/// Starts the scene: sets the colour blend, puts objects 24 and 25 in mode 2
/// and starts the screen transition; when gMainRoutineWork's 0x1e2 halfword is
/// 1 and flag 0x200 is set, it sets the blend's weights again.
s32 Scene_Initialize(void) {
  s32 t0;
  s32 blend;
  s32 alpha;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  t0 = (s32)Actor_Get(gMainRoutineWork.leaderActorId);
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  blend = 0x3f42;
  FIELD_AT(0x4000050, u16, 0) = blend;
  do {
  } while (0);
  alpha = 0x80c;
  FIELD_AT(0x4000052, u16, 0) = alpha;
  Actor_SetAnimation(ACTOR_LEMURIA_SPRING_EAST_SIGN_GRASS, 2);
  Actor_SetAnimation(ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS, 2);
  FIELD_AT(((s32)Actor_Get(ACTOR_LEMURIA_SPRING_EAST_SIGN_GRASS) + 0x23), u8,
           0) = 2;
  FIELD_AT(((s32)Actor_Get(ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS) + 0x23), u8,
           0) = 2;
  Event_StartScreenTransition();
  if (gMainRoutineWork.entrance == 1) {
    LemuriaSpring_InitializeTossSimulation();
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
      FIELD_AT(0x4000050, u16, 0) = blend;
      FIELD_AT(0x4000052, u16, 0) = 0x1000;
    }
  }
  return 0;
}

#include "text/msg_ids.h"

s32 Scene_PrepareMap(void) { return 0; }

/// The Japanese catalog does not name these yet.
extern u8 MsgLemuriaSpringWhatWillYouThrow[], MsgLemuriaSpringCoins[],
    MsgLemuriaSpringToss[], MsgLemuriaSpringYouWonCoin[];
s32 Menu_SelectCoinsOrMedals();

// French draws the count window two tiles wider, its numbers 16 pixels further
// right; Japanese draws them 16 pixels further left, each followed by its
// counter word.
#if LANGUAGE == LANGUAGE_FRENCH
#define COUNT_WINDOW_WIDTH 19
#define COUNT_NUMBER_X 88
#elif LANGUAGE == LANGUAGE_JAPANESE
#define COUNT_WINDOW_WIDTH 17
#define COUNT_NUMBER_X 56
#else
#define COUNT_WINDOW_WIDTH 17
#define COUNT_NUMBER_X 72
#endif

// The toss prompt's window: two tiles narrower in Japanese, against the right
// edge.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TOSS_WINDOW_X 22
#define TOSS_WINDOW_WIDTH 7
#else
#define TOSS_WINDOW_X 20
#define TOSS_WINDOW_WIDTH 9
#endif

#define WEIGHT(i) ((s8)gMainRoutineWork.prizeWeights[i])

/// The toss: the party picks what to throw and how many, the coins fly, and
/// what lands in the spring wins its prize, as in Tolbi's spring game.
/// "What will you throw?"
s32 LemuriaSpring_RunSpringGame(void) {
  s32 choice;
#if !TLA_PATCH_1
  s32 text;
#endif
  s32 message;
  s32 coins;
  s32 medals;
  struct Window *window;
  s32 result;
  s32 prizeRoll;
#if TLA_PATCH_1
  s32 notice;
#endif
  s32 total;
  s32 prize;
  /* FAKEMATCH(temporary): minusOne, a 64-bit -1 for these calls. */
  s64 minusOne = -1;

#if TLA_PATCH_1
  Event_Start();
  choice = 0;
  Event_ResolvePendingActions(0);
prompt:
  coins = gMainRoutineWork.coins;
  medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
  message = (s32)MsgLemuriaSpringWhatWillYouThrow;
  Event_SetMessageId(message);
  // "What will you throw?"
  Event_OpenMessage(-1, 0);
  window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgLemuriaSpringCoins;
  Text_DrawMessageAsTiles(result, window, 0, 0);
  Text_DrawNumberInWindow(coins, 6, window, COUNT_NUMBER_X, 0);
  Text_DrawMessageAsTiles(result + 1, window, 0, 8);
  Text_DrawNumberInWindow(medals, 6, window, COUNT_NUMBER_X, 8);
  choice = Menu_SelectCoinsOrMedals(choice);
  Window_Close(window, 2);
  Message_CloseFinishedWindows();
  if (choice == -1)
    goto end;
  if (choice == 0) {
    if (coins != 0)
      goto toss;
    notice = message + 1;
  } else {
    if (choice != 1)
      goto toss;
    if (medals != 0)
      goto full;
    notice = message + 2;
  }
  Event_SetMessageId(notice);
  Event_ShowMessage(-1, 0);
  wait(1);
  goto prompt;
full:
  if (PartyInventory_CountFreeSlots() == 0) {
    Event_SetMessageId(message + 4);
    // "You can't carry more items. Do you still want to play?"
    Event_OpenMessage(minusOne, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
      goto end;
  }
toss:
  window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgLemuriaSpringToss;
  Text_DrawMessageAsTiles(result, window, 0, 0);
  Text_DrawMessageAsTiles(result + 1, window, 0, 8);
  wait(5);
  Event_PlaySound(SE_TOLBI_SPRING_TOSS_PROMPT);
  for (;;) {
    if (gInput.pressed & KEY_A) {
      Event_PlaySound(SE_MENU_CONFIRM);
      result = 0;
      break;
    }
    if (gInput.pressed & KEY_B) {
      Event_PlaySound(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 2);
#else
  // The Japanese release keeps The Broken Seal's Japanese layout: a real loop
  // around the prompt and the toss, a counter word after each count, and the
  // coin or medal taken as soon as it is chosen, so cancelling the toss loses
  // it. From patch 1 it is taken once the toss is confirmed.
  choice = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  for (;;) {
    coins = gMainRoutineWork.coins;
    medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
    message = (s32)MsgLemuriaSpringWhatWillYouThrow;
    Event_SetMessageId(message);
    // "What will you throw?"
    Event_OpenMessage(-1, 0);
    window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
    text = (s32)MsgLemuriaSpringCoins;
    Text_DrawMessageAsTiles(text, window, 0, 0);
    Text_DrawNumberInWindow(coins, 6, window, COUNT_NUMBER_X, 0);
    Text_DrawMessageAsTiles(text + 2, window, 104, 0);
    Text_DrawMessageAsTiles(text + 1, window, 0, 8);
    Text_DrawNumberInWindow(medals, 6, window, COUNT_NUMBER_X, 8);
    Text_DrawMessageAsTiles(text + 2, window, 104, 8);
    choice = Menu_SelectCoinsOrMedals(choice);
    Window_Close(window, 2);
    Message_CloseFinishedWindows();
    if (choice == -1)
      goto end;
    if (choice == 0) {
      if (coins == 0) {
        Event_SetMessageId(message + 1);
        // "You don't have any coins!"
        Event_ShowMessage(-1, 0);
        wait(1);
        continue;
      }
      Party_AdjustCoins(-1);
    } else if (choice == 1) {
      if (medals == 0) {
        Event_SetMessageId(message + 2);
        // "You have no Lucky Medals!"
        Event_ShowMessage(-1, 0);
        wait(1);
        continue;
      }
      PartyInventory_Remove(ITEM_LUCKY_MEDAL);
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessageId(message + 4);
        // "You can't carry more items. Do you still want to play?"
        Event_OpenMessage(-1, 0);
        if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
          goto end;
      }
    }
    window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
    result = (s32)MsgLemuriaSpringToss;
    Text_DrawMessageAsTiles(result, window, 0, 0);
    Text_DrawMessageAsTiles(result + 1, window, 0, 8);
    wait(5);
    Event_PlaySound(SE_TOLBI_SPRING_TOSS_PROMPT);
    for (;;) {
      if (gInput.pressed & KEY_A) {
        Event_PlaySound(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if (gInput.pressed & KEY_B) {
        Event_PlaySound(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      wait(1);
    }
    Window_Close(window, 2);
    break;
  }
#endif
  if (result != -1) {
#if TLA_PATCH_1
    if (choice == 0) {
      Party_AdjustCoins(minusOne);
    } else if (choice == 1) {
      PartyInventory_Remove(ITEM_LUCKY_MEDAL);
    }
#endif
    result = LemuriaSpring_RunToss(choice);
    if (choice == 0) {
      if (result != 4) {
        Party_AdjustCoins(gLemuriaSpring_CoinPrizes[result]);
        Event_PlaySound(MUS_JINGLE_SMALL);
        MessageArgument_Push(gLemuriaSpring_CoinPrizes[result],
                             MESSAGE_ARGUMENT_NUMBER);
        Event_SetMessageId((s32)MsgLemuriaSpringYouWonCoin);
        // "You won ... coin!"
        Event_ShowMessage((s32)minusOne, 0);
      } else {
        Event_PlaySound(SE_MENU_CANCEL);
        FieldEvent_Wait(10);
      }
    } else {
      total = 0;
      if (result == 0 && Event_GetFlag(0x90b) == 0) {
        Event_SetFlag(0x90b);
        LemuriaSpring_GivePrize((s32)minusOne);
      } else {
        for (prize = 0; prize < result * 3 + 3; prize++)
          total += (s8)gMainRoutineWork.prizeWeights[prize];
        prizeRoll = Random_Below(total);
        for (prize = 0; prize < 15; prize++) {
          prizeRoll -= WEIGHT(prize);
          if (prizeRoll < 0)
            break;
        }
        if (prize == 15)
          prize = 14;
        LemuriaSpring_GivePrize(gLemuriaSpring_MedalPrizes[prize]);
        if (WEIGHT(prize) > 1)
          gMainRoutineWork.prizeWeights[prize] = WEIGHT(prize) / 2;
      }
    }
  }
end:
  Event_End();
  return 0;
}

s32 Actor_MoveToObject();
s32 Summon_ShowLearnedMessage();

/// The spring's statue opens and gives the prize: an item, or, for the first
/// medal to land in the middle (flag 0x90b), the special prize actor 29 brings.
void LemuriaSpring_GivePrize(s32 a0) {
  s32 t52;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_BOSS_ROAR);
  FieldEvent_Wait(100);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(82, 20, 70, 0, 3, 8);
  FieldEvent_Wait(3);
  MapGrid_CopyCellsTo(85, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(88, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(91, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(94, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(97, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(100, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(79, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(82, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(85, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(88, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(91, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(94, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(97, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(100, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(70);
  Event_PlaySound(SE_RECOVERY);
  if (a0 == -1) {
    t52 = Actor_Get(ACTOR_LEMURIA_SPRING_PRIZE);
    Actor_MoveToObject(ACTOR_LEMURIA_SPRING_PRIZE,
                       FIELD_AT(&gMainRoutineWork, s32, 532));
    FIELD_AT((t52 + 0x55), u8, 0) = 0;
    FIELD_AT(t52, s32, 12) = 0x200000;
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 0x1c);
    SummonTablet_PlayRunes(0x1d);
    Summon_ShowLearnedMessage(0x16);
    Class_GetSeries(0x16);
    Actor_SetPosition(ACTOR_LEMURIA_SPRING_PRIZE, 0, 0);
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  } else {
    Call2(ItemDisplay_ShowItem, a0, 3);
    Party_GiveItem(a0, 0);
    FieldEvent_Wait(20);
  }
  MapGrid_CopyCellsTo(97, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(94, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(91, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(88, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(85, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(82, 29, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(100, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(97, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(94, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(91, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(88, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(85, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(82, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(79, 20, 70, 0, 3, 8);
  Event_PlaySound(SE_CAST);
  FieldEvent_Wait(8);
  Event_End();
}

void LemuriaSpring_SetActorAppearance(s32 id, s32 *pos, s32 dir, s32 palette,
                                      s32 value) {
  struct Actor *object = Actor_Get(id);
  u8 *sprite;
  u8 **list;
  s32 i;

  if (object != NULL) {
    struct Half zero;

    object->x.fixed = *pos++;
    object->y.fixed = *pos++;
    object->z.fixed = *pos;
    object->facing = dir;
    zero.v = 0;
    object->motionFlags = zero.v;
    ((u8 *)object->sprite)[26] = zero.v;
    Actor_SetAnimationSpeed(object, value);
  }
  sprite = object->sprite;
  i = 0;
  if (i >= sprite[27])
    return;
  list = (u8 **)(sprite + 40);
next:
  {
    u8 *cell = *list++;

    if (cell[5] != palette) {
      cell[5] = palette;
      cell[22] = 255;
    }
  }
  i++;
  if (i < sprite[27])
    goto next;
}

void LemuriaSpring_SetSpriteStorageKind(s32 a0, s32 a1) {
  struct Actor *object;

  object = Actor_GetById(a0);
  if (object != 0) {
    object->spriteStorageKind = a1;
  }
}

void LemuriaSpring_UpdateTossSimulation(void) {
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
  // Unused: gives the stack frame its 12 bytes.
  struct Vector3 spare;

  toss = &gLemuriaSpring_Toss;
  index = 3;
  do {
    toss->trail[index].x = toss->trail[index - 1].x;
    toss->trail[index].y = toss->trail[index - 1].y;
    toss->trail[index].z = toss->trail[index - 1].z;
    index--;
  } while (index != 0);

  if (toss->launchFrame > 31) {
    toss->trail[0].x += toss->velocity.x;
    height = toss->trail[0].y + toss->velocity.y;
    toss->trail[0].y = height;
    toss->trail[0].z += toss->velocity.z;

    if (height <= 0) {
      toss->trail[0].y = 0;
      if (toss->velocity.y != 0) {
        toss->velocity.y = 0;
        if (gLemuriaSpring_TossChoice == 1) {
          Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_MEDAL), 1);
        } else {
          Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_COIN), 1);
        }
      }

      if (toss->attractionFrames > 0) {
        s32 deltaX;
        s32 deltaZ;
        s32 distance;

        deltaX = 0x780000 - toss->trail[0].x;
        deltaZ = 0x470000 - toss->trail[0].z;
        deltaX >>= 8;
        deltaZ >>= 8;
        distance = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        toss->velocity.x += (0x1999 * deltaX) / distance;
        toss->velocity.z += (0x1999 * deltaZ) / distance;
        toss->velocity.x = toss->velocity.x * 253 / 256;
        toss->velocity.z = toss->velocity.z * 253 / 256;
        toss->attractionFrames = toss->attractionFrames - 1;
      } else {
        toss->velocity.x = toss->velocity.x * 220 / 256;
        toss->velocity.z = toss->velocity.z * 220 / 256;
        if (toss->velocity.x > -1024 && toss->velocity.x < 1024) {
          toss->velocity.x = 0;
        }
        if (toss->velocity.z > -1024 && toss->velocity.z < 1024) {
          toss->velocity.z = 0;
        }
        if (toss->velocity.x == 0 && toss->velocity.z == 0) {
          if (gLemuriaSpring_TossChoice == 1) {
            Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_MEDAL), 2);
            LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(1),
                                               0);
            LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(2),
                                               0);
            LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(3),
                                               0);
          } else {
            Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_COIN), 2);
            LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_COIN_TRAIL(1), 0);
            LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_COIN_TRAIL(2), 0);
            LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_COIN_TRAIL(3), 0);
          }
          {
            s32 deltaX = (0x780000 - toss->trail[0].x) >> 16;
            s32 deltaZ = (0x470000 - toss->trail[0].z) >> 16;
            s32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

            gLemuriaSpring_TossDone = 1;
            if (distanceSquared <= 224) {
              gLemuriaSpring_TossResult = 0;
            } else if (distanceSquared <= 624) {
              gLemuriaSpring_TossResult = 1;
            } else if (distanceSquared <= 1088) {
              gLemuriaSpring_TossResult = 2;
            } else if (distanceSquared <= 1680) {
              gLemuriaSpring_TossResult = 3;
            } else {
              gLemuriaSpring_TossResult = 4;
            }
          }
        }
      }

      minX = 0x300000;
      maxX = 0xc00000;
      minZ = 0x180000;
      maxZ = 0x780000;
      if (toss->trail[0].z < px(42)) {
        edgeInset = (0x2a0000 - toss->trail[0].z) * 42 / 18;
        minX += edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX -= edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      if (toss->trail[0].z > px(102)) {
        edgeInset = (toss->trail[0].z * 42 - 0x660000 * 42) / 18;
        minX = 0x300000 + edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX = 0xc00000 - edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      if (toss->trail[0].x < px(90)) {
        edgeInset = (0x5a0000 - toss->trail[0].x) * 18 / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }
      if (toss->trail[0].x > px(150)) {
        edgeInset = (toss->trail[0].x * 18 - 0x960000 * 18) / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }

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
      toss->velocity.y -= px(1) / 4;
    }
  }

  index = 0;
  do {
    obstacle = &gLemuriaSpring_Obstacles[index];

    if (obstacle->reactionFrames > 0) {
      obstacle->reactionFrames = obstacle->reactionFrames - 1;
    }
    if (obstacle->hitCooldownFrames > 0) {
      obstacle->hitCooldownFrames = obstacle->hitCooldownFrames - 1;
    }
    if (index <= 1) {
      speedMode = obstacle->speedMode;
      motionStep = 0x10000;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        if (index == 0) {
          Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_CRAB_A), 3);
        } else {
          Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_CRAB_B), 3);
        }
      } else {
        if (index == 0) {
          Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_CRAB_A), 1);
        } else {
          Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_CRAB_B), 1);
        }
        if (obstacle->pauseOrPhase == 0) {
          s32 pos;

          pos = obstacle->position.x;
          if (obstacle->heading == 0) {
            obstacle->position.x = pos + motionStep;
          } else {
            obstacle->position.x = pos - motionStep;
          }
          if (obstacle->position.x <= px(64)) {
            obstacle->heading = 0;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
          if (obstacle->position.x > 0xafffff) {
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
        Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_TURTLE_A), 3);
      } else {
        Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_TURTLE_A), 2);
        obstacle->position.x = Trig_Sin(obstacle->heading) * 48 + 0x700000;
        obstacle->position.z = Trig_Cos(obstacle->heading) * 40 + 0x480000;
        /* FAKEMATCH(temporary): the new heading held in cycleFrame, the local
         * the last obstacle keeps its phase in. */
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
        Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_TURTLE_B), 3);
      } else if (cycleFrame <= 383) {
        obstacle->position.x = Trig_Sin(obstacle->heading) * 52 + 0x700000;
        obstacle->position.z = Trig_Cos(obstacle->heading) * 24 + 0x480000;
        obstacle->heading = obstacle->heading + motionStep;
        Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_TURTLE_B), 2);
      } else {
        Actor_SelectAnimation(Actor_GetById(ACTOR_LEMURIA_SPRING_TURTLE_B), 3);
      }
      obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
    }

    if (obstacle->hitCooldownFrames == 0 && toss->trail[0].y == 0) {
      s32 distanceSquared;
      s32 deltaX;
      s32 deltaZ;

      deltaX = (obstacle->position.x - toss->trail[0].x) >> 16;
      deltaZ = (obstacle->position.z - toss->trail[0].z) >> 16;
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && toss->attractionFrames > 30) {
        s32 nextMode;
        s32 speed;

        speed = 0x30000;
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

          distance = Iwram_Sqrt(distanceSquared);
          toss->velocity.x = -deltaX * speed / distance;
          toss->velocity.z = -deltaZ * speed / distance;
          toss->attractionFrames = toss->attractionFrames - 100;
        }
        Event_PlaySound(SE_TOLBI_SPRING_OBSTACLE_HIT);
        nextMode = (obstacle->speedMode + 1) % 3;
        obstacle->reactionFrames = 36;
        obstacle->speedMode = nextMode;
        obstacle->hitCooldownFrames = 30;
      }
    }

    switch (index) {
    case 0:
      LemuriaSpring_SetActorAppearance(
          18, &obstacle->position.x, 0,
          gLemuriaSpring_FrontObstacleAnimations[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 1:
      LemuriaSpring_SetActorAppearance(
          19, &obstacle->position.x, 0,
          gLemuriaSpring_FrontObstacleAnimations[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 2:
      LemuriaSpring_SetActorAppearance(
          20, &obstacle->position.x, 0x8000 - obstacle->heading,
          gLemuriaSpring_BackObstacleAnimations[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 3:
      LemuriaSpring_SetActorAppearance(
          21, &obstacle->position.x, 0xffff - obstacle->heading,
          gLemuriaSpring_BackObstacleAnimations[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    }
    index++;
  } while (index != 4);

  toss->shadow.x = toss->trail[0].x;
  toss->shadow.y = 0;
  toss->shadow.z = toss->trail[0].z;

  if (gLemuriaSpring_TossChoice == 1) {
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL,
                                     &toss->trail[0].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW,
                                     &toss->shadow.x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(1),
                                     &toss->trail[1].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(2),
                                     &toss->trail[2].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(3),
                                     &toss->trail[3].x, 0, 0, 16);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_MEDAL_TRAIL(1)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_MEDAL_TRAIL(2)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_MEDAL_TRAIL(3)), 4);
  } else {
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_COIN,
                                     &toss->trail[0].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_COIN_SHADOW,
                                     &toss->shadow.x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(1),
                                     &toss->trail[1].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(2),
                                     &toss->trail[2].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(3),
                                     &toss->trail[3].x, 0, 0, 16);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_COIN_TRAIL(1)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_COIN_TRAIL(2)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_COIN_TRAIL(3)), 4);
  }

  if (toss->launchFrame != -1) {
    toss->launchFrame = toss->launchFrame + 1;
  }
}
#undef FIELD_AT
#undef COUNT_WINDOW_WIDTH
#undef COUNT_NUMBER_X
#undef TOSS_WINDOW_X
#undef TOSS_WINDOW_WIDTH
#undef WEIGHT

void LemuriaSpring_UpdateTossSimulation(void);

/// Places the four actors from their tables, resets the ride and installs
/// its task.
void LemuriaSpring_InitializeTossSimulation(void) {
  struct SpringRide *ride = &gLemuriaSpring_Toss;
  s32 i = 0;
  u8 *xtbl;
  u16 *htbl;
  struct SpringObstacle *rec;
  u8 *ztbl;
  s32 zero;

  htbl = gLemuriaSpring_ObstacleHeadings;
  ztbl = gLemuriaSpring_ObstacleStartZ;
  xtbl = gLemuriaSpring_ObstacleStartX;
  rec = gLemuriaSpring_Obstacles;

  do {
    zero = 0;
    rec->position.x = *xtbl << 16;
    /* FAKEMATCH(barrier): the x store ends the first group. */
    __asm__("");
    rec->position.z = *ztbl << 16;
    rec->position.y = zero;
    rec->heading = *htbl;
    rec->pauseOrPhase = zero;
    rec->speedMode = zero;
    rec->reactionFrames = zero;
    rec->hitCooldownFrames = zero;
    i++;
    xtbl++;
    ztbl++;
    htbl++;
    rec++;
  } while (i != 4);

  ride->trail[0].x = -px(30);
  ride->trail[0].y = 0;
  ride->trail[0].z = px(100);
  ride->velocity.x = 0;
  ride->velocity.y = 0;
  ride->velocity.z = 0;
  ride->frames = 0;

  Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_TURTLE_A), 2);
  Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_TURTLE_B), 2);

  Task_AddFrame((TaskFunc)LemuriaSpring_UpdateTossSimulation,
                TASK_PASS_VBLANK + 3);
}

/// Runs the spring ride from one side until it ends, and returns its result.
s32 LemuriaSpring_RunToss(s32 side) {
  struct SpringRide *ride;

  ride = &gLemuriaSpring_Toss;
  ride->trail[0].y = 0;
  ride->trail[1].y = 0;
  ride->trail[2].y = 0;
  ride->trail[3].y = 0;
  gLemuriaSpring_TossChoice = side;
  gLemuriaSpring_TossDone = 0;
  ride->hold = 0xffff;
  for (gLemuriaSpring_TossFrame = 0;; gLemuriaSpring_TossFrame++) {
    if (gLemuriaSpring_TossFrame == 50)
      Event_PlaySound(SE_TOLBI_SPRING_TOSS);
    if (gLemuriaSpring_TossFrame == 16) {
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, 39);
      ride->hold = 0;
      ride->velocity.x = 0x14ccc;
      ride->velocity.y = px(4);
      ride->velocity.z = -px(2);
      ride->trail[0].x = px(120);
      ride->trail[0].y = px(16);
      ride->trail[0].z = px(152);
      ride->frames = 300;
      if (gLemuriaSpring_TossChoice == 1) {
        Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW), 3);
        Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_MEDAL), 0);
        LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(1), 1);
        LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(2), 1);
        LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(3), 1);
      } else {
        Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_COIN_SHADOW), 3);
        Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_COIN), 0);
        LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_COIN_TRAIL(1), 1);
        LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_COIN_TRAIL(2), 1);
        LemuriaSpring_SetSpriteStorageKind(LEMURIA_SPRING_COIN_TRAIL(3), 1);
      }
    }
    System_Wait(1);
    if (gLemuriaSpring_TossDone == 1)
      break;
  }
  return gLemuriaSpring_TossResult;
}

void LemuriaSpring_DrinkFromSpring(void);
void LemuriaSpring_TossCoin(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemuriaSpring),
    SCENE_EXIT_UNLESS(1, Lemuria, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, 7, 176, 0, 144, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, 7, 72, 0, 144, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, 7, 176, 0, 168, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, 7, 72, 0, 168, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 48, 0, 136,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(165, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 96,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    {322, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
/// The blend weights the spring fades in through, eight steps from 10/9 to
/// 16/0 (first target over second, in sixteenths).
u16 gLemuriaSpring_AlphaSteps[] = {
    0x090a, 0x0a08, 0x0b06, 0x0c04, 0x0d03, 0x0e02, 0x0f01, 0x1000,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_MEDAL_SIGN, CONDITION_ALWAYS, MSGNO(MsgReadTheSign)},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_COIN_SIGN, CONDITION_ALWAYS, MSGNO(MsgReadTheSignThe)},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIfYouFaceAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIdLoveToTest)},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIRememberOutsiderWhoCame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringHeHadSuchGoodTime)},
    {TRIGGER_EXAMINE, ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS, 0x200, EVENT(LemuriaSpring_DrinkFromSpring)},
    {TRIGGER_EXAMINE, 80, CONDITION_ALWAYS, EVENT(LemuriaSpring_TossCoin)},
    {TRIGGER_TABLE_END},
};

/// The items a medal can win, drawn by the weights in gMainRoutineWork.
u32 gLemuriaSpring_MedalPrizes[] = {
    280, 359, 335, 380, 302, 342,
    387, 372, 320, 395, 364, 341,
    183, 186, 189,
};

/// The coins a coin wins by where it lands, the middle first; 0 for a miss.
u16 gLemuriaSpring_CoinPrizes[] = {
    20, 10, 2, 1, 0,
};

/// The animations of the spring's two pairs of moving obstacles by their speed.
u8 gLemuriaSpring_FrontObstacleAnimations[] = {
    0, 1, 9,
};

u8 gLemuriaSpring_BackObstacleAnimations[] = {
    0, 3, 4,
};

u8 gLemuriaSpring_ObstacleStartX[] = {
    80, 160, 80, 72,
};

u8 gLemuriaSpring_ObstacleStartZ[] = {
    32, 104, 68, 72,
};

u16 gLemuriaSpring_ObstacleHeadings[] = {
    0, 1, 0, 0x8000,
};
// clang-format on
