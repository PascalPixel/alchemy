/// @file
/// The sanctum's services, their prices and their sparkles.
// Part of shop.c.

#include "shop_internal.h"

/// The donations the sanctum asks, in coins: reviving and removing a curse ask
/// so many for each of the party member's levels.
enum SanctumDonation {
  DONATION_REVIVE_PER_LEVEL = 20,
  DONATION_CURE_POISON = 10,
  DONATION_REPEL_EVIL = 50,
  DONATION_REMOVE_CURSE_PER_LEVEL = 10
};

/// Returns the sanctum's fee, in coins, for `serviceKind` on party member
/// `unitId`.
s32 Sanctum_ServicePrice(s32 unitId, s32 serviceKind) {
  u8 level = Unit_GetFar(unitId)->level;
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

/// Returns 1 when a party member needs the service: down for Revive, poisoned
/// for Cure Poison, haunted for Repel Evil, cursed for Remove Curse.
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

/// Counts party members who need the service in shop->action.
s32 Sanctum_CountMembersInNeed(void) {
  struct ShopState *shop = gMenuState;
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
  s8 serviceKind = ((struct ShopState *)gMenuState)->action;

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

/// Shows the healer's line and waits until it is typed out. A service's line is
/// given as Revive's, and the chosen service says its own; the sanctum's other
/// lines are shown while shop->action is Revive.
void Sanctum_ShowKeeperMessage(s32 messageId) {
  s32 voice;

  voice = Speaker_GetVoiceFar(((struct ShopState *)gMenuState)->keeperGraphics);
  Message_CloseFinishedWindowsFar();
  messageId = Sanctum_GetServiceMessage(messageId);
  Message_OpenWindowFar(messageId, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0) {
    wait(1);
  }
  wait(1);
}

/// As Sanctum_ShowKeeperMessage, hiding the hand cursor while the line shows.
void Sanctum_ShowKeeperMessageHidingCursor(s32 messageId) {
  struct ShopState *shop = gMenuState;
  u8 savedCursorMode = shop->cursor.anchor->motion;
  s32 voice = Speaker_GetVoiceFar(shop->keeperGraphics);

  messageId = Sanctum_GetServiceMessage(messageId);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(messageId, 5, 0,
                        MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X |
                            MESSAGE_WINDOW_UNREAD);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
  shop->cursor.anchor->motion = savedCursorMode;
}

/// Runs a sanctum: the healer's welcome, the service menu until B, and the
/// farewell; keeperId is the healer's actor, whose portrait the lines show.
s32 Sanctum_Run(s32 keeperId) {
  s32 serviceSelection = 0;
  struct Window *listWindow = 0;
  struct ShopState *shop;
  struct WindowSprite *cursorAnchor;

  Shop_InitializeState();
  shop = gMenuState;
  shop->action = serviceSelection;

  {
    s32 keeperAnimationId =
        (u16)(Actor_GetByIdFar(keeperId))->sprite->entries[0]->animationId;
    shop->keeperGraphics = keeperAnimationId;
  }

  // The healer's portrait window; failing that an empty one off the left
  // edge, and failing that one on screen with Ivan's portrait
  listWindow = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = Window_CreateFar(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
  }
  if (listWindow == 0) {
    listWindow = Window_CreateFar(0, 0, 5, 5, WINDOW_FLAG_INSTANT);
    Portrait_CreateFar(IVAN, 0, 0, listWindow, -4, -4);
  }

  cursorAnchor = WindowSprite_CreateFar(
      shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, listWindow, 0, 0);
  cursorAnchor->motion = WINDOW_SPRITE_MOTION_STILL;
  cursorAnchor->kind = WINDOW_SPRITE_PLAIN; // the icon is the shop state's
  MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
  shop->cursor.anchor = cursorAnchor;
  // "Welcome, weary wanderers. What aid do you seek?"
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
      s32 message = MsgReviveService;

      // Revive's words: "Hmm, you were downed in battle and need reviving, do
      // you?"
      Sanctum_ShowKeeperMessage(message);
      if (Sanctum_CountMembersInNeed() == 0) {
        // "Fear not, none of your companions are down."
        Sanctum_ShowKeeperMessage(message + 1);
      } else {
        Sanctum_RunPartyService();
      }
    }
    shop->action = SANCTUM_SERVICE_REVIVE;
    MenuCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    // "Do you wish for more aid?"
    Sanctum_ShowKeeperMessage(MsgSanctumMoreAid);
  }

  // "Visit us again anytime you need healing."
  Sanctum_ShowKeeperMessage(MsgSanctumFarewell);
  Window_CloseFar(shop->moneyWindow, 2);
  Window_CloseFar(listWindow, 2);
  Shop_ReleaseState();
  return 0;
}

/// Lets the player pick a party member for the service, asks for the donation,
/// then applies it with its burst; offers another while any still need it.
s32 Sanctum_RunPartyService(void) {
  struct ShopState *shop = gMenuState;
  struct Window *priceWindow;
  s32 redraw;
  s32 serviceKind;
  struct Window *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  u32 donation;
  s32 messageBase;

  priceWindow = 0;
  redraw = 1;
  serviceKind = shop->action;
  // Revive's words, as below: "Who shall I revive?"
  Sanctum_ShowKeeperMessage(MsgWhoToRevive);

  listWindow = Window_CreateFar(1, 12, 13, 3, WINDOW_FLAG_INSTANT);
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  // redraw is 1 here: the hand's first move to a party member takes one frame
  shop->cursorFrames = redraw;
  Menu_CreatePartyFiguresFar(listWindow, 2, 0, 8, 0);
  priceWindow =
      Window_CreateFar(SANCTUM_PRICE_WINDOW_X, 16, SANCTUM_PRICE_WINDOW_WIDTH,
                       3, WINDOW_FLAG_INSTANT);

  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  for (; memberIndex < shop->partyMemberCount; memberIndex++) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Sanctum_NeedsService(unitId, serviceKind) != 0)
      break;
  }

  redraw = 1;
  for (;;) {
    if (chooseAgain != 0) {
      chooseAgain = 0;
      Sanctum_ShowKeeperMessage(MsgWhoToRevive);
      redraw = 1;
      for (memberIndex = 0; memberIndex < shop->partyMemberCount;
           memberIndex++) {
        unitId = shop->partyMemberIds[memberIndex];
        if (Sanctum_NeedsService(unitId, serviceKind) != 0)
          break;
      }
    }

    if (redraw != 0) {
      redraw = 0;
      memberIndex =
          (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(listWindow, memberIndex * 24 - 12, 0);
      shop->cursorFrames = 3;
      Sanctum_HighlightMembers(listWindow, memberIndex);
      Sanctum_DrawFee(priceWindow, unitId);
    }

    if ((gKeysPressed & KEY_A) != 0) {
      wait(1);
      donation = Sanctum_ServicePrice(unitId, serviceKind);
      if (Sanctum_NeedsService(unitId, serviceKind) == 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(donation, MESSAGE_ARGUMENT_NUMBER);
      messageBase = MsgReviveDonation;
      // "Reviving (the member) requires a donation of (fee) coins. OK?"
      Sanctum_ShowKeeperMessage(messageBase);
      if (Sanctum_AskYesNo(0) != 0) {
        // "The donation is too much? Then weary wanderers you shall remain."
        Sanctum_ShowKeeperMessageHidingCursor(messageBase + 2);
        chooseAgain = 1;
        continue;
      }
      if (donation > gMainRoutineState.holdings.coins) {
        Sound_PlayFar(SE_MENU_CANCEL);
        // "I'm sorry, but you must be able to pay the donation."
        Sanctum_ShowKeeperMessageHidingCursor(messageBase + 1);
        chooseAgain = 1;
        continue;
      }
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      // "Then I call upon my healing powers."
      Sanctum_ShowKeeperMessage(messageBase + 3);
      Message_CloseFinishedWindowsFar();
      Sanctum_ApplyService(unitId, serviceKind);
      Sanctum_RunMemberIconBurst(memberIndex);
      Party_AdjustCoinsFar(-donation);
      Shop_DrawMoney();
      MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
      // "(The member) has been revived!"
      Sanctum_ShowKeeperMessage(messageBase + 4);
      if (Sanctum_CountMembersInNeed() != 0) {
        chooseAgain = 1;
        continue;
      }
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      Sound_PlayFar(SE_MENU_CANCEL);
      break;
    }
    if ((gKeysRepeat & KEY_LEFT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex--;
    }
    if ((gKeysRepeat & KEY_RIGHT) != 0) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      redraw = 1;
      memberIndex++;
    }
    wait(1);
    continue;
  }

  Menu_ReleasePartyFiguresFar();
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(listWindow, 2);
  wait(1);
  return 0;
}

/// Applies a sanctum service to a party member: full HP for a revive, poison or
/// the haunting cleared, or every cursed item taken off.
void Sanctum_ApplyService(s32 unitId, s32 serviceKind) {
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

/// Shows the chosen party member running in place, and those who do not need
/// the service drawn smaller.
void Sanctum_HighlightMembers(struct Window *window, s32 selected) {
  struct ShopState *shop = gMenuState;
  s32 memberIndex;
  s32 serviceKind = shop->action;

  if (window != 0) {
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
      if (memberIndex == selected)
        Sprite_StartAnimationFar(shop->partyMemberIcons[memberIndex],
                                 ANIM_PARTY_RUN);
      else
        Sprite_StartAnimationFar(shop->partyMemberIcons[memberIndex],
                                 ANIM_STAND);
      shop->partyMemberScale[memberIndex] = FIXED_ONE;
      if (!Sanctum_NeedsService(shop->partyMemberIds[memberIndex], serviceKind))
        shop->partyMemberScale[memberIndex] = FIXED_ONE * 7 / 10;
    }
  }
}

/// Draws the party member's fee for the current service, or that they need no
/// treatment.
void Sanctum_DrawFee(struct Window *target, s32 unitId) {
  s32 serviceOrMessage;
  s32 donation;

  serviceOrMessage = ((struct ShopState *)gMenuState)->action;
  donation = Sanctum_ServicePrice(unitId, serviceOrMessage);
  if (target != 0) {
    WindowSprite_PrepareForRedrawFar(target);
    if (Sanctum_NeedsService(unitId, serviceOrMessage) != 0) {
      // "Revive for (fee) coins"
      serviceOrMessage = MsgReviveCost;
    } else {
      // "This ally needs no healing."
      serviceOrMessage = MsgNoHealingNeeded;
    }
    serviceOrMessage = Sanctum_GetServiceMessage(serviceOrMessage);
    MessageArgument_PushFar(donation, MESSAGE_ARGUMENT_NUMBER);
    Text_DrawMessageFar(serviceOrMessage, target, 0, 0);
  }
}

/// Steps one sparkle of the sanctum's burst, which flies in from a ring around
/// the icon, again and again until the burst ends.
void Sanctum_UpdateBurstSparkle(struct FieldParticle *effect) {
  struct Point position;
  s8 *statePointer;
  s32 state;
  s32 targetPending;

  statePointer = &effect->state;
  state = *statePointer;
  if (state == SANCTUM_SPARKLE_START) {
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(toFIXED(40), Random16(), &position);
    FieldParticle_SetPositionFar(effect, position.x, position.z);
    position.x = effect->originX;
    position.z = effect->originZ;
    Vector_AddPolarOffset(toFIXED(4), Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    // 16.16 pixels: at most 2 a frame, gaining 0.4 a frame
    effect->speedMax = toFIXED(2);
    effect->acceleration = toFIXED(2) / 5;
    effect->turnStepLimited = state;
    *statePointer = *statePointer + 1;
  } else if (state == SANCTUM_SPARKLE_FLYING) {
    targetPending = FieldParticle_HasPendingTargetFar(effect);
    if (targetPending == 0)
      *statePointer = targetPending;
  } else if (state == SANCTUM_SPARKLE_LAST_FLIGHT) {
    if (FieldParticle_HasPendingTargetFar(effect) == 0)
      FieldEffect_ClearOwnedSlotFar(effect);
  }
}

/// Steps the burst's sparkles, and while the tune plays flickers the party
/// member's icon through random colours, a new one each frame.
void Sanctum_UpdateBurst(void) {
  struct ShopState *shop = gMenuState;
  s32 i;
  s8 burstMember;

  for (i = 0; i < SANCTUM_PARTICLE_COUNT; i++)
    FieldParticle_UpdateFar(&shop->effects[i]);
  burstMember = shop->burstMember;
  if (burstMember != -1)
    Sprite_SetAnimationColorModesFar(shop->partyMemberIcons[burstMember],
                                     Random16_Below(7));
}

/// The tune each service's burst plays.
static const s8 sSanctumServiceCues[4] = {
    [SANCTUM_SERVICE_REVIVE] = MUS_RECOVERING_AFTER_COMBAT,
    [SANCTUM_SERVICE_CURE_POISON] = MUS_POISON_CURED,
    [SANCTUM_SERVICE_REPEL_EVIL] = MUS_EXORCISED,
    [SANCTUM_SERVICE_REMOVE_CURSE] = MUS_CURSE_LIFTED,
};

/// Plays the service's burst on a party member's icon: its tune, the field
/// greyed, and SANCTUM_BURST_SPARKLES sparkles flying in at the icon as it
/// flickers through colours, then the recovery sound.
void Sanctum_RunMemberIconBurst(s32 member) {
  struct ShopState *shop;
  s8 savedCursorMode;
  struct Point position;
  struct FieldParticle *effect;
  s32 effectIndex;

  shop = gMenuState;
  savedCursorMode = shop->cursor.anchor->motion;
  shop->burstMember = -1;
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  Sound_PlayFar(sSanctumServiceCues[shop->action]);
  Shop_SaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
  Sprite_SetAnimationSpeedFar(shop->partyMemberIcons[member], 0);
  wait(20);
  Task_Add(Sanctum_UpdateBurst, TASK_PASS_FRAME);

  // The burst's centre, in 16.16 pixels: 12 above the party member's icon
  position.x = toFIXED(shop->partyMemberX[member]);
  position.z = toFIXED(shop->partyMemberY[member]) - toFIXED(12);

  // A sparkle starts every 3 frames; the burst's task moves them.
  for (effectIndex = 0; effectIndex < SANCTUM_BURST_SPARKLES; effectIndex++) {
    effect = &shop->effects[effectIndex];
    FieldParticle_InitializeFar(effect, SPRITE_PARTICLE, position.x,
                                position.z);
    FieldParticle_SetCallbackFar(effect, Sanctum_UpdateBurstSparkle);
    FieldParticle_SetAnimationFar(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, Random16_Below(7));
    effect->scaleY = FIXED_ONE * 7 / 10;
    effect->scaleX = FIXED_ONE * 7 / 10;
    wait(3);
    // The icon starts to flicker with the sixth sparkle.
    if (effectIndex == 5) {
      shop->burstMember = member;
    }
  }

  // As the tune ends, every sparkle makes one last flight and is freed when it
  // arrives.
  Sound_WaitForJingle();
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

  // Free the sparkles still flying.
  {
    u8 *flagEntry = (u8 *)&shop->effects[0].active;
    struct FieldParticle *effectSlot = shop->effects;
    for (effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++) {
      s32 flag = *flagEntry;

      flag <<= 24;

      flagEntry += sizeof(struct FieldParticle);
      if (flag != 0) {
        FieldEffect_ClearOwnedSlotFar(effectSlot);
      }
      effectSlot++;
    }
  }

  Task_Remove(Sanctum_UpdateBurst);
  Sprite_SetAnimationSpeedFar(shop->partyMemberIcons[member], ANIM_STEP_NORMAL);
  Shop_RestoreFieldPalette();
  wait(30);
  shop->cursor.anchor->motion = savedCursorMode;
}
