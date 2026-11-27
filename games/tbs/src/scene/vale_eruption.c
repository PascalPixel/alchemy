/* @file */
/* Vale (SceneId_ValeEruption) while Mt. Aleph erupts is the ground before the
 */
/* sanctum's door, where the villagers wait for Isaac and Garet. */
/* */
/* It stands on the village's own layout (SceneId_Vale's). */
/* */
/* The scene falls between Sol Sanctum's eruption (sol_sanctum_eruption.c), */
/* where Jenna and Kraden are carried off, and the party's journey. The path */
/* down the mountain leads here, by VALE_ERUPTION_FROM_SOL_ENTRANCE */
/* (sol_sanctum_entrance.c's SOL_ENTRANCE_TO_VALE after the eruption). */
/* */
/* Gathered before the sanctum are the Great Healer, Garet's grandfather the */
/* mayor, Isaac's mother Dora, Garet's sister and brother, and the guard who */
/* saw the two hanging around on his watch. */
/* */
/* The scene has no triggers. scInit plays the whole event on */
/* arrival by VALE_ERUPTION_FROM_SOL_ENTRANCE */
/* (scValeEruptionReturnFromMtAleph). */
/* */
/* - The ground shakes, and the villagers wonder where the two are. */
/* - Isaac and Garet come down the path and are called over. */
/* - Dora's question about Jenna and Kraden is answered the same way whatever */
/*   Isaac replies. */
/* - The Great Healer then opens the sanctum's door (fdMapGridAnimateCells) and
 */
/*   leads everyone in. */
/* - The scene's one exit takes the party into the Vale Sanctum */
/*   (SANCTUM_FROM_VALE_ERUPTION), where the Great Healer passes on the Wise */
/*   One's quest (Sanctum_RequestQuestAcceptance). */
/* */
/* The tremors are fdMapGridSetShake calls. Each comes with a flash */
/* (fdWeatherLastLightning) and SE_CRASH, over a burst of red flashes on the */
/* dimmed colours (fdWeatherStartLightningFlash). */
/* */
/* The scene sets FLAG_VALE_RETURNED_FROM_MT_ALEPH but never reads it. Sol */
/* Sanctum's entrance scene does (sol_sanctum_entrance.c). */

#include "flags.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern s32 ValeEruption_LeaderWalksDown[];
extern s32 ValeEruption_GaretWalksDown[];
extern s32 ValeEruption_EnterSanctum[];
extern SCENE_ENTRANCE ValeEruption_Entrances[];
extern u32 ValeEruption_Exits[];
extern ACTOR_PLACE ValeEruption_ActorPlacements[];
extern TRIGGER ValeEruption_Triggers[];
extern u16 ValeEruption_SanctumDoorOpens[];

/* The placed actors, numbered in the order of ValeEruption_ActorPlacements. */
/* Garet's row places the party's Garet and takes no number. */
typedef enum VALE_ERUPTION_ACTOR_ID {
	/* The Great Healer is a man with a white beard, in a tall hat and a long */
	/* purple and blue robe. He has the sprite of sanctum.c's healer, and leads
	 */
	/* the villagers into the sanctum. "Come into the sanctum, both of you." */
	ACTOR_GREAT_HEALER = ACTOR_FIRST_PLACED,
	/* Garet's grandfather is an old man with white hair and a cane. He is the
	 */
	/* mayor, whom the Great Healer calls Mayor in sanctum.c. "Oooh, my
	   grandson! */
	/* Will Garet be safe in this eruption?!?" */
	ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED + 1,
	/* Isaac's mother, who is Dora in sanctum.c. "Isaac and Garet went to Mr. */
	/* Kraden's house." */
	ACTOR_DORA = ACTOR_FIRST_PLACED + 2,
	/* Garet's sister is a young woman with an orange ponytail, whom Garet calls
	 */
	/* Sis at the farewell (vale_plaza.c). She calls the mayor Grandpa, and */
	/* ACTOR_GARETS_BROTHER calls her Sis. "Grandpa, we don't even know if he's
	 */
	/* up there!" */
	ACTOR_GARETS_SISTER = ACTOR_FIRST_PLACED + 3,
	/* Garet's brother is the small boy with orange hair of vale.c's */
	/* ACTOR_GARETS_BROTHER. He says someone saw Garet climb, and is first to
	   cry */
	/* "Garet!" */
	ACTOR_GARETS_BROTHER = ACTOR_FIRST_PLACED + 4,
	/* The guard is the man with long brown hair and a beard, in a long robe,
	   who */
	/* guards Mt. Aleph in vale.c (ACTOR_MAN_GUARDING_MT_ALEPH). "They were */
	/* hanging around here while I was on guard duty." */
	ACTOR_GUARD = ACTOR_FIRST_PLACED + 5,
	/* The first of the six villagers gathered behind them. They are a burly */
	/* grey-haired man, a bald muscular man, a woman in a green dress, a little
	 */
	/* girl with pigtails, a young man with brown hair and a strongman. */
	ACTOR_FIRST_VILLAGER = ACTOR_FIRST_PLACED + 6
} VALE_ERUPTION_ACTOR_ID;

static void scValeEruptionReturnFromMtAleph(void);

/* Returns where the party appears. VALE_ERUPTION_FROM_SOL_ENTRANCE is the path
 */
/* down from the mountain, Mt. Aleph. */
const SCENE_ENTRANCE *scGetEntrances(void) { return ValeEruption_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

/* Returns the one exit, VALE_ERUPTION_TO_SANCTUM into the Vale Sanctum */
/* (SANCTUM_FROM_VALE_ERUPTION), which scValeEruptionReturnFromMtAleph takes. */
const u32 *scGetExits(void) { return ValeEruption_Exits; }

/* Returns Garet and the villagers gathered before the sanctum. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	return ValeEruption_ActorPlacements;
}

/* The scene has no triggers of its own, since everything happens on arrival. */
const TRIGGER *scGetTriggers(void) { return ValeEruption_Triggers; }

/* Arriving in Vale during the eruption plays the return from the mountain, */
/* when the party comes by VALE_ERUPTION_FROM_SOL_ENTRANCE. */
s32 scInit(void) {
	if(MainRoutineStateWork.entrance == VALE_ERUPTION_FROM_SOL_ENTRANCE) {
		scValeEruptionReturnFromMtAleph();
	}
	return 0;
}

/* In Vale during the eruption, the volcano shakes the ground before the */
/* sanctum while the villagers wonder where Isaac and Garet are. */
static void scValeEruptionReturnFromMtAleph(void) {
	FIELD_STATE *state;
	const s32 *actionScript;

	fdEventStart();
	fdActorSetAnimation(ACTOR_FIRST_VILLAGER, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_VILLAGER + 1, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_VILLAGER + 2, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_VILLAGER + 3, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_VILLAGER + 4, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_VILLAGER + 5, ANIM_STILL);
	fdActorWalkToAndWait(ACTOR_GARETS_SISTER, 265, 487);
	fdActorFaceDirection(ACTOR_GARETS_SISTER, FACING_NORTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_GARETS_BROTHER, 256, 500);
	fdActorFaceDirection(ACTOR_GARETS_BROTHER, FACING_NORTHWEST, 0);
	/* A lightning flash, then the screen opens on the villagers below. */
	fdWeatherStartLightningFlash(PALETTE_TINT_DIM, PALETTE_TINT_RED);
	fdWeatherLastLightning();
	sySynch(60);
	fdCameraMoveTo(256, CAMERA_KEEP, 612, 0);
	fdCameraWaitForArrival();
	fdMapGridCenterOnFollowedPoint();
	state = FieldStateWork;
	state->transition = 0;
	state->transitionFrm = 32;
	fdEventStartScrTransition();

	/* The camera pans up to the villagers as the ground shakes. */
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(256, CAMERA_KEEP, 500, 1);
	fdEventWait(20);
	fdMapGridSetShake(1, 2, 1);
	fdWeatherLastLightning();
	sySndPlay(SE_CRASH);
	fdEventWait(30);
	fdWeatherLastLightning();
	sySndPlay(SE_CRASH);
	fdCameraWaitForArrival();
	fdMapGridSetShake(2, 3, 1);
	fdWeatherLastLightning();
	sySndPlay(SE_CRASH);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdEventWait(60);

	/* The villagers worry about Isaac and Garet, last seen going to Kraden's */
	/* house. */
	fdEventSetMessageId(MsgValeMtAlephWasInactive);
	fdActorShowEmote(ACTOR_GREAT_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(60);
	/* "I thought Mt. Aleph was inactive..." */
	fdEventShowMessage(ACTOR_GREAT_HEALER, 0);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(30);
	/* The mayor: "Oooh, my grandson!" */
	fdEventShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
	fdEventWait(30);
	/* Garet's sister doubts he is up there; her brother says someone saw him */
	/* climb. */
	fdActorSetAnimationAndWait(ACTOR_GARETS_SISTER, ANIM_SHAKE_HEAD);
	/* "Grandpa, we don't even know if he's up there!" */
	fdEventShowMessage(ACTOR_GARETS_SISTER, 0);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceActor(ACTOR_GARETS_BROTHER, ACTOR_GARETS_SISTER, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARETS_BROTHER, ANIM_SHAKE_HEAD);
	/* "But, Sis, someone saw him and his friends climbing Mt. Aleph." */
	fdEventShowMessage(ACTOR_GARETS_BROTHER, 0);
	/* The guard saw them hanging around; Dora says they went to Kraden's house,
	 */
	/* and the mayor and Dora wonder what Alchemy has to do with the eruption.
	 */
	fdActorRunRepeatedMotion(ACTOR_GUARD, 1);
	/* "They were hanging around here while I was on guard duty." */
	fdEventShowMessage(ACTOR_GUARD, 0);
	fdActorFaceActor(ACTOR_DORA, ACTOR_GUARD, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	/* "Isaac and Garet went to Mr. Kraden's house." */
	fdEventShowMessage(ACTOR_DORA, 0);
	fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_DORA, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
	/* "Didn't they say they were studying Alchemy?" */
	fdEventShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
	fdActorFaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	/* "Yes, they did...but... what does that have to do with the..." */
	fdEventShowMessage(ACTOR_DORA, 0);
	fdEventWait(60);
	fdMapGridSetShake(2, 3, 1);
	fdWeatherLastLightning();
	sySndPlay(SE_CRASH);
	fdEventWait(60);
	fdActorTurnToFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
	fdActorTurnToFaceEachOther(ACTOR_DORA, ACTOR_GARETS_SISTER, 0);
	fdActorTurnToFaceEachOther(ACTOR_GARETS_BROTHER, ACTOR_GUARD, 0);
	fdActorStartRepeatedMotion(ACTOR_GREAT_HEALER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdActorStartRepeatedMotion(ACTOR_GARETS_SISTER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARETS_BROTHER, 2);
	fdActorStartRepeatedMotion(ACTOR_GUARD, 2);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();

	/* Isaac and Garet come down the path, and Garet sees the villagers: "Uh-oh.
	 */
	/* It's Grandpa and the others." */
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorSetPos(ACTOR_PARTY_LEADER, 120, 258);
	fdCameraSetSpeed(1.5);
	fdCameraMoveTo(112, CAMERA_KEEP, 320, 1);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 120, 320);
	fdActorMoveToAndWait(ACTOR_GARET, 104, 320);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdCameraWaitForArrival();
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdEventWait(50);
	fdActorSetSpeed(ACTOR_GARET, 1.5);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_GARET, 105, 342);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	/* "Uh-oh. It's Grandpa and the others." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdEventWait(20);

	/* "What should we do, Isaac? Should we go back up?" Either way, the two
	 * stay. */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(60);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdEventWait(50);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
		fdActorSetSpeed(ACTOR_GARET, 1);
		fdActorMoveToAndWait(ACTOR_GARET, 103, 320);
		fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	}
	else {
		fdEventWait(60);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdEventWait(50);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 120, 340);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	}

	/* Garet's brother spots them first, and the villagers turn to the path. */
	/* "Garet!" */
	fdEventShowMessage(ACTOR_GARETS_BROTHER, 0);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARETS_SISTER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARETS_BROTHER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GUARD, FACING_NORTHWEST, 0);
	/* The camera flies to Dora and the Great Healer. */
	fdCameraSetSpeed(3);
	fdCameraMoveToActorById(ACTOR_DORA, 1);
	fdCameraWaitForArrival();
	fdEventWait(50);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	/* "Isaac!" */
	fdEventShowMessage(ACTOR_DORA, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
	/* "You're safe!" */
	fdEventShowMessage(ACTOR_GREAT_HEALER, 0);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
	/* "Come over here, both of you!" */
	fdEventShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdCameraMoveTo(112, CAMERA_KEEP, 320, 1);
	fdCameraWaitForArrival();
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorWaitForScriptEndById(ACTOR_GARET);
	fdEventWait(50);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorWaitForScriptEndById(ACTOR_GARET);
	fdEventWait(60);

	/* The two walk down to the villagers. */
	fdCameraSetSpeed(1);
	fdCameraMoveTo(214, CAMERA_KEEP, 472, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValeEruption_LeaderWalksDown);
	fdEventWait(30);
	fdActorSetScript(ACTOR_GARET, ValeEruption_GaretWalksDown);
	fdActorWaitForScriptEndById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
	fdActorSetSpeed(ACTOR_GREAT_HEALER, 0.8);
	fdActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_GREAT_HEALER, 265, 455);
	fdActorMoveToAndWait(ACTOR_GREAT_HEALER, 246, 455);
	fdActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_STAND);
	fdEventWait(30);

	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
	/* "Where have you been?" */
	fdEventShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(50);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(50);
	fdActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
	/* "What happened?" */
	fdEventShowMessage(ACTOR_GREAT_HEALER, 0);
	fdEventWait(40);
	/* The two look at each other and shake their heads. */
	fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdEventWait(40);
	/* Dora notices who is missing. */
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdEventWait(10);

	/* "Weren't Jenna and Kraden with you?" Yes: "They were taken away? By */
	/* whom?"; no: "What do you mean, they were taken away?" */
	fdEventOpenMessage(ACTOR_DORA, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		/* The mayor answers either reply; the other reply is skipped. */
		fdEventWait(40);
		fdActorTurnToFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER,
								   0);
		fdEventWait(50);
		fdActorFaceDirection(ACTOR_GREAT_HEALER, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
		fdEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
		/* "They were taken away? By whom?" */
		fdEventShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
		fdEventSkipMessages(1);
	}
	else {
		fdEventWait(40);
		fdActorTurnToFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER,
								   0);
		fdEventWait(50);
		fdActorFaceDirection(ACTOR_GREAT_HEALER, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
		fdEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
		fdEventSkipMessages(1);
		/* "What do you mean, they were taken away?" */
		fdEventShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
	}
	fdEventWait(30);
	/* Garet starts to tell, and both shake their heads again. */
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdEventWait(30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdEventWait(60);

	/* The Great Healer sends the two into the sanctum to tell what happened. */
	fdActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
	/* "This is all very complicated." */
	fdEventShowMessage(ACTOR_GREAT_HEALER, 0);
	fdActorFaceActor(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
	fdEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_GREAT_HEALER, ANIM_NOD);
	fdEventWait(30);
	/* "We'll need you to tell us exactly what happened on the mountain." */
	fdEventShowMessage(ACTOR_GREAT_HEALER, 0);
	fdEventWait(20);
	/* The mayor agrees with a nod; the Great Healer sends them inside. */
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
	fdEventWait(50);
	fdActorFaceDirection(ACTOR_GREAT_HEALER, FACING_WEST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
	/* "Come into the sanctum, both of you." */
	fdEventShowMessage(ACTOR_GREAT_HEALER, 0);
	fdEventWait(40);
	fdActorTurnToFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
	fdEventWait(30);

	/* The sanctum's door opens and everyone files inside. */
	fdActorWalkToAndWait(ACTOR_GREAT_HEALER, 255, 445);
	fdEventWait(40);
	fdMapGridAnimateCells(ValeEruption_SanctumDoorOpens, 45, 11);
	sySndPlay(SE_THUMP);
	fdEventWait(30);
	fdActorWalkTo(ACTOR_GREAT_HEALER, 255, 390);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_GARETS_GRANDFATHER, 0.8);
	fdActorSetSpeed(ACTOR_DORA, 0.8);
	fdActorWalkTo(ACTOR_GARETS_GRANDFATHER, 255, 390);
	fdActorWalkToAndWait(ACTOR_DORA, 255, 460);
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 0);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdEventWait(30);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdEventWait(40);
	fdActorWalkTo(ACTOR_DORA, 255, 390);
	actionScript = ValeEruption_EnterSanctum;
	fdActorSetScript(ACTOR_PARTY_LEADER, actionScript);
	fdEventWait(40);
	fdActorSetScript(ACTOR_GARET, actionScript);
	fdActorWaitForScriptEndById(ACTOR_GARET);
	fdActorShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARETS_BROTHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdEventWait(40);
	fdMapGridSetShake(2, 3, 1);
	fdWeatherLastLightning();
	sySndPlay(SE_CRASH);
	fdEventWait(30);
	state = FieldStateWork;
	state->transition = 0;
	state->transitionFrm = 64;
	fdEventFinishScrTransition();
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagSet(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
	syImportEventSetExitRequest(VALE_ERUPTION_TO_SANCTUM);
	fdEventEnd();
}

/* Isaac walks down from the path to the villagers before the sanctum. */
s32 ValeEruption_LeaderWalksDown[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(128, 0, 378),
	SC_SYNC,
	SC_MOVE(203, 0, 378),
	SC_SYNC,
	SC_MOVE(212, 0, 472),
	SC_SYNC,
	SC_MOVE(214, 0, 472),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Garet follows him, stopping a little above. */
s32 ValeEruption_GaretWalksDown[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(128, 0, 378),
	SC_SYNC,
	SC_MOVE(203, 0, 378),
	SC_SYNC,
	SC_MOVE(214, 0, 455),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Isaac, then Garet, walk through the sanctum's door. */
s32 ValeEruption_EnterSanctum[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(255, 0, 452),
	SC_SYNC,
	SC_MOVE(255, 0, 390),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

SCENE_ENTRANCE ValeEruption_Entrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 110, 0, 270, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{VALE_ERUPTION_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 103, 0, 284,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 ValeEruption_Exits[] = {
	SCENE_EXITS(ValeEruption),
	SCENE_EXIT(VALE_ERUPTION_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE_ERUPTION),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE ValeEruption_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 258, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        265, 0, 468, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 468, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 487, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        265, 0, 477, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 490, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        265, 0, 523, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        245, 0, 532, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 543, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        283, 0, 536, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        235, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        265, 0, 561, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        247, 0, 573, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
/* clang-format on */

TRIGGER ValeEruption_Triggers[] = {
	{TRIGGER_TABLE_END},
};

/* The sanctum's door, two cells wide, opening in two steps. */
u16 ValeEruption_SanctumDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
