/// @file
/// The sanctum's services, their prices and the effects that go with them.
// Part of shop.c.

#include "../shop_internal.h"
#include "gamepak/far_call/field_sequence.h"
#include "gamepak/far_call/window.h"
#include "module/shop/shop.h"
#include "gamepak/far_call/menu.h"
#include "module/system/system.h"
#include "module/tla.h"

s32 Sanctum_ServicePrice(s32 entry_id, s32 kind) {
  u8 value = ((struct BattleUnit *)Unit_Get(entry_id))->level;
  s32 result = 0;

  if (kind == 0) {
    result = value * 20;
  } else if (kind == 1) {
    result = 10;
  } else if (kind == 2) {
    result = 50;
  } else if (kind == 3) {
    result = value * 10;
  }
  return result;
}

s32 Sanctum_NeedsService(s32 entry_id, s32 kind) {
  struct BattleUnit *entry = Unit_Get(entry_id);
  s32 result = 0;

  if ((kind == 0 && entry->hp <= 0) || (kind == 1 && entry->poison != 0) ||
      (kind == 2 && entry->haunt != 0) ||
      (kind == 3 && entry->restraint != 0)) {
    result = 1;
  }
  return result;
}

/// Counts the party members who need the selected sanctum service.
// As ☀️'s Sanctum_CountMembersInNeed.
s32 Sanctum_CountMembersInNeed(void) {
  struct ShopRuntime *shop;
  s32 eligibleCount;
  s32 serviceKind;
  s32 memberIndex;

  shop = Ram_HeapSlots->menuRuntime;
  eligibleCount = 0;
  serviceKind = shop->partyAction;
  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Sanctum_NeedsService(shop->partyMemberIds[memberIndex], serviceKind) !=
        0)
      eligibleCount++;
  }
  return eligibleCount;
}

s32 Sanctum_GetServiceMessage(s32 value) {
  s8 *shop;
  s8 mode;

  shop = Ram_HeapSlots->menuRuntime;
  mode = shop[0x507];
  if (mode == 1) {
    // MsgReviveService: "Hmm, you were downed ...", then "Fear not ..."
    value += (u32)MsgCurePoisonService - (u32)MsgReviveService;
  }
  if (mode == 2) {
    value += (u32)MsgExorcismService - (u32)MsgReviveService;
  }
  if (mode == 3) {
    value += (u32)MsgRemoveCurseService - (u32)MsgReviveService;
  }
  return value;
}

/// Shows the service message, its wording for the shop's mode, with the
/// keeper's portrait, and waits until it has been read.
// The Broken Seal's Sanctum_ShowKeeperMessage.
void Sanctum_ShowKeeperMessage(s32 message) {
  u8 *shop;
  s32 portrait;

  shop = Ram_HeapSlots->menuRuntime;
  portrait = BattleEffect_GetResourceIdFar(*(u16 *)(shop + 0x4fa));
  Message_CloseFinishedWindowsFar();
  message = Sanctum_GetServiceMessage(message);
  Message_OpenWindowFar(message, 5, 0, (portrait << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
}

/// As Sanctum_ShowKeeperMessage, with the cursor's anchor object set to state
/// 13 while the message shows and restored after.
// The Broken Seal's Sanctum_ShowKeeperMessageHidingCursor.
void Sanctum_ShowKeeperMessageHidingCursor(s32 message) {
  struct ShopRuntime *shop;
  u8 **anchor;
  s32 portrait;
  u8 saved;

  shop = Ram_HeapSlots->menuRuntime;
  anchor = &shop->cursor.anchor;
  saved = (*anchor)[5];
  portrait = BattleEffect_GetResourceIdFar(shop->keeperResource);
  message = Sanctum_GetServiceMessage(message);
  (*anchor)[5] = 13;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(message, 5, 0, (portrait << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
  shop->cursor.anchor->active = saved;
}

// Sanctum_Run is the sanctum's service loop. It opens the priest's window
// (his face resource from the actor's sprite, or a plain window), the cursor
// and the coins window, then offers the party service until the menu is
// cancelled. The Broken Seal's Sanctum_Run in games/tbs/src/shop/shop.c,
// written with this game's calls (Shop_InitializeWork for the cursor work,
// Portrait_OpenWindowFar for the side-object window,
// MenuCursor_SetPositionImmediate to place the cursor,
// Sanctum_ShowKeeperMessage to show a message, Sanctum_CountMembersInNeed to
// count the downed, Sanctum_RunPartyService for the service, Shop_ReleaseWork
// to clean up), its work-block offsets and numbered messages.

void SideObject_CreateFar(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);

s32 Sanctum_Run(s32 unitId) {
  s32 partyAction = 0;
  s32 listWindow = 0;
  struct ShopRuntime *shop;
  struct ShopCursorAnchor *cursorAnchor;

  Shop_InitializeWork();
  shop = Ram_HeapSlots->menuRuntime;
  shop->partyAction = listWindow;

  shop->keeperResource =
      **(u16 **)(*(u8 **)((u8 *)Actor_GetByIdFar__shop(unitId) + 80) + 40);

  listWindow = Portrait_OpenWindowFar(shop->keeperResource, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = Window_CreateFar(-5, 0, 5, 5, 2);
    if (listWindow == 0) {
      listWindow = Window_CreateFar(0, 0, 5, 5, 2);
      SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
    }
  }

  cursorAnchor = (struct ShopCursorAnchor *)WindowSprite_CreateFar(
      shop->cursorIcon, OUTPUT_SIZE_16X16, (struct RenderInput *)listWindow, 0,
      0);
  cursorAnchor->active = 1;
  cursorAnchor->unk00[4] = 0;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = (struct WindowSprite *)cursorAnchor;
  Sanctum_ShowKeeperMessage((s32)MsgWelcomeWearyWanderers);

  shop->moneyWindow = Window_CreateFar(16, 11, SHOP_COIN_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();

  for (;;) {
    partyAction = Menu_SelectSanctumServiceFar(partyAction);
    shop->partyAction = partyAction;
    if (partyAction == -1)
      break;

    Sanctum_ShowKeeperMessage((s32)MsgReviveService);
    if (Sanctum_CountMembersInNeed() == 0) {
      Sanctum_ShowKeeperMessage((s32)MsgReviveService + 1);
    } else {
      Sanctum_RunPartyService();
    }
    shop->partyAction = 0;
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    Sanctum_ShowKeeperMessage((s32)MsgSanctumMoreAid);
  }

  Sanctum_ShowKeeperMessage((s32)MsgSanctumFarewell);
  Window_CloseFar(shop->moneyWindow, 2);
  Window_CloseFar(listWindow, 2);
  Shop_ReleaseWork();
  return 0;
}

// Italian creates no price window and passes 0 where the others pass it, a
// layout difference of that language's edition.
// XXX: what Italian shows in its place; the Italian render of the sanctum
// would settle it.
#define SERVICE_PRICE_WINDOW (LANGUAGE != LANGUAGE_ITALIAN)

// The price window's left column and width in tiles, sized to each
// language's text.
#if LANGUAGE == LANGUAGE_GERMAN
#define SERVICE_PRICE_WINDOW_X 0
#define SERVICE_PRICE_WINDOW_WIDTH 30
#elif LANGUAGE == LANGUAGE_FRENCH
#define SERVICE_PRICE_WINDOW_X 1
#define SERVICE_PRICE_WINDOW_WIDTH 25
#else
#define SERVICE_PRICE_WINDOW_X 1
#define SERVICE_PRICE_WINDOW_WIDTH 23
#endif

// One source for all six editions: every local takes its default at the
// top, in this order, and memberIndex is zeroed again after the windows.
// How the listing's zeros come out of the defaults:
// - listWindow's NULL comes first, so cse stores memberIndex's first 0 from
//   listWindow's register, r11. A store from a high register is reloaded
//   through a low one, the function's first reload, so r0: reload registers
//   go round r0 to r3 from one instruction to the next, which is why redraw's
//   1 then takes r1 and the 0x507 offset r2. listWindow is set again before
//   any read, so its own mov from r0 is deleted after reload; only
//   movs r0, #0 and the store remain.
// - In Italian priceWindow's NULL is the second zero. cse1 stores it for the
//   second memberIndex = 0 while its uses in the loop still exist; gcse then
//   turns those uses into constants, which leaves it local to the first
//   block and alive across the three calls: local-alloc gives it r5, ahead
//   of global's choice for shop. unitId's 0 beside it is reloaded as a copy
//   of r5, and both, with chooseAgain's 0, sink below the last call in
//   sched2.
// - With a price window the second store's zero is a temporary of its own,
//   local to the first block across its four calls: local-alloc gives it r6
//   (r5 holds the windows' style, 2), which leaves global one register short,
//   so chooseAgain and priceWindow go to the stack and chooseAgain's 0 is
//   stored from r6.
s32 Sanctum_RunPartyService(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  void *priceWindow;
  s32 redraw;
  s32 serviceKind;
  void *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  s32 price;
  s32 messageBase;

  // Default: neither window exists yet; Italian's price window never does.
  listWindow = NULL;
  priceWindow = NULL;
  // DEAD: memberIndex is set again after the windows before any read.
  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  redraw = 1;
  {
    s8 *partyAction = &shop->partyAction;
    /* FAKEMATCH(temporary): partyAction read through a pointer to it, so
     * memberIndex's 0 is stored before it is read; as serviceKind =
     * shop->partyAction, 4 of 297 instructions differ in tla-en. */
    serviceKind = *partyAction;
  }
  Sanctum_ShowKeeperMessage((s32)MsgWhoShallIRevive);

  listWindow = Window_CreateFar(1, 12, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = redraw;
  Menu_PlaceOwnerEntryObjectsFar(listWindow, 2, 0);
#if SERVICE_PRICE_WINDOW
  priceWindow = Window_CreateFar(SERVICE_PRICE_WINDOW_X, 16,
                                 SERVICE_PRICE_WINDOW_WIDTH, 3, 2);
#endif
  memberIndex = 0;
  while (memberIndex < shop->partyMemberCount) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Sanctum_NeedsService(unitId, serviceKind) != 0)
      break;
    memberIndex++;
  }

  redraw = 2;
loop:
  if (chooseAgain != 0) {
    chooseAgain = 0;
    Sanctum_ShowKeeperMessage((s32)MsgWhoShallIRevive);
    redraw = 2;
    memberIndex = 0;
    while (memberIndex < shop->partyMemberCount) {
      unitId = shop->partyMemberIds[memberIndex];
      if (Sanctum_NeedsService(unitId, serviceKind) != 0)
        break;
      memberIndex++;
    }
  }

  if (redraw != 0) {
    unitId = shop->partyMemberIds[memberIndex];
    MenuCursor_Place(listWindow, (memberIndex % 4) * 24 - 12, 0);
    shop->mode = 3;
    if (redraw == 2) {
      Menu_CreateOwnerEntryObjectsFar(memberIndex / 4 * 4);
      wait(1);
    }
    Sanctum_HighlightMembers(listWindow, memberIndex);
    Sanctum_DrawFee(priceWindow, unitId);
    redraw = 0;
  }

  if ((gInput.pressed & 1) != 0) {
    wait(1);
    price = Sanctum_ServicePrice(unitId, serviceKind);
    if (Sanctum_NeedsService(unitId, serviceKind) == 0) {
      Sound_PlayFar(113);
      goto loop;
    }
    WindowSprite_PrepareForRedrawFar(listWindow);
    MessageArgument_PushFar(unitId, 1);
    MessageArgument_PushFar(price, 5);
    messageBase = (s32)MsgReviving;
    Sanctum_ShowKeeperMessage(messageBase);
    if (Sanctum_AskYesNo(0) != 0) {
      Sanctum_ShowKeeperMessageHidingCursor(messageBase + 2);
      chooseAgain = 1;
      goto loop;
    }
    if ((u32)price > gMainRoutineWork.coins) {
      Sound_PlayFar(113);
      Sanctum_ShowKeeperMessageHidingCursor(messageBase + 1);
      chooseAgain = 1;
      goto loop;
    }
    MessageArgument_PushFar(unitId, 1);
    Sanctum_ShowKeeperMessage(messageBase + 3);
    Message_CloseFinishedWindowsFar();
    Sanctum_ApplyService(unitId, serviceKind);
    Sanctum_RunMemberIconBurst(memberIndex % 4);
    Party_AdjustCoinsFar(-price);
    Shop_DrawMoney();
    MessageArgument_PushFar(unitId, 1);
    Sanctum_ShowKeeperMessage(messageBase + 4);
    if (Sanctum_CountMembersInNeed() != 0) {
      chooseAgain = 1;
      goto loop;
    }
    goto done;
  }
  if ((gInput.pressed & 2) != 0) {
    Sound_PlayFar(113);
    goto done;
  }
  {
    s32 count = shop->partyMemberCount;
    /* FAKEMATCH(temporary): the member count read into a local of its own, so
     * it loads before memberIndex's address is set; as
     * MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount, 4), 2 of 297
     * instructions differ in tla-en. */
    redraw = MenuGrid_MoveCursor(&memberIndex, count, 4);
  }
  wait(1);
  goto loop;

done:
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(listWindow, 2);
  wait(1);
  return 0;
}

#undef SERVICE_PRICE_WINDOW_WIDTH
#undef SERVICE_PRICE_WINDOW_WIDTH
#undef SERVICE_PRICE_WINDOW_WIDTH

#define Item_Get ((u8 *(*)(u16))Item_Get)

void Sanctum_ApplyService(s32 id, s32 mode) {
  struct BattleUnit *unit;

  unit = Unit_Get(id);
  if (mode == 0) {
    unit->hp = unit->maxHp;
    Unit_RecalculateGaugesFar(id);
  } else if (mode == 1) {
    unit->poison = 0;
  } else if (mode == 2) {
    unit->haunt = 0;
  } else if (mode == 3) {
    s32 i;

    for (i = 0; i < 15; i++) {
      if (unit->inventory[i] & 0x200) {
        if (Item_Get(unit->inventory[i])[3] & 1) {
          unit->inventory[i] ^= 0x200;
          Unit_CalculateFar(id);
        }
      }
    }
  }
}

#undef Item_Get

// Sanctum_HighlightMembers draws the sanctum's party page, four members at a
// time with page arrows: The Broken Seal's Shop_DrawParty
// (games/tbs/src/shop/shop.c) shape over a page, each member dimmed when
// Sanctum_NeedsService refuses the chosen service.

void Sanctum_HighlightMembers(void *window, s32 selected) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 serviceKind = shop->partyAction;
  s32 first = selected / 4 * 4;
  s32 count = shop->partyMemberCount;
  struct WindowSprite *icon;
  s32 i;

  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (first != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, OUTPUT_SIZE_16X16,
                                    window, 88, -12);
      icon->kind = 0;
      icon->active = 17;
      icon->animStep = 0;
    }
    if (first + 4 < count) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon, OUTPUT_SIZE_16X16,
                                    window, 88, 0);
      icon->kind = 0;
      icon->active = 15;
      icon->animStep = 0;
    }
    for (i = 0; i < 4; i++) {
      s32 unitId = shop->partyMemberIds[first + i];

      if (shop->partyMemberIcons[i] != 0) {
        if (first + i == selected)
          Animation_ApplyChildArgumentFar(shop->partyMemberIcons[i], 30);
        else
          Animation_ApplyChildArgumentFar(shop->partyMemberIcons[i], 1);
        shop->partyMemberScale[i] = Q16_ONE;
        if (!Sanctum_NeedsService(unitId, serviceKind))
          shop->partyMemberScale[i] = 0xb333;
      }
    }
  }
}

/// Shows the selected member's service price, or that no treatment is needed.
// As ☀️'s Sanctum_DrawFee. The service kind's variable takes the message
// once the kind is no longer needed, as the native's register does.
void Sanctum_DrawFee(s32 window, s32 unitId) {
  s32 kindOrMessage;
  s32 price;
  struct ShopRuntime *shop;

  shop = Ram_HeapSlots->menuRuntime;
  kindOrMessage = shop->partyAction;
  price = Sanctum_ServicePrice(unitId, kindOrMessage);
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (Sanctum_NeedsService(unitId, kindOrMessage) != 0)
      kindOrMessage = (s32)MsgReviveCost;
    else
      kindOrMessage = (s32)MsgNoHealingNeeded;
    kindOrMessage = Sanctum_GetServiceMessage(kindOrMessage);
    MessageArgument_PushFar(price, 5);
    Text_DrawMessageFar(kindOrMessage, window, 0, 0);
  }
}

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Sanctum_UpdateBurstSparkle(struct Effect_080b2f4c *effect) {
  struct Position position;
  s8 *statePointer;
  s32 state;
  s32 result;

  statePointer = &effect->state;
  state = *statePointer;
  if (state == 0) {
    position.x = effect->sourceX;
    position.z = effect->sourceZ;
    Vector_AddPolarOffset(0x280000, Random16(), &position);
    FieldParticle_SetPositionFar(effect, position.x, position.z);
    position.x = effect->sourceX;
    position.z = effect->sourceZ;
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->x = position.x;
    effect->z = position.z;
    effect->velocity = 0x20000;
    effect->acceleration = 0x6666;
    effect->flag = state;
    *statePointer = (u8)*statePointer + 1;
  } else if (state == 1) {
    result = FieldParticle_HasPendingTargetFar(effect);
    if (result == 0)
      *statePointer = result;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTargetFar(effect) == 0)
      FieldEffect_ClearOwnedSlotFar(effect);
  }
}

/* VIEW(shop): routine Animation_ApplyChildValuesToRecordFar as void
 * Animation_ApplyChildValuesToRecordFar(void *child, u32 value); actor_view.h
 * has void Animation_ApplyChildValuesToRecordFar(struct ActorSpriteView *,
 * u32). */
extern void Animation_ApplyChildValuesToRecordFar__shop(
    void *child, u32 value) __asm__("Animation_ApplyChildValuesToRecordFar");

void Sanctum_UpdateBurst(void) {
  struct ShopFxWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;
  s32 selected;

  for (i = 0; i < 24; i++)
    FieldParticle_UpdateFar(work->slots[i]);
  selected = work->selected;
  if (selected != -1)
    Animation_ApplyChildValuesToRecordFar__shop(work->children[selected],
                                                (Random16() * 7) >> 16);
}

void Animation_ApplyChildValueFar(s32 object, s32 arg);

void Sanctum_RunMemberIconBurst(s32 member) {
  struct BurstShop *shop;
  s8 savedCursorMode;
  struct Position position;
  struct BurstSprite *effect;
  s32 callbackPriority;
  s32 effectIndex;

  shop = Ram_HeapSlots->menuRuntime;
  savedCursorMode = shop->cursor.anchor->active;
  shop->burstMember = -1;
  shop->cursor.anchor->active = 13;
  Sound_PlayFar(Data_0810cf28[shop->action]);
  Shop_SaveAndTintFieldPalette(0x00202108);
  Animation_ApplyChildValueFar(shop->partyMemberIcons[member], 0);
  wait(20);
  callbackPriority = 0x480;
  Task_Add(Sanctum_UpdateBurst, callbackPriority);

  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) - (12 << 16);

  effectIndex = 0;
  effect = shop->effects;
  do {
    FieldParticle_InitializeFar((struct Effect_080b2f4c *)effect, 0x2a0,
                                position.x, position.z);
    FieldParticle_SetCallbackFar((struct Effect_080b2f4c *)effect,
                                 Sanctum_UpdateBurstSparkle);
    FieldParticle_SetActorModeFar((struct Effect_080b2f4c *)effect, 7);
    Animation_ApplyChildValuesToRecordFar__shop(effect->object,
                                                (Random16() * 7) >> 16);
    effect->scaleY = Q16_ONE * 7 / 10;
    effect->scaleX = Q16_ONE * 7 / 10;
    wait(3);
    if (effectIndex == 5) {
      shop->burstMember = member;
    }
    effectIndex++;
    effect++;
  } while (effectIndex <= 17);

  Sound_WaitForJingle();
  {
    u8 activeMode = 2;
    u8 *entry = (u8 *)&shop->effects[0].state;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      if (*(s8 *)(entry + 5) != 0) {
        *entry = activeMode;
      }
      entry += sizeof(struct BurstSprite);
    }
  }

  wait(20);
  Sound_PlayFar(126);
  shop->burstMember = -1;
  Animation_ApplyChildValuesToRecordFar__shop(shop->partyMemberIcons[member],
                                              0);
  wait(20);

  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct BurstSprite *effectSlot = shop->effects;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += sizeof(struct BurstSprite);
      if (flag != 0) {
        FieldEffect_ClearOwnedSlotFar((struct Effect_080b2f4c *)effectSlot);
      }
      effectSlot++;
    }
  }

  Task_Remove(Sanctum_UpdateBurst);
  Animation_ApplyChildValueFar(shop->partyMemberIcons[member], 16);
  Shop_RestoreFieldPalette();
  wait(30);
  shop->cursor.anchor->active = savedCursorMode;
}
