/// @file
/// Lemuria's spring: the party drinks from it to restore its HP and PP, and
/// tosses coins or Lucky Medals into it as in Tolbi's spring game, past the
/// moving obstacles, for coins or for items.

#include "battle/battle.h"
#include "system/io_write.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/game/game.h"

/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

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

#include "module/scene/scene.h"
#include "module/tla.h"
#include "module/window/window.h"
#include "text/msg_ids.h"
#include "graphics/text_render.h"
#include "graphics/uiflow.h"

extern struct SpringTossMotion gLemuriaSpring_Toss;
extern struct SpringObstacle gLemuriaSpring_Obstacles[];
extern s32 gLemuriaSpring_TossDone;
extern s32 gLemuriaSpring_TossResult;
void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern s32 gLemuriaSpring_TossFrame;
extern s32 gLemuriaSpring_TossChoice;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
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
  Message_ShowCentered((s32)MsgAndTheOthers, 1);
  Event_PlaySound(0x7e);
  Party_RestoreHpPp(1);
}

extern u16 gLemuriaSpring_AlphaSteps[];

/// Raises the spring's displayed layer and blends it into view, as Tolbi's
/// does.
void LemuriaSpring_RiseAndFadeIn(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  Event_PlaySound(216);
  layer = (s32 *)(map + 0x178);
  for (i = 0; i < 16; i++) {
    layer[3] -= PIXELS(1);
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
  s32 t0;
  s32 actor = FIELD_AT(&gMainRoutineWork, s32, 532);

  t0 = Event_GetFlag(0x200);
  if (t0 == 0) {
    Event_SetFlag(0x200);
    LemuriaSpring_RiseAndFadeIn();
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(actor, 120, 152);
  Actor_FaceDirection(actor, FACING_SOUTH, 0);
  LemuriaSpring_RunSpringGame();
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "system/main_routine_work.h"

void LemuriaSpring_InitializeTossSimulation(void);

/// Starts the scene: sets the colour blend, puts objects 24 and 25 in mode 2
/// and starts the screen transition; when gMainRoutineWork's 0x1e2 halfword is
/// 1 and flag 0x200 is set, it sets the blend's weights again.
s32 Scene_Initialize(void) {
  s32 t0;
  s32 t3;
  s32 t4;
  s32 t7;
  s32 blend;
  s32 alpha;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x100;
  t0 = (s32)Actor_Get(gMainRoutineWork.currentOwner);
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  blend = 0x3f42;
  FIELD_AT(0x4000050, u16, 0) = blend;
  // A once-only block: its end holds the order around it (as the plain
  // statements, 8 of 80 differ in tla-en).
  do {
  } while (0);
  alpha = 0x80c;
  FIELD_AT(0x4000052, u16, 0) = alpha;
  Actor_SetAnimation(0x18, 2);
  Actor_SetAnimation(0x19, 2);
  t3 = (s32)Actor_Get(0x18);
  FIELD_AT((t3 + 0x23), u8, 0) = 2;
  t4 = (s32)Actor_Get(0x19);
  FIELD_AT((t4 + 0x23), u8, 0) = 2;
  Event_StartScreenTransition();
  if (gMainRoutineWork.entrance == 1) {
    LemuriaSpring_InitializeTossSimulation();
    t7 = Event_GetFlag(0x200);
    if (t7 != 0) {
      FIELD_AT(0x4000050, u16, 0) = blend;
      FIELD_AT(0x4000052, u16, 0) = 0x1000;
    }
  }
  return 0;
}

#include "text/msg_ids.h"
#include "version.h"
#include "edition.h"
#include "system/input.h"
#include "actor.h"

s32 Scene_PrepareMap(void) { return 0; }

// The Japanese catalog does not name these yet.
extern u8 MsgWhatWillYou[], MsgCoins2[], MsgToss2[], MsgYouWonCoin[];
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

#define WEIGHT(i) ((s8)gMainRoutineWork.unk13d[i])

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
  /* FAKEMATCH(temporary): minusOne, a 64-bit -1 for these calls, so CSE does
   * not share a compare's -1 with them; as -1, 57 of 286 instructions differ in
   * tla-en and 68 of 300 in tla-ja. */
  s64 minusOne = -1;

#if TLA_PATCH_1
  Event_Start();
  choice = 0;
  Event_ResolvePendingActions(0);
prompt:
  coins = gMainRoutineWork.coins;
  medals = PartyInventory_CountItem(229);
  message = (s32)MsgWhatWillYou;
  Event_SetMessageId(message);
  Event_OpenMessage(-1, 0);
  window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgCoins2;
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
    Event_OpenMessage(minusOne, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
      goto end;
  }
toss:
  window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgToss2;
  Text_DrawMessageAsTiles(result, window, 0, 0);
  Text_DrawMessageAsTiles(result + 1, window, 0, 8);
  wait(5);
  Event_PlaySound(116);
  for (;;) {
    if (gInput.pressed & 1) {
      Event_PlaySound(112);
      result = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Event_PlaySound(113);
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
    medals = PartyInventory_CountItem(229);
    message = (s32)MsgWhatWillYou;
    Event_SetMessageId(message);
    Event_OpenMessage(-1, 0);
    window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
    text = (s32)MsgCoins2;
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
        Event_ShowMessage(-1, 0);
        wait(1);
        continue;
      }
      Party_AdjustCoins(-1);
    } else if (choice == 1) {
      if (medals == 0) {
        Event_SetMessageId(message + 2);
        Event_ShowMessage(-1, 0);
        wait(1);
        continue;
      }
      PartyInventory_Remove(229);
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessageId(message + 4);
        Event_OpenMessage(-1, 0);
        if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
          goto end;
      }
    }
    window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
    result = (s32)MsgToss2;
    Text_DrawMessageAsTiles(result, window, 0, 0);
    Text_DrawMessageAsTiles(result + 1, window, 0, 8);
    wait(5);
    Event_PlaySound(116);
    for (;;) {
      if (gInput.pressed & 1) {
        Event_PlaySound(112);
        result = 0;
        break;
      }
      if (gInput.pressed & 2) {
        Event_PlaySound(113);
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
      PartyInventory_Remove(229);
    }
#endif
    result = LemuriaSpring_RunToss(choice);
    if (choice == 0) {
      if (result != 4) {
        Party_AdjustCoins(gLemuriaSpring_CoinPrizes[result]);
        Event_PlaySound(91);
        MessageArgument_Push(gLemuriaSpring_CoinPrizes[result], 5);
        Event_SetMessageId((s32)MsgYouWonCoin);
        Event_ShowMessage((s32)minusOne, 0);
      } else {
        Event_PlaySound(113);
        FieldEvent_Wait(10);
      }
    } else {
      total = 0;
      if (result == 0 && Event_GetFlag(0x90b) == 0) {
        Event_SetFlag(0x90b);
        LemuriaSpring_GivePrize((s32)minusOne);
      } else {
        for (prize = 0; prize < result * 3 + 3; prize++)
          total += (s8)gMainRoutineWork.unk13d[prize];
        prizeRoll = (u32)(Random16() * total) >> 16;
        for (prize = 0; prize < 15; prize++) {
          prizeRoll -= WEIGHT(prize);
          if (prizeRoll < 0)
            break;
        }
        if (prize == 15)
          prize = 14;
        LemuriaSpring_GivePrize(gLemuriaSpring_MedalPrizes[prize]);
        if (WEIGHT(prize) > 1)
          gMainRoutineWork.unk13d[prize] = WEIGHT(prize) / 2;
      }
    }
  }
end:
  Event_End();
  return 0;
}

#include "call.h"
s32 Actor_MoveToObject();
s32 Summon_ShowLearnedMessage();

/// The spring's statue opens and gives the prize: an item, or, for the first
/// medal to land in the middle (flag 0x90b), the special prize actor 29 brings.
void LemuriaSpring_GivePrize(s32 a0) {
  s32 t52;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(30);
  Event_PlaySound(0x94);
  FieldEvent_Wait(100);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(0x52, 0x14, 0x46, 0, 3, 8);
  FieldEvent_Wait(3);
  MapGrid_CopyCellsTo(0x55, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x4f, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(70);
  Event_PlaySound(0x7e);
  if (a0 == -1) {
    t52 = Actor_Get(0x1d);
    Actor_MoveToObject(0x1d, FIELD_AT(&gMainRoutineWork, s32, 532));
    FIELD_AT((t52 + 0x55), u8, 0) = 0;
    FIELD_AT(t52, s32, 12) = 0x200000;
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 0x1c);
    SummonTablet_PlayRunes(0x1d);
    Summon_ShowLearnedMessage(0x16);
    Class_GetSeries(0x16);
    Actor_SetPosition(0x1d, 0, 0);
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  } else {
    Call2(ItemDisplay_ShowItem, a0, 3);
    Party_GiveItem(a0, 0);
    FieldEvent_Wait(20);
  }
  MapGrid_CopyCellsTo(0x61, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x4f, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
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

    object->x = *pos++;
    object->y = *pos++;
    object->z = *pos;
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
    object->animationKind = a1;
  }
}

#include "system/iwram_call.h"
#include "system/vector3.h"

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
          Actor_SelectAnimation(Actor_GetById(17), 1);
        } else {
          Actor_SelectAnimation(Actor_GetById(12), 1);
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
            Actor_SelectAnimation(Actor_GetById(17), 2);
            LemuriaSpring_SetSpriteStorageKind(15, 0);
            LemuriaSpring_SetSpriteStorageKind(14, 0);
            LemuriaSpring_SetSpriteStorageKind(13, 0);
          } else {
            Actor_SelectAnimation(Actor_GetById(12), 2);
            LemuriaSpring_SetSpriteStorageKind(10, 0);
            LemuriaSpring_SetSpriteStorageKind(9, 0);
            LemuriaSpring_SetSpriteStorageKind(8, 0);
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
      if (toss->trail[0].z < 0x2a0000) {
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
      if (toss->trail[0].z > 0x660000) {
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
      if (toss->trail[0].x < 0x5a0000) {
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
      if (toss->trail[0].x > 0x960000) {
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
      toss->velocity.y -= 0x4000;
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
          Actor_SelectAnimation(Actor_GetById(18), 3);
        } else {
          Actor_SelectAnimation(Actor_GetById(19), 3);
        }
      } else {
        if (index == 0) {
          Actor_SelectAnimation(Actor_GetById(18), 1);
        } else {
          Actor_SelectAnimation(Actor_GetById(19), 1);
        }
        if (obstacle->pauseOrPhase == 0) {
          s32 pos;

          pos = obstacle->position.x;
          if (obstacle->heading == 0) {
            obstacle->position.x = pos + motionStep;
          } else {
            obstacle->position.x = pos - motionStep;
          }
          if (obstacle->position.x <= 0x400000) {
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
        Actor_SelectAnimation(Actor_GetById(20), 3);
      } else {
        Actor_SelectAnimation(Actor_GetById(20), 2);
        obstacle->position.x = Trig_Sin(obstacle->heading) * 48 + 0x700000;
        obstacle->position.z = Trig_Cos(obstacle->heading) * 40 + 0x480000;
        /* FAKEMATCH(temporary): the new heading held in cycleFrame, the local
         * the last obstacle keeps its phase in; as obstacle->heading +=
         * motionStep, 15 of 918 instructions differ in tla-en. */
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
        Actor_SelectAnimation(Actor_GetById(21), 3);
      } else if (cycleFrame <= 383) {
        obstacle->position.x = Trig_Sin(obstacle->heading) * 52 + 0x700000;
        obstacle->position.z = Trig_Cos(obstacle->heading) * 24 + 0x480000;
        obstacle->heading = obstacle->heading + motionStep;
        Actor_SelectAnimation(Actor_GetById(21), 2);
      } else {
        Actor_SelectAnimation(Actor_GetById(21), 3);
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
        Event_PlaySound(301);
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
    LemuriaSpring_SetActorAppearance(17, &toss->trail[0].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(16, &toss->shadow.x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(15, &toss->trail[1].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(14, &toss->trail[2].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(13, &toss->trail[3].x, 0, 0, 16);
    Actor_SelectAnimation(Actor_GetById(15), 4);
    Actor_SelectAnimation(Actor_GetById(14), 4);
    Actor_SelectAnimation(Actor_GetById(13), 4);
  } else {
    LemuriaSpring_SetActorAppearance(12, &toss->trail[0].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(11, &toss->shadow.x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(10, &toss->trail[1].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(9, &toss->trail[2].x, 0, 0, 16);
    LemuriaSpring_SetActorAppearance(8, &toss->trail[3].x, 0, 0, 16);
    Actor_SelectAnimation(Actor_GetById(10), 4);
    Actor_SelectAnimation(Actor_GetById(9), 4);
    Actor_SelectAnimation(Actor_GetById(8), 4);
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

#include "system/task.h"

/* VIEW(ride): variable gLemuriaSpring_Toss as extern struct SpringRide
 * gLemuriaSpring_Toss; lemuria_spring has extern struct SpringTossMotion
 * gLemuriaSpring_Toss; tried as the one declaration extern struct SpringRide
 * gLemuriaSpring_Toss, it does not compile in tla-ja:
 * games/tla/src/scene/lemuria_spring.c:957: conflicting types for
 * `gLemuriaSpring_Toss'. */
extern struct SpringRide
    gLemuriaSpring_Toss__ride __asm__("gLemuriaSpring_Toss");
/* VIEW(ride): variable gLemuriaSpring_Obstacles as extern struct
 * SpringObstacle__ride gLemuriaSpring_Obstacles[]; lemuria_spring has extern
 * struct SpringObstacle gLemuriaSpring_Obstacles[]; tried as the rest of the
 * object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/lemuria_spring.c:977: `gLemuriaSpring_Obstacles__ride'
 * undeclared (first use in this function). */
extern struct SpringObstacle
    gLemuriaSpring_Obstacles__ride[] __asm__("gLemuriaSpring_Obstacles");

void LemuriaSpring_UpdateTossSimulation(void);

/// Places the four actors from their tables, resets the ride and installs
/// its task.
void LemuriaSpring_InitializeTossSimulation(void) {
  struct SpringRide *ride = &gLemuriaSpring_Toss__ride;
  s32 i = 0;
  u8 *xtbl;
  u16 *htbl;
  struct SpringObstacle *rec;
  u8 *ztbl;
  s32 zero;

  htbl = gLemuriaSpring_ObstacleHeadings;
  ztbl = gLemuriaSpring_ObstacleStartZ;
  xtbl = gLemuriaSpring_ObstacleStartX;
  rec = gLemuriaSpring_Obstacles__ride;

  do {
    zero = 0;
    rec->position.x = *xtbl << 16;
    /* FAKEMATCH(barrier): the x store ends the first group, so the z table's
     * read follows it at once; as plain stores of 0 with no barrier, 2 of 52
     * instructions differ in tla-en. */
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

  ride->trail[0].x = 0xffe20000;
  ride->trail[0].y = 0;
  ride->trail[0].z = 0x640000;
  ride->velocity.x = 0;
  ride->velocity.y = 0;
  ride->velocity.z = 0;
  ride->frames = 0;

  Actor_SelectAnimation(Actor_Get(20), 2);
  Actor_SelectAnimation(Actor_Get(21), 2);

  Task_AddFrame((TaskFunc)LemuriaSpring_UpdateTossSimulation,
                TASK_PASS_VBLANK + 3);
}

/// Runs the spring ride from one side until it ends, and returns its result.
s32 LemuriaSpring_RunToss(s32 side) {
  struct SpringRide *ride;

  ride = &gLemuriaSpring_Toss__ride;
  ride->trail[0].y = 0;
  ride->trail[1].y = 0;
  ride->trail[2].y = 0;
  ride->trail[3].y = 0;
  gLemuriaSpring_TossChoice = side;
  gLemuriaSpring_TossDone = 0;
  ride->hold = 0xffff;
  for (gLemuriaSpring_TossFrame = 0;; gLemuriaSpring_TossFrame++) {
    if (gLemuriaSpring_TossFrame == 50)
      Event_PlaySound(300);
    if (gLemuriaSpring_TossFrame == 16) {
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 39);
      ride->hold = 0;
      ride->velocity.x = 0x14ccc;
      ride->velocity.y = 0x40000;
      ride->velocity.z = -0x20000;
      ride->trail[0].x = 0x780000;
      ride->trail[0].y = 0x100000;
      ride->trail[0].z = 0x980000;
      ride->frames = 300;
      if (gLemuriaSpring_TossChoice == 1) {
        Actor_SelectAnimation(Actor_Get(16), 3);
        Actor_SelectAnimation(Actor_Get(17), 0);
        LemuriaSpring_SetSpriteStorageKind(15, 1);
        LemuriaSpring_SetSpriteStorageKind(14, 1);
        LemuriaSpring_SetSpriteStorageKind(13, 1);
      } else {
        Actor_SelectAnimation(Actor_Get(11), 3);
        Actor_SelectAnimation(Actor_Get(12), 0);
        LemuriaSpring_SetSpriteStorageKind(10, 1);
        LemuriaSpring_SetSpriteStorageKind(9, 1);
        LemuriaSpring_SetSpriteStorageKind(8, 1);
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
    SCENE_EXITS(205),
    SCENE_EXIT_IF(1, 202, 5, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {669, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {669, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {669, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {669, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {669, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {670, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {670, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {670, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {670, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {670, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {667, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {667, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {668, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {668, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {450, CONDITION_ALWAYS, 7, PIXELS(176), 0, PIXELS(144), FACING_SOUTH, 1, 0},
    {450, CONDITION_ALWAYS, 7, PIXELS(72), 0, PIXELS(144), FACING_SOUTH, 1, 0},
    {450, CONDITION_ALWAYS, 7, PIXELS(176), 0, PIXELS(168), FACING_SOUTH, 1, 0},
    {450, CONDITION_ALWAYS, 7, PIXELS(72), 0, PIXELS(168), FACING_SOUTH, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(48), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {238, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_EAST, 0, 0},
    {165, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(96), FACING_WEST, 1, 0},
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
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgReadTheSign)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgReadTheSignThe)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgIfYouFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgIDLove)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgIRememberAn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgHeHadSuch)},
    {TRIGGER_EXAMINE, 25, 0x200, EVENT(LemuriaSpring_DrinkFromSpring)},
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
