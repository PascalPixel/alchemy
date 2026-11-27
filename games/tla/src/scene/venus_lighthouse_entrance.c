// Outside the Venus Lighthouse, where the prologue hands the story to Jenna. Felix has left the
// group, and Jenna and Kraden come out of the lighthouse alone:
//
// - Two birds land below the door (scVenusLighthouseEntranceBirdsLand) and fly
//       off as the party walks past (scVenusLighthouseEntranceBirdsFlyOff).
// - Tolbi's soldiers and the workmen of Lalivero close in. Alex arrives, sends
//       Jenna and Kraden west to Idejima, throws two soldiers skyward on
// columns of water and drives the rest off
//       (scVenusLighthouseEntranceSoldiersAmbushParty).
// - On the road west a Ruffian left to wait for backup fights the two
//       (scVenusLighthouseEntranceRuffianWaitsForBackup). A lost battle in the
//       prologue wakes them by the door, with Alex come back for them
//       (scVenusLighthouseEntranceWakeUp).
#include "field/actor_script.h"
#include "field/call.h"
#include "field/script_inline.h"
#include "field/field.h"
#include "field/field_mode_script.h"
#include "field/field_sprite.h"
#include "game/game.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

void scVenusLighthouseEntranceBlastActorSkyward(Sint32 id);

// Set as Tolbi's soldiers ambush the party; the soldiers and workmen are gone from then on.
#define FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN    0x890
// Set as the birds land the first time the party comes out of the lighthouse.
#define FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED   0x891
// Set as the birds fly off; they are gone from then on.
#define FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF 0x892
// Set as the party comes back from beating the Ruffian who waited for backup; he then lies beaten
// by the road.
#define FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN 0x893

// The ways into the scene that its set-up answers, besides scene_exits.h's.
typedef enum VENUS_LIGHTHOUSE_ENTRANCE_ENTRANCE
{
	// The map's own way in, in the lighthouse's doorway at the top of its stairs: the party goes on
	// to it once Jenna takes over, and wakes there after a defeat.
	ENTRANCE_LIGHTHOUSE_DOOR      = 1,
	// On the path south of the plaza, between the bushes: where the debug menu's Venus Lighthouse
	// start puts the party (gmMainRoutineRun).
	ENTRANCE_PLAZA_PATH           = 10,
	// Further south, between the workmen and the soldiers.
	ENTRANCE_BY_WORKMEN           = 11,
	// On the west road, beside the Ruffian who waits for backup.
	ENTRANCE_WEST_ROAD            = 12,
	// Where the prologue hands the party to Jenna: Felix, Sheba and Piers leave it.
	ENTRANCE_JENNA_TAKES_OVER     = 90,
	// Back from the battle with the Ruffian who waited for backup.
	ENTRANCE_AFTER_RUFFIAN_BATTLE = 99
} VENUS_LIGHTHOUSE_ENTRANCE_ENTRANCE;

// The map metatiles the step cues answer, by the behaviour the map gives them: the row before the
// lighthouse door, either side of it (metatiles (17, 7) to (29, 7)), where the birds take off; the
// path below the door, metatiles (21, 15) to (25, 15), where the soldiers close in; the road west,
// metatiles (5, 14) to (5, 18), where the Ruffian waits; and the way east, metatiles (28, 24) to
// (28, 29), where Kraden turns Jenna back.
typedef enum VENUS_LIGHTHOUSE_ENTRANCE_METATILE
{
	METATILE_BESIDE_DOOR = 9,
	METATILE_BELOW_DOOR  = 10,
	METATILE_ROAD_WEST   = 11,
	METATILE_WAY_EAST    = 12
} VENUS_LIGHTHOUSE_ENTRANCE_METATILE;

// The scene's placed actors, in the rows of ActorPlacements.
typedef enum VENUS_LIGHTHOUSE_ENTRANCE_ACTOR_ID
{
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	ACTOR_ALEX,
	// The two birds that land by the door and fly off.
	ACTOR_WEST_BIRD,
	ACTOR_EAST_BIRD,
	// Tolbi's soldiers: the two Alex hurls skyward, the two who flee before him and the one sent to
	// Lalivero for backup.
	ACTOR_SHOUTING_SOLDIER,
	ACTOR_SURPRISED_SOLDIER,
	ACTOR_DOUBTING_SOLDIER,
	ACTOR_SOLDIER_CAPTAIN,
	ACTOR_SOLDIER_SENT_FOR_BACKUP,
	// The workmen of Lalivero who come looking for Sheba: four who run from Alex, and the one their
	// boss sends to the caves.
	ACTOR_SCOLDING_WORKMAN,
	ACTOR_INDIGNANT_WORKMAN,
	ACTOR_EAGER_WORKMAN,
	ACTOR_WORKMAN_BOSS,
	ACTOR_WORKMAN_SENT_TO_CAVES,
	// The Ruffian left on the road west to wait for backup.
	ACTOR_WAITING_RUFFIAN
} VENUS_LIGHTHOUSE_ENTRANCE_ACTOR_ID;

struct ACTOR;

// Starts the next dust puff at a random place near the given point, rising and drifting outwards.
void          scVenusLighthouseEntranceEmitDust(ACTOR *actor, Sint32 x, Sint32 y, Sint32 z);
// Loads the dust puffs' pictures and starts drawing them each tick.
void          scVenusLighthouseEntranceStartDust(void);
// Stops the dust puffs and frees their pictures and state.
void          scVenusLighthouseEntranceStopDust(void);
extern Uint32 VenusLighthouseEntranceBirdLands[];

// Each tick of the column of water: it rises, holds, hurls the soldier off the top of the screen,
// sinks, and its puddle dries away as the ground shakes.
void          scVenusLighthouseEntranceUpdateBlastColumn(void);
// Each tick, draws and moves the dust puffs still playing.
void          scVenusLighthouseEntranceUpdateDust(void);

void          scVenusLighthouseEntranceBirdsFlyOff(void);

void          scVenusLighthouseEntranceSoldiersAmbushParty(void);

void          scVenusLighthouseEntranceRuffianWaitsForBackup(void);

void          scVenusLighthouseEntrancePointToMeetingSpot(void);

// The dust puffs kept at once, each emitted in turn over the oldest, and the ticks a puff lasts.
#define DUST_PUFFS      96
#define DUST_PUFF_TICKS 16

// The dust puffs' state, kept in RamHeapSlots' menu runtime while they play: their pictures, the
// puffs each with its place and sprite, and the next to emit.
typedef struct DUST_PUFF_STATE
{
	Uint8       pictures[2048]; // decompressed, then loaded into the VRAM cache
	SPARK_STATE states[DUST_PUFFS];
	EFFECT_SPR  sprites[DUST_PUFFS];
	Sint16      next;           // the next puff to emit, taken in turn
	Uint16      tileBase;       // the pictures' first tile
	Sint16      resource;       // their VRAM cache entry
} DUST_PUFF_STATE;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

// Talking to the soldier in front of the three in a row, before the ambush: the screen turns blue,
// a column of water rises under him and throws him up off the top of the screen, as Alex's Psynergy
// does in the ambush, and the colours come back. The player never gets to talk to him: the soldiers
// stand south of the plaza, and the only way down to them is the path between the bushes, which the
// metatiles below the door, (21, 15) to (25, 15), span from side to side; stepping there starts the
// ambush, after which they are gone. Walking Jenna down either side stair from the lighthouse door
// reaches the plaza without starting it, and the bushes stop her there.
void scVenusLighthouseEntranceBlastActorWithPsynergy(void *soldierId)
{
	fdStartColorTransition(8, RGB(4, 4, 16));
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(fdActorGet(ACTOR_JENNA), CASTER_LOOK_CYCLING_COLORS);
	syFieldScriptWait(20);
	scVenusLighthouseEntranceBlastActorSkyward(soldierId);
	fdBattleEffectSetObjectUpdateMode(fdActorGet(ACTOR_JENNA), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdStartPaletteTransition(8);
}

// A running soldier's update: it kicks up dust where it is, with a scuff every eighth tick.
void scVenusLighthouseEntranceKickUpDust(ACTOR *actor)
{
	scVenusLighthouseEntranceEmitDust(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if (++actor->timer % 8 == 0)
		fdScriptPlaySound(SE_CHEST_CLOSE);
}

// Talking to the soldier captain, before the ambush: he dashes off north-east a few steps and back
// again, kicking up a trail of dust with a scuffing sound. The player never gets to talk to him,
// for the reason given at scVenusLighthouseEntranceBlastActorWithPsynergy.
void scVenusLighthouseEntranceRunAndKickUpDust(Sint32 actorId)
{
	ACTOR *actor = fdActorGet(actorId);

	scVenusLighthouseEntranceStartDust();
	actor->update        = scVenusLighthouseEntranceKickUpDust;
	actor->priorityFlags = 0;
	fdActorWalkByAndWait(actorId, 96, -96);
	fdActorWalkByAndWait(actorId, -96, 96);
	actor->update        = 0;
	actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
	syFieldScriptWait(30);
	scVenusLighthouseEntranceStopDust();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

void   scVenusLighthouseEntranceBirdsLand(void);
void   scVenusLighthouseEntranceWakeUp(void);

Sint32 scInit(void)
{
	ACTOR *ruffian;
	ACTOR *leader;

	FieldStateWork->transition  = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	leader                      = fdActorGet(MainRoutineStateWork.leaderActorId);
	leader->priorityFlags      |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorSetAnimation(ACTOR_WEST_BIRD, 5);
	scFieldActorSetSpritePriority(ACTOR_WEST_BIRD, 1);
	fdActorSetAnimation(ACTOR_EAST_BIRD, 5);
	scFieldActorSetSpritePriority(ACTOR_EAST_BIRD, 1);
	scFieldActorSetSpritePriority(ACTOR_DOUBTING_SOLDIER, 2);
	scFieldActorSetSpritePriority(ACTOR_EAGER_WORKMAN, 2);
	if (fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF))
	{
		fdActorSetPos(ACTOR_WEST_BIRD, 0, 0);
		fdActorSetPos(ACTOR_EAST_BIRD, 0, 0);
	}
	if (fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN))
	{
		fdActorSetPos(ACTOR_SHOUTING_SOLDIER, 0, 0);
		fdActorSetPos(ACTOR_SURPRISED_SOLDIER, 0, 0);
		fdActorSetPos(ACTOR_DOUBTING_SOLDIER, 0, 0);
		fdActorSetPos(ACTOR_SOLDIER_CAPTAIN, 0, 0);
		fdActorSetPos(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 0);
		fdActorSetPos(ACTOR_SCOLDING_WORKMAN, 0, 0);
		fdActorSetPos(ACTOR_INDIGNANT_WORKMAN, 0, 0);
		fdActorSetPos(ACTOR_EAGER_WORKMAN, 0, 0);
		fdActorSetPos(ACTOR_WORKMAN_BOSS, 0, 0);
		fdActorSetPos(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 0);
	}
	if (MainRoutineStateWork.entrance == VENUS_LIGHTHOUSE_ENTRANCE_FROM_VENUS_LIGHTHOUSE)
	{
		if (!fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED))
		{
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			scVenusLighthouseEntranceBirdsLand();
			fdScriptEnd();
		}
	}
	if (MainRoutineStateWork.entrance == VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT)
	{
		MainRoutineStateWork.returnEntrance = ENTRANCE_LIGHTHOUSE_DOOR;
		MainRoutineStateWork.entrance       = ENTRANCE_LIGHTHOUSE_DOOR;
		gmPartyRestoreHpPp(1);
		scVenusLighthouseEntranceWakeUp();
	}
	if (MainRoutineStateWork.entrance == ENTRANCE_AFTER_RUFFIAN_BATTLE)
	{
		fdScriptSetFlag(FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN);
	}
	if (MainRoutineStateWork.entrance == ENTRANCE_JENNA_TAKES_OVER)
	{
		MainRoutineStateWork.leaderActorId = ACTOR_JENNA;
		gmPartyRemoveMember(ACTOR_FELIX);
		gmPartyRemoveMember(ACTOR_SHEBA);
		gmPartyRemoveMember(ACTOR_PIERS);
		fdScriptSetDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE, ENTRANCE_LIGHTHOUSE_DOOR);
	}
	if (!fdScriptGetFlag(FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN))
	{
		fdActorSetAnimation(ACTOR_WAITING_RUFFIAN, 5);
		scFieldActorSetSpritePriority(ACTOR_WAITING_RUFFIAN, 1);
	}
	else
	{
		fdActorSetPos(ACTOR_WAITING_RUFFIAN, 40, 248);
		ruffian         = fdActorGet(ACTOR_WAITING_RUFFIAN);
		ruffian->facing = FACING_SOUTH;
		fdActorFallOverWithAnimationById(ACTOR_WAITING_RUFFIAN, FACING_SOUTH, 0);
		scFieldActorSetSpritePriority(ACTOR_WAITING_RUFFIAN, 3);
		fdActorSetMovementType(fdActorGet(ACTOR_WAITING_RUFFIAN), MOVEMENT_TYPE_SQUASH);
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

// As Jenna and Kraden first come out of the lighthouse, two birds fly down and land below the door.
void scVenusLighthouseEntranceBirdsLand(void)
{
	ACTOR *westBird;
	ACTOR *eastBird;

	fdScriptSetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED);
	westBird               = fdActorGet(ACTOR_WEST_BIRD);
	westBird->motionFlags  = 0;
	westBird->gravity      = toFIXED(1) / 100;
	westBird->speedLimit   = toFIXED(3) / 2;
	westBird->acceleration = toFIXED(1) / 2;
	scFieldActorSetPosAndResetMotion(westBird, toFIXED(216), toFIXED(96), toFIXED(176));
	eastBird               = fdActorGet(ACTOR_EAST_BIRD);
	eastBird->motionFlags  = 0;
	eastBird->gravity      = toFIXED(3) / 100;
	eastBird->speedLimit   = toFIXED(3) / 2;
	eastBird->acceleration = toFIXED(1) / 2;
	scFieldActorSetPosAndResetMotion(eastBird, toFIXED(616), toFIXED(96), toFIXED(152));
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(376, CAMERA_KEEP, 248, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	westBird = fdActorGet(ACTOR_WEST_BIRD);
	fdActorSetAnimation(ACTOR_WEST_BIRD, 4);
	fdScriptPlaySound(SE_BIRD_WINGS);
	scFieldActorSetMoveTarget(westBird, 352, 8, 176);
	fdActorStartScript(westBird, (Sint32)VenusLighthouseEntranceBirdLands);
	syFieldScriptWait(25);
	fdCameraMoveTo(376, CAMERA_KEEP, 104, 1);
	eastBird = fdActorGet(ACTOR_EAST_BIRD);
	fdActorSetAnimation(ACTOR_EAST_BIRD, 4);
	scFieldActorSetMoveTarget(eastBird, 416, 8, 152);
	fdActorStartScript(eastBird, (Sint32)VenusLighthouseEntranceBirdLands);
	syFieldScriptWait(60);
	fdScriptPlaySound(SE_BIRD_WINGS);
	scFieldActorWaitForMoveById(ACTOR_WEST_BIRD);
	scFieldActorWaitForMoveById(ACTOR_EAST_BIRD);
	fdCameraWaitForArrival();
}

extern Uint32 VenusLighthouseEntranceBirdFlies[];

// Walking past the lighthouse door, metatiles (17, 7) to (29, 7), the birds take off and fly away.
void          scVenusLighthouseEntranceBirdsFlyOff(void)
{
	ACTOR *westBird;
	ACTOR *eastBird;

	fdScriptSetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySound(SE_BIRD_WINGS);
	fdActorSetAnimation(ACTOR_WEST_BIRD, 3);
	scFieldActorSetAnimation(ACTOR_EAST_BIRD, 3);
	syFieldScriptWait(21);
	westBird               = fdActorGet(ACTOR_WEST_BIRD);
	westBird->motionFlags  = 0;
	westBird->speedLimit   = toFIXED(2);
	westBird->acceleration = toFIXED(1) / 2;
	fdActorSetAnimation(ACTOR_WEST_BIRD, 4);
	scFieldActorSetMoveTarget(westBird, 616, 96, 176);
	fdActorStartScript(westBird, (Sint32)VenusLighthouseEntranceBirdFlies);
	eastBird               = fdActorGet(ACTOR_EAST_BIRD);
	eastBird->motionFlags  = 0;
	eastBird->speedLimit   = toFIXED(2);
	eastBird->acceleration = toFIXED(1) / 2;
	fdActorSetAnimation(ACTOR_EAST_BIRD, 4);
	scFieldActorSetMoveTarget(eastBird, 120, 96, 176);
	Call2(fdActorStartScript, (Sint32)eastBird, (Sint32)VenusLighthouseEntranceBirdFlies);
	fdScriptEnd();
}

// Walking down the path below the lighthouse door, metatiles (21, 15) to (25, 15): Tolbi's soldiers
// and the workmen of Lalivero close in on Jenna and Kraden. Alex arrives, tells them to meet him at
// Idejima, throws two soldiers skyward on columns of water and sends the workmen running, then
// follows the last soldiers to Lalivero.
void scVenusLighthouseEntranceSoldiersAmbushParty(void)
{
	ACTOR *workman;
	ACTOR *actor;

	fdScriptSetFlag(FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgThereTheyAre);
	fdActorSetSpeed(ACTOR_SHOUTING_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_SURPRISED_SOLDIER, 1);
	// "There they are! Get 'em!"
	syFieldScriptShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 0);
	fdActorWalkToAndWait(ACTOR_JENNA, 376, 280);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(20);
	fdActorWalkTo(ACTOR_JENNA, 376, 360);
	syFieldScriptWait(30);
	fdCameraMoveTo(376, CAMERA_KEEP, 424, 1);
	fdCameraWaitForArrival();
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_SHOUTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "Iodem sent us to stand guard here, but..."
	syFieldScriptShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SURPRISED_SOLDIER, 2);
	syFieldScriptWait(10);
	// "I never thought we'd actually run into them!"
	syFieldScriptShowMessageThenWait(ACTOR_SURPRISED_SOLDIER, 0, 10);
	fdActorShowEmote(ACTOR_SCOLDING_WORKMAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	// "Scoundrels! What have you done with Sheba?"
	syFieldScriptShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
	scFieldActorJump(ACTOR_INDIGNANT_WORKMAN, 6, 15);
	scFieldActorJump(ACTOR_INDIGNANT_WORKMAN, 6, 25);
	// "Return her now, or you're really going to regret it!"
	syFieldScriptShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_NORTHWEST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "Um... Aren't there supposed to be more of them?"
	syFieldScriptShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
	syFieldScriptWait(10);
	// "Huh... Maybe they're just the first ones to come out..."
	syFieldScriptShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
	fdActorFaceDirection(ACTOR_EAGER_WORKMAN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "Great! That means we've got 'em outnumbered!"
	syFieldScriptShowMessageThenWait(ACTOR_EAGER_WORKMAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_WORKMAN_BOSS, ANIM_NOD);
	syFieldScriptWait(10);
	// "We can probably take 'em, you think?"
	syFieldScriptShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_EAGER_WORKMAN, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_SOLDIER_SENT_FOR_BACKUP, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "Go fetch the other Tolbi soldiers still stationed in Lalivero..."
	syFieldScriptShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, ANIM_NOD);
	syFieldScriptWait(10);
	// "Yes, sir!"
	syFieldScriptShowMessageThenWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 10);
	fdActorWalkByAndWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, 64, 0);
	fdActorSetPos(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 0);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_WORKMAN_SENT_TO_CAVES, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "And we'll go tell the guys in the caves!"
	syFieldScriptShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_WORKMAN_SENT_TO_CAVES, ANIM_NOD);
	syFieldScriptWait(10);
	// "Got it, Boss!"
	syFieldScriptShowMessageThenWait(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 10);
	fdActorWalkByAndWait(ACTOR_WORKMAN_SENT_TO_CAVES, -64, 64);
	fdActorSetPos(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 0);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_EAGER_WORKMAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHEAST, 0);
	syFieldScriptWait(30);
	fdCameraMoveTo(376, CAMERA_KEEP, 360, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, -16, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_ALEX, 16, -16, FACING_SOUTH);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_ALEX, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 16);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldScriptWait(20);
	// "How unfortunate... It looks like an ambush. What should we do, Jenna?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldScriptWait(30);
	// "What do you mean? Look around... We'll have to fight them!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "Are you serious?"
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 16);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "Jenna, are you really prepared to fight these men?"
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(10);
	// "We don't have any other choice. We have to..."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldScriptWait(10);
	// "Well, then. Allow me to shoulder some of your burden."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorSetSpeed(ACTOR_ALEX, 0.5);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 16);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "...Burden?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	// "We regroup along the road leading away from Lalivero. Do you understand?" Yes: "You don't
	// need to remind us, Alex." No: "It's quite simple, Jenna."
	((void (*)())fdMessageShowAtActor)(ACTOR_ALEX, 0);
	syFieldScriptWait(10);
	if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_JENNA, 0) == 0)
	{
		syFieldScriptWait(20);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
		syFieldScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		syFieldScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		syFieldScriptWait(30);
		fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
		syFieldScriptWait(20);
		// "You don't need to remind us, Alex. We know the place."
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		FieldStateWork->msg += 1;
	}
	else
	{
		syFieldScriptWait(30);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
		syFieldScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		FieldStateWork->msg += 1;
		// "It's quite simple, Jenna. The meeting place is in the direction..."
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	}
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(20);
	// "Follow this road west, and take the path down through the canyon..."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldScriptWait(10);
	// "On the other side of that cave, you will find a small peninsula..."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdCameraMoveTo(312, CAMERA_KEEP, 424, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	// "Thankfully, only those workmen block your route."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	// "And fortunately..."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	// "They don't look too tough..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST, 0);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	// "Hello! We can hear you!!!"
	syFieldScriptShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
	fdActorShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	// "That's just plain rude!"
	syFieldScriptShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
	fdCameraMoveTo(376, CAMERA_KEEP, 424, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	syFieldScriptWait(25);
	// "Why don't the two of you escape and leave the Tolbi soldiers to me?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "The two of us? Alone?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdCameraMoveTo(376, CAMERA_KEEP, 360, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	// "You can use Psynergy, can't you, Jenna?"
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "Um... Yes."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldScriptWait(10);
	// "Then you'll be fine."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	// "We shall reconvene at the peninsula."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdCameraMoveTo(376, CAMERA_KEEP, 408, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_ALEX, 0.5);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 16);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_EAST_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_EAGER_WORKMAN, FACING_EAST_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WORKMAN_BOSS, FACING_EAST_NORTHEAST, 0);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_SHOUTING_SOLDIER, 2);
	syFieldScriptWait(10);
	// "You think you can take on Tolbi's finest on your own, little man?"
	syFieldScriptShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	// "I do not wish to inflict unnecessary bloodshed."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SURPRISED_SOLDIER, 2);
	syFieldScriptWait(10);
	// "Don't wish to... What's that supposed to mean?"
	syFieldScriptShowMessageThenWait(ACTOR_SURPRISED_SOLDIER, 0, 10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_WEST_NORTHWEST, 0);
	syFieldScriptWait(25);
	// "If you lay a hand on these two, I assure you that you will be made..."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_WEST, 0);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_SHOUTING_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SURPRISED_SOLDIER, ANIM_NOD);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_NORTHWEST, 0);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_SHOUTING_SOLDIER, 1.2);
	fdActorSetSpeed(ACTOR_SURPRISED_SOLDIER, 1.2);
	fdActorWalkByAndWait(ACTOR_SHOUTING_SOLDIER, -26, 8);
	fdActorFaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_NORTH_NORTHWEST, 0);
	fdActorWalkByAndWait(ACTOR_SURPRISED_SOLDIER, -16, -12);
	fdActorFaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_WEST, 0);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldScriptWait(10);
	// "By advancing, I assume you mean to fight. Permit me to strike the..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_ALEX, 8);
	syFieldScriptWait(50);
	fdBattleEffectStartArcEffect();
	actor = fdActorGetById(ACTOR_ALEX);
	fdBattleEffectSetObjectUpdateMode(actor, CASTER_LOOK_CYCLING_COLORS);
	fdScriptPlaySound(SE_GATHERING_POWER);
	syFieldScriptWait(20);
	fdStartColorTransition(8, RGB(4, 4, 16));
	syFieldScriptWait(60);
	fdScriptPlaySound(SOUND_FADE_OUT_ACTION_SE);
	scFieldActorSetSpritePriority(ACTOR_SHOUTING_SOLDIER, 0);
	scVenusLighthouseEntranceBlastActorSkyward(ACTOR_SHOUTING_SOLDIER);
	fdActorShowEmote(ACTOR_SCOLDING_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_EAGER_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_WORKMAN_BOSS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SURPRISED_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_DOUBTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SOLDIER_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	scFieldActorSetSpritePriority(ACTOR_SURPRISED_SOLDIER, 1);
	scVenusLighthouseEntranceBlastActorSkyward(ACTOR_SURPRISED_SOLDIER);
	syFieldScriptWait(30);
	actor = fdActorGetById(ACTOR_ALEX);
	fdBattleEffectSetObjectUpdateMode(actor, CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdStartPaletteTransition(16);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_ALEX, 9);
	syFieldScriptWait(30);
	fdActorShowEmote(ACTOR_DOUBTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "What was that!?"
	syFieldScriptShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
	syFieldScriptWait(10);
	// "I don't know, but whatever he did, it's out of our league!"
	syFieldScriptShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 3);
	fdActorSetAttachedEffect(ACTOR_DOUBTING_SOLDIER,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 3);
	fdActorSetAttachedEffect(ACTOR_SOLDIER_CAPTAIN,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_WEST, 0);
	scFieldActorJump(ACTOR_DOUBTING_SOLDIER, 6, 15);
	scFieldActorJump(ACTOR_DOUBTING_SOLDIER, 6, 25);
	// "You guys deal with him until backup arrives!"
	syFieldScriptShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "Oh, so you're next?"
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorWalkByAndWait(ACTOR_ALEX, -16, 16);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
	fdCameraMoveTo(360, CAMERA_KEEP, 408, 1);
	fdCameraWaitForArrival();
	scFieldActorStartRepeatedMotion(ACTOR_SCOLDING_WORKMAN, 3);
	fdActorSetAttachedEffect(ACTOR_SCOLDING_WORKMAN,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_INDIGNANT_WORKMAN, 3);
	fdActorSetAttachedEffect(ACTOR_INDIGNANT_WORKMAN,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_EAGER_WORKMAN, 3);
	fdActorSetAttachedEffect(ACTOR_EAGER_WORKMAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_WORKMAN_BOSS, 3);
	fdActorSetAttachedEffect(ACTOR_WORKMAN_BOSS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_WORKMAN_SENT_TO_CAVES, 3);
	fdActorSetAttachedEffect(ACTOR_WORKMAN_SENT_TO_CAVES,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorSetAnimationAndWait(ACTOR_SCOLDING_WORKMAN, ANIM_SHAKE_HEAD);
	// "Hey, no need to worry about us!"
	syFieldScriptShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
	fdActorShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "We're not soldiers... We're not anybody!"
	syFieldScriptShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_EAGER_WORKMAN, ANIM_SHAKE_HEAD);
	// "Seriously, there's no way we're gonna face off against you."
	syFieldScriptShowMessageThenWait(ACTOR_EAGER_WORKMAN, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_WORKMAN_BOSS, 3);
	fdActorSetAttachedEffect(ACTOR_WORKMAN_BOSS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(55);
	fdActorFaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_EAGER_WORKMAN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHEAST, 0);
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_WORKMAN_BOSS, 2);
	syFieldScriptWait(10);
	// "Let's get outta here!"
	syFieldScriptShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
	scFieldActorJump(ACTOR_INDIGNANT_WORKMAN, 6, 0);
	scFieldActorJump(ACTOR_EAGER_WORKMAN, 6, 0);
	scFieldActorJump(ACTOR_WORKMAN_BOSS, 6, 25);
	scVenusLighthouseEntranceStartDust();
	fdActorSetSpeed(ACTOR_WORKMAN_BOSS, 1.8);
	fdActorSetSpeed(ACTOR_EAGER_WORKMAN, 1.8);
	fdActorSetSpeed(ACTOR_INDIGNANT_WORKMAN, 1.8);
	fdActorSetSpeed(ACTOR_SCOLDING_WORKMAN, 1.8);
	workman         = fdActorGetById(ACTOR_WORKMAN_BOSS);
	workman->timer  = 0;
	workman->update = scVenusLighthouseEntranceKickUpDust;
	workman         = fdActorGetById(ACTOR_EAGER_WORKMAN);
	workman->timer  = 0;
	workman->update = scVenusLighthouseEntranceKickUpDust;
	workman         = fdActorGetById(ACTOR_INDIGNANT_WORKMAN);
	workman->timer  = 0;
	workman->update = scVenusLighthouseEntranceKickUpDust;
	fdActorSetAnimation(ACTOR_WORKMAN_BOSS, 10);
	fdActorSetAnimation(ACTOR_EAGER_WORKMAN, 10);
	fdActorSetAnimation(ACTOR_INDIGNANT_WORKMAN, 10);
	fdActorSetDestinationOffset(ACTOR_WORKMAN_BOSS, -24, 24);
	fdActorSetDestinationOffset(ACTOR_EAGER_WORKMAN, -24, 24);
	fdActorSetDestinationOffset(ACTOR_INDIGNANT_WORKMAN, -24, 24);
	scFieldActorWaitForMoveById(ACTOR_INDIGNANT_WORKMAN);
	fdActorSetDestination(ACTOR_WORKMAN_BOSS, 224, 480);
	fdActorSetDestination(ACTOR_EAGER_WORKMAN, 224, 480);
	fdActorMoveToAndWait(ACTOR_INDIGNANT_WORKMAN, 224, 480);
	workman         = fdActorGetById(ACTOR_WORKMAN_BOSS);
	workman->update = NULL;
	workman         = fdActorGetById(ACTOR_EAGER_WORKMAN);
	workman->update = NULL;
	workman         = fdActorGetById(ACTOR_INDIGNANT_WORKMAN);
	workman->update = NULL;
	syFieldScriptWait(1);
	scFieldActorJump(ACTOR_SCOLDING_WORKMAN, 6, 15);
	scFieldActorJump(ACTOR_SCOLDING_WORKMAN, 6, 25);
	workman         = fdActorGetById(ACTOR_SCOLDING_WORKMAN);
	workman->timer  = 0;
	workman->update = scVenusLighthouseEntranceKickUpDust;
	fdActorSetAnimation(ACTOR_SCOLDING_WORKMAN, 10);
	fdActorSetDestinationOffset(ACTOR_SCOLDING_WORKMAN, -20, 20);
	scFieldActorWaitForMoveById(ACTOR_SCOLDING_WORKMAN);
	scFieldActorJump(ACTOR_SCOLDING_WORKMAN, 4, 0);
	fdActorSetDestinationOffset(ACTOR_SCOLDING_WORKMAN, -20, 20);
	scFieldActorWaitForMoveById(ACTOR_SCOLDING_WORKMAN);
	fdActorSetAnimation(ACTOR_SCOLDING_WORKMAN, ANIM_STAND);
	fdScriptPlaySound(SE_IMPACT);
	workman->x.fixed                         += toFIXED(8);
	workman->update                           = NULL;
	workman->z.fixed                         += -toFIXED(8);
	workman->facing                           = FACING_NORTH;
	((FIELD_SPR *)workman->sprite)->rotation  = TRIG_TURN * 5 / 8;
	syFieldScriptWait(55);
	workman->x.fixed                         += -toFIXED(4);
	workman->z.fixed                         += toFIXED(4);
	workman->facing                           = FACING_SOUTH;
	((FIELD_SPR *)workman->sprite)->rotation  = 0;
	scFieldActorJump(ACTOR_SCOLDING_WORKMAN, 6, 40);
	fdActorSetAnimation(ACTOR_SCOLDING_WORKMAN, 11);
	fdScriptPlaySound(SE_CLICK);
	syFieldScriptWait(14);
	fdScriptPlaySound(SE_CLICK);
	syFieldScriptWait(14);
	fdScriptPlaySound(SE_CLICK);
	syFieldScriptWait(14);
	fdScriptPlaySound(SE_CLICK);
	syFieldScriptWait(14);
	syFieldScriptWait(20);
	workman->timer  = 0;
	workman->update = scVenusLighthouseEntranceKickUpDust;
	fdActorSetAnimation(ACTOR_SCOLDING_WORKMAN, 10);
	fdActorMoveToAndWait(ACTOR_SCOLDING_WORKMAN, 224, 472);
	syFieldScriptWait(30);
	workman->update = NULL;
	fdActorSetPos(ACTOR_WORKMAN_BOSS, 0, 0);
	fdActorSetPos(ACTOR_EAGER_WORKMAN, 0, 0);
	fdActorSetPos(ACTOR_INDIGNANT_WORKMAN, 0, 0);
	fdActorSetPos(ACTOR_SCOLDING_WORKMAN, 0, 0);
	scVenusLighthouseEntranceStopDust();
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	syFieldScriptWait(10);
	// "How undignified... and how shameful."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdCameraMoveTo(376, CAMERA_KEEP, 408, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(15);
	// "Jenna, now is your chance. Go to Idejima and wait for me."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHWEST, 0);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_WEST, 0);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_DOUBTING_SOLDIER, 1.5);
	fdActorWalkByAndWait(ACTOR_DOUBTING_SOLDIER, -32, 0);
	actor         = fdActorGetById(ACTOR_ALEX);
	actor->facing = FACING_SOUTHEAST;
	// "I'm not finished with you yet."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	fdActorSetSpeed(ACTOR_ALEX, 1.5);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 16);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
	syFieldScriptWait(10);
	scFieldActorSetSpritePriority(ACTOR_DOUBTING_SOLDIER, 3);
	fdActorSetSpeed(ACTOR_DOUBTING_SOLDIER, 2);
	fdActorGetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_DOUBTING_SOLDIER, 456, 424);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHWEST, 0);
	fdActorSetSpeed(ACTOR_DOUBTING_SOLDIER, 1);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	// "You mentioned backup from Lalivero?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	fdActorSetSpeed(ACTOR_ALEX, 0.7);
	fdActorWalkByAndWait(ACTOR_ALEX, 48, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_SOLDIER_CAPTAIN, 1);
	fdActorGetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 456, 440);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags |= ACTOR_FACES_MOTION;
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "Let's go meet them together, shall we?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
	syFieldScriptWait(10);
	// "Hey, pal! When they get here, you'll really be in for it!"
	syFieldScriptShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
	fdActorWalkByAndWait(ACTOR_ALEX, 16, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	// "Do you honestly believe that even a hundred of you could stop me?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
	fdActorRunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_DOUBTING_SOLDIER, 0.5);
	fdActorSetSpeed(ACTOR_SOLDIER_CAPTAIN, 0.5);
	fdActorGetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_DOUBTING_SOLDIER, 488, 424);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags  &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 488, 440);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_ALEX, 456, 432);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	syFieldScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	// "How amusing... This I must see."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 3);
	fdActorSetAttachedEffect(ACTOR_DOUBTING_SOLDIER,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 3);
	fdActorSetAttachedEffect(ACTOR_SOLDIER_CAPTAIN,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(50);
	fdActorGetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_DOUBTING_SOLDIER, 504, 424);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_DOUBTING_SOLDIER)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorSetPos(ACTOR_DOUBTING_SOLDIER, 0, 0);
	fdActorGetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 504, 440);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_SOLDIER_CAPTAIN)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorSetPos(ACTOR_SOLDIER_CAPTAIN, 0, 0);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorWalkByAndWait(ACTOR_ALEX, 64, 0);
	fdActorSetPos(ACTOR_ALEX, 0, 0);
	syFieldScriptWait(30);
	fdCameraMoveToActorById(ACTOR_JENNA, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	// "They're gone..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "Come on, Jenna... Let's be going."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	syFieldScriptWait(10);
	scFieldActorJump(ACTOR_KRADEN, 4, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldScriptWait(20);
	// "There's nothing standing in our way now!"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "But..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "What is it now?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	// "Ah... You're worried about hurting any innocents?" Yes: "Well, just try to hold back when
	// you're fighting." No: "Oh. You must be concerned about Felix."
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_JENNA, 0) == 0)
	{
		syFieldScriptWait(20);
		// "Well, just try to hold back when you're fighting."
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		FieldStateWork->msg += 1;
	}
	else
	{
		syFieldScriptWait(40);
		FieldStateWork->msg += 1;
		// "Oh. You must be concerned about Felix. We can't do anything to..."
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	}
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(10);
	// "Let's get going! To the peninsula!"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGet(ACTOR_JENNA);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syScriptCameraFollowActor(ACTOR_JENNA, 1);
	fdScriptEnd();
}

// On the road west, metatiles (5, 14) to (5, 18), the Ruffian left to wait for backup sees that
// Alex is not with Jenna and Kraden, and fights them.
void scVenusLighthouseEntranceRuffianWaitsForBackup(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgWhyDoI);
	fdActorWalkToAndWait(ACTOR_JENNA, 72, 266);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	syFieldScriptWait(10);
	// "Why do I gotta be the one to wait for backup!?"
	syFieldScriptShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
	scFieldActorSetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_WAITING_RUFFIAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_WAITING_RUFFIAN, FACING_EAST, 0);
	syFieldScriptWait(20);
	scFieldActorJump(ACTOR_WAITING_RUFFIAN, 6, 15);
	scFieldActorJump(ACTOR_WAITING_RUFFIAN, 6, 35);
	fdActorFaceDirection(ACTOR_WAITING_RUFFIAN, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_WAITING_RUFFIAN, 12);
	// "Yaaah! P-P-Please! Just let me go, and... Hey!"
	syFieldScriptShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 0);
	fdActorSetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
	fdActorShowEmote(ACTOR_WAITING_RUFFIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorFaceDirection(ACTOR_WAITING_RUFFIAN, FACING_EAST, 0);
	syFieldScriptWait(20);
	// "That scary guy with the blue hair's not with you..."
	syFieldScriptShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
	scFieldActorSetAnimationAndWait(ACTOR_WAITING_RUFFIAN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_WAITING_RUFFIAN, 1);
	fdActorWalkByAndWait(ACTOR_WAITING_RUFFIAN, 16, 0);
	syFieldScriptWait(10);
	// "Great! Now, you'll see what I'm really made of!"
	syFieldScriptShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
	fdActorSetSpeed(ACTOR_WAITING_RUFFIAN, 2);
	scFieldActorJump(ACTOR_WAITING_RUFFIAN, 6, 15);
	scFieldActorJump(ACTOR_WAITING_RUFFIAN, 6, 25);
	scFieldActorJump(ACTOR_WAITING_RUFFIAN, 6, 0);
	fdActorWalkBy(ACTOR_WAITING_RUFFIAN, 26, 0);
	syFieldScriptWait(8);
	fdActorSetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
	syFieldScriptSetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
	                                       ENTRANCE_AFTER_RUFFIAN_BATTLE);
	fdScriptSetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE,
	                             VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 0);
	fdScriptEnd();
}

// Heading east, metatiles (28, 24) to (28, 29), Kraden jumps out from behind Jenna: "Not that way,
// Jenna! The meeting spot is west of here." She steps back.
void scVenusLighthouseEntrancePointToMeetingSpot(void)
{
	ACTOR *actor;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgNotThatWayThe);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_WEST);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	// "Not that way, Jenna! The meeting spot is west of here. We must go..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGet(ACTOR_JENNA);
	if (actor != 0)
	{
		fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_JENNA, -16, 0);
	fdScriptEnd();
}

// After a lost battle in the prologue, Jenna wakes by the door with Kraden and Alex beside her;
// Alex leaves again for Lalivero.
void scVenusLighthouseEntranceWakeUp(void)
{
	ACTOR *actor;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_JENNA, 364, 156);
	fdActorSetPos(ACTOR_KRADEN, 362, 142);
	fdActorSetPos(ACTOR_ALEX, 390, 144);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
	scFieldActorSetSpritePriority(ACTOR_KRADEN, 2);
	fdActorSetSpriteRotationById(ACTOR_JENNA, DEGtoANG(90));
	fdActorSetSpriteFlags(fdActorGet(ACTOR_JENNA), 0);
	fdCameraMoveToActorById(ACTOR_JENNA, 0);
	FieldStateWork->transitionFrame = 60;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptWait(20);
	syFieldScriptSetMessageId((Sint32)MsgAreAreYou);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	// "Jenna... Are... Are you all right?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	// "Ah, Jenna... You're awake."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "Can you stand?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(30);
	fdActorSetPos(ACTOR_JENNA, 376, 164);
	fdActorSetSpriteRotationById(ACTOR_JENNA, 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_JENNA), 1);
	scFieldActorJump(ACTOR_JENNA, 6, 50);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_JENNA, -14, -2);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(20);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "It's a good thing I came back for you two..."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(10);
	// "Perhaps you ought to come with us this time..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	// "No. Someone has to keep Tolbi's soldiers at bay."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 16);
	fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "I'll have to go to Lalivero and confront them there."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldScriptWait(10);
	// "We'll meet at Idejima."
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_ALEX);
	fdActorSetSpeed(ACTOR_ALEX, 0.8);
	fdActorWalkBy(ACTOR_ALEX, 0, 120);
	syFieldScriptWait(40);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkByAndWait(ACTOR_KRADEN, 24, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
	scFieldActorWaitForMoveById(ACTOR_ALEX);
	fdActorSetPos(ACTOR_ALEX, 0, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	// "He's gone again."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(20);
	scFieldActorJump(ACTOR_KRADEN, 4, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "And we'd best be going as well..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGet(ACTOR_JENNA);
	if (actor != 0)
	{
		fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	FieldStateWork->transitionFrame = 8;
	fdScriptEnd();
}

// The blast column's stage once it has burnt out.
#define BLAST_FINISHED      999

// The blast column: its pieces, 16 pixels each, and its 16x16 pictures in 256 colours, four ticks
// for its top and then four for the column below.
#define BLAST_COLUMN_PIECES 16
#define BLAST_COLUMN_TICKS  4
#define BLAST_PICTURE_TILES SPRITE_TILES_8BPP(16, 16)

// The rising column's state, kept in RamHeapSlots' menu runtime while it plays: its pictures, its
// pieces of water, the soldier it lifts and the puddle left under him.
typedef struct BLAST_COLUMN_STATE
{
	Uint8       pictures[1024]; // decompressed, then loaded into the VRAM cache
	EFFECT_SPR  sprites[BLAST_COLUMN_PIECES];
	SPARK_STATE states[BLAST_COLUMN_PIECES];
	ACTOR      *soldier;
	ACTOR      *puddle;
	FIXED       x;        // where the column stands: the soldier's feet
	FIXED       y;
	FIXED       z;
	Sint16      stage;    // BLAST_FINISHED once the column has burnt out
	Sint16      timer;    // ticks into the stage
	Uint16      columnTile;
	Uint16      topTile;
	Sint16      height;   // the column's height in pixels
	Sint8       carrying; // nonzero while the soldier rides the column's top
} BLAST_COLUMN_STATE;

void scVenusLighthouseEntranceUpdateBlastColumn(void)
{
	BLAST_COLUMN_STATE *blastColumnState = RamHeapSlots->menuState;
	ACTOR              *actor            = blastColumnState->soldier;
	ACTOR              *puddle           = blastColumnState->puddle;
	Sint32              i, y;
	FIXED               scale;
	Sint32              frameStep;
	Sint32              mask;
	SPARK_STATE        *state;
	EFFECT_SPR         *sprite;

	switch (blastColumnState->stage)
	{
		case 0: // the first tick
			if (blastColumnState->timer == 0)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		case 1: // the column rises under the soldier, 4 pixels a tick
			blastColumnState->height += 4;
			if (blastColumnState->height > 31)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		case 2: // it holds him up for a second and a half
			if (blastColumnState->timer == 90)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		case 3: // it shoots up and hurls him off the top of the screen
			if (blastColumnState->timer == 0)
			{
				actor->motionFlags         = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				actor->velocityY           = toFIXED(24);
				blastColumnState->carrying = 0;
				sySoundPlay(SE_HEAVY_IMPACT);
			}
			blastColumnState->height += 8;
			if (blastColumnState->height > 79)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		case 4:
		{ // out of sight, he is taken off the stage
			Sint16 *timer = &blastColumnState->timer;

			if (blastColumnState->timer == 10)
			{
				actor->z.fixed = 0;
				actor->x.fixed = 0;
			}
			if (*timer == 30)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		}
		case 5: // the column sinks
			blastColumnState->height -= 8;
			if (blastColumnState->height <= 0)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		case 6:
		{ // the puddle dries away, with the sound of a fall
			Sint16 *timer;

			scale = puddle->scaleX;
			if (scale > 0)
			{
				scale          -= toFIXED(2) / 100;
				puddle->scaleX  = scale;
				puddle->scaleY  = scale;
			}
			else
			{
				puddle->scaleX = 0;
				puddle->scaleY = 0;
			}
			timer = &blastColumnState->timer;
			if (blastColumnState->timer == 10)
				sySoundPlay(SE_FALL);
			if (*timer == 90)
			{
				blastColumnState->stage++;
				blastColumnState->timer = -1;
			}
			break;
		}
		case 7:
		{ // a clatter, and the ground shakes for half a second
			Sint16 *timer = &blastColumnState->timer;

			if (blastColumnState->timer == 0)
			{
				sySoundPlay(SE_CLATTER);
				fdMapGridSetShake(3, 3, 1);
			}
			if (*timer == 30)
			{
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				if (blastColumnState->timer == 30)
				{
					blastColumnState->stage = BLAST_FINISHED;
					blastColumnState->timer = -1;
				}
			}
			break;
		}
	}
	actor->facing += TRIG_TURN / 16;
	if (blastColumnState->height > 0)
	{
		for (i = 0, y  = 0; i < BLAST_COLUMN_PIECES && y < blastColumnState->height - 8;
		     i++, y   += 16)
		{
			state    = &blastColumnState->states[i];
			state->x = blastColumnState->x;
			state->y = blastColumnState->y + toFIXED(y) + toFIXED(8);
			state->z = blastColumnState->z;
			sprite   = &blastColumnState->sprites[i];
			state->frame++;
			frameStep = state->frame >> 2;
			mask      = 3;
			sprite->oam.index =
				blastColumnState->columnTile + (frameStep & mask) * BLAST_PICTURE_TILES;
			fdEffectSpriteDrawAtWorldPos(sprite, &state->x);
		}
		if (i < BLAST_COLUMN_PIECES)
		{
			state    = &blastColumnState->states[i];
			state->x = blastColumnState->x;
			state->y = blastColumnState->y + toFIXED(blastColumnState->height);
			state->z = blastColumnState->z;
			state->frame++;
			state->y +=
				syMathMulFX(-toFIXED(2), syMathSin(state->frame * (TRIG_TURN / 16))) - toFIXED(2);
			sprite = &blastColumnState->sprites[i];
			sprite->oam.index =
				blastColumnState->topTile + ((state->frame >> 2) & 3) * BLAST_PICTURE_TILES;
			fdEffectSpriteDrawAtWorldPos(sprite, &state->x);
			if (blastColumnState->carrying != 0)
				actor->y.fixed = state->y;
		}
	}
	blastColumnState->timer++;
}

// A column of water rises under a soldier, holds him up, then hurls him off the top of the screen,
// and the ground shakes as the column sinks.
void scVenusLighthouseEntranceBlastActorSkyward(Sint32 id)
{
	BLAST_COLUMN_STATE *blastColumnState =
		syHeapSlotAllocate(HEAP_SLOT_MENU, sizeof(BLAST_COLUMN_STATE));
	ACTOR       *actor = fdActorGet(id);
	ACTOR       *puddle;
	EFFECT_SPR  *sprite;
	SPARK_STATE *state;
	Sint32       resource, tile, i;
	Uint8        unpaused     = 0;

	actor->paused             = unpaused;
	blastColumnState->soldier = actor;
	blastColumnState->x       = actor->x.fixed;
	blastColumnState->y       = actor->y.fixed;
	blastColumnState->z       = actor->z.fixed + toFIXED(2);
	syDecompressTaggedLZ(FieldGeyserTiles, blastColumnState);
	resource                     = syVRAMCacheFindFreeEntry();
	tile                         = mnVRAMLoad(resource, 2048, blastColumnState);
	blastColumnState->topTile    = tile;
	blastColumnState->columnTile = tile + BLAST_COLUMN_TICKS * BLAST_PICTURE_TILES;
	state                        = blastColumnState->states;
	sprite                       = blastColumnState->sprites;
	for (i = BLAST_COLUMN_PIECES - 1; i >= 0;)
	{
		fdEffectSpriteInit(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->depth         = fdActorGetSpriteDepth(actor) - 2;
		sprite->oam.priority  = fdActorGetSpritePriority(actor);
		sprite->oam.fullColor = 1;
		sprite->oam.palette   = 0;
		state->frame          = -1;
		i--;
		sprite++;
		state++;
	}
	puddle = (scFieldActorCreate(SPRITE_PUDDLE, actor->x.fixed, actor->y.fixed, actor->z.fixed));
	blastColumnState->puddle = puddle;
	fdActorSelectAnimation(puddle, 1);
	fdActorSetSpriteFlags(puddle, 0);
	puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSpriteFlags(actor, 0);
	((SPR *)actor->sprite)->rotation  = DEGtoANG(90);
	actor->x.fixed                   += -toFIXED(12);
	actor->motionFlags                = 0;
	blastColumnState->stage           = 0;
	blastColumnState->timer           = 0;
	blastColumnState->height          = 0;
	blastColumnState->carrying        = 1;
	fdScriptPlaySound(SE_RISE_WITH_SPARKS);
	sySetTickEvent(scVenusLighthouseEntranceUpdateBlastColumn, EVENT_PASS_TICK);
	while (blastColumnState->stage != BLAST_FINISHED)
		syWait(1);
	syFieldCloseEvent(scVenusLighthouseEntranceUpdateBlastColumn);
	fdActorRelease(puddle);
	syVRAMCacheFreeEntry(resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

void scVenusLighthouseEntranceUpdateDust(void)
{
	DUST_PUFF_STATE *dustPuffState = RamHeapSlots->menuState;
	SPARK_STATE     *state         = dustPuffState->states;
	EFFECT_SPR      *sprite        = dustPuffState->sprites;
	Sint32           frame;
	Sint32           i;

	for (i = 0; i < DUST_PUFFS; i++, sprite++, state++)
	{
		frame = state->frame;
		if ((Uint32)frame < DUST_PUFF_TICKS)
		{
			sprite->oam.index =
				dustPuffState->tileBase + ((frame >> 1) & 7) * SPRITE_TILES_8BPP(16, 16);
			fdEffectSpriteDrawAtWorldPos(sprite, &state->x);
			fdBattleEffectStepVector3(state, 64, 0);
			state->frame++;
		}
	}
}

void scVenusLighthouseEntranceEmitDust(ACTOR *src, Sint32 x, Sint32 y, Sint32 z)
{
	DUST_PUFF_STATE *dustPuffState = RamHeapSlots->menuState;
	Sint32           angle         = syRandom();
	Sint16          *next          = &dustPuffState->next;
	EFFECT_SPR      *sprite        = &dustPuffState->sprites[*next];
	SPARK_STATE     *state         = &dustPuffState->states[*next];

	sprite->oam.priority           = fdActorGetSpritePriority(src);
	sprite->depth                  = fdActorGetSpriteDepth(src);
	state->frame                   = 0;
	state->x                       = x;
	state->y                       = y;
	state->z                       = z;
	syFieldVectorAddPolarOffsetFX(syRandom() * 2 + toFIXED(2), angle, &state->x);
	state->velocityX = 0;
	state->velocityY = (syRandom() >> 1) + toFIXED(3) / 4;
	state->velocityZ = 0;
	syFieldVectorAddPolarOffsetFX(syRandom() >> 2, angle, &state->velocityX);
	*next = *next + 1;
	*next = *next % DUST_PUFFS;
}

void scVenusLighthouseEntranceStartDust(void)
{
	DUST_PUFF_STATE *dustPuffState = syHeapSlotAllocate(HEAP_SLOT_MENU, sizeof(DUST_PUFF_STATE));
	Sint32           resource;
	Sint32           tileBase;
	Sint32           i;
	SPARK_STATE     *state;
	EFFECT_SPR      *sprite;

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_DUST_PUFF_A), dustPuffState);
	resource                = syVRAMCacheFindFreeEntry();
	tileBase                = mnVRAMLoad(resource, 2048, dustPuffState);
	dustPuffState->tileBase = tileBase;
	dustPuffState->resource = resource;
	state                   = dustPuffState->states;
	for (i = 0; i < DUST_PUFFS; i++)
	{
		sprite = &dustPuffState->sprites[i];
		fdEffectSpriteInit(sprite, 8, 8, OBJ_SIZE_16X16, tileBase);
		sprite->oam.fullColor = 1;
		sprite->oam.palette   = 0;
		sprite->oam.priority  = 0;
		sprite->depth         = OAM_DEPTH_WINDOW;
		state->frame          = -1;
		state++;
	}
	dustPuffState->next = 0;
	sySetTickEvent(scVenusLighthouseEntranceUpdateDust, EVENT_PASS_TICK);
}

void scVenusLighthouseEntranceStopDust(void)
{
	DUST_PUFF_STATE *state = RamHeapSlots->menuState;

	syFieldCloseEvent(scVenusLighthouseEntranceUpdateDust);
	syVRAMCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VENUS_LIGHTHOUSE_ENTRANCE_FROM_VENUS_LIGHTHOUSE,
     CONDITION_ALWAYS, 376,
     0, 104,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{ENTRANCE_PLAZA_PATH, CONDITION_ALWAYS, 376, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_BY_WORKMEN, CONDITION_ALWAYS, 384, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_WEST_ROAD, CONDITION_ALWAYS, 112, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{ENTRANCE_JENNA_TAKES_OVER, CONDITION_ALWAYS, 376, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VENUS_LIGHTHOUSE_ENTRANCE_AFTER_DEFEAT,
     CONDITION_ALWAYS, 384,
     0, 168,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{ENTRANCE_AFTER_RUFFIAN_BATTLE,
     CONDITION_ALWAYS, 72,
     0, 264,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(VenusLighthouseEntrance),
	SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRANCE_TO_SUHALLA_GATE_ROAD_EAST,
               SuhallaGateRoad,
               SUHALLA_GATE_ROAD_FROM_VENUS_LIGHTHOUSE_ENTRANCE_EAST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_ENTRANCE_TO_VENUS_LIGHTHOUSE_A,
               VenusLighthouse,
               VENUS_LIGHTHOUSE_FROM_VENUS_LIGHTHOUSE_ENTRANCE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_KRADEN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    8,
                    0,
                    8,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_ALEX,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    8,
                    0,
                    8,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SEAGULL,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    352,
                    0,
                    176,
                    FACING_EAST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SEAGULL,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    416,
                    0,
                    152,
                    FACING_WEST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    424,
                    0,
                    408,
                    FACING_NORTHWEST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    440,
                    0,
                    408,
                    FACING_NORTHWEST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    424,
                    0,
                    424,
                    FACING_NORTHWEST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    440,
                    0,
                    440,
                    FACING_NORTHWEST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    456,
                    0,
                    408,
                    FACING_NORTHWEST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    328,
                    0,
                    408,
                    FACING_NORTHEAST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    312,
                    0,
                    408,
                    FACING_NORTHEAST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    328,
                    0,
                    424,
                    FACING_NORTHEAST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    312,
                    0,
                    440,
                    FACING_NORTHEAST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    296,
                    0,
                    408,
                    FACING_NORTHEAST,
                    TALK_FACING_KEEP,
                    0),
	ACTOR_PLACEMENT(SPRITE_RUFFIAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    24,
                    0,
                    264,
                    FACING_WEST,
                    TALK_FACING_KEEP,
                    0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP,
     METATILE_BESIDE_DOOR, FLAG_VENUS_LIGHTHOUSE_BIRDS_FLEW_OFF,
     SCRIPT_REF(scVenusLighthouseEntranceBirdsFlyOff)},
	{CUE_STEP,
     METATILE_BELOW_DOOR, FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN,
     SCRIPT_REF(scVenusLighthouseEntranceSoldiersAmbushParty)},
	{CUE_STEP,
     METATILE_ROAD_WEST, FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN,
     SCRIPT_REF(scVenusLighthouseEntranceRuffianWaitsForBackup)},
	{CUE_STEP,
     METATILE_WAY_EAST, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseEntrancePointToMeetingSpot)},
	{CUE_TALK, ACTOR_WAITING_RUFFIAN, CONDITION_ALWAYS, MSGNO(MsgUggghICan)},
	{CUE_TALK,
     ACTOR_DOUBTING_SOLDIER, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseEntranceBlastActorWithPsynergy)},
	{CUE_TALK,
     ACTOR_SOLDIER_CAPTAIN, CONDITION_ALWAYS,
     SCRIPT_REF(scVenusLighthouseEntranceRunAndKickUpDust)},
	{CUE_TABLE_END},
};

// clang-format off
Uint32 VenusLighthouseEntranceBirdLands[] = {
	SC_SYNC,
	SC_WAIT(1),
	SC_ANIMATION(2),
	SC_WAIT(4),
	SC_SET(SW_MOTION, ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, 0),
	SC_WAIT(1),
	SC_END,
};

Uint32 VenusLighthouseEntranceBirdFlies[] = {
	SC_SYNC,
	SC_WAIT(1),
	SC_RELEASE,
};
// clang-format on
