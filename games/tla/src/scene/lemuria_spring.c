// ---- games/tla/src/scene/lemuria_spring.c ----
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
#include "ram_buffer.h"
#include "io_reg.h"
#include "event_inline.h"

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

s32 Func_02001358();
extern u8 MsgAndTheOthers[];

void Func_02000054(void) {
  Message_ShowCentered((s32)MsgAndTheOthers, 1);
  Event_PlaySound(0x7e);
  Func_02001358(1);
}

/* VIEW(lemuria_spring): variable gData_02001678 as extern const u16 gData_02001678[]; the definition has u32 gData_02001678[] = { 0xa08090a, 0xc040b06, 0xe020d03, 0x10000f01, }; tried as the definition's extern u32 gData_02001678[], Func_02000070 moves: 2 of 40 instructions in tla-ja. */
extern const u16 gData_02001678__lemuria_spring[] __asm__("gData_02001678");

static inline void RegSet(s32 value, s32 reg) {
  /* FAKEMATCH(wrapper): the store forwarded through this inline with s32
   * parameters, so 0x3f42 is built in r2 and the register address held in r3;
   * as direct stores to REG_BLDCNT and REG_BLDALPHA, 0x3f42 is a pool word, 27
   * of 40 instructions differ in tla-en. */
  *(u16 *)reg = value;
}

void Func_02000070(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  Event_PlaySound(216);
  layer = (s32 *)(map + 0x178);
  i = 15;
next:
  layer[3] -= 0x10000;
  System_Wait(4);
  i--;
  if (i >= 0)
    goto next;
  i = 0;
next2:
  RegSet(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
         (s32)&REG_BLDCNT);
  RegSet(gData_02001678__lemuria_spring[i], (s32)&REG_BLDALPHA);
  System_Wait(8);
  i++;
  if (i <= 7)
    goto next2;
}

void Func_02001380(void);
s32 Func_020001e4(void);
void Func_02001388(void);

void Func_020000d0(void) {
  s32 t0;
  s32 actor = FIELD_AT(&gMainRoutineWork, s32, 532);

  t0 = Event_GetFlag(0x200);
  if (t0 == 0) {
    Event_SetFlag(0x200);
    Func_02000070();
  }
  Func_02001380();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(actor, 120, 152);
  Actor_FaceDirection(actor, FACING_SOUTH, 0);
  Func_020001e4();
  Func_02001388();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "main_routine_work.h"

s32 Event_StartScreenTransition();
void Func_020010f4(void);

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
    Func_020010f4();
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
#include "input.h"
#include "actor.h"

s32 Scene_PrepareMap(void) { return 0; }

// The Japanese catalog does not name these yet.
extern u8 MsgWhatWillYou[], MsgCoins2[], MsgToss2[], MsgYouWonCoin[];
extern u16 gData_0200173c[];
extern u32 gData_02001700[];
s32 PartyInventory_CountItem();
struct Window *Window_Create();
void Text_DrawCharacterAtOffset();
void Text_DrawNumberInWindow();
s32 Menu_SelectCoinsOrMedals();
void Window_Close();
void Message_CloseFinishedWindows();
s32 PartyInventory_CountFreeSlots();
s32 Party_AdjustCoins(s32 amount);
s32 PartyInventory_Remove();
s32 Func_02001184();
void Func_02001328();
void Func_0200049c(s32 a0);

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

s32 Func_020001e4(void) {
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
  Func_02001380();
  choice = 0;
  Event_ResolvePendingActions(0);
prompt:
  coins = gMainRoutineWork.coins;
  medals = PartyInventory_CountItem(229);
  message = (s32)MsgWhatWillYou;
  Event_SetMessageId(message);
  /* FAKEMATCH(wrapper): forwarded, so the -1 is built after movs r1, #0; as a
   * direct call, 144 of 286 instructions differ in tla-en. */
  Event_OpenMessage(-1, 0);
  window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgCoins2;
  Text_DrawCharacterAtOffset(result, window, 0, 0);
  Text_DrawNumberInWindow(coins, 6, window, COUNT_NUMBER_X, 0);
  Text_DrawCharacterAtOffset(result + 1, window, 0, 8);
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
  Event_ShowMessageAndWait(-1, 0);
  wait(1);
  goto prompt;
full:
  if (PartyInventory_CountFreeSlots() == 0) {
    Event_SetMessageId(message + 4);
    FieldMessage_ShowAtActor(minusOne, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
      goto end;
  }
toss:
  window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgToss2;
  Text_DrawCharacterAtOffset(result, window, 0, 0);
  Text_DrawCharacterAtOffset(result + 1, window, 0, 8);
  wait(5);
  Sound_Play(116);
  for (;;) {
    if (gInput.pressed & 1) {
      Sound_Play(112);
      result = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Sound_Play(113);
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
  Func_02001380();
  Event_ResolvePendingActions(0);
  for (;;) {
    coins = gMainRoutineWork.coins;
    medals = PartyInventory_CountItem(229);
    message = (s32)MsgWhatWillYou;
    Event_SetMessageId(message);
    /* FAKEMATCH(wrapper): forwarded, so the -1 is built after movs r1, #0; as
     * a direct call, 38 of 300 instructions differ in tla-ja. */
    Event_OpenMessage(-1, 0);
    window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
    text = (s32)MsgCoins2;
    Text_DrawCharacterAtOffset(text, window, 0, 0);
    Text_DrawNumberInWindow(coins, 6, window, COUNT_NUMBER_X, 0);
    Text_DrawCharacterAtOffset(text + 2, window, 104, 0);
    Text_DrawCharacterAtOffset(text + 1, window, 0, 8);
    Text_DrawNumberInWindow(medals, 6, window, COUNT_NUMBER_X, 8);
    Text_DrawCharacterAtOffset(text + 2, window, 104, 8);
    choice = Menu_SelectCoinsOrMedals(choice);
    Window_Close(window, 2);
    Message_CloseFinishedWindows();
    if (choice == -1)
      goto end;
    if (choice == 0) {
      if (coins == 0) {
        Event_SetMessageId(message + 1);
        Event_ShowMessageAndWait(-1, 0);
        wait(1);
        continue;
      }
      Party_AdjustCoins(-1);
    } else if (choice == 1) {
      if (medals == 0) {
        Event_SetMessageId(message + 2);
        Event_ShowMessageAndWait(-1, 0);
        wait(1);
        continue;
      }
      PartyInventory_Remove(229);
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessageId(message + 4);
        FieldMessage_ShowAtActor(-1, 0);
        if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
          goto end;
      }
    }
    window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
    result = (s32)MsgToss2;
    Text_DrawCharacterAtOffset(result, window, 0, 0);
    Text_DrawCharacterAtOffset(result + 1, window, 0, 8);
    wait(5);
    Sound_Play(116);
    for (;;) {
      if (gInput.pressed & 1) {
        Sound_Play(112);
        result = 0;
        break;
      }
      if (gInput.pressed & 2) {
        Sound_Play(113);
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
    result = Func_02001184(choice);
    if (choice == 0) {
      if (result != 4) {
        Party_AdjustCoins(gData_0200173c[result]);
        Sound_Play(91);
        Func_02001328(gData_0200173c[result], 5);
        Event_SetMessageId((s32)MsgYouWonCoin);
        Event_ShowMessageAndWait((s32)minusOne, 0);
      } else {
        Sound_Play(113);
        FieldEvent_Wait(10);
      }
    } else {
      total = 0;
      /* FAKEMATCH(wrapper): forwarded, so the flag number is built for each
       * call; as a direct call, 43 of 286 instructions differ in tla-en and 40
       * of 300 in tla-ja. */
      if (result == 0 && Event_GetFlag(0x90b) == 0) {
        Event_SetFlag(0x90b);
        Func_0200049c((s32)minusOne);
      } else {
        for (prize = 0; prize < result * 3 + 3; prize++)
          total += (s8)gMainRoutineWork.unk13d[prize];
        prizeRoll = (u32)(Random16Far() * total) >> 16;
        for (prize = 0; prize < 15; prize++) {
          prizeRoll -= WEIGHT(prize);
          if (prizeRoll < 0)
            break;
        }
        if (prize == 15)
          prize = 14;
        Func_0200049c(gData_02001700[prize]);
        if (WEIGHT(prize) > 1)
          gMainRoutineWork.unk13d[prize] = WEIGHT(prize) / 2;
      }
    }
  }
end:
  Func_02001388();
  return 0;
}

#include "call.h"
s32 Func_020013b8();
s32 Func_02001400();
s32 Func_02001338();
s32 Func_02001370();
s32 Func_020013f0();

void Func_0200049c(s32 a0) {
  s32 t52;

  Func_02001380();
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
    Func_020013b8(0x1d, FIELD_AT(&gMainRoutineWork, s32, 532));
    FIELD_AT((t52 + 0x55), u8, 0) = 0;
    FIELD_AT(t52, s32, 12) = 0x200000;
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 0x1c);
    Func_02001400(0x1d);
    Func_02001338(0x16);
    Func_02001370(0x16);
    Actor_SetPosition(0x1d, 0, 0);
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  } else {
    Call2(Func_020013f0, a0, 3);
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
  Func_02001388();
}

void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

struct Half {
  u16 v;
};

void Func_02000884(s32 id, s32 *pos, s32 dir, s32 palette, s32 value) {
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

void Func_020008f0(s32 a0, s32 a1) {
  struct Actor *object;

  object = Actor_GetById(a0);
  if (object != 0) {
    object->animationKind = a1;
  }
}

#include "iwram_call.h"
#include "vector3.h"

struct SpringTossMotion {
  u8 unused[2];
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
};

extern struct SpringTossMotion gData_02001760;
extern s32 gData_020017b0;
extern struct SpringObstacle gData_020017c0[];
extern s32 gData_02001824;
extern s32 gData_02001828;
extern u8 gData_02001746[];
extern u8 gData_02001749[];

void Func_020012e8(struct Actor *object, s32 mode);
s32 Math_Cosine(s32 angle);

void Func_02000904(void) {
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

  toss = &gData_02001760;
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
        if (gData_020017b0 == 1) {
          Func_020012e8(Actor_GetById(17), 1);
        } else {
          Func_020012e8(Actor_GetById(12), 1);
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
          if (gData_020017b0 == 1) {
            Func_020012e8(Actor_GetById(17), 2);
            Func_020008f0(15, 0);
            Func_020008f0(14, 0);
            Func_020008f0(13, 0);
          } else {
            Func_020012e8(Actor_GetById(12), 2);
            Func_020008f0(10, 0);
            Func_020008f0(9, 0);
            Func_020008f0(8, 0);
          }
          {
            s32 deltaX = (0x780000 - toss->trail[0].x) >> 16;
            s32 deltaZ = (0x470000 - toss->trail[0].z) >> 16;
            s32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

            gData_02001824 = 1;
            if (distanceSquared <= 224) {
              gData_02001828 = 0;
            } else if (distanceSquared <= 624) {
              gData_02001828 = 1;
            } else if (distanceSquared <= 1088) {
              gData_02001828 = 2;
            } else if (distanceSquared <= 1680) {
              gData_02001828 = 3;
            } else {
              gData_02001828 = 4;
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
    obstacle = &gData_020017c0[index];

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
          Func_020012e8(Actor_GetById(18), 3);
        } else {
          Func_020012e8(Actor_GetById(19), 3);
        }
      } else {
        if (index == 0) {
          Func_020012e8(Actor_GetById(18), 1);
        } else {
          Func_020012e8(Actor_GetById(19), 1);
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
        Func_020012e8(Actor_GetById(20), 3);
      } else {
        Func_020012e8(Actor_GetById(20), 2);
        obstacle->position.x = Math_Sine(obstacle->heading) * 48 + 0x700000;
        obstacle->position.z = Math_Cosine(obstacle->heading) * 40 + 0x480000;
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
        Func_020012e8(Actor_GetById(21), 3);
      } else if (cycleFrame <= 383) {
        obstacle->position.x = Math_Sine(obstacle->heading) * 52 + 0x700000;
        obstacle->position.z = Math_Cosine(obstacle->heading) * 24 + 0x480000;
        obstacle->heading = obstacle->heading + motionStep;
        Func_020012e8(Actor_GetById(21), 2);
      } else {
        Func_020012e8(Actor_GetById(21), 3);
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
        Sound_Play(301);
        nextMode = (obstacle->speedMode + 1) % 3;
        obstacle->reactionFrames = 36;
        obstacle->speedMode = nextMode;
        obstacle->hitCooldownFrames = 30;
      }
    }

    switch (index) {
    case 0:
      Func_02000884(18, &obstacle->position.x, 0,
                    gData_02001746[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    case 1:
      Func_02000884(19, &obstacle->position.x, 0,
                    gData_02001746[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    case 2:
      Func_02000884(20, &obstacle->position.x, 0x8000 - obstacle->heading,
                    gData_02001749[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    case 3:
      Func_02000884(21, &obstacle->position.x, 0xffff - obstacle->heading,
                    gData_02001749[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    }
    index++;
  } while (index != 4);

  toss->shadow.x = toss->trail[0].x;
  toss->shadow.y = 0;
  toss->shadow.z = toss->trail[0].z;

  if (gData_020017b0 == 1) {
    Func_02000884(17, &toss->trail[0].x, 0, 0, 16);
    Func_02000884(16, &toss->shadow.x, 0, 0, 16);
    Func_02000884(15, &toss->trail[1].x, 0, 0, 16);
    Func_02000884(14, &toss->trail[2].x, 0, 0, 16);
    Func_02000884(13, &toss->trail[3].x, 0, 0, 16);
    Func_020012e8(Actor_GetById(15), 4);
    Func_020012e8(Actor_GetById(14), 4);
    Func_020012e8(Actor_GetById(13), 4);
  } else {
    Func_02000884(12, &toss->trail[0].x, 0, 0, 16);
    Func_02000884(11, &toss->shadow.x, 0, 0, 16);
    Func_02000884(10, &toss->trail[1].x, 0, 0, 16);
    Func_02000884(9, &toss->trail[2].x, 0, 0, 16);
    Func_02000884(8, &toss->trail[3].x, 0, 0, 16);
    Func_020012e8(Actor_GetById(10), 4);
    Func_020012e8(Actor_GetById(9), 4);
    Func_020012e8(Actor_GetById(8), 4);
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

// ---- games/tla/src/scene/lemuria_spring/ride.c ----
#include "task.h"

struct SpringPoint {
  s32 x;
  s32 y;
  s32 z;
};


struct SpringRide {
  u16 unk00;
  u16 hold;
  struct SpringPoint trail[5];
  struct SpringPoint velocity;
  s32 frames;
};

/* VIEW(ride): variable gData_02001760 as extern struct SpringRide gData_02001760; lemuria_spring has extern struct SpringTossMotion gData_02001760; tried as the one declaration extern struct SpringRide gData_02001760, it does not compile in tla-ja: games/tla/src/scene/lemuria_spring.c:1014: conflicting types for `gData_02001760'. */
extern struct SpringRide gData_02001760__ride __asm__("gData_02001760");
extern s32 gData_020017b0;
/* VIEW(ride): variable gData_020017c0 as extern struct SpringObstacle__ride gData_020017c0[]; lemuria_spring has extern struct SpringObstacle gData_020017c0[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/lemuria_spring.c:1042: `gData_020017c0__ride' undeclared (first use in this function). */
extern struct SpringObstacle gData_020017c0__ride[] __asm__("gData_020017c0");
extern s32 gData_02001820;
extern s32 gData_02001824;
extern s32 gData_02001828;
extern u8 gData_0200174c[];
extern u8 gData_02001750[];
extern u16 gData_02001754[];

void Func_020008f0(s32 actor, s32 active);
void Func_02000904(void);
void Func_020012e8(struct Actor *object, s32 mode);

/// Places the four actors from their tables, resets the ride and installs
/// its task.
void Func_020010f4(void) {
  struct SpringRide *ride = &gData_02001760__ride;
  s32 i = 0;
  u8 *xtbl;
  u16 *htbl;
  struct SpringObstacle *rec;
  u8 *ztbl;
  s32 zero;

  htbl = gData_02001754;
  ztbl = gData_02001750;
  xtbl = gData_0200174c;
  rec = gData_020017c0__ride;

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

  Func_020012e8(Actor_Get(20), 2);
  Func_020012e8(Actor_Get(21), 2);

  Task_AddFrame((TaskFunc)Func_02000904, TASK_PASS_VBLANK + 3);
}

/// Runs the spring ride from one side until it ends, and returns its result.
s32 Func_02001184(s32 side) {
  struct SpringRide *ride;

  ride = &gData_02001760__ride;
  ride->trail[0].y = 0;
  ride->trail[1].y = 0;
  ride->trail[2].y = 0;
  ride->trail[3].y = 0;
  gData_020017b0 = side;
  gData_02001824 = 0;
  ride->hold = 0xffff;
  for (gData_02001820 = 0;; gData_02001820++) {
  if (gData_02001820 == 50)
    Event_PlaySound(300);
  if (gData_02001820 == 16) {
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 39);
    ride->hold = 0;
    ride->velocity.x = 0x14ccc;
    ride->velocity.y = 0x40000;
    ride->velocity.z = -0x20000;
    ride->trail[0].x = 0x780000;
    ride->trail[0].y = 0x100000;
    ride->trail[0].z = 0x980000;
    ride->frames = 300;
    if (gData_020017b0 == 1) {
      Func_020012e8(Actor_Get(16), 3);
      Func_020012e8(Actor_Get(17), 0);
      Func_020008f0(15, 1);
      Func_020008f0(14, 1);
      Func_020008f0(13, 1);
    } else {
      Func_020012e8(Actor_Get(11), 3);
      Func_020012e8(Actor_Get(12), 0);
      Func_020008f0(10, 1);
      Func_020008f0(9, 1);
      Func_020008f0(8, 1);
    }
  }
  System_Wait(1);
  if (gData_02001824 == 1)
    break;
  }
  return gData_02001828;
}


void Func_02000054(void);
void Func_020000d0(void);

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
u32 gData_02001678[] = {
    0xa08090a, 0xc040b06, 0xe020d03, 0x10000f01,
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0, 22, CONDITION_ALWAYS, MSGNO(MsgReadTheSign)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgReadTheSignThe)},
    {0, 26, CONDITION_ALWAYS, MSGNO(MsgIfYouFace)},
    {0x8d15, 26, CONDITION_ALWAYS, MSGNO(MsgIDLove)},
    {0, 28, CONDITION_ALWAYS, MSGNO(MsgIRememberAn)},
    {0x8d15, 28, CONDITION_ALWAYS, MSGNO(MsgHeHadSuch)},
    {3, 25, 0x200, EVENT(Func_02000054)},
    {3, 80, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {TRIGGERS_END},
};

u32 gData_02001700[] = {
    280, 359, 335, 380, 302, 342,
    387, 372, 320, 395, 364, 341,
    183, 186, 189,
};

u16 gData_0200173c[] = {
    20, 10, 2, 1, 0,
};

u8 gData_02001746[] = {
    0, 1, 9,
};

u8 gData_02001749[] = {
    0, 3, 4,
};

u8 gData_0200174c[] = {
    80, 160, 80, 72,
};

u8 gData_02001750[] = {
    32, 104, 68, 72,
};

u16 gData_02001754[] = {
    0, 1, 0, 0x8000,
};
// clang-format on
