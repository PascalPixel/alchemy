/// @file
/// The hideout in the Gondowan Cliffs where four warriors from Kibombo camp,
/// until flag 2219 is set.

#include "field/field.h"
#include "flags.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "field/event_inline.h"

/// The hideout's placed actors: two not yet understood, then the four
/// Kibombo warriors, named for what each says when spoken to.
enum GondowanCliffsHideoutActorId {
  ACTOR_WARRIOR_FROM_KIBOMBO = ACTOR_FIRST_PLACED + 2,
  ACTOR_WARRIOR_BLAMING_THE_COLLISION,
  ACTOR_WARRIOR_CLAIMING_TO_CAMP,
  ACTOR_WARRIOR_AWAITING_FRIENDS
};

extern struct ActorPlacement gPlacementsWithoutWarriors[];
extern struct ActorPlacement gPlacementsWithWarriors[];
extern u32 gGondowanCliffsHideout_StandScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT))
    return gPlacementsWithoutWarriors;
  return gPlacementsWithWarriors;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Felix walks in on the four warriors: they jump, turn to him and talk over
/// one another, once, until the scene's flag 2447 is set.
void GondowanCliffsHideout_WarriorsAreFound(void) {
  if (!GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboWarriorsFound);
    Camera_SetSpeedParameters(0x10000, 0x2000);
    FieldCamera_FollowActor(8, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceActorById(ACTOR_WARRIOR_FROM_KIBOMBO, ACTOR_FELIX, 2);
    {
      s32 half = 129;
      s32 id = ACTOR_WARRIOR_FROM_KIBOMBO;

      /* FAKEMATCH(barrier): an empty asm on half and id. */
      asm("" : "+l"(half), "+l"(id));
      FieldEffect_SetAttached(id, half << 1);
    }
    Actor_Jump(ACTOR_WARRIOR_FROM_KIBOMBO, 4, 15);
    // "We've been found!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_FROM_KIBOMBO, 0);
    Actor_FaceActorById(ACTOR_WARRIOR_BLAMING_THE_COLLISION, ACTOR_FELIX, 0);
    Actor_FaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP, ACTOR_FELIX, 0);
    Actor_FaceActorById(ACTOR_WARRIOR_AWAITING_FRIENDS, ACTOR_FELIX, 0);
    Actor_Jump(ACTOR_WARRIOR_BLAMING_THE_COLLISION, 2, 15);
    // "You wanna fight, punk?"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_BLAMING_THE_COLLISION, 0);
    Actor_FaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP,
                        ACTOR_WARRIOR_BLAMING_THE_COLLISION, 0);
    Motion_SetModeAndWaitAnimation(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 4);
    // "No one said anything about fighting yet!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 0);
    Actor_FaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP, ACTOR_FELIX, 0);
    // "We haven't done anything to you!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 0);
    Motion_SetModeAndWaitAnimation(ACTOR_WARRIOR_AWAITING_FRIENDS, 3);
    // "And if you think we did, then prove it!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_AWAITING_FRIENDS, 0);
    Event_End();
  }
  GameFlag_Set(2447);
}

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 513;
  Actor_SetSpriteFlags(Actor_GetById(8), 0);
  Actor_SetSpriteFlags(Actor_GetById(9), 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
  return 0;
}

u32 gGondowanCliffsHideout_StandScript[] = {
    39,
    0,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 194, 0, 215, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GondowanCliffsHideout),
    SCENE_EXIT_UNLESS(1, GondowanCliffs, 3, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gPlacementsWithWarriors[] = {
    {410, CONDITION_ALWAYS, (s32)gGondowanCliffsHideout_StandScript, PIXELS(216), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {306, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {83, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_WEST, 0, 2},
    {83, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 0, 2},
    {83, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 0, 2},
    {83, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_EAST, 0, 2},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gPlacementsWithoutWarriors[] = {
    {410, CONDITION_ALWAYS, (s32)gGondowanCliffsHideout_StandScript, PIXELS(216), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {306, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 10, 2447, EVENT(GondowanCliffsHideout_WarriorsAreFound)},
    {TRIGGER_TALK, ACTOR_WARRIOR_FROM_KIBOMBO, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWarriorIntroducesHimself)},
    {TRIGGER_TALK, ACTOR_WARRIOR_BLAMING_THE_COLLISION, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWarriorsBlameContinentsCollision)},
    {TRIGGER_TALK, ACTOR_WARRIOR_CLAIMING_TO_CAMP, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWarriorsClaimCamping)},
    {TRIGGER_TALK, ACTOR_WARRIOR_AWAITING_FRIENDS, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWarriorsFriendsComing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_FROM_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsRegretMadraRaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_BLAMING_THE_COLLISION,
     CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsLostFood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_CLAIMING_TO_CAMP,
     CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsThreatenTown)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_AWAITING_FRIENDS,
     CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsWonderAboutPrisoners)},
    {TRIGGER_TABLE_END},
};
