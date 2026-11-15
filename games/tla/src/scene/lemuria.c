/// @file
/// Lemuria, the hidden city beyond the Sea of Time: its doors and houses, the
/// children, the spring, the vine Growth raises, the treasure Scoop finds, and
/// the water's rippling scroll and cycling colours.

#include "system/task.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "flags.h"
#include "types.h"
#include "system/iwram_call.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/call.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
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
#include "text/msg_ids.h"
#include "field/sprite.h"
#include "field/field_psynergy_work.h"
#include "ability_ids.h"

/// Lemuria, by the rows of gActorPlacements; Piers' row goes to his party
/// slot.
enum LemuriaActorId {
  /// The two posts of the rope Lash ties.
  ACTOR_ROPE_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_POST_B,
  ACTOR_SPROUT, ///< grown into a hanging vine (FLAG_LEMURIA_VINE_GROWN)
  ACTOR_SIGNPOST,
  /// A girl at the south of the town, whom the arrival events dress as
  /// Kraden (sprite 56) to speak his lines; Lemuria_RestoreGirl puts her
  /// back.
  ACTOR_KRADEN_STAND_IN,
  /// "Lemuria has no disease, no war..."
  ACTOR_WOMAN_ON_PARADISE,
  /// "In Lemuria, we do not..."
  ACTOR_MAN_ON_LEMURIAN_WAYS,
  /// "Life never changes here..."
  ACTOR_LEMURIAN_ON_UNCHANGING_LIFE,
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
  /// The rope let down from the ledge (FLAG_LEMURIA_ROPE_LET_DOWN), as
  /// Rope_Place takes its posts: the low one, then the high one.
  ACTOR_LET_DOWN_ROPE_HIGH_POST,
  ACTOR_LET_DOWN_ROPE_LOW_POST,
  ACTOR_GIRL_BY_THE_SPRING,
  ACTOR_MAN_AT_THE_SPRING, ///< Lemuria_OfferSpringLuck
  ACTOR_SPRING             ///< Lemuria_DrinkFromSpring
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
/// Set once the Lemurian at the window has told the party the door is broken
/// and to climb through ("The door's broken, I'm afraid.").
#define FLAG_LEMURIA_TOLD_DOOR_IS_BROKEN 0x968
/// Piers away at his mother's grave, and Sheba having told why, both set in
/// Lemuria's houses (lemuria_interiors.c).
#define FLAG_PIERS_AT_MOTHERS_GRAVE 0x966
#define FLAG_TOLD_OF_PIERS_MOTHER 0x969
/// Set once the Lemurian has let a rope down from the window ("There! Is that
/// better? Now climb up here.").
#define FLAG_LEMURIA_ROPE_LET_DOWN 0x96f
/// Set as Felix is taken in to King Hydros (lemuria_palace.c); the palace door
/// is then locked while the senate meets.
#define FLAG_MET_KING_HYDROS 0x96a

void FieldCamera_WaitForArrival();

void Rope_Lash();

void Rope_Place();

void Lemuria_TakeLeaderLayer(void);
extern s32 gLemuria_FirstWalkerPaces[];
extern s32 gLemuria_SecondWalkerPaces[];
extern u16 gLemuria_DoorOpening[];
extern u16 gLemuria_WideDoorOpening[];

#include "shared/position_distance.h"

/// The fifth spot's treasure, dug up.
#define FLAG_FIFTH_SPOT_DUG 0xfbb

/// The rope Lash ties to the posts, kept tied (Rope_Place).
#define FLAG_ROPE_TIED 0x962

s32 Actor_IsFacingNear(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x.fixed;
  from = &self->x.fixed;
  if (Position_Distance(to, from) < range) {
    u32 angle = (u16)ArcTan2(other->z.fixed - self->z.fixed, *to - *from);
    u32 left = (angle - FACING_STEP) & 0xf000;
    u32 right = (angle + FACING_STEP) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = self->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}

s32 BattleEffect_GetObjectPriority(struct Actor *object);

s32 Lemuria_NoticeLeader(struct Actor *self) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u8 *state;
  u32 mode;
  struct FieldEffectWork *effect;
  struct Actor *other;

  Actor_SetSpritePriorityDirect(self, BattleEffect_GetObjectPriority(Actor_Get(
                                          gMainRoutineWork.leaderActorId)));
  mode = 0x3ff & event->psynergy;
  state = &self->paused;
  if (mode == ABILITY_MIND_READ) {
    effect = Ram_HeapSlots->fieldEffectWork;
    if ((s8)effect->messageShown != 0)
      goto near;
  }
  if (self->timer == 1)
    goto near;
  other = Actor_Get(gMainRoutineWork.leaderActorId);
  if (Position_Distance(&self->x.fixed, &other->x.fixed) <= 11)
    goto far;
  if (Actor_IsFacingNear(self, self->watchedActor, 18) != 0)
    goto near;
  if (Actor_IsFacingNear(self, Actor_Get(gMainRoutineWork.leaderActorId), 26) !=
      0)
    goto near;
far:
  *state = 0;
  Actor_SelectAnimation(self, 2);
  return 0;
near:
  *state = 1;
  Actor_SelectAnimation(self, 1);
  return 0;
}

void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
s32 MapGrid_GetAttributeBAt(s32 x, s32 z, s32 layer);

extern struct MapGridCellAnimation gLemuria_FirstDoor;
extern struct MapGridCellAnimation gLemuria_SecondDoor;
extern struct MapGridCellAnimation gLemuria_PalaceDoor;
extern struct MapGridCellAnimation gLemuria_ThirdDoor;
extern struct MapGridCellAnimation gLemuria_PalaceFrontDoor;
extern struct MapGridCellAnimation gLemuria_PalaceUpperDoor;
extern struct MapGridCellAnimation gLemuria_FourthDoor;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

void Lemuria_RestoreGirl(void) {
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 488, 1256);
  Actor_SetFirstSpriteAnimation(Actor_Get(ACTOR_KRADEN_STAND_IN), 157);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 3);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_WEST, 0);
}

void Lemuria_OpenDoorAndEnter(const struct MapGridCellAnimation *door, s32 exit,
                              s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(door->steps, door->x, door->y);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  id = gMainRoutineWork.leaderActorId;
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

/// The party leader walks into one of Lemuria's doors; the palace's is locked
/// while the council meets.
/// "The door is locked... It sounds as if there's a raucous caucus going on
/// inside..."
void Lemuria_EnterDoor(s32 exit) {
  const struct MapGridCellAnimation *door = &gLemuria_FirstDoor;
  s32 mode = 0;

  if (exit == 3 && Event_GetFlag(FLAG_MET_KING_HYDROS)) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaDoorIsLockedItSounds);
    // "The exit is locked... It sounds as if there's a raucous caucus going..."
    Event_ShowMessage(-1, 0);
    return;
  }
  if (exit == 1) {
    door = &gLemuria_FirstDoor;
    mode = 0;
  }
  if (exit == 2) {
    door = &gLemuria_SecondDoor;
    mode = 0;
  }
  if (exit == 3) {
    door = &gLemuria_PalaceDoor;
    mode = 1;
  }
  if (exit == 12) {
    door = &gLemuria_ThirdDoor;
    mode = 0;
  }
  if (exit == 11) {
    door = &gLemuria_PalaceFrontDoor;
    mode = 1;
  }
  if (exit == 10) {
    door = &gLemuria_PalaceUpperDoor;
    mode = 1;
  }
  if (exit == 16) {
    door = &gLemuria_FourthDoor;
    mode = 0;
  }
  Lemuria_OpenDoorAndEnter(door, exit, mode);
}

/// The spoiled child's talk: one message, and Actor_TurnToFaceEachOther with
/// the party leader, while no field Psynergy is pending, another while one is;
/// the child's facing is restored after.
void Lemuria_TalkToSpoiledChild(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object = Actor_Get(ACTOR_SPOILED_CHILD);
  s16 angle = object->facing;

  object->timer = 1;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (work->psynergy == 0) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaItHasBeenManyMany);
    Actor_FaceEachOther(ACTOR_SPOILED_CHILD, gMainRoutineWork.leaderActorId, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgLemuriaEveryoneSpoilsMeSoMuch);
  }
  Actor_SetAnimation(ACTOR_SPOILED_CHILD, 0);
  // "Everyone spoils me so much... I don't ever want to grow up! But in..."
  FieldEvent_ShowMessageThenWait(ACTOR_SPOILED_CHILD, 0, 10);
  Event_End();
  object->timer = 0;
  object->facing = angle;
  System_Wait(1);
}

/// The same for actor 19.
void Lemuria_TalkToLonelyChild(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object = Actor_Get(ACTOR_LONELY_CHILD);
  s16 angle = object->facing;

  object->timer = 1;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (work->psynergy == 0) {
    FieldEvent_SetMessageId((s32)&MsgLemuriaWeAreOnlyChildren);
    Actor_FaceEachOther(ACTOR_LONELY_CHILD, gMainRoutineWork.leaderActorId, 2);
  } else {
    FieldEvent_SetMessageId((s32)&MsgLemuriaAreThereOtherChildren);
  }
  Actor_SetAnimation(ACTOR_LONELY_CHILD, 0);
  // "Are there other children in the lands beyond the Sea of Time? Are..."
  FieldEvent_ShowMessageThenWait(ACTOR_LONELY_CHILD, 0, 10);
  Event_End();
  object->timer = 0;
  object->facing = angle;
  System_Wait(1);
}

static __inline__ void TileCopies_CopyTouchedCells_(s32 a, s32 b, s32 c, s32 d,
                                                    s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

void Lemuria_DigFirstSpot(void) {
  TileCopies_CopyTouchedCells_(37, 72, 2, 2, 39, 71, 2);
}

void Lemuria_DigSecondSpot(void) {
  TileCopies_CopyTouchedCells_(59, 64, 4, 3, 49, 71, 2);
}

void Lemuria_DigThirdSpot(void) {
  TileCopies_CopyTouchedCells_(52, 69, 1, 1, 53, 69, 2);
}

void Lemuria_DigFourthSpot(void) {
  TileCopies_CopyTouchedCells_(53, 64, 1, 1, 52, 64, 2);
}

void Lemuria_DigFifthSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  TileCopies_CopyTouchedCells_(59, 64, 4, 3, 49, 71, 2);
  if (Event_GetFlag(FLAG_FIFTH_SPOT_DUG) == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR + 5, 808, 1176);
    if (FIELD_CELL(leader->x.fixed) == 50 &&
        FIELD_CELL(leader->z.fixed) == 73) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 16, 0);
      leader->velocityY = px(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

void Teleport_PlaySparksAndFade(void);

void Lemuria_LeaveForAncientLemuria(void) {
  struct MainRoutineWork *party;

  Teleport_PlaySparksAndFade();
  Event_SetDestination(SCENE_LEMURIA, 2);
  party = &gMainRoutineWork;

  party->teleportState = 2;
}

void Lemuria_GrowVine(void) {
  s32 x;
  s32 z;
  s32 flag;
  s32 value;
  s32 old;

  flag = 0x900;
  /* FAKEMATCH(barrier): the empty asm on flag. */
  asm volatile("" : "+l"(flag));
  x = px(856);
  z = px(1016);
  Event_SetFlag(flag + 0x63);
  value = MapGrid_GetAttributeBAt(x, z, 0);
  /* FAKEMATCH(temporary): the first z copied to old. */
  old = z;
  z = px(1032);
  MapGrid_SetAttributeBAt(x, old, 0, value | 64);
  MapGrid_SetAttributeBAt(x, z, 0, MapGrid_GetAttributeBAt(x, z, 0) | 64);
}

/// The Scoop spots: each digs up its treasure actor and sets its flag.
void Lemuria_ScoopFirstTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR, FLAG_AREA_TEMP_00);
}

void Lemuria_ScoopSecondTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 1, FLAG_AREA_TEMP_01);
}

void Lemuria_ScoopThirdTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 2, FLAG_AREA_TEMP_02);
}

void Lemuria_ScoopFourthTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 3, FLAG_AREA_TEMP_03);
}

void Lemuria_ScoopFifthTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 4, FLAG_AREA_TEMP_04);
}

void Lemuria_ScoopSixthTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 6, FLAG_AREA_TEMP_06);
}

void Lemuria_ScoopSeventhTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 7, FLAG_AREA_TEMP_07);
}

void Lemuria_ScoopEighthTreasure(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 8, FLAG_AREA_TEMP_08);
}

s32 Party_FindOwnerWithAction();
void Rope_Coil();

/// A door that will not open, until a Lemurian inside takes Piers's companions
/// in another way.
/// "The door doesn't want to open."
void Lemuria_KnockAtBrokenDoor(void) {
  struct Actor *object;

  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed > px(96) ||
      Event_GetFlag(FLAG_TOLD_OF_PIERS_MOTHER)) {
    gFieldWork->fieldMenu = 1;
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
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgLemuriaAreYouKidsPierssTraveling);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 872, 880);
  object = Actor_Get(ACTOR_KRADEN_STAND_IN);
  Actor_SetFirstSpriteAnimation(object, 43);
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
  if (Party_FindOwnerWithAction(ABILITY_LASH) != -1 ||
      Event_GetFlag(FLAG_ROPE_TIED))
    goto done;
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
  // "You came all this way and then got stuck here? You can't be very bright."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
  Actor_SetPosition(ACTOR_LET_DOWN_ROPE_LOW_POST, 840, 920);
  Actor_SetPosition(ACTOR_LET_DOWN_ROPE_HIGH_POST, 840, 936);
  Value3((s32 (*)())Rope_Coil, 1, 28, 27);
  Rope_Lash(1, 28, 27);
  FieldEvent_Wait(30);
  // "There! Is that better? Now climb up here."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 5);
  Actor_WalkByAndWait(ACTOR_KRADEN_STAND_IN, 32, 0);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  Actor_WalkByAndWait(ACTOR_KRADEN_STAND_IN, 0, -32);
  Event_SetFlag(FLAG_LEMURIA_ROPE_LET_DOWN);
done:
  Lemuria_RestoreGirl();
  Event_End();
}

void Party_RestoreHpPp(s32 value);

/// "Felix and the others drank of the spring. Their HP and PP were fully
/// replenished!"
void Lemuria_DrinkFromSpring(void) {
  s32 y = Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed;

  if (y < px(90)) {
    // "Felix and the others drank of the spring. Their HP and PP were..."
    Message_ShowCentered((s32)MsgAndTheOthers, 1);
    Event_PlaySound(SE_RECOVERY);
    Party_RestoreHpPp(1);
  } else {
    gFieldWork->fieldMenu = 1;
  }
}

/// Actor 30 offers a test of luck at the Spring of Lemuria, once.
void Lemuria_OfferSpringLuck(void) {
  if (Event_GetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED)) {
    Actor_FaceActor(ACTOR_MAN_AT_THE_SPRING, gMainRoutineWork.leaderActorId, 0);
    FieldActor_WaitForScriptEndById(ACTOR_MAN_AT_THE_SPRING);
    FieldEvent_SetMessageId((s32)MsgLemuriaYouLookLikeYoure);
    // "You look like you're in for a spell of good luck. After all, your..."
    Event_ShowMessage(ACTOR_MAN_AT_THE_SPRING, 0);
    Actor_FaceDirectionAndWait(ACTOR_MAN_AT_THE_SPRING, FACING_NORTH_NORTHEAST);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaDoYouWishToTest);
  Actor_ShowEmote(ACTOR_MAN_AT_THE_SPRING, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  Actor_FaceActor(ACTOR_MAN_AT_THE_SPRING, gMainRoutineWork.leaderActorId, 0);
  FieldActor_WaitForScriptEndById(ACTOR_MAN_AT_THE_SPRING);
  FieldEvent_Wait(5);
  // "Do you wish to test your luck at the Spring of Lemuria?"
  Event_OpenMessage(ACTOR_MAN_AT_THE_SPRING, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(ACTOR_MAN_AT_THE_SPRING, 0, 0);
  } else {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MAN_AT_THE_SPRING, 3);
    (gFieldWork->message)++;
    FieldEvent_ShowMessageThenWait(ACTOR_MAN_AT_THE_SPRING, 0, 5);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 464, 1040);
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
    FieldEvent_Wait(5);
    Actor_SetSpeed(ACTOR_MAN_AT_THE_SPRING, 1.2);
    Actor_WalkToAndWait(ACTOR_MAN_AT_THE_SPRING, 440, 1048);
    Event_SetFlag(FLAG_LEMURIA_SPRING_LUCK_TRIED);
  }
  Actor_FaceDirectionAndWait(ACTOR_MAN_AT_THE_SPRING, FACING_NORTH_NORTHEAST);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern u8 gLemuria_RippleBuffer;
extern s16 gLemuria_RippleLines[2][160];

/// Every fourth frame rotates colours 1-14 of background palettes 7 and 8 by
/// one place. The loops are written with goto:
/// the bytes rebuild the palette address on every pass.
void Lemuria_CycleWaterColors(void) {
  if ((gFrameCount & 3) == 0) {
    volatile u16 *palette;
    volatile u16 *slot;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(7, 14);
    index = 14;
  first:
    palette = (volatile u16 *)&BG_PLTT_COLOR(7, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    slot = (volatile u16 *)&BG_PLTT_COLOR(7, 1);
    *slot = saved;
    slot += 29;
    saved = *slot;
    index = 14;
  second:
    palette = (volatile u16 *)&BG_PLTT_COLOR(8, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)&BG_PLTT_COLOR(8, 1) = saved;
  }
}

/// Puts the per-line BG2 horizontal scroll on an HBlank DMA.
void Lemuria_StartRippleDma(void) {
  s16 *lines = gLemuria_RippleLines[gLemuria_RippleBuffer];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): the empty asm reading first. */
  __asm__("" : : "r"(first));
  scroll = &REG_BG2HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= 0xc5ff; // Clear the start timing and repeat
  dma[5] &= 0x7fff; // Stop
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

void Lemuria_ComputeRipple(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  s16 *lines = gLemuria_RippleLines[gLemuria_RippleBuffer ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 sine;

  for (*count = 0; *count != DISPLAY_HEIGHT; (*count)++) {
    sine = Math_Sin((*count + (u8)gFrameCount + (layer->y >> 16)) << 9);
    v[0] = sine;
    *lines++ = (layer->x >> 16) + (sine >> 15) + *count % 3 - 1;
  }
  gLemuria_RippleBuffer ^= 1;
}

void Lemuria_StartRipple(void) {
  s32 priority;

  priority = TASK_PASS_VBLANK;
  Task_AddFrame(Lemuria_ComputeRipple, priority);
  priority = TASK_PASS_FRAME;
  Task_AddFrame(Lemuria_StartRippleDma, priority);
}

void FieldEffect_StartBlankOverlay();
void HangingVine_Place();
void Actor_SetWideSprite();

s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  FieldTask_AddFrame((s32)Lemuria_CycleWaterColors, TASK_PASS_FRAME);
  FieldEffect_StartBlankOverlay(0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SIGNPOST), 0);
  FieldActor_SetSpritePriority(ACTOR_ROPE_POST_B, 3);
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
      !Event_GetFlag(FLAG_TOLD_OF_PIERS_MOTHER)) {
    Actor_SetPosition(ACTOR_WEST_PALACE_GUARD, 456, 520);
    Actor_SetPosition(ACTOR_EAST_PALACE_GUARD, 472, 520);
  }
  if (!Event_GetFlag(FLAG_TOLD_OF_PIERS_MOTHER)) {
    Actor_SetPosition(ACTOR_GIRL_ON_LUNPA, 0, 0);
  } else {
    Actor_Get(ACTOR_GIRL_ON_LUNPA)->update = Lemuria_TakeLeaderLayer;
  }
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE) &&
      !Event_GetFlag(FLAG_TOLD_OF_PIERS_MOTHER)) {
    Actor_SetPosition(ACTOR_PIERS, 56, 1016);
    Actor_SetAnimation(ACTOR_PIERS, 9);
  }
  Rope_Initialize();
  Rope_Place(0, FLAG_ROPE_TIED, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  Actor_Get(ACTOR_ROPE_POST_A)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_ROPE_POST_B)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (Event_GetFlag(FLAG_LEMURIA_ROPE_LET_DOWN)) {
    FieldActor_SetSpritePriority(ACTOR_LET_DOWN_ROPE_LOW_POST, 2);
    FieldActor_SetSpritePriority(ACTOR_LET_DOWN_ROPE_HIGH_POST, 2);
    Actor_SetPosition(ACTOR_LET_DOWN_ROPE_LOW_POST, 840, 920);
    Actor_SetPosition(ACTOR_LET_DOWN_ROPE_HIGH_POST, 840, 936);
    Call4(Rope_Place, 1, FLAG_LEMURIA_ROPE_LET_DOWN,
          ACTOR_LET_DOWN_ROPE_LOW_POST, ACTOR_LET_DOWN_ROPE_HIGH_POST);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LET_DOWN_ROPE_LOW_POST), 0);
  if (Event_GetFlag(FLAG_LEMURIA_VINE_GROWN)) {
    HangingVine_Place(ACTOR_SPROUT, 1);
    Lemuria_GrowVine();
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_03)) {
    Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 3, 0);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_04)) {
    Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 4, 0);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_06)) {
    Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 6, 0);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_07)) {
    Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 7, 0);
  }
  if (Event_GetFlag(FLAG_PIERS_AT_MOTHERS_GRAVE)) {
    MapGrid_CopyCellsLayered(3, 1, 1, 1, 54, 45);
  }
  actor = Actor_Get(ACTOR_SPRING);
  actor->motionFlags = 0;
  actor->groundHeight = px(64);
  actor->y.fixed = px(64);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// "Oh! Goodness! Felix! Look over here! This palace is magnificient!"
void Lemuria_AdmirePalace(s32 id) {
  struct Actor *leader;
  s32 a;
  s32 b;
  s32 c;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaOh);
  Actor_SetFirstSpriteAnimation((s32)Actor_Get(ACTOR_KRADEN_STAND_IN),
                                SPRITE_KRADEN);
  if (id == ACTOR_DOG) {
    Actor_WalkToAndWait(ACTOR_FELIX, 376, 968);
    Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
    FieldActor_LaunchFromLeader(ACTOR_KRADEN_STAND_IN, 16, -16, FACING_EAST);
    a = 0;
    b = FACING_WEST + 10;
    c = FACING_SOUTH_SOUTHWEST;
  } else {
    Actor_WalkToAndWait(ACTOR_FELIX, 552, 968);
    Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
    FieldActor_LaunchFromLeader(ACTOR_KRADEN_STAND_IN, -16, -16, FACING_WEST);
    a = FACING_WEST;
    b = FACING_EAST - 10;
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
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 0, 0);
  FieldEvent_Wait(10);
  Actor_Get(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_WalkToAndWait(ACTOR_FELIX, 464, 776);
  Lemuria_RestoreGirl();
  Event_End();
}

/// "Where are you going, Felix? Let's take a longer look around!"
void Lemuria_KeepLookingAround(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaWhereAreYouGoingFelix);
  Actor_SetFirstSpriteAnimation((s32)Actor_Get(ACTOR_KRADEN_STAND_IN),
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
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -16);
  Lemuria_RestoreGirl();
  Event_End();
}

/// The party marvels at the city, and is stopped by a Lemurian who has never
/// seen it before.
/// "This place is remarkable... Truly remarkable..."
void Lemuria_MarvelAtCity(void) {
  struct Actor *leader;
  s32 result;

  Event_SetFlag(FLAG_LEMURIA_CITY_ADMIRED);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgLemuriaThisPlaceIsRemarkableTruly);
  Actor_SetSpeed(ACTOR_WEST_PALACE_GUARD, 1);
  Actor_SetSpeed(ACTOR_EAST_PALACE_GUARD, 1);
  Actor_SetFirstSpriteAnimation((s32)Actor_Get(ACTOR_KRADEN_STAND_IN),
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
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    gFieldWork->message++;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    FieldEvent_Wait(10);
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  }
  FieldActor_WatchAndSetScript(ACTOR_FELIX, 7);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN_STAND_IN, 7);
  FieldActor_WatchAndSetScript(ACTOR_WEST_PALACE_GUARD, 7);
  FieldActor_WatchAndSetScript(ACTOR_EAST_PALACE_GUARD, 7);
  FieldActor_SetSpritePriority(ACTOR_PIERS, 2);
  Actor_WalkByAndWait(ACTOR_PIERS, 32, 0);
  Actor_FaceDirection(ACTOR_WEST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_EAST_PALACE_GUARD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN_STAND_IN, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_STAND_IN, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN_STAND_IN, 6, 23);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_WEST_PALACE_GUARD, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 2);
  FieldEvent_Wait(10);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN_STAND_IN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WEST_PALACE_GUARD, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_PALACE_GUARD, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
  Actor_WalkByAndWait(ACTOR_PIERS, -16, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_WEST_PALACE_GUARD, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_PALACE_GUARD, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EAST_PALACE_GUARD, 4);
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_PALACE_GUARD, 0, 10);
  Actor_WalkByAndWait(ACTOR_PIERS, -16, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(10);
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
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_PIERS, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  FieldActor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN_STAND_IN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN_STAND_IN, 2);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN_STAND_IN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  FieldActor_WaitForMoveById(ACTOR_KRADEN_STAND_IN);
  Actor_SetPosition(ACTOR_KRADEN_STAND_IN, 0, 0);
  Lemuria_RestoreGirl();
  Event_End();
}

void Actor_TakeLeaderLayer(void);

/// An actor's per-frame hook: it is drawn on the party leader's layer.
void Lemuria_TakeLeaderLayer(void) { Actor_TakeLeaderLayer(); }

extern u16 gLemuria_ThirdDoorOpening[];

// clang-format off
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
    SCENE_EXIT_UNLESS(1, LemuriaInteriors, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaInteriors, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemuriaInteriors, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AncientLemuria, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaSpring, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, LemuriaInteriors, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemuriaInteriors, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, LemuriaInteriors, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, LemuriaPalace, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, LemuriaPalace, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, LemuriaPalace, 17, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 952, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 880, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1256, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(156, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 1040, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 1064, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(165, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 856, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(164, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 920, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(166, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 920, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(167, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 952, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(168, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 80, 0, 1104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 712, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 1080, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHICKEN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 984, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 520, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(166, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 552, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 1008, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 1016, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 488, 0, 984, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// How a door opens, as two frames of cells copied over the closed door.
u16 gLemuria_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(2, 1, 1, 1, 6),
    MAPGRID_ANIMATION_STEP(2, 2, 1, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The same for the palace's doors, two cells wide.
u16 gLemuria_WideDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(0, 1, 2, 1, 6),
    MAPGRID_ANIMATION_STEP(0, 2, 2, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The third door opens in one frame.
u16 gLemuria_ThirdDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(3, 1, 1, 1, 0),
    MAPGRID_ANIMATION_STEPS_END,
};
// clang-format on

struct MapGridCellAnimation gLemuria_FirstDoor = {(s32)gLemuria_DoorOpening, 22,
                                                  47};

struct MapGridCellAnimation gLemuria_SecondDoor = {(s32)gLemuria_DoorOpening,
                                                   35, 47};

struct MapGridCellAnimation gLemuria_PalaceDoor = {gLemuria_WideDoorOpening, 9,
                                                   40};

struct MapGridCellAnimation gLemuria_ThirdDoor = {gLemuria_ThirdDoorOpening, 54,
                                                  45};

struct MapGridCellAnimation gLemuria_PalaceFrontDoor = {
    gLemuria_WideDoorOpening, 28, 20};

struct MapGridCellAnimation gLemuria_PalaceUpperDoor = {
    gLemuria_WideDoorOpening, 28, 13};

struct MapGridCellAnimation gLemuria_FourthDoor = {(s32)gLemuria_DoorOpening,
                                                   54, 50};

struct Trigger gSceneEvents[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 5, 0x1908, 5},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), ACTOR_KRADEN_STAND_IN, 0x1966,
     EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, ACTOR_LEMURIAN_ON_UNCHANGING_LIFE, CONDITION_ALWAYS, 15},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), ACTOR_OLD_WOMAN, 0x1969,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), ACTOR_SIGNPOST, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), ACTOR_SPROUT, CONDITION_ALWAYS,
     EVENT(Lemuria_EnterDoor)},
    {TRIGGER_EXIT, ACTOR_GIRL_FORGETTING_HER_AGE, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, ACTOR_CAT, FLAG_LEMURIA_CITY_ADMIRED,
     EVENT(Lemuria_MarvelAtCity)},
    {TRIGGER_STEP, ACTOR_DOG, FLAG_LEMURIA_CITY_ADMIRED,
     EVENT(Lemuria_AdmirePalace)},
    {TRIGGER_STEP, ACTOR_COW, FLAG_LEMURIA_CITY_ADMIRED,
     EVENT(Lemuria_AdmirePalace)},
    {TRIGGER_STEP, ACTOR_CHICKEN, FLAG_LEMURIA_CITY_ADMIRED,
     EVENT(Lemuria_KeepLookingAround)},
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
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, MSGNO(MsgLemuriaSilence)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaMotherWhyHaveYouLeft)},
    {TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, 0x196a,
     MSGNO(MsgLemuriaIveNeverSeenLordConservato)},
    {TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, 0x196a,
     MSGNO(MsgLemuriaLordConservatoHasOrderedUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD, 0x196a,
     MSGNO(MsgLemuriaImSureSenateHasSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD, 0x196a,
     MSGNO(MsgLemuriaHowCouldLordConservatoExpect)},
    {TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, 0x1969,
     MSGNO(MsgLemuriaKingHydrosAwaits)},
    {TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, 0x1969,
     MSGNO(MsgLemuriaHisHighnessKingHasInstructed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD, 0x1969,
     MSGNO(MsgLemuriaTheyAreClearlyUsingPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD, 0x1969,
     MSGNO(MsgLemuriaWhatDoesHisHighnessHave)},
    {TRIGGER_TALK, ACTOR_WEST_PALACE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaKingIsHoldingCounsel)},
    {TRIGGER_TALK, ACTOR_EAST_PALACE_GUARD, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaKingWillBeThrilled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_PALACE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaHowMuchLongerWillConservato)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_PALACE_GUARD,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaIHadHeardPiers)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_LUNPA, 0x196a, MSGNO(MsgLemuriaIfYouAndLunpa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_LUNPA, 0x196a,
     MSGNO(MsgLemuriaIKnowWhyLemuriaEnded)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_LUNPA, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaSenateHoldsKingHydros)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_LUNPA,
     CONDITION_ALWAYS, MSGNO(MsgLemuriaITrustKingIfHis)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SIGNPOST, CONDITION_ALWAYS,
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
    {TRIGGER_EXAMINE, ACTOR_EAST_PALACE_GUARD, CONDITION_ALWAYS,
     EVENT(Lemuria_DrinkFromSpring)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(Lemuria_KnockAtBrokenDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, FLAG_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_LEMURIA_VINE_GROWN,
     EVENT(Lemuria_GrowVine)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(Lemuria_DigFirstSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(Lemuria_DigSecondSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32,
     CONDITION_ALWAYS, EVENT(Lemuria_DigThirdSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(Lemuria_DigFourthSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 38,
     CONDITION_ALWAYS, EVENT(Lemuria_DigFifthSpot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 104,
     FLAG_AREA_TEMP_03, EVENT(Lemuria_ScoopFourthTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 105,
     FLAG_AREA_TEMP_04, EVENT(Lemuria_ScoopFifthTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 107,
     FLAG_AREA_TEMP_06, EVENT(Lemuria_ScoopSixthTreasure)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 108,
     FLAG_AREA_TEMP_07, EVENT(Lemuria_ScoopSeventhTreasure)},
    {TRIGGER_TABLE_END},
};
