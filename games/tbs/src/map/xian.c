/// @file
/// MapId_Xian, the village of Xian. Its houses, shops and inn are
/// xian_interiors.c; Master Feh's school is a room of the Fuchin Temple overlay
/// (FuchinTempleInteriors, which XIAN_TO_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL
/// leads into; fuchin_temple.c).
///
/// The village's talk turns on FLAG_XIAN_HEARD_HSU_TRAPPED, which Master
/// Hama's Reveal lesson at Lama Temple sets: every villager's talk and Mind
/// Read has a row for each side of it, and after it Xian_Initialize also turns
/// LOCALID_BOY_ON_SILK_CLOTHES west and stands the kung fu student where her
/// strike leaves her, and Xian_GetObjectEventTemplates turns
/// LOCALID_MUSCULAR_MAN_ON_DISASTERS to face the party. Feizhi stands by the
/// village's south exit from FLAG_FEIZHI_WENT_TO_HSU (set in fuchin_temple.c)
/// until she has found the road blocked at Alpine Crossing
/// (FLAG_FEIZHI_SAW_ROCKSLIDE) or Xian has heard the news.
///
/// Two small scenes run on map flags, which last until the party next
/// enters a map other than by returning from a battle:
/// - the water carrier walks her round (gXian_WaterCarrierRoute). Spoken to on
///   its eastern part, she spills her water (FLAG_MAP_TEMP_00): a puddle
///   spreads at her feet, its cell takes a metatile behavior that walking into
///   jumps the leader over it (Xian_JumpOverSpilledWater), and she walks back
///   for more. Frost freezes the puddle (FLAG_MAP_TEMP_01) into ice that can be
///   stood on. A leader who stands in her way for two seconds makes her raise
///   XIAN_TRIGGER_CARRIER_BLOCKED (Xian_WatchWaterCarrierPath), and both walk
///   back to the start of her round.
/// - the kung fu student, a little girl, shows her strike on the tree before
///   her once (FLAG_MAP_TEMP_02), and Force on the tree makes it jump on its
///   own, to her surprise (FLAG_MAP_TEMP_03 picks her second line).
///
/// Mist, a Mercury Djinni, waits in the village until she joins, which she
/// does when spoken to.
///
/// A jar holds a Lucky Medal. Five doors lead into the houses, shops and inn
/// (MapId_XianInteriors, XIAN_TO_HOUSE to XIAN_TO_WEAPON_AND_ARMOR_SHOP),
/// XIAN_TO_SANCTUM into Xian Sanctum, its sliding door playing SE_188,
/// XIAN_TO_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL into Master Feh's school, and
/// XIAN_TO_WORLD_MAP out to the world map. Coming back from the school
/// (XIAN_FROM_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL) or the Sanctum
/// (XIAN_FROM_SANCTUM_WEST) clears FLAG_SHOW_LOCATION_NAME, so the name is not
/// shown again.
///
/// Open questions:
///
/// - XXX (no evidence): why Xian places three crates as object events
///   (LOCALID_FIRST_CRATE on) that Xian_Initialize makes invisible
///   (SPRITE_COLOR_HIDDEN), not solid and never a target, so that the player
///   neither sees nor meets them; nothing in the ROM reads them again, so only
///   the authors' intent would explain them.
#include "game_state.h"
#include "djinn_ids.h"
#include "unit/import.h"
#include "unit/map/xian.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "macros.h"

#define NULL ((void *)0)
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "map_effect_sequence.h"
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
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Xian's temporary flags:
//   FLAG_MAP_TEMP_00  the water carrier has spilled her water
//   FLAG_MAP_TEMP_01  the spilled water is frozen
//   FLAG_MAP_TEMP_02  the kung fu student has shown her strike on the tree
//   FLAG_MAP_TEMP_03  Force has made the tree jump once

/// The object events, numbering the rows of gXian_ObjectEventTemplates. Each
/// person is named by what their sprite shows and what they talk of before Xian
/// hears of Hsu.
enum XianLocalId {
  /// A young man in a blue jacket.
  /// ★ Named from the game: "This is Xian. Travelers are welcome here!"
  /// (MsgXianThisIsXianTravelersAreWelcome)
  LOCALID_YOUNG_MAN_WELCOMING_TRAVELERS = LOCALID_FIRST_PLACED,
  /// A young woman (Xian_AskAboutYoungWarriors).
  /// ★ Named from the game: "Young warriors, very gallant! You came through
  /// Mogall Forest?" (MsgXianYoungWarriorsVeryGallantCame)
  LOCALID_WOMAN_ON_YOUNG_WARRIORS,
  /// A man with a topknot.
  /// ★ Named from the game: "Xian is the village of kinu. Kinu means silk."
  /// (MsgXianXianIsTheVillageOfKinu)
  LOCALID_MAN_ON_KINU,
  /// An old man.
  /// ★ Named from the game: "Martial arts are very popular in Xian."
  /// (MsgXianMartialArtsAreVeryPopularIn)
  LOCALID_OLD_MAN_ON_MARTIAL_ARTS,
  /// An old woman with a cane.
  /// ★ Named from the game: "You warriors should visit Master Feh's school. You
  /// will be surprised!" (MsgXianYouWarriorsShouldVisitMasterFehs)
  LOCALID_OLD_WOMAN_ON_FEHS_SCHOOL,
  /// A small boy.
  /// ★ Named from the game: "My clothes are not silk. Silk is very expensive."
  /// (MsgXianMyClothesAreNotSilkSilk)
  LOCALID_BOY_ON_SILK_CLOTHES,
  /// A little girl who strikes the tree before her with a shout.
  /// ★ Named from the game: "Hoo-whaaa... Hachaaa!" (MsgXianHooWhaaaHachaaa)
  LOCALID_KUNG_FU_STUDENT,
  /// A bare-chested, muscular man.
  /// ★ Named from the game: "Disasters from the sky... Many, many thieves..."
  /// (MsgXianDisastersFromTheSkyManyMany)
  LOCALID_MUSCULAR_MAN_ON_DISASTERS,
  /// A young woman.
  /// ★ Named from the game: "This is the mulberry orchard."
  /// (MsgXianThisIsTheMulberryOrchardWhen)
  LOCALID_WOMAN_IN_THE_MULBERRY_ORCHARD,
  /// Feizhi, as at Alpine Crossing: looking for someone to take her west.
  /// ★ Named from the game: "You must be glad, Feizhi." (MsgMountainHsuOkay)
  LOCALID_FEIZHI,
  /// A young woman who carries water along the street. "You made me spill my
  /// water!"
  /// ★ Named from the game: "I must carry water every day."
  /// (MsgXianIMustCarryWaterEveryDay)
  LOCALID_WOMAN_CARRYING_WATER,
  /// The tree the student strikes; it jumps when hit.
  /// ★ Named from the game: "The tree jumped on its own!"
  /// (MsgXianWhaWhaHappened)
  LOCALID_TRAINING_TREE,
  /// The spilled water, which Frost freezes.
  /// ★ Named from the game: "You made me spill my water!"
  /// (MsgXianDoingMadeMeSpillMy)
  LOCALID_SPILLED_WATER,
  /// Force's mark on the training tree.
  /// ★ Named from what the player sees: nothing of its own, a blank object
  /// event on the bare trunk the student strikes (its render)
  LOCALID_FORCE_TARGET,
  /// Mist, a Mercury Djinni.
  /// ★ Named from the game: "Mist" (MsgDjinnName)
  LOCALID_MIST,
  /// The first of three crates placed with SPRITE_CRATE, which Xian_Initialize
  /// hides (SPRITE_COLOR_HIDDEN): the player never sees them.
  /// ★ Named from the game: "Strange... I don't remember all these crates being
  /// up here before!" (MsgVaultVaultCutFree)
  LOCALID_FIRST_CRATE
};

/// SPRITE_PUDDLE's animation once Frost has frozen it into ice.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

/// The event the water carrier raises when the leader has stood in her way
/// for two seconds.
enum { XIAN_TRIGGER_CARRIER_BLOCKED = 200, XIAN_CARRIER_BLOCKED_FRAMES = 120 };

/// Talking to Mist in Xian: Mist, a Mercury Djinni, joins the party.
static void Xian_MistJoins(void) {
  Djinn_JoinParty(LOCALID_MIST, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
}

/// Draws the frozen puddle under the leader while the leader stands on top of
/// it.
static void Xian_UpdateFrozenPuddleLayer(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->y.part.pixel > 31) {
    ObjectEvent_GetById(LOCALID_SPILLED_WATER)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
  } else {
    ObjectEvent_GetById(LOCALID_SPILLED_WATER)->priorityFlags &=
        ~OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
}

/// Rotates the training tree during the student's strike and Force animation.
static void Xian_RotateTrainingTree(void) {
  struct ObjectEvent *tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  struct Sprite *sprite = tree->sprite;
  sprite->rotation += 0x1400;
}

/// Emits particles beside the student every fourth frame.
static void Xian_SpawnTrainingParticles(void) {
  // The byte view keeps the Z load ahead of the options-script store.
  u8 *studentObjectEvent = (u8 *)ObjectEvent_GetById(LOCALID_KUNG_FU_STUDENT);

  if ((gFrameCount & 3) == 0) {
    struct EffectOptions config;
    config.priority = 1;
    config.palette = 9;
    config.type = 169;
    config.script = gXian_TrainingParticleScript;
    FieldEffect_Spawn(
        *(s32 *)(studentObjectEvent + offsetof(struct ObjectEvent, x)),
        *(s32 *)(studentObjectEvent + offsetof(struct ObjectEvent, y)),
        *(s32 *)(studentObjectEvent + offsetof(struct ObjectEvent, z)) -
            PIXELS(1),
        0, -PIXELS(1), -PIXELS(1),
        EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE | EFFECT_USE_TYPE |
            EFFECT_USE_SCRIPT,
        &config);
  }
}

#include "common/object_distance.h"

/// Tracks a nearby target only within the object event's current facing sector.
static s32 Xian_UpdateCarrierTracking(struct ObjectEvent *objectEvent,
                                      struct ObjectEvent *target,
                                      s32 distanceLimit, s32 forceTracking) {
  s32 isTracking = 0;

  // This route uses paused/timer as its tracking latch and mode.
  if (objectEvent->paused == 1 && objectEvent->timer == 0) {
    ObjectEvent_SetMode(objectEvent, 1);
    return 1;
  }

  if (FixedPoint_Distance((const u8 *)target + offsetof(struct ObjectEvent, x),
                          (const u8 *)objectEvent +
                              offsetof(struct ObjectEvent, x)) <
          distanceLimit ||
      forceTracking != 0) {
    u16 angle = ArcTan2(target->z.fixed - objectEvent->z.fixed,
                        target->x.fixed - objectEvent->x.fixed);
    s32 previousHeading = (angle - 0x1000) & 0xf000;
    s32 nextHeading = (angle + 0x1000) & 0xf000;
    s32 targetHeading = angle & 0xf000;
    s32 currentHeading = objectEvent->facing & 0xf000;

    if (targetHeading == currentHeading || nextHeading == currentHeading ||
        previousHeading == currentHeading || forceTracking != 0) {
      objectEvent->paused = 1;
      ObjectEvent_SetMode(objectEvent, 1);
      isTracking = 1;
      objectEvent->timer = isTracking;
    } else {
      objectEvent->paused = 0;
      ObjectEvent_SetMode(objectEvent, 2);
      objectEvent->timer = 0;
    }
  } else {
    objectEvent->paused = 0;
    ObjectEvent_SetMode(objectEvent, 2);
    objectEvent->timer = 0;
  }

  return isTracking;
}

// Ask the carrier to move after the leader blocks her route for two seconds.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 154 to 150 bytes in tbs-en.
#ifdef UBFIX
static void Xian_WatchWaterCarrierPath(union FieldObject *fieldObject) {
#else
static s32 Xian_WatchWaterCarrierPath(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *self = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *leader;
  s32 near = 0;

  if (self->targetX == OBJ_EVENT_NO_TARGET &&
      self->targetZ == OBJ_EVENT_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
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
    gFieldWork->raisedEvent = XIAN_TRIGGER_CARRIER_BLOCKED;
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

const struct MapWarpDest *Xian_GetWarpDests(void) { return gXian_WarpDests; }

const struct MapWarpRegion *Xian_GetRegions(void) { return NULL; }

const u32 *Xian_GetWarps(void) { return gXian_Warps; }

/// Once Xian has heard Hsu is trapped, LOCALID_MUSCULAR_MAN_ON_DISASTERS turns
/// to face the party.
const struct ObjectEventTemplate *Xian_GetObjectEventTemplates(void) {
  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0)
    gXian_ObjectEventTemplates[LOCALID_MUSCULAR_MAN_ON_DISASTERS -
                               LOCALID_FIRST_PLACED]
        .talkFacing = TALK_FACING_PARTY;
  return gXian_ObjectEventTemplates;
}

/// A villager in Xian asks whether the young warriors came through Mogall
/// Forest. She is LOCALID_WOMAN_ON_YOUNG_WARRIORS, and asks before Xian hears
/// of Hsu. "Young warriors, very gallant! You came through Mogall Forest?" A
/// yes earns respect; a no is a lie, since Silk Road from Altin is blocked.
static void Xian_AskAboutYoungWarriors(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgXianYoungWarriorsVeryGallantCame);
  Engine_ScriptAskYesNo(LOCALID_WOMAN_ON_YOUNG_WARRIORS, 0);
  Engine_ScriptEnd();
}

/// Talking to the water carrier in Xian. While she carries her water east of
/// x = 320 the first time, she spills it: a puddle spreads at her feet, she
/// cries "You made me spill my water!" and walks back west to fetch more, the
/// leader stepping aside if he is in her way. Otherwise she only answers: "Do
/// not speak to me. I might spill my water.", or once it is spilled, "Now I
/// must get water again."
static void Xian_TalkToWaterCarrier(void) {
  struct ObjectEvent *puddle;
  struct ObjectEvent *carrier;
  struct ObjectEvent *leader;
  s32 leaderMustStepAside;
  s32 savedFacing;
  s32 carrierX;
  s32 carrierCellZ;
  s32 carrierColumnX;
  s32 scaleY;
  s32 scaleX;

  puddle = ObjectEvent_GetById(LOCALID_SPILLED_WATER);
  Engine_ScriptStart();
  leaderMustStepAside = 0;
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->update = NULL;
  if (GameFlag_IsSet(FLAG_MAP_TEMP_00) != 0 ||
      (ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER)->x.fixed >> 20) <=
          19) {
    carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
    savedFacing = carrier->facing;
    ObjectEvent_FaceObjectEvent(LOCALID_WOMAN_CARRYING_WATER,
                                LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(10);
    Engine_ScriptSetMessage(MsgXianNowMustGetWaterAgain);
    if (GameFlag_IsSet(FLAG_MAP_TEMP_00) == 0) {
      gFieldWork->message++;
      Script_ShowMessage(LOCALID_WOMAN_CARRYING_WATER, 0);
      ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER)->timer = 0;
      carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
      carrier->facing = savedFacing;
    } else {
      Script_ShowMessage(LOCALID_WOMAN_CARRYING_WATER, 0);
      ObjectEvent_FaceDirection(LOCALID_WOMAN_CARRYING_WATER, FACING_WEST, 20);
    }
    carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
    carrier->update = XIAN_WATER_CARRIER_UPDATE;
    Engine_ScriptEnd();
    return;
  }
  // Make room for the carrier before spilling the water.
  if ((ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed >> 19) > 27 &&
      (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed >> 19) <= 29 &&
      (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed >> 20) != 26) {
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER,
                                      LOCALID_WOMAN_CARRYING_WATER, 0);
    Engine_ScriptWait(5);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
    if (leader->x.fixed < carrier->x.fixed) {
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
      ObjectEvent_WalkTo(LOCALID_PARTY_LEADER,
                         ((carrier->x.fixed >> 20) << 4) - 8, 232);
      leaderMustStepAside = 1;
    } else {
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
          ~OBJ_EVENT_FACES_MOTION;
      carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
      ObjectEvent_WalkTo(LOCALID_PARTY_LEADER,
                         ((carrier->x.fixed >> 20) << 4) + 24, 232);
    }
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  }
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->targetX = OBJ_EVENT_NO_TARGET;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrier->targetY = OBJ_EVENT_NO_TARGET;
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->targetZ = OBJ_EVENT_NO_TARGET;
  ObjectEvent_SetBuiltinAction(LOCALID_WOMAN_CARRYING_WATER,
                               OBJ_EVENT_ACTION_IDLE);
  Engine_ObjectEventSetAnimation(LOCALID_WOMAN_CARRYING_WATER, ANIMATION_STAND);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WOMAN_CARRYING_WATER, 2);
  Engine_ScriptWait(10);
  Script_PlaySong(SE_228);
  // Copy the wet terrain cell and grow the puddle into view.
  puddle->scaleX = 0x4ccc;
  puddle->scaleY = 0x4ccc;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrierX = carrier->x.fixed;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrierCellZ = carrier->z.fixed >> 20;
  ObjectEvent_SetPosition(LOCALID_SPILLED_WATER,
                          ((carrierX >> 20) << 20) + PIXELS(8),
                          (carrierCellZ << 20) + PIXELS(8));
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrierColumnX = carrier->x.fixed;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  MapGrid_CopyCellAttributes(16, 16, 1, 1, carrierColumnX >> 20,
                             carrier->z.fixed >> 20);
  Engine_ObjectEventSetSpritePriority(LOCALID_SPILLED_WATER, 2);
  puddle->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
  do {
    Engine_TaskWait(3);
    scaleY = puddle->scaleY;
    scaleX = puddle->scaleX;
    puddle->scaleY = scaleY + 0x1999;
    scaleX += 0x1999;
    puddle->scaleX = scaleX;
  } while (scaleX <= 0xffff);
  ObjectEvent_ShowEmote(LOCALID_WOMAN_CARRYING_WATER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  ObjectEvent_FaceObjectEvent(LOCALID_WOMAN_CARRYING_WATER,
                              LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_WOMAN_CARRYING_WATER,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WOMAN_CARRYING_WATER, 2);
  Engine_ScriptWait(70);
  Engine_ScriptSetMessage(MsgXianDoingMadeMeSpillMy);
  Script_ShowMessageAndWait(LOCALID_WOMAN_CARRYING_WATER, 0, 20);
  Map_PlayQueuedMusic();
  if ((ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed >> 20) == 26 &&
      (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed >> 20) > 13) {
    leaderMustStepAside = 1;
  }
  // Clear the carrier's route back to the water supply.
  if (leaderMustStepAside != 0) {
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0xcccc, 0x6666);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 10);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
    ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, 16);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  }
  ObjectEvent_SetSpeed(LOCALID_WOMAN_CARRYING_WATER, 0xcccc, 0x6666);
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  if ((carrier->z.fixed >> 20) != 14) {
    carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
    ObjectEvent_WalkToAndWait(LOCALID_WOMAN_CARRYING_WATER,
                              carrier->x.part.pixel, 232);
  }
  ObjectEvent_WalkToAndWait(LOCALID_WOMAN_CARRYING_WATER, 280, 232);
  GameFlag_Set(FLAG_MAP_TEMP_00);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  Engine_ScriptEnd();
}

/// Frost on the spilled water in Xian, before it freezes: the puddle is drawn
/// in front while the leader stands north of it.
static void Xian_UpdatePuddlePriority(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if ((leader->z.fixed >> 20) <= 13)
    Engine_ObjectEventSetSpritePriority(LOCALID_SPILLED_WATER, 1);
}

/// Frost freezes the spilled water in Xian: the puddle turns to ice the leader
/// can stand on, and there is nothing left to jump over.
static void Xian_FreezeSpilledWater(void) {
  struct ObjectEvent *puddle;
  s32 stationaryMotionFlags;
  s32 puddleX;

  Engine_ScriptStart();
  ObjectEvent_GetById(LOCALID_SPILLED_WATER)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_UNDERFOOT;
  stationaryMotionFlags = 0;
  ObjectEvent_GetById(LOCALID_SPILLED_WATER)->motionFlags =
      stationaryMotionFlags;
  puddle = ObjectEvent_GetById(LOCALID_SPILLED_WATER);
  puddleX = puddle->x.fixed;
  puddle = ObjectEvent_GetById(LOCALID_SPILLED_WATER);
  MapGrid_CopyCellAttributes(3, 17, 1, 1, (puddleX >> 20),
                             (puddle->z.fixed >> 20));
  FieldScheduler_AddCallback(Xian_UpdateFrozenPuddleLayer, TASK_PRIORITY_MAP);
  GameFlag_Set(FLAG_MAP_TEMP_01);
  Engine_ObjectEventSetSpritePriority(LOCALID_SPILLED_WATER, 2);
  Engine_ScriptEnd();
}

/// Talking to the kung fu student in Xian. The first time: "Hoo-whaaa...
/// Hachaaa!", the leader steps back, and the student leaps at the tree, which
/// jumps under her strike; "Heh heh heh..." Afterwards: "My kung fu is strong."
static void Xian_TalkToKungFuStudent(void) {
  s32 demonstrationSeen;
  struct ObjectEvent *leader;
  TaskFunc particleCallback;
  s32 reboundVelocity;
  s32 stepBackOffset[3];
  struct EffectOptions effectParams;

  Engine_ScriptStart();
  demonstrationSeen = Engine_GameFlagIsSet(FLAG_MAP_TEMP_02);
  if (demonstrationSeen != 0) {
    Engine_ObjectEventFaceObjectEvent(LOCALID_KUNG_FU_STUDENT,
                                      LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(10);
    Engine_ScriptSetMessage(MsgXianKungFuStrong);
    Engine_ScriptShowMessage(LOCALID_KUNG_FU_STUDENT, 0);
    Engine_ObjectEventFaceDirection(LOCALID_KUNG_FU_STUDENT, FACING_EAST, 10);
    Engine_ScriptEnd();
  } else {
    // Step back, then watch the student jump and strike the tree.
    Engine_ScriptSetMessage(MsgXianHooWhaaaHachaaa);
    Engine_ScriptShowMessage(LOCALID_KUNG_FU_STUDENT, 0);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    SET_VECTOR(stepBackOffset, demonstrationSeen, demonstrationSeen,
               demonstrationSeen);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    FieldVector_AddPolarOffset(-PIXELS(8), leader->facing,
                               (struct FieldPosition *)stepBackOffset);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
    Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER,
                                           stepBackOffset[0] / 0x10000,
                                           stepBackOffset[2] / 0x10000);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
        OBJ_EVENT_FACES_MOTION;
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_KUNG_FU_STUDENT, 2);
    effectParams.priority = 1;
    FieldEffect_Spawn(PIXELS(192), 0, PIXELS(312), 0x1999, 0x3333, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(192), 0, PIXELS(312), 0x3333, 0x1999, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Engine_AudioPlayCue(SE_132);
    particleCallback = Xian_SpawnTrainingParticles;
    Task_AddCallback(particleCallback, TASK_PRIORITY_MAP);
    ObjectEvent_GetById(LOCALID_KUNG_FU_STUDENT)->velocityY = 0x60000;
    ObjectEvent_GetById(LOCALID_KUNG_FU_STUDENT)->gravity = 0x10000;
    ObjectEvent_GetById(LOCALID_KUNG_FU_STUDENT)->bounce = 0;
    ObjectEvent_SetSpeed(LOCALID_KUNG_FU_STUDENT, 0x30000, 0x18000);
    ObjectEvent_SetDestination(LOCALID_KUNG_FU_STUDENT, 168, 312);
    Engine_ObjectEventWaitForMove(LOCALID_KUNG_FU_STUDENT);
    Engine_AudioPlayCue(SE_HEAVY_IMPACT);
    ObjectEvent_SetActionScript(LOCALID_TRAINING_TREE, gXian_TrainingTreeJump);
    FieldScheduler_AddCallback(Xian_RotateTrainingTree, TASK_PRIORITY_MAP);
    effectParams.type = 0x11b;
    effectParams.script = gXian_TreeImpactScript;
    effectParams.rotation = 0x4000;
    FieldEffect_Spawn(PIXELS(168), 0, PIXELS(332), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
                          EFFECT_USE_SCRIPT | EFFECT_USE_ROTATION,
                      &effectParams);
    ObjectEvent_SetDestination(LOCALID_KUNG_FU_STUDENT, 146, 312);
    Engine_ObjectEventWaitForMove(LOCALID_KUNG_FU_STUDENT);
    // Finish the impact effects and leave the student in the final pose.
    Engine_TaskRemoveCallback((void (*)(void))particleCallback);
    FieldEffect_Spawn(PIXELS(144), 0, PIXELS(312), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(144), 0, PIXELS(312), -0x3333, 0x1999, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    reboundVelocity = -0x8000;
    FieldEffect_Spawn(PIXELS(144), 0, PIXELS(312), reboundVelocity, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    ObjectEvent_RefreshSelectorById(LOCALID_TRAINING_TREE);
    Engine_AudioPlayCue(SE_124);
    FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0x3333, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), -0x3333, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
    Engine_TaskRemoveCallback(Xian_RotateTrainingTree);
    ObjectEvent_GetById(LOCALID_TRAINING_TREE)->sprite->rotation =
        reboundVelocity;
    ObjectEvent_GetById(LOCALID_KUNG_FU_STUDENT)->bounce = 0x4000;
    ObjectEvent_GetById(LOCALID_KUNG_FU_STUDENT)->gravity = 0x10000;
    Engine_ScriptWait(30);
    Engine_ObjectEventFaceDirection(LOCALID_KUNG_FU_STUDENT, FACING_EAST, 20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_KUNG_FU_STUDENT, 2);
    Engine_ScriptWait(20);
    Engine_ScriptShowMessageAndWait(LOCALID_KUNG_FU_STUDENT, 0, 20);
    Engine_GameFlagSet(FLAG_MAP_TEMP_02);
    Map_PlayQueuedMusic();
    Engine_ScriptEnd();
  }
}

/// Feizhi asks, in Xian, whether the party are strong warriors. She is by the
/// south exit after she has left Master Feh's school, and before Xian hears of
/// Hsu. "You are the warriors from the school... Are you strong warriors?" A
/// yes: she hopes they will go west; a no: very few warriors would call
/// themselves weak.
static void Xian_TalkToFeizhi(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgXianWarriorsFromSchoolStrongWarriors);
  Engine_ScriptAskYesNo(LOCALID_FEIZHI, 0);
  Engine_ScriptEnd();
}

/// Walking into the spilled water in Xian before it is frozen: the leader jumps
/// two cells over it, where nothing stands in the way.
static void Xian_JumpOverSpilledWater(void) {
  s32 savedMotionFlags;
  struct ObjectEvent *leader;
  struct FieldPosition landingPosition;
  struct FieldPosition *landing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_00) != 0) {
    landing = &landingPosition;
    landing->x = (leader->x.fixed & -0x100000) + PIXELS(8);
    landing->y = leader->y.fixed;
    landing->z = (leader->z.fixed & -0x100000) + PIXELS(8);
    FieldVector_AddPolarOffset(PIXELS(32), (leader->facing + 0x2000) & 0xc000,
                               landing);
    if (FieldObject_CheckMovementCollision(leader, landing) == 0) {
      Engine_ScriptStart();
      ObjectEvent_SetMode(leader, ANIMATION_LEADER_SPRING);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(SE_JUMP);
      ObjectEvent_SetMode(leader, ANIMATION_LEADER_LEAP);
      leader->speed = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      leader->motionFlags &= 126;
      Engine_ObjectEventSetSpriteFlags(leader, 0);
      Engine_ObjectMotionSetPositionAndCommit(
          LOCALID_PARTY_LEADER, landing->x >> 16, landing->z >> 16);
      ObjectEvent_SetMode(leader, ANIMATION_LEADER_SPRING);
      Engine_ObjectEventSetSpriteFlags(leader, 1);
      leader->motionFlags = savedMotionFlags;
      Engine_ScriptEnd();
    }
  }
}

/// Walks the leader through a doorway and leaves by its warp; through the
/// Sanctum's door the leader walks straight in, without centering on it.
static void Xian_WalkThroughExit(s32 warpNumber) {
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
  if (warpNumber == XIAN_TO_SANCTUM) {
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
    ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -16);
  } else {
    ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -16);
  }
  gFieldWork->transitionFrames = 16;
  Engine_ScriptRequestWarp(warpNumber);
}

/// Walking north into one of Xian's doors: it opens and the party goes in. Each
/// door's metatile behavior is the number of the warp it leads by: five lead
/// into the houses, shops and inn, XIAN_TO_SANCTUM into Xian Sanctum (SE_188)
/// and XIAN_TO_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL into Master Feh's school.
static void Xian_OpenDoorAndLeave(void) {
  struct EventWork *work = gFieldWork;

  Engine_ScriptStart();

  switch (work->coordEvent) {
  case XIAN_TO_HOUSE:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gXian_HouseDoorOpens, 81, 18);
    break;
  case XIAN_TO_HOUSE_NORTH:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gXian_NorthHouseDoorOpens, 83, 11);
    break;
  case XIAN_TO_XIAN_INTERIORS_EAST:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gXian_NorthHouseDoorOpens, 86, 11);
    break;
  case XIAN_TO_INN:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gXian_InnDoorOpens, 84, 24);
    break;
  case XIAN_TO_WEAPON_AND_ARMOR_SHOP:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gXian_InnDoorOpens, 72, 7);
    break;
  case XIAN_TO_SANCTUM:
    Engine_AudioPlayCue(SE_188);
    Engine_MapGridAnimateCells(gXian_SanctumDoorOpens, 69, 11);
    break;
  case XIAN_TO_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL:
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapGridAnimateCells(gXian_SchoolDoorOpens, 83, 7);
    break;
  default:
    break;
  }

  Xian_WalkThroughExit(work->coordEvent);
  Engine_ScriptEnd();
}

/// Force on the training tree in Xian: it jumps, and the student cries out.
/// "Wha-Wha-What happened? The tree jumped on its own!", the next time
/// "...It jumped again... I am too tired...".
static void Xian_StrikeTrainingTreeWithForce(void) {
  TaskFunc rotationCallback;

  Engine_ScriptStart();
  ObjectEvent_SetActionScript(LOCALID_TRAINING_TREE, gXian_TrainingTreeJump);
  rotationCallback = Xian_RotateTrainingTree;
  FieldScheduler_AddCallback(rotationCallback, TASK_PRIORITY_MAP);
  ObjectEvent_RefreshSelectorById(LOCALID_TRAINING_TREE);
  Engine_AudioPlayCue(SE_124);
  FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), 0x3333, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  FieldEffect_Spawn(PIXELS(168), PIXELS(8), PIXELS(312), -0x3333, 0, 0,
                    EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
  Engine_TaskRemoveCallback(rotationCallback);
  ObjectEvent_GetById(LOCALID_TRAINING_TREE)->sprite->rotation = 0x8000;
  Engine_ObjectEventSetPosition(LOCALID_FORCE_TARGET, PIXELS(168), PIXELS(312));
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_KUNG_FU_STUDENT,
                                    LOCALID_TRAINING_TREE, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_KUNG_FU_STUDENT, 2);
  Engine_ScriptWait(10);
  Engine_ScriptSetMessage(MsgXianWhaWhaHappened);
  if (GameFlag_IsSet(FLAG_MAP_TEMP_03)) {
    gFieldWork->message++;
  }
  Engine_ScriptShowMessage(LOCALID_KUNG_FU_STUDENT, 0);
  Engine_GameFlagSet(FLAG_MAP_TEMP_03);
  Map_PlayQueuedMusic();
  Engine_ScriptEnd();
}

/// Raised in Xian when the leader has stood in the water carrier's way for two
/// seconds: both walk to the start of her route, the leader turns west, and she
/// sets off again.
static void Xian_MoveAsideForWaterCarrier(void) {
  struct ObjectEvent *carrier;

  Engine_ScriptStart();
  ObjectEvent_SetBuiltinAction(LOCALID_WOMAN_CARRYING_WATER,
                               OBJ_EVENT_ACTION_IDLE);
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->update = 0;
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->targetX = OBJ_EVENT_NO_TARGET;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrier->targetZ = OBJ_EVENT_NO_TARGET;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrier->velocityX = 0;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrier->velocityZ = 0;
  carrier = ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER);
  carrier->speed = 0;
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->acceleration = 0;
  ObjectEvent_ShowEmote(LOCALID_WOMAN_CARRYING_WATER,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WOMAN_CARRYING_WATER, 2);
  Engine_ScriptWait(60);
  ObjectEvent_SetSpeed(LOCALID_WOMAN_CARRYING_WATER, 0x18000, 0xc000);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x18000, 0xc000);
  ObjectEvent_WalkTo(LOCALID_WOMAN_CARRYING_WATER, 280, 232);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 296, 232);
  Engine_ObjectEventWaitForMove(LOCALID_WOMAN_CARRYING_WATER);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_SetActionScript(LOCALID_WOMAN_CARRYING_WATER,
                              gXian_WaterCarrierRoute);
  carrier = ObjectEvent_Get(LOCALID_WOMAN_CARRYING_WATER);
  carrier->update = XIAN_WATER_CARRIER_UPDATE;
  Engine_ScriptEnd();
}

const struct MapEvent *Xian_GetEvents(void) { return gXian_Events; }

/// Queues and plays SE_169 and sets up the village as the story and this visit
/// left it: after the news of Hsu (or, back from a battle, after her strike)
/// the kung fu student stands where her strike leaves her; Feizhi is gone
/// after the news or once she has seen the rockslide; back from a battle, the
/// frozen puddle keeps its ice; the water carrier is hooked to
/// Xian_WatchWaterCarrierPath; and the training tree, its Force mark and the
/// three crates are drawn under the object events.
s32 Xian_Initialize(void) {
  struct ObjectEvent *objectEvent;
  u32 crateIndex;

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Map_QueueMusic(SE_169);
  // Back from the school (XIAN_FROM_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL) or the
  // Sanctum (XIAN_FROM_SANCTUM_WEST), the last two warp destinations, the name
  // is not shown.
  if (gGameState.warpDest >= XIAN_FROM_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    ObjectEvent_FaceDirection(LOCALID_BOY_ON_SILK_CLOTHES, FACING_WEST, 0);
    ObjectEvent_SetPosition(LOCALID_KUNG_FU_STUDENT, PIXELS(146), PIXELS(312));
    Engine_ObjectEventFaceDirection(LOCALID_KUNG_FU_STUDENT, FACING_EAST, 0);
    if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
      Engine_ObjectEventSetPosition(LOCALID_FEIZHI, 0, 0);
    }
  }
  // Gone once she has found the road blocked (Alpine Crossing's
  // FLAG_FEIZHI_SAW_ROCKSLIDE).
  if (Engine_GameFlagIsSet(FLAG_FEIZHI_SAW_ROCKSLIDE)) {
    Engine_ObjectEventSetPosition(LOCALID_FEIZHI, 0, 0);
  }
  // Set the three crates up as ObjectEvent_CreateConfiguredObject sets up
  // its objects: still, not solid, never a target and hidden.
  for (crateIndex = 0; crateIndex <= 2; crateIndex++) {
    struct ObjectEvent *objectEvent;
    objectEvent = ObjectEvent_GetById(crateIndex + LOCALID_FIRST_CRATE);
    objectEvent->sprite->image.priority = 1;
    objectEvent->motionFlags = 0;
    objectEvent->collisionFlags = OBJ_EVENT_NOT_TARGETED;
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    ObjectGroup_SetChildValue(objectEvent, SPRITE_COLOR_HIDDEN);
    objectEvent->priorityFlags =
        (objectEvent->priorityFlags & ~OBJ_EVENT_PRIORITY_AUTOMATIC) |
        OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_02)) {
    ObjectEvent_SetPosition(LOCALID_KUNG_FU_STUDENT, PIXELS(146), PIXELS(312));
    Engine_ObjectEventFaceDirection(LOCALID_KUNG_FU_STUDENT, FACING_EAST, 0);
  }
  // Restore the frozen puddle and callbacks when re-entering the village.
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01)) {
    Engine_ObjectEventSetAnimation(LOCALID_SPILLED_WATER,
                                   PUDDLE_ANIMATION_FROZEN);
    {
      s32 puddleX = ObjectEvent_GetById(LOCALID_SPILLED_WATER)->x.fixed;

      Engine_MapGridCopyCellAttributes(
          3, 17, 1, 1, puddleX >> 20,
          ObjectEvent_GetById(LOCALID_SPILLED_WATER)->z.fixed >> 20);
    }
    FieldScheduler_AddCallback(Xian_UpdateFrozenPuddleLayer, TASK_PRIORITY_MAP);
  }
  // The water carrier's sprite is drawn with its green and blue swapped.
  Engine_ObjectEventSetChildValue(LOCALID_WOMAN_CARRYING_WATER,
                                  SPRITE_COLOR_RBG);
  ObjectEvent_GetById(LOCALID_WOMAN_CARRYING_WATER)->update =
      XIAN_WATER_CARRIER_UPDATE;
  objectEvent = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  objectEvent->motionFlags = 0;
  objectEvent->y.fixed = PIXELS(16);
  objectEvent->targetY = PIXELS(16);
  objectEvent->scaleX = 0x8ccc;
  objectEvent->scaleY = 0x6666;
  objectEvent->sprite->rotation = 0x8000;
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_FORCE_TARGET),
                                   0);
  ObjectEvent_GetById(LOCALID_FORCE_TARGET)->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_FORCE_TARGET)->y.fixed = 0;
  ObjectEvent_GetById(LOCALID_FORCE_TARGET)->targetY = OBJ_EVENT_NO_TARGET;
  return 0;
}

s32 gShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gMediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gLongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
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
    SC_WAIT(18), SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1), SC_FREE,
};

/// Each particle beside the leaping student, facing west: 16 frames.
s32 gXian_TrainingParticleScript[] = {
    SC_SET(SW_FACING, FACING_WEST), SC_WAIT(16), SC_WAIT(1),
    SC_POSITION(0, 0, 0),           SC_WAIT(1),  SC_FREE,
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

struct MapWarpDest gXian_WarpDests[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_HOUSE, CONDITION_ALWAYS, 376, 0, 504, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {XIAN_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 344, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 376, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {XIAN_FROM_XIAN_INTERIORS_EAST, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {XIAN_FROM_XIAN_INTERIORS_SOUTH, CONDITION_ALWAYS, 408, 0, 424,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_XIAN_INTERIORS_WEST, CONDITION_ALWAYS, 216, 0, 184, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {XIAN_FROM_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL, CONDITION_ALWAYS, 392, 0,
     184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {XIAN_FROM_SANCTUM_WEST, CONDITION_ALWAYS, 176, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gXian_Warps[] = {
    MAP_WARPS(Xian),
    MAP_WARP(XIAN_TO_HOUSE, XianInteriors, XIAN_INTERIORS_HOUSE),
    MAP_WARP(XIAN_TO_HOUSE_NORTH, XianInteriors, XIAN_INTERIORS_HOUSE_NORTH),
    MAP_WARP(XIAN_TO_XIAN_INTERIORS_EAST, XianInteriors,
             XIAN_INTERIORS_FROM_XIAN_EAST),
    MAP_WARP(XIAN_TO_INN, XianInteriors, XIAN_INTERIORS_INN_SOUTH),
    MAP_WARP(XIAN_TO_WEAPON_AND_ARMOR_SHOP, XianInteriors,
             XIAN_INTERIORS_WEAPON_AND_ARMOR_SHOP),
    MAP_WARP(XIAN_TO_SANCTUM, Sanctum, SANCTUM_FROM_XIAN),
    MAP_WARP(XIAN_TO_FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL, FuchinTempleInteriors,
             FUCHIN_TEMPLE_INTERIORS_FEHS_SCHOOL_FROM_XIAN),
    MAP_WARP(XIAN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_XIAN),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gXian_ObjectEventTemplates[] = {
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
        PIXELS(299), 0, PIXELS(222), FACING_WEST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(280), FACING_WEST, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct MapEvent gXian_Events[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_OpenDoorAndLeave) },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, XIAN_TO_WORLD_MAP },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 20, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 21, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 2, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 3, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Xian_JumpOverSpilledWater) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_WELCOMING_TRAVELERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsXianTravelersAreWelcome) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_AskAboutYoungWarriors) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianXianIsTheVillageOfKinu) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMartialArtsAreVeryPopularIn) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouWarriorsShouldVisitMasterFehs) },
    { MAP_EVENT_TALK, LOCALID_BOY_ON_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMyClothesAreNotSilkSilk) },
    { MAP_EVENT_TALK, LOCALID_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_TalkToKungFuStudent) },
    { MAP_EVENT_TALK, LOCALID_MUSCULAR_MAN_ON_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianDisastersFromTheSkyManyMany) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_IN_THE_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsTheMulberryOrchardWhen) },
    { MAP_EVENT_TALK, LOCALID_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(Xian_TalkToFeizhi) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_CARRYING_WATER, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_TalkToWaterCarrier) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_WELCOMING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgXianXianIsAPeacefulVillageBut) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianThereWasARockslideOnSilk) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_KINU, CONDITION_ALWAYS, MSGNO(MsgXianIWentToHelpMoveBoulders) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleWhoClearedTheRockslide) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsVeryGoodAtKung) },
    { MAP_EVENT_TALK, LOCALID_BOY_ON_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleOfXianLoveTheir) },
    { MAP_EVENT_TALK, LOCALID_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianAltinWasFloodedWithWaterBut) },
    { MAP_EVENT_TALK, LOCALID_MUSCULAR_MAN_ON_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianWeClearedTheRocksFromSilk) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_IN_THE_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianManyStrangeThingsHappenedAllAt) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_SPILLED_WATER, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Xian_UpdatePuddlePriority) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SPILLED_WATER, FLAG_MAP_TEMP_01, EVENT_SCRIPT(Xian_FreezeSpilledWater) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_FORCE_TARGET, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_StrikeTrainingTreeWithForce) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_WELCOMING_TRAVELERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheseChildrenMustHaveComeTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouYoungWarriorsLookStrongBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianIHopeManyPeopleComeTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianFightingWithKicksAndPunchesIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehsSchoolIsTrainingIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ON_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianSilkIsShinyAndSmoothIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianBreakingTreesWithMyHandHurts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MUSCULAR_MAN_ON_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehSaidYahAndThen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_IN_THE_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheOrchardGivesTheVillageLife) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianGoingWestAloneIsVeryDangerous) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_CARRYING_WATER, CONDITION_ALWAYS, MSGNO(MsgXianIMustCarryWaterEveryDay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_WELCOMING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgXianHsuWasCaughtUnderABoulder) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianTheRocksThereWereVeryHard) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_KINU, CONDITION_ALWAYS, MSGNO(MsgXianSomePeopleDidNotCarryRocks) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianSandIsTheColorOfThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsStrongHeadedHeWould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ON_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianEveryoneStaysInXianTheyDo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianThisWarriorSavedAltinVillageNo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MUSCULAR_MAN_ON_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianItIsBadForXianIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_IN_THE_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianIWasHappyToSeeGems) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_XIAN_LUCKY_MEDAL_JAR, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENT_TALK, LOCALID_MIST, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_MistJoins) },
    { MAP_EVENT_RAISED, XIAN_TRIGGER_CARRIER_BLOCKED, CONDITION_ALWAYS, EVENT_SCRIPT(Xian_MoveAsideForWaterCarrier) },
    { MAP_EVENTS_END },
};
// clang-format on
