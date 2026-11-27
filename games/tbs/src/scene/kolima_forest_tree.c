// Kolima Forest (SceneIdKolimaForestTree), the last of the forest's five scenes, where Tret and
// Laurel, the spirits of the holy trees, stand. The party has come from Kolima to reawaken Tret's
// gentle side and save the villagers he turned into trees, and here the curse is lifted.
//
// At first Tret is silent and his wrathful side answers for him (ACTOR_TRETS_WRATHFUL_SIDE, with a
// portrait): "I control all in Tret's realm!"
//
// Laurel, on the first talk (FLAG_MET_LAUREL), knows the party as the ones who resisted Tret's
// glamour, but says they are too late. Tret will die and the forest with him.
//
// Mind Read hears Laurel think that the healing waters of Mercury Lighthouse might save Tret,
// though its fountain has dried up. It hears Tret ask Kolima's forgiveness.
//
// Using Hermes' Water on Tret now, the wrathful side throws the leader back.
//
// The way on is into Tret himself. The region at his foot is KOLIMA_FOREST_TREE_TO_TREE_ROOMS, into
// the Tret Tree (SceneIdTreeRooms). There the party defeats the evil within him
// (FLAG_TRET_DEFEATED, set there) and finds him too weak to save Kolima.
//
// Back here, with Hermes' Water, which Mercury Lighthouse's fountain gives in an Empty Bottle,
// talking to Tret opens the party's items. Using the water plays scKolimaForestTreeRestoreTret:
//
// - The forest's colours return.
// - Tret frees Kolima's people (FLAG_KOLIMA_CURSE_LIFTED).
// - He and Laurel tell how the gems that fell from the sky, the Psynergy
//       Stones, twisted their hearts.
// - Laurel warns of an evil forest risen south of the river.
//
// The bottle is left empty. Afterwards Tret and Laurel thank the party and worry about what lies
// beyond the river.
//
// Until the curse is lifted the forest keeps its cursed colours (scKolimaPaletteCurse), which keeps
// the healthy set and the cursed set both.
//
// As the water heals Tret, scKolimaForestTreeRestoreTret fades background palettes 0 to 13 to the
// healthy set. Then it pulses them towards the healthy set and back towards the cursed one every 30
// ticks (scKolimaForestTreeUpdateHealingPalette) while the camera pans to three places. It flashes
// between the two sets four times and settles on the healthy one. All this runs from the line that
// the water has seeped into Tret until Tret speaks.
//
// Retreat from here always leads back to the forest's way in (SceneIdKolimaForestEntrance,
// KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP). The way back,
// KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, leads to the Floodgate Switch's scene
// (SceneIdKolimaForestFloodgate).

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/field_effect.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "scene/kolima_palette.h"
#include "scene/party_nod.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern Sint32            KolimaForestTreeGaretApproachActions[];
extern Sint32            KolimaForestTreeIvanApproachActions[];
extern Sint32            KolimaForestTreeMiaApproachActions[];
extern Sint32            KolimaForestTreeReturnToPartyActions[];
extern SCENE_ENTRANCE    KolimaForestTreeEntrances[];
extern SCENE_EXIT_REGION KolimaForestTreeRegions[];
extern Uint32            KolimaForestTreeExits[];
extern ACTOR_PLACE       KolimaForestTreeActorPlacements[];
extern CUE               KolimaForestTreeCues[];
extern Sint32            KolimaForestTreeRepelRingScript[];

// The actors, numbering KolimaForestTreeActorPlacements' rows from ACTOR_FIRST_PLACED.
enum
{
	// Tret's face, drawn in SPRITE_TRET_RAISED. Its animations are the changes of his face
	// (scKolimaForestTreeChangeFace).
	ACTOR_TRET = ACTOR_FIRST_PLACED,
	ACTOR_LAUREL,
	// XXX (no evidence): why these exist; likely left from a version where they were drawn, but the
	// ROM keeps no such version. Blank actors 28 pixels up at Tret's and Laurel's spots, never
	// targeted. Nothing addresses them but scKolimaForestTreeChangeFace, which takes one to say
	// whose face to move.
	ACTOR_INVISIBLE_TRET,
	ACTOR_INVISIBLE_LAUREL,
	// An invisible actor on Tret's spot that answers for him. Talking to it, Mind Read on it and
	// Hermes' Water used on it reach Tret.
	ACTOR_TRET_STAND_IN,
	// The invisible actor on Laurel's spot that answers for her when she is talked to or her mind
	// is read.
	ACTOR_LAUREL_STAND_IN,
	// Tret's wrathful side, the voice that speaks in a portrait window on the right: SPRITE_TRET on
	// Tret's spot, 28 pixels up, kept hidden and never targeted (scInit).
	ACTOR_TRETS_WRATHFUL_SIDE
};

static void scKolimaForestTreeRejectHermesWater(void);
static void scKolimaForestTreeRestoreTret(void);
static void scKolimaForestTreeSpawnRepelSpark(void);
static void scKolimaForestTreeUpdateHealingPalette(void);

// The face changes scKolimaForestTreeChangeFace plays, each from the face with its eyes open.
// Laurel has only the first six, and the wrathful ones, on Tret's wrathful face, are never asked
// for here.
typedef enum KOLIMA_FOREST_TREE_FACE_CHANGE
{
	FACE_CHANGE_CLOSE_EYES          = 0,
	FACE_CHANGE_OPEN_EYES           = 1,
	FACE_CHANGE_LOOK_LEFT           = 2,
	FACE_CHANGE_LOOK_RIGHT          = 3,
	FACE_CHANGE_BLINK               = 4, // twice
	FACE_CHANGE_HALF_CLOSE_EYES     = 5,
	FACE_CHANGE_WRATHFUL_CLOSE_EYES = 6,
	FACE_CHANGE_WRATHFUL_OPEN_EYES  = 7,
	FACE_CHANGE_WRATHFUL_LOOK_RIGHT = 8,
	FACE_CHANGE_WRATHFUL_LOOK_LEFT  = 9,
	FACE_CHANGE_WRATHFUL_BLINK      = 10,
	FACE_CHANGE_WRATHFUL_STARE      = 11 // as FACE_CHANGE_WRATHFUL_OPEN_EYES
} KOLIMA_FOREST_TREE_FACE_CHANGE;

static void scKolimaForestTreeChangeFace(Sint32 blankActorId, Sint32 change);
static void scKolimaForestTreeSpawnOrbitSparks(void);
static void scInventoryReplaceItem(Sint32 oldItem, Sint32 newItem);

// Where Tret's sparks spring from (16.16 pixels), the tick of their cycle, whether a repelling
// spark plays its sound, and the ticks of the healing palette's spread.
Sint32 KolimaForestTreeSparkOrigin[3] BSS_DATA ALIGNED(16);
Sint32 KolimaForestTreeSparkTick               BSS_DATA;
Sint32 KolimaForestTreePlaySparkSound          BSS_DATA;
Sint32 KolimaForestTreePaletteTick             BSS_DATA;
// Unused: eight bytes after the palette tick that nothing reads.
Uint8                                          KolimaForestTreeUnusedAfterPaletteTick[8] BSS_DATA;
// The healthy colours, kept by scKolimaPaletteSaveHealthy: the palette fade's 448 background and
// sprite colours, in the array's first half.
Uint8                                          KolimaPaletteHealthy[1792] BSS_DATA;
// The cursed colours, kept by scKolimaPaletteSaveCursed.
Uint8                                          KolimaPaletteCursed[896] BSS_DATA;

const SCENE_ENTRANCE                          *scGetEntrances(void)
{
	return KolimaForestTreeEntrances;
}

// The region at Tret's foot that leads into the Tret Tree.
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return KolimaForestTreeRegions;
}

// The ways back (SceneIdKolimaForestFloodgate) and into the Tret Tree (SceneIdTreeRooms).
const Uint32 *scGetExits(void)
{
	return KolimaForestTreeExits;
}

// Tret and Laurel, the blank actors at their spots, the ones the party talks to, Tret's wrathful
// side, and Garet, Ivan and Mia for the healing.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return KolimaForestTreeActorPlacements;
}

// Unused: takes the touched exit after the leader's current movement. No cue row names it.
UNUSED static void scKolimaForestTreeExitTouchedRegion(void)
{
	FIELD_STATE *state;

	state = FieldStateWork;
	fdScriptStart();
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
	syImportScriptSetExitRequest(state->stepCue);
	fdScriptEnd();
}

// Talking to Tret and Laurel, reading their minds, and Hermes' Water on Tret.
const CUE *scGetCues(void)
{
	return KolimaForestTreeCues;
}

// In Kolima Forest (SceneIdKolimaForestTree), the first time the party talks to Laurel
// (ACTOR_LAUREL_STAND_IN), she asks who the party is.
static void scKolimaForestTreeTalkToLaurel(void)
{
	fdScriptStart();
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_OPEN_EYES);
	if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
	{
		fdScriptSetMessageId(MsgKolimaForestKolimaAlive);
		// "The forest of Kolima is alive once again, thanks to you......"
		fdScriptShowMessage(ACTOR_LAUREL, 0);
	}
	else if (fdScriptGetFlag(FLAG_MET_LAUREL) != 0)
	{
		fdScriptSetMessageId(MsgKolimaLeaveBeforeForest);
		// "Leave now, before this forest is destroyed..."
		fdScriptShowMessage(ACTOR_LAUREL, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgKolimaWho);
		// "Who are you...?"
		fdScriptShowMessageThenWait(ACTOR_LAUREL, 0, 20);
		scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_CLOSE_EYES);
		fdScriptWait(60);
		scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_OPEN_EYES);
		// "Oh! You are the ones who resisted Tret's glamour?"
		fdScriptShowMessageThenWait(ACTOR_LAUREL, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(40);
		// "I sense a great warmth within you..."
		fdScriptShowMessage(ACTOR_LAUREL, 0);
		scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_CLOSE_EYES);
		fdScriptWait(80);
		// "If you came to rescue Tret, you are already too late."
		fdScriptShowMessageThenWait(ACTOR_LAUREL, 0, 20);
		scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_OPEN_EYES);
		// "Soon, Tret will die, and we shall wither away without him...."
		fdScriptShowMessageThenWait(ACTOR_LAUREL, 0, 20);
		gmFlagSet(FLAG_MET_LAUREL);
	}
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_CLOSE_EYES);
	fdScriptEnd();
}

// In Kolima Forest (SceneIdKolimaForestTree), talking to Tret (ACTOR_TRET_STAND_IN) finds him
// silent until the curse is lifted.
static void scKolimaForestTreeTalkToTret(void)
{
	Sint32 waterHolder;

	fdScriptStart();
	if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
	{
		scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
		fdScriptSetMessageId(MsgKolimaNowHaveSuchPowerAxe);
		// "Now that I have such power, no axe shall ever touch me..."
		fdScriptShowMessage(ACTOR_TRET, 0);
		scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_CLOSE_EYES);
	}
	else
	{
		if (gmFlagGet(FLAG_TRET_DEFEATED) != 0)
		{
			scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
			fdScriptSetMessageId(MsgKolimaSilenceAfterTretDefeated);
			// "..."
			fdScriptShowMessage(ACTOR_TRET, 0);
			scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_CLOSE_EYES);
			waterHolder = gmPartyInventoryFindMember(ITEM_HERMES_WATER);
			// Someone carries Hermes' Water: the field menu opens, for it.
			if (waterHolder != -1)
			{
				FieldStateWork->fieldMenu = 1;
			}
		}
		else
		{
			fdScriptSetMessageId(MsgKolimaSilence);
			// "..."
			fdScriptShowMessage(ACTOR_TRET, 0);
			fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
			fdPaletteFadeStartInterpolation(20);
			sySynch(40);
			// "Hroooom! He will not speak to you, child!"
			syFieldScriptShowMessageThenWait(
				SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0, 10);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			// "I control all in Tret's realm!"
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0);
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeStartInterpolation(20);
			sySynch(40);
		}
	}
	fdScriptEnd();
}

// In Kolima Forest (SceneIdKolimaForestTree), Mind Read on Laurel hears her thoughts.
static void scKolimaForestTreeReadLaurelThoughts(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
	{
		fdScriptSetMessageId(MsgKolimaMustHorribleBeyondRiverAm);
		// "It must be horrible beyond the river... I am so worried about..."
	}
	else
	{
		fdScriptSetMessageId(MsgKolimaHealingWatersMercuryLighthouseMight);
		// "The healing waters of Mercury Lighthouse might save Tret..."
	}
	fdScriptShowMessage(ACTOR_LAUREL, 0);
	fdScriptEnd();
}

// In Kolima Forest (SceneIdKolimaForestTree), Mind Read on Tret hears his thoughts.
static void scKolimaForestTreeReadTretThoughts(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
	{
		fdScriptSetMessageId(MsgKolimaKnowCannotStopButPlease);
		// "I know that I cannot stop you, but please be careful beyond..."
	}
	else
	{
		fdScriptSetMessageId(MsgKolimaPeopleKolimaForgiveMe);
		// "People of Kolima... Forgive me..."
	}
	fdScriptShowMessage(ACTOR_TRET, 0);
	fdScriptEnd();
}

// In Kolima Forest (SceneIdKolimaForestTree), using Hermes' Water on Tret while the curse lasts
// heals him only once the evil within him is defeated in the Tret Tree (FLAG_TRET_DEFEATED). Before
// that his wrathful side repels it (scKolimaForestTreeRejectHermesWater). Afterwards it heals him
// (scKolimaForestTreeRestoreTret).
static void scKolimaForestTreeUseHermesWater(void)
{
	fdScriptStart();
	fdEndBusyAndReveal();
	if (gmFlagGet(FLAG_TRET_DEFEATED) == 0)
	{
		scKolimaForestTreeRejectHermesWater();
	}
	else
	{
		scKolimaForestTreeRestoreTret();
	}
	fdScriptEnd();
}

// Tret's wrathful side repels Hermes' Water.
static void scKolimaForestTreeRejectHermesWater(void)
{
	ACTOR *leader;

	// The wrathful side throws the leader back before the water can heal Tret.
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(20);
	sySynch(40);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	KolimaForestTreePlaySparkSound = 1;
	sySetTickEvent(scKolimaForestTreeSpawnRepelSpark, EVENT_PASS_TICK);
	sySynch(30);
	KolimaForestTreePlaySparkSound = 0;
	fdCameraMoveTo(328, CAMERA_KEEP, 235, 1);
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
	fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
	sySoundPlay(SE_IMPACT);
	leader->velocityY = toFIXED(5);
	leader->gravity   = toFIXED(1) / 4;
	leader->bounce    = toFIXED(5) / 8;
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 335, 258);
	while (leader->velocityY >= 0)
	{
		sySynch(1);
	}
	do
	{
		sySynch(1);
	} while (leader->velocityY <= 0);
	sySoundPlay(SE_RATTLE);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
	fdScriptWait(120);
	syCloseEvent(scKolimaForestTreeSpawnRepelSpark);
	sySynch(40);
	leader->bounce = toFIXED(1) / 4;
	{
		ACTOR *leader         = fdActorGet(ACTOR_PARTY_LEADER);
		leader->steeringFlags = ACTOR_FACES_MOTION | leader->steeringFlags;
	}
	fdScriptWait(80);
	// Hear the wrathful side's refusal, then restore the leader's motion.
	fdScriptSetMessageId(MsgKolimaControlTretsHeartShallNot);
	// "I control Tret's heart! I shall not let you do anything to him!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	// "Tret, the forest, and the village of Kolima all shall wither..."
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0);
	gmPlayMusic();
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(20);
	sySynch(40);
	leader->facing  = FACING_NORTH;
	leader->gravity = toFIXED(1);
	leader->bounce  = toFIXED(1) / 4;
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(40);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(20);
}

// Turns the four of them to facing together, then waits waitTicks.
static inline void scKolimaForestTreeFaceParty(Sint32 facing, Sint32 waitTicks)
{
	fdActorFaceDirection(ACTOR_PARTY_LEADER, facing, 0);
	fdActorFaceDirection(ACTOR_GARET, facing, 0);
	fdActorFaceDirection(ACTOR_MIA, facing, 0);
	fdActorFaceDirection(ACTOR_IVAN, facing, waitTicks);
}

// Hermes' Water heals Tret.
static void scKolimaForestTreeRestoreTret(void)
{
	Sint32        includeMia;
	ACTOR        *leader;
	Uint32        flash;
	void          (*spawnOrbitSparks)(void);
	const Sint32 *finalActions;

	includeMia = gmFlagGet(FLAG_IN_PARTY(MIA));
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 328, 212);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	mnFieldMessageShow(MsgKolimaWaterHermesSeepedIntoTret, 1);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if (leader != NULL)
	{
		fdActorSetPosFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
	}
	fdActorSetScript(ACTOR_GARET, KolimaForestTreeGaretApproachActions);
	fdActorSetScript(ACTOR_IVAN, KolimaForestTreeIvanApproachActions);
	if (includeMia != 0)
	{
		fdActorSetSpeed(ACTOR_MIA, 1);
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			fdActorSetPosFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
		}
		fdActorSetScript(ACTOR_MIA, KolimaForestTreeMiaApproachActions);
	}
	fdActorWaitForScriptEndById(ACTOR_IVAN);
	fdScriptWait(40);
	scKolimaPaletteRestore(0);
	fdPaletteFadeStartInterpolation(32);
	sySynch(40);
	// Carry the healing across the forest as its healthy colours return.
	KolimaForestTreePaletteTick = 0;
	sySetTickEvent(scKolimaForestTreeUpdateHealingPalette, EVENT_PASS_TICK);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(256, CAMERA_KEEP, 254, 1);
	fdCameraWaitForArrival();
	sySoundPlay(SE_GLITTER);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 20);
	fdCameraMoveTo(413, CAMERA_KEEP, 261, 1);
	fdCameraWaitForArrival();
	sySoundPlay(SE_GLITTER);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
	fdCameraMoveTo(326, CAMERA_KEEP, 384, 1);
	fdCameraWaitForArrival();
	sySoundPlay(SE_GLITTER);
	while (KolimaForestTreePaletteTick != 24)
	{
		sySynch(1);
	}
	syCloseEvent(scKolimaForestTreeUpdateHealingPalette);
	sySynch(10);
	for (flash = 0; flash <= 3; flash++)
	{
		scKolimaPaletteRestore(0);
		fdPaletteFadeStartInterpolation(6);
		sySynch(6);
		scKolimaPaletteRestore(1);
		fdPaletteFadeStartInterpolation(6);
		sySynch(6);
	}
	scKolimaPaletteRestore(0);
	fdPaletteFadeStartInterpolation(40);
	sySynch(80);
	fdCameraMoveTo(328, 8, 212, 1);
	fdCameraWaitForArrival();
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(40);
	sySoundPlay(MUS_TRIAL_ROAD);
	// Tret and Laurel discover that their powers have returned.
	fdScriptSetMessageId(MsgKolimaFeelGreatPowerSpreadingThrough);
	// "I feel a great power spreading through me..."
	fdScriptShowMessage(ACTOR_TRET, 0);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	scKolimaForestTreeFaceParty(FACING_NORTH, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(20);
	// "Hroooom... Life flows within me once again."
	fdScriptShowMessage(ACTOR_TRET, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdCameraMoveTo(234, 0, 232, 1);
	fdCameraWaitForArrival();
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(20);
	// "Tret..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	// "As you have recovered, so has our forest..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_CLOSE_EYES);
	fdScriptWait(20);
	// "I am sorry to have worried you so, Laurel."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(20);
	// "And you, young ones... Didn't you have a favor to ask of me?"
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
	{
		// "Yes, Tret... You must turn the people of Kolima into humans..."
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
		// "Yes, that's right... I must hurry."
		fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	}
	else
	{
		FieldStateWork->msg += 2;
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
		// "Isaac! Did you forget? We need to save the people of Kolima!"
		fdScriptShowMessage(ACTOR_GARET, 0);
		if (includeMia != 0)
		{
			fdActorRunRepeatedMotion(ACTOR_MIA, 2);
			// "Tret should be able to do it, now that his energy has returned."
			fdScriptShowMessage(ACTOR_MIA, 0);
		}
		else
		{
			FieldStateWork->msg += 1;
		}
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		// "Ask him to save everyone he turned into a tree!"
		fdScriptShowMessage(ACTOR_IVAN, 0);
		// "Please, Tret... You have to save those whom you turned into..."
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
		// "That's right... I must hurry."
		fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	}
	scNodTogether();
	fdCameraMoveTo(328, 8, 212, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_CLOSE_EYES);
	fdScriptWait(20);
	scKolimaPaletteRestore(0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(40);
	fdScriptWait(60);
	KolimaForestTreeSparkTick      = 0;
	// Tret sends out the power that frees Kolima's villagers.
	KolimaForestTreeSparkOrigin[0] = toFIXED(328);
	KolimaForestTreeSparkOrigin[1] = toFIXED(48);
	spawnOrbitSparks               = scKolimaForestTreeSpawnOrbitSparks;
	KolimaForestTreeSparkOrigin[2] = toFIXED(205);
	sySetTickEvent(spawnOrbitSparks, EVENT_PASS_TICK);
	fdScriptWait(100);
	syCloseEvent(spawnOrbitSparks);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(100);
	scKolimaPaletteRestore(0);
	fdPaletteFadeStartInterpolation(20);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(10);
	fdScriptSetMessageId(MsgKolimaShouldDoPeopleKolimaCursed);
	// "That should do it. The people of Kolima are cursed no more."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	scNodTogether();
	fdCameraMoveTo(234, 0, 232, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	// "Tret did not always possess this horrible power."
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
	// "It only started when those strange gemstones fell from the sky."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	// "Gems from the sky... Do you think he means the Psynergy Stones?"
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
	{
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	}
	else
	{
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
	}
	// Yes: "You really think Psynergy Stones caused all of this trouble?" No: "I know you don't
	// want to believe it, but nothing else makes..."
	fdScriptShowMessage(ACTOR_GARET, 0);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgKolimaWasIndeedAngryPeopleHad);
	// "I was indeed angry that people had laid waste to my..."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	// "Then those gems fell into my branches..."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	scNodTogether();
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	// "Then all of a sudden... my fury overwhelmed me, and I was lost..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_CLOSE_EYES);
	// "The gem reached me when I was filled with sorrow..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(20);
	// "Our actions spoke our hearts' desires, but we had no idea..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	// "Those gems twisted my anger into a force beyond my control."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_HALF_CLOSE_EYES);
	fdScriptWait(60);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(20);
	// "Have any of the saplings been transformed by those stones, Tret?"
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_HALF_CLOSE_EYES);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	// "No... Only the elder trees were affected by the falling..."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_TRET, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 10);
	// "There are others that this happened to?"
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
	// "So the same thing could be happening elsewhere!"
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	// "You must have fought many monsters on your way here."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	// "Those monsters... Maybe they were just animals that were..."
	fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	// "Once, they were pure of heart... Hroom... Those gems have..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_CLOSE_EYES);
	fdScriptWait(20);
	scKolimaPaletteRestore(0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(40);
	fdScriptWait(60);
	KolimaForestTreeSparkTick      = 0;
	// Laurel looks beyond the river and warns of the forest to the south.
	KolimaForestTreeSparkOrigin[0] = toFIXED(136);
	KolimaForestTreeSparkOrigin[1] = toFIXED(20);
	spawnOrbitSparks               = scKolimaForestTreeSpawnOrbitSparks;
	KolimaForestTreeSparkOrigin[2] = toFIXED(258);
	sySetTickEvent(spawnOrbitSparks, EVENT_PASS_TICK);
	fdScriptWait(100);
	scKolimaForestTreeFaceParty(FACING_SOUTHWEST, 40);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	// "Oh! Laurel, you were given powers, too?"
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	// "Different from my own, but stemming from the same source."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	// "Laurel! What in the world are you doing?"
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 20);
	// "Wait. You will soon see."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
	scNodTogether();
	fdScriptWait(10);
	scKolimaForestTreeFaceParty(FACING_SOUTHWEST, 120);
	syCloseEvent(spawnOrbitSparks);
	fdScriptWait(60);
	scKolimaPaletteRestore(0);
	fdPaletteFadeStartInterpolation(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	// "What did you see, Laurel?"
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	// "It's true... even the animals have become monsters. And..."
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
	// "The river that feeds our forest?"
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	// "Yes... An evil forest has risen up to the south of the river."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	// "Will you be heading south across the river?"
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	if (fdScriptAskYesNoWithActorAnimation(0, 0) == 1)
	{
		FieldStateWork->msg += 1;
	}
	fdScriptWait(10);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_LOOK_LEFT);
	fdScriptWait(20);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_LOOK_RIGHT);
	fdScriptWait(40);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_OPEN_EYES);
	fdScriptWait(20);
	// Yes: "That area will only grow more dangerous as time passes..." No: "That's good... It would
	// be foolish to risk your lives further."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
	scNodTogether();
	sySoundPlay(SOUND_FADE_OUT_BGM);
	finalActions = KolimaForestTreeReturnToPartyActions;
	fdActorSetScript(ACTOR_GARET, finalActions);
	if (includeMia != 0)
	{
		fdActorSetScript(ACTOR_MIA, finalActions);
	}
	fdActorRunScriptAndWait(ACTOR_IVAN, finalActions);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_BLINK);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_TRET, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgKolimaOweGreatDebtHaveSaved);
	// "I owe you a great debt. You have saved Kolima, the forest,..."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_BLINK);
	scKolimaForestTreeChangeFace(ACTOR_INVISIBLE_LAUREL, FACE_CHANGE_BLINK);
	fdScriptWait(20);
	// "May your roots dig deep and your branches reach high."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	gmFlagSet(FLAG_KOLIMA_CURSE_LIFTED);
	sySoundPlay(MUS_TOWN_THEME);
	scInventoryReplaceItem(ITEM_HERMES_WATER, ITEM_EMPTY_BOTTLE);
}

// How far the curse tints the forest round Tret until it is lifted: green and blue lose 1 /
// TREE_CURSE_DIVISOR of themselves (scKolimaPaletteCurse).
#define TREE_CURSE_DIVISOR 3

// Sets up Tret, Laurel and the forest's colours on entry.
Sint32 scInit(void)
{
	ACTOR *tretBlank;
	ACTOR *tretsWrathfulSide;
	ACTOR *laurelBlank;

	tretBlank         = fdActorGet(ACTOR_INVISIBLE_TRET);
	tretsWrathfulSide = fdActorGet(ACTOR_TRETS_WRATHFUL_SIDE);
	laurelBlank       = fdActorGet(ACTOR_INVISIBLE_LAUREL);
	sySynch(1);
	fdActorSetSpriteColorById(ACTOR_TRETS_WRATHFUL_SIDE, SPRITE_COLOR_HIDDEN);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	MainRoutineStateWork.retreatScene    = SCENE_KOLIMA_FOREST_ENTRANCE;
	MainRoutineStateWork.retreatEntrance = KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP;
	if (!gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED))
		scKolimaPaletteCurse(TREE_CURSE_DIVISOR);
	fdActorGet(ACTOR_TRET)->radius            = 6;
	fdActorGet(ACTOR_LAUREL)->radius          = 6;
	fdActorGet(ACTOR_TRET_STAND_IN)->radius   = 6;
	fdActorGet(ACTOR_LAUREL_STAND_IN)->radius = 6;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_TRETS_WRATHFUL_SIDE), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_INVISIBLE_TRET), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_INVISIBLE_LAUREL), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_TRET), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_LAUREL), 0);
	fdActorSetSpritePriority(ACTOR_TRET, 2);
	fdActorSetSpritePriority(ACTOR_TRETS_WRATHFUL_SIDE, 2);
	fdActorSetSpritePriority(ACTOR_LAUREL, 2);
	tretBlank->motionFlags         = 0;
	tretBlank->y.fixed             = toFIXED(28);
	laurelBlank->motionFlags       = 0;
	laurelBlank->y.fixed           = toFIXED(28);
	tretsWrathfulSide->motionFlags = 0;
	tretsWrathfulSide->y.fixed     = toFIXED(28);
	fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_CLOSED);
	fdActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
	fdActorGet(ACTOR_TRET)->collisionFlags                |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_LAUREL)->collisionFlags              |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_INVISIBLE_TRET)->collisionFlags      |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_INVISIBLE_LAUREL)->collisionFlags    |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_TRETS_WRATHFUL_SIDE)->collisionFlags |= ACTOR_NOT_TARGETED;
	return 0;
}

#include "shared/kolima_grow.c"

// Every fourth tick, a golden ring falls from Tret onto the leader as his wrathful side throws him
// back, with its sound while KolimaForestTreePlaySparkSound is set.
static void scKolimaForestTreeSpawnRepelSpark(void)
{
	ACTOR *effect;
	SPR   *sprite;
	Sint32 phase;

	phase = TickCount & 3;
	if (phase != 0)
		return;
	if (KolimaForestTreePlaySparkSound != 0)
		sySoundPlay(SE_SPARKLE_FALL);
	effect = syFieldActorCreate(SPRITE_GOLDEN_RINGS, toFIXED(326), toFIXED(32), toFIXED(192));
	if (effect == NULL)
		return;
	sprite                  = effect->sprite;
	sprite->flags           = phase;
	effect->priorityFlags  &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.priority  = 1;
	effect->scaleX          = toFIXED(1) / 10;
	effect->speedLimit      = toFIXED(4);
	effect->acceleration    = toFIXED(4);
	effect->motionFlags     = phase;
	fdActorSelectAnimation(effect, 2);
	syFieldActorSetMoveTargetFX(effect, toFIXED(326), 0, toFIXED(240));
	fdActorStartScript(effect, KolimaForestTreeRepelRingScript);
}

// The healing palette's cycle: HEALTHY_FADE_TICKS towards the healthy colours, then back towards
// the cursed ones until HEALING_CYCLE_TICKS.
#define HEALTHY_FADE_TICKS  20
#define HEALING_CYCLE_TICKS 30

// Each tick of the healing: the forest's colours fade towards the healthy set, then back towards
// the cursed one, every HEALING_CYCLE_TICKS.
static void scKolimaForestTreeUpdateHealingPalette(void)
{
	Sint32 step = KolimaForestTreePaletteTick;

	if (step == 0)
	{
		scKolimaPaletteRestore(0);
		fdPaletteFadeStartInterpolation(HEALTHY_FADE_TICKS);
	}
	else if (step == HEALTHY_FADE_TICKS)
	{
		scKolimaPaletteRestore(1);
		fdPaletteFadeStartInterpolation(8);
	}
	step                        = KolimaForestTreePaletteTick + 1;
	KolimaForestTreePaletteTick = step;
	if (step == HEALING_CYCLE_TICKS)
	{
		KolimaForestTreePaletteTick = 0;
	}
}

// Alternates two faces twice, ending on the first.
static inline void scKolimaForestTreeSwapFaces(Sint32 actorId, Sint32 firstFace, Sint32 secondFace)
{
	fdActorSetAnimation(actorId, firstFace);
	sySynch(6);
	fdActorSetAnimation(actorId, secondFace);
	sySynch(6);
	fdActorSetAnimation(actorId, firstFace);
	sySynch(6);
	fdActorSetAnimation(actorId, secondFace);
	sySynch(6);
	fdActorSetAnimation(actorId, firstFace);
}

// Plays one of Tret's or Laurel's face changes, chosen by the blank actor at their spot.
static void scKolimaForestTreeChangeFace(Sint32 blankActorId, Sint32 change)
{
	if (blankActorId == ACTOR_INVISIBLE_TRET)
	{
		switch (change)
		{
			case FACE_CHANGE_CLOSE_EYES:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
				break;
			case FACE_CHANGE_OPEN_EYES: fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN); break;
			case FACE_CHANGE_LOOK_LEFT:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_LEFT);
				break;
			case FACE_CHANGE_LOOK_RIGHT:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_RIGHT);
				break;
			case FACE_CHANGE_BLINK:
				scKolimaForestTreeSwapFaces(ACTOR_TRET, TRET_FACE_OPEN, TRET_FACE_CLOSED);
				break;
			case FACE_CHANGE_HALF_CLOSE_EYES:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_HALF_CLOSED);
				break;
			case FACE_CHANGE_WRATHFUL_CLOSE_EYES:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
				break;
			case FACE_CHANGE_WRATHFUL_LOOK_RIGHT:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
				break;
			case FACE_CHANGE_WRATHFUL_LOOK_LEFT:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
				break;
			case FACE_CHANGE_WRATHFUL_BLINK:
				scKolimaForestTreeSwapFaces(
					ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN, TRET_FACE_WRATHFUL_CLOSED);
				break;
			case FACE_CHANGE_WRATHFUL_OPEN_EYES:
			case FACE_CHANGE_WRATHFUL_STARE:
				fdActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
				break;
		}
	}
	else
	{
		switch (change)
		{
			case FACE_CHANGE_CLOSE_EYES:
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_CLOSED);
				break;
			case FACE_CHANGE_OPEN_EYES: fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN); break;
			case FACE_CHANGE_LOOK_LEFT:
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_LOOK_LEFT);
				break;
			case FACE_CHANGE_LOOK_RIGHT:
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_LOOK_RIGHT);
				break;
			case FACE_CHANGE_BLINK:
				scKolimaForestTreeSwapFaces(ACTOR_LAUREL, LAUREL_FACE_OPEN, LAUREL_FACE_CLOSED);
				break;
			case FACE_CHANGE_HALF_CLOSE_EYES:
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
				sySynch(6);
				fdActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_HALF_CLOSED);
				break;
		}
	}
	sySynch(12);
}

// A spark's spiral: it lasts ORBIT_SPARK_TICKS, moving a pixel further out and ORBIT_SPARK_TURN
// round each tick.
#define ORBIT_SPARK_TICKS 120
#define ORBIT_SPARK_TURN  (TRIG_TURN * 3 / 256)

// Moves a sparkle out along its spiral, swelling, for ORBIT_SPARK_TICKS; then it is gone.
static void scKolimaForestTreeUpdateOrbitSpark(ACTOR *spark)
{
	Sint32 pos[3];
	Sint32 age = spark->timer;
	Sint32 heading;

	if (age < ORBIT_SPARK_TICKS)
	{
		SET_VECTOR(pos, spark->targetX, spark->targetY, spark->targetZ);
		heading = spark->userValue;
		syVectorAddPolarOffset(toFIXED(age), age * ORBIT_SPARK_TURN + heading, pos);
		spark->x.fixed  = pos[0];
		spark->y.fixed  = pos[1];
		spark->z.fixed  = pos[2];
		spark->scaleX  += toFIXED(1) / 200;
		spark->scaleY  += toFIXED(1) / 200;
		spark->timer++;
	}
	else
	{
		syVRAMCacheFreeEntry(spark->sprite->slot);
		fdActorRelease(spark);
	}
}

// Sends out a ring of six sparkles spiralling from KolimaForestTreeSparkOrigin with a shimmer,
// every ten ticks for the first 50 of each 121.
static void scKolimaForestTreeSpawnOrbitSparks(void)
{
	ACTOR *spark;
	SPR   *previousSprite;
	Uint32 sparkIndex;

	previousSprite = NULL;
	switch (KolimaForestTreeSparkTick)
	{
		case 0:
		case 10:
		case 20:
		case 30:
		case 40:
			sySoundPlay(SE_SHIMMER);
			for (sparkIndex = 0; sparkIndex <= 5; sparkIndex++)
			{
				spark = syFieldActorCreate(SPRITE_SPARKLE,
				                           KolimaForestTreeSparkOrigin[0],
				                           KolimaForestTreeSparkOrigin[1],
				                           KolimaForestTreeSparkOrigin[2]);
				if (spark != NULL)
				{
					previousSprite     = fdSpriteShareVRAMCacheEntry(spark->sprite, previousSprite);
					spark->motionFlags = 0;
					spark->sprite->image.priority = 1;
					fdActorSetSpriteFlags(spark, 0);
					fdActorSelectAnimation(spark, 1);
					spark->timer      = 0;
					spark->userValue  = DEGtoANG(sparkIndex * 60);
					spark->targetX    = KolimaForestTreeSparkOrigin[0];
					spark->targetY    = KolimaForestTreeSparkOrigin[1];
					spark->targetZ    = KolimaForestTreeSparkOrigin[2];
					spark->speedLimit = toFIXED(8) / 5;
					spark->update     = scKolimaForestTreeUpdateOrbitSpark;
				}
			}
			break;
	}
	if (++KolimaForestTreeSparkTick > 120)
	{
		KolimaForestTreeSparkTick = 0;
	}
}

#include "shared/hermes_water_bottle.c"

#include "shared/kolima_palette.c"

Sint32 KolimaForestTreeGaretApproachActions[] = {
	SC_ANIMATION(2),
	SC_MOVE(312, 0, 230),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

Sint32 KolimaForestTreeIvanApproachActions[] = {
	SC_ANIMATION(2),
	SC_MOVE(346, 0, 224),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

Sint32 KolimaForestTreeMiaApproachActions[] = {
	SC_ANIMATION(2),
	SC_MOVE(333, 0, 240),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

Sint32 KolimaForestTreeReturnToPartyActions[] = {
	SC_ANIMATION(2),
	SC_MOVE(328, 0, 212),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

SCENE_ENTRANCE KolimaForestTreeEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
	{KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE,
     CONDITION_ALWAYS, 136,
     0, 424,
     FACING_NORTH, 0,
     0, 0,
     512, 448,
     0},
	{KOLIMA_FOREST_TREE_FROM_TREE_ROOMS,
     CONDITION_ALWAYS, 328,
     16, 200,
     FACING_SOUTH, 0,
     0, 0,
     512, 448,
     0},
	{SCENE_TABLE_END},
};

// clang-format off
SCENE_EXIT_REGION KolimaForestTreeRegions[] = {
	{320, 88, 176, 336, 104, 192, -1, KOLIMA_FOREST_TREE_TO_TREE_ROOMS},
	{-1, 0, 0, 0, 0, 0, 0, 0},
};

Uint32 KolimaForestTreeExits[] = {
	SCENE_EXITS(KolimaForestTree),
	SCENE_EXIT(KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE),
	SCENE_EXIT(KOLIMA_FOREST_TREE_TO_TREE_ROOMS, TreeRooms, TREE_ROOMS_FROM_KOLIMA_FOREST_TREE),
	SCENE_EXITS_END,
};

ACTOR_PLACE KolimaForestTreeActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TRET_RAISED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		327, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LAUREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 270, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		327, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		136, 0, 270, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		327, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		136, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		327, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE KolimaForestTreeCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE },
	{ USE_ITEM_ON_ACTOR(ITEM_HERMES_WATER), ACTOR_TRET_STAND_IN, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scKolimaForestTreeUseHermesWater) },
	{ CUE_TALK, ACTOR_LAUREL_STAND_IN, CONDITION_ALWAYS, SCRIPT_REF(scKolimaForestTreeTalkToLaurel) },
	{ CUE_TALK, ACTOR_TRET_STAND_IN, CONDITION_ALWAYS, SCRIPT_REF(scKolimaForestTreeTalkToTret) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAUREL_STAND_IN, CONDITION_ALWAYS, SCRIPT_REF(scKolimaForestTreeReadLaurelThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_TRET_STAND_IN, FLAG_TRET_DEFEATED, SCRIPT_REF(scKolimaForestTreeTalkToTret) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRET_STAND_IN, CONDITION_ALWAYS, SCRIPT_REF(scKolimaForestTreeReadTretThoughts) },
	{ CUE_TABLE_END },
};
// clang-format on

Sint32 KolimaForestTreeRepelRingScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 10),
	SC_CALL(scFieldEffectStretchUntilArrived),
	SC_LOOP(SC_FOREVER, SC_TOP),
};
