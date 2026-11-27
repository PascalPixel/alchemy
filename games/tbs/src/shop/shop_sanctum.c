/* @file */
/* The sanctum's services, their prices and their sparkles. */
/* Part of shop.c. */

#include "shop_internal.h"

/* The colour modes a burst picks from at random for its sparkles and the */
/* party member's icon: SPRITE_COLOR_NORMAL, the channel swaps and */
/* SPRITE_COLOR_GREY. */
#define SANCTUM_BURST_COLOR_MODES (SPRITE_COLOR_GREY + 1)

/* The donations the sanctum asks, in coins: reviving and removing a curse ask
 */
/* so many for each of the party member's levels. */
typedef enum SANCTUM_DONATION {
	DONATION_REVIVE_PER_LEVEL = 20,
	DONATION_CURE_POISON = 10,
	DONATION_REPEL_EVIL = 50,
	DONATION_REMOVE_CURSE_PER_LEVEL = 10
} SANCTUM_DONATION;

/* Returns the sanctum's fee, in coins, for `serviceKind` on party member */
/* `unitId`. */
s32 mnSanctumServicePrice(s32 unitId, s32 serviceKind) {
	u8 level = gmUnitGetStub(unitId)->level;
	s32 donation = 0;

	if(serviceKind == SANCTUM_SERVICE_REVIVE) {
		donation = level * DONATION_REVIVE_PER_LEVEL;
	}
	else if(serviceKind == SANCTUM_SERVICE_CURE_POISON) {
		donation = DONATION_CURE_POISON;
	}
	else if(serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
		donation = DONATION_REPEL_EVIL;
	}
	else if(serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
		donation = level * DONATION_REMOVE_CURSE_PER_LEVEL;
	}
	return donation;
}

/* Returns 1 when a party member needs the service: down for Revive, poisoned */
/* for Cure Poison, haunted for Repel Evil, cursed for Remove Curse. */
s32 mnSanctumNeedsService(s32 unitId, s32 serviceKind) {
	UNIT *unit = gmUnitGetStub(unitId);
	s32 eligible = 0;

	if((serviceKind == SANCTUM_SERVICE_REVIVE && unit->hp <= 0) ||
	   (serviceKind == SANCTUM_SERVICE_CURE_POISON && unit->poison != 0) ||
	   (serviceKind == SANCTUM_SERVICE_REPEL_EVIL && unit->haunt != 0) ||
	   (serviceKind == SANCTUM_SERVICE_REMOVE_CURSE && unit->curse != 0)) {
		eligible = 1;
	}
	return eligible;
}

/* Counts party members who need the service in shop->action. */
s32 mnSanctumCountMembersInNeed(void) {
	SHOP_STATE *shop = MenuState;
	s32 eligibleCount = 0;
	s32 serviceKind = shop->action;
	s32 memberIndex;

	for(memberIndex = 0; memberIndex < shop->partyMemberCnt; memberIndex++) {
		if(mnSanctumNeedsService(shop->partyMemberIds[memberIndex],
								 serviceKind))
			eligibleCount++;
	}
	return eligibleCount;
}

/* Returns the current service's line that stands where messageId stands among
 */
/* the revive service's lines. */
s32 mnSanctumGetServiceMessage(s32 messageId) {
	s8 serviceKind = ((SHOP_STATE *)MenuState)->action;

	if(serviceKind == SANCTUM_SERVICE_CURE_POISON) {
		messageId += MsgCurePoisonService - MsgReviveService;
	}
	if(serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
		messageId += MsgExorcismService - MsgReviveService;
	}
	if(serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
		messageId += MsgRemoveCurseService - MsgReviveService;
	}
	return messageId;
}

/* Shows the healer's line and waits until it is typed out. A service's line is
 */
/* given as Revive's, and the chosen service says its own; the sanctum's other
 */
/* lines are shown while shop->action is Revive. */
void mnSanctumShowKeeperMessage(s32 messageId) {
	s32 voice;

	voice = fdSpeakerGetVoiceStub(((SHOP_STATE *)MenuState)->keeperGraphics);
	mnMessageCloseFinishedWindowsStub();
	messageId = mnSanctumGetServiceMessage(messageId);
	mnMessageOpenWindowStub(messageId, 5, 0,
							MESSAGE_WINDOW_VOICE(voice) |
								MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTypedStub() == 0) {
		sySynch(1);
	}
	sySynch(1);
}

/* As mnSanctumShowKeeperMessage, hiding the hand cursor while the line shows.
 */
void mnSanctumShowKeeperMessageHidingCursor(s32 messageId) {
	SHOP_STATE *shop = MenuState;
	u8 savedCursorMode = shop->cursor.anchor->motion;
	s32 voice = fdSpeakerGetVoiceStub(shop->keeperGraphics);

	messageId = mnSanctumGetServiceMessage(messageId);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	mnMessageCloseFinishedWindowsStub();
	mnMessageOpenWindowStub(messageId, 5, 0,
							MESSAGE_WINDOW_VOICE(voice) |
								MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTypedStub() == 0)
		sySynch(1);
	sySynch(1);
	shop->cursor.anchor->motion = savedCursorMode;
}

/* Runs a sanctum: the healer's welcome, the service menu until B, and the */
/* farewell; keeperId is the healer's actor, whose portrait the lines show. */
s32 mnSanctumRun(s32 keeperId) {
	s32 serviceSelection = 0;
	WINDOW *listWindow = 0;
	SHOP_STATE *shop;
	WINDOW_SPR *cursorAnchor;

	mnShopInitState();
	shop = MenuState;
	shop->action = serviceSelection;

	{
		s32 keeperAnimationId =
			(u16)(fdActorGetByIdStub(keeperId))->spr->entries[0]->anmId;
		shop->keeperGraphics = keeperAnimationId;
	}

	/* The healer's portrait window; failing that an empty one off the left */
	/* edge, and failing that one on screen with Ivan's portrait */
	listWindow = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
	if(listWindow == 0) {
		listWindow = mnWindowCreateStub(-5, 0, 5, 5, WINDOW_FLAG_INSTANT);
	}
	if(listWindow == 0) {
		listWindow = mnWindowCreateStub(0, 0, 5, 5, WINDOW_FLAG_INSTANT);
		mnPortraitCreateStub(IVAN, 0, 0, listWindow, -4, -4);
	}

	cursorAnchor = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, listWindow, 0, 0);
	cursorAnchor->motion = WINDOW_SPRITE_MOTION_STILL;
	cursorAnchor->kind = WINDOW_SPRITE_PLAIN; /* the icon is the shop state's */
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursorAnchor;
	/* "Welcome, weary wanderers. What aid do you seek?" */
	mnSanctumShowKeeperMessage(MsgSanctumWelcome);

	shop->moneyWin = mnWindowCreateStub(16, 11, SHOP_MONEY_WINDOW_WIDTH, 4,
										WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();

	for(;;) {
		serviceSelection = mnSelectSanctumServiceStub(serviceSelection);
		shop->action = serviceSelection;
		if(serviceSelection == -1)
			break;

		{
			s32 message = MsgReviveService;

			/* Revive's words: "Hmm, you were downed in battle and need
			 * reviving, do */
			/* you?" */
			mnSanctumShowKeeperMessage(message);
			if(mnSanctumCountMembersInNeed() == 0) {
				/* "Fear not, none of your companions are down." */
				mnSanctumShowKeeperMessage(message + 1);
			}
			else {
				mnSanctumRunPartyService();
			}
		}
		shop->action = SANCTUM_SERVICE_REVIVE;
		mnCursorSetPosImmediate(&shop->cursor, -32, 112);
		/* "Do you wish for more aid?" */
		mnSanctumShowKeeperMessage(MsgSanctumMoreAid);
	}

	/* "Visit us again anytime you need healing." */
	mnSanctumShowKeeperMessage(MsgSanctumFarewell);
	mnWindowCloseStub(shop->moneyWin, 2);
	mnWindowCloseStub(listWindow, 2);
	mnShopReleaseState();
	return 0;
}

/* Lets the player pick a party member for the service, asks for the donation,
 */
/* then applies it with its burst; offers another while any still need it. */
s32 mnSanctumRunPartyService(void) {
	SHOP_STATE *shop = MenuState;
	WINDOW *priceWindow;
	s32 redraw;
	s32 serviceKind;
	WINDOW *listWindow;
	s32 memberIndex;
	s32 unitId;
	s32 chooseAgain;
	u32 donation;
	s32 messageBase;

	priceWindow = 0;
	redraw = 1;
	serviceKind = shop->action;
	/* Revive's words, as below: "Who shall I revive?" */
	mnSanctumShowKeeperMessage(MsgWhoToRevive);

	listWindow = mnWindowCreateStub(1, 12, 13, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	/* redraw is 1 here: the hand's first move to a party member takes one frame
	 */
	shop->cursorFrm = redraw;
	mnCreatePartyFiguresStub(listWindow, 2, 0, 8, 0);
	priceWindow =
		mnWindowCreateStub(SANCTUM_PRICE_WINDOW_X, 16,
						   SANCTUM_PRICE_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);

	memberIndex = 0;
	unitId = 0;
	chooseAgain = 0;
	for(; memberIndex < shop->partyMemberCnt; memberIndex++) {
		unitId = shop->partyMemberIds[memberIndex];
		if(mnSanctumNeedsService(unitId, serviceKind) != 0)
			break;
	}

	redraw = 1;
	for(;;) {
		if(chooseAgain != 0) {
			chooseAgain = 0;
			mnSanctumShowKeeperMessage(MsgWhoToRevive);
			redraw = 1;
			for(memberIndex = 0; memberIndex < shop->partyMemberCnt;
				memberIndex++) {
				unitId = shop->partyMemberIds[memberIndex];
				if(mnSanctumNeedsService(unitId, serviceKind) != 0)
					break;
			}
		}

		if(redraw != 0) {
			redraw = 0;
			memberIndex =
				(memberIndex + shop->partyMemberCnt) % shop->partyMemberCnt;
			unitId = shop->partyMemberIds[memberIndex];
			mnCursorPlace(listWindow, memberIndex * 24 - 12, 0);
			shop->cursorFrm = 3;
			mnSanctumHighlightMembers(listWindow, memberIndex);
			mnSanctumDrawFee(priceWindow, unitId);
		}

		if((KeysPressed & KEY_A) != 0) {
			sySynch(1);
			donation = mnSanctumServicePrice(unitId, serviceKind);
			if(mnSanctumNeedsService(unitId, serviceKind) == 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}
			mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(donation, MESSAGE_ARGUMENT_NUMBER);
			messageBase = MsgReviveDonation;
			/* "Reviving (the member) requires a donation of (fee) coins. OK?"
			 */
			mnSanctumShowKeeperMessage(messageBase);
			if(mnSanctumAskYesNo(0) != 0) {
				/* "The donation is too much? Then weary wanderers you shall
				 * remain." */
				mnSanctumShowKeeperMessageHidingCursor(messageBase + 2);
				chooseAgain = 1;
				continue;
			}
			if(donation > MainRoutineStateWork.holdings.coins) {
				sySndPlayStub(SE_MENU_CANCEL);
				/* "I'm sorry, but you must be able to pay the donation." */
				mnSanctumShowKeeperMessageHidingCursor(messageBase + 1);
				chooseAgain = 1;
				continue;
			}
			mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
			/* "Then I call upon my healing powers." */
			mnSanctumShowKeeperMessage(messageBase + 3);
			mnMessageCloseFinishedWindowsStub();
			mnSanctumApplyService(unitId, serviceKind);
			mnSanctumRunMemberIconBurst(memberIndex);
			gmPartyAdjustCoinsStub(-donation);
			mnShopDrawMoney();
			mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
			/* "(The member) has been revived!" */
			mnSanctumShowKeeperMessage(messageBase + 4);
			if(mnSanctumCountMembersInNeed() != 0) {
				chooseAgain = 1;
				continue;
			}
			break;
		}
		if((KeysPressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			break;
		}
		if((KeysRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = 1;
			memberIndex--;
		}
		if((KeysRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = 1;
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

/* Applies a sanctum service to a party member: full HP for a revive, poison or
 */
/* the haunting cleared, or every cursed item taken off. */
void mnSanctumApplyService(s32 unitId, s32 serviceKind) {
	UNIT *unit;

	unit = gmUnitGetStub(unitId);
	if(serviceKind == SANCTUM_SERVICE_REVIVE) {
		unit->hp = unit->hpMax;
		gmUnitRecalculateGaugesStub(unitId);
	}
	else if(serviceKind == SANCTUM_SERVICE_CURE_POISON) {
		unit->poison = 0;
	}
	else if(serviceKind == SANCTUM_SERVICE_REPEL_EVIL) {
		unit->haunt = 0;
	}
	else if(serviceKind == SANCTUM_SERVICE_REMOVE_CURSE) {
		s32 inventorySlot;

		for(inventorySlot = 0; inventorySlot < INVENTORY_SLOTS;
			inventorySlot++) {
			if(unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
				if(gmItemGetDirectStub(unit->inventory[inventorySlot])->flg &
				   ITEM_CURSED) {
					unit->inventory[inventorySlot] ^= INVENTORY_EQUIPPED;
					gmUnitCalcStub(unitId);
				}
			}
		}
	}
}

/* Shows the chosen party member running in place, and those who do not need */
/* the service drawn smaller. */
void mnSanctumHighlightMembers(WINDOW *window, s32 selected) {
	SHOP_STATE *shop = MenuState;
	s32 memberIndex;
	s32 serviceKind = shop->action;

	if(window != 0) {
		for(memberIndex = 0; memberIndex < shop->partyMemberCnt;
			memberIndex++) {
			if(memberIndex == selected)
				fdSprStartAnimationStub(shop->partyMemberIcons[memberIndex],
										ANIM_PARTY_RUN);
			else
				fdSprStartAnimationStub(shop->partyMemberIcons[memberIndex],
										ANIM_STAND);
			shop->partyMemberScale[memberIndex] = FIXED_ONE;
			if(!mnSanctumNeedsService(shop->partyMemberIds[memberIndex],
									  serviceKind))
				shop->partyMemberScale[memberIndex] = FIXED_ONE * 7 / 10;
		}
	}
}

/* Draws the party member's fee for the current service, or that they need no */
/* treatment. */
void mnSanctumDrawFee(WINDOW *target, s32 unitId) {
	s32 serviceOrMessage;
	s32 donation;

	serviceOrMessage = ((SHOP_STATE *)MenuState)->action;
	donation = mnSanctumServicePrice(unitId, serviceOrMessage);
	if(target != 0) {
		mnWindowSprPrepareForRedrawStub(target);
		if(mnSanctumNeedsService(unitId, serviceOrMessage) != 0) {
			/* "Revive for (fee) coins" */
			serviceOrMessage = MsgReviveCost;
		}
		else {
			/* "This ally needs no healing." */
			serviceOrMessage = MsgNoHealingNeeded;
		}
		serviceOrMessage = mnSanctumGetServiceMessage(serviceOrMessage);
		mnMessageArgumentPushStub(donation, MESSAGE_ARGUMENT_NUMBER);
		mnTextDrawMessageStub(serviceOrMessage, target, 0, 0);
	}
}

/* Steps one sparkle of the sanctum's burst, which flies in from a ring around
 */
/* the icon, again and again until the burst ends. */
void mnSanctumUpdateBurstSparkle(FIELD_PARTICLE *effect) {
	POS position;
	s8 *statePointer;
	s32 state;
	s32 targetPending;

	statePointer = &effect->state;
	state = *statePointer;
	if(state == SANCTUM_SPARKLE_START) {
		position.x = effect->originX;
		position.z = effect->originZ;
		syVectorAddPolarOffset(toFIXED(40), syRandom(), (FIXED *)&position);
		fdParticleSetPosStub(effect, position.x, position.z);
		position.x = effect->originX;
		position.z = effect->originZ;
		syVectorAddPolarOffset(toFIXED(4), syRandom(), (FIXED *)&position);
		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		/* 16.16 pixels: at most 2 a frame, gaining 0.4 a frame */
		effect->speedMax = toFIXED(2);
		effect->acceleration = toFIXED(2) / 5;
		effect->turnStepLimited = state;
		*statePointer = *statePointer + 1;
	}
	else if(state == SANCTUM_SPARKLE_FLYING) {
		targetPending = fdParticleHasPendingTargetStub(effect);
		if(targetPending == 0)
			*statePointer = targetPending;
	}
	else if(state == SANCTUM_SPARKLE_LAST_FLIGHT) {
		if(fdParticleHasPendingTargetStub(effect) == 0)
			fdEffectClearOwnedSlotStub(effect);
	}
}

/* Steps the burst's sparkles, and while the tune plays flickers the party */
/* member's icon through random colours, a new one each frame. */
void mnSanctumUpdateBurst(void) {
	SHOP_STATE *shop = MenuState;
	s32 i;
	s8 burstMember;

	for(i = 0; i < SANCTUM_PARTICLE_COUNT; i++)
		fdParticleUpdateStub(&shop->effs[i]);
	burstMember = shop->burstMember;
	if(burstMember != -1)
		fdSprSetAnimationColModesStub(
			shop->partyMemberIcons[burstMember],
			syRandom16Below(SANCTUM_BURST_COLOR_MODES));
}

/* The tune each service's burst plays. */
static const s8 sSanctumServiceCues[4] = {
	[SANCTUM_SERVICE_REVIVE] = MUS_RECOVERING_AFTER_COMBAT,
	[SANCTUM_SERVICE_CURE_POISON] = MUS_POISON_CURED,
	[SANCTUM_SERVICE_REPEL_EVIL] = MUS_EXORCISED,
	[SANCTUM_SERVICE_REMOVE_CURSE] = MUS_CURSE_LIFTED,
};

/* Plays the service's burst on a party member's icon: its tune, the field */
/* greyed, and SANCTUM_BURST_SPARKLES sparkles flying in at the icon as it */
/* flickers through colours, then the recovery sound. */
void mnSanctumRunMemberIconBurst(s32 member) {
	SHOP_STATE *shop;
	s8 savedCursorMode;
	POS position;
	FIELD_PARTICLE *effect;
	s32 effectIndex;

	shop = MenuState;
	savedCursorMode = shop->cursor.anchor->motion;
	shop->burstMember = -1;
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	sySndPlayStub(sSanctumServiceCues[shop->action]);
	mnShopSaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
	fdSprSetAnimationSpeedStub(shop->partyMemberIcons[member], 0);
	sySynch(20);
	sySetEvent(mnSanctumUpdateBurst, TASK_PASS_FRAME);

	/* The burst's centre, in 16.16 pixels: 12 above the party member's icon */
	position.x = toFIXED(shop->partyMemberX[member]);
	position.z = toFIXED(shop->partyMemberY[member]) - toFIXED(12);

	/* A sparkle starts every 3 frames; the burst's task moves them. */
	for(effectIndex = 0; effectIndex < SANCTUM_BURST_SPARKLES; effectIndex++) {
		effect = &shop->effs[effectIndex];
		fdParticleInitStub(effect, SPRITE_PARTICLE, position.x, position.z);
		fdParticleSetCallbackStub(effect, mnSanctumUpdateBurstSparkle);
		fdParticleSetAnimationStub(effect, PARTICLE_ANIMATION_FLICKER);
		fdSprSetAnimationColModesStub(
			effect->spr, syRandom16Below(SANCTUM_BURST_COLOR_MODES));
		effect->scaleY = FIXED_ONE * 7 / 10;
		effect->scaleX = FIXED_ONE * 7 / 10;
		sySynch(3);
		/* The icon starts to flicker with the sixth sparkle. */
		if(effectIndex == 5) {
			shop->burstMember = member;
		}
	}

	/* As the tune ends, every sparkle makes one last flight and is freed when
	 * it */
	/* arrives. */
	mnSndWaitForJingle();
	for(effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++) {
		if(shop->effs[effectIndex].active)
			shop->effs[effectIndex].state = SANCTUM_SPARKLE_LAST_FLIGHT;
	}

	sySynch(20);
	sySndPlayStub(SE_RECOVERY);
	shop->burstMember = -1;
	fdSprSetAnimationColModesStub(shop->partyMemberIcons[member], 0);
	sySynch(20);

	/* Free the sparkles still flying. */
	for(effectIndex = 0; effectIndex < SANCTUM_PARTICLE_COUNT; effectIndex++) {
		effect = &shop->effs[effectIndex];
		if(effect->active != 0)
			fdEffectClearOwnedSlotStub(effect);
	}

	syCloseEvent(mnSanctumUpdateBurst);
	fdSprSetAnimationSpeedStub(shop->partyMemberIcons[member],
							   ANIM_STEP_NORMAL);
	mnShopRestoreFieldPalette();
	sySynch(30);
	shop->cursor.anchor->motion = savedCursorMode;
}
