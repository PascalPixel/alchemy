/* @file */
/* Champa's Sanctum, whose healer serves the party across the counter and */
/* otherwise talks of Briggs: the village's last hope until he sails from */
/* Alhafra, and after it the jewels that made Champa's people lazy. */

#include "game/game.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "shop/shop.h"

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* The healer behind the counter, a bearded man in a long robe. */
#define ACTOR_HEALER ACTOR_FIRST_PLACED

/* Talking to the healer from across the counter, facing north: the Sanctum's */
/* services. */
void scChampaSanctumServeAcrossTheCounter(s32 keeper) { mnSanctumRun(keeper); }

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

s32 scInit(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdActorSetSprPriority(ACTOR_HEALER, 2);
	fdActorGetById(ACTOR_HEALER)->collisionFlg |= ACTOR_NEAR_TALK;
	fdActorGetById(ACTOR_HEALER)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
	return 0;
}

s32 scPrepareMap(void) { return 0; }

#include "text/msg_ids.h"

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
	SCENE_EXITS(ChampaSanctum),
	SCENE_EXIT_UNLESS(CHAMPA_SANCTUM_TO_CHAMPA, Champa,
					  CHAMPA_FROM_CHAMPA_SANCTUM, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 720, 0, 147, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, CHAMPA_SANCTUM_TO_CHAMPA},
	{TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_HEALER, CONDITION_ALWAYS,
	 EVENT(scChampaSanctumServeAcrossTheCounter)},
	{TRIGGER_TALK, ACTOR_HEALER, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgChampaTreasureEncouragesLaziness)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER,
	 IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
	 MSGNO(MsgChampaVillagersStoppedWorking)},
	{TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
	 MSGNO(MsgChampaCaptainLastHope)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
	 MSGNO(MsgChampaNeedsCaptainTreasure)},
	{TRIGGER_TABLE_END},
};
