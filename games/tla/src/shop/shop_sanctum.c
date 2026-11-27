// The sanctum's services, their donations and their sparkles. Part of shop.c.

// The donations the sanctum asks, in coins: reviving and removing a curse ask so many for each of
// the party member's levels.
typedef enum SANCTUM_DONATION
{
    DONATION_REVIVE_PER_LEVEL       = 20,
    DONATION_CURE_POISON            = 10,
    DONATION_REPEL_EVIL             = 50,
    DONATION_REMOVE_CURSE_PER_LEVEL = 10
} SANCTUM_DONATION;

// Returns the sanctum's donation, in coins, for `serviceKind` on party member `unitId`.
Sint32 mnSanctumServicePrice(Sint32 unitId, Sint32 serviceKind)
{
    Uint8  level    = syUnitGet(unitId)->level;
    Sint32 donation = 0;

    if (serviceKind == SANCTUM_SERVICE_REVIVE)
    {
        donation = level * DONATION_REVIVE_PER_LEVEL;
    }
    else if (serviceKind == SANCTUM_SERVICE_CURE_POISON)
    {
        donation = DONATION_CURE_POISON;
    }
    else if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL)
    {
        donation = DONATION_REPEL_EVIL;
    }
    else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE)
    {
        donation = level * DONATION_REMOVE_CURSE_PER_LEVEL;
    }
    return donation;
}

// Returns 1 when a party member needs the service: down for a revive, poisoned for a cure, haunted
// for an exorcism, cursed for removing a curse.
Sint32 mnSanctumNeedsService(Sint32 unitId, Sint32 serviceKind)
{
    UNIT *unit     = syUnitGet(unitId);
    Bool  eligible = FALSE;

    if ((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0)
        || (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0)
        || (serviceKind == SANCTUM_SERVICE_REPEL_EVIL && unit->haunt != 0)
        || (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->curse != 0))
    {
        eligible = TRUE;
    }
    return eligible;
}

// Counts party members who need the service in shop->action.
Sint32 mnSanctumCountMembersInNeed(void)
{
    SHOP_STATE *shop          = RamHeapSlots->menuState;
    Sint32      eligibleCount = 0;
    Sint32      serviceKind   = shop->action;
    Sint32      memberIndex;

    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++)
    {
        if (mnSanctumNeedsService(shop->partyMemberIds[memberIndex], serviceKind))
            eligibleCount++;
    }
    return eligibleCount;
}

// Returns the current service's line that stands where msgId stands among the revive service's
// lines.
Sint32 mnSanctumGetServiceMessage(Sint32 msgId)
{
    SHOP_STATE *shop;
    Sint8       serviceKind;

    shop        = RamHeapSlots->menuState;
    serviceKind = shop->action;
    if (serviceKind == SANCTUM_SERVICE_CURE_POISON)
    {
        msgId += (Uint32)MsgCurePoisonService - (Uint32)MsgReviveService;
    }
    if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL)
    {
        msgId += (Uint32)MsgExorcismService - (Uint32)MsgReviveService;
    }
    if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE)
    {
        msgId += (Uint32)MsgRemoveCurseService - (Uint32)MsgReviveService;
    }
    return msgId;
}

// Shows the keeper's line for the current service and waits for its window.
void mnSanctumShowKeeperMessage(Sint32 msgId)
{
    SHOP_STATE *shop;
    Sint32      voice;

    shop  = RamHeapSlots->menuState;
    voice = fdSpeakerGetVoiceStub(shop->keeperGraphics);
    mnMessageCloseFinishedWindowsStub();
    msgId = mnSanctumGetServiceMessage(msgId);
    mnMessageOpenWindowStub(
        msgId, 5, 0, MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
    while (mnMessageAllTypedStub() == 0)
        sySynch(1);
    sySynch(1);
}

// As mnSanctumShowKeeperMessage, hiding the hand cursor while the line shows.
void mnSanctumShowKeeperMessageHidingCursor(Sint32 msgId)
{
    SHOP_STATE  *shop;
    WINDOW_SPR **cursorSlot;
    Sint32       voice;
    Uint8        savedCursorMode;

    shop                  = RamHeapSlots->menuState;
    cursorSlot            = &shop->cursor.anchor;
    savedCursorMode       = (*cursorSlot)->motion;
    voice                 = fdSpeakerGetVoiceStub(shop->keeperGraphics);
    msgId                 = mnSanctumGetServiceMessage(msgId);
    (*cursorSlot)->motion = WINDOW_SPRITE_MOTION_HIDDEN;
    mnMessageCloseFinishedWindowsStub();
    mnMessageOpenWindowStub(
        msgId, 5, 0, MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
    while (mnMessageAllTypedStub() == 0)
        sySynch(1);
    sySynch(1);
    shop->cursor.anchor->motion = savedCursorMode;
}

void mnPortraitCreateStub(Sint32 portrait, Sint32 a, Sint32 b, WINDOW *window, Sint32 x, Sint32 y);

// Runs a sanctum: the keeper's welcome, the service menu until B, and the farewell; `actorId` is
// the keeper, whose portrait the lines show.
Sint32 mnSanctumRun(Sint32 actorId)
{
    Sint32      serviceSelection = 0;
    WINDOW     *listWindow       = 0;
    SHOP_STATE *shop;
    WINDOW_SPR *cursorAnchor;

    mnShopInitState();
    shop                 = RamHeapSlots->menuState;
    shop->action         = serviceSelection;

    shop->keeperGraphics = ((SPR *)fdActorGetByIdStub(actorId)->sprite)->entries[0]->animationId;

    // The keeper's portrait window; failing that an empty one off the left edge, and failing that
    // one on screen with Ivan's portrait
    listWindow           = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
    if (listWindow == 0)
    {
        listWindow = mnWindowCreateStub(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
        if (listWindow == 0)
        {
            listWindow = mnWindowCreateStub(0, 0, 5, 5, WINDOW_FLAG_INSTANT);
            mnPortraitCreateStub(IVAN, 0, 0, listWindow, -4, -4);
        }
    }

    cursorAnchor =
        mnWindowSpriteCreateStub(shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, listWindow, 0, 0);
    cursorAnchor->motion = WINDOW_SPRITE_MOTION_STILL;
    cursorAnchor->kind   = WINDOW_SPRITE_PLAIN;
    mnCursorSetPosImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursorAnchor;
    mnSanctumShowKeeperMessage((Sint32)MsgWelcomeWearyWanderers);

    shop->moneyWindow = mnWindowCreateStub(16, 11, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
    mnShopDrawMoney();

    for (;;)
    {
        serviceSelection = mnSelectSanctumServiceStub(serviceSelection);
        shop->action     = serviceSelection;
        if (serviceSelection == -1)
            break;

        // "Hmm, you were downed in battle and need reviving, do you?", or the chosen service's own
        // line.
        mnSanctumShowKeeperMessage((Sint32)MsgReviveService);
        if (mnSanctumCountMembersInNeed() == 0)
        {
            // "Fear not, none of your companions is down."
            mnSanctumShowKeeperMessage((Sint32)MsgReviveService + 1);
        }
        else
        {
            mnSanctumRunPartyService();
        }
        shop->action = SANCTUM_SERVICE_REVIVE;
        mnCursorSetPosImmediate(&shop->cursor, -32, 112);
        mnSanctumShowKeeperMessage((Sint32)MsgSanctumMoreAid);
    }

    mnSanctumShowKeeperMessage((Sint32)MsgSanctumFarewell);
    mnWindowCloseStub(shop->moneyWindow, 2);
    mnWindowCloseStub(listWindow, 2);
    mnShopReleaseState();
    return 0;
}

// Italian creates no donation window and passes 0 where the others pass it, a layout difference of
// that language's edition: mnSanctumDrawFee draws nothing without a window, so Italian shows no
// donation while a party member is chosen, and the donation is first seen in the keeper's line
// (MsgReviving).
#define SANCTUM_HAS_DONATION_WINDOW   (LANGUAGE != LANGUAGE_ITALIAN)

// The donation window's left column and width in tiles, sized to each language's text.
#define SANCTUM_DONATION_WINDOW_X     GERMAN_OR(0, 1)
#define SANCTUM_DONATION_WINDOW_WIDTH BY_LANGUAGE(23, 23, 30, 23, 25, 23)

// Runs one service: the player chooses the party member, sees the donation, confirms, pays and
// watches the burst. Repeats while anyone still needs it, until B.
Sint32 mnSanctumRunPartyService(void)
{
    SHOP_STATE *shop = RamHeapSlots->menuState;
    WINDOW     *donationWindow;
    Sint32      redraw;
    Sint32      serviceKind;
    WINDOW     *listWindow;
    Sint32      memberIndex;
    Sint32      unitId;
    Bool        chooseAgain;
    Sint32      donation;
    Sint32      msgBase;

    // Neither window exists yet; Italian's donation window never does.
    listWindow     = NULL;
    donationWindow = NULL;
    // DEAD: memberIndex is set again after the windows before any read; the ROM keeps its store.
    memberIndex    = 0;
    unitId         = 0;
    chooseAgain    = FALSE;
    redraw         = 1;
    serviceKind    = shop->action;
    mnSanctumShowKeeperMessage((Sint32)MsgWhoShallIRevive);

    listWindow                  = mnWindowCreateStub(1, 12, 13, 3, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
    shop->cursorFrame           = 1;
    mnShowPartyFiguresStub(listWindow, 2, 0);
#if SANCTUM_HAS_DONATION_WINDOW
    donationWindow = mnWindowCreateStub(
        SANCTUM_DONATION_WINDOW_X, 16, SANCTUM_DONATION_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);
#endif
    for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++)
    {
        unitId = shop->partyMemberIds[memberIndex];
        if (mnSanctumNeedsService(unitId, serviceKind) != 0)
            break;
    }

    redraw = MENU_GRID_CHANGED_PAGE;
    for (;;)
    {
        if (chooseAgain != 0)
        {
            chooseAgain = FALSE;
            mnSanctumShowKeeperMessage((Sint32)MsgWhoShallIRevive);
            redraw = MENU_GRID_CHANGED_PAGE;
            for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++)
            {
                unitId = shop->partyMemberIds[memberIndex];
                if (mnSanctumNeedsService(unitId, serviceKind) != 0)
                    break;
            }
        }

        if (redraw != 0)
        {
            unitId = shop->partyMemberIds[memberIndex];
            mnCursorPlace(listWindow, (memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
            shop->cursorFrame = 3;
            if (redraw == MENU_GRID_CHANGED_PAGE)
            {
                mnCreatePartyFiguresStub(memberIndex / SHOP_PARTY_PAGE_MEMBERS
                                         * SHOP_PARTY_PAGE_MEMBERS);
                sySynch(1);
            }
            mnSanctumHighlightMembers(listWindow, memberIndex);
            mnSanctumDrawFee(donationWindow, unitId);
            redraw = 0;
        }

        if ((PerWork.push & KEY_A) != 0)
        {
            sySynch(1);
            donation = mnSanctumServicePrice(unitId, serviceKind);
            if (mnSanctumNeedsService(unitId, serviceKind) == 0)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                continue;
            }
            mnWindowSpritePrepareForRedrawStub(listWindow);
            mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
            mnMessageArgumentPushStub(donation, MESSAGE_ARGUMENT_NUMBER);
            // The revive service's lines are quoted; the others say their own.
            msgBase = (Sint32)MsgReviving;
            // "Reviving <name> requires a donation of <n> coins. OK?"
            mnSanctumShowKeeperMessage(msgBase);
            if (mnSanctumAskYesNo(0) != 0)
            {
                // "The donation is too much? Then weary wanderers you shall remain."
                mnSanctumShowKeeperMessageHidingCursor(msgBase + 2);
                chooseAgain = TRUE;
                continue;
            }
            if ((Uint32)donation > MainRoutineStateWork.coins)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                // "I'm sorry, but you must be able to pay the donation."
                mnSanctumShowKeeperMessageHidingCursor(msgBase + 1);
                chooseAgain = TRUE;
                continue;
            }
            mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
            // "Then I call upon my healing powers."
            mnSanctumShowKeeperMessage(msgBase + 3);
            mnMessageCloseFinishedWindowsStub();
            mnSanctumApplyService(unitId, serviceKind);
            mnSanctumRunMemberIconBurst(memberIndex % SHOP_PARTY_PAGE_MEMBERS);
            gmPartyAdjustCoinsStub(-donation);
            mnShopDrawMoney();
            mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
            // "<name> has been revived!"
            mnSanctumShowKeeperMessage(msgBase + 4);
            if (mnSanctumCountMembersInNeed() != 0)
            {
                chooseAgain = TRUE;
                continue;
            }
            break;
        }
        if ((PerWork.push & KEY_B) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            break;
        }
        redraw = mnGridMoveCursor(&memberIndex, shop->partyMemberCount, SHOP_PARTY_PAGE_MEMBERS);
        sySynch(1);
    }

    mnReleasePartyFiguresStub();
    mnWindowCloseStub(donationWindow, 2);
    mnWindowCloseStub(listWindow, 2);
    sySynch(1);
    return 0;
}

#undef SANCTUM_HAS_DONATION_WINDOW
#undef SANCTUM_DONATION_WINDOW_X
#undef SANCTUM_DONATION_WINDOW_WIDTH

// Gives the party member the service: full HP for a revive, the poison or the haunting lifted, or
// every cursed item it wears taken off.
void mnSanctumApplyService(Sint32 unitId, Sint32 serviceKind)
{
    UNIT *unit;

    unit = syUnitGet(unitId);
    if (serviceKind == SANCTUM_SERVICE_REVIVE)
    {
        unit->hp = unit->hpMax;
        gmUnitRecalculateGaugesStub(unitId);
    }
    else if (serviceKind == SANCTUM_SERVICE_CURE_POISON)
    {
        unit->poison = 0;
    }
    else if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL)
    {
        unit->haunt = 0;
    }
    else if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE)
    {
        Sint32 inventorySlot;

        for (inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++)
        {
            if (unit->inventory[inventorySlot] & INVENTORY_EQUIPPED)
            {
                if (gmItemGetDirectStub(unit->inventory[inventorySlot])->flags & ITEM_CURSED)
                {
                    unit->inventory[inventorySlot] ^= INVENTORY_EQUIPPED;
                    gmUnitCalculateStub(unitId);
                }
            }
        }
    }
}

// Shows the chosen party member running in place and the others standing, four a page with its
// arrows, and draws at seven tenths of their size the party members who do not need the service.
void mnSanctumHighlightMembers(WINDOW *window, Sint32 selected)
{
    SHOP_STATE *shop        = RamHeapSlots->menuState;
    Sint32      serviceKind = shop->action;
    Sint32      first       = selected / SHOP_PARTY_PAGE_MEMBERS * SHOP_PARTY_PAGE_MEMBERS;
    Sint32      count       = shop->partyMemberCount;
    WINDOW_SPR *icon;
    Sint32      i;

    if (window != 0)
    {
        mnWindowSpritePrepareForRedrawStub(window);
        if (first != 0)
        {
            icon = mnWindowSpriteCreateStub(
                shop->prevPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 88, -12);
            icon->kind          = WINDOW_SPRITE_PLAIN;
            icon->motion        = WINDOW_SPRITE_MOTION_HOP_UP;
            icon->animationStep = 0;
        }
        if (first + SHOP_PARTY_PAGE_MEMBERS < count)
        {
            icon = mnWindowSpriteCreateStub(
                shop->nextPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 88, 0);
            icon->kind          = WINDOW_SPRITE_PLAIN;
            icon->motion        = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
            icon->animationStep = 0;
        }
        for (i = 0; i < SHOP_PARTY_PAGE_MEMBERS; i++)
        {
            Sint32 unitId = shop->partyMemberIds[first + i];

            if (shop->partyMemberIcons[i] != 0)
            {
                if (first + i == selected)
                    fdSpriteStartAnimationStub(shop->partyMemberIcons[i], ANIM_PARTY_RUN);
                else
                    fdSpriteStartAnimationStub(shop->partyMemberIcons[i], ANIM_STAND);
                shop->partyMemberScale[i] = toFIXED(1);
                if (!mnSanctumNeedsService(unitId, serviceKind))
                    shop->partyMemberScale[i] = toFIXED(7) / 10;
            }
        }
    }
}

// Shows the chosen party member's donation for the service, or that none is needed.
void mnSanctumDrawFee(WINDOW *window, Sint32 unitId)
{
    Sint32      msg;
    Sint32      donation;
    SHOP_STATE *shop;

    shop     = RamHeapSlots->menuState;
    msg      = shop->action;
    donation = mnSanctumServicePrice(unitId, msg);
    if (window != 0)
    {
        mnWindowSpritePrepareForRedrawStub(window);
        if (mnSanctumNeedsService(unitId, msg) != 0)
            msg = (Sint32)MsgReviveCost;
        else
            msg = (Sint32)MsgNoHealingNeeded;
        msg = mnSanctumGetServiceMessage(msg);
        mnMessageArgumentPushStub(donation, MESSAGE_ARGUMENT_NUMBER);
        mnTextDrawMessageStub(msg, (Sint32)window, 0, 0);
    }
}

// Runs one sparkle of the service's burst: placed on a ring 40 pixels out and sent to a point near
// the centre, it flies in again each time it arrives, until the burst ends and it is freed on
// arrival.
void mnSanctumUpdateBurstSparkle(FIELD_PARTICLE *sparkle)
{
    FIELD_PARTICLE *particle = (FIELD_PARTICLE *)sparkle;
    POS             pos;
    Sint8          *statePtr;
    Sint32          state;
    Sint32          result;

    statePtr = &particle->state;
    state    = *statePtr;
    if (state == SANCTUM_SPARKLE_START)
    {
        pos.x = particle->originX;
        pos.z = particle->originZ;
        syVectorAddPolarOffset(toFIXED(40), syRandom(), &pos.x);
        fdParticleSetPosStub(sparkle, pos.x, pos.z);
        pos.x = particle->originX;
        pos.z = particle->originZ;
        syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos.x);
        particle->targetX         = pos.x;
        particle->targetZ         = pos.z;
        particle->speedMax        = toFIXED(2);
        particle->acceleration    = toFIXED(2) / 5;
        particle->turnStepLimited = state;
        *statePtr                 = (Uint8)*statePtr + 1;
    }
    else if (state == SANCTUM_SPARKLE_FLYING)
    {
        result = fdParticleHasPendingTargetStub(sparkle);
        if (result == 0)
            *statePtr = result;
    }
    else if (state == SANCTUM_SPARKLE_LAST_FLIGHT)
    {
        if (fdParticleHasPendingTargetStub(sparkle) == 0)
            fdEffectClearOwnedSlotStub(sparkle);
    }
}

void fdSpriteSetAnimationColorModesStub(void *object, Sint32 value);

// Moves the burst's sparkles each tick and gives the party member's icon a random colour mode while
// the burst plays on it. Runs as an event.
void mnSanctumUpdateBurst(void)
{
    SHOP_STATE *shop = RamHeapSlots->menuState;
    Sint32      i;
    Sint32      burstMember;

    for (i = 0; i < SANCTUM_PARTICLE_COUNT; i++)
        fdParticleUpdateStub(&shop->effects[i]);
    burstMember = shop->burstMember;
    if (burstMember != -1)
        fdSpriteSetAnimationColorModesStub(shop->partyMemberIcons[burstMember],
                                           (syRandom() * 7) >> 16);
}

void fdSpriteSetAnimationSpeedStub(void *object, Sint32 speed);

// Plays the service's burst on a party member's icon: its tune, the colours tinted, and
// SANCTUM_BURST_SPARKLES sparkles flying in around the icon as it changes colour.
void mnSanctumRunMemberIconBurst(Sint32 member)
{
    SHOP_STATE     *shop;
    Sint8           savedCursorMode;
    POS             pos;
    FIELD_PARTICLE *effect;
    Sint32          effectIndex;

    shop                        = RamHeapSlots->menuState;
    savedCursorMode             = shop->cursor.anchor->motion;
    shop->burstMember           = -1;
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
    sySoundPlayStub(SanctumServiceTunes[shop->action]);
    mnShopSaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
    fdSpriteSetAnimationSpeedStub(shop->partyMemberIcons[member], 0);
    sySynch(20);
    sySetEvent(mnSanctumUpdateBurst, EVENT_PASS_TICK);

    pos.x = toFIXED(shop->partyMemberX[member]);
    pos.z = toFIXED(shop->partyMemberY[member]) - toFIXED(12);

    for (effectIndex = 0; effectIndex < SANCTUM_BURST_SPARKLES; effectIndex++)
    {
        effect = &shop->effects[effectIndex];
        fdParticleInitStub(effect, SPRITE_PARTICLE, pos.x, pos.z);
        fdParticleSetCallbackStub(effect, mnSanctumUpdateBurstSparkle);
        fdParticleSetAnimationStub(effect, 7);
        fdSpriteSetAnimationColorModesStub(effect->sprite, (syRandom() * 7) >> 16);
        effect->scaleY = toFIXED(7) / 10;
        effect->scaleX = toFIXED(7) / 10;
        sySynch(3);
        // From the sixth sparkle on, the party member's figure flickers through colours.
        if (effectIndex == 5)
        {
            shop->burstMember = member;
        }
    }

    mnSoundWaitForJingle();
    // Every sparkle still flying makes its last flight.
    for (effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++)
    {
        if (shop->effects[effectIndex].active)
            shop->effects[effectIndex].state = SANCTUM_SPARKLE_LAST_FLIGHT;
    }

    sySynch(20);
    sySoundPlayStub(SE_RECOVERY);
    shop->burstMember = -1;
    fdSpriteSetAnimationColorModesStub(shop->partyMemberIcons[member], 0);
    sySynch(20);

    {
        Uint8          *active   = (Uint8 *)&shop->effects[0].active;
        FIELD_PARTICLE *particle = shop->effects;

        for (effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++)
        {
            Sint32 flag   = *active;

            // DEAD: the byte moved to the top of the word, which cannot change whether it is zero;
            // the ROM keeps the shift.
            flag        <<= 24;

            active       += sizeof(FIELD_PARTICLE);
            if (flag != 0)
            {
                fdEffectClearOwnedSlotStub(particle);
            }
            particle++;
        }
    }

    syCloseEvent(mnSanctumUpdateBurst);
    fdSpriteSetAnimationSpeedStub(shop->partyMemberIcons[member], ANIM_STEP_NORMAL);
    mnShopRestoreFieldPalette();
    sySynch(30);
    shop->cursor.anchor->motion = savedCursorMode;
}
