/* Vale (SceneId_ValePlaza): the part of the village with the Psynergy Stone and
   the town gate, Vale's way out to the world.

   The scene sees the prologue end and the journey begin. scGetActorPlacements
   and scGetCues pick its people and scripts by the time of the story. In story
   order:

   - The stormy night of the prologue (FLAG_VALE_STORM), when the villagers
	 have gathered in the plaza here. Isaac and Garet come from the river
	 (Vale, SceneId_ValePrologue, by VALE_PROLOGUE_TO_VALE_PLAZA), where
	 Felix has fallen in. Walking east toward the villagers gathered by the
	 Psynergy Stone's pond (metatiles (10, 47..53)) plays
	 scValePlazaSentToAidElders. Garet's grandfather, the mayor, sends
	 two strongmen to aid the elders holding the Boulder. Then he sends Isaac
	 and Garet back to show Jenna and a strongman whose Psynergy the Psynergy
	 Stone has restored the way to the river
	 (FLAG_VALE_STORM_SENT_TO_AID_ELDERS). Others have spent their Psynergy,
	 and one lifts a small rock to show he has a little left
	 (scValePlazaBoyLiftsSmallRock).
   - Later the same night, the Boulder has swept Kyle, Jenna's parents and
	 Felix away and Garet has gone after Isaac (FLAG_VALE_STORM_GARET_LEFT).
	 Arriving here then plays scValePlazaMeetSurvivingTravelers. Isaac
	 overhears two strangers saying only the two of them survived Sol
	 Sanctum's fury. Garet catches up, and the strangers fight the boys to
	 make them forget (scValePlazaFightTravelers). Win or lose, the battle
	 returns to VALE_PLAZA_AFTER_TRAVELERS_BATTLE, where the strangers leave
	 the boys lying in the storm. The storm ends (FLAG_VALE_STORM is
	 cleared), and VALE_PLAZA_TO_TITLE leads to the title shown after the
	 prologue, which opens on Vale three years later (scene/title.c). The
	 game names the two only three years later, as Saturos and Menardi
	 (vale_kradens_cottage.c). Their sprites here are the same.
   - Three years later, the day the story begins. The villagers talk of
	 meditating on Mt. Aleph, of the travelers who stopped by, and of the
	 storm, in which Isaac lost his father and Jenna her whole family. One
	 shows off his Psynergy, shaking the ground
	 (scValePlazaDemonstratePsynergy).
   - The farewell (VALE_PLAZA_FROM_SANCTUM), once Isaac and Garet have
	 accepted the Wise One's quest in the Vale Sanctum (sanctum.c).
	 scValePlazaDepartForQuest restores the party's HP and PP and plays the
	 village seeing them off at the town gate. Garet's sister gives a Herb.
	 His brother gives the Catch Beads that Aunty Dora sent, for Dora herself
	 does not come. VALE_PLAZA_TO_WORLD_MAP leads out to the World Map.
   - After the party has left (FLAG_VALE_PARTY_LEFT), the villagers point the
	 way to Vault and talk of the eruption, whose sparkles they take for
	 crushed Psynergy Stones. A large Psynergy Stone has fallen on the hut,
	 whose roof scInit redraws. Only now does the town gate lead
	 out (scValePlazaLeaveThroughTownGate).
   - Back from Kalay (FLAG_KALAY_REACHED), the villagers welcome Isaac home.
	 The hut's owner finds the stone gone, leaving a hole in his roof
	 (scValePlazaPsynergyStoneIsGone), and a door in the north-east opens.

   Searching the Psynergy Stone, a purple crystal on a rock in the pond in the
   middle of the plaza (metatile (13, 44)), restores the party's PP at every
   time but the farewell.

   Three clumps of ivy grow against the cliffs: one behind the shop, hiding the
   mouth of a small cave, one at the foot of the same cliff a little east of it,
   and one on the cliff behind the inn. Whirlwind blows each away for good.

   The ways out of the scene:

   - Six doors lead into three houses and the shop (SceneId_CottageRoomsA),
	 and into the forge and the inn (SceneId_CottageRoomsB). A seventh, a
	 doorway in the north-east, opens only once the party is back from Kalay.
   - The path off the west edge, at metatiles (0, 20..21), leads to the rest
   of  the village (SceneId_Vale) or, on the stormy night, back to the
   storm's  scene (SceneId_ValePrologue).
   - The town gate at the south edge, metatiles (12..14, 54), leads out to
   the  World Map once the party has set out.
   - Behind the ivy that hides it, the cave's mouth at metatile (11, 28)
   leads into  a small cave with a chest at its far end, which holds a Power
   Bread.

   The cave is drawn on Goma Cave's map (SceneId_GomaCaveStream,
   GOMA_CAVE_STREAM_CHEST_CAVE_FROM_VALE_PLAZA), walled off from the rest of it,
   and no other way leads in or out. The party comes back out one metatile south
   of the mouth (VALE_PLAZA_FROM_CHEST_CAVE, at (186, 474)).

   Three debug entrances are in no entrance table and no exit leads to them:

   - VALE_PLAZA_DEBUG_BEFORE_TRAVELERS_LEAVE replays the strangers'
	 departure.
   - VALE_PLAZA_DEBUG_CHILDHOOD sets FLAG_CHILDHOOD and takes
	 VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTHEAST into Isaac's house
	 (SceneId_CottageRoomsB).
   - VALE_PLAZA_DEBUG_STORY_FLAGS sets a preset of story flags and takes
	 VALE_PLAZA_TO_BILIBIN (scValePlazaSetProgressFlagsAndExit).

   The hut's owner (ACTOR_HUT_OWNER) is never seen by day after the prologue.
   scInit hides the storm's Jenna by number without testing FLAG_VALE_STORM, and
   by day that number is his (a BUG, fixed by BUGFIX).

   A villager showing Psynergy flashes white (scValePlazaFlashPsynergyPalette).
   By day that is the strongman in scValePlazaDemonstratePsynergy, and on the
   stormy night it is scValePlazaBoyLiftsSmallRock. He then cycles through his
   colour modes while pairs of sparks rise and fall about him every 16 ticks
   (scValePlazaCyclePsynergyPaletteAndSpawnEffects). */

#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/script_psynergy.h"
#include "scene/walk_into_leader.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

Sint32				  scInit(void);
extern Sint32		  ValePlazaChargedUpStrongmanActions[];
extern Sint32		  ValePlazaSecondTravelerLeapActions[];
extern Sint32		  ValePlazaFirstTravelerLeapActions[];
extern Sint32		  ValePlazaLeaderActions[];
extern Sint32		  ValePlazaStormGaretLeapActions[];
extern Sint32		  ValePlazaFirstTravelerLeapAwayActions[];
extern Sint32		  ValePlazaSecondTravelerLeapAwayActions[];
extern Sint32		  ValePlazaFaceTargetActions[];
extern Sint32		  ValePlazaFarewellDogActions[];
extern Sint32		  ValePlazaFarewellChaseActions[];
extern Sint32		  ValePlazaFarewellFirstChildActions[];
extern Sint32		  ValePlazaFarewellSecondChildActions[];
extern Sint32		  ValePlazaPartyDepartureActions[];
extern Sint32		  ValePlazaHutOwnerSearchActions[];
extern Sint32		  ValePlazaAideRunActions[];
extern Sint32		  ValePlazaLeaderStepActions[];
extern Sint32		  ValePlazaStormGaretStepActions[];
extern SCENE_ENTRANCE ValePlazaEntrances[];
extern Uint32		  ValePlazaExits[];
extern Uint32		  ValePlazaStormExits[];
extern ACTOR_PLACE	  ValePlazaActorPlacements[];
extern ACTOR_PLACE	  ValePlazaStormActorPlacements[];
extern ACTOR_PLACE	  ValePlazaDepartureActorPlacements[];
extern ACTOR_PLACE	  ValePlazaActorPlacementsAfterKalay[];
extern CUE			  ValePlazaCues[];
extern CUE			  ValePlazaStormCues[];
extern CUE			  ValePlazaDepartureCues[];
extern CUE			  ValePlazaCuesAfterLeaving[];
extern CUE			  ValePlazaCuesAfterKalay[];
extern Uint16		  ValePlazaDoorOpens[];

/* Vale's area flags, kept while the party stays in Vale's scenes. Back from
   Kalay, a young man has welcomed Isaac home. */
#define FLAG_VALE_WELCOMED_HOME			   FLAG_AREA_TEMP_02
/* Back from Kalay, the old man searching for the Psynergy Stone has told Isaac
   it is gone. */
#define FLAG_VALE_PSYNERGY_STONE_GONE_TOLD FLAG_AREA_TEMP_06

/* Young Garet sprawled on his back, beaten, held on its one picture
   (garet_child.png). */
enum { ANIM_GARET_CHILD_SPRAWLED = 7 };

/* The travelers' leap away after the battle, the same in Saturos's and
   Menardi's sprites (saturos.png, menardi.png). */
typedef enum TRAVELER_ANIMATION {
	TRAVELER_ANIMATION_CROUCH = 5, /* crouched low before the spring */
	TRAVELER_ANIMATION_LEAP = 6	   /* in the air, cape flying */
} TRAVELER_ANIMATION;

/* The metatiles the plaza's cues answer, besides its doors, whose behavior is
   their exit's number. */
typedef enum VALE_PLAZA_METATILE_BEHAVIOR {
	/* The town gate at the plaza's south edge, metatiles (12..14, 54). */
	METATILE_BEHAVIOR_VALE_PLAZA_TOWN_GATE = 8,
	/* The path off the plaza's west edge, metatiles (0, 20..21). */
	METATILE_BEHAVIOR_VALE_PLAZA_WEST_PATH,
	/* On the stormy night, the line west of the Psynergy Stone's pond where the
	   villagers gather, metatiles (10, 47..53). */
	METATILE_BEHAVIOR_VALE_PLAZA_GATHERED_VILLAGERS = 20,
	/* The Psynergy Stone, metatile (13, 44). */
	METATILE_BEHAVIOR_VALE_PLAZA_PSYNERGY_STONE = 100
} VALE_PLAZA_METATILE_BEHAVIOR;

/* Each actor placement table numbers its own rows from ACTOR_FIRST_PLACED,
   Garet keeping his own party slot. The same people stand in several tables
   under other numbers. The three ivy actors come first in every table. */
typedef enum VALE_PLAZA_ACTOR_ID {
	/* Ivy, drawn half as tall again. It is the sprite of the ivy that holds the
	   stump in goma_cave_entrance.c ("It looks like it's trapped in the ivy.").
	   This clump grows on the cliff behind the shop and hides the small cave's
	   mouth. */
	ACTOR_FIRST_IVY = ACTOR_FIRST_PLACED,
	/* At the foot of the cliff behind the shop, a little east of the first. */
	ACTOR_SECOND_IVY,
	/* On the cliff behind the inn, by its chimney. */
	ACTOR_THIRD_IVY
} VALE_PLAZA_ACTOR_ID;

/* By day (ValePlazaActorPlacements, before and after the party leaves). The
   notes quote the day the story begins. */
typedef enum VALE_PLAZA_DAY_ACTOR_ID {
	/* An old man with a cane: "The Valeans are a chosen people." On the stormy
	   night, after the party has left and back from Kalay, he has a question
	   for Isaac instead (scValePlazaAskAboutJourney). After the party has left
	   it is "You saw the Wise One?" */
	ACTOR_CURIOUS_OLD_MAN = ACTOR_FIRST_PLACED + 3,
	/* In the old woman's sprite, who on the stormy night asks "You expect an
	   old woman like me to use Psynergy?" By day: "Long ago, my grandfather was
	   a powerful Adept... I'm so proud of him." */
	ACTOR_OLD_WOMAN,
	/* A young man with brown hair: "Well, if it isn't Isaac. It's just another
	   quiet day in Vale." */
	ACTOR_YOUNG_MAN_QUIET_DAY,
	/* A woman with fair hair: "Kraden had visitors when I dropped by his place
	   this morning." */
	ACTOR_WOMAN_ON_KRADENS_VISITORS,
	/* Two children, a small boy with fair hair and a little girl with orange
	   pigtails. By day the boy asks "Did the travelers that stopped by today go
	   to meet the mayor?" Back from Kalay he thinks "I wonder if we can play
	   with Isaac and his pals again, like we used to." */
	ACTOR_FIRST_CHILD,
	/* The little girl: "With Psynergy, the more you practice, the more you
	   learn." */
	ACTOR_SECOND_CHILD,
	/* A strongman who shakes the ground with his Psynergy
	   (scValePlazaDemonstratePsynergy). "Hey, Isaac, have I shown you my new
	   ability?" */
	ACTOR_STRONGMAN_SHOWING_PSYNERGY,
	/* A second strongman: "If your Psynergy runs low, just walk around. You'll
	   recover it slowly." */
	ACTOR_STRONGMAN_WALK_TO_RECOVER,
	/* An invisible actor (as in gondowan_cave.c), out of sight until the party
	   has left. Then it stands on the large Psynergy Stone that fell on the
	   hut, for the hut's owner to turn to. */
	ACTOR_FALLEN_STONE_SPOT = ACTOR_FIRST_PLACED + 13,
	/* A burly grey-haired man: "Isaac! Have you remembered to meditate on Mt.
	   Aleph daily?" */
	ACTOR_MAN_ASKING_ABOUT_MEDITATION = ACTOR_FIRST_PLACED + 15,
	/* A woman in a green dress: "You had a very difficult time three years ago,
	   didn't you?" */
	ACTOR_WOMAN_RECALLING_STORM,
	/* The hut's owner, an old man with a cane, who back from Kalay finds "a big
	   hole in my roof". "Since food is stored in this hut, it isn't scary at
	   all..."

	   Once the prologue is over, scInit hides him by day as if he were the
	   storm's ACTOR_STORM_JENNA (a BUG), so only a debug jump past the prologue
	   shows him. The renders show him by the hut with no story flags set and
	   gone once the party has left. */
	ACTOR_HUT_OWNER = ACTOR_FIRST_PLACED + 18
} VALE_PLAZA_DAY_ACTOR_ID;

/* On the stormy night (ValePlazaStormActorPlacements). */
typedef enum VALE_PLAZA_STORM_ACTOR_ID {
	/* The same old man as ACTOR_CURIOUS_OLD_MAN: "Huh? Are you...asking me if I
	   can use Psynergy, Isaac?" */
	ACTOR_STORM_CURIOUS_OLD_MAN = ACTOR_FIRST_PLACED + 3,
	/* The old woman: "You expect an old woman like me to use Psynergy?" */
	ACTOR_STORM_OLD_WOMAN,
	/* A young man with brown hair: "I'm exhausted. I used up all my Psynergy
	   deflecting falling rocks..." */
	ACTOR_STORM_ROCK_DEFLECTING_YOUNG_MAN,
	/* A woman with fair hair: "Oh! Little Felix has fallen into the river? That
	   must be why Jenna looked so upset." */
	ACTOR_STORM_WOMAN_HEARING_OF_FELIX,
	/* A small boy with fair hair who lifts ACTOR_STORM_SMALL_ROCK
	   (scValePlazaBoyLiftsSmallRock). "I have some Psynergy left." */
	ACTOR_STORM_ROCK_LIFTING_BOY,
	/* A little girl with orange pigtails: "Jenna went off that way!" */
	ACTOR_STORM_GIRL_WHO_SAW_JENNA,
	/* The two strongmen Garet's grandfather sends to aid the elders. "We can't
	   let the Mt. Aleph Boulder destroy our village." */
	ACTOR_STORM_FIRST_AIDE,
	ACTOR_STORM_SECOND_AIDE, /* "Let's go help the elders." */
	/* Garet's grandfather, the mayor, an old man with white hair and a cane.
	   Garet calls him "Grandpa!", and his sprite is cottage_rooms_a.c's. "Go
	   aid the elders!" */
	ACTOR_STORM_GARETS_GRANDFATHER,
	/* The storm's rock sprite at three tenths of its size. */
	ACTOR_STORM_SMALL_ROCK,
	/* The small rock's shadow. It is a blank actor on ACTOR_STORM_SMALL_ROCK's
	   spot at three fifths of its size, whose sprite draws only its shadow,
	   while the setup turns off the rock's own.

	   It blocks nothing the rock does not already block, and nothing moves it,
	   so the shadow stays on the ground as the boy lifts the rock. */
	ACTOR_STORM_SMALL_ROCK_SHADOW,
	/* Rested at the Psynergy Stone, the strongman vale_prologue.c calls
	   ACTOR_EXHAUSTED_STRONGMAN. "Great! I'm all charged up!" */
	ACTOR_STORM_CHARGED_UP_STRONGMAN,
	/* The two travelers who survived Sol Sanctum, Saturos and Menardi by their
	   sprites (as in vale_kradens_cottage.c). "Only the two of us
	   survived..." */
	ACTOR_FIRST_TRAVELER,
	ACTOR_SECOND_TRAVELER,
	/* Garet as a child, who follows Isaac through the night. "Isaac! Wait!" */
	ACTOR_STORM_GARET,
	ACTOR_STORM_JENNA /* "Garet, Isaac... You came to help me?" */
} VALE_PLAZA_STORM_ACTOR_ID;

/* At the farewell (ValePlazaDepartureActorPlacements). */
typedef enum VALE_PLAZA_FAREWELL_ACTOR_ID {
	/* The small boy of ACTOR_FIRST_CHILD, at the top of the path. "What, didn't
	   you hear? Isaac and Garet are leaving Vale on a journey." */
	ACTOR_FAREWELL_FIRST_CHILD = ACTOR_FIRST_PLACED + 3,
	/* The little girl of ACTOR_SECOND_CHILD: "Why's everyone hanging around the
	   town gate?" */
	ACTOR_FAREWELL_SECOND_CHILD,
	/* The Great Healer, by his sprite (as in vale_eruption.c and sanctum.c).
	   "We are counting on you, Isaac. Oh, and you, too, Garet." */
	ACTOR_FAREWELL_GREAT_HEALER,
	/* The man with long brown hair and a beard in a long robe of vale.c's
	   ACTOR_MAN_GUARDING_MT_ALEPH: "Hmm... Dora is very late..." */
	ACTOR_FAREWELL_MAN_AWAITING_DORA,
	/* Garet's grandfather, the old man with white hair and a cane. "They can't
	   keep standing around here like this, or they'll never leave." */
	ACTOR_FAREWELL_GARETS_GRANDFATHER,
	/* Garet's sister, the young woman with an orange ponytail, who gives the
	   Herb. "I know not to drink stagnant water, Sis!", Garet answers her. */
	ACTOR_FAREWELL_GARETS_SISTER,
	/* Garet's brother, the small boy with orange hair of vale.c's
	   ACTOR_GARETS_BROTHER, who brings the Catch Beads: "Aunty Dora asked me to
	   give this to you, Isaac." Of Garet: "He's a pig! He'll eat anything
	   that's not tied down!" */
	ACTOR_FAREWELL_GARETS_BROTHER,
	/* A woman with orange hair under a green headscarf: "As Garet's mother, I
	   know how Dora must be feeling." */
	ACTOR_FAREWELL_GARETS_MOTHER,
	/* Garet's father, a man with brown hair in a teal shirt, by his sprite
	   (cottage_rooms_a.c's). */
	ACTOR_FAREWELL_GARETS_FATHER,
	/* The rest of the crowd at the gate, ten who only nod and jump, by their
	   sprites: an old man with a cane, an old woman, a young man with brown
	   hair, a woman with fair hair, three strongmen, a burly grey-haired man, a
	   woman in a green dress and a stout man. */
	ACTOR_FAREWELL_OLD_MAN,
	ACTOR_FAREWELL_OLD_WOMAN,
	ACTOR_FAREWELL_YOUNG_MAN,
	ACTOR_FAREWELL_FAIR_HAIRED_WOMAN,
	ACTOR_FAREWELL_FIRST_STRONGMAN,
	ACTOR_FAREWELL_SECOND_STRONGMAN,
	ACTOR_FAREWELL_THIRD_STRONGMAN,
	ACTOR_FAREWELL_BURLY_MAN,
	ACTOR_FAREWELL_WOMAN_IN_GREEN,
	ACTOR_FAREWELL_STOUT_MAN,
	/* The sprite of the dog of vault.c ("Woof! Woof!"), which runs past the
	   children and later after ACTOR_FAREWELL_CAT. */
	ACTOR_FAREWELL_DOG,
	/* A small cat set down where the dog stops, which runs off with the dog
	   after it. No other scene places its sprite. */
	ACTOR_FAREWELL_CAT
} VALE_PLAZA_FAREWELL_ACTOR_ID;

/* Back from Kalay (ValePlazaActorPlacementsAfterKalay). */
typedef enum VALE_PLAZA_AFTER_KALAY_ACTOR_ID {
	/* A young man with brown hair: "Hey, if it isn't Isaac! You came back home,
	   huh?" */
	ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED + 3,
	/* A woman with fair hair: "Oh, Isaac! You came back sooner than I
	   thought!" */
	ACTOR_AFTER_KALAY_WOMAN_BACK_SOONER,
	/* A burly grey-haired man: "It looks like you made some friends in your
	   travels, Isaac." */
	ACTOR_AFTER_KALAY_MAN_ON_NEW_FRIENDS,
	/* A woman in a green dress: "The river in town grew so hot after Mt. Aleph
	   erupted." */
	ACTOR_AFTER_KALAY_WOMAN_ON_HOT_RIVER,
	/* The same old man as ACTOR_CURIOUS_OLD_MAN: "Did you see anything
	   interesting on your trip, Isaac?" */
	ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN,
	ACTOR_AFTER_KALAY_OLD_WOMAN, /* "Oh, my! Isaac! How have you been!" */
	/* The small boy of ACTOR_FIRST_CHILD: "Welcome back, Isaac! It's good to
	   have you home!" */
	ACTOR_AFTER_KALAY_FIRST_CHILD,
	/* The little girl of ACTOR_SECOND_CHILD: "I see you made many friends on
	   your journey." */
	ACTOR_AFTER_KALAY_SECOND_CHILD,
	/* A strongman: "The eruption created a terrible mess on the other side of
	   Mt. Aleph." */
	ACTOR_AFTER_KALAY_STRONGMAN_ON_ERUPTION,
	/* A second strongman: "The eruption caused a crack to open on Mt. Aleph's
	   northwest side." */
	ACTOR_AFTER_KALAY_STRONGMAN_ON_CRACK,
	/* The hut's owner, the old man with a cane of ACTOR_HUT_OWNER, searching
	   the ground: "Hey! Where'd that Psynergy Stone go?" */
	ACTOR_AFTER_KALAY_HUT_OWNER
} VALE_PLAZA_AFTER_KALAY_ACTOR_ID;

static void scValePlazaMeetSurvivingTravelers(void);
static void scValePlazaFightTravelers(void);
static void scValePlazaTravelersLeaveAfterBattle(void);
static void scValePlazaDepartForQuest(void);
static void scValePlazaSayThenWait(Sint32 speaker, Sint32 ticks);
static void scValePlazaFlashPsynergyPalette(ACTOR *actor);
static void scValePlazaFaceDirectionAndWait(
	Sint32 actorId, Sint32 facing, Sint32 ticks);
static void scValePlazaSpawnEffectPair(ACTOR *parent);
static void scValePlazaUpdateRockLiftingBoyPsynergy(void);
static void scValePlazaUpdateShowingStrongmanPsynergy(void);
static void scValePlazaUpdateSmallRockPalette(void);
static void scValePlazaSetProgressFlagsAndExit(void);

#include "shared/actor_facing.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return ValePlazaEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* Returns the stormy night's exits while FLAG_VALE_STORM is set, else the
   day's. */
const Uint32 *scGetExits(void)
{
	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		return ValePlazaStormExits;
	}
	return ValePlazaExits;
}

/* Selects the actor placements: the stormy night's, the farewell's
   (VALE_PLAZA_FROM_SANCTUM), after Kalay, else the day's. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		return ValePlazaStormActorPlacements;
	}
	if(MainRoutineStateWork.entrance == VALE_PLAZA_FROM_SANCTUM) {
		return ValePlazaDepartureActorPlacements;
	}
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		return ValePlazaActorPlacementsAfterKalay;
	}
	return ValePlazaActorPlacements;
}

/* The PP a Psynergy Stone gives each party member, as the field's own stones
   give it (field_mode_cues.c). */
#define PP_RESTORED_BY_STONE 999

/* Searching the Psynergy Stone in the middle of Vale's plaza restores the
   party's PP. */
static void scValePlazaUsePsynergyStone(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgValeCheckedThePsynergyStone, 1);
	sySndPlay(SE_RECOVERY);
	fdPartyAddPp(PP_RESTORED_BY_STONE, 0);
	fdScriptWait(10);
	mnFieldMessageShow(MsgValePartyPpRestored, 1);
	mnMessageCloseFinishedWindows();
	gmFlagClear(FLAG_NOTHING_HAPPENS);
	fdScriptEnd();
}

/* Selects the scripts: after Kalay, after the party has left, the farewell's
   (VALE_PLAZA_FROM_SANCTUM), the stormy night's, else the day's. */
const CUE *scGetCues(void)
{
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		return ValePlazaCuesAfterKalay;
	}
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		return ValePlazaCuesAfterLeaving;
	}
	if(MainRoutineStateWork.entrance == VALE_PLAZA_FROM_SANCTUM) {
		return ValePlazaDepartureCues;
	}
	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		return ValePlazaStormCues;
	}
	return ValePlazaCues;
}

/* A burly grey-haired man on the shore in front of the Psynergy Stone asks
   whether Isaac meditates on Mt. Aleph each day. He stands there on the day the
   story begins. */
static void scValePlazaAskAboutMeditation(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeMeditateOnMtAlephDaily);
	fdActorFaceEachOther(
		ACTOR_MAN_ASKING_ABOUT_MEDITATION, ACTOR_PARTY_LEADER, 2);
	/* "Isaac! Have you remembered to meditate on Mt. Aleph daily?" Yes: "That's
	   very good of you! Meditating helps keep the peace in..." No: "For shame,
	   Isaac! Mt. Aleph keeps watch over Vale. Meditate..." */
	fdScriptAskYesNo(ACTOR_MAN_ASKING_ABOUT_MEDITATION, 0);
	fdScriptEnd();
}

/* A woman in a green dress, by the door of the inn, recalls the difficult time
   three years ago and asks Isaac about it, on the day the story begins. */
static void scValePlazaRecallThreeYearsAgo(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeADifficultTimeThreeYears);
	fdActorFaceEachOther(ACTOR_WOMAN_RECALLING_STORM, ACTOR_PARTY_LEADER, 2);
	/* "You had a very difficult time three years ago, didn't you?" Yes: "You
	   lost your father... Jenna lost her whole family... It..." No: "You're a
	   strong lad, Isaac, so you were able to deal with it..." */
	fdScriptAskYesNo(ACTOR_WOMAN_RECALLING_STORM, 0);
	fdScriptEnd();
}

/* A small boy playing below the shop asks whether the travelers went to meet
   the mayor. */
static void scValePlazaAskAboutTheTravelers(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeDidTheTravelersMeetThe);
	fdActorFaceEachOther(ACTOR_FIRST_CHILD, ACTOR_PARTY_LEADER, 2);
	/* "Did the travelers that stopped by today go to meet the mayor?" Yes: "I
	   told them where Kraden lives, and they didn't even thank..." No: "It's
	   very polite to visit the mayor when coming to a new..." */
	fdScriptAskYesNo(ACTOR_FIRST_CHILD, 0);
	fdScriptEnd();
}

/* The strongman's tremor shifts the view a pixel, each way in turn, every this
   many ticks. */
#define TREMOR_STEP_TICKS 10

/* A strongman south-east of the Psynergy Stone's pond, near the town gate,
   shows Isaac his Psynergy and shakes the ground. */
static void scValePlazaDemonstratePsynergy(void)
{
	Uint32	frm;
	Sint32	msg;
	Sint32	shakes;
	Sint32 *origin;

	fdScriptStart();
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		msg = MsgValeArentWorriedCrossing;
		fdScriptSetMessageId(msg);
		/* "...with just the two of you?", three or four: the flag numbered for
		   a character is set once that character has joined. */
		if(gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0) {
			FieldStateWork->msg++;
		}
		if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
			FieldStateWork->msg++;
		}
		fdScriptOpenMessage(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptSetMessageId(msg + 3);
		} else {
			fdScriptSetMessageId(msg + 4);
		}
		fdScriptShowMessage(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
	} else {
		origin = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
		fdScriptSetMessageId(MsgValeShownNewAbility);
		fdActorFaceEachOther(
			ACTOR_STRONGMAN_SHOWING_PSYNERGY, ACTOR_PARTY_LEADER, 0);
		/* "Hey, Isaac, have I shown you my new ability?" Yes: "It takes hard
		   work to reach this level of mastery, Isaac." No: "Really? Well,
		   here's a little sample for you!" */
		fdScriptAskYesNo(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
		fdScriptWait(20);
		fdActorStartRepeatedMotion(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 2);
		fdScriptWait(15);
		scStartScriptPsynergy();
		shakes = 0;
		for(frm = 0; frm < 40; frm++) {
			scValePlazaFlashPsynergyPalette(
				fdActorGet(ACTOR_STRONGMAN_SHOWING_PSYNERGY));
			sySynch(1);
		}
		sySetTickEvent(
			scValePlazaUpdateShowingStrongmanPsynergy, EVENT_PASS_TICK);
		sySndPlay(SE_TREMOR);
		for(frm = 0; frm != 180; frm++) {
			if(syMathRemainderUnsigned(frm, TREMOR_STEP_TICKS) == 0) {
				if((1 & shakes) != 0) {
					*origin -= toFIXED(1);
				} else {
					*origin += toFIXED(1);
				}
				shakes++;
			}
			fdScriptWait(1);
		}
		sySndPlay(SOUND_FADE_OUT_ACTION_SE);
		syCloseEvent(scValePlazaUpdateShowingStrongmanPsynergy);
		sySynch(1);
		scEndScriptPsynergy();
		fdActorSetSprColById(
			ACTOR_STRONGMAN_SHOWING_PSYNERGY, SPRITE_COLOR_NORMAL);
		fdScriptWait(40);
		fdScriptSetMessageId(MsgValeBeholdPowerPsynergy);
		/* "Behold, the power of Psynergy! Hahh! Impressive, isn't it?" */
		fdScriptShowMessage(ACTOR_STRONGMAN_SHOWING_PSYNERGY, 0);
	}
	fdScriptEnd();
}

/* An old man with a cane asks Isaac a question that changes with the story. */
static void scValePlazaAskAboutJourney(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		fdScriptSetMessageId(MsgValeAnythingInterestingOnYourTrip);
		/* "Did you see anything interesting on your trip, Isaac?" */
		fdScriptOpenMessage(ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
			/* "You didn't, hmm? I can see in your eyes that you're not..." */
			fdScriptShowMessage(ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
		} else {
			FIELD_STATE *state = FieldStateWork;

			state->msg++;
			fdScriptAskYesNo(ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, 0);
		}
	} else if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		fdScriptSetMessageId(MsgValeYouSawTheWiseOne);
		/* "You saw the Wise One? The same Wise One from the healers' tales?"
		   Yes: "That is a blessing... A blessing, indeed." No: "It was the Wise
		   One who saved the town when Mt. Aleph erupted." */
		fdScriptAskYesNo(ACTOR_CURIOUS_OLD_MAN, 0);
	} else {
		fdScriptSetMessageId(MsgValeCanIUsePsynergy);
		/* "Huh? Are you...asking me if I can use Psynergy, Isaac?" Yes: "If I
		   were ten years younger, I'd have plenty to offer, but..." No: "I
		   thought not... You have to be young and strong to use..." */
		fdScriptAskYesNo(ACTOR_STORM_CURIOUS_OLD_MAN, 0);
	}
	fdScriptEnd();
}

/* An old man shows Isaac the Psynergy Stone that fell on his hut.

   Reached by talking to ACTOR_HUT_OWNER after the party has left Vale. In a
   play-through scInit has hidden him by then (a BUG). So only the debug menus,
   setting FLAG_VALE_PARTY_LEFT without the prologue's flags, reach it. */
static void scValePlazaTalkAboutFallenStone(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_HUT_OWNER, ANIM_STAND);
	fdActorFaceActor(ACTOR_HUT_OWNER, ACTOR_PARTY_LEADER, 20);
	fdActorFaceActor(ACTOR_HUT_OWNER, ACTOR_FALLEN_STONE_SPOT, 40);
	fdScriptSetMessageId(MsgValeTheStoneFellOnThe);
	scValePlazaSayThenWait(ACTOR_HUT_OWNER, 20);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(337, CAMERA_KEEP, 272, 1);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_HUT_OWNER, 2);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_HUT_OWNER, ACTOR_PARTY_LEADER, 10);
	scValePlazaSayThenWait(ACTOR_HUT_OWNER, 40);
	fdActorSetMovementTypeById(ACTOR_HUT_OWNER, MOVEMENT_TYPE_WANDER);
	fdScriptEnd();
}

/* In Vale on the stormy night, a small boy with some Psynergy left lifts a
   small rock (ACTOR_STORM_SMALL_ROCK) into the air. */
static void scValePlazaBoyLiftsSmallRock(void)
{
	Uint32 frm;
	Sint32 savedMotionFlg;
	ACTOR *rock;

	fdScriptStart();
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 82, 760);
	fdActorFaceEachOther(ACTOR_STORM_ROCK_LIFTING_BOY, ACTOR_PARTY_LEADER, 30);
	fdScriptSetMessageId(MsgValeIHaveSomePsynergyLeft);
	scValePlazaSayThenWait(ACTOR_STORM_ROCK_LIFTING_BOY, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_STORM_ROCK_LIFTING_BOY, FACING_NORTHWEST, 20);
	fdActorSetAttachedEffect(ACTOR_STORM_ROCK_LIFTING_BOY,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* The boy flashes white as he calls up his Psynergy, then he and the rock
	   shift colours as it rises. */
	scStartScriptPsynergy();
	for(frm = 0; frm < 40; frm++) {
		scValePlazaFlashPsynergyPalette(
			fdActorGet(ACTOR_STORM_ROCK_LIFTING_BOY));
		sySynch(1);
	}
	sySetOrUpdateTickEvent(
		scValePlazaUpdateRockLiftingBoyPsynergy, EVENT_PASS_TICK);
	sySetTickEvent(scValePlazaUpdateSmallRockPalette, EVENT_PASS_TICK);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	/* The rock floats up a little each tick, kept from falling back. */
	rock = fdActorGet(ACTOR_STORM_SMALL_ROCK);
	savedMotionFlg = rock->motionFlg;
	rock->motionFlg = 0;
	for(frm = 0; frm < 40; frm++) {
		rock->y.fixed += toFIXED(3) / 32;
		sySynch(1);
	}
	rock->motionFlg = savedMotionFlg;
	syCloseEvent(scValePlazaUpdateRockLiftingBoyPsynergy);
	syCloseEvent(scValePlazaUpdateSmallRockPalette);
	sySynch(1);
	sySndPlay(SE_RATTLE);
	fdActorSetSprColById(ACTOR_STORM_ROCK_LIFTING_BOY, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_STORM_SMALL_ROCK, SPRITE_COLOR_NORMAL);
	fdScriptWait(40);
	scEndScriptPsynergy();
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STORM_ROCK_LIFTING_BOY, 30);
	fdScriptShowMessage(ACTOR_STORM_ROCK_LIFTING_BOY, 0);
	fdScriptEnd();
}

/* On the stormy night, a little girl in the crowd tells Isaac which way Jenna
   went. */
static void scValePlazaTalkAboutHelping(void)
{
	fdScriptStart();
	fdActorFaceEachOther(
		ACTOR_STORM_GIRL_WHO_SAW_JENNA, ACTOR_PARTY_LEADER, 10);
	if(gmFlagGet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
		fdScriptSetMessageId(MsgValeSureHelp);
		/* "Be sure to help Felix." */
		fdScriptShowMessage(ACTOR_STORM_GIRL_WHO_SAW_JENNA, 0);
	} else {
		fdScriptSetMessageId(MsgValeWentOffWay);
		/* "Jenna went off that way!" */
		fdScriptShowMessage(ACTOR_STORM_GIRL_WHO_SAW_JENNA, 0);
	}
	fdScriptEnd();
}

/* In Vale back from Kalay, a young man welcomes Isaac home. Later he only says
   this is Vale. */
static void scValePlazaWelcomeIsaacHome(void)
{
	fdScriptStart();
	if(fdScriptGetFlag(FLAG_VALE_WELCOMED_HOME) != 0) {
		fdScriptSetMessageId(MsgValeThisIsVale);
	} else {
		fdScriptSetMessageId(MsgValeYouCameBackHome);
		gmFlagSet(FLAG_VALE_WELCOMED_HOME);
	}
	fdScriptShowMessage(ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN, 0);
	fdScriptEnd();
}

/* Back from Kalay, the hut's owner, searching the ground, says the Psynergy
   Stone that fell on his hut is gone. */
static void scValePlazaPsynergyStoneIsGone(void)
{
	ACTOR *actor = fdActorGet(ACTOR_AFTER_KALAY_HUT_OWNER);
	fdScriptStart();
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	fdActorSetAnimation(ACTOR_AFTER_KALAY_HUT_OWNER, ANIM_STAND);
	fdActorStop(ACTOR_AFTER_KALAY_HUT_OWNER);
	fdActorShowEmote(
		ACTOR_AFTER_KALAY_HUT_OWNER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	actor->facing = FACING_NORTH_NORTHWEST;
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_AFTER_KALAY_HUT_OWNER, 2);
	fdScriptSetMessageId(MsgValeThePsynergyStoneIsGone);
	/* "Hey! Where'd that Psynergy Stone go? It's gone!" */
	fdScriptShowMessageThenWait(ACTOR_AFTER_KALAY_HUT_OWNER, 0, 40);
	fdActorFaceActor(ACTOR_AFTER_KALAY_HUT_OWNER, ACTOR_PARTY_LEADER, 20);
	fdActorStartRepeatedMotion(ACTOR_AFTER_KALAY_HUT_OWNER, 2);
	/* "All that's left is a big hole in my roof. But the memory of..." */
	fdScriptShowMessage(ACTOR_AFTER_KALAY_HUT_OWNER, 0);
	gmFlagSet(FLAG_VALE_PSYNERGY_STONE_GONE_TOLD);
	fdActorStop(ACTOR_AFTER_KALAY_HUT_OWNER);
	sySynch(1);
	fdActorSetScript(
		ACTOR_AFTER_KALAY_HUT_OWNER, ValePlazaHutOwnerSearchActions);
	fdScriptEnd();
}

#include "shared/vale.c"

/* Opens a door's map grid metatiles and sends the leader through its
   threshold. */
static inline void scValePlazaOpenDoorAndWalkThrough(
	Sint32 metatileX, Sint32 metatileZ, Sint32 x, Sint32 z)
{
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePlazaDoorOpens, metatileX, metatileZ);
	fdActorWalkTo(ACTOR_PARTY_LEADER, x, z);
}

/* The plaza's doors: walking north into a door's metatile opens it and takes
   the exit of the metatile's number (scValeRequestBackdropExit), into the
   houses (SceneId_CottageRoomsA) or the inn and the forge
   (SceneId_CottageRoomsB). */

/* Isaac opens the door of the house at the top of the plaza (metatile (15, 18))
   and walks in (SceneId_CottageRoomsA,
   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH). */
static void scValePlazaOpenNorthwestDoor(void)
{
	scValePlazaOpenDoorAndWalkThrough(44, 7, 248, 279);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH);
}

/* Back from Kalay, Isaac opens the doorway in the north-east of the plaza
   (metatiles (21..22, 19)) with a thump and walks into a room of sacks and
   crates with weapons on its wall (SceneId_CottageRoomsB,
   COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHEAST). */
static void scValePlazaOpenNortheastDoor(void)
{
	sySndPlay(SE_THUMP);
	fdMapGridCopyMetatilesTo(0, 63, 51, 8, 2, 2);
	sySynch(10);
	fdMapGridCopyMetatilesTo(2, 63, 51, 8, 2, 2);
	sySynch(10);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 352, 306);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 352, 296);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTH);
}

/* Isaac opens the door of the western house of the middle row (metatile (14,
   26)) and walks in (SceneId_CottageRoomsA,
   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST). */
static void scValePlazaOpenMiddleWestDoor(void)
{
	scValePlazaOpenDoorAndWalkThrough(43, 15, 230, 407);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST);
}

/* Isaac opens the door of the eastern house of the middle row (metatile (23,
   27)) and walks in (SceneId_CottageRoomsA,
   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST). */
static void scValePlazaOpenMiddleEastDoor(void)
{
	scValePlazaOpenDoorAndWalkThrough(52, 18, 374, 419);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST);
}

/* Isaac opens the door of the shop, under its sign of a potion and a sword
   (metatile (12, 35)), and walks in (SceneId_CottageRoomsA,
   COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH). */
static void scValePlazaOpenSouthwestDoor(void)
{
	scValePlazaOpenDoorAndWalkThrough(41, 32, 200, 546);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH);
}

/* Isaac opens the door of the forge at the plaza's west edge (metatile (6, 39))
   and walks in (SceneId_CottageRoomsB,
   COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHWEST). */
static void scValePlazaOpenWestEdgeDoor(void)
{
	scValePlazaOpenDoorAndWalkThrough(35, 36, 102, 611);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST);
}

/* Isaac opens the door of the inn (metatile (22, 42)) and walks in
   (SceneId_CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_WEST). */
static void scValePlazaOpenSoutheastDoor(void)
{
	scValePlazaOpenDoorAndWalkThrough(51, 39, 358, 670);
	scValeRequestBackdropExit(VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH);
}

/* On the stormy night, Isaac leaves the plaza by the path off its west edge
   (metatiles (0, 20..21)), back to the river and the rest of the village. */
static void scValePlazaLeaveThroughWestPassage(void)
{
	sySndPlay(SE_MAP_EXIT);
	scValeRequestBackdropExit(VALE_PLAZA_TO_VALE);
}

/* Walking out through the town gate at the plaza's south edge (metatiles
   (12..14, 54)) leads to the World Map, once the party has set out on its
   journey. Until then the gate does not lead out. */
static void scValePlazaLeaveThroughTownGate(void)
{
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		sySndPlay(SE_MAP_EXIT);
		scValeRequestBackdropExit(VALE_PLAZA_TO_WORLD_MAP);
	}
}

/* Whirlwind on the ivy on the cliff behind the shop blows it away, uncovering
   the mouth of the small cave with the Power Bread. */
static void scValePlazaBlowAwayFirstIvy(void)
{
	gmFlagSet(FLAG_VALE_FIRST_IVY_BLOWN_AWAY);
}

/* Whirlwind on the ivy at the foot of the cliff behind the shop, east of the
   cave's ivy, blows it away. */
static void scValePlazaBlowAwaySecondIvy(void)
{
	gmFlagSet(FLAG_VALE_SECOND_IVY_BLOWN_AWAY);
}

/* Whirlwind on the ivy on the cliff behind the inn blows it away. */
static void scValePlazaBlowAwayThirdIvy(void)
{
	gmFlagSet(FLAG_VALE_THIRD_IVY_BLOWN_AWAY);
}

/* Arriving in this part of Vale sets the scene up for the time of the story. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 flag;
	FIXED  verticalScale;

	if(gmFlagGet(FLAG_VALE_FIRST_IVY_BLOWN_AWAY) != 0) {
		fdActorSetPos(ACTOR_FIRST_IVY, 0, 0);
	}
	if(gmFlagGet(FLAG_VALE_SECOND_IVY_BLOWN_AWAY) != 0) {
		fdActorSetPos(ACTOR_SECOND_IVY, 0, 0);
	}
	if(gmFlagGet(FLAG_VALE_THIRD_IVY_BLOWN_AWAY) != 0) {
		fdActorSetPos(ACTOR_THIRD_IVY, 0, 0);
	}
	switch(MainRoutineStateWork.entrance) {
	case VALE_PLAZA_DEBUG_CHILDHOOD:
		gmFlagSet(FLAG_CHILDHOOD);
		syImportScriptSetExitRequest(VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTHEAST);
		return 0;
	case VALE_PLAZA_DEBUG_STORY_FLAGS:
		scValePlazaSetProgressFlagsAndExit();
		return 0;
	case VALE_PLAZA_DEBUG_BEFORE_TRAVELERS_LEAVE:
		scValePlazaTravelersLeaveAfterBattle();
		return 0;
	}
	verticalScale = toFIXED(3) / 2;
	fdActorGet(ACTOR_FIRST_IVY)->scaleY = verticalScale;
	fdActorGet(ACTOR_SECOND_IVY)->scaleY = verticalScale;
	fdActorGet(ACTOR_THIRD_IVY)->scaleY = verticalScale;
	/* Later in the story two things in the plaza are redrawn: the thatched roof
	   north of it shows a dark crack once Kalay is reached, or a blue-grey
	   patch once the party has left Vale, and the stretch of fence between the
	   trees south of it is gone. */
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		fdMapGridCopyMetatilesTo(97, 2, 80, 5, 2, 2);
		fdMapGridCopyMetatilesTo(42, 53, 42, 54, 3, 1);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	} else {
		if(gmFlagGet(FLAG_VALE_STORM) != 0) {
			fdWeatherStartRain();
			fdWeatherStartLightning();
			fdMapGridCopyMetatileAttributes(21, 38, 1, 1, 18, 41);
			flag = gmFlagGet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
			if(flag != 0) {
				fdActorSetPos(ACTOR_STORM_FIRST_AIDE, 0, 0);
				fdActorSetPos(ACTOR_STORM_SECOND_AIDE, 0, 0);
				syFieldActorWatchAndStartScript(ACTOR_STORM_GARETS_GRANDFATHER,
					ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
					ValePlazaFaceTargetActions);
			}
		} else {
			if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
				fdActorSetPos(ACTOR_SECOND_CHILD, 180, 568);
				fdMapGridCopyMetatilesTo(92, 2, 80, 5, 2, 2);
				fdMapGridCopyMetatilesTo(42, 53, 42, 54, 3, 1);
				fdMapGridCenterOnFollowedPoint();
				sySynch(1);
			}
		}
		if(MainRoutineStateWork.entrance == VALE_PLAZA_FROM_SANCTUM) {
			scValePlazaDepartForQuest();
		} else {
			if(gmFlagGet(FLAG_VALE_STORM) != 0) {
				actor = fdActorGet(ACTOR_STORM_SMALL_ROCK);
				actor->scaleX = toFIXED(3) / 10;
				actor->scaleY = toFIXED(3) / 10;
				fdActorSetSprFlags(fdActorGet(ACTOR_STORM_SMALL_ROCK), 0);
				actor = fdActorGet(ACTOR_STORM_SMALL_ROCK_SHADOW);
				actor->scaleX = toFIXED(3) / 5;
				actor->scaleY = toFIXED(3) / 5;
				fdActorSetAnimation(ACTOR_STORM_ROCK_DEFLECTING_YOUNG_MAN,
					YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_LIE);
			} else {
				if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
					fdActorSetPos(ACTOR_FALLEN_STONE_SPOT, 331, 249);
					fdActorSetSprFlags(fdActorGet(ACTOR_FALLEN_STONE_SPOT), 0);
				}
			}
			/* BUG: the test is meant for the stormy night, but the flag stays
			   set, and by day these numbers are ACTOR_HUT_OWNER and a strongman
			   already out of sight: the hut's owner is hidden from the
			   prologue's end until Kalay, and his talk and
			   scValePlazaTalkAboutFallenStone never play. */
#if FIX_HUT_OWNER_HIDDEN_BY_DAY
			if(gmFlagGet(FLAG_VALE_STORM) != 0 &&
				gmFlagGet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
#else
			if(gmFlagGet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0) {
#endif
				fdActorSetPos(ACTOR_STORM_JENNA, 0, 0);
				fdActorSetPos(ACTOR_STORM_CHARGED_UP_STRONGMAN, 0, 0);
			}
			if(MainRoutineStateWork.entrance ==
				VALE_PLAZA_AFTER_TRAVELERS_BATTLE) {
				scValePlazaTravelersLeaveAfterBattle();
			} else {
				if(gmFlagGet(FLAG_VALE_STORM) != 0 &&
					gmFlagGet(FLAG_VALE_STORM_GARET_LEFT) != 0) {
					scValePlazaMeetSurvivingTravelers();
				} else {
					if(gmFlagGet(FLAG_VALE_STORM) != 0) {
						fdScriptStartScreenTransition();
						fdScriptWaitTransitionTicks();
						fdWeatherResumeLightning();
					}
				}
			}
		}
	}
	return 0;
}

/* On the stormy night, as Isaac and Garet walk toward the villagers gathered by
   the Psynergy Stone's pond (metatiles (10, 47..53)), Garet's grandfather, the
   mayor, sends help to the elders and the boys back to the river. */
static void scValePlazaSentToAidElders(void)
{
	Sint32 skipReply = 0;

	if(gmFlagGet(FLAG_VALE_STORM) != 0 &&
		gmFlagGet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) == 0) {
		/* The camera goes to the villagers by the stone. Garet's grandfather:
		   go aid the elders, the Boulder is too heavy for them. */
		fdScriptStart();
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(197, CAMERA_KEEP, 768, 1);
		fdCameraWaitForMove();
		fdScriptSetMessageId(MsgValeGoAidElders);
		fdActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
		/* "Go aid the elders! The Boulder is too heavy for them to hold..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0, 10);
		/* Isaac walks over and Garet steps out of him. */
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorSetSpeed(ACTOR_STORM_GARET, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 179, 789);
		scPlaceAtLeader(ACTOR_STORM_GARET);
		fdActorWalkToAndWait(ACTOR_STORM_GARET, 179, 804);
		fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STORM_GARET, 40);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_STORM_GARET, FACING_EAST, 0);
		/* The two aides will not let the Boulder destroy the village and go to
		   help; Garet's grandfather counts on them. */
		fdActorSetAnimation(ACTOR_STORM_FIRST_AIDE, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_STORM_SECOND_AIDE, ANIM_NOD);
		fdActorFaceEachOther(
			ACTOR_STORM_FIRST_AIDE, ACTOR_STORM_SECOND_AIDE, 0);
		fdScriptWait(20);
		fdActorStartRepeatedMotion(ACTOR_STORM_FIRST_AIDE, 1);
		/* "We can't let the Mt. Aleph Boulder destroy our village." */
		fdScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_STORM_FIRST_AIDE, 0, 10);
		fdActorSetAnimation(ACTOR_STORM_SECOND_AIDE, ANIM_NOD);
		/* "Let's go help the elders." */
		fdScriptShowMessageThenWait(ACTOR_STORM_SECOND_AIDE, 0, 10);
		fdActorFaceDirection(ACTOR_STORM_FIRST_AIDE, FACING_EAST, 0);
		fdActorFaceDirection(
			ACTOR_STORM_SECOND_AIDE, FACING_EAST_NORTHEAST, 10);
		fdActorSetAnimationAndWait(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_NOD);
		/* "We're counting on you." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0, 10);
#if VALE_PLAZA_SECOND_AIDE_PRIORITY_PINNED
		fdActorSetSprPriority(ACTOR_STORM_SECOND_AIDE, 1);
#endif
		fdActorSetSpeed(ACTOR_STORM_FIRST_AIDE, 1.6);
		fdActorSetSpeed(ACTOR_STORM_SECOND_AIDE, 1.6);
		/* The two run off; Isaac and Garet step up to the group. */
		fdActorSetScript(ACTOR_STORM_FIRST_AIDE, ValePlazaAideRunActions);
		fdScriptWait(20);
#if VALE_PLAZA_SECOND_AIDE_PRIORITY_PINNED
		fdActorGet(ACTOR_STORM_SECOND_AIDE)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
#endif
		fdActorSetScript(ACTOR_STORM_SECOND_AIDE, ValePlazaAideRunActions);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_STORM_GARET, FACING_NORTH, 60);
		fdActorSetScript(ACTOR_PARTY_LEADER, ValePlazaLeaderStepActions);
		fdActorRunScriptAndWait(
			ACTOR_STORM_GARET, ValePlazaStormGaretStepActions);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_STORM_GARET, FACING_EAST, 10);
		/* Garet calls out to Grandpa and Jenna; Jenna asks whether they came to
		   help her. "Grandpa! Jenna!" */
		fdScriptShowMessage(ACTOR_STORM_GARET, 0);
		fdActorFaceDirection(ACTOR_STORM_GARETS_GRANDFATHER, FACING_WEST, 0);
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_JENNA, FACING_SOUTHWEST, 20);
		fdActorRunRepeatedMotion(ACTOR_STORM_JENNA, 2);
		/* "Garet, Isaac... You came to help me?" */
		scValePlazaSayThenWait(ACTOR_STORM_JENNA, 10);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_STORM_GARET, ANIM_NOD);
		fdScriptWait(20);
		/* Garet's grandfather asks whether Jenna told them about Felix. Yes:
		   good boys, Jenna could use a hand; no: help her regardless. */
		fdActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
		/* "Jenna told me about Felix. Is that why you're here?" */
		fdScriptOpenMessage(
			SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
			skipReply = 1;
			fdActorSetAnimation(
				ACTOR_STORM_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
		} else {
			fdActorSetAnimation(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_NOD);
			FieldStateWork->msg++;
		}
		/* Yes: "Good boys! Jenna could use a hand." No: "Well, regardless, I
		   want you two to help Jenna." */
		fdScriptShowMessage(
			SPEAKER_WINDOW_ABOVE | ACTOR_STORM_GARETS_GRANDFATHER, 0);
		if(skipReply != 0) {
			FieldStateWork->msg++;
		}
		/* The rested strongman is all charged up; everyone turns to him. */
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_CHARGED_UP_STRONGMAN, FACING_SOUTH, 30);
		/* "Great! I'm all charged up!" */
		fdScriptShowMessage(ACTOR_STORM_CHARGED_UP_STRONGMAN, 0);
		fdActorShowEmote(ACTOR_STORM_GARETS_GRANDFATHER,
			EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(
			ACTOR_STORM_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(
			ACTOR_STORM_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		fdActorFaceDirection(
			ACTOR_STORM_GARETS_GRANDFATHER, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_STORM_JENNA, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_GARET, FACING_NORTHEAST, 10);
		fdCameraSetSpeed(1.2);
		fdCameraMoveTo(215, CAMERA_KEEP, 758, 1);
		fdCameraWaitForMove();
		fdCameraSetSpeed(0.8);
		fdCameraMoveTo(205, CAMERA_KEEP, 778, 1);
		/* He hops down to them; asked about his Psynergy, he has enough to save
		   Felix, thanks to the Psynergy Stone. */
		fdActorSetScript(ACTOR_STORM_CHARGED_UP_STRONGMAN,
			ValePlazaChargedUpStrongmanActions);
		fdActorWaitForScriptEndById(ACTOR_STORM_CHARGED_UP_STRONGMAN);
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_CHARGED_UP_STRONGMAN, FACING_SOUTHEAST, 60);
		fdActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
		/* "How are you? Have you recovered your Psynergy?" */
		scValePlazaSayThenWait(ACTOR_STORM_GARETS_GRANDFATHER, 10);
		fdActorSetAnimationAndWait(ACTOR_STORM_CHARGED_UP_STRONGMAN, ANIM_NOD);
		/* "Enough to save Felix, thanks to the Psynergy Stone." */
		scValePlazaSayThenWait(ACTOR_STORM_CHARGED_UP_STRONGMAN, 20);
		fdActorSetAnimationAndWait(ACTOR_STORM_GARETS_GRANDFATHER, ANIM_NOD);
		fdScriptWait(10);
		/* Garet's grandfather sends Jenna back with the rested strongman, and
		   Isaac and Garet to show the way past the fallen rocks; Jenna thanks
		   Isaac. */
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_GARETS_GRANDFATHER, FACING_SOUTH, 30);
		/* "OK, Jenna, you heard him. Now hurry back!" */
		scValePlazaSayThenWait(
			SPEAKER_WINDOW_ABOVE + ACTOR_STORM_GARETS_GRANDFATHER, 10);
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_JENNA, FACING_NORTHEAST, 30);
		fdActorSetAnimationAndWait(ACTOR_STORM_JENNA, ANIM_NOD);
		scValePlazaFaceDirectionAndWait(
			ACTOR_STORM_GARETS_GRANDFATHER, FACING_WEST, 30);
		fdActorRunRepeatedMotion(ACTOR_STORM_GARETS_GRANDFATHER, 2);
		/* "The path may be blocked by fallen rocks. You two can show them the
		   way!" */
		scValePlazaSayThenWait(
			SPEAKER_WINDOW_ABOVE + ACTOR_STORM_GARETS_GRANDFATHER, 10);
		fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_STORM_GARET, 40);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		scValePlazaFaceDirectionAndWait(ACTOR_STORM_GARET, FACING_EAST, 20);
		scValePlazaFaceDirectionAndWait(ACTOR_STORM_JENNA, FACING_WEST, 30);
		fdActorSetAnimationAndWait(ACTOR_STORM_JENNA, ANIM_NOD);
		/* "...Thank you, Isaac." */
		scValePlazaSayThenWait(ACTOR_STORM_JENNA, 30);
		scValePlazaFaceDirectionAndWait(ACTOR_STORM_JENNA, FACING_NORTH, 30);
		fdActorSetAnimationAndWait(ACTOR_STORM_JENNA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_STORM_CHARGED_UP_STRONGMAN, ANIM_NOD);
		/* Garet, Jenna and the rested strongman walk into Isaac; Garet's
		   grandfather keeps turning to watch him. */
		scWalkIntoLeader(ACTOR_STORM_GARET);
		scWalkIntoLeader(ACTOR_STORM_JENNA);
		scWalkIntoLeader(ACTOR_STORM_CHARGED_UP_STRONGMAN);
		syFieldActorWatchAndStartScript(ACTOR_STORM_GARETS_GRANDFATHER,
			ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
			ValePlazaFaceTargetActions);
		gmFlagSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
		fdScriptEnd();
	}
}

/* In Vale on the stormy night, after the river, Isaac overhears two travelers
   who survived Sol Sanctum's fury, the game's first sight of Saturos and
   Menardi. "Only the two of us survived..." */
static void scValePlazaMeetSurvivingTravelers(void)
{
	ACTOR *camera;

	camera = fdCameraGetActor();
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	/* The screen opens on the storm at the west path, where Isaac comes in from
	   the river; the music fades out as he walks up and hears the first
	   traveler. */
	fdCameraMoveTo(64, 144, 350, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptAddPartyMember(GARET, 0);
	fdScriptStartScreenTransition();
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdWeatherResumeLightning();
	fdActorSetPos(ACTOR_FIRST_TRAVELER, 105, 267);
	sySynch(1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 93, 343);
	fdScriptSetMessageId(MsgValeOnlyTwoSurvived);
	/* "Only the two of us survived..." */
	fdScriptShowMessage(ACTOR_FIRST_TRAVELER, 0);
	/* The ominous music starts; the camera rises to the travelers and the
	   second traveler walks in. */
	sySndPlay(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
	camera->motionFlg = 0;
	fdCameraSetSpeed(3);
	fdCameraMoveTo(109, 176, 281, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorSetPos(ACTOR_SECOND_TRAVELER, 135, 177);
	fdActorSetSpeed(ACTOR_SECOND_TRAVELER, 0.8);
	fdActorWalkTo(ACTOR_SECOND_TRAVELER, 126, 258);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_NORTH_NORTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_SECOND_TRAVELER);
	fdActorSetAnimation(ACTOR_SECOND_TRAVELER, ANIM_STAND);
	scValePlazaFaceDirectionAndWait(
		ACTOR_SECOND_TRAVELER, FACING_WEST_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
	/* They talk of Sol Sanctum's fury and its trap switch, of the power of
	   Alchemy, and of their next attempt on the sanctum. */
	fdActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
	/* "How could we have anticipated Sol Sanctum would unleash such fury?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
	fdActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
	/* "It's a miracle that even the two of us were spared..." */
	scValePlazaSayThenWait(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 30);
	scValePlazaFaceDirectionAndWait(
		ACTOR_SECOND_TRAVELER, FACING_NORTH_NORTHWEST, 20);
	/* "That switch... It must have been a trap..." */
	scValePlazaSayThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 10);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FIRST_TRAVELER, FACING_NORTH_NORTHWEST, 40);
	/* "But to think it could conjure up a storm this powerful!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 0);
	fdActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
	/* "...Another demonstration of the awesome powers of Alchemy." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FIRST_TRAVELER, FACING_EAST_NORTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
	/* "Regardless, we must not fail the next time we challenge Sol Sanctum." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FIRST_TRAVELER, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_SECOND_TRAVELER, FACING_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
	fdScriptWait(20);
	/* "Next time, we shall certainly..." */
	scValePlazaSayThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 20);
	scValePlazaFightTravelers();
	fdScriptEnd();
}

/* In Vale on the stormy night, the travelers catch Isaac eavesdropping and
   fight. Win or lose, the party returns by VALE_PLAZA_AFTER_TRAVELERS_BATTLE.
   "You must forget everything you heard." */
static void scValePlazaFightTravelers(void)
{
	/* Garet, still out of sight, calls Isaac to wait; the travelers turn and
	   look down at Isaac. */
	fdActorSetSprColById(ACTOR_STORM_GARET, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_STORM_GARET), 0);
	fdActorSetPos(ACTOR_STORM_GARET, 0, 331);
	sySynch(1);
	/* "Isaac! Wait!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_STORM_GARET, 0);
	fdActorShowEmote(
		ACTOR_FIRST_TRAVELER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(
		ACTOR_SECOND_TRAVELER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetPos(ACTOR_STORM_GARET, 0, 0);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTH_SOUTHWEST, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_SECOND_TRAVELER, FACING_SOUTH_SOUTHWEST, 40);
	fdCameraSetSpeed(1.5);
	fdCameraMoveTo(89, 176, 313, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_NORTHEAST, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_SECOND_TRAVELER, FACING_WEST_SOUTHWEST, 40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(100, 144, 333, 1);
	fdActorSetSpeed(ACTOR_FIRST_TRAVELER, 1);
	fdActorSetSpeed(ACTOR_SECOND_TRAVELER, 1);
	fdActorWalkTo(ACTOR_FIRST_TRAVELER, 105, 329);
	fdScriptWait(10);
	fdActorWalkTo(ACTOR_SECOND_TRAVELER, 124, 329);
	fdActorWaitForMoveById(ACTOR_FIRST_TRAVELER);
	fdActorSetAnimation(ACTOR_FIRST_TRAVELER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWaitForMoveById(ACTOR_SECOND_TRAVELER);
	fdActorSetAnimation(ACTOR_SECOND_TRAVELER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_SECOND_TRAVELER, FACING_SOUTH_SOUTHWEST, 0);
	/* Garet runs in from the west. The first traveler asks whether Isaac was
	   eavesdropping; whatever he answers, Garet thinks they look scary. */
	fdActorSetSprColById(ACTOR_STORM_GARET, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_STORM_GARET), SPRITE_FLAG_SHADOW);
	fdActorSetPos(ACTOR_STORM_GARET, 0, 331);
	fdActorSetSpeed(ACTOR_STORM_GARET, 1.2);
	fdActorWalkToAndWait(ACTOR_STORM_GARET, 37, 339);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
	/* "You were eavesdropping on us just now... weren't you?" */
	fdScriptOpenMessage(ACTOR_FIRST_TRAVELER, 0);
	fdActorShowEmote(ACTOR_STORM_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 10);
	fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_EAST, 0);
	scValePlazaFaceDirectionAndWait(ACTOR_SECOND_TRAVELER, FACING_WEST, 20);
	fdActorRunRepeatedMotion(ACTOR_STORM_GARET, 2);
	/* "Isaac..." */
	scValePlazaSayThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_STORM_GARET, 10);
	syFieldActorWatchAndStartScript(ACTOR_PARTY_LEADER,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_STORM_GARET,
		ValePlazaFaceTargetActions);
	fdActorWalkToAndWait(ACTOR_STORM_GARET, 93, 361);
	scValePlazaFaceDirectionAndWait(
		ACTOR_STORM_GARET, FACING_NORTH_NORTHEAST, 40);
	/* "Isaac, they look scary... Are you sure you should be talking to
	   them?" */
	scValePlazaSayThenWait(ACTOR_STORM_GARET, 20);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_EAST, 0);
	scValePlazaFaceDirectionAndWait(ACTOR_SECOND_TRAVELER, FACING_WEST, 15);
	fdActorSetAnimation(ACTOR_FIRST_TRAVELER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTH_SOUTHWEST, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_SECOND_TRAVELER, FACING_SOUTH_SOUTHWEST, 30);
	/* The travelers: forget everything you heard; we'll help you forget. */
	fdActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_SHAKE_HEAD);
	/* "You must forget everything you heard." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SECOND_TRAVELER, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_PARTY_LEADER, FACING_NORTH_NORTHEAST, 30);
	scValePlazaFaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
	fdActorRunRepeatedMotion(ACTOR_FIRST_TRAVELER, 2);
	fdActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
	/* "Don't worry... We'll help you forget!" */
	scValePlazaSayThenWait(ACTOR_FIRST_TRAVELER, 20);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_STORM_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorSetScript(ACTOR_SECOND_TRAVELER, ValePlazaSecondTravelerLeapActions);
	fdScriptWait(6);
	fdActorSetScript(ACTOR_FIRST_TRAVELER, ValePlazaFirstTravelerLeapActions);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValePlazaLeaderActions);
	fdScriptWait(6);
	fdActorRunScriptAndWait(ACTOR_STORM_GARET, ValePlazaStormGaretLeapActions);
	/* The confrontation continues in battle; both outcomes return here. */
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	{
		const void *scene = &SceneId_ValePlaza;

		fdScriptSetAfterBattleDestination(
			(Sint32)scene, VALE_PLAZA_AFTER_TRAVELERS_BATTLE);
		fdScriptSetDefeatDestination(
			(Sint32)scene, VALE_PLAZA_AFTER_TRAVELERS_BATTLE);
	}
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 4);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
}

/* In Vale after the battle, the travelers leave Isaac and Garet lying in the
   storm, and the storm, and the prologue, end. */
static void scValePlazaTravelersLeaveAfterBattle(void)
{
	ACTOR *actor;

	/* The screen opens on Isaac and Garet lying beaten in the storm, the
	   travelers standing over them. */
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdCameraMoveTo(64, 144, 350, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 48, 346);
	fdActorSetPos(ACTOR_STORM_GARET, 78, 358);
	fdActorSetPos(ACTOR_FIRST_TRAVELER, 103, 342);
	fdActorSetPos(ACTOR_SECOND_TRAVELER, 112, 360);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_TRAVELER, FACING_WEST, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
	fdActorGet(ACTOR_PARTY_LEADER)->scaleX = -toFIXED(1);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetAnimation(ACTOR_STORM_GARET, ANIM_GARET_CHILD_SPRAWLED);
	fdActorGet(ACTOR_STORM_GARET)->spr->rotation = DEGtoANG(30);
	fdActorSetSprFlags(fdActorGet(ACTOR_STORM_GARET), 0);
	{
		FIELD_STATE *state = FieldStateWork;

		state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	}
	fdScriptStartScreenTransition();
	fdWeatherResumeLightning();
	fdScriptWaitTransitionTicks();
	fdScriptWait(80);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_TRAVELER, FACING_NORTH, 40);
	fdActorSetAnimationAndWait(ACTOR_FIRST_TRAVELER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_TRAVELER, ANIM_NOD);
	fdActorFaceDirection(ACTOR_FIRST_TRAVELER, FACING_WEST, 10);
	fdActorFaceDirection(ACTOR_SECOND_TRAVELER, FACING_WEST, 10);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
	fdActorSetSprPriority(ACTOR_STORM_GARET, 3);
	fdActorSetSpeed(ACTOR_FIRST_TRAVELER, 2.4);
	actor = fdActorGet(ACTOR_FIRST_TRAVELER);
	actor->bounce = toFIXED(1) / 100;
	actor->gravity = toFIXED(1) / 2;
	fdActorSetScript(
		ACTOR_FIRST_TRAVELER, ValePlazaFirstTravelerLeapAwayActions);
	fdScriptWait(24);
	fdActorSetSpeed(ACTOR_SECOND_TRAVELER, 2.4);
	actor = fdActorGet(ACTOR_SECOND_TRAVELER);
	actor->bounce = toFIXED(1) / 100;
	actor->gravity = toFIXED(1) / 2;
	fdActorRunScriptAndWait(
		ACTOR_SECOND_TRAVELER, ValePlazaSecondTravelerLeapAwayActions);
	fdScriptWait(40);
	/* Three last flashes of lightning, then the storm ends and the screen fades
	   slowly out. */
	fdWeatherStopLightning();
	fdWeatherLastLightning();
	sySynch(20);
	fdWeatherLastLightning();
	sySynch(60);
	fdWeatherLastLightning();
	sySynch(20);
	fdWeatherStopLightning();
	fdScriptWait(40);
	FieldStateWork->transitionFrm = 120;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagClear(FLAG_VALE_STORM);
	syImportScriptSetExitRequest(VALE_PLAZA_TO_TITLE);
	fdScriptEnd();
}

/* In Vale, at the town gate, the villagers give their supplies and farewells as
   Isaac and Garet set out on the Wise One's quest. */
static void scValePlazaDepartForQuest(void)
{
	ACTOR		 *actor;
	FIELD_STATE	 *state;
	Sint32		  priFlg;
	const Sint32 *faceActions;
	const Sint32 *chaseActions;
	const Sint32 *departureActions;

	/* The village gathers at the gate to see Isaac and Garet off; the camera
	   starts on the two children up the path. */
	gmPartyRestoreHpPp(1);
	fdScriptStart();
	fdMapGridCopyMetatilesTo(42, 53, 42, 54, 3, 1);
	fdCameraMoveTo(180, 16, 618, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorSetSprFlags(fdActorGet(ACTOR_FAREWELL_YOUNG_MAN), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FAREWELL_FAIR_HAIRED_WOMAN), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FAREWELL_FIRST_STRONGMAN), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FAREWELL_SECOND_STRONGMAN), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FAREWELL_THIRD_STRONGMAN), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FAREWELL_STOUT_MAN), 0);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 1);
	fdActorSetSprPriority(ACTOR_GARET, 1);
	fdActorSetSprPriority(ACTOR_FAREWELL_GARETS_BROTHER, 1);
	fdActorSetSprPriority(ACTOR_FAREWELL_GARETS_SISTER, 1);
	fdActorSetSprPriority(ACTOR_FAREWELL_GARETS_GRANDFATHER, 1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 208, 814);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(80);
	/* The second child asks why everyone is at the gate; the first says Isaac
	   and Garet are leaving on a journey. */
	fdActorShowEmote(
		ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_WEST_SOUTHWEST, 20);
	fdScriptSetMessageId(MSGNO(MsgValeWhysEveryoneHanging));
	/* "Why's everyone hanging around the town gate?" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 10);
	fdActorShowEmote(
		ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_EAST_SOUTHEAST, 10);
	/* "What, didn't you hear?" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 10);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_FIRST_CHILD, 2);
	/* "Isaac and Garet are leaving Vale on a journey." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 10);
	/* They walk to the top of the path: the second child wishes to go too, the
	   first has heard it will be dangerous, and everyone is down there to say
	   good-bye. */
	fdActorShowEmote(
		ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_FAREWELL_SECOND_CHILD, 0.8);
	fdActorWalkToAndWait(ACTOR_FAREWELL_SECOND_CHILD, 184, 618);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHEAST, 60);
	/* "I wish I could go!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 20);
	fdActorSetSpeed(ACTOR_FAREWELL_FIRST_CHILD, 1);
	fdActorWalkToAndWait(ACTOR_FAREWELL_FIRST_CHILD, 168, 618);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_EAST_NORTHEAST, 10);
	fdActorSetAnimation(ACTOR_FAREWELL_FIRST_CHILD, ANIM_SHAKE_HEAD);
	/* "I heard it's going to be really dangerous!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_WEST_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_SECOND_CHILD, 1);
	/* "Oh, yeah? So everyone's down there to say good-bye?" */
	fdScriptShowMessage(ACTOR_FAREWELL_SECOND_CHILD, 0);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_FAREWELL_DOG, 2.4);
	fdActorSetPos(ACTOR_FAREWELL_DOG, 110, 744);
	sySynch(2);
	fdActorSetAnimation(ACTOR_FAREWELL_DOG, ANIM_NOD);
	fdActorSetScript(ACTOR_FAREWELL_DOG, ValePlazaFarewellDogActions);
	fdScriptWait(40);
	faceActions = ValePlazaFaceTargetActions;
	syFieldActorWatchAndStartScript(ACTOR_FAREWELL_FIRST_CHILD,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_FAREWELL_DOG, faceActions);
	syFieldActorWatchAndStartScript(ACTOR_FAREWELL_SECOND_CHILD,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_FAREWELL_DOG, faceActions);
	fdActorWaitForScriptEndById(ACTOR_FAREWELL_DOG);
	fdActorStop(ACTOR_FAREWELL_FIRST_CHILD);
	fdActorStop(ACTOR_FAREWELL_SECOND_CHILD);
	fdScriptWait(60);
	fdActorShowEmote(
		ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(
		ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
	fdActorFaceDirection(ACTOR_FAREWELL_FIRST_CHILD, FACING_EAST_SOUTHEAST, 0);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_WEST_SOUTHWEST, 80);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHWEST, 40);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_EAST_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
	fdScriptWait(20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHWEST, 60);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHEAST, 40);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHWEST, 60);
	/* They step down to look over the crowd: Isaac's mother is not there to see
	   him off. */
	fdActorShowEmote(
		ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorFaceDirection(ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_FAREWELL_SECOND_CHILD, 184, 630);
	fdScriptWait(20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHEAST, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHWEST, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHEAST, 20);
	fdActorShowEmote(
		ACTOR_FAREWELL_SECOND_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorShowEmote(
		ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorWalkToAndWait(ACTOR_FAREWELL_FIRST_CHILD, 168, 630);
	fdScriptWait(20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHEAST, 40);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHWEST, 40);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHEAST, 40);
	fdActorShowEmote(
		ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_EAST_SOUTHEAST, 10);
	/* "Hey, what's the matter?" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 20);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
	/* "Weird. Isaac's mother isn't there to see him off!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 10);
	fdActorShowEmote(
		ACTOR_FAREWELL_FIRST_CHILD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHWEST, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHEAST, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHWEST, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_SOUTH_SOUTHWEST, 60);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
	/* "You're right. She's not there." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 10);
	actor = fdActorGetById(ACTOR_FAREWELL_DOG);

	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_FAREWELL_CAT, actor->x.fixed, actor->z.fixed);
	}
	priFlg = (Uint8)~ACTOR_PRIORITY_AUTOMATIC;
	sySynch(2);
	fdActorGetById(ACTOR_FAREWELL_DOG)->priFlg &= priFlg;
	fdActorGetById(ACTOR_FAREWELL_CAT)->priFlg &= priFlg;
	fdActorSetSprPriority(ACTOR_FAREWELL_DOG, 2);
	fdActorSetSprPriority(ACTOR_FAREWELL_CAT, 2);
	fdActorSetSpeed(ACTOR_FAREWELL_CAT, 3.6);
	fdActorSetAnimation(ACTOR_FAREWELL_CAT, ANIM_WALK);
	chaseActions = ValePlazaFarewellChaseActions;
	fdActorSetScript(ACTOR_FAREWELL_CAT, chaseActions);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_FAREWELL_DOG, ANIM_NOD);
	fdActorSetSpeed(ACTOR_FAREWELL_DOG, 4.8);
	fdActorRunScriptAndWait(ACTOR_FAREWELL_DOG, chaseActions);
	fdScriptWait(60);
	/* Should they say good-bye too? Sure: both run down, and the camera follows
	   them to the gate. */
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_SECOND_CHILD, 2);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_WEST_SOUTHWEST, 10);
	/* "Hey, should we go say good-bye, too?" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_SECOND_CHILD, 10);
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_FIRST_CHILD, 1);
	fdScriptWait(20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_FIRST_CHILD, FACING_EAST_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
	/* "Sure! Let's go!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_FIRST_CHILD, 20);
	fdActorSetAnimation(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_FAREWELL_FIRST_CHILD, 2.4);
	fdActorSetSpeed(ACTOR_FAREWELL_SECOND_CHILD, 2.4);
	fdActorSetScript(
		ACTOR_FAREWELL_FIRST_CHILD, ValePlazaFarewellFirstChildActions);
	fdScriptWait(10);
	fdCameraSetSpeed(2.4);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(215, 16, 801, 1);
	fdScriptWait(10);
	fdActorSetScript(
		ACTOR_FAREWELL_SECOND_CHILD, ValePlazaFarewellSecondChildActions);
	fdActorWaitForScriptEndById(ACTOR_FAREWELL_SECOND_CHILD);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_SECOND_CHILD, FACING_SOUTH_SOUTHEAST, 120);
	/* The Great Healer counts on Isaac, and on Garet too; the two nod. */
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_GREAT_HEALER, 2);
	fdScriptWait(20);
	/* "We are counting on you, Isaac. Oh, and you, too, Garet." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GREAT_HEALER, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	scValePlazaFaceDirectionAndWait(ACTOR_GARET, FACING_WEST_NORTHWEST, 20);
	scValePlazaFaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
	scValePlazaFaceDirectionAndWait(ACTOR_GARET, FACING_NORTH_NORTHWEST, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(40);
	/* Garet's sister warns them off water that smells funny and hands over a
	   Herb. */
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_GARETS_SISTER, 2);
	fdScriptWait(20);
	/* "Don't drink the water if it smells funny. And...take this with you,
	   too." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_SISTER, 10);
	fdActorSetSpeed(ACTOR_FAREWELL_GARETS_SISTER, 1);
	fdActorWalkToAndWait(ACTOR_FAREWELL_GARETS_SISTER, 216, 800);
	fdActorFaceDirection(ACTOR_FAREWELL_GARETS_SISTER, FACING_SOUTH, 0);
	gmPartyGiveItem(ITEM_HERB, 0);
	fdActorWalkToAndWait(ACTOR_FAREWELL_GARETS_SISTER, 264, 800);
	fdActorFaceDirection(ACTOR_FAREWELL_GARETS_SISTER, FACING_SOUTHWEST, 0);
	/* Garet protests he knows that, Sis; his brother says he will eat anything,
	   and Garet jumps. */
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	scValePlazaFaceDirectionAndWait(ACTOR_GARET, FACING_EAST_NORTHEAST, 10);
	/* "I know not to drink stagnant water, Sis!" */
	scValePlazaSayThenWait(ACTOR_GARET, 10);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_BROTHER, ANIM_SHAKE_HEAD);
	/* "Don't listen to him, Isaac! He's a pig! He'll eat anything that's not
	   tied down!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_BROTHER, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorJump(ACTOR_GARET, 4, 60);
	/* Dora is very late, the bearded man in a long robe says. Everyone wonders;
	   Garet's brother remembers. */
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_MAN_AWAITING_DORA, 2);
	fdScriptWait(20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_MAN_AWAITING_DORA, FACING_NORTH_NORTHEAST, 10);
	/* "Hmm... Dora is very late..." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_MAN_AWAITING_DORA, 60);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(
		ACTOR_FAREWELL_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(
		ACTOR_FAREWELL_GARETS_BROTHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(
		ACTOR_FAREWELL_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(
		ACTOR_FAREWELL_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(
		ACTOR_FAREWELL_GARETS_BROTHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "Oh! That's right!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_BROTHER, 60);
	/* Garet's brother brings the Catch Beads Aunty Dora sent for Isaac; Isaac
	   and Garet turn to follow. */
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_FAREWELL_GARETS_BROTHER);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_FAREWELL_GARETS_BROTHER);
	fdActorWalkToAndWait(ACTOR_FAREWELL_GARETS_BROTHER, 216, 800);
	fdActorFaceDirection(ACTOR_FAREWELL_GARETS_BROTHER, FACING_SOUTH, 0);
	/* "Aunty Dora asked me to give this to you, Isaac." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_BROTHER, 60);
	gmPartyGiveItem(ITEM_CATCH_BEADS, 0);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorWalkToAndWait(ACTOR_FAREWELL_GARETS_BROTHER, 272, 816);
	fdActorFaceDirection(ACTOR_FAREWELL_GARETS_BROTHER, FACING_WEST, 0);
	/* Garet thinks they are all set; his sister says think again, it must be
	   hard for Dora. */
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scValePlazaFaceDirectionAndWait(ACTOR_GARET, FACING_WEST_NORTHWEST, 10);
	/* "I guess we're all set to go, huh, Isaac?" */
	scValePlazaSayThenWait(ACTOR_GARET, 10);
	fdActorFaceDirection(
		ACTOR_FAREWELL_MAN_AWAITING_DORA, FACING_SOUTH_SOUTHEAST, 0);
	scValePlazaFaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_EAST, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_FAREWELL_GARETS_SISTER, 1);
	/* "Think again, Garet!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_SISTER, 10);
	scValePlazaFaceDirectionAndWait(ACTOR_GARET, FACING_EAST_NORTHEAST, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_SISTER, ANIM_SHAKE_HEAD);
	/* "It must be very difficult for Dora..." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_SISTER, 10);
	/* Garet's mother knows how Dora feels: she does not want Isaac to go, but
	   knows he must. The crowd nods. */
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_NOD);
	/* "As Garet's mother, I know how Dora must be feeling." */
	fdScriptShowMessage(ACTOR_FAREWELL_GARETS_MOTHER, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_SHAKE_HEAD);
	/* "But I still have Garet's father, while Dora has only Isaac." */
	fdScriptShowMessage(ACTOR_FAREWELL_GARETS_MOTHER, 0);
	fdActorStartRepeatedMotion(ACTOR_FAREWELL_GARETS_MOTHER, 3);
	/* "She doesn't want you to go, Isaac, but she knows you have to, and it
	   hurts to see you leave." */
	fdScriptShowMessage(ACTOR_FAREWELL_GARETS_MOTHER, 0);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_GARETS_SISTER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_FATHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_BROTHER, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FAREWELL_FIRST_STRONGMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_BURLY_MAN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FAREWELL_WOMAN_IN_GREEN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FAREWELL_SECOND_STRONGMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_OLD_MAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_OLD_WOMAN, ANIM_NOD);
	/* Garet's grandfather hops: they will never leave standing around like
	   this, so one final farewell. */
	fdActorJump(ACTOR_FAREWELL_GARETS_GRANDFATHER, 2, 10);
	fdActorJump(ACTOR_FAREWELL_GARETS_GRANDFATHER, 4, 40);
	/* "They can't keep standing around here like this, or they'll never
	   leave." */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_GRANDFATHER, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	scValePlazaFaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_GARETS_GRANDFATHER, FACING_NORTH_NORTHEAST, 10);
	/* "So, one final farewell!" */
	scValePlazaSayThenWait(ACTOR_FAREWELL_GARETS_GRANDFATHER, 10);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_GARETS_GRANDFATHER, FACING_WEST_NORTHWEST, 20);
	scValePlazaFaceDirectionAndWait(
		ACTOR_FAREWELL_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHWEST, 10);
	/* The crowd nods in three waves, then all jump together: "Farewell!" */
	fdActorSetAnimation(ACTOR_FAREWELL_FIRST_CHILD, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_MAN_AWAITING_DORA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_BROTHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_OLD_MAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_FAIR_HAIRED_WOMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_THIRD_STRONGMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_STOUT_MAN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FAREWELL_SECOND_CHILD, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_GRANDFATHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_MOTHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_OLD_WOMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_FIRST_STRONGMAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_BURLY_MAN, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_FAREWELL_GREAT_HEALER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_SISTER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_GARETS_FATHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_YOUNG_MAN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FAREWELL_SECOND_STRONGMAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FAREWELL_WOMAN_IN_GREEN, ANIM_NOD);
	fdScriptWait(80);
	fdActorJump(ACTOR_FAREWELL_FIRST_CHILD, 4, 0);
	fdActorJump(ACTOR_FAREWELL_MAN_AWAITING_DORA, 4, 0);
	fdActorJump(ACTOR_FAREWELL_GARETS_BROTHER, 4, 0);
	fdActorJump(ACTOR_FAREWELL_OLD_MAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_FAIR_HAIRED_WOMAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_THIRD_STRONGMAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_STOUT_MAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_SECOND_CHILD, 4, 0);
	fdActorJump(ACTOR_FAREWELL_GARETS_GRANDFATHER, 4, 0);
	fdActorJump(ACTOR_FAREWELL_GARETS_MOTHER, 4, 0);
	fdActorJump(ACTOR_FAREWELL_OLD_WOMAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_FIRST_STRONGMAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_BURLY_MAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_GREAT_HEALER, 4, 0);
	fdActorJump(ACTOR_FAREWELL_GARETS_SISTER, 4, 0);
	fdActorJump(ACTOR_FAREWELL_GARETS_FATHER, 4, 0);
	fdActorJump(ACTOR_FAREWELL_YOUNG_MAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_SECOND_STRONGMAN, 4, 0);
	fdActorJump(ACTOR_FAREWELL_WOMAN_IN_GREEN, 4, 0);
	/* "Farewell!" */
	mnFieldMessageShow(MSGNO(MsgValeFarewell), 1);
	priFlg = ACTOR_PRIORITY_AUTOMATIC;
	fdScriptWait(80);
	fdActorGetById(ACTOR_PARTY_LEADER)->priFlg |= priFlg;
	{
		ACTOR *garet = fdActorGetById(ACTOR_GARET);
		Uint8  flg = priFlg | garet->priFlg;

		garet->priFlg = flg;
	}
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	scValePlazaFaceDirectionAndWait(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	scValePlazaFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	departureActions = ValePlazaPartyDepartureActions;
	fdActorSetScript(ACTOR_PARTY_LEADER, departureActions);
	fdScriptWait(20);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(216, 16, 905, 1);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetScript(ACTOR_GARET, departureActions);
	fdScriptWait(60);
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 60;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	syImportScriptSetExitRequest(VALE_PLAZA_TO_WORLD_MAP);
	fdScriptEnd();
}

static void scValePlazaSayThenWait(Sint32 speaker, Sint32 ticks)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(ticks);
}

static void scValePlazaFaceDirectionAndWait(
	Sint32 actorId, Sint32 facing, Sint32 ticks)
{
	fdActorFaceDirection(actorId, facing, 0);
	fdScriptWait(ticks);
}

/* A villager calling up Psynergy. He is drawn SPRITE_COLOR_WHITE for two ticks
   in four, with a pair of sparks every 16 ticks. */
static void scValePlazaFlashPsynergyPalette(ACTOR *actor)
{
	if((TickCount & 2) != 0) {
		fdActorSetColMode(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((TickCount & 15) == 0) {
		scValePlazaSpawnEffectPair(actor);
	}
}

/* A villager using Psynergy. */
static void scValePlazaCyclePsynergyPaletteAndSpawnEffects(ACTOR *actor)
{
	if((TickCount & 1) != 0) {
		Sint32 palette = syMathRemainderUnsigned(TickCount >> 1, 6);
		fdActorSetColMode(actor, palette);
	}
	if((TickCount & 15) == 0) {
		scValePlazaSpawnEffectPair(actor);
	}
}

/* Cycles the lifted rock through colour modes SPRITE_COLOR_NORMAL to
   SPRITE_COLOR_BRG, a step every second tick. */
static void scValePlazaCyclePsynergyPalette(ACTOR *actor)
{
	if((TickCount & 1) != 0) {
		Sint32 palette = syMathRemainderUnsigned(TickCount >> 1, 6);
		fdActorSetColMode(actor, palette);
	}
}

#include "shared/field_effect_vertical.c"

/* Spawns a pair of Psynergy sparks, one rising and one falling
   (shared/field_effect_vertical.c). */
static void scValePlazaSpawnEffectPair(ACTOR *parent)
{
	ACTOR			   *pair[2];
	ACTOR			   *child;
	SPR				   *spr;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	Sint32				i;

	sySndPlay(SE_PSYNERGY_SPARKLE);
	for(i = 0; i < 2; ++i) {
		child = syFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
			parent->y.fixed, parent->z.fixed);
		pair[i] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			spr = child->spr;
			child->motionFlg = 0;
			child->tmr = 0;
			child->watchedActor = (ACTOR_OR_EFFECT *)parent;
			if(spr != NULL) {
				fdSprStartAnimation(spr, 0);
				spr->flg = 0;
				syVRAMCacheFreeEntry(spr->slot);
				spr->slot = effectState->tileSlot;
				spr->retainCacheEntry = 1;
				spr->image.tile =
					(VramCacheEntries[spr->slot].ofs >> 5) & OBJ_TILE_MASK;
				spr->image.fullCol = 0;
				spr->image.shape = RING_HALF_SHAPE;
				spr->image.size = RING_HALF_SIZE;
				spr->entries[0]->picture = 0;
			}
		}
	}
	{
		ACTOR *effect = pair[0];
		SPR	  *effectSpr = effect->spr;

		effect->update = scFieldEffectRiseRingFrontHalf;
		effectSpr->image.pri = 2;
	}
	{
		ACTOR *effect = pair[1];
		SPR	  *effectSpr = effect->spr;

		effectSpr->image.pri = 2;
		effect->update = scFieldEffectRiseRingBackHalf;
		effect->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
}

#include "shared/script_psynergy.c"

/* The rock-lifting boy's colours and sparks while his Psynergy lifts the
   rock. */
static void scValePlazaUpdateRockLiftingBoyPsynergy(void)
{
	scValePlazaCyclePsynergyPaletteAndSpawnEffects(
		fdActorGet(ACTOR_STORM_ROCK_LIFTING_BOY));
}

/* The strongman's colours and sparks while his Psynergy shakes the ground. */
static void scValePlazaUpdateShowingStrongmanPsynergy(void)
{
	scValePlazaCyclePsynergyPaletteAndSpawnEffects(
		fdActorGet(ACTOR_STRONGMAN_SHOWING_PSYNERGY));
}

/* The small rock shifting colours while the boy's Psynergy lifts it. */
static void scValePlazaUpdateSmallRockPalette(void)
{
	scValePlazaCyclePsynergyPalette(fdActorGet(ACTOR_STORM_SMALL_ROCK));
}

/* Arriving by VALE_PLAZA_DEBUG_STORY_FLAGS sets a preset of story flags, which
   differs by edition, and leaves by VALE_PLAZA_TO_BILIBIN. */
static void scValePlazaSetProgressFlagsAndExit(void)
{
	gmFlagSet(FLAG_NO_ENCOUNTERS);
	gmFlagSet(FLAG_DEBUG_BATTLE_KEYS);
#if DEBUG_STORY_PRESET_THROUGH_KALAY
	gmFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
	gmFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
	gmFlagSet(FLAG_VALE_STORM_THIRD_ROCK_FELL);
	gmFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
	gmFlagSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
	gmFlagSet(FLAG_VALE_STORM_HEARD_GARET);
	gmFlagSet(FLAG_VALE_STORM_GARET_JOINED);
	gmFlagSet(FLAG_VALE_BOULDER_FELL);
	gmFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
	gmFlagSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
	gmFlagSet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS);
	gmFlagSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
	gmFlagSet(FLAG_VALE_STORM_GARET_LEFT);
	gmFlagSet(FLAG_VALE_ASKED_IF_GOING_TO_KRADEN);
	gmFlagSet(FLAG_VALE_GARET_TOLD_OFF_FOR_FLOWERS);
	gmFlagSet(FLAG_MET_SATUROS_AND_MENARDI);
	gmFlagSet(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
	gmFlagSet(FLAG_SANCTUM_VISIT_PLANNED);
	gmFlagSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
	gmFlagSet(FLAG_SOL_SANCTUM_ENTERED);
	gmFlagSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND);
	gmFlagSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
	gmFlagSet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
	gmFlagSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND);
	gmFlagSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET);
	gmFlagSet(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
	gmFlagSet(FLAG_SOL_SANCTUM_FOUND);
	gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
	gmFlagSet(FLAG_SOL_SANCTUM_CENTER_STATUE_MOVED);
	gmFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
	gmFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
	gmFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
	gmFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
	gmFlagSet(FLAG_SOL_SANCTUM_ALL_SEALS_OPEN);
	gmFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
	gmFlagSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
	gmFlagSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
	gmFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
	gmFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_EXPLAINED);
	gmFlagSet(FLAG_SOL_SANCTUM_VENUS_STAR_BAGGED);
	gmFlagSet(FLAG_SOL_SANCTUM_MERCURY_STAR_BAGGED);
	gmFlagSet(FLAG_SOL_SANCTUM_STARS_GIVEN_TO_ALEX);
	gmFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
	gmFlagSet(FLAG_SOL_SANCTUM_ERUPTED);
	gmFlagSet(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
	gmFlagSet(FLAG_VALE_PARTY_LEFT);
	gmFlagSet(FLAG_VALE_FAREWELL_HERB_RECEIVED);
	gmFlagSet(FLAG_GARET_CAME_HOME);
	gmFlagSet(FLAG_KALAY_REACHED);
#else
	gmFlagSet(FLAG_TRET_DEFEATED);
	gmFlagSet(FLAG_KOLIMA_CURSE_LIFTED);
#endif
	syImportScriptSetExitRequest(VALE_PLAZA_TO_BILIBIN);
}

/* clang-format off */
/* Unused: nothing refers to it. A run of palette numbers, 0 to 6 three times */
/* each and then 0 to 5 twice each, as a palette cycle of the Psynergy */
/* displays would step through them. */
Uint8 ValePlazaUnusedPaletteSteps[] = {
	0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
	5, 5, 6, 6, 6, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5,
};

Sint32 ValePlazaChargedUpStrongmanActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(214, 0, 721),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(40),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_MOTION, ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(214, 0, 763),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePlazaSecondTravelerLeapActions[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(12)),
	SC_MOVE(114, 144, 360),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ValePlazaFirstTravelerLeapActions[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(12)),
	SC_MOVE(114, 144, 343),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ValePlazaLeaderActions[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE(82, 144, 343),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ValePlazaStormGaretLeapActions[] = {
	SC_SPEED(2),
	SC_ACCELERATION(2),
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE(82, 144, 360),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ValePlazaFirstTravelerLeapAwayActions[] = {
	SC_ANIMATION(TRAVELER_ANIMATION_CROUCH),
	SC_WAIT(10),
	SC_ANIMATION(TRAVELER_ANIMATION_LEAP),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_MOVE(65, 0, 328),
	SC_SYNC,
	SC_WAIT(5),
	SC_ANIMATION(TRAVELER_ANIMATION_CROUCH),
	SC_WAIT(10),
	SC_ANIMATION(TRAVELER_ANIMATION_LEAP),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(6)),
	SC_MOVE(31, 0, 338),
	SC_SYNC,
	SC_WAIT(5),
	SC_ANIMATION(TRAVELER_ANIMATION_CROUCH),
	SC_WAIT(10),
	SC_ANIMATION(TRAVELER_ANIMATION_LEAP),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE(0, 0, 338),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ValePlazaSecondTravelerLeapAwayActions[] = {
	SC_ANIMATION(TRAVELER_ANIMATION_CROUCH),
	SC_WAIT(10),
	SC_ANIMATION(TRAVELER_ANIMATION_LEAP),
	SC_SET(SW_VELOCITY_Y, toFIXED(6)),
	SC_MOVE(63, 0, 374),
	SC_SYNC,
	SC_WAIT(5),
	SC_ANIMATION(TRAVELER_ANIMATION_CROUCH),
	SC_WAIT(10),
	SC_ANIMATION(TRAVELER_ANIMATION_LEAP),
	SC_SET(SW_VELOCITY_Y, toFIXED(7)),
	SC_MOVE(17, 0, 338),
	SC_SYNC,
	SC_WAIT(5),
	SC_ANIMATION(TRAVELER_ANIMATION_CROUCH),
	SC_WAIT(10),
	SC_ANIMATION(TRAVELER_ANIMATION_LEAP),
	SC_SE(SE_HIGH_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE(0, 0, 338),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ValePlazaFaceTargetActions[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 ValePlazaFarewellDogActions[] = {
	SC_MOVE(132, 0, 636),
	SC_SYNC,
	SC_MOVE(171, 0, 629),
	SC_SYNC,
	SC_MOVE(207, 0, 627),
	SC_SYNC,
	SC_MOVE(235, 0, 625),
	SC_SYNC,
	SC_MOVE(261, 0, 574),
	SC_SYNC,
	SC_MOVE(327, 0, 547),
	SC_SYNC,
	SC_END,
};

Sint32 ValePlazaFarewellChaseActions[] = {
	SC_MOVE(271, 0, 571),
	SC_SYNC,
	SC_MOVE(187, 0, 577),
	SC_SYNC,
	SC_MOVE(154, 0, 580),
	SC_SYNC,
	SC_MOVE(127, 0, 543),
	SC_SYNC,
	SC_MOVE(139, 0, 498),
	SC_SYNC,
	SC_END,
};

Sint32 ValePlazaFarewellFirstChildActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(137, 0, 639),
	SC_SYNC,
	SC_MOVE(122, 0, 692),
	SC_SYNC,
	SC_MOVE(141, 0, 746),
	SC_SYNC,
	SC_SPEED(1.2),
	SC_ACCELERATION(0.6),
	SC_MOVE(148, 0, 770),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePlazaFarewellSecondChildActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(137, 0, 639),
	SC_SYNC,
	SC_MOVE(122, 0, 692),
	SC_SYNC,
	SC_MOVE(141, 0, 746),
	SC_SYNC,
	SC_SPEED(1.2),
	SC_ACCELERATION(0.6),
	SC_MOVE(164, 0, 758),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePlazaPartyDepartureActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(216, 0, 854),
	SC_SYNC,
	SC_MOVE(215, 0, 921),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ValePlazaHutOwnerSearchActions[] = {
	SC_SPEED(0.5),
	SC_ACCELERATION(0.05),
	SC_SET(SW_COLLISION, ACTOR_SOLID | ACTOR_PUSHABLE),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(20),
	SC_ANIMATION(ANIM_WALK),
	SC_WANDER_HOME(4, 8, 16),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

Sint32 ValePlazaAideRunActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(148, 0, 758),
	SC_SYNC,
	SC_MOVE(128, 0, 709),
	SC_SYNC,
	SC_MOVE(129, 0, 647),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ValePlazaLeaderStepActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(196, 0, 774),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePlazaStormGaretStepActions[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(196, 0, 790),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

SCENE_ENTRANCE ValePlazaEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, 248, 0, 295, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_B_NORTH, CONDITION_ALWAYS, 352, 0, 308, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, 230, 0, 423, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, 374, 0, 435, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, 200, 0, 562, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_B_WEST, CONDITION_ALWAYS, 102, 0, 627, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_COTTAGE_ROOMS_B_SOUTH, CONDITION_ALWAYS, 358, 0, 678, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_VALE, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_WORLD_MAP, CONDITION_ALWAYS, 217, 0, 843, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_CHEST_CAVE, CONDITION_ALWAYS, 186, 0, 474, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_FROM_SANCTUM, CONDITION_ALWAYS, 180, 0, 618, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_CHEST_CAVE_UNUSED, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ VALE_PLAZA_AFTER_TRAVELERS_BATTLE, CONDITION_ALWAYS, 27, 0, 338, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 ValePlazaExits[] = {
	SCENE_EXITS(ValePlaza),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTH, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHEAST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHWEST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_WEST),
	SCENE_EXIT(VALE_PLAZA_TO_VALE, Vale, VALE_FROM_VALE_PLAZA),
	SCENE_EXIT(VALE_PLAZA_TO_TITLE, Title, TITLE_FROM_VALE_PLAZA),
	SCENE_EXIT(VALE_PLAZA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_VALE_PLAZA),
	SCENE_EXIT(VALE_PLAZA_TO_CHEST_CAVE, GomaCaveStream, GOMA_CAVE_STREAM_CHEST_CAVE_FROM_VALE_PLAZA),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTHEAST, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_SOUTHEAST),
#if !DEBUG_STORY_PRESET_THROUGH_KALAY
	SCENE_EXIT(VALE_PLAZA_TO_BILIBIN, Bilibin, BILIBIN_FROM_WORLD_MAP),
#else
	SCENE_EXIT(VALE_PLAZA_TO_BILIBIN, SolSanctumStatue, SOL_SANCTUM_STATUE_FROM_VALE_PLAZA),
#endif
	SCENE_EXITS_END,
};

Uint32 ValePlazaStormExits[] = {
	SCENE_EXITS(ValePlaza),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTH, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHEAST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHWEST),
	SCENE_EXIT(VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE_PLAZA_WEST),
	SCENE_EXIT(VALE_PLAZA_TO_VALE, ValePrologue, VALE_PROLOGUE_FROM_VALE_PLAZA),
	SCENE_EXIT(VALE_PLAZA_TO_TITLE, Title, TITLE_FROM_VALE_PLAZA),
	SCENE_EXITS_END,
};

ACTOR_PLACE ValePlazaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		186, 0, 460, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 560, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		392, 0, 712, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		56, 0, 616, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 760, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 744, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		168, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 600, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 824, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 568, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		240, 0, 760, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 680, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1608, 0, 1608, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		370, 0, 319, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValePlazaStormActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		186, 0, 460, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 560, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		249, 0, 628, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		214, 0, 635, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		294, 0, 670, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		136, 0, 678, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		87, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		173, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		213, 0, 771, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
#if VALE_PLAZA_SECOND_AIDE_MOVED_NORTHWEST
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		198, 0, 779, FACING_EAST_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
#else
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		204, 0, 783, FACING_EAST_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
#endif
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		250, 0, 773, FACING_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		56, 0, 729, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		56, 0, 729, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		215, 0, 709, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GARET_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		234, 0, 786, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValePlazaDepartureActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		186, 0, 460, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 560, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		225, 0, 814, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 607, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 607, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		199, 0, 781, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		180, 0, 790, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 778, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		262, 0, 799, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		270, 0, 814, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 787, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		234, 0, 781, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		268, 0, 742, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		286, 0, 761, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		271, 0, 778, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		246, 0, 762, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		227, 0, 760, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		209, 0, 760, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		288, 0, 795, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		161, 0, 790, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		171, 0, 815, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		191, 0, 758, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValePlazaActorPlacementsAfterKalay[] = {
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		186, 0, 460, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		252, 0, 480, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 560, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		217, 0, 804, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		107, 0, 765, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		267, 0, 744, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 681, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
		391, 0, 716, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		56, 0, 617, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		166, 0, 603, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		176, 0, 568, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		315, 0, 823, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		287, 0, 571, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		370, 0, 319, FACING_WEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE ValePlazaCues[] = {
	{ CUE_TALK, ACTOR_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeQuietDay) },
	{ CUE_TALK, ACTOR_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeKradenHadVisitors) },
	{ CUE_TALK, ACTOR_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaAskAboutMeditation) },
	{ CUE_TALK, ACTOR_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaRecallThreeYearsAgo) },
	{ CUE_TALK, ACTOR_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeChosenPeople) },
	{ CUE_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeProudOfAdeptGrandfather) },
	{ CUE_TALK, ACTOR_FIRST_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaAskAboutTheTravelers) },
	{ CUE_TALK, ACTOR_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValePracticePsynergy) },
	{ CUE_TALK, ACTOR_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaDemonstratePsynergy) },
	{ CUE_TALK, ACTOR_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeWalkToRecoverPsynergy) },
	{ CUE_TALK, ACTOR_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeFoodStoredForWinter) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenNorthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleWestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleEastDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSouthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenWestEdgeDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSoutheastDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PLAZA_TOWN_GATE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaLeaveThroughTownGate) },
	{ CUE_EXIT, METATILE_BEHAVIOR_VALE_PLAZA_WEST_PATH, CONDITION_ALWAYS, VALE_PLAZA_TO_VALE },
	{ CUE_EXIT, VALE_PLAZA_TO_CHEST_CAVE, CONDITION_ALWAYS, VALE_PLAZA_TO_CHEST_CAVE },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, FLAG_VALE_FIRST_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwayFirstIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_IVY, FLAG_VALE_SECOND_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwaySecondIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THIRD_IVY, FLAG_VALE_THIRD_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwayThirdIvy) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_PLAZA_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaUsePsynergyStone) },
	{ CUE_TABLE_END },
};

CUE ValePlazaStormCues[] = {
	{ CUE_TALK, ACTOR_STORM_CURIOUS_OLD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaAskAboutJourney) },
	{ CUE_TALK, ACTOR_STORM_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeOldWomanCannotUsePsynergy) },
	{ CUE_TALK, ACTOR_STORM_ROCK_DEFLECTING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgValeExhaustedFromFallingRocks) },
	{ CUE_TALK, ACTOR_STORM_WOMAN_HEARING_OF_FELIX, CONDITION_ALWAYS, MSGNO(MsgValeFelixFellInRiver) },
	{ CUE_TALK, ACTOR_STORM_ROCK_LIFTING_BOY, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaBoyLiftsSmallRock) },
	{ CUE_TALK, ACTOR_STORM_GIRL_WHO_SAW_JENNA, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaTalkAboutHelping) },
	{ CUE_TALK, ACTOR_STORM_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeRaceAgainstTime) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenNorthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleWestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleEastDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSouthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenWestEdgeDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSoutheastDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PLAZA_WEST_PATH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaLeaveThroughWestPassage) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PLAZA_GATHERED_VILLAGERS, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaSentToAidElders) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_PLAZA_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaUsePsynergyStone) },
	{ CUE_TABLE_END },
};

CUE ValePlazaDepartureCues[] = {
	{ CUE_TABLE_END },
};

CUE ValePlazaCuesAfterLeaving[] = {
	{ CUE_TALK, ACTOR_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeTremorsSettled) },
	{ CUE_TALK, ACTOR_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeRoadToVault) },
	{ CUE_TALK, ACTOR_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, MSGNO(MsgValeRoadToGomaRange) },
	{ CUE_TALK, ACTOR_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, MSGNO(MsgValePsynergyStonePowder) },
	{ CUE_TALK, ACTOR_CURIOUS_OLD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaAskAboutJourney) },
	{ CUE_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeLunpaFoundedNorthernTown) },
	{ CUE_TALK, ACTOR_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeDoraHasBeenCrying) },
	{ CUE_TALK, ACTOR_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeStonesScatteredByEruption) },
	{ CUE_TALK, ACTOR_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaDemonstratePsynergy) },
	{ CUE_TALK, ACTOR_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeReplenishPsynergyBeforeDungeon) },
	{ CUE_TALK, ACTOR_HUT_OWNER, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaTalkAboutFallenStone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_QUIET_DAY, CONDITION_ALWAYS, MSGNO(MsgValeWorriedAboutAnotherEruption) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_KRADENS_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeVaultUndergroundPath) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_ABOUT_MEDITATION, CONDITION_ALWAYS, MSGNO(MsgValeGomaWinterCold) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_RECALLING_STORM, CONDITION_ALWAYS, MSGNO(MsgValeStonesPowerAleph) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeProofOfWiseOne) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeLunpaThievesChanged) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeDoraMustBeLonely) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeLuckyToFindPsynergyStones) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_SHOWING_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAlive) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_WALK_TO_RECOVER, CONDITION_ALWAYS, MSGNO(MsgValeRecoverPsynergyByWalking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeStonesReplenishPsynergy) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenNorthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleWestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleEastDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSouthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenWestEdgeDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSoutheastDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PLAZA_TOWN_GATE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaLeaveThroughTownGate) },
	{ CUE_EXIT, METATILE_BEHAVIOR_VALE_PLAZA_WEST_PATH, CONDITION_ALWAYS, VALE_PLAZA_TO_VALE },
	{ CUE_EXIT, VALE_PLAZA_TO_CHEST_CAVE, CONDITION_ALWAYS, VALE_PLAZA_TO_CHEST_CAVE },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, FLAG_VALE_FIRST_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwayFirstIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_IVY, FLAG_VALE_SECOND_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwaySecondIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THIRD_IVY, FLAG_VALE_THIRD_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwayThirdIvy) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_PLAZA_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaUsePsynergyStone) },
	{ CUE_TABLE_END },
};

CUE ValePlazaCuesAfterKalay[] = {
	{ CUE_TALK, ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaWelcomeIsaacHome) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_WOMAN_BACK_SOONER, CONDITION_ALWAYS, MSGNO(MsgValeCameBackSooner) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_MAN_ON_NEW_FRIENDS, CONDITION_ALWAYS, MSGNO(MsgValeMadeFriendsOnJourney) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_WOMAN_ON_HOT_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeRiverHeatedAndCooled) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaAskAboutJourney) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeReturnHowHaveYouBeen) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHome) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWantToMakeFriends) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_STRONGMAN_ON_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeEruptionMadeMess) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_STRONGMAN_ON_CRACK, CONDITION_ALWAYS, MSGNO(MsgValeNorthwestCrack) },
	{ CUE_TALK, ACTOR_AFTER_KALAY_HUT_OWNER, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaPsynergyStoneIsGone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgValeSurprisedByEarlyReturn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_WOMAN_BACK_SOONER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacHasGrown) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_MAN_ON_NEW_FRIENDS, CONDITION_ALWAYS, MSGNO(MsgValeNewFriendsAreAdepts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_WOMAN_ON_HOT_RIVER, CONDITION_ALWAYS, MSGNO(MsgValeEruptionHeatsRiver) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_CURIOUS_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgValeStrangeEventsElsewhere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgValeWhenDidYouReturn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_FIRST_CHILD, CONDITION_ALWAYS, MSGNO(MsgValePlayTogetherAgain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_SECOND_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeWantToJourney) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_STRONGMAN_ON_ERUPTION, CONDITION_ALWAYS, MSGNO(MsgValeLavaBurntOtherSide) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_STRONGMAN_ON_CRACK, CONDITION_ALWAYS, MSGNO(MsgValeCrackWidening) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_AFTER_KALAY_HUT_OWNER, FLAG_VALE_PSYNERGY_STONE_GONE_TOLD, SCRIPT_REF(scValePlazaPsynergyStoneIsGone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFTER_KALAY_HUT_OWNER, CONDITION_ALWAYS, MSGNO(MsgValeEruptionTooMuchForPeople) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenNorthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenNortheastDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_WEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleWestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_EAST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenMiddleEastDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_A_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSouthwestDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenWestEdgeDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaOpenSoutheastDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_VALE_PLAZA_TOWN_GATE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaLeaveThroughTownGate) },
	{ CUE_EXIT, METATILE_BEHAVIOR_VALE_PLAZA_WEST_PATH, CONDITION_ALWAYS, VALE_PLAZA_TO_VALE },
	{ CUE_EXIT, VALE_PLAZA_TO_CHEST_CAVE, CONDITION_ALWAYS, VALE_PLAZA_TO_CHEST_CAVE },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, FLAG_VALE_FIRST_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwayFirstIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_IVY, FLAG_VALE_SECOND_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwaySecondIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THIRD_IVY, FLAG_VALE_THIRD_IVY_BLOWN_AWAY, SCRIPT_REF(scValePlazaBlowAwayThirdIvy) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VALE_PLAZA_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scValePlazaUsePsynergyStone) },
	{ CUE_TABLE_END },
};

/* A door of the plaza opening, in three pictures a tick each. */
Uint16 ValePlazaDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(0, 57, 2, 2, 1), MAPGRID_ANIMATION_STEP(0, 59, 2, 2, 1), MAPGRID_ANIMATION_STEP(2, 59, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */
