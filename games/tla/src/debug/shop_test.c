/// @file
/// Overlay 0x6ba's scene: its code up to Func_02000520, then its tables, in ROM
/// order.

#include "flags.h"
#include "battle/battle.h"
#include "system/io_reg.h"
#include "sounds.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"

#include "inventory.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void *Portrait_OpenWindow(s32, s32, s32, s32);
void MessageArgument_Push(s32, s32);
void Message_CloseFinishedWindows(void);
void Message_OpenWindow(s32, s32, s32, s32);
s32 Message_AllTyped(void);

void Test_ReceiveFlint(s32 message, s32 count) {
  s32 i;
  s32 frames;
  void *window;
  s32 typed;

  window = Portrait_OpenWindow(206, 0, 0, 0);
  i = 0;
  if (i >= count)
    goto done;
loop:
  MessageArgument_Push(1, 1);
  MessageArgument_Push(5, 2);
  MessageArgument_Push(0x1e240, 5);
  Message_CloseFinishedWindows();
  Message_OpenWindow(message, 5, 0, 0x22);
  frames = 0;
  if (gInput.held & 3)
    goto shown;
  do {
    wait(1);
    frames++;
    if (frames > 59)
      goto shown;
  } while (!(gInput.held & 3));
  for (;;) {
  shown:
    typed = Message_AllTyped();
    if (typed != 0)
      break;
    gInput.held = 1;
    gInput.pressed = 1;
    gInput.repeat = 1;
    wait(1);
  }
  frames = 0;
  if (gInput.held & KEY_B)
    goto done;
  if (!(gInput.held & KEY_A))
    goto hold;
  goto next;
hold:
  wait(1);
  frames++;
  if (frames > 9)
    goto next;
  if (gInput.held & KEY_B)
    goto done;
  if (!(gInput.held & KEY_A))
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

void Shop_Run(s32 kind, s32 value);
void Sanctum_Run(void);
void Inn_CheckIn(s32 kind, s32 value);
void ItemPicker_RunMode1(s32 *a, s32 *b);
void NameEntry_EditMemberName(s32 index);
void Summon_ShowLearnedMessage(s32 value);
void Class_GetSeries(s32 value);
void DebugBrowser_Icons(void);
void DebugBrowser_Portraits(void);
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

void Func_02000190(s32 value) { Shop_Run(1, value); }

void Func_0200019c(s32 value) { Shop_Run(2, value); }

void Func_020001a8(s32 value) { Shop_Run(3, value); }

void Func_020001b4(s32 value) { Shop_Run(24, value); }

void Func_020001c0(s32 value) { Inn_CheckIn(0, value); }

void Func_020001cc(void) { Sanctum_Run(); }

void Party_AdjustCoins(s32 amount);
void MessageArgument_Push(s32 value, s32 slot);
u16 *ShopTest_GetItem(s32 item);
s32 PartyInventory_FindRoom(s32 item);
void ItemDisplay_ShowItem(s32 item, s32 mode);
void Forge_Run(s32 actor);
void Forge_SetItem(s32 value);
s32 Forge_GetItem(void);

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
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    Event_SetMessageId((s32)MsgICanTWait);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Forge_GetItem()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
      Forge_Run(actor);
      if (Forge_GetItem())
        Event_SetFlag(FLAG_SCENE_TEMP_00);
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
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Forge_GetItem()) {
    s32 item = Forge_GetItem();
    u32 price = *ShopTest_GetItem(item);
    s32 message;

    MessageArgument_Push(item, 2);
    MessageArgument_Push(price, 5);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else if (PartyInventory_FindRoom(item) < 0) {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      Event_ShowMessageAndWait(actor, 0);
      ItemDisplay_ShowItem(item, 3);
      PartyInventory_GiveItem(item, 0);
      Forge_SetItem(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000394(void) {
  s32 pos[2];

  ItemPicker_RunMode1(&pos[1], &pos[0]);
}

void Func_020003a4(void) {
  s32 i;

  for (i = 0; i < 8; i++)
    NameEntry_EditMemberName(i);
}

void Party_RemoveMember(s32 owner);

static __inline__ void Inventory_AddItemAgain(s32 owner, s32 item) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline where the next
   * call adds the same item. */
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
    SCENE_EXITS(PsynergyTest),
    SCENE_EXIT(20, PsynergyTest, 2),
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
    {TRIGGER_TABLE_END},
};

// Overlay 0x6ba's code from Func_02000520 on, in ROM order.

#include "owner_state.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "actor.h"

#include "fixed_math.h"

void Unit_AddPp(s32 owner, s32 amount);
void Inventory_Break(s32 owner, s32 slot);
/* VIEW(s0520): routine ShopTest_GetItem as struct ItemDefinition
 * *ShopTest_GetItem(s32 item); shop_test has u16 *ShopTest_GetItem(s32 item); tried
 * as the definition's void ShopTest_GetItem(struct Actor *shot), it does not
 * compile in tla-ja: games/tla/src/debug/shop_test.c:427: conflicting types for
 * `ShopTest_GetItem'. */
extern struct ItemDefinition *
Func_02000d48__s0520(s32 item) __asm__("ShopTest_GetItem");
void GameFlag_RefreshLureCap(void);

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
  GameFlag_RefreshLureCap();
}

void Func_020005fc(s32 owner, s32 element, s32 djinn, s32 activate) {
  GameFlag_Set(element * 20 + djinn + 48);
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
  GameFlag_RefreshLureCap();
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
  GameFlag_RefreshLureCap();
}
#endif

void Func_02000664(void) {}

void Shop_Run(s32 kind, s32 value);
void Sanctum_Run(void);
void Inn_CheckIn(s32 kind, s32 value);
void ItemPicker_RunMode1(s32 *a, s32 *b);
void NameEntry_EditMemberName(s32 index);
void Summon_ShowLearnedMessage(s32 value);
void Class_GetSeries(s32 value);
void DebugBrowser_Icons(void);
void DebugBrowser_Portraits(void);

void Func_02000668(void) {
  PartyInventory_GiveItem(11, 0);
  Summon_ShowLearnedMessage(22);
  Class_GetSeries(22);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Party_AddIsaacsParty(void);

/// The scene's first entry: sets screen effect 256 and event mode 2, puts
/// objects 16 to 19 in mode 5 and 30 to 33 in mode 6, and runs
/// Party_AddIsaacsParty when the party came in by entrance 99.
s32 Func_02000688(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 *effect = &work->transition;
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
    Party_AddIsaacsParty();
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
    if ((gInput.pressed & KEY_START) || (gInput.pressed & KEY_SELECT)) {
      Func_02000704(5);
      Sound_Play(MUS_093);
      redraw = 1;
    }
    if (gInput.pressed & KEY_A) {
      Func_02000704(1);
      Sound_Play(MUS_091);
      redraw = 1;
    }
    if (gInput.pressed & KEY_B)
      break;
  }
  Sound_Play(SE_MENU_CANCEL);
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
  Dma_Set((const void *)OBJ_PLTT, (void *)(BG_PLTT + 14 * 16), 0x80000010,
          REG_DMA3);
  Dma_Set((const void *)(BG_PLTT + 244), (void *)(BG_PLTT + 238), 0x80000001,
          REG_DMA3);
  Sound_Play(SE_MENU_CONFIRM);
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
    if (gInput.pressed & KEY_A) {
      if (PartyInventory_Add(item) == -1) {
        Sound_Play(SE_MENU_CANCEL);
        break;
      }
      Sound_Play(SE_NOTE_HIGH);
    }
    if (gInput.pressed & KEY_B) {
      Sound_Play(SE_MENU_CANCEL);
      break;
    }
    if (gInput.repeat & KEY_DOWN) {
      step = 1;
      item++;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_UP) {
      step = -1;
      item--;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_RIGHT) {
      step = 1;
      item += 10;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_LEFT) {
      step = -1;
      item -= 10;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_R) {
      step = 1;
      item += 30;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
    }
    if (gInput.repeat & KEY_L) {
      step = -1;
      item -= 30;
      redraw = 1;
      Sound_Play(SE_MENU_CURSOR_MOVE);
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

void Func_02000aac(void) { DebugBrowser_Icons(); }

void Func_02000ab4(void) { DebugBrowser_Portraits(); }

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
    object->y = px(32);
    object->motionFlags = 3;
    object->gravity = 0x170a;
    wait(50);
  }
}

void Field_RequestBattleFromEncounterTable(s32 scene, s32 entrance);

/// Mode 1 places object kind 354 at (x, z) in pixels; mode 2 grows object
/// kind 252 there from 0x6666 to full size while it turns, then turns it
/// opposite the leader's facing and passes scene and entrance on to
/// Field_RequestBattleFromEncounterTable.
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
      object->velocityY = px(6);
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word. */
          s32 angle = object->facing + TRIG_TURN / 16;

          object->facing = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->facing =
          Actor_GetById(gMainRoutineWork.currentOwner)->facing + TRIG_TURN / 2;
      Field_RequestBattleFromEncounterTable(scene, entrance);
    }
  }
}
#undef ITEM_COUNT
