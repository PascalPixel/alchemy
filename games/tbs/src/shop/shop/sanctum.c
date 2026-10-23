/// @file
/// The sanctum's services, their prices and their sparkles.
// Part of shop.c.

#include "../shop_internal.h"

/// Returns the sanctum's fee, in coins, for `serviceKind` on member `unitId`:
/// 20 a level to revive, 10 to cure poison, 50 to exorcise and 10 a level to
/// remove a curse. Called by the member choice and its fee line.
s32 Shop_ServicePrice(s32 unitId, s32 serviceKind) {
  u8 level = Unit_GetFar(unitId)->level;
  s32 donation = 0;

  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    donation = level * 20;
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    donation = 10;
  } else if (serviceKind == SANCTUM_SERVICE_EXORCISE) {
    donation = 50;
  } else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    donation = level * 10;
  }
  return donation;
}

/// Returns 1 when member `unitId` needs `serviceKind`: down (HP 0) for a
/// revive, poisoned for a cure, haunted for an exorcism, any restraint bit set
/// for removing a curse. Called by the member choice, its count, its highlight
/// and its fee line.
s32 Shop_CanServe(s32 unitId, s32 serviceKind) {
  struct BattleUnit *unit = Unit_GetFar(unitId);
  s32 eligible = 0;

  if ((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0) ||
      (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0) ||
      (serviceKind == SANCTUM_SERVICE_EXORCISE && unit->haunt != 0) ||
      (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->restraint != 0)) {
    eligible = 1;
  }
  return eligible;
}

/// Counts party members who need the service in shop->partyAction. Called by
/// Shop_ConfirmAct, which says nobody needs it when the count is 0, and by
/// Sanctum_RunPartyService, which offers another member while any remain.
s32 Shop_CountUnits(void) {
  struct ShopRuntime *shop = gMenuWork;
  s32 eligibleCount = 0;
  s32 serviceKind = shop->partyAction;
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Shop_CanServe(shop->partyMemberIds[memberIndex], serviceKind))
      eligibleCount++;
  }
  return eligibleCount;
}

/// Returns the line of the service in shop->partyAction that stands where
/// `messageId` stands among the revive service's lines. Each service's lines
/// are a run in the same order, from MsgReviveService, MsgCurePoisonService,
/// MsgExorcismService and MsgRemoveCurseService.
s32 Shop_MessageByMode(s32 messageId) {
  s8 serviceKind = ((struct ShopRuntime *)gMenuWork)->partyAction;

  if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    messageId += MsgCurePoisonService - MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_EXORCISE) {
    messageId += MsgExorcismService - MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    messageId += MsgRemoveCurseService - MsgReviveService;
  }
  return messageId;
}

/// Shows the keeper's line `messageId`, moved to the current service's
/// (Shop_MessageByMode), and waits for its window, one more frame included: the
/// sanctum's UiMessage_ShowAndWait.
void UiMessage_ShowResolvedAndWait(s32 messageId) {
  s32 portraitResource;

  portraitResource = FieldEffect_GetResourceIdFar(
      ((struct ShopRuntime *)gMenuWork)->keeperResource);
  UiWork_FinalizePendingCoreFar();
  messageId = Shop_MessageByMode(messageId);
  UiText_OpenMessageWindowFar(messageId, 5, 0,
                              (portraitResource << 0x10) | 0x22);
  while (UiWork_IsCompleteFar() == 0) {
    WaitFrames(1U);
  }
  WaitFrames(1U);
}

/// As UiMessage_ShowResolvedAndWait, hiding the hand cursor while the line
/// shows and putting back its motion after: the sanctum's
/// UiMessage_ShowAndRestoreState.
void UiMessage_ShowResolvedAndRestoreState(s32 messageId) {
  struct ShopRuntime *shop;
  struct RenderOutput **cursorSlot;
  s32 portraitResource;
  u8 savedCursorMode;

  shop = gMenuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->active;
  portraitResource = FieldEffect_GetResourceIdFar(shop->keeperResource);
  messageId = Shop_MessageByMode(messageId);
  (*cursorSlot)->active = 13;
  UiWork_FinalizePendingCoreFar();
  UiText_OpenMessageWindowFar(messageId, 5, 0, (portraitResource << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
  shop->cursor.anchor->active = savedCursorMode;
}

/// Runs a sanctum: the keeper's welcome, then the service menu (revive, cure
/// poison, exorcise, remove a curse) until B, and the farewell. `unitId` is
/// the keeper's object, whose portrait the lines show. Holds the shop work
/// block while it runs; returns 0. Called through Engine_SanctumOpen by the
/// scenes' sanctum keepers and the link lobby, and by the debug test.
s32 Shop_ConfirmAct(s32 unitId) {
  s32 serviceSelection = 0;
  struct UiWindow *listWindow = 0;
  struct ShopRuntime *shop;
  struct RenderOutput *cursorAnchor;

  Shop_InitializeCursorWork();
  shop = gMenuWork;
  shop->partyAction = serviceSelection;

  {
    s32 keeperAnimationId =
        (u16)((struct AnimationObject *)((struct ObjectRuntime *)
                                             ObjectEvent_GetByIdFar(unitId))
                  ->animation)
            ->entries[0]
            ->animationId;
    shop->keeperResource = keeperAnimationId;
  }

  // The keeper's portrait window; failing that an empty one off the left
  // edge, and failing that one on screen with side object 2
  listWindow = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(-5, 0, 5, 5, UI_WINDOW_INSTANT);
  }
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(0, 0, 5, 5, UI_WINDOW_INSTANT);
    SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
  }

  cursorAnchor =
      RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, listWindow, 0, 0);
  cursorAnchor->active = 1;
  cursorAnchor->kind = 0;
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  UiMessage_ShowResolvedAndWait(MsgSanctumWelcome);

  shop->moneyWindow =
      UiWindow_CreateFar(16, 11, SHOP_MONEY_WINDOW_WIDTH, 4, UI_WINDOW_INSTANT);
  Shop_DrawMoney();

  for (;;) {
    serviceSelection = Menu_SelectSanctumServiceFar(serviceSelection);
    shop->partyAction = serviceSelection;
    if (serviceSelection == -1)
      break;

    {
      s32 messageBase = MsgReviveService;
      s32 message = messageBase;

      messageBase = 0;
      UiMessage_ShowResolvedAndWait(message);
      if (Shop_CountUnits() == 0) {
        UiMessage_ShowResolvedAndWait(message + 1);
      } else {
        Sanctum_RunPartyService();
      }
    }
    shop->partyAction = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    UiMessage_ShowResolvedAndWait(MsgSanctumMoreAid);
  }

  UiMessage_ShowResolvedAndWait(MsgSanctumFarewell);
  UiWork_FinalizeFar(shop->moneyWindow, 2);
  UiWork_FinalizeFar(listWindow, 2);
  Shop_ReleaseCursorWork();
  return 0;
}

/// Lets the player pick a member for the service in shop->partyAction,
/// starting on the first who needs it, asks for the donation, then applies
/// the service with its burst and takes the coins; offers another member
/// while any still need it, until B. A member who needs nothing can be
/// pointed at, but A only buzzes. Called by Shop_ConfirmAct; returns 0.
s32 Sanctum_RunPartyService(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct UiWindow *priceWindow;
  s32 redraw;
  s32 serviceKind;
  struct UiWindow *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  s32 donation;
  s32 messageBase;

  priceWindow = 0;
  redraw = 1;
  serviceKind = shop->partyAction;
  UiMessage_ShowResolvedAndWait(MsgWhoToRevive);

  listWindow = UiWindow_CreateFar(1, 12, 13, 3, UI_WINDOW_INSTANT);
  shop->cursor.anchor->active = UI_SPRITE_WOBBLE;
  // redraw is 1 here: the hand's first move to a member takes one frame
  shop->mode = redraw;
  PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);
  priceWindow =
      UiWindow_CreateFar(SANCTUM_PRICE_WINDOW_X, 16, SANCTUM_PRICE_WINDOW_WIDTH,
                         3, UI_WINDOW_INSTANT);

  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  while (memberIndex < shop->partyMemberCount) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Shop_CanServe(unitId, serviceKind) != 0)
      break;
    memberIndex++;
  }

  redraw = 1;
  for (;;) {
    if (chooseAgain != 0) {
      chooseAgain = 0;
      UiMessage_ShowResolvedAndWait(MsgWhoToRevive);
      redraw = 1;
      memberIndex = 0;
      while (memberIndex < shop->partyMemberCount) {
        unitId = shop->partyMemberIds[memberIndex];
        if (Shop_CanServe(unitId, serviceKind) != 0)
          break;
        memberIndex++;
      }
    }

    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Shop_HiliteUnit(listWindow, memberIndex);
      Shop_DrawSelectMessage(priceWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      WaitFrames(1);
      donation = Shop_ServicePrice(unitId, serviceKind);
      if (Shop_CanServe(unitId, serviceKind) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      UiWork_PushValueSlotFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      UiWork_PushValueSlotFar(donation, MESSAGE_ARGUMENT_NUMBER);
      messageBase = MsgReviveDonation;
      UiMessage_ShowResolvedAndWait(messageBase);
      if (UiMessage_ShowChoiceVariant(0) != 0) {
        UiMessage_ShowResolvedAndRestoreState(messageBase + 2);
        chooseAgain = 1;
        continue;
      }
      if (donation > (u32)gGameState.coins) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiMessage_ShowResolvedAndRestoreState(messageBase + 1);
        chooseAgain = 1;
        continue;
      }
      UiWork_PushValueSlotFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      UiMessage_ShowResolvedAndWait(messageBase + 3);
      UiWork_FinalizePendingCoreFar();
      // Apply the cure and animation before collecting the donation.
      BattleUnit_ResetStateByMode(unitId, serviceKind);
      Shop_RunPartyMemberIconBurst(memberIndex);
      Party_AdjustCoinsFar(-donation);
      Shop_DrawMoney();
      UiWork_PushValueSlotFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      UiMessage_ShowResolvedAndWait(messageBase + 4);
      if (Shop_CountUnits() != 0) {
        chooseAgain = 1;
        continue;
      }
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex -= 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex += 1;
    }
    WaitFrames(1);
    continue;
  }

  Menu_ReleaseEntryObjectsFar();
  UiWork_FinalizeFar(priceWindow, 2);
  UiWork_FinalizeFar(listWindow, 2);
  WaitFrames(1);
  return 0;
}

/// Applies sanctum service `serviceKind` to member `unitId`: full HP for a
/// revive, poison or the haunting cleared, or every cursed item taken off
/// (Unit_Calculate then rebuilds the curse bits from what is still equipped).
/// With no cursed item equipped, removing a curse changes nothing, whatever
/// restraint bits Shop_CanServe saw. Called by Sanctum_RunPartyService.
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind) {
  struct BattleUnit *unit;

  unit = Unit_GetFar(unitId);
  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    unit->hp = unit->hpMax;
    Unit_UpdateRatiosFar(unitId);
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    unit->poison = 0;
  } else if (serviceKind == SANCTUM_SERVICE_EXORCISE) {
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

/// Shows the chosen member walking and the others standing, and draws at
/// seven tenths of their size (0xb333 in 16.16) the members who do not need
/// the service. The sanctum's Shop_DrawParty.
void Shop_HiliteUnit(struct UiWindow *window, s32 selected) {
  struct ShopRuntime *shop = gMenuWork;
  s32 memberIndex;
  s32 serviceKind = shop->partyAction;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        AnimationObject_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                           30);
      else
        AnimationObject_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                           1);
      shop->partyMemberScale[memberIndex] = 0x10000;
      if (!Shop_CanServe(shop->partyMemberIds[memberIndex], serviceKind))
        shop->partyMemberScale[memberIndex] = 0xb333;
    }
  }
}

/// Draws in `target` the member's fee for the current service (its cost line
/// with the donation filled in), or the line that the member needs no
/// treatment. Called by Sanctum_RunPartyService each time the cursor moves.
void Shop_DrawSelectMessage(struct UiWindow *target, s32 unitId) {
  s32 serviceOrMessage;
  s32 donation;

  serviceOrMessage = ((struct ShopRuntime *)gMenuWork)->partyAction;
  donation = Shop_ServicePrice(unitId, serviceOrMessage);
  if (target != 0) {
    RenderOutput_PrepareForRedrawFar(target);
    if (Shop_CanServe(unitId, serviceOrMessage) != 0) {
      serviceOrMessage = MsgReviveCost;
    } else {
      serviceOrMessage = MsgNoHealingNeeded;
    }
    serviceOrMessage = Shop_MessageByMode(serviceOrMessage);
    UiWork_PushValueSlotFar(donation, MESSAGE_ARGUMENT_NUMBER);
    UiText_DrawResourceFar(serviceOrMessage, target, 0, 0);
  }
}

/// Steps one sparkle of the sanctum's burst, its slot's callback.
/// SANCTUM_SPARKLE_START places it at a random point 40 pixels from the burst's
/// centre and aims it at a random point 4 pixels from it;
/// SANCTUM_SPARKLE_FLYING waits for it to arrive and starts it again, so it
/// flies in again; SANCTUM_SPARKLE_LAST_FLIGHT, which
/// Shop_RunPartyMemberIconBurst sets as the burst ends, frees the slot on
/// arrival.
void Shop_UpdateBurstSparkle(struct EffectSlot *effect) {
  struct FixedPointPosition position;
  s8 *statePointer;
  s32 state;
  s32 targetPending;

  statePointer = &effect->state;
  state = *statePointer;
  if (state == SANCTUM_SPARKLE_START) {
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(0x280000, Random16(), &position);
    EffectSlot_SetPositionFar(effect, position.x, position.z);
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    // 16.16 pixels: at most 2 a frame, gaining 0.4 a frame
    effect->speedMax = 0x20000;
    effect->acceleration = 0x6666;
    effect->turnStepLimited = state;
    *statePointer = *statePointer + 1;
  } else if (state == SANCTUM_SPARKLE_FLYING) {
    targetPending = EffectSlot_HasPendingTargetFar(effect);
    if (targetPending == 0)
      *statePointer = targetPending;
  } else if (state == SANCTUM_SPARKLE_LAST_FLIGHT) {
    if (EffectSlot_HasPendingTargetFar(effect) == 0)
      FieldEffect_ClearOwnedSlotFar(effect);
  }
}

/// The burst's main-loop task while Shop_RunPartyMemberIconBurst plays: steps
/// all 24 sparkle slots, then, from the sixth sparkle until 20 frames after
/// the tune ends (while burstMember is not -1), gives the member's icon a
/// random colour mode each frame: SpriteColorMode 0 to 6, normal, the channel
/// swaps or grey.
void Shop_ResetEffects(void) {
  struct ShopRuntime *shop = gMenuWork;
  struct EffectSlot *effect = shop->effects;
  s32 remainingEffects = 23;
  s8 burstMember;

  do {
    remainingEffects--;
    EffectSlot_UpdateFar(effect);
    effect++;
  } while (remainingEffects >= 0);
  burstMember = shop->burstMember;
  if (burstMember != -1)
    ObjectGroup_SetChildValueUnlessFifteenFar(
        shop->partyMemberIcons[burstMember], (Random16() * 7) >> 16);
}

/// The tune each service's burst plays, by enum SanctumService; the burst
/// waits for the sound driver (AudioCommand_WaitForStateByteClear) before
/// freeing its sparkles.
static const s8 sSanctumServiceCues[4] = {
    [SANCTUM_SERVICE_REVIVE] = MUS_REVIVE,
    [SANCTUM_SERVICE_CURE_POISON] = MUS_CURE,
    [SANCTUM_SERVICE_EXORCISE] = MUS_EXORCISE,
    [SANCTUM_SERVICE_REMOVE_CURSE] = MUS_REMOVE_CURSE,
};

/// Plays the service's burst on party member `member` (an index into the
/// party row): the service's tune, the field's colours tinted, 18 sparkles
/// in random colour modes flying in from a ring around the member's icon
/// three frames apart, the icon cycling through colour modes, then, once the
/// sound driver is done, the sparkles freed and the colours faded back. The
/// hand cursor is hidden throughout. Called by Sanctum_RunPartyService once
/// the member is cured.
void Shop_RunPartyMemberIconBurst(s32 member) {

  struct ShopRuntime *shop;
  s8 savedCursorMode;
  struct FixedPointPosition position;
  struct EffectSlot *effect;
  s32 callbackPriority;
  s32 effectIndex;

  shop = gMenuWork;
  savedCursorMode = shop->cursor.anchor->active;
  shop->burstMember = -1;
  shop->cursor.anchor->active = UI_SPRITE_HIDDEN;
  AudioCommand_PlayFar(sSanctumServiceCues[shop->partyAction]);
  Shop_SaveAndTintFieldPalette(0x00202108);
  AnimationObject_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);
  callbackPriority = SCHEDULER_KEY_MAIN_LOOP;
  Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callbackPriority);

  // The burst's centre, in 16.16 pixels: 12 above the member's icon
  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) + 0xfff40000;

  // Spawn eighteen particles while the task continues updating all twenty-four.
  effectIndex = 0;
  effect = shop->effects;
  do {
    EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
    EffectSlot_SetCallbackFar(effect, Shop_UpdateBurstSparkle);
    EffectSlot_SetObjectModeFar(effect, 7);
    ObjectGroup_SetChildValueUnlessFifteenFar(effect->object,
                                              (Random16() * 7) >> 16);
    effect->scaleY = 0xb333;
    effect->scaleX = 0xb333;
    WaitFrames(3);
    if (effectIndex == 5) {
      shop->burstMember = member;
    }
    effectIndex++;
    effect++;
  } while (effectIndex <= 17);

  // Let active particles finish, then release remaining slots and restore UI.
  AudioCommand_WaitForStateByteClear();
  {
    u8 activeMode = SANCTUM_SPARKLE_LAST_FLIGHT;
    u8 *entry = (u8 *)&shop->effects[0].state;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      if (*(s8 *)(entry + 5) != 0) {
        *entry = activeMode;
      }
      entry += sizeof(struct EffectSlot);
    }
  }

  WaitFrames(20);
  AudioCommand_PlayFar(SE_RECOVERY);
  shop->burstMember = -1;
  ObjectGroup_SetChildValueUnlessFifteenFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);

  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct EffectSlot *effectSlot = shop->effects;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += 0x48;
      if (flag != 0) {
        FieldEffect_ClearOwnedSlotFar(effectSlot);
      }
      effectSlot++;
    }
  }

  Scheduler_RemoveCallback(Shop_ResetEffects);
  AnimationObject_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 16);
  Shop_RestoreFieldPalette();
  WaitFrames(30);
  shop->cursor.anchor->active = savedCursorMode;
}
