/// @file
/// Outside the Venus Lighthouse, where the prologue hands the story to Jenna.
/// Felix has left the group, and Jenna and Kraden come out of the lighthouse
/// alone:
///
/// - Two birds land below the door (VenusLighthouseEntrance_BirdsLand) and fly
///   off as the party walks past (VenusLighthouseEntrance_BirdsFlyOff).
/// - Tolbi's soldiers and the workmen of Lalivero close in. Alex arrives, sends
///   Jenna and Kraden west to Idejima, hurls two soldiers skyward with Psynergy
///   and drives the rest off (VenusLighthouseEntrance_SoldiersAmbushParty).
/// - On the road west a Ruffian left to wait for backup fights the two
///   (VenusLighthouseEntrance_RuffianWaitsForBackup). A lost battle in the
///   prologue wakes them by the door, with Alex come back for them
///   (VenusLighthouseEntrance_WakeUp).

#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "gamepak/far_call/system.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "module/game/game.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "call.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "field_sprite.h"
#include "text/msg_ids.h"

/// Set as Tolbi's soldiers ambush the party; the soldiers and workmen are gone
/// from then on.
#define FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN 0x890
/// Set as the birds land the first time the party comes out of the lighthouse.
#define FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED 0x891
/// Set as the birds fly off; they are gone from then on.
#define FLAG_VENUS_LIGHTHOUSE_BIRDS_FLOWN 0x892
/// Set as the party comes back from beating the Ruffian who waited for backup;
/// he then lies beaten by the road.
#define FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN 0x893

/// The ways into the scene that its set-up answers.
enum VenusLighthouseEntranceEntrance {
  /// Out of the lighthouse's door; the birds land the first time.
  ENTRANCE_FROM_LIGHTHOUSE = 9,
  /// Where the prologue hands the party to Jenna: Felix, Sheba and Piers leave
  /// it.
  ENTRANCE_JENNA_TAKES_OVER = 90,
  /// After a lost battle in the prologue: Jenna wakes by the door.
  ENTRANCE_DEFEAT = 98,
  /// Back from the Ruffian's battle.
  ENTRANCE_AFTER_BATTLE = 99
};

/// The scene's placed actors, in the rows of gActorPlacements.
enum VenusLighthouseEntranceActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  ACTOR_ALEX,
  /// The two birds by the door.
  // XXX: birds, read from their flight and landing; a render of entrance 9
  // would settle it.
  ACTOR_WEST_BIRD,
  ACTOR_EAST_BIRD,
  /// Tolbi's soldiers: the two Alex hurls skyward, the two who flee before
  /// him and the one sent to Lalivero for backup.
  ACTOR_SHOUTING_SOLDIER,
  ACTOR_SURPRISED_SOLDIER,
  ACTOR_DOUBTING_SOLDIER,
  ACTOR_SOLDIER_CAPTAIN,
  ACTOR_SOLDIER_SENT_FOR_BACKUP,
  /// The workmen of Lalivero who come looking for Sheba: four who run from
  /// Alex, and the one their boss sends to the caves.
  ACTOR_SCOLDING_WORKMAN,
  ACTOR_INDIGNANT_WORKMAN,
  ACTOR_EAGER_WORKMAN,
  ACTOR_WORKMAN_BOSS,
  ACTOR_WORKMAN_SENT_TO_CAVES,
  /// The Ruffian left on the road west to wait for backup.
  ACTOR_WAITING_RUFFIAN
};

struct Actor;
struct GlowSprite;
struct GlowState;

void VenusLighthouseEntrance_EmitDust(struct Actor *object, s32 x, s32 y,
                                      s32 z);
void VenusLighthouseEntrance_StartDust(void);
void VenusLighthouseEntrance_StopDust(void);
extern u32 gVenusLighthouseEntrance_BirdLands[];

void EffectSprite_DrawAtWorldPosition(struct GlowSprite *sprite,
                                      struct GlowState *state);

void VenusLighthouseEntrance_UpdateBlastColumn(void);
void VenusLighthouseEntrance_UpdateDust(void);

void VenusLighthouseEntrance_BirdsFlyOff(void);

void VenusLighthouseEntrance_SoldiersAmbushParty(void);

void VenusLighthouseEntrance_RuffianWaitsForBackup(void);

void VenusLighthouseEntrance_PointToMeetingSpot(void);

/// The dust puffs' work, kept in Ram_HeapSlots' menu runtime while they play.
struct DustPuffState {
  u8 unk00[24];
  s32 frame;
};

struct DustPuffSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[30];
};

struct DustPuffWork {
  u8 unk0000[0x800];
  struct DustPuffState states[96];
  struct DustPuffSprite sprites[96];
  u16 unk2180;
  u16 unk2182;
};

s32 Actor_FallOverWithAnimationById();
void VenusLighthouseEntrance_InitGlowSprite(struct GlowSprite *sprite,
                                            s32 width, s32 height, s32 attr,
                                            s32 tile);
void Actor_SelectAnimation(struct Actor *actor, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

void VenusLighthouseEntrance_BlastActorWithPsynergy(void *object) {
  Field_StartColorTransition(8, 0x4084);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_JENNA), 2);
  FieldEvent_Wait(20);
  VenusLighthouseEntrance_BlastActorSkyward(object);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_JENNA), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
}

void VenusLighthouseEntrance_TrailDust(struct Actor *object) {
  VenusLighthouseEntrance_EmitDust(object, object->x, object->y, object->z);
  if (++object->timer % 8 == 0)
    Event_PlaySound(SE_CHEST_CLOSE);
}

/// Talked to, a soldier runs 96 pixels out and back trailing dust.
void VenusLighthouseEntrance_RunWithDustTrail(s32 actor) {
  struct Actor *object = Actor_Get(actor);

  VenusLighthouseEntrance_StartDust();
  object->update = VenusLighthouseEntrance_TrailDust;
  object->priorityFlags = 0;
  {
    s32 a = actor;
    /* FAKEMATCH(pin): the 96 pinned to r1. */
    register s32 dx asm("r1") = 96;
    /* FAKEMATCH(pin): the -96 pinned to r2, the same call's second. */
    register s32 dz asm("r2") = -96;

    Actor_WalkByAndWaitById(a, dx, dz);
  }
  {
    /* FAKEMATCH(pin): the arguments pinned in order. */
    register s32 a asm("r0") = actor;
    s32 dx = -96;

    Actor_WalkByAndWaitById(a, dx, 96);
  }
  object->update = 0;
  object->priorityFlags = 1;
  FieldEvent_Wait(30);
  VenusLighthouseEntrance_StopDust();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void VenusLighthouseEntrance_BirdsLand(void);
void VenusLighthouseEntrance_WakeUp(void);

s32 Scene_Initialize(void) {
  struct Actor *ruffian;
  struct Actor *leader;

  gFieldWork->transition = 256;
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  leader->priorityFlags |= 0x20;
  Actor_SetAnimation(ACTOR_WEST_BIRD, 5);
  FieldActor_SetSpritePriority(ACTOR_WEST_BIRD, 1);
  Actor_SetAnimation(ACTOR_EAST_BIRD, 5);
  FieldActor_SetSpritePriority(ACTOR_EAST_BIRD, 1);
  FieldActor_SetSpritePriority(ACTOR_DOUBTING_SOLDIER, 2);
  FieldActor_SetSpritePriority(ACTOR_EAGER_WORKMAN, 2);
  if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_FLOWN)) {
    Actor_SetPosition(ACTOR_WEST_BIRD, 0, 0);
    Actor_SetPosition(ACTOR_EAST_BIRD, 0, 0);
  }
  if (Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN)) {
    Actor_SetPosition(ACTOR_SHOUTING_SOLDIER, 0, 0);
    Actor_SetPosition(ACTOR_SURPRISED_SOLDIER, 0, 0);
    Actor_SetPosition(ACTOR_DOUBTING_SOLDIER, 0, 0);
    Actor_SetPosition(ACTOR_SOLDIER_CAPTAIN, 0, 0);
    Actor_SetPosition(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 0);
    Actor_SetPosition(ACTOR_SCOLDING_WORKMAN, 0, 0);
    Actor_SetPosition(ACTOR_INDIGNANT_WORKMAN, 0, 0);
    Actor_SetPosition(ACTOR_EAGER_WORKMAN, 0, 0);
    Actor_SetPosition(ACTOR_WORKMAN_BOSS, 0, 0);
    Actor_SetPosition(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 0);
  }
  if (gMainRoutineWork.entrance == ENTRANCE_FROM_LIGHTHOUSE) {
    if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED)) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      VenusLighthouseEntrance_BirdsLand();
      Event_End();
    }
  }
  if (gMainRoutineWork.entrance == ENTRANCE_DEFEAT) {
    gMainRoutineWork.returnEntrance = 1;
    gMainRoutineWork.entrance = 1;
    Party_RestoreHpPp(1);
    VenusLighthouseEntrance_WakeUp();
  }
  if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
    Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN);
  }
  if (gMainRoutineWork.entrance == ENTRANCE_JENNA_TAKES_OVER) {
    gMainRoutineWork.leaderActorId = ACTOR_JENNA;
    Party_RemoveMember(ACTOR_FELIX);
    Party_RemoveMember(ACTOR_SHEBA);
    Party_RemoveMember(ACTOR_PIERS);
    Event_SetDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE, 1);
  }
  if (!Event_GetFlag(FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN)) {
    Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, 5);
    FieldActor_SetSpritePriority(ACTOR_WAITING_RUFFIAN, 1);
  } else {
    Actor_SetPosition(ACTOR_WAITING_RUFFIAN, 40, 248);
    ruffian = Actor_Get(ACTOR_WAITING_RUFFIAN);
    ruffian->facing = FACING_SOUTH;
    Actor_FallOverWithAnimationById(ACTOR_WAITING_RUFFIAN, FACING_SOUTH, 0);
    FieldActor_SetSpritePriority(ACTOR_WAITING_RUFFIAN, 3);
    Actor_SetActionCallback(Actor_Get(ACTOR_WAITING_RUFFIAN), 8);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 Actor_StartScript();
void FieldCamera_WaitForArrival(void);

/// The two birds fly down and land below the door as the party first comes out
/// of the lighthouse.
void VenusLighthouseEntrance_BirdsLand(void) {
  struct Actor *westBird;
  struct Actor *eastBird;

  Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_LANDED);
  westBird = Actor_Get(ACTOR_WEST_BIRD);
  westBird->motionFlags = 0;
  westBird->gravity = px(1) / 100;
  westBird->speedLimit = px(3) / 2;
  westBird->acceleration = px(1) / 2;
  FieldActor_SetPositionAndResetMotion(westBird, px(216), px(96), px(176));
  eastBird = Actor_Get(ACTOR_EAST_BIRD);
  eastBird->motionFlags = 0;
  eastBird->gravity = px(3) / 100;
  eastBird->speedLimit = px(3) / 2;
  eastBird->acceleration = px(1) / 2;
  FieldActor_SetPositionAndResetMotion(eastBird, px(616), px(96), px(152));
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 248, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  westBird = Actor_Get(ACTOR_WEST_BIRD);
  Actor_SetAnimation(ACTOR_WEST_BIRD, 4);
  Event_PlaySound(SE_BIRD_WINGS);
  FieldActor_SetMoveTarget(westBird, 352, 8, 176);
  Actor_StartScript(westBird, (s32)gVenusLighthouseEntrance_BirdLands);
  FieldEvent_Wait(25);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 104, 1);
  eastBird = Actor_Get(ACTOR_EAST_BIRD);
  Actor_SetAnimation(ACTOR_EAST_BIRD, 4);
  FieldActor_SetMoveTarget(eastBird, 416, 8, 152);
  Actor_StartScript(eastBird, (s32)gVenusLighthouseEntrance_BirdLands);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_BIRD_WINGS);
  FieldActor_WaitForMoveById(ACTOR_WEST_BIRD);
  FieldActor_WaitForMoveById(ACTOR_EAST_BIRD);
  FieldCamera_WaitForArrival();
}

extern u32 gVenusLighthouseEntrance_BirdFlies[];

/// The birds take off as the party walks past them.
void VenusLighthouseEntrance_BirdsFlyOff(void) {
  struct Actor *westBird;
  struct Actor *eastBird;

  Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_BIRDS_FLOWN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_BIRD_WINGS);
  Actor_SetAnimation(ACTOR_WEST_BIRD, 3);
  FieldActor_SetAnimation(ACTOR_EAST_BIRD, 3);
  FieldEvent_Wait(21);
  westBird = Actor_Get(ACTOR_WEST_BIRD);
  westBird->motionFlags = 0;
  westBird->speedLimit = px(2);
  westBird->acceleration = px(1) / 2;
  Actor_SetAnimation(ACTOR_WEST_BIRD, 4);
  FieldActor_SetMoveTarget(westBird, 616, 96, 176);
  Actor_StartScript(westBird, (s32)gVenusLighthouseEntrance_BirdFlies);
  eastBird = Actor_Get(ACTOR_EAST_BIRD);
  eastBird->motionFlags = 0;
  eastBird->speedLimit = px(2);
  eastBird->acceleration = px(1) / 2;
  Actor_SetAnimation(ACTOR_EAST_BIRD, 4);
  FieldActor_SetMoveTarget(eastBird, 120, 96, 176);
  Call2(Actor_StartScript, (s32)eastBird,
        (s32)gVenusLighthouseEntrance_BirdFlies);
  Event_End();
}

/// Tolbi's soldiers and the workmen of Lalivero ambush Jenna and Kraden. Alex
/// arrives, tells them to meet him at Idejima, hurls two soldiers skyward and
/// sends the workmen running, then follows the last soldiers to Lalivero.
void VenusLighthouseEntrance_SoldiersAmbushParty(void) {
  struct Actor *workman;
  struct Actor *actor;
  s32 zero;
  void (*trailDust)(struct Actor *);

  Event_SetFlag(FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgThereTheyAre);
  Actor_SetSpeed(ACTOR_SHOUTING_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_SURPRISED_SOLDIER, 1);
  // "There they are! Get 'em!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 376, 280);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  Actor_WalkTo(ACTOR_JENNA, 376, 360);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SHOUTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Iodem sent us to stand guard here, but..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SURPRISED_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "I never thought we'd actually run into them!"
  FieldEvent_ShowMessageThenWait(ACTOR_SURPRISED_SOLDIER, 0, 10);
  Actor_ShowEmote(ACTOR_SCOLDING_WORKMAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Scoundrels! What have you done with Sheba?"
  FieldEvent_ShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
  FieldActor_Jump(ACTOR_INDIGNANT_WORKMAN, 6, 15);
  FieldActor_Jump(ACTOR_INDIGNANT_WORKMAN, 6, 25);
  // "Return her now, or you're really going to regret it!"
  FieldEvent_ShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "Um... Aren't there supposed to be more of them?"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
  FieldEvent_Wait(10);
  // "Huh... Maybe they're just the first ones to come out..."
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Great! That means we've got 'em outnumbered!"
  FieldEvent_ShowMessageThenWait(ACTOR_EAGER_WORKMAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WORKMAN_BOSS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We can probably take 'em, you think?"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAGER_WORKMAN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SOLDIER_SENT_FOR_BACKUP, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Go fetch the other Tolbi soldiers still stationed in Lalivero..."
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Yes, sir!"
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 10);
  Actor_WalkByAndWait(ACTOR_SOLDIER_SENT_FOR_BACKUP, 64, 0);
  Actor_SetPosition(ACTOR_SOLDIER_SENT_FOR_BACKUP, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WORKMAN_SENT_TO_CAVES, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "And we'll go tell the guys in the caves!"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WORKMAN_SENT_TO_CAVES, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Got it, Boss!"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 10);
  Actor_WalkByAndWait(ACTOR_WORKMAN_SENT_TO_CAVES, -64, 64);
  Actor_SetPosition(ACTOR_WORKMAN_SENT_TO_CAVES, 0, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 360, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_ALEX, 16, -16, FACING_SOUTH);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALEX, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "How unfortunate... It looks like an ambush. What should we do, Jenna?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "What do you mean? Look around... We'll have to fight them!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Are you serious?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 0);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Jenna, are you really prepared to fight these men?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We don't have any other choice. We have to..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "Well, then. Allow me to shoulder some of your burden."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.5);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "...Burden?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "We regroup along the road leading away from Lalivero. Do you
  // understand?" Yes: "You don't need to remind us, Alex." No: "It's quite
  // simple, Jenna."
  ((void (*)())FieldMessage_ShowAtActor)(ACTOR_ALEX, 0);
  FieldEvent_Wait(10);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_JENNA, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
    FieldEvent_Wait(20);
    // "You don't need to remind us, Alex. We know the place."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    // "It's quite simple, Jenna. The meeting place is in the direction..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  }
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Follow this road west, and take the path down through the canyon..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "On the other side of that cave, you will find a small peninsula..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  // "Thankfully, only those workmen block your route."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  // "And fortunately..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  // "They don't look too tough..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "Hello! We can hear you!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
  Actor_ShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "That's just plain rude!"
  FieldEvent_ShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(25);
  // "Why don't the two of you escape and leave the Tolbi soldiers to me?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "The two of us? Alone?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 360, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You can use Psynergy, can't you, Jenna?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Um... Yes."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "Then you'll be fine."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We shall reconvene at the peninsula."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALEX, 0.5);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_EAST_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SHOUTING_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "You think you can take on Tolbi's finest on your own, little man?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHOUTING_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "I do not wish to inflict unnecessary bloodshed."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SURPRISED_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "Don't wish to... What's that supposed to mean?"
  FieldEvent_ShowMessageThenWait(ACTOR_SURPRISED_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST_NORTHWEST, 0);
  FieldEvent_Wait(25);
  // "If you lay a hand on these two, I assure you that you will be made..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SHOUTING_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SURPRISED_SOLDIER, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_NORTHWEST, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_SHOUTING_SOLDIER, 1.2);
  Actor_SetSpeed(ACTOR_SURPRISED_SOLDIER, 1.2);
  Actor_WalkByAndWait(ACTOR_SHOUTING_SOLDIER, -26, 8);
  Actor_FaceDirection(ACTOR_SHOUTING_SOLDIER, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkByAndWait(ACTOR_SURPRISED_SOLDIER, -16, -12);
  Actor_FaceDirection(ACTOR_SURPRISED_SOLDIER, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "By advancing, I assume you mean to fight. Permit me to strike the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ALEX, 8);
  FieldEvent_Wait(50);
  BattleEffect_StartArcEffect();
  actor = Actor_GetById(ACTOR_ALEX);
  BattleEffect_SetObjectUpdateMode(actor, 2);
  Event_PlaySound(SE_178);
  FieldEvent_Wait(20);
  Field_StartColorTransition(8, 0x4084);
  FieldEvent_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldActor_SetSpritePriority(ACTOR_SHOUTING_SOLDIER, 0);
  VenusLighthouseEntrance_BlastActorSkyward(ACTOR_SHOUTING_SOLDIER);
  Actor_ShowEmote(ACTOR_SCOLDING_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_EAGER_WORKMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_WORKMAN_BOSS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SURPRISED_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_DOUBTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_SOLDIER_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  50);
  FieldActor_SetSpritePriority(ACTOR_SURPRISED_SOLDIER, 1);
  VenusLighthouseEntrance_BlastActorSkyward(ACTOR_SURPRISED_SOLDIER);
  FieldEvent_Wait(30);
  actor = Actor_GetById(ACTOR_ALEX);
  BattleEffect_SetObjectUpdateMode(actor, 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(16);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ALEX, 9);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_DOUBTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What was that!?"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
  FieldEvent_Wait(10);
  // "I don't know, but whatever he did, it's out of our league!"
  FieldEvent_ShowMessageThenWait(ACTOR_SOLDIER_CAPTAIN, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 3);
  Actor_SetAttachedEffect(ACTOR_DOUBTING_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 3);
  Actor_SetAttachedEffect(ACTOR_SOLDIER_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_WEST, 0);
  FieldActor_Jump(ACTOR_DOUBTING_SOLDIER, 6, 15);
  FieldActor_Jump(ACTOR_DOUBTING_SOLDIER, 6, 25);
  // "You guys deal with him until backup arrives!"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Oh, so you're next?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkByAndWait(ACTOR_ALEX, -16, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
  FieldCamera_MoveTo(360, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldActor_StartRepeatedMotion(ACTOR_SCOLDING_WORKMAN, 3);
  Actor_SetAttachedEffect(ACTOR_SCOLDING_WORKMAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_INDIGNANT_WORKMAN, 3);
  Actor_SetAttachedEffect(ACTOR_INDIGNANT_WORKMAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_EAGER_WORKMAN, 3);
  Actor_SetAttachedEffect(ACTOR_EAGER_WORKMAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_WORKMAN_BOSS, 3);
  Actor_SetAttachedEffect(ACTOR_WORKMAN_BOSS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_WORKMAN_SENT_TO_CAVES, 3);
  Actor_SetAttachedEffect(ACTOR_WORKMAN_SENT_TO_CAVES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_SCOLDING_WORKMAN, ANIM_SHAKE_HEAD);
  // "Hey, no need to worry about us!"
  FieldEvent_ShowMessageThenWait(ACTOR_SCOLDING_WORKMAN, 0, 10);
  Actor_ShowEmote(ACTOR_INDIGNANT_WORKMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "We're not soldiers... We're not anybody!"
  FieldEvent_ShowMessageThenWait(ACTOR_INDIGNANT_WORKMAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAGER_WORKMAN, ANIM_SHAKE_HEAD);
  // "Seriously, there's no way we're gonna face off against you."
  FieldEvent_ShowMessageThenWait(ACTOR_EAGER_WORKMAN, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_WORKMAN_BOSS, 3);
  Actor_SetAttachedEffect(ACTOR_WORKMAN_BOSS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(55);
  Actor_FaceDirection(ACTOR_SCOLDING_WORKMAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_INDIGNANT_WORKMAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_EAGER_WORKMAN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_WORKMAN_BOSS, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_WORKMAN_BOSS, 2);
  FieldEvent_Wait(10);
  // "Let's get outta here!"
  FieldEvent_ShowMessageThenWait(ACTOR_WORKMAN_BOSS, 0, 10);
  FieldActor_Jump(ACTOR_INDIGNANT_WORKMAN, 6, 0);
  FieldActor_Jump(ACTOR_EAGER_WORKMAN, 6, 0);
  FieldActor_Jump(ACTOR_WORKMAN_BOSS, 6, 25);
  VenusLighthouseEntrance_StartDust();
  Actor_SetSpeed(ACTOR_WORKMAN_BOSS, 1.8);
  Actor_SetSpeed(ACTOR_EAGER_WORKMAN, 1.8);
  Actor_SetSpeed(ACTOR_INDIGNANT_WORKMAN, 1.8);
  Actor_SetSpeed(ACTOR_SCOLDING_WORKMAN, 1.8);
  zero = 0;
  workman = Actor_GetById(ACTOR_WORKMAN_BOSS);
  trailDust = VenusLighthouseEntrance_TrailDust;
  workman->timer = zero;
  {
    s32 next = ACTOR_EAGER_WORKMAN;

    workman->update = trailDust;
    workman = Actor_GetById(next);
  }
  workman->timer = zero;
  {
    s32 next = ACTOR_INDIGNANT_WORKMAN;

    workman->update = trailDust;
    workman = Actor_GetById(next);
  }
  workman->timer = zero;
  {
    s32 next = ACTOR_WORKMAN_BOSS;

    workman->update = trailDust;
    Actor_SetAnimation(next, 10);
  }
  Actor_SetAnimation(ACTOR_EAGER_WORKMAN, 10);
  Actor_SetAnimation(ACTOR_INDIGNANT_WORKMAN, 10);
  Actor_SetDestinationOffset(ACTOR_WORKMAN_BOSS, -24, 24);
  Actor_SetDestinationOffset(ACTOR_EAGER_WORKMAN, -24, 24);
  Actor_SetDestinationOffset(ACTOR_INDIGNANT_WORKMAN, -24, 24);
  FieldActor_WaitForMoveById(ACTOR_INDIGNANT_WORKMAN);
  Actor_SetDestination(ACTOR_WORKMAN_BOSS, 224, 480);
  Actor_SetDestination(ACTOR_EAGER_WORKMAN, 224, 480);
  Actor_MoveToAndWait(ACTOR_INDIGNANT_WORKMAN, 224, 480);
  workman = Actor_GetById(ACTOR_WORKMAN_BOSS);
  workman->update = zero;
  workman = Actor_GetById(ACTOR_EAGER_WORKMAN);
  workman->update = zero;
  workman = Actor_GetById(ACTOR_INDIGNANT_WORKMAN);
  workman->update = zero;
  FieldEvent_Wait(1);
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 6, 15);
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 6, 25);
  workman = Actor_GetById(ACTOR_SCOLDING_WORKMAN);
  workman->timer = zero;
  {
    s32 next = ACTOR_SCOLDING_WORKMAN;

    workman->update = trailDust;
    Actor_SetAnimation(next, 10);
  }
  Actor_SetDestinationOffset(ACTOR_SCOLDING_WORKMAN, -20, 20);
  FieldActor_WaitForMoveById(ACTOR_SCOLDING_WORKMAN);
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 4, 0);
  Actor_SetDestinationOffset(ACTOR_SCOLDING_WORKMAN, -20, 20);
  FieldActor_WaitForMoveById(ACTOR_SCOLDING_WORKMAN);
  Actor_SetAnimation(ACTOR_SCOLDING_WORKMAN, ANIM_STAND);
  Event_PlaySound(SE_IMPACT);
  workman->x += px(8);
  workman->update = zero;
  workman->z += -px(8);
  {
    s32 shown = FACING_NORTH;

    workman->facing = shown;
  }
  {
    struct FieldSprite *sprite = workman->sprite;
    s32 shown = TRIG_TURN * 5 / 8;

    sprite->rotation = shown;
  }
  FieldEvent_Wait(55);
  workman->x += -px(4);
  workman->z += px(4);
  {
    s32 shown = FACING_SOUTH;
    struct FieldSprite *sprite = workman->sprite;

    sprite->rotation = zero;
    workman->facing = shown;
  }
  FieldActor_Jump(ACTOR_SCOLDING_WORKMAN, 6, 40);
  Actor_SetAnimation(ACTOR_SCOLDING_WORKMAN, 11);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  Event_PlaySound(SE_CLICK);
  FieldEvent_Wait(14);
  FieldEvent_Wait(20);
  workman->timer = zero;
  {
    s32 next = ACTOR_SCOLDING_WORKMAN;

    workman->update = trailDust;
    Actor_SetAnimation(next, 10);
  }
  Actor_MoveToAndWait(ACTOR_SCOLDING_WORKMAN, 224, 472);
  FieldEvent_Wait(30);
  workman->update = zero;
  Actor_SetPosition(ACTOR_WORKMAN_BOSS, 0, 0);
  Actor_SetPosition(ACTOR_EAGER_WORKMAN, 0, 0);
  Actor_SetPosition(ACTOR_INDIGNANT_WORKMAN, 0, 0);
  Actor_SetPosition(ACTOR_SCOLDING_WORKMAN, 0, 0);
  VenusLighthouseEntrance_StopDust();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  // "How undignified... and how shameful."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
  {
    s32 a0 = ACTOR_JENNA;

    /* FAKEMATCH(dead): an empty once-only block before the angle's load. */
    do {
    } while (0);
    Actor_FaceDirection(a0, FACING_SOUTH, 0);
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(15);
  // "Jenna, now is your chance. Go to Idejima and wait for me."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SOLDIER_CAPTAIN, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 1.5);
  Actor_WalkByAndWait(ACTOR_DOUBTING_SOLDIER, -32, 0);
  actor = Actor_GetById(ACTOR_ALEX);
  {
    s32 shown = FACING_SOUTHEAST;

    actor->facing = shown;
  }
  // "I'm not finished with you yet."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 1.5);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
  FieldEvent_Wait(10);
  FieldActor_SetSpritePriority(ACTOR_DOUBTING_SOLDIER, 3);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 2);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_DOUBTING_SOLDIER)) + 90) &= 254;
  Actor_WalkToAndWait(ACTOR_DOUBTING_SOLDIER, 456, 424);
  FieldEvent_Wait(1);
  /* FAKEMATCH(dead): an empty once-only block ends CSE's block. */
  do {
  } while (0);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_DOUBTING_SOLDIER)) + 90) |= 1;
  Actor_FaceDirection(ACTOR_DOUBTING_SOLDIER, FACING_SOUTHWEST, 0);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 1);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You mentioned backup from Lalivero?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.7);
  Actor_WalkByAndWait(ACTOR_ALEX, 48, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_SOLDIER_CAPTAIN, 1);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_SOLDIER_CAPTAIN)) + 90) &= 254;
  Actor_WalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 456, 440);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_SOLDIER_CAPTAIN)) + 90) |= 1;
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Let's go meet them together, shall we?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
  FieldEvent_Wait(10);
  // "Hey, pal! When they get here, you'll really be in for it!"
  FieldEvent_ShowMessageThenWait(ACTOR_DOUBTING_SOLDIER, 0, 10);
  Actor_WalkByAndWait(ACTOR_ALEX, 16, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Do you honestly believe that even a hundred of you could stop me?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 2);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 2);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_DOUBTING_SOLDIER, 0.5);
  Actor_SetSpeed(ACTOR_SOLDIER_CAPTAIN, 0.5);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_DOUBTING_SOLDIER)) + 90) &= 254;
  Actor_WalkToAndWait(ACTOR_DOUBTING_SOLDIER, 488, 424);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_DOUBTING_SOLDIER)) + 90) |= 1;
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_SOLDIER_CAPTAIN)) + 90) &= 254;
  Actor_WalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 488, 440);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_SOLDIER_CAPTAIN)) + 90) |= 1;
  Actor_WalkToAndWait(ACTOR_ALEX, 456, 432);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "How amusing... This I must see."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DOUBTING_SOLDIER, 3);
  Actor_SetAttachedEffect(ACTOR_DOUBTING_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SOLDIER_CAPTAIN, 3);
  Actor_SetAttachedEffect(ACTOR_SOLDIER_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_DOUBTING_SOLDIER)) + 90) &= 254;
  Actor_WalkToAndWait(ACTOR_DOUBTING_SOLDIER, 504, 424);
  FieldEvent_Wait(1);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_DOUBTING_SOLDIER)) + 90) |= 1;
  Actor_SetPosition(ACTOR_DOUBTING_SOLDIER, 0, 0);
  *(u8 *)(((u8 *)Actor_GetById(ACTOR_SOLDIER_CAPTAIN)) + 90) &= 254;
  Actor_WalkToAndWait(ACTOR_SOLDIER_CAPTAIN, 504, 440);
  FieldEvent_Wait(1);
  {
    u8 *flags = ((u8 *)Actor_GetById(ACTOR_SOLDIER_CAPTAIN)) + 90;
    u8 v = *flags | 1;

    *flags = v;
  }
  Actor_SetPosition(ACTOR_SOLDIER_CAPTAIN, 0, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkByAndWait(ACTOR_ALEX, 64, 0);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(ACTOR_JENNA, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "They're gone..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Come on, Jenna... Let's be going."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "There's nothing standing in our way now!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "But Felix's party can't carry any more, so they left it behind."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What is it now?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
  FieldEvent_Wait(10);
  {
    s32 a1 = EMOTE_IN_FRONT | EMOTE_LIGHT_BULB;
    s32 a2 = 40;
    s32 a0 = ACTOR_KRADEN;

    Actor_ShowEmote(a0, a1, a2);
  }
  // "Ah... You're worried about hurting any innocents?" Yes: "Well, just try
  // to hold back when you're fighting." No: "Oh. You must be concerned
  // about Felix."
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_JENNA, 0) == 0) {
    FieldEvent_Wait(20);
    // "Well, just try to hold back when you're fighting."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message += 1;
    // "Oh. You must be concerned about Felix. We can't do anything to..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Let's get going! To the peninsula!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  actor = Actor_Get(ACTOR_JENNA);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, ((s16 *)&actor->x)[1],
                         ((s16 *)&actor->z)[1]);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  EventCamera_FollowActor(ACTOR_JENNA, 1);
  Event_End();
}

/// The Ruffian left to wait for backup sees that Alex is not with the two, and
/// fights them.
void VenusLighthouseEntrance_RuffianWaitsForBackup(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhyDoI);
  Actor_WalkToAndWait(ACTOR_JENNA, 72, 266);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(10);
  // "Why do I gotta be the one to wait for backup!?"
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
  FieldActor_SetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_WAITING_RUFFIAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WAITING_RUFFIAN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 15);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 35);
  Actor_FaceDirection(ACTOR_WAITING_RUFFIAN, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, 0xc);
  // "Yaaah! P-P-Please! Just let me go, and... Hey!"
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 0);
  Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
  Actor_ShowEmote(ACTOR_WAITING_RUFFIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  50);
  Actor_FaceDirection(ACTOR_WAITING_RUFFIAN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "That scary guy with the blue hair's not with you..."
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
  FieldActor_SetAnimationAndWait(ACTOR_WAITING_RUFFIAN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_WAITING_RUFFIAN, 1);
  Actor_WalkByAndWait(ACTOR_WAITING_RUFFIAN, 16, 0);
  FieldEvent_Wait(10);
  // "Great! Now, you'll see what I'm really made of!"
  FieldEvent_ShowMessageThenWait(ACTOR_WAITING_RUFFIAN, 0, 10);
  Actor_SetSpeed(ACTOR_WAITING_RUFFIAN, 2);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 15);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 25);
  FieldActor_Jump(ACTOR_WAITING_RUFFIAN, 6, 0);
  Actor_WalkBy(ACTOR_WAITING_RUFFIAN, 26, 0);
  FieldEvent_Wait(8);
  Actor_SetAnimation(ACTOR_WAITING_RUFFIAN, ANIM_STAND);
  FieldEvent_SetAfterBattleDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE, 99);
  Event_SetDefeatDestination(SCENE_VENUS_LIGHTHOUSE_ENTRANCE, 98);
  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(9, 0);
  Event_End();
}

/// Kraden jumps out from behind Jenna to send her west, and she steps back.
void VenusLighthouseEntrance_PointToMeetingSpot(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgNotThatWayThe);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_WEST);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  // "Not that way, Jenna! The meeting spot is west of here. We must go..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  object = Actor_Get(ACTOR_JENNA);
  if (object != 0) {
    Actor_SetDestination(ACTOR_KRADEN, ((s16 *)&object->x)[1],
                         ((s16 *)&object->z)[1]);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_JENNA, -16, 0);
  Event_End();
}

/// After a lost battle in the prologue, Jenna wakes by the door with Kraden and
/// Alex beside her; Alex leaves again for Lalivero.
void VenusLighthouseEntrance_WakeUp(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_JENNA, 364, 156);
  Actor_SetPosition(ACTOR_KRADEN, 362, 142);
  Actor_SetPosition(ACTOR_ALEX, 390, 144);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
  FieldActor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_SetSpriteRotationById(ACTOR_JENNA, TRIG_TURN / 4);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
  FieldCamera_MoveToActorById(ACTOR_JENNA, 0);
  gFieldWork->transitionFrames = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgAreAreYou);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Jenna... Are... Are you all right?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Ah, Jenna... You're awake."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Can you stand?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(30);
  Actor_SetPosition(ACTOR_JENNA, 376, 164);
  Actor_SetSpriteRotationById(ACTOR_JENNA, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  FieldActor_Jump(ACTOR_JENNA, 6, 50);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_JENNA, -14, -2);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "It's a good thing I came back for you two..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Perhaps you ought to come with us this time..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "No. Someone has to keep Tolbi's soldiers at bay."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "I'll have to go to Lalivero and confront them there."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "We'll meet at Idejima."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_ALEX);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkBy(ACTOR_ALEX, 0, 120);
  FieldEvent_Wait(40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_KRADEN, 24, 0);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_WaitForMoveById(ACTOR_ALEX);
  Actor_SetPosition(ACTOR_ALEX, 0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "He's gone again."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_KRADEN, 4, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "And we'd best be going as well..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  object = Actor_Get(ACTOR_JENNA);
  if (object != 0) {
    Actor_SetDestination(ACTOR_KRADEN, ((s16 *)&object->x)[1],
                         ((s16 *)&object->z)[1]);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  gFieldWork->transitionFrames = 8;
  Event_End();
}

struct GlowWork {
  u8 unk000[0x400];
  struct GlowSprite sprites[16];
  struct GlowState states[16];
  struct Actor *object;
  struct Actor *effect;
  s32 x;
  s32 y;
  s32 z;
  s16 stage;
  s16 timer;
  u16 tileTop;
  u16 tile;
  s16 height;
  s8 follow;
};

void VenusLighthouseEntrance_UpdateBlastColumn(void) {
  struct GlowWork *work = Ram_HeapSlots->menuWork;
  struct Actor *object = work->object;
  struct Actor *effect = work->effect;
  s32 i, y;
  s32 scale;
  s32 frameStep;
  s32 mask;
  struct GlowState *state;
  struct GlowSprite *sprite;

  switch (work->stage) {
  case 0:
    if (work->timer == 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 1:
    work->height += 4;
    if (work->height > 31) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0) {
      object->motionFlags = 3;
      object->velocityY = px(24);
      work->follow = 0;
      Sound_Play(SE_HEAVY_IMPACT);
    }
    work->height += 8;
    if (work->height > 79) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 4: {
    s16 *timer = &work->timer;

    if (work->timer == 10) {
      object->z = 0;
      object->x = 0;
    }
    if (*timer == 30) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 5:
    work->height -= 8;
    if (work->height <= 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 6: {
    s16 *timer;

    scale = effect->scaleX;
    if (scale > 0)
      scale -= 0x51e;
    else
      scale = 0;
    effect->scaleX = scale;
    effect->scaleY = scale;
    /* FAKEMATCH(dead): an empty once-only block after the scale's stores. */
    do {
    } while (0);
    timer = &work->timer;
    if (work->timer == 10)
      Sound_Play(SE_310);
    if (*timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 7: {
    s16 *timer = &work->timer;

    if (work->timer == 0) {
      Sound_Play(SE_CLATTER);
      MapGrid_SetShake(3, 3, 1);
    }
    if (*timer == 30) {
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (work->timer == 30) {
        work->stage = 999;
        work->timer = -1;
      }
    }
    break;
  }
  }
  object->facing += 0x1000;
  if (work->height > 0) {
    for (i = 0, y = 0; i < 16 && y < work->height - 8; i++, y += 16) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (y << 16) + 0x80000;
      state->z = work->z;
      sprite = &work->sprites[i];
      state->frame++;
      frameStep = state->frame >> 2;
      mask = 3;
      sprite->tile = work->tileTop + (frameStep & mask) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, state);
    }
    if (i <= 15) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (work->height << 16);
      state->z = work->z;
      state->frame++;
      state->y +=
          Math_MulQ16(0xfffe0000, Math_Sin(state->frame << 12)) + 0xfffe0000;
      sprite = &work->sprites[i];
      sprite->tile = work->tile + ((state->frame >> 2) & 3) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, state);
      if (work->follow != 0)
        object->y = state->y;
    }
  }
  work->timer++;
}

// object the overlay links.

s32 BattleEffect_GetObjectDepth(struct Actor *actor);

/// A column of rising earth lifts an actor, holds it, then hurls it off the
/// top of the screen, and the ground shakes as the column sinks.
void VenusLighthouseEntrance_BlastActorSkyward(s32 id) {
  struct GlowWork *work = HeapSlot_Alloc(HEAP_SLOT_MENU, 0x860);
  struct Actor *actor = Actor_Get(id);
  struct Actor *effect;
  struct GlowSprite *sprite;
  struct GlowState *state;
  s32 resource, tile, i;
  u8 zero = 0; // Holds 0 for its whole life.

  actor->paused = zero;
  work->object = actor;
  work->x = actor->x;
  work->y = actor->y;
  work->z = actor->z + px(2);
  Decompress_TaggedLz(gField_Tiles64D, work);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, 0x800, work);
  work->tile = tile;
  work->tileTop = tile + 32;
  state = work->states;
  sprite = work->sprites;
  for (i = 15; i >= 0;) {
    VenusLighthouseEntrance_InitGlowSprite(sprite, 8, 8, 0x40000000, tile);
    sprite->unk1e = BattleEffect_GetObjectDepth(actor) - 2;
    sprite->priority = BattleEffect_GetObjectPriority(actor);
    sprite->fullColor = 1;
    sprite->palette = 0;
    state->frame = -1;
    i--;
    sprite++;
    state++;
  }
  effect = (FieldActor_Create(290, actor->x, actor->y, actor->z));
  work->effect = effect;
  Actor_SelectAnimation(effect, 1);
  Actor_SetSpriteFlags(effect, 0);
  effect->priorityFlags = 2;
  Actor_SetSpriteFlags(actor, 0);
  ((struct GlowAnimation *)actor->sprite)->angle = 0x4000;
  actor->x += -px(12);
  actor->motionFlags = 0;
  work->stage = 0;
  work->timer = 0;
  work->height = 0;
  work->follow = 1;
  Event_PlaySound(SE_RISE_WITH_SPARKS);
  Task_AddFrame(VenusLighthouseEntrance_UpdateBlastColumn, 0x480);
  if (work->stage != 999) {
  wait:
    System_Wait(1);
    if (work->stage != 999)
      goto wait;
  }
  FieldTask_Remove(VenusLighthouseEntrance_UpdateBlastColumn);
  Actor_Release(effect);
  VramCache_FreeEntry(resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

void BattleEffect_StepVector3(struct DustPuffState *state, s32 a, s32 b);

void VenusLighthouseEntrance_UpdateDust(void) {
  struct DustPuffWork *work = Ram_HeapSlots->menuWork;
  struct DustPuffState *state = work->states;
  struct DustPuffSprite *sprite = work->sprites;
  s32 value;
  s32 i;

  for (i = 0; i < 96; i++, sprite++, state++) {
    value = state->frame;
    if ((u32)value < 16) {
      sprite->tile = work->unk2182 + ((value >> 1) & 7) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, state);
      BattleEffect_StepVector3(state, 64, 0);
      state->frame++;
    }
  }
}

/* VIEW(venus_lighthouse_entrance): tag struct SparkSprite as struct
 * SparkSprite__venus_lighthouse_entrance; module/tla.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/venus_lighthouse_entrance.c:1407: structure has
 * no member named `unk1e'. */
struct SparkSprite__venus_lighthouse_entrance {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite__venus_lighthouse_entrance sprites[96];
  s16 next;
  u16 tileBase;
};

void VenusLighthouseEntrance_EmitDust(struct Actor *source, s32 x, s32 y,
                                      s32 z) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;
  s32 angle = Random16();
  s16 *next = &work->next;
  struct SparkSprite__venus_lighthouse_entrance *sprite = &work->sprites[*next];
  struct SparkState *state = &work->states[*next];

  sprite->priority = BattleEffect_GetObjectPriority(source);
  sprite->unk1e = BattleEffect_GetObjectDepth(source);
  state->frame = 0;
  state->x = x;
  state->y = y;
  state->z = z;
  FieldVector_AddPolarOffset(Random16() * 2 + 0x20000, angle, &state->x);
  state->velocityX = 0;
  state->velocityY = (Random16() >> 1) + 0xc000;
  state->velocityZ = 0;
  FieldVector_AddPolarOffset(Random16() >> 2, angle, &state->velocityX);
  *next = *next + 1;
  *next = *next % 96;
}

/* VIEW(s26f4): tag struct SparkSprite as struct SparkSprite__s26f4;
 * venus_lighthouse_entrance.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse_entrance.c:1470: structure has no member
 * named `unk1e'. */
struct SparkSprite__s26f4 {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

/* VIEW(s26f4): tag struct SparkWork as struct SparkWork__s26f4;
 * venus_lighthouse_entrance.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse_entrance.c:1467: structure has no member
 * named `resource'. */
struct SparkWork__s26f4 {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite__s26f4 sprites[96];
  u16 unk2180;
  u16 tileBase;
  u16 resource;
};

void VenusLighthouseEntrance_StartDust(void) {
  struct SparkWork__s26f4 *work = HeapSlot_Alloc(HEAP_SLOT_MENU, 0x2188);
  s32 resource;
  s32 tileBase;
  s32 i;
  struct SparkState *state;
  struct SparkSprite__s26f4 *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  resource = VramCache_FindFreeEntry();
  tileBase = Vram_Load(resource, 0x800, work);
  work->tileBase = tileBase;
  work->resource = resource;
  state = work->states;
  sprite = work->sprites;
  for (i = 95; i >= 0; i--) {
    VenusLighthouseEntrance_InitGlowSprite(sprite, 8, 8, 0x40000000, tileBase);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    state->frame = -1;
    state++;
    sprite++;
  }
  {
    s32 zero = 0;
    work->unk2180 = zero;
  }
  Task_AddFrame(VenusLighthouseEntrance_UpdateDust, (s32)(0x480));
}

/* VIEW(sparks): tag struct SparkWork as struct SparkWork__sparks;
 * venus_lighthouse_entrance.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/venus_lighthouse_entrance.c:1505: structure has no member
 * named `resource'. */
struct SparkWork__sparks {
  u8 unk0000[0x2102];
  u16 tileBase;
  u8 unk2104[0x80];
  s16 resource;
};

void VenusLighthouseEntrance_StopDust(void) {
  struct SparkWork__sparks *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(VenusLighthouseEntrance_UpdateDust);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_FROM_LIGHTHOUSE, CONDITION_ALWAYS, 376, 0, 104, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 376, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 384, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 112, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_JENNA_TAKES_OVER, CONDITION_ALWAYS, 376, 0, 104, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {ENTRANCE_DEFEAT, CONDITION_ALWAYS, 384, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 72, 0, 264, FACING_WEST, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(VenusLighthouseEntrance),
    SCENE_EXIT(1, SuhallaGate1, 5),
    SCENE_EXIT(2, VenusLighthouse, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(92, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 176,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(92, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 152,
                    FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(232, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 408,
                    FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(232, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 408,
                    FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(232, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 424,
                    FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(232, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 440,
                    FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(232, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 408,
                    FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 408,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 408,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 424,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 408,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 264,
                    FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 9, FLAG_VENUS_LIGHTHOUSE_BIRDS_FLOWN,
     EVENT(VenusLighthouseEntrance_BirdsFlyOff)},
    {TRIGGER_STEP, 10, FLAG_VENUS_LIGHTHOUSE_AMBUSH_SEEN,
     EVENT(VenusLighthouseEntrance_SoldiersAmbushParty)},
    {TRIGGER_STEP, 11, FLAG_VENUS_LIGHTHOUSE_RUFFIAN_BEATEN,
     EVENT(VenusLighthouseEntrance_RuffianWaitsForBackup)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_PointToMeetingSpot)},
    {TRIGGER_TALK, ACTOR_WAITING_RUFFIAN, CONDITION_ALWAYS,
     MSGNO(MsgUggghICan)},
    {TRIGGER_TALK, ACTOR_DOUBTING_SOLDIER, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_BlastActorWithPsynergy)},
    {TRIGGER_TALK, ACTOR_SOLDIER_CAPTAIN, CONDITION_ALWAYS,
     EVENT(VenusLighthouseEntrance_RunWithDustTrail)},
    {TRIGGER_TABLE_END},
};

// clang-format off
u32 gVenusLighthouseEntrance_BirdLands[] = {
    1, 0, 1, 39, 2, 0,
    4, 22, 26, 3, 22, 13,
    0, 0, 1, 17,
};

u32 gVenusLighthouseEntrance_BirdFlies[] = {
    1, 0, 1, 38,
};
// clang-format on
