/* The inn: room prices, checking in and the night's sleep. Part of shop.c. */

/* The innkeeper's lines, in message order from MsgInnWelcome ("Welcome to our
   inn."). */
typedef enum INN_MESSAGE_ID {
	INN_MESSAGE_WELCOME,		  /* "Welcome to our inn." */
	INN_MESSAGE_ENJOY_STAY,		  /* "Please enjoy your stay." */
	INN_MESSAGE_NOT_ENOUGH_COINS, /* "I'm afraid you don't have enough." */
	INN_MESSAGE_COME_AGAIN,		  /* "Please come again." */
	INN_MESSAGE_SLEPT_WELL,		  /* "I hope you slept well." */
} INN_MESSAGE_ID;

/* Returns the night's price: the inn's price per guest times the party members
   still standing. */
Sint32 mnInnRoomPrice(Sint32 innId)
{
	SHOP_STATE *shop;
	Sint32		guestCount;
	Sint32		pricePerGuest;
	Sint32		memberIndex;

	shop = RamHeapSlots->menuState;
	guestCount = 0;
	pricePerGuest = InnPricesPerGuest[innId];
	for(memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
		if(syUnitGet(shop->partyMemberIds[memberIndex])->hp != 0)
			guestCount++;
	}
	return pricePerGuest * guestCount;
}

/* Returns the night's price at inn `innId` with the shop's state opened for it,
   for a script to quote before checking in. */
Sint32 mnInnQuoteRoomPrice(Sint32 innId)
{
	SHOP_STATE *shop;

	mnShopInitState();
	shop = RamHeapSlots->menuState;
	shop->shopType = SHOP_TYPE_WEAPONS;
	innId = mnInnRoomPrice(innId);
	mnShopReleaseState();
	return innId;
}

/* Runs an inn's counter: the innkeeper names the price, and on Yes the party
   sleeps. `innId` is the row of InnPricesPerGuest and `actorId` the innkeeper,
   whose portrait the lines show. */
Sint32 mnInnCheckIn(Sint32 innId, Sint32 actorId)
{
	SHOP_STATE *shop;
	ACTOR	   *innkeeper;
	WINDOW	   *innkeeperWindow;
	Sint32		roomPrice;
	Sint32		msgBase;

	mnShopInitState();
	shop = RamHeapSlots->menuState;
	shop->shopType = SHOP_TYPE_WEAPONS;

	innkeeper = fdActorGetByIdStub(actorId);
	shop->keeperGraphics = ((SPR *)innkeeper->sprite)->entries[0]->animationId;
	innkeeperWindow = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);

	roomPrice = mnInnRoomPrice(innId);
	mnMessageArgumentPushStub(roomPrice, MESSAGE_ARGUMENT_NUMBER);
	msgBase = (Sint32)MsgInnWelcome;
	mnShopShowKeeperMessage(msgBase);
	shop->moneyWindow = mnWindowCreateStub(
		0, 16, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();

	if(mnShopAskYesNo(0) != 0) {
		mnShopShowKeeperMessage(msgBase + INN_MESSAGE_COME_AGAIN);
		mnWindowCloseStub(shop->moneyWindow, 2);
	} else if((Uint32)roomPrice > MainRoutineStateWork.coins) {
		mnShopShowKeeperMessage(msgBase + INN_MESSAGE_NOT_ENOUGH_COINS);
		mnWindowCloseStub(shop->moneyWindow, 2);
	} else {
		mnWindowCloseStub(shop->moneyWindow, 2);
		mnShopShowKeeperMessage(msgBase + INN_MESSAGE_ENJOY_STAY);
		mnWindowCloseStub(innkeeperWindow, 2);
		mnInnPlaySleep(roomPrice);

		innkeeper = fdActorGetByIdStub(actorId);
		shop->keeperGraphics =
			((SPR *)innkeeper->sprite)->entries[0]->animationId;
		innkeeperWindow =
			mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
		mnShopShowKeeperMessage(msgBase + INN_MESSAGE_SLEPT_WELL);
	}

	mnWindowCloseStub(innkeeperWindow, 2);
	mnShopReleaseState();
	return 0;
}

/* Charges the room and restores the HP and PP of each party member still
   standing over a night: a fade to black, the inn's music and a fade back. */
void mnInnPlaySleep(Sint32 roomPrice)
{
	Sint16		 partyMemberIds[8];
	Sint32		 memberCount;
	Sint32		 memberIndex;
	UNIT		*unit;
	FIELD_STATE *state;

	memberCount = gmPartyListMembersStub(partyMemberIds);
	gmPartyAdjustCoinsStub(-roomPrice);

	for(memberIndex = 0; memberIndex < memberCount; memberIndex++) {
		unit = syUnitGet(partyMemberIds[memberIndex]);
		if(unit->hp != 0) {
			unit->hp = unit->hpMax;
			unit->pp = unit->ppMax;
			gmUnitRecalculateGaugesStub(partyMemberIds[memberIndex]);
		}
	}

	/* The night's fades take 60 ticks each, a window closing round the leader
	   as The Broken Seal's. */
	state = RamHeapSlots->fieldState;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	state->transitionFrame = 60;
	sySynch(20);
	fdScriptFinishScreenTransitionStub();
	fdScriptWaitTransitionTicksStub();
	sySoundPlayStub(MUS_OVERNIGHT_AT_THE_INN);
	mnSoundWaitForJingle();
	sySynch(10);
	fdScriptStartScreenTransitionStub();
	fdScriptWaitTransitionTicksStub();
	sySynch(30);
	FieldStateWork->transitionFrame = 16;
}
