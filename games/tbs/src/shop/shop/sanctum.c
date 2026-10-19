// Part of shop.c, the shop object, compiled only through it.
// The sanctum's services, their prices and their sparkles.

#include "../shop_internal.h"

// Calculates the existing fee for reviving, curing poison, exorcising or
// removing a curse.
s32 Shop_ServicePrice(s32 unitId, s32 serviceKind) {
  u8 level = UnitGetFar(unitId)->level;
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

// Checks whether a member needs the selected sanctum service.
s32 Shop_CanServe(s32 unitId, s32 serviceKind) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 eligible = 0;

  if ((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0) ||
      (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0) ||
      (serviceKind == SANCTUM_SERVICE_EXORCISE && unit->haunt != 0) ||
      (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->restraint != 0)) {
    eligible = 1;
  }
  return eligible;
}

// Counts party members who need the currently selected sanctum service.
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

// Selects the matching poison, exorcism or curse-service message from a revive
// message base.
s32 Shop_MsgByMode(s32 messageId) {
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

// Resolves service dialogue and waits for completion, including the final
// frame.
void UiMessage_ShowResolvedAndWait(s32 messageId) {
  s32 portraitResource;

  portraitResource = BattleFx_GetResourceIdFar(
      ((struct ShopRuntime *)gMenuWork)->keeperResource);
  UiWork_FinalizePendingCoreFar();
  messageId = Shop_MsgByMode(messageId);
  UiText_OpenMessageWindowFar(messageId, 5, 0,
                              (portraitResource << 0x10) | 0x22);
  while (UiWork_IsCompleteFar() == 0) {
    WaitFrames(1U);
  }
  WaitFrames(1U);
}

// Resolves service dialogue, waits for it and restores the cursor's saved mode.
void UiMessage_ShowResolvedAndRestoreState(s32 messageId) {
  struct ShopRuntime *shop;
  struct RenderOutput **cursorSlot;
  s32 portraitResource;
  u8 savedCursorMode;

  shop = gMenuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->active;
  portraitResource = BattleFx_GetResourceIdFar(shop->keeperResource);
  messageId = Shop_MsgByMode(messageId);
  (*cursorSlot)->active = 13;
  UiWork_FinalizePendingCoreFar();
  UiText_OpenMessageWindowFar(messageId, 5, 0, (portraitResource << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
  shop->cursor.anchor->active = savedCursorMode;
}

// Runs the sanctum service menu using the shared shop cursor and window
// resources.
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
                                             Object_GetByIdFar(unitId))
                  ->animation)
            ->entries[0]
            ->animId;
    shop->keeperResource = keeperAnimationId;
  }

  listWindow = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(-5, 0, 5, 5, 2);
  }
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(0, 0, 5, 5, 2);
    SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
  }

  cursorAnchor =
      RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, listWindow, 0, 0);
  cursorAnchor->active = 1;
  cursorAnchor->kind = 0;
  ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  UiMessage_ShowResolvedAndWait(MsgSanctumWelcome);

  shop->moneyWindow = UiWindow_CreateFar(16, 11, SHOP_MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();

  for (;;) {
    serviceSelection = Menu_SelectEntry19To1cFar(serviceSelection);
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
  Inn_Cleanup();
  return 0;
}

// Selects an eligible member, confirms the donation and applies the chosen
// service.
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

  listWindow = UiWindow_CreateFar(1, 12, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = redraw;
  PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);
  priceWindow = UiWindow_CreateFar(SANCTUM_PRICE_WINDOW_X, 16,
                                   SANCTUM_PRICE_WINDOW_WIDTH, 3, 2);

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
      Shop_DrawSelectMsg(priceWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      WaitFrames(1);
      donation = Shop_ServicePrice(unitId, serviceKind);
      if (Shop_CanServe(unitId, serviceKind) == 0) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      UiWork_PushValueSlotFar(unitId, 1);
      UiWork_PushValueSlotFar(donation, 5);
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
      UiWork_PushValueSlotFar(unitId, 1);
      UiMessage_ShowResolvedAndWait(messageBase + 3);
      UiWork_FinalizePendingCoreFar();
      // Apply the cure and animation before collecting the donation.
      BattleUnit_ResetStateByMode(unitId, serviceKind);
      Shop_RunPartyMemberIconBurst(memberIndex);
      Party_AdjustCoinsFar(-donation);
      Shop_DrawMoney();
      UiWork_PushValueSlotFar(unitId, 1);
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

// Revives, cures poison, removes a haunting or unequips cursed items for a
// sanctum service.
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind) {
  struct BattleUnit *unit;

  unit = UnitGetFar(unitId);
  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    unit->hp = unit->maxHp;
    UnitRatioFar(unitId);
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
          UnitCalcFar(unitId);
        }
      }
    }
  }
}

// Highlights the selected party icon and dims members who do not need the
// sanctum service.
void Shop_HiliteUnit(struct UiWindow *window, s32 selected) {
  struct ShopRuntime *shop = gMenuWork;
  s32 memberIndex;
  s32 serviceKind = shop->partyAction;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            30);
      else
        AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[memberIndex],
                                            1);
      shop->partyMemberScale[memberIndex] = 0x10000;
      if (!Shop_CanServe(shop->partyMemberIds[memberIndex], serviceKind))
        shop->partyMemberScale[memberIndex] = 0xb333;
    }
  }
}

// Shows the selected member's service price or the message that no treatment is
// needed.
void Shop_DrawSelectMsg(struct UiWindow *target, s32 unitId) {
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
    serviceOrMessage = Shop_MsgByMode(serviceOrMessage);
    UiWork_PushValueSlotFar(donation, 5);
    UiText_DrawResourceFar(serviceOrMessage, target, 0, 0);
  }
}

// Picks random particle positions, moves toward the target and releases
// finished particles.
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect) {
  struct FixedPointPosition position;
  s8 *statePointer;
  s32 state;
  s32 targetPending;

  statePointer = &effect->state;
  state = *statePointer;
  if (state == 0) {
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(0x280000, Random16(), &position);
    EffectSlot_SetPositionFar(effect, position.x, position.z);
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->maxSpeed = 0x20000;
    effect->acceleration = 0x6666;
    effect->limitTurnStep = state;
    *statePointer = *statePointer + 1;
  } else if (state == 1) {
    targetPending = EffectSlot_HasPendingTargetFar(effect);
    if (targetPending == 0)
      *statePointer = targetPending;
  } else if (state == 2) {
    if (EffectSlot_HasPendingTargetFar(effect) == 0)
      BattleFx_ClearOwnedSlotFar(effect);
  }
}

// Updates all 24 service particles, then randomizes the active member's pose.
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

// Sound effects for each party member action
static const s8 sSanctumServiceCues[4] = {
    0x52,
    0x54,
    0x58,
    0x57,
};

// Animates the selected member and service particles, then restores the menu
// and cursor.
void Shop_RunPartyMemberIconBurst(s32 member) {

  struct ShopRuntime *shop;
  s8 savedCursorMode;
  struct FixedPointPosition position;
  struct EffectSlot *effect;
  s32 callbackPriority;
  s32 effectIndex;

  shop = gMenuWork;
  savedCursorMode = shop->cursor.anchor->active;
  shop->burstMember = 0xff;
  shop->cursor.anchor->active = 13;
  AudioCommand_PlayFar(sSanctumServiceCues[shop->partyAction]);
  Shop_RestoreSceneTiles(0x00202108);
  AnimationObjects_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);
  callbackPriority = 0xc80;
  Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callbackPriority);

  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) + 0xfff40000;

  // Spawn eighteen particles while the task continues updating all twenty-four.
  effectIndex = 0;
  effect = shop->effects;
  do {
    EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
    EffectSlot_SetCallbackFar(effect, BattleFx_UpdateRadialMotion);
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
    u8 activeMode = 2;
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
        BattleFx_ClearOwnedSlotFar(effectSlot);
      }
      effectSlot++;
    }
  }

  Scheduler_RemoveCallback(Shop_ResetEffects);
  AnimationObjects_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 16);
  Shop_InitEffect();
  WaitFrames(30);
  shop->cursor.anchor->active = savedCursorMode;
}
