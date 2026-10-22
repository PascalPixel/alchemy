/// @file
/// Vale (SceneId_ValeEruption) while Mt. Aleph erupts: the ground before the
/// sanctum's door, on the village's own map (SceneId_Vale's), where the
/// villagers wait for Isaac and Garet.
///
/// The scene falls between Sol Sanctum's eruption (sol_sanctum_eruption.c),
/// where Jenna and Kraden are carried off, and the party's journey. The path
/// down the mountain leads here, by VALE_ERUPTION_FROM_SOL_ENTRANCE
/// (sol_sanctum_entrance.c's SOL_ENTRANCE_TO_VALE after the eruption).
/// Gathered before the sanctum are the Great Healer, Garet's grandfather the
/// mayor, Isaac's mother Dora, Garet's sister and brother, and the guard who
/// saw the two hanging around on his watch.
///
/// The scene has no events. ValeEruption_Initialize plays the whole scene on
/// arrival by VALE_ERUPTION_FROM_SOL_ENTRANCE (Event_ReturnFromMtAleph): the
/// ground shakes, the villagers wonder where the two are, Isaac and Garet come
/// down the path and are called over, and Dora's question about Jenna and
/// Kraden is answered the same way whatever Isaac replies. The Great Healer
/// then opens the sanctum's door (Map_AnimateCells) and leads everyone in, and
/// the scene's one exit takes the party into the Vale Sanctum
/// (SANCTUM_FROM_VALE_ERUPTION), where the Great Healer passes on the Wise
/// One's quest (Sanctum_RequestQuestAcceptance). The tremors are Map_SetShake
/// calls, each with a flash (FieldFx_LastLightning) and SE_145, over a burst
/// of red flashes on the warm colours (FieldFx_StartBufferBlend). The scene
/// sets FLAG_VALE_RETURNED_FROM_MT_ALEPH but never reads it; Sol Sanctum's
/// entrance scene does (sol_sanctum_entrance.c).
///
/// Open questions:
/// - XXX: what animation 0, which the setup gives the six villagers at the
///   back, shows; their sprites' animation tables under graphics/sprite would
///   settle it.

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
  /// The Great Healer, a man with a white beard in a tall hat and a long
  /// purple and blue robe (the sprite of sanctum.c's healer), who leads the
  /// villagers into the sanctum. "Come into the sanctum, both of you."
  /// ★ Named from the game: "Come into the sanctum" (MsgValeMtAlephWasInactive)
  ACTOR_GREAT_HEALER = ACTOR_FIRST_PLACED,
  /// Garet's grandfather, an old man with white hair and a cane, the mayor
  /// (whom the Great Healer calls Mayor in sanctum.c). "Oooh, my grandson! Will
  /// Garet be safe in this eruption?!?"
  /// ★ Named from the game: "Oooh, my grandson!" (MsgValeMtAlephWasInactive)
  ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED + 1,
  /// Isaac's mother (Dora in sanctum.c). "Isaac and Garet went to Mr. Kraden's
  /// house."
  /// ★ Named from the game: "your mother, Dora, has been crying"
  /// (MsgValeDoraHasBeenCrying)
  ACTOR_DORA = ACTOR_FIRST_PLACED + 2,
  /// Garet's sister, a young woman with an orange ponytail, whom Garet calls
  /// Sis at the farewell (vale_gate.c). She calls the mayor Grandpa, and
  /// ACTOR_GARETS_BROTHER calls her Sis. "Grandpa, we don't even know if he's
  /// up there!"
  /// ★ Named from the game: "we don't even know if he's up there"
  /// (MsgValeMtAlephWasInactive)
  ACTOR_GARETS_SISTER = ACTOR_FIRST_PLACED + 3,
  /// Garet's brother, the small boy with orange hair of vale.c's
  /// ACTOR_GARETS_BROTHER, who says someone saw Garet climb and is first to cry
  /// "Garet!"
  /// ★ Named from the game: "But, Sis, someone saw him"
  /// (MsgValeMtAlephWasInactive), to Garet's sister
  ACTOR_GARETS_BROTHER = ACTOR_FIRST_PLACED + 4,
  /// The guard, the man with long brown hair and a beard in a long robe who
  /// guards Mt. Aleph in vale.c (ACTOR_MAN_GUARDING_MT_ALEPH). "They were
  /// hanging around here while I was on guard duty."
  /// ★ Named from the game: "on guard duty" (MsgValeMtAlephWasInactive)
  ACTOR_GUARD = ACTOR_FIRST_PLACED + 5,
  /// The first of the six villagers gathered behind them: a burly grey-haired
  /// man, a bald muscular man, a woman in a green dress, a little girl with
  /// pigtails, a young man with brown hair and a strongman.
  /// ★ Named from what the player sees: men, a woman and children of Vale in
  /// their sprites (its render)
  ACTOR_FIRST_VILLAGER = ACTOR_FIRST_PLACED + 6
};

static void Event_ReturnFromMtAleph(void);

/// Returns where the party appears: VALE_ERUPTION_FROM_SOL_ENTRANCE is the path
/// down from the mountain, Mt. Aleph.
const struct SceneEntrance *ValeEruption_GetEntrances(void) {
  return gValeEruption_Entrances;
}

const struct SceneRegion *ValeEruption_GetRegions(void) { return NULL; }

/// Returns the one exit, VALE_ERUPTION_TO_SANCTUM into the Vale Sanctum
/// (SANCTUM_FROM_VALE_ERUPTION), which Event_ReturnFromMtAleph takes.
const u32 *ValeEruption_GetExits(void) { return gValeEruption_Exits; }

/// Returns Garet and the villagers gathered before the sanctum.
const struct ScenePlacement *ValeEruption_GetPlacements(void) {
  return gValeEruption_Placements;
}

/// The scene has no events of its own: everything happens on arrival.
const struct SceneEvent *ValeEruption_GetEvents(void) {
  return gValeEruption_Events;
}

/// Arriving in Vale during the eruption: plays the return from the mountain
/// when the party comes by VALE_ERUPTION_FROM_SOL_ENTRANCE. Called by
/// Field_RunScene once the actors are placed; VALE_ERUPTION_FROM_SOL_ENTRANCE
/// is where Sol Sanctum's approach leads after the eruption and before the
/// party leaves Vale.
s32 ValeEruption_Initialize(void) {
  if (gGameState.entrance == VALE_ERUPTION_FROM_SOL_ENTRANCE) {
    Event_ReturnFromMtAleph();
  }
  return 0;
}

/// In Vale during the eruption, before the sanctum, the volcano shakes the
/// ground while the villagers wonder where Isaac and Garet are; the two come
/// down the mountain, tell that Jenna and Kraden were taken away, and follow
/// the elders into the sanctum. Called by ValeEruption_Initialize on arrival by
/// VALE_ERUPTION_FROM_SOL_ENTRANCE. Clears FLAG_SHOW_LOCATION_NAME, sets
/// FLAG_VALE_RETURNED_FROM_MT_ALEPH and takes VALE_ERUPTION_TO_SANCTUM, where
/// Sanctum_RequestQuestAcceptance follows.
static void Event_ReturnFromMtAleph(void) {
  struct EventWork *work;
  const s32 *actionScript;

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_FIRST_VILLAGER, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_VILLAGER + 1, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_VILLAGER + 2, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_VILLAGER + 3, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_VILLAGER + 4, 0);
  Engine_ActorSetAnimation(ACTOR_FIRST_VILLAGER + 5, 0);
  Actor_WalkToAndWait(ACTOR_GARETS_SISTER, 265, 487);
  Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_GARETS_BROTHER, 256, 500);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER, FACING_NORTHWEST, 0);
  // A lightning flash, then the screen opens on the villagers below.
  FieldFx_StartBufferBlend(PALETTE_TRANSFORM_WARM, PALETTE_TRANSFORM_RED);
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
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
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
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Engine_EventWait(60);

  // "I thought Mt. Aleph was inactive..." The villagers worry about Isaac and
  // Garet, last seen going to Kraden's house.
  Engine_EventSetMessage(MsgValeMtAlephWasInactive);
  Actor_ShowEmote(ACTOR_GREAT_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_GREAT_HEALER, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  // The mayor: "Oooh, my grandson!"
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  // Garet's sister doubts he is up there; her brother says someone saw him
  // climb.
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_SISTER, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_GARETS_SISTER, 0);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_FaceActor(ACTOR_GARETS_BROTHER, ACTOR_GARETS_SISTER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_BROTHER, ANIM_SHAKE_HEAD);
  Event_ShowMessage(ACTOR_GARETS_BROTHER, 0);
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
  Engine_ActorFaceEachOther(ACTOR_GARETS_BROTHER, ACTOR_GUARD, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_GREAT_HEALER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_SISTER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_BROTHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GUARD, 2);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();

  // Isaac and Garet come down the path, and Garet sees the villagers: "Uh-oh.
  // It's Grandpa and the others."
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(120), PIXELS(258));
  Camera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
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
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(50);
  Actor_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
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
    Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
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

  // The villagers spot them: "Garet!" "Isaac!" Garet's brother cries "Garet!"
  // and the villagers turn to the path.
  Event_ShowMessage(ACTOR_GARETS_BROTHER, 0);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_SISTER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARETS_BROTHER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_NORTHWEST, 0);
  // The camera flies to Dora: "Isaac!" The Great Healer: "You're safe!"
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
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
  Object_RefreshSelectorById(GARET);
  Engine_EventWait(50);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Object_RefreshSelectorById(GARET);
  Engine_EventWait(60);

  // "Come over here, both of you!" The two walk down to the villagers.
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(214), -1, PIXELS(472), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeEruption_LeaderWalksDown);
  Engine_EventWait(30);
  Actor_SetActionScript(GARET, gValeEruption_GaretWalksDown);
  Object_RefreshSelectorById(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_WEST, 0);
  Actor_SetSpeed(ACTOR_GREAT_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_GREAT_HEALER, 265, 455);
  Actor_MoveToAndWait(ACTOR_GREAT_HEALER, 246, 455);
  Engine_ActorSetAnimation(ACTOR_GREAT_HEALER, ANIM_STAND);
  Engine_EventWait(30);

  // "Where have you been?" "What happened?"
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(50);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
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
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
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
  Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
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
  Object_RefreshSelectorById(GARET);
  Actor_ShowEmote(ACTOR_GARETS_SISTER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARETS_BROTHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(40);
  Map_SetShake(PIXELS(2), PIXELS(3), Q16_ONE);
  FieldFx_LastLightning();
  Event_PlayCue(SE_145);
  Engine_EventWait(30);
  work = gEventWork;
  work->startTransition = 0;
  work->transitionFrames = 64;
  Engine_EventCloseScreen();
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Set(FLAG_VALE_RETURNED_FROM_MT_ALEPH);
  Engine_EventRequestExit(VALE_ERUPTION_TO_SANCTUM);
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
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 110, 0, 270, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VALE_ERUPTION_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 103, 0, 284,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gValeEruption_Exits[] = {
    SCENE_EXITS(ValeEruption),
    SCENE_EXIT(VALE_ERUPTION_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE_ERUPTION),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeEruption_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(258), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(468), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(468), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(487), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(477), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_ORANGE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(490), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(523), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(245), 0, PIXELS(532), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(543), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(283), 0, PIXELS(536), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(235), 0, PIXELS(552), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(561), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
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
