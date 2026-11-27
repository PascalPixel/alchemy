/* Kolima's overlay, which holds three scenes. The village's houses and shops
   are another overlay (log_rooms_c.c), and so is each scene of Kolima Forest.

   - Kolima (SceneId_Kolima), the village of lumberjacks, to which Lord McCoy
	 has barricaded the road from Bilibin since the trouble began.

	 The party's first entry from the World Map (KOLIMA_FROM_WORLD_MAP) plays
	 scKolimaWitnessForestCurse, unless the party has already seen it in Kolima
	 Forest (FLAG_KOLIMA_CURSE_WITNESSED). Every villager has been turned into
	 a tree. Sparkles fall from the sky and press the party down until domes of
	 their own Psynergy shield them. Two unseen voices speak, Tret and Laurel,
	 the spirits of the holy trees, who send the party to look for Tret deep
	 within Kolima Forest.

	 Until the curse is lifted in Kolima Forest (FLAG_KOLIMA_CURSE_LIFTED),
	 scGetActorPlacements gives the nine villagers (ACTOR_FIRST_VILLAGER
	 to ACTOR_LAST_VILLAGER) tree sprites that stand still. Sparkles twinkle on
	 the ground (ACTOR_FIRST_SPARKLE and the three after it, and five more that
	 scInit places). The villagers answer only Mind Read, with the
	 thoughts of people stuck as trees.

	 Afterwards they are people again and talk of the curse, the holy tree and
	 McCoy's money. The healer among them (ACTOR_HEALER) offers the Sanctum's
	 services across his counter.

	 Granite, a Venus Djinni (ACTOR_GRANITE), waits in the village and joins
	 when spoken to.

	 Five doorways (KOLIMA_TO_INN to KOLIMA_TO_HOUSE_NORTH) lead into the
	 houses and shops (SceneId_LogRoomsC). KOLIMA_TO_WORLD_MAP and
	 KOLIMA_TO_WORLD_MAP_EAST lead to the World Map and KOLIMA_TO_KOLIMA_TUNNEL
	 into Kolima Tunnel.

   - Kolima Tunnel (SceneId_KolimaTunnel), a passage with no one in it from one
	 of the houses (LOG_ROOMS_C_FROM_KOLIMA_TUNNEL) to the village
	 (KOLIMA_FROM_KOLIMA_TUNNEL).

	 It shares the village's cue table, whose exits KOLIMA_TO_WORLD_MAP and
	 KOLIMA_TO_WORLD_MAP_EAST are, by the same numbers, its two ways out
	 (KOLIMA_TUNNEL_TO_LOG_ROOMS_C and KOLIMA_TUNNEL_TO_KOLIMA).

	 That table's two jars, an Apple and an Herb (examine cues 100 and
	 101), are in the village. Only Kolima's layout (kolima's
	 map.inc) places them, at metatiles (23, 8) and (13, 23).

   - Kolima Bridge (SceneId_KolimaBridge), "the grand bridge across the
	 river" and "the pride of Kolima" to the village's armor seller, with the
	 World Map at both ends. It is a drawbridge, standing raised between its
	 posts.

	 A villager stands beside it, a tree while the curse lasts
	 (ACTOR_BRIDGE_VILLAGER_AS_TREE) and a man afterwards
	 (ACTOR_BRIDGE_VILLAGER).

	 The first time the party then steps up to the bridge (step cue 10),
	 scKolimaLowerBridge plays. He asks whether they are going to cross, hops
	 aside and lets the drawbridge down across the river in front of the leader
	 (FLAG_KOLIMA_BRIDGE_LOWERED; its recording,
	 places/kolima-bridge/scripts/scKolimaLowerBridge.gif).

	 From then on he says that for a while he thought no one would ever cross
	 it again. scKolimaRestoreBridgeState draws it down on every later visit,
	 and the bridge on the World Map is drawn down with the same flag. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "scene/kolima.h"
#include "scene/kolima_curse_dust.h"
#include "scene/party_nod.h"
#include "text/msg_ids.h"

extern Sint32			 Kolima_GaretWalksIn[];
extern Sint32			 Kolima_IvanWalksIn[];
extern Sint32			 Kolima_MiaWalksIn[];
extern Sint32			 Kolima_PsynergyFieldGrowActions[];
extern Sint32			 Kolima_PsynergyFieldShrinkActions[];
extern Sint32			 Kolima_GroundSparkleActions[];
extern Sint32			 Kolima_DebrisScript[];
extern Sint32			 Kolima_FaceLeaderScript[];
extern Uint16			 Kolima_DoorMetatiles[];
extern SCENE_ENTRANCE	 KolimaEntrances[];
extern SCENE_ENTRANCE	 KolimaBridgeEntrances[];
extern SCENE_ENTRANCE	 KolimaTunnelEntrances[];
extern SCENE_EXIT_REGION KolimaTunnelRegions[];
extern Uint32			 KolimaExits[];
extern ACTOR_PLACE		 KolimaTunnelActorPlacements[];
extern ACTOR_PLACE		 KolimaActorPlacements[];
extern ACTOR_PLACE		 Kolima_MoreGroundSparklePlacements[];
extern ACTOR_PLACE		 KolimaBridgeActorPlacements[];
extern CUE				 KolimaCues[];
extern CUE				 KolimaBridgeCues[];
extern Sint32			 Kolima_GoldenRingsScript[];

/* The pieces the drawbridge throws out where it lands (SPRITE_DUST_PUFF). */
enum { BRIDGE_DUST_PUFFS = 10 };

static void scKolimaWitnessForestCurse(void);
static void scKolimaShowMessageAndWait(Sint32 actorId, Sint32 waitFrames);
static void scKolimaFaceActorAndWait(
	Sint32 actorId, Sint32 facing, Sint32 waitFrames);
static void scKolimaSpawnCurseSparkle(void);
static void scKolimaRestoreBridgeState(void);

/* Actors of Kolima, numbering the rows of KolimaActorPlacements from
   ACTOR_FIRST_PLACED. Garet, Ivan and Mia, who stand by for the curse, take
   their own actor slots. The nine villagers are named by what they say once the
   curse is lifted. */
typedef enum KOLIMA_ACTOR_ID {
	/* A young man with brown hair. */
	ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS = ACTOR_FIRST_PLACED,
	/* A woman with fair hair, whose mind Ivan reads in
	   scKolimaWitnessForestCurse. */
	ACTOR_VILLAGER_RELYING_ON_FORESTS,
	/* A burly grey-haired man. */
	ACTOR_VILLAGER_TIME_TO_THINK,
	/* A woman in a green dress. */
	ACTOR_VILLAGER_SPARKLES_FELL,
	/* An old man with a cane. */
	ACTOR_OLD_MAN_MCCOYS_MONEY,
	/* An old woman. */
	ACTOR_OLD_WOMAN_HOLY_TREE_PALACE,
	/* A small boy with fair hair. */
	ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE,
	/* A little girl with pigtails. */
	ACTOR_GIRL_SLEEPY_TREES,
	/* A man with a white beard in a tall hat and a long robe. He opens the
	   Sanctum's services across his counter (mnSanctumRun). */
	ACTOR_HEALER,
	/* Tret's wrathful side, unseen while the curse falls. */
	ACTOR_TRET,
	/* Unseen while the curse falls. */
	ACTOR_LAUREL,
	/* The fields of Psynergy that rise over the party while the curse falls
	   ("You know what those fields were?"), each set where its character stands
	   in scKolimaWitnessForestCurse. Mia's is set only with her in the
	   party. */
	ACTOR_LEADER_PSYNERGY_FIELD,
	ACTOR_IVAN_PSYNERGY_FIELD,
	ACTOR_MIA_PSYNERGY_FIELD,
	ACTOR_GARET_PSYNERGY_FIELD,
	/* The four sparkles on the ground while the curse lasts
	   (Kolima_GroundSparkleActions). */
	ACTOR_FIRST_SPARKLE,
	/* Granite, a Venus Djinni, placed until it has joined. */
	ACTOR_GRANITE = ACTOR_FIRST_SPARKLE + 4,
	ACTOR_FIRST_VILLAGER = ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS,
	ACTOR_LAST_VILLAGER = ACTOR_HEALER
} KOLIMA_ACTOR_ID;

/* Kolima Bridge's villager, numbering the rows of KolimaBridgeActorPlacements
   from ACTOR_FIRST_PLACED: the same villager as a man and as a tree, in the
   same place. */
typedef enum KOLIMA_BRIDGE_ACTOR_ID {
	/* A burly grey-haired man. */
	ACTOR_BRIDGE_VILLAGER = ACTOR_FIRST_PLACED,
	/* The same villager as a tree, while the curse lasts. */
	ACTOR_BRIDGE_VILLAGER_AS_TREE
} KOLIMA_BRIDGE_ACTOR_ID;

/* The end of a Psynergy field's growing: if a speck of the curse has burst on
   the fields meanwhile, this one takes its next pose. */
static Sint32 scKolimaFlashHitPsynergyFields(ACTOR *actor)
{
	if(Kolima_PsynergyFieldHit != 0) {
		fdActorSelectAnimation(actor, 2);
		Kolima_PsynergyFieldHit = 0;
	}
	return 1;
}

/* Each frame, a sparkle on Kolima's ground: red-tinted, it flashes white for
   the last few frames of each twinkle, which lasts about 24 frames at a random
   pace. */
static Sint32 scKolimaTwinkleGroundSparkle(ACTOR *actor)
{
	Sint32 randomValue = syRandomNext();
	Sint32 scaledRandom = randomValue * 100;
	Sint32 appearanceTimer = (Uint16)actor->tmr + ((Uint32)scaledRandom >> 16);
	actor->tmr = appearanceTimer;
	if((Sint16)appearanceTimer > 1000) {
		fdActorSetSprCol(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	}
	if(actor->tmr > 1200) {
		actor->tmr = 0;
	}
	return 1;
}

/* KolimaActorPlacements' rows for Garet, Ivan and Mia, before the
   villagers'. */
#define KOLIMA_PARTY_ROWS 3

/* While the curse lasts, gives Kolima's nine villagers, ACTOR_FIRST_VILLAGER to
   ACTOR_LAST_VILLAGER, tree sprites: SPRITE_TREE, or SPRITE_SMALL_TREE for the
   two children, ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE and
   ACTOR_GIRL_SLEEPY_TREES. They stand still and keep their facing when spoken
   to. */
static void scKolimaInitCursedVillagers(ACTOR_PLACE *placements)
{
	ACTOR_PLACE *villager = &placements[KOLIMA_PARTY_ROWS +
		ACTOR_FIRST_VILLAGER - ACTOR_FIRST_PLACED];
	Uint32		 villagerIndex;
	Sint32		 treeSprite;
	Sint32		 alternateTreeSprite;

	villagerIndex = 0;
	treeSprite = SPRITE_TREE;
	alternateTreeSprite = SPRITE_SMALL_TREE;
	for(; villagerIndex <= ACTOR_LAST_VILLAGER - ACTOR_FIRST_VILLAGER;
		villagerIndex++) {
		villager->spr = treeSprite;
		if(villagerIndex -
				(ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE - ACTOR_FIRST_VILLAGER) <=
			ACTOR_GIRL_SLEEPY_TREES - ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE) {
			villager->spr = alternateTreeSprite;
		}
		villager->talkFacing = TALK_FACING_KEEP;
		villager->movement = MOVEMENT_TYPE_STAND;
		villager++;
	}
}

/* Each frame of a piece thrown out where the drawbridge lands: it flies, swells
   and is gone when its timer runs out. */
static Sint32 scKolimaUpdateFallingDebris(ACTOR *debris)
{
	debris->x.fixed += debris->velX;
	debris->z.fixed += debris->velZ;
	debris->velZ -= toFIXED(1) / 25;
	debris->scaleX += toFIXED(3) / 128;
	debris->scaleY += toFIXED(3) / 128;
	{
		Sint32 framesLeft = (Uint16)debris->tmr - 1;
		debris->tmr = framesLeft;
		if((Uint16)framesLeft == 0) {
			fdActorRelease(debris);
		}
	}
	return 1;
}

#include "shared/actor_facing.c"

/* Selects the entrances of Kolima, Kolima Tunnel or Kolima Bridge. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KOLIMA_BRIDGE) {
		return KolimaBridgeEntrances;
	}
	if(sceneId == SCENE_KOLIMA_TUNNEL) {
		return KolimaTunnelEntrances;
	}
	return KolimaEntrances;
}

/* Only Kolima Tunnel has regions: its two ways out,
   KOLIMA_TUNNEL_TO_LOG_ROOMS_C and KOLIMA_TUNNEL_TO_KOLIMA. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	if(MainRoutineStateWork.next == SCENE_KOLIMA_TUNNEL) {
		return KolimaTunnelRegions;
	}
	return 0;
}

/* The ways out of all three scenes. */
const Uint32 *scGetExits(void)
{
	return KolimaExits;
}

/* Selects the people of Kolima or Kolima Bridge. Kolima Tunnel has none. In
   Kolima the villagers are trees until the curse is lifted. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KOLIMA) {
		if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
			scKolimaInitCursedVillagers(KolimaActorPlacements);
		}
		return KolimaActorPlacements;
	}
	if(sceneId == SCENE_KOLIMA_BRIDGE) {
		return KolimaBridgeActorPlacements;
	}
	return KolimaTunnelActorPlacements;
}

/* In Kolima, once the curse is lifted, talking to the healer (ACTOR_HEALER)
   across his counter (facing north) opens the Sanctum's services. */
static void scKolimaTalkToHealer(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32 facing = leader->facing;
	fdScriptStart();
	if(FACING_IS_NORTH(facing)) {
		mnSanctumRun(ACTOR_HEALER);
	} else {
		fdScriptSetMessageId(MsgKolimaToldHolyTrees);
		/* "I am told the holy tree's rage caused this horrible..." Yes: "You
		   may not have noticed, but something changed when those..." No: "I
		   agree. Something did change when the gems fell." */
		fdScriptAskYesNo(ACTOR_HEALER, 0);
	}
	fdScriptEnd();
}

/* In Kolima, talking to Granite, a Venus Djinni, makes it join the party. */
static void scKolimaGraniteJoins(void)
{
	fdDjinnJoinParty(
		ACTOR_GRANITE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
}

/* Selects Kolima Bridge's cues, or the table Kolima and Kolima Tunnel share. */
const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_KOLIMA_BRIDGE) {
		return KolimaBridgeCues;
	}
	return KolimaCues;
}

/* Walks the leader north through a door of Kolima into the exit of the same
   number. */
static void scKolimaWalkThroughExit(Sint32 exitNumber)
{
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	syImportScriptSetExitRequest(exitNumber);
}

/* In Kolima, walking into one of the five doorways leads into the houses and
   shops. */
static void scKolimaEnterDoor(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 x = 0;
	Sint32		 y = 0;

	fdScriptStart();
	sySndPlay(SE_DOOR_OPEN);
	switch(state->stepCue) {
	case KOLIMA_TO_INN:
		x = 71;
		y = 9;
		break;
	case KOLIMA_TO_ITEM_SHOP:
		x = 73;
		y = 17;
		break;
	case KOLIMA_TO_HOUSE_SOUTH:
		x = 80;
		y = 21;
		break;
	case KOLIMA_TO_WEAPON_AND_ARMOR_SHOP:
		x = 84;
		y = 12;
		break;
	case KOLIMA_TO_HOUSE_NORTH:
		fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
		FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
		syImportScriptSetExitRequest(KOLIMA_TO_HOUSE_NORTH);
		fdScriptEnd();
		return;
	}
	fdMapGridPlayMapEdit(Kolima_DoorMetatiles, x, y);
	scKolimaWalkThroughExit(state->stepCue);
	fdScriptEnd();
}

/* At Kolima Bridge, the first time the party steps up to it, the villager
   beside it (ACTOR_BRIDGE_VILLAGER) asks whether they are going to cross. */
static void scKolimaLowerBridge(void)
{
	Uint32 debrisIndex;

	fdScriptStart();
	fdCameraSetSpeed(2);
	fdCameraMoveTo(168, 0, 246, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorShowEmote(
		ACTOR_BRIDGE_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorRunRepeatedMotion(ACTOR_BRIDGE_VILLAGER, 2);
	fdScriptSetMessageId(MsgKolimaGoingCrossBridge);
	/* "Are you going to cross the bridge?" */
	fdScriptShowMessage(ACTOR_BRIDGE_VILLAGER, 0);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(168, 0, 234, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 174, 278);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_BRIDGE_VILLAGER, ANIM_NOD);
	/* "Oh... Right! Go right ahead!" */
	fdScriptShowMessage(ACTOR_BRIDGE_VILLAGER, 0);
	fdActorFaceDirection(ACTOR_BRIDGE_VILLAGER, FACING_WEST_NORTHWEST, 20);
	fdActorRunRepeatedMotion(ACTOR_BRIDGE_VILLAGER, 1);
	fdScriptWait(20);
	fdActorGet(ACTOR_BRIDGE_VILLAGER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_BRIDGE_VILLAGER, 2);
	fdActorJump(ACTOR_BRIDGE_VILLAGER, 2, 0);
	fdActorMoveToAndWait(ACTOR_BRIDGE_VILLAGER, 224, 197);
	sySndPlay(SE_KOLIMA_BRIDGE_HOP);
	fdScriptWait(10);
	fdActorMoveToAndWait(ACTOR_BRIDGE_VILLAGER, 234, 200);
	fdScriptWait(10);
	sySndPlay(SE_KOLIMA_BRIDGE_POST);
	fdScriptWait(30);
	/* The drawbridge comes down in ever shorter steps. */
	fdMapGridCopyMetatilesTo(91, 0, 72, 9, 5, 4);
	fdScriptWait(12);
	fdMapGridCopyMetatilesTo(91, 4, 72, 9, 5, 4);
	fdScriptWait(9);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdMapGridCopyMetatilesTo(91, 8, 72, 9, 5, 5);
	fdScriptWait(6);
	fdMapGridCopyMetatilesTo(91, 13, 72, 9, 5, 6);
	fdScriptWait(3);
	sySndPlay(SE_THUMP);
	/* Puffs of dust where it lands, 4 pixels apart, thrown alternately left and
	   right. */
	for(debrisIndex = 0; debrisIndex < BRIDGE_DUST_PUFFS; debrisIndex++) {
		ACTOR *debris = syFieldActorCreate(
			SPRITE_DUST_PUFF, toFIXED(148 + debrisIndex * 4), 0, toFIXED(258));

		if(debris != NULL) {
			debris->motionFlg = 0;
			fdActorSetSprFlags(debris, 0);
			debris->spr->image.pri = 0;
			debris->tmr = (syRandomBelow(40)) + 40;
			debris->velX = (toFIXED(debrisIndex & 3) + toFIXED(1)) >> 1;
			debris->velZ = toFIXED(1);
			if(debrisIndex & 1) {
				debris->velX = -((toFIXED(debrisIndex & 3) + toFIXED(1)) >> 1);
			}
			fdActorSelectAnimation(debris, 1);
			fdActorStartScript(debris, Kolima_DebrisScript);
		}
	}
	fdMapGridCopyMetatilesTo(91, 19, 72, 9, 5, 7);
	fdMapGridCopyMetatileAttributes(23, 11, 5, 7, 8, 11);
	fdMapGridSetShake(0, 4, 1);
	fdScriptWait(10);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdScriptWait(20);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	/* From now on the villager turns to keep facing the leader. */
	syFieldActorWatchAndStartScript(ACTOR_BRIDGE_VILLAGER,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER, Kolima_FaceLeaderScript);
	fdScriptWait(60);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	gmFlagSet(FLAG_KOLIMA_BRIDGE_LOWERED);
	fdScriptEnd();
}

/* Sets up whichever scene is entered: Kolima Bridge as the curse and the bridge
   leave it (scKolimaRestoreBridgeState), Kolima Tunnel with a window
   transition, and Kolima itself. */
Sint32 scInit(void)
{
	Sint16		  sceneId;
	Uint32		  actorId;
	const Sint32 *table;

	sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KOLIMA_BRIDGE) {
		scKolimaRestoreBridgeState();
		return 0;
	}

	if(sceneId == SCENE_KOLIMA_TUNNEL) {
		FieldStateWork->transition = SCENE_TRANSITION(
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
		return 0;
	}

	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SPARKLE), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SPARKLE + 1), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SPARKLE + 2), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SPARKLE + 3), 0);

	table = Kolima_GroundSparkleActions;
	fdActorSetScript(ACTOR_FIRST_SPARKLE, table);
	fdActorSetScript(ACTOR_FIRST_SPARKLE + 1, table);
	fdActorSetScript(ACTOR_FIRST_SPARKLE + 2, table);
	fdActorSetScript(ACTOR_FIRST_SPARKLE + 3, table);

	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		for(actorId = ACTOR_FIRST_VILLAGER; actorId <= ACTOR_LAST_VILLAGER;
			actorId++) {
			fdActorSetSprFlags(fdActorGet(actorId), 0);
		}
		/* The healer's counter west, east and south of his tree searches as a
		   wooden box, as its corners always do. */
		fdMapGridCopyMetatileAttributes(13, 9, 1, 1, 13, 8);
		fdMapGridCopyMetatileAttributes(13, 9, 1, 1, 15, 8);
		fdMapGridCopyMetatileAttributes(13, 9, 1, 1, 14, 9);
	}

	if(gmFlagGet(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
		if(MainRoutineStateWork.entrance == KOLIMA_FROM_WORLD_MAP) {
			scKolimaWitnessForestCurse();
		}
	}

	if(gmFlagGet(FLAG_KOLIMA_CURSE_WITNESSED) != 0) {
		fdActorReleaseById(ACTOR_GARET);
		fdActorReleaseById(ACTOR_IVAN);
		fdActorReleaseById(ACTOR_MIA);
		fdActorReleaseById(ACTOR_TRET);
		fdActorReleaseById(ACTOR_LAUREL);
		fdActorReleaseById(ACTOR_LEADER_PSYNERGY_FIELD);
		fdActorReleaseById(ACTOR_IVAN_PSYNERGY_FIELD);
		fdActorReleaseById(ACTOR_MIA_PSYNERGY_FIELD);
		fdActorReleaseById(ACTOR_GARET_PSYNERGY_FIELD);
		fdScriptAppendActorPlacements(Kolima_MoreGroundSparklePlacements);
	}

	return 0;
}

/* Shows one of the domes of Psynergy on its frames, even or odd, and lifts it
   500 pixels out of sight on the others. */
static inline void scKolimaFlickerPsynergyField(
	Sint32 actorId, Sint32 visibleOnOddFrames)
{
	ACTOR *actor = fdActorGet(actorId);
	if(actor != NULL) {
		actor->motionFlg = 0;
		if(visibleOnOddFrames ? (FrameCount & 1) : !(FrameCount & 1)) {
			actor->y.fixed = 0;
		} else {
			actor->y.fixed = toFIXED(500);
		}
	}
}

/* Makes the four domes flicker, two on even frames and two on odd. */
static void scKolimaFlickerPsynergyFields(void)
{
	scKolimaFlickerPsynergyField(ACTOR_LEADER_PSYNERGY_FIELD, 0);
	scKolimaFlickerPsynergyField(ACTOR_IVAN_PSYNERGY_FIELD, 1);
	scKolimaFlickerPsynergyField(ACTOR_MIA_PSYNERGY_FIELD, 0);
	scKolimaFlickerPsynergyField(ACTOR_GARET_PSYNERGY_FIELD, 1);
}

#include "shared/mind_read.c"

/* In Kolima, on the first entry from the World Map (KOLIMA_FROM_WORLD_MAP), the
   party sees the curse fall. This plays until FLAG_KOLIMA_CURSE_WITNESSED. */
static void scKolimaWitnessForestCurse(void)
{
	ACTOR		 *actor;
	Sint32		 *partyLayout;
	const Sint32 *squashActions;
	const Sint32 *restoreScaleActions;
	Sint32		 *miaInParty;
	const Sint32 *growFieldActions;
	const Sint32 *shrinkFieldActions;

	/* Enter the deserted village and let Ivan inspect one of its trees. */
	fdScriptStart();
	partyLayout = &Kolima_MiaInParty;
	*partyLayout = gmFlagGet(FLAG_IN_PARTY(MIA));
	fdActorSetSprFlags(fdActorGet(ACTOR_LEADER_PSYNERGY_FIELD), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_IVAN_PSYNERGY_FIELD), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_MIA_PSYNERGY_FIELD), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET_PSYNERGY_FIELD), 0);
	fdCameraMoveTo(104, CAMERA_KEEP, 256, 0);
	fdMapGridCenterOnFollowedPoint();
	fdActorSetPos(ACTOR_PARTY_LEADER, 23, 247);
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetSpeed(ACTOR_IVAN, 0.6);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	fdActorSetScript(ACTOR_GARET, Kolima_GaretWalksIn);
	fdActorSetScript(ACTOR_IVAN, Kolima_IvanWalksIn);
	if(*partyLayout != 0) {
		fdActorSetSpeed(ACTOR_MIA, 0.6);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
		}
		fdActorSetScript(ACTOR_MIA, Kolima_MiaWalksIn);
	}
	fdActorWaitForScriptEndById(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
	partyLayout = &Kolima_MiaInParty;
	if(*partyLayout != 0) {
		fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 60);
	if(*partyLayout != 0) {
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	if(*partyLayout != 0) {
		fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
	}
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdScriptSetMessageId(MsgKolimaQuiet);
	/* "It's so quiet..." */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_IVAN, 72, 286);
	fdActorWalkToAndWait(ACTOR_IVAN, 72, 302);
	fdActorWalkToAndWait(ACTOR_IVAN, 88, 310);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	if(*partyLayout != 0) {
		fdActorFaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
	}
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	if(*partyLayout != 0) {
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	}
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdScriptWait(20);
	/* Mind Read reveals that the trees are the missing villagers. */
	scStartMindRead(ACTOR_IVAN, ACTOR_VILLAGER_RELYING_ON_FORESTS);
	fdScriptWait(40);
	scEndMindRead();
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorGet(ACTOR_IVAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_IVAN, 80, 310);
	fdScriptWait(1);
	fdActorGet(ACTOR_IVAN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What is the matter, Ivan?" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "This tree...was a person." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
	if(*partyLayout != 0) {
		fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
	}
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_GARET, 60);
	if(*partyLayout != 0) {
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	}
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	/* "Everyone in Kolima has been turned into a tree." */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetSpeed(ACTOR_IVAN, 0.5);
	fdActorGet(ACTOR_IVAN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_IVAN, 72, 286);
	fdScriptWait(1);
	fdActorGet(ACTOR_IVAN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetScript(ACTOR_IVAN, Kolima_IvanWalksIn);
	if(*partyLayout != 0) {
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdScriptWait(60);
		/* "It's horrible! All these trees with clothes are really..." */
		scKolimaShowMessageAndWait(ACTOR_MIA, 20);
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	} else {
		FieldStateWork->msg++;
	}
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(10);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	/* "Why? Do you know why this happened?" */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdScriptWait(20);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	} else {
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
		FieldStateWork->msg++;
	}
	/* Yes: "I see... It was because they hurt the holy tree?" No: "I see... Who
	   could have known?" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 40);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_SOUTHEAST, 40);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_WEST, 20);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_SOUTH, 40);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptSetMessageId(MsgKolimaWhatIsIt);
	/* "What is it now, Ivan?" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 10);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "That sparkly stuff on the ground... What is it?" */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_EAST, 20);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 30);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	sySndPlay(SOUND_FADE_OUT_BGM);
	sySndPlay(SE_CURSE_FALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	/* The falling sparkles threaten to turn the party into trees as well. */
	Kolima_SparkleSoundOn = 1;
	sySetFrameEvent(scKolimaSpawnCurseSparkle, EVENT_PASS_FRAME);
	sySynch(20);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(16, 16, 20), PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(120);
	sySynch(60);
	squashActions = Kolima_PartySquashScript;
	fdActorSetScript(ACTOR_PARTY_LEADER, squashActions);
	fdActorSetScript(ACTOR_GARET, squashActions);
	fdActorSetScript(ACTOR_IVAN, squashActions);
	fdActorSetScript(ACTOR_MIA, squashActions);
	fdScriptWait(100);
	/* "Aaah! What's happening!?" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	/* "My mind!!! Someone is attacking my-" */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 40);
	if(Kolima_MiaInParty != 0) {
		fdScriptWait(40);
		fdActorSetAttachedEffect(
			ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		/* "I... I can't... I can't even stand up!" */
		scKolimaShowMessageAndWait(ACTOR_MIA, 40);
	} else {
		FieldStateWork->msg++;
	}
	fdScriptWait(20);
	if(Kolima_MiaInParty != 0) {
		fdActorGet(ACTOR_MIA)->velY = toFIXED(2);
		fdScriptWait(10);
		fdActorSetSpeedFX(ACTOR_MIA, toFIXED(2), toFIXED(2));
		fdActorSetDestinationOffset(ACTOR_MIA, -2, 0);
		fdActorSetScript(ACTOR_MIA, Kolima_PartyRestoreScaleScript);
		fdActorSetSprFlags(fdActorGet(ACTOR_MIA), 0);
		fdActorSetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
		fdScriptWait(10);
	}
	fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(2);
	fdScriptWait(10);
	fdActorSetSpeedFX(ACTOR_PARTY_LEADER, toFIXED(2), toFIXED(2));
	restoreScaleActions = Kolima_PartyRestoreScaleScript;
	fdActorSetScript(ACTOR_PARTY_LEADER, restoreScaleActions);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
	fdScriptWait(20);
	fdActorGet(ACTOR_GARET)->velY = toFIXED(2);
	fdScriptWait(10);
	fdActorSetSpeedFX(ACTOR_GARET, toFIXED(2), toFIXED(2));
	fdActorSetScript(ACTOR_GARET, restoreScaleActions);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
	fdScriptWait(40);
	fdActorGet(ACTOR_IVAN)->velY = toFIXED(2);
	fdScriptWait(10);
	fdActorSetScript(ACTOR_IVAN, restoreScaleActions);
	fdActorSetSprFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
	Kolima_SparkleSoundOn = 0;
	fdScriptWait(160);
	syCloseEvent(scKolimaSpawnCurseSparkle);
	fdScriptWait(120);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	Kolima_CurseDustWidth = 0;
	Kolima_CurseDustOffset = toFIXED(128);
	Kolima_CurseDustState = KOLIMA_DUST_CLOSING;
	sySetFrameEvent(scKolimaUpdateCurseDust, EVENT_PASS_FRAME);
	fdScriptWait(180);
	sySndPlay(MUS_FOREST_THEME);
	/* "What is that? Something's falling from the sky!" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 80);
	/* "It's sparkling! Could this be what turned those people into..." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 40);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	/* "We've got to get out of here!" */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
	Kolima_CurseDustState = KOLIMA_DUST_CLOSING_WIDE;
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 1);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 3);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "I can't! I can't move!!" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	if(Kolima_MiaInParty != 0) {
		fdActorSetAttachedEffect(
			ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		/* "Oh, no... If it touches us, we'll turn into trees!" */
		scKolimaShowMessageAndWait(ACTOR_MIA, 10);
	} else {
		FieldStateWork->msg++;
	}
	Kolima_CurseDustState = KOLIMA_DUST_DRIFTING;
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
	fdActorSetSprPriority(ACTOR_GARET, 3);
	fdActorSetSprPriority(ACTOR_IVAN, 3);
	fdActorSetSprPriority(ACTOR_MIA, 3);
	Kolima_PsynergyFieldHit = 0;
	sySetFrameEvent(scKolimaFlickerPsynergyFields, EVENT_PASS_FRAME);
	sySndPlay(SE_SHIMMER);
	fdActorGet(ACTOR_LEADER_PSYNERGY_FIELD)->priFlg &=
		~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_LEADER_PSYNERGY_FIELD, 2);
	fdActorSetPos(ACTOR_LEADER_PSYNERGY_FIELD, 120, 248);
	growFieldActions = Kolima_PsynergyFieldGrowActions;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldActions);
	fdActorGet(ACTOR_IVAN_PSYNERGY_FIELD)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_IVAN_PSYNERGY_FIELD, 2);
	fdActorSetPos(ACTOR_IVAN_PSYNERGY_FIELD, 100, 274);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldActions);
	if(Kolima_MiaInParty != 0) {
		fdActorGet(ACTOR_MIA_PSYNERGY_FIELD)->priFlg &=
			~ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSprPriority(ACTOR_MIA_PSYNERGY_FIELD, 2);
		fdActorSetPos(ACTOR_MIA_PSYNERGY_FIELD, 74, 254);
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldActions);
	}
	fdActorGet(ACTOR_GARET_PSYNERGY_FIELD)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_GARET_PSYNERGY_FIELD, 2);
	fdActorSetPos(ACTOR_GARET_PSYNERGY_FIELD, 94, 225);
	fdActorSetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldActions);
	while(Kolima_CurseDustState != KOLIMA_DUST_CENTRED) {
		sySynch(1);
	}
	fdScriptWait(300);
	syCloseEvent(scKolimaUpdateCurseDust);
	fdScriptWait(120);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	fdActorStop(ACTOR_LEADER_PSYNERGY_FIELD);
	fdActorStop(ACTOR_IVAN_PSYNERGY_FIELD);
	if(Kolima_MiaInParty != 0) {
		fdActorStop(ACTOR_MIA_PSYNERGY_FIELD);
	}
	fdActorStop(ACTOR_GARET_PSYNERGY_FIELD);
	sySynch(1);
	shrinkFieldActions = Kolima_PsynergyFieldShrinkActions;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldActions);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldActions);
	if(Kolima_MiaInParty != 0) {
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldActions);
	}
	fdActorRunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldActions);
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(40);
	/* "Isaac... What just happened to us? That barrier... Did it save..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	fdActorSetPos(ACTOR_TRET, 87, 139);
	fdActorSetPos(ACTOR_LAUREL, 87, 139);
	sySynch(1);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_TRET, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	/* Yes: "I think so, too, Isaac. I think I know what those domes..." No:
	   "No, I think Garet is right. I think I know what those..." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
	if(Kolima_MiaInParty != 0) {
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(10);
		fdScriptSetMessageId(MsgKolimaKnowThoseFieldsWere);
		/* "You know what those fields were?" */
		scKolimaShowMessageAndWait(ACTOR_MIA, 40);
	}
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	/* The companions discover that their Psynergy formed the barriers. */
	fdScriptSetMessageId(MsgKolimaWasOurPsynergy);
	/* "It was our Psynergy." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 3);
	fdScriptWait(40);
	fdActorSetSprPriority(ACTOR_GARET, 2);
	fdActorGet(ACTOR_GARET)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_GARET, 6, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 60);
	/* "What are you, crazy?" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	/* "I didn't use any Psynergy!" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(40);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 40);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	fdActorJump(ACTOR_GARET, 2, 0);
	fdScriptWait(40);
	fdActorJump(ACTOR_GARET, 2, 0);
	fdScriptWait(10);
	fdActorJump(ACTOR_GARET, 4, 0);
	fdScriptWait(20);
	/* "Hey... I can stand... I can move again!" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	if(Kolima_MiaInParty != 0) {
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdScriptWait(60);
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(80);
		fdActorSetSprPriority(ACTOR_MIA, 2);
		fdActorGet(ACTOR_MIA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSprFlags(fdActorGet(ACTOR_MIA), SPRITE_FLAG_SHADOW);
		fdActorJump(ACTOR_MIA, 4, 0);
		fdActorSetDestinationOffset(ACTOR_MIA, -2, 0);
		fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
		scKolimaFaceActorAndWait(ACTOR_MIA, FACING_NORTHEAST, 60);
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(20);
		/* "You're right. I can stand, too..." */
		scKolimaShowMessageAndWait(ACTOR_MIA, 20);
	} else {
		FieldStateWork->msg++;
	}
	fdActorJump(ACTOR_GARET, 2, 0);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	/* "Ivan, it's okay! Go ahead! Stand up!" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdActorSetSprPriority(ACTOR_IVAN, 2);
	fdActorGet(ACTOR_IVAN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprFlags(fdActorGet(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_IVAN, 4, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(10);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(60);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "Those barriers..." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	/* "We thought that Psynergy was only used when we concentrate,..." */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		FieldStateWork->msg++;
	} else {
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
		fdActorStartRepeatedMotion(ACTOR_GARET, 2);
		/* "You're such a liar, Isaac!" */
		fdScriptShowMessage(ACTOR_GARET, 0);
	}
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	/* "When I was training, I'd focus so hard, I thought my head..." */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "Well, maybe it's just easier for you, but when I use Psynergy..." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 10);
	if(Kolima_MiaInParty != 0) {
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
		scKolimaFaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
		fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
		/* "But what? The only way you can use Psynergy is by concentrating!" */
		scKolimaShowMessageAndWait(ACTOR_MIA, 10);
	} else {
		FieldStateWork->msg++;
	}
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	/* "Anyway, I'm certain that was Psynergy we were projecting." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "That must mean that some Psynergy acts only when it's needed." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 10);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "You mean, like when we're in danger?" */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "I think it's like when we strike critical hits in battle..." */
	scKolimaShowMessageAndWait(ACTOR_GARET, 20);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	/* "I can agree with what Garet is saying. It is definitely..." */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
	/* "Wouldn't it be something if we could use this power at any..." */
	fdScriptOpenMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	} else {
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(40);
		FieldStateWork->msg++;
	}
	/* Yes: "Yeah! Let's start trying to master this power!" No: "You... You're
	   not really into this, are you?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	sySndPlay(MUS_FOREST_THEME);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	Kolima_CurseDustWidth = 0;
	Kolima_CurseDustOffset = toFIXED(128);
	Kolima_CurseDustState = KOLIMA_DUST_CLOSING;
	sySetFrameEvent(scKolimaUpdateCurseDust, EVENT_PASS_FRAME);
	fdScriptWait(80);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(60);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
	/* A second shower raises the domes again before Tret and Laurel speak. */
	fdScriptSetMessageId(MsgKolimaWatchOutItsHappeningAgain);
	/* "Watch out! It's happening again!" */
	scKolimaShowMessageAndWait(ACTOR_IVAN, 10);
	scKolimaFaceActorAndWait(ACTOR_GARET, FACING_NORTH, 10);
	scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
	if(Kolima_MiaInParty != 0) {
		scKolimaFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
	}
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
	fdActorSetSprPriority(ACTOR_GARET, 3);
	fdActorSetSprPriority(ACTOR_IVAN, 3);
	fdActorSetSprPriority(ACTOR_MIA, 3);
	Kolima_CurseDustState = KOLIMA_DUST_CLOSING_WIDE;
	sySndPlay(SE_SHIMMER);
	fdActorSetPos(ACTOR_LEADER_PSYNERGY_FIELD, 120, 248);
	growFieldActions = Kolima_PsynergyFieldGrowActions;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldActions);
	fdActorSetPos(ACTOR_IVAN_PSYNERGY_FIELD, 100, 274);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldActions);
	if(Kolima_MiaInParty != 0) {
		fdActorSetPos(ACTOR_MIA_PSYNERGY_FIELD, 74, 254);
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldActions);
	}
	fdActorSetPos(ACTOR_GARET_PSYNERGY_FIELD, 94, 225);
	fdActorSetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldActions);
	fdScriptWait(120);
	Kolima_CurseDustState = KOLIMA_DUST_DRIFTING;
	do {
		sySynch(1);
	} while(Kolima_CurseDustState != KOLIMA_DUST_CENTRED);
	/* Tret and Laurel, unseen, wonder at the party's immunity to the glamour.
	   "Hroom! Why? Why don't they turn into trees...?" */
	scKolimaShowMessageAndWait(ACTOR_TRET, 80);
	/* "Who are they?" */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	/* "What people are these, who are immune to Tret's glamour?" */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 20);
	scNodTogether();
	/* "Have they come to save that village with their strange powers?" */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	scNodTogether();
	/* "It will not be enough to save the people of Kolima." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(40);
	/* "They have earned their fate...Kolima took their axes to me,..." */
	scKolimaShowMessageAndWait(ACTOR_TRET, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 40);
	/* "You children may not know this...but we are dying." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 80);
	/* "And Kolima Forest will wither and die when we are gone." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 40);
	/* "We are not the only ones who will die, Laurel. Once we are..." */
	scKolimaShowMessageAndWait(ACTOR_TRET, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptWait(60);
	/* "Tret was once a kindly forest king." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	scNodTogether();
	/* "But he is slowly dying, and his heart has torn in two..." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
	/* "His wrathful side turned the people of Kolima into trees." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_MIA, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	/* "The kind Tret speaks no more." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
	/* "Hroom! The kindly heart invites destruction... I need no..." */
	scKolimaShowMessageAndWait(ACTOR_TRET, 20);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	/* "If you wish to save those whom Tret turned into trees, you..." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scKolimaFaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
	/* "But...if you cannot stop Tret from withering, we all will..." */
	scKolimaShowMessageAndWait(ACTOR_LAUREL, 10);
	scNodTogether();
	fdScriptWait(60);
	/* Laurel's and Tret's last words; then the party decides whether to help.
	   "Look for Tret deep within the forest." */
	fdScriptShowMessage(ACTOR_LAUREL, 0);
	/* "It's no use, Laurel. Humans cannot be trusted! I shall die..." */
	fdScriptShowMessage(ACTOR_TRET, 0);
	syCloseEvent(scKolimaUpdateCurseDust);
	fdScriptWait(80);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	sySynch(80);
	fdActorStop(ACTOR_LEADER_PSYNERGY_FIELD);
	fdActorStop(ACTOR_IVAN_PSYNERGY_FIELD);
	miaInParty = &Kolima_MiaInParty;
	fdActorStop(ACTOR_MIA_PSYNERGY_FIELD);
	fdActorStop(ACTOR_GARET_PSYNERGY_FIELD);
	sySynch(1);
	shrinkFieldActions = Kolima_PsynergyFieldShrinkActions;
	fdActorSetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldActions);
	fdActorSetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldActions);
	if(*miaInParty != 0) {
		fdActorSetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldActions);
	}
	fdActorRunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldActions);
	fdScriptWait(20);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	fdActorSetSprPriority(ACTOR_GARET, 2);
	fdActorSetSprPriority(ACTOR_IVAN, 2);
	fdActorSetSprPriority(ACTOR_MIA, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
	/* "Isaac... Isn't there anything we can do?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdScriptWait(10);
		/* "What can we do? Do you have any idea?" */
		fdScriptOpenMessage(ACTOR_GARET, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
			scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
			fdScriptWait(40);
			scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
			/* "It looks like Isaac has something in mind." */
			scKolimaShowMessageAndWait(ACTOR_GARET, 10);
			scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
			scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
			/* "We'll leave it up to you, Isaac. Let's go save everyone." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
			scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
		} else {
			scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
			scKolimaFaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
			fdScriptSetMessageId(MsgKolimaHesAsStumpedAsWe);
			/* "He's as stumped as we are, but he says he'll help..." */
			scKolimaShowMessageAndWait(ACTOR_GARET, 20);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
			/* "I trust Isaac. He'll come up with an idea." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
		}
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	} else {
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdScriptWait(10);
		fdScriptSetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
		/* "You're right, Isaac... There isn't much we can do now." */
		scKolimaShowMessageAndWait(ACTOR_GARET, 10);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
		scKolimaFaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		/* "So the people of Kolima will just remain trees until they..." */
		fdScriptOpenMessage(ACTOR_IVAN, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
			fdScriptWait(20);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
			fdScriptWait(40);
			scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
			/* "I'm disappointed in you, Isaac... How can you give up so..." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 10);
			if(Kolima_MiaInParty != 0) {
				scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
				fdActorStartRepeatedMotion(ACTOR_MIA, 3);
				/* "That's right... We can't just leave them to rot!" */
				scKolimaShowMessageAndWait(ACTOR_MIA, 20);
			} else {
				FieldStateWork->msg++;
			}
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetAttachedEffect(
				ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(40);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			/* "But how can we bring back the nice Tret and save the
			   village?" */
			scKolimaShowMessageAndWait(ACTOR_GARET, 20);
			fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			fdScriptWait(120);
			/* "I guess I really don't know what to do." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 40);
			if(Kolima_MiaInParty != 0) {
				scKolimaFaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
				fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
				/* "Ivan, you can't give up! Please, convince him to help!" */
				scKolimaShowMessageAndWait(ACTOR_MIA, 10);
			} else {
				FieldStateWork->msg++;
			}
			fdScriptWait(60);
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			if(Kolima_MiaInParty != 0) {
				scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTHWEST, 40);
				scKolimaFaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
			}
			/* "Just promise me this one thing..." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 10);
			fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			fdScriptWait(40);
			/* "Promise you will do everything in your power to save Kolima." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
			fdScriptWait(20);
			fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		} else {
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			fdScriptWait(40);
			fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
			fdScriptSetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
			/* "That's a relief, Isaac. I thought you'd given up on
			   Kolima..." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
			if(Kolima_MiaInParty != 0) {
				scKolimaFaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
				fdActorStartRepeatedMotion(ACTOR_MIA, 1);
				/* "I was beginning to lose faith in you, Isaac." */
				scKolimaShowMessageAndWait(ACTOR_MIA, 20);
			} else {
				FieldStateWork->msg++;
			}
			fdActorSetAttachedEffect(
				ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(40);
			fdActorRunRepeatedMotion(ACTOR_GARET, 2);
			/* "But what can we do? How can we bring back the good Tret
			   and..." */
			scKolimaShowMessageAndWait(ACTOR_GARET, 20);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
			fdScriptWait(80);
			/* "I really don't have any idea right now..." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 40);
			if(Kolima_MiaInParty != 0) {
				scKolimaFaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 20);
				fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
				/* "I'm not sure... What should we do?" */
				scKolimaShowMessageAndWait(ACTOR_MIA, 40);
			} else {
				FieldStateWork->msg++;
			}
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			fdScriptWait(20);
			/* "I don't know just yet... But I'm sure we'll think of
			   something." */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
			fdActorStartRepeatedMotion(ACTOR_GARET, 2);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdScriptWait(40);
			/* "Well, let's get on with it!" */
			scKolimaShowMessageAndWait(ACTOR_IVAN, 20);
		}
	}
	/* Reunite the party and remember that the first visit has played. */
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_TRET, 0, 0);
	fdActorSetPos(ACTOR_LAUREL, 0, 0);
	if(Kolima_MiaInParty != 0) {
		fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(
				ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_MIA);
		fdActorSetPos(ACTOR_MIA, 0, 0);
	}
	gmFlagSet(FLAG_KOLIMA_CURSE_WITNESSED);
	gmPlayMusic();
	fdScriptEnd();
}

/* Shows the script's next line, spoken by actorId, then waits waitFrames. */
static void scKolimaShowMessageAndWait(Sint32 actorId, Sint32 waitFrames)
{
	fdScriptShowMessage(actorId, 0);
	fdScriptWait(waitFrames);
}

/* Turns an actor at once to facing, then waits waitFrames. */
static void scKolimaFaceActorAndWait(
	Sint32 actorId, Sint32 facing, Sint32 waitFrames)
{
	fdActorFaceDirection(actorId, facing, 0);
	fdScriptWait(waitFrames);
}

#include "shared/kolima_grow.c"

/* Every eighth frame, a golden ring of the curse falls from the sky over the
   party, with its sound while Kolima_SparkleSoundOn is set. */
static void scKolimaSpawnCurseSparkle(void)
{
	ACTOR *sparkle;
	SPR	  *sprite;
	Sint32 framePhase;

	framePhase = FrameCount & 7;
	if(framePhase != 0) {
		return;
	}
	if(Kolima_SparkleSoundOn != 0) {
		sySndPlay(SE_SPARKLE_FALL);
	}
	sparkle =
		syFieldActorCreate(SPRITE_GOLDEN_RINGS, toFIXED(98), 0, toFIXED(105));
	if(sparkle == NULL) {
		return;
	}
	sprite = sparkle->spr;
	sprite->flg = framePhase;
	sparkle->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->image.pri = 1;
	sparkle->scaleX = toFIXED(1) / 10;
	sparkle->speedLimit = toFIXED(8);
	sparkle->acceleration = toFIXED(8);
	sparkle->motionFlg = framePhase;
	fdActorSelectAnimation(sparkle, 2);
	syFieldActorSetMoveTargetFX(sparkle, toFIXED(98), 0, toFIXED(269));
	fdActorStartScript(sparkle, Kolima_GoldenRingsScript);
}

#include "shared/kolima_blink.c"

/* A speck of the curse that lands inside the party's Psynergy fields (two boxes
   with Mia in the party, three without her) bursts there with a drip, and the
   fields take their next pose once grown (scKolimaFlashHitPsynergyFields). */
static Sint32 scKolimaBurstOnPsynergyField(ACTOR *actor)
{
	if(Kolima_MiaInParty != 0) {
		if((actor->x.fixed > toFIXED(59) && actor->x.fixed < toFIXED(141) &&
			   actor->z.fixed > toFIXED(211) &&
			   actor->z.fixed < toFIXED(257)) ||
			(actor->x.fixed > toFIXED(69) && actor->x.fixed < toFIXED(122) &&
				actor->z.fixed > toFIXED(194) &&
				actor->z.fixed < toFIXED(277))) {
			sySndPlay(SE_DRIP);
			fdActorStartScript(actor, Kolima_SparkleBurstScript);
			Kolima_PsynergyFieldHit = 1;
		}
	} else {
		if((actor->x.fixed > toFIXED(59) && actor->x.fixed < toFIXED(111) &&
			   actor->z.fixed > toFIXED(194) &&
			   actor->z.fixed < toFIXED(230)) ||
			(actor->x.fixed > toFIXED(111) && actor->x.fixed < toFIXED(141) &&
				actor->z.fixed > toFIXED(216) &&
				actor->z.fixed < toFIXED(250)) ||
			(actor->x.fixed > toFIXED(78) && actor->x.fixed < toFIXED(122) &&
				actor->z.fixed > toFIXED(241) &&
				actor->z.fixed < toFIXED(277))) {
			sySndPlay(SE_DRIP);
			fdActorStartScript(actor, Kolima_SparkleBurstScript);
			Kolima_PsynergyFieldHit = 1;
		}
	}
	return 0;
}

#include "shared/kolima_curse_dust.c"
Sint32 Kolima_PsynergyFieldHit BSS_DATA;
Sint32 Kolima_MiaInParty	   BSS_DATA;
Sint32 Kolima_SparkleSoundOn   BSS_DATA;

/* Sets up Kolima Bridge. */
static void scKolimaRestoreBridgeState(void)
{
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		fdActorSetPos(ACTOR_BRIDGE_VILLAGER, 0, 0);
		fdMapGridCopyMetatileAttributes(9, 17, 5, 1, 9, 18);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	} else {
		fdActorSetPos(ACTOR_BRIDGE_VILLAGER_AS_TREE, 0, 0);
	}
	if(gmFlagGet(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
		fdMapGridCopyMetatilesTo(91, 19, 72, 9, 5, 7);
		fdMapGridCopyMetatileAttributes(23, 11, 5, 7, 8, 11);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
}

/* Where Garet, Ivan and Mia walk on into the silent village and the way each
   turns (scKolimaWitnessForestCurse). */
Sint32 Kolima_GaretWalksIn[] = {
	SC_ANIMATION(2),
	SC_MOVE(97, 0, 221),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_ANIMATION(1),
	SC_END,
};

Sint32 Kolima_IvanWalksIn[] = {
	SC_ANIMATION(2),
	SC_MOVE(101, 0, 268),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_ANIMATION(1),
	SC_END,
};

Sint32 Kolima_MiaWalksIn[] = {
	SC_ANIMATION(2),
	SC_MOVE(78, 0, 247),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH),
	SC_ANIMATION(1),
	SC_END,
};

#include "shared/kolima_squash.c"

Sint32 Kolima_PsynergyFieldGrowActions[] = {
	SC_SET(SW_SCALE_X, toFIXED(1) / 10),
	SC_SET(SW_SCALE_Y, toFIXED(1) / 10),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 50),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 50),
	SC_WAIT(1),
	SC_LOOP(70, SC_TO(1)),
	SC_CALL(scKolimaFlashHitPsynergyFields),
	SC_END,
};

Sint32 Kolima_PsynergyFieldShrinkActions[] = {
	SC_ANIMATION(1),
	SC_ADD(SW_Z, -toFIXED(1) / 10),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 200),
	SC_ADD(SW_SCALE_Y, -toFIXED(3) / 200),
	SC_WAIT(1),
	SC_LOOP(90, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

/* The sparkles on Kolima's ground: three quarters of their size, passed
   through, twinkling at random (scKolimaTwinkleGroundSparkle). */
Sint32 Kolima_GroundSparkleActions[] = {
	SC_SET(SW_SCALE_X, toFIXED(3) / 4),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 4),
	SC_SET(SW_COLLISION, 0),
	SC_SET(SW_TIMER, 0),
	SC_LABEL(1),
	SC_CALL(scKolimaTwinkleGroundSparkle),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 Kolima_DebrisScript[] = {
	SC_CALL(scKolimaUpdateFallingDebris),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 Kolima_FaceLeaderScript[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A door opening: two pictures of a metatile column two metatiles high. */
Uint16 Kolima_DoorMetatiles[] = {
	MAPGRID_ANIMATION_STEP(66, 32, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(67, 32, 1, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};

/* Kolima's entrances. No exit leads to entrances 2, 10 and 12. Entrance 10 puts
   the party where KOLIMA_FROM_WORLD_MAP does. */
SCENE_ENTRANCE KolimaEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 232, 0, 120, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{KOLIMA_FROM_WORLD_MAP, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1,
		-1, -1, -1, 0},
	{2, CONDITION_ALWAYS, 418, 0, 285, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{KOLIMA_FROM_INN, CONDITION_ALWAYS, 119, 0, 182, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{KOLIMA_FROM_ITEM_SHOP, CONDITION_ALWAYS, 151, 0, 310, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{KOLIMA_FROM_HOUSE_SOUTH, CONDITION_ALWAYS, 264, 0, 373, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{KOLIMA_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 329, 0, 230,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KOLIMA_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 263, 0, 326, FACING_NORTH, 0,
		-1, -1, -1, -1, 0},
	{KOLIMA_FROM_KOLIMA_TUNNEL, CONDITION_ALWAYS, 360, 0, 346, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{10, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{12, CONDITION_ALWAYS, 232, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE KolimaBridgeEntrances[] = {
	{KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 170, 0, 386,
		FACING_NORTH, 0, 0, 0, 288, 416, 0},
	{KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 168, 0, 50,
		FACING_SOUTH, 0, 48, 0, 288, 416, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE KolimaTunnelEntrances[] = {
	{KOLIMA_TUNNEL_FROM_LOG_ROOMS_C, CONDITION_ALWAYS, 216, 0, 320,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KOLIMA_TUNNEL_FROM_KOLIMA, CONDITION_ALWAYS, 312, 0, 312, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* clang-format off */
SCENE_EXIT_REGION KolimaTunnelRegions[] = {
	{207, 31, 311, 225, 49, 329, -1, 1},
	{303, 31, 303, 321, 49, 321, -1, 2},
	{-1, 0, 0, 0, 0, 0, 0, 0},
};

/* Kolima's exits, then Kolima Bridge's and Kolima Tunnel's. */
Uint32 KolimaExits[] = {
	SCENE_EXITS(Kolima),
	SCENE_EXIT(KOLIMA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KOLIMA_WEST),
	SCENE_EXIT(KOLIMA_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_KOLIMA_EAST),
	SCENE_EXIT(KOLIMA_TO_INN, LogRoomsC, LOG_ROOMS_C_INN_FROM_KOLIMA),
	SCENE_EXIT(KOLIMA_TO_ITEM_SHOP, LogRoomsC, LOG_ROOMS_C_ITEM_SHOP_FROM_KOLIMA),
	SCENE_EXIT(KOLIMA_TO_HOUSE_SOUTH, LogRoomsC, LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH),
	SCENE_EXIT(KOLIMA_TO_WEAPON_AND_ARMOR_SHOP, LogRoomsC, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA),
	SCENE_EXIT(KOLIMA_TO_HOUSE_NORTH, LogRoomsC, LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH),
	SCENE_EXIT(KOLIMA_TO_KOLIMA_TUNNEL, KolimaTunnel, KOLIMA_TUNNEL_FROM_KOLIMA),
	SCENE_EXITS(KolimaBridge),
	SCENE_EXIT(KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_SOUTH),
	SCENE_EXIT(KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_NORTH),
	SCENE_EXITS(KolimaTunnel),
	SCENE_EXIT(KOLIMA_TUNNEL_TO_LOG_ROOMS_C, LogRoomsC, LOG_ROOMS_C_FROM_KOLIMA_TUNNEL),
	SCENE_EXIT(KOLIMA_TUNNEL_TO_KOLIMA, Kolima, KOLIMA_FROM_KOLIMA_TUNNEL),
	SCENE_EXITS_END,
};

ACTOR_PLACE KolimaTunnelActorPlacements[] = {
	{ SCENE_TABLE_END },
};

/* Kolima's people: Garet, Ivan and Mia for the curse, the nine villagers, */
/* Tret's and Laurel's voices and the four domes until the curse has been */
/* seen (FLAG_KOLIMA_CURSE_WITNESSED), four sparkles while it lasts, and */
/* Granite until it joins (its Djinni flag, FLAG_DJINNI(DJINNI_GRANITE)). */
ACTOR_PLACE KolimaActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 192, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		168, 0, 360, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		344, 0, 280, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		248, 0, 184, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 264, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 264, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_TRET, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LAUREL, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
		24, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
		120, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
		152, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
		40, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GRANITE), MOVEMENT_TYPE_STAND,
		328, 0, 360, FACING_EAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

/* Five more sparkles, placed by scInit once the curse has been seen */
/* and kept while it lasts. */
ACTOR_PLACE Kolima_MoreGroundSparklePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
		120, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
		296, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
		440, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
		296, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
		152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

/* Kolima Bridge's villager, as a man (ACTOR_BRIDGE_VILLAGER) and as a tree */
/* (ACTOR_BRIDGE_VILLAGER_AS_TREE), in the same place. */
/* scKolimaRestoreBridgeState puts one of them out of sight. */
ACTOR_PLACE KolimaBridgeActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		234, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		234, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

/* The cues Kolima and Kolima Tunnel share: the exits and doorways, the */
/* villagers' words once the curse is lifted and their thoughts before and */
/* after, the healer, Granite and two jars. */
/* */
/* Exits 1 and 2, which metatile 99 takes too, keep their numbers, since each */
/* scene's exit table gives them its own doors. Those are KOLIMA_TO_WORLD_MAP */
/* and KOLIMA_TO_WORLD_MAP_EAST in Kolima, KOLIMA_TUNNEL_TO_LOG_ROOMS_C and */
/* KOLIMA_TUNNEL_TO_KOLIMA in Kolima Tunnel. */
CUE KolimaCues[] = {
	{ CUE_EXIT, 99, CONDITION_ALWAYS, 1 },
	{ CUE_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, SCRIPT_REF(scKolimaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, SCRIPT_REF(scKolimaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, SCRIPT_REF(scKolimaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, SCRIPT_REF(scKolimaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), 9, CONDITION_ALWAYS, SCRIPT_REF(scKolimaEnterDoor) },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, KOLIMA_TO_KOLIMA_TUNNEL },
	{ CUE_TALK, ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaThisIsKolimaVillageOfLumberjacks) },
	{ CUE_TALK, ACTOR_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWeRelyOnTheForestsSo) },
	{ CUE_TALK, ACTOR_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYouKnowBeingATreeGives) },
	{ CUE_TALK, ACTOR_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaITurnedIntoATreeRight) },
	{ CUE_TALK, ACTOR_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaDontBlameTheVillagersForBeing) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWhoWouldTryToUseThe) },
	{ CUE_TALK, ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIDidntMindBeingATree) },
	{ CUE_TALK, ACTOR_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYawnTreesAreSoSleepy) },
	{ CUE_TALK, ACTOR_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), SCRIPT_REF(scKolimaTalkToHealer) },
	{ CUE_TALK, ACTOR_GRANITE, CONDITION_ALWAYS, SCRIPT_REF(scKolimaGraniteJoins) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaForAWhileIThoughtId) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaTheForestTreesAreAliveJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIWonderIfIdLiveLonger) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfThoseSparklesStartFallingAgain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIveBeenPoorAllMyLife) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaItsNotCalledTheHolyTree) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfWeWereAllTreesThered) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaNobodyYellsAtYouWhenYoure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfTheHolyTreeAloneCaused) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICouldSpeakIdTell) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_RELYING_ON_FORESTS, CONDITION_ALWAYS, MSGNO(MsgKolimaHowCouldAnyoneTurnAGirl) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TIME_TO_THINK, CONDITION_ALWAYS, MSGNO(MsgKolimaThisIsADreamItHas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SPARKLES_FELL, CONDITION_ALWAYS, MSGNO(MsgKolimaBeingUnableToMoveIsSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_MCCOYS_MONEY, CONDITION_ALWAYS, MSGNO(MsgKolimaIAmOldIveLivedA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, CONDITION_ALWAYS, MSGNO(MsgKolimaItsTheSacredTreesCurseWhy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, CONDITION_ALWAYS, MSGNO(MsgKolimaTheWaterAtMyFeetHuh) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SLEEPY_TREES, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICantMoveThenMom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgKolimaMaybeIfIMeditateHarderIll) },
	{ EXAMINE_CUE(EXAMINE_JAR), 100, FLAG_KOLIMA_APPLE, FIND_ITEM(ITEM_APPLE) },
	{ EXAMINE_CUE(EXAMINE_JAR), 101, FLAG_KOLIMA_HERB, FIND_ITEM(ITEM_HERB) },
	{ CUE_TABLE_END },
};

/* Kolima Bridge's cues: its two ends, the bridge's fall and the villager. */
CUE KolimaBridgeCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH },
	{ CUE_STEP, 10, FLAG_KOLIMA_BRIDGE_LOWERED, SCRIPT_REF(scKolimaLowerBridge) },
	{ CUE_TALK, ACTOR_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaForAWhileIThoughtNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaYouWouldntUnderstandButItsTruly) },
	{ CUE_TABLE_END },
};
/* clang-format on */

Sint32 Kolima_GoldenRingsScript[] = {
	SC_CALL(scFieldEffectStretchUntilArrived),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/kolima_sparkle_burst_script.c"

Sint32 Kolima_CurseDustScript[] = {
	SC_ADD(SW_Y, -toFIXED(2)),
	SC_CALL(scKolimaFlickerCurseDust),
	SC_WAIT(1),
	SC_ADD(SW_Y, -toFIXED(2)),
	SC_CALL(scKolimaFlickerCurseDust),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_Y, -toFIXED(1)),
	SC_CALL(scKolimaFlickerCurseDust),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(1)),
	SC_CALL(scKolimaBurstOnPsynergyField),
	SC_RELEASE,
};
