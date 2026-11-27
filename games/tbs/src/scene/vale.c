/* Vale by day (SceneId_Vale), Isaac's village at the foot of Mt. Aleph, outside
   the houses. A river runs down it from a waterfall in the north. In the north
   are the Sanctum, a small stone shrine, and west of it the path to the steps
   up Mt. Aleph. Isaac's house stands in the middle, east of the river, with a
   ladder against its front; Garet's house is north-east of it, below the cliff
   where Garet's training stone stands in his sister's flowers. Two ways lead
   west out of the village to Kraden's cottage, and puppies play in the southern
   one. In the south are the house of Jenna's grandparents, a house on the west
   bank wrecked by the storm with its door knocked off its hinges, the rope
   bridge across the river, and the way east to the plaza.

   The actor placements and cues come in three sets, chosen by the story flags:
   before the party leaves Vale, after it has left (FLAG_VALE_PARTY_LEFT) and
   back from Kalay (FLAG_KALAY_REACHED). The opening (VALE_FROM_TITLE) has a set
   of its own.

   In story order:

   - The day the story begins, three years after the storm (cottage_rooms_b.c),
	 which took Jenna's family and Isaac's father, Kyle. The game opens with
	 scValeMendingTheRoof. Under the caption "Three years later...", Isaac is
	 mending the roof of his house while Dora, his mother, calls up to him.
	 Garet, who will travel with him, trains his Psynergy on his training
	 stone. Jenna comes up from the south, and the two speak of that day before
	 going to fetch Isaac to go to Kraden's. The party comes back in front of
	 Isaac's house (VALE_FROM_ROOF_MENDED), where scValeRoofMended plays once.
	 Dora praises the roof and remembers Kyle, and Jenna and Garet tell her
	 they are going up Mt. Aleph with Kraden, the scholar who teaches them
	 Alchemy. For the rest of that visit Dora frets about the hole clumsy
	 Garet left.
   - Vale before the party leaves. Until Kraden has planned the visit to Sol
	 Sanctum (FLAG_SANCTUM_VISIT_PLANNED), Jenna turns Isaac back at the steps
	 up Mt. Aleph. The first time Isaac takes the path there, a bearded man in
	 a long robe jumps out from the trees by the river to warn him against
	 sneaking up the sacred peak. He then walks a beat in front of the Sanctum
	 and along the path, and drives Isaac back whenever he catches him on it.
	 Three puppies play in the southern way to Kraden's cottage. Walking up to
	 them from either side sets them barking and Isaac turns back, until the
	 party has met Saturos and Menardi, two of the strangers staying at the
	 inn; then the puppies are gone. Villagers talk of the mayor, Garet's
	 grandfather, and of not going beyond Kraden's cottage. Garet's sister
	 scolds him for crushing her flowers, and Dora says Isaac can fix the hole
	 later.
   - After the party has left Vale (FLAG_VALE_PARTY_LEFT), Dora will not open
	 the door of Isaac's house to him ("My Isaac would keep his promise... Go
	 away."). The villagers speak of the tremors and of Kraden, who has not
	 come back.
   - Back from Kalay (FLAG_KALAY_REACHED), Garet meets his sister by the
	 training stone and asks his little brother to protect the family while he
	 is gone. A young man asks whether Jenna is back, a woman feels another
	 tremor, and a man wonders why no travellers have come since the eruption.
	 Isaac's house opens again, and Dora lies ill inside (cottage_rooms_b.c).

   In every set Catch takes the Nut from the tree on the ledge by the river
   above the wrecked house (FLAG_VALE_NUT_CAUGHT), Isaac can push or Move
   Garet's training stone a metatile east or west, and he can climb the ladder
   onto the roof of his house. The game also changes metatiles as Isaac steps on
   and off the ladder's top and the rope bridges, which the player does not see
   (FLAG_VALE_ON_ROOF and FLAG_VALE_ON_BRIDGE). After a battle
   (FLAG_KEEP_PARTY_POSITION) the setup puts both back as they were.

   The ways out:

   - The steps in the north-west go up Mt. Aleph to Sol Sanctum's door
	 (SceneId_SolEntrance).
   - Two ways out at the west edge lead to the part of Vale around Kraden's
	 cottage (SceneId_ValeKradensCottage), and one at the east edge in the
	 south to the plaza with the Psynergy Stone (SceneId_ValePlaza).
   - The doors of Garet's house, the house north-west of Isaac's, the house of
	 Jenna's grandparents and the house south of theirs lead into the rooms of
	 SceneId_CottageRoomsA (cottage_rooms_a.c), and the door of Isaac's house
	 into SceneId_CottageRoomsB.
   - The Sanctum's door leads into SceneId_Sanctum.

   Two scripts no table reaches remain. They are scValeUnusedRoofMended, a copy
   of the roof's end, and scValeUnusedWaterBurstsByTheRiver.

   The man guarding Mt. Aleph stands still while a menu is open
   (scValeSynchronizeAlephGuardPause). The Spanish and Italian editions also set
   Dora's sprite priority before Kalay.

   In the opening Garet trains his Psynergy on his training stone. His sprite
   flashes white, then cycles through its colour modes with sparks about him.
   Meanwhile the hand of Move pushes the stone west and sheds sparkles, and the
   stone cycles its colour modes too. */

#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "scene/floating_nut.h"
#include "scene/actor_polar_target.h"
#include "scene/catch_nut.h"
#include "scene/script_psynergy.h"
#include "scene/vale.h"
#include "scene/walk_into_leader.h"
#include "scene/golden_ring.h"
#include "text/msg_ids.h"

extern Sint32		  ValeAlephGuardPatrolRoute[];
extern Sint32		  ValeLeaderHammers[];
extern Sint32		  ValeGaretWalksToTheHouse[];
extern Sint32		  ValeUnusedWaterGrows[];
extern Sint32		  ValeSparkleScript[];
extern SCENE_ENTRANCE ValeEntrances[];
extern Uint32		  ValeExits[];
extern ACTOR_PLACE	  ValeActorPlacements[];
extern ACTOR_PLACE	  ValeActorPlacementsAfterLeaving[];
extern ACTOR_PLACE	  ValeActorPlacementsAfterKalay[];
extern ACTOR_PLACE	  ValeRepairActorPlacements[];
extern CUE			  ValeCues[];
extern CUE			  ValeCuesAfterLeaving[];
extern Uint16		  ValeSanctumDoorOpens[];
extern Uint16		  ValeIsaacsDoorOpens[];
extern Uint16		  ValeGrandparentsDoorOpens[];
extern Uint16		  ValeWestHouseDoorOpens[];
extern Sint32		  ValeFirstWarningLeaderPath[];
extern Sint32		  ValeFirstWarningGuardPath[];
extern Sint32		  ValeLeaderRunsBackFromThePath[];
extern Sint32		  ValeGuardComesDownThePath[];
extern Sint32		  ValeLeaderRunsBackBeforeTheDoor[];
extern Sint32		  ValeGuardStepsBeforeTheDoor[];

extern CUE	  ValeCuesAfterKalay[];
extern Sint32 ValeJennaWalksToTheHouse[];

/* Where Garet lands after his jump once Dora has gone in, at the roof's end
   (scValeRoofMended): two pixels further south in the German, Spanish, French
   and Italian editions. */
#define VALE_GARET_REUNION_Z \
	WHEN(VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH, 829, 827)

/* Where Dora stands by the house in the opening and before the party leaves:
   two pixels further north in the Spanish and Italian editions. */
#define VALE_DORA_Z WHEN(VALE_DORA_SPOT_MATCHES_ROOF_SCENE, 887, 889)

/* Where Garet's training stone stands in the west of the three metatiles it can
   be pushed between below the cliff, (22, 36) to (24, 36), two pixels east of
   each metatile's middle. */
#define PILLAR_WEST_X	362
#define PILLAR_MIDDLE_X 378 /* in the middle column */
#define PILLAR_EAST_X	394 /* in the east column */
#define PILLAR_Z		584 /* in row 36 */

static void scValeJennaSaysKradenIsWaiting(void);
static void scValeAskAboutSneakingToAleph(
	const Sint32 *leaderActions, const Sint32 *guardActions);
static void scValeClimbUpRoofLadder(Sint32 actorId, Sint32 approachAnmId,
	Sint32 departureAnmId, Sint32 resumeMotion);
static void scValeClimbDownRoofLadder(Sint32 actorId, Sint32 approachAnmId,
	Sint32 departureAnmId, Sint32 resumeMotion);
static void scValePlaceOrHideBoards(Sint32 first, Uint32 cnt, Sint32 mode);
static void scValeMendingTheRoof(void);
static void scValeRoofMended(void);
static void scValeUpdateGaretPaletteAndEffects(void);
static void scValeUpdateLeaderPaletteAndEffects(void);
static void scValeCycleOpeningPillarPalette(void);
static void scValeCycleFirstBoardPalette(void);
static void scValeCycleSecondBoardPalette(void);
static void scValeCycleThirdBoardPalette(void);
static UNREAD_RESULT scValeHaveGuardWatchTheLeader(ACTOR *actor);
static void			 scValeTrailSparkles(ACTOR *actor);
static void			 scValeVanishMoveHand(ACTOR *emitter);
static void			 fdActorSetTargetPosFromMagnitudeAng(
			 ACTOR *actor, Sint32 magnitude, Sint32 ang);
static void scValeMovePillarBelowCliff(void);
static void scValePlayPsynergySparkleSnd(void);
static void scValeSynchronizeAlephGuardPause(void);
static void scValeSetPillarPriorityByLeaderHeight(void);
static void scValeFlashActorAndSpawnEffects(ACTOR *actor);
static void scValeCycleActorPaletteAndSpawnEffects(ACTOR *actor);
static void scValeCycleActorPalette(ACTOR *actor);

/* The placed actors of Vale by day, numbered in the order of
   ValeActorPlacements and ValeActorPlacementsAfterLeaving, which place the
   same people in the same rows.

   A row whose sprite is a party member's (Garet's, Jenna's) takes that party
   member's actor id instead (fdScriptSpawnActorPlacements), so the numbering
   skips it. */
typedef enum VALE_ACTOR_ID {
	/* An old woman. "Mt. Aleph looks beautiful today. It's so peaceful." After
	   the party has left, "Vale is saved thanks to the Wise One!" */
	ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH = ACTOR_FIRST_PLACED,
	/* A woman in a green dress. "Vale stays pristine because of Mt. Aleph's
	   purity." */
	ACTOR_WOMAN_ON_ALEPHS_PURITY,
	/* A little girl with orange pigtails. "I'm jealous of you, Garet, because
	   your grandpa's the mayor." (scValeTalkAboutTheMayorAndTremors) */
	ACTOR_GIRL_JEALOUS_OF_GARET,
	/* A young man with brown hair. "This place looks just like it did three
	   years ago... after the storm..." */
	ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM,
	/* A strongman by the river. "Isaac, watch out! Don't get too close to the
	   river!" */
	ACTOR_STRONGMAN_BY_THE_RIVER,
	/* A burly grey-haired man. "Vale must remain hidden. Psynergy's secrets
	   mustn't fall into the wrong hands." */
	ACTOR_MAN_KEEPING_VALE_HIDDEN,
	/* A woman with fair hair who stands by the way out to Kraden's cottage. "Do
	   you need to go past here?", then "You mustn't go beyond Kraden's
	   cottage." (scValeWarnAboutGoingBeyondKradensCottage) */
	ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN,
	/* The three puppies, small white dogs, removed once the party has met
	   Saturos and Menardi. "Rrruff! Rrrruff!" */
	ACTOR_FIRST_PUPPY,
	/* A young woman with an orange ponytail. "Garet, you make me so mad
	   sometimes!" (scValeArgueWithGaretsSister) Garet calls her "Sis" back from
	   Kalay. */
	ACTOR_GARETS_SISTER = ACTOR_FIRST_PUPPY + 3,
	/* "You're in trouble now, Garet... Your sister's really mad!" After the
	   party has left, "Please take care of my brother, Isaac." */
	ACTOR_GARETS_BROTHER,
	/* Garet's training stone, a stone pillar below the cliff north of his
	   house, which crushed his sister's flowers. Isaac pushes it
	   (scValePushPillar) or moves it with Move (scValeMovePillarBelowCliff). */
	ACTOR_PILLAR,
	ACTOR_DORA, /* Isaac's mother */
	/* A man with long brown hair and a beard in a long robe. He jumps out to
	   warn Isaac off the sacred peak, then keeps watching him
	   (scValeWarnAgainstSneakingUpMtAleph). "Mt. Aleph is a sacred peak. It is
	   forbidden to climb it!" */
	ACTOR_MAN_GUARDING_MT_ALEPH,
	/* The boards on the roof, SPRITE_ROOF_BOARDS, which Isaac moves into place
	   with Psynergy at the roof's end (scValeRoofMended). */
	ACTOR_FIRST_BOARD,
	ACTOR_SECOND_BOARD,
	ACTOR_THIRD_BOARD,
	ACTOR_NUT /* the Nut for Catch, until FLAG_VALE_NUT_CAUGHT is set */
} VALE_ACTOR_ID;

/* Back from Kalay, the placed actors of ValeActorPlacementsAfterKalay. Garet's
   row takes his own actor id. */
typedef enum VALE_AFTER_KALAY_ACTOR_ID {
	/* Garet's sister. "Ah! It's been a while... You're back?"
	   (scValeGaretMeetsHisSister) */
	ACTOR_GARETS_SISTER_AFTER_KALAY = ACTOR_FIRST_PLACED,
	/* "I will do my best to show everyone that I am strong enough to protect
	   our family and Vale." (scValeGaretAsksToProtectFamily) */
	ACTOR_GARETS_BROTHER_AFTER_KALAY,
	/* The young man of ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM. "Is Jenna back
	   yet?" (scValeAskWhetherJennaHasReturned) */
	ACTOR_YOUNG_MAN_ASKING_FOR_JENNA,
	/* The strongman of ACTOR_STRONGMAN_BY_THE_RIVER. "I still remember when
	   Felix and I used to play here...*Sniff*" */
	ACTOR_STRONGMAN_REMEMBERING_FELIX,
	/* Three sprites of water, CrossboneIsleFloodgate's water sprite
	   SPRITE_SPLASH. Only scValeUnusedWaterBurstsByTheRiver shows them. During
	   a tremor each in turn grows from a tenth of its size at (266, 1230),
	   north of ACTOR_STRONGMAN_REMEMBERING_FELIX, and vanishes
	   (ValeUnusedWaterGrows).

	   The row after them places the woman with fair hair of
	   ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN at the origin, and nothing names her. */
	ACTOR_FIRST_WATER,
	/* An old woman. "Mt. Aleph's eruption was so powerful, it caused the
	   continent to crack!" */
	ACTOR_OLD_WOMAN_ON_THE_ERUPTION = ACTOR_FIRST_WATER + 4,
	/* A woman in a green dress. "Aghh! I felt another one!"
	   (scValeFeelAnotherTremor) */
	ACTOR_WOMAN_FEELING_TREMORS,
	/* A little girl with orange pigtails. "They say the danger is gone, even
	   though the earthquakes continue..." */
	ACTOR_GIRL_DOUBTING_SAFETY,
	/* A burly grey-haired man. "I wonder why we haven't seen any travelers
	   since the eruption..." (scValeAskAboutTheLackOfTravelers) */
	ACTOR_MAN_MISSING_TRAVELERS,
	ACTOR_NUT_AFTER_KALAY,
	ACTOR_PILLAR_AFTER_KALAY
} VALE_AFTER_KALAY_ACTOR_ID;

/* The game's opening (scValeMendingTheRoof), the placed actors of
   ValeRepairActorPlacements. Jenna's and Garet's rows take their own actor
   ids. */
typedef enum VALE_OPENING_ACTOR_ID {
	/* A woman in a green dress who meets Jenna on her way. */
	ACTOR_OPENING_WOMAN = ACTOR_FIRST_PLACED,
	/* The pillar of ACTOR_PILLAR, which Garet trains his Psynergy on. */
	ACTOR_OPENING_PILLAR,
	ACTOR_OPENING_DORA, /* calls up to Isaac on the roof */
	/* The boards on the roof, as ACTOR_FIRST_BOARD. */
	ACTOR_OPENING_FIRST_BOARD,
	ACTOR_OPENING_SECOND_BOARD,
	ACTOR_OPENING_THIRD_BOARD,
	/* SPRITE_MOVE_HAND, the hand of Move (script_commands_catch.c). It pushes
	   the pillar for Garet, shedding sparkles. */
	ACTOR_OPENING_MOVE_HAND
} VALE_OPENING_ACTOR_ID;

/* What Vale's metatiles do when Isaac steps on them, walks into them or
   searches them, by the cue rows that answer them. */
typedef enum VALE_METATILE_BEHAVIOR {
	/* The Sanctum's door: metatiles (15, 27) and (16, 27). */
	METATILE_BEHAVIOR_VALE_SANCTUM_DOOR = 1,
	/* The steps up Mt. Aleph in the north-west: metatiles (5, 16) to (8,
	   16). */
	METATILE_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH,
	/* The northern way west to Kraden's cottage: metatiles (0, 38) to (0,
	   40). */
	METATILE_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN,
	/* The southern way west, where the puppies play: (0, 49) and (0, 50). */
	METATILE_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN,
	/* The way east to the plaza, in the south: metatiles (29, 86) and (29,
	   87). */
	METATILE_BEHAVIOR_VALE_WAY_TO_THE_PLAZA,
	/* The door of Garet's house: metatile (25, 46). */
	METATILE_BEHAVIOR_VALE_GARETS_DOOR,
	/* The door of the house north-west of Isaac's: metatile (16, 51). */
	METATILE_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR,
	/* The door of Isaac's house: metatile (21, 56). */
	METATILE_BEHAVIOR_VALE_ISAACS_DOOR,
	/* The door of Jenna's grandparents' house: metatile (20, 71). */
	METATILE_BEHAVIOR_VALE_GRANDPARENTS_DOOR,
	/* The door of the house south of theirs: metatile (23, 78). */
	METATILE_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR,
	/* Unused: no metatile carries it. */
	METATILE_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR,
	/* A metatile beyond each end of the rope bridges, a step from the river. */
	METATILE_BEHAVIOR_VALE_OFF_BRIDGE = 14,
	/* The metatiles at each end of the rope bridges. */
	METATILE_BEHAVIOR_VALE_ONTO_BRIDGE,
	/* North of the puppies: metatiles (1, 47) to (5, 47) and (1, 48). */
	METATILE_BEHAVIOR_VALE_NORTH_OF_PUPPIES = 20,
	/* South of the puppies: metatiles (3, 53) and (4, 53). */
	METATILE_BEHAVIOR_VALE_SOUTH_OF_PUPPIES,
	/* The stuck door of the wrecked house on the west bank: metatile (6,
	   74). */
	METATILE_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR = 24,
	/* West of Garet's training stone, pushing it east. */
	METATILE_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE = 30,
	/* East of Garet's training stone, pushing it west. */
	METATILE_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE,
	/* The north end of the path up the Sanctum's west side: metatile (10,
	   23). */
	METATILE_BEHAVIOR_VALE_GUARD_JUMPS_OUT = 50,
	/* The path up the Sanctum's west side: metatiles (11, 23) to (13, 27), and
	   (12, 30) and (13, 30) south of its foot. */
	METATILE_BEHAVIOR_VALE_PATH_UP_MT_ALEPH,
	/* The corner west of the Sanctum's front: metatiles (14, 28) and (14,
	   29). */
	METATILE_BEHAVIOR_VALE_BESIDE_THE_SANCTUM,
	/* The foot of the path: metatiles (12, 28) to (13, 29). */
	METATILE_BEHAVIOR_VALE_FOOT_OF_THE_PATH,
	/* The foot of the ladder against Isaac's house: metatile (24, 55). */
	METATILE_BEHAVIOR_VALE_LADDER_FOOT = 100,
	/* The top of the ladder, on the roof: metatile (24, 53) once the roof's
	   metatiles are in. */
	METATILE_BEHAVIOR_VALE_LADDER_TOP
} VALE_METATILE_BEHAVIOR;

/* Which stretch of his beat the man guarding Mt. Aleph is on, kept in his
   SW_TIMER (ValeAlephGuardPatrolRoute). He catches Isaac sneaking only on the
   first two. */
typedef enum VALE_GUARD_BEAT {
	/* Up and down the path along the Sanctum's west side. */
	GUARD_ON_THE_PATH = 1,
	/* Along the front of the Sanctum, past its door. */
	GUARD_BEFORE_THE_SANCTUM,
	/* Up the Sanctum's east side, behind it. */
	GUARD_BEHIND_THE_SANCTUM
} VALE_GUARD_BEAT;

/* The leader's animation of hammering on the roof, beyond enum
   ACTOR_ANIMATION's. */
enum { ANIM_HAMMER = 11 };

/* Animations of Garet's sprite beyond ACTOR_ANIMATION, as its animation table
   and ticks under graphics/sprite draw them. */
typedef enum VALE_ANIMATION {
	/* Garet throwing up his arms, then crouching, as he jumps in fright
	   (garet.png). */
	ANIM_GARET_STARTLED = 13
} VALE_ANIMATION;

#include "shared/pushed_actor.c"

/* The roof of Isaac's house has just been mended. For the rest of the visit
   Dora frets about the hole Garet left. */
#define FLAG_VALE_ROOF_JUST_MENDED				FLAG_SCENE_TEMP_02
/* The leader has climbed the ladder onto the roof of Isaac's house, whose
   metatiles are changed so it can be walked. */
#define FLAG_VALE_ON_ROOF						FLAG_SCENE_TEMP_04
/* The man guarding Mt. Aleph is paused while a menu is open. */
#define FLAG_VALE_MT_ALEPH_GUARD_PAUSED			FLAG_SCENE_TEMP_41
/* Back from Kalay, Garet has met his sister. */
#define FLAG_VALE_GARET_MET_SISTER				FLAG_AREA_TEMP_03
/* Back from Kalay, Garet has asked his brother to protect the family while he
   is gone. */
#define FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY FLAG_AREA_TEMP_04
/* Unused: the water has burst up by the river and the strongman has remembered
   Felix (scValeUnusedWaterBurstsByTheRiver, which nothing reaches). */
#define FLAG_VALE_UNUSED_WATER_BURST_SEEN		FLAG_AREA_TEMP_05
/* Back from Kalay, the burly man has asked about the lack of travellers since
   the eruption. */
#define FLAG_VALE_ASKED_ABOUT_TRAVELERS			FLAG_AREA_TEMP_07
/* scValeRoofMended has played, in which Dora praises the roof and sends the
   three off to Mt. Aleph. */
#define FLAG_VALE_ROOF_MENDED_SCRIPT_PLAYED		FLAG_AREA_TEMP_08
/* The stone pillar below the cliff stands in its west column. */
#define FLAG_VALE_PILLAR_IN_WEST_COLUMN			FLAG_AREA_TEMP_14
/* The stone pillar below the cliff stands in its middle column, where it
   starts. */
#define FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN		FLAG_AREA_TEMP_15
/* The stone pillar below the cliff stands in its east column. */
#define FLAG_VALE_PILLAR_IN_EAST_COLUMN			FLAG_AREA_TEMP_16

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return ValeEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return ValeExits;
}

/* Places the villagers for the story so far, or for the game's opening, when
   Isaac is mending the roof (VALE_FROM_TITLE). */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.entrance == VALE_FROM_TITLE)
		return ValeRepairActorPlacements;
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0)
		return ValeActorPlacementsAfterKalay;
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
		return ValeActorPlacementsAfterLeaving;
	return ValeActorPlacements;
}

/* Catch on the Nut in the tree on the ledge by the river, above the house the
   storm wrecked on the west bank, plucks it from the tree, and Isaac holds it
   up and gets it.

   The Nut hangs at metatile (9, 63) and floats in it until FLAG_VALE_NUT_CAUGHT
   is set (scFloatNut, scCatchNut). */
static void scValeCatchNut(void)
{
	scCatchNut(ACTOR_NUT, FLAG_VALE_NUT_CAUGHT);
}

/* Back from Kalay, Catch on the Nut in the tree on the ledge by the river,
   above the wrecked house on the west bank, plucks it from the tree for Isaac,
   if he did not take it before.

   The placements back from Kalay number the Nut ACTOR_NUT_AFTER_KALAY. */
static void scValeCatchNutAfterKalay(void)
{
	scCatchNut(ACTOR_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT);
}

/* Vale's cues for the story so far: before the party leaves, after it has left,
   or back from Kalay. */
const CUE *scGetCues(void)
{
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0)
		return ValeCuesAfterKalay;
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
		return ValeCuesAfterLeaving;
	return ValeCues;
}

/* A little girl with orange pigtails, west of Garet's training stone, tells
   Garet she is jealous that his grandpa is the mayor and asks whether Isaac is
   too. After the party has left she speaks of the tremors. */
static void scValeTalkAboutTheMayorAndTremors(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		fdScriptSetMessageId(MsgValeTheGroundStillShakes);
		/* "The ground still shakes sometimes, but everyone says the..." */
		fdScriptShowMessage(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
	} else {
		fdScriptSetMessageId(MsgValeYourGrandpaIsTheMayor);
		fdActorFaceEachOther(
			ACTOR_GIRL_JEALOUS_OF_GARET, ACTOR_PARTY_LEADER, 4);
		/* "I'm jealous of you, Garet, because your grandpa's the mayor...."
		   Yes: "Yeah... When travelers come, you get to hear all about the..."
		   No: "Hmph... I can't believe you're not jealous, Isaac. I sure..." */
		fdScriptAskYesNo(ACTOR_GIRL_JEALOUS_OF_GARET, 0);
	}
	fdScriptEnd();
}

/* A woman with fair hair at the northern way west to Kraden's cottage asks
   whether Isaac needs to go past, and warns him not to go beyond Kraden's
   cottage. After the party has left she says Kraden still has not come back.

   She asks only the first time (FLAG_VALE_ASKED_IF_GOING_TO_KRADEN), and after
   that gives the warning alone. */
static void scValeWarnAboutGoingBeyondKradensCottage(void)
{
	Sint32 asked = FLAG_VALE_ASKED_IF_GOING_TO_KRADEN;

	fdScriptStart();
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		fdScriptSetMessageId(MsgValeKradenHasntComeBack);
		/* "I guess Kraden still hasn't come back." */
		fdScriptShowMessage(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
	} else if(gmFlagGet(asked) == 0) {
		gmFlagSet(asked);
		fdScriptSetMessageId(MsgValeDoYouNeedToGo);
		fdActorFaceEachOther(
			ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, ACTOR_PARTY_LEADER, 4);
		/* "Do you need to go past here?" Yes: "You must be going to Kraden's
		   cottage..." No: "You mustn't go beyond Kraden's cottage. There are
		   monsters..." */
		fdScriptAskYesNo(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
	} else {
		fdScriptSetMessageId(MsgValeDontGoBeyondKradensCottage);
		fdActorFaceEachOther(
			ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, ACTOR_PARTY_LEADER, 4);
		/* "You mustn't go beyond Kraden's cottage. There are monsters..." */
		fdScriptShowMessage(ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, 0);
	}
	fdScriptEnd();
}

/* Garet's sister, by his training stone, scolds him: "Your training stone
   crushed my flowers!" Afterwards she only threatens him with what will happen
   tonight. */
static void scValeArgueWithGaretsSister(void)
{
	fdScriptStart();
	if(fdScriptGetFlag(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS) == 0) {
		gmFlagSet(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
		fdScriptSetMessageId(MsgValeYouMakeMeSoMad);
		fdActorShowEmote(
			ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_SISTER, 20);
		/* "Garet, you make me so mad sometimes!" */
		fdScriptShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 6);
		fdActorFaceDirection(ACTOR_GARETS_SISTER, FACING_WEST, 30);
		fdActorJump(ACTOR_GARETS_SISTER, 2, 20);
		/* "Your training stone crushed my flowers!" */
		fdScriptShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 6);
		fdActorFaceEachOther(ACTOR_GARETS_SISTER, ACTOR_PARTY_LEADER, 10);
		fdActorShowEmote(
			ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		/* "You're in for it tonight... Just wait and see!" */
		fdScriptShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 10);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	} else {
		fdActorShowEmote(
			ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdScriptSetMessageId(MsgValeIllGetYouForMy);
		/* "I'll get you for my flowers! Just see what happens...tonight!" */
		fdScriptShowMessageThenWait(ACTOR_GARETS_SISTER, 0, 20);
	}
	fdScriptEnd();
}

/* Dora, by the door of Isaac's house, speaks of the hole in the roof before the
   party leaves. On the morning the roof was mended she frets about the hole
   clumsy Garet left; later she says it won't rain for some time, so Isaac can
   fix it later. */
static void scValeDoraTalksAboutTheHole(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_VALE_ROOF_JUST_MENDED) != 0) {
		fdScriptSetMessageId(MsgValeIToldGaretItWas);
	} else {
		fdScriptSetMessageId(MsgValeItWontRainForSome);
	}
	fdScriptShowMessage(ACTOR_DORA, 0);
	fdScriptEnd();
}

/* Back from Kalay, the young man by the river in the south of Vale asks Isaac
   whether Jenna is back yet, and makes him promise to bring her home.

   Mind Read plays the same the first time. After he has asked
   (FLAG_VALE_ASKED_IF_JENNA_IS_BACK), talking gives his plea alone and Mind
   Read a thought of his. */
static void scValeAskWhetherJennaHasReturned(void)
{
	fdScriptStart();
	fdActorFaceEachOther(
		ACTOR_PARTY_LEADER, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 20);
	fdScriptSetMessageId(MsgValeIsJennaBackYet);
	/* "Oh, it's Isaac, isn't it? Is Jenna back yet? Promise me,..." */
	fdScriptShowMessage(ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, 0);
	gmFlagSet(FLAG_VALE_ASKED_IF_JENNA_IS_BACK);
	fdScriptEnd();
}

/* Back from Kalay, a woman in a green dress cries out as the ground shakes:
   "Aghh! I felt another one!" The tremors keep her awake at night. */
static void scValeFeelAnotherTremor(void)
{
	fdScriptStart();
	fdMapGridSetShake(3, 3, 1);
	sySynch(10);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptSetMessageId(MsgValeIFeltAnotherOne);
	/* "Aghh! I felt another one!" */
	fdScriptShowMessageThenWait(ACTOR_WOMAN_FEELING_TREMORS, 0, 20);
	fdActorFaceActor(ACTOR_WOMAN_FEELING_TREMORS, ACTOR_PARTY_LEADER, 20);
	/* "They've been going on for a while. I can't even sleep at night!" */
	fdScriptShowMessage(ACTOR_WOMAN_FEELING_TREMORS, 0);
	fdScriptEnd();
}

/* Back from Kalay, a burly grey-haired man wonders why no travellers have come
   since the eruption, and asks whether Isaac knows. "Yes" tells him the bridge
   to the south was ruined.

   Talking to him always plays this. Mind Read plays it too until he has asked
   (FLAG_VALE_ASKED_ABOUT_TRAVELERS), and then reads a thought of his. */
static void scValeAskAboutTheLackOfTravelers(void)
{
	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_MAN_MISSING_TRAVELERS, 2);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_MAN_MISSING_TRAVELERS, ACTOR_PARTY_LEADER, 20);
	fdScriptSetMessageId(MsgValeNoTravelersSinceTheEruption);
	/* "I wonder why we haven't seen any travelers since the..." Yes: "Oh, the
	   bridge to the south was ruined? That explains it." No: "You don't know
	   either? It's not like we banned visitors..." */
	fdScriptAskYesNo(ACTOR_MAN_MISSING_TRAVELERS, 0);
	gmFlagSet(FLAG_VALE_ASKED_ABOUT_TRAVELERS);
	fdScriptEnd();
}

/* Nothing visible happens as Isaac steps up to either end of the rope bridge
   across the river in the south of Vale, at metatiles (8, 85) to (8, 88) on the
   west bank or (18, 84) to (18, 87) on the east. The game lays the bridge's
   planks into the metatiles over the river, so that he can walk across it.

   Until then the metatiles under that bridge are the river's, which he cannot
   walk on. The planks are copied from the bridge's own layer, metatiles (40,
   84) to (46, 87) of the map, onto metatiles (10, 84) to (16, 87), and
   FLAG_VALE_ON_BRIDGE keeps them there after a battle. The ends of the rope
   bridge west of Isaac's house, metatiles (5, 56) to (5, 57) and (13, 56) to
   (13, 57), run this too, though that bridge is always open. */
static void scValeStepOntoBridge(void)
{
	gmFlagSet(FLAG_VALE_ON_BRIDGE);
	fdMapGridCopyMetatileAttributes(40, 84, 7, 4, 10, 84);
}

/* Nothing visible happens as Isaac steps off either end of the rope bridge
   across the river in the south of Vale, a metatile further from the river than
   scValeStepOntoBridge's metatiles. The game puts the river's metatiles back
   under the bridge.

   The river's metatiles are kept below the bridge's own, at metatiles (40, 89)
   to (46, 92) of the map. The ends of the bridge west of Isaac's house run this
   too. */
static void scValeStepOffBridge(void)
{
	gmFlagClear(FLAG_VALE_ON_BRIDGE);
	fdMapGridCopyMetatileAttributes(40, 89, 7, 4, 10, 84);
}

/* Walking into the door of the Sanctum, the small stone shrine in the north of
   Vale, at metatiles (15, 27) and (16, 27), opens it with a thump, and Isaac
   walks inside. */
static void scValeEnterSanctum(void)
{
	sySndPlay(SE_THUMP);
	fdMapGridAnimateMetatiles(ValeSanctumDoorOpens, 45, 11);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 257, 420);
	syImportScriptSetExitRequest(VALE_TO_SANCTUM);
}

/* Climbing the steps up Mt. Aleph in the north-west of Vale, metatiles (5, 16)
   to (8, 16), takes Isaac up to Sol Sanctum's door, once Kraden has planned the
   visit. Before that, Jenna steps out to turn him back
   (scValeJennaSaysKradenIsWaiting).

   Kraden plans the visit in his cottage (FLAG_SANCTUM_VISIT_PLANNED,
   vale_kradens_cottage.c). */
static void scValeApproachMtAleph(void)
{
	if(gmFlagGet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
		scValeJennaSaysKradenIsWaiting();
	} else {
		sySndPlay(SE_MAP_EXIT);
		syImportScriptSetExitRequest(VALE_TO_SOL_ENTRANCE);
	}
}

/* Walking out of Vale by the northern of its two ways west, metatiles (0, 38)
   to (0, 40), takes Isaac on to the part of the village around Kraden's
   cottage. */
static void scValeExitToValeKradensCottageNorth(void)
{
	sySndPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(VALE_TO_VALE_KRADENS_COTTAGE_NORTH);
}

/* Walking out of Vale by the southern of its two ways west, where the puppies
   play, metatiles (0, 49) and (0, 50), takes Isaac on to the part of the
   village around Kraden's cottage. */
static void scValeExitToValeKradensCottageSouth(void)
{
	sySndPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(VALE_TO_VALE_KRADENS_COTTAGE_SOUTH);
}

/* Walking out of Vale by the path at its east edge in the south, metatiles (29,
   86) and (29, 87), takes Isaac on to the plaza with the Psynergy Stone and the
   town gate. */
static void scValeExitToValePlaza(void)
{
	sySndPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(VALE_TO_VALE_PLAZA);
}

/* Walking into the door of Garet's house, north-east of Isaac's, at metatile
   (25, 46), opens it, and Isaac walks in to Garet's family.

   The door opens with the same pictures as Isaac's (ValeIsaacsDoorOpens). */
static void scValeEnterGaretsHouse(void)
{
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValeIsaacsDoorOpens, 54, 32);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 406, 727);
	syImportScriptSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_EAST);
}

/* Walking into the door of the house just north-west of Isaac's, at metatile
   (16, 51), opens it, and Isaac walks in to the villagers who talk of Kraden
   and his visitors. */
static void scValeEnterCottageRoomsA(void)
{
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValeGrandparentsDoorOpens, 45, 39);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 262, 805);
	syImportScriptSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_SOUTH);
}

/* Walking into the door of Isaac's house, at metatile (21, 56), opens it, and
   he walks in. After the party has left Vale and until it is back from Kalay,
   Dora will not open it: she asks who is there, and does not believe it is her
   Isaac. */
static void scValeEnterIsaacsHouse(void)
{
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0 &&
		gmFlagGet(FLAG_KALAY_REACHED) == 0) {
		fdScriptStart();
		fdScriptSetMessageId(MsgValeYouCantBeIsaac);
		/* "Who are you? You can't be... You're not Isaac, are you?" */
		fdScriptOpenMessage(ACTOR_DORA, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			/* "Isaac... That's a lie!" */
			fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
			/* "He promised not to return until he fulfills his mission.
			   My..." */
			fdScriptShowMessage(ACTOR_DORA, 0);
		} else {
			FieldStateWork->msg += 2;
			fdScriptWait(40);
			fdScriptShowMessage(ACTOR_DORA, 0);
		}
		fdScriptEnd();
	} else {
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(ValeIsaacsDoorOpens, 50, 44);
		fdActorWalkTo(ACTOR_PARTY_LEADER, 340, 888);
		syImportScriptSetExitRequest(VALE_TO_COTTAGE_ROOMS_B);
	}
}

/* Walking into the door of the house of Jenna's grandparents, in the south of
   Vale east of the river, at metatile (20, 71), opens it, and Isaac walks
   in. */
static void scValeEnterJennasGrandparentsHouse(void)
{
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValeGrandparentsDoorOpens, 49, 69);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 326, 1126);
	syImportScriptSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH);
}

/* Walking into the door of the house south of Jenna's grandparents', at
   metatile (23, 78), opens it, and Isaac walks in to the young man who dreams
   of seeing the world.

   The room is the one cottage_rooms_a.c enters from
   COTTAGE_ROOMS_A_FROM_VALE_WEST, after its place on that map. */
static void scValeEnterCottageRoomsAWest(void)
{
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValeWestHouseDoorOpens, 52, 76);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 374, 1238);
	syImportScriptSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_WEST);
}

/* Unused: walking into the door of the house the storm wrecked on the west bank
   would have opened it and taken Isaac into the house of Jenna's grandparents.

   No metatile of Vale's map, by day or on the stormy night, carries its
   metatile behaviour, and that door's own metatile answers
   scValeSearchTheLockedDoor instead. vale_prologue.c keeps the same unused door
   (scValePrologueUnusedLeaveToCottageRoomsAWestByWestBankDoor). */
static void scValeUnusedEnterJennasGrandparentsHouseByWestBankDoor(void)
{
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValeIsaacsDoorOpens, 35, 74);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 102, 1206);
	syImportScriptSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH_WEST_BANK_DOOR);
}

/* At the foot of the steps up Mt. Aleph, before Kraden has planned the visit,
   Jenna steps out of Isaac and tells him "Kraden is waiting for us." He nods,
   she steps back in, and he walks back down off the steps.

   Jenna is put where Isaac stands, as the party is drawn as its leader alone,
   and walks out from there (scWalkIntoLeader takes her back). */
static void scValeJennaSaysKradenIsWaiting(void)
{
	ACTOR *leader;
	ACTOR *jenna;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	jenna = fdActorGet(ACTOR_JENNA);
	fdScriptStart();
	jenna->x.fixed = leader->x.fixed;
	jenna->y.fixed = leader->y.fixed;
	jenna->z.fixed = leader->z.fixed;
	jenna->tgtX = ACTOR_NO_TARGET;
	jenna->tgtY = ACTOR_NO_TARGET;
	jenna->tgtZ = ACTOR_NO_TARGET;
	SET_VELOCITY(jenna, 0, 0, 0);
	jenna->groundHeight = leader->y.fixed;
	sySynch(1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorWalkToAndWait(ACTOR_JENNA, 110, 283);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 2);
	fdScriptSetMessageId(MsgValeKradenIsWaitingForUs);
	if(leader->x.fixed < jenna->x.fixed) {
		/* "Kraden is waiting for us. We should go to his house." */
		fdScriptShowMessageThenWait(
			SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 2);
	} else {
		/* "Kraden is waiting for us. We should go to his house." */
		fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 2);
	}
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(2);
	scWalkIntoLeader(ACTOR_JENNA);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 110, 303);
	fdScriptEnd();
}

/* Walking towards the puppies in the southern way west out of Vale from the
   ledge north of them, metatiles (1, 47) to (5, 47) and (1, 48), sets them
   barking. The view slides south to show them at play below, a message reads
   "There are some puppies playing over there. We should let them be.", and the
   view comes back before Isaac walks away north.

   Once the party has met Saturos and Menardi the puppies are gone, and nothing
   happens here. The view follows a point of its own, moved 2 pixels a tick for
   40 ticks, while Isaac stands still. */
static void scValeWatchThePuppies(void)
{
	MAP_GRID_SCROLL_STATE *grid;
	Sint32				  *savedOrigin;
	ACTOR				  *leader;
	Sint32				   frm;
	FIXED				   cameraPos[3];

	if(gmFlagGet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
		grid = MapGridState[0];
		fdScriptStart();
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		fdScriptWait(2);
		fdScriptSetMessageId(MsgValeRrruffRrrruff);
		/* "Rrruff! Rrrruff!" */
		fdScriptShowMessageThenWait(ACTOR_FIRST_PUPPY, 0, 2);
		/* "Woof! Woof!" */
		fdScriptShowMessageThenWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		SET_VECTOR(
			cameraPos, leader->x.fixed, leader->y.fixed, leader->z.fixed);
		savedOrigin = grid->origin;
		grid->origin = cameraPos;
		for(frm = 0; frm != 40; frm++) {
			cameraPos[2] += toFIXED(2);
			fdScriptWait(1);
			fdMapGridCenterOnFollowedPoint();
		}
		fdScriptWait(60);
		mnFieldMessageShow(MsgValePuppiesPlayingOver, 1);
		frm = 0;
		fdScriptWait(6);
		do {
			cameraPos[2] -= toFIXED(2);
			fdScriptWait(1);
			frm++;
			fdMapGridCenterOnFollowedPoint();
		} while(frm != 40);
		grid->origin = savedOrigin;
		fdScriptWait(60);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 70, 741);
		fdScriptEnd();
	}
}

/* Walking up to the puppies in the southern way west out of Vale from the
   south, at metatiles (3, 53) and (4, 53), sets them barking. A message reads
   "There are some puppies playing over there. We should let them be." and Isaac
   walks back south.

   The view stays on him, as the puppies are already in sight from this side.
   Once the party has met Saturos and Menardi the puppies are gone, and nothing
   happens here. */
static void scValeTurnBackFromThePuppies(void)
{
	Sint32 msg;

	if(gmFlagGet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
		fdScriptStart();
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		msg = MsgValeRrruffRrrruff;
		fdScriptSetMessageId(msg);
		/* "Rrruff! Rrrruff!" */
		fdScriptShowMessageThenWait(ACTOR_FIRST_PUPPY, 0, 2);
		/* "Woof! Woof!" */
		fdScriptShowMessageThenWait(ACTOR_FIRST_PUPPY + 1, 0, 2);
		mnFieldMessageShow(msg + 2, 1);
		fdScriptWait(6);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 69, 870);
		fdScriptEnd();
	}
}

/* Walking north into the foot of the ladder against the front of Isaac's house,
   east of his door, at metatile (24, 55), climbs him up the ladder and over the
   eaves onto the roof.

   The roof's metatiles are copied in from metatiles (49, 53) to (56, 56) of the
   map onto metatiles (20, 50) to (27, 53), so that he can walk on it, and
   FLAG_VALE_ON_ROOF keeps them there after a battle. */
static void scValeClimbLadderOntoRoof(void)
{
	fdScriptStart();
	fdMapGridCopyMetatileAttributes(49, 53, 8, 4, 20, 50);
	scValeClimbUpRoofLadder(
		ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB, ANIM_LEADER_CLIMB_OVER_TOP, 1);
	gmFlagSet(FLAG_VALE_ON_ROOF);
	fdScriptEnd();
}

/* Walking south off the roof of Isaac's house at the top of the ladder,
   metatile (24, 53) once he is on the roof, climbs him down the ladder to the
   ground in front of the house.

   The ground's metatiles come back from metatiles (49, 46) to (56, 49) of the
   map. */
static void scValeClimbDownFromRoof(void)
{
	fdScriptStart();
	scValeClimbDownRoofLadder(ACTOR_PARTY_LEADER,
		ANIM_LEADER_CLIMB_DOWN_FROM_TOP, ANIM_LEADER_CLIMB, 1);
	gmFlagClear(FLAG_VALE_ON_ROOF);
	fdMapGridCopyMetatileAttributes(49, 46, 8, 4, 20, 50);
	fdScriptEnd();
}

/* The first time Isaac takes the path up the Sanctum's west side towards the
   steps up Mt. Aleph, at its north end (metatile (10, 23)), a bearded man in a
   long robe jumps out from the trees by the river and drives him back to the
   Sanctum's door. "You're not planning on sneaking up to Mt. Aleph, are you?"

   From then on the man walks his beat (ValeAlephGuardPatrolRoute) and watches
   for Isaac on the path (scValeCatchSneakingOnThePath and the two after it).
   FLAG_VALE_WARNED_AGAINST_MT_ALEPH keeps this from playing again. */
static void scValeWarnAgainstSneakingUpMtAleph(void)
{
	ACTOR *guard;

	guard = fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH);
	fdScriptStart();
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
	fdActorJump(ACTOR_PARTY_LEADER, 5, 0);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 215, 403);
	guard->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetPos(ACTOR_MAN_GUARDING_MT_ALEPH, 166, 375);
	fdActorFaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTHEAST, 20);
	guard->steeringFlg = (guard->steeringFlg ^ ACTOR_FACES_MOTION);
	fdActorSetSpeedAndAcceleration(ACTOR_MAN_GUARDING_MT_ALEPH, 2.5, 2.5);
	fdActorJump(ACTOR_MAN_GUARDING_MT_ALEPH, 4, 0);
	fdActorWalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 202, 395);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTH_SOUTHEAST, 24);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	fdActorSetSpeedAndAcceleration(ACTOR_MAN_GUARDING_MT_ALEPH, 1.5, 1);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeFirstWarningLeaderPath);
	fdScriptWait(10);
	fdActorShowEmote(
		ACTOR_MAN_GUARDING_MT_ALEPH, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorSetScript(ACTOR_MAN_GUARDING_MT_ALEPH, ValeFirstWarningGuardPath);
	fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 256, 474);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorWaitForScriptEndById(ACTOR_MAN_GUARDING_MT_ALEPH);
	fdActorWalkToAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, 256, 456);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_MAN_GUARDING_MT_ALEPH, FACING_SOUTH, 20);
	fdActorStartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgValeNotSneakingUpMtAleph);
	/* "You're not planning on sneaking up to Mt. Aleph, are you?" Yes: "Mt.
	   Aleph is a sacred peak. No one can climb it without..." No: "Well, I'll
	   overlook it this time... But I've got my eye on..." */
	fdScriptAskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
	fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
		(void (*)(ACTOR *))scValeHaveGuardWatchTheLeader;
	fdActorSetScript(ACTOR_MAN_GUARDING_MT_ALEPH, ValeAlephGuardPatrolRoute);
	gmFlagSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
	fdScriptEnd();
}

/* Walking up the path along the Sanctum's west side, metatiles (11, 23) to (13,
   27), or just south of its foot, metatiles (12, 30) and (13, 30), while the
   man guarding Mt. Aleph is on that path catches Isaac: he runs back to the
   Sanctum's door, the man follows him down, and asks again whether he is
   sneaking up Mt. Aleph.

   While the man is in front of the Sanctum or behind it, Isaac walks on
   unseen. */
static void scValeCatchSneakingOnThePath(void)
{
	ACTOR *guard = fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH);
	if(gmFlagGet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
		if(guard->tmr == GUARD_ON_THE_PATH) {
			scValeAskAboutSneakingToAleph(
				ValeLeaderRunsBackFromThePath, ValeGuardComesDownThePath);
		}
	}
}

/* The man guarding Mt. Aleph catches Isaac sneaking: both start, Isaac winces
   and runs back by `leaderActions`, the man follows him by `guardActions` to
   stand before the Sanctum's door, and asks "You're not planning on sneaking up
   to Mt. Aleph, are you?" before going back to his beat. */
static void scValeAskAboutSneakingToAleph(
	const Sint32 *leaderActions, const Sint32 *guardActions)
{
	const Sint32 *replyActions;
	ACTOR		 *guard;
	ACTOR		 *actor;

	replyActions = guardActions;
	guard = fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH);
	fdScriptStart();
	fdActorStartRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
	fdActorShowEmote(
		ACTOR_MAN_GUARDING_MT_ALEPH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetScript(ACTOR_PARTY_LEADER, leaderActions);
	fdScriptWait(10);
	fdActorShowEmote(
		ACTOR_MAN_GUARDING_MT_ALEPH, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorRunScriptAndWait(ACTOR_MAN_GUARDING_MT_ALEPH, replyActions);
	fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MAN_GUARDING_MT_ALEPH, 2);
	guard->scaleX = toFIXED(1);
	guard->scaleY = toFIXED(1);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	fdScriptSetMessageId(MsgValeNotSneakingUpMtAleph);
	/* "You're not planning on sneaking up to Mt. Aleph, are you?" Yes: "Mt.
	   Aleph is a sacred peak. No one can climb it without..." No: "Well, I'll
	   overlook it this time... But I've got my eye on..." */
	fdScriptAskYesNo(ACTOR_MAN_GUARDING_MT_ALEPH, 0);
	fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
		(void (*)(ACTOR *))scValeHaveGuardWatchTheLeader;
	fdActorSetScript(ACTOR_MAN_GUARDING_MT_ALEPH, ValeAlephGuardPatrolRoute);
	fdScriptEnd();
}

/* Stepping into the corner just west of the Sanctum's front, metatiles (14, 28)
   and (14, 29), while the man guarding Mt. Aleph stands or walks in front of
   the Sanctum catches Isaac: he runs back before the Sanctum's door, and the
   man steps up to him and asks again whether he is sneaking up Mt. Aleph. */
static void scValeCatchSneakingBesideTheSanctum(void)
{
	ACTOR *guard = fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH);
	if(gmFlagGet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
		if(guard->tmr == GUARD_BEFORE_THE_SANCTUM) {
			scValeAskAboutSneakingToAleph(
				ValeLeaderRunsBackBeforeTheDoor, ValeGuardStepsBeforeTheDoor);
		}
	}
}

/* Stepping onto the foot of the path along the Sanctum's west side, metatiles
   (12, 28) to (13, 29), while the man guarding Mt. Aleph is on the path or in
   front of the Sanctum catches Isaac: he runs back before the Sanctum's door,
   and the man comes to him and asks again whether he is sneaking up Mt.
   Aleph. */
static void scValeCatchSneakingAtThePathsFoot(void)
{
	ACTOR *guard = fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH);
	if(gmFlagGet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
		Sint32 stretch;
		stretch = guard->tmr;
		if(stretch == GUARD_ON_THE_PATH) {
			scValeAskAboutSneakingToAleph(
				ValeLeaderRunsBackBeforeTheDoor, ValeGuardComesDownThePath);
		} else if(stretch == GUARD_BEFORE_THE_SANCTUM) {
			scValeAskAboutSneakingToAleph(
				ValeLeaderRunsBackBeforeTheDoor, ValeGuardStepsBeforeTheDoor);
		}
	}
}

/* Unused: another cut of scValeRoofMended, the end of the morning the story
   begins.

   No table names it. It plays the same lines as the script the game uses, `Good
   job, Isaac.` and `I was devastated when Kyle died...` among them, but ticks
   them with its own camera and map grid metatiles, and sets
   FLAG_VALE_ROOF_JUST_MENDED as that script does. */
UNUSED static void scValeUnusedRoofMended(void)
{
	ACTOR *leader;
	ACTOR *actor;
	Uint8  leaderMotionFlg;
	Sint32 i;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	actor = fdCameraGetActor();
	actor->motionFlg = 0;
	fdCameraMoveTo(383, 160, 877, 0);
	fdScriptWait(1);
	fdMapGridCenterOnFollowedPoint();
	fdMapGridCopyMetatileAttributes(49, 41, 7, 3, 20, 50);
	fdMapGridCopyMetatilesTo(2, 102, 84, 41, 2, 1);
	fdMapGridCopyMetatilesTo(1, 102, 83, 41, 1, 1);
	fdMapGridCopyMetatilesTo(0, 103, 82, 42, 1, 1);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	leaderMotionFlg = leader->motionFlg;
	leader->motionFlg = 0;
	fdActorSetPos(ACTOR_PARTY_LEADER, 407, 690);
	fdActorSetPos(ACTOR_DORA, 392, 896);
	fdActorSetPos(ACTOR_GARET, 298, 736);
	fdActorSetPos(ACTOR_JENNA, 298, 760);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	scValePlaceOrHideBoards(ACTOR_FIRST_BOARD, 2, 1);
	FieldStateWork->transition = 0;
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_JENNA, 0.5);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorSetScript(ACTOR_JENNA, ValeJennaWalksToTheHouse);
	fdActorSetScript(ACTOR_GARET, ValeGaretWalksToTheHouse);

	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.3);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
	fdActorSetSprColById(ACTOR_FIRST_BOARD, SPRITE_COLOR_RBG);
	fdActorSetSpeed(ACTOR_FIRST_BOARD, 0.3);
	fdActorMoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_FIRST_BOARD, 402, 828);
	fdScriptWait(80);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
	fdActorSetPos(ACTOR_FIRST_BOARD, 390, 842);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdScriptWait(200);
	fdMapGridCopyMetatilesTo(7, 102, 84, 41, 2, 1);

	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
	fdActorSetSprColById(ACTOR_FIRST_BOARD, SPRITE_COLOR_RBG);
	fdActorSetSpeed(ACTOR_FIRST_BOARD, 0.3);
	fdActorMoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_FIRST_BOARD, 377, 828);
	fdScriptWait(80);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
	fdActorSetPos(ACTOR_FIRST_BOARD, 0, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdScriptWait(200);
	fdMapGridCopyMetatilesTo(6, 102, 83, 41, 1, 1);

	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHWEST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_RBG);
	fdActorSetSprColById(ACTOR_SECOND_BOARD, SPRITE_COLOR_RBG);
	fdActorSetSpeed(ACTOR_SECOND_BOARD, 0.3);
	fdActorMoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_SECOND_BOARD, 360, 842);
	fdScriptWait(80);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_SECOND_BOARD, SPRITE_COLOR_NORMAL);
	fdActorSetPos(ACTOR_SECOND_BOARD, 0, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdScriptWait(200);
	fdMapGridCopyMetatilesTo(5, 103, 82, 42, 1, 1);
	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);

	fdScriptSetMessageId(MsgValeGoodJob);
	fdActorJump(ACTOR_DORA, 2, 20);
	/* "Good job, Isaac." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
	scValeClimbUpRoofLadder(
		ACTOR_DORA, DORA_ANIMATION_CLIMB, DORA_ANIMATION_CLIMB_OVER_TOP, 0);
	fdActorSetSpeed(ACTOR_DORA, 0.3);
	fdActorWalkToAndWait(ACTOR_DORA, 397, 832);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(10);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorWalkToAndWait(ACTOR_DORA, 372, 832);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 40);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(10);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(10);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdScriptOpenMessage(ACTOR_DORA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		FieldStateWork->msg++;
	}
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdScriptSetMessageId(MsgValeWorkingYourselvesBone);
	fdActorWalkToAndWait(ACTOR_DORA, 386, 841);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(20);
	/* "You and Garet have been working yourselves to the bone..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	/* "Do you think things would be any different if you'd had..." */
	fdScriptOpenMessage(ACTOR_DORA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgValeDevastatedWhenKyle);
	/* "I was devastated when Kyle died..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorWalkToAndWait(ACTOR_DORA, 386, 825);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	/* "I felt all alone..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 10);
	/* "It's taken some time, but I'm better now." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorWalkToAndWait(ACTOR_DORA, 372, 832);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(20);
	/* "I still have you, Isaac." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(20);
	/* "You're almost seventeen. You're so much like Kyle was when..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(20);
	/* "I'm looking forward to seeing where life takes you." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);

	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(377, 160, 860, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkTo(ACTOR_GARET, 369, 904);
	fdActorWalkToAndWait(ACTOR_JENNA, 392, 904);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	scValeClimbUpRoofLadder(
		ACTOR_JENNA, ANIM_LEADER_CLIMB, ANIM_LEADER_CLIMB_OVER_TOP, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
	fdActorJump(ACTOR_JENNA, 4, 0);
	fdActorWalkToAndWait(ACTOR_JENNA, 392, 843);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 40);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	scValeClimbUpRoofLadder(
		ACTOR_GARET, ANIM_LEADER_CLIMB, ANIM_LEADER_CLIMB_OVER_TOP, 0);
	fdActorSetSpeed(ACTOR_JENNA, 0.3);
	fdActorSetSpeed(ACTOR_GARET, 0.3);
	fdActorWalkTo(ACTOR_GARET, 392, 843);
	fdActorGet(ACTOR_JENNA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JENNA, 408, 843);
	fdScriptWait(1);
	fdActorGet(ACTOR_JENNA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 30);
	fdActorJump(ACTOR_DORA, 4, 30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 30);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 40);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(100);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 30);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(30);
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(373, 160, 837, 1);
	fdActorWalkToAndWait(ACTOR_DORA, 364, 816);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 30);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 30);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdScriptOpenMessage(ACTOR_DORA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdScriptWait(40);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);

	fdScriptSetMessageId(MsgValeAsStubbornAsYourFather);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	fdScriptWait(30);
	/* "You're just as stubborn as your father!" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorJump(ACTOR_DORA, 4, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_HANDS_UP);
	fdScriptWait(5);
	/* "Ahhh!" "Be careful, ma'am!" */
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_DORA, 14, 2, 24, 2, ACTOR_GARET, 10, 14, 4, 14, 0);
	actor = fdActorGet(ACTOR_DORA);
	actor->spr->flg = 0;
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_DORA, 3);
	fdActorWalkToAndWait(ACTOR_DORA, 364, 815);
	fdScriptWait(4);
	for(i = 0; i != 4; i++) {
		actor->z.fixed += toFIXED(3) / 2;
		actor->scaleY -= toFIXED(1) / 10;
		fdScriptWait(1);
	}
	fdActorSetPos(ACTOR_DORA, 0, 0);
	fdActorSetSpeed(ACTOR_GARET, 3);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 374, 827);
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
	fdActorJump(ACTOR_GARET, 2, 5);
	fdMapGridSetShake(0, 4, 1);
	fdMapGridCopyMetatilesTo(1, 102, 83, 41, 1, 1);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_GARET, 3);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_PEEK);
	actor->scaleY = toFIXED(1) / 2;
	fdActorSetPos(ACTOR_DORA, 364, 811);
	for(i = 0; i != 5; i++) {
		actor->scaleY += toFIXED(1) / 10;
		fdScriptWait(1);
	}
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdCameraSetSpeed(0.3);
	fdCameraMoveTo(372, 160, 859, 1);
	fdActorSetSpeed(ACTOR_DORA, 3);
	fdActorJump(ACTOR_DORA, 6, 0);
	fdActorWalkToAndWait(ACTOR_DORA, 359, 835);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(30);
	actor = fdActorGet(ACTOR_DORA);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 80);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAttachedEffect(
		ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetSpeed(ACTOR_GARET, 4);
	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_GARET, 403, 827);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 20);
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 1);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
	fdActorJump(ACTOR_GARET, 2, 5);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdMapGridCopyMetatilesTo(2, 102, 84, 41, 2, 1);
	fdMapGridSetShake(0, 4, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorSetSpeed(ACTOR_JENNA, 0.3);
	fdActorWalkToAndWait(ACTOR_JENNA, 408, 855);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(10);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 80);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 70);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 30);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	fdScriptWait(30);
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorJump(ACTOR_GARET, 4, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 398, 828);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(60);

	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor = fdActorGet(ACTOR_JENNA);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorWalkTo(
		ACTOR_JENNA, actor->x.part[0].integer + 16, actor->z.part[0].integer);
	fdActorWalkToAndWait(ACTOR_GARET, actor->x.part[0].integer + 16,
		actor->z.part[0].integer - 16);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(40);
	fdActorWalkToAndWait(
		ACTOR_JENNA, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorWalkToAndWait(
		ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdScriptAddPartyMemberPair(GARET, JENNA);
	fdCameraMoveTo(377, 160, 887, 1);
	scValeClimbDownRoofLadder(ACTOR_PARTY_LEADER,
		ANIM_LEADER_CLIMB_DOWN_FROM_TOP, ANIM_LEADER_CLIMB, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorGet(ACTOR_DORA)->steeringFlg |= ACTOR_FACES_MOTION;
	scValeClimbDownRoofLadder(
		ACTOR_DORA, DORA_ANIMATION_CLIMB_OVER_TOP, DORA_ANIMATION_CLIMB, 0);
	fdActorWalkToAndWait(ACTOR_DORA, 373, 887);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdCameraWaitForMove();
	fdScriptWait(100);
	gmFlagSet(FLAG_VALE_ROOF_JUST_MENDED);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	leader->motionFlg = leaderMotionFlg;
	fdScriptEnd();
}

/* Sets Vale up as the party arrives. Arriving from the title, the game's
   opening plays, with Isaac mending the roof; arriving back in front of his
   house after it, the end of that morning plays once.

   Otherwise the setup floats the Nut in its tree until it is caught, stands
   Garet's training stone in its column, sends the puppies away once the party
   has met Saturos and Menardi, starts the man guarding Mt. Aleph on his beat
   once he has warned Isaac, and after a battle puts back the roof's and the
   bridge's metatiles. After the party has left and until it is back from Kalay,
   Dora stands inside Isaac's house shrunk to nothing, so that she can answer
   through the door (scValeEnterIsaacsHouse). */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 set;

	if(MainRoutineStateWork.entrance == VALE_FROM_TITLE) {
		mnWindowColApply(
			MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
		scValeMendingTheRoof();
	} else {
		if(!gmFlagGet(FLAG_VALE_NUT_CAUGHT)) {
			if(!gmFlagGet(FLAG_KALAY_REACHED)) {
				scFloatNut(ACTOR_NUT);
			} else {
				scFloatNut(ACTOR_NUT_AFTER_KALAY);
			}
		}
		/* The hole Garet left in the roof when he jumped onto it, on map layer
		   2 (scValeMendingTheRoof). */
		fdMapGridCopyMetatilesTo(2, 102, 84, 41, 2, 1);
		fdMapGridCopyMetatilesTo(1, 102, 83, 41, 1, 1);
		actor = fdActorGet((gmFlagGet(FLAG_KALAY_REACHED) != 0) + ACTOR_PILLAR);
		fdActorSetSprFlags(actor, 0);
		if(gmFlagGet(FLAG_VALE_PILLAR_IN_WEST_COLUMN)) {
			actor->x.fixed = toFIXED(PILLAR_WEST_X);
		} else if(gmFlagGet(FLAG_VALE_PILLAR_IN_EAST_COLUMN)) {
			actor->x.fixed = toFIXED(PILLAR_EAST_X);
		} else {
			actor->x.fixed = toFIXED(PILLAR_MIDDLE_X);
		}
		actor->z.fixed = toFIXED(PILLAR_Z);
		actor->y.fixed = toFIXED(192);
		scValeMovePillarBelowCliff();
		actor->layer = 3;
		actor->motionFlg = 0;
		sySetTickEvent(scValeSetPillarPriorityByLeaderHeight, EVENT_PASS_TICK);
#if VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY
		if(gmFlagGet(FLAG_KALAY_REACHED)) {
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				/* The metatiles scValeClimbLadderOntoRoof copies as it sets the
				   flag, redrawn on arrival. */
				if(gmFlagGet(FLAG_VALE_ON_ROOF)) {
					fdMapGridCopyMetatileAttributes(49, 53, 8, 4, 20, 50);
				}
				if(gmFlagGet(FLAG_VALE_ON_BRIDGE)) {
					scValeStepOntoBridge();
				}
			}
		} else {
#else
		if(!gmFlagGet(FLAG_KALAY_REACHED)) {
#endif
			if(fdScriptGetFlag(FLAG_VALE_PARTY_LEFT)) {
				actor = fdActorGet(ACTOR_DORA);
				fdActorSetSprFlags(fdActorGet(ACTOR_DORA), 0);
				actor->scaleX = toFIXED(1) / 100;
				actor->scaleY = toFIXED(1) / 100;
			}
			if(gmFlagGet(FLAG_MET_SATUROS_AND_MENARDI)) {
				fdActorSetPos(ACTOR_FIRST_PUPPY, 0, 0);
				fdActorSetPos(ACTOR_FIRST_PUPPY + 1, 0, 0);
				fdActorSetPos(ACTOR_FIRST_PUPPY + 2, 0, 0);
			}
			set = gmFlagGet(FLAG_VALE_PARTY_LEFT);
			if(set == 0) {
				if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
					if(gmFlagGet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH)) {
						fdActorSetPos(ACTOR_MAN_GUARDING_MT_ALEPH, 256, 456);
#if FIX_FUNCTION_SIGNATURES
						fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
							scValeHaveGuardWatchTheLeader;
#else
						fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->update =
							(void *)scValeHaveGuardWatchTheLeader;
#endif
						fdActorSetScript(ACTOR_MAN_GUARDING_MT_ALEPH,
							ValeAlephGuardPatrolRoute);
					}
				} else {
					fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = set;
					gmFlagClear(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
				}
				if(MainRoutineStateWork.entrance != VALE_FROM_TITLE &&
					!gmFlagGet(FLAG_KALAY_REACHED)) {
					sySetTickEvent(
						scValeSynchronizeAlephGuardPause, EVENT_PASS_TICK);
				}
			}
			if(!fdScriptGetFlag(FLAG_VALE_ROOF_MENDED_SCRIPT_PLAYED) &&
				MainRoutineStateWork.entrance == VALE_FROM_ROOF_MENDED) {
				scValeRoofMended();
				gmFlagSet(FLAG_VALE_ROOF_MENDED_SCRIPT_PLAYED);
			}
#if VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				/* The metatiles scValeClimbLadderOntoRoof copies as it sets the
				   flag, redrawn on arrival. */
				if(gmFlagGet(FLAG_VALE_ON_ROOF)) {
					fdMapGridCopyMetatileAttributes(49, 53, 8, 4, 20, 50);
				}
				if(gmFlagGet(FLAG_VALE_ON_BRIDGE)) {
					scValeStepOntoBridge();
				}
			}
			{
				Sint32 pri;

				/* DEAD: a byte shifted down 8 places is 0, so Dora's priority
				   is 2; the ROM keeps the byte's mask and shift. */
				pri = ((Uint32)(Uint8)set >> 8) + 2;
				fdActorSetSprPriority(ACTOR_DORA, pri);
			}
#endif
		}
#if !VALE_DORA_FIXED_PRIORITY_BEFORE_KALAY
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			/* The metatiles scValeClimbLadderOntoRoof copies as it sets the
			   flag, redrawn on arrival. */
			if(gmFlagGet(FLAG_VALE_ON_ROOF)) {
				fdMapGridCopyMetatileAttributes(49, 53, 8, 4, 20, 50);
			}
			if(gmFlagGet(FLAG_VALE_ON_BRIDGE)) {
				scValeStepOntoBridge();
			}
		}
#endif
		fdQueueMusic(SE_AMBIENT_WATER);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
	return 0;
}

/* Back from Kalay, talking to Garet's sister by his training stone, or reading
   her mind, brings Garet out to her. "It's been a while..." She admits she was
   worried about him, and is sorry he is leaving again.

   FLAG_VALE_GARET_MET_SISTER keeps this from playing twice. */
static void scValeGaretMeetsHisSister(void)
{
	ACTOR *actor;
	Sint32 msg;

	fdScriptStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorFaceActor(ACTOR_GARETS_SISTER_AFTER_KALAY, ACTOR_PARTY_LEADER, 20);
	msg = MsgValeAh;
	fdScriptSetMessageId(msg);
	fdActorStartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
	/* "Ah!" */
	fdScriptShowMessageThenWait(ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(398, CAMERA_KEEP, 582, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 420, 608);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_SOUTH_SOUTHEAST, 0);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	fdActorWalkToAndWait(ACTOR_GARET, 402, 608);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
	/* "It's been a while..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
	fdActorFaceDirection(
		ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
	/* "You're back?" */
	fdScriptShowMessage(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 2);
	/* "Have you gone home to see Dad yet?" */
	fdScriptOpenMessage(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		FieldStateWork->msg++;
		fdActorStartRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
	}
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 40);
	fdActorShowEmote(
		ACTOR_GARETS_SISTER_AFTER_KALAY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdScriptSetMessageId(msg + 6);
	/* "...So have I." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(40);
	/* "Sis..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_GARETS_SISTER_AFTER_KALAY, 1);
	fdActorFaceDirection(
		ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_NORTH_NORTHEAST, 20);
	/* "Don't get me wrong. I... I was just worried you might get..." */
	fdScriptShowMessage(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	/* "Yeah, all right... Thanks, Sis." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 120);
	/* "So, are you home for good? Or..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Not just yet..." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 40);
	fdActorSetAnimationAndWait(
		ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_SHAKE_HEAD);
	/* "You're leaving... again?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(40);
	fdActorFaceDirection(
		ACTOR_GARETS_SISTER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST, 20);
	/* "Don't push yourself too hard...when you leave again." */
	fdScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_SISTER_AFTER_KALAY, 0, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARETS_SISTER_AFTER_KALAY, ANIM_NOD);
	scWalkIntoLeader(ACTOR_GARET);
	gmFlagSet(FLAG_VALE_GARET_MET_SISTER);
	fdScriptEnd();
}

/* Back from Kalay, talking to Garet's little brother west of the door of
   Garet's house, or reading his mind, brings Garet out to him. The boy wonders
   that they are back already, and Garet tells him to protect the family while
   he is gone.

   FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY keeps this from playing twice. */
static void scValeGaretAsksToProtectFamily(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdCameraMoveTo(357, CAMERA_KEEP, 738, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 367, 745);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	fdActorWalkToAndWait(ACTOR_GARET, 346, 745);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
	fdScriptSetMessageId(MsgValeHowHaveYouBeen);
	/* "Hey, how have you been?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorRunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
	fdActorShowEmote(ACTOR_GARETS_BROTHER_AFTER_KALAY,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(
		ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirection(
		ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST, 10);
	fdActorFaceDirection(
		ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHEAST, 40);
	/* "Oh, Garet! And Isaac!" */
	fdScriptShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 1);
	fdActorFaceDirection(
		ACTOR_GARETS_BROTHER_AFTER_KALAY, FACING_SOUTH_SOUTHWEST, 10);
	/* "What happened!? You're back already!?" */
	fdScriptShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	/* "Already? What kind of way is that to greet us?" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
	/* "But-But they said you might never come back!" */
	fdScriptShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 40);
	fdActorSetAnimationAndWait(
		ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_SHAKE_HEAD);
	/* "And it still seems like you're not back for good, like..." */
	fdScriptShowMessage(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "Really? Is that what it seems like?" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
	/* "Yeah... You and your friends... You all look like you are..." */
	fdScriptShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	/* "You're smarter than you look." */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARETS_BROTHER_AFTER_KALAY, 2);
	/* "I have to shape up while you're gone, right? If not..." */
	fdScriptShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 20);
	/* "Hey, the family'll be fine, as long as you're here! Am I..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	} else {
		FieldStateWork->msg++;
	}
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 10);
	fdScriptSetMessageId(MsgValeEverProtectFamily);
	/* "WHATEVER YOU DO, you must protect the family while I'm gone...." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorSetAnimationAndWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, ANIM_NOD);
	/* "Don't push yourself too hard, OK?" */
	fdScriptShowMessageThenWait(ACTOR_GARETS_BROTHER_AFTER_KALAY, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	scWalkIntoLeader(ACTOR_GARET);
	gmFlagSet(FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY);
	fdScriptEnd();
}

/* Unused: water bursts up by the river after a tremor. Back from Kalay, a
   tremor shakes Vale and water bursts up three times north of the strongman by
   the river who remembers Felix.

   No table names it. He would have started, sniffled through `I still remember
   when Felix and I used to play here...*Sniff*`, turned to the leader to say
   that Felix must be protecting Jenna, and set area flag
   FLAG_VALE_UNUSED_WATER_BURST_SEEN. His talk and Mind Read rows show the same
   two lines. */
UNUSED static void scValeUnusedWaterBurstsByTheRiver(void)
{
	const Sint32 *actions;

	fdScriptStart();
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER + 1), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER + 2), 0);
	fdActorSetAnimation(ACTOR_FIRST_WATER, 0);
	fdActorSetAnimation(ACTOR_FIRST_WATER + 1, 0);
	fdActorSetAnimation(ACTOR_FIRST_WATER + 2, 0);
	sySynch(20);
	fdMapGridSetShake(2, 2, 1);
	actions = ValeUnusedWaterGrows;
	fdActorSetScript(ACTOR_FIRST_WATER, actions);
	sySynch(10);
	fdActorSetScript(ACTOR_FIRST_WATER + 1, actions);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	sySynch(20);
	fdActorRunScriptAndWait(ACTOR_FIRST_WATER + 2, actions);
	fdActorShowEmote(ACTOR_STRONGMAN_REMEMBERING_FELIX,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorRunRepeatedMotion(ACTOR_STRONGMAN_REMEMBERING_FELIX, 2);
	fdActorFaceDirection(
		ACTOR_STRONGMAN_REMEMBERING_FELIX, FACING_NORTH_NORTHEAST, 10);
	fdScriptSetMessageId(MsgValeIUsedToPlayHere);
	/* "I still remember when Felix and I used to play here...*Sniff*" */
	fdScriptShowMessageThenWait(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0, 40);
	fdActorFaceActor(ACTOR_STRONGMAN_REMEMBERING_FELIX, ACTOR_PARTY_LEADER, 20);
	/* "Felix must be protecting Jenna... I'm sure of it." */
	fdScriptShowMessage(ACTOR_STRONGMAN_REMEMBERING_FELIX, 0);
	fdActorFaceDirection(ACTOR_STRONGMAN_REMEMBERING_FELIX, FACING_WEST, 10);
	gmFlagSet(FLAG_VALE_UNUSED_WATER_BURST_SEEN);
	fdScriptEnd();
}

/* Walks an actor to the foot of the ladder beside Isaac's house and climbs it
   onto the roof, in `approachAnmId` on the rungs and
   `departureAnmId` over the eaves. */
static void scValeClimbUpRoofLadder(Sint32 actorId, Sint32 approachAnmId,
	Sint32 departureAnmId, Sint32 resumeMotion)
{
	ACTOR *actor;
	SPR	  *spr;

	actor = fdActorGet(actorId);
	spr = actor->spr;
	fdActorSetSpeed(actorId, 1);
	fdActorWalkToAndWait(actorId, 392, 886);
	/* The leader, not actorId, is turned north. The player sees no turn: Isaac
	   stays facing south on the roof while Dora climbs, and already faces north
	   when Jenna and Garet do. */
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
	actor->motionFlg = 0;
	spr->flg = 0;
	fdActorSetAnimation(actorId, approachAnmId);
	fdActorSetSpeed(actorId, 0.3);
	fdActorMoveToAndWait(actorId, 392, 875);
	fdScriptWait(10);
	fdActorSetAnimation(actorId, departureAnmId);
	fdActorSetSpeed(actorId, 1);
	fdActorMoveToAndWait(actorId, 392, 859);
	spr->flg = SPRITE_FLAG_SHADOW;
	if(resumeMotion != 0) {
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	}
	fdScriptWait(10);
	fdActorSetAnimation(actorId, ANIM_STAND);
}

/* Climbs an actor down the ladder beside Isaac's house from the roof, and
   restores falling motion if requested. */
static void scValeClimbDownRoofLadder(Sint32 actorId, Sint32 approachAnmId,
	Sint32 departureAnmId, Sint32 resumeMotion)
{
	ACTOR *actor;
	SPR	  *spr;

	actor = fdActorGet(actorId);
	spr = actor->spr;
	fdActorSetSpeed(actorId, 1);
	fdActorWalkToAndWait(actorId, 392, 859);
	fdActorFaceDirection(actorId, FACING_NORTH, 10);
	actor->motionFlg = 0;
	spr->flg = 0;
	fdActorSetAnimation(actorId, approachAnmId);
	fdActorSetSpeed(actorId, 1);
	fdActorMoveToAndWait(actorId, 392, 875);
	fdScriptWait(10);
	fdActorSetSpeed(actorId, 0.3);
	fdActorSetAnimation(actorId, departureAnmId);
	fdActorMoveToAndWait(actorId, 392, 890);
	actor->velY = toFIXED(2);
	spr->flg = SPRITE_FLAG_SHADOW;
	if(resumeMotion != 0) {
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	}
	fdActorSetAnimation(actorId, ANIM_STAND);
}

/* Lays `cnt` of the roof's boards, from actor `first` on, by the hole in the
   roof, still and without shadows; with any `mode` but 0 it puts them out of
   sight instead. */
static void scValePlaceOrHideBoards(Sint32 first, Uint32 cnt, Sint32 mode)
{
	Sint32 actorId = first;
	Uint32 i;

	if(mode == 0) {
		for(i = 0; i < cnt; i++) {
			ACTOR *actor = fdActorGet(actorId);

			actor->motionFlg = 0;
			fdActorSetSprFlags(actor, 0);
			actor->x.fixed = toFIXED(390);
			actor->y.fixed = toFIXED(160);
			actor->z.fixed = toFIXED(842);
			actorId++;
		}
		return;
	}

	for(i = 0; i < cnt; i++) {
		fdActorSetPos(actorId, 0, 0);
		actorId++;
	}
}

/* The game's opening, three years after the storm: Isaac is mending the roof of
   his house while Dora calls up to him, "Isaac! Haven't you finished mending
   the roof yet?" Jenna comes up from the south past them, finds Garet training
   his Psynergy on his training stone, and the two talk before setting off to
   fetch Isaac.

   It plays as Vale is set up from the title (VALE_FROM_TITLE), and ends by
   taking the party back in front of Isaac's house (VALE_TO_ROOF_MENDED), where
   scValeRoofMended goes on. */
static void scValeMendingTheRoof(void)
{
	Uint32 i;
	ACTOR *repairActor = NULL;
	ACTOR *leader;
	ACTOR *moveHand;
	Sint32 msg;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	moveHand = fdActorGet(ACTOR_OPENING_MOVE_HAND);

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	(fdCameraGetActor())->motionFlg = 0;
	sySynch(1);
	fdMapGridCopyMetatileAttributes(49, 53, 8, 4, 20, 50);
	/* The hole in the roof, three metatiles of its picture, which the boards
	   mend one by one at the roof's end from the whole pictures at (5, 102) to
	   (7, 102). The roof is on map layer 2, the roofs over the actors, whose
	   metatiles begin at the map's column 60: its metatiles (22, 41) to (24,
	   42) are the map's (82, 41) to (84, 42). */
	fdMapGridCopyMetatilesTo(2, 102, 84, 41, 2, 1);
	fdMapGridCopyMetatilesTo(1, 102, 83, 41, 1, 1);
	fdMapGridCopyMetatilesTo(0, 103, 82, 42, 1, 1);
	repairActor = fdActorGetById(ACTOR_OPENING_FIRST_BOARD);
	repairActor->motionFlg = 0;
	repairActor->x.fixed = toFIXED(388);
	repairActor->y.fixed = toFIXED(160);
	repairActor->z.fixed = toFIXED(840);
	fdActorSetSprFlags(repairActor, 0);
	repairActor = fdActorGetById(ACTOR_OPENING_SECOND_BOARD);
	repairActor->motionFlg = 0;
	repairActor->y.fixed = toFIXED(160);
	repairActor->z.fixed = toFIXED(844);
	repairActor->x.fixed = toFIXED(388);
	fdActorSetSprFlags(repairActor, 0);
	repairActor = fdActorGetById(ACTOR_OPENING_THIRD_BOARD);
	repairActor->motionFlg = 0;
	repairActor->z.fixed = toFIXED(848);
	repairActor->y.fixed = toFIXED(160);
	repairActor->x.fixed = toFIXED(388);
	fdActorSetSprFlags(repairActor, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_OPENING_FIRST_BOARD), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_OPENING_SECOND_BOARD), 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdActorSystemSuspendProjectionAndShowSprs();
	msg = MsgValeRepairCaption;
	mnFieldMessageShowCentered(msg, 0, 0);
	fdActorSystemResumeProjectionAndHideLayers();

	fdCameraMoveTo(339, 160, 1173, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdCameraSetSpeed(0.33);
	fdCameraMoveTo(296, 160, 921, 1);
	fdActorSetPos(ACTOR_JENNA, 409, 1134);
	sySynch(1);
	fdActorSetSpeed(ACTOR_JENNA, 0.7);
	fdActorWalkTo(ACTOR_JENNA, 420, 1068);
	gmPlayMusic();
	FieldStateWork->transitionFrm = 60;
	fdScriptStartScreenTransition();
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorWalkToAndWait(ACTOR_JENNA, 341, 1064);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkToAndWait(ACTOR_JENNA, 359, 1033);
	fdActorSetSpeed(ACTOR_OPENING_WOMAN, 0.5);
	fdActorWalkTo(ACTOR_OPENING_WOMAN, 318, 947);
	fdActorSetAnimation(ACTOR_OPENING_WOMAN, ANIM_WALK);
	fdActorWalkToAndWait(ACTOR_JENNA, 412, 1033);
	fdActorWalkToAndWait(ACTOR_JENNA, 412, 1019);
	fdActorWalkToAndWait(ACTOR_JENNA, 374, 1008);
	fdActorWalkToAndWait(ACTOR_JENNA, 347, 955);
	fdActorWalkToAndWait(ACTOR_OPENING_WOMAN, 318, 947);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_OPENING_WOMAN, 40);
	fdActorRunRepeatedMotion(ACTOR_OPENING_WOMAN, 2);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdActorWalkTo(ACTOR_OPENING_WOMAN, 379, 1017);
	fdCameraSetSpeed(0.5);
	fdActorWalkToAndWait(ACTOR_JENNA, 333, 920);
	fdActorWalkToAndWait(ACTOR_JENNA, 299, 924);
	fdCameraWaitForMove();
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST_NORTHEAST, 30);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(387, 160, 866, 1);
	fdCameraWaitForMove();
	fdActorJump(ACTOR_OPENING_DORA, 2, 20);
	fdScriptSetMessageId(msg + 1);
	/* "Isaac! Haven't you finished mending the roof yet?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0, 10);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	fdActorFaceEachOther(ACTOR_OPENING_DORA, ACTOR_PARTY_LEADER, 40);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_OPENING_DORA, 2);
	fdScriptWait(40);
	/* "You're almost done? OK! Keep at it!" */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_OPENING_DORA, 0, 40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdCameraFollowActor(ACTOR_JENNA, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 10);
	fdActorWalkToAndWait(ACTOR_JENNA, 312, 759);
	fdActorWalkToAndWait(ACTOR_JENNA, 361, 760);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 40);
	/* "Hyaa!" */
	fdScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_JENNA, 4, 40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 30);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(40);
	/* "Hee hee... Still training, Garet?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(396, CAMERA_KEEP, 588, 1);
	fdActorWalkToAndWait(ACTOR_JENNA, 456, 739);
	fdCameraWaitForMove();
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);

	/* Garet trains his Psynergy: the hand of Move pushes the pillar, shedding
	   sparkles. */
	scStartScriptPsynergy();
	fdActorSetAnimation(ACTOR_GARET, ANIM_CAST_PSYNERGY);
	/* "Hyaaaa!" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
	sySndPlay(SE_PSYNERGY_SPARKLE);
	for(i = 0; i < 60; i++) {
		scValeFlashActorAndSpawnEffects(fdActorGet(ACTOR_GARET));
		sySynch(1);
	}
	fdActorSetSprPriority(ACTOR_GARET, 1);
	sySetTickEvent(scValeUpdateGaretPaletteAndEffects, EVENT_PASS_TICK);
	sySetTickEvent(scValeCycleOpeningPillarPalette, EVENT_PASS_TICK);
	fdActorSetSprFlags(fdActorGet(ACTOR_OPENING_MOVE_HAND), 0);
	moveHand->motionFlg = 0;
	moveHand->x.fixed = toFIXED(428);
	moveHand->y.fixed = toFIXED(208);
	moveHand->z.fixed = toFIXED(584);
	moveHand->facing = FACING_WEST;
	moveHand->update = scValeTrailSparkles;
	fdScriptWait(4);
	fdActorSetSpeedAndAcceleration(ACTOR_OPENING_MOVE_HAND, 2, 2);
	syFieldActorSetMoveTarget(moveHand, 408, 208, 584);
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_OPENING_PILLAR, 0.15);
	fdActorSetSpeed(ACTOR_OPENING_MOVE_HAND, 0.15);
	/* DEAD: the pillar is looked up and dropped; the ROM keeps the call. */
	fdActorGet(ACTOR_OPENING_PILLAR);
	syFieldActorSetMoveTarget(moveHand, 392, 208, 584);
	fdActorMoveToAndWait(ACTOR_OPENING_PILLAR, PILLAR_MIDDLE_X, PILLAR_Z);
	fdScriptWait(20);
	/* "You sure do put your heart into it, Garet!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
	moveHand->update = NULL;
	fdActorSetSprPriority(ACTOR_GARET, 2);
	fdActorGet(ACTOR_GARET)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	syCloseEvent(scValeUpdateGaretPaletteAndEffects);
	syCloseEvent(scValeCycleOpeningPillarPalette);
	sySynch(1);
	fdActorSetSprColById(ACTOR_GARET, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_OPENING_PILLAR, SPRITE_COLOR_NORMAL);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	scValeVanishMoveHand(moveHand);
	scEndScriptPsynergy();

	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_JENNA, 424, 624);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_JENNA, 60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(10);
	/* "Oh, I'm just finishing up." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	/* "That's good. It's time to go, and I was about to leave you behind." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	/* "Say, what exactly happened to you two?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	fdScriptWait(10);
	/* "The accident three years ago... Is that why you've been studying
	   Psynergy?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	/* "I appreciate your concern, but...my family's never coming back, and
	   neither is Isaac's father" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_JENNA, 388, 604);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	/* "I just want to forget that day. I wish everyone would..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 40);
	/* Garet: "That awful day... We went to get help, but they found us
	   unconscious..." and at once Jenna: "Well, let's get Isaac and go to
	   Kraden's..." */
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_GARET, 1, 2, 25, 2, ACTOR_JENNA, 10, 14, 4, 14, 0);
	fdScriptWait(40);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What, Garet?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Huh? You were about to say something, weren't you?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	/* "Don't be weird, Garet!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 40);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_GARET, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_JENNA, 380, 620);
	fdScriptWait(10);
	/* "Whatever..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(10);
	/* "Jenna, are you mad at me?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorWalkToAndWait(ACTOR_GARET, 412, 604);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	fdScriptWait(10);
	/* "I don't want to talk about it." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorJump(ACTOR_JENNA, 4, 30);
	/* "Come on!!!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	/* "Huh?" */
	fdScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_GARET, 0, 20);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	/* "Isaac's waiting for us." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkTo(ACTOR_JENNA, 428, 628);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_JENNA);
	/* "But, Jenna...!" */
	fdScriptShowMessage(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorWalkToAndWait(ACTOR_JENNA, 428, 628);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	/* "I'll leave you behind if you're too slow." */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 20);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "Jenna..." */
	fdScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_JENNA, 0.5);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorWalkTo(ACTOR_JENNA, 450, 750);
	fdActorWalkTo(ACTOR_GARET, 450, 750);
	fdScriptWait(60);
	FieldStateWork->transitionFrm = 60;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(VALE_TO_ROOF_MENDED);
	fdScriptEnd();
}

/* The end of the opening, on the roof of Isaac's house: Isaac lifts the last
   boards into place with his Psynergy, and Dora climbs the ladder to see. "Good
   job, Isaac. You patched it nicely." She remembers Kyle. Jenna and Garet climb
   up too and tell her they are going up Mt. Aleph with Kraden, and at last they
   all climb back down.

   It plays once, as Vale is set up from VALE_FROM_ROOF_MENDED
   (FLAG_VALE_ROOF_MENDED_SCRIPT_PLAYED), and sets FLAG_VALE_ROOF_JUST_MENDED
   for the rest of the visit. */
static void scValeRoofMended(void)
{
	ACTOR *leader;
	ACTOR *actor;
	Uint32 i;
	ACTOR *dora;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdMapGridCopyMetatileAttributes(49, 53, 8, 4, 20, 50);
	fdMapGridCopyMetatilesTo(2, 102, 84, 41, 2, 1);
	fdMapGridCopyMetatilesTo(1, 102, 83, 41, 1, 1);
	fdMapGridCopyMetatilesTo(0, 103, 82, 42, 1, 1);
	fdActorSetPos(ACTOR_DORA, 392, 896);
	dora = fdActorGet(ACTOR_DORA);
	/* DEAD: Dora is turned east and at once north; the ROM keeps both
	   stores. */
	dora->facing = FACING_EAST;
	dora->facing = FACING_NORTH;
	fdActorSetPos(ACTOR_GARET, 298, 736);
	fdActorGet(ACTOR_GARET)->facing = FACING_SOUTH;
	fdActorSetPos(ACTOR_JENNA, 298, 760);
	fdActorGet(ACTOR_JENNA)->facing = FACING_SOUTH;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	actor = fdActorGet(ACTOR_FIRST_BOARD);
	actor->motionFlg = 0;
	actor->x.fixed = toFIXED(388);
	actor->y.fixed = toFIXED(160);
	actor->z.fixed = toFIXED(840);
	fdActorSetSprFlags(actor, 0);
	actor = fdActorGet(ACTOR_SECOND_BOARD);
	actor->motionFlg = 0;
	actor->x.fixed = toFIXED(388);
	actor->y.fixed = toFIXED(160);
	actor->z.fixed = toFIXED(844);
	fdActorSetSprFlags(actor, 0);
	actor = fdActorGet(ACTOR_THIRD_BOARD);
	actor->motionFlg = 0;
	actor->x.fixed = toFIXED(388);
	actor->y.fixed = toFIXED(160);
	actor->z.fixed = toFIXED(848);
	fdActorSetSprFlags(actor, 0);
	fdCameraGetActor()->motionFlg = 0;
	sySynch(1);
	fdCameraMoveTo(383, 160, 877, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdActorSetSpeedFX(ACTOR_JENNA, toFIXED(1) / 2, FACING_SOUTH);
	fdActorSetSpeedFX(ACTOR_GARET, toFIXED(1) / 2, FACING_SOUTH);
	fdActorSetScript(ACTOR_JENNA, ValeJennaWalksToTheHouse);
	fdActorSetScript(ACTOR_GARET, ValeGaretWalksToTheHouse);
	fdScriptWait(40);
	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.3);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
	scStartScriptPsynergy();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
	sySetTickEvent(scValePlayPsynergySparkleSnd, EVENT_PASS_TICK);
	for(i = 0; i < 40; i++) {
		scValeFlashActorAndSpawnEffects(leader);
		sySynch(1);
	}
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 1);

	sySetTickEvent(scValeUpdateLeaderPaletteAndEffects, EVENT_PASS_TICK);
	sySetTickEvent(scValeCycleFirstBoardPalette, EVENT_PASS_TICK);
	fdActorSetSpeed(ACTOR_FIRST_BOARD, 0.2);
	fdActorMoveToAndWait(ACTOR_FIRST_BOARD, 390, 832);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_FIRST_BOARD, 400, 826);
	fdScriptWait(20);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	syCloseEvent(scValePlayPsynergySparkleSnd);
	syCloseEvent(scValeUpdateLeaderPaletteAndEffects);
	syCloseEvent(scValeCycleFirstBoardPalette);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_FIRST_BOARD, SPRITE_COLOR_NORMAL);
	fdActorSetPos(ACTOR_FIRST_BOARD, 0, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdScriptWait(120);
	fdMapGridCopyMetatilesTo(7, 102, 84, 41, 2, 1);
	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
	sySetTickEvent(scValePlayPsynergySparkleSnd, EVENT_PASS_TICK);
	for(i = 0; i < 40; i++) {
		scValeFlashActorAndSpawnEffects(leader);
		sySynch(1);
	}
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 1);
	sySetTickEvent(scValeUpdateLeaderPaletteAndEffects, EVENT_PASS_TICK);
	sySetTickEvent(scValeCycleSecondBoardPalette, EVENT_PASS_TICK);
	fdActorSetSpeed(ACTOR_SECOND_BOARD, 0.2);
	fdActorMoveToAndWait(ACTOR_SECOND_BOARD, 390, 832);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_SECOND_BOARD, 377, 828);
	fdScriptWait(20);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	syCloseEvent(scValePlayPsynergySparkleSnd);
	syCloseEvent(scValeUpdateLeaderPaletteAndEffects);
	syCloseEvent(scValeCycleSecondBoardPalette);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_SECOND_BOARD, SPRITE_COLOR_NORMAL);
	fdActorSetPos(ACTOR_SECOND_BOARD, 0, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdScriptWait(120);
	fdMapGridCopyMetatilesTo(6, 102, 83, 41, 1, 1);
	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHWEST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_CAST_PSYNERGY);
	sySetTickEvent(scValePlayPsynergySparkleSnd, EVENT_PASS_TICK);
	for(i = 0; i < 40; i++) {
		scValeFlashActorAndSpawnEffects(leader);
		sySynch(1);
	}
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 1);
	sySetTickEvent(scValeUpdateLeaderPaletteAndEffects, EVENT_PASS_TICK);
	sySetTickEvent(scValeCycleThirdBoardPalette, EVENT_PASS_TICK);
	fdActorSetSpeed(ACTOR_THIRD_BOARD, 0.2);
	fdActorMoveToAndWait(ACTOR_THIRD_BOARD, 390, 832);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_THIRD_BOARD, 360, 837);
	fdScriptWait(20);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	syCloseEvent(scValePlayPsynergySparkleSnd);
	syCloseEvent(scValeUpdateLeaderPaletteAndEffects);
	syCloseEvent(scValeCycleThirdBoardPalette);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_THIRD_BOARD, SPRITE_COLOR_NORMAL);
	fdActorSetPos(ACTOR_THIRD_BOARD, 0, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeLeaderHammers);
	fdScriptWait(120);
	scEndScriptPsynergy();
	fdMapGridCopyMetatilesTo(5, 103, 82, 42, 1, 1);
	fdActorSetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	fdActorJump(ACTOR_DORA, 2, 20);
	fdScriptSetMessageId(MsgValeGoodJob);
	/* "Good job, Isaac." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST_SOUTHEAST, 10);
	scValeClimbUpRoofLadder(
		ACTOR_DORA, DORA_ANIMATION_CLIMB, DORA_ANIMATION_CLIMB_OVER_TOP, 0);
	fdActorSetSpeed(ACTOR_DORA, 0.3);
	fdActorWalkToAndWait(ACTOR_DORA, 397, 832);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	/* "You patched it nicely." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorWalkToAndWait(ACTOR_DORA, 372, 832);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 40);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	/* "All right. You're all done." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	/* "You're very reliable, Isaac...just like your father was." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	/* "What is it, Isaac? Do you still think about the accident?" */
	fdScriptOpenMessage(ACTOR_DORA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
		FieldStateWork->msg++;
	} else {
		fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	}
	/* Yes: "I thought so... Isaac..." No: "I don't believe you..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdScriptSetMessageId(MsgValeWorkingYourselvesBone);
	fdActorWalkToAndWait(ACTOR_DORA, 386, 841);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	/* "You and Garet have been working yourselves to the bone..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	/* "Do you think things would be any different if you'd had..." */
	fdScriptOpenMessage(ACTOR_DORA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		FieldStateWork->msg++;
	}
	/* Yes: "That's what I thought." No: "Hmmm... I wonder..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptSetMessageId(MsgValeDevastatedWhenKyle);
	/* "I was devastated when Kyle died..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorWalkToAndWait(ACTOR_DORA, 386, 825);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	/* "I felt all alone..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 10);
	/* "It's taken some time, but I'm better now." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorWalkToAndWait(ACTOR_DORA, 372, 832);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(20);
	/* "I still have you, Isaac." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	/* "You're almost seventeen. You're so much like Kyle was when..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	/* "I'm looking forward to seeing where life takes you." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(377, 160, 860, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkTo(ACTOR_GARET, 369, 904);
	fdActorWalkToAndWait(ACTOR_JENNA, 392, 904);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	scValeClimbUpRoofLadder(
		ACTOR_JENNA, ANIM_LEADER_CLIMB, ANIM_LEADER_CLIMB_OVER_TOP, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	/* "MA'AAAAM!!!" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST_SOUTHEAST, 20);
	fdActorJump(ACTOR_JENNA, 4, 0);
	fdActorWalkToAndWait(ACTOR_JENNA, 392, 843);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	/* "Hello, Jenna." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	/* "Good morning." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	/* "Yes, yes... Good morning." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	scValeClimbUpRoofLadder(
		ACTOR_GARET, ANIM_LEADER_CLIMB, ANIM_LEADER_CLIMB_OVER_TOP, 0);
	fdActorSetSpeed(ACTOR_JENNA, 0.3);
	fdActorSetSpeed(ACTOR_GARET, 0.3);
	fdActorWalkTo(ACTOR_GARET, 392, 843);
	fdActorGet(ACTOR_JENNA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JENNA, 408, 843);
	fdScriptWait(1);
	fdActorGet(ACTOR_JENNA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 30);
	fdActorJump(ACTOR_DORA, 4, 30);
	/* "Well, if it isn't Garet, too!" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
	/* "Where do you all plan to go today?" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 30);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	/* "We're going to Mt. Aleph with Kraden." */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 30);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 20);
	/* "Mountain climbing with Kraden, eh?" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(80);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "Kids and their games..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	/* "No! It's part of our studies..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(40);
	/* "Ah, yes... Alchemy." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	/* "Alchemy is the foundation of all Psynergy..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	/* "And you all know Mt. Aleph is the greatest source of Alchemy." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	/* "They say Kraden is the best teacher around." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(373, 160, 837, 1);
	fdActorWalkToAndWait(ACTOR_DORA, 364, 816);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 10);
	/* "Alchemy... I wonder what use it all is?" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_GARET, 30);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHWEST, 20);
	/* "Your mother would rather see you grow up to be a happy, ordinary
	   man." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	/* "But I suppose you still want to go..." */
	fdScriptOpenMessage(ACTOR_DORA, 0);

	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		FieldStateWork->msg++;
	}
	/* Yes: "Well, if that won't persuade you, I guess nothing will!" No: "It's
	   nice of you to humor me, but I know you don't mean it." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdScriptSetMessageId(MsgValeAsStubbornAsYourFather);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	/* "You're just as stubborn as your father!" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorJump(ACTOR_DORA, 4, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_HANDS_UP);
	fdScriptWait(5);
	/* "Ahhh!" "Be careful, ma'am!" */
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_DORA, 14, 2, 24, 2, ACTOR_GARET, 10, 14, 4, 14, 0);
	sySndPlay(SE_RATTLE);
	actor = fdActorGet(ACTOR_DORA);
	actor->spr->flg = 0;
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_DORA, 3);
	fdActorWalkToAndWait(ACTOR_DORA, 364, 815);
	fdScriptWait(4);
	for(i = 0; i != 4; i++) {
		actor->z.fixed += toFIXED(3) / 2;
		actor->scaleY -= toFIXED(1) / 10;
		fdScriptWait(1);
	}
	fdActorSetPos(ACTOR_DORA, 0, 0);
	fdActorSetSpeed(ACTOR_GARET, 3);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 374, VALE_GARET_REUNION_Z);
#if VALE_ROOF_MENDED_GARET_LANDS_FURTHER_SOUTH
	fdActorSetPos(ACTOR_GARET, 374, VALE_GARET_REUNION_Z);
#endif
	/* "Garet, don't!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
	fdActorJump(ACTOR_GARET, 2, 5);
	sySndPlay(SE_CRACK);
	fdMapGridSetShake(0, 4, 1);
	fdMapGridCopyMetatilesTo(1, 102, 83, 41, 1, 1);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_GARET, 3);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_PEEK);
	actor->scaleY = toFIXED(1) / 2;
	fdActorSetPos(ACTOR_DORA, 364, 811);
	for(i = 0; i != 5; i++) {
		actor->scaleY += toFIXED(1) / 10;
		fdScriptWait(1);
	}
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdScriptWait(20);
	/* "Oh! That was exciting!" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdCameraSetSpeed(0.3);
	fdCameraMoveTo(372, 160, 859, 1);
	fdActorSetSpeed(ACTOR_DORA, 3);
	fdActorJump(ACTOR_DORA, 6, 0);
	fdActorWalkToAndWait(ACTOR_DORA, 359, 835);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 20);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	/* "I'm sorry I scared you like that." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 80);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	/* "Well!" */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAttachedEffect(
		ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 40);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	/* "I... I'm sorry, ma'am. I'll get out of your way right away!" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetSpeed(ACTOR_GARET, 4);
	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_GARET, 403, VALE_GARET_REUNION_Z);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 20);
	/* "Wait! Not there!" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 1);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	/* "Huh? What's wrong with—" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_GARET_STARTLED);
	fdActorJump(ACTOR_GARET, 2, 5);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdMapGridCopyMetatilesTo(2, 102, 84, 41, 2, 1);
	sySndPlay(SE_CRACK);
	fdMapGridSetShake(0, 4, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorSetSpeed(ACTOR_JENNA, 0.3);
	fdActorWalkToAndWait(ACTOR_JENNA, 408, 855);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(10);
	/* "Oh, never mind..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 80);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "But you'd better get going..." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(30);
	/* "This old house will fall apart if you stay much longer." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
	/* "I'll get this fixed up. You don't have to hang around here." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(10);
	/* "Thank you, ma'am." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 30);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Please, call me Dora." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	/* "We'll get going right away!" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 3);
	fdScriptWait(30);
	/* "Wait, Garet... Just watch your step." */
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorJump(ACTOR_GARET, 4, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 398, 828);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(60);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptWait(60);
	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor = fdActorGet(ACTOR_JENNA);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorWalkTo(
		ACTOR_JENNA, actor->x.part[0].integer + 16, actor->z.part[0].integer);
	fdActorWalkToAndWait(ACTOR_GARET, actor->x.part[0].integer + 16,
		actor->z.part[0].integer - 16);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(40);
	fdActorWalkToAndWait(
		ACTOR_JENNA, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorWalkToAndWait(
		ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdScriptAddPartyMemberPair(GARET, JENNA);
	fdCameraMoveTo(377, 160, 887, 1);
	scValeClimbDownRoofLadder(ACTOR_PARTY_LEADER,
		ANIM_LEADER_CLIMB_DOWN_FROM_TOP, ANIM_LEADER_CLIMB, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorGet(ACTOR_DORA)->steeringFlg |= ACTOR_FACES_MOTION;
	scValeClimbDownRoofLadder(
		ACTOR_DORA, DORA_ANIMATION_CLIMB_OVER_TOP, DORA_ANIMATION_CLIMB, 0);
	fdActorWalkToAndWait(ACTOR_DORA, 373, 887);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdCameraWaitForMove();
	fdScriptWait(100);
	fdMapGridCopyMetatileAttributes(49, 46, 8, 4, 20, 50);
	gmFlagSet(FLAG_VALE_ROOF_JUST_MENDED);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
#if VALE_ROOF_MENDED_LEADER_GROUNDED
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	leader->y.fixed = toFIXED(160);
	leader->tgtY = ACTOR_NO_TARGET;
	leader->velY = 0;
#else
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#endif
	fdScriptEnd();
}

#include "shared/script_psynergy.c"

/* Garet using his Psynergy on his training stone in the opening: his colours
   shift and sparks fly about him (scValeCycleActorPaletteAndSpawnEffects). */
static void scValeUpdateGaretPaletteAndEffects(void)
{
	scValeCycleActorPaletteAndSpawnEffects(fdActorGet(ACTOR_GARET));
}

/* Isaac using his Psynergy on the roof's boards at the end of the opening: his
   colours shift and sparks fly about him. */
static void scValeUpdateLeaderPaletteAndEffects(void)
{
	scValeCycleActorPaletteAndSpawnEffects(fdActorGet(ACTOR_PARTY_LEADER));
}

/* Garet's training stone shifting colours while his Psynergy moves it in the
   opening. */
static void scValeCycleOpeningPillarPalette(void)
{
	scValeCycleActorPalette(fdActorGet(ACTOR_OPENING_PILLAR));
}

/* The first board shifting colours while Isaac's Psynergy lifts it onto the
   roof. */
static void scValeCycleFirstBoardPalette(void)
{
	scValeCycleActorPalette(fdActorGet(ACTOR_FIRST_BOARD));
}

/* The second board shifting colours while Isaac's Psynergy lifts it. */
static void scValeCycleSecondBoardPalette(void)
{
	scValeCycleActorPalette(fdActorGet(ACTOR_SECOND_BOARD));
}

/* The third board shifting colours while Isaac's Psynergy lifts it. */
static void scValeCycleThirdBoardPalette(void)
{
	scValeCycleActorPalette(fdActorGet(ACTOR_THIRD_BOARD));
}

/* The distance in whole pixels between two positions of 16.16 fixed-point x, y
   and z. The same function as scPosDistance, which this scene also holds
   through shared/pushed_actor.c, under another name. */
static Sint32 scValeDistanceInPixels(FIXED *first, FIXED *second)
{
	Sint32 dx = FIXEDtoINT(*first++ - *second++);
	Sint32 dy = FIXEDtoINT(*first++ - *second++);
	Sint32 dz = FIXEDtoINT(*first - *second);

	return sySquartIWRAM(dx * dx + dy * dy + dz * dz);
}

/* Whether the man guarding Mt. Aleph sees the target.

   He sees it within range pixels and in the sixteenth of a turn he faces or the
   one either side of it, or always when force is set.

   Returns 1 when he is stopped. */
static Sint32 scValeTestFacing(
	ACTOR *actor, ACTOR *tgt, Sint32 range, Sint32 force)
{
	Sint32 result;
	Uint32 ang;
	Uint32 left;
	Uint32 right;
	Uint32 dir;

	result = 0;
	if(actor->paused == 1) {
		if(actor->sceneCnt == 0) {
			fdActorSelectAnimation(actor, ANIM_STAND);
			return 1;
		}
	}
	if(scValeDistanceInPixels(&tgt->x.fixed, &actor->x.fixed) < range ||
		force != 0) {
		ang = (Uint16)syAtan(
			tgt->z.fixed - actor->z.fixed, tgt->x.fixed - actor->x.fixed);
		left = FACING_SIXTEENTH(ang - FACING_STEP);
		right = FACING_SIXTEENTH(ang + FACING_STEP);
		ang = FACING_SIXTEENTH(ang);
		dir = FACING_SIXTEENTH(actor->facing);
		if(ang == dir || right == dir || left == dir || force != 0) {
			actor->paused = 1;
			fdActorSelectAnimation(actor, ANIM_STAND);
			result = 1;
			actor->sceneCnt = result;
		} else {
			actor->paused = 0;
			fdActorSelectAnimation(actor, ANIM_WALK);
			actor->sceneCnt = 0;
		}
	} else {
		actor->paused = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
		actor->sceneCnt = 0;
	}
	return result;
}

/* The update function of the man guarding Mt. Aleph. While he walks, he stops
   when the leader comes within 18 pixels in front of him (scValeTestFacing). */
static UNREAD_RESULT scValeHaveGuardWatchTheLeader(ACTOR *guard)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(guard->tgtX == ACTOR_NO_TARGET && guard->tgtZ == ACTOR_NO_TARGET)
		RETURN_UNREAD(0);
	scValeTestFacing(guard, leader, 18, 0);
	RETURN_UNREAD(0);
}

/* The Move hand's update function in the opening. Each tick it sheds a sparkle
   (SPRITE_SPARKLE) just below and around it, with SE_ITEM_BREAK. */
static void scValeTrailSparkles(ACTOR *src)
{
	Sint32 spawnPos[3];
	Sint32 randomDistance;
	ACTOR *particle;
	SET_VECTOR(spawnPos, src->x.fixed,
		src->y.fixed - (syRandomNext() << 4) - toFIXED(8), src->z.fixed);
	randomDistance = syRandomNext();
	syVectorAddPolarOffset(randomDistance * 48, syRandomNext(), spawnPos);
	particle = syFieldActorCreate(
		SPRITE_SPARKLE, spawnPos[0], spawnPos[1], spawnPos[2]);
	if(particle != NULL) {
		particle->motionFlg = ACTOR_FALLS;
		particle->gravity = toFIXED(1) / 10;
		particle->wait = 12;
		fdActorSetSprFlags(particle, 0);
		fdActorSelectAnimation(particle, 0);
		fdActorStartScript(particle, ValeSparkleScript);
		particle->spr->image.pri = 1;
	}
	sySndPlay(SE_ITEM_BREAK);
}

/* The Move hand rises, spins and shrinks for 31 ticks, bursts into eight
   sparkles with SE_PSYNERGY_SPARKLE, and is put out of sight. */
static void scValeVanishMoveHand(ACTOR *emitter)
{
	Sint32 frm;

	sySndPlay(SE_CAST);

	for(frm = 0; frm < 31; frm++) {
		emitter->y.fixed += toFIXED(1);
		emitter->facing = (Uint16)(emitter->facing + DEGtoANG(45));
		emitter->scaleX -= toFIXED(1) / 32;
		emitter->scaleY -= toFIXED(1) / 32;
		sySynch(1);
	}

	for(frm = 0; frm < 8; frm++) {
		ACTOR *particle = syFieldActorCreate(SPRITE_SPARKLE, emitter->x.fixed,
			emitter->y.fixed, emitter->z.fixed);

		if(particle != NULL) {
			FIXED verticalSpeed;

			fdActorSetSprFlags(particle, 0);
			fdActorStartScript(particle, (const void *)&ValeSparkleScript[1]);

			verticalSpeed = syRandomNext() + toFIXED(1);
			particle->acceleration = toFIXED(1);
			particle->speedLimit = verticalSpeed;
			particle->motionFlg = ACTOR_FALLS;
			particle->gravity = toFIXED(1) / 25;

			particle->velY = syRandomNext() - syRandomNext();

			fdActorSetTargetPosFromMagnitudeAng(particle,
				((syRandomNext() * 3) << 3) + toFIXED(8), syRandomNext());
		}
	}

	sySndPlay(SE_PSYNERGY_SPARKLE);

	emitter->x.fixed = 0;
	emitter->y.fixed = 0;
	emitter->z.fixed = 0;
	emitter->tgtX = ACTOR_NO_TARGET;
	emitter->tgtY = ACTOR_NO_TARGET;
	emitter->tgtZ = ACTOR_NO_TARGET;
	SET_VELOCITY(emitter, 0, 0, 0);
}

DEFINE_SET_TARGET_POSITION_FROM_MAGNITUDE_ANGLE(
	static, syFieldActorSetMoveTargetFX)

/* Walking into Garet's training stone, below the cliff north of his house,
   pushes it a metatile: east from its west side, metatiles (22, 36) to (23,
   36), or west from its east side, metatiles (24, 36) to (25, 36). It moves
   only between metatiles (22, 36) and (24, 36).

   The push itself is the shared one (scPushedActorAdvancePair), and
   scValeMovePillarBelowCliff then settles the metatiles under it. */
static void scValePushPillar(void)
{
#if !VALE_PILLAR_PUSH_DEAD_LOOKUP_DROPPED
	/* DEAD: the Japanese edition looks the pillar up and drops it; the ROM
	   keeps the calls. */
	if(gmFlagGet(FLAG_KALAY_REACHED))
		fdActorGet(ACTOR_PILLAR_AFTER_KALAY);
	else
		fdActorGet(ACTOR_PILLAR);
#endif
	fdMapGridCopyMetatileAttributes(17, 0, 3, 1, 22, 36);
	scPushedActorAdvancePair();
	scValeMovePillarBelowCliff();
}

/* Move on Garet's training stone, below the cliff north of his house, pushes it
   a metatile east or west with the hand of Move, between metatiles (22, 36) and
   (24, 36).

   The Psynergy moves the stone; this script then settles the row under it, as
   it does after a push and when Vale is set up. The metatiles either side of
   the stone push it, and its own metatile is one Isaac jumps across
   (METATILE_BEHAVIOR_JUMP_FORWARD), as the metatile west of the row is. The
   row's metatiles are copied from metatiles (17, 0) to (19, 0) of the map and
   the stone's own from metatile (17, 1), and its column is kept in
   FLAG_VALE_PILLAR_IN_WEST_COLUMN, _IN_MIDDLE_COLUMN or _IN_EAST_COLUMN. */
static void scValeMovePillarBelowCliff(void)
{
	ACTOR *actor;
	Sint32 column;

	fdMapGridCopyMetatileAttributes(17, 0, 3, 1, 22, 36);
	if(gmFlagGet(FLAG_KALAY_REACHED))
		actor = fdActorGet(ACTOR_PILLAR_AFTER_KALAY);
	else
		actor = fdActorGet(ACTOR_PILLAR);
	if(actor == NULL)
		return;
	fdScriptClearFlag(FLAG_VALE_PILLAR_IN_WEST_COLUMN);
	fdScriptClearFlag(FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN);
	fdScriptClearFlag(FLAG_VALE_PILLAR_IN_EAST_COLUMN);
	column = FIELD_METATILE(actor->x.fixed);
	if(column == 22) {
		fdMapGridCopyMetatileAttributes(17, 1, 1, 1, column, 36);
		gmFlagSet(FLAG_VALE_PILLAR_IN_WEST_COLUMN);
	} else if(column == 23) {
		fdMapGridCopyMetatileAttributes(17, 1, 1, 1, column, 36);
		gmFlagSet(FLAG_VALE_PILLAR_IN_MIDDLE_COLUMN);
	} else {
		fdMapGridCopyMetatileAttributes(17, 1, 1, 1, 24, 36);
		gmFlagSet(FLAG_VALE_PILLAR_IN_EAST_COLUMN);
	}
}

/* The event that plays SE_PSYNERGY_SPARKLE every 16 ticks while Isaac's
   Psynergy lifts a board at the roof's end. */
static void scValePlayPsynergySparkleSnd(void)
{
	if((TickCount & 15) == 0)
		sySndPlay(SE_PSYNERGY_SPARKLE);
}

/* Searching the door of the house the storm wrecked, on the west bank in the
   south of Vale, at metatile (6, 74), finds it stuck: "The door won't open. The
   force of the impact must have knocked it off its hinges." */
static void scValeSearchTheLockedDoor(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgValeDoorWontOpen, 1);
	fdScriptEnd();
}

/* Keeps the man guarding Mt. Aleph standing still while a menu is open, and
   lets him walk on when it closes.

   It runs every tick from Vale's setup, before the party leaves, and
   FLAG_VALE_MT_ALEPH_GUARD_PAUSED remembers that it stopped him. */
static void scValeSynchronizeAlephGuardPause(void)
{
	Sint32 pauseFlg;

	if(fdScriptGetFlag(FLAG_VALE_MT_ALEPH_GUARD_PAUSED) != 0) {
		pauseFlg = gmFlagGet(FLAG_MENU_OPEN);
		if(pauseFlg == 0) {
			fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = pauseFlg;
			gmFlagClear(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
		}
	} else {
		if(gmFlagGet(FLAG_MENU_OPEN) != 0) {
			fdActorGet(ACTOR_MAN_GUARDING_MT_ALEPH)->paused = 1;
			gmFlagSet(FLAG_VALE_MT_ALEPH_GUARD_PAUSED);
		}
	}
}

/* Draws Garet's training stone under Isaac while he is up on the cliff above
   it, and in front of him or behind as usual while he is down beside it.

   It runs every tick. Isaac is above the stone when his height is over 200;
   the stone's foot is at height 192. */
static void scValeSetPillarPriorityByLeaderHeight(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	ACTOR *pillar;
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0)
		pillar = fdActorGet(ACTOR_PILLAR_AFTER_KALAY);
	else
		pillar = fdActorGet(ACTOR_PILLAR);
	if(pillar != NULL) {
		if(leader->y.fixed > toFIXED(200))
			pillar->priFlg =
				ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
		else
			pillar->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	}
}

#include "shared/floating_nut.c"

/* Someone calling up Psynergy. They are drawn SPRITE_COLOR_WHITE for two ticks
   in four, with the World Map's attached sparks every 16 ticks. */
static void scValeFlashActorAndSpawnEffects(ACTOR *actor)
{
	if((TickCount & 2) != 0)
		fdActorSetColMode(actor, SPRITE_COLOR_WHITE);
	else
		fdActorSetColMode(actor, SPRITE_COLOR_NORMAL);
	if((TickCount & 0xf) == 0)
		scSpawnGoldenRing(actor);
}

/* Someone using Psynergy. */
static void scValeCycleActorPaletteAndSpawnEffects(ACTOR *actor)
{
	if((TickCount & 1) != 0)
		fdActorSetColMode(actor, (TickCount >> 1) % 6);
	if((TickCount & 0xf) == 0)
		scSpawnGoldenRing(actor);
}

/* What Psynergy moves. It goes through colour modes SPRITE_COLOR_NORMAL to
   SPRITE_COLOR_BRG in turn, a step every second tick. */
static void scValeCycleActorPalette(ACTOR *actor)
{
	if((TickCount & 1) != 0)
		fdActorSetColMode(actor, (TickCount >> 1) % 6);
}

#include "shared/field_effect_vertical.c"
#include "shared/golden_ring.c"

#include "shared/pushed_actor_tables.c"

/* clang-format off */

/* The beat the man guarding Mt. Aleph walks once he has warned Isaac, */
/* looking about at each stop: east along the front of the Sanctum and up */
/* behind it, back before its door, then west and up the path along its west */
/* side, and back. SW_TIMER says which stretch of it he is on */
/* (VALE_GUARD_BEAT). */
Sint32 ValeAlephGuardPatrolRoute[] = {
	SC_SPEED(0.5),
	SC_ACCELERATION(0.25),
	SC_SET(SW_TIMER, GUARD_BEFORE_THE_SANCTUM),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(296, 0, 456),
	SC_SYNC,
	SC_SET(SW_TIMER, GUARD_BEHIND_THE_SANCTUM),
	SC_MOVE(296, 0, 420),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(296, 0, 456),
	SC_SYNC,
	SC_SET(SW_TIMER, GUARD_BEFORE_THE_SANCTUM),
	SC_MOVE(256, 0, 456),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(15),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(60),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(212, 0, 456),
	SC_SYNC,
	SC_SET(SW_TIMER, GUARD_ON_THE_PATH),
	SC_MOVE(212, 0, 388),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(40),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(212, 0, 429),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(30),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(5),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(212, 0, 456),
	SC_SYNC,
	SC_SET(SW_TIMER, GUARD_BEFORE_THE_SANCTUM),
	SC_MOVE(256, 0, 456),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(40),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(5),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* Isaac on the roof, his sprite stretching and settling with each blow of */
/* the hammer. */
Sint32 ValeLeaderHammers[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 100),
	SC_WAIT(12),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(12),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

/* Garet and Jenna walk to the front of Isaac's house to watch him on the */
/* roof. */

Sint32 ValeGaretWalksToTheHouse[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(300, 0, 880),
	SC_SYNC,
	SC_MOVE(300, 0, 904),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

Sint32 ValeJennaWalksToTheHouse[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(300, 0, 904),
	SC_SYNC,
	SC_MOVE(321, 0, 904),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

/* A spout of water grows from a tenth of its size north of the strongman by */
/* the river and vanishes (scValeUnusedWaterBurstsByTheRiver). */
Sint32 ValeUnusedWaterGrows[] = {
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(266, 0, 1230),
	SC_SET(SW_SCALE_X, toFIXED(1) / 10),
	SC_SET(SW_SCALE_Y, toFIXED(1) / 10),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(3) / 50),
	SC_ADD(SW_SCALE_Y, toFIXED(2) / 25),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

/* A sparkle falls and vanishes after 10 ticks. The burst's sparkles start */
/* at its second word, past the first SC_RELEASE. */
Sint32 ValeSparkleScript[] = {
	SC_RELEASE,
	SC_SYNC,
	SC_SPEED(0.0625),
	SC_ANIMATION(ANIM_STILL),
	SC_WAIT(10),
	SC_RELEASE,
};

SCENE_ENTRANCE ValeEntrances[] = {
	{ 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_SANCTUM, CONDITION_ALWAYS, 256, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, 38, 0, 636, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_VALE_KRADENS_COTTAGE_SOUTH, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_VALE_PLAZA, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_COTTAGE_ROOMS_B, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_COTTAGE_ROOMS_A_NORTH_WEST_BANK_DOOR, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_TITLE, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_FROM_ROOF_MENDED, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ValeExits[] = {
	SCENE_EXITS(Vale),
	SCENE_EXIT(VALE_TO_SOL_ENTRANCE, SolEntrance, SOL_ENTRANCE_FROM_VALE),
	SCENE_EXIT(VALE_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_VALE),
	SCENE_EXIT(VALE_TO_VALE_KRADENS_COTTAGE_NORTH, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_NORTH),
	SCENE_EXIT(VALE_TO_VALE_KRADENS_COTTAGE_SOUTH, ValeKradensCottage, VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH),
	SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_EAST),
	SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_SOUTH),
	SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_B, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE),
	SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
	SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_WEST),
	SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH_WEST_BANK_DOOR, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
	SCENE_EXIT(VALE_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE),
	SCENE_EXIT(VALE_TO_ROOF_MENDED, Vale, VALE_FROM_ROOF_MENDED),
	SCENE_EXITS_END,
};

ACTOR_PLACE ValeActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		132, 0, 908, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		92, 0, 508, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 620, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		164, 0, 1236, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		268, 0, 1276, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		276, 0, 1044, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		19, 0, 614, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
		50, 0, 795, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
		65, 0, 800, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
		57, 0, 805, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		412, 0, 586, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		398, 0, 616, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		PILLAR_MIDDLE_X, 0, PILLAR_Z, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		373, 0, VALE_DORA_Z, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
		152, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValeActorPlacementsAfterLeaving[] = {
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		132, 0, 908, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		92, 0, 508, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 620, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		164, 0, 1236, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		268, 0, 1276, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		276, 0, 1044, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		19, 0, 614, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
		50, 0, 795, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
		65, 0, 800, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_CLOSE,
		57, 0, 805, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		412, 0, 586, FACING_WEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		398, 0, 616, FACING_NORTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		PILLAR_MIDDLE_X, 0, PILLAR_Z, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		335, 0, 869, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
		152, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValeActorPlacementsAfterKalay[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		412, 0, 586, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		356, 0, 727, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		164, 0, 1236, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		268, 0, 1276, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		132, 0, 908, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		92, 0, 508, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 620, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		276, 0, 1044, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VALE_NUT_CAUGHT, MOVEMENT_TYPE_STAND,
		152, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		PILLAR_MIDDLE_X, 0, PILLAR_Z, FACING_EAST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValeRepairActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		409, 0, 1134, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		428, 0, 588, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		204, 0, 924, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		PILLAR_EAST_X, 0, PILLAR_Z, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		373, 0, VALE_DORA_Z, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ROOF_BOARDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MOVE_HAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE ValeCues[] = {
	{ CUE_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephLooksBeautifulTodayIts) },
	{ CUE_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeValeStaysPristineBecauseOfMt) },
	{ CUE_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, SCRIPT_REF(scValeTalkAboutTheMayorAndTremors) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeThisPlaceLooksJustLikeIt) },
	{ CUE_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacWatchOutDontGetToo) },
	{ CUE_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeValeMustRemainHiddenPsynergysSecrets) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeWarnAboutGoingBeyondKradensCottage) },
	{ CUE_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, SCRIPT_REF(scValeArgueWithGaretsSister) },
	{ CUE_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeYoureInTroubleNowGaretYour) },
	{ CUE_TALK, ACTOR_DORA, CONDITION_ALWAYS, SCRIPT_REF(scValeDoraTalksAboutTheHole) },
	{ CUE_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephIsASacredPeak) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_SANCTUM_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterSanctum) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS, SCRIPT_REF(scValeApproachMtAleph) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValeKradensCottageNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValeKradensCottageSouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValePlaza) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterGaretsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterCottageRoomsA) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterIsaacsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterJennasGrandparentsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterCottageRoomsAWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeUnusedEnterJennasGrandparentsHouseByWestBankDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scValeStepOntoBridge) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scValeStepOffBridge) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_NORTH_OF_PUPPIES, CONDITION_ALWAYS, SCRIPT_REF(scValeWatchThePuppies) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_SOUTH_OF_PUPPIES, CONDITION_ALWAYS, SCRIPT_REF(scValeTurnBackFromThePuppies) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_GUARD_JUMPS_OUT, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, SCRIPT_REF(scValeWarnAgainstSneakingUpMtAleph) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PATH_UP_MT_ALEPH, CONDITION_ALWAYS, SCRIPT_REF(scValeCatchSneakingOnThePath) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_BESIDE_THE_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scValeCatchSneakingBesideTheSanctum) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_FOOT_OF_THE_PATH, CONDITION_ALWAYS, SCRIPT_REF(scValeCatchSneakingAtThePathsFoot) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_LADDER_FOOT, CONDITION_ALWAYS, SCRIPT_REF(scValeClimbLadderOntoRoof) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_VALE_LADDER_TOP, CONDITION_ALWAYS, SCRIPT_REF(scValeClimbDownFromRoof) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, SCRIPT_REF(scValeCatchNut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scValeMovePillarBelowCliff) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePushPillar) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePushPillar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeSearchTheLockedDoor) },
	{ CUE_TABLE_END },
};

CUE ValeCuesAfterLeaving[] = {
	{ CUE_TALK, ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeIsSavedThanksToThe) },
	{ CUE_TALK, ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeEverSinceTheTremorsStartedMt) },
	{ CUE_TALK, ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, SCRIPT_REF(scValeTalkAboutTheMayorAndTremors) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeSighNoLongerWillISee) },
	{ CUE_TALK, ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeIDontBlameFelixIfOnly) },
	{ CUE_TALK, ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeTheMayorWasTooKindTo) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeWarnAboutGoingBeyondKradensCottage) },
	{ CUE_TALK, ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
	{ CUE_TALK, ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseTakeCareOfMyBrother) },
	{ CUE_TALK, ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GUARDING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER, CONDITION_ALWAYS, MSGNO(MsgValeGeeIWishICouldSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgValeMyBrothersStrengthIsHisBest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_REMEMBERING_THE_STORM, CONDITION_ALWAYS, MSGNO(MsgValeImGladThatFelixIsAlive) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_BY_THE_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeFelixWasAGoodPersonI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_WAY_TO_KRADEN, CONDITION_ALWAYS, MSGNO(MsgValePoorKradenWasTakenToSome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ADMIRING_MT_ALEPH, CONDITION_ALWAYS, MSGNO(MsgValeValeWasSavedBecausePeopleLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALEPHS_PURITY, CONDITION_ALWAYS, MSGNO(MsgValeICantBelieveAnyoneWouldClimb) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_JEALOUS_OF_GARET, CONDITION_ALWAYS, MSGNO(MsgValeIJustDontFeelSafeKnowing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_VALE_HIDDEN, CONDITION_ALWAYS, MSGNO(MsgValeThePeopleOfValeWillNever) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_SANCTUM_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterSanctum) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS, SCRIPT_REF(scValeApproachMtAleph) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValeKradensCottageNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValeKradensCottageSouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValePlaza) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterGaretsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterCottageRoomsA) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterIsaacsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterJennasGrandparentsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterCottageRoomsAWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeUnusedEnterJennasGrandparentsHouseByWestBankDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scValeStepOntoBridge) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scValeStepOffBridge) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_NORTH_OF_PUPPIES, CONDITION_ALWAYS, SCRIPT_REF(scValeWatchThePuppies) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_SOUTH_OF_PUPPIES, CONDITION_ALWAYS, SCRIPT_REF(scValeTurnBackFromThePuppies) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_GUARD_JUMPS_OUT, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, SCRIPT_REF(scValeWarnAgainstSneakingUpMtAleph) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PATH_UP_MT_ALEPH, CONDITION_ALWAYS, SCRIPT_REF(scValeCatchSneakingOnThePath) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_BESIDE_THE_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scValeCatchSneakingBesideTheSanctum) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_FOOT_OF_THE_PATH, CONDITION_ALWAYS, SCRIPT_REF(scValeCatchSneakingAtThePathsFoot) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_LADDER_FOOT, CONDITION_ALWAYS, SCRIPT_REF(scValeClimbLadderOntoRoof) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_VALE_LADDER_TOP, CONDITION_ALWAYS, SCRIPT_REF(scValeClimbDownFromRoof) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT, SCRIPT_REF(scValeCatchNut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scValeMovePillarBelowCliff) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePushPillar) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePushPillar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeSearchTheLockedDoor) },
	{ CUE_TABLE_END },
};

CUE ValeCuesAfterKalay[] = {
	{ CUE_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_VALE_GARET_MET_SISTER, SCRIPT_REF(scValeGaretMeetsHisSister) },
	{ CUE_TALK, ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICanTellJustByLooking) },
	{ CUE_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY, SCRIPT_REF(scValeGaretAsksToProtectFamily) },
	{ CUE_TALK, ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeIWillDoMyBestTo) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, SCRIPT_REF(scValeAskWhetherJennaHasReturned) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValePromiseMeIsaacPromiseMeYoull) },
	{ CUE_TALK, ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeIUsedToPlayHere) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephsEruptionWasSoPowerful) },
	{ CUE_TALK, ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, SCRIPT_REF(scValeFeelAnotherTremor) },
	{ CUE_TALK, ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheDangerIsGone) },
	{ CUE_TALK, ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, SCRIPT_REF(scValeAskAboutTheLackOfTravelers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_GARETS_SISTER_AFTER_KALAY, FLAG_VALE_GARET_MET_SISTER, SCRIPT_REF(scValeGaretMeetsHisSister) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_SISTER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreWorkingSoHard) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_GARETS_BROTHER_AFTER_KALAY, FLAG_VALE_GARET_ASKED_TO_PROTECT_FAMILY, SCRIPT_REF(scValeGaretAsksToProtectFamily) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_BROTHER_AFTER_KALAY, CONDITION_ALWAYS, MSGNO(MsgValeICouldGoWithYouIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, SCRIPT_REF(scValeAskWhetherJennaHasReturned) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_FOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgValeYouDidntBringJennaWithYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_REMEMBERING_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixMustBeProtectingJennaIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeTheEarthquakesArentCausedByEruptions) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FEELING_TREMORS, CONDITION_ALWAYS, MSGNO(MsgValeAhhICantStandTheseEarthquakes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_DOUBTING_SAFETY, CONDITION_ALWAYS, MSGNO(MsgValeTheSmokeFromMtAlephIsnt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_MAN_MISSING_TRAVELERS, FLAG_VALE_ASKED_ABOUT_TRAVELERS, SCRIPT_REF(scValeAskAboutTheLackOfTravelers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_MISSING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgValeHavingNoTravelersIsNoFun) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_SANCTUM_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterSanctum) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS, SCRIPT_REF(scValeApproachMtAleph) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValeKradensCottageNorth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValeKradensCottageSouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS, SCRIPT_REF(scValeExitToValePlaza) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterGaretsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterCottageRoomsA) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterIsaacsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterJennasGrandparentsHouse) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeEnterCottageRoomsAWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeUnusedEnterJennasGrandparentsHouseByWestBankDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scValeStepOntoBridge) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scValeStepOffBridge) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VALE_LADDER_FOOT, CONDITION_ALWAYS, SCRIPT_REF(scValeClimbLadderOntoRoof) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_VALE_LADDER_TOP, CONDITION_ALWAYS, SCRIPT_REF(scValeClimbDownFromRoof) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT_AFTER_KALAY, FLAG_VALE_NUT_CAUGHT, SCRIPT_REF(scValeCatchNutAfterKalay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_AFTER_KALAY, CONDITION_ALWAYS, SCRIPT_REF(scValeMovePillarBelowCliff) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_VALE_WEST_OF_TRAINING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePushPillar) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_VALE_EAST_OF_TRAINING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePushPillar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeSearchTheLockedDoor) },
	{ CUE_TABLE_END },
};
/* clang-format on */

/* The doors' metatile animations, from the layout's metatiles below the
   village, each picture shown for two ticks; vale_prologue.c has the same four
   kinds. Each table takes the name of a door it opens. The two of Jenna's
   grandparents' kind and the west house's show their one open picture twice,
   where the stormy night's show two. */
Uint16 ValeSanctumDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
Uint16 ValeIsaacsDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
Uint16 ValeGrandparentsDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
Uint16 ValeWestHouseDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};

/* The man guarding Mt. Aleph drives Isaac back down the path to the Sanctum's
   door: at the first warning (scValeWarnAgainstSneakingUpMtAleph), and each
   time he catches him on it again (scValeAskAboutSneakingToAleph). */

Sint32 ValeFirstWarningLeaderPath[] = {
	SC_ANIMATION(ANIM_LEADER_RUN),
	SC_MOVE(199, 0, 446),
	SC_SYNC,
	SC_MOVE(215, 0, 472),
	SC_SYNC,
	SC_MOVE(239, 0, 461),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValeFirstWarningGuardPath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(199, 0, 446),
	SC_SYNC,
	SC_MOVE(215, 0, 472),
	SC_SYNC,
	SC_MOVE(239, 0, 461),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValeLeaderRunsBackFromThePath[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_ANIMATION(ANIM_LEADER_RUN),
	SC_MOVE(224, 0, 458),
	SC_SYNC,
	SC_MOVE(256, 0, 458),
	SC_SYNC,
	SC_MOVE(256, 0, 474),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_END,
};

Sint32 ValeGuardComesDownThePath[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(216, 0, 458),
	SC_SYNC,
	SC_MOVE(256, 0, 456),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};

Sint32 ValeLeaderRunsBackBeforeTheDoor[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_ANIMATION(ANIM_LEADER_RUN),
	SC_MOVE(256, 0, 458),
	SC_SYNC,
	SC_MOVE(256, 0, 474),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_END,
};

Sint32 ValeGuardStepsBeforeTheDoor[] = {
	SC_SPEED(1.5),
	SC_ACCELERATION(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(256, 0, 456),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};
