/* Log rooms C (SceneId_LogRoomsC), a rooms map drawn in logs, holds only
   Kolima's rooms: the village's houses, shops and inn.

   Some of its exits join its own rooms, by the stairs up the inn and the weapon
   and armor shop. Five lead back out to the village (SceneId_Kolima) and one
   into Kolima Tunnel (SceneId_KolimaTunnel, LOG_ROOMS_C_TO_KOLIMA_TUNNEL).

   Like the people outside, the fifteen residents (ACTOR_FIRST_VILLAGER to
   ACTOR_LAST_VILLAGER) were turned into trees when the curse fell. Until it is
   lifted in Kolima Forest (FLAG_KOLIMA_CURSE_LIFTED), scGetActorPlacements
   gives them tree sprites that stand still. They answer only Mind Read, each
   with the thoughts of someone stuck as a tree ("Who caused all this? Someone
   must have tried to cut down Tret!").

   Once the curse is lifted they talk. They speak of the warriors said to have
   saved the village, of McCoy's halted palace and the lumberjacks' lost work,
   and of a father's secret treasure "hidden deep in the forest". The weapon,
   armor and item sellers and the innkeeper serve the party across their
   counters.

   Three bookcases hold books on the holy tree ("Tret and Laurel are the spirits
   of the holy trees protecting Kolima forest"), on trees and flowers and on
   logging. A jar holds a Lucky Medal and a barrel 7 coins.

   scInit opens with a window transition and draws one of two rooms into the
   same place on the map.

   Entered by its door from the village (LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH),
   it is the father's house, with its bookcase and rug and the father's family
   (ACTOR_FATHER_WITH_A_SECRET, ACTOR_WIFE_KEPT_FROM_THE_SECRET and
   ACTOR_CHILD_TOLD_THE_SECRET).

   It can also be entered from behind: by the village doorway the leader steps
   south into (LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH), or up the ladder from
   Kolima Tunnel (LOG_ROOMS_C_FROM_KOLIMA_TUNNEL). Then other metatiles are
   copied in, and it is a storeroom of crates and sacks with the ladder down in
   its floor. The family is put out of sight. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE LogRoomsC_Entrances[];
extern Uint32		  LogRoomsC_Exits[];
extern ACTOR_PLACE	  LogRoomsC_ActorPlacements[];
extern CUE			  LogRoomsC_Cues[];

/* The metatiles the jar, the barrel and the three bookcases answer. */
typedef enum LOG_ROOMS_C_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_LOG_ROOMS_C_JAR = 100,
	METATILE_BEHAVIOR_LOG_ROOMS_C_BARREL = 101,
	/* "The Holy Tree of Kolima Forest." */
	METATILE_BEHAVIOR_LOG_ROOMS_C_HOLY_TREE_BOOKCASE = 200,
	/* "Dictionary of Trees and Flowers." */
	METATILE_BEHAVIOR_LOG_ROOMS_C_TREES_BOOKCASE = 201,
	/* "The Art of Logging." */
	METATILE_BEHAVIOR_LOG_ROOMS_C_LOGGING_BOOKCASE = 202
} LOG_ROOMS_C_METATILE_BEHAVIOR;

/* Kolima's shops and inn, as mnShopRun and mnInnCheckIn number them. */
enum {
	KOLIMA_WEAPON_SHOP = 13,
	KOLIMA_ARMOR_SHOP = 14,
	KOLIMA_ITEM_SHOP = 15,
	KOLIMA_INN = 3
};

/* The residents, numbering LogRoomsC_ActorPlacements' rows from
   ACTOR_FIRST_PLACED. Their notes quote what they say once the curse is
   lifted. */
typedef enum KOLIMA_ROOM_ACTOR_ID {
	/* An old man with a cane. */
	ACTOR_OLD_MAN_HEARD_OF_WARRIORS = ACTOR_FIRST_PLACED,
	/* An old woman. */
	ACTOR_OLD_WOMAN_BEARDED_WARRIORS,
	/* The father with a secret, a burly grey-haired man: "You saved Kolima?
	   Wow! That makes me want to tell you a secret!" His thoughts: "It's hidden
	   deep in the forest, but I can't tell him that!" */
	ACTOR_FATHER_WITH_A_SECRET,
	/* His wife, a woman in a green dress. */
	ACTOR_WIFE_KEPT_FROM_THE_SECRET,
	/* Their son, a small boy with fair hair. */
	ACTOR_CHILD_TOLD_THE_SECRET,
	/* A strongman, stocky and muscular. */
	ACTOR_LUMBERJACK_FATHER,
	/* His daughter, a woman with long fair hair. */
	ACTOR_LUMBERJACKS_DAUGHTER,
	/* His son, a small boy with fair hair. */
	ACTOR_LUMBERJACKS_HUNGRY_SON,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	ACTOR_ITEM_MERCHANT,
	ACTOR_INNKEEPER,
	/* A woman in a pink dress and an apron. */
	ACTOR_VILLAGER_QUIET_VILLAGE,
	/* A strongman, stocky and muscular. */
	ACTOR_LUMBERJACK_FROM_IMIL,
	/* A man carrying a pack. */
	ACTOR_MERCHANT_GUEST,
	ACTOR_FIRST_VILLAGER = ACTOR_OLD_MAN_HEARD_OF_WARRIORS,
	ACTOR_LAST_VILLAGER = ACTOR_MERCHANT_GUEST
} KOLIMA_ROOM_ACTOR_ID;

/* Replaces the cursed residents' sprites and disables their movement and
   turning. */
static void scLogRoomsCTurnVillagersIntoTrees(ACTOR_PLACE *placement)
{
	Uint32 villagerIndex;

	villagerIndex = 0;
	{
		Uint8  talkFacing = TALK_FACING_KEEP;
		Sint32 behavior = MOVEMENT_TYPE_STAND;

		do {
			placement->talkFacing = talkFacing;
			placement->movement = behavior;
			placement->spr = SPRITE_TREE;
			/* The two residents drawn as SPRITE_BOY_WITH_FAIR_HAIR take the
			   small tree. */
			if(villagerIndex ==
					ACTOR_CHILD_TOLD_THE_SECRET - ACTOR_FIRST_VILLAGER ||
				villagerIndex ==
					ACTOR_LUMBERJACKS_HUNGRY_SON - ACTOR_FIRST_VILLAGER) {
				placement->spr = SPRITE_SMALL_TREE;
			}
			villagerIndex++;
			placement++;
		} while(villagerIndex < ACTOR_LAST_VILLAGER - ACTOR_FIRST_VILLAGER + 1);
	}
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return LogRoomsC_Entrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* The exits between the rooms, back to the village and into Kolima Tunnel. */
const Uint32 *scGetExits(void)
{
	return LogRoomsC_Exits;
}

/* Kolima's residents, turned into trees first while the curse lasts. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	ACTOR_PLACE *placements;

	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		scLogRoomsCTurnVillagersIntoTrees(LogRoomsC_ActorPlacements);
	}
	placements = LogRoomsC_ActorPlacements;
	fdAssignViewFlags(placements);
	return placements;
}

/* Kolima's weapon seller, once the curse is lifted, opens the shop when spoken
   to across the counter (facing north). */
static void scLogRoomsCTalkToWeaponMerchant(void)
{
	Uint32 facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if(FACING_IS_NORTH(facing)) {
		mnShopRun(KOLIMA_WEAPON_SHOP, ACTOR_WEAPON_MERCHANT);
	} else {
		fdScriptSetMessageId(MsgKolimaFuchinTempleOnOtherSide);
		/* "Fuchin Temple is on the other side of the river. It's quite..." */
		fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	}
	fdScriptEnd();
}

/* Kolima's armor seller, once the curse is lifted, opens the shop across the
   counter. */
static void scLogRoomsCTalkToArmorMerchant(void)
{
	Uint32 facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if(FACING_IS_NORTH(facing)) {
		mnShopRun(KOLIMA_ARMOR_SHOP, ACTOR_ARMOR_MERCHANT);
	} else {
		fdScriptSetMessageId(MsgKolimaGrandBridgeAcrossRiverPride);
		/* "The grand bridge across the river is the pride of Kolima." */
		fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	}
	fdScriptEnd();
}

/* Kolima's item seller, once the curse is lifted, opens the shop across the
   counter. */
static void scLogRoomsCTalkToItemMerchant(void)
{
	Uint32 facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if(FACING_IS_NORTH(facing)) {
		mnShopRun(KOLIMA_ITEM_SHOP, ACTOR_ITEM_MERCHANT);
	} else {
		fdScriptSetMessageId(MsgKolimaWhenWasTreeLearnedAppreciate);
		/* "When I was a tree, I learned to appreciate the sun." */
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
	}
	fdScriptEnd();
}

/* Kolima's innkeeper, once the curse is lifted, opens the inn across the
   counter. */
static void scLogRoomsCTalkToInnkeeper(void)
{
	Uint32 facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();

	if(FACING_IS_NORTH(facing)) {
		mnInnCheckIn(KOLIMA_INN, ACTOR_INNKEEPER);
	} else {
		fdScriptSetMessageId(MsgKolimaTheySayMccoyHaltedConstruction);
		/* "They say that McCoy halted construction of the palace. With..." */
		fdScriptShowMessage(ACTOR_INNKEEPER, 0);
	}

	fdScriptEnd();
}

/* The exits, the residents' words and thoughts, the shops, the bookcases, the
   jar and the barrel. */
const CUE *scGetCues(void)
{
	return LogRoomsC_Cues;
}

/* Sets up the houses for the curse and the way they're entered. */
Sint32 scInit(void)
{
	FIELD_STATE *state = FieldStateWork;
	Uint32		 actorId;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);

	/* While the curse lasts, the trees cast no shadow. */
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		for(actorId = ACTOR_FIRST_VILLAGER; actorId <= ACTOR_LAST_VILLAGER;
			actorId++) {
			ACTOR *actor = fdActorGet(actorId);

			fdActorSetSprFlags(actor, 0);
		}
	}

	/* By the village's south door the room is the father's house. By the north
	   doorway or up from Kolima Tunnel, the two entrances after it, other
	   metatiles make it a storeroom and the family is put out of sight. */
	if(MainRoutineStateWork.entrance == LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH) {
		Sint32 height;
		Sint32 width;
		width = 13;
		height = 8;
		fdMapGridCopyMetatilesTo(34, 34, 18, 16, width, height);
		fdMapGridCopyMetatilesTo(34, 94, 18, 76, width, height);
		fdMapGridCopyMetatilesTo(94, 34, 78, 16, width, height);
	} else if(MainRoutineStateWork.entrance >=
			LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH &&
		MainRoutineStateWork.entrance <= LOG_ROOMS_C_FROM_KOLIMA_TUNNEL) {
		Sint32 height;
		Sint32 width;
		width = 11;
		height = 8;
		fdMapGridCopyMetatilesTo(34, 43, 19, 23, width, height);
		fdMapGridCopyMetatilesTo(34, 94, 19, 83, width, height);
		fdMapGridCopyMetatilesTo(94, 34, 79, 23, width, height);
		fdActorSetPos(ACTOR_FATHER_WITH_A_SECRET, 0, 0);
		fdActorSetPos(ACTOR_WIFE_KEPT_FROM_THE_SECRET, 0, 0);
		fdActorSetPos(ACTOR_CHILD_TOLD_THE_SECRET, 0, 0);
	}
	return 0;
}

SCENE_ENTRANCE LogRoomsC_Entrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP,
		CONDITION_ALWAYS, 128, 0, 192, FACING_EAST, 0, 16, 56, 256, 216, 0},
	{LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR,
		CONDITION_ALWAYS, 192, 0, 168, FACING_SOUTHWEST, 0, 16, 56, 256, 216,
		0},
	{LOG_ROOMS_C_ITEM_SHOP_FROM_KOLIMA, CONDITION_ALWAYS, 392, 0, 200,
		FACING_NORTH, 0, 288, 64, 512, 232, 0},
	{LOG_ROOMS_C_INN_FROM_KOLIMA, CONDITION_ALWAYS, 648, 0, 200, FACING_NORTH,
		0, 528, 64, 768, 232, 0},
	{LOG_ROOMS_C_INN_FROM_INN_SECOND_FLOOR, CONDITION_ALWAYS, 704, 0, 168,
		FACING_SOUTHWEST, 0, 528, 64, 768, 224, 0},
	{LOG_ROOMS_C_INN_THIRD_FLOOR_FROM_INN_SECOND_FLOOR, CONDITION_ALWAYS, 128,
		0, 416, FACING_EAST, 0, 16, 280, 256, 440, 0},
	{LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH, CONDITION_ALWAYS, 392, 0, 472,
		FACING_NORTH, 0, 288, 264, 512, 504, 0},
	{LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH, CONDITION_ALWAYS, 392, 0, 328,
		FACING_SOUTH, 0, 288, 264, 512, 496, 0},
	{LOG_ROOMS_C_FROM_KOLIMA_TUNNEL, CONDITION_ALWAYS, 392, 0, 344,
		FACING_NORTH, 0, 288, 264, 512, 496, 0},
	{LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA, CONDITION_ALWAYS, 648, 0,
		424, FACING_NORTH, 0, 528, 288, 768, 456, 0},
	{LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
		CONDITION_ALWAYS, 704, 0, 392, FACING_SOUTHWEST, 0, 528, 288, 768, 448,
		0},
	{LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN, CONDITION_ALWAYS, 128, 0, 688,
		FACING_EAST, 0, 16, 552, 256, 712, 0},
	{LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN_THIRD_FLOOR, CONDITION_ALWAYS, 192,
		0, 664, FACING_SOUTHWEST, 0, 16, 552, 256, 712, 0},
	{LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
		CONDITION_ALWAYS, 400, 0, 688, FACING_NORTH, 0, 288, 552, 528, 712, 0},
	{SCENE_TABLE_END},
};

Uint32 LogRoomsC_Exits[] = {
	SCENE_EXITS(LogRoomsC),
	SCENE_EXIT(
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP,
		LogRoomsC,
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR),
	SCENE_EXIT(
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR,
		LogRoomsC,
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR),
	SCENE_EXIT(LOG_ROOMS_C_ITEM_SHOP_TO_KOLIMA, Kolima, KOLIMA_FROM_ITEM_SHOP),
	SCENE_EXIT(LOG_ROOMS_C_INN_TO_KOLIMA, Kolima, KOLIMA_FROM_INN),
	SCENE_EXIT(LOG_ROOMS_C_INN_TO_INN_SECOND_FLOOR, LogRoomsC,
		LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN),
	SCENE_EXIT(LOG_ROOMS_C_INN_THIRD_FLOOR_TO_INN_SECOND_FLOOR, LogRoomsC,
		LOG_ROOMS_C_INN_SECOND_FLOOR_FROM_INN_THIRD_FLOOR),
	SCENE_EXIT(
		LOG_ROOMS_C_HOUSE_TO_KOLIMA_SOUTH, Kolima, KOLIMA_FROM_HOUSE_SOUTH),
	SCENE_EXIT(
		LOG_ROOMS_C_HOUSE_TO_KOLIMA_NORTH, Kolima, KOLIMA_FROM_HOUSE_NORTH),
	SCENE_EXIT(LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_KOLIMA, Kolima,
		KOLIMA_FROM_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
		LogRoomsC,
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN, LogRoomsC,
		LOG_ROOMS_C_INN_FROM_INN_SECOND_FLOOR),
	SCENE_EXIT(LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN_THIRD_FLOOR, LogRoomsC,
		LOG_ROOMS_C_INN_THIRD_FLOOR_FROM_INN_SECOND_FLOOR),
	SCENE_EXIT(
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR,
		LogRoomsC,
		LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_FROM_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR),
	SCENE_EXIT(LOG_ROOMS_C_TO_KOLIMA_TUNNEL, KolimaTunnel,
		KOLIMA_TUNNEL_FROM_LOG_ROOMS_C),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE LogRoomsC_ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		166, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		88, 0, 144, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		408, 0, 472, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		360, 0, 440, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER_NEAR,
		440, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		376, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		448, 0, 680, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		424, 0, 648, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		624, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		648, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		392, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		648, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		624, 0, 200, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		120, 0, 656, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		144, 0, 368, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE LogRoomsC_Cues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_C_ITEM_SHOP_TO_KOLIMA },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_C_INN_TO_KOLIMA },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_C_INN_TO_INN_SECOND_FLOOR },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_C_INN_THIRD_FLOOR_TO_INN_SECOND_FLOOR },
	{ CUE_EXIT, 7, CONDITION_ALWAYS, LOG_ROOMS_C_HOUSE_TO_KOLIMA_SOUTH },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_C_HOUSE_TO_KOLIMA_NORTH },
	{ CUE_EXIT, 9, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_KOLIMA },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN },
	{ CUE_EXIT, 12, CONDITION_ALWAYS, LOG_ROOMS_C_INN_SECOND_FLOOR_TO_INN_THIRD_FLOOR },
	{ CUE_EXIT, 13, CONDITION_ALWAYS, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_THIRD_FLOOR_TO_WEAPON_AND_ARMOR_SHOP_SECOND_FLOOR },
	{ CUE_EXIT, 14, CONDITION_ALWAYS, LOG_ROOMS_C_TO_KOLIMA_TUNNEL },
	{ CUE_TALK, ACTOR_OLD_MAN_HEARD_OF_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIHeardThatTravelingWarriorsFrom) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_BEARDED_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsBigStrongWarriorsWithGreatBeards) },
	{ CUE_TALK, ACTOR_FATHER_WITH_A_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsYouSavedKolimaWowThatMakes) },
	{ CUE_TALK, ACTOR_WIFE_KEPT_FROM_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsMyHusbandSeemsToBeKeeping) },
	{ CUE_TALK, ACTOR_CHILD_TOLD_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDaddyToldMeHisSecret) },
	{ CUE_TALK, ACTOR_LUMBERJACK_FATHER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImNeverGoingToUseAn) },
	{ CUE_TALK, ACTOR_LUMBERJACKS_DAUGHTER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDadWillYouPleaseGoBack) },
	{ CUE_TALK, ACTOR_LUMBERJACKS_HUNGRY_SON, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDadHowAreWeGoingTo) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), SCRIPT_REF(scLogRoomsCTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), SCRIPT_REF(scLogRoomsCTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), SCRIPT_REF(scLogRoomsCTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_INNKEEPER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), SCRIPT_REF(scLogRoomsCTalkToInnkeeper) },
	{ CUE_TALK, ACTOR_VILLAGER_QUIET_VILLAGE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsWithNoMoreWorkHereKolima) },
	{ CUE_TALK, ACTOR_LUMBERJACK_FROM_IMIL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIveBeenALumberjackForYears) },
	{ CUE_TALK, ACTOR_MERCHANT_GUEST, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImOnlyAMerchantButI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HEARD_OF_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIDontReallyKnowButYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_BEARDED_WARRIORS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsAllWarriorsAreBigAndAll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WITH_A_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItsHiddenDeepInTheForest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIFE_KEPT_FROM_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIThinkHeFoundSomethingValuable) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_TOLD_THE_SECRET, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsStillTellingMeItsOutsideThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FATHER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsPeopleTurnedIntoTreesBecauseOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_DAUGHTER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsKnowingMyDadHellStartWorking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_HUNGRY_SON, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsHowAmISupposedToGrow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsFuchinTemplesMasterNyunpaIsSaid) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItsAShameWeHadTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItFeltSoGoodWhenMy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsTheVillageWasReallyBoomingWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_QUIET_VILLAGE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsRemainingAQuietVillageMeansRemaining) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FROM_IMIL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImALumberjackAndImOkay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT_GUEST, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIWishIdGrabbedSomeOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_HEARD_OF_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatAreTheyStaringAtThey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_BEARDED_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIDreamtThatAMightyWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WITH_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatWillHappenToMyTreasure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIFE_KEPT_FROM_THE_SECRET, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsSoonAllOfKolimaWillBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_TOLD_THE_SECRET, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIWonderWhatFathersSecretIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FATHER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsMaybeWeTurnedIntoTreesBecause) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsAtLeastMyFatherWasntThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACKS_HUNGRY_SON, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheHolyTreeOfKolimaHas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIUsedToCarveWeaponsOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsManImStuckInsideThatMeans) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsNoIDontWantToBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhoCausedAllThisSomeoneMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_QUIET_VILLAGE, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsHowAmISupposedToCharge) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBERJACK_FROM_IMIL, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheVillagersWereRightINever) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT_GUEST, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIfOnlyIdLeftTownA) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_LOG_ROOMS_C_JAR, FLAG_KOLIMA_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_C_BARREL, FLAG_KOLIMA_COINS, FIND_COINS(7) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LOG_ROOMS_C_HOLY_TREE_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledTheHoly)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LOG_ROOMS_C_TREES_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledDictionaryOf)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LOG_ROOMS_C_LOGGING_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledTheArt)) },
	{ CUE_TABLE_END },
};
/* clang-format on */
