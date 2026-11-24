/// @file
/// Lemuria, the hidden city beyond the Sea of Time, which Piers leads the party
/// into. At the top of the avenue the two palace guards stop the party until
/// Piers speaks for it, and Kraden runs up from the paths above the spring to
/// admire the palace, calling Felix back if he heads down the stairs through
/// the colonnade first. The Senate, Piers's uncle's house and a girl's house
/// open off the west of the town, the Spring of Lemuria is in the middle, and
/// in the east stands Lunpa's tower, whose stuck front door sends the party up
/// a rope to the window above it. Growth on a sprout at the cliff's edge below
/// the tower grows a vine down to the cow's pasture, whose weeds Cyclone
/// scatters (one clump hides a Hard Nut), and Scoop digs up a Lucky Medal, a
/// Rusty Sword, a Bone and Star Dust about the town. The water ripples and its
/// colours cycle.

#include "system/task.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "flags.h"
#include "types.h"
#include "macros.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "field/map_grid.h"
#include "field/field_sequence_event.h"
#include "field/actor_view.h"
#include "field/actor_script.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "text/msg_ids.h"
#include "field/sprite.h"
#include "field/field_psynergy_state.h"
#include "field/field_sequence.h"
#include "scene_exits.h"
#include "ability_ids.h"

/// Lemuria, by the rows of gActorPlacements; Piers' row goes to his party
/// slot.
enum LemuriaActorId {
  /// The posts of the rope Lash ties up the east side of Lunpa's tower: the
  /// low one on the grass, where the rope lies coiled, and the high one on the
  /// tower's middle balcony.
  ACTOR_ROPE_LOW_POST = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_HIGH_POST,
  ACTOR_SPROUT, ///< grown into a hanging vine (FLAG_LEMURIA_VINE_GROWN)
  ACTOR_SIGN,   ///< "House of the Senate", below the Senate's door
  /// A girl at the south of the town, whom the events dress as Kraden to speak
  /// his lines, and as Lunpa on the top balcony of his tower;
  /// Lemuria_RestoreGirl puts her back.
  ACTOR_KRADEN_STAND_IN,
  /// "Lemuria has no disease, no war..."
  ACTOR_WOMAN_ON_PARADISE,
  /// "In Lemuria, we do not..."
  ACTOR_MAN_ON_LEMURIAN_WAYS,
  /// "Life never changes here..."
  ACTOR_LEMURIAN_ON_UNCHANGING_LIFE,
  /// "I ought to feel surprised to see outsiders here..."
  ACTOR_OLD_WOMAN,
  /// "How old am I again?"
  ACTOR_GIRL_FORGETTING_HER_AGE,
  ACTOR_SPOILED_CHILD, ///< Lemuria_TalkToSpoiledChild
  ACTOR_LONELY_CHILD,  ///< Lemuria_TalkToLonelyChild
  ACTOR_CAT,
  ACTOR_DOG,
  ACTOR_COW,
  ACTOR_CHICKEN,
  /// The two guards at the palace stair. "Who goes there!"
  ACTOR_WEST_PALACE_GUARD,
  ACTOR_EAST_PALACE_GUARD,
  /// "If you and Lunpa..."
  ACTOR_GIRL_ON_LUNPA,
  /// The rope Lunpa throws down from his tower's middle balcony
  /// (FLAG_LEMURIA_ROPE_LET_DOWN): its end on the ground, and the post at the
  /// window it lies coiled at and is thrown from (Rope_Place, Rope_Lash).
  ACTOR_LET_DOWN_ROPE_LOW_END,
  ACTOR_LET_DOWN_ROPE_HIGH_POST,
  ACTOR_GIRL_BY_THE_SPRING,
  ACTOR_MAN_AT_THE_SPRING, ///< Lemuria_OfferSpringLuck
  ACTOR_SPRING             ///< Lemuria_DrinkFromSpring
};

/// The cells whose step, examine and Psynergy triggers run Lemuria's events:
/// before the palace guards, the paths west and east of the avenue above the
/// spring, the stairs down through the colonnade, the spring's pool, Lunpa's
/// stuck front door, the clumps of weeds Cyclone scatters and the spots Scoop
/// digs. A door's or an exit's cells take the number of the exit they lead
/// through (LEMURIA_TO_), which Lemuria_EnterDoor passes to
/// FieldEvent_SetExitRequest. A Scoop spot's number is its row in the scene's
/// treasure spots and items.
enum LemuriaCellBehavior {
  CELL_BEHAVIOR_LEMURIA_BEFORE_PALACE = 20,
  CELL_BEHAVIOR_LEMURIA_WEST_PATH,
  CELL_BEHAVIOR_LEMURIA_EAST_PATH,
  CELL_BEHAVIOR_LEMURIA_COLONNADE_STAIRS,
  CELL_BEHAVIOR_LEMURIA_SPRING = 25,
  CELL_BEHAVIOR_LEMURIA_WEST_WEEDS = 30,
  CELL_BEHAVIOR_LEMURIA_PASTURE_WEEDS,
  CELL_BEHAVIOR_LEMURIA_WEEDS_SOUTH_OF_COW,
  CELL_BEHAVIOR_LEMURIA_WEEDS_NORTH_OF_COW,
  CELL_BEHAVIOR_LEMURIA_HARD_NUT_WEEDS = 38,
  CELL_BEHAVIOR_LEMURIA_STUCK_DOOR = 40,
  CELL_BEHAVIOR_LEMURIA_LUCKY_MEDAL = 104,
  CELL_BEHAVIOR_LEMURIA_RUSTY_SWORD,
  CELL_BEHAVIOR_LEMURIA_BONE = 107,
  CELL_BEHAVIOR_LEMURIA_STAR_DUST
};

/// Set once the party has taken the test of luck offered at the Spring of
/// Lemuria (Lemuria_OfferSpringLuck).
#define FLAG_LEMURIA_SPRING_LUCK_TRIED 0x908
/// Set once Growth has grown the vine (Lemuria_GrowVine); the set-up grows it
/// again on every visit.
#define FLAG_LEMURIA_VINE_GROWN 0x963
/// Set once the party has marvelled at the city ("This place is remarkable...
/// Truly remarkable...").
#define FLAG_LEMURIA_CITY_ADMIRED 0x964
/// Set once Lunpa has told the party from his balcony that his door is broken
/// and to climb through the window above it ("The door's broken, I'm afraid.").
#define FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN 0x968
/// Set once the Lemurian has let a rope down from the window ("There! Is that
/// better? Now climb up here.").
#define FLAG_LEMURIA_ROPE_LET_DOWN 0x96f

void FieldCamera_WaitForArrival();

void Rope_Lash();

void Rope_Place();

void Lemuria_TakeLeaderLayer(void);
extern s32 gLemuria_FirstWalkerPaces[];
extern s32 gLemuria_SecondWalkerPaces[];
extern u16 gLemuria_DoorOpening[];
extern u16 gLemuria_WideDoorOpening[];

#include "shared/position_distance.h"

/// Set once the party has taken the Hard Nut hidden in the pasture's weeds
/// (Lemuria's scene item 106).
#define FLAG_LEMURIA_HARD_NUT 0xfbb

/// Set once Lash has tied the rope between the low post and the high one;
/// Rope_Place keeps it tied.
#define FLAG_LEMURIA_ROPE_TIED 0x962

/// For this visit: each Scoop spot dug, by the treasure it holds. Only four
/// spots have a trigger; the Water of Life, Nut, Bramble Seed and Vial are
/// never dug.
#define FLAG_WATER_OF_LIFE_DUG_UP FLAG_AREA_TEMP_00
#define FLAG_NUT_DUG_UP FLAG_AREA_TEMP_01
#define FLAG_BRAMBLE_SEED_DUG_UP FLAG_AREA_TEMP_02
#define FLAG_LUCKY_MEDAL_DUG_UP FLAG_AREA_TEMP_03
#define FLAG_RUSTY_SWORD_DUG_UP FLAG_AREA_TEMP_04
#define FLAG_BONE_DUG_UP FLAG_AREA_TEMP_06
#define FLAG_STAR_DUST_DUG_UP FLAG_AREA_TEMP_07
#define FLAG_VIAL_DUG_UP FLAG_AREA_TEMP_08

/// The actors of Lemuria's treasure spots, in the order of its rows in
/// gSceneItems (the scene items 101 to 109): eight buried for Scoop, and the
/// Hard Nut in the pasture's weeds.
enum LemuriaTreasureActorId {
  ACTOR_BURIED_WATER_OF_LIFE = FIELD_TREASURE_ACTOR,
  ACTOR_BURIED_NUT,
  ACTOR_BURIED_BRAMBLE_SEED,
  ACTOR_BURIED_LUCKY_MEDAL,
  ACTOR_BURIED_RUSTY_SWORD,
  ACTOR_HARD_NUT,
  ACTOR_BURIED_BONE,
  ACTOR_BURIED_STAR_DUST,
  ACTOR_BURIED_VIAL
};

#include "shared/actor_is_facing_near.h"

s32 Actor_GetSpritePriority(struct Actor *actor);

/// An update for the two walking children: each stands while it is talked to,
/// while Mind Read shows its thoughts, or while it faces the other child or the
/// leader close by, and walks on otherwise or once the leader is right by it.
s32 Lemuria_NoticeLeader(struct Actor *self) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  u8 *state;
  u32 mode;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(self, Actor_GetSpritePriority(Actor_Get(
                                          gMainRoutineState.leaderActorId)));
  mode = ABILITY_ID(event->psynergy);
  state = &self->paused;
  if (mode != ABILITY_MIND_READ ||
      (s8)((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)
              ->messageShown == 0) {
    if (self->timer != 1) {
      other = Actor_Get(gMainRoutineState.leaderActorId);
      if (Position_Distance(&self->x.fixed, &other->x.fixed) <=
              CHILD_WALKS_ON_RANGE ||
          (Actor_IsFacingNear(self, self->watchedActor,
                              CHILD_SEES_CHILD_RANGE) == 0 &&
           Actor_IsFacingNear(self, Actor_Get(gMainRoutineState.leaderActorId),
                              CHILD_SEES_LEADER_RANGE) == 0)) {
        *state = 0;
        Actor_SelectAnimation(self, ANIM_WALK);
        return 0;
      }
    }
  }
  *state = 1;
  Actor_SelectAnimation(self, ANIM_STAND);
  return 0;
}

s32 MapGrid_GetCellAttributesAt(s32 x, s32 z, s32 layer);

extern struct MapGridCellAnimation gLemuria_UnclesDoor;
extern struct MapGridCellAnimation gLemuria_GirlsHouseDoor;
extern struct MapGridCellAnimation gLemuria_SenateDoor;
extern struct MapGridCellAnimation gLemuria_LunpasWindow;
extern struct MapGridCellAnimation gLemuria_PalaceFrontDoor;
extern struct MapGridCellAnimation gLemuria_PalaceUpperDoor;
extern struct MapGridCellAnimation gLemuria_LunpasDoor;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Puts the girl who stood in for Kraden or Lunpa back in her place at the
/// south of the town, as herself.
void Lemuria_RestoreGirl(void) {
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 488, 1256);
  Actor_SetFirstSpriteAnimation(Actor_Get(ACTOR_KRADEN_STAND_IN),
                                SPRITE_LEMURIAN_GIRL);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 3);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_WEST, 0);
}

/// The door opens and Felix walks in through it, lined up with it first
/// (`mode` 0) or straight ahead (`mode` 1), as the screen closes.
void Lemuria_OpenDoorAndEnter(const struct MapGridCellAnimation *door, s32 exit,
                              s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(door->steps, door->x, door->y);
  Actor_Get(gMainRoutineState.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  id = gMainRoutineState.leaderActorId;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix walks north into one of Lemuria's doors: the door opens and he goes
/// in. The doors are those of Piers's uncle's house left of the spring, cell
/// (22, 47); the girl's house, cell (35, 47); the House of the Senate in the
/// west, cell (9, 40); the window above Lunpa's door, cell (54, 45), and
/// Lunpa's own door below it, cell (54, 50); and the palace's front and upper
/// doors, cells (28, 20) and (28, 13). Once the party has met King Hydros the
/// Senate's door stays shut: "The door is locked... It sounds as if there's a
/// raucous caucus going on inside..."
void Lemuria_EnterDoor(s32 exit) {
  const struct MapGridCellAnimation *door = &gLemuria_UnclesDoor;
  s32 mode = 0;

  if (exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_C &&
      Event_GetFlag(FLAG_MET_KING_HYDROS)) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaDoorIsLockedItSounds);
    // "The door is locked... It sounds as if there's a raucous caucus going..."
    Event_ShowMessage(-1, 0);
    return;
  }
  if (exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_A) {
    door = &gLemuria_UnclesDoor;
    mode = 0;
  }
  if (exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_B) {
    door = &gLemuria_GirlsHouseDoor;
    mode = 0;
  }
  if (exit == LEMURIA_TO_LEMURIA_ROOMS_WEST_C) {
    door = &gLemuria_SenateDoor;
    mode = 1;
  }
  if (exit == LEMURIA_TO_LEMURIA_ROOMS_EAST_A) {
    door = &gLemuria_LunpasWindow;
    mode = 0;
  }
  if (exit == LEMURIA_TO_LEMURIA_PALACE_SOUTH) {
    door = &gLemuria_PalaceFrontDoor;
    mode = 1;
  }
  if (exit == LEMURIA_TO_LEMURIA_PALACE_NORTHEAST) {
    door = &gLemuria_PalaceUpperDoor;
    mode = 1;
  }
  if (exit == LEMURIA_TO_LEMURIA_ROOMS_EAST_C) {
    door = &gLemuria_LunpasDoor;
    mode = 0;
  }
  Lemuria_OpenDoorAndEnter(door, exit, mode);
}

/// Felix talks to the child who walks about with another in the west of the
/// town: the child stops and turns to him, "It has been many, many years since
/// a new child was born in Lemuria." Mind Read hears the child think,
/// "Everyone spoils me so much... I don't ever want to grow up!" Afterwards
/// the child faces the way it was walking again.
void Lemuria_TalkToSpoiledChild(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(ACTOR_SPOILED_CHILD);
  s16 angle = actor->facing;

  actor->timer = 1;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (state->psynergy == 0) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaItHasBeenManyMany);
    Actor_FaceEachOther(ACTOR_SPOILED_CHILD, gMainRoutineState.leaderActorId,
                        2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgLemuriaEveryoneSpoilsMeSoMuch);
  }
  Actor_SetAnimation(ACTOR_SPOILED_CHILD, 0);
  // "It has been many, many years since a new child was born in Lemuria." or,
  // to Mind Read, "Everyone spoils me so much... I don't ever want to grow up!"
  FieldEvent_ShowMessageThenWait(ACTOR_SPOILED_CHILD, 0, 10);
  Event_End();
  actor->timer = 0;
  actor->facing = angle;
  System_Wait(1);
}

/// Felix talks to the other walking child in the west of the town: the child
/// stops and turns to him, "We are the only children in Lemuria. I often wish
/// there were other children to play with." Mind Read hears the child wonder,
/// "Are there other children in the lands beyond the Sea of Time?"
void Lemuria_TalkToLonelyChild(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(ACTOR_LONELY_CHILD);
  s16 angle = actor->facing;

  actor->timer = 1;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (state->psynergy == 0) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaWeAreOnlyChildren);
    Actor_FaceEachOther(ACTOR_LONELY_CHILD, gMainRoutineState.leaderActorId, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgLemuriaAreThereOtherChildren);
  }
  Actor_SetAnimation(ACTOR_LONELY_CHILD, 0);
  // "We are the only children in Lemuria. I often wish there were other..." or,
  // to Mind Read, "Are there other children in the lands beyond the Sea of..."
  FieldEvent_ShowMessageThenWait(ACTOR_LONELY_CHILD, 0, 10);
  Event_End();
  actor->timer = 0;
  actor->facing = angle;
  System_Wait(1);
}

/// Felix casts Cyclone on the lone clump of weeds south-east of the spring,
/// cells (39, 67) to (40, 68): leaves whirl up and the clump is gone, leaving
/// bare grass.
void Lemuria_CycloneWestWeeds(void) {
  FieldTileCopies_CopyTouchedCells(37, 72, 2, 2, 39, 71, 2);
}

/// Felix casts Cyclone on the weeds beside the tree in the cow's pasture, below
/// the cliff under Lunpa's tower, cells (49, 67) to (52, 69): leaves whirl up
/// and the clumps are gone, leaving bare grass.
void Lemuria_CyclonePastureWeeds(void) {
  FieldTileCopies_CopyTouchedCells(59, 64, 4, 3, 49, 71, 2);
}

/// Felix casts Cyclone on the clump of weeds just south of the cow in its
/// pasture, cell (53, 65): leaves whirl up and the clump is gone.
void Lemuria_CycloneWeedsSouthOfCow(void) {
  FieldTileCopies_CopyTouchedCells(52, 69, 1, 1, 53, 69, 2);
}

/// Felix casts Cyclone on the clump of weeds just north of the cow, at the foot
/// of the cliff the vine hangs down, cell (52, 60): leaves whirl up and the
/// clump is gone.
void Lemuria_CycloneWeedsNorthOfCow(void) {
  FieldTileCopies_CopyTouchedCells(53, 64, 1, 1, 52, 64, 2);
}

/// Where the Hard Nut shows once its weeds are scattered.
enum { HARD_NUT_X = 808, HARD_NUT_Z = 1176 };

/// Felix casts Cyclone over the south of the weeds beside the tree in the
/// cow's pasture, cell (50, 69): the clumps are gone as for
/// Lemuria_CyclonePastureWeeds, and a Hard Nut lies in the grass where they
/// were until it is picked up. If Felix stands on that very spot he hops a step
/// east off it.
void Lemuria_CycloneHardNutWeeds(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);

  FieldTileCopies_CopyTouchedCells(59, 64, 4, 3, 49, 71, 2);
  if (Event_GetFlag(FLAG_LEMURIA_HARD_NUT) == 0) {
    Actor_SetPosition(ACTOR_HARD_NUT, HARD_NUT_X, HARD_NUT_Z);
    if (FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(HARD_NUT_X)) &&
        FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(HARD_NUT_Z))) {
      Event_Start();
      FieldEvent_EndLastingPsynergy(0);
      Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 16, 0);
      leader->velocityY = toFIXED(6);
      FieldActor_WaitForMoveById(gMainRoutineState.leaderActorId);
      Event_End();
    }
  }
}

void Teleport_PlaySparksAndFade(void);

/// Unused: teleports the party, with sparks and a fade, to the door of the
/// girl's house. No trigger or call of the scene reaches it.
UNUSED void Lemuria_TeleportToGirlsHouse(void) {
  struct MainRoutineState *party;

  Teleport_PlaySparksAndFade();
  Event_SetDestination(SCENE_LEMURIA, LEMURIA_FROM_LEMURIA_ROOMS_WEST_B);
  party = &gMainRoutineState;

  party->teleportState = TELEPORT_ARRIVING;
}

/// Felix casts Growth on the sprout at the cliff's edge below Lunpa's tower: a
/// vine grows down the cliff, and from then on Felix can climb it down to the
/// cow's pasture and back up. The vine stays grown.
void Lemuria_GrowVine(void) {
  FIXED x;
  FIXED z;
  s32 attribute;

  Event_SetFlag(FLAG_LEMURIA_VINE_GROWN);
  x = toFIXED(856);
  z = toFIXED(1016);
  attribute = MapGrid_GetCellAttributesAt(x, z, 0);
  MapGrid_SetCellAttributesAt(x, z, 0, attribute | MAP_CELL_CLIMBABLE);
  z = toFIXED(1032);
  MapGrid_SetCellAttributesAt(
      x, z, 0, MapGrid_GetCellAttributesAt(x, z, 0) | MAP_CELL_CLIMBABLE);
}

/// Unused: Scoop digs up the Water of Life of Lemuria's first treasure spot. No
/// trigger reaches it.
UNUSED void Lemuria_ScoopWaterOfLife(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_WATER_OF_LIFE,
                      FLAG_WATER_OF_LIFE_DUG_UP);
}

/// Unused: Scoop digs up the Nut of Lemuria's second treasure spot. No trigger
/// reaches it.
UNUSED void Lemuria_ScoopNut(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_NUT, FLAG_NUT_DUG_UP);
}

/// Unused: Scoop digs up the Bramble Seed of Lemuria's third treasure spot. No
/// trigger reaches it.
UNUSED void Lemuria_ScoopBrambleSeed(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_BRAMBLE_SEED, FLAG_BRAMBLE_SEED_DUG_UP);
}

/// Felix casts Scoop on the patch of ground right of the Spring of Lemuria,
/// where a butterfly flutters, cell (33, 61): he digs up a Lucky Medal.
void Lemuria_ScoopLuckyMedal(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_LUCKY_MEDAL, FLAG_LUCKY_MEDAL_DUG_UP);
}

/// Felix casts Scoop on the patch of ground near Lunpa's tower in the east of
/// the town, where a butterfly flutters, cell (55, 58): he digs up a Rusty
/// Sword.
void Lemuria_ScoopRustySword(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_RUSTY_SWORD, FLAG_RUSTY_SWORD_DUG_UP);
}

/// Felix casts Scoop on the patch of ground near the dog, where a butterfly
/// flutters, cell (23, 38): he digs up a Bone.
void Lemuria_ScoopBone(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_BONE, FLAG_BONE_DUG_UP);
}

/// Felix casts Scoop on the patch of ground left of the palace, where a
/// butterfly flutters, cell (18, 26): he digs up Star Dust.
void Lemuria_ScoopStarDust(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_STAR_DUST, FLAG_STAR_DUST_DUG_UP);
}

/// Unused: Scoop digs up the Vial of Lemuria's ninth treasure spot. No trigger
/// reaches it.
UNUSED void Lemuria_ScoopVial(s32 id) {
  Event_DigUpTreasure(id, ACTOR_BURIED_VIAL, FLAG_VIAL_DUG_UP);
}

s32 Party_FindOwnerWithAction();
void Rope_Coil();

/// Felix checks the front door of Lunpa's tower in the east of the town, cell
/// (54, 52): "The door doesn't want to open." Once Piers has gone to his
/// mother's grave, the first time, Lunpa looks down from the top balcony and
/// asks whether they are Piers's traveling companions: "The door's
/// broken, I'm afraid. But I opened up a window just above the door." If no
/// one in the party has Lash and the rope east of the tower is not tied, Felix
/// just looks up, and Lunpa throws a rope down from the middle balcony: "There!
/// Is that better? Now climb up here." From up on the tower, or once Lunpa
/// has burst the door open, checking it does nothing. A girl of the town stands
/// in for Lunpa.
void Lemuria_TryStuckDoor(void) {
  struct Actor *actor;

  if (Actor_Get(gMainRoutineState.leaderActorId)->y.fixed > toFIXED(96) ||
      Event_GetFlag(FLAG_MET_LUNPA)) {
    gFieldState->fieldMenu = 1;
    return;
  }
  if (!Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) ||
      Event_GetFlag(FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN)) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaDoorDoesntWantToOpen);
    // "The door doesn't want to open."
    Event_ShowMessage(-1, 0);
    return;
  }
  Event_SetFlag(FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)&MsgLemuriaAreYouKidsPierssTraveling);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 872, 880);
  actor = Actor_Get(ACTOR_KRADEN_STAND_IN);
  Actor_SetFirstSpriteAnimation(actor, SPRITE_LUNPA);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 3);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHWEST, 0);
  // "Are you kids Piers's traveling companions?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 776, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "The door's broken, I'm afraid."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 4);
  // "But I opened up a window just above the door."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 856, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  // "Sorry about this, but you'll have to climb through there."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 928, 1);
  FieldCamera_WaitForArrival();
  if (Party_FindOwnerWithAction(ABILITY_LASH) == -1 &&
      !Event_GetFlag(FLAG_LEMURIA_ROPE_TIED)) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaWhatsMatterCantYouClimb);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(ACTOR_FELIX, 856, 952);
    Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
    FieldEvent_Wait(10);
    Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    FieldCamera_MoveTo(872, CAMERA_KEEP, 792, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(20);
    Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "What's the matter? Can't you climb up? ...Fine. Just a moment."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
    FieldCamera_MoveTo(872, CAMERA_KEEP, 856, 1);
    FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 4, 0);
    Actor_WalkToAndWait(ACTOR_KRADEN_STAND_IN, 872, 912);
    FieldEvent_Wait(20);
    Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1);
    Actor_WalkByAndWait(ACTOR_KRADEN_STAND_IN, -32, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHEAST);
    FieldCamera_MoveTo(848, CAMERA_KEEP, 904, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(15);
    // "You came all this way and then got stuck here? You can't be very
    // bright."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
    Actor_SetPosition(ACTOR_LET_DOWN_ROPE_HIGH_POST, 840, 920);
    Actor_SetPosition(ACTOR_LET_DOWN_ROPE_LOW_END, 840, 936);
    Value3((s32 (*)())Rope_Coil, 1, ACTOR_LET_DOWN_ROPE_HIGH_POST,
           ACTOR_LET_DOWN_ROPE_LOW_END);
    Rope_Lash(1, ACTOR_LET_DOWN_ROPE_HIGH_POST, ACTOR_LET_DOWN_ROPE_LOW_END);
    FieldEvent_Wait(30);
    // "There! Is that better? Now climb up here."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
    Actor_WalkByAndWait(ACTOR_KRADEN_STAND_IN, 32, 0);
    EventCamera_FollowActor(ACTOR_FELIX, 1);
    Actor_WalkByAndWait(ACTOR_KRADEN_STAND_IN, 0, -32);
    Event_SetFlag(FLAG_LEMURIA_ROPE_LET_DOWN);
  }
  Lemuria_RestoreGirl();
  Event_End();
}

void Party_RestoreHpPp(s32 value);

/// Felix drinks from the spring in the middle of the town, the turtles' pool
/// beside the man who offers the test of luck, cells (27, 57) to (30, 58):
/// "Felix and the others drank of the spring. Their HP and PP were fully
/// replenished!" From up on the spring's rim nothing happens.
void Lemuria_DrinkFromSpring(void) {
  FIXED y = Actor_Get(gMainRoutineState.leaderActorId)->y.fixed;

  if (y < toFIXED(90)) {
    // "Felix and the others drank of the spring. Their HP and PP were..."
    Message_ShowCentered((s32)MsgAndTheOthers, 1);
    Event_PlaySound(SE_RECOVERY);
    Party_RestoreHpPp(1);
  } else {
    gFieldState->fieldMenu = 1;
  }
}

/// Felix talks to the man before the spring: "Do you wish to test your luck at
/// the Spring of Lemuria?" Said no, he will take a turn himself; said yes, he
/// steps aside so that the party can walk up to the spring, and from then on
/// he only wishes the party good luck.
void Lemuria_OfferSpringLuck(void) {
  if (Event_GetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED)) {
    Actor_FaceActor(ACTOR_MAN_AT_THE_SPRING, gMainRoutineState.leaderActorId,
                    0);
    FieldActor_WaitForScriptEndById(ACTOR_MAN_AT_THE_SPRING);
    FieldEvent_SetMessageId((s32)MsgLemuriaYouLookLikeYoure);
    // "You look like you're in for a spell of good luck. After all, your..."
    Event_ShowMessage(ACTOR_MAN_AT_THE_SPRING, 0);
    Actor_FaceDirectionAndWait(ACTOR_MAN_AT_THE_SPRING, FACING_NORTH_NORTHEAST);
    return;
  }
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaDoYouWishToTest);
  Actor_ShowEmote(ACTOR_MAN_AT_THE_SPRING, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  Actor_FaceActor(ACTOR_MAN_AT_THE_SPRING, gMainRoutineState.leaderActorId, 0);
  FieldActor_WaitForScriptEndById(ACTOR_MAN_AT_THE_SPRING);
  FieldEvent_Wait(5);
  // "Do you wish to test your luck at the Spring of Lemuria?"
  Event_OpenMessage(ACTOR_MAN_AT_THE_SPRING, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(30);
    // "Well, then, I'll just take a turn... If you change your mind, let me..."
    FieldEvent_ShowMessageThenWait(ACTOR_MAN_AT_THE_SPRING, 0, 0);
  } else {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MAN_AT_THE_SPRING, 3);
    (gFieldState->message)++;
    // "Go ahead. Test your luck!"
    FieldEvent_ShowMessageThenWait(ACTOR_MAN_AT_THE_SPRING, 0, 5);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 464, 1040);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
    FieldEvent_Wait(5);
    Actor_SetSpeed(ACTOR_MAN_AT_THE_SPRING, 1.2);
    Actor_WalkToAndWait(ACTOR_MAN_AT_THE_SPRING, 440, 1048);
    Event_SetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED);
  }
  Actor_FaceDirectionAndWait(ACTOR_MAN_AT_THE_SPRING, FACING_NORTH_NORTHEAST);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Which of gLemuria_RippleLines' two tables the HBlank DMA reads; the other is
/// filled for the next frame.
u8 gLemuria_RippleBuffer BSS_DATA;
/// The water's two tables of BG2 horizontal offsets, a halfword a screen line.
s16 gLemuria_RippleLines[2][160] BSS_DATA ALIGNED(16);

/// Background palette `n`, as the water's colour cycle turns it.
#define WATER_PALETTE(n) ((volatile u16 *)&BG_PLTT_COLOR(n, 0))

/// The water's cycling colours in each of its palettes, 1 to 14: all but the
/// palette's first and last.
enum { WATER_CYCLE_FIRST_COLOR = 1, WATER_CYCLE_LAST_COLOR = 14 };

/// Every fourth frame rotates the water's colours of background palettes 7 and
/// 8 by one place.
void Lemuria_CycleWaterColors(void) {
  if ((gFrameCount & 3) == 0) {
    s16 saved;
    s32 i;

    saved = WATER_PALETTE(7)[WATER_CYCLE_LAST_COLOR];
    for (i = WATER_CYCLE_LAST_COLOR; i != WATER_CYCLE_FIRST_COLOR; i--)
      WATER_PALETTE(7)[i] = WATER_PALETTE(7)[i - 1];
    WATER_PALETTE(7)[WATER_CYCLE_FIRST_COLOR] = saved;
    saved = WATER_PALETTE(8)[WATER_CYCLE_LAST_COLOR];
    for (i = WATER_CYCLE_LAST_COLOR; i != WATER_CYCLE_FIRST_COLOR; i--)
      WATER_PALETTE(8)[i] = WATER_PALETTE(8)[i - 1];
    WATER_PALETTE(8)[WATER_CYCLE_FIRST_COLOR] = saved;
  }
}

/// Puts the per-line BG2 horizontal scroll on an HBlank DMA.
void Lemuria_StartRippleDma(void) {
  s16 *lines = gLemuria_RippleLines[gLemuria_RippleBuffer];
  volatile u16 *dma;

  Io_Write16(lines[0], &REG_BG2HOFS);
  dma = REG_DMA0;

  dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma[5] &= ~DMA_ENABLE;
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

/// The ripple's sway: a sine shifted down to at most 2 pixels either way.
enum { RIPPLE_SWAY_SHIFT = 15 };

/// A task: fills the back buffer of line scrolls with the water's ripple, a
/// sine of the line and the frame, and swaps the buffers.
void Lemuria_FillRippleLines(void) {
  struct MapLayer *layer =
      &((struct MapState *)Ram_HeapSlots->mapGrid)->layers[1];
  s16 *lines = gLemuria_RippleLines[gLemuria_RippleBuffer ^ 1];
  FIXED v[2];
  s32 *count = &v[1];
  FIXED sine;

  for (*count = 0; *count != DISPLAY_HEIGHT; (*count)++) {
    sine = Math_Sin((*count + (u8)gFrameCount + FIXEDtoINT(layer->y)) << 9);
    v[0] = sine;
    *lines++ =
        FIXEDtoINT(layer->x) + (sine >> RIPPLE_SWAY_SHIFT) + *count % 3 - 1;
  }
  gLemuria_RippleBuffer ^= 1;
}

/// Starts the water's ripple: its line scrolls each VBlank and their DMA each
/// frame.
void Lemuria_StartRipple(void) {
  s32 priority;

  priority = TASK_PASS_FRAME;
  Task_AddFrame(Lemuria_FillRippleLines, priority);
  priority = TASK_PASS_VBLANK;
  Task_AddFrame(Lemuria_StartRippleDma, priority);
}

void FieldEffect_StartBlankOverlay();
void HangingVine_Place();

/// Sets up the town: the water's colours cycling, the two children walking
/// together, the palace guards, the spring's man, Piers by his mother's grave,
/// the ropes, the vine and Lunpa's opened window as the story leaves them.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldTask_AddFrame((s32)Lemuria_CycleWaterColors, TASK_PASS_VBLANK);
  FieldEffect_StartBlankOverlay(0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SIGN), 0);
  FieldActor_SetSpritePriority(ACTOR_ROPE_HIGH_POST, 3);
  FieldActor_SetSpritePriority(ACTOR_GIRL_BY_THE_SPRING, 3);
  Actor_Get(ACTOR_DOG)->update = Lemuria_TakeLeaderLayer;
  Actor_SetAnimation(ACTOR_COW, 3);
  Actor_SetPosition(ACTOR_SPOILED_CHILD, 40, 952);
  Actor_SetScript(ACTOR_SPOILED_CHILD, gLemuria_FirstWalkerPaces);
  actor = Actor_Get(ACTOR_SPOILED_CHILD);
  actor->watchedActor = Actor_Get(ACTOR_LONELY_CHILD);
  actor->update = Lemuria_NoticeLeader;
  Actor_SetPosition(ACTOR_LONELY_CHILD, 88, 904);
  Actor_SetScript(ACTOR_LONELY_CHILD, gLemuria_SecondWalkerPaces);
  actor = Actor_Get(ACTOR_LONELY_CHILD);
  actor->watchedActor = Actor_Get(ACTOR_SPOILED_CHILD);
  actor->update = Lemuria_NoticeLeader;
  if (Event_GetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED)) {
    Actor_SetPosition(ACTOR_MAN_AT_THE_SPRING, 440, 1048);
  }
  if (Event_GetFlag(FLAG_LEMURIA_CITY_ADMIRED) &&
      !Event_GetFlag(FLAG_MET_LUNPA)) {
    Actor_SetPosition(ACTOR_WEST_PALACE_GUARD, 456, 520);
    Actor_SetPosition(ACTOR_EAST_PALACE_GUARD, 472, 520);
  }
  if (!Event_GetFlag(FLAG_MET_LUNPA)) {
    Actor_SetPosition(ACTOR_GIRL_ON_LUNPA, 0, 0);
  } else {
    Actor_Get(ACTOR_GIRL_ON_LUNPA)->update = Lemuria_TakeLeaderLayer;
  }
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) &&
      !Event_GetFlag(FLAG_MET_LUNPA)) {
    Actor_SetPosition(ACTOR_PIERS, 56, 1016);
    Actor_SetAnimation(ACTOR_PIERS, 9);
  }
  Rope_Initialize();
  Rope_Place(0, FLAG_LEMURIA_ROPE_TIED, ACTOR_ROPE_LOW_POST,
             ACTOR_ROPE_HIGH_POST);
  Actor_Get(ACTOR_ROPE_LOW_POST)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_ROPE_HIGH_POST)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (Event_GetFlag(FLAG_LEMURIA_ROPE_LET_DOWN)) {
    FieldActor_SetSpritePriority(ACTOR_LET_DOWN_ROPE_HIGH_POST, 2);
    FieldActor_SetSpritePriority(ACTOR_LET_DOWN_ROPE_LOW_END, 2);
    Actor_SetPosition(ACTOR_LET_DOWN_ROPE_HIGH_POST, 840, 920);
    Actor_SetPosition(ACTOR_LET_DOWN_ROPE_LOW_END, 840, 936);
    Call4(Rope_Place, 1, FLAG_LEMURIA_ROPE_LET_DOWN,
          ACTOR_LET_DOWN_ROPE_HIGH_POST, ACTOR_LET_DOWN_ROPE_LOW_END);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LET_DOWN_ROPE_HIGH_POST), 0);
  if (Event_GetFlag(FLAG_LEMURIA_VINE_GROWN)) {
    HangingVine_Place(ACTOR_SPROUT, 1);
    Lemuria_GrowVine();
  }
  // Each treasure still buried has its actor taken off to the map's corner
  // and marked as standing for a map cell instead.
  if (!Event_GetFlag(FLAG_LUCKY_MEDAL_DUG_UP)) {
    Actor_SetWideSprite(ACTOR_BURIED_LUCKY_MEDAL, TREASURE_SPOT_PUT_AWAY);
  }
  if (!Event_GetFlag(FLAG_RUSTY_SWORD_DUG_UP)) {
    Actor_SetWideSprite(ACTOR_BURIED_RUSTY_SWORD, TREASURE_SPOT_PUT_AWAY);
  }
  if (!Event_GetFlag(FLAG_BONE_DUG_UP)) {
    Actor_SetWideSprite(ACTOR_BURIED_BONE, TREASURE_SPOT_PUT_AWAY);
  }
  if (!Event_GetFlag(FLAG_STAR_DUST_DUG_UP)) {
    Actor_SetWideSprite(ACTOR_BURIED_STAR_DUST, TREASURE_SPOT_PUT_AWAY);
  }
  // Lunpa has opened the window above his door.
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
    MapGrid_CopyCellsLayered(3, 1, 1, 1, 54, 45);
  }
  actor = Actor_Get(ACTOR_SPRING);
  actor->motionFlags = 0;
  actor->groundHeight = toFIXED(64);
  actor->y.fixed = toFIXED(64);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Felix steps onto the stone path west of the spring's avenue, cells (22, 56)
/// to (23, 57), or east of it, cells (34, 56) to (35, 57), before the party has
/// gone up to the palace: Kraden comes out, looks round, "Oh! Goodness!", and
/// runs up the avenue and the stairs through the colonnade, jumping for joy:
/// "Felix! Look over here! This palace is magnificient!" Felix follows him up
/// the stairs.
void Lemuria_AdmirePalace(s32 id) {
  struct Actor *leader;
  s32 a;
  s32 b;
  s32 c;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaOh);
  Actor_SetFirstSpriteAnimation(Actor_Get(ACTOR_KRADEN_STAND_IN),
                                SPRITE_KRADEN);
  if (id == CELL_BEHAVIOR_LEMURIA_WEST_PATH) {
    Actor_WalkToAndWait(ACTOR_FELIX, 376, 968);
    Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
    FieldActor_LaunchFromLeader(ACTOR_KRADEN_STAND_IN, 16, -16, FACING_EAST);
    a = FACING_EAST;
    b = FACING_WEST + FACING_NUDGE;
    c = FACING_SOUTH_SOUTHWEST;
  } else {
    Actor_WalkToAndWait(ACTOR_FELIX, 552, 968);
    Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
    FieldActor_LaunchFromLeader(ACTOR_KRADEN_STAND_IN, -16, -16, FACING_WEST);
    a = FACING_WEST;
    b = FACING_EAST - FACING_NUDGE;
    c = FACING_SOUTH_SOUTHEAST;
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_KRADEN_STAND_IN);
  Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1);
  EventCamera_FollowActor(ACTOR_KRADEN_STAND_IN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN_STAND_IN, 464, 952);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, a);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Oh!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, b);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Goodness!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
  Actor_WalkToAndWait(ACTOR_KRADEN_STAND_IN, 464, 784);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 23);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN_STAND_IN, c);
  FieldEvent_Wait(10);
  // "Felix! Look over here! This palace is magnificient!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
  FieldEvent_Wait(20);
  FieldActor_SetSpritePriority(ACTOR_FELIX, 1);
  Actor_SetPosition(ACTOR_FELIX, 464, 856);
  Actor_WalkToAndWait(ACTOR_FELIX, 464, 796);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 2);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 0, 0);
  FieldEvent_Wait(10);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_WalkToAndWait(ACTOR_FELIX, 464, 776);
  Lemuria_RestoreGirl();
  Event_End();
}

/// Felix heads down the stairs through the colonnade, cells (28, 45) and
/// (29, 45), before the party has gone up to the palace: Kraden comes up behind
/// him, "Where are you going, Felix? Let's take a longer look around!", and
/// Felix steps back up.
void Lemuria_KeepLookingAround(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaWhereAreYouGoingFelix);
  Actor_SetFirstSpriteAnimation(Actor_Get(ACTOR_KRADEN_STAND_IN),
                                SPRITE_KRADEN);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldActor_SetSpritePriority(ACTOR_KRADEN_STAND_IN, 1);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN_STAND_IN, 0, 16,
                              FACING_NORTH_NORTHEAST);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  FieldEvent_Wait(5);
  // "Where are you going, Felix? Let's take a longer look around!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
  Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 2);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -16);
  Lemuria_RestoreGirl();
  Event_End();
}

/// Felix walks up to the palace at the top of the avenue, cells (28, 33) and
/// (29, 33), the first time: Kraden marvels, "This place is remarkable...
/// Truly remarkable...", and the two guards at the palace stair stop the
/// party, "Who goes there! I've not seen you in Lemuria before!", until Piers
/// steps forward and asks to see King Hydros.
void Lemuria_MarvelAtCity(void) {
  struct Actor *leader;
  s32 result;

  Event_SetFlag(FLAG_LEMURIA_CITY_ADMIRED);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaThisPlaceIsRemarkableTruly);
  Actor_SetSpeed(ACTOR_WEST_PALACE_GUARD, 1);
  Actor_SetSpeed(ACTOR_EAST_PALACE_GUARD, 1);
  Actor_SetFirstSpriteAnimation(Actor_Get(ACTOR_KRADEN_STAND_IN),
                                SPRITE_KRADEN);
  Actor_WalkToAndWait(ACTOR_FELIX, 472, 568);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN_STAND_IN, -16, 0,
                              FACING_NORTH_NORTHEAST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 23);
  // "This place is remarkable... Truly remarkable..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_ShowEmote(ACTOR_WEST_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  Actor_WalkByAndWait(ACTOR_WEST_PALACE_GUARD, 16, 0);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "Who goes there! I've not seen you in Lemuria before!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_EAST_PALACE_GUARD, -16, 0);
  // "Wait a moment..."
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_WalkToAndWait(ACTOR_EAST_PALACE_GUARD, 464, 552);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(ACTOR_EAST_PALACE_GUARD, 2);
  Actor_WalkByAndWait(ACTOR_EAST_PALACE_GUARD, 16, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(ACTOR_EAST_PALACE_GUARD, -8, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 3);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(ACTOR_EAST_PALACE_GUARD, 472, 520);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "You must be the travelers who arrived with Piers. Is your name..."
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_WEST_PALACE_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Piers? You mean Piers, the Lost One?"
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -8, -16, FACING_NORTH);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Have I earned a title in my absence? Well, worry not! Piers is lost..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_WEST_PALACE_GUARD, 3);
  Actor_SetAttachedEffect(ACTOR_WEST_PALACE_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 3);
  Actor_SetAttachedEffect(ACTOR_EAST_PALACE_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(45);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I seek counsel with King Hydros. Has His Highness been told of my..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_WEST, 0);
  FieldEvent_Wait(35);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 3);
  FieldEvent_Wait(10);
  // "Word was sent, but I do not know if it has reached the king."
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What do you mean?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "Lord Conservato is meeting with His Highness as we speak."
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Not Conservato..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What's wrong, Piers? You don't look pleased to hear that name..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "Kraden... What do you think of what you have seen of Lemuria?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  60);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_KRADEN_STAND_IN);
  FieldActor_WatchAndSetScript(ACTOR_PIERS, ACTOR_KRADEN_STAND_IN);
  FieldActor_WatchAndSetScript(ACTOR_WEST_PALACE_GUARD, ACTOR_KRADEN_STAND_IN);
  FieldActor_WatchAndSetScript(ACTOR_EAST_PALACE_GUARD, ACTOR_KRADEN_STAND_IN);
  Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1);
  EventCamera_FollowActor(ACTOR_KRADEN_STAND_IN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN_STAND_IN, 424, 600);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(400, CAMERA_KEEP, 648, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(40);
  EventCamera_FollowActor(ACTOR_KRADEN_STAND_IN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN_STAND_IN, 432, 536);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 288, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 528, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
  FieldEvent_Wait(10);
  EventCamera_FollowActor(ACTOR_KRADEN_STAND_IN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN_STAND_IN, 456, 568);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "It is a magnificent city! Or do you mean the castle? Both are..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_SetSpeed(ACTOR_PIERS, 0.7);
  Actor_WalkByAndWait(ACTOR_PIERS, 8, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  // "And you, Felix? Do you agree?"
  Event_OpenMessage(ACTOR_PIERS, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result == 0) {
    FieldEvent_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    // "Any newcomer to our town would doubtless feel the same at first."
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    gFieldState->message++;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    FieldEvent_Wait(10);
    gFieldState->message++;
    // "Ah! Yes, you marvel not at its present condition, but at the greater..."
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  }
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_PIERS);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN_STAND_IN, ACTOR_PIERS);
  FieldActor_WatchAndSetScript(ACTOR_WEST_PALACE_GUARD, ACTOR_PIERS);
  FieldActor_WatchAndSetScript(ACTOR_EAST_PALACE_GUARD, ACTOR_PIERS);
  FieldActor_SetSpritePriority(ACTOR_PIERS, 2);
  Actor_WalkByAndWait(ACTOR_PIERS, 32, 0);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "In ages past, Lemuria was a far lovelier, more grand, and more lively..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(10);
  // "His Highness, the king, tells of days gone by, of a more brilliant..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Over the countless years, our people have lost vigor, enthusiasm..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Lemuria as you see it is only a shadow of the city of legend."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
  FieldEvent_Wait(10);
  // "If only I had the chance to see the city in all its splendor. But how..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "King Hydros believes that our decay is related in some way to Alchemy."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Alchemy!?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 23);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(10);
  // "King Hydros wants to find a way to stop the ruin of Lemuria."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_WEST_PALACE_GUARD, 2);
  FieldEvent_Wait(10);
  // "If there were anything we could do to help stop it, we would."
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 2);
  FieldEvent_Wait(10);
  // "But the senators are unwilling to take action! They're stubborn, and..."
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Senate? What is a senate?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "The senate is the group of elders who determine policy in Lemuria."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WEST_PALACE_GUARD, 3);
  FieldEvent_Wait(10);
  // "Lord Conservato holds the highest office in the senate."
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 2);
  FieldEvent_Wait(10);
  // "The senate usually opposes any new ideas or proposals introduced..."
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
  Actor_WalkByAndWait(ACTOR_PIERS, -16, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(10);
  // "Word of our arrival will never reach the king as long as Conservato..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WEST_PALACE_GUARD, 3);
  FieldEvent_Wait(10);
  // "And his conversations with the king are incredibly long."
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 4);
  // "We have no choice but to wait patiently here."
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_WalkByAndWait(ACTOR_PIERS, -16, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Why don't we spend our time looking around Lemuria until then..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_PIERS, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 2);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 0, 0);
  Lemuria_RestoreGirl();
  Event_End();
}

void Actor_TakeLeaderLayer(void);

/// An actor's per-frame hook: it is drawn on the party leader's layer.
void Lemuria_TakeLeaderLayer(void) { Actor_TakeLeaderLayer(); }

extern u16 gLemuria_WindowOpening[];

// clang-format off
/// The spoiled child's walk.
s32 gLemuria_FirstWalkerPaces[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_SPEED(1),                                               
      SC_ACCELERATION(1),                                               
      SC_LABEL(1),                                                             
      SC_MOVE(40, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_MOVE(40, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_MOVE(88, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_MOVE(88, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
  };

/// The lonely child's walk.
s32 gLemuria_SecondWalkerPaces[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_SPEED(1),                                               
      SC_ACCELERATION(1),                                               
      SC_LABEL(1),                                                             
      SC_MOVE(88, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_MOVE(88, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_MOVE(40, 0, 952),                                      
      SC_SYNC,                                                                 
      SC_MOVE(40, 0, 904),                                      
      SC_SYNC,                                                                 
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 460, 0, 550, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Lemuria),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_WEST_A, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_WEST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_WEST_B, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_WEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_WEST_C, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_WEST_C, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_ANCIENT_LEMURIA, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_SPRING, LemuriaSpring, LEMURIA_SPRING_FROM_LEMURIA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_EAST_A, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_EAST_B, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_ROOMS_EAST_C, LemuriaRooms, LEMURIA_ROOMS_FROM_LEMURIA_EAST_C, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_PALACE_SOUTH, LemuriaPalace, LEMURIA_PALACE_FROM_LEMURIA_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_PALACE_NORTHEAST, LemuriaPalace, LEMURIA_PALACE_FROM_LEMURIA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_TO_LEMURIA_PALACE_NORTH, LemuriaPalace, LEMURIA_PALACE_FROM_LEMURIA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 952, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 880, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1256, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 1040, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 1064, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_IN_TEAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 856, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 920, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WITH_STAFF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 920, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 952, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 80, 0, 1104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 712, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 1080, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHICKEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 984, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 520, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WITH_STAFF, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 552, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1008, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 1016, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 984, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// How a door opens, as two frames of cells copied over the closed door.
u16 gLemuria_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(2, 1, 1, 1, 6),
    MAPGRID_ANIMATION_STEP(2, 2, 1, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The same for the Senate's and the palace's doors, two cells wide.
u16 gLemuria_WideDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(0, 1, 2, 1, 6),
    MAPGRID_ANIMATION_STEP(0, 2, 2, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The window on Lunpa's balcony opens in one frame.
u16 gLemuria_WindowOpening[] = {
    MAPGRID_ANIMATION_STEP(3, 1, 1, 1, 0),
    MAPGRID_ANIMATION_STEPS_END,
};
// clang-format on

/// The opening of the door of Piers's uncle's house, at its place on the map
/// grid.
struct MapGridCellAnimation gLemuria_UnclesDoor = {gLemuria_DoorOpening, 22,
                                                   47};

/// The opening of the door of the girl's house, at its place on the map grid.
struct MapGridCellAnimation gLemuria_GirlsHouseDoor = {gLemuria_DoorOpening, 35,
                                                       47};

/// The Senate's door's opening, at its place on the map grid.
struct MapGridCellAnimation gLemuria_SenateDoor = {gLemuria_WideDoorOpening, 9,
                                                   40};

/// The opening of the window on the middle balcony of Lunpa's tower, which
/// Lunpa opened for the party, at its place on the map grid.
struct MapGridCellAnimation gLemuria_LunpasWindow = {gLemuria_WindowOpening, 54,
                                                     45};

/// The palace front door's opening, at its place on the map grid.
struct MapGridCellAnimation gLemuria_PalaceFrontDoor = {
    gLemuria_WideDoorOpening, 28, 20};

/// The palace upper door's opening, at its place on the map grid.
struct MapGridCellAnimation gLemuria_PalaceUpperDoor = {
    gLemuria_WideDoorOpening, 28, 13};

/// The opening of the front door of Lunpa's tower, at its place on the map
/// grid.
struct MapGridCellAnimation gLemuria_LunpasDoor = {gLemuria_DoorOpening, 54,
                                                   50};

struct Trigger gSceneEvents[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_WEST_A,
     CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_WEST_B,
     CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_WEST_C,
     CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, LEMURIA_TO_ANCIENT_LEMURIA,
     CONDITION_ALWAYS, LEMURIA_TO_ANCIENT_LEMURIA},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH,
     LEMURIA_TO_LEMURIA_SPRING, IF_FLAG_SET(FLAG_LEMURIA_SPRING_LUCK_TRIED),
     LEMURIA_TO_LEMURIA_SPRING},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_EAST_A,
     IF_FLAG_SET(FLAG_PIERS_AT_MOTHERS_GRAVE), EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, LEMURIA_TO_LEMURIA_ROOMS_EAST_B, CONDITION_ALWAYS,
     LEMURIA_TO_LEMURIA_ROOMS_EAST_B},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_ROOMS_EAST_C,
     IF_FLAG_SET(FLAG_MET_LUNPA), EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), LEMURIA_TO_LEMURIA_PALACE_SOUTH,
     CONDITION_ALWAYS, EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     LEMURIA_TO_LEMURIA_PALACE_NORTHEAST, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, LEMURIA_TO_LEMURIA_PALACE_NORTH, CONDITION_ALWAYS,
     LEMURIA_TO_LEMURIA_PALACE_NORTH},
    {TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_BEFORE_PALACE,
     FLAG_LEMURIA_CITY_ADMIRED, EVENT(Lemuria_MarvelAtCity)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_WEST_PATH, FLAG_LEMURIA_CITY_ADMIRED,
     EVENT(Lemuria_AdmirePalace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_EAST_PATH, FLAG_LEMURIA_CITY_ADMIRED,
     EVENT(Lemuria_AdmirePalace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LEMURIA_COLONNADE_STAIRS,
     FLAG_LEMURIA_CITY_ADMIRED, EVENT(Lemuria_KeepLookingAround)},
    {TRIGGER_TALK, ACTOR_KRADEN_STAND_IN, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaICannotRememberLastTime)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_PARADISE, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaHasNo)},
    {TRIGGER_TALK, ACTOR_MAN_ON_LEMURIAN_WAYS, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInLemuriaWeDoNot)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_ON_UNCHANGING_LIFE, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaLifeNeverChangesHere)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaIOughtToFeelSurprised)},
    {TRIGGER_TALK, ACTOR_GIRL_FORGETTING_HER_AGE, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaHowOldAmIAgain)},
    {TRIGGER_TALK, ACTOR_SPOILED_CHILD, CONDITION_ALWAYS,
     EVENT(Lemuria_TalkToSpoiledChild)},
    {TRIGGER_TALK, ACTOR_LONELY_CHILD, CONDITION_ALWAYS,
     EVENT(Lemuria_TalkToLonelyChild)},
    {TRIGGER_TALK, ACTOR_CAT, CONDITION_ALWAYS, MSGNO(MsgLemuriaMeowMrrrfle)},
    {TRIGGER_TALK, ACTOR_DOG, CONDITION_ALWAYS, MSGNO(MsgLemuriaWoofRrrrrroof)},
    {TRIGGER_TALK, ACTOR_COW, CONDITION_ALWAYS, MSGNO(MsgLemuriaMoo)},
    {TRIGGER_TALK, ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgLemuriaBukbukbuk)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgLemuriaSilence)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KRADEN_STAND_IN,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaMoreOfSameToday)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PARADISE,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaIHaventBeenSurprised)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_LEMURIAN_WAYS,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaIveAgedSoSlowly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_ON_UNCHANGING_LIFE,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaSometimesIThinkBuildingsFall)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaLongAgoManNamedLunpa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_FORGETTING_HER_AGE,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaKingHydrosIsFarOlder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPOILED_CHILD,
     CONDITION_ALWAYS, EVENT(Lemuria_TalkToSpoiledChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LONELY_CHILD, CONDITION_ALWAYS,
     EVENT(Lemuria_TalkToLonelyChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAT, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaNoOneInLemuriaFeels)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaTheresSoMuchTreasureBuried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COW, CONDITION_ALWAYS,
     MSGNO(MsgLemuriansAreSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHICKEN, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaIfItWerentForPeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaMotherWhyHaveYouLeft)},
    {TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_KING_HYDROS),
     MSGNO(MsgLemuriaIveNeverSeenLordConservato)},
    {TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_KING_HYDROS),
     MSGNO(MsgLemuriaLordConservatoHasOrderedUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
     IF_FLAG_SET(FLAG_MET_KING_HYDROS),
     MSGNO(MsgLemuriaImSureSenateHasSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
     IF_FLAG_SET(FLAG_MET_KING_HYDROS),
     MSGNO(MsgLemuriaHowCouldLordConservatoExpect)},
    {TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_LUNPA),
     MSGNO(MsgLemuriaKingHydrosAwaits)},
    {TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, IF_FLAG_SET(FLAG_MET_LUNPA),
     MSGNO(MsgLemuriaHisHighnessKingHasInstructed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
     IF_FLAG_SET(FLAG_MET_LUNPA), MSGNO(MsgLemuriaTheyAreClearlyUsingPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
     IF_FLAG_SET(FLAG_MET_LUNPA), MSGNO(MsgLemuriaWhatDoesHisHighnessHave)},
    {TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaKingIsHoldingCounsel)},
    {TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaKingWillBeThrilled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaHowMuchLongerWillConservato)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaIHadHeardPiers)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_LUNPA, IF_FLAG_SET(FLAG_MET_KING_HYDROS),
     MSGNO(MsgLemuriaIfYouAndLunpa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_LUNPA,
     IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaIKnowWhyLemuriaEnded)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_LUNPA, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaSenateHoldsKingHydros)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_LUNPA,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaITrustKingIfHis)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SIGN, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaHouseOfSenate)},
    {TRIGGER_TALK, ACTOR_GIRL_BY_THE_SPRING, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaDrinkingFromThisSpringRestores)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_BY_THE_SPRING,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaTheseTurtlesHaveLived)},
    {TRIGGER_TALK, ACTOR_MAN_AT_THE_SPRING, CONDITION_ALWAYS,
     EVENT(Lemuria_OfferSpringLuck)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_THE_SPRING,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaNoOneIsLuckierThan)},
    {TRIGGER_TALK, ACTOR_SPRING, CONDITION_ALWAYS,
     EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LEMURIA_SPRING, CONDITION_ALWAYS,
     EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LEMURIA_STUCK_DOOR, CONDITION_ALWAYS,
     EVENT(Lemuria_TryStuckDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_LOW_POST,
     FLAG_LEMURIA_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_LEMURIA_VINE_GROWN,
     EVENT(Lemuria_GrowVine)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_WEST_WEEDS, CONDITION_ALWAYS,
     EVENT(Lemuria_CycloneWestWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_PASTURE_WEEDS, CONDITION_ALWAYS,
     EVENT(Lemuria_CyclonePastureWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_WEEDS_SOUTH_OF_COW, CONDITION_ALWAYS,
     EVENT(Lemuria_CycloneWeedsSouthOfCow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_WEEDS_NORTH_OF_COW, CONDITION_ALWAYS,
     EVENT(Lemuria_CycloneWeedsNorthOfCow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_HARD_NUT_WEEDS, CONDITION_ALWAYS,
     EVENT(Lemuria_CycloneHardNutWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_LUCKY_MEDAL, FLAG_LUCKY_MEDAL_DUG_UP,
     EVENT(Lemuria_ScoopLuckyMedal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_RUSTY_SWORD, FLAG_RUSTY_SWORD_DUG_UP,
     EVENT(Lemuria_ScoopRustySword)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_BONE, FLAG_BONE_DUG_UP, EVENT(Lemuria_ScoopBone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEMURIA_STAR_DUST, FLAG_STAR_DUST_DUG_UP,
     EVENT(Lemuria_ScoopStarDust)},
    {TRIGGER_TABLE_END},
};
