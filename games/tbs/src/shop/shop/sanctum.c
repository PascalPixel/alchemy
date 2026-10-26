/// @file
/// The sanctum's services, their prices and their sparkles.
// Part of shop.c.

#include "../shop_internal.h"

/// Returns the sanctum's fee, in coins, for `serviceKind` on member `unitId`:
/// 20 a level to revive, 10 to cure poison, 50 to exorcise and 10 a level to
/// remove a curse. Called by the member choice and its fee line.
s32 Sanctum_ServicePrice(s32 unitId, s32 serviceKind) {
  u8 level = Unit_GetFar(unitId)->level;
  s32 donation = 0;

  if (serviceKind == SANCTUM_SERVICE_REVIVE) {
    donation = level * 20;
  } else if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    donation = 10;
  } else if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
    donation = 50;
  } else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    donation = level * 10;
  }
  return donation;
}

/// Returns 1 when member `unitId` needs `serviceKind`: down (HP 0) for a
/// revive, poisoned for a cure, haunted for an exorcism, any curse bit set
/// for removing a curse. Called by the member choice, its count, its highlight
/// and its fee line. Unit_Calculate sets the curse bits only for a cursed item
/// worn, and nothing sets CURSE_KEEP_BOUND, so a member accepted for removing
/// a curse always has a cursed item to take off.
s32 Sanctum_NeedsService(s32 unitId, s32 serviceKind) {
  struct Unit *unit = Unit_GetFar(unitId);
  s32 eligible = 0;

  if ((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0) ||
      (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0) ||
      (serviceKind == SANCTUM_SERVICE_REPEL_EVIL && unit->haunt != 0) ||
      (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->curse != 0)) {
    eligible = 1;
  }
  return eligible;
}

/// Counts party members who need the service in shop->action. Called by
/// Sanctum_Run, which says nobody needs it when the count is 0, and by
/// Sanctum_RunPartyService, which offers another member while any remain.
s32 Sanctum_CountMembersInNeed(void) {
  struct ShopWork *shop = gMenuWork;
  s32 eligibleCount = 0;
  s32 serviceKind = shop->action;
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Sanctum_NeedsService(shop->partyMemberIds[memberIndex], serviceKind))
      eligibleCount++;
  }
  return eligibleCount;
}

/// Returns the line of the service in shop->action that stands where
/// `messageId` stands among the revive service's lines. Each service's lines
/// are a run in the same order, from MsgReviveService, MsgCurePoisonService,
/// MsgExorcismService and MsgRemoveCurseService.
s32 Shop_MessageByMode(s32 messageId) {
  s8 serviceKind = ((struct ShopWork *)gMenuWork)->action;

  if (serviceKind == SANCTUM_SERVICE_CURE_POISON) {
    messageId += MsgCurePoisonService - MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
    messageId += MsgExorcismService - MsgReviveService;
  }
  if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
    messageId += MsgRemoveCurseService - MsgReviveService;
  }
  return messageId;
}

/// Shows the keeper's line `messageId`, moved to the current service's
/// (Shop_MessageByMode), and waits for its window, one more frame included: the
/// sanctum's Shop_ShowKeeperMessage.
void Sanctum_ShowKeeperMessage(s32 messageId) {
  s32 portraitResource;

  portraitResource =
      Speaker_GetVoiceFar(((struct ShopWork *)gMenuWork)->keeperGraphics);
  TextPrinter_CloseFinishedWindowsFar();
  messageId = Shop_MessageByMode(messageId);
  Message_OpenWindowFar(messageId, 5, 0, (portraitResource << 0x10) | 0x22);
  while (TextPrinter_AllFinishedFar() == 0) {
    WaitFrames(1U);
  }
  WaitFrames(1U);
}

/// As Sanctum_ShowKeeperMessage, hiding the hand cursor while the line
/// shows and putting back its motion after: the sanctum's
/// Shop_ShowKeeperMessageHidingCursor.
void Sanctum_ShowKeeperMessageHidingCursor(s32 messageId) {
  struct ShopWork *shop;
  struct WindowSprite **cursorSlot;
  s32 portraitResource;
  u8 savedCursorMode;

  shop = gMenuWork;
  cursorSlot = &shop->cursor.anchor;
  savedCursorMode = (*cursorSlot)->motion;
  portraitResource = Speaker_GetVoiceFar(shop->keeperGraphics);
  messageId = Shop_MessageByMode(messageId);
  (*cursorSlot)->motion = 13;
  TextPrinter_CloseFinishedWindowsFar();
  Message_OpenWindowFar(messageId, 5, 0, (portraitResource << 16) | 0x22);
  while (TextPrinter_AllFinishedFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
  shop->cursor.anchor->motion = savedCursorMode;
}

/// Runs a sanctum: the keeper's welcome, then the service menu (revive, cure
/// poison, exorcise, remove a curse) until B, and the farewell. `unitId` is
/// the keeper's object, whose portrait the lines show. Holds the shop work
/// block while it runs; returns 0. Called by the
/// maps' sanctum keepers and the Battle Arena, and by the debug test.
s32 Sanctum_Run(s32 unitId) {
  s32 serviceSelection = 0;
  struct Window *listWindow = 0;
  struct ShopWork *shop;
  struct WindowSprite *cursorAnchor;

  Shop_InitializeWork();
  shop = gMenuWork;
  shop->action = serviceSelection;

  {
    s32 keeperAnimationId = (u16)((struct Object *)Actor_GetByIdFar(unitId))
                                ->sprite->entries[0]
                                ->animationId;
    shop->keeperGraphics = keeperAnimationId;
  }

  // The keeper's portrait window; failing that an empty one off the left
  // edge, and failing that one on screen with side object 2
  listWindow = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = Window_CreateFar(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
  }
  if (listWindow == 0) {
    listWindow = Window_CreateFar(0, 0, 5, 5, WINDOW_FLAG_INSTANT);
    Portrait_CreateFar(2, 0, 0, listWindow, -4, -4);
  }

  cursorAnchor = WindowSprite_CreateFar(shop->cursorIcon, OUTPUT_SIZE_16X16,
                                        listWindow, 0, 0);
  cursorAnchor->motion = 1;
  cursorAnchor->kind = 0;
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  Sanctum_ShowKeeperMessage(MsgSanctumWelcome);

  shop->moneyWindow =
      Window_CreateFar(16, 11, SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();

  for (;;) {
    serviceSelection = Menu_SelectSanctumServiceFar(serviceSelection);
    shop->action = serviceSelection;
    if (serviceSelection == -1)
      break;

    {
      s32 messageBase = MsgReviveService;
      s32 message = messageBase;

      messageBase = 0;
      Sanctum_ShowKeeperMessage(message);
      if (Sanctum_CountMembersInNeed() == 0) {
        Sanctum_ShowKeeperMessage(message + 1);
      } else {
        Sanctum_RunPartyService();
      }
    }
    shop->action = 0;
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    Sanctum_ShowKeeperMessage(MsgSanctumMoreAid);
  }

  Sanctum_ShowKeeperMessage(MsgSanctumFarewell);
  Window_CloseFar(shop->moneyWindow, 2);
  Window_CloseFar(listWindow, 2);
  Shop_ReleaseWork();
  return 0;
}

/// Lets the player pick a member for the service in shop->action,
/// starting on the first who needs it, asks for the donation, then applies
/// the service with its burst and takes the coins; offers another member
/// while any still need it, until B. A member who needs nothing can be
/// pointed at, but A only buzzes. Called by Sanctum_Run; returns 0.
s32 Sanctum_RunPartyService(void) {
  struct ShopWork *shop = gMenuWork;
  struct Window *priceWindow;
  s32 redraw;
  s32 serviceKind;
  struct Window *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  s32 donation;
  s32 messageBase;

  priceWindow = 0;
  redraw = 1;
  serviceKind = shop->action;
  Sanctum_ShowKeeperMessage(MsgWhoToRevive);

  listWindow = Window_CreateFar(1, 12, 13, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  // redraw is 1 here: the hand's first move to a member takes one frame
  shop->mode = redraw;
  PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);
  priceWindow =
      Window_CreateFar(SANCTUM_PRICE_WINDOW_X, 16, SANCTUM_PRICE_WINDOW_WIDTH,
                       3, WINDOW_FLAG_INSTANT);

  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  while (memberIndex < shop->partyMemberCount) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Sanctum_NeedsService(unitId, serviceKind) != 0)
      break;
    memberIndex++;
  }

  redraw = 1;
  for (;;) {
    if (chooseAgain != 0) {
      chooseAgain = 0;
      Sanctum_ShowKeeperMessage(MsgWhoToRevive);
      redraw = 1;
      memberIndex = 0;
      while (memberIndex < shop->partyMemberCount) {
        unitId = shop->partyMemberIds[memberIndex];
        if (Sanctum_NeedsService(unitId, serviceKind) != 0)
          break;
        memberIndex++;
      }
    }

    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(listWindow, memberIndex * 24 - 12, 0);
      shop->mode = 3;
      Sanctum_HighlightMembers(listWindow, memberIndex);
      Shop_DrawSelectMessage(priceWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      WaitFrames(1);
      donation = Sanctum_ServicePrice(unitId, serviceKind);
      if (Sanctum_NeedsService(unitId, serviceKind) == 0) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        continue;
      }
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(donation, MESSAGE_ARGUMENT_NUMBER);
      messageBase = MsgReviveDonation;
      Sanctum_ShowKeeperMessage(messageBase);
      if (Sanctum_AskYesNo(0) != 0) {
        Sanctum_ShowKeeperMessageHidingCursor(messageBase + 2);
        chooseAgain = 1;
        continue;
      }
      if (donation > (u32)gSaveBlock.coins) {
        Sound_PlaySongFar(SE_MENU_CANCEL);
        Sanctum_ShowKeeperMessageHidingCursor(messageBase + 1);
        chooseAgain = 1;
        continue;
      }
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      Sanctum_ShowKeeperMessage(messageBase + 3);
      TextPrinter_CloseFinishedWindowsFar();
      // Apply the cure and animation before collecting the donation.
      BattleUnit_ResetStateByMode(unitId, serviceKind);
      Sanctum_RunMemberIconBurst(memberIndex);
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
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex -= 1;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex += 1;
    }
    WaitFrames(1);
    continue;
  }

  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(listWindow, 2);
  WaitFrames(1);
  return 0;
}

/// Applies sanctum service `serviceKind` to member `unitId`: full HP for a
/// revive, poison or the haunting cleared, or every cursed item taken off
/// (Unit_Calculate then rebuilds the curse bits from what is still equipped).
/// With no cursed item equipped, removing a curse changes nothing, whatever
/// curse bits Sanctum_NeedsService saw. Called by Sanctum_RunPartyService.
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind) {
  struct Unit *unit;

  unit = Unit_GetFar(unitId);
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

/// Shows the chosen member walking and the others standing, and draws at
/// seven tenths of their size (0xb333 in 16.16) the members who do not need
/// the service. The sanctum's Shop_DrawParty.
void Sanctum_HighlightMembers(struct Window *window, s32 selected) {
  struct ShopWork *shop = gMenuWork;
  s32 memberIndex;
  s32 serviceKind = shop->action;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        Sprite_StartAnimFar(shop->partyMemberIcons[memberIndex], 30);
      else
        Sprite_StartAnimFar(shop->partyMemberIcons[memberIndex], 1);
      shop->partyMemberScale[memberIndex] = Q16_ONE;
      if (!Sanctum_NeedsService(shop->partyMemberIds[memberIndex], serviceKind))
        shop->partyMemberScale[memberIndex] = 0xb333;
    }
  }
}

/// Draws in `target` the member's fee for the current service (its cost line
/// with the donation filled in), or the line that the member needs no
/// treatment. Called by Sanctum_RunPartyService each time the cursor moves.
void Shop_DrawSelectMessage(struct Window *target, s32 unitId) {
  s32 serviceOrMessage;
  s32 donation;

  serviceOrMessage = ((struct ShopWork *)gMenuWork)->action;
  donation = Sanctum_ServicePrice(unitId, serviceOrMessage);
  if (target != 0) {
    WindowSprite_PrepareForRedrawFar(target);
    if (Sanctum_NeedsService(unitId, serviceOrMessage) != 0) {
      serviceOrMessage = MsgReviveCost;
    } else {
      serviceOrMessage = MsgNoHealingNeeded;
    }
    serviceOrMessage = Shop_MessageByMode(serviceOrMessage);
    MessageArgument_PushFar(donation, MESSAGE_ARGUMENT_NUMBER);
    Text_DrawMessageFar(serviceOrMessage, target, 0, 0);
  }
}

/// Steps one sparkle of the sanctum's burst, its slot's callback.
/// SANCTUM_SPARKLE_START places it at a random point 40 pixels from the burst's
/// centre and aims it at a random point 4 pixels from it;
/// SANCTUM_SPARKLE_FLYING waits for it to arrive and starts it again, so it
/// flies in again; SANCTUM_SPARKLE_LAST_FLIGHT, which
/// Sanctum_RunMemberIconBurst sets as the burst ends, frees the slot on
/// arrival.
void Sanctum_UpdateBurstSparkle(struct FieldEffectSprite *effect) {
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
    FieldEffectSprite_SetPositionFar(effect, position.x, position.z);
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
    targetPending = FieldEffectSprite_HasPendingTargetFar(effect);
    if (targetPending == 0)
      *statePointer = targetPending;
  } else if (state == SANCTUM_SPARKLE_LAST_FLIGHT) {
    if (FieldEffectSprite_HasPendingTargetFar(effect) == 0)
      FieldEffect_ClearOwnedSlotFar(effect);
  }
}

/// The burst's main-loop task while Sanctum_RunMemberIconBurst plays: steps
/// all 24 sparkle slots, then, from the sixth sparkle until 20 frames after
/// the tune ends (while burstMember is not -1), gives the member's icon a
/// random colour mode each frame: SpriteColorMode 0 to 6, normal, the channel
/// swaps or grey.
void Shop_ResetEffects(void) {
  struct ShopWork *shop = gMenuWork;
  struct FieldEffectSprite *effect = shop->effects;
  s32 remainingEffects = 23;
  s8 burstMember;

  do {
    remainingEffects--;
    FieldEffectSprite_UpdateFar(effect);
    effect++;
  } while (remainingEffects >= 0);
  burstMember = shop->burstMember;
  if (burstMember != -1)
    Sprite_SetAnimColorModesFar(shop->partyMemberIcons[burstMember],
                                (Random16() * 7) >> 16);
}

/// The tune each service's burst plays, by enum SanctumService; the burst
/// waits for the sound driver (Sound_WaitForFanfare) before
/// freeing its sparkles.
static const s8 sSanctumServiceCues[4] = {
    [SANCTUM_SERVICE_REVIVE] = MUS_REVIVE,
    [SANCTUM_SERVICE_CURE_POISON] = MUS_CURE,
    [SANCTUM_SERVICE_REPEL_EVIL] = MUS_REPEL_EVIL,
    [SANCTUM_SERVICE_REMOVE_CURSE] = MUS_REMOVE_CURSE,
};

/// Plays the service's burst on party member `member` (an index into the
/// party row): the service's tune, the field's colours tinted, 18 sparkles
/// in random colour modes flying in from a ring around the member's icon
/// three frames apart, the icon cycling through colour modes, then, once the
/// sound driver is done, the sparkles freed and the colours faded back. The
/// hand cursor is hidden throughout. Called by Sanctum_RunPartyService once
/// the member is cured.
void Sanctum_RunMemberIconBurst(s32 member) {

  struct ShopWork *shop;
  s8 savedCursorMode;
  struct FixedPointPosition position;
  struct FieldEffectSprite *effect;
  s32 callbackPriority;
  s32 effectIndex;

  shop = gMenuWork;
  savedCursorMode = shop->cursor.anchor->motion;
  shop->burstMember = -1;
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Sound_PlaySongFar(sSanctumServiceCues[shop->action]);
  Shop_SaveAndTintFieldPalette(0x00202108);
  Sprite_SetAnimSpeedFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);
  callbackPriority = TASK_PRIORITY_MAIN_LOOP;
  Task_Add(Shop_ResetEffects, callbackPriority);

  // The burst's centre, in 16.16 pixels: 12 above the member's icon
  position.x = shop->partyMemberX[member] << 16;
  position.z = (shop->partyMemberY[member] << 16) + 0xfff40000;

  // Spawn eighteen particles while the task continues updating all twenty-four.
  effectIndex = 0;
  effect = shop->effects;
  do {
    FieldEffectSprite_InitializeFar(effect, 0x11c, position.x, position.z);
    FieldEffectSprite_SetCallbackFar(effect, Sanctum_UpdateBurstSparkle);
    FieldEffectSprite_SetObjectModeFar(effect, 7);
    Sprite_SetAnimColorModesFar(effect->object, (Random16() * 7) >> 16);
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
  Sound_WaitForFanfare();
  {
    u8 activeMode = SANCTUM_SPARKLE_LAST_FLIGHT;
    u8 *entry = (u8 *)&shop->effects[0].state;
    for (effectIndex = 23; effectIndex >= 0; effectIndex--) {
      if (*(s8 *)(entry + 5) != 0) {
        *entry = activeMode;
      }
      entry += sizeof(struct FieldEffectSprite);
    }
  }

  WaitFrames(20);
  Sound_PlaySongFar(SE_RECOVERY);
  shop->burstMember = -1;
  Sprite_SetAnimColorModesFar(shop->partyMemberIcons[member], 0);
  WaitFrames(20);

  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct FieldEffectSprite *effectSlot = shop->effects;
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

  Task_Remove(Shop_ResetEffects);
  Sprite_SetAnimSpeedFar(shop->partyMemberIcons[member], 16);
  Shop_RestoreFieldPalette();
  WaitFrames(30);
  shop->cursor.anchor->motion = savedCursorMode;
}
