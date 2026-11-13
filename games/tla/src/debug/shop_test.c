/// @file
/// Overlay 0x6ba's scene: its code up to Func_02000520, then its tables, in ROM
/// order.

// ---- games/tla/src/debug/shop_test.c ----

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

void Test_ReceiveFlint(s32 message, s32 count) {
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
void Test_ReceiveFlint(s32 message, s32 count);

void Func_0200012c(void) {
  Test_ReceiveFlint((s32)MsgWeaponShopWelcome,
                    MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void MenuTest_BrowseWeaponShopMessages(void) {
  Test_ReceiveFlint((s32)MsgArmorShopWelcome,
                    MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_0200015c(void) {
  Test_ReceiveFlint((s32)MsgItemShopWelcome,
                    MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000178(void) {
  Test_ReceiveFlint((s32)MsgWelcomeWearyWanderers,
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

void Party_RemoveMember(s32 owner);

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
  Party_RemoveMember(0);
  Party_RemoveMember(1);
  Party_RemoveMember(2);
  Party_RemoveMember(3);
}

void Func_0200012c(void);
void MenuTest_BrowseWeaponShopMessages(void);
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
    {0, 18, CONDITION_ALWAYS, EVENT(MenuTest_BrowseWeaponShopMessages)},
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
#undef INPUT_HELD

// ---- games/tla/src/debug/shop_test/s0520.c ----
// Overlay 0x6ba's code from Func_02000520 on, in ROM order.

#include "owner_state.h"
#include "version.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "dma.h"
#include "io_reg.h"
#include "actor.h"

#include "fixed_math.h"

void Unit_AddPp(s32 owner, s32 amount);
void Inventory_Break(s32 owner, s32 slot);
/* VIEW(s0520): routine Func_02000d48 as struct ItemDefinition
 * *Func_02000d48(s32 item); shop_test has u16 *Func_02000d48(s32 item); tried
 * as the definition's void Func_02000d48(struct Actor *shot), it does not
 * compile in tla-ja: games/tla/src/debug/shop_test.c:438: conflicting types for
 * `Func_02000d48'. */
extern struct ItemDefinition *
Func_02000d48__s0520(s32 item) __asm__("Func_02000d48");
void Func_02000dc0(void);

void Func_02000520(void) {
  u8 *unit;
  u8 *item;
  s32 i;

  FieldMessage_Show(MsgTheBodyIs, 1);
  FieldUnit_AddHp(4, -100);
  FieldUnit_AddHp(5, -100);
  FieldUnit_AddHp(6, -33);
  Unit_AddPp(4, -50);
  Unit_AddPp(5, -40);
  Unit_AddPp(6, -35);
  unit = Unit_Get(4);
  unit[0x131] = 1;
  unit[0x140] = 1;
  unit = Unit_Get(5);
  unit[0x130] = 1;
  unit[0x131] = 2;
  unit = Unit_Get(4);
  for (i = 0; i <= 14; i++) {
    item = Func_02000d48__s0520(
        ((struct OwnerInventoryState *)unit)->inventory[i]);
    if (((struct OwnerInventoryState *)unit)->inventory[i] == 0)
      break;
    if (item[12] == 2) {
      Inventory_Break(4, i);
      break;
    }
  }
  unit = Unit_Get(5);
  for (i = 0; i <= 14; i++) {
    item = Func_02000d48__s0520(
        ((struct OwnerInventoryState *)unit)->inventory[i]);
    if (((struct OwnerInventoryState *)unit)->inventory[i] == 0)
      break;
    if (item[12] == 2) {
      Inventory_Break(5, i);
      break;
    }
  }
  Func_02000dc0();
}

void Func_020005fc(s32 owner, s32 element, s32 djinn, s32 activate) {
  GameFlag_SetBit(element * 20 + djinn + 48);
  Djinn_Give(owner, element, djinn);
  if (activate == 1)
    Djinn_Set(owner, element, djinn);
}

#if !TLA_PATCH_1
/// Gives each of four characters a set of Djinn from 7, until patch 1.
void Func_02000638(void) {
  s32 djinn;

  FieldMessage_Show((s32)MsgYouGotA, 1);
  for (djinn = 7; djinn < 13; djinn++)
    Func_020005fc(4, 0, djinn, 1);
  for (djinn = 7; djinn < 14; djinn++)
    Func_020005fc(5, 2, djinn, 1);
  for (djinn = 7; djinn < 13; djinn++)
    Func_020005fc(6, 3, djinn, 1);
  for (djinn = 7; djinn < 14; djinn++)
    Func_020005fc(7, 1, djinn, 1);
  Func_02000dc0();
}
#else
void Func_02000638(void) {
  s32 element;
  s32 djinn;

  FieldMessage_Show((s32)MsgYouGotA, 1);
  for (element = 0; element < 4; element++) {
    for (djinn = 0; djinn < 18; djinn++)
      Djinn_GiveToParty(element, djinn);
  }
  Func_02000dc0();
}
#endif

void Func_02000664(void) {}

void Func_02000e40(s32 kind, s32 value);
void Func_02000e48(void);
void Func_02000e50(s32 kind, s32 value);
void Func_02000e60(s32 *a, s32 *b);
void Func_02000d20(s32 index);
void Func_02000d38(s32 value);
void Func_02000dd0(s32 value);
void Func_02000d28(void);
void Func_02000d30(void);

void Func_02000668(void) {
  PartyInventory_GiveItem(11, 0);
  Func_02000d38(22);
  Func_02000dd0(22);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_02000db0(void);

/// The scene's first entry: sets screen effect 256 and event mode 2, puts
/// objects 16 to 19 in mode 5 and 30 to 33 in mode 6, and runs
/// Func_02000db0 when the party came in by entrance 99.
s32 Func_02000688(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 *effect = &work->screenEffect;
  struct MainRoutineWork *party = &gMainRoutineWork;

  *effect = 256;
  party->eventMode = 2;
  Actor_SetModeById(16, 5);
  Actor_SetModeById(17, 5);
  Actor_SetModeById(18, 5);
  Actor_SetModeById(19, 5);
  Actor_SetModeById(30, 6);
  Actor_SetModeById(31, 6);
  Actor_SetModeById(32, 6);
  Actor_SetModeById(33, 6);
  if (party->entrance == 99)
    Func_02000db0();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Unit_Calculate(s32 owner);

void Func_02000704(s32 levels) {
  s16 owners[16];
  s32 count = FieldParty_ListMembers(owners);
  s32 i;

  for (i = 0; i < count;) {
    u16 owner = owners[i++];

    Unit_LevelUpTo(owner,
                   ((struct BattleUnit *)Unit_Get(owner))->level + levels);
    Unit_Calculate(owner);
  }
}

extern u32 gData_02000e80[];

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffset(const void *text, void *window, s32 x, s32 y);
void Text_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 a, void *window, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(void *window);
void Window_Close(void *window, s32 mode);

/// The level-raising window: shows the current member's name and level,
/// raises every active member 5 levels on Start or Select and 1 on A, and
/// closes on B.
// ☀️'s debug EV11_TALK.
void Func_02000748(void) {
  u8 *unit = Unit_Get(gMainRoutineWork.currentOwner);
  void *window = Window_Create(0, 0, 30, 9, 2);
  const u8 *message = MsgRaiseEveryoneS;
  s32 redraw;

  Text_DrawMessage(message, window, 0, 0);
  Text_DrawMessage(message + 1, window, 0, 16);
  redraw = 1;
  Text_DrawMessage(message + 2, window, 0, 32);
  for (;;) {
    if (redraw) {
      WindowSprite_RedrawSavedRect(window);
      Text_DrawStringAtOffset(unit, window, 0, 48);
      Text_DrawStringInWindow(gData_02000e80, window, 48, 48);
      redraw = 0;
      Text_DrawNumberInWindow(unit[15], 0, window, 72, 48);
    }
    wait(1);
    if ((gInput.pressed & 8) || (gInput.pressed & 4)) {
      Func_02000704(5);
      Sound_Play(93);
      redraw = 1;
    }
    if (gInput.pressed & 1) {
      Func_02000704(1);
      Sound_Play(91);
      redraw = 1;
    }
    if (gInput.pressed & 2)
      break;
  }
  Sound_Play(113);
  Window_Close(window, 2);
}

extern u32 gData_02000e84[];
extern u32 gData_02000e90[];
extern u32 gData_02000ea8[];

struct ItemSummary {
  u8 unk00[6];
  u16 icon;
};

void WindowSprite_PrepareForRedraw(void *window);
void Text_DrawNumberAtOffset(s32 value, s32 digits, void *window, s32 x, s32 y);
void Text_DrawMessageAsTiles(s32 message, void *window, s32 x, s32 y);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);

#define ITEM_COUNT 500

void Func_02000848(void) {
  void *window;
  void *details;
  s32 item;
  s32 redraw;
  s32 entryCount;
  s8 step;

  entryCount = ITEM_COUNT;
  step = 0;
  item = 1;
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, 2);
  details = Window_Create(0, 8, 13, 10, 2);
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
  Sound_Play(112);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      WindowSprite_PrepareForRedraw(window);
      WindowSprite_PrepareForRedraw(details);
      Text_DrawStringInWindow(gData_02000e84, window, 0, 0);
      Text_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gData_02000e90, window, 0, 32);
        Text_DrawMessageAsTiles((s32)(item + MsgItemName), window, 120, 0);
        Text_DrawMessage((s32)(item + MsgItemPlainName), window, 0, 16);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        Text_DrawStringInWindow(gData_02000ea8, window, 0, 32);
      }
    }
    wait(1);
    if (gInput.pressed & 1) {
      if (PartyInventory_Add(item) == -1) {
        Sound_Play(113);
        break;
      }
      Sound_Play(175);
    }
    if (gInput.pressed & 2) {
      Sound_Play(113);
      break;
    }
    if (gInput.repeat & 0x80) {
      step = 1;
      item++;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x40) {
      step = -1;
      item--;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x10) {
      step = 1;
      item += 10;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x20) {
      step = -1;
      item -= 10;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x100) {
      step = 1;
      item += 30;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 0x200) {
      step = -1;
      item -= 30;
      redraw = 1;
      Sound_Play(111);
    }
    if (step == -1) {
      for (item = (item + ITEM_COUNT) % entryCount;
           Func_02000d48__s0520(item & 0x1ff)->icon == 0;
           item = (item + ITEM_COUNT - 1) % entryCount)
        ;
    }
    if (step == 1) {
      for (item = (item + ITEM_COUNT) % entryCount;
           Func_02000d48__s0520(item & 0x1ff)->icon == 0;
           item = (item + ITEM_COUNT + 1) % entryCount)
        ;
    }
    step = 0;
  }
  Window_Close(window, 2);
  Window_Close(details, 2);
}

void Func_02000aac(void) { Func_02000d28(); }

void Func_02000ab4(void) { Func_02000d30(); }

void Func_02000abc(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    Map_CopyCellsRect(a, b, c, d, e, f);
}

void Func_02000ae0(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_GetById(id);
  if (mode == 1) {
    Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    while (object->scaleX < Q16_ONE) {
      wait(1);
      scale = object->scaleX;
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
}

void Func_02000b50(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_GetById(id);
  if (mode == 2) {
    Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    wait(50);
  }
}

void Func_02000e20(s32 scene, s32 entrance);

/// Mode 1 places object kind 354 at (x, z) in pixels; mode 2 grows object
/// kind 252 there from 0x6666 to full size while it turns, then turns it
/// opposite the leader's facing and passes scene and entrance on to
/// Func_02000e20.
void Func_02000ba0(s32 mode, s32 x, s32 z, s32 scene, s32 entrance) {
  struct Actor *object;

  if (mode == 1) {
    object = FieldActor_Create(354, x << 16, 0, z << 16);
    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = ((struct Actor *)Import_ActorCreate(252, x << 16, 0, z << 16));
    if (object != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(object, 0);
      object->velocityY = 0x60000;
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word, so its step
           * is built inside the loop as in the native; as
           * object->angle += 0x1000, 22 of 86 instructions differ in
           * tla-en. */
          s32 angle = object->facing + 0x1000;

          object->facing = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->facing =
          Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x8000;
      Func_02000e20(scene, entrance);
    }
  }
}
#undef ITEM_COUNT
