/* Elemental Star Room (SceneIdSolSanctumElementalStarRoom), the heart of Sol
   Sanctum, reached through the portal in the Luna and Sol rooms
   (SceneIdSolSanctumLunaSolRooms). Stone pillars stand in a starry dark, and
   at the four corners a statue holds up a glowing Elemental Star: the Venus
   Star in the south east, the Mercury Star in the north east, the Jupiter Star
   in the south west and the Mars Star in the north west. The party arrives on
   the platform in the middle, under the beam of light it came through.

   In story order:
   - On the first arrival (scSolSanctumElementalStarRoomEnterStarRoom) Isaac,
	 Garet, Jenna and Kraden appear on the middle platform. Kraden names each
	 star as the view goes to it, is too unsteady on the slippery stone to
	 fetch them himself, and gives Isaac four Mythril Bags. Jenna leaves the
	 party and waits with him on the platform, and Kraden calls the boys back
	 if they walk back into the light
	 (scSolSanctumElementalStarRoomKradenStopsPartyLeaving).
   - Only the Venus Star can be reached at first. Searching a statue puts its
	 star in a Mythril Bag; the room shakes and three sunken pillars rise out
	 of the dark as stepping stones to the next. The Venus Star opens the way
	 to the Mercury Star, and the two open the way to the Jupiter Star.
   - Taking the Jupiter Star brings out Saturos and Menardi, holding Jenna and
	 Kraden on the middle platform
	 (scSolSanctumElementalStarRoomBagJupiterStarAndMeetSaturos). Their masked
	 companion takes off his mask and is Felix, Jenna's brother, and Alex
	 appears and takes the three stars from Garet. From then on the four stand
	 round Jenna and Kraden, each with a word about the last star.
   - Taking the Mars Star, in the north west, sets the room quaking, and the
	 story goes on in the eruption (scSolSanctumElementalStarRoomTakeMarsStar,
	 SceneIdSolSanctumEruption, sol_sanctum_eruption.c).

   A star is taken once, because each statue's examine cue row holds only until
   its flag is set. A return after a battle finds the room as it was: on every
   entry scInit puts out the taken stars' glow and redraws the pillars each star
   raised, and once Alex holds the stars it lines up the captors again
   (scSolSanctumElementalStarRoomStandAroundHostages).

   Ten lights float about the room (scSolSanctumStartFloatingLights), and
   Retreat does not work here.

   Whoever arrives on the platform appears 40 pixels above it at a tenth of his
   size. He grows to full size and floats down onto it, over a shadow that grows
   with him. The platform's actor, ACTOR_PLATFORM_SHADOW, draws that shadow. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "scene/sol_sanctum_elemental_star_lights.h"
#include "scene/sparkle_warp.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern Sint32	   SolSanctumElementalStarRoomAngleScript[];
extern Sint32	   ItemIconGrowScript[];
extern Uint32	   SolSanctumElementalStarRoomExits[];
extern ACTOR_PLACE SolSanctumElementalStarRoomActorPlacements[];
extern CUE		   SolSanctumElementalStarRoomCues[];
extern Uint16	   SolSanctumStarMetatiles[];

static void scSolSanctumElementalStarRoomBagJupiterStar(void);
static void scSolSanctumElementalStarRoomRevealSaturosAndMenardi(void);
static void scSolSanctumElementalStarRoomDemandElementalStars(void);
static void scSolSanctumElementalStarRoomOfferHostageGuarantee(void);
static void scSolSanctumElementalStarRoomFelixRemovesMask(void);
static void scSolSanctumElementalStarRoomAlexTakesStars(void);
static void scSolSanctumElementalStarRoomStandAroundHostages(void);
static void scSolSanctumElementalStarRoomShowMessageAndWait(
	Sint32 speaker, Sint32 ticks);
static Sint32 scSolSanctumElementalStarRoomPresentStarFX(
	Sint32 item, FIXED x, FIXED y, FIXED z);
/* scSolSanctumElementalStarRoomPresentStarFX at a point in whole pixels. */
#define scSolSanctumElementalStarRoomPresentStar(item, x, y, z) \
	scSolSanctumElementalStarRoomPresentStarFX( \
		item, toFIXED(x), toFIXED(y), toFIXED(z))
static void scSolSanctumElementalStarRoomGiveMythrilBags(void);
static void scSolSanctumElementalStarRoomAlexWarpsIn(void);

/* The placed actors, numbered in the order of
   SolSanctumElementalStarRoomActorPlacements.

   Garet and Jenna, placed first, keep their own slots. */
typedef enum STAR_ROOM_ACTOR_ID {
	/* The shadow on the arrival platform under whoever appears there.

	   It is a blank actor, whose sprite draws only its shadow, set at (471,
	   290). It grows from a tenth of its size with the arriving actor, whose
	   own shadow is turned off while he floats above the platform. That is
	   Isaac and Kraden in scSolSanctumElementalStarRoomEnterStarRoom, and Felix
	   in scSolSanctumElementalStarRoomDemandElementalStars. */
	ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
	ACTOR_KRADEN,
	ACTOR_SATUROS,
	ACTOR_MENARDI,
	/* Felix in his mask, until
	   scSolSanctumElementalStarRoomFelixRemovesMask. */
	ACTOR_FELIX_MASKED,
	ACTOR_FELIX,
	ACTOR_ALEX,
	/* The first of the ten lights that float about the room, all one sprite
	   (scSolSanctumStartFloatingLights). */
	ACTOR_FIRST_FLOATING_LIGHT,
	ACTOR_LAST_FLOATING_LIGHT = ACTOR_FIRST_FLOATING_LIGHT + 9
} STAR_ROOM_ACTOR_ID;

/* The metatile behaviors the scripts answer. */
typedef enum SOL_SANCTUM_ELEMENTAL_STAR_ROOM_METATILE_BEHAVIOR {
	/* The beam of light north of the middle platform, the way back to the Luna
	   and Sol rooms: metatiles (29, 17) and (29, 18). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK = 1,
	/* The statue holding the Venus Star, in the south east: metatile (44,
	   28). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_STATUE = 2,
	/* The statue holding the Mercury Star, in the north east: metatile (44,
	   8). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_STATUE = 3,
	/* The statue holding the Jupiter Star, in the south west: metatile (14,
	   28). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_STATUE = 4,
	/* The statue holding the Mars Star, in the north west: metatile (14, 8). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_STATUE = 5,
	/* A pillar's edge to jump east from: (15, 21) and (45, 28). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST = 10,
	/* The middle platform's south edge, to jump south from: (29, 22). */
	METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH = 11
} SOL_SANCTUM_ELEMENTAL_STAR_ROOM_METATILE_BEHAVIOR;

/* The room's map (sol_sanctum_elemental_star_room) draws its middle layer from
   the grid 40 rows down and its overhead layer from 56 columns across, so a
   metatile of either layer is written as the room's metatile plus that
   start. */
#define MIDDLE_LAYER_ROW	  40
#define OVERHEAD_LAYER_COLUMN 56

/* Puts out the glow round a statue's star once the star is taken.

   It draws a blank 3-by-3 of the middle layer over the glow drawn above the
   statue, whose top-left metatile is (column, row). */
#define PUT_OUT_STAR_LIGHT(column, row) \
	fdMapGridCopyMetatilesTo( \
		0, MIDDLE_LAYER_ROW, column, MIDDLE_LAYER_ROW + (row), 3, 3)

/* The overhead layer's column where a risen pillar's picture starts, three
   metatiles wide about the pillar's own column. */
#define RISEN_PILLAR_LEFT(column) (OVERHEAD_LAYER_COLUMN + (column) - 1)

/* Lets the party onto a risen pillar whose top is the metatile (column, row).

   The top takes plain ground, and the two ground-layer metatiles under its
   shaft are redrawn. */
#define OPEN_RISEN_PILLAR(column, row) \
	(fdMapGridCopyMetatileAttributes(0, 0, 1, 1, column, row), \
		fdMapGridCopyMetatilesTo(87, 42, column, (row) + 2, 1, 2))

/* Draws a sunken pillar risen, as bagging a star raises three of them into
   stepping stones: its picture, 3 by 4 metatiles on the overhead layer, then
   OPEN_RISEN_PILLAR. The scripts play the rise (SolSanctumStarMetatiles) where
   this draws its last picture. */
#define DRAW_RISEN_PILLAR(column, row) \
	(fdMapGridCopyMetatilesTo(83, 40, RISEN_PILLAR_LEFT(column), row, 3, 4), \
		OPEN_RISEN_PILLAR(column, row))

struct ACTOR;

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Advances through the current dialogue's consecutive messages. */
static inline void scSolSanctumElementalStarRoomSkipMessages(Sint32 amount)
{
	FieldStateWork->msg += amount;
}

#include "shared/actor_facing.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SolSanctumStarRoomEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SolSanctumElementalStarRoomExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return SolSanctumElementalStarRoomActorPlacements;
}

/* Returns the statues' examine cues, the talk rows, the beam of light that
   Kraden calls the party back from, and the edges to jump from. The Mind Read
   rows are there too, though no one in the party knows Mind Read yet. */
const CUE *scGetCues(void)
{
	return SolSanctumElementalStarRoomCues;
}

/* Isaac searches the statue in the south east of the room (metatile (44, 28))
   and puts the Venus Star in a Mythril Bag, and three pillars rise out of the
   dark.

   The room rumbles and flashes the yellow of earth, and the view goes to each
   sunken pillar in turn as it rises with a clatter: one west of the Venus
   statue, one east of the Jupiter statue in the south west, and one west of the
   Mercury statue in the north east. Back at the Venus statue the star's glow
   goes out and the star rises over Isaac's head. Kraden, on the middle
   platform, hops for joy: "Good, good! Now, go fetch the others!" */
static void scSolSanctumElementalStarRoomBagVenusStar(void)
{
	Uint32 i;
	Sint32 member;
	Sint32 msg;

	fdScriptStart();
	sySndPlay(SE_RUMBLE);
	for(i = 0; i != 6; i++) {
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_BLEND | RGB(18, 18, 14), PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		if(i == 1) {
			fdMapGridSetScreenShake(toFIXED(1), toFIXED(1), toFIXED(1));
		}
	}
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(30);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(664, CAMERA_KEEP, 497, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdMapGridSetShake(1, 2, 1);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(41), 29);
	OPEN_RISEN_PILLAR(41, 29);
	fdScriptWait(40);
	fdMapGridSetScreenShake(0, 0, 0);
	fdCameraSetSpeed(6.4);
	fdCameraMoveTo(311, CAMERA_KEEP, 497, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(19), 29);
	OPEN_RISEN_PILLAR(19, 29);
	fdScriptWait(40);
	fdMapGridSetScreenShake(0, 0, 0);
	fdCameraMoveTo(663, CAMERA_KEEP, 192, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(41), 10);
	OPEN_RISEN_PILLAR(41, 10);
	fdScriptWait(40);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(710, CAMERA_KEEP, 474, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdMapGridSetShake(2, 1, 1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdScriptWait(20);
	PUT_OUT_STAR_LIGHT(43, 26);
	fdScriptWait(20);
	member =
		scSolSanctumElementalStarRoomPresentStar(ITEM_VENUS_STAR, 712, 16, 464);
	fdScriptWait(40);
	mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
	msg = MsgSolSanctumVenusStarBagged;
	/* "...put the Venus Star in a Mythril Bag." */
	mnFieldMessageShow(msg, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(462, CAMERA_KEEP, 350, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorJump(ACTOR_KRADEN, 4, 30);
	fdScriptSetMessageId(msg - 1);
	/* "Good, good! Now, go fetch the others!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(710, CAMERA_KEEP, 474, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
	fdScriptEnd();
}

/* Isaac searches the statue in the north east of the room (metatile (44, 8))
   and puts the Mercury Star in a Mythril Bag, and three more pillars rise.

   It plays as the Venus Star does, the room flashing the blue of water. The
   pillars rise north of the middle platform, west of it, and in the south west
   by the Jupiter statue, which can now be reached. Kraden hops again: "Good,
   good! Now, go fetch the others!" */
static void scSolSanctumElementalStarRoomBagMercuryStar(void)
{
	Uint32 i;
	Sint32 member;
	Sint32 msg;

	fdScriptStart();
	sySndPlay(SE_RUMBLE);
	for(i = 0; i != 6; i++) {
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_BLEND | RGB(14, 18, 18), PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		if(i == 1) {
			fdMapGridSetShake(1, 1, 1);
		}
	}
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(30);
	fdMapGridSetShake(0, 0, 0);
	fdCameraSetSpeed(5.6);
	fdCameraMoveTo(472, CAMERA_KEEP, 98, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(29), 4);
	OPEN_RISEN_PILLAR(29, 4);
	fdScriptWait(40);
	fdMapGridSetShake(0, 0, 0);
	fdCameraMoveTo(343, CAMERA_KEEP, 369, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(21), 21);
	OPEN_RISEN_PILLAR(21, 21);
	fdScriptWait(40);
	fdMapGridSetShake(0, 0, 0);
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(343, CAMERA_KEEP, 497, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(21), 29);
	OPEN_RISEN_PILLAR(21, 29);
	fdScriptWait(40);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(712, CAMERA_KEEP, 152, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdMapGridSetShake(2, 1, 1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdScriptWait(20);
	PUT_OUT_STAR_LIGHT(43, 6);
	fdScriptWait(20);
	member = scSolSanctumElementalStarRoomPresentStar(
		ITEM_MERCURY_STAR, 712, 16, 144);
	fdScriptWait(40);
	mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
	msg = MsgSolSanctumPutMercuryStar;
	/* "...put the Mercury Star in a Mythril Bag." */
	mnFieldMessageShow(msg, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(462, CAMERA_KEEP, 350, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorJump(ACTOR_KRADEN, 4, 30);
	fdScriptSetMessageId(msg - 2);
	/* "Good, good! Now, go fetch the others!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(712, CAMERA_KEEP, 152, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
	fdScriptEnd();
}

/* Isaac searches the statue in the south west of the room (metatile (14, 28))
   and puts the Jupiter Star in a Mythril Bag. Turning back, he and Garet find
   Saturos and Menardi holding Jenna and Kraden on the middle platform.

   The captors demand the stars. Their masked companion takes off his mask and
   is Felix, Jenna's brother, believed dead since the day of the Boulder, and he
   becomes their guarantee that the hostages will not be harmed. Garet carries
   the three stars over, Alex warps in to take them, and the captors send the
   boys for the last one. The four then stand round Jenna and Kraden until the
   Mars Star is taken. */
static void scSolSanctumElementalStarRoomBagJupiterStarAndMeetSaturos(void)
{
	fdScriptStart();
	scSolSanctumElementalStarRoomBagJupiterStar();
	scSolSanctumElementalStarRoomRevealSaturosAndMenardi();
	scSolSanctumElementalStarRoomDemandElementalStars();
	scSolSanctumElementalStarRoomOfferHostageGuarantee();
	scSolSanctumElementalStarRoomFelixRemovesMask();
	scSolSanctumElementalStarRoomAlexTakesStars();
	gmFlagSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
	fdScriptEnd();
	scSolSanctumElementalStarRoomStandAroundHostages();
}

/* The room flashes the purple of wind and three pillars rise, by the Jupiter
   statue and on the way north to the Mars Star; then the star's glow goes out
   and Isaac puts the Jupiter Star in a Mythril Bag. */
static void scSolSanctumElementalStarRoomBagJupiterStar(void)
{
	Uint32 i;
	Sint32 member;

	sySndPlay(SE_RUMBLE);
	for(i = 0; i != 6; i++) {
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_BLEND | RGB(18, 14, 18), PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		if(i == 1) {
			fdMapGridSetShake(1, 1, 1);
		}
	}
	fdMapGridSetShake(2, 1, 1);
	fdScriptWait(30);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(167, CAMERA_KEEP, 529, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(10), 31);
	OPEN_RISEN_PILLAR(10, 31);
	fdScriptWait(40);
	fdMapGridSetShake(0, 0, 0);
	fdCameraSetSpeed(6.4);
	fdCameraMoveTo(391, CAMERA_KEEP, 177, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(24), 9);
	OPEN_RISEN_PILLAR(24, 9);
	fdScriptWait(40);
	fdMapGridSetShake(0, 0, 0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(583, CAMERA_KEEP, 193, 1);
	fdCameraWaitForMove();
	fdMapGridSetShake(1, 2, 1);
	fdScriptWait(20);
	sySndPlay(SE_CLATTER);
	fdMapGridPlayMapEdit(SolSanctumStarMetatiles, RISEN_PILLAR_LEFT(36), 10);
	OPEN_RISEN_PILLAR(36, 10);
	fdScriptWait(40);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(232, CAMERA_KEEP, 477, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdMapGridSetShake(2, 1, 1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdScriptWait(20);
	PUT_OUT_STAR_LIGHT(13, 26);
	fdScriptWait(20);
	member = scSolSanctumElementalStarRoomPresentStar(
		ITEM_JUPITER_STAR, 232, 16, 464);
	fdScriptWait(40);
	mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
	/* "...put the Jupiter Star in a Mythril Bag." */
	mnFieldMessageShow(MsgSolSanctumJupiterStarBagged, 1);
}

/* Isaac and Garet turn back to find Saturos and Menardi holding Jenna and
   Kraden on the centre platform. */
static void scSolSanctumElementalStarRoomRevealSaturosAndMenardi(void)
{
	ACTOR *leader;

	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 231, 490);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(180);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(80);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 246, 479);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkToAndWait(ACTOR_GARET, 257, 491);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 265, 453);
	fdActorWalkToAndWait(ACTOR_GARET, 282, 469);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorJump(ACTOR_GARET, 6, 60);
	fdActorSetPos(ACTOR_JENNA, 475, 332);
	fdActorSetPos(ACTOR_KRADEN, 491, 332);
	fdActorSetPos(ACTOR_MENARDI, 459, 348);
	fdActorSetPos(ACTOR_SATUROS, 507, 348);
	fdCameraSetSpeed(7.2);
	fdCameraMoveTo(485, CAMERA_KEEP, 345, 1);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
	fdCameraWaitForMove();
	fdScriptWait(40);
}

/* Saturos and Menardi demand the stars, and the masked man with them, Felix,
   appears beside them. */
static void scSolSanctumElementalStarRoomDemandElementalStars(void)
{
	ACTOR *felix;
	ACTOR *shadow;
	Sint32 frm;
	SPR	  *spr;

	sySndPlay(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	fdScriptSetMessageId(MsgSolSanctumLooksLikeTheyveSpottedUs);
	/* "It looks like they've spotted us." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
	/* "I thought we'd be safe until they'd gotten all the Elemental..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorJump(ACTOR_KRADEN, 4, 10);
	fdActorJump(ACTOR_KRADEN, 6, 30);
	/* "You! You mean to steal the Elemental Stars!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
	/* "Master Kraden... Are you calling us thieves?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 20);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
	/* "As I recall, aren't you the one who stole our information?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_SOUTHWEST, 80);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What will you do with us once you've got the Elemental Stars?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "I'm sure they'll do us in once they no longer need us!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	felix = fdActorGet(ACTOR_FELIX_MASKED);
	shadow = fdActorGet(ACTOR_PLATFORM_SHADOW);
	spr = felix->spr;
	spr->flg = 0;
	felix->scaleX = toFIXED(1) / 10;
	felix->scaleY = toFIXED(1) / 10;
	shadow->scaleX = toFIXED(1) / 10;
	shadow->scaleY = toFIXED(1) / 10;
	fdActorSetSprColById(ACTOR_FELIX_MASKED, SPRITE_COLOR_BLINK);
	fdActorSetPos(ACTOR_FELIX_MASKED, 471, 290);
	felix->motionFlg = 0;
	felix->y.fixed = toFIXED(40);
	fdScriptWait(1);
	/* "Relax... You will not be hurt." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(471, CAMERA_KEEP, 309, 1);
	fdCameraWaitForMove();
	fdActorSetPos(ACTOR_PLATFORM_SHADOW, 471, 290);
	sySndPlay(SE_WARP);
	fdActorSetSprPriority(ACTOR_FELIX_MASKED, 2);
	for(frm = 0; frm != 90; frm++) {
		felix->y.fixed -= toFIXED(1) / 10;
		felix->scaleX += toFIXED(1) / 100;
		felix->scaleY += toFIXED(1) / 100;
		shadow->scaleX += toFIXED(1) / 100;
		shadow->scaleY += toFIXED(1) / 100;
		fdScriptWait(1);
	}
	felix->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
	fdScriptWait(80);
	for(frm = 0; frm != 60; frm++) {
		felix->y.fixed -= toFIXED(1) / 2;
		fdScriptWait(1);
	}
	felix->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(30);
	spr->flg = SPRITE_FLAG_SHADOW;
	fdActorSetPos(ACTOR_PLATFORM_SHADOW, 0, 0);
	fdActorSetSprPriority(ACTOR_FELIX_MASKED, 1);
	fdActorGet(ACTOR_FELIX_MASKED)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprColById(ACTOR_FELIX_MASKED, SPRITE_COLOR_NORMAL);
	fdActorSetSpeed(ACTOR_FELIX_MASKED, 0.5);
	fdActorWalkToAndWait(ACTOR_FELIX_MASKED, 471, 306);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
	/* "That was our agreement, was it not?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_FELIX_MASKED, 20);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 0);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_SATUROS, 1);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 10);
	/* "That all depends on how well they cooperate." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 30);
	fdActorSetAttachedEffect(
		ACTOR_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	fdScriptWait(10);
	/* "They'll be safe, as long as they hand over the Elemental Stars." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 30);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 30);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
	fdScriptWait(20);
	/* "Isn't that enough?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 30);
	fdActorSetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	fdScriptWait(20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(264, CAMERA_KEEP, 460, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	/* "You heard us... If you wish to save your friends, then give us..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(80);
	/* "Do you accept our terms?" */
	fdScriptOpenMessage(ACTOR_MENARDI, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) != 0) {
		scSolSanctumElementalStarRoomSkipMessages(1);
	}
	/* Yes: "No, Isaac! You must not give them the Elemental Stars!" No: "Very
	   good, Isaac! You must not give them the Elemental Stars!" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(477, CAMERA_KEEP, 334, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
	fdScriptSetMessageId(MsgSolSanctumWhyDenyDont);
	/* "Why would you deny us? Don't you want your friends to be safe?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "What guarantee do we have once we've given you the Elemental..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 40);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	/* "Guarantee... Ah. I have one." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 10);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
}

/* Menardi offers a guarantee of the hostages' safety, and the masked man must
   remove his mask. */
static void scSolSanctumElementalStarRoomOfferHostageGuarantee(void)
{
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX_MASKED, FACING_SOUTH_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
	/* "You will have to remove your mask." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 20);
	fdActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
	fdActorSetAttachedEffect(
		ACTOR_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	/* "Th- That means..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX_MASKED, 10);
	fdActorFaceActor(ACTOR_SATUROS, ACTOR_FELIX_MASKED, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_FELIX_MASKED, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_FELIX_MASKED, 0);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
	/* "Ah, of course. A fine idea... He will be our guarantee." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
	fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	/* "Under the circumstances, we have little choice. Felix!.." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorSetAttachedEffect(
		ACTOR_FELIX_MASKED, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
	fdScriptWait(40);
	fdActorSetPos(ACTOR_GARET, 346, 440);
	sySynch(1);
	/* "What? What did he say, Isaac?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetPos(ACTOR_GARET, 280, 470);
	fdCameraMoveTo(261, CAMERA_KEEP, 466, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	/* "He called that guy Felix... That's what he said, isn't it?" Yes: "You
	   heard it too? Felix? It can't be..." No: "But that's what I heard. Felix?
	   It can't be..." */
	fdScriptAskYesNo(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	/* "That day... The Boulder... We saw him die, didn't we?" Yes: "You
	   remember it, too, Isaac?.." No: "We searched for weeks, but we still
	   couldn't find him." */
	fdScriptAskYesNo(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdScriptSetMessageId(MsgSolSanctumMenardiDontYouWantThem);
	/* "What's wrong, Felix... Don't you want them to feel safe?" */
	fdScriptShowMessage(ACTOR_MENARDI, 0);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(477, CAMERA_KEEP, 334, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
	/* "Felix... Wasn't that the name of your brother?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 40);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "But that accident... My brother, he..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 10);
	fdActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 2);
	fdScriptWait(80);
	/* "I'll do it." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_FELIX_MASKED, ANIM_NOD);
	/* "I'll . . .take off my mask." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX_MASKED, 20);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorFaceDirection(ACTOR_FELIX_MASKED, FACING_NORTH_NORTHWEST, 40);
}

/* Felix unmasks and tells Jenna he survived, saved by Saturos and Menardi. */
static void scSolSanctumElementalStarRoomFelixRemovesMask(void)
{
	ACTOR  *actor;
	ACTOR  *other;
	Uint16 *inventoryEntry;
	Sint32	slot;
	Sint32	starCnt;
	Sint32	msgA;
	Sint32	msgB;

	sySndPlay(SE_RATTLE);
	fdActorRunRepeatedMotion(ACTOR_FELIX_MASKED, 3);
	fdScriptWait(40);
	other = fdActorGet(ACTOR_FELIX_MASKED);
	if(other != NULL) {
		fdActorSetPosFX(ACTOR_FELIX, other->x.fixed, other->z.fixed);
	}
	fdActorSetPos(ACTOR_FELIX_MASKED, 0, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 3);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 3);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	/* "I -It's not possible..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "Is he..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 40);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	/* "I know I've caused you much grief, Jenna." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX, 20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdScriptWait(10);
	/* "It was a miracle that I survived that day..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX, 40);
	fdActorStartRepeatedMotion(ACTOR_SATUROS, 1);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
	fdScriptWait(10);
	/* "We are the ones who saved him." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	fdActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
	/* "We saw him floating unconscious in the river as we passed." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 80);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	/* "I've been with them ever since... I've experienced a lot." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX, 40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(10);
	/* "But why?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 10);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 2);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Why did you leave me all alone..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 80);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
	/* "Jenna..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_FELIX, 80);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(4);
	/* "My own brother... I thought you were dead!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	fdActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
	/* "Save this teary reunion for later." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	/* "That's right. The Elemental Stars come first." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 10);
	fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
	/* "Is this acceptable? Felix would never allow us to harm you..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 80);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
	fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHWEST, 40);
	fdActorStartRepeatedMotion(ACTOR_MENARDI, 2);
	/* "Felix will be our guarantee. Now, give us the Elemental Stars!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_MENARDI, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(261, CAMERA_KEEP, 466, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 244, 478);
	fdActorWalkToAndWait(ACTOR_GARET, 260, 490);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	starCnt = 0;
	inventoryEntry = gmUnitGet(GARET)->inventory;
	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		Uint32 item = *inventoryEntry & INVENTORY_ITEM_ID_MASK;
		inventoryEntry++;
		if(item == ITEM_VENUS_STAR || item == ITEM_MERCURY_STAR ||
			item == ITEM_JUPITER_STAR)
			starCnt++;
	}
	/* "We don't have any choice..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		msgA = MsgSolSanctumGuessTakeElemental;
		fdScriptSetMessageId(msgA);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(10);
		if(starCnt <= 2) {
			/* "I guess I should take the Elemental Stars to them." */
			scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 30);
			fdActorWalkToAndWait(ACTOR_GARET, 252, 486);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			fdScriptWait(10);
			/* "Isaac handed over the Elemental Stars." */
			mnFieldMessageShowCentered((msgA + 1), 1, 0);
		} else {
			fdScriptSetMessageId(MsgSolSanctumRightTake);
			/* "All right, I'll take them." */
			scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 30);
		}
	} else {
		if(starCnt <= 2) {
			msgB = MsgSolSanctumDontHandOver;
			fdScriptSetMessageId(msgB);
			fdActorRunRepeatedMotion(ACTOR_GARET, 3);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
			/* "If you don't hand them over, they won't let Jenna go!" */
			scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 10);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
			fdActorRunRepeatedMotion(ACTOR_GARET, 1);
			fdActorSetSpeed(ACTOR_GARET, 2);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			actor->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_GARET, 244, 478);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
			fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
			fdActorSetDestination(ACTOR_PARTY_LEADER, 218, 471);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
			/* "Garet took the Elemental Stars." */
			mnFieldMessageShowCentered((msgB + 1), 1, 0);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
			actor->steeringFlg |= ACTOR_FACES_MOTION;
		} else {
			fdScriptSetMessageId(MsgSolSanctumWontLetGo);
			fdActorRunRepeatedMotion(ACTOR_GARET, 3);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
			/* "They won't let Jenna go if we don't hand them over!" */
			scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 10);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
		}
	}
	fdCameraSetSpeed(0.5);
	fdCameraFollowActor(ACTOR_GARET, 1);
	fdCameraWaitForMove();
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_GARET, 264, 482);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_GARET, 278, 480);
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
	fdActorMoveToAndWait(ACTOR_GARET, 312, 470);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdScriptWait(30);
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
	fdActorMoveToAndWait(ACTOR_GARET, 342, 470);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdScriptWait(30);
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
	fdActorMoveToAndWait(ACTOR_GARET, 376, 470);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
}

/* Alex appears, takes the three stars from Garet and demands the last one. */
static void scSolSanctumElementalStarRoomAlexTakesStars(void)
{
	Uint32 frm;
	ACTOR *actor;

	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
	fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
	fdActorSetPos(ACTOR_ALEX, 392, 454);
	scSolSanctumElementalStarRoomAlexWarpsIn();
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgSolSanctumPermitRelieveElemental);
	/* "Will you permit me to relieve you of those Elemental Stars?" */
	fdScriptShowMessage(ACTOR_ALEX, 0);
	fdActorSetPos(ACTOR_SATUROS, 469, 348);
	fdScriptWait(20);
	/* "Oh, Alex... You're late!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 10);
	/* "Alex is another of our companions. Hand the stones to him." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_SATUROS, 40);
	fdActorSetPos(ACTOR_SATUROS, 507, 348);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorWalkToAndWait(ACTOR_GARET, 389, 468);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 60);
	/* "Oh, great. After we did all that work..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 20);
	/* "Garet gave him the Elemental Stars." */
	mnFieldMessageShowCentered(MsgSolSanctumPermitRelieveElemental + 4, 1, 10);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_GARET, 376, 470);
	fdScriptWait(30);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	fdScriptSetMessageId(MsgSolSanctumPermitRelieveElemental + 5);
	/* "I'm afraid I must apologize in advance..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_ALEX, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	/* "For you will be bringing us the remaining Elemental Star, as..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_ALEX, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH, 20);
	fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
	actor = fdActorGet(ACTOR_ALEX);
	actor->motionFlg = 0;
	sySndPlay(SE_SHIMMER);
	for(frm = 0; frm != 30; frm++) {
		actor->y.fixed += toFIXED(1);
		fdScriptWait(1);
	}
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	/* "Wait a second!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 10);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 10);
	/* "You want us to grab the other one for you, too?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 20);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 1);
	/* "Oh, did you not understand me?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_ALEX, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "You said you'd set Jenna free once we gave you the Elemental..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 30);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 10);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraMoveTo(477, CAMERA_KEEP, 334, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
	/* "No, my friend... We want you to bring us ALL the Elemental..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_SATUROS, 10);
	/* "Stop grumbling and bring us the last Star!" */
	fdScriptOpenMessage(ACTOR_MENARDI, 0);
	fdCameraSetSpeed(6.4);
	fdCameraMoveTo(374, CAMERA_KEEP, 470, 1);
	fdCameraWaitForMove();
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	if(fdScriptAskYesNoWithActorAnimation(1, 0) != 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
		fdScriptSetMessageId(MsgSolSanctumDontWantAnything);
		/* "You don't want anything to happen to your friends, do you?" */
		fdScriptOpenMessage(ACTOR_ALEX, 0);
		if(fdScriptAskYesNoWithActorAnimation(1, 0) == 0) {
			do {
				fdScriptWait(20);
				fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
				fdScriptWait(10);
				fdScriptSetMessageId(MsgSolSanctumDoubtHowFeel);
				/* "I doubt that's how you really feel. I'll ask you once
				   again...." */
				fdScriptOpenMessage(ACTOR_ALEX, 0);
			} while(fdScriptAskYesNoWithActorAnimation(1, 0) == 0);
		}
	}
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgSolSanctumThankCooperation);
	/* "Thank you for your cooperation." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_ALEX, 30);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	fdScriptWait(10);
	/* "We'll be waiting for the rest." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_ALEX, 30);
	actor->motionFlg = 0;
	fdActorSetSpeed(ACTOR_ALEX, 2.4);
	syFieldActorSetMoveTarget(actor, 460, 0, 360);
	fdActorWaitForMoveById(ACTOR_ALEX);
	fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
	fdScriptWait(30);
	fdCameraFollowActor(ACTOR_GARET, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(20);
	actor = fdActorGet(ACTOR_GARET);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
	fdActorMoveToAndWait(ACTOR_GARET, 342, 470);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdScriptWait(30);
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
	fdActorMoveToAndWait(ACTOR_GARET, 312, 470);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdScriptWait(30);
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_LEAP);
	fdActorMoveToAndWait(ACTOR_GARET, 278, 480);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdScriptWait(30);
	fdCameraSetSpeed(0.5);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	scWalkIntoLeader(ACTOR_GARET);
	gmPartyInventoryDiscard(ITEM_VENUS_STAR);
	gmPartyInventoryDiscard(ITEM_MERCURY_STAR);
	gmPartyInventoryDiscard(ITEM_JUPITER_STAR);
}

/* Stands Saturos, Menardi, Felix and Alex round Jenna and Kraden after the
   exchange, all turning to face the leader. */
static void scSolSanctumElementalStarRoomStandAroundHostages(void)
{
	ACTOR *leader;
	ACTOR *actor;
	ACTOR *alex;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	fdActorSetMovementTypeById(ACTOR_JENNA, MOVEMENT_TYPE_STAND);
	fdActorSetMovementTypeById(ACTOR_KRADEN, MOVEMENT_TYPE_STAND);
	fdActorSetMovementTypeById(ACTOR_MENARDI, MOVEMENT_TYPE_STAND);
	fdActorSetMovementTypeById(ACTOR_SATUROS, MOVEMENT_TYPE_STAND);
	fdActorSetMovementTypeById(ACTOR_ALEX, MOVEMENT_TYPE_STAND);
	fdActorSetMovementTypeById(ACTOR_FELIX, MOVEMENT_TYPE_STAND);
	fdActorSetPos(ACTOR_JENNA, 475, 332);
	fdActorSetPos(ACTOR_KRADEN, 491, 332);
	fdActorSetPos(ACTOR_MENARDI, 459, 348);
	fdActorSetPos(ACTOR_SATUROS, 507, 348);
	fdActorSetPos(ACTOR_ALEX, 460, 360);
	fdActorSetPos(ACTOR_FELIX, 471, 306);
	actor = fdActorGet(ACTOR_JENNA);
	actor->watchedActor = (ACTOR_OR_EFFECT *)leader;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorStartScript(actor, SolSanctumElementalStarRoomAngleScript);
	actor = fdActorGet(ACTOR_KRADEN);
	actor->watchedActor = (ACTOR_OR_EFFECT *)leader;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorStartScript(actor, SolSanctumElementalStarRoomAngleScript);
	actor = fdActorGet(ACTOR_MENARDI);
	actor->watchedActor = (ACTOR_OR_EFFECT *)leader;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorStartScript(actor, SolSanctumElementalStarRoomAngleScript);
	actor = fdActorGet(ACTOR_SATUROS);
	actor->watchedActor = (ACTOR_OR_EFFECT *)leader;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorStartScript(actor, SolSanctumElementalStarRoomAngleScript);
	alex = fdActorGet(ACTOR_ALEX);
	alex->watchedActor = (ACTOR_OR_EFFECT *)leader;
	alex->steeringFlg |= ACTOR_FACES_MOTION;
	alex->scaleX = toFIXED(1);
	alex->scaleY = toFIXED(1);
	alex->motionFlg = fdActorGet(ACTOR_MENARDI)->motionFlg;
	alex->y.fixed = 0;
	fdActorStartScript(alex, SolSanctumElementalStarRoomAngleScript);
	actor = fdActorGet(ACTOR_FELIX);
	actor->watchedActor = (ACTOR_OR_EFFECT *)leader;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorStartScript(actor, SolSanctumElementalStarRoomAngleScript);
	fdScriptEnd();
}

/* Isaac searches the statue in the north west of the room (metatile (14, 8))
   and puts the Mars Star in a Mythril Bag, and the Sanctum starts to shake
   apart.

   The room flashes red, the star's glow goes out and the star rises over
   Isaac's head. Garet steps out beside him; a crash shakes the room, the two
   look about them as it shakes again, and jump in alarm as the screen fades.
   The party goes on in the eruption, the same room breaking up
   (SceneIdSolSanctumEruption). The statue can be searched only until then
   (FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED). */
static void scSolSanctumElementalStarRoomTakeMarsStar(void)
{
	Uint8  pass;
	Sint32 member;

	fdScriptStart();
	sySndPlay(SE_RUMBLE);
	for(pass = 0; pass != 6; pass++) {
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_BLEND | RGB(18, 14, 14), PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(8);
		fdScriptWait(8);
		if(pass == 1) {
			fdMapGridSetShake(1, 1, 1);
		}
	}
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	PUT_OUT_STAR_LIGHT(13, 6);
	fdScriptWait(20);
	member =
		scSolSanctumElementalStarRoomPresentStar(ITEM_MARS_STAR, 232, 16, 144);
	fdScriptWait(40);
	mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
	/* "...put the Mars Star in a Mythril Bag." */
	mnFieldMessageShow(MsgSolSanctumMarsStarBagged, 1);
	fdActorSetPos(ACTOR_JENNA, 307, 277);
	fdActorSetPos(ACTOR_KRADEN, 307, 277);
	fdActorSetPos(ACTOR_MENARDI, 307, 277);
	fdActorSetPos(ACTOR_SATUROS, 307, 277);
	fdActorSetPos(ACTOR_ALEX, 307, 277);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 232, 156);
	fdScriptWait(10);
	scPlaceAtLeader(ACTOR_GARET);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorWalkToAndWait(ACTOR_GARET, 218, 172);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	sySndPlay(SE_CRASH);
	fdMapGridSetShake(4, 4, 1);
	fdScriptWait(20);
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 50);
	sySndPlay(SE_CLATTER);
	fdMapGridSetShake(3, 3, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 50);
	fdMapGridSetShake(1, 1, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
	sySndPlay(SE_CLATTER);
	fdMapGridSetShake(3, 3, 1);
	fdScriptWait(30);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
	fdActorJump(ACTOR_GARET, 2, 20);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorJump(ACTOR_GARET, 6, 40);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION);
}

/* Kraden, on the middle platform, sends the boys for the stars: "Now, put the
   Elemental Stars in those bags and bring them here," and once the Venus Star
   is taken, "Good, good. Now go fetch the others!"

   Once Saturos holds him, he says only, "I am sorry, everyone... I am
   responsible for all of this." His Mind Read row shows that line at any time,
   though neither Isaac nor Garet knows Mind Read yet. */
static void scSolSanctumElementalStarRoomTalkToKraden(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
		fdScriptSetMessageId(MsgSolSanctumKradenIAmResponsible);
		/* "I am sorry, everyone... I am responsible for all of this." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
			fdScriptSetMessageId(MsgSolSanctumKradenBringTheStarsHere);
		else
			fdScriptSetMessageId(MsgSolSanctumKradenFetchTheOthers);
		fdActorFaceEachOther(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(10);
		/* "Good, good. Now go fetch the others!" */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
	}
	fdScriptEnd();
}

/* Jenna, on the middle platform, says "Be careful, both of you," and once the
   Venus Star is taken, teases "Kraden, you're as giddy as a schoolgirl."

   Once Saturos holds her, she wonders, "My brother is alive... but...why didn't
   he tell me sooner?" Her Mind Read row runs the same. */
static void scSolSanctumElementalStarRoomTalkToJenna(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX)) {
		fdScriptSetMessageId(MsgSolSanctumJennaMyBrotherIsAlive);
		/* "My brother is alive... but...why didn't he tell me sooner?" */
		fdScriptShowMessage(ACTOR_JENNA, 0);
	} else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) == 0)
			fdScriptSetMessageId(MsgSolSanctumJennaBeCareful);
		else
			fdScriptSetMessageId(MsgSolSanctumJennaKradenIsGiddy);
		fdActorFaceEachOther(ACTOR_JENNA, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(10);
		/* "Kraden, you're as giddy as a schoolgirl." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
	}
	fdScriptEnd();
}

/* Saturos, standing over the hostages, says "If you want to save them, you'll
   bring us the final Elemental Star." His Mind Read row shows the same. */
static void scSolSanctumElementalStarRoomTalkToSaturos(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgSolSanctumSaturosBringTheFinalStar);
	/* "If you want to save them, you'll bring us the final..." */
	fdScriptShowMessage(ACTOR_SATUROS, 0);
	fdScriptEnd();
}

/* Menardi, standing over the hostages, snaps "What are you doing? Bring us the
   final gem!" Her Mind Read row runs the same. */
static void scSolSanctumElementalStarRoomTalkToMenardi(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgSolSanctumMenardiBringTheFinalGem);
	/* "What are you doing? Bring us the final gem!" */
	fdScriptShowMessage(ACTOR_MENARDI, 0);
	fdScriptEnd();
}

/* Felix, unmasked beside the hostages, says nothing: "..." */
static void scSolSanctumElementalStarRoomTalkToFelix(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgSolSanctumFelixSilence);
	/* "..." */
	fdScriptShowMessage(ACTOR_FELIX, 0);
	fdScriptEnd();
}

/* Alex, holding the three stars, says "There is only one left. You're almost
   done." His Mind Read row shows the same. */
static void scSolSanctumElementalStarRoomTalkToAlex(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgSolSanctumAlexOnlyOneLeft);
	/* "There is only one left. You're almost done." */
	fdScriptShowMessage(ACTOR_ALEX, 0);
	fdScriptEnd();
}

/* Walking north from the middle platform into the beam of light the party came
   through (metatiles (29, 17) and (29, 18)) has Kraden call out "There aren't
   any Elemental Stars out there!", and Isaac stays in the room. */
static void scSolSanctumElementalStarRoomKradenStopsPartyLeaving(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgSolSanctumNoStarsOutThere);
	/* "There aren't any Elemental Stars out there!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdScriptEnd();
}

/* Isaac jumps across a gap to the next pillar when he walks off one of three
   edges: east off the Venus statue's pillar to the slender pillar beside it
   (metatile (45, 28)), east at metatile (15, 21) on the west side, or south off
   the middle platform (metatile (29, 22)).

   fdTryJumpForward makes the jump, two metatiles ahead. */
static void scSolSanctumElementalStarRoomJumpAhead(void)
{
	fdTryJumpForward();
}

/* Shows one of the Elemental Stars, Kraden beside it, while he describes it,
   then turns the view back to the party; the camera's point and Kraden's spot
   are in whole pixels. */
static inline void scSolSanctumElementalStarRoomShowElementalStar(
	Sint32 cameraX, Sint32 cameraZ, Sint32 actorX, Sint32 actorZ,
	Sint32 speaker)
{
	fdCameraMoveToFX(toFIXED(cameraX), CAMERA_KEEP, toFIXED(cameraZ), 1);
	fdCameraWaitForMove();
	fdActorSetPos(ACTOR_KRADEN, actorX, actorZ);
	fdScriptWait(40);
	fdScriptShowMessage(speaker, 0);
	fdActorSetPos(ACTOR_KRADEN, 471, 360);
	fdScriptWait(80);
	fdCameraMoveTo(471, CAMERA_KEEP, 370, 1);
	fdCameraWaitForMove();
}

/* The first time through the portal, Isaac floats down onto the middle platform
   with Garet and Jenna, Kraden follows, and he tells them of the Elemental
   Stars before sending the boys to fetch them with four Mythril Bags.

   Kraden names each star as the view goes to its statue, speaks of the Wisdom
   Stone, then steps out onto the stone and nearly slips ("Be careful, Kraden!
   It looks slippery."). Jenna asks Isaac to fetch the stars for him, and asks
   again until he says yes. Jenna leaves the party here and stays with
   Kraden. */
static void scSolSanctumElementalStarRoomEnterStarRoom(void)
{
	Uint16 frm;
	ACTOR *rising;
	SPR	  *spr;
	ACTOR *shadow;

	fdScriptStart();
	fdMapGridCopyMetatileAttributes(27, 16, 5, 1, 27, 17);
	fdCameraMoveTo(471, CAMERA_KEEP, 261, 0);
	fdCameraWaitForMove();
	fdMapGridCenterOnFollowedPoint();
	/* Materialize Isaac above the arrival platform with the expanding
	   shadow. */
	shadow = fdActorGet(ACTOR_PLATFORM_SHADOW);
	shadow->scaleX = toFIXED(1) / 10;
	shadow->scaleY = toFIXED(1) / 10;
	rising = fdActorGet(ACTOR_PARTY_LEADER);
	spr = rising->spr;
	spr->flg = 0;
	rising->scaleX = toFIXED(1) / 10;
	rising->scaleY = toFIXED(1) / 10;
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_BLINK);
	fdActorSetPos(ACTOR_PARTY_LEADER, 471, 290);
	rising->motionFlg = 0;
	rising->y.fixed = toFIXED(40);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetPos(ACTOR_PLATFORM_SHADOW, 471, 290);
	sySndPlay(SE_WARP);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	for(frm = 0; frm != 90; frm++) {
		rising->y.fixed -= toFIXED(1) / 10;
		rising->scaleX += toFIXED(1) / 100;
		rising->scaleY += toFIXED(1) / 100;
		shadow->scaleX += toFIXED(1) / 100;
		shadow->scaleY += toFIXED(1) / 100;
		fdScriptWait(1);
	}
	rising->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
	fdScriptWait(80);
	fdCameraSetSpeed(0.3);
	fdCameraMoveTo(471, CAMERA_KEEP, 290, 1);
	for(frm = 0; frm != 60; frm++) {
		rising->y.fixed -= toFIXED(1) / 2;
		fdScriptWait(1);
	}
	rising->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(20);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 1);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	spr->flg = SPRITE_FLAG_SHADOW;
	fdActorSetPos(ACTOR_PLATFORM_SHADOW, 0, 0);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 471, 310);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	scPlaceAtLeader(ACTOR_JENNA);
	scPlaceAtLeader(ACTOR_GARET);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorSetSpeed(ACTOR_JENNA, 0.5);
	fdActorWalkTo(ACTOR_JENNA, 453, 302);
	fdActorWalkToAndWait(ACTOR_GARET, 489, 302);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdActorJump(ACTOR_GARET, 2, 40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(40);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 471, 346);
	fdActorWalkTo(ACTOR_JENNA, 431, 338);
	fdActorWalkTo(ACTOR_GARET, 511, 338);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.3);
	fdActorSetSpeed(ACTOR_JENNA, 0.3);
	fdActorSetSpeed(ACTOR_GARET, 0.3);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 60);
	/* Materialize Kraden after the companions have explored the chamber. */
	rising = fdActorGet(ACTOR_KRADEN);
	spr = rising->spr;
	spr->flg = 0;
	rising->scaleX = toFIXED(1) / 10;
	rising->scaleY = toFIXED(1) / 10;
	shadow->scaleX = toFIXED(1) / 10;
	shadow->scaleY = toFIXED(1) / 10;
	fdActorSetSprColById(ACTOR_KRADEN, SPRITE_COLOR_BLINK);
	fdActorSetPos(ACTOR_KRADEN, 471, 290);
	rising->motionFlg = 0;
	rising->y.fixed = toFIXED(40);
	fdScriptWait(1);
	fdScriptSetMessageId(MsgSolSanctumKradenWhereAreWe);
	/* "Wha-Where are we?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorJump(ACTOR_JENNA, 4, 0);
	fdActorJump(ACTOR_GARET, 4, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(471, CAMERA_KEEP, 309, 1);
	fdCameraWaitForMove();
	fdActorSetPos(ACTOR_PLATFORM_SHADOW, 471, 290);
	sySndPlay(SE_WARP);
	fdActorSetSprPriority(ACTOR_KRADEN, 2);
	for(frm = 0; frm != 90; frm++) {
		rising->y.fixed -= toFIXED(1) / 10;
		rising->scaleX += toFIXED(1) / 100;
		rising->scaleY += toFIXED(1) / 100;
		shadow->scaleX += toFIXED(1) / 100;
		shadow->scaleY += toFIXED(1) / 100;
		fdScriptWait(1);
	}
	rising->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
	fdScriptWait(80);
	for(frm = 0; frm != 60; frm++) {
		rising->y.fixed -= toFIXED(1) / 2;
		fdScriptWait(1);
	}
	rising->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdScriptWait(30);
	fdActorSetSprPriority(ACTOR_KRADEN, 1);
	fdActorGet(ACTOR_KRADEN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprColById(ACTOR_KRADEN, SPRITE_COLOR_NORMAL);
	spr->flg = SPRITE_FLAG_SHADOW;
	fdActorSetPos(ACTOR_PLATFORM_SHADOW, 0, 0);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 471, 306);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_KRADEN, 2, 80);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(30);
	/* "Is that. . . the ocean?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 20);
	fdActorSetSpeed(ACTOR_KRADEN, 2.4);
	fdActorWalkToAndWait(ACTOR_KRADEN, 423, 306);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(40);
	fdActorWalkToAndWait(ACTOR_KRADEN, 519, 306);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	fdScriptWait(40);
	/* "No, it couldn't be... The ocean has waves." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 30);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "The ocean... What's that?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	/* "The ocean is a thing of endless water at the end of the world." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "You would not understand... Someday, I'll show it to you myself." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 10);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(40);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(533, CAMERA_KEEP, 339, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 0.5);
	fdActorWalkToAndWait(ACTOR_KRADEN, 533, 339);
	fdCameraWaitForMove();
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 60);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(30);
	fdCameraFollowActor(ACTOR_KRADEN, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorWalkTo(ACTOR_KRADEN, 455, 360);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 455, 360);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_KRADEN, 471, 360);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(30);
	/* "The Elemental Stars..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(20);
	/* "They really exist... The Elemental Stars exist!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 30);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkToAndWait(ACTOR_JENNA, 440, 346);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 10);
	/* "You taught us about those..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorWalkToAndWait(ACTOR_GARET, 495, 346);
	fdCameraWaitForMove();
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
	/* "Stones, wood, even us... All things are made from those four elements.
	   Did I get it right, Isaac?" Yes: Kraden, "Exactly..." No: Kraden, "No,
	   Isaac... Garet is quite correct." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 30);
	/* Kraden explains the elements and the stars at the room's four corners. */
	fdScriptSetMessageId(MsgSolSanctumKradenTheElementsAreThe);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "As I have told you many times, the elements are the source of..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Earth, water, fire, and wind-these are the four elements." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "Legend holds that the four Elemental Stars contain the purified..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "And now, those legendary stones are before us!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_GARET);
	fdCameraSetSpeed(8);
	/* Kraden tours the four Elemental Stars, the view returning to the party
	   after each. "The Venus Star... The elemental stone of earth!" */
	scSolSanctumElementalStarRoomShowElementalStar(
		711, 464, 589, 353, SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
	/* "Mercury Star, possessing the power of water's many forms!" */
	scSolSanctumElementalStarRoomShowElementalStar(
		711, 147, 596, 238, SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(10);
	fdActorJump(ACTOR_KRADEN, 4, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	/* "There is the Mars Star, glowing red with the fire contained within!" */
	scSolSanctumElementalStarRoomShowElementalStar(
		231, 147, 308, 250, SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdScriptWait(10);
	fdActorJump(ACTOR_KRADEN, 6, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	/* "And Jupiter Star, bound with the wind's fury!!!" */
	scSolSanctumElementalStarRoomShowElementalStar(
		231, 464, 306, 362, SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	/* "Kraden... Why are you getting so excited?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorJump(ACTOR_KRADEN, 4, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "How can you be so calm in the face of such wonders!?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	/* "You're looking at the very source of all Alchemy!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "...Haven't I told you all this before?" Yes: Garet, "What? I must have
	   fallen asleep, then, because I don't have a clue what he's talking
	   about!" No: Garet, "Then this must be a lot for you to take in." */
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 10);
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 10);
	/* Discuss the Wisdom Stone and the power stored in the Elemental Stars. */
	fdScriptSetMessageId(MsgSolSanctumKradenTheWisdomStone);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	fdActorJump(ACTOR_KRADEN, 4, 40);
	/* "The Wisdom Stone... the Stone of Sages." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "This stone dominates everything..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
	/* "You understand now, don't you?" Yes: "The study of Alchemy began with
	   the search for this very stone." No: "You still do not understand?
	   Alchemy exists because of the Elemental Stars." */
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
	fdScriptSetMessageId(MsgSolSanctumTheStarsHaveEnormousPower);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "I don't really get how it works, but they have enormous power..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdActorJump(ACTOR_KRADEN, 4, 40);
	/* "Beyond enormous... With a single gem, one could conquer the..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	/* "Now, perhaps, you begin to understand." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorJump(ACTOR_GARET, 4, 40);
	/* "Don't scare us like that, Kraden..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 80);
	/* "Who can say what the truth is?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 40);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	/* "That's why I've been searching for the Elemental Stars..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 40);
	rising = fdActorGet(ACTOR_KRADEN);
	/* Kraden steps out towards the stars and nearly slips, then asks the boys
	   to help. */
	sySynch(6);
	rising->speedLimit = toFIXED(3);
	rising->acceleration = toFIXED(2);
	sySndPlay(SE_HIGH_JUMP);
	rising->velY = toFIXED(6);
	fdActorMoveToAndWait(ACTOR_KRADEN, 471, 395);
	sySynch(6);
	fdActorSetSpeed(ACTOR_KRADEN, 0.3);
	rising->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_KRADEN, 473, 395);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorSetDestination(ACTOR_KRADEN, 469, 395);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorSetDestination(ACTOR_KRADEN, 471, 395);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	/* "Now they lie before me..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 471, 411);
	fdActorSetSpeed(ACTOR_KRADEN, 0.3);
	rising->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_KRADEN, 474, 411);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetDestination(ACTOR_KRADEN, 468, 411);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetDestination(ACTOR_KRADEN, 471, 411);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	/* "Whoa..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_KRADEN, 10);
	fdActorSetSpeed(ACTOR_KRADEN, 0.2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 471, 395);
	fdActorSetMovementTypeById(ACTOR_KRADEN, MOVEMENT_TYPE_STAND);
	fdScriptWait(30);
	rising->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
	sySynch(6);
	rising->speedLimit = toFIXED(3);
	rising->acceleration = toFIXED(2);
	sySndPlay(SE_HIGH_JUMP);
	rising->velY = toFIXED(6);
	fdActorMoveToAndWait(ACTOR_KRADEN, 471, 360);
	sySynch(6);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	/* "Be careful, Kraden! It looks slippery." */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "But I must examine the Elemental Stars..." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 30);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 30);
	/* "Make the boys do it!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 20);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorJump(ACTOR_GARET, 4, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	fdScriptWait(20);
	/* "Oh, I couldn't ask . . . Would you mind?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 30);
	/* "He'd be glad to get the stones for you, right, Isaac?" */
	fdScriptOpenMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	/* Jenna keeps asking until Isaac agrees to collect the stars. */
	while(fdScriptAskYesNoWithActorAnimation(0, 0) != 0) {
		fdScriptSetMessageId(MsgSolSanctumJennaWontYouPlease);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
		fdActorJump(ACTOR_JENNA, 4, 60);
		/* "Don't make me ask you again! Won't you please do it?" */
		fdScriptOpenMessage(ACTOR_JENNA, 0);
	}
	fdScriptSetMessageId(MsgSolSanctumJennaThankYou);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 30);
	/* "Thank you!" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
	fdActorSetSpeed(ACTOR_KRADEN, 0.2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 471, 352);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	/* "You'll need this, Isaac." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 60);
	rising->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_KRADEN, 456, 360);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgSolSanctumTheBagsAreFromKradens);
	/* "You brought those from your cottage, right, Kraden?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_JENNA, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "What are those bags?" */
	scSolSanctumElementalStarRoomShowMessageAndWait(ACTOR_GARET, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "They're made of woven threads of Mythril." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "I prepared them for just such an occasion." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdScriptWait(10);
	/* "Now, go fetch the Elemental Stars and put them in those bags." */
	scSolSanctumElementalStarRoomShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 30);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_GARET, 1);
	scWalkIntoLeader(ACTOR_GARET);
	gmFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
	fdPartyRemoveMemberRestored(JENNA);
	scSolSanctumElementalStarRoomGiveMythrilBags();
	fdMapGridCopyMetatileAttributes(8, 0, 5, 1, 27, 17);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	fdScriptEnd();
}

/* Sets up the Elemental Star Room. */
Sint32 scInit(void)
{
	Uint32 lightActorId;

	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	gmFlagSet(FLAG_RETREAT_BLOCKED);
	lightActorId = ACTOR_FIRST_FLOATING_LIGHT;
	do {
		fdActorGet(lightActorId)->collisionFlg = 0;
		fdActorSetSprPriority(lightActorId++, 1);
	} while(lightActorId <= ACTOR_LAST_FLOATING_LIGHT);
	/* It asks for 16 lights; the event keeps the ten the room places. */
	scSolSanctumStartFloatingLights(ACTOR_FIRST_FLOATING_LIGHT, 16);
	if(fdScriptGetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) != 0) {
		fdActorSetPos(ACTOR_KRADEN, 456, 360);
		fdActorSetPos(ACTOR_JENNA, 440, 346);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED) != 0) {
		PUT_OUT_STAR_LIGHT(43, 26);
		DRAW_RISEN_PILLAR(41, 29);
		DRAW_RISEN_PILLAR(19, 29);
		DRAW_RISEN_PILLAR(41, 10);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED) != 0) {
		PUT_OUT_STAR_LIGHT(43, 6);
		DRAW_RISEN_PILLAR(29, 4);
		DRAW_RISEN_PILLAR(21, 21);
		DRAW_RISEN_PILLAR(21, 29);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX) != 0) {
		PUT_OUT_STAR_LIGHT(13, 26);
		DRAW_RISEN_PILLAR(10, 31);
		DRAW_RISEN_PILLAR(24, 9);
		DRAW_RISEN_PILLAR(36, 10);
		scSolSanctumElementalStarRoomStandAroundHostages();
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED) == 0) {
		if(MainRoutineStateWork.entrance ==
			SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
			scSolSanctumElementalStarRoomEnterStarRoom();
		}
	}
	return 0;
}

/* Shows the script's next line at the speaker, then waits the ticks. */
static void scSolSanctumElementalStarRoomShowMessageAndWait(
	Sint32 speaker, Sint32 ticks)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(ticks);
}

/* Animates a star's icon and replaces its owner's mythril bag with the star. */
static Sint32 scSolSanctumElementalStarRoomPresentStarFX(
	Sint32 item, FIXED x, FIXED y, FIXED z)
{
	ICON_TRANSFER *glyph;
	ACTOR		  *actor;
	SPR			  *spr;
	Sint32		   member;
	Sint32		   slot;

	glyph = NULL;
	actor = syFieldActorCreate(SPRITE_BLANK, x, y, z);
	member = gmPartyInventoryFindMember(ITEM_MYTHRIL_BAG);
	slot = gmInventoryFind(member, ITEM_MYTHRIL_BAG);
	if(actor == NULL)
		return member;
	fdActorStartScript(actor, ItemIconGrowScript);
	spr = actor->spr;
	spr->flg = 0;
	spr->cnt = 0;
	spr->image.fullCol = 0;
	spr->image.pal = 0;
	actor->velY = toFIXED(5) / 2;
	actor->gravity = toFIXED(1) / 4;
	glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(*glyph));
	mnIconDrawItem(item);
	syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	sySndPlay(MUS_TREASURE_FOUND);
	fdItemDispShowPicture(actor, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	gmInventoryDiscard(member, slot);
	gmInventoryAddItem(member, item);
	fdActorRelease(actor);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	return member;
}

/* Makes room among the items and gives the party four Mythril Bags, keeping the
   dialogue's place. */
static void scSolSanctumElementalStarRoomGiveMythrilBags(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint16		 savedMsg = state->msg;
	Sint32		 itemSlot;
	Sint32		 itemOwner;
	Sint32		 freeSlots;

	sySndPlay(MUS_TREASURE_FOUND);
	gmItemShowFound(ITEM_MYTHRIL_BAG, 3);
	/* "Isaac got four Mythril Bags." */
	mnFieldMessageShow(MsgSolSanctumGotFourMythrilBags, 1);
	do {
		freeSlots = 2 * INVENTORY_SLOTS - gmInventoryCount(ISAAC);
		freeSlots -= gmInventoryCount(GARET);

		if(freeSlots <= 3) {
			/* "You have too many items. Will you drop something?" */
			mnFieldMessageShow(MsgSolSanctumTooManyItems, 1);
			if(mnItemPickerRun(&itemOwner, &itemSlot) != -1)
				gmInventoryDiscard(itemOwner, itemSlot);
		}
	} while(freeSlots <= 3);
	gmPartyInventoryAdd(ITEM_MYTHRIL_BAG);
	gmPartyInventoryAdd(ITEM_MYTHRIL_BAG);
	gmPartyInventoryAdd(ITEM_MYTHRIL_BAG);
	gmPartyInventoryAdd(ITEM_MYTHRIL_BAG);
	state->msg = savedMsg;
}

/* Alex warps in where he has been placed, hidden. */
static void scSolSanctumElementalStarRoomAlexWarpsIn(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *alex;
	Uint32		   frm;
	FIXED		   x;
	FIXED		   xOfs;

	alex = fdActorGet(ACTOR_ALEX);
	sySndPlay(SE_WARP);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
	options.pri = 1;
	options.pal = SPRITE_COLOR_BRG;
	options.type = SPRITE_PARTICLE;
	options.startScaleX = toFIXED(2) / 5;
	options.startScaleY = toFIXED(3);
	for(frm = 0; frm < WARP_TICKS; frm++) {
		fdScriptWait(1);
		if(!(frm & 1)) {
			xOfs = toFIXED(syRandomBelow(24));
			x = alex->x.fixed + xOfs;
			x -= toFIXED(12);
			fdEffectSpawn(x,
				alex->y.fixed + toFIXED(syRandomBelow(32)) + toFIXED(32),
				alex->z.fixed, 0, -toFIXED(4), 0,
				EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
					EFFECT_USE_PALETTE,
				&options);
		}
		if(frm == WARP_IN_BLINK_TICK)
			fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
	}
	fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

#include "shared/sol_sanctum_elemental_star_lights.c"

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Sint32 SolSanctumElementalStarRoomAngleScript[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/item_icon_grow_script.c"

#include "shared/sol_sanctum_star_room_entrances.c"

Uint32 SolSanctumElementalStarRoomExits[] = {
	SCENE_EXITS(SolSanctumElementalStarRoom),
	SCENE_EXIT(SOL_SANCTUM_ELEMENTAL_STAR_ROOM_TO_SOL_SANCTUM_ERUPTION,
		SolSanctumEruption,
		SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE SolSanctumElementalStarRoomActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX_MASKED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		617, 0, 408, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		429, 0, 221, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		696, 0, 113, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		170, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		308, 0, 511, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		397, 0, 418, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		542, 0, 269, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		421, 0, 117, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		295, 0, 210, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		742, 0, 310, FACING_EAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE SolSanctumElementalStarRoomCues[] = {
	{ CUE_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToKraden) },
	{ CUE_TALK, ACTOR_JENNA, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToJenna) },
	{ CUE_TALK, ACTOR_SATUROS, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToSaturos) },
	{ CUE_TALK, ACTOR_MENARDI, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToMenardi) },
	{ CUE_TALK, ACTOR_FELIX, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToFelix) },
	{ CUE_TALK, ACTOR_ALEX, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToAlex) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KRADEN, CONDITION_ALWAYS, MSGNO(MsgSolSanctumKradenIAmResponsible) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNA, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToJenna) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgSolSanctumSaturosBringTheFinalStar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MENARDI, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomTalkToMenardi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgSolSanctumAlexOnlyOneLeft) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_WAY_BACK, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomKradenStopsPartyLeaving) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_EAST, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomJumpAhead) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUMP_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scSolSanctumElementalStarRoomJumpAhead) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_VENUS_STATUE, FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED, SCRIPT_REF(scSolSanctumElementalStarRoomBagVenusStar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MERCURY_STATUE, FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED, SCRIPT_REF(scSolSanctumElementalStarRoomBagMercuryStar) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_JUPITER_STATUE, FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX, SCRIPT_REF(scSolSanctumElementalStarRoomBagJupiterStarAndMeetSaturos) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_MARS_STATUE, FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED, SCRIPT_REF(scSolSanctumElementalStarRoomTakeMarsStar) },
	{ CUE_TABLE_END },
};

#include "shared/sol_sanctum_star_metatiles.c"

#include "shared/sol_sanctum_elemental_star_light_tables.c"
/* clang-format on */
