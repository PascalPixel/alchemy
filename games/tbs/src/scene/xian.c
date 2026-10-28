/// @file
/// SceneId_Xian, the village of Xian. Its houses, shops and inn are
/// lacquer_rooms_b.c; Master Feh's school is a room of the Fuchin Temple
/// overlay (LacquerRoomsA, which XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL leads
/// into; fuchin_temple.c).
///
/// The village's talk turns on FLAG_XIAN_HEARD_HSU_TRAPPED, which Master Hama's
/// Reveal lesson at Lama Temple sets: every villager's talk and Mind Read has a
/// row for each side of it, and after it Xian_Initialize also turns
/// ACTOR_BOY_ON_SILK_CLOTHES west and stands the kung fu student where her
/// strike leaves her, and Xian_GetActorPlacements turns
/// ACTOR_MUSCULAR_MAN_ON_DISASTERS to face the party. Feizhi stands by the
/// village's south exit from FLAG_FEIZHI_WENT_TO_HSU (set in fuchin_temple.c)
/// until she has found the road blocked at Alpine Crossing
/// (FLAG_FEIZHI_SAW_ROCKSLIDE) or Xian has heard the news.
///
/// Two small events run on scene flags, which last until the party next enters
/// a scene other than by returning from a battle:
/// - the water carrier walks her round (gXian_WaterCarrierRoute). Spoken to on
///   its eastern part, she spills her water (FLAG_XIAN_WATER_SPILLED): a puddle
///   spreads at her feet, its cell takes a cell behavior that walking into
///   jumps the leader over it (Xian_JumpOverSpilledWater), and she walks back
///   for more. Frost freezes the puddle (FLAG_XIAN_SPILLED_WATER_FROZEN) into
///   ice that can be stood on. A leader who stands in her way for two seconds
///   makes her raise RAISED_CARRIER_BLOCKED (Xian_WatchWaterCarrierPath), and
///   both walk back to the start of her round.
/// - the kung fu student, a little girl, shows her strike on the tree before
///   her once (FLAG_XIAN_STRIKE_SHOWN), and Force on the tree makes it jump on
///   its own, to her surprise (FLAG_XIAN_TREE_JUMPED picks her second line).
///
/// Mist, a Mercury Djinni, waits in the village until she joins, which she does
/// when spoken to.
///
/// A jar holds a Lucky Medal. Five doors lead into the houses, shops and inn
/// (SceneId_LacquerRoomsB, XIAN_TO_HOUSE to XIAN_TO_WEAPON_AND_ARMOR_SHOP),
/// XIAN_TO_SANCTUM into Xian Sanctum, its sliding door playing SE_THUMP,
/// XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL into Master Feh's school, and
/// XIAN_TO_WORLD_MAP out to the World Map. Coming back from the school
/// (XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) or the Sanctum
/// (XIAN_FROM_SANCTUM_WEST) clears FLAG_SHOW_AREA_NAME, so the name is not
/// shown again.
#include "adventure.h"
#include "djinn_ids.h"
#include "module/import.h"
#include "module/scene/xian.h"
#include "module/scene/shared/effect_spawn.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/runtime/runtime.h"
#include "macros.h"

#define NULL ((void *)0)
#include "actor.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "map_edit.h"
#include "iwram_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "flags.h"
#include "sounds.h"

#include "shared/effect_create.h"
#include "shared/effect_spawn.h"

// Xian's temporary flags: FLAG_XIAN_WATER_SPILLED the water carrier has spilled
// her water FLAG_XIAN_SPILLED_WATER_FROZEN the spilled water is frozen
// FLAG_XIAN_STRIKE_SHOWN the kung fu student has shown her strike on the tree
// FLAG_XIAN_TREE_JUMPED Force has made the tree jump once

/// The actors, numbering the rows of gXian_ActorPlacements. Each
/// person is named by what their sprite shows and what they talk of before Xian
/// hears of Hsu.
enum XianActorId {
  /// A young man in a blue jacket.
  /// ★ Named from the game: "This is Xian. Travelers are welcome here!"
  /// (MsgXianThisIsXianTravelersAreWelcome)
  ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS = ACTOR_FIRST_PLACED,
  /// A young woman (Xian_AskAboutYoungWarriors).
  /// ★ Named from the game: "Young warriors, very gallant! You came through
  /// Mogall Forest?" (MsgXianYoungWarriorsVeryGallantCame)
  ACTOR_WOMAN_ON_YOUNG_WARRIORS,
  /// A man with a topknot.
  /// ★ Named from the game: "Xian is the village of kinu. Kinu means silk."
  /// (MsgXianXianIsTheVillageOfKinu)
  ACTOR_MAN_ON_KINU,
  /// An old man.
  /// ★ Named from the game: "Martial arts are very popular in Xian."
  /// (MsgXianMartialArtsAreVeryPopularIn)
  ACTOR_OLD_MAN_ON_MARTIAL_ARTS,
  /// An old woman with a cane.
  /// ★ Named from the game: "You warriors should visit Master Feh's school. You
  /// will be surprised!" (MsgXianYouWarriorsShouldVisitMasterFehs)
  ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL,
  /// A small boy.
  /// ★ Named from the game: "My clothes are not silk. Silk is very expensive."
  /// (MsgXianMyClothesAreNotSilkSilk)
  ACTOR_BOY_ON_SILK_CLOTHES,
  /// A little girl who strikes the tree before her with a shout.
  /// ★ Named from the game: "Hoo-whaaa... Hachaaa!" (MsgXianHooWhaaaHachaaa)
  ACTOR_KUNG_FU_STUDENT,
  /// A bare-chested, muscular man.
  /// ★ Named from the game: "Disasters from the sky... Many, many thieves..."
  /// (MsgXianDisastersFromTheSkyManyMany)
  ACTOR_MUSCULAR_MAN_ON_DISASTERS,
  /// A young woman.
  /// ★ Named from the game: "This is the mulberry orchard."
  /// (MsgXianThisIsTheMulberryOrchardWhen)
  ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD,
  /// Feizhi, as at Alpine Crossing: looking for someone to take her west.
  /// ★ Named from the game: "You must be glad, Feizhi." (MsgMountainHsuOkay)
  ACTOR_FEIZHI,
  /// A young woman who carries water along the street. "You made me spill my
  /// water!"
  /// ★ Named from the game: "I must carry water every day."
  /// (MsgXianIMustCarryWaterEveryDay)
  ACTOR_WOMAN_CARRYING_WATER,
  /// The tree the student strikes; it jumps when hit.
  /// ★ Named from the game: "The tree jumped on its own!"
  /// (MsgXianWhaWhaHappened)
  ACTOR_TRAINING_TREE,
  /// The spilled water, which Frost freezes.
  /// ★ Named from the game: "You made me spill my water!"
  /// (MsgXianDoingMadeMeSpillMy)
  ACTOR_SPILLED_WATER,
  /// Force's mark on the training tree.
  /// ★ Named from what the player sees: nothing of its own, a blank
  /// actor on the bare trunk the student strikes (its render)
  ACTOR_FORCE_TARGET,
  /// Mist, a Mercury Djinni.
  /// ★ Named from the game: "Mist" (MsgDjinnName)
  ACTOR_MIST,
  /// The first of three crates placed with SPRITE_CRATE, which Xian_Initialize
  /// hides (SPRITE_COLOR_HIDDEN): the player never sees them.
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  ACTOR_FIRST_CRATE
};

/// SPRITE_PUDDLE's animation once Frost has frozen it into ice.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

/// The event the water carrier raises when the leader has stood in her way
/// for XIAN_CARRIER_BLOCKED_FRAMES.
enum XianRaisedTrigger { RAISED_CARRIER_BLOCKED = 200 };

enum { XIAN_CARRIER_BLOCKED_FRAMES = 120 };

/// Talking to Mist in Xian: Mist, a Mercury Djinni, joins the party.
static void Xian_MistJoins(void) {
  Djinn_JoinParty(ACTOR_MIST, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
}

/// Draws the frozen puddle under the leader while the leader stands on top of
/// it.
static void Xian_UpdateFrozenPuddleLayer(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->y.part.pixel > 31) {
    Actor_GetById(ACTOR_SPILLED_WATER)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Actor_GetById(ACTOR_SPILLED_WATER)->priorityFlags &=
        ~ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// Rotates the training tree during the student's strike and Force animation.
static void Xian_RotateTrainingTree(void) {
  struct Actor *tree = Actor_GetById(ACTOR_TRAINING_TREE);
  struct Sprite *sprite = tree->sprite;
  sprite->rotation += 0x1400;
}

/// Emits particles beside the student every fourth frame.
static void Xian_SpawnTrainingParticles(void) {
  // The byte view keeps the Z load ahead of the options-script store.
  u8 *studentActor = (u8 *)Actor_GetById(ACTOR_KUNG_FU_STUDENT);

  if ((gFrameCount & 3) == 0) {
    struct EffectOptions config;
    config.priority = 1;
    config.palette = 9;
    config.type = 169;
    config.script = gXian_TrainingParticleScript;
    FieldEffect_Spawn(*(s32 *)(studentActor + offsetof(struct Actor, x)),
                      *(s32 *)(studentActor + offsetof(struct Actor, y)),
                      *(s32 *)(studentActor + offsetof(struct Actor, z)) -
                          PIXELS(1),
                      0, -PIXELS(1), -PIXELS(1),
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                          EFFECT_USE_TYPE | EFFECT_USE_SCRIPT,
                      &config);
  }
}

#include "shared/actor_distance.h"

/// The water carrier has spilled her water.
/// ★ Named from the game: "You made me spill my water!"
/// (MsgXianDoingMadeMeSpillMy)
#define FLAG_XIAN_WATER_SPILLED FLAG_SCENE_TEMP_00
/// Frost has frozen the spilled water into ice that can be stood on.
/// ★ Named from the game: "You made me spill my water!"
/// (MsgXianDoingMadeMeSpillMy)
#define FLAG_XIAN_SPILLED_WATER_FROZEN FLAG_SCENE_TEMP_01
/// The kung fu student has shown her strike on the tree.
/// ★ Named from the game: "Hoo-whaaa... Hachaaa!" (MsgXianHooWhaaaHachaaa)
#define FLAG_XIAN_STRIKE_SHOWN FLAG_SCENE_TEMP_02
/// Force has made the kung fu student's tree jump once, which picks her second
/// line.
/// ★ Named from the game: "What happened? The tree jumped on its own!"
/// (MsgXianWhaWhaHappened)
#define FLAG_XIAN_TREE_JUMPED FLAG_SCENE_TEMP_03

/// Tracks a nearby target only within the actor's current facing sector.
static s32 Xian_UpdateCarrierTracking(struct Actor *actor, struct Actor *target,
                                      s32 distanceLimit, s32 forceTracking) {
  s32 isTracking = 0;

  // This route uses paused/timer as its tracking latch and mode.
  if (actor->paused == 1 && actor->timer == 0) {
    Actor_SelectAnimation(actor, 1);
    return 1;
  }

  if (FixedPoint_Distance((const u8 *)target + offsetof(struct Actor, x),
                          (const u8 *)actor + offsetof(struct Actor, x)) <
          distanceLimit ||
      forceTracking != 0) {
    u16 angle = ArcTan2(target->z.fixed - actor->z.fixed,
                        target->x.fixed - actor->x.fixed);
    s32 previousHeading = (angle - 0x1000) & 0xf000;
    s32 nextHeading = (angle + 0x1000) & 0xf000;
    s32 targetHeading = angle & 0xf000;
    s32 currentHeading = actor->facing & 0xf000;

    if (targetHeading == currentHeading || nextHeading == currentHeading ||
        previousHeading == currentHeading || forceTracking != 0) {
      actor->paused = 1;
      Actor_SelectAnimation(actor, 1);
      isTracking = 1;
      actor->timer = isTracking;
    } else {
      actor->paused = 0;
      Actor_SelectAnimation(actor, 2);
      actor->timer = 0;
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, 2);
    actor->timer = 0;
  }

  return isTracking;
}

// Ask the carrier to move after the leader blocks her route for two seconds.
// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 154 to 150 bytes in tbs-en.
#ifdef UBFIX
static void Xian_WatchWaterCarrierPath(struct Actor *updated) {
#else
static s32 Xian_WatchWaterCarrierPath(struct Actor *updated) {
#endif
  struct Actor *self = (struct Actor *)updated;
  struct Actor *leader;
  s32 near = 0;

  if (self->targetX == ACTOR_NO_TARGET && self->targetZ == ACTOR_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if ((u32)((leader->x.fixed >> 20) - 17) <= 1 && leader->z.fixed >> 20 == 14 &&
      self->x.fixed >> 20 <= 19 && self->velocityX <= 0) {
    if (leader->x.fixed <= self->x.fixed) {
      self->riseCounter++;
      near = 1;
    }
  } else {
    self->riseCounter = 0;
  }
  if (near && self->riseCounter > XIAN_CARRIER_BLOCKED_FRAMES - 1) {
    gFieldWork->raisedTrigger = RAISED_CARRIER_BLOCKED;
    self->riseCounter = 0;
  }
  Xian_UpdateCarrierTracking(self, leader, 18, near);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Keep the native callback representation beside its modern signature choice.
#ifdef UBFIX
#define XIAN_WATER_CARRIER_UPDATE Xian_WatchWaterCarrierPath
#else
#define XIAN_WATER_CARRIER_UPDATE ((void *)Xian_WatchWaterCarrierPath)
#endif

const struct SceneEntrance *Xian_GetEntrances(void) { return gXian_Entrances; }

const struct SceneExitRegion *Xian_GetRegions(void) { return NULL; }

const u32 *Xian_GetExits(void) { return gXian_Exits; }

/// Once Xian has heard Hsu is trapped, ACTOR_MUSCULAR_MAN_ON_DISASTERS turns
/// to face the party.
const struct ActorPlacement *Xian_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0)
    gXian_ActorPlacements[ACTOR_MUSCULAR_MAN_ON_DISASTERS - ACTOR_FIRST_PLACED]
        .talkFacing = TALK_FACING_PARTY;
  return gXian_ActorPlacements;
}

/// A villager in Xian asks whether the young warriors came through Mogall
/// Forest. She is ACTOR_WOMAN_ON_YOUNG_WARRIORS, and asks before Xian hears
/// of Hsu. "Young warriors, very gallant! You came through Mogall Forest?" A
/// yes earns respect; a no is a lie, since Silk Road from Altin is blocked.
static void Xian_AskAboutYoungWarriors(void) {
  Event_Start();
  Event_SetMessageId(MsgXianYoungWarriorsVeryGallantCame);
  Event_AskYesNoAndShowResponse(ACTOR_WOMAN_ON_YOUNG_WARRIORS, 0);
  Event_End();
}

/// Talking to the water carrier in Xian. While she carries her water east of
/// x = 320 the first time, she spills it: a puddle spreads at her feet, she
/// cries "You made me spill my water!" and walks back west to fetch more, the
/// leader stepping aside if he is in her way. Otherwise she only answers: "Do
/// not speak to me. I might spill my water.", or once it is spilled, "Now I
/// must get water again."
static void Xian_TalkToWaterCarrier(void) {
  struct Actor *puddle;
  struct Actor *carrier;
  struct Actor *leader;
  s32 leaderMustStepAside;
  s32 savedFacing;
  s32 carrierX;
  s32 carrierCellZ;
  s32 carrierColumnX;
  s32 scaleY;
  s32 scaleX;

  puddle = Actor_GetById(ACTOR_SPILLED_WATER);
  Event_Start();
  leaderMustStepAside = 0;
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->update = NULL;
  if (Event_GetFlag(FLAG_XIAN_WATER_SPILLED) != 0 ||
      (Actor_GetById(ACTOR_WOMAN_CARRYING_WATER)->x.fixed >> 20) <= 19) {
    carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
    savedFacing = carrier->facing;
    Actor_FaceActor(ACTOR_WOMAN_CARRYING_WATER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgXianNowMustGetWaterAgain);
    if (GameFlag_Get(FLAG_XIAN_WATER_SPILLED) == 0) {
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_WOMAN_CARRYING_WATER, 0);
      Actor_GetById(ACTOR_WOMAN_CARRYING_WATER)->timer = 0;
      carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
      carrier->facing = savedFacing;
    } else {
      Event_ShowMessage(ACTOR_WOMAN_CARRYING_WATER, 0);
      Actor_FaceDirection(ACTOR_WOMAN_CARRYING_WATER, FACING_WEST, 20);
    }
    carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
    carrier->update = XIAN_WATER_CARRIER_UPDATE;
    Event_End();
    return;
  }
  // Make room for the carrier before spilling the water.
  if ((Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) > 27 &&
      (Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 19) <= 29 &&
      (Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) != 26) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_WOMAN_CARRYING_WATER, 0);
    Event_Wait(5);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
    if (leader->x.fixed < carrier->x.fixed) {
      Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
      Actor_WalkTo(ACTOR_PARTY_LEADER, ((carrier->x.fixed >> 20) << 4) - 8,
                   232);
      leaderMustStepAside = 1;
    } else {
      Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
      carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
      Actor_WalkTo(ACTOR_PARTY_LEADER, ((carrier->x.fixed >> 20) << 4) + 24,
                   232);
    }
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  }
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->targetX = ACTOR_NO_TARGET;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrier->targetY = ACTOR_NO_TARGET;
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->targetZ = ACTOR_NO_TARGET;
  Actor_SetMovementTypeById(ACTOR_WOMAN_CARRYING_WATER, MOVEMENT_TYPE_STAND);
  Import_ActorSetAnimation(ACTOR_WOMAN_CARRYING_WATER, ANIM_STAND);
  Actor_RunRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
  Event_Wait(10);
  Event_PlaySound(SE_SPARKLING_PASSAGE);
  // Copy the wet terrain cell and grow the puddle into view.
  puddle->scaleX = 0x4ccc;
  puddle->scaleY = 0x4ccc;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrierX = carrier->x.fixed;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrierCellZ = carrier->z.fixed >> 20;
  Actor_SetPosition(ACTOR_SPILLED_WATER, ((carrierX >> 20) << 20) + PIXELS(8),
                    (carrierCellZ << 20) + PIXELS(8));
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrierColumnX = carrier->x.fixed;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  MapGrid_CopyCellAttributes(16, 16, 1, 1, carrierColumnX >> 20,
                             carrier->z.fixed >> 20);
  Actor_SetSpritePriority(ACTOR_SPILLED_WATER, 2);
  puddle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  do {
    wait(3);
    scaleY = puddle->scaleY;
    scaleX = puddle->scaleX;
    puddle->scaleY = scaleY + 0x1999;
    scaleX += 0x1999;
    puddle->scaleX = scaleX;
  } while (scaleX <= 0xffff);
  Actor_ShowEmote(ACTOR_WOMAN_CARRYING_WATER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                  70);
  Actor_FaceActor(ACTOR_WOMAN_CARRYING_WATER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WOMAN_CARRYING_WATER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  0);
  Actor_StartRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
  Event_Wait(70);
  Event_SetMessageId(MsgXianDoingMadeMeSpillMy);
  FieldEvent_ShowMessageThenWait(ACTOR_WOMAN_CARRYING_WATER, 0, 20);
  Scene_PlayQueuedMusic();
  if ((Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed >> 20) == 26 &&
      (Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed >> 20) > 13) {
    leaderMustStepAside = 1;
  }
  // Clear the carrier's route back to the water supply.
  if (leaderMustStepAside != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
    Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  }
  Actor_SetSpeed(ACTOR_WOMAN_CARRYING_WATER, 0xcccc, 0x6666);
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  if ((carrier->z.fixed >> 20) != 14) {
    carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
    Actor_WalkToAndWait(ACTOR_WOMAN_CARRYING_WATER, carrier->x.part.pixel, 232);
  }
  Actor_WalkToAndWait(ACTOR_WOMAN_CARRYING_WATER, 280, 232);
  GameFlag_Set(FLAG_XIAN_WATER_SPILLED);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_End();
}

/// Frost on the spilled water in Xian, before it freezes: the puddle is drawn
/// in front while the leader stands north of it.
static void Xian_UpdatePuddlePriority(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if ((leader->z.fixed >> 20) <= 13)
    Actor_SetSpritePriority(ACTOR_SPILLED_WATER, 1);
}

/// Frost freezes the spilled water in Xian: the puddle turns to ice the leader
/// can stand on, and there is nothing left to jump over.
static void Xian_FreezeSpilledWater(void) {
  struct Actor *puddle;
  s32 stationaryMotionFlags;
  s32 puddleX;

  Event_Start();
  Actor_GetById(ACTOR_SPILLED_WATER)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  stationaryMotionFlags = 0;
  Actor_GetById(ACTOR_SPILLED_WATER)->motionFlags = stationaryMotionFlags;
  puddle = Actor_GetById(ACTOR_SPILLED_WATER);
  puddleX = puddle->x.fixed;
  puddle = Actor_GetById(ACTOR_SPILLED_WATER);
  MapGrid_CopyCellAttributes(3, 17, 1, 1, (puddleX >> 20),
                             (puddle->z.fixed >> 20));
  Task_AddFrame(Xian_UpdateFrozenPuddleLayer, TASK_PASS_FRAME);
  GameFlag_Set(FLAG_XIAN_SPILLED_WATER_FROZEN);
  Actor_SetSpritePriority(ACTOR_SPILLED_WATER, 2);
  Event_End();
}

/// Talking to the kung fu student in Xian. The first time: "Hoo-whaaa...
/// Hachaaa!", the leader steps back, and the student leaps at the tree, which
/// jumps under her strike; "Heh heh heh..." Afterwards: "My kung fu is strong."
static void Xian_TalkToKungFuStudent(void) {
  s32 demonstrationSeen;
  struct Actor *leader;
  TaskFunc particleFunc;
  s32 reboundVelocity;
  s32 stepBackOffset[3];
  struct EffectOptions effectParams;

  Event_Start();
  demonstrationSeen = GameFlag_Get(FLAG_XIAN_STRIKE_SHOWN);
  if (demonstrationSeen != 0) {
    Actor_SetAngleToward(ACTOR_KUNG_FU_STUDENT, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessageId(MsgXianKungFuStrong);
    Event_ShowMessageAndWait(ACTOR_KUNG_FU_STUDENT, 0);
    Actor_ArmCallback(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 10);
    Event_End();
  } else {
    // Step back, then watch the student jump and strike the tree.
    Event_SetMessageId(MsgXianHooWhaaaHachaaa);
    Event_ShowMessageAndWait(ACTOR_KUNG_FU_STUDENT, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    SET_VECTOR(stepBackOffset, demonstrationSeen, demonstrationSeen,
               demonstrationSeen);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    FieldVector_AddPolarOffset(-PIXELS(8), leader->facing,
                               (struct FieldPosition *)stepBackOffset);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER,
                                       stepBackOffset[0] / 0x10000,
                                       stepBackOffset[2] / 0x10000);
    Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
    effectParams.priority = 1;
    FieldEffect_Spawn(PIXELS(192), 0, PIXELS(312), 0x1999, 0x3333, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(192), 0, PIXELS(312), 0x3333, 0x1999, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Sound_Play(SE_WATER);
    particleFunc = Xian_SpawnTrainingParticles;
    Task_AddFrame(particleFunc, TASK_PASS_FRAME);
    Actor_GetById(ACTOR_KUNG_FU_STUDENT)->velocityY = 0x60000;
    Actor_GetById(ACTOR_KUNG_FU_STUDENT)->gravity = 0x10000;
    Actor_GetById(ACTOR_KUNG_FU_STUDENT)->bounce = 0;
    Actor_SetSpeed(ACTOR_KUNG_FU_STUDENT, 0x30000, 0x18000);
    Actor_SetDestination(ACTOR_KUNG_FU_STUDENT, 168, 312);
    Actor_WaitForMoveById(ACTOR_KUNG_FU_STUDENT);
    Sound_Play(SE_HEAVY_IMPACT);
    Actor_SetScript(ACTOR_TRAINING_TREE, gXian_TrainingTreeJump);
    Task_AddFrame(Xian_RotateTrainingTree, TASK_PASS_FRAME);
    effectParams.type = 0x11b;
    effectParams.script = gXian_TreeImpactScript;
    effectParams.rotation = 0x4000;
    FieldEffect_Spawn(PIXELS(168), 0, PIXELS(332), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
                          EFFECT_USE_SCRIPT | EFFECT_USE_ROTATION,
                      &effectParams);
    Actor_SetDestination(ACTOR_KUNG_FU_STUDENT, 146, 312);
    Actor_WaitForMoveById(ACTOR_KUNG_FU_STUDENT);
    // Finish the impact effects and leave the student in the final pose.
    Task_Remove((void (*)(void))particleFunc);
    FieldEffect_Spawn(PIXELS(144), 0, PIXELS(312), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(144), 0, PIXELS(312), -0x3333, 0x1999, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    reboundVelocity = -0x8000;
    FieldEffect_Spawn(PIXELS(144), 0, PIXELS(312), reboundVelocity, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Actor_RefreshSelectorById(ACTOR_TRAINING_TREE);
    Sound_Play(SE_CHEST_OPEN);
    FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0x3333, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), -0x3333, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Task_Remove(Xian_RotateTrainingTree);
    Actor_GetById(ACTOR_TRAINING_TREE)->sprite->rotation = reboundVelocity;
    Actor_GetById(ACTOR_KUNG_FU_STUDENT)->bounce = 0x4000;
    Actor_GetById(ACTOR_KUNG_FU_STUDENT)->gravity = 0x10000;
    Event_Wait(30);
    Actor_ArmCallback(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 20);
    Actor_RunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_KUNG_FU_STUDENT, 0, 20);
    GameFlag_Set(FLAG_XIAN_STRIKE_SHOWN);
    Scene_PlayQueuedMusic();
    Event_End();
  }
}

/// Feizhi asks, in Xian, whether the party are strong warriors. She is by the
/// south exit after she has left Master Feh's school, and before Xian hears of
/// Hsu. "You are the warriors from the school... Are you strong warriors?" A
/// yes: she hopes they will go west; a no: very few warriors would call
/// themselves weak.
static void Xian_TalkToFeizhi(void) {
  Event_Start();
  Event_SetMessageId(MsgXianWarriorsFromSchoolStrongWarriors);
  Event_AskYesNoAndShowResponse(ACTOR_FEIZHI, 0);
  Event_End();
}

/// Walking into the spilled water in Xian before it is frozen: the leader jumps
/// two cells over it, where nothing stands in the way.
static void Xian_JumpOverSpilledWater(void) {
  s32 savedMotionFlags;
  struct Actor *leader;
  struct FieldPosition landingPosition;
  struct FieldPosition *landing;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
  if (GameFlag_Get(FLAG_XIAN_WATER_SPILLED) != 0) {
    landing = &landingPosition;
    landing->x = (leader->x.fixed & -0x100000) + PIXELS(8);
    landing->y = leader->y.fixed;
    landing->z = (leader->z.fixed & -0x100000) + PIXELS(8);
    FieldVector_AddPolarOffset(PIXELS(32), (leader->facing + 0x2000) & 0xc000,
                               landing);
    if (FieldActor_CheckMovementCollision(leader, landing) == 0) {
      Event_Start();
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(6);
      Sound_Play(SE_JUMP);
      Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
      leader->speedLimit = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      leader->motionFlags &= 126;
      Actor_SetSpriteFlags(leader, 0);
      Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, landing->x >> 16,
                                 landing->z >> 16);
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      Actor_SetSpriteFlags(leader, 1);
      leader->motionFlags = savedMotionFlags;
      Event_End();
    }
  }
}

/// Walks the leader through a doorway and leaves by its exit; through the
/// Sanctum's door the leader walks straight in, without centering on it.
static void Xian_WalkThroughExit(s32 exitNumber) {
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  if (exitNumber == XIAN_TO_SANCTUM) {
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  }
  gFieldWork->transitionFrames = 16;
  Import_EventSetExitRequest(exitNumber);
}

/// Walking north into one of Xian's doors: it opens and the party goes in. Each
/// door's cell behavior is the number of the exit it leads by: five lead
/// into the houses, shops and inn, XIAN_TO_SANCTUM into Xian Sanctum (SE_THUMP)
/// and XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL into Master Feh's school.
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

/// Force on the training tree in Xian: it jumps, and the student cries out.
/// "Wha-Wha-What happened? The tree jumped on its own!", the next time
/// "...It jumped again... I am too tired...".
static void Xian_StrikeTrainingTreeWithForce(void) {
  TaskFunc rotationFunc;

  Event_Start();
  Actor_SetScript(ACTOR_TRAINING_TREE, gXian_TrainingTreeJump);
  rotationFunc = Xian_RotateTrainingTree;
  Task_AddFrame(rotationFunc, TASK_PASS_FRAME);
  Actor_RefreshSelectorById(ACTOR_TRAINING_TREE);
  Sound_Play(SE_CHEST_OPEN);
  FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0x3333, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), -0x3333, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  Task_Remove(rotationFunc);
  Actor_GetById(ACTOR_TRAINING_TREE)->sprite->rotation = 0x8000;
  Import_ActorSetPosition(ACTOR_FORCE_TARGET, PIXELS(168), PIXELS(312));
  Event_Wait(20);
  Actor_SetAngleToward(ACTOR_KUNG_FU_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_RunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
  Event_Wait(10);
  Event_SetMessageId(MsgXianWhaWhaHappened);
  if (Event_GetFlag(FLAG_XIAN_TREE_JUMPED)) {
    gFieldWork->message++;
  }
  Event_ShowMessageAndWait(ACTOR_KUNG_FU_STUDENT, 0);
  GameFlag_Set(FLAG_XIAN_TREE_JUMPED);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Raised in Xian when the leader has stood in the water carrier's way for two
/// seconds: both walk to the start of her route, the leader turns west, and she
/// sets off again.
static void Xian_MoveAsideForWaterCarrier(void) {
  struct Actor *carrier;

  Event_Start();
  Actor_SetMovementTypeById(ACTOR_WOMAN_CARRYING_WATER, MOVEMENT_TYPE_STAND);
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->update = 0;
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->targetX = ACTOR_NO_TARGET;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrier->targetZ = ACTOR_NO_TARGET;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrier->velocityX = 0;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrier->velocityZ = 0;
  carrier = Actor_GetById(ACTOR_WOMAN_CARRYING_WATER);
  carrier->speedLimit = 0;
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->acceleration = 0;
  Actor_ShowEmote(ACTOR_WOMAN_CARRYING_WATER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  0);
  Actor_StartRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_WOMAN_CARRYING_WATER, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
  Actor_WalkTo(ACTOR_WOMAN_CARRYING_WATER, 280, 232);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 232);
  Actor_WaitForMoveById(ACTOR_WOMAN_CARRYING_WATER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetScript(ACTOR_WOMAN_CARRYING_WATER, gXian_WaterCarrierRoute);
  carrier = Actor_Get(ACTOR_WOMAN_CARRYING_WATER);
  carrier->update = XIAN_WATER_CARRIER_UPDATE;
  Event_End();
}

const struct Trigger *Xian_GetTriggers(void) { return gXian_Triggers; }

/// Queues and plays SE_AMBIENT_WIND and sets up the village as the story and
/// this visit left it: after the news of Hsu (or, back from a battle, after her
/// strike) the kung fu student stands where her strike leaves her; Feizhi is
/// gone after the news or once she has seen the rockslide; back from a battle,
/// the frozen puddle keeps its ice; the water carrier is hooked to
/// Xian_WatchWaterCarrierPath; and the training tree, its Force mark and the
/// three crates are drawn under the actors.
s32 Xian_Initialize(void) {
  struct Actor *actor;
  u32 crateIndex;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Scene_QueueMusic(SE_AMBIENT_WIND);
  // Back from the school (XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) or the
  // Sanctum (XIAN_FROM_SANCTUM_WEST), the last two entrances, the name
  // is not shown.
  if (gAdventure.entrance >= XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    Actor_FaceDirection(ACTOR_BOY_ON_SILK_CLOTHES, FACING_WEST, 0);
    Actor_SetPosition(ACTOR_KUNG_FU_STUDENT, PIXELS(146), PIXELS(312));
    Actor_ArmCallback(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
    if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU)) {
      Import_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
    }
  }
  // Gone once she has found the road blocked (Alpine Crossing's
  // FLAG_FEIZHI_SAW_ROCKSLIDE).
  if (GameFlag_Get(FLAG_FEIZHI_SAW_ROCKSLIDE)) {
    Import_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
  }
  // Set the three crates up as Actor_CreateConfigured sets up
  // its actors: still, not solid, never a target and hidden.
  // XXX (no evidence): what the crates are for; nothing reads them again.
  for (crateIndex = 0; crateIndex <= 2; crateIndex++) {
    struct Actor *actor;
    actor = Actor_GetById(crateIndex + ACTOR_FIRST_CRATE);
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags = (actor->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) |
                           ACTOR_PRIORITY_UNDERFOOT;
  }
  if (GameFlag_Get(FLAG_XIAN_STRIKE_SHOWN)) {
    Actor_SetPosition(ACTOR_KUNG_FU_STUDENT, PIXELS(146), PIXELS(312));
    Actor_ArmCallback(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
  }
  // Restore the frozen puddle and tasks when re-entering the village.
  if (GameFlag_Get(FLAG_XIAN_SPILLED_WATER_FROZEN)) {
    Import_ActorSetAnimation(ACTOR_SPILLED_WATER, PUDDLE_ANIMATION_FROZEN);
    {
      s32 puddleX = Actor_GetById(ACTOR_SPILLED_WATER)->x.fixed;

      MapGrid_CopyCellAttributeRect(
          3, 17, 1, 1, puddleX >> 20,
          Actor_GetById(ACTOR_SPILLED_WATER)->z.fixed >> 20);
    }
    Task_AddFrame(Xian_UpdateFrozenPuddleLayer, TASK_PASS_FRAME);
  }
  // The water carrier's sprite is drawn with its green and blue swapped.
  Actor_ConfigureSpriteColor(ACTOR_WOMAN_CARRYING_WATER, SPRITE_COLOR_RBG);
  Actor_GetById(ACTOR_WOMAN_CARRYING_WATER)->update = XIAN_WATER_CARRIER_UPDATE;
  actor = Actor_GetById(ACTOR_TRAINING_TREE);
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(16);
  actor->targetY = PIXELS(16);
  actor->scaleX = 0x8ccc;
  actor->scaleY = 0x6666;
  actor->sprite->rotation = 0x8000;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FORCE_TARGET), 0);
  Actor_GetById(ACTOR_FORCE_TARGET)->motionFlags = 0;
  Actor_GetById(ACTOR_FORCE_TARGET)->y.fixed = 0;
  Actor_GetById(ACTOR_FORCE_TARGET)->targetY = ACTOR_NO_TARGET;
  return 0;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The water carrier's round: from (280, 232) east to (424, 232), up to
/// (424, 216) and back, pausing at each end.
s32 gXian_WaterCarrierRoute[] = {
    SC_SET(SW_SPEED, 0x6666),
    SC_SET(SW_ACCELERATION, 0x1999),
    SC_WAIT(60),
    SC_LABEL(1),
    SC_ANIMATION(3),
    SC_WAIT(60),
    SC_ANIMATION(3),
    SC_WAIT(180),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(424), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(424), 0, PIXELS(216)),
    SC_SYNC,
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST + FACING_STEP),
    SC_WAIT(10),
    SC_ANIMATION(3),
    SC_WAIT(60),
    SC_ANIMATION(3),
    SC_WAIT(120),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(424), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(280), 0, PIXELS(232)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// The struck tree jumps 32 pixels and drops back.
s32 gXian_TrainingTreeJump[] = {
    SC_SET(SW_SNAP, 0),
    SC_SET(SW_SPEED, 0x30000),
    SC_SET(SW_ACCELERATION, 0x18000),
    SC_MOVE(PIXELS(168), PIXELS(48), PIXELS(312)),
    SC_SYNC,
    SC_SET(SW_SPEED, 0x30000),
    SC_SET(SW_ACCELERATION, 0x1999),
    SC_SET(SW_SNAP, 1),
    SC_MOVE(PIXELS(168), PIXELS(16), PIXELS(312)),
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
    { SPRITE_YOUNG_MAN_IN_BLUE_JACKET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(448), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(420), 0, PIXELS(304), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_IN_BLUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(200), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(312), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_MUSCULAR_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(428), 0, PIXELS(432), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(440), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_FEIZHI, IF_FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU), MOVEMENT_TYPE_STAND,
        PIXELS(368), 0, PIXELS(496), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0 },
    { SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)gXian_WaterCarrierRoute,
        PIXELS(280), 0, PIXELS(232), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(312), FACING_WEST, TALK_FACING_PARTY, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI(DJINNI_MIST), MOVEMENT_TYPE_STAND,
        PIXELS(448), 0, PIXELS(328), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(426), 0, PIXELS(428), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(299), 0, PIXELS(222), FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(280), FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct Trigger gXian_Triggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Xian_OpenDoorAndLeave) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, XIAN_TO_WORLD_MAP },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 20, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 21, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 2, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(Xian_JumpOverSpilledWater) },
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
    { TRIGGER_RAISED, RAISED_CARRIER_BLOCKED, CONDITION_ALWAYS, EVENT(Xian_MoveAsideForWaterCarrier) },
    { TRIGGERS_END },
};
// clang-format on
