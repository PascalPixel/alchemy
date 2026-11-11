/// @file
/// Overlay 0x6ba's scene: its code up to Func_02000520, then its tables, in ROM order.

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
#include "input.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "main_routine_work.h"

#include "inventory.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void *Func_02000d00(s32, s32, s32, s32);
void Func_02000d08(s32, s32);
void Message_CloseFinishedWindows(void);
void Message_OpenWindow(s32, s32, s32, s32);
s32 Message_AllTyped(void);

#define INPUT_HELD (gInput.held)

void Func_02000054(s32 message, s32 count) {
  s32 i;
  s32 frames;
  void *window;
  s32 typed;
  volatile struct InputState *input;

  /* FAKEMATCH(volatile): the input read through a volatile pointer; as a plain
   * pointer, 31 of 94 instructions differ in tla-en. */
  window = Func_02000d00(206, 0, 0, 0);
  i = 0;
  if (i >= count)
    goto done;
loop:
  Func_02000d08(1, 1);
  Func_02000d08(5, 2);
  Func_02000d08(0x1e240, 5);
  Message_CloseFinishedWindows();
  Message_OpenWindow(message, 5, 0, 0x22);
  frames = 0;
  if (INPUT_HELD & 3)
    goto shown;
wait:
  wait(1);
  frames++;
  if (frames > 59)
    goto shown;
  if (!(INPUT_HELD & 3))
    goto wait;
  for (;;) {
  shown:
    typed = Message_AllTyped();
    input = (volatile struct InputState *)&gInput;
    if (typed != 0)
      break;
    input->held = 1;
    input->pressed = 1;
    input->repeat = 1;
    wait(1);
  }
  frames = 0;
  if (input->held & 2)
    goto done;
  if (!(input->held & 1))
    goto hold;
  goto next;
hold:
  wait(1);
  frames++;
  if (frames > 9)
    goto next;
  input = (volatile struct InputState *)&gInput;
  if (input->held & 2)
    goto done;
  if (!(input->held & 1))
    goto hold;
next:
  message++;
  i++;
  if (i < count)
    goto loop;
done:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}

void Func_02000e40(s32 kind, s32 value);
void Func_02000e48(void);
void Func_02000e50(s32 kind, s32 value);
void Func_02000e60(s32 *a, s32 *b);
void Func_02000d20(s32 index);
void Func_02000d38(s32 value);
void Func_02000dd0(s32 value);
void Func_02000d28(void);
void Func_02000d30(void);
void Func_02000054(s32 message, s32 count);

void Func_0200012c(void) {
  Func_02000054((s32)MsgWeaponShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000144(void) {
  Func_02000054((s32)MsgArmorShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_0200015c(void) {
  Func_02000054((s32)MsgItemShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000178(void) {
  Func_02000054((s32)MsgWelcomeWearyWanderers,
                MsgRightWhenI - MsgWelcomeWearyWanderers);
}

void Func_02000190(s32 value) { Func_02000e40(1, value); }

void Func_0200019c(s32 value) { Func_02000e40(2, value); }

void Func_020001a8(s32 value) { Func_02000e40(3, value); }

void Func_020001b4(s32 value) { Func_02000e40(24, value); }

void Func_020001c0(s32 value) { Func_02000e50(0, value); }

void Func_020001cc(void) { Func_02000e48(); }

void Party_AdjustCoins(s32 amount);
void Func_02000d08(s32 value, s32 slot);
u16 *Func_02000d48(s32 item);
s32 Func_02000dc8(s32 item);
void Func_02000e28(s32 item, s32 mode);
void Func_02000e58(s32 actor);
void Func_02000e68(s32 value);
s32 Func_02000e70(void);

void Func_020001d4(s32 actor) {
  s32 message;

  if (!Event_GetFlag(0x10f)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else if (Event_GetFlag(0x200)) {
    Event_SetMessageId((s32)MsgICanTWait);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Func_02000e70()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
      Func_02000e58(actor);
      if (Func_02000e70())
        Event_SetFlag(0x200);
    } else {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

void Func_020002a8(s32 actor) {
  if (!Event_GetFlag(0x10f)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Event_GetFlag(0x200)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Func_02000e70()) {
    s32 item = Func_02000e70();
    u32 price = *Func_02000d48(item);
    s32 message;

    Func_02000d08(item, 2);
    Func_02000d08(price, 5);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else if (Func_02000dc8(item) < 0) {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      Event_ShowMessageAndWait(actor, 0);
      Func_02000e28(item, 3);
      PartyInventory_GiveItem(item, 0);
      Func_02000e68(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000394(void) {
  s32 pos[2];

  Func_02000e60(&pos[1], &pos[0]);
}

void Func_020003a4(void) {
  s32 i;

  for (i = 0; i < 8; i++)
    Func_02000d20(i);
}

void Party_RemoveActiveOwner(s32 owner);

static __inline__ void Inventory_AddItemAgain(s32 owner, s32 item) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline where the next
   * call adds the same item, so the item is built again, as the ROM has it;
   * where a direct call would keep it in r5; as direct Inventory_AddItem
   * calls, 24 of 134 instructions differ in tla-en. */
  Inventory_AddItem(owner, item);
}

/// Coins and equipment for owners 4 to 7, then owners 0 to 3 leave the party.
void Func_020003b8(void) {
  s32 i;

  FieldMessage_Show((s32)MsgYouGotTreasure, 1);
  gMainRoutineWork.coins = 777777;
  for (i = 9; i >= 0; i--) {
    Inventory_AddItem(4, 228);
    Inventory_AddItem(4, 229);
  }
  Inventory_AddItem(4, 184);
  Inventory_AddItem(4, 204);
  Inventory_AddItem(4, 224);
  Inventory_AddItem(4, 11);
  Inventory_AddItem(4, 267);
  Inventory_AddItem(4, 223);
  Inventory_AddItem(5, 226);
  Inventory_AddItem(5, 227);
  Inventory_AddItem(5, 230);
  Inventory_AddItem(5, 232);
  Inventory_AddItem(5, 231);
  Inventory_AddItem(5, 237);
  Inventory_AddItem(5, 265);
  Inventory_AddItem(6, 242);
  Inventory_AddItem(6, 258);
  Inventory_AddItem(6, 252);
  Inventory_AddItemAgain(6, 429);
  Inventory_AddItemAgain(6, 429);
  Inventory_AddItem(6, 429);
  Inventory_AddItem(6, 433);
  Inventory_AddItem(6, 435);
  Inventory_AddItemAgain(6, 417);
  Inventory_AddItem(6, 417);
  Inventory_AddItem(6, 418);
  Inventory_AddItem(6, 419);
  Inventory_AddItem(7, 189);
  Inventory_AddItem(7, 200);
  Inventory_AddItem(7, 201);
  Inventory_AddItem(7, 202);
  Inventory_AddItem(7, 203);
  Inventory_AddItem(7, 204);
  Inventory_AddItem(7, 207);
  Party_RemoveActiveOwner(0);
  Party_RemoveActiveOwner(1);
  Party_RemoveActiveOwner(2);
  Party_RemoveActiveOwner(3);
}

void Func_0200012c(void);
void Func_02000144(void);
void Func_0200015c(void);
void Func_02000178(void);
void Func_020001cc(void);
void Func_02000394(void);
void Func_020003a4(void);
void Func_020003b8(void);
void Func_02000520(void);
void Func_02000638(void);
void Func_02000668(void);
void Func_02000748(void);
void Func_02000848(void);
void Func_02000aac(void);
void Func_02000ab4(void);

void Func_02000664(void);

u32 gData_02000e80[] = {
    0x764c,
};

u32 gData_02000e84[] = {
    0x6d657449,
    0x3a6f4e20,
    0,
};

// clang-format off
u32 gData_02000e90[] = {
    0x65473a41, 0x74492074, 0x20206d65, 0x65523a42, 0x6e727574, 0,
};

u32 gData_02000ea8[] = {
    0x4d455449, 0x4c554620, 0x2e2e2e4c, 0x2e2e2e2e, 46,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {1, CONDITION_ALWAYS, 616, 0, 328, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {2, CONDITION_ALWAYS, 648, 0, 152, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {3, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {4, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {5, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(322),
    SCENE_EXIT(20, 322, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {209, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {205, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {206, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {70, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {215, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {200, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {450, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(200), FACING_SOUTH, 0, 0},
    {321, CONDITION_ALWAYS, 1, PIXELS(640), 0, PIXELS(96), FACING_SOUTH, 0, 0},
    {375, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(96), FACING_SOUTH, 0, 0},
    {296, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {238, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {487, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {243, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {25, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(280), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(200), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(360), FACING_SOUTH, 0, 0},
    {488, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(160), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000190)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_0200019c)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020001a8)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_020001b4)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_020001c0)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_020001cc)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_020001d4)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_020002a8)},
    {0, 17, CONDITION_ALWAYS, EVENT(Func_0200012c)},
    {0, 18, CONDITION_ALWAYS, EVENT(Func_02000144)},
    {0, 19, CONDITION_ALWAYS, EVENT(Func_0200015c)},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_02000178)},
    {0, 20, CONDITION_ALWAYS, EVENT(Func_02000394)},
    {0, 21, CONDITION_ALWAYS, EVENT(Func_020003a4)},
    {0, 22, CONDITION_ALWAYS, EVENT(Func_02000668)},
    {0, 23, CONDITION_ALWAYS, EVENT(Func_02000aac)},
    {0, 24, CONDITION_ALWAYS, EVENT(Func_02000ab4)},
    {0, 25, CONDITION_ALWAYS, EVENT(Func_02000748)},
    {0, 26, CONDITION_ALWAYS, EVENT(Func_02000848)},
    {0, 29, CONDITION_ALWAYS, EVENT(Func_02000520)},
    {0, 27, CONDITION_ALWAYS, EVENT(Func_020003b8)},
    {0, 28, CONDITION_ALWAYS, EVENT(Func_02000638)},
    {0, 30, CONDITION_ALWAYS, (u32)(Func_02000664)},
    {0, 31, CONDITION_ALWAYS, (u32)(Func_02000664)},
    {0, 32, CONDITION_ALWAYS, (u32)(Func_02000664)},
    {0, 33, CONDITION_ALWAYS, (u32)(Func_02000664)},
    {TRIGGERS_END},
};
