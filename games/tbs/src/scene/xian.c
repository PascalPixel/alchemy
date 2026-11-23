/// @file
/// SceneId_Xian, the village of Xian.
///
/// Its houses, shops and inn are lacquer_rooms_b.c. Master Feh's school is a
/// room of the Fuchin Temple overlay, fuchin_temple.c. That overlay is
/// LacquerRoomsA, which XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL leads into.
///
/// Xian is the village of silk and kung fu beyond Mogall Forest. The party
/// comes in from the World Map at the south end and may pass straight through
/// to Altin, but there is a Djinni to win here first.
///
/// What the villagers say turns on the news that Hsu is trapped
/// (FLAG_XIAN_HEARD_HSU_TRAPPED), which Master Hama's Reveal lesson at Lama
/// Temple brings. Every villager's talk and Mind Read has a row for each side
/// of it. After the news the small boy looks west, the kung fu student stands
/// where her strike leaves her, and the muscular man turns to face the party.
///
/// Once Feizhi has left Master Feh's school to look for Hsu
/// (FLAG_FEIZHI_WENT_TO_HSU, fuchin_temple.c) she waits by the village's south
/// way out, looking for someone to take her west. She is gone once she has
/// found the road blocked at Alpine Crossing (FLAG_FEIZHI_SAW_ROCKSLIDE) or
/// Xian has heard the news.
///
/// A woman carries water along the street below the waterfall, from the pool
/// at its west end, cell (17, 14), to the door of the east house at its east
/// end, cell (26, 13), and back. Spoken to on the eastern part of her round she
/// spills her water, and a puddle spreads at her feet that Isaac must hop over.
/// Spilled at the east end, by the wall, Frost freezes it into an ice pillar
/// that Isaac climbs onto the ledge where Mist, a Mercury Djinni, waits at
/// cell (28, 20); Mist joins when spoken to. Standing in her way at the west
/// end for two seconds makes her march Isaac back to the pool.
///
/// The kung fu student, a little girl by the bridge north of the mulberry
/// orchard, shows her strike on the post before her once, and Force on the
/// post makes it jump on its own, to her surprise.
///
/// The spilled water and the student's strike last until the party next
/// enters a scene other than by returning from a battle.
///
/// A jar by the mulberry orchard, at cell (10, 22), holds a Lucky Medal.
///
/// The village's doors and where they lead:
/// - Five doors lead into the houses, shops and inn (SceneId_LacquerRoomsB):
///   the house at cell (21, 20), the north and east houses on the water
///   carrier's street at cells (23, 13) and (26, 13), the inn at (25, 26) and
///   the weapon and armor shop at (13, 11).
/// - The Sanctum's sliding door at cells (10, 13) and (11, 13) leads into Xian
///   Sanctum.
/// - The door at cell (24, 11) leads into Master Feh's school.
/// - The road south at cells (23, 32) and (24, 32) leads out to the World Map.
///
/// Coming back from the school (XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) or the
/// Sanctum (XIAN_FROM_SANCTUM_WEST) clears FLAG_SHOW_AREA_NAME, so the name is
/// not shown again.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

s32 Scene_Initialize(void);
extern s32 gXian_WaterCarrierRoute[];
extern s32 gXian_TrainingTreeJump[];
extern s32 gXian_TreeImpactScript[];
extern s32 gXian_TrainingParticleScript[];
extern u16 gXian_HouseDoorOpens[];
extern u16 gXian_NorthHouseDoorOpens[];
extern u16 gXian_InnDoorOpens[];
extern u16 gXian_SanctumDoorOpens[];
extern u16 gXian_SchoolDoorOpens[];
extern struct SceneEntrance gXian_Entrances[];
extern u32 gXian_Exits[];
extern struct ActorPlacement gXian_ActorPlacements[];
extern struct Trigger gXian_Triggers[];

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

// Xian's temporary flags: FLAG_XIAN_WATER_SPILLED the water carrier has spilled
// her water FLAG_XIAN_SPILLED_WATER_FROZEN the spilled water is frozen
// FLAG_XIAN_STRIKE_SHOWN the kung fu student has shown her strike on the tree
// FLAG_XIAN_TREE_JUMPED Force has made the tree jump once

/// The actors, numbering the rows of gXian_ActorPlacements. Each
/// person is named by what their sprite shows and what they talk of before Xian
/// hears of Hsu.
enum XianActorId {
  /// A young man in a blue jacket.
  ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS = ACTOR_FIRST_PLACED,
  /// A young woman (Xian_AskAboutYoungWarriors).
  ACTOR_WOMAN_ON_YOUNG_WARRIORS,
  /// A man with a topknot.
  ACTOR_MAN_ON_KINU,
  /// An old man.
  ACTOR_OLD_MAN_ON_MARTIAL_ARTS,
  /// An old woman with a cane.
  ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL,
  /// A small boy.
  ACTOR_BOY_ON_SILK_CLOTHES,
  /// A little girl who strikes the tree before her with a shout.
  ACTOR_KUNG_FU_STUDENT,
  /// A bare-chested, muscular man.
  ACTOR_MUSCULAR_MAN_ON_DISASTERS,
  /// A young woman.
  ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD,
  /// Feizhi, as at Alpine Crossing: looking for someone to take her west.
  ACTOR_FEIZHI,
  /// A young woman who carries water along the street. "You made me spill my
  /// water!"
  ACTOR_WOMAN_CARRYING_WATER,
  /// The tree the student strikes; it jumps when hit.
  ACTOR_TRAINING_TREE,
  /// The spilled water, which Frost freezes.
  ACTOR_SPILLED_WATER,
  /// Force's mark on the training tree.
  ACTOR_FORCE_TARGET,
  /// Mist, a Mercury Djinni.
  ACTOR_MIST,
  /// The first of three crates placed with SPRITE_CRATE, which Scene_Initialize
  /// hides (SPRITE_COLOR_HIDDEN): the player never sees them.
  ACTOR_FIRST_CRATE
};

/// The event the water carrier raises when the leader has stood in her way
/// for XIAN_CARRIER_BLOCKED_FRAMES.
enum XianRaisedTrigger { RAISED_CARRIER_BLOCKED = 200 };

/// Xian's own cell behaviors. The doors' cells carry the numbers of their
/// exits, so their rows of gXian_Triggers name them by the exit.
enum XianCellBehavior {
  /// The water carrier's street, cells (19, 14) to (27, 14), and the cells
  /// beside it, where Isaac hops over the spilled water.
  CELL_BEHAVIOR_XIAN_WATER_STREET = 20,
  /// The cell north of the street between the north and east houses' doors,
  /// cell (25, 13), from which Isaac hops south over the spilled water.
  CELL_BEHAVIOR_XIAN_WATER_STREET_NORTH = 21
};

enum { XIAN_CARRIER_BLOCKED_FRAMES = 120 };

/// The water carrier's round (gXian_WaterCarrierRoute) runs along map row 14
/// between columns 17 and 26, where she turns north and back.
enum {
  CARRIER_ROUTE_ROW = 14,
  CARRIER_ROUTE_WEST_COLUMN = 17,
  CARRIER_ROUTE_EAST_COLUMN = 26
};

/// Talking to Mist in Xian: Mist, a Mercury Djinni, joins the party.
static void Xian_MistJoins(void) {
  Djinn_JoinParty(ACTOR_MIST, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
}

/// Keeps the ice pillar drawn under Isaac while he stands on top of it.
static void Xian_UpdateFrozenPuddleLayer(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader->y.part[0].integer > 31) {
    Actor_Get(ACTOR_SPILLED_WATER)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Actor_Get(ACTOR_SPILLED_WATER)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// Spins the post as it jumps, under the student's strike or Force.
static void Xian_RotateTrainingTree(void) {
  struct Actor *tree = Actor_Get(ACTOR_TRAINING_TREE);
  struct Sprite *sprite = tree->sprite;
  sprite->rotation += TRIG_TURN * 5 / 64;
}

/// Leaves a trail of blue after-images behind the student as she leaps.
static void Xian_SpawnTrainingParticles(void) {
  struct Actor *student = Actor_Get(ACTOR_KUNG_FU_STUDENT);

  if ((gFrameCount & 3) == 0) {
    struct EffectOptions options;

    options.priority = 1;
    options.palette = SPRITE_COLOR_BLUE_TINT;
    options.type = SPRITE_GIRL_IN_PINK;
    options.script = gXian_TrainingParticleScript;
    FieldEffect_Spawn(student->x.fixed, student->y.fixed,
                      student->z.fixed - toFIXED(1), 0, -toFIXED(1),
                      -toFIXED(1),
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                          EFFECT_USE_TYPE | EFFECT_USE_SCRIPT,
                      &options);
  }
}

#include "shared/actor_distance.c"

/// The water carrier has spilled her water.
#define FLAG_XIAN_WATER_SPILLED FLAG_SCENE_TEMP_00
/// Frost has frozen the spilled water into ice that can be stood on.
#define FLAG_XIAN_SPILLED_WATER_FROZEN FLAG_SCENE_TEMP_01
/// The kung fu student has shown her strike on the tree.
#define FLAG_XIAN_STRIKE_SHOWN FLAG_SCENE_TEMP_02
/// Force has made the kung fu student's tree jump once, which picks her second
/// line.
#define FLAG_XIAN_TREE_JUMPED FLAG_SCENE_TEMP_03

/// Tracks a nearby target only within the actor's current facing sector.
static s32 Xian_UpdateCarrierTracking(struct Actor *actor, struct Actor *target,
                                      s32 distanceLimit, s32 forceTracking) {
  s32 isTracking = 0;

  // This route uses paused/timer as its tracking latch and mode.
  if (actor->paused == 1 && actor->timer == 0) {
    Actor_SelectAnimation(actor, ANIM_STAND);
    return 1;
  }

  if (Actor_Distance(target, actor) < distanceLimit || forceTracking != 0) {
    u16 angle = Trig_Atan(target->z.fixed - actor->z.fixed,
                          target->x.fixed - actor->x.fixed);
    s32 previousHeading = FACING_SIXTEENTH(angle - FACING_STEP);
    s32 nextHeading = FACING_SIXTEENTH(angle + FACING_STEP);
    s32 targetHeading = FACING_SIXTEENTH(angle);
    s32 currentHeading = FACING_SIXTEENTH(actor->facing);

    if (targetHeading == currentHeading || nextHeading == currentHeading ||
        previousHeading == currentHeading || forceTracking != 0) {
      actor->paused = 1;
      Actor_SelectAnimation(actor, ANIM_STAND);
      isTracking = 1;
      actor->timer = isTracking;
    } else {
      actor->paused = 0;
      Actor_SelectAnimation(actor, ANIM_WALK);
      actor->timer = 0;
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
    actor->timer = 0;
  }

  return isTracking;
}

/// The water carrier's update: she stops and looks at Isaac when he stands in
/// front of her, and when he has stood in her way at the west end of her
/// round, cells (17, 14) and (18, 14), for two seconds on her way back, she
/// raises RAISED_CARRIER_BLOCKED.
static UNREAD_RESULT Xian_WatchWaterCarrierPath(struct Actor *updated) {
  struct Actor *leader;
  s32 near = 0;

  if (updated->targetX == ACTOR_NO_TARGET &&
      updated->targetZ == ACTOR_NO_TARGET)
    RETURN_UNREAD(0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FIELD_CELL(leader->x.fixed) >= CARRIER_ROUTE_WEST_COLUMN &&
      FIELD_CELL(leader->x.fixed) <= CARRIER_ROUTE_WEST_COLUMN + 1 &&
      FIELD_CELL(leader->z.fixed) == CARRIER_ROUTE_ROW &&
      FIELD_CELL(updated->x.fixed) <= CARRIER_ROUTE_WEST_COLUMN + 2 &&
      updated->velocityX <= 0) {
    if (leader->x.fixed <= updated->x.fixed) {
      updated->riseCounter++;
      near = 1;
    }
  } else {
    updated->riseCounter = 0;
  }
  if (near && updated->riseCounter > XIAN_CARRIER_BLOCKED_FRAMES - 1) {
    gFieldWork->raisedTrigger = RAISED_CARRIER_BLOCKED;
    updated->riseCounter = 0;
  }
  Xian_UpdateCarrierTracking(updated, leader, 18, near);
  RETURN_UNREAD(0);
}

#ifdef UBFIX
#define XIAN_WATER_CARRIER_UPDATE Xian_WatchWaterCarrierPath
#else
#define XIAN_WATER_CARRIER_UPDATE ((void *)Xian_WatchWaterCarrierPath)
#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gXian_Entrances; }

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gXian_Exits; }

/// Once Xian has heard Hsu is trapped, the muscular man turns to face the party
/// when spoken to.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0)
    gXian_ActorPlacements[ACTOR_MUSCULAR_MAN_ON_DISASTERS - ACTOR_FIRST_PLACED]
        .talkFacing = TALK_FACING_PARTY;
  return gXian_ActorPlacements;
}

/// A villager in Xian asks whether the young warriors came through Mogall
/// Forest.
static void Xian_AskAboutYoungWarriors(void) {
  Event_Start();
  Event_SetMessageId(MsgXianYoungWarriorsVeryGallantCame);
  // "Young warriors, very gallant! You came through Mogall Forest?"
  // Yes: "Incredible. I respect your deeds."
  // No: "Came from Altin? No. That is a lie. Silk Road is blocked. No..."
  Event_AskYesNo(ACTOR_WOMAN_ON_YOUNG_WARRIORS, 0);
  Event_End();
}

/// Talking to the water carrier on the eastern part of her round makes her
/// spill her water: a puddle spreads at her feet, she cries "What are you
/// doing! You made me spill my water!" and walks back to the pool for more.
///
/// Near the pool, or once her water is spilled, she only says "Do not speak to
/// me. I might spill my water." or "Now I must get water again." If Isaac is
/// on the street with her, he first steps up beside her, and he steps back out
/// of her way before she leaves. The puddle's cell takes a blocking cell
/// behavior copied from the map's store at cell (16, 16).
static void Xian_TalkToWaterCarrier(void) {
  struct Actor *puddle;
  struct Actor *carrier;
  struct Actor *leader;
  s32 leaderMustStepAside;
  s32 savedFacing;
  FIXED carrierX;
  s32 carrierCellZ;
  s32 carrierColumnX;
  s32 scaleY;
  FIXED scaleX;

  puddle = Actor_Get(ACTOR_SPILLED_WATER);
  Event_Start();
  leaderMustStepAside = 0;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->update = NULL;
  if (Event_GetFlag(FLAG_XIAN_WATER_SPILLED) != 0 ||
      (FIELD_CELL(Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.fixed)) <=
          CARRIER_ROUTE_WEST_COLUMN + 2) {
    savedFacing = Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->facing;
    Actor_FaceActor(ACTOR_WOMAN_CARRYING_WATER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgXianNowMustGetWaterAgain);
    if (GameFlag_Get(FLAG_XIAN_WATER_SPILLED) == 0) {
      gFieldWork->message++;
      // "Do not speak to me. I might spill my water."
      Event_ShowMessage(ACTOR_WOMAN_CARRYING_WATER, 0);
      Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->timer = 0;
      Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->facing = savedFacing;
    } else {
      // "Now I must get water again. This work is hard on my back."
      Event_ShowMessage(ACTOR_WOMAN_CARRYING_WATER, 0);
      Actor_FaceDirection(ACTOR_WOMAN_CARRYING_WATER, FACING_WEST, 20);
    }
    Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->update = XIAN_WATER_CARRIER_UPDATE;
    Event_End();
    return;
  }
  if (FIELD_HALF_CELL(Actor_Get(ACTOR_PARTY_LEADER)->z.fixed) >=
          2 * CARRIER_ROUTE_ROW &&
      FIELD_HALF_CELL(Actor_Get(ACTOR_PARTY_LEADER)->z.fixed) <=
          2 * CARRIER_ROUTE_ROW + 1 &&
      (FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->x.fixed)) !=
          CARRIER_ROUTE_EAST_COLUMN) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WOMAN_CARRYING_WATER, 0);
    Event_Wait(5);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader->x.fixed < Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.fixed) {
      Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkTo(
          ACTOR_PARTY_LEADER,
          (FIELD_CELL(Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.fixed) << 4) - 8,
          232);
      leaderMustStepAside = 1;
    } else {
      Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkTo(
          ACTOR_PARTY_LEADER,
          (FIELD_CELL(Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.fixed) << 4) +
              24,
          232);
    }
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  }
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->targetX = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->targetY = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->targetZ = ACTOR_NO_TARGET;
  Actor_SetMovementTypeById(ACTOR_WOMAN_CARRYING_WATER, MOVEMENT_TYPE_STAND);
  Actor_SetAnimation(ACTOR_WOMAN_CARRYING_WATER, ANIM_STAND);
  Actor_RunRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
  Event_Wait(10);
  Sound_Play(SE_SPARKLING_PASSAGE);
  puddle->scaleX = 3 * FIXED_ONE / 10;
  puddle->scaleY = 3 * FIXED_ONE / 10;
  carrierX = Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.fixed;
  carrierCellZ = FIELD_CELL(Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->z.fixed);
  Actor_SetPositionFX(ACTOR_SPILLED_WATER,
                      FIELD_CELL_CENTER(FIELD_CELL(carrierX)),
                      FIELD_CELL_CENTER(carrierCellZ));
  carrierColumnX = Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.fixed;
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  MapGrid_CopyCellAttributes(16, 16, 1, 1, FIELD_CELL(carrierColumnX),
                             FIELD_CELL(carrier->z.fixed));
  Actor_SetSpritePriority(ACTOR_SPILLED_WATER, 2);
  puddle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  do {
    wait(3);
    scaleY = puddle->scaleY;
    scaleX = puddle->scaleX;
    puddle->scaleY = scaleY + FIXED_ONE / 10;
    scaleX += FIXED_ONE / 10;
    puddle->scaleX = scaleX;
  } while (scaleX <= FIXED_ONE - 1);
  Actor_ShowEmote(ACTOR_WOMAN_CARRYING_WATER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                  70);
  Actor_FaceActor(ACTOR_WOMAN_CARRYING_WATER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WOMAN_CARRYING_WATER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  0);
  Actor_StartRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
  Event_Wait(70);
  Event_SetMessageId(MsgXianDoingMadeMeSpillMy);
  // "What are you doing! You made me spill my water!"
  Event_ShowMessageThenWait(ACTOR_WOMAN_CARRYING_WATER, 0, 20);
  Scene_PlayQueuedMusic();
  if (FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->x.fixed) ==
          CARRIER_ROUTE_EAST_COLUMN &&
      (FIELD_CELL(Actor_Get(ACTOR_PARTY_LEADER)->z.fixed)) >=
          CARRIER_ROUTE_ROW) {
    leaderMustStepAside = 1;
  }
  if (leaderMustStepAside != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  }
  Actor_SetSpeed(ACTOR_WOMAN_CARRYING_WATER, 0.8);
  if (FIELD_CELL(Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->z.fixed) !=
      CARRIER_ROUTE_ROW) {
    Actor_WalkToAndWait(
        ACTOR_WOMAN_CARRYING_WATER,
        Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->x.part[0].integer, 232);
  }
  Actor_WalkToAndWait(ACTOR_WOMAN_CARRYING_WATER, 280, 232);
  GameFlag_Set(FLAG_XIAN_WATER_SPILLED);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_End();
}

/// When Isaac casts Frost on the spilled water from north of the street, at
/// the foot of the cliff, the puddle is brought to the front drawing layer
/// before it freezes. All this changes on screen is brief: as the ice rises
/// it is drawn over the left of the Psynergy ring around Isaac rather than
/// behind it, and a moment later the ice stands as it would without this.
static void Xian_UpdatePuddlePriority(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FIELD_CELL(leader->z.fixed) < CARRIER_ROUTE_ROW)
    Actor_SetSpritePriority(ACTOR_SPILLED_WATER, 1);
}

/// Frost on the spilled water in Xian freezes the puddle into an ice pillar.
/// Isaac can climb onto it, and from the one at the street's east end onto the
/// ledge where Mist waits; there is nothing left to hop over.
///
/// The pillar's cell takes the raised cell stored at cell (3, 17).
static void Xian_FreezeSpilledWater(void) {
  struct Actor *puddle;
  FIXED puddleX;

  Event_Start();
  Actor_Get(ACTOR_SPILLED_WATER)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_SPILLED_WATER)->motionFlags = 0;
  puddleX = Actor_Get(ACTOR_SPILLED_WATER)->x.fixed;
  puddle = Actor_Get(ACTOR_SPILLED_WATER);
  MapGrid_CopyCellAttributes(3, 17, 1, 1, FIELD_CELL(puddleX),
                             (FIELD_CELL(puddle->z.fixed)));
  Task_AddFrame(Xian_UpdateFrozenPuddleLayer, TASK_PASS_FRAME);
  GameFlag_Set(FLAG_XIAN_SPILLED_WATER_FROZEN);
  Actor_SetSpritePriority(ACTOR_SPILLED_WATER, 2);
  Event_End();
}

/// Talking to the kung fu student by the bridge the first time, she cries
/// "Hoo-whaaa... Hachaaa!", Isaac steps back, and she leaps west in a blur of
/// after-images and strikes the post, which jumps spinning into the air and
/// drops back. "Heh heh heh..." After that she only boasts of her kung fu.
static void Xian_TalkToKungFuStudent(void) {
  s32 demonstrationSeen;
  FIXED reboundVelocity;
  s32 stepBackOffset[3];
  struct EffectOptions effectParams;

  Event_Start();
  demonstrationSeen = GameFlag_Get(FLAG_XIAN_STRIKE_SHOWN);
  if (demonstrationSeen != 0) {
    Actor_FaceActor(ACTOR_KUNG_FU_STUDENT, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgXianKungFuStrong);
    // "My kung fu is strong. If you go to Master Feh's school every..."
    Event_ShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
    Actor_FaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 10);
    Event_End();
  } else {
    Event_SetMessageId(MsgXianHooWhaaaHachaaa);
    // "Hoo-whaaa... Hachaaa!"
    Event_ShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    SET_VECTOR(stepBackOffset, demonstrationSeen, demonstrationSeen,
               demonstrationSeen);
    FieldVector_AddPolarOffset(-8, Actor_Get(ACTOR_PARTY_LEADER)->facing,
                               (struct FieldPosition *)stepBackOffset);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER,
                               stepBackOffset[0] / toFIXED(1),
                               stepBackOffset[2] / toFIXED(1));
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
    effectParams.priority = 1;
    FieldEffect_Spawn(toFIXED(192), 0, toFIXED(312), toFIXED(1) / 10,
                      toFIXED(1) / 5, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(toFIXED(192), 0, toFIXED(312), toFIXED(1) / 5,
                      toFIXED(1) / 10, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Sound_Play(SE_WATER);
    Task_AddFrame(Xian_SpawnTrainingParticles, TASK_PASS_FRAME);
    Actor_Get(ACTOR_KUNG_FU_STUDENT)->velocityY = toFIXED(6);
    Actor_Get(ACTOR_KUNG_FU_STUDENT)->gravity = toFIXED(1);
    Actor_Get(ACTOR_KUNG_FU_STUDENT)->bounce = 0;
    Actor_SetSpeed(ACTOR_KUNG_FU_STUDENT, 3);
    Actor_SetDestination(ACTOR_KUNG_FU_STUDENT, 168, 312);
    Actor_WaitForMoveById(ACTOR_KUNG_FU_STUDENT);
    Sound_Play(SE_HEAVY_IMPACT);
    Actor_SetScript(ACTOR_TRAINING_TREE, gXian_TrainingTreeJump);
    Task_AddFrame(Xian_RotateTrainingTree, TASK_PASS_FRAME);
    effectParams.type = SPRITE_STAR_BURST;
    effectParams.script = gXian_TreeImpactScript;
    effectParams.rotation = DEGtoANG(90);
    FieldEffect_Spawn(toFIXED(168), 0, toFIXED(332), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
                          EFFECT_USE_SCRIPT | EFFECT_USE_ROTATION,
                      &effectParams);
    Actor_SetDestination(ACTOR_KUNG_FU_STUDENT, 146, 312);
    Actor_WaitForMoveById(ACTOR_KUNG_FU_STUDENT);
    Task_Remove(Xian_SpawnTrainingParticles);
    FieldEffect_Spawn(toFIXED(144), 0, toFIXED(312), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(toFIXED(144), 0, toFIXED(312), -toFIXED(1) / 5,
                      toFIXED(1) / 10, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    reboundVelocity = -toFIXED(1) / 2;
    FieldEffect_Spawn(toFIXED(144), 0, toFIXED(312), reboundVelocity, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Actor_WaitForScriptEndById(ACTOR_TRAINING_TREE);
    Sound_Play(SE_CHEST_OPEN);
    FieldEffect_Spawn(toFIXED(168), toFIXED(8), toFIXED(312), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(toFIXED(168), toFIXED(8), toFIXED(312), toFIXED(1) / 5, 0,
                      0, EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(toFIXED(168), toFIXED(8), toFIXED(312), -toFIXED(1) / 5,
                      0, 0, EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Task_Remove(Xian_RotateTrainingTree);
    Actor_Get(ACTOR_TRAINING_TREE)->sprite->rotation = reboundVelocity;
    Actor_Get(ACTOR_KUNG_FU_STUDENT)->bounce = FIXED_ONE / 4;
    Actor_Get(ACTOR_KUNG_FU_STUDENT)->gravity = toFIXED(1);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 20);
    Actor_RunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
    Event_Wait(20);
    // "Heh heh heh..."
    Event_ShowMessageThenWait(ACTOR_KUNG_FU_STUDENT, 0, 20);
    GameFlag_Set(FLAG_XIAN_STRIKE_SHOWN);
    Scene_PlayQueuedMusic();
    Event_End();
  }
}

/// Feizhi asks, in Xian, whether the party are strong warriors.
static void Xian_TalkToFeizhi(void) {
  Event_Start();
  Event_SetMessageId(MsgXianWarriorsFromSchoolStrongWarriors);
  // "You are the warriors from the school... Are you strong warriors?"
  // Yes: "I am happy to see strong warriors. I hope you will go west."
  // No: "You are weak warriors? Very few warriors would call..."
  Event_AskYesNo(ACTOR_FEIZHI, 0);
  Event_End();
}

/// Walking into the spilled water in Xian before it is frozen, along the
/// street or south from the doorsteps above it, makes Isaac hop over it,
/// landing two cells on if nothing stands there.
static void Xian_JumpOverSpilledWater(void) {
  s32 savedMotionFlags;
  struct Actor *leader;
  struct FieldPosition landingPosition;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
  if (GameFlag_Get(FLAG_XIAN_WATER_SPILLED) != 0) {
    landingPosition.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    landingPosition.y = leader->y.fixed;
    landingPosition.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    FieldVector_AddPolarOffset(32, FACING_NEAREST_CARDINAL(leader->facing),
                               &landingPosition);
    if (FieldActor_CheckMovementCollision(leader, &landingPosition) == 0) {
      Event_Start();
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(6);
      Sound_Play(SE_JUMP);
      Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
      leader->speedLimit = toFIXED(3);
      leader->acceleration = toFIXED(2);
      leader->velocityY = toFIXED(4);
      leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
      Actor_SetSpriteFlags(leader, 0);
      Actor_MoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(landingPosition.x),
                          FIXEDtoINT(landingPosition.z));
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
      leader->motionFlags = savedMotionFlags;
      Event_End();
    }
  }
}

/// Walks the leader through a doorway and leaves by its exit. Through the
/// Sanctum's door the leader walks straight in, without centering on it.
static void Xian_WalkThroughExit(s32 exitNumber) {
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  if (exitNumber == XIAN_TO_SANCTUM) {
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  }
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
  Import_EventSetExitRequest(exitNumber);
}

/// Walking north into one of Xian's doors opens it with a creak, or the
/// Sanctum's sliding door with a thump, and the party goes in.
static void Xian_OpenDoorAndLeave(void) {
  struct FieldWork *work = gFieldWork;

  Event_Start();

  switch (work->stepTrigger) {
  case XIAN_TO_HOUSE:
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gXian_HouseDoorOpens, 81, 18);
    break;
  case XIAN_TO_HOUSE_NORTH:
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gXian_NorthHouseDoorOpens, 83, 11);
    break;
  case XIAN_TO_LACQUER_ROOMS_B_EAST:
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gXian_NorthHouseDoorOpens, 86, 11);
    break;
  case XIAN_TO_INN:
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gXian_InnDoorOpens, 84, 24);
    break;
  case XIAN_TO_WEAPON_AND_ARMOR_SHOP:
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gXian_InnDoorOpens, 72, 7);
    break;
  case XIAN_TO_SANCTUM:
    Sound_Play(SE_THUMP);
    MapGrid_PlayMapEdit(gXian_SanctumDoorOpens, 69, 11);
    break;
  case XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL:
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_PlayMapEdit(gXian_SchoolDoorOpens, 83, 7);
    break;
  default:
    break;
  }

  Xian_WalkThroughExit(work->stepTrigger);
  Event_End();
}

/// Force on the post the kung fu student strikes makes it jump into the air
/// spinning and drop back, and she cries "Wha-Wha-What happened? The tree
/// jumped on its own!" Each time after that: "...It jumped again... I am too
/// tired..."
static void Xian_StrikeTrainingTreeWithForce(void) {
  TaskFunc rotate;

  Event_Start();
  Actor_SetScript(ACTOR_TRAINING_TREE, gXian_TrainingTreeJump);
  rotate = Xian_RotateTrainingTree;
  Task_AddFrame(rotate, TASK_PASS_FRAME);
  Actor_WaitForScriptEndById(ACTOR_TRAINING_TREE);
  Sound_Play(SE_CHEST_OPEN);
  FieldEffect_Spawn(toFIXED(168), toFIXED(8), toFIXED(312), 0, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  FieldEffect_Spawn(toFIXED(168), toFIXED(8), toFIXED(312), toFIXED(1) / 5, 0,
                    0, EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  FieldEffect_Spawn(toFIXED(168), toFIXED(8), toFIXED(312), -toFIXED(1) / 5, 0,
                    0, EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  Task_Remove(rotate);
  Actor_Get(ACTOR_TRAINING_TREE)->sprite->rotation = DEGtoANG(180);
  Actor_SetPosition(ACTOR_FORCE_TARGET, 168, 312);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_KUNG_FU_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_RunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
  Event_Wait(10);
  Event_SetMessageId(MsgXianWhaWhaHappened);
  if (Event_GetFlag(FLAG_XIAN_TREE_JUMPED)) {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
  GameFlag_Set(FLAG_XIAN_TREE_JUMPED);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Standing in the water carrier's way at the west end of her round for two
/// seconds makes her angry: she marches back to the pool with Isaac before
/// her, he winces, and she starts her round again.
static void Xian_WaterCarrierStartsRoundAgain(void) {
  Event_Start();
  Actor_SetMovementTypeById(ACTOR_WOMAN_CARRYING_WATER, MOVEMENT_TYPE_STAND);
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->update = 0;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->targetX = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->targetZ = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->velocityX = 0;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->velocityZ = 0;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->speedLimit = 0;
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->acceleration = 0;
  Actor_ShowEmote(ACTOR_WOMAN_CARRYING_WATER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  0);
  Actor_StartRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_WOMAN_CARRYING_WATER, 1.5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
  Actor_WalkTo(ACTOR_WOMAN_CARRYING_WATER, 280, 232);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 232);
  Actor_WaitForMoveById(ACTOR_WOMAN_CARRYING_WATER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetScript(ACTOR_WOMAN_CARRYING_WATER, gXian_WaterCarrierRoute);
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->update = XIAN_WATER_CARRIER_UPDATE;
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gXian_Triggers; }

/// Plays the village's wind and sets up the village as the story and this
/// visit left it.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  u32 crateIndex;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Scene_QueueMusic(SE_AMBIENT_WIND);
  // Back from the school (XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) or the
  // Sanctum (XIAN_FROM_SANCTUM_WEST), the last two entrances, the name
  // is not shown.
  if (gMainRoutineWork.entrance >= XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    Actor_FaceDirection(ACTOR_BOY_ON_SILK_CLOTHES, FACING_WEST, 0);
    Actor_SetPosition(ACTOR_KUNG_FU_STUDENT, 146, 312);
    Actor_FaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
    if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU)) {
      Actor_SetPosition(ACTOR_FEIZHI, 0, 0);
    }
  }
  // Gone once she has found the road blocked (Alpine Crossing's
  // FLAG_FEIZHI_SAW_ROCKSLIDE).
  if (GameFlag_Get(FLAG_FEIZHI_SAW_ROCKSLIDE)) {
    Actor_SetPosition(ACTOR_FEIZHI, 0, 0);
  }
  // Set the three crates up as Actor_CreateConfigured sets up
  // its actors: still, not solid, never a target and hidden.
  // XXX (no evidence): what the crates are for; nothing reads them again.
  for (crateIndex = 0; crateIndex <= 2; crateIndex++) {
    struct Actor *actor;
    actor = Actor_Get(crateIndex + ACTOR_FIRST_CRATE);
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags = (actor->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) |
                           ACTOR_PRIORITY_UNDERFOOT;
  }
  if (GameFlag_Get(FLAG_XIAN_STRIKE_SHOWN)) {
    Actor_SetPosition(ACTOR_KUNG_FU_STUDENT, 146, 312);
    Actor_FaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
  }
  if (GameFlag_Get(FLAG_XIAN_SPILLED_WATER_FROZEN)) {
    Actor_SetAnimation(ACTOR_SPILLED_WATER, PUDDLE_ANIMATION_PILLAR);
    {
      FIXED puddleX = Actor_Get(ACTOR_SPILLED_WATER)->x.fixed;

      MapGrid_CopyCellAttributeRect(
          3, 17, 1, 1, FIELD_CELL(puddleX),
          FIELD_CELL(Actor_Get(ACTOR_SPILLED_WATER)->z.fixed));
    }
    Task_AddFrame(Xian_UpdateFrozenPuddleLayer, TASK_PASS_FRAME);
  }
  // The water carrier's sprite is drawn with its green and blue swapped.
  Actor_SetSpriteColorById(ACTOR_WOMAN_CARRYING_WATER, SPRITE_COLOR_RBG);
  Actor_Get(ACTOR_WOMAN_CARRYING_WATER)->update = XIAN_WATER_CARRIER_UPDATE;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(16);
  actor->targetY = toFIXED(16);
  actor->scaleX = 11 * FIXED_ONE / 20;
  actor->scaleY = 2 * FIXED_ONE / 5;
  actor->sprite->rotation = DEGtoANG(180);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FORCE_TARGET), 0);
  Actor_Get(ACTOR_FORCE_TARGET)->motionFlags = 0;
  Actor_Get(ACTOR_FORCE_TARGET)->y.fixed = 0;
  Actor_Get(ACTOR_FORCE_TARGET)->targetY = ACTOR_NO_TARGET;
  return 0;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The water carrier's round: from the pool at the street's west end, cell
/// (17, 14), east to cell (26, 14), up to the east house's door and back,
/// pausing at each end.
s32 gXian_WaterCarrierRoute[] = {
    SC_SPEED(0.4),
    SC_ACCELERATION(0.1),
    SC_WAIT(60),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(60),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(180),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(424, 0, 232),
    SC_SYNC,
    SC_MOVE(424, 0, 216),
    SC_SYNC,
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST_SOUTHEAST),
    SC_WAIT(10),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(60),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(120),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(424, 0, 232),
    SC_SYNC,
    SC_MOVE(280, 0, 232),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// The struck post jumps 32 pixels into the air and drops back.
s32 gXian_TrainingTreeJump[] = {
    SC_SET(SW_SNAP, 0),
    SC_SPEED(3),
    SC_ACCELERATION(1.5),
    SC_MOVE(168, 48, 312),
    SC_SYNC,
    SC_SPEED(3),
    SC_ACCELERATION(0.1),
    SC_SET(SW_SNAP, 1),
    SC_MOVE(168, 16, 312),
    SC_SYNC,
    SC_END,
};

/// The effect at the tree's foot when the student strikes it: 18 frames.
s32 gXian_TreeImpactScript[] = {
    SC_WAIT(18), SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1), SC_RELEASE,
};

/// Each particle beside the leaping student, facing west: 16 frames.
s32 gXian_TrainingParticleScript[] = {
    SC_SET(SW_FACING, FACING_WEST), SC_WAIT(16), SC_WAIT(1),
    SC_POSITION(0, 0, 0),           SC_WAIT(1),  SC_RELEASE,
};

/// The doors' opening frames, copied from map cells.
u16 gXian_HouseDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(64, 48, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(63, 48, 1, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gXian_NorthHouseDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(69, 50, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(65, 50, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gXian_InnDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 4),
    MAPGRID_ANIMATION_STEP(63, 46, 3, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gXian_SanctumDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(67, 50, 4, 2, 4),
    MAPGRID_ANIMATION_STEP(63, 50, 4, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gXian_SchoolDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(66, 44, 3, 2, 4),
    MAPGRID_ANIMATION_STEP(63, 44, 3, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

struct SceneEntrance gXian_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_HOUSE, CONDITION_ALWAYS, 376, 0, 504, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {XIAN_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 344, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 376, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {XIAN_FROM_LACQUER_ROOMS_B_EAST, CONDITION_ALWAYS, 424, 0, 216,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_LACQUER_ROOMS_B_SOUTH, CONDITION_ALWAYS, 408, 0, 424,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_LACQUER_ROOMS_B_WEST, CONDITION_ALWAYS, 216, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL, CONDITION_ALWAYS, 392, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_SANCTUM_WEST, CONDITION_ALWAYS, 176, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gXian_Exits[] = {
    SCENE_EXITS(Xian),
    SCENE_EXIT(XIAN_TO_HOUSE, LacquerRoomsB, LACQUER_ROOMS_B_HOUSE),
    SCENE_EXIT(XIAN_TO_HOUSE_NORTH, LacquerRoomsB, LACQUER_ROOMS_B_HOUSE_NORTH),
    SCENE_EXIT(XIAN_TO_LACQUER_ROOMS_B_EAST, LacquerRoomsB,
               LACQUER_ROOMS_B_FROM_XIAN_EAST),
    SCENE_EXIT(XIAN_TO_INN, LacquerRoomsB, LACQUER_ROOMS_B_INN_SOUTH),
    SCENE_EXIT(XIAN_TO_WEAPON_AND_ARMOR_SHOP, LacquerRoomsB,
               LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(XIAN_TO_SANCTUM, Sanctum, SANCTUM_FROM_XIAN),
    SCENE_EXIT(XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL, LacquerRoomsA,
               LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN),
    SCENE_EXIT(XIAN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_XIAN),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gXian_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_BLUE_JACKET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 448, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        420, 0, 304, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        144, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 200, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 184, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 312, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        428, 0, 432, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 440, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, IF_FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU), MOVEMENT_TYPE_STAND,
        368, 0, 496, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)gXian_WaterCarrierRoute,
        280, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        168, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_MIST), MOVEMENT_TYPE_STAND,
        448, 0, 328, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        426, 0, 428, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        299, 0, 222, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 280, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct Trigger gXian_Triggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_HOUSE, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_HOUSE_NORTH, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_LACQUER_ROOMS_B_EAST, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_INN, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { TRIGGER_EXIT, XIAN_TO_WORLD_MAP, CONDITION_ALWAYS, XIAN_TO_WORLD_MAP },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_XIAN_WATER_STREET, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_XIAN_WATER_STREET_NORTH, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), XIAN_TO_HOUSE_NORTH, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), XIAN_TO_LACQUER_ROOMS_B_EAST, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsXianTravelersAreWelcome) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(Xian_AskAboutYoungWarriors) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianXianIsTheVillageOfKinu) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMartialArtsAreVeryPopularIn) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouWarriorsShouldVisitMasterFehs) },
    { TRIGGER_TALK, ACTOR_BOY_ON_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMyClothesAreNotSilkSilk) },
    { TRIGGER_TALK, ACTOR_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(Xian_TalkToKungFuStudent) },
    { TRIGGER_TALK, ACTOR_MUSCULAR_MAN_ON_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianDisastersFromTheSkyManyMany) },
    { TRIGGER_TALK, ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsTheMulberryOrchardWhen) },
    { TRIGGER_TALK, ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(Xian_TalkToFeizhi) },
    { TRIGGER_TALK, ACTOR_WOMAN_CARRYING_WATER, CONDITION_ALWAYS, EVENT(Xian_TalkToWaterCarrier) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgXianXianIsAPeacefulVillageBut) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianThereWasARockslideOnSilk) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KINU, CONDITION_ALWAYS, MSGNO(MsgXianIWentToHelpMoveBoulders) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleWhoClearedTheRockslide) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsVeryGoodAtKung) },
    { TRIGGER_TALK, ACTOR_BOY_ON_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleOfXianLoveTheir) },
    { TRIGGER_TALK, ACTOR_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianAltinWasFloodedWithWaterBut) },
    { TRIGGER_TALK, ACTOR_MUSCULAR_MAN_ON_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianWeClearedTheRocksFromSilk) },
    { TRIGGER_TALK, ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianManyStrangeThingsHappenedAllAt) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SPILLED_WATER, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_UpdatePuddlePriority) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPILLED_WATER, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_FreezeSpilledWater) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, CONDITION_ALWAYS, EVENT(Xian_StrikeTrainingTreeWithForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheseChildrenMustHaveComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouYoungWarriorsLookStrongBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianIHopeManyPeopleComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianFightingWithKicksAndPunchesIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehsSchoolIsTrainingIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianSilkIsShinyAndSmoothIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianBreakingTreesWithMyHandHurts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSCULAR_MAN_ON_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehSaidYahAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheOrchardGivesTheVillageLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianGoingWestAloneIsVeryDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CARRYING_WATER, CONDITION_ALWAYS, MSGNO(MsgXianIMustCarryWaterEveryDay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgXianHsuWasCaughtUnderABoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianTheRocksThereWereVeryHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KINU, CONDITION_ALWAYS, MSGNO(MsgXianSomePeopleDidNotCarryRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianSandIsTheColorOfThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsStrongHeadedHeWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianEveryoneStaysInXianTheyDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianThisWarriorSavedAltinVillageNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSCULAR_MAN_ON_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianItIsBadForXianIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianIWasHappyToSeeGems) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_XIAN_LUCKY_MEDAL_JAR, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TALK, ACTOR_MIST, CONDITION_ALWAYS, EVENT(Xian_MistJoins) },
    { TRIGGER_RAISED, RAISED_CARRIER_BLOCKED, CONDITION_ALWAYS, EVENT(Xian_WaterCarrierStartsRoundAgain) },
    { TRIGGER_TABLE_END },
};
// clang-format on
