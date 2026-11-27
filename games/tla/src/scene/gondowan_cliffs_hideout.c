/* The hideout in the Gondowan Cliffs where four warriors from Kibombo camp
   round a fire, until Briggs is caught (FLAG_BRIGGS_CAUGHT). */

#include "field/field_mode.h"
#include "game/game.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "field/actor_script.h"

/* The hideout's placed actors: an unseen marker and the fire at one spot, then
   the four Kibombo warriors, named for what each says when spoken to. */
typedef enum GONDOWAN_CLIFFS_HIDEOUT_ACTOR_ID {
	/* An unseen marker at the fire (SPRITE_EXCLAMATION_BUBBLE's blank first
	   picture), which the camera pans to as the warriors are found. */
	ACTOR_FIRE_MARKER = ACTOR_FIRST_PLACED,
	ACTOR_FIRE,
	ACTOR_WARRIOR_FROM_KIBOMBO,
	ACTOR_WARRIOR_BLAMING_THE_COLLISION,
	ACTOR_WARRIOR_CLAIMING_TO_CAMP,
	ACTOR_WARRIOR_AWAITING_FRIENDS
} GONDOWAN_CLIFFS_HIDEOUT_ACTOR_ID;

extern ACTOR_PLACE GondowanCliffsHideoutPlacementsAfterBriggsCaught[];
extern ACTOR_PLACE GondowanCliffsHideoutPlacements[];
extern Uint32	   GondowanCliffsHideoutFireMarkerScript[];

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

#include "text/msg_ids.h"

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(syGameFlagGet(FLAG_BRIGGS_CAUGHT))
		return GondowanCliffsHideoutPlacementsAfterBriggsCaught;
	return GondowanCliffsHideoutPlacements;
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Felix walks in on the four warriors: they jump, turn to him and talk over one
   another, once. */
void scGondowanCliffsHideoutWarriorsAreFound(void)
{
	if(!syGameFlagGet(FLAG_BRIGGS_CAUGHT)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgKibomboWarriorsFound);
		fdCameraSetSpeed(1);
		fdCameraFollowActor(ACTOR_FIRE_MARKER, 1);
		fdCameraWaitForArrival();
		fdActorFaceActorById(ACTOR_WARRIOR_FROM_KIBOMBO, ACTOR_FELIX, 2);
		fdActorSetAttachedEffect(ACTOR_WARRIOR_FROM_KIBOMBO,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorJump(ACTOR_WARRIOR_FROM_KIBOMBO, 4, 15);
		/* "We've been found!" */
		fdScriptShowMessageAndWait(ACTOR_WARRIOR_FROM_KIBOMBO, 0);
		fdActorFaceActorById(
			ACTOR_WARRIOR_BLAMING_THE_COLLISION, ACTOR_FELIX, 0);
		fdActorFaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP, ACTOR_FELIX, 0);
		fdActorFaceActorById(ACTOR_WARRIOR_AWAITING_FRIENDS, ACTOR_FELIX, 0);
		fdActorJump(ACTOR_WARRIOR_BLAMING_THE_COLLISION, 2, 15);
		/* "You wanna fight, punk?" */
		fdScriptShowMessageAndWait(ACTOR_WARRIOR_BLAMING_THE_COLLISION, 0);
		fdActorFaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP,
			ACTOR_WARRIOR_BLAMING_THE_COLLISION, 0);
		fdActorSetAnimationAndWait(
			ACTOR_WARRIOR_CLAIMING_TO_CAMP, ANIM_SHAKE_HEAD);
		/* "No one said anything about fighting yet!" */
		fdScriptShowMessageAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 0);
		fdActorFaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP, ACTOR_FELIX, 0);
		/* "We haven't done anything to you!" */
		fdScriptShowMessageAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 0);
		fdActorSetAnimationAndWait(ACTOR_WARRIOR_AWAITING_FRIENDS, ANIM_NOD);
		/* "And if you think we did, then prove it!" */
		fdScriptShowMessageAndWait(ACTOR_WARRIOR_AWAITING_FRIENDS, 0);
		fdScriptEnd();
	}
	syGameFlagSet(FLAG_KIBOMBO_WARRIORS_FOUND);
}

/* Entering the hideout: the fire and its marker cast no shadow. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_FIRE_MARKER), 0);
	fdActorSetSpriteFlags(fdActorGetById(ACTOR_FIRE), 0);
	return 0;
}

/* The leader ignores the ground's rises in the hideout. */
Sint32 scPrepareMap(void)
{
	fdActorGetById(fdPartyGetLeaderActor())->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	return 0;
}

/* The fire marker's script: it shows its sprite sheet's blank first picture. */
Uint32 GondowanCliffsHideoutFireMarkerScript[] = {
	SC_ANIMATION(0),
	SC_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 194, 0, 215, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(GondowanCliffsHideout),
	SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_HIDEOUT_TO_GONDOWAN_CLIFFS,
		GondowanCliffs, GONDOWAN_CLIFFS_FROM_GONDOWAN_CLIFFS_HIDEOUT,
		EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE GondowanCliffsHideoutPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, (Sint32)GondowanCliffsHideoutFireMarkerScript, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE GondowanCliffsHideoutPlacementsAfterBriggsCaught[] = {
	ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, (Sint32)GondowanCliffsHideoutFireMarkerScript, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE SceneCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_CLIFFS_HIDEOUT_TO_GONDOWAN_CLIFFS},
	{CUE_STEP, 10, FLAG_KIBOMBO_WARRIORS_FOUND,
		SCRIPT_REF(scGondowanCliffsHideoutWarriorsAreFound)},
	{CUE_TALK, ACTOR_WARRIOR_FROM_KIBOMBO, CONDITION_ALWAYS,
		MSGNO(MsgKibomboWarriorIntroducesHimself)},
	{CUE_TALK, ACTOR_WARRIOR_BLAMING_THE_COLLISION, CONDITION_ALWAYS,
		MSGNO(MsgKibomboWarriorsBlameContinentsCollision)},
	{CUE_TALK, ACTOR_WARRIOR_CLAIMING_TO_CAMP, CONDITION_ALWAYS,
		MSGNO(MsgKibomboWarriorsClaimCamping)},
	{CUE_TALK, ACTOR_WARRIOR_AWAITING_FRIENDS, CONDITION_ALWAYS,
		MSGNO(MsgKibomboWarriorsFriendsComing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_FROM_KIBOMBO,
		CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsRegretMadraRaid)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_BLAMING_THE_COLLISION,
		CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsLostFood)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_CLAIMING_TO_CAMP,
		CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsThreatenTown)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_AWAITING_FRIENDS,
		CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsWonderAboutPrisoners)},
	{CUE_TABLE_END},
};
