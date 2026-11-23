/// @file
/// The sanctum's services, their donations and their sparkles.
// Part of shop.c.

#include "shop_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/field_sequence.h"
#include "gamepak/far_call/window.h"
#include "shop/shop.h"
#include "gamepak/far_call/menu.h"
#include "system/system.h"
#include "window/cursor.h"

/// The donations the sanctum asks, in coins: reviving and removing a curse
/// ask so many for each of the party member's levels.
enum SanctumDonation {
  DONATION_REVIVE_PER_LEVEL = 20,
  DONATION_CURE_POISON = 10,
  DONATION_REPEL_EVIL = 50,
  DONATION_REMOVE_CURSE_PER_LEVEL = 10
};

/// Returns the sanctum's donation, in coins, for `serviceKind` on party member
/// `unitId`.
s32 Sanctum_ServicePrice(s32 unitId, s32 serviceKind) {
  u8 level = Unit_Get(unitId)->level;
  s32 donation = 0;

  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    donation = level * DONATION_REVIVE_PER_LEVEL;
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    donation = DONATION_CURE_POISON;
  } else if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
    donation = DONATION_REPEL_EVIL;
  } else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    donation = level * DONATION_REMOVE_CURSE_PER_LEVEL;
  }
  return donation;
}

/// Returns 1 when a party member needs the service: down for a revive, poisoned
/// for a cure, haunted for an exorcism, cursed for removing a curse.
s32 Sanctum_NeedsService(s32 unitId, s32 serviceKind) {
  struct Unit *unit = Unit_Get(unitId);
  s32 eligible = 0;

  if ((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0) ||
      (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0) ||
      (serviceKind == SANCTUM_SERVICE_REPEL_EVIL && unit->haunt != 0) ||
      (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->curse != 0)) {
    eligible = 1;
  }
  return eligible;
}

/// Counts party members who need the service in shop->action.
s32 Sanctum_CountMembersInNeed(void) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;
  s32 eligibleCount = 0;
  s32 serviceKind = shop->action;
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Sanctum_NeedsService(shop->partyMemberIds[memberIndex], serviceKind))
      eligibleCount++;
  }
  return eligibleCount;
}

/// Returns the current service's line that stands where messageId stands among
/// the revive service's lines.
s32 Sanctum_GetServiceMessage(s32 messageId) {
  struct ShopWork *shop;
  s8 serviceKind;

  shop = Ram_HeapSlots->menuWork;
  serviceKind = shop->action;
  if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    messageId += (u32)MsgCurePoisonService - (u32)MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
    messageId += (u32)MsgExorcismService - (u32)MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    messageId += (u32)MsgRemoveCurseService - (u32)MsgReviveService;
  }
  return messageId;
}

/// Shows the keeper's line for the current service and waits for its window.
void Sanctum_ShowKeeperMessage(s32 messageId) {
  struct ShopWork *shop;
  s32 voice;

  shop = Ram_HeapSlots->menuWork;
  voice = Speaker_GetVoiceFar(shop->keeperGraphics);
  Message_CloseFinishedWindowsFar();
  messageId = Sanctum_GetServiceMessage(messageId);
  Message_OpenWindowFar(messageId, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
}

/// As Sanctum_ShowKeeperMessage, hiding the hand cursor while the line shows.
void Sanctum_ShowKeeperMessageHidingCursor(s32 messageId) {
  struct ShopWork *shop;
  struct WindowSprite **cursorSlot;
  s32 voice;
  u8 savedCursorMode;

  shop = Ram_HeapSlots->menuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->motion;
  voice = Speaker_GetVoiceFar(shop->keeperGraphics);
  messageId = Sanctum_GetServiceMessage(messageId);
  (*cursorSlot)->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(messageId, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
  shop->cursor.anchor->motion = savedCursorMode;
}

void Portrait_CreateFar(s32 portrait, s32 a, s32 b, struct Window *window,
                        s32 x, s32 y);

/// Runs a sanctum: the keeper's welcome, the service menu until B, and the
/// farewell; `actorId` is the keeper, whose portrait the lines show.
s32 Sanctum_Run(s32 actorId) {
  s32 serviceSelection = 0;
  struct Window *listWindow = 0;
  struct ShopWork *shop;
  struct WindowSprite *cursorAnchor;

  Shop_InitializeWork();
  shop = Ram_HeapSlots->menuWork;
  shop->action = serviceSelection;

  shop->keeperGraphics = ((struct Sprite *)Actor_GetByIdFar(actorId)->sprite)
                             ->entries[0]
                             ->animationId;

  // The keeper's portrait window; failing that an empty one off the left
  // edge, and failing that one on screen with Ivan's portrait
  listWindow = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = Window_CreateFar(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
    if (listWindow == 0) {
      listWindow = Window_CreateFar(0, 0, 5, 5, WINDOW_FLAG_INSTANT);
      Portrait_CreateFar(IVAN, 0, 0, listWindow, -4, -4);
    }
  }

  cursorAnchor = WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16,
                                        listWindow, 0, 0);
  cursorAnchor->motion = WINDOW_SPRITE_MOTION_STILL;
  cursorAnchor->kind = 0;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  Sanctum_ShowKeeperMessage((s32)MsgWelcomeWearyWanderers);

  shop->moneyWindow =
      Window_CreateFar(16, 11, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();

  for (;;) {
    serviceSelection = Menu_SelectSanctumServiceFar(serviceSelection);
    shop->action = serviceSelection;
    if (serviceSelection == -1)
      break;

    Sanctum_ShowKeeperMessage((s32)MsgReviveService);
    if (Sanctum_CountMembersInNeed() == 0) {
      Sanctum_ShowKeeperMessage((s32)MsgReviveService + 1);
    } else {
      Sanctum_RunPartyService();
    }
    shop->action = SANCTUM_SERVICE_REVIVE;
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    Sanctum_ShowKeeperMessage((s32)MsgSanctumMoreAid);
  }

  Sanctum_ShowKeeperMessage((s32)MsgSanctumFarewell);
  Window_CloseFar(shop->moneyWindow, 2);
  Window_CloseFar(listWindow, 2);
  Shop_ReleaseWork();
  return 0;
}

/// Italian creates no donation window and passes 0 where the others pass it, a
/// layout difference of that language's edition: Sanctum_DrawFee draws nothing
/// without a window, so Italian shows no donation while a party member is
/// chosen, and the donation is first seen in the keeper's line (MsgReviving).
#define SANCTUM_HAS_DONATION_WINDOW (LANGUAGE != LANGUAGE_ITALIAN)

/// The donation window's left column and width in tiles, sized to each
/// language's text.
#if LANGUAGE == LANGUAGE_GERMAN
#define SANCTUM_DONATION_WINDOW_X 0
#define SANCTUM_DONATION_WINDOW_WIDTH 30
#elif LANGUAGE == LANGUAGE_FRENCH
#define SANCTUM_DONATION_WINDOW_X 1
#define SANCTUM_DONATION_WINDOW_WIDTH 25
#else
#define SANCTUM_DONATION_WINDOW_X 1
#define SANCTUM_DONATION_WINDOW_WIDTH 23
#endif

/// Runs one service: the player chooses the party member, sees the donation,
/// confirms, pays and watches the burst. Repeats while anyone still needs it,
/// until B.
s32 Sanctum_RunPartyService(void) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;
  struct Window *donationWindow;
  s32 redraw;
  s32 serviceKind;
  struct Window *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  s32 donation;
  s32 messageBase;

  // Neither window exists yet; Italian's donation window never does.
  listWindow = NULL;
  donationWindow = NULL;
  // DEAD: memberIndex is set again after the windows before any read; the ROM
  // keeps its store.
  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  redraw = 1;
  serviceKind = shop->action;
  Sanctum_ShowKeeperMessage((s32)MsgWhoShallIRevive);

  listWindow = Window_CreateFar(1, 12, 13, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  shop->cursorFrames = redraw;
  Menu_PlaceOwnerEntryObjectsFar(listWindow, 2, 0);
#if SANCTUM_HAS_DONATION_WINDOW
  donationWindow =
      Window_CreateFar(SANCTUM_DONATION_WINDOW_X, 16,
                       SANCTUM_DONATION_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);
#endif
  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Sanctum_NeedsService(unitId, serviceKind) != 0)
      break;
  }

  redraw = 2;
  for (;;) {
    if (chooseAgain != 0) {
      chooseAgain = 0;
      Sanctum_ShowKeeperMessage((s32)MsgWhoShallIRevive);
      redraw = 2;
      for (memberIndex = 0; memberIndex < shop->partyMemberCount;
           memberIndex++) {
        unitId = shop->partyMemberIds[memberIndex];
        if (Sanctum_NeedsService(unitId, serviceKind) != 0)
          break;
      }
    }

    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(listWindow,
                       (memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
      shop->cursorFrames = 3;
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjectsFar(memberIndex / SHOP_PARTY_PAGE_MEMBERS *
                                        SHOP_PARTY_PAGE_MEMBERS);
        wait(1);
      }
      Sanctum_HighlightMembers(listWindow, memberIndex);
      Sanctum_DrawFee(donationWindow, unitId);
      redraw = 0;
    }

    if ((gInput.pressed & KEY_A) != 0) {
      wait(1);
      donation = Sanctum_ServicePrice(unitId, serviceKind);
      if (Sanctum_NeedsService(unitId, serviceKind) == 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(donation, MESSAGE_ARGUMENT_NUMBER);
      messageBase = (s32)MsgReviving;
      Sanctum_ShowKeeperMessage(messageBase);
      if (Sanctum_AskYesNo(0) != 0) {
        Sanctum_ShowKeeperMessageHidingCursor(messageBase + 2);
        chooseAgain = 1;
        continue;
      }
      if ((u32)donation > gMainRoutineWork.coins) {
        Sound_PlayFar(SE_MENU_CANCEL);
        Sanctum_ShowKeeperMessageHidingCursor(messageBase + 1);
        chooseAgain = 1;
        continue;
      }
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      Sanctum_ShowKeeperMessage(messageBase + 3);
      Message_CloseFinishedWindowsFar();
      Sanctum_ApplyService(unitId, serviceKind);
      Sanctum_RunMemberIconBurst(memberIndex % SHOP_PARTY_PAGE_MEMBERS);
      Party_AdjustCoinsFar(-donation);
      Shop_DrawMoney();
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      Sanctum_ShowKeeperMessage(messageBase + 4);
      if (Sanctum_CountMembersInNeed() != 0) {
        chooseAgain = 1;
        continue;
      }
      break;
    }
    if ((gInput.pressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    }
    redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount,
                                 SHOP_PARTY_PAGE_MEMBERS);
    wait(1);
  }

  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(donationWindow, 2);
  Window_CloseFar(listWindow, 2);
  wait(1);
  return 0;
}

#undef SANCTUM_HAS_DONATION_WINDOW
#undef SANCTUM_DONATION_WINDOW_X
#undef SANCTUM_DONATION_WINDOW_WIDTH

/// Gives the party member the service: full HP for a revive, the poison or the
/// haunting lifted, or every cursed item it wears taken off.
void Sanctum_ApplyService(s32 unitId, s32 serviceKind) {
  struct Unit *unit;

  unit = Unit_Get(unitId);
  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    unit->hp = unit->hpMax;
    Unit_RecalculateGaugesFar(unitId);
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    unit->poison = 0;
  } else if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
    unit->haunt = 0;
  } else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    s32 inventorySlot;

    for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
      if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
        if (Item_GetDirectFar(unit->inventory[inventorySlot])->flags &
            ITEM_CURSED) {
          unit->inventory[inventorySlot] ^= INVENTORY_EQUIPPED;
          Unit_CalculateFar(unitId);
        }
      }
    }
  }
}

/// Shows the chosen party member running in place and the others standing,
/// four a page with its arrows, and draws at seven tenths of their size the
/// party members who do not need the service.
void Sanctum_HighlightMembers(struct Window *window, s32 selected) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;
  s32 serviceKind = shop->action;
  s32 first = selected / SHOP_PARTY_PAGE_MEMBERS * SHOP_PARTY_PAGE_MEMBERS;
  s32 count = shop->partyMemberCount;
  struct WindowSprite *icon;
  s32 i;

  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (first != 0) {
      icon = WindowSprite_CreateFar(shop->previousPageIcon, OUTPUT_SIZE_16X16,
                                    window, 88, -12);
      icon->kind = 0;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
      icon->animationStep = 0;
    }
    if (first + SHOP_PARTY_PAGE_MEMBERS < count) {
      icon = WindowSprite_CreateFar(shop->nextPageIcon, OUTPUT_SIZE_16X16,
                                    window, 88, 0);
      icon->kind = 0;
      icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
      icon->animationStep = 0;
    }
    for (i = 0; i < SHOP_PARTY_PAGE_MEMBERS; i++) {
      s32 unitId = shop->partyMemberIds[first + i];

      if (shop->partyMemberIcons[i] != 0) {
        if (first + i == selected)
          Sprite_StartAnimationFar(shop->partyMemberIcons[i], ANIM_PARTY_RUN);
        else
          Sprite_StartAnimationFar(shop->partyMemberIcons[i], ANIM_STAND);
        shop->partyMemberScale[i] = FIXED_ONE;
        if (!Sanctum_NeedsService(unitId, serviceKind))
          shop->partyMemberScale[i] = FIXED_ONE * 7 / 10;
      }
    }
  }
}

/// Shows the chosen party member's donation for the service, or that none is
/// needed.
void Sanctum_DrawFee(struct Window *window, s32 unitId) {
  s32 message;
  s32 donation;
  struct ShopWork *shop;

  shop = Ram_HeapSlots->menuWork;
  message = shop->action;
  donation = Sanctum_ServicePrice(unitId, message);
  if (window != 0) {
    WindowSprite_PrepareForRedrawFar(window);
    if (Sanctum_NeedsService(unitId, message) != 0)
      message = (s32)MsgReviveCost;
    else
      message = (s32)MsgNoHealingNeeded;
    message = Sanctum_GetServiceMessage(message);
    MessageArgument_PushFar(donation, MESSAGE_ARGUMENT_NUMBER);
    Text_DrawMessageFar(message, (s32)window, 0, 0);
  }
}

/// A position in 16.16 fixed-point pixels, as Vector_AddPolarOffset moves it.
struct Point {
  FIXED x;
  FIXED y;
  FIXED z;
};

/// Runs one sparkle of the service's burst: placed on a ring 40 pixels out and
/// sent to a point near the centre, it flies in again each time it arrives,
/// until the burst ends and it is freed on arrival.
void Sanctum_UpdateBurstSparkle(struct FieldParticle *sparkle) {
  struct FieldParticle *particle = (struct FieldParticle *)sparkle;
  struct Point position;
  s8 *statePointer;
  s32 state;
  s32 result;

  statePointer = &particle->state;
  state = *statePointer;
  if (state == SANCTUM_SPARKLE_START) {
    position.x = particle->originX;
    position.z = particle->originZ;
    Vector_AddPolarOffset(toFIXED(40), Random16(), &position.x);
    FieldParticle_SetPositionFar(sparkle, position.x, position.z);
    position.x = particle->originX;
    position.z = particle->originZ;
    Vector_AddPolarOffset(toFIXED(4), Random16(), &position.x);
    particle->targetX = position.x;
    particle->targetZ = position.z;
    particle->speedMax = toFIXED(2);
    particle->acceleration = toFIXED(2) / 5;
    particle->turnStepLimited = state;
    *statePointer = (u8)*statePointer + 1;
  } else if (state == SANCTUM_SPARKLE_FLYING) {
    result = FieldParticle_HasPendingTargetFar(sparkle);
    if (result == 0)
      *statePointer = result;
  } else if (state == SANCTUM_SPARKLE_LAST_FLIGHT) {
    if (FieldParticle_HasPendingTargetFar(sparkle) == 0)
      FieldEffect_ClearOwnedSlotFar(sparkle);
  }
}

void Sprite_SetAnimationColorModesFar(void *object, s32 value);

/// Moves the burst's sparkles each frame and gives the party member's icon a
/// random colour mode while the burst plays on it. Runs as a task.
void Sanctum_UpdateBurst(void) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;
  s32 i;
  s32 burstMember;

  for (i = 0; i < SANCTUM_PARTICLE_COUNT; i++)
    FieldParticle_UpdateFar(&shop->effects[i]);
  burstMember = shop->burstMember;
  if (burstMember != -1)
    Sprite_SetAnimationColorModesFar(shop->partyMemberIcons[burstMember],
                                     (Random16() * 7) >> 16);
}

void Sprite_SetAnimationSpeedFar(void *object, s32 speed);

/// Plays the service's burst on a party member's icon: its tune, the colours
/// tinted, and SANCTUM_BURST_SPARKLES sparkles flying in around the icon as it
/// changes colour.
void Sanctum_RunMemberIconBurst(s32 member) {
  struct ShopWork *shop;
  s8 savedCursorMode;
  struct Point position;
  struct FieldParticle *effect;
  s32 effectIndex;

  shop = Ram_HeapSlots->menuWork;
  savedCursorMode = shop->cursor.anchor->motion;
  shop->burstMember = -1;
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Sound_PlayFar(gSanctum_ServiceTunes[shop->action]);
  Shop_SaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
  Sprite_SetAnimationSpeedFar(shop->partyMemberIcons[member], 0);
  wait(20);
  Task_Add(Sanctum_UpdateBurst, TASK_PASS_FRAME);

  position.x = toFIXED(shop->partyMemberX[member]);
  position.z = toFIXED(shop->partyMemberY[member]) - toFIXED(12);

  for (effectIndex = 0; effectIndex < SANCTUM_BURST_SPARKLES; effectIndex++) {
    effect = &shop->effects[effectIndex];
    FieldParticle_InitializeFar(effect, SPRITE_PARTICLE, position.x,
                                position.z);
    FieldParticle_SetCallbackFar(effect, Sanctum_UpdateBurstSparkle);
    FieldParticle_SetActorModeFar(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->actor, (Random16() * 7) >> 16);
    effect->scaleY = FIXED_ONE * 7 / 10;
    effect->scaleX = FIXED_ONE * 7 / 10;
    wait(3);
    if (effectIndex == 5) {
      shop->burstMember = member;
    }
  }

  Sound_WaitForJingle();
  // Every sparkle still flying makes its last flight.
  {
    u8 *entry;
    FIELD_PARTICLES_SET_ACTIVE_STATE(entry, effectIndex, shop, effects,
                                     SANCTUM_SPARKLE_LAST_FLIGHT);
  }

  wait(20);
  Sound_PlayFar(SE_RECOVERY);
  shop->burstMember = -1;
  Sprite_SetAnimationColorModesFar(shop->partyMemberIcons[member], 0);
  wait(20);

  {
    u8 *active = (u8 *)&shop->effects[0].active;
    struct FieldParticle *particle = shop->effects;

    for (effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++) {
      s32 flag = *active;

      // DEAD: the byte moved to the top of the word, which cannot change
      // whether it is zero; the ROM keeps the shift.
      flag <<= 24;

      active += sizeof(struct FieldParticle);
      if (flag != 0) {
        FieldEffect_ClearOwnedSlotFar(particle);
      }
      particle++;
    }
  }

  Task_Remove(Sanctum_UpdateBurst);
  Sprite_SetAnimationSpeedFar(shop->partyMemberIcons[member], ANIM_STEP_NORMAL);
  Shop_RestoreFieldPalette();
  wait(30);
  shop->cursor.anchor->motion = savedCursorMode;
}
