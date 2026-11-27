/* Kibombo, the warriors' village below the Gabomba Statue, in the six rooms of
   this overlay: the village, its houses, shops, inn and Sanctum
   (scKibomboOpenWeaponShop, scKibomboOpenInn), and the grounds before the
   statue, each by day and on the ceremony night.

   The party arrives on the night of the ceremony. The crowd fills the way
   north, so it ties the rope in the village's south west corner with Lash and
   jumps along the painted poles (scKibomboJumpForward) to the grounds. There
   Akafubu offers the Black Orb to the Great Gabomba (scKibomboCeremonyStarts to
   scKibomboCeremonyEnds), and Piers, come to take his orb back, joins the party
   once Move drops a pillar for him (scKibomboPiersJoinsParty). The path east of
   the statue ends at four marks on the ground; digging them shows the party
   that the patch it passed covers a vent into the statue
   (scKibomboDeadEndByTheStatue, scKibomboScoopVent). The villagers' talk
   changes once Akafubu is the witch doctor (scKibomboTalkPathToPeace). */

#include "graphics/palette_buffers.h"
#include "game/system_call.h"
#include "item_ids.h"
#include "macros.h"
#include "field/field_mode_script.h"
#include "scene/kibombo.h"
#include "field/field_mode.h"
#include "field/ground_mask.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "field/field_sprite.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "shop/shop.h"

ACTOR *syImportActorCreate(Sint32 type, Sint32 x, Sint32 y, Sint32 z);
void   syImportScriptSetExitRequest(Sint32 exit);

void   scKibomboInitVillage(void);
Sint32 scKibomboClampColor(Sint32 value);
void   scKibomboFlashSprite(ACTOR *actor);
void   scKibomboUncoverNorthMark(void);
void   scKibomboUncoverSouthMark(void);
void   scKibomboUncoverWestMark(void);
void   scKibomboUncoverEastMark(void);
void   scKibomboPiersHasForgottenParty(void);
void   scKibomboPiersTakesPartyForEnemies(void);
void   scKibomboAkafubuOffersCrystal(void);
void   scKibomboPiersJoinsParty(void);
void   scKibomboFlickerColor(Sint32 index, Sint32 entry, Sint32 delta);
void   scKibomboFlickerRedColor(Sint32 index, Sint32 entry, Sint32 delta);
void   scKibomboFlickerVillageFires(void);
void   scKibomboFlickerCeremonyFires(void);
void   scKibomboFlickerStatueFires(void);
void   scKibomboRaiseEmbers(void);
void   scKibomboRaiseSmoke(void);
void   scKibomboLightSignalFire(void);
void   scActorHide(Sint32 id);
void   scKibomboRestoreBlocks(void);
void   scKibomboAkafubuChants(void);
void   scKibomboSetStatueMouth(Sint32 open, Sint32 glow);
void   scKibomboSetStatueTongue(Sint32 open, Sint32 pose);
void   scKibomboStopFireFlicker(void);
void   scKibomboClimbDownLadder(Sint32 x, Sint32 z);
void   scKibomboOpenCeremonyScene(void);
void   scKibomboCrowdPanics(void);

extern Sint32 LeaderWalkThroughDoorScript[];
extern Uint32 KibomboKradenMeetsPiersScript[];
extern Uint32 KibomboJennaMeetsPiersScript[];
extern Sint32 KibomboShebaMeetsPiersScript[];
extern Uint32 KibomboPartyGathersScript[];
extern Sint32 KibomboEyesShakeScript[];
extern Sint32 KibomboEyesPopScript[];
extern Sint16 KibomboFrozenPuddle[];
/* Unused: nothing reads these bytes. */
Uint8 KibomboUnusedBeforeFireColors[4] BSS_DATA;
/* The signal fire's four colours as the village has them, which its flicker
   brightens and dims. */
Sint16 KibomboFireColors[4] BSS_DATA;

ACTOR *KibomboBlackCrystal BSS_DATA;

/* A background colour's place in BG_PLTT: colour `color` of 16-colour palette
   `palette`, as the fires' flicker indexes the colours it sets. */
#define BG_PLTT_INDEX(palette, color) ((palette) * PALETTE_COLORS + (color))

/* The fires' colours in the background palettes, which each room keeps in
   KibomboFireColors as it opens and its flicker event brightens and dims: four
   in the night village, four on the night grounds, the first the same, and
   three before the statue's face. */
typedef enum KIBOMBO_FIRE_COL {
	FIRE_COLOR_FIRST = BG_PLTT_INDEX(0, 9),
	VILLAGE_FIRE_COLOR_SECOND = BG_PLTT_INDEX(5, 7),
	VILLAGE_FIRE_COLOR_THIRD = BG_PLTT_INDEX(6, 7),
	VILLAGE_FIRE_COLOR_FOURTH = BG_PLTT_INDEX(7, 7),
	GROUNDS_FIRE_COLOR_SECOND = BG_PLTT_INDEX(0, 7),
	GROUNDS_FIRE_COLOR_THIRD = BG_PLTT_INDEX(0, 6),
	GROUNDS_FIRE_COLOR_FOURTH = BG_PLTT_INDEX(4, 12),
	STATUE_FIRE_COLOR_FIRST = BG_PLTT_INDEX(10, 9),
	STATUE_FIRE_COLOR_SECOND = BG_PLTT_INDEX(10, 7),
	STATUE_FIRE_COLOR_THIRD = BG_PLTT_INDEX(10, 6)
} KIBOMBO_FIRE_COL;

/* Set once the party has found the dead end by the statue and turned back
   (scKibomboDeadEndByTheStatue). */
#define FLAG_KIBOMBO_DEAD_END_SEEN		 0x8f5
/* Set while Piers goes about Kibombo with the party: as he joins it, and as he
   thanks it; cleared when he stays behind. */
#define FLAG_KIBOMBO_PIERS_WITH_PARTY	 0x92e
/* Set once Piers has refused to leave the village without the orb and stayed
   behind (scKibomboPiersRefusesToLeave). */
#define FLAG_KIBOMBO_PIERS_STAYED_BEHIND 0x92f

/* Story flags: the vent Scoop uncovers by the statue, the pillar Moved for
   Piers, the ceremony begun and Piers joined. */
#define FLAG_KIBOMBO_VENT_UNCOVERED			0x86a
#define FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS 0x9ea
#define FLAG_KIBOMBO_CEREMONY_BEGUN			0x8f1
#define FLAG_PIERS_JOINED					0x8f4
/* Set once the party has heard of the foreign ceremony. */
#define FLAG_HEARD_OF_THE_FOREIGN_CEREMONY	0x9df
/* Set once Lash has tied the village's rope across. */
#define FLAG_KIBOMBO_VILLAGE_ROPE_TIED		0x9e9
/* Set once the party has been to the statue's grounds on the ceremony night. */
#define FLAG_SAW_THE_GROUNDS_CEREMONY		0x869
/* Set as the party climbs down the ladder from Akafubu's home: he stands apart,
   turns to whoever talks to him and speaks, as his father does, of the days
   after the ceremony. */
#define FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU	0x8ee
/* For this visit: each of the four marks at the dead end by the statue
   uncovered, and the one Scoop is uncovering. */
#define FLAG_NORTH_MARK_UNCOVERED			FLAG_SCENE_TEMP_04
#define FLAG_SOUTH_MARK_UNCOVERED			FLAG_SCENE_TEMP_05
#define FLAG_WEST_MARK_UNCOVERED			FLAG_SCENE_TEMP_06
#define FLAG_EAST_MARK_UNCOVERED			FLAG_SCENE_TEMP_07
#define FLAG_SCOOPING_NORTH_MARK			FLAG_SCENE_TEMP_20
#define FLAG_SCOOPING_SOUTH_MARK			FLAG_SCENE_TEMP_21
#define FLAG_SCOOPING_WEST_MARK				FLAG_SCENE_TEMP_22
#define FLAG_SCOOPING_EAST_MARK				FLAG_SCENE_TEMP_23
/* For this visit: the room's unseen blocks drawn (scKibomboRestoreBlocks). */
#define FLAG_KIBOMBO_BLOCKS_SHOWN			FLAG_SCENE_TEMP_00
/* For this visit: the puddle frozen into a pillar (KibomboFrozenPuddle). On
   the grounds by day it is set until the ceremony night has been seen, so Frost
   leaves the puddle alone. */
#define FLAG_KIBOMBO_PUDDLE_FROZEN			FLAG_SCENE_TEMP_01
/* For this visit: the room's fires flicker, and a way out stops them. */
#define FLAG_KIBOMBO_FIRES_FLICKER			FLAG_SCENE_TEMP_02
/* For this visit: the village's doors are shut on the ceremony night. */
#define FLAG_KIBOMBO_DOORS_SHUT				FLAG_SCENE_TEMP_09

/* The puddle Frost freezes, the first row of the village's and the grounds'
   placements. */
enum { ACTOR_PUDDLE = 8 };

/* The grounds before the statue (SceneIdKibomboStatueGrounds and, on the
   ceremony night, SceneIdKibomboStatueGroundsCeremony), numbering the rows of
   their placements; the night's rows place the warriors and Akafubu's party
   after them, and Kraden after those. */
typedef enum KIBOMBO_STATUE_GROUNDS_ACTOR_ID {
	/* The stone pillar Move pushes for Piers (scKibomboMovePillarForPiers). */
	ACTOR_GROUNDS_PILLAR = 11,
	/* The Great Gabomba's two eyes, which light up as its mouth opens
	   (scKibomboSetStatueMouth), and its tongue (scKibomboSetStatueTongue). */
	ACTOR_STATUE_LEFT_EYE = 12,
	ACTOR_STATUE_RIGHT_EYE = 13,
	ACTOR_STATUE_TONGUE = 14,
	/* Two unseen blocks (SPRITE_UNSEEN_BLOCK, black squares drawn transparent),
	   hidden as the room opens (scKibomboHideGroundsBlocks). */
	ACTOR_GROUNDS_FIRST_BLOCK = 15,
	ACTOR_GROUNDS_SECOND_BLOCK = 16,
	/* On the ceremony night: Akafubu, "Let us now present our jewel to the
	   Great Gabomba!", and the eight warriors in a ring before him. */
	ACTOR_AKAFUBU = 17,
	ACTOR_WARRIOR_A = 18,
	ACTOR_WARRIOR_B = 19,
	ACTOR_WARRIOR_C = 20,
	ACTOR_WARRIOR_D = 21,
	ACTOR_WARRIOR_E = 22,
	ACTOR_WARRIOR_F = 23,
	ACTOR_WARRIOR_G = 24,
	ACTOR_WARRIOR_H = 25,
	ACTOR_KRADEN = 26,
	/* An unseen marker the Black Crystal rides on as Akafubu raises it
	   (scKibomboCarryBlackCrystal); Piers's, Jenna's and Sheba's rows take
	   their own numbers, so it follows Kraden. */
	ACTOR_CRYSTAL_MARKER = 27
} KIBOMBO_STATUE_GROUNDS_ACTOR_ID;

/* The poses scKibomboSetStatueTongue gives the Great Gabomba's tongue, each in
   the sprite sheet's daylight colours or those it wears by the ceremony night's
   firelight. Out, it hangs from the mouth to the drums; to the ground, 34
   pixels lower, as the statue leaves it once it has answered. */
typedef enum STATUE_TONGUE_POSE {
	STATUE_TONGUE_IN_MOUTH, /* no tongue but the map's, inside the mouth */
	/* Unused: no call unrolls the tongue through this function. */
	STATUE_TONGUE_UNROLLING_BY_DAY,
	STATUE_TONGUE_OUT_BY_DAY,
	/* Unused: no call unrolls the tongue through this function. */
	STATUE_TONGUE_UNROLLING_BY_FIRELIGHT,
	STATUE_TONGUE_OUT_BY_FIRELIGHT,
	STATUE_TONGUE_TO_THE_GROUND_BY_DAY,
	STATUE_TONGUE_TO_THE_GROUND_BY_FIRELIGHT
} STATUE_TONGUE_POSE;

/* The village by day and on the ceremony night: after the puddle, the two
   candlesticks by a house's door, the flame (by day the cooking fire's, at
   night burning on the totem pole in the square) and the rope's two posts. By
   day a woman remarks on the warriors, two warriors keep the gate and the
   signal fire burns; at night villagers crowd the square, an unseen block
   stands at a house's door and six candlesticks burn. */
enum {
	ACTOR_CANDLESTICK_A = 9,
	ACTOR_CANDLESTICK_B,
	ACTOR_FLAME,
	ACTOR_ROPE_POST_A,
	ACTOR_ROPE_POST_B,
	/* By day, the villagers, each named for what they say: a small child,
	   "Akafubu has finally completed his ceremony...", and the woman, "It looks
	   like there are fewer warriors in Kibombo now, huh?" */
	ACTOR_CHILD_GLAD_OF_THE_CALM,
	ACTOR_WOMAN_NOTING_FEWER_WARRIORS,
	/* "The Great Gabomba's mouth will remain open until Akafubu has earned the
	   gift of the Gabomba." */
	ACTOR_MAN_ON_THE_OPEN_MOUTH,
	/* "Now that the ceremony is over, I feel a touch depressed." */
	ACTOR_WOMAN_MISSING_THE_CEREMONY,
	/* "I couldn't for the life of me figure out how our warriors got to Indra
	   without a boat." */
	ACTOR_OLD_MAN_PUZZLED_BY_INDRA,
	/* "Oh... I smell something delectable." */
	ACTOR_OLD_WOMAN_SMELLING_MEAT,
	/* "Good ol' meat... I love meat." */
	ACTOR_BOY_WHO_LOVES_MEAT,
	/* "We get to eat normal animals again!" */
	ACTOR_GIRL_TIRED_OF_MONSTERS,
	ACTOR_GATE_GUARD_A = 22,
	ACTOR_GATE_GUARD_B,
	ACTOR_SIGNAL_FIRE = 25,
	ACTOR_DOOR_BLOCK = 29,
	ACTOR_LIT_CANDLESTICK_A,
	ACTOR_LIT_CANDLESTICK_B,
	ACTOR_LIT_CANDLESTICK_C,
	ACTOR_LIT_CANDLESTICK_D,
	ACTOR_LIT_CANDLESTICK_E,
	ACTOR_LIT_CANDLESTICK_F
};

/* The houses, inn and shops (SceneIdKibomboHouses). */
enum {
	/* Akafubu at home after the ceremony: "I'm going to be the one to get the
	   Great Gabomba's magic! You'll see!" */
	ACTOR_AKAFUBU_AT_HOME = 8,
	/* "My son has been so kind and compassionate since the ceremony." */
	ACTOR_FATHER_OF_AKAFUBU = 9,
	ACTOR_KRADEN_AT_HOME = 10,
	/* The other villagers at home, each named for what they say: "I have heard
	   that if you head west from southern Gondowan, you'll see the Eastern
	   Sea." */
	ACTOR_CHILD_ON_THE_EASTERN_SEA = 11,
	/* "Are you going to head to western Gondowan?" */
	ACTOR_WOMAN_ASKING_ABOUT_THE_WEST,
	/* "You can see Atteka from the bridge in western Gondowan." */
	ACTOR_BOY_WHO_HEARD_OF_ATTEKA,
	/* "If you are looking for treasure, look west of the river." */
	ACTOR_MAN_ON_TREASURE_WEST,
	/* "Each year, when the rains stop, our warriors look for treasure in the
	   belly of the continent." */
	ACTOR_WOMAN_ON_THE_TREASURE_HUNTS,
	/* The old man lying in bed, his back aching since the ceremony. */
	ACTOR_MAN_WITH_ACHING_BACK,
	/* "As soon as Akafubu became our recognized witch doctor, he became so much
	   nicer." */
	ACTOR_OLD_WOMAN_ON_A_NICER_AKAFUBU,
	/* Two warriors at home: "Many in Gondowan fear the might of the Kibombo
	   warriors." and "We think it is time to inform other villages..." */
	ACTOR_WARRIOR_NOT_TO_BE_FEARED,
	ACTOR_WARRIOR_TO_TELL_THE_VILLAGES,
	ACTOR_WEAPON_SHOPKEEPER,
	ACTOR_ARMOR_SHOPKEEPER,
	ACTOR_ITEM_SHOPKEEPER,
	ACTOR_INNKEEPER,
	/* "I can't remember how long it's been since we've had any guests." */
	ACTOR_INN_WORKER,
	ACTOR_HEALER,
	/* Two unseen blocks in the inn's back doorway, hidden as the room opens
	   (scKibomboHideInnBlocks). */
	ACTOR_INN_FIRST_BLOCK,
	ACTOR_INN_SECOND_BLOCK,
	/* An unseen marker on the bed where the man with the aching back lies,
	   which answers for him from that side. */
	ACTOR_ACHING_BACK_BEDSIDE
};

/* The village by day: "This is a signal fire. We use these to send messages to
   neighboring..." */
enum { ACTOR_SIGNAL_FIRE_KEEPER = 24 };

/* The village on the ceremony night: the crowd in the square, each named for
   what they say before Akafubu has asked the party's help; some move or leave
   once he has. Last, the healer who talks of the foreign ceremony. */
enum {
	/* "During the last ceremony, Akafubu made the jewel float in the air..." */
	ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT = 14,
	/* "Akafubu looks really confident tonight." */
	ACTOR_MAN_SURE_OF_AKAFUBU,
	/* "Akafubu is attempting the ceremony again tonight. Oeia performed the
	   same..." */
	ACTOR_OLD_MAN_RECALLING_OEIA,
	/* "Is making the jewel float with the power of his mind all there is to
	   it?" */
	ACTOR_WOMAN_DOUBTING_THE_JEWEL,
	/* "Akafubu has tried this before, but he has always failed." */
	ACTOR_WOMAN_RECALLING_HIS_FAILURES,
	/* "Akafubu blames his failures on the jewel he originally used." */
	ACTOR_OLD_WOMAN_ON_THE_JEWEL,
	/* "Akafubu's ceremony is set to begin soon." */
	ACTOR_BOY_KEPT_AWAKE,
	ACTOR_GIRL_WHO_HOPES,
	/* "When the Gabomba is angered, his eyes glow with rage and fire shoots out
	   of his mouth." */
	ACTOR_WOMAN_WARNING_OF_ANGER,
	/* "He who is the witch doctor of Kibombo holds the most revered office in
	   our whole town." */
	ACTOR_OLD_MAN_ON_THE_OFFICE,
	/* "If the Great Gabomba's mouth doesn't open, Akafubu must not yet be a
	   true witch doctor." */
	ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
	/* "If the Great Gabomba likes the jewel, his mouth will open and his tongue
	   will stick out." */
	ACTOR_MAN_WHO_KNOWS_THE_SIGN,
	ACTOR_WOMAN_NO_PUSHING = 26,
	ACTOR_AFRAID_MAN,
	ACTOR_MAN_WHO_KNEW_OEIA,
	ACTOR_HEALER_ON_THE_CEREMONY = 36
};

/* The village on the ceremony night: where the party starts again, before the
   healer, after a defeat (SceneReturnRules). */
enum { ENTRANCE_CEREMONY_AFTER_DEFEAT = 19 };

/* The rooms' own metatile behaviours; where a metatile leads out of a room its
   behaviour is the exit's number, named in scene_exits.h. */
typedef enum KIBOMBO_METATILE_BEHAVIOR {
	/* The village: the ring of metatiles (19, 7) to (21, 10) among the painted
	   poles in its north, and the two metatiles inside it, (20, 8) and (20,
	   9). */
	METATILE_BEHAVIOR_MIDDLE_OF_POLE_RING = 15,
	METATILE_BEHAVIOR_POLE_RING,
	/* The village: the jump north from metatile (19, 8) among the poles, and by
	   day the jump west from the ledge by the puddle, metatile (25, 27). */
	METATILE_BEHAVIOR_JUMP_NORTH_AMONG_POLES = 20,
	METATILE_BEHAVIOR_JUMP_WEST_BY_PUDDLE,
	/* The statue's grounds: the patch over the vent, metatile (23, 18), and the
	   four marks at the dead end, north (16, 7), south (16, 11), west (14, 9)
	   and east (18, 9). */
	METATILE_BEHAVIOR_VENT_PATCH = 10,
	METATILE_BEHAVIOR_NORTH_MARK,
	METATILE_BEHAVIOR_SOUTH_MARK,
	METATILE_BEHAVIOR_WEST_MARK,
	METATILE_BEHAVIOR_EAST_MARK,
	/* The grounds on the ceremony night: the ledge where Piers stands,
	   metatiles (3, 25) to (3, 26); the way up to the dead end, (21, 14) and
	   (22, 14); and the top of the grounds beside Piers once he has stayed
	   behind, (16, 5). */
	METATILE_BEHAVIOR_BY_PIERS = 20,
	METATILE_BEHAVIOR_WAY_TO_DEAD_END,
	METATILE_BEHAVIOR_BY_PIERS_WAITING,
	/* The houses: the metatiles beside the three ladders down, each the number
	   of the exit it leads by: in the item shop's upper room, from Akafubu's
	   home to the Sanctum, and in the upper room with two beds. */
	METATILE_BEHAVIOR_ITEM_SHOP_LADDER = 7,
	METATILE_BEHAVIOR_AKAFUBUS_LADDER = 9,
	METATILE_BEHAVIOR_TWO_BEDS_LADDER = 13,
	/* The Sanctum: the counter before the healer, metatile (23, 22). */
	METATILE_BEHAVIOR_SANCTUM_COUNTER = 24
} KIBOMBO_METATILE_BEHAVIOR;

struct ACTOR;

/* An actor's update: turns its sprite a little each tick. */
Sint32 scKibomboSpinSprite(ACTOR *actor)
{
	((FIELD_SPR *)actor->sprite)->rotation += TRIG_TURN * 3 / 128;
	return 0;
}

/* An actor's update: keeps its position as the center scKibomboShakeInPlace
   shakes it about. */
Sint32 scKibomboKeepShakeCenter(ACTOR *actor)
{
	actor->bounce = actor->x.fixed;
	actor->gravity = actor->y.fixed;
	return 0;
}

extern SCENE_REGION KibomboHousesExitRegions[];

/* An actor's update: every other tick, puts it up to a pixel either way of its
   kept center. */
Sint32 scKibomboShakeInPlace(ACTOR *actor)
{
	if(TickCount & 1) {
		FIXED dx;
		FIXED dy;

		dx = -toFIXED(1);
		dy = dx;
		dx += toFIXED(syRandomBelow(3));
		dy += toFIXED(syRandomBelow(3));

		fdActorSetPosAndResetMotion(
			actor, actor->bounce + dx, actor->gravity + dy, actor->z.fixed);
	}
	return 1;
}

/* An actor's update: flashes its sprite's colour on and off every two
   ticks. */
void scKibomboFlashSprite(ACTOR *actor)
{
	if(TickCount & 2)
		fdActorSetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
	else
		fdActorSetSpriteColor(actor, SPRITE_COLOR_NORMAL);
}

/* An actor's update: every ten to twenty ticks, faces a random way. */
Sint32 scKibomboFaceRandomWay(ACTOR *actor)
{
	if(actor->sceneCount != 0) {
		actor->sceneCount--;
	} else {
		actor->sceneCount = (syRandomBelow(10)) + 10;
		actor->facing = syRandom();
	}
	return 1;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

/* The houses have exit regions of their own; the other rooms have none. */
const SCENE_REGION *scGetExitRegions(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KIBOMBO_HOUSES)
		return KibomboHousesExitRegions;
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE KibomboVillageActorPlacements[];
extern ACTOR_PLACE KibomboCeremonyActorPlacements[];
extern ACTOR_PLACE KibomboStatueGroundsActorPlacements[];
extern ACTOR_PLACE KibomboStatueGroundsCeremonyActorPlacements[];
extern ACTOR_PLACE KibomboHousesActorPlacements[];
extern ACTOR_PLACE KibomboActorPlacements[];
extern CUE		   KibomboVillageCues[];
extern CUE		   KibomboCeremonyCues[];
extern CUE		   KibomboStatueGroundsCues[];
extern CUE		   KibomboStatueGroundsCeremonyCues[];
extern CUE		   KibomboHousesCues[];
extern CUE		   KibomboCues[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KIBOMBO)
		return KibomboVillageActorPlacements;
	if(scene == SCENE_KIBOMBO_CEREMONY)
		return KibomboCeremonyActorPlacements;
	if(scene == SCENE_KIBOMBO_STATUE_GROUNDS)
		return KibomboStatueGroundsActorPlacements;
	if(scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
		return KibomboStatueGroundsCeremonyActorPlacements;
	if(scene == SCENE_KIBOMBO_HOUSES) {
		if(syGameFlagGet(FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU))
			KibomboHousesActorPlacements[ACTOR_AKAFUBU_AT_HOME -
				ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
		return KibomboHousesActorPlacements;
	}
	return KibomboActorPlacements;
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KIBOMBO)
		return KibomboVillageCues;
	if(scene == SCENE_KIBOMBO_CEREMONY)
		return KibomboCeremonyCues;
	if(scene == SCENE_KIBOMBO_STATUE_GROUNDS)
		return KibomboStatueGroundsCues;
	if(scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
		return KibomboStatueGroundsCeremonyCues;
	if(scene == SCENE_KIBOMBO_HOUSES)
		return KibomboHousesCues;
	return KibomboCues;
}

/* A woman asks whether the party has noticed there are fewer warriors. */
void scKibomboAskAboutFewerWarriors(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgItLooksLikeThere);
	/* "It looks like there are fewer warriors in Kibombo now, huh?" */
	fdScriptAskYesNoAndShowResponse(ACTOR_WOMAN_NOTING_FEWER_WARRIORS, 0);
	fdScriptEnd();
}

/* Talking to the west gate guard: the guards ask whether the party has come for
   revenge for the raid, the east guard speaking. */
void scKibomboAskAboutRevenge(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgYouDidnT);
	/* "You didn't come to Kibombo to get revenge on us for that raid, did
	   you?" */
	fdScriptAskYesNoAndShowResponse(ACTOR_GATE_GUARD_B, 0);
	fdScriptEnd();
}

/* The signal fire's keeper tells of the path to peace under Akafubu, and of the
   signal fire. */
void scKibomboTalkPathToPeace(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgNowThatAkafubu);
	/* "Now that Akafubu is our new witch doctor, we shall embark on a
	   path..." */
	fdScriptShowMessageAndWait(ACTOR_SIGNAL_FIRE_KEEPER, 0);
	fdActorFaceActorById(
		ACTOR_SIGNAL_FIRE_KEEPER, MainRoutineStateWork.leaderActorId, 0);
	fdScriptWait(20);
	/* "This is a signal fire. We use these to send messages to
	   neighboring..." */
	fdScriptShowMessageAndWait(ACTOR_SIGNAL_FIRE_KEEPER, 0);
	fdActorFaceDirectionAndWait(
		ACTOR_SIGNAL_FIRE_KEEPER, FACING_SOUTH_SOUTHEAST);
	fdScriptEnd();
}

/* A woman asks whether the party is heading to western Gondowan. */
void scKibomboAskGoingWest(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgAreYouGoing);
	/* "Are you going to head to western Gondowan?" */
	fdScriptAskYesNoAndShowResponse(ACTOR_WOMAN_ASKING_ABOUT_THE_WEST, 0);
	fdScriptEnd();
}

#include "shared/leader_is_facing_north.h"

/* Kibombo's shops, as mnShopRun numbers them. */
enum {
	SHOP_KIBOMBO_WEAPONS = 17,
	SHOP_KIBOMBO_ARMOR = 18,
	SHOP_KIBOMBO_ITEMS = 19
};

/* Kibombo's inn, as mnInnCheckIn numbers the inns. */
enum { KIBOMBO_INN = 6 };

/* Talking to the weapon shopkeeper: across the counter, the shop; from
   elsewhere, a word about Kibombo's weapons. */
void scKibomboOpenWeaponShop(void)
{
	if(scLeaderIsFacingNorth()) {
		mnShopRun(SHOP_KIBOMBO_WEAPONS, ACTOR_WEAPON_SHOPKEEPER);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgTheWeaponsI);
		/* "The weapons I sell in this shop are ones we do not use in
		   Kibombo...." */
		fdScriptShowMessageAndWait(ACTOR_WEAPON_SHOPKEEPER, 0);
		fdScriptEnd();
	}
}

/* Talking to the armor shopkeeper: across the counter, the shop; from
   elsewhere, a word about shields and war paint. */
void scKibomboOpenArmorShop(void)
{
	if(scLeaderIsFacingNorth()) {
		mnShopRun(SHOP_KIBOMBO_ARMOR, ACTOR_ARMOR_SHOPKEEPER);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgTheOnlyDefenses);
		/* "The only defenses we Kibombo need are our shields and our war
		   paint...." */
		fdScriptShowMessageAndWait(ACTOR_ARMOR_SHOPKEEPER, 0);
		fdScriptEnd();
	}
}

/* Talking to the item shopkeeper: across the counter, the shop; from elsewhere,
   a word about the spearhead poison. */
void scKibomboOpenItemShop(void)
{
	if(scLeaderIsFacingNorth()) {
		mnShopRun(SHOP_KIBOMBO_ITEMS, ACTOR_ITEM_SHOPKEEPER);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgIMSorryBut);
		/* "I'm sorry, but the Kibombo spearhead poison is a tribal secret,
		   used..." */
		fdScriptShowMessageAndWait(ACTOR_ITEM_SHOPKEEPER, 0);
		fdScriptEnd();
	}
}

/* Talking to the innkeeper: across the counter, the inn; from elsewhere, a word
   about the beds. */
void scKibomboOpenInn(void)
{
	if(scLeaderIsFacingNorth()) {
		mnInnCheckIn(KIBOMBO_INN, ACTOR_INNKEEPER);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgDonTWorryAbout);
		/* "Don't worry about the beds here... We used to have insect
		   problems..." */
		fdScriptShowMessageAndWait(ACTOR_INNKEEPER, 0);
		fdScriptEnd();
	}
}

/* Talking to the healer, or checking the counter before her, metatile (23, 22):
   across the counter, the Sanctum; from elsewhere, a word about the peace the
   ceremony brought. */
void scKibomboOpenSanctum(void)
{
	if(scLeaderIsFacingNorth()) {
		mnSanctumRun(ACTOR_HEALER);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgKibomboSPeace);
		/* "Kibombo's peace is restored now that the ceremony has reached
		   its..." */
		fdScriptShowMessageAndWait(ACTOR_HEALER, 0);
		fdScriptEnd();
	}
}

/* The healer on the ceremony night: the first time, she muses on foreign
   ceremonies; then she turns to the party and runs her Sanctum. */
void scKibomboTalkForeignCeremony(void)
{
	if(fdScriptGetFlag(FLAG_HEARD_OF_THE_FOREIGN_CEREMONY)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorFaceActorById(ACTOR_HEALER_ON_THE_CEREMONY,
			MainRoutineStateWork.leaderActorId, 0);
		fdScriptWait(10);
		mnSanctumRun(ACTOR_HEALER_ON_THE_CEREMONY);
		fdActorFaceDirection(
			ACTOR_HEALER_ON_THE_CEREMONY, FACING_NORTH_NORTHWEST, 0);
		fdScriptEnd();
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgOhOhMy);
		/* "Oh... Oh my... I've heard of ceremonies like these... Foreign..." */
		fdScriptShowMessageThenWait(ACTOR_HEALER_ON_THE_CEREMONY, 0, 40);
		/* "Hmm?" */
		fdScriptShowMessageAndWait(ACTOR_HEALER_ON_THE_CEREMONY, 0);
		fdActorFaceActorById(ACTOR_HEALER_ON_THE_CEREMONY,
			MainRoutineStateWork.leaderActorId, 0);
		fdScriptWait(10);
		mnSanctumRun(ACTOR_HEALER_ON_THE_CEREMONY);
		fdActorFaceDirection(
			ACTOR_HEALER_ON_THE_CEREMONY, FACING_NORTH_NORTHWEST, 0);
		syGameFlagSet(FLAG_HEARD_OF_THE_FOREIGN_CEREMONY);
		fdScriptEnd();
	}
}

/* The man whose back has ached since the ceremony. */
void scKibomboTalkAchingBack(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgOhMyBack);
	/* "Oh, my back... It's been aching ever since that ceremony. I guess
	   I..." */
	fdScriptShowMessageAndWait(ACTOR_MAN_WITH_ACHING_BACK, 0);
	fdScriptEnd();
}

/* Mind Read on the man with the aching back. */
void scKibomboReadMindNeverAgain(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgISwearThat);
	/* "I swear, that's the last time I'll ever let loose like that again!" */
	fdScriptShowMessageAndWait(ACTOR_MAN_WITH_ACHING_BACK, 0);
	fdScriptEnd();
}

/* Draws (show 1) or stops drawing the room's two unseen blocks: on the statue's
   grounds by its paths, in the houses in the inn's back doorway. */
void scKibomboSetBlocksShown(Sint32 show)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KIBOMBO_STATUE_GROUNDS ||
		scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
		scActorsSetDrawn(show, ACTOR_GROUNDS_FIRST_BLOCK, 2);
	else if(scene == SCENE_KIBOMBO_HOUSES)
		scActorsSetDrawn(show, ACTOR_INN_FIRST_BLOCK, 2);
}

/* Unused: draws the room's two unseen blocks and notes them shown. */
UNUSED void scKibomboShowBlocks(void)
{
	scKibomboSetBlocksShown(1);
	syGameFlagSet(FLAG_KIBOMBO_BLOCKS_SHOWN);
}

/* Stops drawing the room's two unseen blocks. */
void scKibomboHideBlocks(void)
{
	scKibomboSetBlocksShown(0);
	syGameFlagClear(FLAG_KIBOMBO_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

/* Felix casts Frost on the puddle: it freezes into a pillar of ice to jump
   onto. */
void scKibomboFreezePuddle(void)
{
	fdFlagBlockApplyFlags(KibomboFrozenPuddle);
}

/* Shows the vent by the statue uncovered, metatile (23, 18), and lets the party
   climb down into it. */
void scKibomboUncoverVent(void)
{
	fdMapGridCopyMetatileTilesIndicesRect(34, 33, 23, 18, 1, 1);
	fdMapGridCopyMetatileAttributes(34, 33, 1, 1, 23, 18);
	syGameFlagSet(FLAG_KIBOMBO_VENT_UNCOVERED);
}

/* Felix casts Scoop on the patch of ground on the statue's grounds, metatile
   (23, 18), which the party passed on the way to the dead end: as the dig opens
   the ground, the vent under it is uncovered, the way into the Gabomba
   Statue. */
void scKibomboScoopVent(Sint32 stage)
{
	if(stage == SCOOP_STAGE_DUG)
		scKibomboUncoverVent();
}

/* Uncovers the north one of the four marks at the dead end by the statue,
   metatile (16, 7), as Scoop does. */
void scKibomboUncoverNorthMark(void)
{
	fdMapGridCopyMetatileTilesIndicesRect(36, 34, 16, 7, 1, 1);
	fdMapGridCopyMetatileAttributes(36, 34, 1, 1, 16, 7);
	syGameFlagSet(FLAG_NORTH_MARK_UNCOVERED);
}

/* Felix casts Scoop on the north mark at the dead end by the statue, metatile
   (16, 7), one of four in a diamond on the ground there: as the dig opens the
   ground, the mark is dug out. */
void scKibomboScoopNorthMark(Sint32 stage)
{
	if(stage == SCOOP_STAGE_DUG)
		scKibomboUncoverNorthMark();
}

/* Uncovers the south mark, metatile (16, 11). */
void scKibomboUncoverSouthMark(void)
{
	fdMapGridCopyMetatileTilesIndicesRect(38, 34, 16, 11, 1, 1);
	fdMapGridCopyMetatileAttributes(38, 34, 1, 1, 16, 11);
	syGameFlagSet(FLAG_SOUTH_MARK_UNCOVERED);
}

/* Felix casts Scoop on the south mark at the dead end by the statue, metatile
   (16, 11), the one Kraden first tells him to dig: as the dig opens the ground,
   the mark is dug out. */
void scKibomboScoopSouthMark(Sint32 stage)
{
	if(stage == SCOOP_STAGE_DUG)
		scKibomboUncoverSouthMark();
}

/* Uncovers the west mark, metatile (14, 9). */
void scKibomboUncoverWestMark(void)
{
	fdMapGridCopyMetatileTilesIndicesRect(36, 36, 14, 9, 1, 1);
	fdMapGridCopyMetatileAttributes(36, 36, 1, 1, 14, 9);
	syGameFlagSet(FLAG_WEST_MARK_UNCOVERED);
}

/* Felix casts Scoop on the west mark at the dead end by the statue, metatile
   (14, 9): as the dig opens the ground, the mark is dug out. */
void scKibomboScoopWestMark(Sint32 stage)
{
	if(stage == SCOOP_STAGE_DUG)
		scKibomboUncoverWestMark();
}

/* Uncovers the east mark, metatile (18, 9). */
void scKibomboUncoverEastMark(void)
{
	fdMapGridCopyMetatileTilesIndicesRect(38, 36, 18, 9, 1, 1);
	fdMapGridCopyMetatileAttributes(38, 36, 1, 1, 18, 9);
	syGameFlagSet(FLAG_EAST_MARK_UNCOVERED);
}

/* Felix casts Scoop on the east mark at the dead end by the statue, metatile
   (18, 9): as the dig opens the ground, the mark is dug out. */
void scKibomboScoopEastMark(Sint32 stage)
{
	if(stage == SCOOP_STAGE_DUG)
		scKibomboUncoverEastMark();
}

/* Felix uses Move on the stone pillar by Piers in the west of the grounds and
   pushes it west off its ledge to metatile (6, 18), where it drops with a thump
   into the place Piers could not reach; Piers then joins the party. */
void scKibomboMovePillarForPiers(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_GROUNDS_PILLAR);

	if(FIELD_METATILE(actor->x.fixed) == 6 &&
		FIELD_METATILE(actor->z.fixed) == 18) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorGetById(ACTOR_GROUNDS_PILLAR)->motionFlags =
			ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		do
			sySynch(1);
		while(actor->velocityY != 0);
		sySoundPlay(SE_THUMP);
		sySynch(10);
		actor->motionFlags = 0;
		fdActorGetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 0;
		fdMapGridCopyMetatileAttributes(5, 18, 1, 1, 6, 18);
		syGameFlagSet(FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS);
		if(!syGameFlagGet(FLAG_PIERS_JOINED))
			scKibomboPiersJoinsParty();
		fdScriptEnd();
	}
}

/* Felix walks up onto the ledge in the west of the statue's grounds where Piers
   stands, metatiles (3, 25) to (3, 26): Piers turns, startled, "Wh-Who are
   you?!?", and asks what the party wants. */
void scKibomboMeetPiers(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(88, CAMERA_KEEP, 392, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 72, 392);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 88, 392);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_SOUTHWEST, 10);
	fdScriptSetMessageId((Sint32)MsgWhWhoAre);
	/* "Wh-Who are you?!?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetSpritePriority(ACTOR_KRADEN, 2);
	fdActorSetSpritePriority(ACTOR_JENNA, 2);
	fdActorSetSpritePriority(ACTOR_SHEBA, 2);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetMovementTypeById(ACTOR_KRADEN, KibomboKradenMeetsPiersScript);
	fdActorSetMovementTypeById(ACTOR_JENNA, KibomboJennaMeetsPiersScript);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_SHEBA, KibomboShebaMeetsPiersScript);
	/* "What... What are your intentions?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	if(syGameFlagGet(FLAG_PIERS_INTERROGATED))
		scKibomboPiersHasForgottenParty();
	else
		scKibomboPiersTakesPartyForEnemies();
	scKibomboAkafubuOffersCrystal();
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorSetMovementTypeById(ACTOR_JENNA, KibomboPartyGathersScript);
	fdActorSetMovementTypeById(ACTOR_SHEBA, KibomboPartyGathersScript);
	fdActorSetMovementTypeAndWaitById(ACTOR_KRADEN, KibomboPartyGathersScript);
	syGameFlagSet(FLAG_KIBOMBO_PIERS_MET);
	fdScriptEnd();
}

/* The party's answer that Piers does not know them: Jenna wonders whether he
   has forgotten them, until he remembers the travellers who came to his
   metatile, and asks when he ever asked for their help. */
void scKibomboPiersHasForgottenParty(void)
{
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_WEST, 20);
	fdScriptSetMessageId((Sint32)MsgItLooksLikeHas);
	/* "It looks like Piers has already forgotten us, huh?" Yes: "I know! We met
	   Piers when he was in JAIL!" No: "What, is he so obsessed with his black
	   orb..." */
	fdMessageShowAtActor(ACTOR_SHEBA, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
		/* "I know! We met Piers when he was in JAIL! And it's not like he
		   had..." */
		fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
		FieldStateWork->msg++;
	} else {
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		FieldStateWork->msg++;
		/* "What, is he so obsessed with his black orb that he can't even..." */
		fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	}
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Wait a second... I know you..." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
	/* "You're the travelers who came seeking me when I was in jail." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "So, you do remember!" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	/* "What are you doing in this place?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	/* "Actually, that's what we wanted to ask you..." */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 40);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	/* "We were worried about you, Piers, so we followed you here..." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	/* "And that's what brings you to this dangerous place?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorJump(ACTOR_SHEBA, 2, 10);
	fdActorJump(ACTOR_SHEBA, 4, 10);
	/* "What's that supposed to mean?" */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
	/* "We came here to help you, Piers. Why are you being like that about
	   it?" */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
	/* "Tell me something... Exactly when did I ask you for your help?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
}

/* The party's answer that it is no enemy: Piers doubts it, the party tells him
   of the tidal wave and the boat it seeks, and he finds it hard to believe. */
void scKibomboPiersTakesPartyForEnemies(void)
{
	fdScriptSetMessageId((Sint32)MsgHeSeemsTo);
	/* "He seems to believe we are enemies..." */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "I hope he doesn't think we're with the Kibombo." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "I must admit, you don't look much like Kibombo..." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdEffectSetAttached(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "But that begs the question, if you're not Kibombo, then who are
	   you..." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	/* "They are...warriors. But how did you get in here?" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	/* "Why are these warriors in a place like this?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	/* "I'm not sure how to answer that..." */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 40);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	/* "Actually, we've been looking for a boat... We kinda need one." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	/* "You're looking for a boat... in the middle of the continent? That..." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorJump(ACTOR_SHEBA, 2, 10);
	fdActorJump(ACTOR_SHEBA, 4, 10);
	/* "Hey, this wasn't exactly our first choice, you know?" */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
	/* "A tidal wave destroyed all the boats in the coastal towns." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
	/* "You're asking me to believe rather a lot, don't you think?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
}

/* Back on the grounds after Piers stayed behind, Felix walks up beside him
   where he waits at the top of the grounds, metatile (16, 5): Piers thanks the
   party for coming to help get his orb back, walks over to Felix and joins it
   again. */
void scKibomboPiersThanksParty(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(280, CAMERA_KEEP, 88, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdScriptSetMessageId((Sint32)MsgYouVeCome);
	/* "You've come to help me get my orb back? Thank you, all of you!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_PIERS, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	syGameFlagClear(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
	syGameFlagSet(FLAG_KIBOMBO_PIERS_WITH_PARTY);
	fdScriptAddPartyMember(PIERS, 0);
	fdScriptEnd();
}

/* Counts one more weird answer Felix gives (fdScriptCountAnswer, through the
   overlay's own import row). */
void scKibomboCountAnswer(void);
void scKibomboCarryBlackCrystal(void);
void scKibomboCrowdBows(void);
void scKibomboCrowdStands(void);
void scKibomboEndOffering(void);

/* Akafubu silences the crowd and raises the Black Crystal before the Great
   Gabomba, whose mouth opens and shuts again; the crowd bows to pray harder.
   Piers sees that Akafubu is an Adept but cannot reach him, and Kraden asks
   Felix to help Piers past the stump; a refusal counts as a weird answer. */
void scKibomboAkafubuOffersCrystal(void)
{
	scKibomboCrowdStands();
	sySoundPlay(MUS_SILENCE);
	/* "SILENCE!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 10);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(264, CAMERA_KEEP, 388, 1);
	fdCameraWaitForArrival();
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
	/* "I shall now present our jewel to the Great Gabomba!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	sySoundPlay(MUS_JUPITER_LIGHTHOUSE);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CRYSTAL_MARKER), 0);
	fdActorGetById(ACTOR_CRYSTAL_MARKER)->motionFlags = 0;
	fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_CRYSTAL_MARKER),
		toFIXED(265), toFIXED(24), toFIXED(344));
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(264, CAMERA_KEEP, 340, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 0.2);
	fdActorWalkToAndWait(ACTOR_AKAFUBU, 264, 374);
	syFieldSetTickEvent(scKibomboCarryBlackCrystal, EVENT_PASS_TICK);
	fdScriptWait(20);
	syCloseEvent(scKibomboFlickerCeremonyFires);
	scKibomboAkafubuCastsOnCrystal();
	sySetEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	/* "!!!" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	/* "O Great Gabomba! Please accept this, our humble gift!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	sySoundPlay(MUS_RITUALS);
	scKibomboEndOffering();
	fdScriptWait(160);
	/* "Please, Great Gabomba! Accept this jewel!" */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
	sySoundPlay(SE_STATUE_STIRS);
	scKibomboSetStatueMouth(1, 1);
	fdScriptWait(20);
	/* "Look! The Great Gabomba has noticed the light of the jewel!" */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
	sySoundPlay(SE_RATTLE);
	scKibomboSetStatueMouth(0, 1);
	fdScriptWait(20);
	fdEffectSetAttached(
		ACTOR_AKAFUBU, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "Why, Great Gabomba?" ("Gabomba, why?" when Piers took the party for
	   enemies) */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
	syCloseEvent(scKibomboCarryBlackCrystal);
	sySynch(1);
	fdActorSetPos(ACTOR_CRYSTAL_MARKER, 0, 0);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
	if(KibomboBlackCrystal != 0) {
		if(KibomboBlackCrystal->y.fixed > toFIXED(36)) {
			do {
				fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
				KibomboBlackCrystal->y.fixed += toFIXED(-0.25);
				sySynch(1);
			} while(KibomboBlackCrystal->y.fixed > toFIXED(36));
		}
	}
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	/* "Your prayers were weak! Open your hearts to the Great Gabomba!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(264, 16, 420, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 1.6);
	fdActorWalkToAndWaitById(ACTOR_AKAFUBU, 264, 392);
	fdScriptWait(20);
	/* "Sing your praises and dance for the Great Gabomba until your
	   prayers..." */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
	sySoundPlay(SE_BOSS_ROAR);
	sySoundPlay(MUS_RITUALS);
	scKibomboCrowdBows();
	fdScriptWait(80);
	fdCameraMoveTo(88, 32, 392, 1);
	fdCameraWaitForArrival();
	fdScriptWait(40);
	/* "Akafubu is no witch doctor! He's an Adept!" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "I may have to confront him to get my orb back... I may have to
	   fight." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I will do whatever it takes, but I will have my black orb back
	   again." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorSetSpritePriority(ACTOR_PIERS, 2);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorWalkToAndWait(ACTOR_PIERS, 152, 386);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 40);
	/* "It's no use. We can't get to Akafubu from here." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorWalkToAndWait(ACTOR_PIERS, 124, 356);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
	/* "This won't work, either... What are we going to do? We can't get
	   over..." */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	/* "Piers's in trouble. Shouldn't we try to help him?" */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 40);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToById(ACTOR_KRADEN, 104, 368);
	fdScriptWait(20);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(111, CAMERA_KEEP, 356, 1);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Maybe if we could do something about that stump, we could reach
	   the..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	/* "We've got to help Piers. Right, Felix?" */
	fdScriptOpenMessage(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		/* "Then hop to it!" */
		fdScriptShowMessageAndWait(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
		FieldStateWork->msg++;
	} else {
		FieldStateWork->msg++;
		/* "What's all this now, Felix? Is it too much to ask that you
		   help..." */
		fdScriptShowMessageAndWait(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
		scKibomboCountAnswer();
	}
}

/* Felix walks off the grounds by the south-west path, metatiles (4, 30) and (5,
   30). With Piers along and the orb not yet won back, Piers will not leave
   without it: he says so, walks back up the path and stays behind as the party
   goes down to the village. */
void scKibomboPiersRefusesToLeave(void)
{
	if(!fdScriptGetFlag(FLAG_KIBOMBO_PIERS_WITH_PARTY)) {
		syImportScriptSetExitRequest(
			KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B);
		return;
	}
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorSetSpritePriority(ACTOR_PIERS,
		((FIELD_SPR *)fdActorGetById(MainRoutineStateWork.leaderActorId)
				->sprite)
			->priority);
	sySynch(1);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorWalkToAndWaitById(ACTOR_PIERS, 62, 444);
	fdActorFaceDirectionAndRefreshById(ACTOR_PIERS, FACING_SOUTHEAST);
	fdScriptSetMessageId((Sint32)MsgNowThatIKnow);
	/* "Now that I know the orb is here, I won't leave this village
	   without..." */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorGetById(ACTOR_PIERS)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorWalkToAndWait(ACTOR_PIERS, 56, 420);
	fdActorWalkToAndWait(ACTOR_PIERS, 40, 344);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 80, 510);
	fdScriptWait(10);
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B);
	syGameFlagClear(FLAG_KIBOMBO_PIERS_WITH_PARTY);
	syGameFlagSet(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
	fdPartyRemoveMemberRestored(PIERS);
	fdScriptEnd();
}

extern Uint32 KibomboLeaderStepsUpScript[];
extern Uint32 KibomboPiersStepsUpScript[];
extern Uint32 KibomboKradenJoinsPiersScript[];
extern Uint32 KibomboJennaJoinsPiersScript[];
extern Uint32 KibomboShebaJoinsPiersScript[];
extern Uint32 KibomboPartyFollowsScript[];

/* Piers, who saw the party's Psynergy, tells them he too is an Adept, from
   Lemuria in the middle of the Eastern Sea, and joins the party with his Djinn;
   Felix's answers to Kraden choose the lines that follow. */
void scKibomboPiersJoinsParty(void)
{
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_NORTH, 0);
	fdScriptSetMessageId(MsgPiersStartled);
	/* "?!?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(96, CAMERA_KEEP, 340, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdEffectSetAttached(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "That power... Was that Psynergy?" */
	fdScriptAskYesNo(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(96, CAMERA_KEEP, 372, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.2);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetMovementTypeById(ACTOR_PIERS, KibomboPiersStepsUpScript);
	fdActorSetMovementTypeAndWaitById(
		MainRoutineStateWork.leaderActorId, KibomboLeaderStepsUpScript);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetPos(ACTOR_KRADEN, 56, 372);
	fdActorSetPos(ACTOR_JENNA, 56, 372);
	fdActorSetPos(ACTOR_SHEBA, 56, 372);
	sySynch(1);
	fdActorSetMovementTypeById(ACTOR_KRADEN, KibomboKradenJoinsPiersScript);
	fdActorSetMovementTypeById(ACTOR_JENNA, KibomboJennaJoinsPiersScript);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_SHEBA, KibomboShebaJoinsPiersScript);
	fdScriptWait(20);
	/* "I should have spotted it at once! You're Adepts!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Wait, so does that mean you're" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "Yes, my name is Piers, and I too am an Adept." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	/* "To be honest, everyone in Lemuria is an Adept, and not just me." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "What's Lemuria?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	/* "Lemuria is an island that lies in the center of the Eastern Sea." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "Wait a second... You said Lemuria? I've heard that name before..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
	/* "Of course! It was Lord Babi! He often spoke to me of Lemuria." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	/* "He said it was so advanced that we can scarcely begin to imagine
	   what..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	/* "You are correct. My home was and is all that you say. Whoever
	   told..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 20);
	/* "But wait, if it's so far away in the middle of the ocean, how
	   come..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "It is not easily reached, and we Lemurians seldom leave... I
	   cannot..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "Except... Hold on a moment... Babi... I've heard that name
	   somewhere..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	/* "Well, yeah... Everyone knows about Lord Babi... He's the ruler of..." */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I've never heard of Tolbi. Until recently, I had never left my
	   home..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	/* "Piers, are you just playing games with us?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	/* "Don't be foolish. What would I gain from lying to you?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I'm being quite serious." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	/* "Piers is not playing games, Jenna. You can trust him." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
	/* "Thank you for believing me, master sage!" */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "Please, call me Kraden, Piers of Lemuria." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "And I am the one who should be thanking you!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	/* "You look puzzled, Felix. You are probably wondering what I'm
	   talking..." */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 1) {
		/* "Oh, good! Then I probably don't need to tell you..." */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		/* "Wait, just because my brother knows what you're talking about..." */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
		FieldStateWork->msg += 1;
	} else {
		FieldStateWork->msg += 2;
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		/* "I thought as much..." */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	}
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "If what Babi told me is true, Lemuria is all that remains of a
	   once..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	/* "Your friend is correct." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	/* "In fact, the reason I was sent to Vale was somewhat related to my..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "I don't understand... What was in Vale that had anything to do
	   with..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Babi believed that, in Lemuria alone, the power of Alchemy
	   remains..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 20);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Lord Babi had need of Lemuria's Alchemy, but despite our effort,
	   we..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "Since we could not find Lemuria, Lord Babi sent me to unlock the..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "Ah, yes! Babi! If I recall correctly, a man named Babi came to..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 20);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "He stole one of our ships and fled the isle..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Yes! That must indeed be the same Lord Babi who know leads the
	   people..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "Now, I'm even more confused. This is making my head hurt..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	/* "I mean, if Babi really stole a ship from your home, why would he
	   be..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "I can't expect you to understand, my dear. Lord Babi's reasons
	   for..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "But if he already found Lemuria once, why doesn't he just go back
	   the..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "Once you have left Lemuria, finding it again is quite difficult,
	   even..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Well...why?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	/* "The waters around Lemuria are wrapped in a perpetual fog... It is..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "I remember Lord Babi once saying something to that effect." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "Even if one locates Lemuria, getting through the fog itself
	   remains..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 40);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_SHEBA, FACING_EAST, 20);
	/* "I don't get it... Why can't you just sail straight through it?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "The ocean currents around Lemuria are swift and treacherous...
	   Only..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "But...Babi stole a Lemurian ship from you, right? That means that
	   he..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Unfortunately, Babi was the only one of us who could use the ship." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	/* "You see, Lemurian ships can only be helmed by Adepts..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "Which is why I have to get my black orb back! I must be able to..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 20);
	/* "There may be something up ahead on this path." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "Then let's go..." */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_PIERS);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_PIERS);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_PIERS);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_PIERS);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorWalkToAndWaitById(ACTOR_PIERS, 72, 368);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorWalkToAndWait(ACTOR_PIERS, 40, 348);
	fdActorWalkToAndWait(ACTOR_PIERS, 40, 332);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	fdActorStop(ACTOR_KRADEN);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	/* "Wait a moment, Piers..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "We want to help you get your orb back." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "You intend to help me, Kraden?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "I am on this quest for many reasons...one of them being that I
	   wish..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	/* "If you would allow me, that is... I'd like to visit Lemuria myself." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "If this is something you would do for Babi, then I cannot permit it." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "What? Why not?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "Babi is a thief. He is not well loved in Lemuria." */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "It is partially on behalf of Babi... ...but there's more to it than
	   that!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
	/* "I want to go to Lemuria so that I can see the full power of Alchemy!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	/* "I want to aid Piers so that he will take us to Lemuria... What do
	   you..." */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
		/* "You'll do it!? Splendid!" */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
		fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		FieldStateWork->msg += 3;
	} else {
		FieldStateWork->msg += 1;
		fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* "Why, Felix? Why? Please, we simply have to!" */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
		/* "What's the matter with you, Felix? I don't see why we can't help
		   Piers." */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
		fdActorFaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
		/* "Yeah. I'm with Jenna on this one." */
		fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
		fdActorFaceDirectionById(
			MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
		fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		scKibomboCountAnswer();
	}
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "I appreciate your offer, but..." */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	/* "I understand, Piers... You're not sure if you'll be able to take
	   us..." */
	fdScriptShowMessageAndWait(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	/* "Can't we talk about this later?" */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "Yes, yes, of course! For now, let's just get that orb back for
	   Piers!" */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetSpeed(ACTOR_PIERS, 0.4);
	fdActorWalkToAndWaitById(ACTOR_PIERS, 40, 350);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetMovementTypeById(ACTOR_JENNA, KibomboPartyFollowsScript + 0);
	fdActorSetMovementTypeById(ACTOR_SHEBA, KibomboPartyFollowsScript + 0);
	fdActorSetMovementTypeById(ACTOR_KRADEN, KibomboPartyFollowsScript + 0);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_PIERS, KibomboPartyFollowsScript + 0);
	syGameFlagSet(FLAG_PIERS_JOINED);
	syGameFlagSet(FLAG_KIBOMBO_PIERS_WITH_PARTY);
	fdScriptAddPartyMember(PIERS, 1);
	gmPartyGivePiersDjinn();
}

extern Uint32 KibomboJennaAtDeadEndScript[];
extern Uint32 KibomboShebaAtDeadEndScript[];
extern Uint32 KibomboKradenAtDeadEndScript[];
extern Uint32 KibomboPiersAtDeadEndScript[];
extern Uint32 KibomboPartyLeavesDeadEndScript[];
void		  scKibomboCastScoop(void);

/* The first time Felix comes up the path east of the statue to the dead end,
   metatiles (21, 14) and (22, 14), before the vent is uncovered: the party
   stops among the four marks on the ground, "It's a dead end...". Kraden has
   Felix Scoop each mark in turn, and the party works out that a vent into the
   statue lies under the patch it passed on the way. */
void scKibomboDeadEndByTheStatue(void)
{
	Sint32 done = fdScriptGetFlag(FLAG_KIBOMBO_VENT_UNCOVERED);

	if(done != 0)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 344, 208);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 308, 200);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 290, 188);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetMovementTypeById(ACTOR_JENNA, KibomboJennaAtDeadEndScript);
	fdActorSetMovementTypeById(ACTOR_SHEBA, KibomboShebaAtDeadEndScript);
	fdActorSetMovementTypeById(ACTOR_KRADEN, KibomboKradenAtDeadEndScript);
	fdActorSetMovementTypeAndWaitById(ACTOR_PIERS, KibomboPiersAtDeadEndScript);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdScriptSetMessageId(MsgItSADead);
	/* "It's a dead end..." */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
	fdCameraSetSpeed(2.4);
	fdCameraGetActor()->motionFlags = done;
	fdCameraMoveTo(200, CAMERA_KEEP, 180, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdCameraMoveTo(174, CAMERA_KEEP, 224, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdCameraMoveTo(268, CAMERA_KEEP, 120, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdCameraMoveTo(272, CAMERA_KEEP, 180, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
	/* "But this was the only path..." */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "Isn't it a little strange that there would be a path that just
	   goes..." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_KRADEN);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_KRADEN);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_KRADEN);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(268, CAMERA_KEEP, 146, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 290, 116);
	fdActorWalkToAndWait(ACTOR_KRADEN, 268, 109);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
	/* "Hmm..." */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	fdCameraMoveTo(268, CAMERA_KEEP, 188, 1);
	fdActorWalkToAndWaitById(ACTOR_KRADEN, 218, 156);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 40);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	fdActorStop(ACTOR_PIERS);
	/* "I see..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
	fdActorSetSpeed(ACTOR_KRADEN, 0.4);
	fdActorWalkToAndWaitById(ACTOR_KRADEN, 248, 156);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "So that's how it works..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_KRADEN);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_KRADEN);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_KRADEN);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_KRADEN);
	fdScriptWait(20);
	fdActorGetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWaitById(ACTOR_KRADEN, 248, 188);
	fdScriptWait(1);
	fdActorGetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorSetAnimationById(ACTOR_KRADEN, 7);
	fdScriptWait(20);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	fdActorStop(ACTOR_PIERS);
	/* "Listen up, Felix..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.4);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 280, 188);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_STAND);
	fdScriptWait(10);
	fdActorGetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWaitById(ACTOR_KRADEN, 238, 188);
	fdScriptWait(1);
	fdActorGetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
	/* "Dig there!" */
	fdMessageShowAtActor(ACTOR_KRADEN, 0);
	fdActorShowEmoteById(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
ask:
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		FieldStateWork->msg += 1;
	} else {
		/* "Don't worry. Just use Scoop... You'll see why in just a few
		   seconds..." */
		fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
		FieldStateWork->msg -= 2;
		/* "Dig there!" */
		fdMessageShowAtActor(ACTOR_KRADEN, 0);
		goto ask;
	}
	fdScriptSetFlag(FLAG_SCOOPING_SOUTH_MARK);
	scKibomboCastScoop();
	syGameFlagClear(FLAG_SCOOPING_SOUTH_MARK);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "I see..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "What's this?" */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "Someone has put something in the ground here." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	/* "And this statue..." */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	/* "Well, take a look..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	/* "Felix! Try using Scoop around there!" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorWatchAndSetScript(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdCameraMoveTo(268, CAMERA_KEEP, 164, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 280, 120);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_WEST);
	fdScriptSetFlag(FLAG_SCOOPING_NORTH_MARK);
	scKibomboCastScoop();
	syGameFlagClear(FLAG_SCOOPING_NORTH_MARK);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 232, 142);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH);
	fdScriptSetFlag(FLAG_SCOOPING_WEST_MARK);
	scKibomboCastScoop();
	syGameFlagClear(FLAG_SCOOPING_WEST_MARK);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 296, 142);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH);
	fdScriptSetFlag(FLAG_SCOOPING_EAST_MARK);
	scKibomboCastScoop();
	syGameFlagClear(FLAG_SCOOPING_EAST_MARK);
	fdCameraMoveTo(268, CAMERA_KEEP, 188, 1);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 284, 188);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 10);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_PIERS);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Ah... I thought so..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "You thought what?" */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	/* "These marks are here to indicate an entrance there, in the center
	   of..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "You mean we can go inside this statue?" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	/* "You could at one point..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "The entrance was probably beneath that stone." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
	/* "I wonder... Was this the only one?" */
	fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 10);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "It seems that way..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdCameraMoveTo(268, CAMERA_KEEP, 164, 1);
	fdActorWalkToAndWait(ACTOR_JENNA, 296, 168);
	fdActorWalkToAndWait(ACTOR_JENNA, 280, 154);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 40);
	/* "Man, if this was the only entrance, then I feel bad for the
	   people..." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "Why is that?" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	/* "Well, if this is the only hole, then there's no way for air to get
	   in..." */
	fdScriptShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "I...hadn't even thought of that." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdCameraMoveTo(268, CAMERA_KEEP, 188, 1);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	/* "We must inspect this area carefully, Felix." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Jenna has raised an interesting point..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "If people did indeed build this, they would not have been able to
	   do..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	/* "They might have opened a vent somewhere nearby." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 10);
	/* "And if so, then there may have been a ladder to enter the vent.
	   We..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	/* "Kraden, there doesn't seem to be anything of the sort here!" */
	fdScriptShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "What were we just looking at?" */
	fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
	/* "If there is a vent shaft, it would probably be quite old, maybe
	   even..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "And if that's the case, we should search more thoroughly." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
	/* "OK, Felix!" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetMovementTypeById(ACTOR_JENNA, KibomboPartyLeavesDeadEndScript);
	fdActorSetMovementTypeById(ACTOR_SHEBA, KibomboPartyLeavesDeadEndScript);
	fdActorSetMovementTypeById(ACTOR_KRADEN, KibomboPartyLeavesDeadEndScript);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_PIERS, KibomboPartyLeavesDeadEndScript);
	syGameFlagSet(FLAG_KIBOMBO_DEAD_END_SEEN);
	fdScriptEnd();
}

/* Nothing visible happens as Felix steps onto the ring of metatiles (19, 7) to
   (21, 10) among the painted poles in the north of the village. The village
   keeps two masks (`FADE_OVERLAY_STATE`, drawn by `fdBlankOverlayDrawMarks`)
   that cover whatever of Felix a raised metatile beside him should hide; from
   here they follow him. */
void scKibomboOverlayFollowsLeader(void)
{
	/* Behind the pole on the ring's north side the masks hide Felix as he
	   passes, and further east the hut's thatched roof hides all of him but the
	   top of his head. */
	FADE_OVERLAY_STATE *state = RamHeapSlots->fadeOverlay;

	state->actor =
		(FADE_ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId);
}

/* Nothing visible happens as Felix steps onto metatile (20, 8) or (20, 9), in
   the middle of that ring among the painted poles; the masks that hide him
   behind raised metatiles stop following him. */
void scKibomboOverlayStopsFollowing(void)
{
	FADE_OVERLAY_STATE *state = RamHeapSlots->fadeOverlay;

	state->actor = 0;
}

/* Clamps a colour channel to 0 to RGB_LEVEL_MAX. */
Sint32 scKibomboClampColor(Sint32 value)
{
	if(value > RGB_LEVEL_MAX)
		value = RGB_LEVEL_MAX;
	if(value < 0)
		value = 0;
	return value;
}

/* Sets a palette colour from its fire colour, brightened by delta (half for
   green, a quarter for blue). */
void scKibomboFlickerColor(Sint32 index, Sint32 entry, Sint32 delta)
{
	Uint16 color = KibomboFireColors[entry];
	Sint16 r = RGB_RED(color);
	Sint16 g = RGB_GREEN(color);
	Sint16 b = RGB_BLUE(color);

	r += delta;
	g += delta >> 1;
	b += delta >> 2;

	r = scKibomboClampColor(r);
	g = scKibomboClampColor(g);
	b = scKibomboClampColor(b);
	{
		Uint16 *dest = &BG_PLTT[index];

		r |= RGB(0, g, b);
		*dest = r;
	}
}

/* As scKibomboFlickerColor, four steps redder, with green and blue two and one
   steps dimmer. */
void scKibomboFlickerRedColor(Sint32 index, Sint32 entry, Sint32 delta)
{
	Uint16 color = KibomboFireColors[entry];
	Sint16 r = RGB_RED(color);
	Sint16 g = RGB_GREEN(color);
	Sint16 b = RGB_BLUE(color);

	r += delta + 4;
	delta -= 4;
	g += delta >> 1;
	b += delta >> 2;

	r = scKibomboClampColor(r);
	g = scKibomboClampColor(g);
	b = scKibomboClampColor(b);
	{
		Uint16 *dest = &BG_PLTT[index];

		r |= RGB(0, g, b);
		*dest = r;
	}
}

/* Defines a tick event in which, every eighth tick outside menus and
   Psynergy, the fires' four colours flicker: the first by up to 5 either way,
   the others by a random amount below their range less their bias, the fourth
   through flickerFourth. */
#define DEFINE_KIBOMBO_FLICKER_FIRES(name, secondIndex, secondRange, \
	secondBias, thirdIndex, flickerFourth, fourthIndex, fourthRange, \
	fourthBias) \
	void name(void) \
	{ \
		FIELD_STATE *state = RamHeapSlots->fieldState; \
\
		if(state->psynergy == 0 && state->fieldMenu == 0 && \
			state->pauseMenu == 0 && (TickCount & 7) == 0) { \
			scKibomboFlickerColor( \
				FIRE_COLOR_FIRST, 0, (syRandomBelow(10)) - 5); \
			scKibomboFlickerColor( \
				secondIndex, 1, (syRandomBelow(secondRange)) - secondBias); \
			scKibomboFlickerColor(thirdIndex, 2, (syRandomBelow(4)) - 2); \
			flickerFourth( \
				fourthIndex, 3, (syRandomBelow(fourthRange)) - fourthBias); \
		} \
	}

/* A tick event in the night village: every eighth tick outside menus and
   Psynergy, the four fire colours flicker. */
DEFINE_KIBOMBO_FLICKER_FIRES(scKibomboFlickerVillageFires,
	VILLAGE_FIRE_COLOR_SECOND, 8, 4, VILLAGE_FIRE_COLOR_THIRD,
	scKibomboFlickerColor, VILLAGE_FIRE_COLOR_FOURTH, 4, 2)

/* As scKibomboFlickerVillageFires on the night grounds, its fourth colour
   flickering redder. */
DEFINE_KIBOMBO_FLICKER_FIRES(scKibomboFlickerCeremonyFires,
	GROUNDS_FIRE_COLOR_SECOND, 6, 3, GROUNDS_FIRE_COLOR_THIRD,
	scKibomboFlickerRedColor, GROUNDS_FIRE_COLOR_FOURTH, 8, 4)

/* A tick event before the statue's face: every eighth tick outside menus and
   Psynergy, the three fire colours flicker. */
void scKibomboFlickerStatueFires(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	if(state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
		(TickCount & 7) == 0) {
		scKibomboFlickerColor(
			STATUE_FIRE_COLOR_FIRST, 0, (syRandomBelow(4)) - 2);
		scKibomboFlickerColor(
			STATUE_FIRE_COLOR_SECOND, 1, (syRandomBelow(2)) - 1);
		scKibomboFlickerColor(STATUE_FIRE_COLOR_THIRD, 2, syRandom() >> 16);
	}
}

/* Creates the Black Crystal, drawn with the item's own icon. */
void scKibomboCreateBlackCrystal(void)
{
	ACTOR **slot = &KibomboBlackCrystal;
	Uint8	clear = 0;
	ACTOR  *actor = syImportActorCreate(
		 SPRITE_UNSEEN_MARKER, toFIXED(265), toFIXED(36), toFIXED(344));

	*slot = actor;
	if(actor != 0) {
		FIELD_SPR	  *sprite = actor->sprite;
		ICON_TRANSFER *glyph;

		sprite->fullColor = 0;
		sprite->palette = 0;
		sprite->priority = 1;
		actor->motionFlags = clear;
		actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
		sprite->flags = clear;
		sprite->count = clear;
		glyph =
			syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_BLACK_CRYSTAL);
		syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
}

/* Unused: with the leader up high, takes the door's unseen block away and draws
   the first lit candlestick in front; lower down, puts the block back at the
   house's door and the candlestick behind. */
UNUSED void scKibomboPlaceDoorBlockByHeight(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	if(leader->y.fixed > toFIXED(16)) {
		fdActorSetPos(ACTOR_DOOR_BLOCK, 0, 0);
		fdActorSetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 2);
	} else {
		scFieldActorSetPosAndResetMotion(
			fdActorGetById(ACTOR_DOOR_BLOCK), toFIXED(120), 0, toFIXED(168));
		fdActorSetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 3);
	}
}

/* Keeps the Black Crystal above the one who holds it. */
void scKibomboCarryBlackCrystal(void)
{
	if(KibomboBlackCrystal != 0) {
		ACTOR *actor = fdActorGetById(ACTOR_CRYSTAL_MARKER);

		fdActorSetPosAndResetMotion(KibomboBlackCrystal, actor->x.fixed,
			actor->y.fixed + toFIXED(12), actor->z.fixed);
	}
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_KIBOMBO_FLICKER_FIRES

extern Uint32 KibomboEmberScript[];

/* Embers rise from the statue's fire every fourth tick. */
void scKibomboRaiseEmbers(void)
{
	Sint32 phase = TickCount & 3;

	if(phase == 0) {
		ACTOR *actor = syImportActorCreate(
			SPRITE_BLAZE_FLAME, toFIXED(264), toFIXED(32), toFIXED(316));
		Sint32 value;

		if(actor != 0) {
			actor->bounce = syCos(syRandomBelow(8192) + TRIG_TURN * 3 / 16);
			actor->gravity = toFIXED(1);
			actor->sceneWord = toFIXED(4) - (Uint16)syRandom();
			value = (Uint16)syRandom() + 0xffff8000;
			actor->timer = value;
			actor->motionFlags = 0;
			fdActorSetSpriteFlags(actor, 0);
			fdActorSelectAnimation(actor, 2);
			fdActorStartScript(actor, KibomboEmberScript);
			((FIELD_SPR *)actor->sprite)->priority = 1;
			actor->timer = 0;
			actor->speedLimit = 0;
			actor->acceleration = 0;
			actor->update = (OBJECT_UPDATE)fdEffectAdvanceMotion;
		}
	}
}

#include "shared/smoke_drift.h"

/* Smoke rises from the cooking fire every 64 ticks. */
void scKibomboRaiseSmoke(void)
{
	if((TickCount & 63) == 0) {
		ACTOR *actor = syImportActorCreate(
			SPRITE_DUST_CLOUD, toFIXED(88), toFIXED(16), toFIXED(314));

		if(actor != 0) {
			actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
			actor->motionFlags = 0;
			actor->timer = 0;
			actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
			((FIELD_SPR *)actor->sprite)->priority = 2;
			fdActorSetSpriteFlags(actor, 0);
			actor->scaleX = toFIXED(1) / 2;
			actor->scaleY = toFIXED(1) / 2;
			*(Sint32 *)&actor->watchedActor = SMOKE_TICKS;
			actor->update = (OBJECT_UPDATE)scSmokeDrift;
			fdActorSelectAnimation(actor, 5);
			fdActorSetBlendMode(actor, 1);
		}
	}
}

/* A flame's update: every ten to twenty ticks it flips between wide and
   mirrored. */
void scKibomboFlickerFlame(ACTOR *actor)
{
	if(actor->timer != 0) {
		actor->timer--;
	} else {
		actor->timer = (syRandomBelow(10)) + 10;
		if(actor->userValue != 0) {
			actor->scaleX = toFIXED(3) / 2;
			actor->userValue = 0;
		} else {
			actor->scaleX = -toFIXED(3) / 2;
			actor->userValue = 1;
		}
	}
}

/* Creates one of the signal fire's flames at x, y, z, flickering by its own
   timer. */
void scKibomboSpawnFlame(Sint32 x, Sint32 y, Sint32 z)
{
	ACTOR	  *actor = syImportActorCreate(SPRITE_FLAME_AND_SMOKE, x, y, z);
	FIELD_SPR *sprite;

	if(actor != 0) {
		actor->motionFlags = 0;
		actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
		actor->scaleY = toFIXED(2);
		actor->timer = (syRandomBelow(10)) + 10;
		actor->userValue = 0;
		actor->update = (OBJECT_UPDATE)scKibomboFlickerFlame;
		sprite = actor->sprite;
		sprite->priority = 1;
		fdActorSetSpriteFlags(actor, 0);
		fdActorSelectAnimation(actor, 5);
		fdActorSetBlendMode(actor, 1);
	}
}

/* Lights the signal fire: five flames rise in a column above it, blended over
   the background. */
void scKibomboLightSignalFire(void)
{
	FIXED x;

	REG_BLDALPHA = BLEND_ALPHA(8, 12);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	x = toFIXED(440);
	scKibomboSpawnFlame(x, toFIXED(40), toFIXED(450));
	scKibomboSpawnFlame(x, toFIXED(46), toFIXED(438));
	scKibomboSpawnFlame(x, toFIXED(52), toFIXED(426));
	scKibomboSpawnFlame(x, toFIXED(58), toFIXED(414));
	scKibomboSpawnFlame(x, toFIXED(64), toFIXED(402));
}

#include "shared/actor_hide.h"

/* Back from a battle or a menu, stops drawing the room's unseen blocks again
   unless they were shown; otherwise notes them shown. */
void scKibomboRestoreBlocks(void)
{
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		if(syGameFlagGet(FLAG_KIBOMBO_BLOCKS_SHOWN) == 0)
			scKibomboHideBlocks();
	} else {
		syGameFlagSet(FLAG_KIBOMBO_BLOCKS_SHOWN);
	}
}

/* Hides the grounds' two unseen blocks as the room opens, the first raised 32
   pixels. */
void scKibomboHideGroundsBlocks(void)
{
	Uint32 i;

	for(i = 0; i <= 1; i++) {
		Sint32 id = i + ACTOR_GROUNDS_FIRST_BLOCK;
		ACTOR *actor = fdActorGetById(id);

		scActorHide(id);
		actor->motionFlags = 0;
	}
	fdActorGetById(ACTOR_GROUNDS_FIRST_BLOCK)->y.fixed = toFIXED(32);
	fdActorGetById(ACTOR_GROUNDS_SECOND_BLOCK)->y.fixed = 0;
	scKibomboRestoreBlocks();
}

/* Hides the two unseen blocks in the inn's back doorway as the room opens. */
void scKibomboHideInnBlocks(void)
{
	Uint32 i;

	for(i = 0; i <= 1; i++) {
		Sint32 id = i + ACTOR_INN_FIRST_BLOCK;
		ACTOR *actor = fdActorGetById(id);

		scActorHide(id);
		actor->motionFlags = 0;
	}
	scKibomboRestoreBlocks();
}

/* Defines a function that gives the eight warriors, ACTOR_WARRIOR_A to H, one
   animation. */
#define DEFINE_KIBOMBO_SET_CROWD_MODE(name, animation) \
	void name(void) \
	{ \
		Uint32 i; \
\
		for(i = 0; i <= ACTOR_WARRIOR_H - ACTOR_WARRIOR_A; i++) \
			fdActorSetAnimationById(i + ACTOR_WARRIOR_A, animation); \
	}

/* The eight warriors, ACTOR_WARRIOR_A to H, bow (their sprite sheet's animation
   5). */
DEFINE_KIBOMBO_SET_CROWD_MODE(scKibomboCrowdBows, 5)

/* The eight warriors stand again. */
DEFINE_KIBOMBO_SET_CROWD_MODE(scKibomboCrowdStands, ANIM_STAND)

/* Akafubu's Psynergy on the Black Crystal. */
void scKibomboAkafubuCastsOnCrystal(void)
{
	fdPsynergyLoadResources(ABILITY_LIFT, 1);
	fdPsynergySetupActorPair(ACTOR_AKAFUBU, ACTOR_CRYSTAL_MARKER);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
}

/* A spark's update while Akafubu casts on the crystal: it circles its center at
   its radius, wobbling in and out, and rises. */
void scKibomboSpinSpark(FIELD_OBJECT *object)
{
	FIXED	wobble = 0;
	Uint8  *timer = &object->actor.sceneByte;
	Sint16 *action;
	Uint8  *stage;
	VECTOR3 center;
	Sint32	radius;
	Sint32	spin;
	Sint32	turn;

	if(*timer != 0)
		wobble =
			sySin((object->actor.timer + (Sint16)object->actor.userValue) * 9
				<< 9) *
			6;
	action = &object->actor.timer;
	center.x = object->actor.targetX;
	center.y = object->actor.targetY;
	center.z = object->actor.targetZ;
	radius = object->actor.sceneWord;
	spin = (Sint16)object->actor.userValue;
	turn = *action;
	syVectorAddPolarOffset(
		toFIXED(radius) + wobble, turn * (TRIG_TURN * 3 / 256) + spin, &center);
	object->actor.x.fixed = center.x;
	object->actor.y.fixed = center.y;
	object->actor.z.fixed = center.z;
	stage = &object->actor.sceneCount;
	switch(*stage) {
	case 0:
		*action += 4;
		object->actor.sceneWord = 1;
		((FIELD_SPR *)object->actor.sprite)->rotation += TRIG_TURN / 16;
		object->actor.scaleX += toFIXED(0.025);
		object->actor.scaleY += toFIXED(0.025);
		if(object->actor.scaleX > toFIXED(3) / 2 &&
			((FIELD_SPR *)object->actor.sprite)->rotation == 0) {
			*stage = 1;
			*timer = 1;
		}
		break;
	case 1:
		*action += 4;
		((FIELD_SPR *)object->actor.sprite)->rotation += TRIG_TURN / 16;
		if(((FIELD_SPR *)object->actor.sprite)->rotation == 0 &&
			--*timer == 0) {
			sySoundPlay(SE_SHIMMER);
			*stage = 2;
		}
		break;
	case 2:
		*action += 1;
		object->actor.targetY += toFIXED(4);
		if(object->actor.targetY > toFIXED(100))
			*stage = 3;
		break;
	case 3:
		*action += 1;
		object->actor.sceneWord += 2;
		*timer = 10;
		if(object->actor.sceneWord > 40)
			*stage = 4;
		break;
	case 4:
		*action += 1;
		if(--*timer == 0)
			*stage = 5;
		break;
	case 5:
		*action += 1;
		object->actor.sceneWord -= 2;
		if(object->actor.targetY < toFIXED(136))
			object->actor.targetY += toFIXED(2);
		if(object->actor.sceneWord <= 1) {
			object->actor.sceneWord = 0;
			*stage = 6;
		}
		break;
	case 6:
		*action += 4;
		((FIELD_SPR *)object->actor.sprite)->rotation += TRIG_TURN / 16;
		object->actor.scaleX += -toFIXED(1) / 16;
		object->actor.scaleY += -toFIXED(1) / 16;
		if(object->actor.scaleX < toFIXED(9) / 128) {
			object->actor.targetX = 100;
			object->actor.targetZ = 100;
			*stage = 7;
		}
		break;
	case 7:
		syVRAMCacheFreeEntry(((FIELD_SPR *)object->actor.sprite)->slot);
		fdActorRelease(&object->actor);
		break;
	}
}

/* The sparks that circle Akafubu as he chants, 22 degrees apart. */
#define CHANT_SPARKS 16

/* Akafubu glows and sparks circle him. */
void scKibomboAkafubuChants(void)
{
	ACTOR *center = fdActorGetById(ACTOR_AKAFUBU);
	ACTOR *spark;
	Sint32 prev = 0;
	Uint32 i;
	Uint32
		/* degrees in 16.16; userValue takes angle / 360, 65,536ths of a turn */
		  angle;
	FIXED x;
	FIXED y;
	FIXED z;

	sySoundPlay(SE_CAST);
	for(i = 0; i <= 7; i++) {
		fdActorSetSpriteColor(
			fdActorGetById(ACTOR_AKAFUBU), SPRITE_COLOR_WHITE);
		sySynch(2);
		fdActorSetSpriteColor(
			fdActorGetById(ACTOR_AKAFUBU), SPRITE_COLOR_NORMAL);
		sySynch(2);
	}
	sySoundPlay(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
	i = 0;
	angle = 0;
	for(; i < CHANT_SPARKS; i++) {
		x = center->x.fixed;
		y = center->y.fixed + toFIXED(24);
		z = center->z.fixed;
		spark = syImportActorCreate(SPRITE_SPARKLE, x, y, z);
		if(spark != 0) {
			prev = fdSpriteShareVRAMCacheEntry(spark->sprite, prev);
			spark->motionFlags = 0;
			((FIELD_SPR *)spark->sprite)->priority = 1;
			fdActorSetSpriteFlags(spark, 0);
			fdActorSelectAnimation(spark, 1);
			spark->timer = 0;
			spark->sceneWord = 0;
			spark->userValue = angle / 360;
			spark->sceneCount = 0;
			spark->targetX = center->x.fixed;
			spark->targetY = center->y.fixed + toFIXED(24);
			spark->targetZ = center->z.fixed;
			spark->update = (OBJECT_UPDATE)scKibomboSpinSpark;
		}
		angle += toFIXED(22);
	}
}

/* Ends the offering: the fires stop flickering and, while the crystal is still
   out, Akafubu chants. */
void scKibomboEndOffering(void)
{
	syCloseEvent(scKibomboFlickerCeremonyFires);
	sySynch(1);
	fdCameraWaitForArrival();
	fdScriptWait(40);
	if(KibomboBlackCrystal != 0)
		scKibomboAkafubuChants();
}

/* Opens or shuts the Great Gabomba's mouth; while it is open its two eyes show,
   ringed with light if `glow`. */
void scKibomboSetStatueMouth(Sint32 open, Sint32 glow)
{
	if(open != 0) {
		fdMapGridCopyMetatilesTo(49, 37, 79, 15, 3, 2);
		scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_STATUE_LEFT_EYE),
			toFIXED(253), toFIXED(32), toFIXED(289));
		fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_STATUE_RIGHT_EYE),
			toFIXED(275), toFIXED(32), toFIXED(289));
		if(glow != 0) {
			fdActorSetAnimationById(ACTOR_STATUE_LEFT_EYE, 0);
			fdActorSetAnimationById(ACTOR_STATUE_RIGHT_EYE, 0);
		} else {
			fdActorSetAnimationById(ACTOR_STATUE_LEFT_EYE, 1);
			fdActorSetAnimationById(ACTOR_STATUE_RIGHT_EYE, 1);
		}
	} else {
		fdMapGridCopyMetatilesTo(45, 37, 79, 15, 3, 2);
		fdActorSetPos(ACTOR_STATUE_LEFT_EYE, 0, 0);
		fdActorSetPos(ACTOR_STATUE_RIGHT_EYE, 0, 0);
	}
}

/* Puts out the Great Gabomba's tongue in a STATUE_TONGUE_POSE, or draws it
   back. */
void scKibomboSetStatueTongue(Sint32 open, Sint32 pose)
{
	if(open != 0) {
		fdMapGridCopyMetatileTilesIndicesRect(41, 38, 79, 16, 3, 3);
		fdMapGridCopyMetatileAttributes(12, 21, 1, 1, 16, 22);
		if(pose != STATUE_TONGUE_IN_MOUTH)
			fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_STATUE_TONGUE),
				toFIXED(264), toFIXED(32), toFIXED(308));
		switch(pose) {
		case STATUE_TONGUE_IN_MOUTH:
			fdActorSetPos(ACTOR_STATUE_TONGUE, 0, 0);
			break;
		case STATUE_TONGUE_UNROLLING_BY_DAY:
			fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 0);
			break;
		case STATUE_TONGUE_OUT_BY_DAY:
			fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 1);
			break;
		case STATUE_TONGUE_UNROLLING_BY_FIRELIGHT:
			fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 2);
			break;
		case STATUE_TONGUE_OUT_BY_FIRELIGHT:
			fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 3);
			break;
		case STATUE_TONGUE_TO_THE_GROUND_BY_DAY:
			fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 6);
			fdActorGetById(ACTOR_STATUE_TONGUE)->y.fixed -= toFIXED(34);
			break;
		case STATUE_TONGUE_TO_THE_GROUND_BY_FIRELIGHT:
			fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 7);
			fdActorGetById(ACTOR_STATUE_TONGUE)->y.fixed -= toFIXED(34);
			break;
		}
	} else {
		fdMapGridCopyMetatileTilesIndicesRect(45, 38, 79, 16, 3, 3);
		fdMapGridCopyMetatileAttributes(17, 22, 1, 1, 16, 22);
		fdActorSetPos(ACTOR_STATUE_TONGUE, 0, 0);
	}
}

/* While Scoop is cast, uncovers the mark the script is scooping. */
void scKibomboUncoverMarkUnderLeader(void)
{
	FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;

	if(state->stage != 0) {
		if(syGameFlagGet(FLAG_SCOOPING_NORTH_MARK))
			scKibomboUncoverNorthMark();
		else if(syGameFlagGet(FLAG_SCOOPING_SOUTH_MARK))
			scKibomboUncoverSouthMark();
		else if(syGameFlagGet(FLAG_SCOOPING_WEST_MARK))
			scKibomboUncoverWestMark();
		else if(syGameFlagGet(FLAG_SCOOPING_EAST_MARK))
			scKibomboUncoverEastMark();
	}
}

/* Felix casts Scoop on the mark he stands on. */
void scKibomboCastScoop(void)
{
	fdPsynergyLoadResources(ABILITY_SCOOP, 1);
	syFieldSetTickEvent(scKibomboUncoverMarkUnderLeader, EVENT_PASS_TICK);
	fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, -1);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	syCloseEvent(scKibomboUncoverMarkUnderLeader);
	fdPsynergyEnd();
}

/* Stops the room's fire flicker event. */
void scKibomboStopFireFlicker(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KIBOMBO_CEREMONY)
		syCloseEvent(scKibomboFlickerVillageFires);
	else if(scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
		syCloseEvent(scKibomboFlickerCeremonyFires);
	else if(scene == SCENE_GABOMBA_STATUE_FACE)
		syCloseEvent(scKibomboFlickerStatueFires);
}

/* Felix walks off the village or the statue's grounds by one of their ways out
   on the ceremony night: the village's south edge to the world map, its north
   and north-west paths up to the statue's grounds, or the grounds' south and
   south-west paths back down. The torches stop flickering, the exit sound plays
   and the screen closes. */
void scKibomboLeaveRoom(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(syGameFlagGet(FLAG_KIBOMBO_FIRES_FLICKER))
		scKibomboStopFireFlicker();
	sySoundPlay(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(state->stepCue);
}

/* Felix walks into the Gabomba Statue's mouth on the statue's grounds, metatile
   (16, 19): the screen closes in on him and he goes inside, the torches going
   still as he leaves. */
void scKibomboLeaveRoomFading(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	scKibomboLeaveRoom();
}

/* Felix walks north into the door of one of the village's six houses: he lines
   up with it and steps into the dark doorway, the screen fades, and he stands
   inside the house. */
void scKibomboEnterDoor(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*leader;
	FIXED		 x;
	Sint32		 pixelX;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	x = leader->x.fixed;
	pixelX = FIXEDtoINT(x);
	fdActorSetMoveTarget(
		leader, toFIXED(8 - pixelX % 16) + x, leader->y.fixed, leader->z.fixed);
	fdActorWaitForMove(leader);
	leader->facing = FACING_NORTH;
	if(syGameFlagGet(FLAG_KIBOMBO_FIRES_FLICKER))
		scKibomboStopFireFlicker();
	fdActorSetMovementTypeById(
		MainRoutineStateWork.leaderActorId, LeaderWalkThroughDoorScript);
	sySynch(12);
	sySoundPlay(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(state->stepCue);
}

/* The leader walks to the top of the ladder at x, z, climbs down it and the
   scene leaves by the step's exit. */
void scKibomboClimbDownLadder(Sint32 x, Sint32 z)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	FIELD_STATE		   *state = RamHeapSlots->fieldState;
	ACTOR			   *leader = fdActorGetById(party->leaderActorId);

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	leader->motionFlags = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, x, z);
	fdActorFaceDirectionAndRefreshById(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	fdActorSetSpriteFlags(
		fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(leader, x, -12, z);
	fdActorWaitForMove(leader);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_CLIMB);
	sySoundPlay(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(state->stepCue);
}

/* Felix steps beside the ladder in the item shop's upper room, metatile (43,
   22) or (42, 23): he climbs down it only while he presses on toward it, Left
   or Up. */
void scKibomboStepOntoFirstLadder(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 x = FIELD_METATILE(leader->x.fixed);
	Sint32 z = FIELD_METATILE(leader->z.fixed);

	if(x == 42 && z == 23 && (PerWork.data & KEY_UP) == 0)
		return;
	if(x == 43 && z == 22 && (PerWork.data & KEY_LEFT) == 0)
		return;
	scKibomboClimbDownLadder(680, 352);
}

/* Defines a step cue by a ladder: Felix climbs down to (ladderX, ladderZ) only
   if he steps on from the west metatile (westX, sideZ) holding right, the east
   metatile (eastX, sideZ) holding left, or the south metatile (southX, southZ)
   holding up. */
#define DEFINE_KIBOMBO_STEP_WHEN_HELD( \
	name, sideZ, westX, eastX, southX, southZ, ladderX, ladderZ) \
	void name(void) \
	{ \
		ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId); \
		Sint32 x = FIELD_METATILE(leader->x.fixed); \
		Sint32 z = FIELD_METATILE(leader->z.fixed); \
\
		if(z == sideZ) { \
			if(x == westX && (PerWork.data & KEY_RIGHT) == 0) \
				return; \
			if(x == eastX && (PerWork.data & KEY_LEFT) == 0) \
				return; \
		} else if(x == southX && z == southZ && \
			(PerWork.data & KEY_UP) == 0) { \
			return; \
		} \
		scKibomboClimbDownLadder(ladderX, ladderZ); \
	}

/* As scKibomboStepOntoFirstLadder, for the ladder down from Akafubu's home to
   the Sanctum, metatiles (23, 38), (25, 38) and (24, 39): Felix may step onto
   it from the west, the east or the south. */
DEFINE_KIBOMBO_STEP_WHEN_HELD(
	KibomboStepOntoSecondLadder, 38, 23, 25, 24, 39, 392, 608)

/* As KibomboStepOntoSecondLadder, for the ladder in the upper room with two
   beds, metatiles (42, 54), (44, 54) and (43, 55), down to the room of beds
   below. */
DEFINE_KIBOMBO_STEP_WHEN_HELD(
	KibomboStepOntoThirdLadder, 54, 42, 44, 43, 55, 696, 864)

/* Felix walks south into the uncovered vent on the statue's grounds, metatile
   (23, 17): he climbs down its ladder into the Gabomba Statue. */
void scKibomboEnterVent(void)
{
	if(syGameFlagGet(FLAG_KIBOMBO_FIRES_FLICKER))
		scKibomboStopFireFlicker();
	scKibomboClimbDownLadder(376, 288);
}

/* During the ceremony the screen shows the Great Gabomba's face close up, its
   fires flickering, for about three seconds or until A, B or Start is held,
   then goes dark and returns to the grounds. */
void scKibomboGazeAtStatue(void)
{
	Uint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySetEvent(scKibomboFlickerStatueFires, EVENT_PASS_TICK);
	fdScriptWait(20);
	for(i = 0; i < 160; i++) {
		fdScriptWait(1);
		if(PerWork.data & (KEY_A | KEY_B | KEY_START))
			break;
	}
	syCloseEvent(scKibomboFlickerStatueFires);
	FieldStateWork->transitionFrame = 32;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B);
}

/* During the ceremony, as the Great Gabomba answers, the screen flashes white
   with a sound and its face is shown close up with its eyes shut, the fires on
   it flickering. It flashes again, and the face has its eyes half open; and
   again, and the eyes are wide open, staring. After a while the screen goes
   dark and the party is back on the grounds. Each view is a copy of the face in
   the map, at (248, 248), (584, 248) and (248, 536). */
void scKibomboStatueFlashes(void)
{
	sySoundPlay(SE_POWER_UP);
	fdPaletteFadeApplyTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	FieldStateWork->transitionFrame = 1;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldSetTickEvent(scKibomboFlickerStatueFires, EVENT_PASS_TICK);
	sySynch(60);
	syCloseEvent(scKibomboFlickerStatueFires);
	sySynch(1);

	sySoundPlay(SE_POWER_UP);
	fdPaletteFadeApplyTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdCameraMoveTo(584, CAMERA_KEEP, 248, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	syFieldSetTickEvent(scKibomboFlickerStatueFires, EVENT_PASS_TICK);
	sySynch(60);
	syCloseEvent(scKibomboFlickerStatueFires);
	sySynch(1);

	sySoundPlay(SE_POWER_UP);
	fdPaletteFadeSetTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdCameraMoveTo(248, CAMERA_KEEP, 536, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	sySetEvent(scKibomboFlickerStatueFires, EVENT_PASS_TICK);
	sySynch(180);
	syCloseEvent(scKibomboFlickerStatueFires);
	sySynch(1);

	FieldStateWork->transitionFrame = 1;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(10);
	syImportScriptSetExitRequest(
		GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A);
}

/* The sacred night begins: Akafubu calls on the village to pray for his
   success, and the party hurries to watch. */
void scKibomboCeremonyStarts(void)
{
	ACTOR *leader;

	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdScriptSetMessageId((Sint32)MsgAtLastThe);
	/* "At last, the sacred night has come!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 86, 455);
	fdActorSetPos(ACTOR_KRADEN, 86, 455);
	sySynch(1);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 72, 455);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(264, 16, 420, 1);
	fdCameraWaitForArrival();
	fdScriptWait(40);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(264, 32, 320, 1);
	fdCameraWaitForArrival();
	fdScriptWait(80);
	fdActorJump(ACTOR_AKAFUBU, 4, 40);
	/* "Tonight, Akafubu becomes the Kibombo's new witch doctor!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(264, 32, 400, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
	/* "All of you, pray in the depths of your hearts for my success!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	sySoundPlay(MUS_RITUALS);
	syGameFlagSet(FLAG_GABOMBA_CEREMONY_UNDER_WAY);
	sySoundPlay(SE_BOSS_ROAR);
	fdActorGetById(ACTOR_WARRIOR_A)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_B)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_C)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_D)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_E)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_F)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_G)->velocityY = toFIXED(4);
	sySynch(5);
	fdActorGetById(ACTOR_WARRIOR_H)->velocityY = toFIXED(4);
	sySynch(20);
	fdActorFaceDirectionById(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 20);
	scKibomboCrowdBows();
	fdScriptWait(40);
	fdCameraSetSpeed(0.2);
	fdCameraMoveTo(264, 0, 400, 1);
	fdCameraWaitForArrival();
	fdScriptWait(200);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(111, 32, 456, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	/* "It looks like the initiation ceremony is beginning." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_WEST, 20);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "I'm not sure what will happen, but we'd better hurry, Felix." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syGameFlagSet(FLAG_KIBOMBO_CEREMONY_BEGUN);
}

void scKibomboCarryBlackCrystal(void);

/* Akafubu presents the Black Orb to the Great Gabomba, and the statue notices
   its light. */
void scKibomboPresentOrbToGabomba(void)
{
	fdActorSetSpriteFlags(
		fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetSpriteColor(fdActorGetById(MainRoutineStateWork.leaderActorId),
		SPRITE_COLOR_HIDDEN);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldSetTickEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	fdScriptWait(40);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(264, CAMERA_KEEP, 388, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
	fdScriptSetMessageId((Sint32)MsgLetUsNow);
	/* "Let us now present our jewel to the Great Gabomba!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	sySoundPlay(MUS_JUPITER_LIGHTHOUSE);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CRYSTAL_MARKER), 0);
	fdActorGetById(ACTOR_CRYSTAL_MARKER)->motionFlags = 0;
	fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_CRYSTAL_MARKER),
		toFIXED(265), toFIXED(24), toFIXED(344));
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(264, CAMERA_KEEP, 340, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 0.2);
	fdActorWalkToAndWaitById(ACTOR_AKAFUBU, 264, 374);
	sySynch(1);
	syFieldSetTickEvent(scKibomboCarryBlackCrystal, EVENT_PASS_TICK);
	fdScriptWait(20);
	syCloseEvent(scKibomboFlickerCeremonyFires);
	scKibomboAkafubuCastsOnCrystal();
	syFieldSetTickEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	/* "O Great Gabomba! Please accept this, our humble gift!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	sySoundPlay(MUS_RITUALS);
	scKibomboEndOffering();
	fdScriptWait(160);
	/* "Please, Great Gabomba! Accept this jewel!" */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
	sySoundPlay(SE_STATUE_STIRS);
	scKibomboSetStatueMouth(1, 1);
	fdScriptWait(20);
	/* "Look! The Great Gabomba has noticed the light of the jewel!" */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 40);
	syCloseEvent(scKibomboFlickerCeremonyFires);
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_A);
}

/* The Great Gabomba stays silent, and Akafubu asks whether it is still not
   pleased. */
void scKibomboGabombaNotPleased(void)
{
	ACTOR *actor;

	fdActorSetSpriteFlags(
		fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetSpriteColor(fdActorGetById(MainRoutineStateWork.leaderActorId),
		SPRITE_COLOR_HIDDEN);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CRYSTAL_MARKER), 0);
	fdActorGetById(ACTOR_CRYSTAL_MARKER)->motionFlags = 0;
	fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_CRYSTAL_MARKER),
		toFIXED(265), toFIXED(24), toFIXED(344));
	scKibomboSetStatueMouth(1, 1);
	sySynch(1);
	if(KibomboBlackCrystal != 0) {
		KibomboBlackCrystal->y.fixed += toFIXED(32);
		sySynch(1);
		KibomboBlackCrystal->motionFlags = ACTOR_FLOATS;
	}
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySetEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	fdScriptWait(40);
	sySoundPlay(SE_RATTLE);
	scKibomboSetStatueMouth(0, 1);
	fdScriptWait(20);
	fdEffectSetAttached(
		ACTOR_AKAFUBU, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	fdScriptSetMessageId((Sint32)MsgYouAreStill);
	/* "You are still not pleased?" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	actor = KibomboBlackCrystal;
	if(actor != 0) {
		actor->motionFlags = 0;
		while(actor->y.fixed > toFIXED(36)) {
			fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
			KibomboBlackCrystal->y.fixed -= toFIXED(1) / 4;
			sySynch(1);
			actor = KibomboBlackCrystal;
		}
	}
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
	/* "No! This time, he seemed almost like he might..." */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	/* "We must continue! Drums! More drums!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	syGameFlagClear(FLAG_GABOMBA_MUSIC_SILENCED);
	sySoundPlay(MUS_RITUALS);
	fdScriptWait(80);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_B);
}

/* Akafubu feels the witch doctor's power fill his body. */
void scKibomboWitchDoctorFeelsPower(void)
{
	FIELD_STATE *state;
	Sint32		 effect;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetSpriteColor(fdActorGetById(MainRoutineStateWork.leaderActorId),
		SPRITE_COLOR_HIDDEN);
	fdActorSetSpriteFlags(
		fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	scKibomboCrowdBows();
	sySetEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	state = RamHeapSlots->fieldState;
	state->transition = effect =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdScriptSetMessageId((Sint32)MsgOhTheMagic);
	/* "Oh! The magic, the power of the witch doctor fills my body!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	/* "At the count of twenty, I shall present our jewel to the Great
	   Gabomba!" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
	/* "Make your hearts as one, and dance for the Great Gabomba!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	syCloseEvent(scKibomboFlickerCeremonyFires);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeSetTarget(0, 0);
	FieldStateWork->transition = effect;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syGameFlagSet(FLAG_GABOMBA_BACK_FROM_CEREMONY);
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/* The screen opens on the ceremony at the statue, with Felix hidden, the fires
   flickering and the Great Gabomba's mouth open, its eyes ringed with light;
   each step of the offering that follows sets the mouth as it needs. */
void scKibomboOpenCeremonyScene(void)
{
	FIELD_STATE *state;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdActorSetSpriteColor(fdActorGetById(MainRoutineStateWork.leaderActorId),
		SPRITE_COLOR_HIDDEN);
	fdActorSetSpriteFlags(
		fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	sySetEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	scKibomboSetStatueMouth(1, 1);
	if(KibomboBlackCrystal != 0) {
		KibomboBlackCrystal->y.fixed += toFIXED(32);
		sySynch(1);
		KibomboBlackCrystal->motionFlags = ACTOR_FLOATS;
	}
	sySynch(1);
	state = RamHeapSlots->fieldState;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
}

extern Sint32 KibomboCrowdDanceScript[];

/* The warriors leap up in fright; two cry out and all dance in a frenzy until
   Akafubu silences them, calms them and calls for more drums. It goes on from
   the caller's message: the jewel seen (scKibomboGabombaSeesOrb) or the gift
   answered (scKibomboGabombaResponds). */
void scKibomboCrowdPanics(void)
{
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(264, 0, 400, 1);
	fdCameraWaitForArrival();
	fdScriptWait(10);
	fdActorJump(ACTOR_WARRIOR_A, 6, 0);
	fdActorJump(ACTOR_WARRIOR_B, 6, 0);
	fdActorJump(ACTOR_WARRIOR_C, 6, 0);
	fdActorJump(ACTOR_WARRIOR_D, 6, 0);
	fdActorJump(ACTOR_WARRIOR_E, 6, 0);
	fdActorJump(ACTOR_WARRIOR_F, 6, 0);
	fdActorJump(ACTOR_WARRIOR_G, 6, 0);
	fdActorJump(ACTOR_WARRIOR_H, 6, 40);
	fdActorFaceDirectionAndWait(ACTOR_WARRIOR_C, FACING_SOUTH_SOUTHWEST);
	fdActorJump(ACTOR_WARRIOR_C, 2, 0);
	/* "Ahh! We have enraged the Great Gabomba!", or after the gift "Wahhh! The
	   Great Gabomba spews jets of flame!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_C, 0);
	fdActorFaceDirectionAndWait(ACTOR_WARRIOR_F, FACING_SOUTH_SOUTHEAST);
	fdActorJump(ACTOR_WARRIOR_F, 2, 0);
	/* "No! Great catastrophe shall befall the Kibombo!", or "It is the end of
	   Kibombo! Our village is doomed!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_F, 0);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_A, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_B, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_C, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_D, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_E, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_F, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_G, KibomboCrowdDanceScript);
	fdActorSetMovementTypeById(ACTOR_WARRIOR_H, KibomboCrowdDanceScript);
	fdScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	fdActorJump(ACTOR_AKAFUBU, 2, 10);
	fdActorJump(ACTOR_AKAFUBU, 4, 10);
	/* "Silence! SILENCE!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorStop(ACTOR_WARRIOR_A);
	fdActorStop(ACTOR_WARRIOR_B);
	fdActorStop(ACTOR_WARRIOR_C);
	fdActorStop(ACTOR_WARRIOR_D);
	fdActorStop(ACTOR_WARRIOR_E);
	fdActorStop(ACTOR_WARRIOR_F);
	fdActorStop(ACTOR_WARRIOR_G);
	fdActorStop(ACTOR_WARRIOR_H);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_WARRIOR_A, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_B, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_C, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_D, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_E, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_F, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_G, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_WARRIOR_H, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	/* "Do not be hasty! The Great Gabomba has not opened his eyes in anger!",
	   or "Do not be hasty, my people! The Great Gabomba is not angered by our
	   ceremony!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
	/* "Your prayers...have not appeased him... That is all!", or "He is angered
	   by the presence of wicked thoughts!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
	/* "Pray! You must pray more! Let the Great Gabomba hear our desires!", or
	   "Cast away your wicked thoughts! Dance! Dance for the Great Gabomba!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
	fdActorJump(ACTOR_AKAFUBU, 4, 10);
	/* "Drums! More drums!!!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
}

/* Still nothing from the statue: Akafubu asks whether it is still not pleased,
   and calls for more drums. */
void scKibomboGabombaStillNotPleased(void)
{
	ACTOR **slot;
	ACTOR  *actor;

	scKibomboOpenCeremonyScene();
	sySoundPlay(SE_RATTLE);
	scKibomboSetStatueMouth(0, 1);
	fdScriptWait(20);
	fdScriptSetMessageId((Sint32)MsgYouAreStillNot);
	/* "You are still not pleased?" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	actor = KibomboBlackCrystal;
	if(actor != 0) {
		actor->motionFlags = 0;
		if(actor->y.fixed > toFIXED(36)) {
			do {
				fdActorFaceDirectionById(
					ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
				slot = &KibomboBlackCrystal;
				actor = *slot;
				actor->y.fixed += toFIXED(-0.25);
				sySynch(1);
			} while((*slot)->y.fixed > toFIXED(36));
		}
	}
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
	/* "No! This time, he seemed almost like he might..." */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	/* "We must continue! Drums! More drums!" */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	syGameFlagSet(FLAG_GABOMBA_BACK_FROM_CEREMONY);
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/* Akafubu cries that the Great Gabomba has seen the jewel; its eyes swell for a
   moment, then shake, and the warriors panic. */
void scKibomboGabombaSeesOrb(void)
{
	scKibomboOpenCeremonyScene();
	sySoundPlay(SE_STATUE_STIRS);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_WHITE);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_RIGHT_EYE), SPRITE_COLOR_WHITE);
	sySynch(8);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_NORMAL);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_RIGHT_EYE), SPRITE_COLOR_NORMAL);
	sySynch(8);
	fdActorShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdScriptSetMessageId((Sint32)MsgLookLookThe);
	/* "Look! Look! The Great Gabomba has seen our jewel!" */
	fdScriptShowMessageAndWait(ACTOR_AKAFUBU, 0);
	sySoundPlay(SE_GABOMBA_JAWS_OPEN);
	fdActorSetMovementTypeById(ACTOR_STATUE_LEFT_EYE, KibomboEyesPopScript);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_STATUE_RIGHT_EYE, KibomboEyesPopScript);
	fdActorSetMovementTypeById(ACTOR_STATUE_LEFT_EYE, KibomboEyesShakeScript);
	fdActorSetMovementTypeById(ACTOR_STATUE_RIGHT_EYE, KibomboEyesShakeScript);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_DARK);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_RIGHT_EYE), SPRITE_COLOR_DARK);
	fdScriptWait(120);
	scKibomboCrowdPanics();
	syGameFlagSet(FLAG_GABOMBA_BACK_FROM_CEREMONY);
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/* Akafubu cries that the Great Gabomba has responded to the gift; its eyes
   flash, embers rise from its fire, and the warriors panic. */
void scKibomboGabombaResponds(void)
{
	scKibomboOpenCeremonyScene();
	fdScriptPlaySound(SE_STATUE_STIRS);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_WHITE);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_RIGHT_EYE), SPRITE_COLOR_WHITE);
	sySynch(8);
	fdActorGetById(ACTOR_STATUE_LEFT_EYE)->update =
		(OBJECT_UPDATE)scKibomboFlashSprite;
	fdActorGetById(ACTOR_STATUE_RIGHT_EYE)->update =
		(OBJECT_UPDATE)scKibomboFlashSprite;
	fdActorShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdScriptSetMessageId((Sint32)MsgLookTheGreat);
	/* "Look! The Great Gabomba has responded to our gift!" */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
	sySoundPlay(SE_STATUE_STIRS);
	scKibomboSetStatueTongue(1, STATUE_TONGUE_IN_MOUTH);
	fdScriptWait(20);
	sySoundPlay(SE_WAVE);
	sySetEvent(scKibomboRaiseEmbers, EVENT_PASS_TICK);
	fdScriptWait(120);
	scKibomboCrowdPanics();
	syGameFlagSet(FLAG_GABOMBA_BACK_FROM_CEREMONY);
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH);
}

/* The Great Gabomba responds to the gift: its tongue reaches for it and its
   mouth opens and shuts. */
void scKibomboGabombaRespondsToGift(void)
{
	ACTOR *actor;
	Uint32 i;

	actor = fdActorGetById(ACTOR_STATUE_TONGUE);
	scKibomboOpenCeremonyScene();
	fdActorSetSpritePriority(ACTOR_AKAFUBU, 1);
	fdScriptPlaySound(SE_STATUE_STIRS);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_WHITE);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_RIGHT_EYE), SPRITE_COLOR_WHITE);
	sySynch(8);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_LEFT_EYE), SPRITE_COLOR_NORMAL);
	fdActorSetSpriteColor(
		fdActorGetById(ACTOR_STATUE_RIGHT_EYE), SPRITE_COLOR_NORMAL);
	sySynch(8);
	fdActorShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdScriptSetMessageId((Sint32)MsgLookTheGreatGabomba);
	/* "Look! The Great Gabomba has responded to our gift!" */
	fdScriptShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
	fdScriptPlaySound(SE_STATUE_STIRS);
	scKibomboSetStatueTongue(1, STATUE_TONGUE_IN_MOUTH);
	fdScriptWait(20);
	sySoundPlay(SE_CART_ROLL);
	actor->scaleY = toFIXED(1) / 10;
	fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_STATUE_TONGUE),
		toFIXED(264), toFIXED(24), toFIXED(308));
	sySynch(1);
	fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 3);
	for(i = 0; i < 64; i++) {
		actor->scaleY += toFIXED(1) / 64;
		actor->y.fixed += toFIXED(1) / 8;
		sySynch(1);
	}
	sySoundPlay(SE_STOP_EXIT_SE);
	if(KibomboBlackCrystal != 0) {
		ACTOR *gift = KibomboBlackCrystal;

		gift->motionFlags = 0;
		if(gift->y.fixed > toFIXED(36)) {
			do {
				KibomboBlackCrystal->y.fixed -= toFIXED(1) / 2;
				sySynch(1);
			} while(KibomboBlackCrystal->y.fixed > toFIXED(36));
		}
	}
	sySoundPlay(SE_CRUNCH);
	if(KibomboBlackCrystal != 0) {
		KibomboBlackCrystal->motionFlags = 0;
		for(i = 0; i < 4; i++) {
			KibomboBlackCrystal->y.fixed += toFIXED(6);
			actor->scaleY -= toFIXED(1) / 8;
			actor->y.fixed -= toFIXED(1) / 2;
			sySynch(1);
		}
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
		for(i = 0; i < 4; i++) {
			actor->scaleY -= toFIXED(1) / 8;
			actor->y.fixed -= toFIXED(1) / 2;
			sySynch(1);
		}
	}
	fdActorSetPos(ACTOR_STATUE_TONGUE, 0, 0);
	scKibomboSetStatueTongue(0, STATUE_TONGUE_IN_MOUTH);
	scKibomboSetStatueMouth(1, 1);
	fdScriptWait(10);
	scKibomboSetStatueMouth(0, 1);
	fdScriptWait(40);
	fdScriptPlaySound(SE_STATUE_STIRS);
	scKibomboSetStatueMouth(1, 1);
	scKibomboSetStatueTongue(1, STATUE_TONGUE_IN_MOUTH);
	sySoundPlay(SE_WATER);
	fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_STATUE_TONGUE),
		toFIXED(264), toFIXED(24), toFIXED(308));
	sySynch(1);
	for(i = 0; i < 8; i++) {
		actor->scaleY += toFIXED(1) / 8;
		actor->y.fixed += toFIXED(1) / 2;
		sySynch(1);
	}
	fdEffectSetAttached(
		ACTOR_AKAFUBU, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	/* "Did you see it, people?" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	scFieldActorShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "At last! The Great Gabomba accepts me!" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
	/* "I shall answer his call! I shall enter the Great Gabomba! Wait
	   for..." */
	fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	fdActorSetSpeed(ACTOR_AKAFUBU, 0.8);
	fdActorWalkToAndWait(ACTOR_AKAFUBU, 264, 320);
	fdActorSetPos(ACTOR_AKAFUBU, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syGameFlagSet(FLAG_GABOMBA_RESPONDED);
	syImportScriptSetExitRequest(
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_NORTH);
}

/* After the ceremony, at Akafubu's home: Kraden, Akafubu and his father talk it
   over, and Kraden walks out with the party. */
void scKibomboCeremonyEnds(void)
{
	FIELD_STATE *state;
	ACTOR		*leader;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraMoveTo(384, CAMERA_KEEP, 601, 0);
	sySynch(1);
	fdActorSetHorizontalPosAndAngleWithTerrain(
		MainRoutineStateWork.leaderActorId, toFIXED(370), toFIXED(636),
		FACING_NORTH);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteFadeSetSource(0, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_SEPIA, 0);
	FieldStateWork->transitionFrame = 60;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_FATHER_OF_AKAFUBU, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU_AT_HOME, ANIM_NOD);
	fdActorFaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 20);
	fdActorGetById(ACTOR_KRADEN_AT_HOME)->velocityY = toFIXED(3);
	fdScriptWait(40);
	fdActorSetAnimationById(ACTOR_FATHER_OF_AKAFUBU, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU_AT_HOME, ANIM_NOD);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(40);
	sySynch(60);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
	fdScriptSetMessageId((Sint32)MsgRightWellI);
	/* "Right... Well, I guess that's that!" */
	fdScriptShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
	fdActorSetAnimationAndWait(ACTOR_FATHER_OF_AKAFUBU, ANIM_NOD);
	/* "You are virtuous men, coming here to deliver this message. My son..." */
	fdScriptShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN_AT_HOME, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTH, 10);
	fdActorSetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorJump(ACTOR_AKAFUBU_AT_HOME, 4, 10);
	/* "What do we have to be thankful of?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_FATHER_OF_AKAFUBU, FACING_EAST);
	fdActorShowEmoteById(
		ACTOR_AKAFUBU_AT_HOME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "If they had just stopped me from leaving the Great Gabomba, this..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
	fdActorShowEmoteById(
		ACTOR_FATHER_OF_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "What!? If you had been more interested in your duty, you would
	   not..." */
	fdScriptShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU_AT_HOME, FACING_WEST);
	fdActorSetAnimationById(ACTOR_AKAFUBU_AT_HOME, ANIM_SHAKE_HEAD);
	/* "But, Father! These people are trying to steal the magic of our..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU_AT_HOME, FACING_SOUTH_SOUTHWEST);
	/* "Mine, I tell you! That power belongs to me!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
	fdActorSetAnimationById(ACTOR_AKAFUBU_AT_HOME, ANIM_SHAKE_HEAD);
	/* "I'm going to be the one to get the Great Gabomba's magic! You'll
	   see!" */
	fdScriptShowMessageAndWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
	fdActorFaceDirection(ACTOR_FATHER_OF_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
	fdActorSetSpeed(ACTOR_AKAFUBU_AT_HOME, 1.6);
	fdActorWalkToAndWait(ACTOR_AKAFUBU_AT_HOME, 364, 576);
	fdActorFaceDirection(ACTOR_AKAFUBU_AT_HOME, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmoteById(
		ACTOR_FATHER_OF_AKAFUBU, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "My son, you have much to learn. He may look like a man, but
	   before..." */
	fdScriptShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(
		ACTOR_FATHER_OF_AKAFUBU, FACING_SOUTH_SOUTHEAST);
	/* "See how he behaves when he gets excited?" */
	fdScriptShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
	fdActorSetAnimationById(ACTOR_FATHER_OF_AKAFUBU, ANIM_SHAKE_HEAD);
	/* "No matter what I say, he won't listen to me right now." */
	fdScriptShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
	/* "I think that he has had enough for today..." */
	fdScriptShowMessageAndWait(ACTOR_FATHER_OF_AKAFUBU, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
	/* "I see your point." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KRADEN_AT_HOME, FACING_EAST);
	/* "Well! I suppose we should be leaving then." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
	fdActorFaceDirectionAndRefreshById(
		MainRoutineStateWork.leaderActorId, FACING_WEST);
	fdActorSetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
	/* "After all, our journey has only just begun. We cannot remain idle..." */
	fdScriptShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetSpeed(ACTOR_KRADEN_AT_HOME, 0.8);
	fdActorSetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestinationById(ACTOR_KRADEN_AT_HOME,
			leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_KRADEN_AT_HOME);
	fdActorSetPos(ACTOR_KRADEN_AT_HOME, 0, 0);
	state = RamHeapSlots->fieldState;
	state->transitionFrame = 16;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	syGameFlagSet(FLAG_GABOMBA_CEREMONY_ENDED);
	fdScriptEnd();
}

/* Sets up whichever of the overlay's six rooms the party enters. */
Sint32 scInit(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_KIBOMBO)
		scKibomboInitVillageByDay();
	else if(scene == SCENE_KIBOMBO_CEREMONY)
		scKibomboInitCeremony();
	else if(scene == SCENE_KIBOMBO_STATUE_GROUNDS)
		scKibomboInitStatueGroundsByDay();
	else if(scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
		scKibomboInitStatueGroundsCeremony();
	else if(scene == SCENE_GABOMBA_STATUE_FACE)
		scKibomboInitStatueFace();
	else if(scene == SCENE_KIBOMBO_HOUSES)
		scKibomboInitHouses();
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* The village: the frozen puddles, the rope and the roofs. */
void scKibomboInitVillage(void)
{
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANDLESTICK_A), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANDLESTICK_B), 0);
	fdEffectStartBlankOverlay(0);
	fdFlagBlockPlaceAll(KibomboFrozenPuddle);
	fdActorSetSpritePriority(ACTOR_PUDDLE, 3);
	fdRopeInit();
	fdRopePlace(0, FLAG_KIBOMBO_VILLAGE_ROPE_TIED, ACTOR_ROPE_POST_A,
		ACTOR_ROPE_POST_B);
	fdActorGetById(ACTOR_GATE_GUARD_A)->collisionFlags |=
		ACTOR_BLOCKED_BY_SOLID;
	fdActorGetById(ACTOR_GATE_GUARD_B)->collisionFlags |=
		ACTOR_BLOCKED_BY_SOLID;
}

/* The village by day: its signal fire smoking and the row of flames lit. */
void scKibomboInitVillageByDay(void)
{
	fdActorSetPos(ACTOR_FLAME, 88, 314);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_FLAME), 0);
	fdActorGetById(ACTOR_FLAME)->scaleY = toFIXED(1) / 2;
	scKibomboInitVillage();
	syGameFlagClear(FLAG_KIBOMBO_AT_CEREMONY);
	fdActorDisableById(ACTOR_SIGNAL_FIRE);
	fdActorSetAnimationById(ACTOR_SIGNAL_FIRE, 2);
	sySetEvent(scKibomboRaiseSmoke, EVENT_PASS_TICK);
	scKibomboLightSignalFire();
}

/* The village on the ceremony night: the signal fire's flame is raised, and
   when the party starts again before the healer after a defeat with Piers
   along, he is healed and stays behind. The fires' colours are kept for their
   flicker, the candlesticks lit and, once Akafubu has offered his help, the
   ceremony's actors moved; the screen fades in with the village fires
   flickering. */
void scKibomboInitCeremony(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_FLAME);
	UNIT  *unit;
	Sint32 done;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	done = syGameFlagGet(FLAG_KEEP_PARTY_POSITION);
	if(!done) {
		actor->motionFlags = done;
		actor->y.fixed = toFIXED(42);
		actor->z.fixed -= toFIXED(42);
		fdActorSetSpriteFlags(fdActorGetById(ACTOR_FLAME), 0);
		fdActorSetSpritePriority(ACTOR_FLAME, 1);
		if(MainRoutineStateWork.entrance == ENTRANCE_CEREMONY_AFTER_DEFEAT &&
			syGameFlagGet(FLAG_IN_PARTY(ACTOR_PIERS))) {
			unit = syUnitGet(PIERS);
			unit->hp = unit->hpMax;
			unit->pp = unit->ppMax;
			UNIT_UPDATE_GAUGES(unit);
			unit->poison = 0;
			unit->haunt = 0;
			syGameFlagSet(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
			syGameFlagClear(FLAG_KIBOMBO_PIERS_WITH_PARTY);
			fdPartyRemoveMemberRestored(PIERS);
		}
	}
	syGameFlagSet(FLAG_KIBOMBO_FIRES_FLICKER);
	KibomboFireColors[0] = *(Uint16 *)&BG_PLTT[FIRE_COLOR_FIRST];
	KibomboFireColors[1] = *(Uint16 *)&BG_PLTT[VILLAGE_FIRE_COLOR_SECOND];
	KibomboFireColors[2] = *(Uint16 *)&BG_PLTT[VILLAGE_FIRE_COLOR_THIRD];
	KibomboFireColors[3] = *(Uint16 *)&BG_PLTT[VILLAGE_FIRE_COLOR_FOURTH];
	scKibomboInitVillage();
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_LIT_CANDLESTICK_A), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_LIT_CANDLESTICK_B), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_LIT_CANDLESTICK_C), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_LIT_CANDLESTICK_D), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_LIT_CANDLESTICK_E), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_LIT_CANDLESTICK_F), 0);
	syGameFlagSet(FLAG_KIBOMBO_DOORS_SHUT);
	actor = fdActorGetById(ACTOR_DOOR_BLOCK);
	actor->motionFlags = 0;
	actor->y.fixed = 0;
	scActorHide(ACTOR_DOOR_BLOCK);
	fdActorSetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 3);
	if(syGameFlagGet(FLAG_AKAFUBU_OFFERED_HELP)) {
		fdActorSetPos(ACTOR_WOMAN_NO_PUSHING, 0, 0);
		fdActorSetPos(ACTOR_AFRAID_MAN, 0, 0);
		fdActorSetPos(ACTOR_MAN_WHO_KNEW_OEIA, 0, 0);
		fdActorSetPos(ACTOR_GIRL_WHO_HOPES, 302, 172);
	}
	syGameFlagSet(FLAG_KIBOMBO_AT_CEREMONY);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	{
		Sint32 priority = EVENT_PASS_TICK;

		sySetEvent(scKibomboFlickerVillageFires, priority);
	}
	fdScriptEnd();
}

void scKibomboCreateBlackCrystal(void);

/* The grounds before the statue: the Black Crystal, the marks uncovered, the
   vent and the pillar Moved for Piers. */
void scKibomboInitStatueGrounds(void)
{
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANDLESTICK_A), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_CANDLESTICK_B), 0);
	if(!syGameFlagGet(FLAG_GABOMBA_RESPONDED))
		scKibomboCreateBlackCrystal();
	scKibomboHideGroundsBlocks();
	if(!syGameFlagGet(FLAG_SAW_THE_GROUNDS_CEREMONY))
		syGameFlagSet(FLAG_KIBOMBO_PUDDLE_FROZEN);
	fdFlagBlockPlaceAll(KibomboFrozenPuddle);
	fdActorSetSpritePriority(ACTOR_PUDDLE, 2);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_GROUNDS_PILLAR), 0);
	fdActorGetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 0;
	if(syGameFlagGet(FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS)) {
		scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_GROUNDS_PILLAR),
			toFIXED(104), 0, toFIXED(296));
		fdMapGridCopyMetatileAttributes(5, 18, 1, 1, 6, 18);
		sySynch(10);
	}
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(syGameFlagGet(FLAG_KIBOMBO_VENT_UNCOVERED))
		scKibomboUncoverVent();
	if(syGameFlagGet(FLAG_NORTH_MARK_UNCOVERED))
		scKibomboUncoverNorthMark();
	if(syGameFlagGet(FLAG_SOUTH_MARK_UNCOVERED))
		scKibomboUncoverSouthMark();
	if(syGameFlagGet(FLAG_WEST_MARK_UNCOVERED))
		scKibomboUncoverWestMark();
	if(syGameFlagGet(FLAG_EAST_MARK_UNCOVERED))
		scKibomboUncoverEastMark();
	fdActorDisableById(ACTOR_STATUE_LEFT_EYE);
	fdActorDisableById(ACTOR_STATUE_RIGHT_EYE);
	fdActorDisableById(ACTOR_STATUE_TONGUE);
	fdActorSetSpritePriority(ACTOR_STATUE_LEFT_EYE, 1);
	fdActorSetSpritePriority(ACTOR_STATUE_RIGHT_EYE, 1);
	fdActorSetSpritePriority(ACTOR_STATUE_TONGUE, 1);
}

/* Entrances of the grounds no exit leads to and no map lists, each showing the
   statue at a stage of the ceremony: its mouth shut, then open with its eyes
   lit, its tongue out, and its tongue reaching for the gift. */
enum {
	ENTRANCE_STATUE_REACHING = 96,
	ENTRANCE_STATUE_TONGUE_OUT = 97,
	ENTRANCE_STATUE_OPEN = 98,
	ENTRANCE_STATUE_SHUT = 99
};

/* The grounds by day, with the statue's mouth and tongue as the ceremony left
   them. */
void scKibomboInitStatueGroundsByDay(void)
{
	Sint16 entrance;

	scKibomboInitStatueGrounds();
	if(syGameFlagGet(FLAG_GABOMBA_RESPONDED)) {
		scKibomboSetStatueMouth(1, 0);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_TO_THE_GROUND_BY_DAY);
	}
	sySynch(1);
	entrance = MainRoutineStateWork.entrance;
	if(entrance == ENTRANCE_STATUE_SHUT) {
		scKibomboSetStatueMouth(0, 0);
		scKibomboSetStatueTongue(0, STATUE_TONGUE_IN_MOUTH);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
	} else if(entrance == ENTRANCE_STATUE_OPEN) {
		scKibomboSetStatueMouth(1, 0);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_IN_MOUTH);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
	} else if(entrance == ENTRANCE_STATUE_TONGUE_OUT) {
		scKibomboSetStatueMouth(1, 0);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_OUT_BY_DAY);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
	} else if(entrance == ENTRANCE_STATUE_REACHING) {
		scKibomboSetStatueMouth(1, 1);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_OUT_BY_FIRELIGHT);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
		fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 4);
	}
}

/* The grounds on the ceremony night: the fires' colours kept, the mouth as the
   statue has answered, and the entrances from the Gabomba Statue's rooms
   playing each step of the offering. */
void scKibomboInitStatueGroundsCeremony(void)
{
	Uint32 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	syGameFlagSet(FLAG_SAW_THE_GROUNDS_CEREMONY);
	syGameFlagSet(FLAG_KIBOMBO_FIRES_FLICKER);
	KibomboFireColors[0] = *(Uint16 *)&BG_PLTT[FIRE_COLOR_FIRST];
	KibomboFireColors[1] = *(Uint16 *)&BG_PLTT[GROUNDS_FIRE_COLOR_SECOND];
	KibomboFireColors[2] = *(Uint16 *)&BG_PLTT[GROUNDS_FIRE_COLOR_THIRD];
	KibomboFireColors[3] = *(Uint16 *)&BG_PLTT[GROUNDS_FIRE_COLOR_FOURTH];
	scKibomboInitStatueGrounds();
	if(syGameFlagGet(FLAG_GABOMBA_RESPONDED)) {
		scKibomboSetStatueMouth(1, 1);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_OUT_BY_FIRELIGHT);
	}
	sySynch(1);
	switch(MainRoutineStateWork.entrance) {
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_A:
		scKibomboPresentOrbToGabomba();
		return;
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_B:
		scKibomboGabombaNotPleased();
		return;
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_SOUTH:
		scKibomboWitchDoctorFeelsPower();
		return;
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_NORTH:
		scKibomboGabombaStillNotPleased();
		return;
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_D:
		scKibomboGabombaSeesOrb();
		return;
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_E:
		scKibomboGabombaResponds();
		return;
	case KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_FACE_EAST_A:
		scKibomboGabombaRespondsToGift();
		return;
	}
	fdActorSetSpritePriority(ACTOR_AKAFUBU, 1);
	fdActorSetSpritePriority(ACTOR_PIERS, 2);
	fdActorSetSpritePriority(ACTOR_KRADEN, 2);
	fdActorSetSpritePriority(ACTOR_JENNA, 2);
	fdActorSetSpritePriority(ACTOR_SHEBA, 2);
	if(syGameFlagGet(FLAG_KIBOMBO_PIERS_WITH_PARTY))
		fdActorSetPos(ACTOR_PIERS, 0, 0);
	else if(syGameFlagGet(FLAG_KIBOMBO_PIERS_STAYED_BEHIND)) {
		fdActorSetPosAndFacing(
			ACTOR_PIERS, toFIXED(304), toFIXED(88), FACING_SOUTHEAST);
	}
	if(syGameFlagGet(FLAG_AKAFUBU_OFFERED_HELP)) {
		scKibomboSetStatueTongue(1, STATUE_TONGUE_TO_THE_GROUND_BY_FIRELIGHT);
		fdActorSetPos(ACTOR_AKAFUBU, 0, 0);
	}
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySetEvent(scKibomboFlickerCeremonyFires, EVENT_PASS_TICK);
	for(i = 0; i <= ACTOR_WARRIOR_H - ACTOR_WARRIOR_A; i++) {
		Uint8 *field = &fdActorGetById(i + ACTOR_WARRIOR_A)->retainSpriteCache;
		Sint32 value = 2;

		*field = value;
	}
	if(syGameFlagGet(FLAG_KIBOMBO_CEREMONY_BEGUN)) {
		if(!syGameFlagGet(FLAG_AKAFUBU_OFFERED_HELP))
			scKibomboCrowdBows();
	} else if(MainRoutineStateWork.entrance ==
		KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_WEST_B) {
		scKibomboCeremonyStarts();
	}
	fdScriptEnd();
	if(MainRoutineStateWork.entrance == ENTRANCE_STATUE_SHUT) {
		scKibomboSetStatueMouth(0, 0);
		scKibomboSetStatueTongue(0, STATUE_TONGUE_IN_MOUTH);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
	} else if(MainRoutineStateWork.entrance == ENTRANCE_STATUE_OPEN) {
		scKibomboSetStatueMouth(1, 1);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_IN_MOUTH);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
	} else if(MainRoutineStateWork.entrance == ENTRANCE_STATUE_TONGUE_OUT) {
		scKibomboSetStatueMouth(1, 1);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_OUT_BY_FIRELIGHT);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
	} else if(MainRoutineStateWork.entrance == ENTRANCE_STATUE_REACHING) {
		scKibomboSetStatueMouth(1, 1);
		scKibomboSetStatueTongue(1, STATUE_TONGUE_OUT_BY_FIRELIGHT);
		fdActorSetPosAndResetMotion(KibomboBlackCrystal, 0, 0, 0);
		fdActorSetAnimationById(ACTOR_STATUE_TONGUE, 5);
	}
}

/* Before the statue's face: brought here from the grounds as the Great Gabomba
   answers, its three flashes; brought here for the close look during the
   ceremony, the gaze into its fires; otherwise its fires flicker. */
void scKibomboInitStatueFace(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	syGameFlagSet(FLAG_KIBOMBO_FIRES_FLICKER);
	KibomboFireColors[0] = *(Uint16 *)&BG_PLTT[STATUE_FIRE_COLOR_FIRST];
	KibomboFireColors[1] = *(Uint16 *)&BG_PLTT[STATUE_FIRE_COLOR_SECOND];
	KibomboFireColors[2] = *(Uint16 *)&BG_PLTT[STATUE_FIRE_COLOR_THIRD];
	if(MainRoutineStateWork.entrance ==
		GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A) {
		fdActorSetSpriteFlags(
			fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
		fdActorSetSpriteColor(
			fdActorGetById(MainRoutineStateWork.leaderActorId),
			SPRITE_COLOR_HIDDEN);
		scKibomboStatueFlashes();
	} else if(MainRoutineStateWork.entrance ==
		GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B) {
		fdActorSetSpriteFlags(
			fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
		fdActorSetSpriteColor(
			fdActorGetById(MainRoutineStateWork.leaderActorId),
			SPRITE_COLOR_HIDDEN);
		scKibomboGazeAtStatue();
	} else {
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		sySetEvent(scKibomboFlickerStatueFires, EVENT_PASS_TICK);
		fdScriptEnd();
	}
}

/* The houses' entrance at the foot of the ladder down from Akafubu's home, in
   the Sanctum. */
enum { ENTRANCE_HOUSES_FROM_AKAFUBUS_LADDER = 14 };

/* The houses: Akafubu at home after the ceremony, or the ceremony's end played,
   and the man with the aching back lying where he fell. */
void scKibomboInitHouses(void)
{
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	scKibomboHideInnBlocks();
	if(MainRoutineStateWork.entrance ==
		KIBOMBO_HOUSES_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR) {
		if(syGameFlagGet(FLAG_GABOMBA_CEREMONY_ENDED)) {
			fdActorSetPos(ACTOR_KRADEN_AT_HOME, 0, 0);
			if(syGameFlagGet(FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU)) {
				fdActorSetPosAndFacing(ACTOR_AKAFUBU_AT_HOME, toFIXED(434),
					toFIXED(634), FACING_NORTH_NORTHWEST);
			} else {
				fdActorSetPosAndFacing(ACTOR_AKAFUBU_AT_HOME, toFIXED(364),
					toFIXED(576), FACING_NORTH_NORTHWEST);
			}
		} else {
			scKibomboCeremonyEnds();
		}
	}
#if ACHING_BACK_MAN_STAYS_PUT
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		actor = fdActorGetById(ACTOR_MAN_WITH_ACHING_BACK);
		actor->x.fixed -= toFIXED(14);
		actor->z.fixed -= toFIXED(8);
	}
#endif
	fdActorFallOverById(ACTOR_MAN_WITH_ACHING_BACK, 0);
	actor = fdActorGetById(ACTOR_MAN_WITH_ACHING_BACK);
	((FIELD_SPR *)actor->sprite)->rotation = 0;
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	actor->x.fixed += toFIXED(14);
	actor->z.fixed += toFIXED(8);
	fdActorSetAnimationById(ACTOR_MAN_WITH_ACHING_BACK, 6);
	if(MainRoutineStateWork.entrance == ENTRANCE_HOUSES_FROM_AKAFUBUS_LADDER)
		syGameFlagSet(FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU);
}

/* Felix walks west off the ledge by the inn in the south east of the village,
   metatile (25, 27), and jumps across onto the top of the nearest painted pole;
   standing on top of the pole at metatile (19, 8), among the poles in the north
   of the village, and walking north, he jumps onto the top of the next pole
   north. */
void scKibomboJumpForward(void)
{
	fdTryJumpForward();
}

void scKibomboAskAboutFewerWarriors(void);
void scKibomboAskAboutRevenge(void);
void scKibomboTalkPathToPeace(void);
void scKibomboAskGoingWest(void);
void scKibomboOpenWeaponShop(void);
void scKibomboOpenArmorShop(void);
void scKibomboOpenItemShop(void);
void scKibomboOpenInn(void);
void scKibomboOpenSanctum(void);
void scKibomboTalkForeignCeremony(void);
void scKibomboTalkAchingBack(void);
void scKibomboReadMindNeverAgain(void);
void scKibomboFreezePuddle(void);
void scKibomboMovePillarForPiers(void);
void scKibomboMeetPiers(void);
void scKibomboPiersThanksParty(void);
void scKibomboPiersRefusesToLeave(void);
void scKibomboDeadEndByTheStatue(void);
void scKibomboOverlayFollowsLeader(void);
void scKibomboOverlayStopsFollowing(void);

/* clang-format off */
Uint32 KibomboEmberScript[] = {
	SC_WAIT(28),
	SC_SET(SW_HOOK, 0),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

#include "shared/leader_walk_through_door_script.c"

Uint32 KibomboKradenMeetsPiersScript[] = {                                                               
	  SC_ANIMATION(2),   SC_MOVE(98, 0, 408),                         
	  SC_SYNC,     SC_ANIMATION(1),                                                  
	  SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
	  SC_WAIT(1), SC_END,                                                     
  };

Uint32 KibomboJennaMeetsPiersScript[] = {                                                               
	  SC_ANIMATION(2),   SC_MOVE(80, 0, 376), SC_SYNC,     SC_ANIMATION(1), 
	  SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

Sint32 KibomboShebaMeetsPiersScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(68, 0, 392),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(1),
	SC_END,
	/* Unused: past the end, an actor grows as it spins for ten ticks, then */
	/* shrinks away for 24. */
	SC_ADD(SW_SCALE_X, toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 32),
	SC_CALL(scKibomboSpinSprite),
	SC_WAIT(1),
	SC_LOOP(10, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_CALL(scKibomboSpinSprite),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_RELEASE,
};

Uint32 KibomboPartyGathersScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(88, 0, 392),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Uint32 KibomboLeaderStepsUpScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(72, 0, 296),
	SC_SYNC,
	SC_MOVE(40, 0, 340),
	SC_SYNC,
	SC_MOVE(40, 0, 356),
	SC_SYNC,
	SC_MOVE(56, 0, 372),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

Uint32 KibomboPiersStepsUpScript[] =                                                                 
	  {                                                                        
		  SC_ANIMATION(2), SC_MOVE(88, 0, 372), SC_SYNC, SC_ANIMATION(1),   
		  SC_END,                                                              
  };

Uint32 KibomboKradenJoinsPiersScript[] = {                                                               
	  SC_ANIMATION(2),   SC_MOVE(48, 0, 384), SC_SYNC,     SC_ANIMATION(1), 
	  SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

Uint32 KibomboJennaJoinsPiersScript[] = {                                                               
	  SC_ANIMATION(2),   SC_MOVE(48, 0, 360), SC_SYNC,     SC_ANIMATION(1), 
	  SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

Uint32 KibomboShebaJoinsPiersScript[] = {                                                               
	  SC_ANIMATION(2),   SC_MOVE(36, 0, 372), SC_SYNC,     SC_ANIMATION(1), 
	  SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

Uint32 KibomboPartyFollowsScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(56, 0, 372),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Uint32 KibomboJennaAtDeadEndScript[] =                                                                 
	  {                                                                        
		  SC_ANIMATION(2),   SC_MOVE(298, 0, 200),                     
		  SC_SYNC,     SC_ANIMATION(1),                                              
		  SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
		  SC_WAIT(1), SC_END,                                                 
  };

Uint32 KibomboShebaAtDeadEndScript[] =                                                                 
	  {                                                                        
		  SC_ANIMATION(2),   SC_MOVE(308, 0, 188),                     
		  SC_SYNC,     SC_ANIMATION(1),                                              
		  SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
		  SC_WAIT(1), SC_END,                                                 
  };

Uint32 KibomboKradenAtDeadEndScript[] =                                                                 
	  {                                                                        
		  SC_ANIMATION(2),   SC_MOVE(298, 0, 176),                     
		  SC_SYNC,     SC_ANIMATION(1),                                              
		  SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
		  SC_WAIT(1), SC_END,                                                 
  };

Uint32 KibomboPiersAtDeadEndScript[] =                                                                 
	  {                                                                        
		  SC_ANIMATION(2), SC_MOVE(222, 0, 184), SC_SYNC, SC_ANIMATION(1),   
		  SC_END,                                                              
  };

Uint32 KibomboPartyLeavesDeadEndScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(284, 0, 188),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 KibomboEyesShakeScript[] = {
	SC_CALL((Sint32)scKibomboKeepShakeCenter),
	SC_CALL((Sint32)scKibomboShakeInPlace),
};

Sint32 KibomboEyesPopScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_ADD(SW_Y, toFIXED(-2)),
	SC_WAIT(20),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_ADD(SW_Y, toFIXED(2)),
	SC_WAIT(1),
	SC_END,
};

Sint32 KibomboCrowdDanceScript[] = {
	SC_SET(SW_SCENE_COUNTER, 0),
	SC_CALL(scKibomboFaceRandomWay),
	SC_SPEED(0.4),
	SC_ACCELERATION(0.4),
	SC_LABEL(1),
	SC_ANIMATION(2),
	SC_MOVE(230, 0, 486),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(40),
	SC_ANIMATION(2),
	SC_MOVE(256, 0, 486),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
	/* Unused: past the endless loop, the same dance further east. */
	SC_SPEED(0.4),
	SC_ACCELERATION(0.4),
	SC_LABEL(1),
	SC_ANIMATION(2),
	SC_MOVE(298, 0, 486),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(40),
	SC_ANIMATION(2),
	SC_MOVE(272, 0, 486),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The puddle Frost freezes into a pillar, and the flag set while it stands. */
Sint16 KibomboFrozenPuddle[] = {
	ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN, -1, 0,
};

#include "shared/scene_entrances_default.c"

SCENE_REGION KibomboHousesExitRegions[] = {
	{400, 40, 368, 416, 56, 384, CONDITION_ALWAYS, 14},
	{112, 40, 624, 128, 56, 640, CONDITION_ALWAYS, 15},
	{384, 40, 880, 400, 56, 896, CONDITION_ALWAYS, 16},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(Kibombo),
	SCENE_EXIT(KIBOMBO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_EAST, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_KIBOMBO_EAST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_NORTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHWEST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_NORTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHEAST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_EAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_EAST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_WEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_WEST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHWEST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHEAST),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_A, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_KIBOMBO_WEST_A),
	SCENE_EXIT(KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_B, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_KIBOMBO_WEST_B),
	SCENE_EXITS(KibomboCeremony),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KIBOMBO),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_EAST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHWEST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHEAST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_EAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_EAST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_WEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_WEST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHWEST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHWEST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHEAST, KibomboHouses, KIBOMBO_HOUSES_FROM_KIBOMBO_SOUTHEAST),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_A, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_WEST_A),
	SCENE_EXIT(KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_KIBOMBO_CEREMONY_WEST_B),
	SCENE_EXITS(KibomboStatueGrounds),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_EAST, Kibombo, KIBOMBO_FROM_KIBOMBO_STATUE_GROUNDS_EAST),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_A, Kibombo, KIBOMBO_FROM_KIBOMBO_STATUE_GROUNDS_WEST_A),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_B, Kibombo, KIBOMBO_FROM_KIBOMBO_STATUE_GROUNDS_WEST_B),
	SCENE_EXITS(KibomboStatueGroundsCeremony),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_EAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_A, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_A),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_SOUTH, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_B),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_A, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_A),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_NORTH_B, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_A),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_ARROWS_SOUTH, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST),
	SCENE_EXIT(KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_NORTH, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH),
	SCENE_EXITS(KibomboHouses),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_NORTHWEST, FLAG_KIBOMBO_AT_CEREMONY),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(2, KibomboHouses, 3, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(3, KibomboHouses, 2, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_WEST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_WEST, FLAG_KIBOMBO_AT_CEREMONY),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_WEST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_EAST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_EAST, FLAG_KIBOMBO_AT_CEREMONY),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_EAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_NORTHEAST, FLAG_KIBOMBO_AT_CEREMONY),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(7, KibomboHouses, 15, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_SOUTHWEST, FLAG_KIBOMBO_AT_CEREMONY),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(9, KibomboHouses, 14, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST, Kibombo, KIBOMBO_FROM_KIBOMBO_HOUSES_SOUTHEAST, FLAG_KIBOMBO_AT_CEREMONY),
	SCENE_EXIT_UNLESS(KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST, KibomboCeremony, KIBOMBO_CEREMONY_FROM_KIBOMBO_HOUSES_SOUTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(11, KibomboHouses, 12, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(12, KibomboHouses, 11, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(13, KibomboHouses, 16, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(14, KibomboHouses, KIBOMBO_HOUSES_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(15, KibomboHouses, 7, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(16, KibomboHouses, 13, EXIT_FLAG_NONE),
	SCENE_EXITS(GabombaStatueFace),
	SCENE_EXIT(GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_FACE_EAST_A),
	SCENE_EXIT(GABOMBA_STATUE_FACE_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_A),
	SCENE_EXITS_END,
};

ACTOR_PLACE KibomboActorPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE KibomboVillageActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 300, 0, 228, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 248, 0, 288, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 208, 0, 176, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 288, 0, 364, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 274, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 58, 0, 298, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 102, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 336, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 488, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 282, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 430, 0, 442, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SIGNAL_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 460, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE KibomboCeremonyActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 114, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 152, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 292, 0, 128, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 112, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 304, 0, 220, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 278, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 158, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 132, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 326, 0, 88, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 308, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 278, 0, 146, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 138, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 196, 0, 142, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_OLIVE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 224, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 181, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 242, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 274, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 338, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 370, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 412, 0, 60, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE KibomboStatueGroundsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_GABOMBA_TONGUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE KibomboStatueGroundsCeremonyActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GABOMBA_EYE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_GABOMBA_TONGUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 218, 0, 420, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 432, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 438, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 440, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 438, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 432, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 310, 0, 422, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 406, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 130, 0, 364, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE KibomboHousesActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_AKAFUBU_WITCH_DOCTOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 366, 0, 586, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_AKAFUBUS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 586, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 634, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 108, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 124, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 100, 0, 132, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 710, 0, 134, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 108, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 108, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 106, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 352, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 600, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 600, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 348, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 84, 0, 858, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 148, 0, 878, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 374, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 448, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE KibomboCues[] = {
	{CUE_TABLE_END},
};

CUE KibomboVillageCues[] = {
	{CUE_EXIT, KIBOMBO_TO_WORLD_MAP, CONDITION_ALWAYS, KIBOMBO_TO_WORLD_MAP},
	{CUE_EXIT, KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_EAST, CONDITION_ALWAYS,
		KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_EAST},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_NORTHWEST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_NORTHEAST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_WEST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHWEST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), KIBOMBO_TO_KIBOMBO_HOUSES_SOUTHEAST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboEnterDoor)},
	{CUE_EXIT, KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_A, CONDITION_ALWAYS,
		KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_A},
	{CUE_EXIT, KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_B, CONDITION_ALWAYS,
		KIBOMBO_TO_KIBOMBO_STATUE_GROUNDS_WEST_B},
	{CUE_STEP, METATILE_BEHAVIOR_POLE_RING, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOverlayFollowsLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_MIDDLE_OF_POLE_RING, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOverlayStopsFollowing)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
		SCRIPT_REF(scKibomboFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A,
		FLAG_KIBOMBO_VILLAGE_ROPE_TIED, 0},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		METATILE_BEHAVIOR_JUMP_NORTH_AMONG_POLES, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_JUMP_WEST_BY_PUDDLE,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboJumpForward)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_FLAME, CONDITION_ALWAYS, 0},
	{CUE_TALK, ACTOR_CHILD_GLAD_OF_THE_CALM, CONDITION_ALWAYS,
		MSGNO(MsgAkafubuHasFinally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_GLAD_OF_THE_CALM,
		CONDITION_ALWAYS, MSGNO(MsgEveryoneWasTerribly)},
	{CUE_TALK, ACTOR_WOMAN_NOTING_FEWER_WARRIORS, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboAskAboutFewerWarriors)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NOTING_FEWER_WARRIORS,
		CONDITION_ALWAYS, MSGNO(MsgCoveringOurWarriors)},
	{CUE_TALK, ACTOR_MAN_ON_THE_OPEN_MOUTH, CONDITION_ALWAYS,
		MSGNO(MsgTheGreatGabomba)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_OPEN_MOUTH,
		CONDITION_ALWAYS, MSGNO(MsgIWantTo)},
	{CUE_TALK, ACTOR_WOMAN_MISSING_THE_CEREMONY, CONDITION_ALWAYS,
		MSGNO(MsgNowThatThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MISSING_THE_CEREMONY,
		CONDITION_ALWAYS, MSGNO(MsgNowThatTheWitch)},
	{CUE_TALK, ACTOR_OLD_MAN_PUZZLED_BY_INDRA, CONDITION_ALWAYS,
		MSGNO(MsgICouldnT)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PUZZLED_BY_INDRA,
		CONDITION_ALWAYS, MSGNO(MsgBecauseKibomboLies)},
	{CUE_TALK, ACTOR_OLD_WOMAN_SMELLING_MEAT, CONDITION_ALWAYS,
		MSGNO(MsgOhISmell)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SMELLING_MEAT,
		CONDITION_ALWAYS, MSGNO(MsgYouDonTThink)},
	{CUE_TALK, ACTOR_BOY_WHO_LOVES_MEAT, CONDITION_ALWAYS,
		MSGNO(MsgGoodOlMeat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_LOVES_MEAT,
		CONDITION_ALWAYS, MSGNO(MsgGulpTheSmell)},
	{CUE_TALK, ACTOR_GIRL_TIRED_OF_MONSTERS, CONDITION_ALWAYS,
		MSGNO(MsgWeGetTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TIRED_OF_MONSTERS,
		CONDITION_ALWAYS, MSGNO(MsgLatelyTheMonsters)},
	{CUE_TALK, ACTOR_GATE_GUARD_A, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboAskAboutRevenge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD_A, CONDITION_ALWAYS,
		MSGNO(MsgNoOneInKibombo)},
	{CUE_TALK, ACTOR_GATE_GUARD_B, CONDITION_ALWAYS,
		MSGNO(MsgTheOtherWarriors)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GATE_GUARD_B, CONDITION_ALWAYS,
		MSGNO(MsgWithAllOur)},
	{CUE_TALK, ACTOR_SIGNAL_FIRE_KEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboTalkPathToPeace)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIGNAL_FIRE_KEEPER,
		CONDITION_ALWAYS, MSGNO(MsgIWonderIfI)},
	{CUE_TABLE_END},
};

CUE KibomboCeremonyCues[] = {
	{CUE_STEP, KIBOMBO_CEREMONY_TO_WORLD_MAP, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboLeaveRoom)},
	{CUE_STEP, KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoom)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHWEST, FLAG_KIBOMBO_DOORS_SHUT,
		SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_NORTHEAST, FLAG_KIBOMBO_DOORS_SHUT,
		SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_EAST, FLAG_KIBOMBO_DOORS_SHUT,
		SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_WEST, FLAG_KIBOMBO_DOORS_SHUT,
		SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHWEST, FLAG_KIBOMBO_DOORS_SHUT,
		SCRIPT_REF(scKibomboEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		KIBOMBO_CEREMONY_TO_KIBOMBO_HOUSES_SOUTHEAST, FLAG_KIBOMBO_DOORS_SHUT,
		SCRIPT_REF(scKibomboEnterDoor)},
	{CUE_STEP, KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_A,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoom)},
	{CUE_STEP, KIBOMBO_CEREMONY_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_WEST_B,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoom)},
	{CUE_STEP, METATILE_BEHAVIOR_POLE_RING, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOverlayFollowsLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_MIDDLE_OF_POLE_RING, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOverlayStopsFollowing)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
		SCRIPT_REF(scKibomboFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A,
		FLAG_KIBOMBO_VILLAGE_ROPE_TIED, 0},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		METATILE_BEHAVIOR_JUMP_NORTH_AMONG_POLES, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboJumpForward)},
	{CUE_TALK, ACTOR_BOY_KEPT_AWAKE, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgAkafubuSCeremony)},
	{CUE_TALK, ACTOR_BOY_KEPT_AWAKE, CONDITION_ALWAYS, MSGNO(MsgTheyToldMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_AWAKE,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgItSStrange)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_AWAKE,
		CONDITION_ALWAYS, MSGNO(MsgAsLongAsThis)},
	{CUE_TALK, ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgDuringTheLast)},
	{CUE_TALK, ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT, CONDITION_ALWAYS,
		MSGNO(MsgAkafubuHasBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgTheGreatGabombaIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SAW_THE_JEWEL_FLOAT,
		CONDITION_ALWAYS, MSGNO(MsgWhatSTaking)},
	{CUE_TALK, ACTOR_AFRAID_MAN, CONDITION_ALWAYS, MSGNO(MsgIMAfraid)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFRAID_MAN, CONDITION_ALWAYS,
		MSGNO(MsgWhenHeFails)},
	{CUE_TALK, ACTOR_GIRL_WHO_HOPES, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgIHopeThe)},
	{CUE_TALK, ACTOR_GIRL_WHO_HOPES, CONDITION_ALWAYS,
		MSGNO(MsgWhewThankGoodness)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WHO_HOPES,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WHO_HOPES,
		CONDITION_ALWAYS, MSGNO(MsgAmIGoing)},
	{CUE_TALK, ACTOR_WOMAN_DOUBTING_THE_JEWEL, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgIsMakingThe)},
	{CUE_TALK, ACTOR_WOMAN_DOUBTING_THE_JEWEL, CONDITION_ALWAYS,
		MSGNO(MsgOnceTheCeremony)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_THE_JEWEL,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgHeSFailed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_THE_JEWEL,
		CONDITION_ALWAYS, MSGNO(MsgKibomboWasSo)},
	{CUE_TALK, ACTOR_WOMAN_WARNING_OF_ANGER, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgWhenTheGabomba)},
	{CUE_TALK, ACTOR_WOMAN_WARNING_OF_ANGER, CONDITION_ALWAYS,
		MSGNO(MsgLastTimeAkafubu)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_ANGER,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIHopeAkafubu)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_ANGER,
		CONDITION_ALWAYS, MSGNO(MsgAkafubuSAlways)},
	{CUE_TALK, ACTOR_MAN_WHO_KNEW_OEIA, CONDITION_ALWAYS,
		MSGNO(MsgAkafubuWasA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_KNEW_OEIA,
		CONDITION_ALWAYS, MSGNO(MsgAkafubuHasNot)},
	{CUE_TALK, ACTOR_MAN_SURE_OF_AKAFUBU, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgAkafubuLooksReally)},
	{CUE_TALK, ACTOR_MAN_SURE_OF_AKAFUBU, CONDITION_ALWAYS,
		MSGNO(MsgIThinkAkafubu)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SURE_OF_AKAFUBU,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgHeTravelsTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_SURE_OF_AKAFUBU,
		CONDITION_ALWAYS, MSGNO(MsgAkafubuIsObsessed)},
	{CUE_TALK, ACTOR_MAN_WHO_KNOWS_THE_SIGN, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgIfTheGreatGabomba)},
	{CUE_TALK, ACTOR_MAN_WHO_KNOWS_THE_SIGN, CONDITION_ALWAYS,
		MSGNO(MsgIBelieveIn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_KNOWS_THE_SIGN,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreatGabombaActually)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_KNOWS_THE_SIGN,
		CONDITION_ALWAYS, MSGNO(MsgAkafubuAndOeia)},
	{CUE_TALK, ACTOR_WOMAN_RECALLING_HIS_FAILURES, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgAkafubuHasTried)},
	{CUE_TALK, ACTOR_WOMAN_RECALLING_HIS_FAILURES, CONDITION_ALWAYS,
		MSGNO(MsgWhatDonT)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_RECALLING_HIS_FAILURES,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAsAChildAkafubu)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_RECALLING_HIS_FAILURES,
		CONDITION_ALWAYS, MSGNO(MsgIMSoFar)},
	{CUE_TALK, ACTOR_WOMAN_NO_PUSHING, CONDITION_ALWAYS,
		MSGNO(MsgHeyNoPushing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NO_PUSHING,
		CONDITION_ALWAYS, MSGNO(MsgThereAreSo)},
	{CUE_TALK, ACTOR_OLD_MAN_RECALLING_OEIA, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgAkafubuIsAttempting)},
	{CUE_TALK, ACTOR_OLD_MAN_RECALLING_OEIA, CONDITION_ALWAYS,
		MSGNO(MsgTonightSeemsTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RECALLING_OEIA,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgOeiaSSudden)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RECALLING_OEIA,
		CONDITION_ALWAYS, MSGNO(MsgWillTheGreat)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_OFFICE, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgHeWhoIs)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_OFFICE, CONDITION_ALWAYS,
		MSGNO(MsgOeiaSDeath)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_OFFICE,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfOnlyOeia)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_OFFICE,
		CONDITION_ALWAYS, MSGNO(MsgISuspectAkafubu)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_JEWEL, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgAkafubuBlamesHis)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_JEWEL, CONDITION_ALWAYS,
		MSGNO(MsgTheySayThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_JEWEL,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgTheyAttackedSome)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_JEWEL,
		CONDITION_ALWAYS, MSGNO(MsgTheyReSaying)},
	{CUE_TALK, ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH, FLAG_AKAFUBU_OFFERED_HELP,
		MSGNO(MsgIfTheGreatGabombaS)},
	{CUE_TALK, ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH, CONDITION_ALWAYS,
		MSGNO(MsgAkafubuSeesThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
		FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIFeelSo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WATCHING_THE_MOUTH,
		CONDITION_ALWAYS, MSGNO(MsgIReallyWonder)},
	{CUE_TALK, ACTOR_HEALER_ON_THE_CEREMONY, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboTalkForeignCeremony)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER_ON_THE_CEREMONY,
		CONDITION_ALWAYS, MSGNO(MsgIWonderIfA)},
	{CUE_TABLE_END},
};

CUE KibomboStatueGroundsCues[] = {
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_EAST, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboLeaveRoom)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_A, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboLeaveRoom)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
		KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_PINION,
		IF_FLAG_SET(FLAG_KIBOMBO_VENT_UNCOVERED),
		SCRIPT_REF(scKibomboEnterVent)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_TO_GABOMBA_STATUE_CHAMBER_IDOL,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoomFading)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_TO_KIBOMBO_WEST_B, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboLeaveRoom)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
		SCRIPT_REF(scKibomboFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GROUNDS_PILLAR,
		FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS,
		SCRIPT_REF(scKibomboMovePillarForPiers)},
	{CUE_PUSHED, 0, FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS,
		SCRIPT_REF(scKibomboMovePillarForPiers)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_VENT_PATCH, FLAG_KIBOMBO_VENT_UNCOVERED,
		SCRIPT_REF(scKibomboScoopVent)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_NORTH_MARK, FLAG_NORTH_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopNorthMark)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_SOUTH_MARK, FLAG_SOUTH_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopSouthMark)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_WEST_MARK, FLAG_WEST_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopWestMark)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_EAST_MARK, FLAG_EAST_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopEastMark)},
	{CUE_TABLE_END},
};

CUE KibomboStatueGroundsCeremonyCues[] = {
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoom)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_A,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoom)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_PINION,
		IF_FLAG_SET(FLAG_KIBOMBO_VENT_UNCOVERED),
		SCRIPT_REF(scKibomboEnterVent)},
	{CUE_STEP,
		KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_GABOMBA_STATUE_CHAMBER_IDOL_SOUTH,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoomFading)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B,
		FLAG_KIBOMBO_PIERS_MET, SCRIPT_REF(scKibomboLeaveRoom)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B,
		FLAG_KIBOMBO_PIERS_STAYED_BEHIND,
		SCRIPT_REF(scKibomboPiersRefusesToLeave)},
	{CUE_STEP, KIBOMBO_STATUE_GROUNDS_CEREMONY_TO_KIBOMBO_CEREMONY_WEST_B,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboLeaveRoom)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_KIBOMBO_PUDDLE_FROZEN,
		SCRIPT_REF(scKibomboFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GROUNDS_PILLAR,
		FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS,
		SCRIPT_REF(scKibomboMovePillarForPiers)},
	{CUE_PUSHED, 0, FLAG_KIBOMBO_PILLAR_MOVED_FOR_PIERS,
		SCRIPT_REF(scKibomboMovePillarForPiers)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_VENT_PATCH, FLAG_KIBOMBO_VENT_UNCOVERED,
		SCRIPT_REF(scKibomboScoopVent)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_NORTH_MARK, FLAG_NORTH_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopNorthMark)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_SOUTH_MARK, FLAG_SOUTH_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopSouthMark)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_WEST_MARK, FLAG_WEST_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopWestMark)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_EAST_MARK, FLAG_EAST_MARK_UNCOVERED,
		SCRIPT_REF(scKibomboScoopEastMark)},
	{CUE_STEP, METATILE_BEHAVIOR_BY_PIERS, FLAG_KIBOMBO_PIERS_MET,
		SCRIPT_REF(scKibomboMeetPiers)},
	{CUE_STEP, METATILE_BEHAVIOR_BY_PIERS_WAITING,
		FLAG_KIBOMBO_PIERS_WITH_PARTY, SCRIPT_REF(scKibomboPiersThanksParty)},
	{CUE_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgTheOrbIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS,
		MSGNO(MsgIAmCertain)},
	{CUE_STEP, METATILE_BEHAVIOR_WAY_TO_DEAD_END, FLAG_KIBOMBO_DEAD_END_SEEN,
		SCRIPT_REF(scKibomboDeadEndByTheStatue)},
	{CUE_TALK, ACTOR_WARRIOR_A, CONDITION_ALWAYS, MSGNO(MsgAhhSomethingIs)},
	{CUE_TALK, ACTOR_WARRIOR_B, CONDITION_ALWAYS, MSGNO(MsgYouLookHuman)},
	{CUE_TALK, ACTOR_WARRIOR_C, CONDITION_ALWAYS,
		MSGNO(MsgAkafubuPossessesThe)},
	{CUE_TALK, ACTOR_WARRIOR_D, CONDITION_ALWAYS, MSGNO(MsgWeMustStand)},
	{CUE_TALK, ACTOR_WARRIOR_E, CONDITION_ALWAYS, MSGNO(MsgMenOrGods)},
	{CUE_TALK, ACTOR_WARRIOR_F, CONDITION_ALWAYS, MSGNO(MsgItSeemsThese)},
	{CUE_TALK, ACTOR_WARRIOR_G, CONDITION_ALWAYS, MSGNO(MsgThisIsnT)},
	{CUE_TALK, ACTOR_WARRIOR_H, CONDITION_ALWAYS, MSGNO(MsgItIsProper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_A, CONDITION_ALWAYS,
		MSGNO(MsgTheyLookLike)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_B, CONDITION_ALWAYS,
		MSGNO(MsgIfNoPerson)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_C, CONDITION_ALWAYS,
		MSGNO(MsgISawAkafubu)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_D, CONDITION_ALWAYS,
		MSGNO(MsgIHopeAkafubuCompletes)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_E, CONDITION_ALWAYS,
		MSGNO(MsgWhatIfThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_F, CONDITION_ALWAYS,
		MSGNO(MsgLookAtThese)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_G, CONDITION_ALWAYS,
		MSGNO(MsgSureThisIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_H, CONDITION_ALWAYS,
		MSGNO(MsgKoffKoffAll)},
	{CUE_TABLE_END},
};

CUE KibomboHousesCues[] = {
	{CUE_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST, CONDITION_ALWAYS,
		KIBOMBO_HOUSES_TO_KIBOMBO_NORTHWEST},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_WEST, CONDITION_ALWAYS,
		KIBOMBO_HOUSES_TO_KIBOMBO_WEST},
	{CUE_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_EAST, CONDITION_ALWAYS,
		KIBOMBO_HOUSES_TO_KIBOMBO_EAST},
	{CUE_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST, CONDITION_ALWAYS,
		KIBOMBO_HOUSES_TO_KIBOMBO_NORTHEAST},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_ITEM_SHOP_LADDER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboStepOntoFirstLadder)},
	{CUE_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST, CONDITION_ALWAYS,
		KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHWEST},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_AKAFUBUS_LADDER, CONDITION_ALWAYS,
		SCRIPT_REF(KibomboStepOntoSecondLadder)},
	{CUE_EXIT, KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST, CONDITION_ALWAYS,
		KIBOMBO_HOUSES_TO_KIBOMBO_SOUTHEAST},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_TWO_BEDS_LADDER, CONDITION_ALWAYS,
		SCRIPT_REF(KibomboStepOntoThirdLadder)},
	{CUE_TALK, ACTOR_AKAFUBU_AT_HOME, FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU,
		MSGNO(MsgIMGoing)},
	{CUE_TALK, ACTOR_AKAFUBU_AT_HOME, CONDITION_ALWAYS, MSGNO(MsgIJustCan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AKAFUBU_AT_HOME,
		CONDITION_ALWAYS, MSGNO(MsgDangWhyDid)},
	{CUE_TALK, ACTOR_FATHER_OF_AKAFUBU, FLAG_KIBOMBO_LEFT_HOME_OF_AKAFUBU,
		MSGNO(MsgIThinkThat)},
	{CUE_TALK, ACTOR_FATHER_OF_AKAFUBU, CONDITION_ALWAYS,
		MSGNO(MsgMySonHasBeen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_OF_AKAFUBU,
		CONDITION_ALWAYS, MSGNO(MsgAfterHeBecame)},
	{CUE_TALK, ACTOR_CHILD_ON_THE_EASTERN_SEA, CONDITION_ALWAYS,
		MSGNO(MsgIHaveHeard)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_ON_THE_EASTERN_SEA,
		CONDITION_ALWAYS, MSGNO(MsgIKnowI)},
	{CUE_TALK, ACTOR_WOMAN_ASKING_ABOUT_THE_WEST, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboAskGoingWest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_THE_WEST,
		CONDITION_ALWAYS, MSGNO(MsgCrossingTheRiver)},
	{CUE_TALK, ACTOR_BOY_WHO_HEARD_OF_ATTEKA, CONDITION_ALWAYS,
		MSGNO(MsgYouCanSee)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_HEARD_OF_ATTEKA,
		CONDITION_ALWAYS, MSGNO(MsgIfIM)},
	{CUE_TALK, ACTOR_MAN_ON_TREASURE_WEST, CONDITION_ALWAYS,
		MSGNO(MsgIfYouAreLooking)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_TREASURE_WEST,
		CONDITION_ALWAYS, MSGNO(MsgSureOurLegends)},
	{CUE_TALK, ACTOR_WOMAN_ON_THE_TREASURE_HUNTS, CONDITION_ALWAYS,
		MSGNO(MsgEachYearWhen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_TREASURE_HUNTS,
		CONDITION_ALWAYS, MSGNO(MsgIWonderWhatOeia)},
	{CUE_TALK, ACTOR_MAN_WITH_ACHING_BACK, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboTalkAchingBack)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WITH_ACHING_BACK,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboReadMindNeverAgain)},
	{CUE_TALK, ACTOR_ACHING_BACK_BEDSIDE, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboTalkAchingBack)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ACHING_BACK_BEDSIDE,
		CONDITION_ALWAYS, SCRIPT_REF(scKibomboReadMindNeverAgain)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_A_NICER_AKAFUBU, CONDITION_ALWAYS,
		MSGNO(MsgAsSoonAs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_A_NICER_AKAFUBU,
		CONDITION_ALWAYS, MSGNO(MsgAkafubuGetsSo)},
	{CUE_TALK, ACTOR_WARRIOR_NOT_TO_BE_FEARED, CONDITION_ALWAYS,
		MSGNO(MsgManyInGondowan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_NOT_TO_BE_FEARED,
		CONDITION_ALWAYS, MSGNO(MsgItWonT)},
	{CUE_TALK, ACTOR_WARRIOR_TO_TELL_THE_VILLAGES, CONDITION_ALWAYS,
		MSGNO(MsgWeThinkIt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_TO_TELL_THE_VILLAGES,
		CONDITION_ALWAYS, MSGNO(MsgIAmAMighty)},
	{CUE_TALK, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenWeaponShop)},
	{CUE_EXAMINE, ACTOR_WEAPON_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenWeaponShop)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SHOPKEEPER,
		CONDITION_ALWAYS, MSGNO(MsgOurWarriorsAppear)},
	{CUE_TALK, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenArmorShop)},
	{CUE_EXAMINE, ACTOR_ARMOR_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenArmorShop)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SHOPKEEPER,
		CONDITION_ALWAYS, MSGNO(MsgBecauseOurPaint)},
	{CUE_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenItemShop)},
	{CUE_EXAMINE, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenItemShop)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
		CONDITION_ALWAYS, MSGNO(MsgIfIHad)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scKibomboOpenInn)},
	{CUE_EXAMINE, ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenInn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgThatBedMakes)},
	{CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgICanTRemember)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
		MSGNO(MsgILoveKibombo)},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenSanctum)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_SANCTUM_COUNTER, CONDITION_ALWAYS,
		SCRIPT_REF(scKibomboOpenSanctum)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
		MSGNO(MsgICanTBelieve)},
	{CUE_TABLE_END},
};
#undef DEFINE_KIBOMBO_SET_CROWD_MODE
#undef DEFINE_KIBOMBO_STEP_WHEN_HELD
#undef fdActorSetSpriteFlags
