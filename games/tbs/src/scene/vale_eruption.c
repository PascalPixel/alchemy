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

/// The placed actors, numbered in the order of gValeEruption_Placements;
/// Garet's row places the party's Garet and takes no number.
enum ValeEruptionActor {
  /// The Great Healer (sprite 0x79, the healer of sanctum.c), who leads the
  /// villagers into the sanctum. "Come into the sanctum, both of you."
  ACTOR_GREAT_HEALER = ACTOR_FIRST_PLACED,
  /// Garet's grandfather, the mayor (sprite 0x75, whom the Great Healer calls
  /// Mayor in sanctum.c). "Oooh, my grandson! Will Garet be safe in this
  /// eruption?!?"
  ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED + 1,
  /// Isaac's mother (sprite 0x1f, Dora in sanctum.c). "Isaac and Garet went
  /// to Mr. Kraden's house."
  ACTOR_DORA = ACTOR_FIRST_PLACED + 2,
  /// She calls the mayor Grandpa, and actor 12 calls her Sis. "Grandpa, we
  /// don't even know if he's up there!"
  ACTOR_GARETS_SISTER = ACTOR_FIRST_PLACED + 3,
  /// The guard. "They were hanging around here while I was on guard duty."
  ACTOR_GUARD = ACTOR_FIRST_PLACED + 5
};

static void Event_ReturnFromMtAleph(void);

/// Returns where the party appears: entrance 15 is the path down from the
/// mountain, Mt. Aleph.
struct SceneEntrance *ValeEruption_GetEntrances(void) {
  return gValeEruption_Entrances;
}

struct SceneRegion *ValeEruption_GetRegions(void) { return NULL; }

/// Returns the one exit, into the sanctum (Sanctum's entrance 20), which
/// Event_ReturnFromMtAleph takes.
u32 *ValeEruption_GetExits(void) { return gValeEruption_Exits; }

/// Returns Garet and the villagers gathered before the sanctum.
struct ScenePlacement *ValeEruption_GetPlacements(void) {
  return gValeEruption_Placements;
}

/// The square has no events of its own: everything happens on arrival.
struct SceneEvent *ValeEruption_GetEvents(void) { return gValeEruption_Events; }

/// Plays the return from the mountain when the party arrives by entrance 15.
/// Called by Field_RunScene once the actors are placed; entrance 15 is where
/// Sol Sanctum's approach leads after the eruption and before the party
/// leaves Vale (its exit 1).
s32 ValeEruption_Initialize(void) {
  if (gGameState.entrance == 15) {
    Event_ReturnFromMtAleph();
  }
  return 0;
}

/// The volcano shakes the square while the villagers wonder where Isaac and
/// Garet are; the two come down the mountain, tell that Jenna and Kraden were
/// taken away, and follow the elders into the sanctum.
/// Called by ValeEruption_Initialize on arrival by entrance 15. Clears
/// FLAG_SHOW_LOCATION_NAME, sets FLAG_VALE_RETURNED_FROM_MT_ALEPH and takes
/// exit 1 into the sanctum, where Sanctum_RequestQuestAcceptance follows.
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
  Actor_WalkToAndWait(ACTOR_GARETS_SISTER, 265, 487);
  Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(12, 256, 500);
  Actor_FaceDirection(12, FACING_NORTHWEST, 0);
  // A lightning flash, then the screen opens on the square below.
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
  Actor_ShowEmote(ACTOR_GREAT_HEALER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  // The mayor: "Oooh, my grandson!"
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  // Garet's sister doubts he is up there; actor 12 says someone saw him climb.
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_SISTER, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_GARETS_SISTER, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceActor(12, ACTOR_GARETS_SISTER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(12, ANIM_SHAKE_HEAD);
  Event_ShowMessage(12, 0);
  // The guard saw them hanging around; Dora says they went to Kraden's house,
  // and the mayor and Dora wonder what Alchemy has to do with the eruption.
  Engine_ActorRunRepeatedMotion(ACTOR_GUARD, 1);
  Event_ShowMessage(ACTOR_GUARD, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GUARD, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
  Event_ShowMessage(ACTOR_DORA, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_DORA, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(60);
  Map_SetShake(PIXELS(2), PIXELS(3), Q16_ONE);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Engine_EventWait(60);
  Engine_ActorFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_ActorFaceEachOther(ACTOR_DORA, ACTOR_GARETS_SISTER, 0);
  Engine_ActorFaceEachOther(12, ACTOR_GUARD, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_GREAT_HEALER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_SISTER, 2);
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
  // Actor 12 cries "Garet!" and the villagers turn to the path.
  Event_ShowMessage(12, 0);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(12, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_NORTHWEST, 0);
  // The camera flies to Dora: "Isaac!" The Great Healer: "You're safe!"
  Camera_SetSpeed(0x30000, 0x6000);
  Engine_CameraMoveToActor(ACTOR_DORA, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Engine_EventWait(40);
  // The mayor: "Come over here, both of you!"
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
  Actor_SetSpeed(ACTOR_GREAT_HEALER, 0xcccc, 0x6666);
  Engine_ActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_GREAT_HEALER, 265, 455);
  Actor_MoveToAndWait(ACTOR_GREAT_HEALER, 246, 455);
  Engine_ActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_STAND);
  Engine_EventWait(30);

  // "Where have you been?" "What happened?"
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(50);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(50);
  // The Great Healer: "What happened?" The two look at each other and shake
  // their heads.
  Engine_ActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(40);
  // Dora notices who is missing.
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(50);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);

  // "Weren't Jenna and Kraden with you?" Yes: "They were taken away? By
  // whom?"; no: "What do you mean, they were taken away?"
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    // The mayor answers either reply; the other reply is skipped.
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_GREAT_HEALER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_SkipMessages(1);
  } else {
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_GREAT_HEALER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_SkipMessages(1);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  }
  Engine_EventWait(30);
  // Garet starts to tell, and both shake their heads again.
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);

  // "This is all very complicated... Come into the sanctum, both of you."
  Engine_ActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Actor_FaceActor(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_GREAT_HEALER, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Engine_EventWait(20);
  // The mayor agrees with a nod; the Great Healer sends them inside.
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTHWEST + FACING_STEP,
                      0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_EventWait(50);
  Actor_FaceDirection(ACTOR_GREAT_HEALER, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GREAT_HEALER, 1);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Engine_EventWait(40);
  Engine_ActorFaceEachOther(ACTOR_GREAT_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Engine_EventWait(30);

  // The sanctum's door opens and everyone files inside.
  Actor_WalkToAndWait(ACTOR_GREAT_HEALER, 255, 445);
  Engine_EventWait(40);
  Map_AnimateCells(gValeEruption_SanctumDoorOpens, 45, 11);
  Event_PlayCue(SE_188);
  Engine_EventWait(30);
  Actor_WalkTo(ACTOR_GREAT_HEALER, 255, 390);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, 0xcccc, 0x6666);
  Actor_SetSpeed(ACTOR_DORA, 0xcccc, 0x6666);
  Actor_WalkTo(ACTOR_GARETS_GRANDFATHER, 255, 390);
  Actor_WalkToAndWait(ACTOR_DORA, 255, 460);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Actor_WalkTo(ACTOR_DORA, 255, 390);
  actionScript = gValeEruption_EnterSanctum;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, actionScript);
  Engine_EventWait(40);
  Actor_SetActionScript(GARET, actionScript);
  Object_RefreshSelectorById(1);
  Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | 2, 0);
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
