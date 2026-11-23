/// @file
/// The Shaman Village Cave, on the way east from Hesperia to the Shaman
/// Village. It has three rooms. The first, entered from the world map at its
/// south, has three passages in its north wall. The back room behind them has
/// raised ledges with four round boulders standing in the gaps between them,
/// and Eddy, the Mercury Djinni, on the ledge in the middle. The pillar room,
/// east of it, has the cave's east mouth.
///
/// On the first visit, Whirlwind clears the weeds off the first room's west
/// wall, Felix climbs to the west passage, crosses the back room to the pillar
/// room and Moves the stone pillar two cells east into the gap in its ledge,
/// which makes a short cut. Once Isaac has joined, with Lift, the party lifts
/// the boulders, freezes the puddle and clears the back room's weeds to climb
/// up to Eddy, who can no longer hop away once the third boulder is up. In a
/// game carried over from a The Broken Seal save that won Colosso, Satrage,
/// Azart and Navampa, the gladiators Isaac beat there, catch the party in the
/// first room for a rematch, and give up a Golden Shirt when they lose.
#include "djinn_ids.h"
#include "edition.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// Set once Move has pushed the stone pillar into the gap in the pillar room's
/// ledge, at cell (38, 19).
#define FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED 0x9a8
/// Set once Whirlwind has scattered the weeds on the first room's west wall.
#define FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED 0x9a9
/// Set once Whirlwind has scattered the weeds in the back room.
#define FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED 0x9aa
/// Set as the gladiators from Colosso find the party: "I found Isaac!"
#define FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS 0xa70
/// Set while the party has lost to the gladiators, who wait for a rematch.
#define FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS 0xa71
/// Set once the party has beaten the gladiators.
#define FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS 0xa72
/// Set while the Golden Shirt the beaten gladiators leave lies in the cave.
#define FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT 0xa73
/// Set as Lift raises one of the back room's four boulders, n counting from 0
/// in the west. While the second or third stands in its gap, the party can jump
/// across the gap in the ledges above it; lifted, it fills the gap and there is
/// no jump.
#define FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(n) (FLAG_SCENE_TEMP_00 + (n))
/// Set once Eddy, its way south blocked by the lifted third boulder, has
/// winced at the party.
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED FLAG_SCENE_TEMP_04
/// Set once Eddy has been fought or has joined: it no longer hops away from the
/// party.
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING FLAG_SCENE_TEMP_05
/// Set as the battle with Eddy begins: back in the cave, Eddy stands on the
/// ledge between the third and fourth boulders.
#define FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT FLAG_SCENE_TEMP_06
/// Set once Frost has frozen the back room's puddle into a pillar of ice.
#define FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN FLAG_AREA_TEMP_02

/// The cave's ways between its rooms. Each passage's cell, and the cell at
/// each room's edge, holds the number of the exit it leads by.
enum ShamanVillageCaveExit {
  /// The first room's west passage, cell (2, 6), at the top of the wall the
  /// first weeds cover: to the back room's west end.
  EXIT_WEST_PASSAGE = 2,
  /// The first room's middle passage, cell (12, 8): to the middle of the back
  /// room.
  EXIT_MIDDLE_PASSAGE = 3,
  /// The first room's east passage, cell (22, 14): to the pillar room's floor.
  EXIT_EAST_PASSAGE = 4,
  /// The back room's west end, cell (4, 59): back below the west passage.
  EXIT_BACK_ROOM_WEST = 5,
  /// The back room's middle, cell (14, 59): back below the middle passage.
  EXIT_BACK_ROOM_MIDDLE = 6,
  /// The back room's east end, cell (26, 55): to the pillar room's ledge.
  EXIT_BACK_ROOM_EAST = 7,
  /// The passage behind the pillar room's ledge, cell (33, 18): to the back
  /// room's east end.
  EXIT_PILLAR_ROOM_PASSAGE = 8,
  /// The pillar room's floor, cell (31, 25): back below the east passage.
  EXIT_PILLAR_ROOM_FLOOR = 9
};

/// Where the party comes in: from another room, or back from the gladiators'
/// battle, having beaten them or lost.
enum ShamanVillageCaveEntrance {
  ENTRANCE_BELOW_WEST_PASSAGE = 2,
  ENTRANCE_BELOW_MIDDLE_PASSAGE = 3,
  ENTRANCE_BELOW_EAST_PASSAGE = 4,
  ENTRANCE_BACK_ROOM_WEST = 5,
  ENTRANCE_BACK_ROOM_MIDDLE = 6,
  ENTRANCE_BACK_ROOM_EAST = 7,
  ENTRANCE_PILLAR_ROOM_LEDGE = 8,
  ENTRANCE_PILLAR_ROOM_FLOOR = 9,
  ENTRANCE_AFTER_BEATING_GLADIATORS = 20,
  ENTRANCE_AFTER_LOSING_TO_GLADIATORS = 21
};

/// The cells that run the cave's other events.
enum ShamanVillageCaveCell {
  /// West of the gap in the back room's ledges above the second boulder, cell
  /// (12, 50), walking east.
  GAP_ABOVE_SECOND_BOULDER_WEST_CELL = 41,
  /// East of that gap, cell (14, 50), walking west.
  GAP_ABOVE_SECOND_BOULDER_EAST_CELL = 42,
  /// West of the gap above the third boulder, cell (17, 49), walking east.
  GAP_ABOVE_THIRD_BOULDER_WEST_CELL = 43,
  /// East of that gap, cell (19, 49), walking west.
  GAP_ABOVE_THIRD_BOULDER_EAST_CELL = 44,
  /// Close to Eddy on its ledge: cells (16, 45), (20, 45), (16, 49), (20, 49)
  /// and (15, 50).
  NEAR_EDDY_CELL = 50,
  /// Halfway up the first room, cells (12, 14) and (13, 14), where the
  /// gladiators catch the party.
  GLADIATORS_AMBUSH_CELL = 100
};

void Message_CloseFinishedWindows(void);

/// The cave's actors, numbering the rows of gActorPlacements.
enum ShamanVillageCaveActorId {
  /// The weeds Whirlwind scatters from the wall below the first room's west
  /// passage, over the steps that climb it (SPRITE_WEEDS).
  ACTOR_FIRST_WEEDS = ACTOR_FIRST_PLACED,
  /// A round boulder placed in the map's top left corner, above every room's
  /// view; nothing moves it.
  ACTOR_HIDDEN_BOULDER,
  /// The second clump of weeds, on a wall in the back room, over the steps up
  /// toward Eddy.
  ACTOR_SECOND_WEEDS,
  /// Four round boulders standing in the gaps between the back room's ledges,
  /// west to east (SPRITE_ROUND_BOULDER).
  ACTOR_FIRST_BOULDER,
  ACTOR_LAST_BOULDER = ACTOR_FIRST_BOULDER + 3,
  /// The stone pillar on the pillar room's ledge, which Move pushes
  /// (SPRITE_STONE_PILLAR).
  ACTOR_PILLAR,
  /// The puddle Frost freezes, on the back room's west side (SPRITE_PUDDLE).
  ACTOR_PUDDLE,
  /// Eddy, the Mercury Djinni, placed until it joins
  /// (FLAG_DJINNI(DJINNI_EDDY)).
  ACTOR_EDDY,
  ACTOR_KRADEN,
  /// Colosso's three gladiators, come to call Isaac a cheat: "The name's
  /// Satrage.", "And I'm Azart!", "And don't forget Navampa!"
  ACTOR_SATRAGE,
  ACTOR_AZART,
  ACTOR_NAVAMPA,
  /// The Golden Shirt the gladiators leave (ShamanVillageCave_TakeGoldenShirt).
  ACTOR_GOLDEN_SHIRT
  // The placement table's last row, in Isaac's sprite, is ACTOR_ISAAC.
};

extern u32 gShamanVillageCave_GladiatorLeaves[];

extern s32 gShamanVillageCave_GladiatorFollowsOut[];

void Field_TryJumpForward();

extern s32 gShamanVillageCave_WeedsScript[];
extern u32 gShamanVillageCave_PillarScript[];

s32 Event_PickUpItem();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Whirlwind blows a clump of weeds away for good: the one on the first room's
/// west wall, cell (2, 8), uncovering the steps up to the west passage, or the
/// one on the back room's wall, cell (14, 38), uncovering the steps up toward
/// Eddy. The leaves flying are the field's own Whirlwind; here the clump's flag
/// is set and its cell made climbable, copied from a spare climbable cell at
/// the map's west edge.
void ShamanVillageCave_ScatterWeeds(s32 unused, s32 weeds) {
  if (weeds == ACTOR_FIRST_WEEDS) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED);
    MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, 8);
  }
  if (weeds == ACTOR_SECOND_WEEDS) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED);
    MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
  }
}

/// The stone pillar drops into the gap in the ledge until it lands on the floor
/// below, or for a second at most, and stands there without its shadow.
void ShamanVillageCave_DropPillarIntoGap(s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 frames = 60;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (frames != 0) {
    System_Wait(1);
    if (actor->velocityY == 0)
      break;
    frames--;
  }
  System_Wait(10);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
}

/// Nothing visible happens as Move takes hold of the stone pillar in the pillar
/// room. Before the push, the game walls in the pillar's lane along the ledge,
/// to its north, south and west, so that it can only travel east; the walls
/// are a spare ring of wall cells copied over the ledge's cells (35, 18) to
/// (38, 20) on the map's middle layer (columns 99 to 102), and come down again
/// in ShamanVillageCave_FinishMovingPillar.
void ShamanVillageCave_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(99, 10, 4, 3, 99, 18);
}

/// Each push of the stone pillar ends here. Pushed two cells east, to the gap
/// in the pillar room's ledge at cell (38, 19), it drops into the gap and
/// stays, its top level with the ledge, and the party can hop across on it: the
/// short cut. Either way the walls round its lane come down again.
void ShamanVillageCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 x = FIELD_CELL(actor->x.fixed);

  if (x == 38) {
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED);
    actor->priorityFlags = 0;
    ShamanVillageCave_DropPillarIntoGap(id);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, x, 19);
  }
  MapGrid_CopyCellAttributes(99, 6, 4, 3, 99, 18);
}

/// Frost freezes the puddle on the back room's west side, cell (6, 42), into a
/// tall pillar of ice, whose top the party can hop across on.
void ShamanVillageCave_FreezePuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_PUDDLE);

  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
}

/// Eddy, the Mercury Djinni on the ledge in the middle of the back room, hops
/// away from the leader who comes near it on its side of the third boulder's
/// column: two cells south (north when the leader is south of it), four cells
/// east or west away from the leader, the last three in a leap, and back. Once
/// the third boulder is lifted, its first hop south is its last: it lands,
/// steps west, jumps, backs away and winces, cornered.
void ShamanVillageCave_EddyFlees(void) {
  struct Actor *actor = Actor_GetById(ACTOR_EDDY);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 leaderX = FIELD_TILE(leader->x.fixed);
  s32 leaderZ = FIELD_TILE(leader->z.fixed);
  s32 eddyX = FIELD_TILE(actor->x.fixed);
  s32 dx;
  s32 dz;

  if (leaderX <= 36 && eddyX > 37)
    return;
  if (leaderX > 37 && eddyX <= 36)
    return;
  dx = 1;
  if (leaderX > 36)
    dx = -dx;
  dz = -1;
  if (leaderZ <= 95)
    dz = 1;
  if (dz == 1 &&
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2)) != 0) {
    if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED) != 0)
      return;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_Jump(ACTOR_EDDY, 4, 20);
    Actor_SetDestinationOffset(ACTOR_EDDY, 0, 32);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldCamera_MoveToActorById(ACTOR_EDDY, 1);
    FieldActor_StartRepeatedMotion(ACTOR_EDDY, 2);
    Actor_FaceDirection(ACTOR_EDDY, FACING_WEST, 0);
    Actor_SetDestinationOffset(ACTOR_EDDY, -8, 0);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldActor_Jump(ACTOR_EDDY, 4, 20);
    Actor_SetAttachedEffect(ACTOR_EDDY,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(30);
    Actor_GetById(ACTOR_EDDY)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkBy(ACTOR_EDDY, 8, 0);
    FieldActor_WaitForMoveById(ACTOR_EDDY);
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_EDDY, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldEvent_Wait(30);
    Event_End();
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_CORNERED);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  dz *= 32;
  FieldActor_Jump(ACTOR_EDDY, 4, 20);
  Actor_SetDestinationOffset(ACTOR_EDDY, 0, dz);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, dx * 16, 0);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, dx * 48, 0);
  actor->velocityY = toFIXED(8);
  dz = -dz;
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Actor_SetDestinationOffset(ACTOR_EDDY, 0, dz);
  FieldActor_WaitForMoveById(ACTOR_EDDY);
  Event_End();
}

/// Talking to Eddy, the Mercury Djinni, once the party has cornered it, starts
/// its battle.
void ShamanVillageCave_FightEddy(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT);
  Event_RequestDjinnBattle(ACTOR_EDDY, DJINNI_EDDY);
}

/// The weeds' set-up, called by their script: they cast no shadow, stand still,
/// and are drawn at priority 2, the priority of the cave's walls.
s32 ShamanVillageCave_SetUpWeeds(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  ((struct FieldSprite *)actor->sprite)->priority = 2;
  actor->motionFlags = 0;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Lift raises the leftmost of the back room's four boulders, cell (8, 50), out
/// of its gap, one of the steps on the way up to Eddy. Like each of the four,
/// it stays up until the party leaves the cave.
void ShamanVillageCave_LiftFirstBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(0));
}

/// Lift raises the second boulder from the west in the back room, cell
/// (13, 50), out of its gap. Up in the gap, it stops the jump across the gap in
/// the ledges above (ShamanVillageCave_JumpForward).
void ShamanVillageCave_LiftSecondBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1));
}

/// Lift raises the third boulder from the west in the back room, cell (18, 49),
/// reached from behind once the fourth is up. Up in the gap beside Eddy's
/// ledge, it cuts off Eddy's way south: the next time it hops south, it is
/// cornered.
void ShamanVillageCave_LiftThirdBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2));
}

/// Lift raises the rightmost of the back room's four boulders, cell (22, 49),
/// opening the way behind it to the third.
void ShamanVillageCave_LiftFourthBoulder(void) {
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(3));
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Talking to the Golden Shirt the gladiators leave on the first room's floor
/// picks it up, if there is room.
void ShamanVillageCave_TakeGoldenShirt(void) {
  s32 result;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result = Value3((s32 (*)())Event_PickUpItem, ACTOR_GOLDEN_SHIRT,
                  ITEM_GOLDEN_SHIRT, -1);
  if (result == 0)
    Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
  Event_End();
}

/// The three gladiators stand in the first room, where their rematch left
/// them.
void ShamanVillageCave_PlaceGladiators(void) {
  s32 angle;

  angle = FACING_SOUTH_SOUTHWEST;
  Actor_SetPositionAndFacing(ACTOR_SATRAGE, toFIXED(231), toFIXED(199), angle);
  Actor_SetPositionAndFacing(ACTOR_AZART, toFIXED(220), toFIXED(191), angle);
  Actor_SetPositionAndFacing(ACTOR_NAVAMPA, toFIXED(204), toFIXED(191), angle);
}

/// The Golden Shirt lies where the gladiators left it
/// (FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT).
void ShamanVillageCave_PlaceGoldenShirt(void) {
  s32 angle;

  FieldActor_ShowItemIconById(ACTOR_GOLDEN_SHIRT, ITEM_GOLDEN_SHIRT);
  angle = FACING_NORTH;
  Actor_SetPositionAndFacing(ACTOR_GOLDEN_SHIRT, toFIXED(212), toFIXED(199),
                             angle);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT);
}

/// Walking halfway up the first room, cells (12, 14) and (13, 14), once Isaac
/// has joined, in a game carried over from a The Broken Seal save that won
/// Colosso: Satrage, Azart and Navampa, the gladiators Isaac beat there, come
/// out of the middle passage, call the win a cheat and fight the party.
void ShamanVillageCave_MeetGladiators(void) {
  s32 angle;

  if (Event_GetFlag(FLAG_TRANSFERRED_WON_COLOSSO) == 0)
    return;
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_ISAAC)) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS);
  FieldEvent_SetMessageId((s32)MsgIFoundName);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STAND);
  // "I found Isaac!"
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 200, 221);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(5);
  Actor_WalkBy(ACTOR_FELIX, -16, 8);
  FieldActor_LaunchFromLeader(ACTOR_ISAAC, 0, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 8, 16, FACING_NORTH);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 177, 1);
  FieldCamera_WaitForArrival();
  angle = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_SATRAGE, toFIXED(199), toFIXED(130), angle);
  Actor_WalkTo(ACTOR_SATRAGE, 231, 199);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(ACTOR_AZART, toFIXED(199), toFIXED(130), angle);
  Actor_WalkTo(ACTOR_AZART, 220, 191);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(ACTOR_NAVAMPA, toFIXED(199), toFIXED(130), angle);
  Actor_WalkTo(ACTOR_NAVAMPA, 204, 191);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 225, 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(ACTOR_SATRAGE);
  Actor_FaceDirection(ACTOR_SATRAGE, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WaitForMoveById(ACTOR_AZART);
  Actor_FaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WaitForMoveById(ACTOR_NAVAMPA);
  Actor_FaceDirectionAndWait(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST);
  Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 2);
  // "We've been looking for you, Isaac!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Who are you guys?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "The name's Satrage. You took me out in the last round at Colosso."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_AZART, 2);
  FieldEvent_Wait(5);
  // "And I'm Azart!"
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "And don't forget Navampa!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Oh. It's you guys."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_SATRAGE, 2);
  FieldEvent_Wait(5);
  // "Is that any way to say hello to some old friends?"
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What's your problem?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "We want to talk to you about your fighting style at Colosso."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  // "Yeah. And we think you shouldn't have won, Isaac. We think you..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "It sounds like you resent the fact that Isaac beat you..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimation(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_SATRAGE, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_AZART, ANIM_SHAKE_HEAD);
  // "We've just been thinking about it, and there's no way that Isaac..."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Right. So you think I cheated just because I used my Pynergy..."
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_FaceEachOther(ACTOR_AZART, ACTOR_NAVAMPA, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I don't know how you did it. I'm just sayin' I think you cheated is..."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_FaceDirection(ACTOR_AZART, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Ah, but Psynergy is a warrior's skill, every bit as much as your..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  // "You're not listening, are you, old man? That fight was fixed, and..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetDestinationOffset(ACTOR_AZART, -16, 24);
  Actor_SetDestinationOffset(ACTOR_NAVAMPA, -16, 24);
  Actor_SetDestinationOffset(ACTOR_SATRAGE, -16, 24);
  FieldActor_WaitForMoveById(ACTOR_SATRAGE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(SCENE_SHAMAN_VILLAGE_CAVE,
                                       ENTRANCE_AFTER_BEATING_GLADIATORS);
  Event_SetDefeatDestination(SCENE_SHAMAN_VILLAGE_CAVE,
                             ENTRANCE_AFTER_LOSING_TO_GLADIATORS);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
}

/// Talking to any of the three gladiators, who wait in the first room after
/// beating the party, starts the rematch.
void ShamanVillageCave_FightGladiators(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  FieldEvent_SetAfterBattleDestination(SCENE_SHAMAN_VILLAGE_CAVE,
                                       ENTRANCE_AFTER_BEATING_GLADIATORS);
  Event_SetDefeatDestination(SCENE_SHAMAN_VILLAGE_CAVE,
                             ENTRANCE_AFTER_LOSING_TO_GLADIATORS);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
}

/// Back in the cave after losing the rematch, the gladiators crow over the
/// party: "Hey, these guys aren't very strong at all!"
void ShamanVillageCave_LoseToGladiators(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 273, 0);
  FieldEvent_SetMessageId((s32)MsgHeyTheseGuys);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(184), toFIXED(229),
                             FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(200), toFIXED(221),
                             FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(218), toFIXED(320),
                             FACING_NORTH);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_ISAAC, 39);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(ACTOR_NAVAMPA, 2);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Hey, these guys aren't very strong at all!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Ha! We were right! They were cheating at Colosso!"
  Event_OpenMessage(ACTOR_SATRAGE, 0);
  if (EventYesNo_Ask(1) == 0) {
    Message_CloseFinishedWindows();
    // Yes: "I knew it!"
    FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
    gFieldWork->message++;
  } else {
    Message_CloseFinishedWindows();
    gFieldWork->message++;
    // No: "It takes a lot to reach the finals at Colosso... A lot more..."
    FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  }
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
  // "You look like you've got business in here, but there's no way you're..."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Give it up! We're staying here. We've got our eyes peeled, and..."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 373, 1);
  FieldEvent_Wait(100);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 1);
  FieldEvent_Wait(20);
  Event_SetDestination((s16)gMainRoutineWork.restartScene,
                       (s16)gMainRoutineWork.restartEntrance);
#if TLA_PATCH_1
  WorldMap_SetReturnLeavingScene(gMainRoutineWork.next,
                                 gMainRoutineWork.entrance);
#endif
  gFieldWork->transition = 0;
}

/// Back in the cave after winning the rematch, the gladiators marvel: "These
/// guys were phenomenal!"
void ShamanVillageCave_BeatGladiators(void) {
  struct Actor *felix;

  Event_ClearFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
  Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_BEAT_GLADIATORS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(199, CAMERA_KEEP, 273, 0);
  FieldEvent_SetMessageId((s32)MsgTheseGuysWere);
  Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(184), toFIXED(229),
                             FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(200), toFIXED(221),
                             FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(208), toFIXED(237),
                             FACING_NORTHEAST);
  Actor_SetAnimation(ACTOR_AZART, 8);
  Actor_SetAnimation(ACTOR_SATRAGE, 8);
  Actor_SetAnimation(ACTOR_NAVAMPA, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(ACTOR_AZART, 2);
  FieldEvent_Wait(5);
  // "These guys were phenomenal!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  // "Which one of you didn't think they won Colosso fair and square?"
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  // "It wasn't me! I think both you and Navampa were saying something..."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "So. You don't still think our victory at Colosso was fixed, do you?"
  FieldEvent_ShowMessageThenWait(ACTOR_ISAAC, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "No way, Isaac! We've seen the error of our ways!"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "We promise we won't badmouth you or call you a fraud ever again. Will..."
  Event_OpenMessage(ACTOR_SATRAGE, 0);
  while (FieldEvent_AskYesNoWithActorAnimation(0, 0) != 0) {
    Actor_ShowEmote(ACTOR_SATRAGE, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // No: "Don't be that way! We've learned our lesson! Forgive us!"
    Event_OpenMessage(ACTOR_AZART, 0);
    gFieldWork->message--;
  }
  gFieldWork->message++;
  // Yes: "You're not such a bad kid after all!"
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_SetAnimation(ACTOR_AZART, ANIM_STAND);
  Actor_SetAnimation(ACTOR_SATRAGE, ANIM_STAND);
  Actor_SetAnimation(ACTOR_NAVAMPA, ANIM_STAND);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You came out here looking for treasure, didn't you?"
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  ShamanVillageCave_PlaceGoldenShirt();
  FieldEvent_Wait(60);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Here, this was in the chest we found."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_AZART, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
  // "You were nice enough to forgive us, so we think you should have it."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_ShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Right, well... I guess we'll be leaving now."
  FieldEvent_ShowMessageThenWait(ACTOR_NAVAMPA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "By the time the next Colosso rolls around, we ought to be strong..."
  FieldEvent_ShowMessageThenWait(ACTOR_SATRAGE, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_AZART, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Maybe we'll see you there."
  FieldEvent_ShowMessageThenWait(ACTOR_AZART, 0, 5);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_SATRAGE, 1);
  Actor_SetSpeed(ACTOR_AZART, 1);
  Actor_SetSpeed(ACTOR_NAVAMPA, 1);
  Actor_SetScript(ACTOR_SATRAGE, gShamanVillageCave_GladiatorLeaves);
  FieldEvent_Wait(20);
  Actor_SetScript(ACTOR_AZART, gShamanVillageCave_GladiatorFollowsOut);
  FieldEvent_Wait(20);
  Actor_SetScript(ACTOR_NAVAMPA, gShamanVillageCave_GladiatorFollowsOut);
  FieldEvent_Wait(100);
  Actor_FaceEachOther(ACTOR_ISAAC, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_WALK);
  felix = Actor_Get(ACTOR_FELIX);
  if (felix != NULL)
    Actor_SetDestination(ACTOR_ISAAC, felix->x.part[0].integer,
                         felix->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  felix = Actor_Get(ACTOR_FELIX);
  if (felix != NULL)
    Actor_SetDestination(ACTOR_KRADEN, felix->x.part[0].integer,
                         felix->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

/// Walking north into one of the dark passages in the rock, Felix steps in and
/// the screen wipes to the room it leads to. The first room has three, at the
/// top of its west wall, cell (2, 6), to the back room's west end; in the
/// middle, cell (12, 8), to the back room's middle; and in the east, cell
/// (22, 14), to the pillar room's floor. The fourth, behind the pillar room's
/// ledge at cell (33, 18), leads back to the back room's east end. Everyone on
/// stage stops as he goes.
void ShamanVillageCave_EnterPassage(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  u32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
    actor = Actor_Get(id);
    if (actor)
      actor->motionFlags = 0;
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  FieldEvent_Wait(5);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Sets the cave up as its flags left it: the scattered weeds gone, the pillar
/// in its gap, the puddle frozen, Eddy where its battle was fought, and the
/// gladiators back from their rematch, won or lost, or waiting for one.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED))
    Actor_SetPosition(ACTOR_FIRST_WEEDS, 0, 0);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED))
    Actor_SetPosition(ACTOR_SECOND_WEEDS, 0, 0);
  if (Event_GetFlag(FLAG_DJINNI(DJINNI_EDDY)) ||
      Event_GetFlag(FLAG_DJINNI_HIDING(DJINNI_EDDY)))
    Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED)) {
    actor = Actor_Get(ACTOR_PILLAR);
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->x.fixed = toFIXED(616);
    actor->y.fixed = 0;
    Actor_SetSpriteFlags(actor, 0);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, 38, 19);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PUDDLE), 0);
  Actor_SetAnimation(ACTOR_PUDDLE, 1);
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN)) {
    actor = Actor_Get(ACTOR_PUDDLE);
    Actor_SetAnimation(ACTOR_PUDDLE, 5);
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_PUDDLE), SPRITE_COLOR_NORMAL);
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->y.fixed = 0;
    MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_EDDY_FOUGHT))
    Actor_SetPosition(ACTOR_EDDY, 328, 792);
  // The three gladiators share one sprite, told apart by its colours.
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_SATRAGE), SPRITE_COLOR_GRB);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AZART), SPRITE_COLOR_NORMAL);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_NAVAMPA), SPRITE_COLOR_RBG);
  if ((!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
       (gMainRoutineWork.entrance == ENTRANCE_AFTER_BEATING_GLADIATORS ||
        gMainRoutineWork.entrance == ENTRANCE_AFTER_LOSING_TO_GLADIATORS)) ||
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS)) {
    ShamanVillageCave_PlaceGladiators();
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == ENTRANCE_AFTER_BEATING_GLADIATORS) {
      ShamanVillageCave_BeatGladiators();
    } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
               gMainRoutineWork.entrance ==
                   ENTRANCE_AFTER_LOSING_TO_GLADIATORS) {
      Event_SetFlag(FLAG_SHAMAN_VILLAGE_CAVE_LOST_TO_GLADIATORS);
      ShamanVillageCave_LoseToGladiators();
    }
  }
  if (Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_GOLDEN_SHIRT_LEFT))
    ShamanVillageCave_PlaceGoldenShirt();
  return 0;
}

/// The walls whose weeds Whirlwind has scattered are climbable again.
s32 Scene_PrepareMap(void) {
  s32 firstScattered;
  s32 secondScattered;

  firstScattered =
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_FIRST_WEEDS_SCATTERED);
  if (firstScattered != 0)
    MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, 8);
  secondScattered =
      Event_GetFlag(FLAG_SHAMAN_VILLAGE_CAVE_SECOND_WEEDS_SCATTERED);
  if (secondScattered != 0)
    MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
  return 0;
}

/// Walking east or west against the gap in the back room's ledges above the
/// second or third boulder (cells (12, 50) and (14, 50), (17, 49) and
/// (19, 49)), the leader jumps across it, while that boulder still stands in
/// the gap below.
void ShamanVillageCave_JumpForward(void) { Field_TryJumpForward(); }

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(ShamanVillageCave),
    SCENE_EXIT_UNLESS(SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST, WorldMap,
                      WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_WEST_PASSAGE, ShamanVillageCave,
                      ENTRANCE_BACK_ROOM_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_MIDDLE_PASSAGE, ShamanVillageCave,
                      ENTRANCE_BACK_ROOM_MIDDLE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_EAST_PASSAGE, ShamanVillageCave,
                      ENTRANCE_PILLAR_ROOM_FLOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_BACK_ROOM_WEST, ShamanVillageCave,
                      ENTRANCE_BELOW_WEST_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_BACK_ROOM_MIDDLE, ShamanVillageCave,
                      ENTRANCE_BELOW_MIDDLE_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_BACK_ROOM_EAST, ShamanVillageCave,
                      ENTRANCE_PILLAR_ROOM_LEDGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_PILLAR_ROOM_PASSAGE, ShamanVillageCave,
                      ENTRANCE_BACK_ROOM_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_PILLAR_ROOM_FLOOR, ShamanVillageCave,
                      ENTRANCE_BELOW_EAST_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST, WorldMap,
                      WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_EAST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The weeds' script: they stand half as tall again, and are set up by
/// ShamanVillageCave_SetUpWeeds.
s32 gShamanVillageCave_WeedsScript[] = {
    SC_SET(SW_SCALE_Y_DUPLICATE, FIXED_ONE * 3 / 2),
    SC_CALL(ShamanVillageCave_SetUpWeeds),
    SC_END,
};

// clang-format off
/// The pillar's script: it stands still.
u32 gShamanVillageCave_PillarScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gShamanVillageCave_WeedsScript, 40, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gShamanVillageCave_WeedsScript, 232, 0, 608, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 792, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gShamanVillageCave_PillarScript, 584, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_EDDY), MOVEMENT_TYPE_STAND, 264, 0, 760, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_WEST},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_WEST_PASSAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_MIDDLE_PASSAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_EAST_PASSAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {TRIGGER_EXIT, EXIT_BACK_ROOM_WEST, CONDITION_ALWAYS, EXIT_BACK_ROOM_WEST},
    {TRIGGER_EXIT, EXIT_BACK_ROOM_MIDDLE, CONDITION_ALWAYS, EXIT_BACK_ROOM_MIDDLE},
    {TRIGGER_EXIT, EXIT_BACK_ROOM_EAST, CONDITION_ALWAYS, EXIT_BACK_ROOM_EAST},
    {STEP_TRIGGER_FACING(FACING_NORTH), EXIT_PILLAR_ROOM_PASSAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {TRIGGER_EXIT, EXIT_PILLAR_ROOM_FLOOR, CONDITION_ALWAYS, EXIT_PILLAR_ROOM_FLOOR},
    {TRIGGER_EXIT, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, SHAMAN_VILLAGE_CAVE_TO_WORLD_MAP_EAST},
    {TRIGGER_STEP, NEAR_EDDY_CELL, FLAG_SHAMAN_VILLAGE_CAVE_EDDY_STOPS_FLEEING, EVENT(ShamanVillageCave_EddyFlees)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), GAP_ABOVE_SECOND_BOULDER_WEST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), GAP_ABOVE_SECOND_BOULDER_EAST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), GAP_ABOVE_THIRD_BOULDER_WEST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(ShamanVillageCave_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), GAP_ABOVE_THIRD_BOULDER_EAST_CELL, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(ShamanVillageCave_JumpForward)},
    {TRIGGER_STEP, GLADIATORS_AMBUSH_CELL, FLAG_SHAMAN_VILLAGE_CAVE_FOUND_BY_GLADIATORS, EVENT(ShamanVillageCave_MeetGladiators)},
    {TRIGGER_TALK, ACTOR_EDDY, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightEddy)},
    {TRIGGER_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_AZART, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, ACTOR_GOLDEN_SHIRT, CONDITION_ALWAYS, EVENT(ShamanVillageCave_TakeGoldenShirt)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_WEEDS, CONDITION_ALWAYS, EVENT(ShamanVillageCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SECOND_WEEDS, CONDITION_ALWAYS, EVENT(ShamanVillageCave_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(0), EVENT(ShamanVillageCave_LiftFirstBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 1, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(1), EVENT(ShamanVillageCave_LiftSecondBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 2, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(2), EVENT(ShamanVillageCave_LiftThirdBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_LAST_BOULDER, FLAG_SHAMAN_VILLAGE_CAVE_BOULDER_LIFTED(3), EVENT(ShamanVillageCave_LiftFourthBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_SHAMAN_VILLAGE_CAVE_PUDDLE_FROZEN, EVENT(ShamanVillageCave_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED, EVENT(ShamanVillageCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SHAMAN_VILLAGE_CAVE_PILLAR_DROPPED, EVENT(ShamanVillageCave_FinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

/// The beaten gladiators walk off south through the first room, one after
/// another.
u32 gShamanVillageCave_GladiatorLeaves[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(232, 227),
    SC_SYNC,
    SC_MOVE_XZ(217, 243),
    SC_SYNC,
    SC_MOVE_XZ(217, 334),
    SC_SYNC,
    SC_RELEASE,
};
// clang-format on

/// The gladiator behind the others steps up to their path and follows them out.
s32 gShamanVillageCave_GladiatorFollowsOut[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(224, 195),
    SC_SYNC,
    SC_GOTO(gShamanVillageCave_GladiatorLeaves),
};
