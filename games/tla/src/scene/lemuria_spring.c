/// @file
/// Lemuria's spring: the party drinks from it to restore its HP and PP, and
/// tosses coins or Lucky Medals into it as in Tolbi's spring game, past the
/// moving obstacles, for coins or for items.

#include "battle/battle.h"
#include "field/map_grid.h"
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
#include "macros.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
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
#include "text/msg_ids.h"

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

s32 LemuriaSpring_RunSpringGame(void);

extern u16 gLemuriaSpring_CoinPrizes[];
extern u32 gLemuriaSpring_MedalPrizes[];

s32 LemuriaSpring_RunToss();

void LemuriaSpring_GivePrize(s32 item);

void SummonTablet_PlayRunes(s32 targetId);

s32 ItemDisplay_ShowItem();

extern u8 gLemuriaSpring_CrabColorModes[];
extern u8 gLemuriaSpring_TurtleColorModes[];
extern u8 gLemuriaSpring_ObstacleStartX[];
extern u8 gLemuriaSpring_ObstacleStartZ[];

extern u16 gLemuriaSpring_ObstacleHeadings[];

void LemuriaSpring_SetSpriteStorageKind(s32 actor, s32 active);

s32 Party_RestoreHpPp();
s32 Class_GetSeries();

struct SpringTossMotion gLemuriaSpring_Toss BSS_DATA ALIGNED(16);
s32 gLemuriaSpring_TossChoice BSS_DATA;
struct SpringObstacle gLemuriaSpring_Obstacles[4] BSS_DATA ALIGNED(16);
s32 gLemuriaSpring_TossFrame BSS_DATA;
s32 gLemuriaSpring_TossDone BSS_DATA;
s32 gLemuriaSpring_TossResult BSS_DATA;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

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
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer;
  s32 i;

  Event_PlaySound(SE_BURST);
  layer = &map->layers[2];
  for (i = 0; i < 16; i++) {
    layer->offsetY -= toFIXED(1);
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
/// into view the first time in the visit (FLAG_SCENE_TEMP_00), as Tolbi's does.
void LemuriaSpring_TossCoin(void) {
  s32 actor = gMainRoutineWork.leaderActorId;

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

/// Starts the scene: sets the colour blend, the grass at the signs' feet
/// animated and drawn underfoot, and starts the screen transition; arriving by
/// entrance 1, it sets the toss up and, once FLAG_SCENE_TEMP_00 is set, blends
/// with weights 0 and 16.
s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 blend;
  s32 alpha;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  leader->priorityFlags = ACTOR_PRIORITY_IGNORES_RISES | leader->priorityFlags;
  blend = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDCNT = blend;
  /* FAKEMATCH(dead): the form kept; as no block, 5 of 80 instructions differ in
   * tla-en. */
  do {
  } while (0);
  alpha = BLEND_ALPHA(12, 8);
  REG_BLDALPHA = alpha;
  Actor_SetAnimation(ACTOR_LEMURIA_SPRING_EAST_SIGN_GRASS, 2);
  Actor_SetAnimation(ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS, 2);
  Actor_Get(ACTOR_LEMURIA_SPRING_EAST_SIGN_GRASS)->priorityFlags =
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS)->priorityFlags =
      ACTOR_PRIORITY_UNDERFOOT;
  Event_StartScreenTransition();
  if (gMainRoutineWork.entrance == 1) {
    LemuriaSpring_InitializeTossSimulation();
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
      Io_Write16(blend, &REG_BLDCNT);
      Io_Write16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
    }
  }
  return 0;
}

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
      if (result == 0 && Event_GetFlag(FLAG_LEMURIA_SPRING_ECLIPSE_WON) == 0) {
        Event_SetFlag(FLAG_LEMURIA_SPRING_ECLIPSE_WON);
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
/// medal to land in the middle (FLAG_LEMURIA_SPRING_ECLIPSE_WON), the summon
/// Eclipse, read from a tablet the prize actor brings.
void LemuriaSpring_GivePrize(s32 item) {
  struct Actor *prize;

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
  if (item == -1) {
    prize = Actor_Get(ACTOR_LEMURIA_SPRING_PRIZE);
    Actor_MoveToObject(ACTOR_LEMURIA_SPRING_PRIZE,
                       gMainRoutineWork.leaderActorId);
    prize->motionFlags = 0;
    prize->y.fixed = toFIXED(32);
    FieldActor_SetAnimation(gMainRoutineWork.leaderActorId,
                            ANIM_LEADER_ARMS_RAISED);
    SummonTablet_PlayRunes(ACTOR_LEMURIA_SPRING_PRIZE);
    Summon_ShowLearnedMessage(SUMMON_ECLIPSE);
    Class_GetSeries(SUMMON_ECLIPSE);
    Actor_SetPosition(ACTOR_LEMURIA_SPRING_PRIZE, 0, 0);
    FieldActor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  } else {
    Call2(ItemDisplay_ShowItem, item, 3);
    Party_GiveItem(item, 0);
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

/// Sets an actor of the toss where the simulation has it: its position,
/// facing, colour mode and animation step.
void LemuriaSpring_SetActorAppearance(s32 id, s32 *position, s32 facing,
                                      s32 colorMode, s32 step) {
  struct Actor *actor = Actor_Get(id);
  struct Sprite *sprite;
  struct SpriteAnimation **entry;
  s32 i;

  if (actor != NULL) {
    actor->x.fixed = *position++;
    actor->y.fixed = *position++;
    actor->z.fixed = *position;
    actor->facing = facing;
    actor->motionFlags = 0;
    ((struct Sprite *)actor->sprite)->flags = 0;
    Actor_SetAnimationSpeed(actor, step);
  }
  sprite = actor->sprite;
  i = 0;
  if (i < sprite->count) {
    entry = sprite->entries;
    do {
      struct SpriteAnimation *animation = *entry++;

      if (animation->colorMode != colorMode) {
        animation->colorMode = colorMode;
        animation->picture = SPRITE_PICTURE_NONE;
      }
      i++;
    } while (i < sprite->count);
  }
}

/// Sets how actor `id` keeps its sprites (Actor.spriteStorageKind).
void LemuriaSpring_SetSpriteStorageKind(s32 id, s32 kind) {
  struct Actor *actor;

  actor = Actor_GetById(id);
  if (actor != 0) {
    actor->spriteStorageKind = kind;
  }
}

/// A task: moves the tossed coin or medal a frame, its trail and shadow
/// behind it, and the crabs and turtles that swim in the spring.
void LemuriaSpring_UpdateTossSimulation(void) {
  struct SpringTossMotion *toss;
  struct SpringObstacle *obstacle;
  s32 index;
  s32 height;
  FIXED edgeInset;
  s32 motionStep;
  s32 speedMode;
  s32 cycleFrame;
  FIXED minX;
  FIXED maxX;
  FIXED minZ;
  FIXED maxZ;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
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

        deltaX = toFIXED(120) - toss->trail[0].x;
        deltaZ = toFIXED(71) - toss->trail[0].z;
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
            s32 deltaX = (toFIXED(120) - toss->trail[0].x) >> 16;
            s32 deltaZ = (toFIXED(71) - toss->trail[0].z) >> 16;
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

      minX = toFIXED(48);
      maxX = toFIXED(192);
      minZ = toFIXED(24);
      maxZ = toFIXED(120);
      if (toss->trail[0].z < toFIXED(42)) {
        edgeInset = (toFIXED(42) - toss->trail[0].z) * 42 / 18;
        minX += edgeInset;
        if (minX > toFIXED(90)) {
          minX = toFIXED(90);
        }
        maxX -= edgeInset;
        if (maxX < toFIXED(150)) {
          maxX = toFIXED(150);
        }
      }
      if (toss->trail[0].z > toFIXED(102)) {
        edgeInset = (toss->trail[0].z * 42 - toFIXED(102) * 42) / 18;
        minX = toFIXED(48) + edgeInset;
        if (minX > toFIXED(90)) {
          minX = toFIXED(90);
        }
        maxX = toFIXED(192) - edgeInset;
        if (maxX < toFIXED(150)) {
          maxX = toFIXED(150);
        }
      }
      if (toss->trail[0].x < toFIXED(90)) {
        edgeInset = (toFIXED(90) - toss->trail[0].x) * 18 / 42;
        minZ = toFIXED(24) + edgeInset;
        if (minZ > toFIXED(42)) {
          minZ = toFIXED(42);
        }
        maxZ = toFIXED(120) - edgeInset;
        if (maxZ < toFIXED(102)) {
          maxZ = toFIXED(102);
        }
      }
      if (toss->trail[0].x > toFIXED(150)) {
        edgeInset = (toss->trail[0].x * 18 - toFIXED(150) * 18) / 42;
        minZ = toFIXED(24) + edgeInset;
        if (minZ > toFIXED(42)) {
          minZ = toFIXED(42);
        }
        maxZ = toFIXED(120) - edgeInset;
        if (maxZ < toFIXED(102)) {
          maxZ = toFIXED(102);
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
      toss->velocity.y -= toFIXED(1) / 4;
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
          if (obstacle->position.x <= toFIXED(64)) {
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
        obstacle->position.x = Trig_Sin(obstacle->heading) * 48 + toFIXED(112);
        obstacle->position.z = Trig_Cos(obstacle->heading) * 40 + toFIXED(72);
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
        obstacle->position.x = Trig_Sin(obstacle->heading) * 52 + toFIXED(112);
        obstacle->position.z = Trig_Cos(obstacle->heading) * 24 + toFIXED(72);
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
        FIXED speed;

        speed = toFIXED(3);
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
          ACTOR_LEMURIA_SPRING_CRAB_A, &obstacle->position.x, 0,
          gLemuriaSpring_CrabColorModes[obstacle->speedMode],
          ANIMATION_STEP_NORMAL * (obstacle->speedMode + 1));
      break;
    case 1:
      LemuriaSpring_SetActorAppearance(
          ACTOR_LEMURIA_SPRING_CRAB_B, &obstacle->position.x, 0,
          gLemuriaSpring_CrabColorModes[obstacle->speedMode],
          ANIMATION_STEP_NORMAL * (obstacle->speedMode + 1));
      break;
    case 2:
      LemuriaSpring_SetActorAppearance(
          ACTOR_LEMURIA_SPRING_TURTLE_A, &obstacle->position.x,
          0x8000 - obstacle->heading,
          gLemuriaSpring_TurtleColorModes[obstacle->speedMode],
          ANIMATION_STEP_NORMAL * (obstacle->speedMode + 1));
      break;
    case 3:
      LemuriaSpring_SetActorAppearance(
          ACTOR_LEMURIA_SPRING_TURTLE_B, &obstacle->position.x,
          0xffff - obstacle->heading,
          gLemuriaSpring_TurtleColorModes[obstacle->speedMode],
          ANIMATION_STEP_NORMAL * (obstacle->speedMode + 1));
      break;
    }
    index++;
  } while (index != 4);

  toss->shadow.x = toss->trail[0].x;
  toss->shadow.y = 0;
  toss->shadow.z = toss->trail[0].z;

  if (gLemuriaSpring_TossChoice == 1) {
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL,
                                     &toss->trail[0].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW,
                                     &toss->shadow.x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(1),
                                     &toss->trail[1].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(2),
                                     &toss->trail[2].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(3),
                                     &toss->trail[3].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_MEDAL_TRAIL(1)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_MEDAL_TRAIL(2)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_MEDAL_TRAIL(3)), 4);
  } else {
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_COIN,
                                     &toss->trail[0].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(ACTOR_LEMURIA_SPRING_COIN_SHADOW,
                                     &toss->shadow.x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(1),
                                     &toss->trail[1].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(2),
                                     &toss->trail[2].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    LemuriaSpring_SetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(3),
                                     &toss->trail[3].x, 0, 0,
                                     ANIMATION_STEP_NORMAL);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_COIN_TRAIL(1)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_COIN_TRAIL(2)), 4);
    Actor_SelectAnimation(Actor_GetById(LEMURIA_SPRING_COIN_TRAIL(3)), 4);
  }

  if (toss->launchFrame != -1) {
    toss->launchFrame = toss->launchFrame + 1;
  }
}
#undef COUNT_WINDOW_WIDTH
#undef COUNT_NUMBER_X
#undef TOSS_WINDOW_X
#undef TOSS_WINDOW_WIDTH
#undef WEIGHT

void LemuriaSpring_UpdateTossSimulation(void);

/// Places the four actors from their tables, resets the ride and installs
/// its task.
void LemuriaSpring_InitializeTossSimulation(void) {
  struct SpringTossMotion *ride = &gLemuriaSpring_Toss;
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

  ride->trail[0].x = -toFIXED(30);
  ride->trail[0].y = 0;
  ride->trail[0].z = toFIXED(100);
  ride->velocity.x = 0;
  ride->velocity.y = 0;
  ride->velocity.z = 0;
  ride->attractionFrames = 0;

  Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_TURTLE_A), 2);
  Actor_SelectAnimation(Actor_Get(ACTOR_LEMURIA_SPRING_TURTLE_B), 2);

  Task_AddFrame((TaskFunc)LemuriaSpring_UpdateTossSimulation,
                TASK_PASS_VBLANK + 3);
}

/// Runs the spring ride from one side until it ends, and returns its result.
s32 LemuriaSpring_RunToss(s32 side) {
  struct SpringTossMotion *ride;

  ride = &gLemuriaSpring_Toss;
  ride->trail[0].y = 0;
  ride->trail[1].y = 0;
  ride->trail[2].y = 0;
  ride->trail[3].y = 0;
  gLemuriaSpring_TossChoice = side;
  gLemuriaSpring_TossDone = 0;
  ride->launchFrame = -1;
  for (gLemuriaSpring_TossFrame = 0;; gLemuriaSpring_TossFrame++) {
    if (gLemuriaSpring_TossFrame == 50)
      Event_PlaySound(SE_TOLBI_SPRING_TOSS);
    if (gLemuriaSpring_TossFrame == 16) {
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, 39);
      ride->launchFrame = 0;
      ride->velocity.x = toFIXED(13) / 10;
      ride->velocity.y = toFIXED(4);
      ride->velocity.z = -toFIXED(2);
      ride->trail[0].x = toFIXED(120);
      ride->trail[0].y = toFIXED(16);
      ride->trail[0].z = toFIXED(152);
      ride->attractionFrames = 300;
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
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, 176, 0, 144,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, 72, 0, 144,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, 176, 0, 168,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, 72, 0, 168,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    48, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_IN_TEAL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 216, 0, 96, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_TABLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    -56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
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
    {TRIGGER_EXAMINE, ACTOR_LEMURIA_SPRING_WEST_SIGN_GRASS, FLAG_SCENE_TEMP_00, EVENT(LemuriaSpring_DrinkFromSpring)},
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

/// The crabs' colour modes, by speed.
u8 gLemuriaSpring_CrabColorModes[] = {
    SPRITE_COLOR_NORMAL, SPRITE_COLOR_GRB, SPRITE_COLOR_BLUE_TINT,
};

/// The turtles' colour modes, by speed.
u8 gLemuriaSpring_TurtleColorModes[] = {
    SPRITE_COLOR_NORMAL, SPRITE_COLOR_BGR, SPRITE_COLOR_GBR,
};

/// Where each obstacle starts, across.
u8 gLemuriaSpring_ObstacleStartX[] = {
    80, 160, 80, 72,
};

/// Where each obstacle starts, down.
u8 gLemuriaSpring_ObstacleStartZ[] = {
    32, 104, 68, 72,
};

/// The way each obstacle starts heading.
u16 gLemuriaSpring_ObstacleHeadings[] = {
    0, 1, 0, 0x8000,
};
// clang-format on
