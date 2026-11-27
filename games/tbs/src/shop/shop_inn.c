/* @file */
/* The inn: room prices, checking in and the night's sleep. */
/* Part of shop.c. */

#include "shop_internal.h"

/* The innkeeper's lines, in message order from MsgInnWelcome. */
typedef enum INN_MESSAGE_ID {
	INN_MESSAGE_WELCOME = MSGNO(MsgInnWelcome),
	INN_MESSAGE_ENJOY_YOUR_STAY,
	INN_MESSAGE_NOT_ENOUGH_COINS,
	INN_MESSAGE_COME_AGAIN,
	INN_MESSAGE_SLEPT_WELL,
} INN_MESSAGE_ID;

/* The coins a night costs each living guest, by inn: Vale, Vault, Bilibin, */
/* Kolima, Imil, Xian, Altin, Kalay, Lunpa, Tolbi, Suhalla, Lalivero, in the */
/* order of their area names. Unused: Tolbi's row, 9, costs nothing and no */
/* event opens it, since Tolbi's inns are always full */
/* (MsgTolbiInteriorsIsaacCheckedTheSignInnFull). */
/* clang-format off */
static const s8 sInnPricesPerGuest[12] = {
    2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25,
};
/* clang-format on */

/* Returns the night's price: the inn's price per guest times the party */
/* members still standing. */
s32 mnInnRoomPrice(s32 innId) {
	SHOP_STATE *shop = MenuState;
	s32 guestCount = 0;
	s32 pricePerGuest = sInnPricesPerGuest[innId];
	s32 memberIndex;

	for(memberIndex = 0; memberIndex < shop->partyMemberCnt; memberIndex++) {
		if(gmUnitGetStub(shop->partyMemberIds[memberIndex])->hp != 0)
			guestCount++;
	}
	return pricePerGuest * guestCount;
}

/* Runs an inn's counter: the innkeeper names the price, and on Yes the party */
/* sleeps. actorId is the innkeeper, whose portrait the lines show. */
s32 mnInnCheckIn(s32 innId, s32 actorId) {
	SHOP_STATE *inn;
	ACTOR *innkeeper;
	WINDOW *innkeeperWindow;
	u32 roomPrice;
	s32 messageBase;

	mnShopInitState();
	inn = MenuState;
	/* The innkeeper's lines are given as they are, as a weapon shop's are */
	/* (mnShopShowKeeperMessage); Xian's inn has lines of its own, as far past
	 * the */
	/* other inns' as the warriors' shops' are past the weapon shop's. */
	inn->shopType = SHOP_TYPE_WEAPONS;
	if(innId == INN_XIAN)
		inn->warriorShop = 1;

	innkeeper = fdActorGetByIdStub(actorId);
	inn->keeperGraphics = innkeeper->spr->entries[0]->anmId;
	innkeeperWindow = mnPortraitOpenWindowStub(inn->keeperGraphics, 0, 0, 0);

	roomPrice = mnInnRoomPrice(innId);
	mnMessageArgumentPushStub(roomPrice, MESSAGE_ARGUMENT_NUMBER);
	messageBase = MsgInnWelcome;
	/* "Welcome to our inn. It's (the price) coins for a room. Would you like to
	 */
	/* stay?" These are the inns' lines; Xian's says its own. */
	mnShopShowKeeperMessage(messageBase);
	inn->moneyWin = mnWindowCreateStub(0, 16, SHOP_MONEY_WINDOW_WIDTH, 4,
									   WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();

	if(mnShopAskYesNo(0) != 0) {
		/* "Please come again." */
		mnShopShowKeeperMessage(messageBase +
								(INN_MESSAGE_COME_AGAIN - INN_MESSAGE_WELCOME));
		mnWindowCloseStub(inn->moneyWin, 2);
	}
	else if(roomPrice > MainRoutineStateWork.holdings.coins) {
		/* "I'm afraid you don't have enough." */
		mnShopShowKeeperMessage(
			messageBase + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
		mnWindowCloseStub(inn->moneyWin, 2);
	}
	else {
		mnWindowCloseStub(inn->moneyWin, 2);
		/* "Please enjoy your stay." */
		mnShopShowKeeperMessage(
			messageBase + (INN_MESSAGE_ENJOY_YOUR_STAY - INN_MESSAGE_WELCOME));
		mnWindowCloseStub(innkeeperWindow, 2);
		mnInnPlaySleep(roomPrice);

		innkeeper = fdActorGetByIdStub(actorId);
		inn->keeperGraphics = innkeeper->spr->entries[0]->anmId;
		innkeeperWindow =
			mnPortraitOpenWindowStub(inn->keeperGraphics, 0, 0, 0);
		/* "I hope you slept well. Have a nice trip!" */
		mnShopShowKeeperMessage(messageBase +
								(INN_MESSAGE_SLEPT_WELL - INN_MESSAGE_WELCOME));
	}

	mnWindowCloseStub(innkeeperWindow, 2);
	mnShopReleaseState();
	return 0;
}

/* Charges the room and restores the HP and PP of each party member still */
/* standing over a night: a fade to black, the inn's music and a fade back. */
void mnInnPlaySleep(s32 roomPrice) {
	s16 partyMemberIds[8];
	s32 memberCount;
	s32 memberIndex;
	UNIT *unit;
	FIELD_STATE *state;

	memberCount = gmPartyListMembersStub(partyMemberIds);
	/* Charge once and restore only party members who are alive. */
	gmPartyAdjustCoinsStub(-roomPrice);

	for(memberIndex = 0; memberIndex < memberCount; memberIndex++) {
		unit = gmUnitGetStub(partyMemberIds[memberIndex]);
		if(unit->hp != 0) {
			unit->hp = unit->hpMax;
			unit->pp = unit->ppMax;
			gmUnitRecalculateGaugesStub(partyMemberIds[memberIndex]);
		}
	}

	/* The night's fades take 60 frames each. */
	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	state->transitionFrm = 60;
	sySynch(20);
	fdEventFinishScrTransitionStub();
	fdEventWaitTransitionFramesStub();
	sySndPlayStub(MUS_OVERNIGHT_AT_THE_INN);
	mnSndWaitForJingle();
	sySynch(10);
	fdEventStartScrTransitionStub();
	fdEventWaitTransitionFramesStub();
	sySynch(30);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
}
