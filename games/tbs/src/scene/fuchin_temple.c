/* Fuchin Temple's grounds, and the rooms map that shares its overlay.

   The game packs rooms of several places that share a tile style onto one map.
   Here that is the inside of Fuchin Temple with two masters' rooms in other
   areas, Master Feh's school in Xian and Master Hama's room in Lama Temple.

   Two scenes hold four areas, by their area names. In each, story flags pick
   where the people stand and which script plays on entry.

   - Fuchin Temple (SceneId_FuchinTempleGrounds), outside: a path from the
	 World Map in the south (FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP) climbs
	 beside a pool and waterfall to the temple, a blue-roofed hall at the top,
	 with a traveling healer by its door and a Psynergy Stone to its east. A
	 defeat anywhere here returns the party to
	 FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, in front of the healer.

	 A monk stands on a tall stump in the pool, in front of the falls,
	 guarding the grotto behind them. Once Nyunpa has set his test
	 (FLAG_NYUNPA_TEST_ACCEPTED) the monk leaps aside to the next stump
	 (FLAG_FUCHIN_FALLS_GROTTO_OPENED), and the grotto leads into Fuchin Falls
	 Cave (FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE).

	 Back out with the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), he
	 sends the party on to Nyunpa (FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST).

   - Lacquer rooms A (SceneId_LacquerRoomsA), a rooms map drawn in red
	 lacquer. Each is a camera box of its own, chosen by the entrance the
	 party comes in by. In story order, by the exits that lead in:
	 - Fuchin Temple (by LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS or the
	   default entrance), inside, where Nyunpa meditates on a dais with the
	   sign of yin and yang.
	   - Mind Read on him offers the test (scFuchinTempleNyunpaOffersTest,
		 again after FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED).
	   - Back with the Orb, scInit plays his lesson on Ki
		 (scFuchinTempleNyunpaTeachesKi, FLAG_NYUNPA_TAUGHT_FORCE).
	   - Once Mogall Forest has set FLAG_NYUNPA_ENLIGHTENED he sits withered
		 (ANIM_NYUNPA_WITHERED) and Mind Read hears his last wish.
	 - Xian (by LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN and
	   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE), Master Feh's kung fu school.
	   The area-name rules give the name Xian only at
	   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN, and scInit hides the
	   name at both.
	   - Force from the white line on the floor fells the tree trunk the
		 students train on (scFuchinTempleForceTree). The first time, Feizhi
		 and Feh come out and she leaves to find Hsu (FLAG_FEIZHI_WENT_TO_HSU),
		 which unlocks LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE.
	   - Feh then waits to see Isaac's Chi (FLAG_FEH_WAITS_TO_SEE_CHI, or
		 FLAG_FEH_CHI_DEMONSTRATION_DECLINED after two refusals). Seeing the
		 tree fall from the line, he names it Ki (FLAG_FEH_SAW_KI).
	   - Moving the tree with Move or by hand only sets
		 FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL.
	   - Once Hsu is known to be trapped (FLAG_XIAN_HEARD_HSU_TRAPPED),
		 scGetActorPlacements removes the tree and two students.
	   - LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN leads out into Xian.
	 - Lama Temple (by LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE), the room
	   in which Master Hama meditates.
	   - The first visit plays her lesson
		 (scFuchinTempleRunHamasRevealLesson). In it she gives Ivan Reveal and
		 leaves with Feizhi to free Hsu, setting FLAG_XIAN_HEARD_HSU_TRAPPED.
	   - Until FLAG_HSU_RESCUED, Hama and the boy monk in her room are away.
	   - LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE leads back into Lama
		 Temple. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/field_psynergy_state.h"
#include "graphics/graphics.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern Sint32		  FuchinTempleJumpingStudentScript[];
extern Uint16		  FuchinTempleTempleDoorOpens[];
extern Uint16		  FuchinTempleFehsDoorOpens[];
extern Uint16		  FuchinTempleFehsDoorCloses[];
extern SCENE_ENTRANCE FuchinTempleGroundsEntrances[];
extern SCENE_ENTRANCE FuchinTempleEntrancesOther[];
extern Uint32		  FuchinTempleExits[];
extern ACTOR_PLACE	  FuchinTempleGroundsActorPlacements[];
extern ACTOR_PLACE	  FuchinTempleActorPlacements[];
extern ACTOR_PLACE	  FuchinTempleActorPlacementsFromLamaTemple[];
extern CUE			  FuchinTempleGroundsCues[];
extern CUE			  FuchinTempleCues[];
extern CUE			  FuchinTempleCuesFromLamaTemple[];
extern Sint32		  FuchinTempleShotScript[];

/* The metatiles outside the temple and in its rooms that run scripts. */
enum {
	/* Outside: the road south, out to the world map. */
	METATILE_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_ROAD = 1,
	/* Outside: the temple's door. */
	METATILE_BEHAVIOR_FUCHIN_TEMPLE_DOOR = 2,
	/* Outside: the falls behind the monk's stump, the way into the grotto. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_GROTTO = 3,
	/* Outside: the stump beside the monk, set only on the way out of the grotto
	   with the Orb of Force. */
	METATILE_BEHAVIOR_FUCHIN_FALLS_MONK_ASKS = 5,
	/* Outside: where the Psynergy Stone lies, metatile (24, 17). */
	METATILE_BEHAVIOR_FUCHIN_TEMPLE_PSYNERGY_STONE = 110,
	/* Inside: the temple's south door. */
	METATILE_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_DOOR = 1,
	/* Inside: the jar in Nyunpa's hall, metatile (12, 27). */
	METATILE_BEHAVIOR_FUCHIN_TEMPLE_UNICORN_RING_JAR = 101,
	/* Inside: the school's south door, out to Xian. */
	METATILE_BEHAVIOR_FEHS_SCHOOL_SOUTH_DOOR = 2,
	/* Inside: the lattice door to Feh's house. */
	METATILE_BEHAVIOR_FEHS_SCHOOL_HOUSE_DOOR = 3,
	/* Inside: the metatiles east and south of the school's tree trunk. */
	METATILE_BEHAVIOR_FEHS_SCHOOL_TREE_EAST_SOUTH = 10,
	/* Inside: the metatiles west and north of the school's tree trunk. */
	METATILE_BEHAVIOR_FEHS_SCHOOL_TREE_WEST_NORTH = 11,
	/* Inside: the barrel in the school, metatile (6, 14). */
	METATILE_BEHAVIOR_FEHS_SCHOOL_ANTIDOTE_BARREL = 100,
	/* Hama's room: the south door, out to Lama Temple. */
	METATILE_BEHAVIOR_HAMAS_ROOM_SOUTH_DOOR = 4,
	/* Hama's room: the jar, metatile (27, 27). */
	METATILE_BEHAVIOR_HAMAS_ROOM_COINS_JAR = 102
};

/* Fuchin Temple's area temp flags last until the party enters another area. */

/* The party refused Nyunpa's test, so talking to him offers it again. */
#define FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED		FLAG_AREA_TEMP_00
/* Feh's students saw the training tree fall, but not to Chi from the line. */
#define FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL FLAG_AREA_TEMP_01

/* The placed actors outside Fuchin Temple (FuchinTempleGrounds), numbered in
   the order of FuchinTempleGroundsActorPlacements. */
typedef enum FUCHIN_TEMPLE_OUTSIDE_ACTOR_ID {
	/* A young man from Kolima. */
	ACTOR_YOUNG_MAN_FROM_KOLIMA = ACTOR_FIRST_PLACED,
	/* The monk beneath the falls, at the waterfall grotto. */
	ACTOR_FALLS_MONK,
	/* A boy monk who reads omens. */
	ACTOR_BOY_MONK_READING_OMENS,
	/* A boy monk who fears for Nyunpa. */
	ACTOR_WORRIED_BOY_MONK,
	/* A boy monk who admires Nyunpa. */
	ACTOR_ADMIRING_BOY_MONK,
	/* The traveling healer, a man in a tall hat and a long robe. */
	ACTOR_HEALER
} FUCHIN_TEMPLE_OUTSIDE_ACTOR_ID;

/* The placed actors inside Fuchin Temple and in Master Feh's school in Xian
   (LacquerRoomsA), numbered in the order of FuchinTempleActorPlacements.

   They are Nyunpa, Feh's students, Feizhi and Feh, and the training tree. */
typedef enum FUCHIN_TEMPLE_INSIDE_ACTOR_ID {
	ACTOR_NYUNPA = ACTOR_FIRST_PLACED,
	/* A young man, by the school's door. */
	ACTOR_WELCOMING_STUDENT,
	/* A student who practices kicks. */
	ACTOR_KICKING_STUDENT,
	/* A student who practices punches. */
	ACTOR_PUNCHING_STUDENT,
	ACTOR_JUMPING_STUDENT,
	/* A student proud of Feh's school. */
	ACTOR_PROUD_STUDENT,
	/* A little girl, a student who learns by watching. */
	ACTOR_WATCHING_STUDENT,
	/* By the training tree, which he sets up again after it falls. */
	ACTOR_TREE_STUDENT,
	/* Keeps the party from Feh, and goes to Feh's door when the tree first
	   falls from the line. */
	ACTOR_MESSENGER_STUDENT,
	ACTOR_FEIZHI,
	ACTOR_FEH,
	ACTOR_TRAINING_TREE,
	/* Where Force strikes the tree. */
	ACTOR_TRAINING_TREE_TARGET
} FUCHIN_TEMPLE_INSIDE_ACTOR_ID;

/* The placed actors in Master Hama's room in Lama Temple (LacquerRoomsA by
   LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE), numbered in the order of
   FuchinTempleActorPlacementsFromLamaTemple.

   Garet, Ivan and Mia, placed first, keep their own slots. */
typedef enum HAMAS_ROOM_ACTOR_ID {
	ACTOR_HAMA = ACTOR_FIRST_PLACED,
	/* A boy monk in Hama's room. */
	ACTOR_HAMAS_BOY_MONK,
	/* Bursts in to say that Hsu is in danger. */
	ACTOR_FEIZHI_IN_LAMA_TEMPLE
} HAMAS_ROOM_ACTOR_ID;

/* Animations of these scenes' own sprites beyond ACTOR_ANIMATION. */
enum {
	/* Seated cross-legged with his eyes shut (nyunpa.png's frame 33). */
	ANIM_NYUNPA_MEDITATE = 5,
	/* Seated as he meditated, shrunken and darkened, once he has died
	   enlightened (nyunpa.png's frame 34). */
	ANIM_NYUNPA_WITHERED = 6,
	/* A drill of kicks, a knee raised, a kick and a leaping kick, from a
	   fighting stance (youth_with_headband.tsv). */
	ANIM_STUDENT_KICKS = 5,
	/* A drill of quick punches with one kick among them
	   (youth_with_tied_hair.tsv). */
	ANIM_STUDENT_PUNCHES = 5
};

static void scFuchinTempleFeizhiLeavesForHsu(void);
static void scFuchinTempleFehSeesChi(void);
static void scHopLeaderBy(Sint32 dx, Sint32 dz);
static void scFuchinTempleStartMindReadDemonstration(Sint32 caster, Sint32 tgt);
static void scFuchinTempleFinishMindReadDemonstration(void);
static void scFuchinTempleSpawnNyunpasParticles(void);
static void scFuchinTempleAnimateRevealLesson(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* The Psynergy Stone outside the temple has been taken. It refills once the
   party enters another area. */
#define FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/* A script step that takes the actor's shadow and other sprite flags away. */
static Sint32 scFuchinTempleClearScriptedActorSprFlags(ACTOR *interpreter)
{
	fdActorSetSprFlags(interpreter, 0);
	return 0;
}

/* Turns an actor toward the party leader, each tick, while Feh's school waits
   to see Isaac's Chi. */
static UNREAD_RESULT scFuchinTempleFaceLeader(ACTOR *actor)
{
	ACTOR *player = fdActorGet(ACTOR_PARTY_LEADER);
	actor->facing = syAtan(
		player->z.fixed - actor->z.fixed, player->x.fixed - actor->x.fixed);
#if !FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* The entrances outside, or those of the rooms map. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_FUCHIN_TEMPLE_GROUNDS) {
		return FuchinTempleGroundsEntrances;
	}
	return FuchinTempleEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return FuchinTempleExits;
}

/* The actors outside, in Hama's room, or in the temple and Feh's school, where
   once Hsu is known to be trapped the tree and two students are gone. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_FUCHIN_TEMPLE_GROUNDS) {
		return FuchinTempleGroundsActorPlacements;
	}
	if(MainRoutineStateWork.entrance ==
		LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
		return FuchinTempleActorPlacementsFromLamaTemple;
	}
	/* Once Hsu is known to be trapped, the tree and two students are gone, and
	   another student stands elsewhere. */
	if(fdScriptGetFlag(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
		FuchinTempleActorPlacements[ACTOR_PROUD_STUDENT - ACTOR_FIRST_PLACED]
			.condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
		FuchinTempleActorPlacements[ACTOR_TREE_STUDENT - ACTOR_FIRST_PLACED]
			.condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
		FuchinTempleActorPlacements[ACTOR_MESSENGER_STUDENT -
			ACTOR_FIRST_PLACED]
			.x = toFIXED(288);
		FuchinTempleActorPlacements[ACTOR_MESSENGER_STUDENT -
			ACTOR_FIRST_PLACED]
			.z = toFIXED(248);
		FuchinTempleActorPlacements[ACTOR_TRAINING_TREE - ACTOR_FIRST_PLACED]
			.condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
		FuchinTempleActorPlacements[ACTOR_TRAINING_TREE_TARGET -
			ACTOR_FIRST_PLACED]
			.condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
	}
	fdAssignViewFlags(FuchinTempleActorPlacements);
	return FuchinTempleActorPlacements;
}

/* Isaac talks to the young man from Kolima on the path below the temple: he
   asks how the party got here, and whether the bridge is open. */
static void scFuchinTempleYoungManFromKolimaAsksAboutBridge(void)
{
	fdScriptStart();
	/* "How did you get here? Is the bridge open again?" */
	fdScriptSetMessageId(MsgXianHowDidGetHereBridge);
	if(fdScriptGetFlag(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN) != 0) {
		/* "Did you know that the forest up ahead used to be a vast prairie?" */
		FieldStateWork->msg += 4;
	}
	fdScriptOpenMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		/* Yes: "Really?! Then I can go home to Kolima now?...", or after the
		   prairie: "They say the prairie became a vast forest overnight..." */
		gmFlagSet(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN);
	} else {
		/* No: "Oh... Then you came through the forest? That's too bad.", or
		   after the prairie: "You don't believe me, do you? Well, it's the
		   truth..." */
		FieldStateWork->msg++;
	}
	fdScriptShowMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
	fdScriptEnd();
}

/* Isaac talks to the boy monk who admires Nyunpa, below the temple: "Isn't it
   noble of him to try to save the world...", with up to two more questions.
   Once Nyunpa has died enlightened, he wonders whether he is watching over
   them. */
static void scFuchinTempleBoyMonkSpeaksOfNyunpa(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_NYUNPA_ENLIGHTENED) != 0) {
		fdScriptSetMessageId(MsgXianNowHeTrulyBeyondWorlds);
		/* "Now, he truly is beyond the world's concerns. Is he watching..."
		   Yes: "I believed that once. But now, I am no longer so sure." No:
		   "Yes... This seems very wrong. The world has lost something..." */
		fdScriptAskYesNo(ACTOR_ADMIRING_BOY_MONK, 0);
		fdScriptEnd();
	} else {
		fdScriptSetMessageId(MsgXianIsntNobleHimTrySave);
		/* "Isn't it noble of him to try to save the world by becoming..." */
		fdScriptOpenMessage(ACTOR_ADMIRING_BOY_MONK, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
			FieldStateWork->msg++;
			/* "Do you know what you mean when you say that?" */
			fdScriptOpenMessage(ACTOR_ADMIRING_BOY_MONK, 0);
			if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
				FieldStateWork->msg++;
			}
		}
		/* Yes: "However, I really do not want to see Nyunpa waste away." Then
		   yes: "Well, one may have many different opinions in this world,
		   but... I wish you could understand our beliefs." Then no: "To achieve
		   enlightenment, one fasts to cleanse body and mind..." */
		fdScriptShowMessage(ACTOR_ADMIRING_BOY_MONK, 0);
		fdScriptEnd();
	}
}

/* Isaac talks to the monk on the stump in front of the falls once Nyunpa has
   set the test: "I have already heard of your test from Master Nyunpa. You may
   enter." He leaps aside onto the next stump with a splash, opening the way
   into the grotto (FLAG_FUCHIN_FALLS_GROTTO_OPENED). */
static void scFuchinTempleMonkLetsPartyIntoGrotto(void)
{
	ACTOR		  *monk;
	EFFECT_OPTIONS options;

	monk = fdActorGet(ACTOR_FALLS_MONK);
	fdScriptStart();
	fdScriptSetMessageId(MsgXianAlreadyHeardTest);
	/* "I have already heard of your test from Master Nyunpa. You..." */
	fdScriptShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
	/* The monk leaps aside and lands in a splash. */
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 392);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	sySndPlay(SE_WATER);
	fdActorGet(ACTOR_FALLS_MONK)->velY = toFIXED(20);
	fdActorGet(ACTOR_FALLS_MONK)->gravity = toFIXED(4);
	fdActorSetSpeed(ACTOR_FALLS_MONK, 3);
	fdActorSetDestination(ACTOR_FALLS_MONK, 152, 392);
	fdActorWaitForMoveById(ACTOR_FALLS_MONK);
	fdActorGet(ACTOR_FALLS_MONK)->gravity = toFIXED(1);
	fdActorFaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
	sySndPlay(SE_WATER);
	/* Three white puffs, with only the colour option set. */
	options.pal = SPRITE_COLOR_WHITE;
	fdEffectSpawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + toFIXED(4)),
		toFIXED(1) / 2, 0, 0, EFFECT_USE_PALETTE, &options);
	fdEffectSpawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + toFIXED(4)), 0,
		0, 0, EFFECT_USE_PALETTE, &options);
	fdEffectSpawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + toFIXED(4)),
		-toFIXED(1) / 2, 0, 0, EFFECT_USE_PALETTE, &options);
	fdScriptWait(30);
	fdPlayQueuedMusic();
	fdMapGridCopyMetatileAttributes(10, 24, 1, 1, 10, 22);
	gmFlagSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED);
	fdScriptEnd();
}

/* Coming back out from behind the falls with the Orb of Force, Isaac hops onto
   the stump beside the monk, metatile (10, 24), and the monk asks: "Young
   master, did you complete the test?" Either way he sends him up to the temple,
   where Master Nyunpa is waiting.

   The metatile is only set for this on the way out of the grotto (scInit), and
   turned back into a plain stepping stone here. */
static void scFuchinTempleMonkAsksWhetherTestIsDone(void)
{
	fdScriptStart();
	gmFlagSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST);
	fdActorFaceActor(ACTOR_FALLS_MONK, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(10);
	fdScriptSetMessageId(MsgXianYoungMasterDidCompleteTest);
	fdActorRunRepeatedMotion(ACTOR_FALLS_MONK, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	/* "Young master, did you complete the test?" Yes: "Why, you have mastered
	   the secrets of Fuchin Temple!?" No: "But you must have..." */
	fdScriptAskYesNo(ACTOR_FALLS_MONK, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_FALLS_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
	fdActorFaceDirection(ACTOR_FALLS_MONK, FACING_NORTH_NORTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_FALLS_MONK, 2);
	fdScriptWait(20);
	/* "Master Nyunpa is waiting for you..." */
	fdScriptShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
	fdActorFaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 20);
	fdActorSetAnimationAndWait(ACTOR_FALLS_MONK, ANIM_NOD);
	fdScriptWait(20);
	/* "Proceed to the temple." */
	fdScriptShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
	fdMapGridCopyMetatileAttributes(10, 26, 1, 1, 10, 24);
	fdScriptEnd();
}

/* Isaac reads Nyunpa's mind as he meditates in the temple, or talks to him
   after turning the test down: Nyunpa offers him the test of the grotto beneath
   the falls. */
static void scFuchinTempleNyunpaOffersTest(void)
{
	Sint32 alreadyDeclinedTrial;

	fdScriptStart();
	/* Revisit an earlier refusal, or introduce the offer through Mind Read. */
	alreadyDeclinedTrial = gmFlagGet(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED);
	if(alreadyDeclinedTrial != 0) {
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
		fdScriptWait(5);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
		fdActorGet(ACTOR_NYUNPA)->paused = 0;
		sySndPlay(SE_JUMP);
		fdActorGet(ACTOR_NYUNPA)->velY = toFIXED(8);
		fdActorSetAnimation(ACTOR_NYUNPA, ANIM_STAND);
		fdScriptWait(30);
		fdScriptSetMessageId(MsgXianMustSurviveTestGrottoBefore);
	} else {
		fdScriptSetMessageId(MsgXianMmmmWhoWhoSpeaksMy);
		scFuchinTempleStartMindReadDemonstration(
			ACTOR_PARTY_LEADER, ACTOR_NYUNPA);
		fdScriptWait(30);
		/* "Mmmm... Who is that? Who speaks to my mind?" */
		fdScriptShowMessage(ACTOR_NYUNPA, 0);
		scFuchinTempleFinishMindReadDemonstration();
		fdScriptWait(20);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
		fdScriptWait(5);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
		sySndPlay(SE_JUMP);
		fdActorGet(ACTOR_NYUNPA)->paused = alreadyDeclinedTrial;
		fdActorGet(ACTOR_NYUNPA)->velY = toFIXED(8);
		fdActorSetAnimation(ACTOR_NYUNPA, ANIM_STAND);
		fdScriptWait(30);
		/* "Young master... Was that your voice I heard in my mind just now?" */
		fdScriptOpenMessage(ACTOR_NYUNPA, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
			fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
			fdScriptWait(20);
			/* No: "Speak no lies to me!" */
			fdScriptShowMessage(ACTOR_NYUNPA, 0);
			fdScriptWait(20);
			scFuchinTempleStartMindReadDemonstration(
				ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
			fdScriptWait(30);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdScriptWait(50);
			scFuchinTempleFinishMindReadDemonstration();
			fdScriptWait(30);
			fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
			/* "I knew it!" */
			fdScriptShowMessage(ACTOR_NYUNPA, 0);
		} else {
			FieldStateWork->msg += 2;
			/* Yes: "I knew it!" */
			fdScriptShowMessage(ACTOR_NYUNPA, 0);
		}
		fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
		fdScriptWait(30);
		fdActorShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		fdScriptSetMessageId(MsgXianFollowThemDoNot);
		/* "You follow them, do you not?" */
		fdScriptOpenMessage(ACTOR_NYUNPA, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
			fdActorShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
			scFuchinTempleStartMindReadDemonstration(
				ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
			fdScriptWait(30);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdScriptWait(50);
			scFuchinTempleFinishMindReadDemonstration();
			fdScriptWait(30);
			/* No: "You speak lies... You are indeed following them!" */
			fdScriptShowMessage(ACTOR_NYUNPA, 0);
			FieldStateWork->msg += 1;
		} else {
			FieldStateWork->msg += 1;
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
			fdScriptWait(20);
			/* "I knew it!" */
			fdScriptShowMessage(ACTOR_NYUNPA, 0);
		}
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "But you must go through Mogall Forest, the endless woods..." */
		fdScriptShowMessage(ACTOR_NYUNPA, 0);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdScriptWait(20);
		/* "But perhaps I could, young master." */
		fdScriptShowMessage(ACTOR_NYUNPA, 0);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_NYUNPA, FACING_NORTH, 20);
		fdActorSetSpeed(ACTOR_NYUNPA, 0.3);
		fdActorWalkToAndWait(ACTOR_NYUNPA, 168, 464);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_NYUNPA, FACING_SOUTH, 40);
		/* "If you can endure the trials in the waterfall grotto, I will tell
		   you." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 10);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorWalkToAndWait(ACTOR_NYUNPA, 168, 472);
	}
	/* "Will you take the test?", or after a refusal, "You must survive the test
	   of the grotto before you can pass through Mogall Forest. Will you take
	   the test?" */
	fdScriptOpenMessage(ACTOR_NYUNPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		fdScriptSetMessageId(MsgXianThenCannotTellGiveUp);
		/* "Then I cannot tell you. Give up and go back from whence you..." */
		fdScriptShowMessage(ACTOR_NYUNPA, 0);
		gmFlagSet(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED);
	} else {
		/* "Do not worry. You will be permitted to enter the grotto." Nyunpa
		   glows as he calls to the monk. */
		fdScriptSetMessageId(MsgXianDoNotWorryWillPermitted);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
		fdScriptWait(20);
		fdCameraSetSpeed(0.5);
		fdCameraMoveToActorById(ACTOR_NYUNPA, 1);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
		fdPaletteFadeStartInterpolation(30);
		fdScriptWaitTransitionTicks();
		fdCameraWaitForArrival();
		scFuchinTempleSpawnNyunpasParticles();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(30);
		/* "Do not worry. You will be permitted to enter the grotto." */
		fdScriptShowMessage(ACTOR_NYUNPA, 0);
		fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
		fdScriptWait(20);
		/* "Go now, Isaac!" */
		fdScriptShowMessage(ACTOR_NYUNPA, 0);
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(60);
		/* "Just remember, you are not the only ones who can read minds." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 10);
		gmFlagSet(FLAG_NYUNPA_TEST_ACCEPTED);
	}
	fdActorSetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
	fdScriptEnd();
}

/* Isaac reads Nyunpa's mind again once the test is accepted, and Nyunpa
   notices: "You enjoy reading the minds of others, do you not?" */
static void scFuchinTempleNyunpaFeelsMindRead(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgXianEnjoyReadingMindsOthersDo);
	/* "You enjoy reading the minds of others, do you not?" Yes: "If you overdo
	   it, you will soon learn to despise the ability." No: "That is good. Enjoy
	   reading the minds of others while you..." */
	fdScriptAskYesNo(ACTOR_NYUNPA, 0);
	fdScriptEnd();
}

/* Isaac talks to the young man by the door of Master Feh's school in Xian:
   "This is Master Feh's school... You came to watch?" */
static void scFuchinTempleStudentWelcomesToSchool(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgXianMasterFehsSchoolCameWatch);
	/* "This is Master Feh's school... You came to watch?" Yes: "Promise me you
	   will be quiet." No: "Warriors should learn martial arts. They will help
	   in battles." */
	fdScriptAskYesNo(ACTOR_WELCOMING_STUDENT, 0);
	fdScriptEnd();
}

/* Isaac talks to the student jumping up and down in Master Feh's school: he
   lands, talks about jumping (later of Lama Temple, or of Hsu), then jumps
   on. */
static void scFuchinTempleTalkToJumpingStudent(void)
{
	fdScriptStart();

	fdActorGet(ACTOR_JUMPING_STUDENT)->paused = 0;

	while(fdActorGet(ACTOR_JUMPING_STUDENT)->y.fixed > 0) {
		sySynch(1);
	}

	fdActorGet(ACTOR_JUMPING_STUDENT)->y.fixed = 0;

	fdActorGet(ACTOR_JUMPING_STUDENT)->tgtY = ACTOR_NO_TARGET;

	fdActorGet(ACTOR_JUMPING_STUDENT)->velY = 0;

	fdActorGet(ACTOR_JUMPING_STUDENT)->paused = 1;

	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);

	if(gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0) {
		fdScriptSetMessageId(MsgXianHsuDidNotPracticeJumping);
	} else if(gmFlagGet(FLAG_FEH_SAW_KI) != 0) {
		fdScriptSetMessageId(MsgXianLamaTempleFarWestIn);
	} else {
		fdScriptSetMessageId(MsgXianFlexibilityJumpingVeryImportantIn);
	}

	fdScriptShowMessage(ACTOR_JUMPING_STUDENT, 0);

	fdActorGet(ACTOR_JUMPING_STUDENT)->facing = FACING_SOUTH;

	fdActorGet(ACTOR_JUMPING_STUDENT)->paused = 0;

	fdActorSetScript(ACTOR_JUMPING_STUDENT, FuchinTempleJumpingStudentScript);
	fdScriptEnd();
}

/* Isaac talks to the student by the tree trunk in Master Feh's school: "Master
   Feh is very busy. Do you still wish to see him?" Yes has him point out the
   line to stand on and use Chi from. */
static void scFuchinTempleStudentAsksAboutFeh(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgXianMasterFehVeryBusyDo);
	/* "Master Feh is very busy. Do you still wish to see him?" Yes: "Look
	   there. You see that line? Stand there and use Chi. If..." No: "You will
	   not see Master Feh? That is too bad." */
	fdScriptAskYesNo(ACTOR_TREE_STUDENT, 0);
	fdScriptEnd();
}

/* The tree trunk's tilt in sixteenths of a turn (FACING_STEP) as it falls
   sideways: it stands at TREE_STANDING_STEP and lies a quarter turn to either
   side. */
enum {
	TREE_STANDING_STEP = DEGtoANG(180) / FACING_STEP,
	TREE_FALLEN_WEST_STEP = DEGtoANG(90) / FACING_STEP,
	TREE_FALLEN_EAST_STEP = DEGtoANG(270) / FACING_STEP
};

/* The tree trunk topples to the west, faster as it goes, and lands with a thud
   and a puff of dust. */
static void scFuchinTempleKnockTreeDownWest(void)
{
	ACTOR *tree = fdActorGet(ACTOR_TRAINING_TREE);
	Uint32 turnStep;
	Sint32 ang;

	for(turnStep = TREE_STANDING_STEP; turnStep >= TREE_FALLEN_WEST_STEP;
		turnStep--) {
		ang = turnStep * FACING_STEP;
		tree->spr->rotation = ang;
		sySynch((turnStep - TREE_FALLEN_WEST_STEP) * 2);
		tree->x.fixed += syCos(ang) * 6;
		tree->z.fixed += sySin(ang) * 6;
	}

	tree->y.fixed = toFIXED(18);
	tree->tgtY = toFIXED(18);

	sySndPlay(SE_TOPPLE);

	fdEffectSpawn(tree->x.fixed - toFIXED(12), tree->y.fixed,
		tree->z.fixed + toFIXED(8), -toFIXED(1) / 5, toFIXED(2) / 5, 0, 0, 0);
	fdEffectSpawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(8),
		-toFIXED(4) / 5, toFIXED(3) / 10, 0, 0, 0);
	fdEffectSpawn(tree->x.fixed + toFIXED(10), tree->y.fixed,
		tree->z.fixed + toFIXED(8), -toFIXED(1), toFIXED(1) / 5, 0, 0, 0);
}

/* The tree trunk topples to the east, faster as it goes, and lands with a thud
   and a puff of dust. */
static void scFuchinTempleKnockTreeDownEast(void)
{
	ACTOR *tree;
	Uint32 turnStep;
	Sint32 ang;

	tree = fdActorGet(ACTOR_TRAINING_TREE);
	for(turnStep = TREE_STANDING_STEP; turnStep <= TREE_FALLEN_EAST_STEP;
		turnStep++) {
		ang = turnStep * FACING_STEP;
		tree->spr->rotation = ang;
		sySynch((TREE_FALLEN_EAST_STEP - turnStep) * 2);
		tree->x.fixed -= syCos(ang) * 6;
		tree->z.fixed -= sySin(ang) * 6;
	}
	tree->y.fixed = toFIXED(18);
	tree->tgtY = toFIXED(18);
	tree->scaleX = -toFIXED(4) / 5;
	sySndPlay(SE_TOPPLE);
	fdEffectSpawn(tree->x.fixed - toFIXED(12), tree->y.fixed,
		tree->z.fixed + toFIXED(8), toFIXED(1), toFIXED(1) / 5, 0, 0, 0);
	fdEffectSpawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(8),
		toFIXED(4) / 5, toFIXED(3) / 10, 0, 0, 0);
	fdEffectSpawn(tree->x.fixed + toFIXED(10), tree->y.fixed,
		tree->z.fixed + toFIXED(8), toFIXED(1) / 5, toFIXED(2) / 5, 0, 0, 0);
}

/* The tree trunk rocks north and falls over that way, with a thud and a puff of
   dust. */
static void scFuchinTempleKnockTreeDownNorth(void)
{
	ACTOR *tree = fdActorGet(ACTOR_TRAINING_TREE);
	Uint32 stepIdx = 0;
	Sint32 delay = 8;
	do {
		sySynch(delay);
		tree->z.fixed -= toFIXED(1);
		tree->tgtZ = ACTOR_NO_TARGET;
		stepIdx++;
		delay -= 2;
	} while(stepIdx < 4);
	{
		Uint8 rotation = 0;

		tree->spr->rotation = rotation;
		sySndPlay(SE_TOPPLE);
		fdEffectSpawn(tree->x.fixed, tree->y.fixed, tree->z.fixed - toFIXED(8),
			-toFIXED(4) / 5, 0, -toFIXED(1) / 5, 0, 0);
		fdEffectSpawn(tree->x.fixed, tree->y.fixed, tree->z.fixed - toFIXED(8),
			toFIXED(4) / 5, 0, -toFIXED(1) / 5, 0, 0);
		fdEffectSpawn(tree->x.fixed - toFIXED(6), tree->y.fixed,
			tree->z.fixed + toFIXED(10), toFIXED(1) / 5, 0, -toFIXED(1), 0, 0);
		fdEffectSpawn(tree->x.fixed + toFIXED(6), tree->y.fixed,
			tree->z.fixed + toFIXED(10), toFIXED(1) / 5, 0, -toFIXED(1), 0, 0);
	}
}

/* The tree trunk rocks south and falls over that way, with a thud and a puff of
   dust. */
static void scFuchinTempleKnockTreeDownSouth(void)
{
	ACTOR *tree;
	Uint32 stepIdx;
	FIXED  particleSpeed;
	Sint32 delay;

	tree = fdActorGet(ACTOR_TRAINING_TREE);
	stepIdx = 0;
	delay = 8;
	do {
		sySynch(delay);
		tree->z.fixed += toFIXED(1);
		tree->tgtZ = ACTOR_NO_TARGET;
		stepIdx++;
		delay -= 2;
	} while(stepIdx < 4);

	tree->spr->rotation = 0;
	tree->z.fixed += toFIXED(24);
	tree->tgtZ = ACTOR_NO_TARGET;
	sySndPlay(SE_TOPPLE);

	fdEffectSpawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(12),
		-toFIXED(4) / 5, 0, toFIXED(1) / 5, 0, 0);
	particleSpeed = toFIXED(1) / 5;
	fdEffectSpawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(12),
		toFIXED(4) / 5, 0, particleSpeed, 0, 0);
	fdEffectSpawn(tree->x.fixed - toFIXED(6), tree->y.fixed,
		tree->z.fixed - toFIXED(8), particleSpeed, 0, toFIXED(1), 0, 0);
	fdEffectSpawn(tree->x.fixed + toFIXED(6), tree->y.fixed,
		tree->z.fixed - toFIXED(8), particleSpeed, 0, toFIXED(1), 0, 0);
}

/* The fallen tree trunk is set up again at its place in the school. */
static inline void scFuchinTempleSetTreeUpAgain(void)
{
	fdActorSetPos(ACTOR_TRAINING_TREE, 232, 168);
	fdActorSetPos(ACTOR_TRAINING_TREE_TARGET, 232, 168);
	fdActorGet(ACTOR_TRAINING_TREE)->y.fixed = toFIXED(12);
	fdActorGet(ACTOR_TRAINING_TREE)->tgtY = ACTOR_NO_TARGET;
	fdActorGet(ACTOR_TRAINING_TREE)->scaleX = toFIXED(4) / 5;
	fdActorGet(ACTOR_TRAINING_TREE)->spr->rotation = DEGtoANG(180);
	sySndPlay(SE_CHEST_OPEN);
}

/* The student by the tree walks round to the fallen trunk, on the side away
   from Isaac. */
static void scFuchinTempleStudentWalksAroundTree(void)
{
	Uint16 leaderFacing;

	leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	if(leaderFacing >= FACING_SOUTHEAST && leaderFacing < FACING_SOUTHWEST) {
		fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
		fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 224, 168);
		fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
	} else if(leaderFacing >= FACING_SOUTHWEST &&
		leaderFacing < FACING_NORTHWEST) {
		fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
		fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH_SOUTHWEST, 20);
	} else if(leaderFacing >= FACING_NORTHWEST &&
		leaderFacing < FACING_NORTHEAST) {
		fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
		fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 224, 172);
		fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_NORTHEAST, 20);
	} else {
		fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
		fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
	}
}

/* What the student by the tree explains
   (scFuchinTempleStudentExplainsChiRules). */
typedef enum FUCHIN_TEMPLE_CHI_RULE {
	/* Psynergy felled the tree, but not from the line. */
	FUCHIN_TEMPLE_CHI_NOT_FROM_LINE = 0,
	FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND = 1, /* Isaac pushed it over by hand */
	/* Praise for a warrior Feh has already seen. */
	FUCHIN_TEMPLE_CHI_SEEN_BEFORE = 2
} FUCHIN_TEMPLE_CHI_RULE;

/* The student by the tree explains the rules, as `mode`, an enum
   FUCHIN_TEMPLE_CHI_RULE, says, and sets the trunk up again. */
static void scFuchinTempleStudentExplainsChiRules(Sint32 mode)
{
	Sint32 rulesMsg;

	fdActorSetSpeed(ACTOR_TREE_STUDENT, 0.8);
	fdScriptWait(60);
	rulesMsg = MsgXianCannotPushHands;
	fdScriptSetMessageId(rulesMsg);
	if(mode == FUCHIN_TEMPLE_CHI_NOT_FROM_LINE) {
		fdScriptSetMessageId(rulesMsg - 1);
		fdActorShowEmote(
			ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		/* "Ya! What happened!? The tree fell on its own!" */
		fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
		fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
		fdScriptSetMessageId(MsgXianWarriorsCannotUse);
		/* "Warriors cannot use... Wait. This is different from Chi." */
		fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "Do not step over the line, warrior." */
		fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_NOD);
		fdScriptWait(20);
	}
	if(mode == FUCHIN_TEMPLE_CHI_SEEN_BEFORE) {
		fdScriptSetMessageId(MsgXianGreatWarriorTrain);
		fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
		fdScriptWait(20);
	}
	/* "You cannot push it with your hands. Attack it with Chi.", "You are a
	   great warrior. I must train, like you must have trained.", or after the
	   warning about the line, "Stand beyond the line. Then try your Chi." */
	fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
	scFuchinTempleStudentWalksAroundTree();
	fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
	scFuchinTempleSetTreeUpAgain();
	fdScriptWait(40);
	fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
	fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 30);
}

/* The students see the tree trunk fall, Feh will not count it because it was
   not done from the line, and the student sets the trunk up again. */
static void scFuchinTempleStudentsSeeTreeFall(void)
{
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_FEH, ACTOR_TRAINING_TREE, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgXianLookTreeFell);
	/* "Look! The tree fell." */
	fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
	/* "Master, did you see? The warrior used Chi!" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmote(
		ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Master, what is the matter?" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "He did not do it from the line, so I cannot be sure." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "That is not good. Warrior, do it right! Do it from the line." */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorTurnToFaceEachOther(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_TREE_STUDENT, 0.8);
	scFuchinTempleStudentWalksAroundTree();
	fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
	scFuchinTempleSetTreeUpAgain();
	fdScriptWait(40);
	fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
	fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
	gmFlagSet(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL);
}

/* Isaac casts Move on the tree trunk in Master Feh's school: it topples, and
   the student by it says that was not Chi, or Feh will not count it because it
   was not done from the line. */
static void scFuchinTempleMoveTree(void)
{
	fdScriptStart();

	if(fdActorGet(ACTOR_PARTY_LEADER)->facing > FACING_SOUTH &&
		fdActorGet(ACTOR_PARTY_LEADER)->facing < FACING_NORTH) {
		scFuchinTempleKnockTreeDownWest();
	} else {
		scFuchinTempleKnockTreeDownEast();
	}

	if(gmFlagGet(FLAG_FEH_WAITS_TO_SEE_CHI) != 0) {
		scFuchinTempleStudentsSeeTreeFall();
	} else {
		scFuchinTempleStudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
	}

	fdScriptEnd();
}

/* Isaac walks into the tree trunk in Master Feh's school and pushes it over by
   hand: "You cannot push it with your hands. Attack it with Chi." The student
   by it sets it up again. */
static void scFuchinTemplePushTreeByHand(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Uint16 facing;

	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdScriptWait(20);

	facing = leader->facing;

	if(facing >= FACING_SOUTHEAST && facing < FACING_SOUTHWEST) {
		scFuchinTempleKnockTreeDownSouth();
	} else if(facing >= FACING_SOUTHWEST && facing < FACING_NORTHWEST) {
		scFuchinTempleKnockTreeDownWest();
	} else if(facing >= FACING_NORTHWEST && facing < FACING_NORTHEAST) {
		scFuchinTempleKnockTreeDownNorth();
	} else {
		scFuchinTempleKnockTreeDownEast();
	}

	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	scFuchinTempleStudentExplainsChiRules(FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND);
	fdScriptEnd();
}

/* Isaac casts Force on the tree trunk in Master Feh's school and it topples.
   Cast from behind the white line on the floor, the first time, Feizhi and Feh
   come out; with Feh waiting to see it, he names it Ki. Cast from nearer, the
   students will not count it. */
static void scFuchinTempleForceTree(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Uint16 facing;

	fdScriptStart();

	facing = leader->facing;
	if(facing >= FACING_SOUTHEAST && facing < FACING_SOUTHWEST) {
		scFuchinTempleKnockTreeDownSouth();
	} else if(facing >= FACING_SOUTHWEST && facing < FACING_NORTHWEST) {
		scFuchinTempleKnockTreeDownWest();
	} else if(facing >= FACING_NORTHWEST && facing < FACING_NORTHEAST) {
		scFuchinTempleKnockTreeDownNorth();
	} else {
		scFuchinTempleKnockTreeDownEast();
	}

	fdCameraSetSpeed(1);
	fdCameraMoveToActorById(ACTOR_TRAINING_TREE_TARGET, 1);
	fdCameraWaitForArrival();

	if(leader->z.part[0].integer <= 209) {
		if(gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU) == 0 ||
			gmFlagGet(FLAG_FEH_SAW_KI) != 0) {
			scFuchinTempleStudentExplainsChiRules(
				FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
		} else {
			scFuchinTempleStudentsSeeTreeFall();
		}
		fdScriptEnd();
		return;
	}

	if(gmFlagGet(FLAG_FEH_SAW_KI) != 0) {
		scFuchinTempleStudentExplainsChiRules(FUCHIN_TEMPLE_CHI_SEEN_BEFORE);
	} else if(gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU) == 0) {
		scFuchinTempleFeizhiLeavesForHsu();
	} else {
		scFuchinTempleFehSeesChi();
	}
	fdScriptEnd();
}

/* The first time the trunk falls to Force from the line, a student runs to
   Feh's door, Feh and his daughter Feizhi come out arguing over her visions,
   and she leaves alone to find Hsu. */
static void scFuchinTempleFeizhiLeavesForHsu(void)
{
	ACTOR *actor;

	gmFlagSet(FLAG_FEIZHI_WENT_TO_HSU);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	fdActorShowEmote(
		ACTOR_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(
		ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	fdScriptSetMessageId(MsgXianDidDoWarrior);
	/* "What did you do, warrior?" */
	fdScriptShowMessageThenWait(ACTOR_PROUD_STUDENT, 0, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_PROUD_STUDENT, 0);
	fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 1);
	fdScriptWait(20);
	/* "The warrior used Chi! He did it from the line... and the..." */
	fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_TREE_STUDENT, 0);
	fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_MESSENGER_STUDENT, 0);
	/* "He knows kung fu! Do you know kung fu?" Yes: "Your Chi must be strong if
	   you can use it from afar. I must..." No: "It is unbelievable that someone
	   who does not know kung fu..." */
	fdScriptAskYesNo(ACTOR_MESSENGER_STUDENT, 0);
	fdScriptWait(50);
	fdCameraFollowActor(ACTOR_MESSENGER_STUDENT, 1);
	fdActorSetSpeed(ACTOR_MESSENGER_STUDENT, 0.8);
	fdActorWalkToAndWait(ACTOR_MESSENGER_STUDENT, 176, 248);
	fdActorWalkToAndWait(ACTOR_MESSENGER_STUDENT, 308, 248);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MESSENGER_STUDENT, FACING_NORTH, 20);
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(FuchinTempleFehsDoorOpens, 78, 13);
	fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_MESSENGER_STUDENT, 1.5);
	fdActorGet(ACTOR_MESSENGER_STUDENT)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_MESSENGER_STUDENT, 308, 272);
	fdScriptWait(1);
	actor = fdActorGet(ACTOR_MESSENGER_STUDENT);

	actor->steeringFlg = ACTOR_FACES_MOTION | actor->steeringFlg;
	/* "Ya! What happened!? The door opened on its own!" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 50);
	fdActorSetPos(ACTOR_FEIZHI, 304, 216);
	fdActorWalkToAndWait(ACTOR_FEIZHI, 304, 248);
	fdActorFaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FEIZHI, 0);
	fdScriptWait(10);
	fdActorStartRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_KICKING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_PUNCHING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_JUMPING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_WATCHING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_TREE_STUDENT, 2);
	fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
	fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorSetPos(ACTOR_FEH, 304, 216);
	fdActorWalkTo(ACTOR_FEH, 304, 248);
	fdActorWalkTo(ACTOR_FEIZHI, 280, 264);
	fdActorWaitForMoveById(ACTOR_FEH);
	fdActorFaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWaitForMoveById(ACTOR_FEIZHI);
	sySndPlay(SE_DOOR_CLOSE);
	fdMapGridAnimateMetatiles(FuchinTempleFehsDoorCloses, 78, 13);
	/* "Feizhi! Wait!" */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorFaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEIZHI, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FEIZHI, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Are you worried that Hsu is late?" */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_FEH, 1);
	fdScriptWait(20);
	/* "He is only a little late in returning from Lama Temple. Do..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
	/* "Something is wrong with Hsu... I sense it." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Ahhh, Feizhi... Is it your power again?" */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
	fdScriptWait(20);
	/* "Ever since gems fell from the sky and hit me, I have sensed..." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	/* "You say you sense evil ever since you received the power of..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I do not believe it, Feizhi. Your visions are just coincidences." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_EAST, 20);
	/* "My visions came true many times, no?" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorFaceDirection(ACTOR_MESSENGER_STUDENT, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
	fdScriptWait(20);
	/* "I had a vision about Mogall Forest, and that came true..." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	/* "Yes, your predictions came true, Feizhi. It was very surprising!" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_WEST, 20);
	/* "The flood in Altin... Did I not predict that, too?" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	/* "All Feizhi's predictions come true." */
	fdScriptShowMessageThenWait(ACTOR_WELCOMING_STUDENT, 0, 20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_FEIZHI, 1);
	fdScriptWait(20);
	/* "See, Father?" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "I have heard this all. I was surprised, too, but it is still..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "You still don't believe it?" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	/* "Hsu is late, so you worry too much. But do not think bad..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "You sent people to find Hsu, correct?" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	/* "Stubborn old man." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "What did you say, Feizhi? You should not speak so to your..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I will not ask anyone. I will go alone, then!" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	/* "It is too dangerous." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_FEIZHI, 256, 280);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_SOUTH, 20);
	fdActorSetPos(ACTOR_FEIZHI, 0, 0);
	fdActorReleaseById(ACTOR_FEIZHI);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
	fdScriptWait(20);
	/* "Ya! Master Feh! Feizhi has gone!" */
	fdScriptShowMessageThenWait(ACTOR_WELCOMING_STUDENT, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
	fdScriptWait(20);
	/* "Feizhi was very angry. She was frightening!" */
	fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEH, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
	fdScriptWait(20);
	/* "Hsu is in danger. Feizhi is going to help." */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorFaceActor(ACTOR_FEH, ACTOR_MESSENGER_STUDENT, 0);
	fdScriptWait(20);
	/* "Feizhi knows kung fu. She will be all right." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Feizhi is alone. She cannot go too far." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	/* "She will give up soon enough. Until then, let her be." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetSpeed(ACTOR_FEH, 0.8);
	fdActorWalkToAndWait(ACTOR_FEH, 256, 248);
	fdActorFaceDirection(ACTOR_FEH, FACING_NORTH, 20);
	fdActorStartRepeatedMotion(ACTOR_FEH, 1);
	fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorWalkToAndWait(ACTOR_FEH, 240, 184);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	scFuchinTempleSetTreeUpAgain();
	/* "The tree... It fell. What happened?" */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	fdActorWalkToAndWait(ACTOR_MESSENGER_STUDENT, 256, 240);
	fdActorFaceDirection(ACTOR_MESSENGER_STUDENT, FACING_NORTH_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
	/* "The warrior did it. He used Chi." */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorFaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_KICKING_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_PARTY_LEADER, 0);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
	fdActorWalkToAndWait(ACTOR_FEH, 248, 208);
	fdActorFaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
	/* "Warrior, you... Can you use Chi?" */
	fdScriptOpenMessage(ACTOR_FEH, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
		fdScriptWait(20);
		/* "Do it again, warrior. Show me your Chi." */
		fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
		gmFlagSet(FLAG_FEH_WAITS_TO_SEE_CHI);
	} else {
		fdScriptSkipMessages(1);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorFaceDirection(ACTOR_FEH, FACING_SOUTH, 20);
		fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
		fdScriptWait(20);
		/* "Warrior, you speak nonsense. You used Chi. Do it again!" */
		fdScriptOpenMessage(ACTOR_MESSENGER_STUDENT, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
			fdScriptWait(20);
			fdActorFaceDirection(ACTOR_FEH, FACING_NORTH_NORTHWEST, 20);
			/* "Show me your Chi, warrior." */
			fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
			gmFlagSet(FLAG_FEH_WAITS_TO_SEE_CHI);
		} else {
			fdScriptSkipMessages(1);
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
			fdScriptWait(20);
			/* "I want to see your Chi, warrior." */
			fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
			fdScriptSetFlag(FLAG_FEH_CHI_DEMONSTRATION_DECLINED);
		}
	}
	fdActorFaceDirection(ACTOR_KICKING_STUDENT, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PUNCHING_STUDENT, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_KICKING_STUDENT, ANIM_STUDENT_KICKS);
	fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STUDENT_PUNCHES);
	fdActorSetScript(ACTOR_JUMPING_STUDENT, FuchinTempleJumpingStudentScript);
}

/* Isaac talks to Feh in his school after turning him down: "Warrior, will you
   show me your Chi?" */
static void scFuchinTempleFehAsksToSeeChi(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgXianWarriorWillShowMeYour);
	/* "Warrior, will you show me your Chi?" */
	fdScriptOpenMessage(ACTOR_FEH, 0);

	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdScriptWait(20);
		/* "Very good. Now show me." */
		fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
		gmFlagSet(FLAG_FEH_WAITS_TO_SEE_CHI);
		fdScriptEnd();
	} else {
		FieldStateWork->msg += 1;
		/* "What you can do is an incredible thing. Hiding it is not good." */
		fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
		fdScriptEnd();
	}
}

/* The trunk falls to Force from the line with Feh watching, and Feh sees that
   it is not Chi but Fuchin's Ki: energy focused from the mind
   (FLAG_FEH_SAW_KI). */
static void scFuchinTempleFehSeesChi(void)
{
	ACTOR *actor;

	fdActorGet(ACTOR_FEH)->update = 0;
	fdActorGet(ACTOR_PROUD_STUDENT)->update = 0;
	fdActorGet(ACTOR_WATCHING_STUDENT)->update = 0;
	fdActorGet(ACTOR_TREE_STUDENT)->update = 0;
	fdActorGet(ACTOR_MESSENGER_STUDENT)->update = 0;
	fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STAND);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(232, CAMERA_KEEP, 200, 1);
	fdCameraWaitForArrival();
	fdScriptSetMessageId(MsgXianYaTreeFell);
	fdActorSetSpeed(ACTOR_KICKING_STUDENT, 0.8);
	fdActorSetSpeed(ACTOR_JUMPING_STUDENT, 0.8);
	fdActorWalkTo(ACTOR_KICKING_STUDENT, 152, 200);
	fdActorWalkToAndWait(ACTOR_JUMPING_STUDENT, 144, 248);
	fdActorWaitForMoveById(ACTOR_KICKING_STUDENT);
	fdActorFaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorFaceActor(ACTOR_FEH, ACTOR_TRAINING_TREE, 0);
	fdActorSetSpeed(ACTOR_KICKING_STUDENT, 1.5);
	fdActorSetSpeed(ACTOR_JUMPING_STUDENT, 2);
	fdActorWalkTo(ACTOR_KICKING_STUDENT, 152, 200);
	fdActorWalkToAndWait(ACTOR_JUMPING_STUDENT, 144, 248);
	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorWaitForMoveById(ACTOR_KICKING_STUDENT);
	fdActorFaceActor(ACTOR_KICKING_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	/* "Ya! The tree fell!" */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 40);
	fdActorFaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceDirection(ACTOR_PUNCHING_STUDENT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceDirection(ACTOR_PROUD_STUDENT, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEH, 0);
	fdScriptWait(20);
	/* "Master, did you see? The warrior used Chi." */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	/* "The warrior's Chi is excellent! Are you surprised, Master?" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmote(
		ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "What is wrong, Master?" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	/* "Hmmmm... The warrior did make the tree fall. I grant you that..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetSpeed(ACTOR_TREE_STUDENT, 0.8);
	fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 176);
	fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH_SOUTHEAST, 20);
	/* "Is something wrong with the warrior's Chi?" */
	fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
	fdActorFaceDirection(ACTOR_FEH, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "What the warrior did... It is not Chi." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_KICKING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_PUNCHING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_JUMPING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_WATCHING_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_TREE_STUDENT, 2);
	fdActorStartRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
	/* "What? It is not Chi?" */
	fdScriptShowMessageThenWait(ACTOR_PROUD_STUDENT, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	/* "Hmmm... It looks very similar...but it is not Chi." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
	/* "Warrior... Have you been to Fuchin Temple?" Yes: "Ah... I knew it." No:
	   "Hmmm... That is strange." */
	fdScriptAskYesNo(ACTOR_FEH, 0);
	fdActorShowEmote(
		ACTOR_WELCOMING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(ACTOR_KICKING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(
		ACTOR_PUNCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(ACTOR_JUMPING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(ACTOR_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(
		ACTOR_WATCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(5);
	fdActorShowEmote(
		ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
	/* "How are Chi and Fuchin Temple related?" */
	fdScriptShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	/* "I learned my Chi from Master Hama of Lama Temple." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Then it is the same as Chi, is it not?" */
	fdScriptShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
	fdActorFaceActor(ACTOR_FEH, ACTOR_TREE_STUDENT, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Very similar... But still different..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 40);
	fdActorTurnToFaceEachOther(
		ACTOR_PUNCHING_STUDENT, ACTOR_KICKING_STUDENT, 0);
	fdActorTurnToFaceEachOther(
		ACTOR_JUMPING_STUDENT, ACTOR_WATCHING_STUDENT, 0);
	fdActorTurnToFaceEachOther(ACTOR_PROUD_STUDENT, ACTOR_TREE_STUDENT, 0);
	fdScriptWait(60);
	fdActorFaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEH, 0);
	fdActorFaceActor(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
	fdScriptWait(20);
	/* "Chi concentrates the energy of the body into the hand......" */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KICKING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JUMPING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PROUD_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_WATCHING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_TREE_STUDENT, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
	/* "You, warrior, used energy focused from the mind. Understand?" Yes: "This
	   warrior is wise. The power to will something to move..." No: "Remember
	   this... Wishing for something is the same as..." */
	fdScriptAskYesNo(ACTOR_FEH, 0);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	/* "Fuchin's Ki releases the mind's energy like Chi releases the..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KICKING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JUMPING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PROUD_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_WATCHING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_TREE_STUDENT, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	/* "The body's energy has limits, but mental energy has few limits." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	/* "Train your Ki. It will grow very strong. Do your best, warrior." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_FEH, FACING_WEST, 20);
	fdActorRunRepeatedMotion(ACTOR_FEH, 2);
	fdScriptWait(20);
	/* "Practice, everyone! Chi has its limits, but anyone can use..." */
	fdScriptShowMessageThenWait(ACTOR_FEH, 0, 20);
	fdActorSetAnimation(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KICKING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JUMPING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PROUD_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_WATCHING_STUDENT, ANIM_NOD);
	fdActorSetAnimation(ACTOR_TREE_STUDENT, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkTo(ACTOR_KICKING_STUDENT, 120, 200);
	fdActorWalkTo(ACTOR_JUMPING_STUDENT, 120, 248);
	fdActorWaitForMoveById(ACTOR_KICKING_STUDENT);
	fdActorFaceDirection(ACTOR_PUNCHING_STUDENT, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_KICKING_STUDENT, ANIM_STUDENT_KICKS);
	fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STUDENT_PUNCHES);
	fdActorWaitForMoveById(ACTOR_JUMPING_STUDENT);
	fdActorSetScript(ACTOR_JUMPING_STUDENT, FuchinTempleJumpingStudentScript);
	fdActorSetSpeed(ACTOR_TREE_STUDENT, 0.8);
	fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
	fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 232, 168);
	fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_NORTH, 20);
	fdActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
	fdActorSetPos(ACTOR_TRAINING_TREE, 232, 168);
	fdActorGet(ACTOR_TRAINING_TREE)->y.fixed = toFIXED(12);
	fdActorGet(ACTOR_TRAINING_TREE)->tgtY = ACTOR_NO_TARGET;
	actor = fdActorGet(ACTOR_TRAINING_TREE);
	actor->spr->rotation = DEGtoANG(180);
	sySndPlay(SE_CHEST_OPEN);
	fdScriptWait(40);
	fdActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
	fdActorFaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
	gmFlagClear(FLAG_FEH_WAITS_TO_SEE_CHI);
	gmFlagSet(FLAG_FEH_SAW_KI);
}

/* Isaac talks to the student practising punches in Master Feh's school once Hsu
   is known to be trapped: "Water monsters flooded Altin. Did you defeat them,
   warrior?" */
static void scFuchinTempleStudentAsksAboutAltin(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgXianWaterMonstersFloodedAltinDid);
	/* "Water monsters flooded Altin. Did you defeat them, warrior?" Yes: "You
	   are much stronger than you look, warrior. I have much..." No: "Oh... You
	   are not the strong warrior Feizhi spoke of?" */
	fdScriptAskYesNo(ACTOR_PUNCHING_STUDENT, 0);
	fdScriptEnd();
}

/* Isaac walks north into the lattice door Feh and Feizhi came out of, in the
   east of the school: "It is locked." until Feizhi has left; then it opens and
   he walks through into Feh's house. */
static void scFuchinTempleOpenLockedDoor(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU) == 0 &&
		gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
		/* "It is locked." */
		mnFieldMessageShow(MsgXianItIsLocked, FIELD_MESSAGE_ZOOM);
		fdScriptEnd();
	} else {
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridAnimateMetatiles(FuchinTempleFehsDoorOpens, 78, 13);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 306, 248);
		fdActorWalkTo(ACTOR_PARTY_LEADER, 304, 216);
		fdScriptWait(20);
		syImportScriptSetExitRequest(LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE);
		fdScriptEnd();
	}
}

/* Isaac talks to the traveling healer by the temple door: from the south he
   offers his healing, as a Sanctum does; from any other side he says he is
   traveling the world to spread good will. */
static void scFuchinTempleTalkToHealer(void)
{
	Uint32 facing;

	facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	fdScriptStart();

	if(FACING_IS_NORTH(facing)) {
		mnSanctumRun(ACTOR_HEALER);
	} else {
		fdScriptSetMessageId(MsgXianImTravelingAroundWorldSpread);
		/* "I'm traveling around the world to spread good will." */
		fdScriptShowMessage(ACTOR_HEALER, 0);
	}

	fdScriptEnd();
}

/* Isaac reads the withered Nyunpa's mind once he has died enlightened:
   "Isaac... Is that you...? I am...counting on you... to bring..." */
static void scFuchinTempleReadNyunpasLastWish(void)
{
	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
	fdScriptSetMessageId(MsgXianIsaacAmCountingOnBring);
	/* "Isaac... Is that you...? I am...counting on you... to bring..." */
	fdScriptShowMessage(ACTOR_NYUNPA, 0);
	fdScriptEnd();
}

/* The cues outside, in Hama's room, or in the temple and Feh's school. */
const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_FUCHIN_TEMPLE_GROUNDS) {
		return FuchinTempleGroundsCues;
	}
	if(MainRoutineStateWork.entrance ==
		LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
		return FuchinTempleCuesFromLamaTemple;
	}
	return FuchinTempleCues;
}

/* Drops spray over Isaac as he walks in behind the falls, and makes him
   flicker. */
static void scFuchinTempleSpawnGrottoEntryParticle(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *leader;
	Sint32		   paletteDraw;
	Sint32		   fallSpeed;

	leader = fdActorGet(ACTOR_PARTY_LEADER);

	options.pal = SPRITE_COLOR_WHITE;
	paletteDraw = syRandomBelow(7);
	if((paletteDraw & 7) == 0)
		options.pal = SPRITE_COLOR_BRG;

	options.startScaleX = toFIXED(7) / 10;
	options.startScaleY = toFIXED(4) / 5;

	fallSpeed = syRandomBelow(8) * (toFIXED(1) / 5);

	fdEffectSpawn(leader->x.fixed + toFIXED(8 - (TickCount & 15)),
		leader->y.fixed + toFIXED(24), leader->z.fixed, 0, -fallSpeed, 0,
		EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);

	if((TickCount & 1) != 0)
		fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	else
		fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_GRB);
}

/* Unused: hops the leader two map metatiles north; nothing calls it. */
static UNUSED void scFuchinTempleHopLeaderNorth(void)
{
	scHopLeaderBy(0, -32);
}

/* Unused: hops the leader two map metatiles south; nothing calls it. */
static UNUSED void scFuchinTempleHopLeaderSouth(void)
{
	scHopLeaderBy(0, 32);
}

/* Unused: hops the leader two map metatiles west; nothing calls it. */
static UNUSED void scFuchinTempleHopLeaderWest(void)
{
	scHopLeaderBy(-32, 0);
}

#include "shared/leader_hop.c"

/* Isaac steps from the monk's stump into the falls, metatile (10, 21): spray
   falls over him and he flickers out of sight as the screen fades, into Fuchin
   Falls Cave. */
static void scFuchinTempleEnterWaterfallGrotto(void)
{
	EVENT_FUNC particleTask;

	fdScriptStart();
	particleTask = scFuchinTempleSpawnGrottoEntryParticle;
	sySetTickEvent(particleTask, EVENT_PASS_TICK);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
	FieldStateWork->transitionFrm = 60;
	fdScriptFinishScreenTransition();
	sySndPlay(SE_CAST);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	syCloseEvent(particleTask);
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE);
	fdScriptEnd();
}

/* Walking out through the temple's south door, Isaac leaves Nyunpa's hall for
   the grounds outside. */
static void scFuchinTempleLeaveTemple(void)
{
	sySndPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(LACQUER_ROOMS_A_TO_FUCHIN_TEMPLE_GROUNDS);
}

/* Isaac walks north into the temple's door, metatile (18, 18): it opens with a
   thump, and he walks inside, where Nyunpa meditates. */
static void scFuchinTempleEnterTemple(void)
{
	fdScriptStart();
	sySndPlay(SE_THUMP);
	fdMapGridAnimateMetatiles(FuchinTempleTempleDoorOpens, 77, 8);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	fdScriptWait(16);
	syImportScriptSetExitRequest(FUCHIN_TEMPLE_GROUNDS_TO_LACQUER_ROOMS_A);
	fdScriptEnd();
}

/* Back in the temple with the Orb of Force, Isaac walks up to Nyunpa, who
   praises him, names the power he has gained Ki, which the western world calls
   Force, and tells him to use it to find his way through Mogall Forest
   (FLAG_NYUNPA_TAUGHT_FORCE). */
static void scFuchinTempleNyunpaTeachesKi(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 168, 504);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_NYUNPA, 2);
	fdActorSetAttachedEffect(
		ACTOR_NYUNPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorGet(ACTOR_NYUNPA)->paused = 0;
	sySndPlay(SE_JUMP);
	fdActorGet(ACTOR_NYUNPA)->velY = toFIXED(8);
	fdActorSetAnimation(ACTOR_NYUNPA, ANIM_STAND);
	fdScriptSetMessageId(MsgXianExcellentIsaac);
	/* "Excellent, Isaac!" */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	/* "I am glad to see you master the secrets of Fuchin Temple!" */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
	fdScriptWait(20);
	/* "We monks call the power you have obtained Ki." */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "You seem confused, Isaac." */
	fdScriptOpenMessage(ACTOR_NYUNPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
		fdScriptWait(20);
		/* "This is called 'force' by the western world, but it is..." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
		FieldStateWork->msg += 2;
	} else {
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
		fdScriptWait(20);
		FieldStateWork->msg++;
		/* "You possess great wisdom for one so young, but I feel I must..." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
		fdScriptWait(20);
		/* "It is called 'force' by the western world, but it is really..." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	}
	/* "I will teach you, as I promised. Use this power to go..." */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
	fdScriptWait(20);
	/* "What would happen if you let loose this energy in the forest?" */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Ha ha! You are curious, no?" */
	fdScriptOpenMessage(ACTOR_NYUNPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdScriptSetMessageId(MsgXianCanReadMindsKnowCurious);
		/* "I can read minds, you know. You are curious, aren't you?" */
		fdScriptOpenMessage(ACTOR_NYUNPA, 0);
		do {
			if(fdScriptAskYesNoWithActorAnimation(0, 0) != 1) {
				break;
			}
			fdScriptWait(10);
			fdActorShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
			fdScriptSetMessageId(MsgXianDontTryHard);
			/* "You don't have to try so hard. You are curious, are you not?" */
			fdScriptOpenMessage(ACTOR_NYUNPA, 0);
		} while(1);
	}
	fdScriptSetMessageId(MsgXianMonstersWaitInHidingWould);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
	fdScriptWait(20);
	/* "Monsters that wait in hiding would show themselves. You..." */
	fdScriptOpenMessage(ACTOR_NYUNPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		/* "Sometimes, you must let go of your belief that fighting..." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
		FieldStateWork->msg++;
		/* "Ha ha... That is right. There is no good in wanton destruction." */
		fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	}
	fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
	fdScriptWait(20);
	/* "Monsters are cowardly. They will run once they know you can..." */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
	fdScriptWait(20);
	/* "They are creatures of the woods... So they do not get lost..." */
	fdScriptShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
	gmFlagSet(FLAG_NYUNPA_TAUGHT_FORCE);
	fdScriptEnd();
}

/* The first time the party enters Master Hama's room in Lama Temple, the boy
   monk asks them to be quiet while she meditates. Hama teaches Ivan Reveal,
   Feizhi bursts in to say Hsu is in danger, and the two leave to free him
   (FLAG_XIAN_HEARD_HSU_TRAPPED). */
static void scFuchinTempleRunHamasRevealLesson(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 472, 536);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorFaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_EAST, 20);
	fdActorRunRepeatedMotion(ACTOR_HAMAS_BOY_MONK, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgXianMasterHamaMeditatingPleaseExtremely);
	/* "Master Hama is meditating. Please be extremely quiet." */
	fdScriptShowMessageThenWait(ACTOR_HAMAS_BOY_MONK, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH, 30);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "So, Isaac, you and your Adept companions have finally come." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_NORTH_NORTHEAST, 0);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorSetSpeed(ACTOR_IVAN, 0.5);
	fdActorSetSpeed(ACTOR_MIA, 0.5);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 464, 504);
	fdActorWalkTo(ACTOR_IVAN, 480, 504);
	fdActorWalkTo(ACTOR_GARET, 496, 496);
	fdActorWalkTo(ACTOR_MIA, 448, 496);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	/* "Finally come? Was she expecting us?" Yes: "But how did she know we'd be
	   here now?" No: "Then, how come she knew we were coming, Isaac?" */
	fdScriptAskYesNo(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "I had anticipated your arrival." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "You knew we were coming, then?" */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "I have been waiting a long while for you. You come seeking..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "You know about the Elemental Stars, too? Who... Who are you?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 20);
	/* "I am a Jupiter Adept, just like Ivan." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	/* "I'm not a Jupiter Adept! What are you talking about?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "You are, Ivan, although you may not yet know it." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	/* "Why do you say that?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	/* "Do you not have the power to read minds?" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 50);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	/* "This ability is unique to the Jupiter Adepts." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "You predicted our coming... Is that also Psynergy?" */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
	/* "Ho ho! Excellent question, Miss Mia." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "Yes, the ability to foresee scripts also belongs to the..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	/* "Then, can Ivan learn the power of prediction, too?" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_HAMA, ACTOR_IVAN, 0);
	fdScriptWait(10);
	/* "He already has." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "I don't think I have-" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "You do. You merely do not yet know that you have it." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	/* "The power of Jupiter must be great if it lets you see the..." */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorFaceActor(ACTOR_HAMA, ACTOR_GARET, 0);
	fdScriptWait(20);
	/* "Oh, but Ivan can learn to use an even greater power!" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "What greater power?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorFaceActor(ACTOR_HAMA, ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "Reveal." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	/* "Reveal?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "Yes. I have waited long to pass the power of true sight on..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(20);
	/* "You waited here in Lama Temple just to teach Reveal to Ivan?" */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "You will need Reveal if you are to achieve your goals..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Jupiter Adepts can learn Reveal on their own, over time..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_HAMA, 2);
	fdScriptWait(20);
	/* "But that power alone will not help you find Saturos as his..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	/* "It was Saturos and his companions who blocked Silk Road." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(60);
	/* "Felix and the rest of them, huh? Those cowards!" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Felix bears a terrible fate, a burden I would not wish on..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "A terrible fate? What do you mean?" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorStartRepeatedMotion(ACTOR_HAMA, 1);
	fdScriptWait(60);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I am sorry, but I can say nothing more of this matter." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	/* "Now, we must focus on the power of Reveal." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "You will need the power of Reveal to cross the evil Lamakan..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	/* "An evil desert? Is that even possible?" Yes: "You believe that, Isaac?
	   Well, I just don't buy it." No: "It's hard to believe, isn't it, Isaac? I
	   feel the same way." */
	fdScriptAskYesNo(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	/* "Why will we need Reveal to cross the desert?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_NORTH, 20);
	/* "Crossing Lamakan Desert was hard enough with the extreme heat." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH, 20);
	/* "But evil has taken hold of the desert, and its heat has..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Entering the desert now is nothing short of asking for death." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Then how can Saturos cross such a deadly place?" */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "Saturos and Menardi are Mars Adepts." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	/* "I am merely guessing... but their powers might help them..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	/* "I still don't understand... How will Reveal help us in such..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
	fdScriptWait(20);
	/* "Reveal will help you find oases to cool you against the heat." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	/* "Why would we need Reveal for that? Won't we be able to see..." */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
	fdActorFaceActor(ACTOR_MIA, ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(20);
	/* "Do you know what a mirage is?" Yes: "The desert is filled with mirages
	   that prevent you from..." No: "Intense heat creates a curtain of air that
	   distorts the..." */
	fdScriptAskYesNo(ACTOR_HAMA, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	/* "Sounds like we should have Ivan learn Reveal, don't you think?" Yes:
	   "Yeah! I don't want to die in any evil desert!" No: "Oh, come on, Isaac!
	   If we're going to follow Felix, we'll..." */
	fdScriptAskYesNo(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
	/* "Ivan, you should let Master Hama teach you how to use Reveal." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
	fdActorFaceDirection(ACTOR_HAMA, FACING_EAST, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "What's wrong, Master Hama? You can pass Reveal on to me,..." */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_NORTH, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "I am unsure that it will work." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "In preparation for this day, I tried to pass a power on to a..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "My student was not an Adept..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_HAMA, 1);
	fdScriptWait(20);
	/* "But I thought anyone could hone a skill he or she already..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "However, I saw no change in the girl." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	/* Feizhi runs in from the south. */
	fdActorSetPos(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
	/* "No! There was change!" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(
		ACTOR_HAMAS_BOY_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_EAST, 0);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0.8);
	fdActorWalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 536);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Feizhi... Why are you here?" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
	fdActorWalkToAndWait(ACTOR_IVAN, 488, 512);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 456, 512);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 2);
	fdScriptWait(20);
	/* "I sensed it!" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Sensed it? What did you sense?" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 2);
	fdActorSetAttachedEffect(ACTOR_FEIZHI_IN_LAMA_TEMPLE,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	/* "Hsu is in great danger!" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Hsu left this place a long time ago." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(
		ACTOR_FEIZHI_IN_LAMA_TEMPLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "I know... But Hsu did not return to Xian." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_HAMA, 1);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	/* "...The boulders in the mountains!" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, ANIM_NOD);
	fdScriptWait(20);
	/* "I sensed it. I came through the mine. I came to ask for help." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I sensed nothing, as I was focused on Isaac's arrival..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "But why has the power come to you now, after all this time?" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	/* BUG: ACTOR_FEIZHI is Feizhi's number in the school's table. Hama's room
	   places no actor at that number, so Feizhi does not bounce before she
	   speaks. */
#if FIX_HAMAS_ROOM_FEIZHI_BOUNCE
	fdActorStartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 1);
#else
	fdActorStartRepeatedMotion(ACTOR_FEIZHI, 1);
#endif
	fdScriptWait(20);
	/* "Gems fell from the sky. One hit me right on the head." */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "Gems..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Psynergy Stones..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(2);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(1);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(5);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorFaceActor(ACTOR_HAMA, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	/* "Then this may work after all." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetSpeed(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0.8);
	fdActorWalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 504);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 1);
	fdScriptWait(20);
	/* "What about Hsu?" */
	fdScriptShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
	fdActorFaceActor(ACTOR_HAMA, ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0);
	fdActorSetSpeed(ACTOR_HAMA, 0.8);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_HAMA);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "Let us hurry to Hsu's aid!" */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorWalkTo(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 568);
	fdActorWalkToAndWait(ACTOR_HAMA, 472, 536);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorWaitForMoveById(ACTOR_FEIZHI_IN_LAMA_TEMPLE);
	/* "Master Hama, what about us? What about Reveal?" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_HAMA, 2);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(
		ACTOR_FEIZHI_IN_LAMA_TEMPLE, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_HAMA, FACING_NORTH_NORTHEAST, 20);
	/* "I had completely forgotten." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorWalkToAndWait(ACTOR_HAMA, 472, 512);
	fdActorFaceDirection(ACTOR_HAMA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "It seems I succeeded in passing my power to Feizhi. You will..." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 448, 512);
	fdScriptWait(1);
	{
		ACTOR *leaderActor = fdActorGet(ACTOR_PARTY_LEADER);
		leaderActor->steeringFlg =
			ACTOR_FACES_MOTION | leaderActor->steeringFlg;
	}
	fdScriptWait(20);
	scFuchinTempleAnimateRevealLesson();
	fdScriptWait(60);
	fdPartyLearnAbility(IVAN, ABILITY_REVEAL);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "This should do it. Use Reveal to cross the desert." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_HAMA, 472, 552);
	fdActorRunRepeatedMotion(ACTOR_HAMA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_HAMA, FACING_NORTH_NORTHEAST, 20);
	/* "But your Reveal may not be that strong yet... The oases may not
	   appear." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdScriptWait(20);
	/* "The rest is up to Ivan's abilities." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetSpeed(ACTOR_HAMA, 0.5);
	fdActorWalkToAndWait(ACTOR_HAMA, 480, 540);
	/* "Ivan, I am very glad to see you have grown to be such a strong man." */
	fdScriptShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetSpeed(ACTOR_HAMA, 0.8);
	fdActorSetSpeed(ACTOR_HAMAS_BOY_MONK, 0.8);
	fdActorWalkTo(ACTOR_HAMA, 472, 608);
	fdActorWalkTo(ACTOR_HAMAS_BOY_MONK, 472, 544);
	fdActorWalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
	fdActorWaitForMoveById(ACTOR_HAMAS_BOY_MONK);
	fdActorWalkTo(ACTOR_HAMAS_BOY_MONK, 472, 608);
	fdActorSetPos(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 0);
	fdActorWaitForMoveById(ACTOR_HAMA);
	fdActorSetPos(ACTOR_HAMA, 0, 0);
	fdActorWaitForMoveById(ACTOR_HAMAS_BOY_MONK);
	fdActorSetPos(ACTOR_HAMAS_BOY_MONK, 0, 0);
	fdActorWalkToAndWait(ACTOR_IVAN, 488, 520);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
	/* "Ivan, do you and Master Hama... know each other?" */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "No... But she doesn't feel like a stranger..." */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	/* "Felix and the others must be far ahead of us by now..." Yes: "Then we'd
	   better get moving! Let's go!" No: "The desert may have slowed them down,
	   but we can't waste a second!" */
	fdScriptAskYesNo(ACTOR_GARET, 0);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	/* "We can't just leave like this!" */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Hama might need our help..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "She only has Feizhi to assist her. Shouldn't we help, too?" */
	fdScriptOpenMessage(ACTOR_MIA, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(20);
		/* "I'm more worried about Felix and the others... But I'm not opposed
		   to rescuing Hsu." */
		fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
		FieldStateWork->msg++;
	} else {
		fdScriptWait(20);
		FieldStateWork->msg++;
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(20);
		/* "I am worried about Feizhi and Master Hama... But I understand why
		   you want to chase after Felix." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	}
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	/* "We'll go with you, Isaac, whatever you decide!" */
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorWalkTo(ACTOR_GARET, 448, 512);
	fdActorWalkToAndWait(ACTOR_MIA, 448, 512);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 20);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 20);
	/* "I wonder what Master Hama was talking about..." */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Oh, I'm sorry... Let's go." */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorWalkToAndWait(ACTOR_IVAN, 448, 512);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdScriptEnd();
	gmFlagSet(FLAG_XIAN_HEARD_HSU_TRAPPED);
}

/* Sets each room up as the story left it. */
Sint32 scInit(void)
{
	ACTOR *trainingTree;
	Sint32 heardHsuTrapped;

	if(MainRoutineStateWork.next == SCENE_LACQUER_ROOMS_A) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		if(MainRoutineStateWork.entrance ==
			LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS) {
			if(gmFlagGet(FLAG_NYUNPA_ENLIGHTENED)) {
				fdActorSetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_WITHERED);
			} else {
				fdActorSetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
				if(gmFlagGet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
					!gmFlagGet(FLAG_NYUNPA_TAUGHT_FORCE) &&
					!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
					scFuchinTempleNyunpaTeachesKi();
				}
			}
		} else if(MainRoutineStateWork.entrance ==
				LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN ||
			MainRoutineStateWork.entrance ==
				LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE) {
			gmFlagClear(FLAG_SHOW_AREA_NAME);
			heardHsuTrapped = gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED);
			if(heardHsuTrapped == 0) {
				trainingTree = fdActorGet(ACTOR_TRAINING_TREE);
				trainingTree->motionFlg = heardHsuTrapped;
				trainingTree->y.fixed = toFIXED(12);
				trainingTree->tgtY = toFIXED(12);
				trainingTree->scaleX = toFIXED(4) / 5;
				trainingTree->scaleY = toFIXED(1) / 2;
				trainingTree->spr->rotation = DEGtoANG(180);
				if(gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU)) {
					fdActorSetPos(ACTOR_FEH, 248, 208);
					if(!gmFlagGet(FLAG_FEH_SAW_KI)) {
						fdActorSetPos(ACTOR_MESSENGER_STUDENT, 256, 240);
						fdActorGet(ACTOR_FEH)->update =
							scFuchinTempleFaceLeader;
						fdActorGet(ACTOR_PROUD_STUDENT)->update =
							scFuchinTempleFaceLeader;
						fdActorGet(ACTOR_WATCHING_STUDENT)->update =
							scFuchinTempleFaceLeader;
						fdActorGet(ACTOR_TREE_STUDENT)->update =
							scFuchinTempleFaceLeader;
						fdActorGet(ACTOR_MESSENGER_STUDENT)->update =
							scFuchinTempleFaceLeader;
					}
				}
			} else {
				trainingTree = fdActorGet(ACTOR_TRAINING_TREE);
				trainingTree->motionFlg = 0;
				trainingTree->y.fixed = toFIXED(12);
				trainingTree->tgtY = toFIXED(12);
				trainingTree->scaleX = toFIXED(4) / 5;
				trainingTree->scaleY = toFIXED(1) / 2;
				trainingTree->collisionFlg |= ACTOR_NOT_TARGETED;
				trainingTree->spr->rotation = DEGtoANG(180);
				fdMapGridCopyMetatileAttributes(14, 11, 1, 1, 14, 10);
			}
			/* A crate painted solid black and translucent, over the lattice
			   door to Feh's house (LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE). The
			   school's render is the same with and without it; drawn plain, it
			   covers the door. XXX (no evidence): why it is there; nothing the
			   player sees shows what it was for. */
			fdActorPrepare(304, 24, 224, SPRITE_CRATE);
			fdActorSetAnimation(ACTOR_KICKING_STUDENT, ANIM_STUDENT_KICKS);
			fdActorSetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STUDENT_PUNCHES);
		} else if(MainRoutineStateWork.entrance ==
			LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
			gmFlagClear(FLAG_SHOW_AREA_NAME);
			if(!gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
				scFuchinTempleRunHamasRevealLesson();
			} else if(!gmFlagGet(FLAG_HSU_RESCUED)) {
				fdActorSetPos(ACTOR_HAMA, 0, 0);
				fdActorSetPos(ACTOR_HAMAS_BOY_MONK, 0, 0);
			}
		}
	} else {
		fdQueueMusic(SE_AMBIENT_WATER);
		fdActorGet(ACTOR_FALLS_MONK)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
		if(MainRoutineStateWork.entrance ==
				FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE &&
			gmFlagGet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
			!gmFlagGet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST)) {
			fdMapGridCopyMetatileAttributes(10, 84, 1, 1, 10, 24);
		}
		if(gmFlagGet(FLAG_FUCHIN_FALLS_GROTTO_OPENED)) {
			fdActorSetPos(ACTOR_FALLS_MONK, 152, 392);
			fdActorFaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
			fdMapGridCopyMetatileAttributes(10, 26, 1, 1, 10, 22);
		}
	}
	return 0;
}

/* Starts Mind Read from caster on target as part of a script, without the
   window that announces its name. */
static void scFuchinTempleStartMindReadDemonstration(Sint32 caster, Sint32 tgt)
{
	FIELD_EFFECT_STATE *effectState;

	gmFlagSet(FLAG_PLAY_PSYNERGY_CAST);
	fdPsynergyLoadResources(ABILITY_MIND_READ, 1);
	effectState = FieldEffectStateWork;
	fdPsynergySetupActorPair(caster, tgt);
	effectState->announceName = 0;
	fdPsynergyStartCast();
	fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
	sySynch(1);
}

/* Finishes the demonstration's Mind Read and lowers the caster's hands. */
static void scFuchinTempleFinishMindReadDemonstration(void)
{
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_FINISH);
	fdPsynergyStopCasterUpdate();
}

/* Eight crescents of light rise from Nyunpa, ten ticks apart, as he glows and
   calls to the monk at the falls; every second one sounds. */
static void scFuchinTempleSpawnNyunpasParticles(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *nyunpa;
	Uint32		   shotIdx;

	nyunpa = fdActorGet(ACTOR_NYUNPA);
	options.pri = 1;
	options.type = SPRITE_CRESCENT;
	options.script = FuchinTempleShotScript;
	options.tgtScaleX = toFIXED(7) / 2;
	options.tgtScaleY = toFIXED(3) / 2;
	for(shotIdx = 0; shotIdx < 8; shotIdx++) {
		fdScriptWait(10);
		if(shotIdx & 1) {
			sySndPlay(SE_PSYNERGY_START);
		}
		fdEffectSpawn(nyunpa->x.fixed, nyunpa->y.fixed,
			nyunpa->z.fixed - toFIXED(24), 0, toFIXED(3) / 5, 0,
			EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_SCALE_TO_TARGET |
				EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM,
			&options);
	}
	fdScriptWait(60);
}

/* Hama's and Ivan's update function during the Reveal lesson: they glow, drawn
   red and blue in turn every two ticks. */
static void scFuchinTempleGlowRedAndBlue(ACTOR *updated)
{
	if((TickCount >> 1) & 1) {
		fdActorSetSprCol(updated, SPRITE_COLOR_RED_TINT);
	} else {
		fdActorSetSprCol(updated, SPRITE_COLOR_BLUE_TINT);
	}
}

/* The sparkles that rise from Hama, two ticks apart, as the Reveal lesson
   ends. */
#define REVEAL_LESSON_SPARKLES 64

/* Hama and then Ivan glow red and blue while the room darkens to purple, then
   sparkles rise from Hama and the room's colours come back. */
static void scFuchinTempleAnimateRevealLesson(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *hama;
	Uint32		   particleIdx;
	FIXED		   particleX;
	FIXED		   particleY;
	Sint32		   riseSpeed;

	sySndPlay(SE_PSYNERGY_SPARKLE);
	fdActorGet(ACTOR_HAMA)->update = scFuchinTempleGlowRedAndBlue;
	fdScriptWait(40);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 2, 23), PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(40);
	sySndPlay(SE_PSYNERGY_SPARKLE);
	fdActorGet(ACTOR_IVAN)->update = scFuchinTempleGlowRedAndBlue;
	fdScriptWait(120);
	hama = fdActorGet(ACTOR_HAMA);
	options.pri = 1;
	options.pal = SPRITE_COLOR_RBG;
	options.type = SPRITE_SPARKLE;
	for(particleIdx = 0; particleIdx < REVEAL_LESSON_SPARKLES; particleIdx++) {
		if((particleIdx & 3) == 0) {
			sySndPlay(SE_GLITTER);
		}
		particleX = hama->x.fixed + toFIXED(syRandomBelow(48)) - toFIXED(12);
		particleY = hama->y.fixed + toFIXED(syRandomBelow(32)) - toFIXED(16);
		riseSpeed = syRandomBelow(4) * (toFIXED(1) / 2) + toFIXED(1) / 2;
		fdEffectSpawn(particleX, particleY, hama->z.fixed, 0, riseSpeed, 0,
			EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
			&options);
		sySynch(2);
	}
	sySndPlay(SE_SHIMMER);
	fdScriptWait(30);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(40);
	fdActorGet(ACTOR_HAMA)->update = NULL;
	fdActorGet(ACTOR_IVAN)->update = NULL;
	fdActorSetSprColById(ACTOR_HAMA, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_IVAN, SPRITE_COLOR_NORMAL);
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

/* The student who trains by jumping: three hops, each higher, spinning a full
   turn in the third, then a second's rest. */
Sint32 FuchinTempleJumpingStudentScript[] = {
	SC_ANIMATION(ANIM_STILL),
	SC_SET(SW_GRAVITY, toFIXED(3) / 5),
	SC_LABEL(1),
	SC_SET(SW_VELOCITY_Y, toFIXED(3)),
	SC_WAIT(30),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_WAIT(30),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(2),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(2),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* The point Force strikes on the training tree: no shadow. */
Sint32 FuchinTempleHiddenSpriteScript[] = {
	SC_CALL(scFuchinTempleClearScriptedActorSprFlags),
	SC_WAIT(1),
	SC_END,
};

/* The doors' map metatiles, tick by tick: Fuchin Temple's door opening as
   Isaac walks in (scFuchinTempleEnterTemple), and the door Feh and Feizhi come
   out of in Master Feh's school, opening and closing behind them. */
Uint16 FuchinTempleTempleDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 5),
	MAPGRID_ANIMATION_STEP(63, 46, 3, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
Uint16 FuchinTempleFehsDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(62, 19, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(64, 19, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
Uint16 FuchinTempleFehsDoorCloses[] = {
	MAPGRID_ANIMATION_STEP(62, 19, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(60, 19, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};

SCENE_ENTRANCE FuchinTempleGroundsEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP, CONDITION_ALWAYS, 296, 0, 504,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{FUCHIN_TEMPLE_GROUNDS_FROM_LACQUER_ROOMS_A, CONDITION_ALWAYS, 296, 0, 304,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS,
		168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, CONDITION_ALWAYS, 232, 0, 328,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE FuchinTempleEntrancesOther[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 168, 0, 552,
		FACING_NORTH, 0, 48, 384, 288, 576, 0},
	{LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN, CONDITION_ALWAYS, 256, 0, 264,
		FACING_NORTH, 0, 48, 80, 368, 288, 0},
	{LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE, CONDITION_ALWAYS, 472, 0, 552,
		FACING_NORTH, 0, 352, 384, 592, 576, 0},
	{LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE, CONDITION_ALWAYS, 304, 0, 248,
		FACING_SOUTH, 0, 48, 80, 368, 288, 0},
	{SCENE_TABLE_END},
};

Uint32 FuchinTempleExits[] = {
	SCENE_EXITS(FuchinTempleGrounds),
	SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP, WorldMap,
		WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS),
	SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_LACQUER_ROOMS_A, LacquerRoomsA,
		LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS),
	SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE,
		FuchinFallsCaveEntrance,
		FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS),
	SCENE_EXITS(LacquerRoomsA),
	SCENE_EXIT(LACQUER_ROOMS_A_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds,
		FUCHIN_TEMPLE_GROUNDS_FROM_LACQUER_ROOMS_A),
	SCENE_EXIT(LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN, Xian,
		XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL),
	SCENE_EXIT(LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE, LamaTemple,
		LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM),
	SCENE_EXIT(LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE, LacquerRoomsB,
		LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE FuchinTempleGroundsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		296, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MONK_IN_WHITE_AND_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		272, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 392, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE FuchinTempleActorPlacements[] = {
	/* Nyunpa */
	ACTOR_PLACEMENT(SPRITE_NYUNPA, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		168, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
	/* Master Feh's students */
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		216, 0, 264, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		120, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		152, 0, 152, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, CONDITION_IN_VIEW, (Sint32)FuchinTempleJumpingStudentScript,
		120, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		176, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		168, 0, 264, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		216, 0, 152, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		176, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
	/* Feizhi and Master Feh, out of sight until they come out */
	ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FEH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	/* The training tree, and where Force strikes it */
	ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		232, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (Sint32)FuchinTempleHiddenSpriteScript,
		232, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE FuchinTempleActorPlacementsFromLamaTemple[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	/* Hama */
	ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		472, 0, 472, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		440, 0, 536, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE FuchinTempleGroundsCues[] = {
	{ CUE_EXIT, METATILE_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_ROAD, CONDITION_ALWAYS, FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FUCHIN_TEMPLE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleEnterTemple) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_GROTTO, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleEnterWaterfallGrotto) },
	{ CUE_STEP, METATILE_BEHAVIOR_FUCHIN_FALLS_MONK_ASKS, FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST, SCRIPT_REF(scFuchinTempleMonkAsksWhetherTestIsDone) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleTheFuchinTempleIsAPleasant) },
	{ CUE_TALK, ACTOR_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleOhIsaacWhatTookYouSo) },
	{ CUE_TALK, ACTOR_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleAhhhMasterNyunpaHeHasPassed) },
	{ CUE_TALK, ACTOR_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaHasWastedAwayAt) },
	{ CUE_TALK, ACTOR_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), SCRIPT_REF(scFuchinTempleBoyMonkSpeaksOfNyunpa) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleYoungManFromKolimaAsksAboutBridge) },
	/* Unused: a copy of the row above, which always answers first. */
	{ CUE_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleYoungManFromKolimaAsksAboutBridge) },
	{ CUE_TALK, ACTOR_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleOmOmBlueLightsInThe) },
	{ CUE_TALK, ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, SCRIPT_REF(scFuchinTempleMonkLetsPartyIntoGrotto) },
	{ CUE_TALK, ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleTheTestOfFuchinFallsIs) },
	{ CUE_TALK, ACTOR_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterNyunpaIsWaitingForYou) },
	{ CUE_TALK, ACTOR_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGemsFromTheSkyBlueLights) },
	{ CUE_TALK, ACTOR_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleNyunpaHasNotEatenInDays) },
	{ CUE_TALK, ACTOR_ADMIRING_BOY_MONK, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleBoyMonkSpeaksOfNyunpa) },
	{ CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleTalkToHealer) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FUCHIN_TEMPLE_PSYNERGY_STONE, FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleGemsFallingFromTheSkyMountains) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleIKnewMasterNyunpaWasFasting) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaReachedEnlightenmentDuringHis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaIsNothingButSkin) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleHeCantLeaveUsYetHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FROM_KOLIMA, FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN, MSGNO(MsgFuchinTempleIBeginningToThinkIllBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDontFeelSafeGoingHome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleManyStarsFellFromTheSky) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheWarriorsHaveMasteredTheSecrets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThereIsNoPointInTraining) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICantImagineFastingItGets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ADMIRING_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWillTrainToBecomeA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleSinceImHereIdLikeTo) },
	{ CUE_TABLE_END },
};

CUE FuchinTempleCues[] = {
	{ STEP_CUE_FACING(FACING_SOUTH), METATILE_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleLeaveTemple) },
	{ CUE_EXIT, METATILE_BEHAVIOR_FEHS_SCHOOL_SOUTH_DOOR, CONDITION_ALWAYS, LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FEHS_SCHOOL_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleOpenLockedDoor) },
	{ CUE_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTemple) },
	{ CUE_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleUseThisEnergyToFindThe) },
	{ CUE_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), MSGNO(MsgFuchinTempleSpeakToTheMonkStandingBeneath) },
	{ CUE_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED), SCRIPT_REF(scFuchinTempleNyunpaOffersTest) },
	{ CUE_TALK, ACTOR_NYUNPA, CONDITION_ALWAYS, MSGNO(MsgXianNyunpaMeditates) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), SCRIPT_REF(scFuchinTempleReadNyunpasLastWish) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleNowThatIHavePassedMy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_CAST | ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), SCRIPT_REF(scFuchinTempleNyunpaFeelsMindRead) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_CAST | ACTOR_NYUNPA, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleNyunpaOffersTest) },
	{ CUE_TALK, ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleThisIsMasterFehsKungFu) },
	{ CUE_TALK, ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleBouldersOvercameHsuKungFuShould) },
	{ CUE_TALK, ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), SCRIPT_REF(scFuchinTempleStudentAsksAboutAltin) },
	{ CUE_TALK, ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), SCRIPT_REF(scFuchinTempleTalkToJumpingStudent) },
	{ CUE_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleFeizhiPredictedHsusDangerHerProphecy) },
	{ CUE_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleHsuIsRestingFeizhiIsWatching) },
	{ CUE_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTemplePracticeEveryoneChiHasItsLimits) },
	{ CUE_TALK, ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouAreGreatYouHave) },
	{ CUE_TALK, ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleEveryoneWantsToLearnTheChi) },
	{ CUE_TALK, ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleConcentrateEnergyIntoTheHandsAnd) },
	{ CUE_TALK, ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), SCRIPT_REF(scFuchinTempleTalkToJumpingStudent) },
	{ CUE_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDidNotThinkMuchOf) },
	{ CUE_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWhereDidFeizhiGoIAm) },
	{ CUE_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHsuIsLateICan) },
	{ CUE_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIWillLearnChiIWill) },
	{ CUE_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorUseYourChiFromThe) },
	{ CUE_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorPleaseUseYourChiFrom) },
	{ CUE_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorLetMeSeeYourChi) },
	{ CUE_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerous) },
	{ CUE_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooks) },
	{ CUE_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowUsYourChiLike) },
	{ CUE_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorThatWasGreatButMaster) },
	{ CUE_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), SCRIPT_REF(scFuchinTempleFehAsksToSeeChi) },
	{ CUE_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerousOutside) },
	{ CUE_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooksShe) },
	{ CUE_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIKnowWhyYouCanUse) },
	{ CUE_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorChiIsAGreatPower) },
	{ CUE_TALK, ACTOR_WELCOMING_STUDENT, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleStudentWelcomesToSchool) },
	{ CUE_TALK, ACTOR_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleStrongKicksAreVeryImportantKick) },
	{ CUE_TALK, ACTOR_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWantToLearnSoI) },
	{ CUE_TALK, ACTOR_JUMPING_STUDENT, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleTalkToJumpingStudent) },
	{ CUE_TALK, ACTOR_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleEveryoneInXianCanDoMartial) },
	{ CUE_TALK, ACTOR_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleJustListeningDoesNotMakeOne) },
	{ CUE_TALK, ACTOR_TREE_STUDENT, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleStudentAsksAboutFeh) },
	{ CUE_TALK, ACTOR_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsVeryBusyHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTemplePeaceHasReturnedToXianBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleNoMatterHowMuchYouPractice) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleTheMonstersAreVeryLargeStriking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhenABoulderFallsJumpRepeat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhereDidFeizhiGainThePower) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleMasterFehIsVeryWorriedAbout) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleCanSuchAYoungWarriorReally) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleYourChiIsFuchinTemplesHow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDontCareWhichChiIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHamasChiCanShatterBoulders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFollowSilkRoadStraightPassThrough) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouUseWeaponsAndChi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFeizhiIsNotInTheFeh) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFeizhiWorriesOnlyAboutMasterHsu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleLearningChiIsVeryHardMaster) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorTheRulesRequireYouTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleSoCloseWarriorUseChiFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowMeYourChiI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIKnowIKnowHow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleMyKickAndPunchAreVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleShowUsYourChiNowDo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleIAmShockedThatThisWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIWantToSeeTheChi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIKnowHowYouFeel) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleMyKickAndPunchAreVeryLoud) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorShowMeYourChiNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorDoNotHideYourChi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsFightWithWeaponsKungFu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTemplePracticingKicksHurtsMyFeetVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThatPunchWasGoodYouSurprise) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUMPING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIMoveTooQuicklyYouCant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsCameToLearnTheFeh) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGreatKickKickFromTheHip) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleYouAreTheWarriorsWhoCame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsTalkingWithFeizhi) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_FEHS_SCHOOL_TREE_EAST_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTemplePushTreeByHand) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_FEHS_SCHOOL_TREE_EAST_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTemplePushTreeByHand) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_FEHS_SCHOOL_TREE_WEST_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTemplePushTreeByHand) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_FEHS_SCHOOL_TREE_WEST_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTemplePushTreeByHand) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAINING_TREE, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleMoveTree) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_TRAINING_TREE_TARGET, CONDITION_ALWAYS, SCRIPT_REF(scFuchinTempleForceTree) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_FEHS_SCHOOL_ANTIDOTE_BARREL, FLAG_FUCHIN_TEMPLE_ANTIDOTE_BARREL, FIND_ITEM(ITEM_ANTIDOTE) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_FUCHIN_TEMPLE_UNICORN_RING_JAR, FLAG_FUCHIN_TEMPLE_UNICORN_RING_JAR, FIND_ITEM(ITEM_UNICORN_RING) },
	{ CUE_TABLE_END },
};

CUE FuchinTempleCuesFromLamaTemple[] = {
	{ CUE_EXIT, METATILE_BEHAVIOR_HAMAS_ROOM_SOUTH_DOOR, CONDITION_ALWAYS, LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE },
	{ CUE_TALK, ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
	{ CUE_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_HAMAS_ROOM_COINS_JAR, FLAG_HAMAS_ROOM_COINS_JAR, FIND_COINS(6) },
	{ CUE_TABLE_END },
};
/* clang-format on */

/* One of Nyunpa's crescents (scFuchinTempleSpawnNyunpasParticles): it rises
   facing north for 45 ticks, then is put away. */
Sint32 FuchinTempleShotScript[] = {
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(44),
	SC_WAIT(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};
