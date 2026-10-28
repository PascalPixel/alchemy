/// @file
/// Tree rooms (SceneId_TreeRooms), a sheet of rooms drawn inside a tree: the
/// inside of Tret, the holy tree of Kolima Forest, and the first of Tret Tree's
/// two scenes. The party comes in from Kolima Forest (kolima_forest_tree.c) to
/// face Tret's wrathful side. FLAG_TRET_DEFEATED, whether that battle is won
/// and Tret has spoken with the party since, decides most of the scene.
///
/// The map is six rooms, the six camera boxes of its entrance table.
/// The party comes in from Kolima Forest (SceneId_KolimaForestTree) at
/// TREE_ROOMS_FROM_KOLIMA_FOREST_TREE and leaves by
/// TREE_ROOMS_TO_KOLIMA_FOREST_TREE; eight exits, from
/// TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST to _NORTH_NORTHEAST,
/// lead out to Tret Tree's other scene (SceneId_TretTreeBranch), and six more,
/// TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR to
/// TREE_ROOMS_ENTRANCE_TO_TRET, lead from room to room, three of them
/// through regions 28 to 36 pixels up (Scene_GetExitRegions).
///
/// Webs hold cocoons, each a step trigger (metatile behaviors 21 to 29, 31, 32
/// and 50 to 59) with a temporary flag of its own
/// (FLAG_TRET_TREE_SECOND_FLOOR_COCOON_1_TORN and the others). The first step
/// on a cocoon tears it: its cell is redrawn and its flag set. The next step on
/// a torn cocoon breaks through, and the leader spins and falls to the room
/// below, arriving at the entrance numbered as the cocoon
/// (FLAG_ARRIVE_FALLING plays the landing): the cocoons 21 to 29 and 31 and 32
/// drop the leader into one room and 50 to 59 into another of the map's six
/// rooms. The layout draws four cocoons already
/// torn (22, 52, 54 and 58) and the setup sets their flags, so they give way at
/// the first step. A latch in the scene state (sLastCocoon) keeps a step from
/// counting twice until the leader walks on. Cocoon 50 is the way down to Tret:
/// entrances 50, 40, 30 and 20 each let the leader fall on to the
/// entrance numbered ten lower, and at entrance 10 the leader lands
/// in a ring of dust as the map shakes, in front of Tret. Torn cocoons stay
/// torn across a battle: after one (FLAG_KEEP_PARTY_POSITION) the setup redraws
/// them from gTreeRooms_MapPatches.
///
/// Tret's face (ACTOR_TRET) and a second Tret actor at the same spot
/// speak his lines. Walking up to him (metatile behavior 40, while
/// FLAG_TRET_DEFEATED is clear) runs TreeRooms_FightTret: he roars, the party
/// gathers in front of him, and the evil within him scorns them before the
/// battle with Tret. The battle returns to
/// TREE_ROOMS_AFTER_FIGHT_TRET, where the setup plays the conversation
/// after it: a passage opens in his room, he comes to himself, thanks the party
/// and asks whether they need something, realises that he turned the people of
/// Kolima into trees ("I turned the people of Kolima into trees?"), tries to
/// free them but cannot reach that far, and his face sinks away as the party
/// resolve to heal him. Only then is FLAG_TRET_DEFEATED set; in Kolima Forest
/// it lets Hermes' Water heal Tret and lift the curse. Once it is set, the
/// setup moves both Tret actors away, opens that passage and redraws
/// cells near entrance 20; before, it keeps the passage closed.
///
/// The setup also saves the background palettes and starts a slow tint that
/// shifts them every 32 frames (not after the battle until the conversation
/// ends; casting Reveal stops it), and scales the first layer of the map grid
/// to the room. Two chests hold a Healing Ring and a Nut, and a Psynergy Stone
/// lies here (examine triggers 100 to 102).

#include "speaker.h"
#include "version.h"
#include "adventure.h"
#include "scene_ids.h"
#include "unit/scene/tree_rooms.h"
#include "unit/scene/shared/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"

#include "actor.h"
#include "types.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "create_configured_actor.h"
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
#include "heap_slots.h"
#include "iwram_call.h"
#include "dma.h"
#include "event_inline.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "heap.h"
#include "field_effect.h"
#include "flags.h"
#include "character_ids.h"
#include "map_grid_scroll.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// Tret's face (SPRITE_TRET), the first actor placed after the party,
/// and a second Tret actor (SPRITE_TRET_RAISED) placed at the same spot.
/// Both speak Tret's lines: the face as he attacks, the second after the
/// battle.
enum {
  /// ★ Named from the game: "Tret was once a kindly forest king."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  ACTOR_TRET = ACTOR_FIRST_PLACED,
  /// Draws nothing the render shows beyond Tret's face.
  /// ★ Named from the game: "We can't let Tret die like this!" (MsgTretMmmmm)
  ACTOR_SECOND_TRET
};

/// The speaker of a message; every speaker in this scene has
/// SPEAKER_WINDOW_BELOW.
#define SPEAKER(actorId) (SPEAKER_WINDOW_BELOW | (actorId))

// The scene's temporary flags: FLAG_TRET_TREE_SECOND_FLOOR_COCOON_1_TORN and
// the others are set the first time each cocoon is stepped on, tearing it; the
// next step breaks through (gTreeRooms_MapPatches lists them).

#define COCOON_FALL_SPIN_FRAMES 30
#define SWIRL_SPARK_LAST_FRAME 79
#define SWIRL_SPARK_SHRINK_LAST_FRAME 39
#define TRET_TINT_COLOR_COUNT 63
#define TRET_TINT_CHANNEL_MAX 31
#define TRET_TINT_END 99

static void TretPalette_SaveMapColors(void);
static void TretPalette_UpdateMapTint(void);
/// The face gestures TreeRooms_PlayGesture plays on Tret, each from
/// the face with its eyes open; the wrathful ones on his wrathful face.
enum TreeRoomsGesture {
  TRET_GESTURE_CLOSE_EYES = 0,
  TRET_GESTURE_OPEN_EYES = 1,
  TRET_GESTURE_LOOK_LEFT = 2,
  TRET_GESTURE_LOOK_RIGHT = 3,
  TRET_GESTURE_BLINK = 4, ///< twice
  TRET_GESTURE_HALF_CLOSE_EYES = 5,
  TRET_GESTURE_WRATHFUL_CLOSE_EYES = 7,
  TRET_GESTURE_WRATHFUL_OPEN_EYES = 8,
  TRET_GESTURE_WRATHFUL_LOOK_RIGHT = 9,
  TRET_GESTURE_WRATHFUL_LOOK_LEFT = 10,
  TRET_GESTURE_WRATHFUL_BLINK = 11, ///< twice
  TRET_GESTURE_WRATHFUL_STARE = 12  ///< as TRET_GESTURE_WRATHFUL_OPEN_EYES
};

static void TreeRooms_PlayGesture(s32 gesture);
static void TreeRooms_SpawnSwirlSparks(void);
static void TreeRooms_ApplyFlaggedMapPatches(void);
static void TreeRooms_RunLandingDustEvent(void);

static struct FieldPosition sSparkOrigin;
static s32 sSparkFrame;
static s16 *sLastCocoon;

#include "shared/effect_create.h"

#include "shared/effect_spawn.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"

/// Cocoon 21 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_1_TORN FLAG_SCENE_TEMP_00
/// Cocoon 22 has been torn by a first step, so the next step on it drops the
/// leader to the room below (the layout draws it torn, and the setup sets this
/// flag).
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_2_TORN FLAG_SCENE_TEMP_01
/// Cocoon 23 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_3_TORN FLAG_SCENE_TEMP_02
/// Cocoon 24 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_4_TORN FLAG_SCENE_TEMP_03
/// Cocoon 25 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_5_TORN FLAG_SCENE_TEMP_04
/// Cocoon 26 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_6_TORN FLAG_SCENE_TEMP_05
/// Cocoon 27 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_7_TORN FLAG_SCENE_TEMP_06
/// Cocoon 28 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_8_TORN FLAG_SCENE_TEMP_07
/// Cocoon 29 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_SECOND_FLOOR_COCOON_9_TORN FLAG_SCENE_TEMP_08
/// Cocoon 31 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_THIRD_FLOOR_WEST_COCOON_TORN FLAG_SCENE_TEMP_09
/// Cocoon 32 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_THIRD_FLOOR_EAST_COCOON_TORN FLAG_SCENE_TEMP_0A
/// Cocoon 50, the way down to Tret, has been torn by a first step, so the next
/// step on it drops the leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_COCOON_TORN FLAG_SCENE_TEMP_0B
/// Cocoon 51 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_COCOON_TORN FLAG_SCENE_TEMP_0C
/// Cocoon 52 has been torn by a first step, so the next step on it drops the
/// leader to the room below (the layout draws it torn, and the setup sets this
/// flag).
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_COCOON_TORN FLAG_SCENE_TEMP_0D
/// Cocoon 53 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_COCOON_TORN FLAG_SCENE_TEMP_0E
/// Cocoon 54 has been torn by a first step, so the next step on it drops the
/// leader to the room below (the layout draws it torn, and the setup sets this
/// flag).
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_WEST_COCOON_TORN FLAG_SCENE_TEMP_0F
/// Cocoon 55 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_EAST_COCOON_TORN FLAG_SCENE_TEMP_10
/// Cocoon 56 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_COCOON_TORN FLAG_SCENE_TEMP_11
/// Cocoon 57 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_COCOON_TORN FLAG_SCENE_TEMP_12
/// Cocoon 58 has been torn by a first step, so the next step on it drops the
/// leader to the room below (the layout draws it torn, and the setup sets this
/// flag).
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_COCOON_TORN FLAG_SCENE_TEMP_13
/// Cocoon 59 has been torn by a first step, so the next step on it drops the
/// leader to the room below.
/// ★ Named from what the player sees: golden cocoons on a spider web stretched
/// across the floor (its render)
#define FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_COCOON_TORN FLAG_SCENE_TEMP_14

/// Returns the scene's entrances, the cocoons' landing spots among them.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTreeRoomsEntrances;
}

/// Returns the three exit regions 28 to 36 pixels up: up to the second floor
/// (TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR), up to the fourth
/// (TREE_ROOMS_THIRD_FLOOR_TO_FOURTH_FLOOR) and back from Tret
/// (TREE_ROOMS_TRET_TO_ENTRANCE).
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gTreeRoomsRegions;
}

const u32 *Scene_GetExits(void) { return gTreeRoomsExits; }

/// Returns the party's other slots (Garet, Ivan and, once she has joined,
/// Mia), placed for the events, and the two Tret actors.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  return gTreeRoomsActorTemplates;
}

/// Stepping on a cocoon in Tret Tree: the first step tears it, copying its
/// torn picture from the staging cell (row 35) and setting its temporary flag;
/// the next step breaks through (row 36's cell), and the leader spins and
/// falls, to land at the given entrance, numbered as the cocoon. The
/// latch ignores the same cocoon again until the leader walks on.
static void TreeRooms_StepOnCocoon(s32 cocoonFlag, s32 sourceColumn,
                                   s32 sourceRow, s32 entrance) {
  s32 destinationColumn;
  s32 destinationRow;
  s32 leaderActorId;
  struct Actor *actor;
  s32 spinFrame;

  destinationColumn = (gAdventure.x >> 20) + 64;
  destinationRow = gAdventure.z >> 20;
  leaderActorId = gAdventure.leaderActorId;
  actor = Actor_GetById(leaderActorId);
  if (entrance == *sLastCocoon)
    return;
  *sLastCocoon = entrance;
  if (GameFlag_Get(cocoonFlag) == 0) {
    MapGrid_CopyCells(sourceColumn, sourceRow, 1, 1, destinationColumn,
                      destinationRow);
    GameFlag_Set(cocoonFlag);
    return;
  }
  // On the next touch, replace the cell and spin the actor into the
  // fall.
  *sLastCocoon = -1;
  MapGrid_CopyCells(sourceColumn, sourceRow + 1, 1, 1, destinationColumn,
                    destinationRow);
  Event_PlaySong(SE_CURSE_FALL);
  Event_Start();
  Event_SetDestination((s32)&SceneId_TreeRooms, entrance);
  Import_ActorSetAnimation(leaderActorId, ANIM_LEADER_SPIN_AWAY);
  Actor_SetSpriteFlags(Actor_GetById(leaderActorId), 0);
  Actor_SetAttachedEffect(leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Event_WaitFrames(30);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  actor->motionFlags = ACTOR_FALLS;
  actor->groundHeight = -PIXELS(160);
  actor->gravity = PIXELS(1) / 2;
  Event_PlaySong(SE_DUST_PUFF);
  Event_WaitFrames(3);
  actor->layer = 2;
  Actor_SetSpritePriority(leaderActorId, 3);
  for (spinFrame = COCOON_FALL_SPIN_FRAMES - 1; spinFrame >= 0; spinFrame--) {

    actor->facing += 2 * FACING_STEP;
    WaitFrames(1);
  }
  if (entrance != TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_COCOON)
    GameFlag_Set(FLAG_ARRIVE_FALLING);
}

// The cocoons' step triggers in Tret Tree, one per cocoon: each passes its
// flag, the staging cell of its torn picture and its own number, the landing
// entrance, to TreeRooms_StepOnCocoon. The second floor's nine
// are alike, on one web, and keep their numbers 1 to 9 in the order of their
// step triggers and landing entrances, 21 to 29.

static void TreeRooms_StepOnSecondFloorCocoon1(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_1_TORN, 64, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_1);
}

static void TreeRooms_StepOnSecondFloorCocoon2(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_2_TORN, 65, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_2);
}

static void TreeRooms_StepOnSecondFloorCocoon3(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_3_TORN, 66, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_3);
}

static void TreeRooms_StepOnSecondFloorCocoon4(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_4_TORN, 67, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_4);
}

static void TreeRooms_StepOnSecondFloorCocoon5(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_5_TORN, 68, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_5);
}

static void TreeRooms_StepOnSecondFloorCocoon6(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_6_TORN, 69, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_6);
}

static void TreeRooms_StepOnSecondFloorCocoon7(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_7_TORN, 70, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_7);
}

static void TreeRooms_StepOnSecondFloorCocoon8(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_8_TORN, 71, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_8);
}

static void TreeRooms_StepOnSecondFloorCocoon9(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_9_TORN, 72, 35,
                         TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_9);
}

static void TreeRooms_StepOnThirdFloorWestCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_THIRD_FLOOR_WEST_COCOON_TORN, 73, 35,
                         TREE_ROOMS_FROM_THIRD_FLOOR_WEST_COCOON);
}

static void TreeRooms_StepOnThirdFloorEastCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_THIRD_FLOOR_EAST_COCOON_TORN, 74, 35,
                         TREE_ROOMS_FROM_THIRD_FLOOR_EAST_COCOON);
}

static void TreeRooms_StepOnFifthFloorCenterCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_COCOON_TORN, 79, 35,
                         TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_COCOON);
}

static void TreeRooms_StepOnFifthFloorNorthwestCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_COCOON_TORN, 75,
                         35, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_COCOON);
}

static void TreeRooms_StepOnFifthFloorNorthCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_COCOON_TORN, 76, 35,
                         TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_COCOON);
}

static void TreeRooms_StepOnFifthFloorNortheastCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_COCOON_TORN, 77,
                         35, TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_COCOON);
}

static void TreeRooms_StepOnFifthFloorWestCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_WEST_COCOON_TORN, 78, 35,
                         TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_COCOON);
}

static void TreeRooms_StepOnFifthFloorEastCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_EAST_COCOON_TORN, 80, 35,
                         TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_COCOON);
}

/// Cocoon 56 lands at the same spot as cocoon 54: their
/// entrances agree.
static void TreeRooms_StepOnFifthFloorSouthwestCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_COCOON_TORN, 81,
                         35, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_COCOON);
}

static void TreeRooms_StepOnFifthFloorSouthCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_COCOON_TORN, 82, 35,
                         TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_COCOON);
}

static void TreeRooms_StepOnFifthFloorSoutheastCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_COCOON_TORN, 83,
                         35, TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_COCOON);
}

static void TreeRooms_StepOnFifthFloorFarSoutheastCocoon(void) {
  TreeRooms_StepOnCocoon(FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_COCOON_TORN,
                         84, 35,
                         TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_COCOON);
}

/// Jumping on from a cocoon in Tret Tree (Field_TryJumpForward) clears the
/// latch, so the next step on a cocoon counts again.
static void TreeRooms_JumpFromCocoon(void) {
  if (Field_TryJumpForward() == 0)
    *sLastCocoon = -1;
}

/// Returns the exits, the cocoons, Tret, Reveal and the chests' events.
const struct Trigger *Scene_GetTriggers(void) { return gTreeRoomsTriggers; }

/// Walking up to Tret in Tret Tree (metatile behavior 40, while
/// FLAG_TRET_DEFEATED is clear): he roars, the party gathers in front of him,
/// and the evil within him scorns them before the battle with Tret starts.
/// "Hm... HROOOM!!!" and "You hoped to find my kindly self?" The battle returns
/// to TREE_ROOMS_AFTER_FIGHT_TRET.
static void TreeRooms_FightTret(void) {
  s32 hasMia;
  struct Actor *leader;

  hasMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Event_Start();
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Event_SetMessageId(MsgTretHmHrooom);
  Event_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Event_PlaySong(MUS_CONFRONTATION);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_GetById(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MIA, 2);
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Import_ActorSetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Import_ActorSetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  if (hasMia != 0) {
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Import_ActorSetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetScript(ACTOR_MIA, gTreeRooms_MiaGathersActions);
  }
  Actor_SetScript(ACTOR_PARTY_LEADER, gTreeRooms_LeaderGathersActions);
  Actor_SetScript(ACTOR_GARET, gTreeRooms_GaretGathersActions);
  Actor_RunScriptAndWait(ACTOR_IVAN, gTreeRooms_IvanGathersActions);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
  Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_TURNING_WRATHFUL);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
  Event_WaitFrames(20);
  TreeRooms_PlayGesture(TRET_GESTURE_WRATHFUL_OPEN_EYES);
  Event_ShowMessage(SPEAKER(ACTOR_TRET), 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  TreeRooms_PlayGesture(TRET_GESTURE_WRATHFUL_BLINK);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_TRET), 0, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_MIA, 1);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_ShowMessage(SPEAKER(ACTOR_TRET), 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEffect_SetAttached(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(40);
  TreeRooms_PlayGesture(TRET_GESTURE_WRATHFUL_BLINK);
  Event_ShowMessageAndWait(SPEAKER(ACTOR_TRET), 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  gFieldWork->transitionFrames = 64;
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_TreeRooms,
                                  TREE_ROOMS_AFTER_FIGHT_TRET);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        0);
  Event_End();
}

/// After the battle with Tret, in Tret Tree: a passage opens in his room, and
/// he comes to, thanks the party and says good-bye. "Oh... Mmmmm..." Garet and
/// Ivan stop him and he asks whether they need something; answering no earns
/// the leader a scolding. Tret realises that he turned the people of Kolima
/// into trees, tries to free them in a swirl of sparks but cannot, and the
/// party resolve to heal him. Sets FLAG_TRET_DEFEATED, which lets Hermes'
/// Water heal him in Kolima Forest.
static void TreeRooms_PlayPostBattleConversation(void) {
  s32 hasMia;
  struct Actor *partyActor;
  s32 openingColumn;
  TaskFunc sparkTask;
  const s32 *actions;

  // Restore the party positions before opening the post-battle conversation.
  hasMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Actor_GetById(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_MIA, 2);
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  PartyInventory_FindMember(ITEM_HERMES_WATER);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_SetSpeed(ACTOR_GARET, 0xcccc, 0x6666);
  Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
  Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(166), PIXELS(80));
  partyActor = Actor_Get(ACTOR_PARTY_LEADER);
  partyActor->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_GARET, PIXELS(148), PIXELS(90));
  partyActor = Actor_Get(ACTOR_GARET);
  partyActor->facing = FACING_NORTH;
  Actor_SetPosition(ACTOR_IVAN, PIXELS(182), PIXELS(90));
  partyActor = Actor_GetById(ACTOR_IVAN);
  partyActor->facing = FACING_NORTH;
  if (hasMia != 0) {
    Actor_SetPosition(ACTOR_MIA, PIXELS(166), PIXELS(104));
    partyActor = Actor_GetById(ACTOR_MIA);
    partyActor->facing = FACING_NORTH;
  }
  TreeRooms_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  WaitFrames(10);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 48;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(152), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(10);
  // Open the passage in Tret's room before continuing the conversation.
  openingColumn = 10;
  Event_PlaySong(SE_MAP_EXIT);
  MapGrid_CopyCellAttributes(26, 3, 1, 2, openingColumn, 8);
  MapGrid_CopyCells(26, 38, 1, 1, openingColumn, 43);
  WaitFrames(4);
  MapGrid_CopyCells(26, 37, 1, 2, openingColumn, 42);
  WaitFrames(4);
  MapGrid_CopyCells(26, 36, 1, 3, openingColumn, 41);
  WaitFrames(4);
  MapGrid_CopyCells(26, 35, 1, 4, openingColumn, 40);
  WaitFrames(80);
  // Tret comes to: "Oh... Mmmmm... What has happened to me?"
  Event_SetMessageId(MsgTretMmmmm);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(90), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(40);
  TreeRooms_PlayGesture(TRET_GESTURE_OPEN_EYES);
  Event_WaitFrames(60);
  Event_PlaySong(MUS_FOREST);
  TreeRooms_PlayGesture(TRET_GESTURE_BLINK);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_ShowMessage(SPEAKER(ACTOR_SECOND_TRET), 0);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  TreeRooms_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  // Garet and Ivan will not let Tret die while they still need his help.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_GARET), 0, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_IVAN), 0, 20);
  TreeRooms_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
  TreeRooms_PlayGesture(TRET_GESTURE_BLINK);
  // "Do you need something?" Answering no: "What are you doing, Isaac! Now's
  // not the time to fool around!"
  Event_OpenMessage(SPEAKER(ACTOR_SECOND_TRET), 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_SetMessageId(MsgTretDoingNowsNot);
    Event_ShowMessage(SPEAKER(ACTOR_GARET), 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Event_ShowMessage(SPEAKER(ACTOR_IVAN), 0);
  }
  Event_WaitFrames(20);
  TreeRooms_PlayGesture(TRET_GESTURE_BLINK);
  Event_SetMessageId(MsgTretTurnedPeopleKolima);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 10);
  TreeRooms_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  Event_WaitFrames(20);
  // Tret tries to release the people of Kolima: the colours shift and sparks
  // swirl out from his face, then fade.
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Event_WaitFrames(20);
  sSparkFrame = 0;
  {
    struct FieldPosition *origin = &sSparkOrigin;
    origin->x = PIXELS(168);
    origin->y = PIXELS(32);
    sparkTask = TreeRooms_SpawnSwirlSparks;
    origin->z = PIXELS(52);
  }
  Task_AddFrame(sparkTask, TASK_PASS_FRAME);
  Event_WaitFrames(220);
  Task_Remove(sparkTask);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  TreeRooms_PlayGesture(TRET_GESTURE_BLINK);
  Event_WaitFrames(20);
  // "I can't do it... My power no longer reaches Kolima..." His face sinks
  // away on "People of Kolima... Forgive me...", and the party resolve to
  // heal him.
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_SECOND_TRET), 0, 10);
  TreeRooms_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  Event_ShowMessage(SPEAKER(ACTOR_SECOND_TRET), 0);
  Actor_RunScriptAndWait(ACTOR_TRET, gTreeRooms_FaceSinksActions);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessage(SPEAKER(ACTOR_GARET), 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 10);
  Event_ShowMessage(SPEAKER(ACTOR_IVAN), 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_GARET), 0, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER(ACTOR_IVAN), 0, 10);
  if (hasMia != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageThenWait(SPEAKER(ACTOR_MIA), 0, 10);
  }
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  actions = gTreeRooms_RejoinLeaderActions;
  Actor_SetScript(ACTOR_GARET, actions);
  if (hasMia != 0) {
    Actor_SetScript(ACTOR_MIA, actions);
  }
  Actor_RunScriptAndWait(ACTOR_IVAN, actions);
  Event_WaitFrames(20);
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  GameFlag_Set(FLAG_TRET_DEFEATED);
  Task_AddFrame(TretPalette_UpdateMapTint, TASK_PASS_FRAME);
  Event_End();
}

/// Sets the map grid by FLAG_TRET_DEFEATED: the passage in Tret's room closed
/// before, open after, with cells near entrance 20 redrawn. With
/// playRecoveryEvent it first plays the conversation after the battle, unless
/// FLAG_KEEP_PARTY_POSITION is set.
static void TreeRooms_ApplyPostBattleState(s32 playRecoveryEvent) {
  if (playRecoveryEvent != 0 && GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    TreeRooms_PlayPostBattleConversation();

  WaitFrames(1);
  if (GameFlag_Get(FLAG_TRET_DEFEATED) != 0) {
    s32 openingColumn = 10;
    MapGrid_CopyCells(121, 34, 3, 1, 93, openingColumn);
    {
      s32 passageColumn = 30;
      MapGrid_CopyCells(46, 38, 1, 1, passageColumn, 43);
      MapGrid_CopyCellAttributes(0, 0, 1, 2, passageColumn, 9);
    }
    MapGrid_CopyCellAttributes(26, 3, 1, 2, openingColumn, 8);
    MapGrid_CopyCells(26, 35, 1, 4, openingColumn, 40);
  } else {
    s32 openingColumn = 10;
    s32 passageColumn = 8;
    MapGrid_CopyCellAttributes(11, 8, 1, 2, openingColumn, passageColumn);
  }
}

/// Tret Tree's setup: places or removes the Tret actors, starts the
/// tint, redraws the torn cocoons after a battle, sets the map grid by
/// FLAG_TRET_DEFEATED, and plays what the entrance calls for: the falls
/// on from entrances 50, 40, 30 and 20, the landing at entrance
/// 10, or the conversation after the battle at
/// TREE_ROOMS_AFTER_FIGHT_TRET.
s32 TreeRooms_EnterRoom(void) {
  struct Actor *tret;
  struct Actor *actor;
  struct MapGridScrollWork *grid;
  struct MapGridLayerScroll *layer;
  struct FieldWork *work;
  s32 entrance;
  s32 leaderActorId;

  tret = Actor_GetById(ACTOR_TRET);
  // Point the cocoon latch at the scene state and save the palette first.
  sLastCocoon = (s16 *)gSceneLocalState;
  TretPalette_SaveMapColors();
  tret->motionFlags = 0;
  tret->y.fixed = -PIXELS(10);
  {
    struct Actor *tretSpeaker = Actor_GetById(ACTOR_SECOND_TRET);

    tretSpeaker->motionFlags = 0;
    tretSpeaker->y.fixed = -PIXELS(10);
  }
  Actor_ConfigureSpriteColor(ACTOR_SECOND_TRET, SPRITE_COLOR_HIDDEN);
  TreeRooms_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  if (gAdventure.entrance != TREE_ROOMS_AFTER_FIGHT_TRET)
    Task_AddFrame(TretPalette_UpdateMapTint, TASK_PASS_FRAME);
  if (GameFlag_Get(FLAG_TRET_DEFEATED)) {
    Import_ActorSetPosition(ACTOR_SECOND_TRET, 0, 0);
    Import_ActorSetPosition(ACTOR_TRET, 0, 0);
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    TreeRooms_ApplyFlaggedMapPatches();
  grid = HEAP_SLOT_BLOCK(8);
  // Match the first layer's parallax offset and scale to the room bounds.
  layer = &grid->layers[0];
  layer->offsetX += Iwram_MulQ16(grid->minX + PIXELS(160), 0x1999);
  layer->offsetY += Iwram_MulQ16(grid->minY + PIXELS(136), 0x1999);
  layer->scaleX = 0xe666;
  layer->scaleY = 0xe666;
  // The layout draws four cocoons already torn: the first step on them breaks
  // through.
  GameFlag_Set(FLAG_TRET_TREE_SECOND_FLOOR_COCOON_2_TORN);
  GameFlag_Set(FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_COCOON_TORN);
  GameFlag_Set(FLAG_TRET_TREE_FIFTH_FLOOR_WEST_COCOON_TORN);
  GameFlag_Set(FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_COCOON_TORN);
  WaitFrames(1);
  TreeRooms_ApplyPostBattleState(0);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  entrance = gAdventure.entrance;
  leaderActorId = gAdventure.leaderActorId;
  actor = Actor_GetById(leaderActorId);
  // Entrances 20, 30, 40 and 50: the leader falls on through the room,
  // to the entrance numbered ten lower. Entrance 10: the leader
  // lands in a cloud of dust in front of Tret.
  // TREE_ROOMS_AFTER_FIGHT_TRET: back from the battle with Tret.
  if (entrance == 50 || entrance == 40 || entrance == 30 || entrance == 20) {
    Event_StartScreenTransition();
    Import_ActorSetAnimation(leaderActorId, ANIM_LEADER_SPIN_AWAY);
    Actor_SetSpriteFlags(Actor_GetById(leaderActorId), 0);
    Actor_SetAttachedEffect(leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    actor->motionFlags = ACTOR_FALLS;
    actor->y.fixed = PIXELS(100);
    actor->groundHeight = -PIXELS(160);
    actor->gravity = PIXELS(1) / 2;
    Event_PlaySong(SE_DUST_PUFF);
    Event_SetDestination((s32)&SceneId_TreeRooms, entrance - 10);
    Event_WaitFrames(20);
    actor->layer = 2;
    Actor_SetSpritePriority(leaderActorId, 3);
    Event_WaitFrames(2);
    FieldEffect_SetAttached(leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    Event_WaitFrames(8);
  } else if (entrance == 10) {
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      TreeRooms_RunLandingDustEvent();
  } else if (entrance == TREE_ROOMS_AFTER_FIGHT_TRET) {
    TreeRooms_ApplyPostBattleState(1);
  }
  return 0;
}

/// Saves background palettes 0 to 13 as the base for Tret's map tint.
static void TretPalette_SaveMapColors(void) {
  Dma_Set((const void *)BG_PLTT, gPaletteFadeWork->palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | 112, REG_DMA3);
}

/// Tints background colours 16 to 78 every thirty-two frames, leaving
/// palette 4's last colour alone. Each later palette receives a weaker tint.
static void TretPalette_UpdateMapTint(void) {
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
  struct FieldWork *work;
  struct PaletteFade *paletteWork;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  // Through its heap slot rather than gPaletteFadeWork, the spelling this
  // site matched with; the other is not measured.
  paletteWork = HEAP_SLOT_BLOCK(HEAP_SLOT_PALETTE_FADE);
  sourceColors = paletteWork->palette;
  if (work->psynergy != 0)
    return;
  if ((gFrameCount & 31) != 0)
    return;
  sourceColors += 16;
  destinationColors = (u16 *)&BG_PLTT_COLOR(1, 0);
  colorIndex = 0;
  for (; colorIndex < TRET_TINT_COLOR_COUNT; colorIndex++, sourceColors++) {
    redStep = gTreeRooms_TintSteps[gTreeRooms_TintIndex];
    greenStep = gTreeRooms_TintSteps[gTreeRooms_TintIndex + 1];
    blueStep = gTreeRooms_TintSteps[gTreeRooms_TintIndex + 2];
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
  gTreeRooms_TintIndex += (Import_RandomNext() & 7) * 3;
  if (gTreeRooms_TintSteps[gTreeRooms_TintIndex] == TRET_TINT_END)
    gTreeRooms_TintIndex = 0;
}

/// Plays one of Tret's gestures with its original pauses.
static void TreeRooms_PlayGesture(s32 gesture) {
  switch (gesture) {
  case TRET_GESTURE_CLOSE_EYES:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
    break;
  case TRET_GESTURE_OPEN_EYES:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    break;
  case TRET_GESTURE_LOOK_LEFT:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_LEFT);
    break;
  case TRET_GESTURE_LOOK_RIGHT:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_RIGHT);
    break;
  case TRET_GESTURE_BLINK:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    break;
  case TRET_GESTURE_HALF_CLOSE_EYES:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_HALF_CLOSED);
    break;
  case TRET_GESTURE_WRATHFUL_CLOSE_EYES:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
    break;
  case TRET_GESTURE_WRATHFUL_OPEN_EYES:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  case TRET_GESTURE_WRATHFUL_LOOK_RIGHT:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
    break;
  case TRET_GESTURE_WRATHFUL_LOOK_LEFT:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
    break;
  case TRET_GESTURE_WRATHFUL_BLINK:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
    WaitFrames(6);
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  case TRET_GESTURE_WRATHFUL_STARE:
    Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  }
  WaitFrames(12);
}

/// Moves a fading spark outward from its saved origin, then releases it.
static void TreeRooms_UpdateSwirlSpark(struct Actor *actor) {
  struct Actor *spark = (struct Actor *)actor;
  struct FieldPosition position;
  s32 frame;

  frame = spark->timer;
  if (frame <= SWIRL_SPARK_LAST_FRAME) {
    position.x = spark->targetX;
    position.y = spark->targetY;
    position.z = spark->targetZ;
    {
      s32 initialAngle = (s16)spark->userValue;

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
    VramCache_FreeEntry(spark->sprite->slot);
    Actor_Release(spark);
  }
}

/// Emits five shrinking waves of fading sparks from the saved origin.
static void TreeRooms_SpawnSwirlSparks(void) {
  struct Actor *spark;
  u32 frame;
  void *previousResourceEntry;
  s32 wave;
  u32 sparkIndex;

  frame = sSparkFrame;
  previousResourceEntry = 0;
  wave = (s32)frame / 10;
  switch (frame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    Event_PlaySong(SE_SHIMMER);
    // Waves contain 6, 5, 4, 3 and 2 sparks. Divide degrees before
    // converting them to turns, preserving the original rounding.
    for (sparkIndex = 0; sparkIndex < 6 - wave; sparkIndex++) {
      spark = FieldActor_Create(SPRITE_SPARKLE, sSparkOrigin.x, sSparkOrigin.y,
                                sSparkOrigin.z);
      if (spark != 0) {
        previousResourceEntry =
            Sprite_ShareVramCacheEntry(spark->sprite, previousResourceEntry);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 0;
        Actor_SetSpriteFlags(spark, 0);
        Actor_SelectAnimation(spark, 1);
        spark->timer = 0;
        spark->userValue = ((360 / (u32)(6 - wave) * sparkIndex) << 16) / 360;
        spark->targetX = sSparkOrigin.x;
        spark->targetY = sSparkOrigin.y;
        spark->targetZ = sSparkOrigin.z;
        spark->speedLimit = 0x19999;
        spark->update = TreeRooms_UpdateSwirlSpark;
      }
    }
    // Each wave also plays the closing sound.
    // fall through
  case 44:
    Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
    break;
  }
  if (++sSparkFrame > 120) {
    sSparkFrame = 0;
  }
}

/// Redraws each torn cocoon whose flag is set, as the party left it before a
/// battle.
static void TreeRooms_ApplyFlaggedMapPatches(void) {
  s32 patchOffset;

  for (patchOffset = 0; gTreeRooms_MapPatches[patchOffset] != -1;
       patchOffset += 6) {
    if (GameFlag_Get(gTreeRooms_MapPatches[patchOffset]) &&
        gTreeRooms_MapPatches[patchOffset + 1] != 0)
      MapGrid_CopyCellsTo(gTreeRooms_MapPatches[patchOffset + 2],
                          gTreeRooms_MapPatches[patchOffset + 3],
                          gTreeRooms_MapPatches[patchOffset + 4],
                          gTreeRooms_MapPatches[patchOffset + 5], 1, 1);
  }
}

/// Casting Reveal in Tret Tree (PSYNERGY_PHASE_ACTS) stops the map
/// tint until the scene is entered again.
static void TreeRooms_CastReveal(void) {
  Task_Remove(TretPalette_UpdateMapTint);
}

#include "shared/effect_advance.h"

/// Entrance 10, at the end of the fall from cocoon 50: the leader drops
/// in from 130 pixels up in front of Tret, lands in a ring of dust and the map
/// shakes.
static void TreeRooms_RunLandingDustEvent(void) {
  struct EffectOptions options;
  struct FieldPosition velocity;
  struct EffectOptions *particleOptions;
  struct FieldPosition *particleVelocity;
  struct Actor *leader;
  u32 particleIndex;
  s32 velocityX;
  s32 velocityZ;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed = PIXELS(130);
  Actor_GetById(ACTOR_PARTY_LEADER)->gravity = 0x8000;
  Actor_GetById(ACTOR_PARTY_LEADER)->bounce = 0;
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySong(SE_DUST_PUFF);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_WaitFrames(24);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  // Spread particles around the leader while the landing effect finishes.
  particleOptions = &options;
  particleOptions->palette = 7;
  particleOptions->update = FieldEffect_AdvanceMotion;
  particleOptions->startScaleX = 0xcccc;
  particleOptions->startScaleY = 0xcccc;
  for (particleIndex = 0; particleIndex <= 16; particleIndex++) {
    particleVelocity = &velocity;
    particleVelocity->x = Trig_Cos(particleIndex << 12);
    particleVelocity->y = 0;
    velocityZ = Trig_Sin(particleIndex << 12);
    velocityX = particleVelocity->x;
    particleVelocity->z = velocityZ;
    velocityX += velocityX / 2;
    particleVelocity->x = velocityX;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocityX, particleVelocity->y, velocityZ,
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      particleOptions);
  }
  Event_PlaySong(SE_THUMP);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  Actor_GetById(ACTOR_PARTY_LEADER)->gravity = 0x10000;
  Actor_GetById(ACTOR_PARTY_LEADER)->bounce = 0x4000;
  Event_End();
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

/// The torn cocoons redrawn after a battle: the cocoon's flag, whether it is
/// redrawn (0 for the four the layout draws torn), the staging cell x/y and the
/// cocoon's cell x/y.
// clang-format off
s16 gTreeRooms_MapPatches[] = {
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_1_TORN, 1, 64, 35, 113, 6,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_2_TORN, 0, 65, 35, 115, 6,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_3_TORN, 1, 66, 35, 110, 8,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_4_TORN, 1, 67, 35, 110, 10,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_5_TORN, 1, 68, 35, 111, 12,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_6_TORN, 1, 69, 35, 113, 12,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_7_TORN, 1, 70, 35, 112, 14,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_8_TORN, 1, 71, 35, 114, 14,
    FLAG_TRET_TREE_SECOND_FLOOR_COCOON_9_TORN, 1, 72, 35, 116, 14,
    FLAG_TRET_TREE_THIRD_FLOOR_WEST_COCOON_TORN, 1, 73, 35, 71, 23,
    FLAG_TRET_TREE_THIRD_FLOOR_EAST_COCOON_TORN, 1, 74, 35, 77, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_NORTHWEST_COCOON_TORN, 1, 75, 35, 112, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_NORTH_COCOON_TORN, 0, 76, 35, 114, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_NORTHEAST_COCOON_TORN, 1, 77, 35, 116, 24,
    FLAG_TRET_TREE_FIFTH_FLOOR_WEST_COCOON_TORN, 0, 78, 35, 112, 26,
    FLAG_TRET_TREE_FIFTH_FLOOR_CENTER_COCOON_TORN, 1, 79, 35, 114, 26,
    FLAG_TRET_TREE_FIFTH_FLOOR_EAST_COCOON_TORN, 1, 80, 35, 116, 26,
    FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHWEST_COCOON_TORN, 1, 81, 35, 112, 28,
    FLAG_TRET_TREE_FIFTH_FLOOR_SOUTH_COCOON_TORN, 1, 82, 35, 114, 28,
    FLAG_TRET_TREE_FIFTH_FLOOR_SOUTHEAST_COCOON_TORN, 0, 83, 35, 116, 28,
    FLAG_TRET_TREE_FIFTH_FLOOR_FAR_SOUTHEAST_COCOON_TORN, 1, 84, 35, 118, 28,
    -1, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The party gathers in front of Tret before the battle (TreeRooms_FightTret),
/// each walking to its own place.
s32 gTreeRooms_LeaderGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(80)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gTreeRooms_GaretGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(148), 0, PIXELS(90)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gTreeRooms_IvanGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(182), 0, PIXELS(90)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gTreeRooms_MiaGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(104)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

/// Tret's face sinks away after the conversation: eight times it shows 80
/// pixels up for 4 frames and drops back 78, shrinking a sixteenth each time,
/// and then it is put away.
s32 gTreeRooms_FaceSinksActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(80)),
    SC_WAIT(4),
    SC_ADD(SW_Y, -PIXELS(78)),
    SC_ADD(SW_SCALE_X, -0x1000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(4),
    SC_LOOP(8, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Garet, Ivan and Mia walk back to the leader and are put away.
s32 gTreeRooms_RejoinLeaderActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(80)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gTreeRooms_TintSteps[] = {
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

struct SceneEntrance gTreeRoomsEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { TREE_ROOMS_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 488, 0, 248, FACING_NORTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, CONDITION_ALWAYS, 712, 0, 152, FACING_EAST, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, CONDITION_ALWAYS, 920, 0, 152, FACING_WEST, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTH, CONDITION_ALWAYS, 168, 0, 520, FACING_NORTH, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_SOUTHEAST, CONDITION_ALWAYS, 296, 0, 424, FACING_WEST, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTHWEST, CONDITION_ALWAYS, 376, 0, 424, FACING_EAST, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH, CONDITION_ALWAYS, 488, 0, 520, FACING_NORTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST, CONDITION_ALWAYS, 712, 0, 424, FACING_EAST, 0, 672, 272, 960, 544, 0 },
    { TREE_ROOMS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST, CONDITION_ALWAYS, 920, 0, 424, FACING_WEST, 0, 672, 272, 960, 544, 0 },
    { 10, CONDITION_ALWAYS, 168, 0, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { TREE_ROOMS_ENTRANCE_FROM_SECOND_FLOOR, CONDITION_ALWAYS, 424, 24, 72, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_SECOND_FLOOR_FROM_ENTRANCE, CONDITION_ALWAYS, 744, 0, 88, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_THIRD_FLOOR_FROM_FOURTH_FLOOR, CONDITION_ALWAYS, 232, 24, 344, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { TREE_ROOMS_FOURTH_FLOOR_FROM_THIRD_FLOOR, CONDITION_ALWAYS, 552, 0, 360, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_TRET_FROM_ENTRANCE, CONDITION_ALWAYS, 168, 24, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { TREE_ROOMS_ENTRANCE_FROM_TRET, CONDITION_ALWAYS, 488, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_1, CONDITION_ALWAYS, 472, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_2, CONDITION_ALWAYS, 504, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_3, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_4, CONDITION_ALWAYS, 424, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_5, CONDITION_ALWAYS, 440, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_6, CONDITION_ALWAYS, 488, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_7, CONDITION_ALWAYS, 456, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_8, CONDITION_ALWAYS, 488, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_SECOND_FLOOR_COCOON_9, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_FROM_THIRD_FLOOR_WEST_COCOON, CONDITION_ALWAYS, 760, 0, 104, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_THIRD_FLOOR_EAST_COCOON, CONDITION_ALWAYS, 856, 0, 120, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHWEST_COCOON, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_NORTH_COCOON, CONDITION_ALWAYS, 488, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_NORTHEAST_COCOON, CONDITION_ALWAYS, 520, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_WEST_COCOON, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_EAST_COCOON, CONDITION_ALWAYS, 520, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHWEST_COCOON, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTH_COCOON, CONDITION_ALWAYS, 488, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_SOUTHEAST_COCOON, CONDITION_ALWAYS, 520, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_FAR_SOUTHEAST_COCOON, CONDITION_ALWAYS, 552, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TREE_ROOMS_FROM_FIFTH_FLOOR_CENTER_COCOON, CONDITION_ALWAYS, 488, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 40, CONDITION_ALWAYS, 168, 0, 424, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { 30, CONDITION_ALWAYS, 808, 0, 152, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 20, CONDITION_ALWAYS, 488, 0, 152, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TREE_ROOMS_AFTER_FIGHT_TRET, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { SCENE_TABLE_END },
};

struct SceneExitRegion gTreeRoomsRegions[] = {
    { 420, 28, 68, 428, 36, 76, -1, TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR },
    { 228, 28, 338, 236, 36, 346, -1, TREE_ROOMS_THIRD_FLOOR_TO_FOURTH_FLOOR },
#if !TBS_PATCH_3
    { 163, 28, 148, 171, 36, 156, -1, TREE_ROOMS_TRET_TO_ENTRANCE },
#else
    { 159, 24, 144, 175, 40, 160, -1, TREE_ROOMS_TRET_TO_ENTRANCE },
#endif
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gTreeRoomsExits[] = {
    SCENE_EXITS(TreeRooms),
    SCENE_EXIT(TREE_ROOMS_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_TREE_ROOMS),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHWEST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH_SOUTHEAST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTH),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_SOUTHEAST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTHWEST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHWEST),
    SCENE_EXIT(TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TREE_ROOMS_NORTH_NORTHEAST),
    SCENE_EXIT(TREE_ROOMS_ENTRANCE_TO_SECOND_FLOOR, TreeRooms, TREE_ROOMS_SECOND_FLOOR_FROM_ENTRANCE),
    SCENE_EXIT(TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE, TreeRooms, TREE_ROOMS_ENTRANCE_FROM_SECOND_FLOOR),
    SCENE_EXIT(TREE_ROOMS_THIRD_FLOOR_TO_FOURTH_FLOOR, TreeRooms, TREE_ROOMS_FOURTH_FLOOR_FROM_THIRD_FLOOR),
    SCENE_EXIT(TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR, TreeRooms, TREE_ROOMS_THIRD_FLOOR_FROM_FOURTH_FLOOR),
    SCENE_EXIT(TREE_ROOMS_TRET_TO_ENTRANCE, TreeRooms, TREE_ROOMS_ENTRANCE_FROM_TRET),
    SCENE_EXIT(TREE_ROOMS_ENTRANCE_TO_TRET, TreeRooms, TREE_ROOMS_TRET_FROM_ENTRANCE),
    SCENE_EXITS_END,
};

struct ActorTemplate gTreeRoomsActorTemplates[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TRET_RAISED, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gTreeRoomsTriggers[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS,
     TREE_ROOMS_TO_KOLIMA_FOREST_TREE},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_WEST, 2, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_EAST, 3, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 4, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTH},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_EAST, 5, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_SOUTHEAST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_WEST, 6, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTHWEST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_WEST, 8, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_EAST, 9, CONDITION_ALWAYS,
     TREE_ROOMS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, TREE_ROOMS_SECOND_FLOOR_TO_ENTRANCE},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS,
     TREE_ROOMS_FOURTH_FLOOR_TO_THIRD_FLOOR},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, TREE_ROOMS_ENTRANCE_TO_TRET},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 21, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 22, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 23, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 24, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 25, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 26, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 27, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 28, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 29, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 31, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 32, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 50, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 51, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 52, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 53, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 54, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 55, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 56, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 57, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 58, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 59, CONDITION_ALWAYS,
     EVENT(TreeRooms_JumpFromCocoon)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon1)},
    {TRIGGER_STEP, 22, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon2)},
    {TRIGGER_STEP, 23, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon3)},
    {TRIGGER_STEP, 24, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon4)},
    {TRIGGER_STEP, 25, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon5)},
    {TRIGGER_STEP, 26, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon6)},
    {TRIGGER_STEP, 27, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon7)},
    {TRIGGER_STEP, 28, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon8)},
    {TRIGGER_STEP, 29, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnSecondFloorCocoon9)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnThirdFloorWestCocoon)},
    {TRIGGER_STEP, 32, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnThirdFloorEastCocoon)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorCenterCocoon)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorNorthwestCocoon)},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorNorthCocoon)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorNortheastCocoon)},
    {TRIGGER_STEP, 54, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorWestCocoon)},
    {TRIGGER_STEP, 55, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorEastCocoon)},
    {TRIGGER_STEP, 56, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorSouthwestCocoon)},
    {TRIGGER_STEP, 57, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorSouthCocoon)},
    {TRIGGER_STEP, 58, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorSoutheastCocoon)},
    {TRIGGER_STEP, 59, CONDITION_ALWAYS,
     EVENT(TreeRooms_StepOnFifthFloorFarSoutheastCocoon)},
    {TRIGGER_STEP, 40, FLAG_TRET_DEFEATED, EVENT(TreeRooms_FightTret)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TreeRooms_CastReveal)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, 0xf5b, FIND_ITEM(ITEM_HEALING_RING)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 101, 0xf5c, FIND_ITEM(ITEM_NUT)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 102, 0x350, FIND_PSYNERGY_STONE},
    {TRIGGERS_END},
};

s32 gTreeRooms_TintIndex = 0;
