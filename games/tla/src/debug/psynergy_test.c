/* The debug room Psynergy test: the shops, an inn and a Sanctum to try, the
   party's levels, items and Djinn to set, and spots for the field Psynergy The
   Lost Age adds, each answered by a small script: Scoop digs up a hole, a
   puddle, a Wicked Mace and a statue before a battle; Cyclone clears leaves and
   sends the leader away and back in a ring of light; Pound lights a torch and
   Parch dries a pool; Blaze startles Kraden; Sand drops Felix to the sand
   layer. */

#include "scene/script_layer.h"
#include "field/field.h"
#include "game/game.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "field/script_inline.h"
#include "field/call.h"
#include "field/field_mode_script.h"
#include "macros.h"
#include "item_ids.h"
#include "window/screens.h"
#include "window/window.h"
#include "scene/sparkle.h"
#include "shop/shop.h"

/* A Mad Plant leaps at the leader (fdMadPlantLeapAtLeader, through the room's
   own import). */
void syImportDebugMadPlantLeap(Sint32 spr, Sint32 x, Sint32 z);

/* The room's flags: the four Scoop spots dug, the Wicked Mace picked up, and
   whether Blaze has sent Kraden to his second spot. */
#define FLAG_HOLE_DUG		   FLAG_AREA_TEMP_00
#define FLAG_PUDDLE_DUG		   FLAG_AREA_TEMP_01
#define FLAG_WICKED_MACE_DUG   FLAG_AREA_TEMP_02
#define FLAG_STATUE_DUG		   FLAG_AREA_TEMP_03
#define FLAG_WICKED_MACE_TAKEN FLAG_AREA_TEMP_10
#define FLAG_KRADEN_MOVED	   FLAG_SCENE_TEMP_00

/* The room's own metatiles, as map.txt has them. */
typedef enum PSYNERGY_TEST_METATILE_BEHAVIOR {
	/* The tile marked "Ouch!" in Japanese, metatile (11, 2). */
	METATILE_BEHAVIOR_OUCH_TILE = 10,
	METATILE_BEHAVIOR_LV_TILE, /* the tile marked LV, metatile (11, 4) */
	/* marked with the kanji for treasure, metatile (11, 6) */
	METATILE_BEHAVIOR_TREASURE_TILE,
	/* the tile marked with a red cross, metatile (11, 8) */
	METATILE_BEHAVIOR_RED_CROSS_TILE,
	/* The floor of a dug hole, copied in from metatile (36, 28): its number is
	   the exit it leaves by (psynergy_test_exits.c), back to entrance 2. */
	METATILE_BEHAVIOR_HOLE_EXIT = 20,
	/* The four Scoop spots, corner to corner from (10, 10) to (13, 13). */
	METATILE_BEHAVIOR_HOLE_SPOT = 50,
	METATILE_BEHAVIOR_PUDDLE_SPOT,
	METATILE_BEHAVIOR_WICKED_MACE_SPOT,
	METATILE_BEHAVIOR_STATUE_SPOT,
	METATILE_BEHAVIOR_LEAVES =
		/* the patch of leaves at (2, 10), 5 by 4 metatiles */
	55,
	METATILE_BEHAVIOR_RING_LEAF,	  /* one of its leaves, metatile (5, 11) */
	METATILE_BEHAVIOR_MAD_PLANT_LEAF, /* its top left leaf, metatile (2, 10) */
	/* The pool of water, 2 by 4 metatiles at (16, 10). */
	METATILE_BEHAVIOR_POOL = 60,
	METATILE_BEHAVIOR_CHEST = 100 /* the chest, metatile (14, 9) */
} PSYNERGY_TEST_METATILE_BEHAVIOR;

/* The entrances Cyclone's spots bring the party back in by: there the leader
   leaves in a ring of light, or arrives in one (scInit). */
enum { ENTRANCE_DEPART_IN_RING = 6, ENTRANCE_ARRIVE_IN_RING = 8 };

/* The room's actors that run its tests, numbered in the order of
   ActorPlacements after the party's four rows. */
typedef enum PSYNERGY_TEST_ACTOR_ID {
	/* The four shops, one for each side he is talked to from. */
	ACTOR_SHOPKEEPER = ACTOR_FIRST_PLACED,
	ACTOR_FALLEN_MAN, /* lies on the floor; talking to him does nothing */
	/* Lies on the floor too: the inn from the south, the Sanctum otherwise. */
	ACTOR_INNKEEPER,
	ACTOR_MESSAGE_BROWSER, /* the shops' and the Sanctum's lines */
	ACTOR_MARS_DJINNI,	   /* lies on its side; Mind Read hears it */
	ACTOR_WEEDS,		   /* the icon browser */
	ACTOR_SPROUT,		   /* ignores Force */
	/* A puddle frozen as the room starts: Isaac's name entry from the north,
	   the item picker from elsewhere; it ignores Force, Whirlwind, Catch,
	   Tremor and Grind. */
	ACTOR_ICE_PILLAR,
	ACTOR_FLAME, /* burns; Blaze at it does nothing */
	/* In Kraden's sprite, he answers Blaze as Daila's practising man. */
	ACTOR_KRADEN,
	ACTOR_PUDDLE,		 /* grows out of the second Scoop spot */
	ACTOR_WICKED_MACE,	 /* the item's icon, which drops onto the third */
	ACTOR_WINGED_STATUE, /* placed off the map; nothing moves it */
	ACTOR_TORCH			 /* Pound or A?? lights it */
} PSYNERGY_TEST_ACTOR_ID;

void dbPsynergyTestDigMetatiles(Sint32 stage, Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY);
void dbPsynergyTestGrowActor(Sint32 stage, Sint32 id, Sint32 x, Sint32 z);
void dbPsynergyTestDropActor(Sint32 stage, Sint32 id, Sint32 x, Sint32 z);
void dbPsynergyTestRaiseStatue(
	Sint32 stage, Sint32 x, Sint32 z, Sint32 row, Sint32 column);
void dbPsynergyTestStartRings(ACTOR *actor, SPARKLE_STATE *state);
void dbPsynergyTestStopRings(ACTOR *actor);

extern Uint32								   LightRingTiles[];
SPARKLE_STATE PsynergyTestLeaderRings BSS_DATA ALIGNED(16);
SPARKLE_STATE PsynergyTestTargetRings BSS_DATA ALIGNED(16);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* The script for actors and Psynergy the room leaves alone. */
void dbPsynergyTestNoOperation(void)
{
}

/* Opens the picker a full party drops an item with, and discards the choice. */
void dbPsynergyTestOpenItemPicker(void)
{
	Sint32 selectedSlot;
	Sint32 selectedUnit;

	mnItemPickerRunToDrop(&selectedUnit, &selectedSlot);
}

/* The name entry screen for Isaac. */
void dbPsynergyTestEditIsaacName(void)
{
	mnNameEntryEditMemberName(ISAAC);
}

/* The script for Force, Whirlwind and Catch cast at the ice pillar. */
void dbPsynergyTestIgnorePsynergy(void)
{
}

/* Closes the finished message windows, opens the message in the window a
   shopkeeper's lines open (mnShopShowKeeperMessage) and waits until it is typed
   out, then a tick more. */
void dbPsynergyTestShowMessageAndWait(Sint32 msg)
{
	mnMessageCloseFinishedWindows();
	mnMessageOpenWindow(
		msg, 5, 0, MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	sySynch(1);
}

/* Browses `cnt` messages from `msg`: A or Down for the next, Up for the
   one before, B to close. */
void dbPsynergyTestBrowseMessageRange(Sint32 msg, Sint32 cnt)
{
	Sint32	i;
	WINDOW *win;

	win = mnPortraitOpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
	for(i = 0; i < cnt; i++) {
		/* Sample name, item and number substitutions before each message:
		   Garet, the Shamshir and 123456. */
		mnMessageArgumentPush(GARET, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(ITEM_SHAMSHIR, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPush(123456, MESSAGE_ARGUMENT_NUMBER);
		dbPsynergyTestShowMessageAndWait(msg);
		for(;;) {
			if(PerWork.data & KEY_B)
				goto done;
			if((PerWork.data & KEY_A) || (PerWork.data & KEY_DOWN)) {
				msg++;
				break;
			}
			if(PerWork.data & KEY_UP) {
				msg--;
				break;
			}
			if(PerWork.data == 0)
				sySynch(1);
			else
				break;
		}
	}
done:
	mnMessageCloseFinishedWindows();
	mnWindowClose(win, 2);
}

/* The shop and sanctum message browsers: talking to ACTOR_MESSAGE_BROWSER from
   each side. */
void dbPsynergyTestBrowseWeaponShopMessages(void)
{
	dbPsynergyTestBrowseMessageRange(
		(Sint32)MsgWeaponShopWelcome, SHOP_MESSAGES);
}

/* Browses the armor shop's messages. */
void dbPsynergyTestBrowseArmorShopMessages(void)
{
	dbPsynergyTestBrowseMessageRange(
		(Sint32)MsgArmorShopWelcome, SHOP_MESSAGES);
}

/* Browses the item shop's messages. */
void dbPsynergyTestBrowseItemShopMessages(void)
{
	dbPsynergyTestBrowseMessageRange((Sint32)MsgItemShopWelcome, SHOP_MESSAGES);
}

/* Browses the Sanctum's messages. */
void dbPsynergyTestBrowseSanctumMessages(void)
{
	dbPsynergyTestBrowseMessageRange(
		(Sint32)MsgWelcomeWearyWanderers, SANCTUM_MESSAGES);
}

/* The shops: talking to ACTOR_SHOPKEEPER from each side opens Daila's weapon,
   armor or item shop or Izumo's general store. Each passes on the keeper's
   actor id. */
void dbPsynergyTestOpenWeaponShop(Sint32 keeper)
{
	mnShopRun(SHOP_DAILA_WEAPONS, keeper);
}

/* Opens Daila's armor shop. */
void dbPsynergyTestOpenArmorShop(Sint32 keeper)
{
	mnShopRun(SHOP_DAILA_ARMOR, keeper);
}

/* Opens Daila's item shop. */
void dbPsynergyTestOpenItemShop(Sint32 keeper)
{
	mnShopRun(SHOP_DAILA_ITEMS, keeper);
}

/* Opens Izumo's general store. */
void dbPsynergyTestOpenGeneralStore(Sint32 keeper)
{
	mnShopRun(SHOP_IZUMO, keeper);
}

/* Opens Daila's inn. */
void dbPsynergyTestOpenInn(Sint32 keeper)
{
	mnInnCheckIn(DAILA_INN, keeper);
}

/* The script for talking to ACTOR_FALLEN_MAN, which does nothing. */
void dbPsynergyTestIgnoreTalk(void)
{
}

/* Raises Felix, Jenna and Sheba ten levels: examining
   METATILE_BEHAVIOR_LV_TILE. */
void dbPsynergyTestRaisePartyLevels(void)
{
	/* "You went up a level!" */
	mnMessageShowCentered((Sint32)MsgYouWentUp, FIELD_MESSAGE_ZOOM);
	gmUnitLevelUpTo(FELIX, syUnitGet(FELIX)->level + 10);
	gmUnitLevelUpTo(JENNA, syUnitGet(JENNA)->level + 10);
	gmUnitLevelUpTo(SHEBA, syUnitGet(SHEBA)->level + 10);
}

/* Gives 777,777 coins and ten each of Game Tickets and Lucky Medals: examining
   METATILE_BEHAVIOR_TREASURE_TILE. */
void dbPsynergyTestGiveTreasureTestStock(void)
{
	Sint32 i;

	/* "You got treasure!" */
	mnMessageShowCentered((Sint32)MsgYouGotTreasure, FIELD_MESSAGE_ZOOM);
	MainRoutineStateWork.coins = 777777;
	for(i = 0; i < 10; i++) {
		gmInventoryAddItem(FELIX, ITEM_GAME_TICKET);
		gmInventoryAddItem(FELIX, ITEM_LUCKY_MEDAL);
	}
	gmUnitCalc(FELIX);
	gmUnitCalc(JENNA);
	gmUnitCalc(SHEBA);
}

/* Takes HP and PP from Felix, Jenna and Sheba, then poisons and haunts Felix
   and binds Jenna and gives her deadly poison: examining
   METATILE_BEHAVIOR_OUCH_TILE. */
void dbPsynergyTestDamageAndAfflictParty(void)
{
	UNIT *felix;
	UNIT *jenna;

	/* "The body is being torn apart." */
	mnMessageShowCentered((Sint32)MsgTheBodyIs, FIELD_MESSAGE_ZOOM);
	scFieldUnitAddHp(FELIX, -100);
	scFieldUnitAddHp(JENNA, -100);
	scFieldUnitAddHp(SHEBA, -33);
	gmUnitAddPp(FELIX, -50);
	gmUnitAddPp(JENNA, -40);
	gmUnitAddPp(SHEBA, -35);
	felix = syUnitGet(FELIX);
	felix->poison = POISON_NORMAL;
	felix->haunt = 1;
	jenna = syUnitGet(JENNA);
	jenna->curse = CURSE_BOUND;
	jenna->poison = POISON_VENOM;
	gmUnitCalc(FELIX);
	gmUnitCalc(JENNA);
	gmUnitCalc(SHEBA);
}

/* Gives `owner` the Djinni `djinn` of `element` (its DJINNI_INDEX) and sets its
   flag; with `activate` 1 the Djinni is also set. */
void dbPsynergyTestGiveDjinni(
	Sint32 owner, Sint32 element, Sint32 djinn, Sint32 activate)
{
	fdScriptSetFlag(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn));
	gmDjinnGive(owner, element, djinn);
	if(activate == 1)
		gmDjinnSet(owner, element, djinn);
}

/* Gives and sets Djinn: Felix seven of Venus, Jenna seven of Mars and Sheba six
   of Mercury: examining METATILE_BEHAVIOR_RED_CROSS_TILE. */
void dbPsynergyTestGivePartyDjinn(void)
{
	/* "You got a Djinni!" */
	mnMessageShowCentered((Sint32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
	dbPsynergyTestGiveDjinni(
		FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT), 1);
	dbPsynergyTestGiveDjinni(
		FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE), 1);
	dbPsynergyTestGiveDjinni(
		FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_QUARTZ), 1);
	dbPsynergyTestGiveDjinni(
		FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_VINE), 1);
	dbPsynergyTestGiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP), 1);
	dbPsynergyTestGiveDjinni(
		FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND), 1);
	dbPsynergyTestGiveDjinni(
		FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FORGE), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_CORONA), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FLASH), 1);
	dbPsynergyTestGiveDjinni(
		JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH), 1);
	dbPsynergyTestGiveDjinni(
		SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ), 1);
	dbPsynergyTestGiveDjinni(
		SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SLEET), 1);
	dbPsynergyTestGiveDjinni(
		SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST), 1);
	dbPsynergyTestGiveDjinni(
		SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SPRITZ), 1);
	dbPsynergyTestGiveDjinni(
		SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_HAIL), 1);
	dbPsynergyTestGiveDjinni(
		SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_TONIC), 1);
	gmUnitCalc(FELIX);
	gmUnitCalc(JENNA);
	gmUnitCalc(SHEBA);
}

/* While Felix is Sand he reads his ground height from layer 2, and from layer 0
   again once Sand fades. */
void dbPsynergyTestSetSandLayer(void)
{
	fdActorGetById(FELIX)->layer = 2;
}

/* Puts Felix back on layer 0 as Sand fades. */
void dbPsynergyTestClearSandLayer(void)
{
	fdActorGetById(FELIX)->layer = 0;
}

/* As Blaze reaches him, Kraden shouts and jumps to the other of his two spots,
   as Daila's practising man would (scDailaRoomsPracticeShout, which Daila never
   runs). */
void dbPsynergyTestPracticeShout(Sint32 unused, Sint32 actor)
{
	scFieldActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	scFieldActorJump(actor, 6, 15);
	scFieldActorJump(actor, 6, 15);
	syFieldScriptWait(10);
	if(fdScriptGetFlag(FLAG_KRADEN_MOVED) == 0) {
		syFieldScriptSetMessageId((Sint32)MsgDailaRoomsGah);
		/* "GAH!" */
		fdScriptShowMessage(actor, 0);
		fdActorWalkToAndWait(actor, 360, 72);
		fdActorFaceDirection(actor, FACING_SOUTH, 0);
		fdScriptSetFlag(FLAG_KRADEN_MOVED);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgDailaRoomsYarg);
		/* "YARG!" */
		fdScriptShowMessage(actor, 0);
		fdActorWalkToAndWait(actor, 392, 104);
		fdActorFaceDirection(actor, FACING_WEST, 0);
		fdScriptClearFlag(FLAG_KRADEN_MOVED);
	}
	syFieldScriptWait(20);
}

/* Once Blaze has ended, Kraden turns to Felix, startled, as Daila's practising
   man would (scDailaRoomsStartleAtTheLeader). */
void dbPsynergyTestStartleAfterBlaze(Sint32 unused, Sint32 actor)
{
	fdActorFaceActor(actor, FELIX, 0);
	syFieldScriptWait(10);
	scFieldActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
	scFieldActorJump(actor, 6, 15);
	scFieldActorJump(actor, 6, 15);
	syFieldScriptWait(10);
	syFieldScriptSetMessageId((Sint32)MsgDailaRoomsWhatAreYouDoing);
	/* "What are you doing???" */
	fdScriptShowMessage(actor, 0);
	syFieldScriptWait(10);
}

/* The same, once A?? (ABILITY_ARROW, "A??" in the game's text) has ended. */
void dbPsynergyTestStartleAgain(Sint32 unused, Sint32 actor)
{
	fdActorFaceActor(actor, FELIX, 0);
	syFieldScriptWait(10);
	scFieldActorShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
	scFieldActorJump(actor, 6, 15);
	scFieldActorJump(actor, 6, 15);
	syFieldScriptWait(10);
	syFieldScriptSetMessageId((Sint32)MsgDailaRoomsWhatAreYouDoing);
	/* "What are you doing???" */
	fdScriptShowMessage(actor, 0);
	syFieldScriptWait(10);
}

/* Once A?? has ended at the torch, it lights, and the 4 by 6 block of metatiles
   at (57, 16) is copied over the one at (25, 0). */
void dbPsynergyTestLightTorch(void)
{
	Sint32 dstX;
	Sint32 dstY;

	fdActorSetAnimation(ACTOR_TORCH, 2);
	syFieldScriptWait(10);
	dstX = 25;
	dstY = 0;
	fdMapGridCopyMetatiles(57, 16, 4, 6, dstX, dstY);
}

/* Pound on the torch does the same as its fist lands, without the wait. */
void dbPsynergyTestPoundTorch(Sint32 stage)
{
	Sint32 dstX;
	Sint32 dstY;

	if(stage == POUND_STAGE_LANDED) {
		fdActorSetAnimation(ACTOR_TORCH, 2);
		dstX = 25;
		dstY = 0;
		fdMapGridCopyMetatiles(57, 16, 4, 6, dstX, dstY);
	}
}

/* Cyclone at the patch of leaves blows them away: each metatile of the patch it
   touched takes its picture from the same place of the bare block at (34,
   26). */
void dbPsynergyTestBlowAwayLeaves(void)
{
	Sint32 winX;
	Sint32 winZ;
	Sint32 layer;

	winX = 2;
	winZ = 10;
	layer = 0;
	fdTileCopiesCopyTouchedMetatiles(34, 26, 5, 4, winX, winZ, layer);
}

/* Cyclone at METATILE_BEHAVIOR_RING_LEAF blows the leaves away too, then brings
   the party back in by ENTRANCE_DEPART_IN_RING. */
void dbPsynergyTestDepartInRing(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(34, 26, 5, 4, 2, 10, 0);
	fdScriptSetDestination(SCENE_PSYNERGY_TEST, ENTRANCE_DEPART_IN_RING);
}

/* Cyclone at METATILE_BEHAVIOR_RED_CROSS_TILE brings the party back in by
   ENTRANCE_ARRIVE_IN_RING. */
void dbPsynergyTestArriveInRing(void)
{
	fdScriptSetDestination(SCENE_PSYNERGY_TEST, ENTRANCE_ARRIVE_IN_RING);
}

/* Cyclone at METATILE_BEHAVIOR_MAD_PLANT_LEAF: a Mad Plant leaps at the leader,
   and a battle with no escape starts,
   ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND's first formation, one or two
   Giant Bats. */
void dbPsynergyTestFightMadPlant(void)
{
	syImportDebugMadPlantLeap(SPRITE_MAD_PLANT, 40, 168);
	MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
}

/* The four Scoop spots, the grass patches set corner to corner east of the
   leaves, each run while its flag is clear. This one opens a hole, whose floor
   is METATILE_BEHAVIOR_HOLE_EXIT. */
void dbPsynergyTestDigUpHole(Sint32 stage)
{
	fdScriptSetFlag(FLAG_HOLE_DUG);
	dbPsynergyTestDigMetatiles(stage, 36, 28, 1, 1, 10, 10);
}

/* The Scoop spot that digs up a puddle, which grows out of the ground. */
void dbPsynergyTestDigUpPuddle(Sint32 stage)
{
	fdScriptSetFlag(FLAG_PUDDLE_DUG);
	dbPsynergyTestGrowActor(stage, ACTOR_PUDDLE, 184, 184);
}

/* The Scoop spot that digs up a Wicked Mace, which drops onto it. */
void dbPsynergyTestDigUpWickedMace(Sint32 stage)
{
	fdScriptSetFlag(FLAG_WICKED_MACE_DUG);
	dbPsynergyTestDropActor(stage, ACTOR_WICKED_MACE, 200, 200);
}

/* The Scoop spot that raises a statue, then a battle. */
void dbPsynergyTestDigUpStatue(Sint32 stage)
{
	fdScriptSetFlag(FLAG_STATUE_DUG);
	dbPsynergyTestRaiseStatue(
		stage, 216, 216, ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
}

/* Talking to the Wicked Mace picks it up. */
void dbPsynergyTestPickUpItem(void)
{
	fdScriptPickUpItem(ACTOR_WICKED_MACE,
		fdActorGet(ACTOR_WICKED_MACE)->iconItem, FLAG_WICKED_MACE_TAKEN);
}

/* Parch at the pool dries it, copying in the dry metatiles at (48, 26). */
void dbPsynergyTestParchMetatiles(void)
{
	Sint32 dstX;
	Sint32 dstY;

	dstX = 16;
	dstY = 10;
	fdMapGridCopyMetatiles(48, 26, 2, 4, dstX, dstY);
}

/* Tremor's script, which does nothing. */
void dbPsynergyTestIgnoreTremor(void)
{
}

/* Grind's script, which does nothing. */
void dbPsynergyTestIgnoreGrind(void)
{
}

/* Talking to Mia: rings of light rise around her and the leader for 100
   ticks. */
void dbPsynergyTestShowRings(Sint32 actor)
{
	dbPsynergyTestStartRings(fdActorGet(actor), &PsynergyTestTargetRings);
	dbPsynergyTestStartRings(fdActorGet(MainRoutineStateWork.leaderActorId),
		&PsynergyTestLeaderRings);
	sySynch(100);
	dbPsynergyTestStopRings(fdActorGet(actor));
	dbPsynergyTestStopRings(fdActorGet(MainRoutineStateWork.leaderActorId));
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets the shops' rare stock, freezes the puddle, lays the fallen actors down,
   puts back what the Scoop spots and the Wicked Mace have already done, and
   sends the leader away or brings it in by ring at the entrances for them. */
Sint32 scInit(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->sceneAttributeHigh = 0;
	state->sceneAttributeLow = 0;
	/* How many of each rare item the shops may sell, as in The Broken Seal's
	   Menu test, with a Fire Brand where it had a Masamune. */
	gmItemAdjustRareItemStock(ITEM_SHAMSHIR, 1);
	gmItemAdjustRareItemStock(ITEM_SHAMSHIR, 1);
	gmItemAdjustRareItemStock(ITEM_SHAMSHIR, 1);
	gmItemAdjustRareItemStock(ITEM_SILVER_BLADE, 1);
	gmItemAdjustRareItemStock(ITEM_SILVER_BLADE, 1);
	gmItemAdjustRareItemStock(ITEM_FIRE_BRAND, 1);
	gmItemAdjustRareItemStock(ITEM_CHINA_DRESS, 1);
	gmItemAdjustRareItemStock(ITEM_COCKTAIL_DRESS, 1);
	gmItemAdjustRareItemStock(ITEM_BLESSED_ROBE, 1);
	gmItemAdjustRareItemStock(ITEM_ORACLES_ROBE, 1);
	gmItemAdjustRareItemStock(ITEM_DRAGON_SHIELD, 1);
	gmItemAdjustRareItemStock(ITEM_VAMBRACE, 1);
	gmItemAdjustRareItemStock(ITEM_SPIRIT_ARMLET, 1);
	gmItemAdjustRareItemStock(ITEM_WARRIORS_HELM, 1);
	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_FAST;
	fdActorSetAnimation(ACTOR_ICE_PILLAR, 2);
	fdActorSetAnimation(ACTOR_FLAME, 2);
	Call3((void (*)())fdActorFallOverWithAnimationById, ACTOR_FALLEN_MAN,
		FACING_WEST, ANIM_STILL);
	Call2((void (*)())fdActorFallOverById, ACTOR_INNKEEPER, FACING_WEST);
	fdActorFallOverById(ACTOR_MARS_DJINNI, FACING_EAST);
	if(fdScriptGetFlag(FLAG_HOLE_DUG))
		fdMapGridCopyMetatiles(36, 28, 1, 1, 10, 10);
	if(fdScriptGetFlag(FLAG_PUDDLE_DUG))
		fdActorSetPos(ACTOR_PUDDLE, 184, 184);
	if(fdScriptGetFlag(FLAG_WICKED_MACE_DUG) &&
		!fdScriptGetFlag(FLAG_WICKED_MACE_TAKEN))
		fdActorSetPos(ACTOR_WICKED_MACE, 200, 200);
	fdActorSetItemIcon(fdActorGet(ACTOR_WICKED_MACE), ITEM_WICKED_MACE);
	if(MainRoutineStateWork.entrance == ENTRANCE_DEPART_IN_RING)
		fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
	if(MainRoutineStateWork.entrance == ENTRANCE_ARRIVE_IN_RING)
		fdActorArriveInRing(MainRoutineStateWork.leaderActorId);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Opens the icon browser. */
void dbPsynergyTestOpenIconBrowser(void)
{
	mnDebugBrowserIcons();
}

/* Opens the Sanctum. */
void dbPsynergyTestOpenSanctum(Sint32 keeper)
{
	mnSanctumRun(keeper);
}

/* As the ground opens, copies the metatiles of the hole in. */
void dbPsynergyTestDigMetatiles(Sint32 stage, Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY)
{
	if(stage == SCOOP_STAGE_DUG)
		fdMapGridCopyMetatiles(srcX, srcY, width, height, dstX, dstY);
}

/* As the ground opens, places the actor at (x, z) in pixels with no size;
   twenty ticks later, grows it to full size. */
void dbPsynergyTestGrowActor(Sint32 stage, Sint32 id, Sint32 x, Sint32 z)
{
	ACTOR *actor;
	Sint32 scale;

	actor = fdActorGet(id);
	if(stage == SCOOP_STAGE_DUG) {
		fdActorSetPos(id, x, z);
		actor->scaleX = 0;
		actor->scaleY = 0;
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		while(actor->scaleX < toFIXED(1)) {
			sySynch(1);
			scale = actor->scaleX;
			scale += toFIXED(0.02);
			actor->scaleX = scale;
			actor->scaleY = scale;
		}
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
}

/* Twenty ticks after the ground opens, drops the actor onto (x, z) in pixels
   from 32 pixels up. */
void dbPsynergyTestDropActor(Sint32 stage, Sint32 id, Sint32 x, Sint32 z)
{
	ACTOR *actor;

	actor = fdActorGet(id);
	if(stage == SCOOP_STAGE_DUG_UP) {
		fdActorSetPos(id, x, z);
		actor->y.fixed = toFIXED(32);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->gravity = toFIXED(0.09);
		sySynch(50);
	}
}

/* As the ground opens, places a brown boulder at (x, z) in pixels; twenty
   ticks later, grows a winged statue there from two fifths to full size while
   it turns, turns it opposite the leader's facing and starts a battle with the
   formation at `column` of encounter zone `row`. */
void dbPsynergyTestRaiseStatue(
	Sint32 stage, Sint32 x, Sint32 z, Sint32 row, Sint32 column)
{
	ACTOR *actor;

	if(stage == SCOOP_STAGE_DUG) {
		actor =
			scFieldActorCreate(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
		if(actor != NULL) {
			fdActorSetSprFlags(actor, 0);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		}
	}
	if(stage == SCOOP_STAGE_DUG_UP) {
		actor = (scFieldActorCreate(
			SPRITE_WINGED_STATUE, toFIXED(x), 0, toFIXED(z)));
		if(actor != NULL) {
			Sint32 scale;

			fdActorSetSprFlags(actor, 0);
			actor->velY = toFIXED(6);
			actor->scaleX = toFIXED(2) / 5;
			actor->scaleY = toFIXED(2) / 5;
			while(actor->scaleX < toFIXED(1)) {
				sySynch(1);
				scale = actor->scaleX + toFIXED(0.02);
				actor->facing += FACING_STEP;
				actor->scaleX = scale;
				actor->scaleY = scale;
			}
			actor->scaleX = toFIXED(1);
			actor->scaleY = toFIXED(1);
			actor->facing =
				fdActorGet(MainRoutineStateWork.leaderActorId)->facing +
				DEGtoANG(180);
			fdRequestBattleFromEncounterTable(row, column);
		}
	}
}

/* Each tick of an actor's rings: a ring climbs RING_CLIMB pixels up the actor,
   swelling and shrinking as it goes, and the next starts with a chime
   RING_INTERVAL ticks later, two at a time, while the actor's colours cycle.
   Once stopped, the rings climb out and the actor's colours come back. */
void dbPsynergyTestUpdateRings(ACTOR *actor)
{
	VECTOR3		   pos;
	Sint32		   x, z;
	FIXED		   sine;
	Sint32		   i, j;
	SPARKLE_STATE *state = (SPARKLE_STATE *)actor->watchedActor;
	Uint8		  *cnt = &actor->sceneByte;
	EFFECT_SPR	  *spr;

	/* Every other tick, the next of the normal colours and the five swaps. */
	if(*cnt & 1)
		fdActorSetSprColMode(actor,
			(Uint8)((Uint32)((COUNTER_BITS *)cnt)->rest %
				(SPRITE_COLOR_BRG + 1)));
	if(actor->sceneCnt == 0) {
		if((*cnt & (RING_INTERVAL - 1)) == 0) {
			fdScriptPlaySnd(SE_RISING_RINGS);
			state->heights[state->nextPair] = 0;
			state->nextPair ^= 1;
		}
	} else if(actor->sceneCnt == 1) {
		if(state->heights[0] >= RING_CLIMB && state->heights[1] >= RING_CLIMB) {
			fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
			syVRAMCacheFreeEntry(state->vramEntry);
			actor->update = NULL;
			return;
		}
	}
	(*cnt)++;
	for(i = 0; i < 2; i++) {
		/* Its size: half a turn of sine over the climb. */
		sine = syMathSin(state->heights[i] * (DEGtoANG(180) / RING_CLIMB));
		if(state->heights[i] >= 0) {
			if(state->heights[i] < RING_CLIMB) {

				SET_POSITION(&pos, actor->x.fixed,
					actor->y.fixed + toFIXED(state->heights[i]),
					actor->z.fixed);
				fdCameraWorldToScreen(&pos);
				x = pos.x;
				j = 0;
				z = pos.z;
				spr = &state->sprs[i * 2];
				for(; j <= 1; j++) {
					spr->x = x;
					spr->y = z;
					spr->scaleX = sine;
					spr->scaleY = sine;
					if(j == 0) {
						spr->depth = fdActorGetSprDepth(actor) - 1;
					} else {
						spr->depth = fdActorGetSprDepth(actor) + 1;
						spr->y -= toFIXED(1);
						spr->scaleY = -spr->scaleY;
					}
					fdEffectSprDraw(spr);
					spr++;
				}
				state->heights[i]++;
			}
		}
	}
}

/* clang-format off */
#include "../scene/shared/light_ring_tiles.c"

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 64, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 112, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{2, CONDITION_ALWAYS, 64, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{3, CONDITION_ALWAYS, 112, 0, 120, FACING_NORTH, 0, 16, 16, 304, 224, 0},
	{4, CONDITION_ALWAYS, 344, 0, 104, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{5, CONDITION_ALWAYS, 88, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_DEPART_IN_RING, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{7, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_ARRIVE_IN_RING, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

#include "../scene/shared/psynergy_test_exits.c"

/* clang-format on */

/* The room's people: the party's four rows, then PSYNERGY_TEST_ACTOR_ID's. */
ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 40,
		FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 72,
		FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 104,
		FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 136,
		FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 88, 0, 40, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 82, 0, 72, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 88, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 40, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0,
		72, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136,
		0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136,
		0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392,
		0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64,
		0, -64, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0,
		104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

/* The room's scripts: talking to its people, Mind Read on them, the metatiles
   examined, and the field Psynergy each spot answers. */
CUE SceneCues[] = {
	{CUE_TALK, JENNA, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestNoOperation)},
	{CUE_TALK, IVAN, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestNoOperation)},
	{CUE_TALK, MIA, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestShowRings)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenWeaponShop)},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenArmorShop)},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenItemShop)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenGeneralStore)},
	{CUE_TALK, ACTOR_FALLEN_MAN, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnoreTalk)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenInn)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenSanctum)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestBrowseWeaponShopMessages)},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestBrowseArmorShopMessages)},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestBrowseItemShopMessages)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestBrowseSanctumMessages)},
	{CUE_TALK, ACTOR_MARS_DJINNI, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestNoOperation)},
	{CUE_TALK, ACTOR_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenIconBrowser)},
	{CUE_TALK, ACTOR_SPROUT, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestNoOperation)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestEditIsaacName)},
	{CUE_TALK, ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestOpenItemPicker)},
	{CUE_TALK, ACTOR_WICKED_MACE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestPickUpItem)},
/* Mind Read hears what Eastern Alhafra's men say after Briggs's attack. The
   Japanese table has one more row, for the innkeeper, which the later editions
   dropped, whose line is "Any time you like; I've been kept waiting till I'm
   worn out." */
#if !PSYNERGY_TEST_DROPS_ANY_TIME_YOU_LIKE_LINE
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgAnyTimeYouLike)},
#endif
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgOooogOw)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_MAN, CONDITION_ALWAYS,
		MSGNO(MsgWhyDoThese)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgStupidBriggs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSAGE_BROWSER,
		CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MARS_DJINNI, CONDITION_ALWAYS,
		MSGNO(MsgIJustCame)},
	{CUE_EXIT, METATILE_BEHAVIOR_HOLE_EXIT, CONDITION_ALWAYS,
		METATILE_BEHAVIOR_HOLE_EXIT},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_OUCH_TILE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestDamageAndAfflictParty)},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_LV_TILE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestRaisePartyLevels)},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_TREASURE_TILE,
		CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestGiveTreasureTestStock)},
	{EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_RED_CROSS_TILE,
		CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestGivePartyDjinn)},
	{EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_CHEST, CONDITION_ALWAYS,
		FIND_COINS(333)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SPROUT, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnorePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnorePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnorePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnorePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestNoOperation)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_TORCH,
		CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestPoundTorch)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestSetSandLayer)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestClearSandLayer)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_FLAME, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestNoOperation)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_KRADEN,
		CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestPracticeShout)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
		ACTOR_KRADEN, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestStartleAfterBlaze)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestNoOperation)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_ARROW, PSYNERGY_PHASE_AFTER_END),
		ACTOR_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestStartleAgain)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_ARROW, PSYNERGY_PHASE_AFTER_END),
		ACTOR_TORCH, CONDITION_ALWAYS, SCRIPT_REF(dbPsynergyTestLightTorch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_HOLE_SPOT, FLAG_HOLE_DUG,
		SCRIPT_REF(dbPsynergyTestDigUpHole)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_PUDDLE_SPOT, FLAG_PUDDLE_DUG,
		SCRIPT_REF(dbPsynergyTestDigUpPuddle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_WICKED_MACE_SPOT, FLAG_WICKED_MACE_DUG,
		SCRIPT_REF(dbPsynergyTestDigUpWickedMace)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_STATUE_SPOT, FLAG_STATUE_DUG,
		SCRIPT_REF(dbPsynergyTestDigUpStatue)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_POOL, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestParchMetatiles)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_LEAVES, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestBlowAwayLeaves)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_RING_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestDepartInRing)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_RED_CROSS_TILE, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestArriveInRing)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_MAD_PLANT_LEAF, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestFightMadPlant)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_WEEDS, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnoreTremor)},
	{PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnoreTremor)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnoreGrind)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(dbPsynergyTestIgnoreGrind)},
	{CUE_TABLE_END},
};

/* The rings' start and stop. */

/* Starts an actor's rings, kept in `state`: loads their tiles and hooks
   dbPsynergyTestUpdateRings to the actor. */
void dbPsynergyTestStartRings(ACTOR *actor, SPARKLE_STATE *state)
{
	EFFECT_SPR *spr;
	Sint32		tile;
	Sint32		row;
	Sint32		col;
	Sint32		ofs;

	state->vramEntry = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(state->vramEntry, 8 * TILE_SIZE_4BPP, LightRingTiles);
	state->nextPair = 0;
	state->heights[0] = RING_NOT_STARTED;
	state->heights[1] = RING_NOT_STARTED;
	state->actor = actor;
	actor->sceneCnt = 0;
	actor->sceneByte = 0;
	actor->update = dbPsynergyTestUpdateRings;
	tile = VramCacheEntries[state->vramEntry].ofs / TILE_SIZE_4BPP;
	actor->watchedActor = (union FIELD_OBJECT *)state;
	for(row = 0; row <= 1; row++) {
		col = 1;
		ofs = (row * 2) * sizeof(EFFECT_SPR);
		spr = (EFFECT_SPR *)(ofs + (Sint32)state);
		for(; col >= 0; col--) {
			fdEffectSprInit(spr, 16, 16, OBJ_SIZE_32X16, tile);
			spr->oam.fullCol = 0;
			spr->oam.pal = 0;
			spr->oam.pri = fdActorGetSprPriority(actor);
			spr++;
		}
	}
}

/* Lets an actor's rings climb out. */
void dbPsynergyTestStopRings(ACTOR *actor)
{
	actor->sceneCnt = 1;
}
