// The sanctum's services, their prices and their sparkles. Part of shop.c.

#include "shop_internal.h"

// The colour modes a burst picks from at random for its sparkles and the party member's icon:
// SPRITE_COLOR_NORMAL, the channel swaps and SPRITE_COLOR_GREY.
#define SANCTUM_BURST_COLOR_MODES (SPRITE_COLOR_GREY + 1)

// The donations the sanctum asks, in coins: reviving and removing a curse ask so many for each of
// the party member's levels.
typedef enum SANCTUM_DONATION
{
    DONATION_REVIVE_PER_LEVEL       = 20,
    DONATION_CURE_POISON            = 10,
    DONATION_REPEL_EVIL             = 50,
    DONATION_REMOVE_CURSE_PER_LEVEL = 10
} SANCTUM_DONATION;

// Returns the sanctum's fee, in coins, for `serviceKind` on party member `unitId`.
Sint32 mnSanctumServicePrice(Sint32 unitId, Sint32 serviceKind)
{
    Uint8  level    = gmUnitGetStub(unitId)->level;
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

// Returns 1 when a party member needs the service: down for Revive, poisoned for Cure Poison,
// haunted for Repel Evil, cursed for Remove Curse.
Sint32 mnSanctumNeedsService(Sint32 unitId, Sint32 serviceKind)
{
    UNIT *unit     = gmUnitGetStub(unitId);
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
    SHOP_STATE *shop          = MenuState;
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
    Sint8 serviceKind = ((SHOP_STATE *)MenuState)->action;

    if (serviceKind == SANCTUM_SERVICE_CURE_POISON)
    {
        msgId += MsgCurePoisonService - MsgReviveService;
    }
    if (serviceKind == SANCTUM_SERVICE_REPEL_EVIL)
    {
        msgId += MsgExorcismService - MsgReviveService;
    }
    if (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE)
    {
        msgId += MsgRemoveCurseService - MsgReviveService;
    }
    return msgId;
}

// Shows the healer's line and waits until it is typed out. A service's line is given as Revive's,
// and the chosen service says its own; the sanctum's other lines are shown while shop->action is
// Revive.
void mnSanctumShowKeeperMessage(Sint32 msgId)
{
    Sint32 voice;

    voice = fdSpeakerGetVoiceStub(((SHOP_STATE *)MenuState)->keeperGraphics);
    mnMessageCloseFinishedWindowsStub();
    msgId = mnSanctumGetServiceMessage(msgId);
    mnMessageOpenWindowStub(
        msgId, 5, 0, MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
    while (mnMessageAllTypedStub() == 0)
    {
        sySynch(1);
    }
    sySynch(1);
}

// As mnSanctumShowKeeperMessage, hiding the hand cursor while the line shows.
void mnSanctumShowKeeperMessageHidingCursor(Sint32 msgId)
{
    SHOP_STATE *shop            = MenuState;
    Uint8       savedCursorMode = shop->cursor.anchor->motion;
    Sint32      voice           = fdSpeakerGetVoiceStub(shop->keeperGraphics);

    msgId                       = mnSanctumGetServiceMessage(msgId);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
    mnMessageCloseFinishedWindowsStub();
    mnMessageOpenWindowStub(
        msgId, 5, 0, MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
    while (mnMessageAllTypedStub() == 0)
        sySynch(1);
    sySynch(1);
    shop->cursor.anchor->motion = savedCursorMode;
}

// Runs a sanctum: the healer's welcome, the service menu until B, and the farewell; keeperId is the
// healer's actor, whose portrait the lines show.
Sint32 mnSanctumRun(Sint32 keeperId)
{
    Sint32      serviceSelection = 0;
    WINDOW     *listWindow       = 0;
    SHOP_STATE *shop;
    WINDOW_SPR *cursorAnchor;

    mnShopInitState();
    shop         = MenuState;
    shop->action = serviceSelection;

    {
        Sint32 keeperAnimationId =
            (Uint16)(fdActorGetByIdStub(keeperId))->sprite->entries[0]->animationId;
        shop->keeperGraphics = keeperAnimationId;
    }

    // The healer's portrait window; failing that an empty one off the left edge, and failing that
    // one on screen with Ivan's portrait
    listWindow = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
    if (listWindow == 0)
    {
        listWindow = mnWindowCreateStub(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
    }
    if (listWindow == 0)
    {
        listWindow = mnWindowCreateStub(0, 0, 5, 5, WINDOW_FLAG_INSTANT);
        mnPortraitCreateStub(IVAN, 0, 0, listWindow, -4, -4);
    }

    cursorAnchor =
        mnWindowSpriteCreateStub(shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, listWindow, 0, 0);
    cursorAnchor->motion = WINDOW_SPRITE_MOTION_STILL;
    cursorAnchor->kind   = WINDOW_SPRITE_PLAIN; // the icon is the shop state's
    mnCursorSetPosImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursorAnchor;
    // "Welcome, weary wanderers. What aid do you seek?"
    mnSanctumShowKeeperMessage(MsgSanctumWelcome);

    shop->moneyWindow = mnWindowCreateStub(16, 11, SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
    mnShopDrawMoney();

    for (;;)
    {
        serviceSelection = mnSelectSanctumServiceStub(serviceSelection);
        shop->action     = serviceSelection;
        if (serviceSelection == -1)
            break;

        {
            Sint32 message = MsgReviveService;

            // Revive's words: "Hmm, you were downed in battle and need reviving, do you?"
            mnSanctumShowKeeperMessage(message);
            if (mnSanctumCountMembersInNeed() == 0)
            {
                // "Fear not, none of your companions are down."
                mnSanctumShowKeeperMessage(message + 1);
            }
            else
            {
                mnSanctumRunPartyService();
            }
        }
        shop->action = SANCTUM_SERVICE_REVIVE;
        mnCursorSetPosImmediate(&shop->cursor, -32, 112);
        // "Do you wish for more aid?"
        mnSanctumShowKeeperMessage(MsgSanctumMoreAid);
    }

    // "Visit us again anytime you need healing."
    mnSanctumShowKeeperMessage(MsgSanctumFarewell);
    mnWindowCloseStub(shop->moneyWindow, 2);
    mnWindowCloseStub(listWindow, 2);
    mnShopReleaseState();
    return 0;
}

// Lets the player pick a party member for the service, asks for the donation, then applies it with
// its burst; offers another while any still need it.
Sint32 mnSanctumRunPartyService(void)
{
    SHOP_STATE *shop = MenuState;
    WINDOW     *priceWindow;
    Bool        redraw;
    Sint32      serviceKind;
    WINDOW     *listWindow;
    Sint32      memberIndex;
    Sint32      unitId;
    Bool        chooseAgain;
    Uint32      donation;
    Sint32      msgBase;

    priceWindow = 0;
    redraw      = TRUE;
    serviceKind = shop->action;
    // Revive's words, as below: "Who shall I revive?"
    mnSanctumShowKeeperMessage(MsgWhoToRevive);

    listWindow                  = mnWindowCreateStub(1, 12, 13, 3, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
    // redraw is 1 here: the hand's first move to a party member takes one tick
    shop->cursorFrame           = redraw;
    mnCreatePartyFiguresStub(listWindow, 2, 0, 8, 0);
    priceWindow = mnWindowCreateStub(
        SANCTUM_PRICE_WINDOW_X, 16, SANCTUM_PRICE_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);

    memberIndex = 0;
    unitId      = 0;
    chooseAgain = FALSE;
    for (; memberIndex < shop->partyMemberCount; memberIndex++)
    {
        unitId = shop->partyMemberIds[memberIndex];
        if (mnSanctumNeedsService(unitId, serviceKind) != 0)
            break;
    }

    redraw = TRUE;
    for (;;)
    {
        if (chooseAgain != 0)
        {
            chooseAgain = FALSE;
            mnSanctumShowKeeperMessage(MsgWhoToRevive);
            redraw = TRUE;
            for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++)
            {
                unitId = shop->partyMemberIds[memberIndex];
                if (mnSanctumNeedsService(unitId, serviceKind) != 0)
                    break;
            }
        }

        if (redraw != 0)
        {
            redraw      = FALSE;
            memberIndex = (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
            unitId      = shop->partyMemberIds[memberIndex];
            mnCursorPlace(listWindow, memberIndex * 24 - 12, 0);
            shop->cursorFrame = 3;
            mnSanctumHighlightMembers(listWindow, memberIndex);
            mnSanctumDrawFee(priceWindow, unitId);
        }

        if ((PerPush & KEY_A) != 0)
        {
            sySynch(1);
            donation = mnSanctumServicePrice(unitId, serviceKind);
            if (mnSanctumNeedsService(unitId, serviceKind) == 0)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                continue;
            }
            mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
            mnMessageArgumentPushStub(donation, MESSAGE_ARGUMENT_NUMBER);
            msgBase = MsgReviveDonation;
            // "Reviving (the member) requires a donation of (fee) coins. OK?"
            mnSanctumShowKeeperMessage(msgBase);
            if (mnSanctumAskYesNo(0) != 0)
            {
                // "The donation is too much? Then weary wanderers you shall remain."
                mnSanctumShowKeeperMessageHidingCursor(msgBase + 2);
                chooseAgain = TRUE;
                continue;
            }
            if (donation > MainRoutineStateWork.holdings.coins)
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
            mnSanctumRunMemberIconBurst(memberIndex);
            gmPartyAdjustCoinsStub(-donation);
            mnShopDrawMoney();
            mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
            // "(The member) has been revived!"
            mnSanctumShowKeeperMessage(msgBase + 4);
            if (mnSanctumCountMembersInNeed() != 0)
            {
                chooseAgain = TRUE;
                continue;
            }
            break;
        }
        if ((PerPush & KEY_B) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            break;
        }
        if ((PerRepeat & KEY_LEFT) != 0)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            redraw = TRUE;
            memberIndex--;
        }
        if ((PerRepeat & KEY_RIGHT) != 0)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            redraw = TRUE;
            memberIndex++;
        }
        sySynch(1);
        continue;
    }

    mnReleasePartyFiguresStub();
    mnWindowCloseStub(priceWindow, 2);
    mnWindowCloseStub(listWindow, 2);
    sySynch(1);
    return 0;
}

// Applies a sanctum service to a party member: full HP for a revive, poison or the haunting
// cleared, or every cursed item taken off.
void mnSanctumApplyService(Sint32 unitId, Sint32 serviceKind)
{
    UNIT *unit;

    unit = gmUnitGetStub(unitId);
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

// Shows the chosen party member running in place, and those who do not need the service drawn
// smaller.
void mnSanctumHighlightMembers(WINDOW *window, Sint32 selected)
{
    SHOP_STATE *shop = MenuState;
    Sint32      memberIndex;
    Sint32      serviceKind = shop->action;

    if (window != 0)
    {
        for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++)
        {
            if (memberIndex == selected)
                fdSpriteStartAnimationStub(shop->partyMemberIcons[memberIndex], ANIM_PARTY_RUN);
            else
                fdSpriteStartAnimationStub(shop->partyMemberIcons[memberIndex], ANIM_STAND);
            shop->partyMemberScale[memberIndex] = toFIXED(1);
            if (!mnSanctumNeedsService(shop->partyMemberIds[memberIndex], serviceKind))
                shop->partyMemberScale[memberIndex] = toFIXED(7) / 10;
        }
    }
}

// Draws the party member's fee for the current service, or that they need no treatment.
void mnSanctumDrawFee(WINDOW *target, Sint32 unitId)
{
    Sint32 serviceOrMsg;
    Sint32 donation;

    serviceOrMsg = ((SHOP_STATE *)MenuState)->action;
    donation     = mnSanctumServicePrice(unitId, serviceOrMsg);
    if (target != 0)
    {
        mnWindowSpritePrepareForRedrawStub(target);
        if (mnSanctumNeedsService(unitId, serviceOrMsg) != 0)
        {
            // "Revive for (fee) coins"
            serviceOrMsg = MsgReviveCost;
        }
        else
        {
            // "This ally needs no healing."
            serviceOrMsg = MsgNoHealingNeeded;
        }
        serviceOrMsg = mnSanctumGetServiceMessage(serviceOrMsg);
        mnMessageArgumentPushStub(donation, MESSAGE_ARGUMENT_NUMBER);
        mnTextDrawMessageStub(serviceOrMsg, target, 0, 0);
    }
}

// Steps one sparkle of the sanctum's burst, which flies in from a ring around the icon, again and
// again until the burst ends.
void mnSanctumUpdateBurstSparkle(FIELD_PARTICLE *effect)
{
    POS    pos;
    Sint8 *statePtr;
    Sint32 state;
    Sint32 targetPending;

    statePtr = &effect->state;
    state    = *statePtr;
    if (state == SANCTUM_SPARKLE_START)
    {
        pos.x = effect->originX;
        pos.z = effect->originZ;
        syVectorAddPolarOffset(toFIXED(40), syRandom(), (FIXED *)&pos);
        fdParticleSetPosStub(effect, pos.x, pos.z);
        pos.x = effect->originX;
        pos.z = effect->originZ;
        syVectorAddPolarOffset(toFIXED(4), syRandom(), (FIXED *)&pos);
        effect->targetX         = pos.x;
        effect->targetZ         = pos.z;
        // 16.16 pixels: at most 2 a tick, gaining 0.4 a tick
        effect->speedMax        = toFIXED(2);
        effect->acceleration    = toFIXED(2) / 5;
        effect->turnStepLimited = state;
        *statePtr               = *statePtr + 1;
    }
    else if (state == SANCTUM_SPARKLE_FLYING)
    {
        targetPending = fdParticleHasPendingTargetStub(effect);
        if (targetPending == 0)
            *statePtr = targetPending;
    }
    else if (state == SANCTUM_SPARKLE_LAST_FLIGHT)
    {
        if (fdParticleHasPendingTargetStub(effect) == 0)
            fdEffectClearOwnedSlotStub(effect);
    }
}

// Steps the burst's sparkles, and while the tune plays flickers the party member's icon through
// random colours, a new one each tick.
void mnSanctumUpdateBurst(void)
{
    SHOP_STATE *shop = MenuState;
    Sint32      i;
    Sint8       burstMember;

    for (i = 0; i < SANCTUM_PARTICLE_COUNT; i++)
        fdParticleUpdateStub(&shop->effects[i]);
    burstMember = shop->burstMember;
    if (burstMember != -1)
        fdSpriteSetAnimationColorModesStub(shop->partyMemberIcons[burstMember],
                                           syRandom16Below(SANCTUM_BURST_COLOR_MODES));
}

// The tune each service's burst plays.
static const Sint8 SanctumServiceSounds[4] = {
    [SANCTUM_SERVICE_REVIVE]       = MUS_RECOVERING_AFTER_COMBAT,
    [SANCTUM_SERVICE_CURE_POISON]  = MUS_POISON_CURED,
    [SANCTUM_SERVICE_REPEL_EVIL]   = MUS_EXORCISED,
    [SANCTUM_SERVICE_REMOVE_CURSE] = MUS_CURSE_LIFTED,
};

// Plays the service's burst on a party member's icon: its tune, the field greyed, and
// SANCTUM_BURST_SPARKLES sparkles flying in at the icon as it flickers through colours, then the
// recovery sound.
void mnSanctumRunMemberIconBurst(Sint32 member)
{
    SHOP_STATE     *shop;
    Sint8           savedCursorMode;
    POS             pos;
    FIELD_PARTICLE *effect;
    Sint32          effectIndex;

    shop                        = MenuState;
    savedCursorMode             = shop->cursor.anchor->motion;
    shop->burstMember           = -1;
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
    sySoundPlayStub(SanctumServiceSounds[shop->action]);
    mnShopSaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
    fdSpriteSetAnimationSpeedStub(shop->partyMemberIcons[member], 0);
    sySynch(20);
    sySetEvent(mnSanctumUpdateBurst, EVENT_PASS_TICK);

    // The burst's centre, in 16.16 pixels: 12 above the party member's icon
    pos.x = toFIXED(shop->partyMemberX[member]);
    pos.z = toFIXED(shop->partyMemberY[member]) - toFIXED(12);

    // A sparkle starts every 3 ticks; the burst's event moves them.
    for (effectIndex = 0; effectIndex < SANCTUM_BURST_SPARKLES; effectIndex++)
    {
        effect = &shop->effects[effectIndex];
        fdParticleInitStub(effect, SPRITE_PARTICLE, pos.x, pos.z);
        fdParticleSetCallbackStub(effect, mnSanctumUpdateBurstSparkle);
        fdParticleSetAnimationStub(effect, PARTICLE_ANIMATION_FLICKER);
        fdSpriteSetAnimationColorModesStub(effect->sprite,
                                           syRandom16Below(SANCTUM_BURST_COLOR_MODES));
        effect->scaleY = toFIXED(7) / 10;
        effect->scaleX = toFIXED(7) / 10;
        sySynch(3);
        // The icon starts to flicker with the sixth sparkle.
        if (effectIndex == 5)
        {
            shop->burstMember = member;
        }
    }

    // As the tune ends, every sparkle makes one last flight and is freed when it arrives.
    mnSoundWaitForJingle();
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

    // Free the sparkles still flying.
    for (effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++)
    {
        effect = &shop->effects[effectIndex];
        if (effect->active != 0)
            fdEffectClearOwnedSlotStub(effect);
    }

    syCloseEvent(mnSanctumUpdateBurst);
    fdSpriteSetAnimationSpeedStub(shop->partyMemberIcons[member], ANIM_STEP_NORMAL);
    mnShopRestoreFieldPalette();
    sySynch(30);
    shop->cursor.anchor->motion = savedCursorMode;
}
