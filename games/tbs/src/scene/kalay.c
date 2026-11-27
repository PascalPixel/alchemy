/* @file */
/* Kalay, the merchants' town Lord Hammet founded, the front of his palace, */
/* and the tunnel beneath them: four places in six scenes. */
/* */
/* Kalay (SceneId_Kalay), the town. Eight doors open into the houses, the */
/* weapon and armor shops, the inn and the Kalay Sanctum (stucco_rooms_a.c), */
/* and two more lead in behind the shops' counters. The way north leads up */
/* to the front of Hammet's palace; the south and west edges lead out to the */
/* World Map. The weapon, armor and item sellers serve the party only when */
/* spoken to across the counter. A Nut circles by a tree near the west way out
 */
/* until Catch pulls it in. On a rooftop in the north-east corner, a stone */
/* statue that Move pushes aside covers a way down into the Kalay Tunnel; */
/* coming back up that way finds it already moved. */
/* */
/* Once the party has heard of the Colosso tour at the inn */
/* (FLAG_KALAY_HEARD_OF_COLOSSO_TOUR), leaving town plays the tour group's */
/* departure, once (FLAG_KALAY_COLOSSO_TOUR_LEFT): the guides stop the group to
 */
/* count it, find a man missing and lead it off without him, and he runs after
 */
/* it. The second guide is the first's sprite with its green and blue swapped */
/* (SPRITE_COLOR_RBG), here and in stucco_rooms_a.c and kalay_tolbi_docks.c. */
/* */
/* The front of Hammet Palace (SceneId_HammetPalaceGrounds), where two */
/* soldiers of Kalay guard the door. The first time the party comes, before */
/* Ivan has reported to Lady Layana (FLAG_IVAN_REPORTED_TO_LAYANA), they know */
/* Ivan and take everyone in to her; afterwards they only talk. Walking into */
/* the door opens it into the palace (hammet_palace.c). */
/* */
/* The Kalay Tunnel Entrance (SceneId_KalayTunnelEntrance), out of town and */
/* reached from the World Map, where a soldier bars `the cave up ahead` by */
/* Hammet and Layana's command and a Nut floats until Catch takes it. Arriving
 */
/* with Hammet in Bunza's covered wagon after Lunpa plays his homecoming: the */
/* guard opens the way, Hammet goes in, and the party rides off. Once the */
/* tunnel is open (FLAG_KALAY_TUNNEL_OPEN) the guard stands aside, turned east.
 */
/* */
/* The Kalay Tunnel (SceneId_KalayTunnelCave to */
/* SceneId_KalayTunnelTreasureRoom), the hidden way between Hammet's palace and
 */
/* his treasure, which Layana opens to the party in thanks once Hammet is home.
 */
/* A cave with a chest holding an Apple leads down from the entrance to a */
/* long passage, and on down into the treasure rooms under the town. There */
/* each of two halls has a waterfall pouring into a pool. Pushing the hall's */
/* stone statue west until it stands in front of the waterfall stops it and */
/* drains the pool, so Isaac can climb down into the dry bed and up the far */
/* side. In the east hall that reaches Scorch, a Mars Djinni, who joins */
/* without a battle; in the west hall the drained bed holds a puddle, which */
/* Frost freezes and sinks out of the way. The drained pools last while the */
/* party stays in the tunnel (FLAG_TUNNEL_FIRST_PASSAGE_OPEN and */
/* FLAG_TUNNEL_SECOND_PASSAGE_OPEN are area temp flags); while a pool is */
/* full, sparkles stream from its statue as it nears the waterfall. Five */
/* chests hold Spirit Gloves, a Sleep Bomb, a Lucky Medal, 200 coins and a */
/* Potion. Once the tunnel is open a man waits here, thinking `The things */
/* beyond these doors belong to you all.` In the narrow corridors, raised */
/* ground in front of Isaac hides his legs, and in the halls it lets go of him
 */
/* (the ground mask, field/field_sequence_ground_mask.c). Stairs lead up into */
/* Hammet's palace, and a way up into the town under the statue. */
/* */
/* Every scene sets FLAG_KALAY_REACHED on entry. */
/* */
/* Kalay and the palace's front also catch the palace's story up with */
/* Lunpa's. Hearing in Lunpa that Hammet is held there */
/* (FLAG_LUNPA_HEARD_OF_PRISONER) sets area temp flag */
/* FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA, which changes what Layana says. Once
 */
/* Hammet is free (FLAG_DODONPA_LOCKED_UP, set as Donpa judges Dodonpa in */
/* Lunpa), Ivan's report and Layana's scolding count as seen and */
/* FLAG_KALAY_TUNNEL_OPEN opens the tunnel. */
/* */
/* The getters choose each scene's tables. The cave and the passage have no */
/* actor placements, and an empty set serves any other scene. */

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/ground_mask.h"
#include "field/map_grid.h"
#include "graphics/graphics.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"
#include "scene/kalay.h"
#include "scene/orbit.h"
#include "sdk/sound_driver_symbols.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

/* Unused: twelve bytes nothing reads or writes. */
u8 Kalay_UnusedBeforePacingStep[12] BSS_DATA ALIGNED(8);
/* Which of the two guides' turns scKalayUpdatePacingGuides is at, 0 to 4. */
s32 Kalay_PacingStep BSS_DATA;

extern s32 PushedActor_DirectionSteps[];
extern s32 Kalay_TourGroupWalkScript[];
extern s32 Kalay_TourGroupLeaveScript[];
extern s32 Kalay_WaitingTouristScript[];
extern s32 Kalay_GuideCountScript[];
extern s32 Kalay_GuidePaceScript[];
extern s32 Kalay_HeadGuideStopScript[];
extern s32 Kalay_SecondGuideStopScript[];
extern s32 Kalay_HeadGuideReturnScript[];
extern s32 Kalay_SecondGuideReturnScript[];
extern s32 Kalay_PartyLookAboutScript[];
extern s32 Kalay_PartyGatherScript[];
extern s32 Kalay_PuddleGrowScript[];
extern SCENE_ENTRANCE Kalay_FallbackEntrances[];
extern SCENE_ENTRANCE Kalay_TownEntrances[];
extern SCENE_ENTRANCE Kalay_HammetPalaceEntrances[];
extern SCENE_ENTRANCE Kalay_TunnelEntranceEntrances[];
extern SCENE_ENTRANCE Kalay_TunnelCaveEntrances[];
extern SCENE_ENTRANCE Kalay_TunnelPassageEntrances[];
extern SCENE_ENTRANCE Kalay_TunnelEntrances[];
extern u32 Kalay_Exits[];
extern ACTOR_PLACE Kalay_NoActorPlacements[];
extern ACTOR_PLACE Kalay_TownActorPlacements[];
extern ACTOR_PLACE Kalay_TourGroupActorPlacements[];
extern ACTOR_PLACE Kalay_HammetPalaceActorPlacements[];
extern ACTOR_PLACE Kalay_TunnelEntranceActorPlacements[];
extern ACTOR_PLACE Kalay_TunnelActorPlacements[];
extern TRIGGER Kalay_NoTriggers[];
extern TRIGGER Kalay_TownTriggers[];
extern TRIGGER Kalay_HammetPalaceTriggers[];
extern TRIGGER Kalay_TunnelEntranceTriggers[];
extern TRIGGER Kalay_TunnelCaveTriggers[];
extern TRIGGER Kalay_TunnelPassageTriggers[];
extern TRIGGER Kalay_TunnelTriggers[];
extern MAP_GRID_CELL_ANIMATION Kalay_DoorCells[];
extern s32 Kalay_TunnelParticleScript[];

/* The cells of the Kalay Tunnel's treasure rooms the scene gives a meaning of
 */
/* its own. */
typedef enum KALAY_TUNNEL_CELL_BEHAVIOR {
	/* The floor of the halls with the statues, where the ground mask lets go of
	 */
	/* Isaac and walking into a statue pushes it. */
	CELL_BEHAVIOR_KALAY_TUNNEL_HALL = 10,
	/* The edge of a hall where it meets a corridor, where the ground mask */
	/* follows Isaac again and walking into a statue still pushes it. */
	CELL_BEHAVIOR_KALAY_TUNNEL_HALL_EDGE = 11,
	/* Cells (26, 45) to (26, 47), and (25, 45), in the west hall, where */
	/* walking west (or south, on (25, 45)) pushes a statue standing ahead. */
	CELL_BEHAVIOR_KALAY_TUNNEL_STATUE_PUSH_WEST = 14,
	CELL_BEHAVIOR_KALAY_TUNNEL_STATUE_PUSH_CORNER = 15,
	/* Cells (14, 51), (14, 52), (43, 51) and (43, 52), and the cells east of */
	/* them: scKalayFixLeaderPriority and scKalayFreeLeaderPriority. */
	CELL_BEHAVIOR_KALAY_TUNNEL_KEEP_DRAWN = 18,
	CELL_BEHAVIOR_KALAY_TUNNEL_DRAWN_BY_PLACE = 19
} KALAY_TUNNEL_CELL_BEHAVIOR;

/* Kalay's actors (Kalay), numbering the rows of */
/* Kalay_TownActorPlacements from ACTOR_FIRST_PLACED, then the tour */
/* group, which scKalayWatchTourGroupDeparture places after them */
/* (Kalay_TourGroupActorPlacements). */
typedef enum KALAY_TOWN_ACTOR_ID {
	/* The two soldiers of Kalay at the town's entrance from the World Map. */
	ACTOR_WELCOMING_SOLDIER = ACTOR_FIRST_PLACED,
	ACTOR_SOLDIER_PROTECTING_THE_PEACE,
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	ACTOR_YOUNG_MAN_ON_HAMMET_THE_MERCHANT,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_WOMAN_ON_HAMMETS_CARAVANS,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). */
	ACTOR_MAN_ON_THE_REBUILT_BRIDGE,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_WOMAN_ON_LAMAKAN_DESERT,
	/* The tourist the guides are missing, an old man with a cane */
	/* (SPRITE_OLD_MAN_WITH_CANE), who stands in town until then and takes his
	 */
	/* place again after. */
	ACTOR_LATE_TOURIST,
	/* An old woman (SPRITE_OLD_WOMAN). "The guests at the inn are tourists on
	 */
	/* their way to see Colosso." */
	ACTOR_WOULD_BE_TOURIST,
	/* A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR). "I hear Colosso
	 */
	/* is incredible. I'd love to see such a huge carnival." */
	ACTOR_BOY_DREAMING_OF_COLOSSO,
	/* A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS). */
	ACTOR_GIRL_ON_MT_ALEPH,
	/* A strongman (SPRITE_STRONGMAN). */
	ACTOR_STRONGMAN_ON_HAMMETS_KNACK,
	/* A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN) who came to Kalay from */
	/* Tolbi, afraid of Babi (scKalayTalkAboutLeavingHome). */
	ACTOR_MAN_WHO_LEFT_TOLBI,
	/* The Colosso tour's guides, men carrying packs */
	/* (SPRITE_MAN_CARRYING_A_PACK), who stand in town and lead the tour off */
	/* when it leaves. */
	ACTOR_HEAD_GUIDE,
	ACTOR_SECOND_GUIDE,
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	ACTOR_ITEM_MERCHANT,
	/* The statue (with the Kalay Tunnel's sprite) that Move pushes aside from
	 */
	/* the way down into the Kalay Tunnel. */
	ACTOR_TOWN_STATUE,
	/* Circles in the air until Catch pulls it in. */
	ACTOR_TOWN_NUT,
	/* The tour group, ACTOR_FIRST_TOURIST and the six after it, then */
	/* ACTOR_OURANOS. In row order they are a chef (SPRITE_CHEF), a young woman
	 */
	/* in green, a young man with brown hair, a woman with fair hair, a woman */
	/* with a sword, a bald, muscular man, a young man with blue hair, and */
	/* Ouranos, a young man with red hair. */
	ACTOR_FIRST_TOURIST,
	/* The tourist the one beside him calls Ouranos. */
	ACTOR_OURANOS = ACTOR_FIRST_TOURIST + 7
} KALAY_TOWN_ACTOR_ID;

/* Hammet Palace's front (HammetPalaceGrounds), numbering */
/* Kalay_HammetPalaceActorPlacements, whose first row places the party's */
/* Ivan. */
typedef enum KALAY_PALACE_ACTOR_ID {
	/* The soldiers of Kalay before the palace door. "This is Lord Hammet's */
	/* palace. The lord is away." */
	ACTOR_PALACE_GUARD = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_PALACE_GUARD
} KALAY_PALACE_ACTOR_ID;

/* The Kalay Tunnel Entrance (KalayTunnelEntrance), numbering */
/* Kalay_TunnelEntranceActorPlacements after its rows for Garet, Ivan and */
/* Mia. */
typedef enum KALAY_TUNNEL_ENTRANCE_ACTOR_ID {
	/* A soldier of Kalay who keeps the passage closed by Hammet and Layana's */
	/* command. */
	ACTOR_TUNNEL_GUARD = ACTOR_FIRST_PLACED,
	/* A covered wagon drawn by a horse (large_wagon.png). */
	ACTOR_WAGON,
	ACTOR_HAMMET,
	ACTOR_BUNZA,
	/* Floats here until Catch takes it. */
	ACTOR_TUNNEL_ENTRANCE_NUT
} KALAY_TUNNEL_ENTRANCE_ACTOR_ID;

/* The Kalay Tunnel's treasure room (KalayTunnelTreasureRoom), numbering */
/* Kalay_TunnelActorPlacements. */
typedef enum KALAY_TUNNEL_ACTOR_ID {
	/* The statues that open the two passages. */
	ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_STATUE,
	/* XXX (no evidence): whether he is Bunza; he has the sprite of the man */
	/* Kalay's arrival calls Bunza ("Well then, Bunza, ..."), but none of his */
	/* three lines names him, and no line spoken while he stands here does. */
	/* A man drawn with SPRITE_BUNZA who waits here once FLAG_KALAY_TUNNEL_OPEN
	 */
	/* is set. */
	ACTOR_WAITING_MAN,
	/* The sprite of the puddle that Frost freezes into an ice pillar in */
	/* bilibin_cave.c. */
	ACTOR_PUDDLE,
	/* A Mars Djinni. */
	ACTOR_SCORCH
} KALAY_TUNNEL_ACTOR_ID;

/* The Kalay Tunnel's own flags. The temporary ones last one entry. The */
/* passages (area temp flags) last while the party stays in the tunnel's */
/* scenes. */
typedef enum KALAY_TUNNEL_FLAG {
	/* The ground mask follows the leader. */
	FLAG_TUNNEL_MASK_FOLLOWS_LEADER = FLAG_SCENE_TEMP_00,
	FLAG_TUNNEL_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
	/* Each statue's particles have started their sound. */
	FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED = FLAG_SCENE_TEMP_02,
	FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED = FLAG_SCENE_TEMP_03,
	FLAG_TUNNEL_FIRST_PASSAGE_OPEN = FLAG_AREA_TEMP_02,
	FLAG_TUNNEL_SECOND_PASSAGE_OPEN = FLAG_AREA_TEMP_03
} KALAY_TUNNEL_FLAG;

/* The stock rows the three counters of Kalay's shops open (mnShopRun). */
enum { SHOP_KALAY_WEAPONS = 22, SHOP_KALAY_ARMOR = 23, SHOP_KALAY_ITEMS = 24 };

static void scKalayPrepareTown(void);
static void scKalayPrepareHammetPalace(void);
static void scKalayPrepareTunnelEntrance(void);
static void scKalayPrepareTunnelPassage(void);
static void scKalayPrepareTunnelTreasureRoom(void);
static void scKalayStartTouristsWaiting(void);
static void scKalayStartTouristWaiting(s32 actorId);
static void scKalayStartGuidesPacing(void);
static void scKalayUpdatePacingGuides(void);
static void scKalayIvanMeetsPalaceGuards(void);
static void scKalayHammetReturnsToPalace(void);
static void scKalayUpdateTunnelPassages(void);
static void scKalayUpdateTunnelStatues(void);
static void scKalaySpawnTunnelParticle(s32 x, s32 height, s32 z);
static void scKalayUpdateNutOrbit(void);

/* Clears the flag a guide's script sets when it is done. */
static inline void scKalayClearScriptDone(ACTOR *actor) { actor->tmr = 0; }

/* Talking to Scorch, a Mars Djinni on the ledge beyond the east hall's pool in
 */
/* the Kalay Tunnel, makes it join the party without a battle. */
static void scKalayScorchJoins(void) {
	fdDjinnJoinParty(ACTOR_SCORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH));
}

/* An actor script call, each frame: in Kalay, while the tour waits, a tourist
 */
/* now and then nods, shakes his head or turns up to half a turn, and then */
/* waits a random 0 to 79 frames. */
static s32 scKalayUpdateWaitingTourist(ACTOR *actor) {
	u32 action;
	u32 nextTimer;

	if(actor->userValue == 0) {
		action = syRandomBelow(8);
		switch(action) {
		case 0:
			fdActorSelectAnimation(actor, ANIM_NOD);
			break;
		case 1:
			fdActorSelectAnimation(actor, ANIM_SHAKE_HEAD);
			break;
		case 3:
		case 4:
			actor->facing += syRandomBelow(32768);
			break;
		}
		nextTimer = syRandomBelow(80);
		actor->userValue = nextTimer;
	}
	if(actor->userValue != 0) {
		actor->userValue--;
	}
	return 1;
}

#include "shared/actor_heading.c"

const SCENE_ENTRANCE *scGetEntrances(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KALAY) {
		return Kalay_TownEntrances;
	}
	if(sceneId == SCENE_HAMMET_PALACE_GROUNDS) {
		return Kalay_HammetPalaceEntrances;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_ENTRANCE) {
		return Kalay_TunnelEntranceEntrances;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_CAVE) {
		return Kalay_TunnelCaveEntrances;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_PASSAGE) {
		return Kalay_TunnelPassageEntrances;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_TREASURE_ROOM) {
		return Kalay_TunnelEntrances;
	}
	return Kalay_FallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

/* The numbered exits of all six scenes. */
const u32 *scGetExits(void) { return Kalay_Exits; }

/* The people of Kalay, Hammet Palace's front, the Kalay Tunnel Entrance and */
/* the Kalay Tunnel's treasure room. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KALAY) {
		return Kalay_TownActorPlacements;
	}
	if(sceneId == SCENE_HAMMET_PALACE_GROUNDS) {
		return Kalay_HammetPalaceActorPlacements;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_ENTRANCE) {
		return Kalay_TunnelEntranceActorPlacements;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_TREASURE_ROOM) {
		return Kalay_TunnelActorPlacements;
	}
	return Kalay_NoActorPlacements;
}

/* Searching the well in Kalay: the party peers in, and it is very clean. */
static void scKalaySearchWell(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgKalayVeryCleanMaintained, 1);
	fdEventEnd();
}

/* Catch at the Kalay Tunnel Entrance pulls in the Nut floating there, until it
 */
/* is caught (FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT). */
static void scKalayCatchTunnelEntranceNut(void) {
	scCatchNut(ACTOR_TUNNEL_ENTRANCE_NUT,
			   FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT);
}

/* The conversations, exits and Psynergy targets of each scene. */
const TRIGGER *scGetTriggers(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KALAY) {
		return Kalay_TownTriggers;
	}
	if(sceneId == SCENE_HAMMET_PALACE_GROUNDS) {
		return Kalay_HammetPalaceTriggers;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_ENTRANCE) {
		return Kalay_TunnelEntranceTriggers;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_CAVE) {
		return Kalay_TunnelCaveTriggers;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_PASSAGE) {
		return Kalay_TunnelPassageTriggers;
	}
	if(sceneId == SCENE_KALAY_TUNNEL_TREASURE_ROOM) {
		return Kalay_TunnelTriggers;
	}
	return Kalay_NoTriggers;
}

/* A man in Kalay tells why he moved there: he left Tolbi, afraid of Babi, who
 */
/* has ruled it for more than a hundred years. */
static void scKalayTalkAboutLeavingHome(void) {
	fdEventStart();
	fdEventSetMessageId(MsgKalayCameKalayBecauseDidntLike);
	/* "I came to Kalay because I didn't like Tolbi... You want to..." */
	/* Yes: "Because Babi, the ruler of Tolbi, frightens me. He has ruled..." */
	/* No: "Kalay is a much better town than Tolbi. I won't go back." */
	fdEventAskYesNo(ACTOR_MAN_WHO_LEFT_TOLBI, 0);
	fdEventEnd();
}

/* The soldier at the Kalay Tunnel Entrance asks `Do you want to go to the cave
 */
/* up ahead?`, and turns the party away whatever it answers. Spoken to from */
/* the north, behind him, he only stands in the way. */
static void scKalayAskAboutGoingToCave(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	/* From the north he only stands in the way. */
	if(leader->facing < FACING_EAST_SOUTHEAST ||
	   leader->facing > FACING_WEST_SOUTHWEST) {
		fdEventStart();
		fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TUNNEL_GUARD, 0);
		fdEventWait(10);
		fdEventSetMessageId(MsgKalayDoWantGoCaveUp);
		/* "Do you want to go to the cave up ahead?" */
		fdEventOpenMessage(ACTOR_TUNNEL_GUARD, 0);
		if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_TUNNEL_GUARD, ANIM_SHAKE_HEAD);
			/* "I'd like to let you go, but I cannot. You'd better just turn..."
			 */
			fdEventShowMessage(ACTOR_TUNNEL_GUARD, 0);
		}
		else {
			FieldStateWork->msg++;
			fdActorSetAnimationAndWait(ACTOR_TUNNEL_GUARD, ANIM_NOD);
			fdEventShowMessage(ACTOR_TUNNEL_GUARD, 0);
		}
		fdEventEnd();
	}
}

/* The man waiting in the Kalay Tunnel once it is open asks whether Layana */
/* was too hard on the party. */
static void scKalayTalkAboutLayana(void) {
	fdEventStart();
	fdEventSetMessageId(MsgKalayLayanaWasVeryHardOn);
	/* "Layana was very hard on all of you, wasn't she?" */
	/* Yes: "I think she went too far, telling you all to leave... But..." */
	/* No: "Her words were hard, but you know how painful they were for..." */
	fdEventAskYesNo(ACTOR_WAITING_MAN, 0);
	fdEventEnd();
}

/* Talking to the weapon merchant in Kalay across the counter, with the leader
 */
/* facing north, opens the weapon shop. */
static void scKalayTalkToWeaponMerchant(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	/* Across the counter: between northwest and southeast, by way of north. */
	if(leader->facing < FACING_SOUTHEAST || leader->facing > FACING_NORTHWEST) {
		mnShopRun(SHOP_KALAY_WEAPONS, ACTOR_WEAPON_MERCHANT);
	}
	else {
		fdEventStart();
		fdEventSetMessageId(MsgKalayLordHammetSellsHisBest);
		/* "Lord Hammet sells all his best weapons elsewhere. These..." */
		fdEventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
		fdEventEnd();
	}
}

/* Talking to the armor merchant in Kalay across the counter, with the leader */
/* facing west to north, opens the armor shop. */
static void scKalayTalkToArmorMerchant(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	/* Across the counter: between southwest and northeast, by way of west. */
	if(leader->facing > FACING_SOUTHWEST && leader->facing < FACING_NORTHEAST) {
		mnShopRun(SHOP_KALAY_ARMOR, ACTOR_ARMOR_MERCHANT);
	}
	else {
		fdEventStart();
		fdEventSetMessageId(MsgKalayDidFindNeededInWeapon);
		/* "Did you find what you needed in the Weapon Shop?" */
		/* Yes: "Then you'll like our armor, too." */
		/* No: "Ah... You seek only the finest items. Kalay's weapons and..." */
		fdEventAskYesNo(ACTOR_ARMOR_MERCHANT, 0);
		fdEventEnd();
	}
}

/* Talking to the item merchant in Kalay across the counter, with the leader */
/* facing east, opens the item shop. */
static void scKalayTalkToItemMerchant(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	/* Across the counter: between northeast and southeast, by way of east. */
	if(leader->facing < FACING_SOUTHEAST || leader->facing > FACING_NORTHEAST) {
		mnShopRun(SHOP_KALAY_ITEMS, ACTOR_ITEM_MERCHANT);
	}
	else {
		fdEventStart();
		fdEventSetMessageId(MsgKalayLadyLayanaSharedInLord);
		/* "Lady Layana shared in Lord Hammet's hardships when he built..." */
		fdEventShowMessage(ACTOR_ITEM_MERCHANT, 0);
		fdEventEnd();
	}
}

/* Walking north into one of Kalay's eight doors opens it, and Isaac walks into
 */
/* a house, a shop, the inn or the Sanctum. */
static void scKalayOpenDoorAndExit(void) {
	FIELD_STATE *state;
	ACTOR *actor;
	s16 *trigger;
	u32 i;
	s32 door;

	state = FieldStateWork;
	fdEventStart();
	for(i = ACTOR_FIRST_PLACED; i <= ACTOR_LAST_ID; i++) {
		actor = fdActorGet(i);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}
	trigger = &state->stepTrigger;
	door = *trigger - 1;
	sySndPlay(SE_DOOR_OPEN);
	{
		s32 x = Kalay_DoorCells[door].x;
		s32 y = Kalay_DoorCells[door].y;

		fdMapGridAnimateCells(Kalay_DoorCells[door].steps, x, y);
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
	fdEventWait(10);
	syImportEventSetExitRequest(*trigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* The palace door opens in two frames. */
static inline void scKalayAnimatePalaceDoor(void) {
	sySndPlay(SE_THUMP);
	fdMapGridCopyCellsTo(36, 23, 43, 12, 2, 2);
	sySynch(5);
	fdMapGridCopyCellsTo(39, 23, 43, 12, 2, 2);
	sySynch(5);
}

/* Walking north into the door of Hammet Palace opens it with a thump, and */
/* Isaac walks in. */
static void scKalayEnterPalace(void) {
	fdEventStart();
	scKalayAnimatePalaceDoor();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	fdEventWait(10);
	syImportEventSetExitRequest(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Walking out of Kalay by the south or west way, once the party has heard of */
/* the Colosso tour at the inn, it watches the tour group stop in the street */
/* to be counted. The guides find a man missing, pace while the tourists */
/* complain, and lead the group off toward Tolbi without him; the old man */
/* with the cane comes out too late and hurries after it. Once. */
static void scKalayWatchTourGroupDeparture(void) {
	s32 message;
	ACTOR *closing;
	ACTOR *last;
	FIELD_STATE *state;

	state = FieldStateWork;
	fdEventStart();
	if(gmFlagGet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR) != 0 &&
	   gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
		fdEventAppendActorPlacements(Kalay_TourGroupActorPlacements);
		fdActorSetPos(ACTOR_HEAD_GUIDE, 252, 272);
		fdActorSetPos(ACTOR_FIRST_TOURIST, 284, 264);
		fdActorSetPos(ACTOR_FIRST_TOURIST + 1, 284, 280);
		fdActorSetPos(ACTOR_FIRST_TOURIST + 2, 300, 264);
		fdActorSetPos(ACTOR_FIRST_TOURIST + 3, 300, 280);
		fdActorSetPos(ACTOR_FIRST_TOURIST + 5, 316, 264);
		fdActorSetPos(ACTOR_FIRST_TOURIST + 4, 316, 280);
		fdActorSetPos(ACTOR_FIRST_TOURIST + 6, 332, 264);
		fdActorSetPos(ACTOR_OURANOS, 332, 280);
		fdActorSetPos(ACTOR_SECOND_GUIDE, 364, 272);
		sySndPlay(SOUND_FADE_OUT_BGM);
		fdMessageSetVoiceFromActor(ACTOR_HEAD_GUIDE);
		/* "OK! Are we all listening? Let's stop right here." */
		mnFieldMessageShowCentered(MsgKalayOkListeningLets, 1, 0);
		sySndPlay(MUS_TRY_YOUR_LUCK);
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		if(state->stepTrigger == 9) {
			fdCameraSetSpeed(2.4);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
		}
		else {
			fdCameraSetSpeed(1.2);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
		}
		/* Stagger the speeds so the group arrives in rows. */
		fdActorSetSpeed(ACTOR_HEAD_GUIDE, 1.1);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST, 1);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 1, 1);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 2, 0.9);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 3, 0.9);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 5, 0.8);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 4, 0.8);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 6, 0.7);
		fdActorSetSpeed(ACTOR_OURANOS, 0.7);
		fdActorSetSpeed(ACTOR_SECOND_GUIDE, 0.6);
		fdActorSetScript(ACTOR_HEAD_GUIDE, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 1, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 2, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 3, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 5, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 4, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 6, Kalay_TourGroupWalkScript);
		fdActorSetScript(ACTOR_OURANOS, Kalay_TourGroupWalkScript);
		last = fdActorGet(ACTOR_SECOND_GUIDE);
		last->tmr = 0;
		fdActorSetScript(ACTOR_SECOND_GUIDE, Kalay_TourGroupWalkScript);
		fdCameraMoveTo(186, CAMERA_KEEP, 272, 1);
		fdActorWaitForScriptEndById(ACTOR_HEAD_GUIDE);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
		do {
			sySynch(1);
		} while(last->tmr == 0);
		fdEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST, 2);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST, FACING_SOUTH_SOUTHWEST, 20);
		fdEventSetMessageId(MsgKalayWhyWeStoppingAtPlace);
		/* "Why are we stopping at a place like this?" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 1, 2);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_NORTH_NORTHWEST,
							 10);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 1, ANIM_NOD);
		/* "Yeah! We have to hurry, or we'll be late for Colosso!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 1, 0, 10);
		fdActorSetAttachedEffect(ACTOR_FIRST_TOURIST + 5,
								 ATTACHED_EFFECT_IN_FRONT |
									 ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		/* "Late for Colosso? We can't have that!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 5, 0, 10);
		fdActorShowEmote(ACTOR_FIRST_TOURIST + 4,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_NORTH_NORTHWEST,
							 10);
		/* "That won't do at all!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 4, 0, 10);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 4, 2);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 4, ANIM_SHAKE_HEAD);
		/* "We paid good money to see Colosso! We can't be late!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 4, 0, 10);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_NORTH_NORTHWEST,
							 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_SOUTH_SOUTHWEST,
							 20);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 4, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_FIRST_TOURIST + 5, ANIM_NOD);
		fdActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
		fdActorSetAttachedEffect(ACTOR_HEAD_GUIDE, ATTACHED_EFFECT_IN_FRONT |
													   ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		/* "Everything will be just fine, folks!" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
		fdActorGet(ACTOR_HEAD_GUIDE)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 172, 264);
		fdEventWait(1);
		fdActorGet(ACTOR_HEAD_GUIDE)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 20);
		fdActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
		fdEventWait(20);
		fdActorGet(ACTOR_HEAD_GUIDE)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 172, 272);
		fdEventWait(1);
		fdActorGet(ACTOR_HEAD_GUIDE)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 180, 272);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
		/* "We're just checking to make sure no one is missing!" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorShowEmote(ACTOR_OURANOS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdActorRunRepeatedMotion(ACTOR_OURANOS, 1);
		fdActorSetAnimationAndWait(ACTOR_OURANOS, ANIM_NOD);
		/* "Well, I guess that's OK..." */
		fdEventShowMessageThenWait(ACTOR_OURANOS, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 6, 1);
		/* "But if anyone is missing, we're the one's who'll miss out." */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 6, 0, 10);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 6, ANIM_SHAKE_HEAD);
		/* "Forget about them! Let's just get a move on!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 6, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_SECOND_GUIDE, 2);
		fdActorShowEmote(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		/* "Oh, dear..." */
		fdEventShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 10);
		fdActorJump(ACTOR_HEAD_GUIDE, 2, 20);
		fdActorJump(ACTOR_HEAD_GUIDE, 4, 40);
		fdActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
		/* "All right, hurry up and start counting!" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorSetSpeed(ACTOR_SECOND_GUIDE, 1.6);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 265, 282);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 251, 284);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 246, 296);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 0);
		/* The tourists wait, nodding, shaking their heads and turning, while
		 * the */
		/* guides count them. */
		scKalayStartTouristsWaiting();
		fdEventWait(40);
		fdActorSetSpeed(ACTOR_SECOND_GUIDE, 1.6);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 228, 296);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 212, 296);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 192, 296);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
		fdActorStartRepeatedMotion(ACTOR_SECOND_GUIDE, 2);
		fdActorShowEmote(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
						 60);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_SOUTH_SOUTHEAST, 0);
		fdActorWalkToAndWait(ACTOR_SECOND_GUIDE, 184, 286);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH_NORTHWEST, 10);
		fdActorSetAttachedEffect(ACTOR_SECOND_GUIDE, ATTACHED_EFFECT_IN_FRONT |
														 ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_SHAKE_HEAD);
		fdActorShowEmote(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		fdActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
		fdActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_EAST, 60);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH_NORTHWEST, 10);
		fdActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_EAST, 0);
		fdActorSetSpeed(ACTOR_HEAD_GUIDE, 1.6);
		fdActorRunScriptAndWait(ACTOR_HEAD_GUIDE, Kalay_GuidePaceScript);
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 228, 296);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 212, 296);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 192, 296);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_SECOND_GUIDE, FACING_SOUTH_SOUTHEAST, 10);
		fdActorSetAttachedEffect(ACTOR_HEAD_GUIDE, ATTACHED_EFFECT_IN_FRONT |
													   ATTACHED_EFFECT_ONCE);
		fdEventWait(60);
		fdActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_SHAKE_HEAD);
		message = MsgKalayShortOnePerson;
		fdEventSetMessageId(message);
		/* "Uh-oh... We're short one person... Now where could he be?" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 40);
		/* The guides pace while the tourists complain one by one. */
		scKalayStartGuidesPacing();
		fdActorWalkToAndWait(ACTOR_HEAD_GUIDE, 178, 272);
		fdActorFaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
		fdEventWait(240);
		fdActorStop(ACTOR_FIRST_TOURIST);
		sySynch(1);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 10);
		fdActorShowEmote(ACTOR_FIRST_TOURIST, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 60);
		/* "What's going on? Why aren't we leaving for Tolbi yet?" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST, 0, 10);
		scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST);
		fdEventWait(80);
		fdActorStop(ACTOR_FIRST_TOURIST + 1);
		sySynch(1);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_NORTH_NORTHEAST,
							 20);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 1, 2);
		/* "There are a lot of people waiting to see Colosso, here!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 1, 0, 10);
		scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 1);
		fdEventWait(160);
		fdActorStop(ACTOR_FIRST_TOURIST + 5);
		sySynch(1);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_SOUTH_SOUTHWEST,
							 10);
		fdActorShowEmote(ACTOR_FIRST_TOURIST + 5,
						 EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		/* "Hey? What's up? Why are the guides pacing back and forth?" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 5, 0, 10);
		scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 5);
		fdEventWait(80);
		fdActorStop(ACTOR_FIRST_TOURIST + 3);
		sySynch(1);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_NORTH_NORTHWEST,
							 10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 3, 1);
		fdEventSetMessageId(message + 6);
		/* "Come on! Why aren't we leaving yet?" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 3, 0, 10);
		syCloseEvent(scKalayUpdatePacingGuides);
		fdActorStop(ACTOR_HEAD_GUIDE);
		fdActorStop(ACTOR_SECOND_GUIDE);
		sySynch(1);
		fdActorGet(ACTOR_HEAD_GUIDE)->tmr = 0;
		fdActorGet(ACTOR_SECOND_GUIDE)->tmr = 0;
		fdActorSetSpeed(ACTOR_HEAD_GUIDE, 0.8);
		fdActorSetSpeed(ACTOR_SECOND_GUIDE, 0.8);
		fdActorSetScript(ACTOR_HEAD_GUIDE, Kalay_HeadGuideStopScript);
		fdActorSetScript(ACTOR_SECOND_GUIDE, Kalay_SecondGuideStopScript);
		fdActorStop(ACTOR_FIRST_TOURIST + 2);
		sySynch(1);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_SOUTH_SOUTHWEST,
							 10);
		message += 5;
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 2, 2);
		fdEventSetMessageId(message);
		/* "I hear there was a landslide on the road to Tolbi. We paid..." */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 2, 0, 20);
		scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 2);
		scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 3);
		do {
			sySynch(1);
		} while(fdActorGet(ACTOR_HEAD_GUIDE)->tmr == 0 ||
				fdActorGet(ACTOR_SECOND_GUIDE)->tmr != 1);
		fdActorSetScript(ACTOR_HEAD_GUIDE, Kalay_HeadGuideReturnScript);
		fdActorSetScript(ACTOR_SECOND_GUIDE, Kalay_SecondGuideReturnScript);
		fdActorStop(ACTOR_FIRST_TOURIST + 4);
		sySynch(1);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_SOUTH_SOUTHWEST,
							 10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 4, 1);
		fdActorSetAnimationAndWait(ACTOR_FIRST_TOURIST + 4, ANIM_SHAKE_HEAD);
		message = MsgKalayDecideEnteringColosso;
		fdEventSetMessageId(message);
		/* "We have to decide if we're entering Colosso once we reach Tolbi." */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 4, 0, 10);
		scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 4);
		fdActorStop(ACTOR_OURANOS);
		fdActorStop(ACTOR_FIRST_TOURIST + 6);
		sySynch(1);
		fdActorShowEmote(ACTOR_OURANOS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		fdActorShowEmote(ACTOR_FIRST_TOURIST + 6,
						 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorFaceDirection(ACTOR_OURANOS, FACING_NORTH_NORTHWEST, 10);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_SOUTH_SOUTHWEST,
							 10);
		message += 3;
		fdActorSetAnimationAndWait(ACTOR_OURANOS, ANIM_SHAKE_HEAD);
		fdEventSetMessageId(message);
		/* "Right when we're leaving, they tell us we have to ride a boat?..."
		 */
		fdEventShowMessageThenWait(ACTOR_OURANOS, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_FIRST_TOURIST + 6, 1);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 6, ANIM_SHAKE_HEAD);
		/* "It's just a shame a man like you can't ride on a boat, Ouranos." */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 6, 0, 10);
		fdActorShowEmote(ACTOR_OURANOS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorShowEmote(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
						 0);
		fdActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
		fdEventSetMessageId(MsgKalayThatsWeCantWaitAny);
		/* "That's it... We can't wait any longer!" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorStop(ACTOR_FIRST_TOURIST);
		fdActorStop(ACTOR_FIRST_TOURIST + 1);
		fdActorStop(ACTOR_FIRST_TOURIST + 2);
		fdActorStop(ACTOR_FIRST_TOURIST + 3);
		fdActorStop(ACTOR_FIRST_TOURIST + 5);
		fdActorStop(ACTOR_FIRST_TOURIST + 4);
		fdActorStop(ACTOR_FIRST_TOURIST + 6);
		fdActorStop(ACTOR_OURANOS);
		fdActorStop(ACTOR_HEAD_GUIDE);
		fdActorStop(ACTOR_SECOND_GUIDE);
		sySynch(1);
		fdActorJump(ACTOR_FIRST_TOURIST, 2, 0);
		fdActorJump(ACTOR_FIRST_TOURIST + 1, 2, 0);
		fdActorJump(ACTOR_FIRST_TOURIST + 2, 2, 0);
		fdActorJump(ACTOR_FIRST_TOURIST + 3, 2, 0);
		fdActorJump(ACTOR_FIRST_TOURIST + 5, 2, 0);
		fdActorJump(ACTOR_FIRST_TOURIST + 4, 2, 0);
		fdActorJump(ACTOR_FIRST_TOURIST + 6, 2, 0);
		fdActorJump(ACTOR_OURANOS, 2, 0);
		fdActorJump(ACTOR_SECOND_GUIDE, 2, 40);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_OURANOS, FACING_WEST, 40);
		fdActorJump(ACTOR_SECOND_GUIDE, 4, 40);
		/* "Can't wait? What do you mean?" */
		fdEventShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 1);
		/* "We all knew that anyone who was late would be left behind,..." */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
		/* "Everyone understood that." */
		fdEventShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
		/* "Then what's the problem? We'll miss the boat if we wait any..." */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorShowEmote(ACTOR_FIRST_TOURIST, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdActorStartRepeatedMotion(ACTOR_FIRST_TOURIST, 1);
		/* "Miss the boat? We can't let that happen!" */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST, 0, 10);
		fdActorShowEmote(ACTOR_FIRST_TOURIST + 1, EMOTE_IN_FRONT | EMOTE_WINCE,
						 40);
		/* "We paid a lot of money... We have to arrive before the..." */
		fdEventShowMessageThenWait(ACTOR_FIRST_TOURIST + 1, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_SHAKE_HEAD);
		fdEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
		/* "There's nothing we can do... Let's go!" */
		fdEventShowMessageThenWait(ACTOR_SECOND_GUIDE, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
		/* "Yeah! Let's go!" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_NORTH_NORTHWEST,
							 4);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_SOUTH_SOUTHWEST,
							 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_NORTH_NORTHWEST,
							 4);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_SOUTH_SOUTHWEST,
							 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_NORTH_NORTHWEST,
							 4);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_SOUTH_SOUTHWEST,
							 0);
		fdActorFaceDirection(ACTOR_OURANOS, FACING_NORTH_NORTHWEST, 4);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_FIRST_TOURIST + 1, ANIM_NOD);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 2, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_FIRST_TOURIST + 3, ANIM_NOD);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 5, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_FIRST_TOURIST + 4, ANIM_NOD);
		fdActorSetAnimation(ACTOR_FIRST_TOURIST + 6, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_OURANOS, ANIM_NOD);
		fdActorJump(ACTOR_HEAD_GUIDE, 2, 40);
		/* "OK... Follow me, and stay in line!" */
		fdEventShowMessageThenWait(ACTOR_HEAD_GUIDE, 0, 10);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 1, FACING_WEST, 4);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 2, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 3, FACING_WEST, 4);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 5, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 4, FACING_WEST, 4);
		fdActorFaceDirection(ACTOR_FIRST_TOURIST + 6, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_OURANOS, FACING_WEST, 4);
		fdActorSetSpeed(ACTOR_HEAD_GUIDE, 1.1);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST, 1.05);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 1, 1.05);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 2, 1);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 3, 1);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 5, 0.95);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 4, 0.95);
		fdActorSetSpeed(ACTOR_FIRST_TOURIST + 6, 0.9);
		fdActorSetSpeed(ACTOR_OURANOS, 0.9);
		fdActorSetSpeed(ACTOR_SECOND_GUIDE, 0.85);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST, 1);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST + 1, 1);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST + 2, 1);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST + 3, 1);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST + 5, 1);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST + 4, 1);
		fdActorSetSprPriority(ACTOR_FIRST_TOURIST + 6, 1);
		fdActorSetSprPriority(ACTOR_OURANOS, 1);
		fdActorSetSprPriority(ACTOR_HEAD_GUIDE, 1);
		fdActorSetSprPriority(ACTOR_SECOND_GUIDE, 1);
		fdActorStop(ACTOR_FIRST_TOURIST);
		fdActorStop(ACTOR_FIRST_TOURIST + 1);
		fdActorStop(ACTOR_FIRST_TOURIST + 2);
		fdActorStop(ACTOR_FIRST_TOURIST + 3);
		fdActorStop(ACTOR_FIRST_TOURIST + 5);
		fdActorStop(ACTOR_FIRST_TOURIST + 4);
		fdActorStop(ACTOR_FIRST_TOURIST + 6);
		fdActorStop(ACTOR_OURANOS);
		fdActorStop(ACTOR_HEAD_GUIDE);
		fdActorStop(ACTOR_SECOND_GUIDE);
		sySynch(1);
		fdActorSetScript(ACTOR_HEAD_GUIDE, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 1, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 2, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 3, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 5, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 4, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_FIRST_TOURIST + 6, Kalay_TourGroupLeaveScript);
		fdActorSetScript(ACTOR_OURANOS, Kalay_TourGroupLeaveScript);
		closing = fdActorGet(ACTOR_SECOND_GUIDE);
		closing->tmr = 0;
		fdActorSetScript(ACTOR_SECOND_GUIDE, Kalay_TourGroupLeaveScript);
		do {
			sySynch(1);
		} while(fdActorGet(ACTOR_SECOND_GUIDE)->tmr != 1);
		fdEventWait(80);
		/* The late tourist finds that the rest of the group has already left.
		 */
		fdActorSetPos(ACTOR_LATE_TOURIST, 340, 274);
		sySynch(1);
		fdActorSetSpeed(ACTOR_LATE_TOURIST, 1);
		fdActorWalkToAndWait(ACTOR_LATE_TOURIST, 224, 274);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_EAST, 40);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_WEST, 40);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_NORTH, 40);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_SOUTH_SOUTHWEST, 40);
		fdActorShowEmote(ACTOR_LATE_TOURIST, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 60);
		/* "Hmm... Now where did my tour group get to...?" */
		fdEventShowMessageThenWait(ACTOR_LATE_TOURIST, 0, 10);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_EAST, 40);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_NORTH, 40);
		fdActorFaceDirection(ACTOR_LATE_TOURIST, FACING_WEST, 40);
		fdActorSetAttachedEffect(ACTOR_LATE_TOURIST, ATTACHED_EFFECT_IN_FRONT |
														 ATTACHED_EFFECT_ONCE);
		fdActorJump(ACTOR_LATE_TOURIST, 4, 40);
		/* "No! They wouldn't have left without me, would they?" */
		fdEventShowMessageThenWait(ACTOR_LATE_TOURIST, 0, 20);
		fdActorRunRepeatedMotion(ACTOR_LATE_TOURIST, 2);
		/* "Oh, no! I'd better get moving!" */
		fdEventShowMessageThenWait(ACTOR_LATE_TOURIST, 0, 10);
		fdActorJump(ACTOR_LATE_TOURIST, 4, 40);
		fdActorSetSpeed(ACTOR_LATE_TOURIST, 1.2);
		closing = fdActorGet(ACTOR_LATE_TOURIST);
		closing->tmr = 0;
		fdActorSetScript(ACTOR_LATE_TOURIST, Kalay_TourGroupLeaveScript);
		do {
			sySynch(1);
		} while(fdActorGet(ACTOR_LATE_TOURIST)->tmr != 1);
		fdActorSetPos(ACTOR_LATE_TOURIST, 359, 314);
		fdActorGet(ACTOR_LATE_TOURIST)->facing = FACING_NORTH_NORTHEAST;
		fdActorSetPos(ACTOR_HEAD_GUIDE, 455, 434);
		fdActorGet(ACTOR_HEAD_GUIDE)->facing = FACING_NORTH_NORTHEAST;
		fdActorSetPos(ACTOR_SECOND_GUIDE, 464, 416);
		fdActorGet(ACTOR_SECOND_GUIDE)->facing = FACING_SOUTH_SOUTHWEST;
		fdActorReleaseById(ACTOR_FIRST_TOURIST);
		fdActorReleaseById(ACTOR_FIRST_TOURIST + 1);
		fdActorReleaseById(ACTOR_FIRST_TOURIST + 2);
		fdActorReleaseById(ACTOR_FIRST_TOURIST + 3);
		fdActorReleaseById(ACTOR_FIRST_TOURIST + 4);
		fdActorReleaseById(ACTOR_FIRST_TOURIST + 5);
		fdActorReleaseById(ACTOR_FIRST_TOURIST + 6);
		fdActorReleaseById(ACTOR_OURANOS);
		sySndPlay(SOUND_FADE_OUT_BGM);
		if(state->stepTrigger == 9) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 224, 458);
			fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
		}
		else {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 40, 248);
			fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_SOUTH;
		}
		scPlayMusic();
		gmFlagSet(FLAG_KALAY_COLOSSO_TOUR_LEFT);
	}
	else {
		sySndPlay(SE_MAP_EXIT);
		syImportEventSetExitRequest(state->stepTrigger);
		fdEventFinishScrTransition();
		fdEventWaitTransitionFrames();
	}
	fdEventEnd();
}

/* Walking out of the Kalay Tunnel's narrow corridors into a hall with a */
/* statue, Isaac is let go by the ground mask: raised ground in front of him no
 */
/* longer hides his legs. */
static void scKalayStopTrackingLeader(void) {
	if(fdEventGetFlag(FLAG_TUNNEL_MASK_FOLLOWS_LEADER) != 0) {
		GroundMaskStateWork->actor = NULL;
		gmFlagClear(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
	}
}

/* Stepping from a hall of the Kalay Tunnel back into its narrow corridors, */
/* Isaac is followed by the ground mask again: raised ground in front of him */
/* hides his legs. */
static void scKalayStartTrackingLeader(void) {
	GROUND_MASK_STATE *state;

	if(fdEventGetFlag(FLAG_TUNNEL_MASK_FOLLOWS_LEADER) == 0) {
		state = GroundMaskStateWork;
		state->actor = fdActorGet(ACTOR_PARTY_LEADER);
		gmFlagSet(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
	}
}

/* Nothing visible happens when Isaac, back from hopping a water channel on */
/* the Kalay Tunnel's narrow ledges, steps onto cell (15, 51) or (44, 51); */
/* from there on he is drawn in front of or behind the scenery by where he */
/* stands again (ACTOR_PRIORITY_AUTOMATIC). */
static void scKalayFreeLeaderPriority(void) {
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
}

/* Nothing visible happens when Isaac steps onto cell (14, 51) or (43, 51), */
/* at the end of a narrow ledge in the Kalay Tunnel where a channel of water */
/* runs south out of the pool beside it. From there he stays drawn as he was */
/* until he is back on the cell east of it: hopping the channel and back, he */
/* is drawn whole in front of the water and the ledges' edges. */
static void scKalayFixLeaderPriority(void) {
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
}

/* Catch in Kalay pulls in the Nut circling there, until it is caught. */
static void scKalayCatchTownNut(void) {
	syCloseEvent(scKalayUpdateNutOrbit);
	sySynch(1);
	fdActorSetPos(ACTOR_TOWN_NUT, 0, 0);
	gmFlagSet(FLAG_KALAY_TOWN_NUT_CAUGHT);
	fdItemDispShowItem(ITEM_NUT, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	fdPartyInventoryGiveItem(ITEM_NUT, 0);
}

/* Every scene of the overlay sets FLAG_KALAY_REACHED, then prepares its own */
/* people and cells. */
s32 scInit(void) {
	s32 sceneId;

	gmFlagSet(FLAG_KALAY_REACHED);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_KALAY) {
		scKalayPrepareTown();
	}
	else if(sceneId == SCENE_HAMMET_PALACE_GROUNDS) {
		scKalayPrepareHammetPalace();
	}
	else if(sceneId == SCENE_KALAY_TUNNEL_ENTRANCE) {
		scKalayPrepareTunnelEntrance();
	}
	else if(sceneId == SCENE_KALAY_TUNNEL_PASSAGE) {
		scKalayPrepareTunnelPassage();
	}
	else if(sceneId == SCENE_KALAY_TUNNEL_TREASURE_ROOM) {
		scKalayPrepareTunnelTreasureRoom();
	}
	return 0;
}

/* Catches the palace's story up with Lunpa's: once Hammet is free, Ivan's */
/* report and Layana's scolding count as seen and the tunnel is open; once the
 */
/* party has heard in Lunpa that he is held there, Kalay has heard it too. */
static inline void scKalayCatchUpWithLunpa(void) {
	if(gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0) {
		gmFlagSet(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA);
		gmFlagSet(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
		gmFlagSet(FLAG_IVAN_REPORTED_TO_LAYANA);
		gmFlagSet(FLAG_KALAY_TUNNEL_OPEN);
	}
	if(gmFlagGet(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
		gmFlagSet(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA);
	}
}

/* Sets up Kalay. */
static void scKalayPrepareTown(void) {
	ORBITING_MAP_ACTOR *nut;
	ICON_TRANSFER *glyph;
	ORBITING_MAP_ACTOR_SPR *sprite;

	scKalayCatchUpWithLunpa();
	if(MainRoutineStateWork.entrance == KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM) {
		fdActorSetPos(ACTOR_TOWN_STATUE, 424, 88);
	}
	/* The second guide shares the first's sprite, drawn with its green and blue
	 */
	/* swapped to tell them apart. */
	fdActorSetSprColById(ACTOR_SECOND_GUIDE, SPRITE_COLOR_RBG);
	if(gmFlagGet(FLAG_KALAY_TOWN_NUT_CAUGHT) != 0) {
		fdActorSetPos(ACTOR_TOWN_NUT, 0, 0);
	}
	else {
		nut = (ORBITING_MAP_ACTOR *)fdActorGet(ACTOR_TOWN_NUT);
		sprite = nut->spr;
		sprite->pri = 1;
		sprite->fullCol = 0;
		sprite->palNum = 0;
		sprite->cnt = 0;
		nut->retainSprCache = 1;
		nut->motionFlg = 0;
		nut->y = toFIXED(10);
		nut->frozen = 1;
		glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_NUT);
		syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		nut->orbitAng = 0;
		nut->orbitCenterX = nut->x;
		nut->orbitCenterY = nut->y;
		nut->orbitCenterZ = nut->z;
		syTaskAddFrame(scKalayUpdateNutOrbit, TASK_PASS_FRAME);
	}
}

/* Sets up Hammet Palace's front (HammetPalaceGrounds). */
static void scKalayPrepareHammetPalace(void) {
	scKalayCatchUpWithLunpa();
	if(MainRoutineStateWork.entrance != ENTRANCE_DEFAULT) {
		if(gmFlagGet(FLAG_IVAN_REPORTED_TO_LAYANA) == 0) {
			scKalayIvanMeetsPalaceGuards();
		}
	}
}

/* Sets up the Kalay Tunnel Entrance (KalayTunnelEntrance). */
static void scKalayPrepareTunnelEntrance(void) {
	if(gmFlagGet(FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT) == 0) {
		scFloatNut(ACTOR_TUNNEL_ENTRANCE_NUT);
	}
	if(gmFlagGet(FLAG_KALAY_TUNNEL_OPEN) != 0) {
		fdActorGet(ACTOR_TUNNEL_GUARD)->facing = FACING_EAST;
	}
	if(MainRoutineStateWork.entrance ==
	   KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH) {
		scKalayHammetReturnsToPalace();
	}
}

/* Sets up the Kalay Tunnel's passage (KalayTunnelPassage). */
static void scKalayPrepareTunnelPassage(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	/* Once the tunnel is open, the barred gate across the long corridor stands
	 */
	/* open. */
	if(gmFlagGet(FLAG_KALAY_TUNNEL_OPEN) != 0) {
		fdMapGridCopyCellsTo(58, 5, 58, 8, 2, 3);
		fdMapGridCopyCellAttributes(8, 11, 2, 1, 8, 10);
		fdMapGridCopyCellsTo(8, 12, 8, 11, 2, 1);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
	if(MainRoutineStateWork.entrance <=
	   KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM) {
		scQueueMusic(SE_AMBIENT_WATER);
	}
}

/* Sets up the Kalay Tunnel's treasure room (KalayTunnelTreasureRoom). */
static void scKalayPrepareTunnelTreasureRoom(void) {
	s32 following;
	GROUND_MASK_STATE *state;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEffectStartGroundMask(0);
	/* The ground mask follows the leader except on the way down from the town.
	 */
#if TBS_PATCH_1
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		state = GroundMaskStateWork;
		/* The flag's own 0 is the ground mask's want of an actor. */
		following = gmFlagGet(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
		if(following != 0)
			following = (s32)fdActorGet(ACTOR_PARTY_LEADER);
		state->actor = (ACTOR *)following;
	}
	else {
		fdEventSetFlag(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
		if(MainRoutineStateWork.entrance ==
		   KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY) {
			GroundMaskStateWork->actor = NULL;
			gmFlagClear(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
		}
	}
#else
	fdEventSetFlag(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		state = GroundMaskStateWork;
		/* The flag's own 0 is the ground mask's want of an actor. */
		following = gmFlagGet(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
		if(following != 0)
			following = (s32)fdActorGet(ACTOR_PARTY_LEADER);
		state->actor = (ACTOR *)following;
	}
	else if(MainRoutineStateWork.entrance ==
			KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY) {
		GroundMaskStateWork->actor = NULL;
		gmFlagClear(FLAG_TUNNEL_MASK_FOLLOWS_LEADER);
	}
#endif
	if(gmFlagGet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
		fdActorSetPos(ACTOR_PUDDLE, 150, 728);
		if(gmFlagGet(FLAG_TUNNEL_PUDDLE_FROZEN) != 0) {
			/* DEAD: a lookup whose actor goes unused; the ROM keeps the call.
			 */
			fdActorGet(ACTOR_PUDDLE);
			fdActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
			fdMapGridCopyCellAttributes(0, 0, 1, 1, 9, 14);
			fdMapGridCopyCellAttributes(0, 0, 1, 1, 9, 45);
			fdActorGet(ACTOR_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		}
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_STATUE), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_STATUE), 0);
	syTaskAddFrame(scKalayUpdateTunnelStatues, TASK_PASS_FRAME);
	/* Once the tunnel is open, the man who waited for it stands here and the */
	/* barred gate beside him stands open. */
	if(gmFlagGet(FLAG_KALAY_TUNNEL_OPEN) != 0) {
		fdActorSetPos(ACTOR_WAITING_MAN, 426, 730);
		fdActorGet(ACTOR_WAITING_MAN)->facing = FACING_SOUTH_SOUTHWEST;
		fdMapGridCopyCellsTo(88, 48, 88, 45, 2, 3);
		fdMapGridCopyCellsTo(24, 49, 24, 48, 2, 1);
		fdMapGridCopyCellsTo(25, 42, 25, 47, 1, 1);
		fdMapGridCopyCellAttributes(22, 50, 2, 1, 24, 49);
	}
	if(gmFlagGet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
		/* The west pool as scKalayUpdateTunnelPassages leaves it: the first
		 * statue */
		/* stands in the arch, the waterfall under it has run dry (the last of
		 * its */
		/* eight pictures, a column of the overhead layer) and the pool has
		 * drained */
		/* to bare floor. */
		fdActorSetPos(ACTOR_FIRST_STATUE, 232, 732);
		fdMapGridCopyCellAttributes(7, 44, 1, 1, 0, 1);
		fdMapGridCopyCellsTo(74, 58, 78, 41, 1, 5);
		fdMapGridCopyCellsTo(16, 109, 13, 109, 3, 2);
		/* DEAD: the draining's stages, drawn a column at a time, twice over, */
		/* where the event shows three of them with a wait between, all covered
		 * by */
		/* the drained floor copied below; the ROM keeps the calls. */
		fdMapGridCopyCellsTo(67, 64, 71, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 72, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 68, 73, 43, 1, 2);
		fdMapGridCopyCellsTo(67, 68, 74, 43, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 75, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 76, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 77, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 78, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 79, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 80, 44, 1, 2);
		fdMapGridCopyCellsTo(2, 0, 9, 42, 2, 2);
		fdMapGridCopyCellsTo(68, 64, 71, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 72, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 68, 73, 43, 1, 2);
		fdMapGridCopyCellsTo(68, 68, 74, 43, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 75, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 76, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 77, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 78, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 79, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 80, 44, 1, 2);
		fdMapGridCopyCellsTo(4, 0, 9, 42, 2, 2);
		/* The drained floor, on the ground layer and the overhead layer, and
		 * its */
		/* ground. */
		fdMapGridCopyCellsTo(7, 11, 7, 42, 10, 8);
		fdMapGridCopyCellsTo(71, 12, 71, 43, 10, 13);
		fdMapGridCopyCellAttributes(6, 13, 12, 12, 6, 44);
		fdMapGridCopyCellAttributes(0, 1, 1, 1, 7, 44);
	}
	else {
		switch(MainRoutineStateWork.entrance) {
		case KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE:
		case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST:
			scQueueMusic(SE_AMBIENT_WATER);
			break;
		}
	}
	if(gmFlagGet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
		/* The east pool the same way, the second statue in its arch. */
		fdActorSetPos(ACTOR_SECOND_STATUE, 696, 732);
		fdMapGridCopyCellsTo(74, 58, 107, 41, 1, 5);
		fdMapGridCopyCellsTo(45, 109, 42, 109, 3, 2);
		/* DEAD: the draining's stages again, covered by the drained floor
		 * below; */
		/* the ROM keeps the calls. */
		fdMapGridCopyCellsTo(67, 64, 102, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 103, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 104, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 105, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 106, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 107, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 108, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 109, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 102, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 103, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 104, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 105, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 106, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 107, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 108, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 109, 44, 1, 2);
		fdMapGridCopyCellsTo(38, 14, 38, 44, 8, 4);
		fdMapGridCopyCellsTo(102, 14, 102, 44, 8, 12);
		fdMapGridCopyCellAttributes(37, 13, 10, 12, 37, 43);
	}
	else {
		switch(MainRoutineStateWork.entrance) {
		case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST:
		case KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY:
			scQueueMusic(SE_AMBIENT_WATER);
			break;
		}
	}
}

/* All eight tourists in Kalay start waiting. */
static void scKalayStartTouristsWaiting(void) {
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST);
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 1);
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 2);
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 3);
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 5);
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 4);
	scKalayStartTouristWaiting(ACTOR_FIRST_TOURIST + 6);
	scKalayStartTouristWaiting(ACTOR_OURANOS);
}

/* A tourist starts waiting (scKalayUpdateWaitingTourist) after a random pause.
 */
static void scKalayStartTouristWaiting(s32 actorId) {
	ACTOR *actor;
	u32 random;

	actor = fdActorGet(actorId);
	actor->tmr = actorId;
	random = syRandomNext();
	actor->userValue = (random * 5) >> 12;
	fdActorStartScript(actor, Kalay_WaitingTouristScript);
}

/* The guides speed up and start pacing in turn. */
static void scKalayStartGuidesPacing(void) {
	Kalay_PacingStep = 0;
	fdActorSetSpeed(ACTOR_HEAD_GUIDE, 1.6);
	fdActorSetSpeed(ACTOR_SECOND_GUIDE, 1.6);
	syTaskAddFrame(scKalayUpdatePacingGuides, TASK_PASS_FRAME);
}

/* Task: the guides take turns, each counting the tourists and then pacing, */
/* one starting when the other finishes. */
static void scKalayUpdatePacingGuides(void) {
	switch(Kalay_PacingStep) {
	case 0:
		scKalayClearScriptDone(fdActorGet(ACTOR_SECOND_GUIDE));
		fdActorSetScript(ACTOR_SECOND_GUIDE, Kalay_GuideCountScript);
		Kalay_PacingStep++;
		break;
	case 1:
		if(fdActorGet(ACTOR_SECOND_GUIDE)->tmr != 0) {
			scKalayClearScriptDone(fdActorGet(ACTOR_HEAD_GUIDE));
			fdActorSetScript(ACTOR_HEAD_GUIDE, Kalay_GuidePaceScript);
			Kalay_PacingStep++;
		}
		break;
	case 2:
		if(fdActorGet(ACTOR_HEAD_GUIDE)->tmr != 0) {
			scKalayClearScriptDone(fdActorGet(ACTOR_HEAD_GUIDE));
			fdActorSetScript(ACTOR_HEAD_GUIDE, Kalay_GuideCountScript);
			Kalay_PacingStep++;
		}
		break;
	case 3:
		if(fdActorGet(ACTOR_HEAD_GUIDE)->tmr != 0) {
			scKalayClearScriptDone(fdActorGet(ACTOR_SECOND_GUIDE));
			fdActorSetScript(ACTOR_SECOND_GUIDE, Kalay_GuidePaceScript);
			Kalay_PacingStep++;
		}
		break;
	case 4:
		if(fdActorGet(ACTOR_SECOND_GUIDE)->tmr != 0) {
			Kalay_PacingStep = 0;
		}
		break;
	}
}

/* The first time the party comes to the front of Hammet Palace, the two */
/* guards at the door know Ivan and take everyone in to Lady Layana. */
static void scKalayIvanMeetsPalaceGuards(void) {
	ACTOR *actor;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 192, 342);
	sySynch(1);
	fdCameraSetSpeed(0.2);
	fdCameraMoveTo(192, CAMERA_KEEP, 252, 1);
	FieldStateWork->transitionFrm = 40;
	fdEventStartScrTransition();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 192, 278);
	fdActorSetSpeed(ACTOR_PALACE_GUARD, 1);
	fdActorSetSpeed(ACTOR_SECOND_PALACE_GUARD, 1);
	fdActorGet(ACTOR_PALACE_GUARD)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorGet(ACTOR_SECOND_PALACE_GUARD)->facing = FACING_SOUTH_SOUTHWEST;
	sySynch(1);
	fdActorGet(ACTOR_PALACE_GUARD)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SECOND_PALACE_GUARD)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkTo(ACTOR_PALACE_GUARD, 184, 232);
	fdActorWalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 198, 232);
	fdActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_STAND);
	fdEventWait(20);
	fdActorGet(ACTOR_PALACE_GUARD)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SECOND_PALACE_GUARD)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_SHAKE_HEAD);
	fdEventSetMessageId(MsgKalayLordHammetsPalaceLordAway);
	/* "This is Lord Hammet's palace. The lord is away. Please, begone." */
	fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
	/* "We will happily welcome visitors once Lord Hammet has returned." */
	fdEventShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorWalkToAndWait(ACTOR_IVAN, 212, 268);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
	/* "Leave this to me." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorWalkToAndWait(ACTOR_IVAN, 202, 254);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(10);
	/* "It is I... Please permit me to see Lady Layana." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 10);
	fdActorShowEmote(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SECOND_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 40);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	/* "Don't you recognize me? I am Ivan, servant to Lord Hammet!" */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 2);
	/* "Ivan...?" */
	fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorStartRepeatedMotion(ACTOR_PALACE_GUARD, 2);
	/* "I didn't recognize you- You're so tan! But it really is you,..." */
	fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
	/* "Oh, it's Ivan! You've grown so much in such a short time, Ivan!" */
	fdEventShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
	/* "Have I really changed that much?" */
	/* Yes: "Really? I'm surprised... It doesn't feel like it's been that..." */
	/* No: "You probably wouldn't know. You only met me after I got left..." */
	fdEventAskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 1);
	fdActorSetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_SHAKE_HEAD);
	/* "That doesn't matter... What's important is that the thieves..." */
	fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 1);
	fdActorSetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	/* "Please tell us, will Lord Hammet be freed soon, too?" */
	fdEventShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	/* "Freed? Lord Hammet is a prisoner?" */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 1);
	/* "...You haven't heard? I can see we'll need to fill you in on..." */
	fdEventOpenMessage(ACTOR_PALACE_GUARD, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
		/* "No, Isaac! There are no secrets between us!" */
		fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
		FieldStateWork->msg++;
	}
	else {
		FieldStateWork->msg++;
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		/* "My companions and I have overcome many hardships together!" */
		syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0,
										10);
	}
	fdActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
	/* "Very well, then... Why were you not with Lord Hammet?" */
	fdEventShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	/* "I remained in Vault to look for his stolen rod." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 10);
	fdActorSetAttachedEffect(ACTOR_PALACE_GUARD,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	/* "Stolen? Someone stole his rod?" */
	fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "Yes, the Shaman's Rod. But I was able to recover it..." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 10);
	fdActorSetAttachedEffect(ACTOR_PALACE_GUARD,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_SECOND_PALACE_GUARD,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	fdActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 2);
	/* "Then Lord Hammet must still be imprisoned in Lunpa..." */
	fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 10);
	/* "I must report this to Lady Layana!" */
	fdEventShowMessageThenWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 10);
	fdActorSetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_NOD);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 10);
	/* "Ivan, come see Lady Layana with me... You can explain it all..." */
	fdEventShowMessage(ACTOR_PALACE_GUARD, 0);
	fdActorFaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
	/* "Isaac, you saw Lord Hammet heading north, didn't you?" */
	/* Yes: "That must have been when Lord Hammet went toward Lunpa." */
	/* No: "No, I mean the day Lord Hammet went toward Lunpa..." */
	fdEventAskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	/* "Lady Layana will surely be glad to hear your tale. Please,..." */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorSetSpeed(ACTOR_PALACE_GUARD, 0.8);
	fdActorSetSpeed(ACTOR_SECOND_PALACE_GUARD, 0.8);
	fdActorGet(ACTOR_PALACE_GUARD)->facing = FACING_EAST;
	fdActorGet(ACTOR_SECOND_PALACE_GUARD)->facing = FACING_WEST;
	fdActorGet(ACTOR_PALACE_GUARD)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SECOND_PALACE_GUARD)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkTo(ACTOR_PALACE_GUARD, 168, 232);
	fdActorWalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 212, 232);
	fdActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_STAND);
	fdEventWait(20);
	fdActorGet(ACTOR_PALACE_GUARD)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_SECOND_PALACE_GUARD)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_IVAN, 192, 232);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKalayAnimatePalaceDoor();
	fdActorWalkToAndWait(ACTOR_IVAN, 192, 222);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 192, 222);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
	fdActorWalkTo(ACTOR_PALACE_GUARD, 184, 232);
	fdActorWalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 198, 232);
	fdActorWalkTo(ACTOR_PALACE_GUARD, 188, 212);
	fdActorWalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 194, 212);
	fdActorSetPos(ACTOR_PALACE_GUARD, 0, 0);
	fdActorSetPos(ACTOR_SECOND_PALACE_GUARD, 0, 0);
	FieldStateWork->transitionFrm = 24;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM);
	fdEventEnd();
}

/* After the party has freed Hammet from Lunpa, he comes home with it in the */
/* covered wagon of Bunza, a Kalay merchant, to the Kalay Tunnel Entrance: the
 */
/* guard opens the way, Hammet goes in, and the party rides on to Kalay. */
static void scKalayHammetReturnsToPalace(void) {
	ACTOR *actor;
	FIELD_STATE *state;
	const s32 *exitActions;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_WAGON, 440, 522);
	fdActorSetSprFlags(fdActorGet(ACTOR_WAGON), 0);
	fdCameraMoveTo(440, CAMERA_KEEP, 522, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdEventStartScrTransition();
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(440, CAMERA_KEEP, 400, 1);
	sySndPlay(SE_RUMBLE);
	fdActorSetSpeed(ACTOR_WAGON, 1.6);
	fdActorWalkToAndWait(ACTOR_WAGON, 440, 400);
	fdCameraSetSpeed(0.75);
	fdCameraMoveTo(440, CAMERA_KEEP, 300, 1);
	fdActorSetSpeed(ACTOR_WAGON, 0.8);
	fdActorWalkToAndWait(ACTOR_WAGON, 440, 300);
	fdActorSetAnimation(ACTOR_WAGON, 0);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdEventWait(40);
	fdActorSetSpeed(ACTOR_BUNZA, 0.8);
	fdActorSetPos(ACTOR_BUNZA, 439, 306);
	fdActorJump(ACTOR_BUNZA, 4, 0);
	fdActorWalkToAndWait(ACTOR_BUNZA, 439, 312);
	fdActorWalkToAndWait(ACTOR_BUNZA, 416, 312);
	fdActorWalkToAndWait(ACTOR_BUNZA, 400, 256);
	fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 40);
	fdCameraMoveTo(410, CAMERA_KEEP, 280, 1);
	fdActorSetSpeed(ACTOR_HAMMET, 0.6);
	fdActorSetPos(ACTOR_HAMMET, 439, 306);
	fdActorJump(ACTOR_HAMMET, 4, 0);
	fdActorWalkToAndWait(ACTOR_HAMMET, 439, 312);
	fdActorWalkToAndWait(ACTOR_HAMMET, 416, 312);
	fdActorWalkToAndWait(ACTOR_HAMMET, 388, 270);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_NORTH_NORTHEAST, 10);
	fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetPos(ACTOR_PARTY_LEADER, 439, 306);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 388, 300);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	/* Seed the party idle phases for the arrival conversation. */
	fdActorGet(ACTOR_PARTY_LEADER)->userValue = syRandomBelow(80);
	fdActorSetScript(ACTOR_PARTY_LEADER, Kalay_PartyLookAboutScript);
	fdActorJump(ACTOR_BUNZA, 2, 20);
	fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	fdEventSetMessageId(MsgKalayWeveArrivedHammet);
	/* "We've arrived, Hammet." */
	fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorWalkTo(ACTOR_GARET, 378, 310);
	fdActorWalkTo(ACTOR_IVAN, 400, 288);
	fdActorWalkToAndWait(ACTOR_MIA, 410, 308);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 40);
	fdActorGet(ACTOR_GARET)->userValue = syRandomBelow(80);
	fdActorGet(ACTOR_IVAN)->userValue = syRandomBelow(80);
	fdActorGet(ACTOR_MIA)->userValue = syRandomBelow(80);
	fdActorSetScript(ACTOR_GARET, Kalay_PartyLookAboutScript);
	fdActorSetScript(ACTOR_IVAN, Kalay_PartyLookAboutScript);
	fdActorSetScript(ACTOR_MIA, Kalay_PartyLookAboutScript);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Arrived where?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
	fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHEAST, 20);
	/* "Oh, didn't I tell you about this entrance, Ivan?" */
	fdEventShowMessageThenWait(ACTOR_HAMMET, 0, 10);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_IVAN);
	fdActorStop(ACTOR_MIA);
	sySynch(1);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	/* "This underground passage leads to Hammet Palace." */
	fdEventShowMessageThenWait(ACTOR_HAMMET, 0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "An underground passage? What for?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	/* "Just to be safe." */
	fdEventShowMessageThenWait(ACTOR_HAMMET, 0, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "I don't understand. Why bring the wagon in this way?" */
	fdEventShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 0);
	fdActorRunRepeatedMotion(ACTOR_BUNZA, 2);
	/* "Because Dodonpa's henchmen may be here in Kalay..." */
	fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 10);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_NORTH_NORTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	/* "Well then, Bunza, would you please take them to the town's..." */
	fdEventShowMessageThenWait(ACTOR_HAMMET, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	fdActorSetAttachedEffect(ACTOR_IVAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	/* "What? Why? We'll just go this way with you." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimation(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	/* "You can't. You must go through town to the Palace." */
	fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "I'm confused..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHEAST, 10);
	fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "You have no reason to hide, now, do you?" */
	fdEventShowMessageThenWait(ACTOR_HAMMET, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	/* "Why shouldn't we just go in this way?" */
	fdEventShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetAnimation(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	/* "That would raise suspicions." */
	fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
	/* "Wouldn't it be strange if people who aren't even supposed to..." */
	fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_SHAKE_HEAD);
	/* "If anyone noticed, they might learn of this secret entrance." */
	fdEventShowMessageThenWait(ACTOR_HAMMET, 0, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
	/* "Now, do you understand?" */
	fdEventOpenMessage(ACTOR_HAMMET, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
		fdEventWait(20);
		FieldStateWork->msg += 3;
	}
	else {
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_BUNZA, 2);
		/* "In that case how about this?" */
		fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 40);
		/* "I think Hammet and Lady Layana need time for their reunion..." */
		fdEventShowMessageThenWait(ACTOR_BUNZA, 0, 10);
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
		/* "In that case, we will gladly ride in the wagon." */
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA, 0, 10);
	}
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_TUNNEL_GUARD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(280, CAMERA_KEEP, 200, 1);
	fdActorWalkToAndWait(ACTOR_HAMMET, 333, 222);
	fdActorWalkToAndWait(ACTOR_HAMMET, 284, 198);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_WEST, 10);
	fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
	fdActorFaceDirection(ACTOR_TUNNEL_GUARD, FACING_EAST, 10);
	fdActorSetAnimationAndWait(ACTOR_TUNNEL_GUARD, ANIM_NOD);
	fdActorSetSpeed(ACTOR_TUNNEL_GUARD, 0.8);
	fdActorWalkToAndWait(ACTOR_TUNNEL_GUARD, 268, 198);
	fdActorFaceDirection(ACTOR_TUNNEL_GUARD, FACING_NORTH, 10);
	fdActorRunRepeatedMotion(ACTOR_TUNNEL_GUARD, 2);
	/* The guard opens the passage and Hammet goes in. */
	sySndPlay(SE_CHEST_CLOSE);
	fdMapGridCopyCellsTo(71, 60, 76, 11, 2, 1);
	fdMapGridCopyCellAttributes(71, 60, 2, 1, 16, 11);
	fdEventWait(20);
	fdActorWalkToAndWait(ACTOR_TUNNEL_GUARD, 246, 198);
	fdActorFaceDirection(ACTOR_TUNNEL_GUARD, FACING_EAST, 20);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(248, CAMERA_KEEP, 170, 1);
	fdActorWalkToAndWait(ACTOR_HAMMET, 270, 198);
	fdActorWalkToAndWait(ACTOR_HAMMET, 270, 174);
	fdActorWalkToAndWait(ACTOR_HAMMET, 224, 170);
	fdActorWalkToAndWait(ACTOR_HAMMET, 210, 158);
	fdActorWalkToAndWait(ACTOR_HAMMET, 246, 148);
	fdActorWalkToAndWait(ACTOR_HAMMET, 246, 142);
	fdActorSetPos(ACTOR_HAMMET, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetPos(ACTOR_WAGON, 440, 340);
	fdActorGet(ACTOR_WAGON)->facing = FACING_SOUTH;
	fdCameraMoveTo(380, CAMERA_KEEP, 280, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(10);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_BUNZA, 420, 282);
	fdActorWalkToAndWait(ACTOR_BUNZA, 420, 312);
	fdActorWalkToAndWait(ACTOR_BUNZA, 439, 312);
	fdActorWalkToAndWait(ACTOR_BUNZA, 439, 306);
	fdActorSetPos(ACTOR_BUNZA, 0, 0);
	/* The party climbs back into the wagon. */
	exitActions = Kalay_PartyGatherScript;
	fdActorSetScript(ACTOR_GARET, exitActions);
	fdActorSetScript(ACTOR_IVAN, exitActions);
	fdActorRunScriptAndWait(ACTOR_MIA, exitActions);
	fdCameraMoveTo(410, CAMERA_KEEP, 300, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 439, 306);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	sySndPlay(SE_RUMBLE);
	fdActorSetSpeed(ACTOR_WAGON, 0.8);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(440, CAMERA_KEEP, 420, 1);
	fdActorWalkToAndWait(ACTOR_WAGON, 440, 420);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(440, CAMERA_KEEP, 600, 1);
	fdActorSetSpeed(ACTOR_WAGON, 1.6);
	fdActorWalkToAndWait(ACTOR_WAGON, 440, 500);
	fdActorWalkTo(ACTOR_WAGON, 440, 600);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	state = FieldStateWork;
	state->transitionFrm = 24;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH);
	fdEventEnd();
}

#include "shared/actor_find_at_cell.c"

/* Walking into the stone statue in either hall of the Kalay Tunnel pushes it */
/* one cell, when there is room, with Isaac following it. */
static void scKalayPushTunnelStatue(void) {
	FIELD_POS destination;
	ACTOR *leader;
	ACTOR *statue;
	s32 direction;
	s32 probeStep;
	s32 leaderX;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	direction = FACING_INDEX(leader->facing);
	leaderX = leader->x.part[0].integer;
	probeStep = PushedActor_DirectionSteps[direction];
	statue = scActorFindAtCell(
		(leaderX + DIRECTION_STEP_X(probeStep)) >> 4,
		(leader->z.part[0].integer + DIRECTION_STEP_Z(probeStep)) >> 4);
	if(statue != NULL) {
		statue->layer = MAPGRID_LAYER_OVERHEAD;
		fdActorStepFrom(&destination, statue,
						PushedActor_DirectionSteps[direction]);
		if(syFieldActorCheckMovementCollision(statue, &destination) <= 0) {
			fdActorSelectAnimation(leader, ANIM_PUSH);
			sySynch(15);
			sySndPlay(SE_PUSH);
			statue->speedLimit = toFIXED(1) / 5;
			statue->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(statue, destination.x, destination.y,
										destination.z);
			leader->speedLimit = toFIXED(1) / 5;
			leader->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(leader, destination.x, destination.y,
										destination.z);
			fdActorWaitForMove(statue);
			scPlayQueuedMusic();
			statue->x.fixed = destination.x;
			statue->z.fixed = destination.z;
			statue->velX = 0;
			statue->velZ = 0;
			fdActorSelectAnimation(leader, ANIM_STAND);
			scKalayUpdateTunnelPassages();
		}
	}
}

/* The eight pictures of a hall's waterfall running dry, copied in turn over it
 */
/* on the overhead layer: each one map column five cells tall from row 58. */
/* This is the first picture's column. */
#define WATERFALL_FIRST_FRAME_COLUMN 67
/* The picture from which the way across counts as open */
/* (FLAG_TUNNEL_FIRST_PASSAGE_OPEN, FLAG_TUNNEL_SECOND_PASSAGE_OPEN). */
#define WATERFALL_OPEN_FRAME_COLUMN 70
/* The last picture's column: the waterfall gone. */
#define WATERFALL_LAST_FRAME_COLUMN 74

/* The half-cell column each statue must be pushed west to, or past, to open */
/* its passage. */
#define FIRST_STATUE_OPENING_HALF_CELL	29
#define SECOND_STATUE_OPENING_HALF_CELL 87

/* Pushing a hall's stone statue in the Kalay Tunnel west until it stands in */
/* front of the waterfall stops the waterfall and drains the pool below it, */
/* leaving a dry bed with ladders down its sides that Isaac can cross. In the */
/* west hall a puddle is left in the bed, for Frost. Each pool drains once per
 */
/* visit to the tunnel, after the statue's push or Move. */
static void scKalayUpdateTunnelPassages(void) {
	ACTOR *firstStatue;
	ACTOR *secondStatue;
	s32 firstPassageOpen;
	ACTOR *puddle;
	u32 i;

	firstStatue = fdActorGet(ACTOR_FIRST_STATUE);
	secondStatue = fdActorGet(ACTOR_SECOND_STATUE);
	firstPassageOpen = gmFlagGet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
	if(firstPassageOpen == 0 && FIELD_HALF_CELL(firstStatue->x.fixed) <=
									FIRST_STATUE_OPENING_HALF_CELL) {
		puddle = fdActorGet(ACTOR_PUDDLE);
		fdEventStart();
		/* Cell (0, 1), outside every camera box of the map, takes the cell */
		/* behavior of the Spirit Gloves chest's cell (7, 44): render probe
		 * scene */
		/* KalayTunnelTreasureRoom --cells 0,0,1,1 reads 100 there once the pool
		 */
		/* has drained, 0 before. Nothing on screen changes, and no function */
		/* reads a cell by that number. */
		/* XXX: whether anything answers it there; a probe that searches cell */
		/* (0, 1) after the draining, which the recorder cannot stage outside */
		/* every camera box, would settle it. */
		fdMapGridCopyCellAttributes(7, 44, 1, 1, 0, 1);
		for(i = WATERFALL_FIRST_FRAME_COLUMN; i <= WATERFALL_LAST_FRAME_COLUMN;
			i++) {
			fdMapGridCopyCellsTo(i, 58, 78, 41, 1, 5);
			fdEventWait(4);
			if(i == WATERFALL_OPEN_FRAME_COLUMN) {
				gmFlagSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
			}
		}
		/* The middle layer's cells (13, 45) to (15, 46), at the waterfall's
		 * foot */
		/* (the map's row 109 is the layer's row 45), take the plain cells
		 * beside */
		/* them at (16, 45). */
		fdMapGridCopyCellsTo(16, 109, 13, 109, 3, 2);
		fdEventWait(40);
		/* The pool drains in three stages, 40 frames apart, and the puddle
		 * grows */
		/* out of its bed. */
		puddle->scaleX = FIXED_ONE / 10;
		puddle->scaleY = FIXED_ONE / 10;
		fdActorSetPos(ACTOR_PUDDLE, 150, 728);
		fdActorSetScript(ACTOR_PUDDLE, Kalay_PuddleGrowScript);
		fdMapGridCopyCellsTo(67, 64, 71, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 72, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 68, 73, 43, 1, 2);
		fdMapGridCopyCellsTo(67, 68, 74, 43, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 75, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 76, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 77, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 78, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 79, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 80, 44, 1, 2);
		fdMapGridCopyCellsTo(2, 0, 9, 42, 2, 2);
		fdEventWait(40);
		fdMapGridCopyCellsTo(68, 64, 71, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 72, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 68, 73, 43, 1, 2);
		fdMapGridCopyCellsTo(68, 68, 74, 43, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 75, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 76, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 77, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 78, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 79, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 80, 44, 1, 2);
		fdMapGridCopyCellsTo(4, 0, 9, 42, 2, 2);
		fdEventWait(40);
		/* The drained floor, on the ground layer and the overhead layer, and
		 * its */
		/* ground. */
		fdMapGridCopyCellsTo(7, 11, 7, 42, 10, 8);
		fdMapGridCopyCellsTo(71, 12, 71, 43, 10, 13);
		fdMapGridCopyCellAttributes(6, 13, 12, 12, 6, 44);
		fdEventWait(40);
		scKalayStartTrackingLeader();
		fdMapGridCopyCellAttributes(0, 1, 1, 1, 7, 44);
		fdEventEnd();
	}
	if(gmFlagGet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) == 0 &&
	   FIELD_HALF_CELL(secondStatue->x.fixed) <=
		   SECOND_STATUE_OPENING_HALF_CELL) {
		fdEventStart();
		for(i = WATERFALL_FIRST_FRAME_COLUMN; i <= WATERFALL_LAST_FRAME_COLUMN;
			i++) {
			fdMapGridCopyCellsTo(i, 58, 107, 41, 1, 5);
			fdEventWait(4);
			if(i == WATERFALL_OPEN_FRAME_COLUMN) {
				gmFlagSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN);
			}
		}
		fdMapGridCopyCellsTo(45, 109, 42, 109, 3, 2);
		fdEventWait(40);
		/* The east pool drains the same way, with no puddle. */
		fdMapGridCopyCellsTo(67, 64, 102, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 103, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 104, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 105, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 106, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 107, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 64, 108, 44, 1, 2);
		fdMapGridCopyCellsTo(67, 66, 109, 44, 1, 2);
		fdEventWait(40);
		fdMapGridCopyCellsTo(68, 64, 102, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 103, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 104, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 105, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 106, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 107, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 64, 108, 44, 1, 2);
		fdMapGridCopyCellsTo(68, 66, 109, 44, 1, 2);
		fdEventWait(40);
		fdMapGridCopyCellsTo(38, 14, 38, 44, 8, 4);
		fdMapGridCopyCellsTo(102, 14, 102, 44, 8, 12);
		fdMapGridCopyCellAttributes(37, 13, 10, 12, 37, 43);
		fdEventWait(40);
		scKalayStartTrackingLeader();
		fdEventEnd();
	}
}

/* The share of a tunnel particle's sideways and vertical speed that drag takes
 */
/* each frame, as one part in this many. */
#define TUNNEL_PARTICLE_DRAG_X 256
#define TUNNEL_PARTICLE_DRAG_Y 16

/* An actor script call, each frame: a tunnel particle swells and drifts, */
/* slowed by drag. On its last frame it stops and moves to x 0 at height 0, and
 */
/* then it is released. */
/* UB: returns nothing, so SC_CALL takes whatever value is left as its result,
 */
/* which is not 0, and calls it again the next frame; UBFIX returns 1. */
#ifdef UBFIX
static s32 scKalayUpdateTunnelParticle(ACTOR *particle) {
#else
static void scKalayUpdateTunnelParticle(ACTOR *particle) {
#endif
	s32 velocityX;
	s32 velocityY;

	if(particle->tmr == 0) {
		fdActorRelease(particle);
	}
	else if(particle->tmr == 1) {
		particle->velX = 0;
		particle->velY = 0;
		particle->x.fixed = 0;
		particle->y.fixed = 0;
	}
	else {
		particle->scaleX += FIXED_ONE / 32;
		particle->scaleY += FIXED_ONE / 32;
	}
	particle->x.fixed += particle->velX;
	particle->y.fixed += particle->velY;
	velocityX = particle->velX;
	velocityY = particle->velY;
	particle->velX = velocityX - velocityX / TUNNEL_PARTICLE_DRAG_X;
	particle->velY = velocityY - velocityY / TUNNEL_PARTICLE_DRAG_Y;
	particle->tmr--;
#ifdef UBFIX
	return 1;
#endif
}

/* Where the first statue stands once pushed a cell east of where it starts, */
/* below the barred gate, in pixels: there it is drawn at the leader's */
/* priority, and the gate hides its top as it would his (render event --scene */
/* KalayTunnelTreasureRoom --at-px 360,826 --face east --hold right:90). */
#define FIRST_STATUE_BY_GATE_WEST_X	 381
#define FIRST_STATUE_BY_GATE_EAST_X	 393
#define FIRST_STATUE_BY_GATE_NORTH_Z 777

/* The x at or west of which each hall's statue, near its waterfall, sparkles */
/* while its pool is still full. */
#define FIRST_STATUE_SPARKLE_X	245
#define SECOND_STATUE_SPARKLE_X 709

/* Each frame in the Kalay Tunnel's halls: while a pool is still full, sparkles
 */
/* stream from its statue as it nears the waterfall, with the sound of water */
/* draining the first time. While the first statue stands a cell east of where
 */
/* it starts, it is drawn at the leader's priority. */
static void scKalayUpdateTunnelStatues(void) {
	ACTOR *firstStatue;
	ACTOR *secondStatue;

	firstStatue = fdActorGet(ACTOR_FIRST_STATUE);
	secondStatue = fdActorGet(ACTOR_SECOND_STATUE);
	if((u32)(firstStatue->x.part[0].integer - FIRST_STATUE_BY_GATE_WEST_X) <=
		   FIRST_STATUE_BY_GATE_EAST_X - FIRST_STATUE_BY_GATE_WEST_X &&
	   firstStatue->z.part[0].integer > FIRST_STATUE_BY_GATE_NORTH_Z) {
		firstStatue->spr->image.pri =
			fdActorGet(ACTOR_PARTY_LEADER)->spr->image.pri;
	}
	else if(!gmFlagGet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) &&
			firstStatue->x.part[0].integer <= FIRST_STATUE_SPARKLE_X &&
			(FrameCount & 1) == 0) {
		if(!fdEventGetFlag(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED)) {
			scQueueMusic(-1);
			sySndPlay(SE_WATER_DRAIN);
			gmFlagSet(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED);
		}
		scKalaySpawnTunnelParticle(firstStatue->x.fixed, firstStatue->y.fixed,
								   firstStatue->z.fixed);
	}
	if(!gmFlagGet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) &&
	   secondStatue->x.part[0].integer <= SECOND_STATUE_SPARKLE_X &&
	   (FrameCount & 1) == 0) {
		if(!fdEventGetFlag(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED)) {
			scQueueMusic(-1);
			sySndPlay(SE_WATER_DRAIN);
			gmFlagSet(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED);
		}
		scKalaySpawnTunnelParticle(secondStatue->x.fixed, secondStatue->y.fixed,
								   secondStatue->z.fixed);
	}
}

/* Creates a short-lived particle with random position and velocity offsets. It
 */
/* is a puff of dust (SPRITE_DUST_PUFF) with its blue raised to full. */
static void scKalaySpawnTunnelParticle(s32 x, s32 height, s32 z) {
	ACTOR *particle;

	/* It starts up to 7 pixels above 16 over the height. */
	particle =
		syFieldActorCreate(SPRITE_DUST_PUFF, x - toFIXED(8),
						   toFIXED(syRandomBelow(8)) + height + toFIXED(16), z);
	if(particle != NULL) {
		particle->motionFlg = 0;
		particle->spr->image.pri = 2;
		fdActorSetSprCol(particle, SPRITE_COLOR_BLUE_TINT);
		fdActorSetSprFlags(particle, 0);
		/* It drifts up to a pixel left and up to three up or down each frame.
		 */
		particle->velX = toFIXED((syRandomBelow(2)) - 1);
		particle->velY = toFIXED((syRandomBelow(6)) - 3);
		particle->tmr = 20;
		particle->frozen = 1;
		fdActorSelectAnimation(particle, 1);
		fdActorStartScript(particle, Kalay_TunnelParticleScript);
	}
}

/* The Nut in Kalay circles its spot and rocks as it goes. */
static void scKalayUpdateNutOrbit(void) {
	ORBITING_MAP_ACTOR *nut = (ORBITING_MAP_ACTOR *)fdActorGet(ACTOR_TOWN_NUT);
	SPR *sprite = (SPR *)nut->spr;
	FIXED offset = sySin(nut->orbitAng) * 2;

	if(offset > 0) {
		offset = -offset;
	}
	nut->x = nut->orbitCenterX + syCos(nut->orbitAng) * 2;
	nut->y = nut->orbitCenterY + offset;
	sprite->rotation = syCos(nut->orbitAng + DEGtoANG(180)) >> 3;
	nut->orbitAng += syRandomBelow(512) + syRandomBelow(512) + TRIG_TURN / 64;
}

/* Frost on the puddle in the drained west pool of the Kalay Tunnel freezes it
 */
/* and sinks it into the floor, out of Isaac's way. */
static void scKalayFreezePuddle(void) {
	ACTOR *puddle = fdActorGet(ACTOR_PUDDLE);

	fdMapGridCopyCellAttributes(0, 0, 1, 1, 9, 14);
	fdMapGridCopyCellAttributes(0, 0, 1, 1, 9, 45);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_TUNNEL_PUDDLE_FROZEN);
}

#include "shared/floating_nut.c"

#include "shared/pushed_actor_direction_steps.c"

s32 Kalay_TourGroupWalkScript[] = {
	SC_ANIMATION(2), SC_MOVE_BY(-80, 0, 0), SC_SYNC,
	SC_ANIMATION(1), SC_SET(SW_TIMER, 1),	SC_WAIT(1),
	SC_END,
};

s32 Kalay_TourGroupLeaveScript[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(-72, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(-100, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(-100, 0, 0),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_TIMER, 1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
	SC_ANIMATION(2),
	SC_MOVE(192, 0, 222),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 Kalay_WaitingTouristScript[] = {
	SC_CALL(scKalayUpdateWaitingTourist),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 Kalay_GuideCountScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(228, 0, 296),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(40),
	SC_ANIMATION(2),
	SC_MOVE(212, 0, 296),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(40),
	SC_ANIMATION(2),
	SC_MOVE(192, 0, 296),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(40),
	SC_SET(SW_TIMER, 1),
	SC_ANIMATION(2),
	SC_MOVE(178, 0, 272),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

s32 Kalay_GuidePaceScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(164, 0, 278),
	SC_SYNC,
	SC_MOVE(182, 0, 304),
	SC_SYNC,
	SC_MOVE(238, 0, 304),
	SC_SYNC,
	SC_MOVE(246, 0, 296),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(40),
	SC_SET(SW_TIMER, 1),
	SC_END,
};

s32 Kalay_HeadGuideStopScript[] = {
	SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
	SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
	SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
	SC_SET(SW_VELOCITY_X, 0),
	SC_SET(SW_VELOCITY_Y, 0),
	SC_SET(SW_VELOCITY_Z, 0),
	SC_WAIT(1),
	SC_ANIMATION(2),
	SC_MOVE(184, 0, 294),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(SW_TIMER, 1),
	SC_WAIT(1),
	SC_END,
};

s32 Kalay_SecondGuideStopScript[] = {
	SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
	SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
	SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
	SC_SET(SW_VELOCITY_X, 0),
	SC_SET(SW_VELOCITY_Y, 0),
	SC_SET(SW_VELOCITY_Z, 0),
	SC_WAIT(1),
	SC_ANIMATION(2),
	SC_MOVE(198, 0, 300),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(SW_TIMER, 1),
	SC_WAIT(1),
	SC_END,
};

s32 Kalay_HeadGuideReturnScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(178, 0, 272),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(SW_TIMER, 0),
	SC_WAIT(1),
	SC_END,
};

s32 Kalay_SecondGuideReturnScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(255, 0, 289),
	SC_SYNC,
	SC_MOVE(265, 0, 280),
	SC_SYNC,
	SC_MOVE(284, 0, 272),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_SET(SW_TIMER, 0),
	SC_WAIT(1),
	SC_END,
};

s32 Kalay_PartyLookAboutScript[] = {
	SC_CALL(scLookAboutRandomly),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 Kalay_PartyGatherScript[] = {
	SC_ANIMATION(2), SC_MOVE(388, 0, 300), SC_SYNC,
	SC_ANIMATION(1), SC_POSITION(0, 0, 0), SC_END,
};

s32 Kalay_PuddleGrowScript[] = {
	SC_ADD(SW_SCALE_X, 546),
	SC_ADD(SW_SCALE_Y, 546),
	SC_WAIT(1),
	SC_LOOP(108, SC_TOP),
	SC_END,
};

#include "shared/kalay_fallback_entrances.c"

SCENE_ENTRANCE Kalay_TownEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 224, 0, 280, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{KALAY_FROM_HOUSE_NORTHWEST, CONDITION_ALWAYS, 72, 0, 200, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{KALAY_FROM_HOUSE_NORTHEAST_UPPER, CONDITION_ALWAYS, 408, 0, 184,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_FROM_HOUSE_NORTHEAST_LOWER, CONDITION_ALWAYS, 408, 0, 216,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_FROM_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 408, 0, 408, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{KALAY_FROM_WEAPON_SHOP, CONDITION_ALWAYS, 328, 0, 440, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{KALAY_FROM_ARMOR_SHOP, CONDITION_ALWAYS, 120, 0, 264, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{KALAY_FROM_INN, CONDITION_ALWAYS, 312, 0, 264, FACING_SOUTH, 0, -1, -1, -1,
	 -1, 0},
	{KALAY_FROM_KALAY_SANCTUM, CONDITION_ALWAYS, 152, 0, 440, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{KALAY_FROM_WORLD_MAP, CONDITION_ALWAYS, 224, 0, 464, FACING_NORTH, 0, -1,
	 -1, -1, -1, 0},
	{KALAY_FROM_WORLD_MAP_WEST_UNUSED, CONDITION_ALWAYS, 56, 0, 272,
	 FACING_EAST, 0, -1, -1, -1, -1, 0},
	{KALAY_FROM_HAMMET_PALACE_GROUNDS, CONDITION_ALWAYS, 224, 0, 88,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_FROM_WEAPON_SHOP_COUNTER, CONDITION_ALWAYS, 296, 0, 408,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_FROM_ARMOR_SHOP_COUNTER, CONDITION_ALWAYS, 152, 0, 232, FACING_SOUTH,
	 0, -1, -1, -1, -1, 0},
	{KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM, CONDITION_ALWAYS, 440, 0, 88,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE Kalay_HammetPalaceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{HAMMET_PALACE_GROUNDS_FROM_KALAY, CONDITION_ALWAYS, 192, 0, 280,
	 FACING_NORTH, 0, 0, 0, 400, 352, 0},
	{HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE, CONDITION_ALWAYS, 192, 0, 232,
	 FACING_SOUTH, 0, 0, 0, 400, 352, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE Kalay_TunnelEntranceEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE, CONDITION_ALWAYS, 248, 0,
	 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 424, 0, 520,
	 FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 440, 0, 540,
	 FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE Kalay_TunnelCaveEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE, CONDITION_ALWAYS, 296, 0, 104,
	 FACING_SOUTH, 0, 100, 50, 440, 445, 0},
	{KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE, CONDITION_ALWAYS, 280, 0,
	 392, FACING_NORTH, 0, 100, 50, 440, 445, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE Kalay_TunnelPassageEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST,
	 CONDITION_ALWAYS, 296, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST,
	 CONDITION_ALWAYS, 632, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM, CONDITION_ALWAYS, 152,
	 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_FROM_LONG_PATH, CONDITION_ALWAYS, 568,
	 0, 152, FACING_SOUTH, 0, 390, 0, 630, 220, 0},
	{KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_CAVE, CONDITION_ALWAYS, 472, 0, 72,
	 FACING_SOUTH, 0, 390, 0, 630, 220, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE Kalay_TunnelEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE, CONDITION_ALWAYS, 408, 0,
	 632, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
	{KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST,
	 CONDITION_ALWAYS, 296, 0, 488, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
	{KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST,
	 CONDITION_ALWAYS, 744, 0, 488, FACING_SOUTH, 0, 595, 400, 930, 880, 0},
	{KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY, CONDITION_ALWAYS, 856, 0, 856,
	 FACING_NORTH, 0, 595, 400, 930, 880, 0},
	{SCENE_TABLE_END},
};

u32 Kalay_Exits[] = {
	SCENE_EXITS(Kalay),
	SCENE_EXIT(KALAY_TO_HOUSE_NORTHWEST, StuccoRoomsA,
			   STUCCO_ROOMS_A_HOUSE_NORTHWEST),
	SCENE_EXIT(KALAY_TO_HOUSE_NORTHEAST_UPPER, StuccoRoomsA,
			   STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER),
	SCENE_EXIT(KALAY_TO_HOUSE_NORTHEAST_LOWER, StuccoRoomsA,
			   STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER),
	SCENE_EXIT(KALAY_TO_HOUSE_SOUTHEAST, StuccoRoomsA,
			   STUCCO_ROOMS_A_HOUSE_SOUTHEAST),
	SCENE_EXIT(KALAY_TO_WEAPON_SHOP, StuccoRoomsA, STUCCO_ROOMS_A_WEAPON_SHOP),
	SCENE_EXIT(KALAY_TO_ARMOR_SHOP, StuccoRoomsA, STUCCO_ROOMS_A_ARMOR_SHOP),
	SCENE_EXIT(KALAY_TO_INN, StuccoRoomsA, STUCCO_ROOMS_A_INN),
	SCENE_EXIT(KALAY_TO_KALAY_SANCTUM, KalaySanctum, KALAY_SANCTUM_FROM_KALAY),
	SCENE_EXIT(KALAY_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KALAY_EAST),
	SCENE_EXIT(KALAY_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_KALAY_WEST),
	SCENE_EXIT(KALAY_TO_HAMMET_PALACE_GROUNDS, HammetPalaceGrounds,
			   HAMMET_PALACE_GROUNDS_FROM_KALAY),
	SCENE_EXIT(KALAY_TO_WEAPON_SHOP_COUNTER, StuccoRoomsA,
			   STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER),
	SCENE_EXIT(KALAY_TO_ARMOR_SHOP_COUNTER, StuccoRoomsA,
			   STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER),
	SCENE_EXIT(KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM, KalayTunnelTreasureRoom,
			   KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY),
	SCENE_EXITS(HammetPalaceGrounds),
	SCENE_EXIT(HAMMET_PALACE_GROUNDS_TO_KALAY, Kalay,
			   KALAY_FROM_HAMMET_PALACE_GROUNDS),
	SCENE_EXIT(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE, HammetPalace,
			   HAMMET_PALACE_FROM_HAMMET_PALACE_GROUNDS),
	SCENE_EXIT(HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE_LAYANAS_ROOM,
			   HammetPalace,
			   HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS),
	SCENE_EXITS(KalayTunnelEntrance),
	SCENE_EXIT(KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE, KalayTunnelCave,
			   KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_ENTRANCE),
	SCENE_EXIT(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH, WorldMap,
			   WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH),
	SCENE_EXIT(KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_SOUTH, WorldMap,
			   WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH),
	SCENE_EXITS(KalayTunnelCave),
	SCENE_EXIT(KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE, KalayTunnelPassage,
			   KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_CAVE),
	SCENE_EXIT(KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE, KalayTunnelEntrance,
			   KALAY_TUNNEL_ENTRANCE_FROM_KALAY_TUNNEL_CAVE),
	SCENE_EXITS(KalayTunnelPassage),
	SCENE_EXIT(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST,
			   KalayTunnelTreasureRoom,
			   KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_WEST),
	SCENE_EXIT(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST,
			   KalayTunnelTreasureRoom,
			   KALAY_TUNNEL_TREASURE_ROOM_FROM_KALAY_TUNNEL_PASSAGE_EAST),
	SCENE_EXIT(KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM,
			   KalayTunnelPassage,
			   KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_FROM_LONG_PATH),
	SCENE_EXIT(KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH,
			   KalayTunnelPassage,
			   KALAY_TUNNEL_PASSAGE_LONG_PATH_FROM_NORTHEAST_ROOM),
	SCENE_EXIT(KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE, KalayTunnelCave,
			   KALAY_TUNNEL_CAVE_FROM_KALAY_TUNNEL_PASSAGE),
	SCENE_EXITS(KalayTunnelTreasureRoom),
	SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE, HammetPalace,
			   HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM),
	SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST,
			   KalayTunnelPassage,
			   KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_WEST),
	SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST,
			   KalayTunnelPassage,
			   KALAY_TUNNEL_PASSAGE_FROM_KALAY_TUNNEL_TREASURE_ROOM_EAST),
	SCENE_EXIT(KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY, Kalay,
			   KALAY_FROM_KALAY_TUNNEL_TREASURE_ROOM),
	SCENE_EXITS_END,
};

ACTOR_PLACE Kalay_NoActorPlacements[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE Kalay_TownActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        188, 0, 476, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        260, 0, 476, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        222, 0, 382, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 134, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        115, 0, 453, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        484, 0, 258, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        359, 0, 314, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        404, 0, 268, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        154, 0, 119, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        198, 0, 84, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        199, 0, 233, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        46, 0, 217, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        468, 0, 434, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        464, 0, 416, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 408, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        278, 0, 318, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        56, 0, 332, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Kalay_TourGroupActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Kalay_HammetPalaceActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        212, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Kalay_TunnelEntranceActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 198, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LARGE_WAGON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE Kalay_TunnelActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 826, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        808, 0, 732, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SCORCH), MOVEMENT_TYPE_STAND,
        632, 0, 712, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

TRIGGER Kalay_NoTriggers[] = {
    { TRIGGER_TABLE_END },
};

TRIGGER Kalay_TownTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_HOUSE_NORTHEAST_UPPER, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_HOUSE_NORTHEAST_LOWER, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_HOUSE_SOUTHEAST, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_WEAPON_SHOP, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_INN, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALAY_TO_KALAY_SANCTUM, CONDITION_ALWAYS, EVENT(scKalayOpenDoorAndExit) },
    { TRIGGER_EXIT, KALAY_TO_HAMMET_PALACE_GROUNDS, CONDITION_ALWAYS, KALAY_TO_HAMMET_PALACE_GROUNDS },
    { TRIGGER_EXIT, KALAY_TO_WEAPON_SHOP_COUNTER, CONDITION_ALWAYS, KALAY_TO_WEAPON_SHOP_COUNTER },
    { TRIGGER_EXIT, KALAY_TO_ARMOR_SHOP_COUNTER, CONDITION_ALWAYS, KALAY_TO_ARMOR_SHOP_COUNTER },
    { TRIGGER_EXIT, KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM, CONDITION_ALWAYS, KALAY_TO_KALAY_TUNNEL_TREASURE_ROOM },
    { TRIGGER_STEP, KALAY_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(scKalayWatchTourGroupDeparture) },
    { TRIGGER_STEP, KALAY_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, EVENT(scKalayWatchTourGroupDeparture) },
    { TRIGGER_TALK, ACTOR_WELCOMING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgKalayWelcomeInPeace) },
    { TRIGGER_TALK, ACTOR_SOLDIER_PROTECTING_THE_PEACE, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersProtectPeace) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_HAMMET_THE_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayHammetGenerousMerchant) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_HAMMETS_CARAVANS, CONDITION_ALWAYS, MSGNO(MsgKalayHammetOnJourney) },
    { TRIGGER_TALK, ACTOR_MAN_ON_THE_REBUILT_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgKalayNorthernBridgeRebuilt) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_LAMAKAN_DESERT, CONDITION_ALWAYS, MSGNO(MsgKalayLamakanTrouble) },
    { TRIGGER_TALK, ACTOR_LATE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayElderTownNews) },
    { TRIGGER_TALK, ACTOR_WOULD_BE_TOURIST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInnFestivalTourists) },
    { TRIGGER_TALK, ACTOR_WOULD_BE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayFormerInnFestivalTourists) },
    { TRIGGER_TALK, ACTOR_BOY_DREAMING_OF_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayChildWantsFestival) },
    { TRIGGER_TALK, ACTOR_GIRL_ON_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgKalayTraderTownNews) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_HAMMETS_KNACK, CONDITION_ALWAYS, MSGNO(MsgKalayHammetBusinessSkill) },
    { TRIGGER_TALK, ACTOR_MAN_WHO_LEFT_TOLBI, CONDITION_ALWAYS, EVENT(scKalayTalkAboutLeavingHome) },
    { TRIGGER_TALK, ACTOR_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCapturedRumor) },
    { TRIGGER_TALK, ACTOR_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayHammetWentNorth) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(scKalayTalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(scKalayTalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(scKalayTalkToItemMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorHonestEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_PROTECTING_THE_PEACE, CONDITION_ALWAYS, MSGNO(MsgKalayGuardsJudgeVisitors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_HAMMET_THE_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayHammetPeaceAndLiberty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_HAMMETS_CARAVANS, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCanBeRash) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_REBUILT_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersWentToVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_LAMAKAN_DESERT, CONDITION_ALWAYS, MSGNO(MsgKalayStrangeEvents) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LATE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayElderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOULD_BE_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayWishSavedForFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DREAMING_OF_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayChildFestivalDream) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgKalayTraderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_HAMMETS_KNACK, CONDITION_ALWAYS, MSGNO(MsgKalayHammetValuesTrust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_LEFT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTrustAgainstForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEAD_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayLunpaNorthOfVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayRumorsHideTruth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayWishBetterWeapons) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayWeaponsHaveSoul) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayPalaceWaterSupply) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TOWN_STATUE, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_TOWN_NUT, FLAG_KALAY_TOWN_NUT_CAUGHT, EVENT(scKalayCatchTownNut) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_KALAY_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_KALAY_VIAL_JAR, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 90, CONDITION_ALWAYS, EVENT(scKalaySearchWell) },
    { TRIGGER_TABLE_END },
};

TRIGGER Kalay_HammetPalaceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, HAMMET_PALACE_GROUNDS_TO_KALAY },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), HAMMET_PALACE_GROUNDS_TO_HAMMET_PALACE, CONDITION_ALWAYS, EVENT(scKalayEnterPalace) },
    { TRIGGER_TALK, ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersHandleAffairs) },
    { TRIGGER_TALK, ACTOR_SECOND_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersRescueHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayPrayForHammetSafety) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorsWillSaveHammet) },
    { TRIGGER_TABLE_END },
};

TRIGGER Kalay_TunnelEntranceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KALAY_TUNNEL_ENTRANCE_TO_KALAY_TUNNEL_CAVE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KALAY_TUNNEL_ENTRANCE_TO_WORLD_MAP_NORTH },
    { TRIGGER_TALK, ACTOR_TUNNEL_GUARD, CONDITION_ALWAYS, EVENT(scKalayAskAboutGoingToCave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TUNNEL_GUARD, CONDITION_ALWAYS, MSGNO(MsgKalayPassageClosedByOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_TUNNEL_ENTRANCE_NUT, FLAG_KALAY_TUNNEL_ENTRANCE_NUT_CAUGHT, EVENT(scKalayCatchTunnelEntranceNut) },
    { TRIGGER_TABLE_END },
};

TRIGGER Kalay_TunnelCaveTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_PASSAGE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KALAY_TUNNEL_CAVE_TO_KALAY_TUNNEL_ENTRANCE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_KALAY_TUNNEL_APPLE_CHEST, FIND_ITEM(ITEM_APPLE) },
    { TRIGGER_TABLE_END },
};

TRIGGER Kalay_TunnelPassageTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_TREASURE_ROOM_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_LONG_PATH_TO_NORTHEAST_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_NORTHEAST_ROOM_TO_LONG_PATH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, KALAY_TUNNEL_PASSAGE_TO_KALAY_TUNNEL_CAVE },
    { TRIGGER_TABLE_END },
};

TRIGGER Kalay_TunnelTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_HAMMET_PALACE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY_TUNNEL_PASSAGE_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, KALAY_TUNNEL_TREASURE_ROOM_TO_KALAY },
    { TRIGGER_TALK, ACTOR_WAITING_MAN, CONDITION_ALWAYS, EVENT(scKalayTalkAboutLayana) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTreasureBeyondDoors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_STATUE, FLAG_TUNNEL_FIRST_PASSAGE_OPEN, EVENT(scKalayUpdateTunnelPassages) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_STATUE, FLAG_TUNNEL_SECOND_PASSAGE_OPEN, EVENT(scKalayUpdateTunnelPassages) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_KALAY_TUNNEL_HALL, CONDITION_ALWAYS, EVENT(scKalayPushTunnelStatue) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_KALAY_TUNNEL_HALL_EDGE, CONDITION_ALWAYS, EVENT(scKalayPushTunnelStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_KALAY_TUNNEL_STATUE_PUSH_WEST, CONDITION_ALWAYS, EVENT(scKalayPushTunnelStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_KALAY_TUNNEL_STATUE_PUSH_CORNER, CONDITION_ALWAYS, EVENT(scKalayPushTunnelStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_KALAY_TUNNEL_STATUE_PUSH_CORNER, CONDITION_ALWAYS, EVENT(scKalayPushTunnelStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_TUNNEL_PUDDLE_FROZEN, EVENT(scKalayFreezePuddle) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KALAY_TUNNEL_HALL, CONDITION_ALWAYS, EVENT(scKalayStopTrackingLeader) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KALAY_TUNNEL_HALL_EDGE, CONDITION_ALWAYS, EVENT(scKalayStartTrackingLeader) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KALAY_TUNNEL_KEEP_DRAWN, CONDITION_ALWAYS, EVENT(scKalayFixLeaderPriority) },
    { TRIGGER_STEP, CELL_BEHAVIOR_KALAY_TUNNEL_DRAWN_BY_PLACE, CONDITION_ALWAYS, EVENT(scKalayFreeLeaderPriority) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_KALAY_TUNNEL_SPIRIT_GLOVES_CHEST, FIND_ITEM(ITEM_SPIRIT_GLOVES) },
    { TRIGGER_TALK, ACTOR_SCORCH, CONDITION_ALWAYS, EVENT(scKalayScorchJoins) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_KALAY_TUNNEL_SLEEP_BOMB_CHEST, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_KALAY_TUNNEL_LUCKY_MEDAL_CHEST, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_KALAY_TUNNEL_COINS_CHEST, FIND_COINS(200) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 105, FLAG_KALAY_TUNNEL_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */

/* A door of the town opening, and where each door is. */
u16 Kalay_DoorCellSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 33, 1, 2, 5),
	MAPGRID_ANIMATION_STEP(37, 33, 1, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
MAP_GRID_CELL_ANIMATION Kalay_DoorCells[] = {
	{Kalay_DoorCellSteps, 36, 10}, {Kalay_DoorCellSteps, 57, 7},
	{Kalay_DoorCellSteps, 57, 11}, {Kalay_DoorCellSteps, 57, 23},
	{Kalay_DoorCellSteps, 52, 25}, {Kalay_DoorCellSteps, 39, 14},
	{Kalay_DoorCellSteps, 51, 14}, {Kalay_DoorCellSteps, 41, 25},
};

s32 Kalay_TunnelParticleScript[] = {
	SC_CALL(scKalayUpdateTunnelParticle),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
