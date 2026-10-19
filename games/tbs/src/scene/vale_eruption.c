/// @file
/// Vale's square during the eruption, where the villagers wait for Isaac and
/// Garet to come down the mountain. The erupting mountain is Mt. Aleph.

#include "game_state.h"
#include "unit/scene/vale_eruption.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "fixed_math.h"

enum ValeEruptionActor {
  /// Garet's grandfather. "Oooh, my grandson! Will Garet be safe in this
  /// eruption?!?"
  ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED + 1,
  /// The guard. "They were hanging around here while I was on guard duty."
  ACTOR_GUARD = ACTOR_FIRST_PLACED + 5
};

static void Event_ReturnFromMtAleph(void);

/// Return where the party appears: the path down from the mountain. The
/// mountain is Mt. Aleph.
struct SceneEntrance *ValeEruption_GetEntrances(void) {
  return gValeEruption_Entrances;
}

/// The square has no scene regions.
struct SceneRegion *ValeEruption_GetRegions(void) { return NULL; }

/// Return the way back to Vale.
u32 *ValeEruption_GetExits(void) { return gValeEruption_Exits; }

/// Return Garet and the villagers gathered before the sanctum.
struct ScenePlacement *ValeEruption_GetPlacements(void) {
  return gValeEruption_Placements;
}

/// The square has no events of its own: everything happens on arrival.
struct SceneEvent *ValeEruption_GetEvents(void) { return gValeEruption_Events; }

/// Arriving by entrance 15, from the top of the mountain, plays the return.
s32 ValeEruption_Initialize(void) {
  if (gGameState.entrance == 15) {
    Event_ReturnFromMtAleph();
  }
  return 0;
}

/// The volcano shakes the square while the villagers wonder where Isaac and
/// Garet are. The two come down the mountain, are welcomed, tell that Jenna and
/// Kraden were taken away, and follow the elders into the sanctum.
static void Event_ReturnFromMtAleph(void) {
  struct EventWork *work;
  const s32 *actionScript;

  Engine_EventBegin();
  Engine_ActorSetAnimation(14, 0);
  Engine_ActorSetAnimation(15, 0);
  Engine_ActorSetAnimation(16, 0);
  Engine_ActorSetAnimation(17, 0);
  Engine_ActorSetAnimation(18, 0);
  Engine_ActorSetAnimation(19, 0);
  Actor_WalkToAndWait(11, 265, 487);
  Actor_FaceDirection(11, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(12, 256, 500);
  Actor_FaceDirection(12, FACING_NORTHWEST, 0);
  BattleFx_StartBufferBlend(0x10003, 0x10006);
  FieldFx_LastLightning();
  Engine_TaskWait(60);
  Camera_MoveTo(PIXELS(256), -1, PIXELS(612), 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  work = gEventWork;
  work->startTransition = 0;
  work->transitionFrames = 32;
  Engine_EventOpenScreen();

  // The camera pans up to the villagers as the ground shakes.
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(PIXELS(256), -1, PIXELS(500), 1);
  Engine_EventWait(20);
  Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Engine_EventWait(30);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Engine_CameraWaitForMove();
  Map_SetShake(PIXELS(2), PIXELS(3), Q16_ONE);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();
  Engine_EventWait(60);

  // "I thought Mt. Aleph was inactive..." The villagers worry about Isaac and
  // Garet, last seen going to Kraden's house.
  Engine_EventSetMessage(MsgValeMtAlephWasInactive);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(11, ANIM_SHAKE_HEAD);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceActor(12, 11, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(12, ANIM_SHAKE_HEAD);
  Event_ShowMessage(12, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_GUARD, 1);
  Event_ShowMessage(ACTOR_GUARD, 0);
  Actor_FaceActor(10, ACTOR_GUARD, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(10, 1);
  Event_ShowMessage(10, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, 10, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(10, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(10, ANIM_SHAKE_HEAD);
  Event_ShowMessage(10, 0);
  Engine_EventWait(60);
  Map_SetShake(PIXELS(2), PIXELS(3), Q16_ONE);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Engine_EventWait(60);
  Engine_ActorFaceEachOther(8, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_ActorFaceEachOther(10, 11, 0);
  Engine_ActorFaceEachOther(12, ACTOR_GUARD, 0);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorStartRepeatedMotion(12, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GUARD, 2);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();

  // Isaac and Garet come down the path, and Garet sees the villagers: "Uh-oh.
  // It's Grandpa and the others."
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_SetSpeed(GARET, 0x8000, 0x4000);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(120), PIXELS(258));
  Camera_SetSpeed(0x18000, 0x3000);
  Camera_MoveTo(PIXELS(112), -1, PIXELS(320), 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 120, 320);
  Actor_MoveToAndWait(GARET, 104, 320);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Engine_EventWait(50);
  Actor_SetSpeed(GARET, 0x18000, 0xc000);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Actor_MoveToAndWait(GARET, 105, 342);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(20);

  // "What should we do, Isaac? Should we go back up?" Either way, the two stay.
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(60);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    Actor_SetSpeed(GARET, Q16_ONE, 0x8000);
    Actor_MoveToAndWait(GARET, 103, 320);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
  } else {
    Engine_EventWait(60);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_EventWait(50);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 120, 340);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  }

  // The villagers spot them: "Garet!" "Isaac!"
  Event_ShowMessage(12, 0);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(11, FACING_NORTHWEST, 0);
  Actor_FaceDirection(10, FACING_NORTHWEST, 0);
  Actor_FaceDirection(12, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_NORTHWEST, 0);
  Camera_SetSpeed(0x30000, 0x6000);
  Engine_CameraMoveToActor(10, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(10, 2);
  Event_ShowMessage(10, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_ShowMessage(8, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_MoveTo(PIXELS(112), -1, PIXELS(320), 1);
  Engine_CameraWaitForMove();
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Object_RefreshSelectorById(1);
  Engine_EventWait(50);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Object_RefreshSelectorById(1);
  Engine_EventWait(60);

  // "Come over here, both of you!" The two walk down to the villagers.
  Camera_SetSpeed(Q16_ONE, 0x2000);
  Camera_MoveTo(PIXELS(214), -1, PIXELS(472), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, Q16_ONE, 0x8000);
  Actor_SetSpeed(GARET, Q16_ONE, 0x8000);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeEruption_LeaderWalksDown);
  Engine_EventWait(30);
  Actor_SetActionScript(GARET, gValeEruption_GaretWalksDown);
  Object_RefreshSelectorById(1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Engine_ActorSetAnimation(8, ANIM_WALK);
  Actor_MoveToAndWait(8, 265, 455);
  Actor_MoveToAndWait(8, 246, 455);
  Engine_ActorSetAnimation(8, ANIM_STAND);
  Engine_EventWait(30);

  // "Where have you been?" "What happened?"
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(50);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_ShowMessage(8, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(40);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(50);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);

  // "Weren't Jenna and Kraden with you?" Yes: "They were taken away? By
  // whom?"; no: "What do you mean, they were taken away?"
  Event_OpenMessage(10, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(8, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_SkipMessages(1);
  } else {
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(8, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(8, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_SkipMessages(1);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  }
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);

  // "This is all very complicated... Come into the sanctum, both of you."
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_ShowMessage(8, 0);
  Actor_FaceActor(8, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(8, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTHWEST + FACING_STEP,
                      0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_EventWait(50);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_ShowMessage(8, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(8, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(8, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_EventWait(30);

  // The sanctum's door opens and everyone files inside.
  Actor_WalkToAndWait(8, 255, 445);
  Engine_EventWait(40);
  Map_AnimateCells(gValeEruption_SanctumDoorOpens, 45, 11);
  Event_PlayCue(SE_188);
  Engine_EventWait(30);
  Actor_WalkTo(8, 255, 390);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, 0xcccc, 0x6666);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkTo(ACTOR_GARETS_GRANDFATHER, 255, 390);
  Actor_WalkToAndWait(10, 255, 460);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Actor_WalkTo(10, 255, 390);
  actionScript = gValeEruption_EnterSanctum;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, actionScript);
  Engine_EventWait(40);
  Actor_SetActionScript(GARET, actionScript);
  Object_RefreshSelectorById(1);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  Map_SetShake(PIXELS(2), PIXELS(3), Q16_ONE);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Engine_EventWait(30);
  work = gEventWork;
  work->startTransition = 0;
  work->transitionFrames = 64;
  Engine_EventCloseScreen();
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Set(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
  Engine_EventRequestExit(1);
  Engine_EventEnd();
}

/// Isaac walks down from the path to the villagers before the sanctum.
s32 gValeEruption_LeaderWalksDown[] = {
    SC_ARG(2), SC_MOVE(PIXELS(128), 0, PIXELS(378)),
    SC_SYNC,   SC_MOVE(PIXELS(203), 0, PIXELS(378)),
    SC_SYNC,   SC_MOVE(PIXELS(212), 0, PIXELS(472)),
    SC_SYNC,   SC_MOVE(PIXELS(214), 0, PIXELS(472)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

/// Garet follows him, stopping a little above.
s32 gValeEruption_GaretWalksDown[] = {
    SC_ARG(2), SC_MOVE(PIXELS(128), 0, PIXELS(378)),
    SC_SYNC,   SC_MOVE(PIXELS(203), 0, PIXELS(378)),
    SC_SYNC,   SC_MOVE(PIXELS(214), 0, PIXELS(455)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

/// Isaac, then Garet, walk through the sanctum's door.
s32 gValeEruption_EnterSanctum[] = {
    SC_ARG(2), SC_MOVE(PIXELS(255), 0, PIXELS(452)),
    SC_SYNC,   SC_MOVE(PIXELS(255), 0, PIXELS(390)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

struct SceneEntrance gValeEruption_Entrances[] = {
    {0, CONDITION_ALWAYS, 110, 0, 270, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 103, 0, 284, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gValeEruption_Exits[] = {
    SCENE_EXITS(10),
    SCENE_EXIT(1, 9, 20),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeEruption_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(258), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(468), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x75, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(468), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(487), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(477), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x25, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(490), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(523), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(245), 0, PIXELS(532), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(543), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(283), 0, PIXELS(536), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(235), 0, PIXELS(552), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(561), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(247), 0, PIXELS(573), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gValeEruption_Events[] = {
    {SCENE_EVENTS_END},
};

/// The sanctum's door, two cells wide, opening in two steps.
u16 gValeEruption_SanctumDoorOpens[] = {
    MAP_CELL_STEP(0, 98, 2, 2, 2),
    MAP_CELL_STEP(2, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};
