/// @file
/// The hideout in the Gondowan Cliffs where four warriors from Kibombo camp
/// round a fire, until Briggs is caught (FLAG_BRIGGS_CAUGHT).

#include "field/field.h"
#include "field/field_sequence.h"
#include "flags.h"
#include "battle/battle.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "sprite_ids.h"

/// The hideout's placed actors: an unseen marker and the fire at one spot,
/// then the four Kibombo warriors, named for what each says when spoken to.
enum GondowanCliffsHideoutActorId {
  /// An unseen marker at the fire (SPRITE_EXCLAMATION_BUBBLE's blank first
  /// picture), which the camera pans to as the warriors are found.
  ACTOR_FIRE_MARKER = ACTOR_FIRST_PLACED,
  ACTOR_FIRE,
  ACTOR_WARRIOR_FROM_KIBOMBO,
  ACTOR_WARRIOR_BLAMING_THE_COLLISION,
  ACTOR_WARRIOR_CLAIMING_TO_CAMP,
  ACTOR_WARRIOR_AWAITING_FRIENDS
};

extern struct ActorPlacement
    gGondowanCliffsHideout_PlacementsAfterBriggsCaught[];
extern struct ActorPlacement gGondowanCliffsHideout_Placements[];
extern u32 gGondowanCliffsHideout_FireMarkerScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT))
    return gGondowanCliffsHideout_PlacementsAfterBriggsCaught;
  return gGondowanCliffsHideout_Placements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Felix walks in on the four warriors: they jump, turn to him and talk over
/// one another, once.
void GondowanCliffsHideout_WarriorsAreFound(void) {
  if (!GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_SetMessageId((s32)MsgKibomboWarriorsFound);
    FieldCamera_SetSpeed(1);
    FieldCamera_FollowActor(ACTOR_FIRE_MARKER, 1);
    FieldCamera_WaitForArrival();
    Actor_FaceActorById(ACTOR_WARRIOR_FROM_KIBOMBO, ACTOR_FELIX, 2);
    Actor_SetAttachedEffect(ACTOR_WARRIOR_FROM_KIBOMBO,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
    Actor_SetAnimationAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, ANIM_SHAKE_HEAD);
    // "No one said anything about fighting yet!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 0);
    Actor_FaceActorById(ACTOR_WARRIOR_CLAIMING_TO_CAMP, ACTOR_FELIX, 0);
    // "We haven't done anything to you!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_CLAIMING_TO_CAMP, 0);
    Actor_SetAnimationAndWait(ACTOR_WARRIOR_AWAITING_FRIENDS, ANIM_NOD);
    // "And if you think we did, then prove it!"
    Event_ShowMessageAndWait(ACTOR_WARRIOR_AWAITING_FRIENDS, 0);
    Event_End();
  }
  GameFlag_Set(FLAG_KIBOMBO_WARRIORS_FOUND);
}

/// Entering the hideout: the fire and its marker cast no shadow.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRE_MARKER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRE), 0);
  return 0;
}

/// The leader ignores the ground's rises in the hideout.
s32 Scene_PrepareMap(void) {
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  return 0;
}

/// The fire marker's script: it shows its sprite sheet's blank first picture.
u32 gGondowanCliffsHideout_FireMarkerScript[] = {
    SC_ANIMATION(0),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 194, 0, 215, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GondowanCliffsHideout),
    SCENE_EXIT_UNLESS(
        GONDOWAN_CLIFFS_HIDEOUT_TO_GONDOWAN_CLIFFS, GondowanCliffs,
        GONDOWAN_CLIFFS_FROM_GONDOWAN_CLIFFS_HIDEOUT, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gGondowanCliffsHideout_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, (s32)gGondowanCliffsHideout_FireMarkerScript, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_KIBOMBO_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gGondowanCliffsHideout_PlacementsAfterBriggsCaught[] = {
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, (s32)gGondowanCliffsHideout_FireMarkerScript, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     GONDOWAN_CLIFFS_HIDEOUT_TO_GONDOWAN_CLIFFS},
    {TRIGGER_STEP, 10, FLAG_KIBOMBO_WARRIORS_FOUND,
     EVENT(GondowanCliffsHideout_WarriorsAreFound)},
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
