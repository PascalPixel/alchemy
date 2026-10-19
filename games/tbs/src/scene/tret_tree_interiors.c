#include "battle_effect_runtime.h"
#include "version.h"
#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/tret_tree_interiors.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"

#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "global_cells.h"
#include "iwram_call.h"
#include "dma.h"
#include "field_service.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "heap_state.h"
#include "field_effect.h"
#include "flags.h"
#include "character_ids.h"
#include "map_scroll.h"
#include "songs.h"
#include "event.h"

/// Tret's face, the first actor placed after the party, and a second Tret
/// actor placed at the same spot. Both speak Tret's lines: the face as he
/// attacks, the second after the battle.
enum {
  ACTOR_TRET = ACTOR_FIRST_PLACED,
  ACTOR_TRET_SPEAKER,
  SPRITE_TRET_FACE = 0x37
};

/// The speaker of a message; every speaker in this scene has
/// SPEAKER_WINDOW_BELOW.
#define SPEAKER(actor) (SPEAKER_WINDOW_BELOW | (actor))

// The scene's temporary flags: FLAG_SCENE_TEMP_00 to _14 are set the first
// time each cracked floor tile is stepped on; the next step makes it give
// way (gTretTreeInteriors_MapPatches lists them).

#define FLOOR_FALL_SPIN_FRAMES 30
#define SWIRL_SPARK_LAST_FRAME 79
#define SWIRL_SPARK_SHRINK_LAST_FRAME 39
#define TRET_TINT_COLOR_COUNT 63
#define TRET_TINT_CHANNEL_MAX 31
#define TRET_TINT_END 99

/// Forward declarations.
static void TretPalette_SaveSceneColors(void);
static void TretPalette_UpdateSceneTint(void);
static void TretTreeInteriors_PlayGesture(s32 gesture);
static void TretTreeInteriors_SpawnSwirlSparks(void);
static void TretTreeInteriors_ApplyFlaggedMapPatches(void);
static void TretTreeInteriors_RunLandingDustScene(void);

static struct FieldPosition sSparkOrigin;
static s32 sSparkFrame;
static s16 *sLastFloorEntrance;

// Shared source is included here in ROM order.
#include "common/effect_create.h"

// Shared source is included here in ROM order.
#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

/// Return the interior entrance and landing positions.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTretTreeInteriorsEntrances;
}

/// Return the upper exit regions for the tree interior.
const struct SceneRegion *Scene_GetRegions(void) {
  return gTretTreeInteriorsRegions;
}

/// Return the interior's packed exits.
const u32 *Scene_GetExits(void) { return gTretTreeInteriorsExits; }

/// Return the staged party and Tret actors.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gTretTreeInteriorsPlacements;
}

/// Stepping on a cracked floor tile: the first step cracks it further and sets
/// its temporary flag; the next step breaks it, and the leader spins and falls
/// through, to land at the given entrance.
static void TretTreeInteriors_StepOnCrackedFloor(s32 floorFlag,
                                                 s32 sourceColumn,
                                                 s32 sourceRow, s32 entrance) {
  s32 destinationColumn;
  s32 destinationRow;
  s32 selectedActorId;
  struct FieldActor *actor;
  s32 spinFrame;

  destinationColumn = (gGameState.x >> 20) + 64;
  destinationRow = gGameState.z >> 20;
  selectedActorId = gGameState.selectedActor;
  actor = Object_GetById(selectedActorId);
  if (entrance == *sLastFloorEntrance)
    return;
  *sLastFloorEntrance = entrance;
  if (FLAG_CHECK(floorFlag) == 0) {
    Map_CopyCells(sourceColumn, sourceRow, 1, 1, destinationColumn,
                  destinationRow);
    FLAG_SET(floorFlag);
    return;
  }
  // On the next touch, replace the tile and spin the actor into the fall.
  *sLastFloorEntrance = -1;
  Map_CopyCells(sourceColumn, sourceRow + 1, 1, 1, destinationColumn,
                destinationRow);
  SE(SE_206);
  EV_BEGIN();
  Event_SetDestination((s32)&SceneId_TretTreeInteriors, entrance);
  OBJ_MODE(selectedActorId, 27);
  Engine_ActorSetSpriteFlags(Object_GetById(selectedActorId), 0);
  Actor_SetAttachedEffect(selectedActorId, 0x101);
  WAIT(30);
  Camera_MoveTo(-1, -1, -1, 0);
  actor->motionFlags = ACTOR_FALLS;
  actor->terrainHeight = -PIXELS(160);
  actor->gravity = 0x8000;
  SE(SE_204);
  WAIT(3);
  actor->terrainId = 2;
  Engine_ActorSetSpritePriority(selectedActorId, 3);
  for (spinFrame = FLOOR_FALL_SPIN_FRAMES - 1; spinFrame >= 0; spinFrame--) {

    actor->facing += 2 * FACING_STEP;
    Engine_TaskWait(1);
  }
  if (entrance != 50)
    FLAG_SET(0x122);
}

/// Cracked floor over the upper middle northwest landing.
static void Event_FloorGivesWayToUpperMiddleNorthwest(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_00, 64, 35, 21);
}

/// Cracked floor over the upper middle northeast landing.
static void Event_FloorGivesWayToUpperMiddleNortheast(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_01, 65, 35, 22);
}

/// Cracked floor over the upper middle west upper landing.
static void Event_FloorGivesWayToUpperMiddleWestUpper(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_02, 66, 35, 23);
}

/// Cracked floor over the upper middle west lower landing.
static void Event_FloorGivesWayToUpperMiddleWestLower(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_03, 67, 35, 24);
}

/// Cracked floor over the upper middle southwest upper landing.
static void Event_FloorGivesWayToUpperMiddleSouthwestUpper(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_04, 68, 35, 25);
}

/// Cracked floor over the upper middle south upper landing.
static void Event_FloorGivesWayToUpperMiddleSouthUpper(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_05, 69, 35, 26);
}

/// Cracked floor over the upper middle southwest lower landing.
static void Event_FloorGivesWayToUpperMiddleSouthwestLower(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_06, 70, 35, 27);
}

/// Cracked floor over the upper middle south lower landing.
static void Event_FloorGivesWayToUpperMiddleSouthLower(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_07, 71, 35, 28);
}

/// Cracked floor over the upper middle southeast lower landing.
static void Event_FloorGivesWayToUpperMiddleSoutheastLower(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_08, 72, 35, 29);
}

/// Cracked floor over the upper right northwest landing.
static void Event_FloorGivesWayToUpperRightNorthwest(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_09, 73, 35, 31);
}

/// Cracked floor over the upper right northeast landing.
static void Event_FloorGivesWayToUpperRightNortheast(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_0A, 74, 35, 32);
}

/// Cracked floor over the lower middle center landing.
static void Event_FloorGivesWayToLowerMiddleCenter(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_0B, 79, 35, 50);
}

/// Cracked floor over the lower middle northwest landing.
static void Event_FloorGivesWayToLowerMiddleNorthwest(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_0C, 75, 35, 51);
}

/// Cracked floor over the lower middle north landing.
static void Event_FloorGivesWayToLowerMiddleNorth(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_0D, 76, 35, 52);
}

/// Cracked floor over the lower middle northeast landing.
static void Event_FloorGivesWayToLowerMiddleNortheast(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_0E, 77, 35, 53);
}

/// Cracked floor over the lower middle west landing.
static void Event_FloorGivesWayToLowerMiddleWestA(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_0F, 78, 35, 54);
}

/// Cracked floor over the lower middle east landing.
static void Event_FloorGivesWayToLowerMiddleEast(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_10, 80, 35, 55);
}

/// A second cracked floor over the same lower middle west landing.
static void Event_FloorGivesWayToLowerMiddleWestB(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_11, 81, 35, 56);
}

/// Cracked floor over the lower middle southwest landing.
static void Event_FloorGivesWayToLowerMiddleSouthwest(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_12, 82, 35, 57);
}

/// Cracked floor over the lower middle south landing.
static void Event_FloorGivesWayToLowerMiddleSouth(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_13, 83, 35, 58);
}

/// Cracked floor over the lower middle southeast landing.
static void Event_FloorGivesWayToLowerMiddleSoutheast(void) {
  TretTreeInteriors_StepOnCrackedFloor(FLAG_SCENE_TEMP_14, 84, 35, 59);
}

/// Walking off a cracked floor tile lets the next step on it count again.
static void Event_ResetFloorLatchWhenClear(void) {
  if (Engine_LeaderCheckAhead() == 0)
    *sLastFloorEntrance = -1;
}

/// Return the floor, story, Reveal and search event bindings.
const struct SceneEvent *Scene_GetEvents(void) {
  return gTretTreeInteriorsEvents;
}

/// Walking up to Tret (touch trigger 40, until FLAG_TRET_DEFEATED): he roars,
/// the party gathers in front of him, and he scorns them before the battle with
/// him starts. "Hm... HROOOM!!!" and "You hoped to find my kindly self?" The
/// battle returns to entrance 19.
static void Event_FightTret(void) {
  s32 hasMia;
  struct FieldActor *leader;

  hasMia = FLAG_CHECK(FLAG_IN_PARTY(MIA));
  EV_BEGIN();
  SE(MUS_017);
  MESSAGE_SET(MsgTretHmHrooom);
  Engine_EventShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  SE(MUS_029);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  OBJ_SPEED(GARET, 0x10000, 0x8000);
  OBJ_SPEED(IVAN, 0x10000, 0x8000);
  OBJ_SPEED(MIA, 0x10000, 0x8000);
  Object_GetById(MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(MIA, 2);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  if (hasMia != 0) {
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Engine_ActorSetPosition(MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetActionScript(MIA, gTretTreeInteriors_TableActions3);
  }
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gTretTreeInteriors_TableActions0);
  Actor_SetActionScript(GARET, gTretTreeInteriors_TableActions1);
  Actor_RunActionScriptAndWait(IVAN, gTretTreeInteriors_TableActions2);
  WAIT(10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 40);
  OBJ_MODE(ACTOR_TRET, 11);
  WAIT(10);
  OBJ_MODE(ACTOR_TRET, 8);
  WAIT(20);
  TretTreeInteriors_PlayGesture(8);
  Event_ShowMessage(SPEAKER(ACTOR_TRET), 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT, 60);
  TretTreeInteriors_PlayGesture(11);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET), 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(MIA, 1);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessage(SPEAKER(ACTOR_TRET), 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_ActorSetAttachedEffect(IVAN, 0x102);
  WAIT(40);
  TretTreeInteriors_PlayGesture(11);
  MESSAGE_PUT(SPEAKER(ACTOR_TRET), 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 64;
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_TretTreeInteriors, 19);
  Field_RequestBattleFromEncounterTable(36, 0);
  EV_END();
}

/// After the battle: a passage opens beside Tret, who comes to, thanks the
/// party and says good-bye. "Oh... Mmmmm..." Garet and Ivan stop him and he
/// asks whether they need something; answering no earns the leader a
/// scolding. Tret learns he turned the people of Kolima into trees, tries to
/// free them in a swirl of sparks but cannot, and the party resolve to heal
/// him. Sets FLAG_TRET_DEFEATED.
static void TretTreeInteriors_PlayPostBattleConversation(void) {
  s32 hasMia;
  struct FieldActor *partyActor;
  s32 openingColumn;
  SchedulerCallback sparkTask;
  const s32 *actions;

  // Restore the party positions before opening the post-battle conversation.
  hasMia = FLAG_CHECK(FLAG_IN_PARTY(MIA));
  Object_GetById(MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(MIA, 2);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  PartyInventory_FindOwner(ITEM_HERMES_WATER);
  SE(MUS_017);
  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  OBJ_SPEED(GARET, 0xcccc, 0x6666);
  OBJ_SPEED(IVAN, 0xcccc, 0x6666);
  OBJ_SPEED(MIA, 0xcccc, 0x6666);
  OBJ_POS_SET(ACTOR_PARTY_LEADER, 166, 80);
  partyActor = Actor_Get(ACTOR_PARTY_LEADER);
  partyActor->facing = FACING_NORTH;
  OBJ_POS_SET(GARET, 148, 90);
  partyActor = Actor_Get(GARET);
  partyActor->facing = FACING_NORTH;
  OBJ_POS_SET(IVAN, 182, 90);
  partyActor = Object_GetById(IVAN);
  partyActor->facing = FACING_NORTH;
  if (hasMia != 0) {
    OBJ_POS_SET(MIA, 166, 104);
    partyActor = Object_GetById(MIA);
    partyActor->facing = FACING_NORTH;
  }
  TretTreeInteriors_PlayGesture(0);
  Engine_TaskWait(10);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 48;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  WAIT(20);
  Camera_SetSpeed(0x13333, 0x2666);
  Camera_MoveTo(PIXELS(168), -1, PIXELS(152), 1);
  Engine_CameraWaitForMove();
  WAIT(10);
  // Open the passage beside Tret before continuing the conversation.
  openingColumn = 10;
  SE(SE_MAP_EXIT);
  Map_CopyCellAttributes(26, 3, 1, 2, openingColumn, 8);
  Map_CopyCells(26, 38, 1, 1, openingColumn, 43);
  Engine_TaskWait(4);
  Map_CopyCells(26, 37, 1, 2, openingColumn, 42);
  Engine_TaskWait(4);
  Map_CopyCells(26, 36, 1, 3, openingColumn, 41);
  Engine_TaskWait(4);
  Map_CopyCells(26, 35, 1, 4, openingColumn, 40);
  Engine_TaskWait(80);
  // Tret comes to: "Oh... Mmmmm... What has happened to me?"
  MESSAGE_SET(MsgTretMmmmm);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  WAIT(20);
  Camera_MoveTo(PIXELS(168), -1, PIXELS(90), 1);
  Engine_CameraWaitForMove();
  WAIT(40);
  TretTreeInteriors_PlayGesture(1);
  WAIT(60);
  SE(MUS_021);
  TretTreeInteriors_PlayGesture(4);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 80);
  Event_ShowMessage(SPEAKER(ACTOR_TRET_SPEAKER), 0);
  WAIT(40);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  TretTreeInteriors_PlayGesture(0);
  WAIT(40);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  WAIT(60);
  // Garet and Ivan will not let Tret die while they still need his help.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  OBJ_DIR(GARET, FACING_NORTHEAST, 10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Event_ShowMessageAndWait(SPEAKER(GARET), 0, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 0);
  Event_ShowMessageAndWait(SPEAKER(IVAN), 0, 20);
  TretTreeInteriors_PlayGesture(0);
  WAIT(40);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 40);
  TretTreeInteriors_PlayGesture(4);
  // "Do you need something?" Answering no: "What are you doing, Isaac! Now's
  // not the time to fool around!"
  Event_OpenMessage(SPEAKER(ACTOR_TRET_SPEAKER), 0);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 20);
    OBJ_MODE(GARET, ANIM_SHAKE_HEAD);
    MESSAGE_SET(MsgTretDoingNowsNot);
    Event_ShowMessage(SPEAKER(GARET), 0);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 3, 10);
    OBJ_MODE(IVAN, ANIM_NOD);
    Event_ShowMessage(SPEAKER(IVAN), 0);
  }
  WAIT(20);
  TretTreeInteriors_PlayGesture(4);
  MESSAGE_SET(MsgTretTurnedPeopleKolima);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 20);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 10);
  TretTreeInteriors_PlayGesture(0);
  WAIT(20);
  // Tret tries to release the people of Kolima: the colours shift and sparks
  // swirl out from his face, then fade.
  ColorBuffer_ApplySource(0x10000, 0);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 20);
  WAIT(20);
  sSparkFrame = 0;
  {
    struct FieldPosition *origin = &sSparkOrigin;
    origin->x = PIXELS(168);
    origin->y = PIXELS(32);
    sparkTask = TretTreeInteriors_SpawnSwirlSparks;
    origin->z = PIXELS(52);
  }
  Task_AddCallback(sparkTask, TASK_PRIORITY_SCENE);
  WAIT(220);
  Engine_TaskRemoveCallback(sparkTask);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  TretTreeInteriors_PlayGesture(4);
  WAIT(20);
  // "I can't do it... My power no longer reaches Kolima..." His face sinks
  // away on "People of Kolima... Forgive me...", and the party resolve to
  // heal him.
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET_SPEAKER), 0, 10);
  TretTreeInteriors_PlayGesture(0);
  Event_ShowMessage(SPEAKER(ACTOR_TRET_SPEAKER), 0);
  Actor_RunActionScriptAndWait(ACTOR_TRET, gTretTreeInteriors_ActionTable1);
  WAIT(40);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessage(SPEAKER(GARET), 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 2, 10);
  Event_ShowMessage(SPEAKER(IVAN), 0);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(SPEAKER(GARET), 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(SPEAKER(IVAN), 0, 10);
  if (hasMia != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 1);
    Engine_EventShowMessageAndWait(SPEAKER(MIA), 0, 10);
  }
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  actions = gTretTreeInteriors_ActionTable2;
  Actor_SetActionScript(GARET, actions);
  if (hasMia != 0) {
    Actor_SetActionScript(MIA, actions);
  }
  Actor_RunActionScriptAndWait(IVAN, actions);
  WAIT(20);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  FLAG_SET(FLAG_TRET_DEFEATED);
  FieldScheduler_AddCallback(TretPalette_UpdateSceneTint, TASK_PRIORITY_SCENE);
  EV_END();
}

/// Restore the post-battle map and optionally play its conversation.
static void TretTreeInteriors_ApplyPostBattleState(s32 playRecoveryScene) {
  if (playRecoveryScene != 0 && FLAG_CHECK(FLAG_KEEP_PARTY_POSITION) == 0)
    TretTreeInteriors_PlayPostBattleConversation();

  Engine_TaskWait(1);
  if (FLAG_CHECK(FLAG_TRET_DEFEATED) != 0) {
    s32 openingColumn = 10;
    Map_CopyCells(121, 34, 3, 1, 93, openingColumn);
    {
      s32 passageColumn = 30;
      Map_CopyCells(46, 38, 1, 1, passageColumn, 43);
      Map_CopyCellAttributes(0, 0, 1, 2, passageColumn, 9);
    }
    Map_CopyCellAttributes(26, 3, 1, 2, openingColumn, 8);
    Map_CopyCells(26, 35, 1, 4, openingColumn, 40);
  } else {
    s32 openingColumn = 10;
    s32 passageColumn = 8;
    Map_CopyCellAttributes(11, 8, 1, 2, openingColumn, passageColumn);
  }
}

/// Restore the room, tint and entry fall before continuing its story event.
s32 TretTreeInteriors_EnterRoom(void) {
  struct FieldActor *tret;
  struct FieldActor *actor;
  struct MapScrollWork *mapWork;
  struct MapLayerScroll *layer;
  struct FieldWork *fieldWork;
  s32 entrance;
  s32 selectedActorId;

  tret = Object_GetById(ACTOR_TRET);
  // Restore scene-local floor state and the saved palette first.
  sLastFloorEntrance = (s16 *)gSceneState;
  TretPalette_SaveSceneColors();
  tret->motionFlags = 0;
  tret->y.fixed = -PIXELS(10);
  {
    struct FieldActor *tretSpeaker = Object_GetById(ACTOR_TRET_SPEAKER);

    tretSpeaker->motionFlags = 0;
    tretSpeaker->y.fixed = -PIXELS(10);
  }
  Engine_ActorSetChildValue(ACTOR_TRET_SPEAKER, 15);
  TretTreeInteriors_PlayGesture(0);
  if (gGameState.entrance != 19)
    FieldScheduler_AddCallback(TretPalette_UpdateSceneTint,
                               TASK_PRIORITY_SCENE);
  if (FLAG_CHECK(FLAG_TRET_DEFEATED)) {
    Engine_ActorSetPosition(ACTOR_TRET_SPEAKER, 0, 0);
    Engine_ActorSetPosition(ACTOR_TRET, 0, 0);
  }
  if (FLAG_CHECK(FLAG_KEEP_PARTY_POSITION))
    TretTreeInteriors_ApplyFlaggedMapPatches();
  mapWork = WORK_SLOT(8);
  // Match the first layer's parallax offset and scale to the room bounds.
  layer = &mapWork->layers[0];
  layer->offsetX += Iwram_MulQ16(mapWork->minX + PIXELS(160), 0x1999);
  layer->offsetY += Iwram_MulQ16(mapWork->minY + PIXELS(136), 0x1999);
  layer->scaleX = 0xe666;
  layer->scaleY = 0xe666;
  // Four floors start out cracked through: the first step on them gives way.
  FLAG_SET(FLAG_SCENE_TEMP_01);
  FLAG_SET(FLAG_SCENE_TEMP_0D);
  FLAG_SET(FLAG_SCENE_TEMP_0F);
  FLAG_SET(FLAG_SCENE_TEMP_13);
  Engine_TaskWait(1);
  TretTreeInteriors_ApplyPostBattleState(0);
  fieldWork = WORK_SLOT(HEAP_SLOT_EVENT);
  fieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  entrance = gGameState.entrance;
  selectedActorId = gGameState.selectedActor;
  actor = Object_GetById(selectedActorId);
  // Entrances 20, 30, 40 and 50: the leader falls on through the floor, to
  // the entrance numbered ten lower. Entrance 10: the leader lands in a cloud
  // of dust.
  // Entrance 19: back from the battle with Tret.
  if (entrance == 50 || entrance == 40 || entrance == 30 || entrance == 20) {
    Engine_EventOpenScreen();
    OBJ_MODE(selectedActorId, 27);
    Engine_ActorSetSpriteFlags(Object_GetById(selectedActorId), 0);
    Actor_SetAttachedEffect(selectedActorId, 0x101);
    Camera_MoveTo(-1, -1, -1, 0);
    actor->motionFlags = ACTOR_FALLS;
    actor->y.fixed = PIXELS(100);
    actor->terrainHeight = -PIXELS(160);
    actor->gravity = 0x8000;
    SE(SE_204);
    Event_SetDestination((s32)&SceneId_TretTreeInteriors, entrance - 10);
    WAIT(20);
    actor->terrainId = 2;
    Engine_ActorSetSpritePriority(selectedActorId, 3);
    WAIT(2);
    Engine_ActorSetAttachedEffect(selectedActorId, 0x100);
    WAIT(8);
  } else if (entrance == 10) {
    if (!FLAG_CHECK(FLAG_KEEP_PARTY_POSITION))
      TretTreeInteriors_RunLandingDustScene();
  } else if (entrance == 19) {
    TretTreeInteriors_ApplyPostBattleState(1);
  }
  return 0;
}

/// Save background palettes 0 to 13 as the base for Tret's scene tint.
static void TretPalette_SaveSceneColors(void) {
  Dma_Set((const void *)BG_PLTT, gPaletteBufferWork->palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | 112, REG_DMA3);
}

/// Tint background colours 16 to 78 every thirty-two frames, leaving
/// palette 4's last colour alone. Each later palette receives a weaker tint.
static void TretPalette_UpdateSceneTint(void) {
  u16 *sourceColors;
  u16 *destinationColors;
  u32 colorIndex;
  s32 redStep;
  s32 greenStep;
  s32 blueStep;
  s32 red;
  s32 green;
  s32 blue;
  u32 color;
  struct FieldWork *fieldWork;
  struct BattleEffectBuffers *paletteWork;

  fieldWork = WORK_SLOT(HEAP_SLOT_EVENT);
  paletteWork = WORK_SLOT(32);
  sourceColors = paletteWork->palette;
  if (fieldWork->command != 0)
    return;
  if ((gFrameCount & 31) != 0)
    return;
  sourceColors += 16;
  destinationColors = (u16 *)0x05000020;
  colorIndex = 0;
  for (; colorIndex < TRET_TINT_COLOR_COUNT; colorIndex++, sourceColors++) {
    redStep = gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex];
    greenStep = gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex + 1];
    blueStep = gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex + 2];
    if (colorIndex > 47) {
      redStep -= redStep / 2 + redStep / 3;
      greenStep -= greenStep / 2 + greenStep / 3;
      blueStep -= blueStep / 2 + blueStep / 3;
    } else if (colorIndex > 31) {
      redStep -= redStep / 3 + redStep / 4;
      greenStep -= greenStep / 3 + greenStep / 4;
      blueStep -= blueStep / 3 + blueStep / 4;
    } else if (colorIndex > 15) {
      redStep -= redStep / 4 + redStep / 5;
      greenStep -= greenStep / 4 + greenStep / 5;
      blueStep -= blueStep / 4 + blueStep / 5;
    }
    color = *sourceColors;
    red = color & 31;
    green = (color >> 5) & 31;
    blue = (color >> 10) & 31;
    red += redStep;
    green += greenStep;
    blue += blueStep;
    if (red > TRET_TINT_CHANNEL_MAX)
      red = TRET_TINT_CHANNEL_MAX;
    if (green > TRET_TINT_CHANNEL_MAX)
      green = TRET_TINT_CHANNEL_MAX;
    if (blue > TRET_TINT_CHANNEL_MAX)
      blue = TRET_TINT_CHANNEL_MAX;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *destinationColors++ = (blue << 10) | (green << 5) | red;
  }
  gTretTreeInteriors_TintIndex += (Engine_RandomNext() & 7) * 3;
  if (gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex] ==
      TRET_TINT_END)
    gTretTreeInteriors_TintIndex = 0;
}

/// Play a Tret animation sequence with its original pauses.
static void TretTreeInteriors_PlayGesture(s32 gesture) {
  switch (gesture) {
  case 0:
    OBJ_MODE(ACTOR_TRET, 1);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 3);
    break;
  case 1:
    OBJ_MODE(ACTOR_TRET, 1);
    break;
  case 2:
    OBJ_MODE(ACTOR_TRET, 1);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 5);
    break;
  case 3:
    OBJ_MODE(ACTOR_TRET, 1);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 4);
    break;
  case 4:
    OBJ_MODE(ACTOR_TRET, 1);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 3);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 1);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 3);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 1);
    break;
  case 5:
    OBJ_MODE(ACTOR_TRET, 1);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 2);
    break;
  case 7:
    OBJ_MODE(ACTOR_TRET, 6);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 8);
    break;
  case 8:
    OBJ_MODE(ACTOR_TRET, 6);
    break;
  case 9:
    OBJ_MODE(ACTOR_TRET, 6);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 9);
    break;
  case 10:
    OBJ_MODE(ACTOR_TRET, 6);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 10);
    break;
  case 11:
    OBJ_MODE(ACTOR_TRET, 6);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 8);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 6);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 8);
    Engine_TaskWait(6);
    OBJ_MODE(ACTOR_TRET, 6);
    break;
  case 12:
    OBJ_MODE(ACTOR_TRET, 6);
    break;
  }
  Engine_TaskWait(12);
}

/// Move a fading spark outward from its saved origin, then release it.
static void TretTreeInteriors_UpdateSwirlSpark(union FieldObject *fieldObject) {
  struct FieldActor *spark = (struct FieldActor *)fieldObject;
  struct FieldPosition position;
  s32 frame;

  frame = spark->timer;
  if (frame <= SWIRL_SPARK_LAST_FRAME) {
    position.x = spark->targetX;
    position.y = spark->targetY;
    position.z = spark->targetZ;
    {
      s32 initialAngle = (s16)spark->work;

      Vector_AddPolarOffset(frame << 16, ((frame * 3) << 8) + initialAngle,
                            &position);
    }
    spark->x.fixed = position.x;
    spark->y.fixed = position.y;
    spark->z.fixed = position.z;
    if (spark->timer <= SWIRL_SPARK_SHRINK_LAST_FRAME) {
      spark->scaleX += -0x51e;
      spark->scaleY += -0x51e;
    }
    spark->timer++;
  } else {
    Resource_ResetEntry(spark->sprite->slot);
    Engine_ObjectDispatchRelease(spark);
  }
}

/// Emit five shrinking waves of fading sparks from the saved origin.
static void TretTreeInteriors_SpawnSwirlSparks(void) {
  struct FieldActor *spark;
  u32 frame;
  void *previousResourceEntry;
  s32 wave;
  u32 sparkIndex;
  s32 *counter;

  counter = &sSparkFrame;
  frame = *counter;
  previousResourceEntry = 0;
  /* FAKEMATCH: the explicit libgcc call keeps the final count's reload
   * in r3. Measured: (s32)frame / 10, or frame declared s32 and
   * divided with /, reload it into r5 (differ 6); the division moved
   * above previousResourceEntry = 0 differs 10. */
  wave = __divsi3(frame, 10);
  switch (frame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    SE(SE_220);
    // Waves contain 6, 5, 4, 3 and 2 sparks. Divide degrees before
    // converting them to turns, preserving the original rounding.
    for (sparkIndex = 0; sparkIndex < 6 - wave; sparkIndex++) {
      spark = Engine_ObjectCreate(0x11d, sSparkOrigin.x, sSparkOrigin.y,
                                  sSparkOrigin.z);
      if (spark != 0) {
        previousResourceEntry =
            Sprite_ShareVramCache(spark->sprite, previousResourceEntry);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 0;
        Engine_ActorSetSpriteFlags(spark, 0);
        Object_SetMode(spark, 1);
        spark->timer = 0;
        spark->work = ((360 / (u32)(6 - wave) * sparkIndex) << 16) / 360;
        spark->targetX = sSparkOrigin.x;
        spark->targetY = sSparkOrigin.y;
        spark->targetZ = sSparkOrigin.z;
        spark->speed = 0x19999;
        spark->update = TretTreeInteriors_UpdateSwirlSpark;
      }
    }
    // Each wave also plays the closing sound.
    // fall through
  case 44:
    SE(SE_289);
    /* FAKEMATCH: the counter pointer is taken again here so it is dead
     * across the spark loop, where the loop index takes its register.
     * Measured: without it, differ 36; with the division also written
     * with /, differ 51. */
    counter = &sSparkFrame;
    break;
  }
  if (++*counter > 120) {
    *counter = 0;
  }
}

/// Restore each enabled single-cell patch whose puzzle flag is set.
static void TretTreeInteriors_ApplyFlaggedMapPatches(void) {
  s32 patchOffset;

  for (patchOffset = 0; gTretTreeInteriors_MapPatches[patchOffset] != -1;
       patchOffset += 6) {
    if (FLAG_CHECK(gTretTreeInteriors_MapPatches[patchOffset]) &&
        gTretTreeInteriors_MapPatches[patchOffset + 1] != 0)
      Map_CopyCellsTo(gTretTreeInteriors_MapPatches[patchOffset + 2],
                      gTretTreeInteriors_MapPatches[patchOffset + 3],
                      gTretTreeInteriors_MapPatches[patchOffset + 4],
                      gTretTreeInteriors_MapPatches[patchOffset + 5], 1, 1);
  }
}

/// Stop the scene tint while Reveal is active.
static void Event_CastReveal(void) {
  Engine_TaskRemoveCallback(TretPalette_UpdateSceneTint);
}

// Shared source is included here in ROM order.
#include "common/effect_advance.h"

/// Drop the leader into the room with a ring of landing particles.
static void TretTreeInteriors_RunLandingDustScene(void) {
  struct EffectOptions options;
  struct FieldPosition velocity;
  struct EffectOptions *particleOptions;
  struct FieldPosition *particleVelocity;
  struct FieldActor *leader;
  u32 particleIndex;
  s32 velocityX;
  s32 velocityZ;

  EV_BEGIN();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = PIXELS(130);
  Object_GetById(ACTOR_PARTY_LEADER)->gravity = 0x8000;
  Object_GetById(ACTOR_PARTY_LEADER)->bounce = 0;
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  SE(SE_204);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  WAIT(24);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Spread particles around the leader while the landing effect finishes.
  particleOptions = &options;
  particleOptions->palette = 7;
  particleOptions->update = Effect_AdvanceMotion;
  particleOptions->startScaleX = 0xcccc;
  particleOptions->startScaleY = 0xcccc;
  for (particleIndex = 0; particleIndex <= 16; particleIndex++) {
    particleVelocity = &velocity;
    particleVelocity->x = Engine_MathCos(particleIndex << 12);
    particleVelocity->y = 0;
    velocityZ = Engine_MathSin(particleIndex << 12);
    velocityX = particleVelocity->x;
    particleVelocity->z = velocityZ;
    velocityX += velocityX / 2;
    particleVelocity->x = velocityX;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocityX,
                 particleVelocity->y, velocityZ,
                 EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                     EFFECT_USE_PALETTE | 1,
                 particleOptions);
  }
  SE(SE_188);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
  OBJ_MODE(ACTOR_PARTY_LEADER, 22);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapWaitWorkValuesBelow256();
  Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
  Object_GetById(ACTOR_PARTY_LEADER)->gravity = 0x10000;
  Object_GetById(ACTOR_PARTY_LEADER)->bounce = 0x4000;
  EV_END();
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// The cracked floors redrawn when the room is restored: floor flag, whether
/// it is redrawn (not the four that start cracked), source cell x/y,
/// destination cell x/y.
// clang-format off
s16 gTretTreeInteriors_MapPatches[] = {
    FLAG_SCENE_TEMP_00, 1, 64, 35, 113, 6,
    FLAG_SCENE_TEMP_01, 0, 65, 35, 115, 6,
    FLAG_SCENE_TEMP_02, 1, 66, 35, 110, 8,
    FLAG_SCENE_TEMP_03, 1, 67, 35, 110, 10,
    FLAG_SCENE_TEMP_04, 1, 68, 35, 111, 12,
    FLAG_SCENE_TEMP_05, 1, 69, 35, 113, 12,
    FLAG_SCENE_TEMP_06, 1, 70, 35, 112, 14,
    FLAG_SCENE_TEMP_07, 1, 71, 35, 114, 14,
    FLAG_SCENE_TEMP_08, 1, 72, 35, 116, 14,
    FLAG_SCENE_TEMP_09, 1, 73, 35, 71, 23,
    FLAG_SCENE_TEMP_0A, 1, 74, 35, 77, 24,
    FLAG_SCENE_TEMP_0C, 1, 75, 35, 112, 24,
    FLAG_SCENE_TEMP_0D, 0, 76, 35, 114, 24,
    FLAG_SCENE_TEMP_0E, 1, 77, 35, 116, 24,
    FLAG_SCENE_TEMP_0F, 0, 78, 35, 112, 26,
    FLAG_SCENE_TEMP_0B, 1, 79, 35, 114, 26,
    FLAG_SCENE_TEMP_10, 1, 80, 35, 116, 26,
    FLAG_SCENE_TEMP_11, 1, 81, 35, 112, 28,
    FLAG_SCENE_TEMP_12, 1, 82, 35, 114, 28,
    FLAG_SCENE_TEMP_13, 0, 83, 35, 116, 28,
    FLAG_SCENE_TEMP_14, 1, 84, 35, 118, 28,
    -1, 0,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gTretTreeInteriors_TableActions0[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(80)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTretTreeInteriors_TableActions1[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(148), 0, PIXELS(90)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTretTreeInteriors_TableActions2[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(182), 0, PIXELS(90)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTretTreeInteriors_TableActions3[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(104)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTretTreeInteriors_ActionTable1[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(80)),
    SC_WAIT(4),
    SC_ADD(SW_Y, -PIXELS(78)),
    SC_ADD(SW_SCALE_X, -0x1000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(4),
    SC_LOOP(8, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gTretTreeInteriors_ActionTable2[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(80)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gTretTreeInteriors_TintSteps[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1,
    1, 2, 1, 1, 2, 1, 1, 2,
    1, 1, 2, 2, 1, 2, 2, 1,
    2, 2, 2, 3, 2, 2, 3, 2,
    2, 3, 3, 2, 3, 3, 2, 3,
    3, 2, 3, 3, 2, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 4, 3, 3, 4,
    3, 3, 4, 3, 3, 4, 3, 3,
    4, 3, 3, 4, 3, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 3, 4,
    4, 3, 4, 4, 3, 4, 4, 3,
    4, 4, 3, 4, 4, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 3, 4, 4, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 3, 4,
    4, 3, 4, 4, 3, 4, 4, 3,
    4, 4, 3, 4, 4, 3, 4, 3,
    3, 4, 3, 3, 4, 3, 3, 4,
    3, 3, 4, 3, 3, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 2, 3, 3, 2, 3, 3, 2,
    3, 3, 2, 3, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 1, 2,
    2, 1, 2, 1, 1, 2, 1, 1,
    2, 1, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99,
};

struct SceneEntrance gTretTreeInteriorsEntrances[] = {
    { 0, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 488, 0, 248, FACING_NORTH, 0, 352, 0, 640, 272, 0 },
    { 2, CONDITION_ALWAYS, 712, 0, 152, FACING_EAST, 0, 672, 0, 944, 272, 0 },
    { 3, CONDITION_ALWAYS, 920, 0, 152, FACING_WEST, 0, 672, 0, 944, 272, 0 },
    { 4, CONDITION_ALWAYS, 168, 0, 520, FACING_NORTH, 0, 32, 272, 304, 544, 0 },
    { 5, CONDITION_ALWAYS, 296, 0, 424, FACING_WEST, 0, 32, 272, 304, 544, 0 },
    { 6, CONDITION_ALWAYS, 376, 0, 424, FACING_EAST, 0, 352, 272, 640, 544, 0 },
    { 7, CONDITION_ALWAYS, 488, 0, 520, FACING_NORTH, 0, 352, 272, 640, 544, 0 },
    { 8, CONDITION_ALWAYS, 712, 0, 424, FACING_EAST, 0, 672, 272, 960, 544, 0 },
    { 9, CONDITION_ALWAYS, 920, 0, 424, FACING_WEST, 0, 672, 272, 960, 544, 0 },
    { 10, CONDITION_ALWAYS, 168, 0, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { 11, CONDITION_ALWAYS, 424, 24, 72, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 12, CONDITION_ALWAYS, 744, 0, 88, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 13, CONDITION_ALWAYS, 232, 24, 344, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { 14, CONDITION_ALWAYS, 552, 0, 360, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 15, CONDITION_ALWAYS, 168, 24, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { 16, CONDITION_ALWAYS, 488, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 21, CONDITION_ALWAYS, 472, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 22, CONDITION_ALWAYS, 504, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 23, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 24, CONDITION_ALWAYS, 424, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 25, CONDITION_ALWAYS, 440, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 26, CONDITION_ALWAYS, 488, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 27, CONDITION_ALWAYS, 456, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 28, CONDITION_ALWAYS, 488, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 29, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 31, CONDITION_ALWAYS, 760, 0, 104, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 32, CONDITION_ALWAYS, 856, 0, 120, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 51, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 52, CONDITION_ALWAYS, 488, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 53, CONDITION_ALWAYS, 520, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 54, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 55, CONDITION_ALWAYS, 520, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 56, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 57, CONDITION_ALWAYS, 488, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 58, CONDITION_ALWAYS, 520, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 59, CONDITION_ALWAYS, 552, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 50, CONDITION_ALWAYS, 488, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 40, CONDITION_ALWAYS, 168, 0, 424, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { 30, CONDITION_ALWAYS, 808, 0, 152, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 20, CONDITION_ALWAYS, 488, 0, 152, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 19, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { SCENE_TABLE_END },
};

struct SceneRegion gTretTreeInteriorsRegions[] = {
    { 420, 28, 68, 428, 36, 76, -1, 10 },
    { 228, 28, 338, 236, 36, 346, -1, 12 },
#if !TBS_PATCH_3
    { 163, 28, 148, 171, 36, 156, -1, 14 },
#else
    { 159, 24, 144, 175, 40, 160, -1, 14 },
#endif
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gTretTreeInteriorsExits[] = {
    SCENE_EXITS(45),
    SCENE_EXIT(1, 44, 2),
    SCENE_EXIT(2, 46, 1),
    SCENE_EXIT(3, 46, 2),
    SCENE_EXIT(4, 46, 3),
    SCENE_EXIT(5, 46, 4),
    SCENE_EXIT(6, 46, 6),
    SCENE_EXIT(7, 46, 5),
    SCENE_EXIT(8, 46, 7),
    SCENE_EXIT(9, 46, 8),
    SCENE_EXIT(10, 45, 12),
    SCENE_EXIT(11, 45, 11),
    SCENE_EXIT(12, 45, 14),
    SCENE_EXIT(13, 45, 13),
    SCENE_EXIT(14, 45, 16),
    SCENE_EXIT(15, 45, 15),
    SCENE_EXITS_END,
};

struct ScenePlacement gTretTreeInteriorsPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x37, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x47, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gTretTreeInteriorsEvents[] = {
    {0x4401, 1, CONDITION_ALWAYS, 1},
    {0x8401, 2, CONDITION_ALWAYS, 2},
    {0x401, 3, CONDITION_ALWAYS, 3},
    {0x4401, 4, CONDITION_ALWAYS, 4},
    {0x401, 5, CONDITION_ALWAYS, 5},
    {0x8401, 6, CONDITION_ALWAYS, 6},
    {0x4401, 7, CONDITION_ALWAYS, 7},
    {0x8401, 8, CONDITION_ALWAYS, 8},
    {0x401, 9, CONDITION_ALWAYS, 9},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
    {EVENT_EXIT, 15, CONDITION_ALWAYS, 15},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 23, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 26, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 27, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 28, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 29, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 32, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 55, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 56, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 57, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 58, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 59, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ResetFloorLatchWhenClear)},
    {EVENT_TOUCH, 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleNorthwest)},
    {EVENT_TOUCH, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleNortheast)},
    {EVENT_TOUCH, 23, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleWestUpper)},
    {EVENT_TOUCH, 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleWestLower)},
    {EVENT_TOUCH, 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleSouthwestUpper)},
    {EVENT_TOUCH, 26, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleSouthUpper)},
    {EVENT_TOUCH, 27, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleSouthwestLower)},
    {EVENT_TOUCH, 28, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleSouthLower)},
    {EVENT_TOUCH, 29, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperMiddleSoutheastLower)},
    {EVENT_TOUCH, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperRightNorthwest)},
    {EVENT_TOUCH, 32, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToUpperRightNortheast)},
    {EVENT_TOUCH, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleCenter)},
    {EVENT_TOUCH, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleNorthwest)},
    {EVENT_TOUCH, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleNorth)},
    {EVENT_TOUCH, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleNortheast)},
    {EVENT_TOUCH, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleWestA)},
    {EVENT_TOUCH, 55, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleEast)},
    {EVENT_TOUCH, 56, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleWestB)},
    {EVENT_TOUCH, 57, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleSouthwest)},
    {EVENT_TOUCH, 58, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleSouth)},
    {EVENT_TOUCH, 59, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_FloorGivesWayToLowerMiddleSoutheast)},
    {EVENT_TOUCH, 40, FLAG_TRET_DEFEATED, EVENT_SCRIPT(Event_FightTret)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_CastReveal)},
    // Item 0x109 is the Healing Ring.
    {SEARCH(SEARCH_CHEST), 100, 0xf5b, FIND_ITEM(ITEM_HEALING_RING)},
    {SEARCH(SEARCH_CHEST), 101, 0xf5c, FIND_ITEM(ITEM_NUT)},
    {SEARCH(SEARCH_UNNAMED), 102, 0x350, FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};

s32 gTretTreeInteriors_TintIndex = 0;
