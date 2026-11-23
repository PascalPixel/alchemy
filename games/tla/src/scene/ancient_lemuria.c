/// @file
/// Ancient Lemuria, the ruined shore before the hidden city, where the
/// party lands after getting past Poseidon.
///
/// From the boat dock at the south, the party climbs the shore. Move pushes
/// a blue statue onto its spot by the path north, cell (8, 36), and the
/// Psynergy brings out two Lemurians, who wonder whether this is the party
/// that defeated Poseidon and, seeing Piers, hurry back into the city
/// (AncientLemuria_MeetTheLemurians). Cyclone clears six patches of weeds;
/// under the first is Rime, the Mercury Djinni, which hops off and hides in
/// another blue statue until Tremor shakes it out
/// (AncientLemuria_CycloneRevealsRime, AncientLemuria_TremorShakesOutRime).
/// Walking back to the dock before King Hydros is met, Kraden calls Felix
/// back to keep exploring.

#include "system/task.h"
#include "types.h"
#include "system/io_reg.h"
#include "flags.h"
#include "djinn_ids.h"
#include "field/map_grid.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "field/field_event.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/event_layer.h"

void AncientLemuria_StillTheStatue(s32 id);
void AncientLemuria_MeetTheLemurians(void);
void Field_TryJumpForward(void);

#include "text/msg_ids.h"
/// The shore's water: its two background palettes, from the first, the colours
/// of each that rotate every four frames, the colour of the first that shines
/// in the colours of gAncientLemuria_ShiningWaterColors in turn, and how many
/// there are.
#define FIRST_SHORE_WATER_PALETTE 7
#define SHORE_WATER_FIRST_COLOR 1
#define SHORE_WATER_LAST_COLOR 14
#define WATER_SHINING_COLOR 15
#define SHINING_WATER_COLORS 12

extern s16 gAncientLemuria_ShiningWaterColors[SHINING_WATER_COLORS];

/// The frames the statue Rime hides in takes to rock from side to side and
/// back, clicking as each rock starts.
#define RIME_STATUE_ROCK_FRAMES 16

/// Rime's story: brought out of the first weeds by Cyclone, hiding in the blue
/// statue, which rocks, until Tremor shakes it out.
#define FLAG_RIME_REVEALED 0x96c
#define FLAG_RIME_OUT_OF_THE_STATUE 0x96d
#define FLAG_RIME_IN_THE_STATUE 0x96e

/// Set once Move has pushed the blue statue onto cell (8, 36), where the
/// Lemurians see the Psynergy and come out.
#define FLAG_BLUE_STATUE_MOVED 0x961

/// The shore's actors, numbering gActorPlacements' rows that are not the
/// party's.
enum AncientLemuriaActorId {
  /// The blue statue Move pushes along the shore.
  ACTOR_BLUE_STATUE = ACTOR_FIRST_PLACED,
  /// The Lemurians who meet the party (AncientLemuria_MeetTheLemurians).
  ///@{
  ACTOR_LEMURIAN,
  ACTOR_SECOND_LEMURIAN,
  ///@}
  ACTOR_KRADEN,
  ACTOR_RIME,
  /// The blue statue Rime hides in until Tremor shakes it out.
  ACTOR_RIMES_STATUE
};

/// Felix talks to Rime once Tremor has shaken it out of its statue, and it
/// joins the party.
void AncientLemuria_RimeJoins(void) {
  Djinn_JoinParty(ACTOR_RIME, DJINNI_RIME / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_RIME));
}

/// The statue Rime hides in rocks from side to side, clicking as each rock
/// starts.
void AncientLemuria_RockTheStatue(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;
  s16 time = actor->timer++;

  sprite->rotation = Iwram_MulFX(
      Trig_Sin(time * (TRIG_TURN / RIME_STATUE_ROCK_FRAMES)), TRIG_TURN / 256);
  if ((time & (RIME_STATUE_ROCK_FRAMES - 1)) == 0)
    Sound_Play(SE_SWITCH);
}

/// The rocking statue stands still.
void AncientLemuria_StillTheStatue(s32 id) {
  struct Actor *actor = Actor_GetById(id);
  struct FieldSprite *sprite = actor->sprite;

  actor->update = 0;
  actor->timer = 0;
  sprite->rotation = 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Felix walks north across the boat dock towards the gangway, over cells
/// (8, 52) to (12, 52): the game stops the view scrolling north. Setting out
/// from the dock's south edge, the view follows him only a step or so (some
/// 18 pixels) and then stays put, and he climbs the screen until he steps
/// onto the gangway; walking back south, the view comes back down with him.
void AncientLemuria_KeepCameraSouth(void) {
  ((struct MapWork *)Ram_HeapSlots->mapGrid)->minZ = toFIXED(750);
}

/// Nothing visible happens as Felix walks over the next row, cells (8, 53) to
/// (12, 53); the view may scroll north over the whole shore again.
void AncientLemuria_LetCameraNorth(void) {
  ((struct MapWork *)Ram_HeapSlots->mapGrid)->minZ = toFIXED(352);
}

/// Nothing more than the move itself happens as Felix starts casting Move on
/// the blue statue by the path north.
void AncientLemuria_StartMovingStatue(void) {}

/// Felix moves the blue statue by the path north with Move. Set down on its
/// spot beside the ledge, cell (8, 36), it stays there, and the two Lemurians
/// come out to meet the party (AncientLemuria_MeetTheLemurians).
void AncientLemuria_FinishMovingStatue(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (x == 8 && z == 36)
    GameFlag_Set(FLAG_BLUE_STATUE_MOVED);
  if (GameFlag_Get(FLAG_BLUE_STATUE_MOVED))
    AncientLemuria_MeetTheLemurians();
}

/// One of Rime's hops, by (dx, dz).
static inline void Hop(s32 dx, s32 dz) {
  Actor_Jump(ACTOR_RIME, 5, 0);
  Actor_WalkByAndWaitById(ACTOR_RIME, dx, dz);
  Event_Wait(3);
}

/// Felix casts Cyclone on the first patch of weeds (cell (5, 13)): the weeds
/// blow away and, the first time, Rime pops out of the ground, spinning as it
/// grows, then hops off east with Felix turning to watch it and hides in a blue
/// statue, which starts to rock.
void AncientLemuria_CycloneRevealsRime(void) {
  struct Actor *actor;
  s32 frame;

  FieldTileCopies_CopyTouchedCells(7, 13, 1, 1, 5, 13, 0);
  if (Event_GetFlag(FLAG_RIME_REVEALED))
    return;
  actor = Actor_GetById(ACTOR_RIME);
  Event_SetFlag(FLAG_RIME_REVEALED);
  Event_SetFlag(FLAG_RIME_IN_THE_STATUE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIME), 0);
  Actor_SetPosition(ACTOR_RIME, 88, 216);
  Actor_Jump(ACTOR_RIME, 9, 0);
  actor->scaleY = FIXED_ONE / 2;
  actor->scaleX = FIXED_ONE / 2;
  for (frame = 0; frame < 30; frame++) {
    actor->facing += TRIG_TURN / 16;
    if (actor->scaleX < FIXED_ONE)
      actor->scaleX += FIXED_ONE * 5 / 128;
    actor->scaleY = actor->scaleX;
    wait(1);
  }
  actor->facing = FACING_SOUTH_SOUTHEAST;
  actor->scaleY = FIXED_ONE;
  actor->scaleX = FIXED_ONE;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIME), 1);
  Actor_SetSpritePriority(ACTOR_RIME, 1);
  Event_Wait(8);
  Actor_Jump(ACTOR_RIME, 9, 0);
  Actor_WalkToAndWaitById(ACTOR_RIME, 120, 200);
  Actor_GetById(ACTOR_RIME)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_WalkToAndWaitById(ACTOR_RIME, 136, 200);
  Event_Wait(3);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Hop(0, 16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Actor_SetPosition(ACTOR_RIME, 0, 0);
  Actor_GetById(ACTOR_RIMES_STATUE)->update = AncientLemuria_RockTheStatue;
  Actor_GetById(ACTOR_RIMES_STATUE)->timer = 0;
}

/// Felix casts Cyclone on the second patch of weeds (cell (8, 9)), and
/// they blow away.
void AncientLemuria_CycloneSecondWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 9, 2, 3, 8, 9, 0);
}

/// Felix casts Cyclone on the third patch of weeds (cell (12, 7)), and
/// they blow away.
void AncientLemuria_CycloneThirdWeeds(void) {
  FieldTileCopies_CopyTouchedCells(38, 0, 5, 2, 12, 7, 0);
}

/// Felix casts Cyclone on the fourth patch of weeds (cell (14, 11)), and they
/// blow away.
void AncientLemuria_CycloneFourthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(39, 2, 4, 3, 14, 11, 0);
}

/// Felix casts Cyclone on the fifth patch of weeds (cell (21, 9)), and they
/// blow away.
void AncientLemuria_CycloneFifthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(39, 2, 3, 3, 21, 9, 0);
}

/// Felix casts Cyclone on the sixth patch of weeds (cell (25, 8)), and they
/// blow away.
void AncientLemuria_CycloneSixthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(39, 0, 4, 3, 25, 8, 0);
}

/// As Felix starts casting Tremor on the rocking statue Rime hides in, it
/// stands still.
void AncientLemuria_StartTremorOnTheStatue(void) {
  AncientLemuria_StillTheStatue(ACTOR_RIMES_STATUE);
}

/// Tremor on the shaking statue, once Rime hides in it: Rime is thrown out,
/// spinning as it grows back to its full size.
void AncientLemuria_TremorShakesOutRime(void) {
  struct Actor *actor;
  s32 frame;

  if (Event_GetFlag(FLAG_RIME_OUT_OF_THE_STATUE))
    return;
  if (!GameFlag_Get(FLAG_RIME_REVEALED))
    return;
  actor = Actor_GetById(ACTOR_RIME);
  GameFlag_Set(FLAG_RIME_OUT_OF_THE_STATUE);
  GameFlag_Clear(FLAG_RIME_IN_THE_STATUE);
  AncientLemuria_StillTheStatue(ACTOR_RIMES_STATUE);
  Actor_SetPosition(ACTOR_RIME, 270, 184);
  actor->y.fixed = toFIXED(22);
  Actor_Jump(ACTOR_RIME, 11, 0);
  actor->scaleY = FIXED_ONE / 2;
  actor->scaleX = FIXED_ONE / 2;
  Actor_SetSpeed(ACTOR_RIME, 2.2);
  Actor_WalkTo(ACTOR_RIME, 312, 184);
  for (frame = 0; frame < 24; frame++) {
    actor->facing += TRIG_TURN / 8;
    if (actor->scaleX < FIXED_ONE)
      actor->scaleX += FIXED_ONE * 5 / 128;
    actor->scaleY = actor->scaleX;
    wait(1);
  }
  actor->facing = FACING_SOUTH_SOUTHEAST;
  actor->scaleY = FIXED_ONE;
  actor->scaleX = FIXED_ONE;
  Actor_WaitForMoveById(ACTOR_RIME);
}

/// Felix walks south onto the boat dock: once the Lemurians have met the
/// party and until King Hydros is met, Kraden calls him back to keep
/// exploring; otherwise the party leaves Lemuria by boat.
void AncientLemuria_HeadForTheDock(void) {
  struct Actor *actor;

  if (!GameFlag_Get(FLAG_BLUE_STATUE_MOVED) ||
      GameFlag_Get(FLAG_MET_KING_HYDROS)) {
    Sound_Play(SE_MAP_EXIT);
    Import_EventSetExitRequest(ANCIENT_LEMURIA_TO_LEMURIA_DOCK_EAST_A);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH_SOUTHWEST);
    Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Event_Wait(20);
    if (GameFlag_Get(FLAG_IN_PARTY(ACTOR_PIERS))) {
      Event_SetMessageId(MsgLemuriaStayExploring);
      // "Lost already, Felix? That's the way to the boat dock! You can't..."
      Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    } else {
      Event_SetMessageId(MsgLemuriaStayExploringAlternate);
      // "Lost already, Felix? That's the way to the boat dock! You can't..."
      Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    }
    Actor_SetSpeed(ACTOR_KRADEN, 1.2);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
    actor = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (actor != NULL)
      Actor_SetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_WalkToAndWait(ACTOR_FELIX, 168, 832);
    Event_End();
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// The water's shine: every four frames the rotating colours of both water
/// palettes move on by one, and the shining colour takes the next of
/// gAncientLemuria_ShiningWaterColors.
s32 AncientLemuria_CycleWaterColors(void) {
  if ((*Ram_FrameCount & 3) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, SHORE_WATER_LAST_COLOR);
    for (index = SHORE_WATER_LAST_COLOR; index != SHORE_WATER_FIRST_COLOR;
         index--)
      BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, index) =
          BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, index - 1);
    BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, SHORE_WATER_FIRST_COLOR) = saved;
    saved =
        BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, SHORE_WATER_LAST_COLOR);
    for (index = SHORE_WATER_LAST_COLOR; index != SHORE_WATER_FIRST_COLOR;
         index--)
      BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, index) =
          BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, index - 1);
    BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE + 1, SHORE_WATER_FIRST_COLOR) =
        saved;
  }
  {
    s16 color = gAncientLemuria_ShiningWaterColors[(*Ram_FrameCount >> 2) %
                                                   SHINING_WATER_COLORS];

    BG_PLTT_COLOR(FIRST_SHORE_WATER_PALETTE, WATER_SHINING_COLOR) = color;
    return color;
  }
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Task_Add(AncientLemuria_CycleWaterColors, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance != 1)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  else
    GameFlag_Set(FLAG_SHOW_AREA_NAME);
  if (GameFlag_Get(FLAG_BLUE_STATUE_MOVED)) {
    Actor_SetPosition(ACTOR_BLUE_STATUE, 136, 584);
    Actor_SetPosition(ACTOR_LEMURIAN, 0, 0);
    Actor_SetPosition(ACTOR_SECOND_LEMURIAN, 0, 0);
  }
  if (Event_GetFlag(FLAG_RIME_OUT_OF_THE_STATUE))
    Actor_SetPosition(ACTOR_RIME, 312, 184);
  if (gMainRoutineWork.entrance == 1 &&
      !GameFlag_Get(FLAG_RIME_OUT_OF_THE_STATUE) &&
      GameFlag_Get(FLAG_RIME_REVEALED)) {
    Actor_GetById(ACTOR_RIMES_STATUE)->update = AncientLemuria_RockTheStatue;
    Actor_GetById(ACTOR_RIMES_STATUE)->timer = 0;
  }
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIMES_STATUE), 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 entrance = gMainRoutineWork.entrance;

  if (entrance == 1)
    GameFlag_Set(FLAG_LEMURIA_OTHER_WAY_IN);
  else if (entrance == ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_A)
    GameFlag_Clear(FLAG_LEMURIA_OTHER_WAY_IN);
  else if (entrance == ANCIENT_LEMURIA_FROM_LEMURIA)
    GameFlag_Clear(FLAG_LEMURIA_OTHER_WAY_IN);
  else if (entrance == 4)
    GameFlag_Set(FLAG_LEMURIA_OTHER_WAY_IN);
  else if (entrance == ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_B)
    GameFlag_Set(FLAG_LEMURIA_OTHER_WAY_IN);
  if (GameFlag_Get(FLAG_LEMURIA_OTHER_WAY_IN)) {
    MapGrid_CopyCellPictureCellsRect(38, 47, 26, 12, 8, 47);
    MapGrid_CopyCellPictureCellsRect(102, 47, 26, 12, 72, 47);
    Map_CopyCellAttributeRect(38, 47, 26, 12, 8, 47);
  }
  return 0;
}

/// Felix walks east off the ledge beside the blue statue's spot (cell
/// (9, 36)) and jumps across, if nothing stands in the way.
void AncientLemuria_JumpForward(void) { Field_TryJumpForward(); }

void Actor_PlaceOnActor();

/// The two Lemurians who saw the Psynergy come out, wonder whether the party
/// are the ones who defeated Poseidon and, seeing Piers, hurry back into the
/// city ahead of the party.
void AncientLemuria_MeetTheLemurians(void) {
  struct Actor *leader;
  s32 result;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaRecognizePsynergy);
  // "Why, that was Psynergy, wasn't it?"
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Are they Adepts?"
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Field_TryJumpForward();
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_FELIX, ANIM_STAND);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -16);
  Event_Wait(10);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 536, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_LEMURIAN, 0.8);
  Actor_WalkByAndWaitById(ACTOR_LEMURIAN, 24, 0);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_LEMURIAN, 2);
  Event_Wait(10);
  // "Who are you!?"
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_SetSpeed(ACTOR_SECOND_LEMURIAN, 0.8);
  Actor_WalkByAndWait(ACTOR_SECOND_LEMURIAN, -24, 0);
  Actor_FaceDirectionById(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_LEMURIAN, 2);
  Event_Wait(10);
  // "Are you the ones who defeated Poseidon?"
  Event_OpenMessage(ACTOR_SECOND_LEMURIAN, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result == 0) {
    Event_Wait(30);
    Actor_StartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
    FieldEffect_SetAttached(ACTOR_SECOND_LEMURIAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    // "That's quite something! Who are you, that you could defeat that..."
    Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
    FieldEffect_SetAttached(ACTOR_SECOND_LEMURIAN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    gFieldWork->message++;
    // "But clearly, you are here, and the beast is gone..."
    Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 0);
    Actor_Jump(ACTOR_SECOND_LEMURIAN, 6, 15);
    Actor_Jump(ACTOR_SECOND_LEMURIAN, 6, 23);
    Event_Wait(10);
  }
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "If you are strong enough to defeat Poseidon, you are too dangerous to..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "The king's orders are clear! We cannot grant you entry."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, -16, FACING_NORTH_NORTHWEST);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "King? Who is your king?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "His Highness, King Hydros, of course..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_LEMURIAN, 2);
  Event_Wait(10);
  // "His Majesty has ruled over Lemuria for centuries."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "King Hydros has decreed that Lemuria must be kept safe from dangerous..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_PlaceOnActor(ACTOR_PIERS, ACTOR_FELIX);
  Actor_GetById(ACTOR_PIERS)->scaleX = 0;
  // "You say that we are dangerous, but do you include me as well?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
  Event_Wait(45);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  Actor_GetById(ACTOR_PIERS)->scaleX = FIXED_ONE;
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 0, -16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Piers!"
  FieldMessage_ShowAtActor(ACTOR_LEMURIAN, 0);
  Event_Wait(20);
  // "Piers!"
  Event_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_SECOND_LEMURIAN, 0, 0);
  Message_CloseFinishedWindows();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_WalkByAndWait(ACTOR_LEMURIAN, -8, 8);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_SECOND_LEMURIAN, -8, 8);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "Did you join them in battle against Poseidon?"
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_SHAKE_HEAD);
  // "Impossible! Everyone knows you possess great courage, but you still..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Such comments do us all a great disservice! I battled fiercely..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(25);
  Actor_StartRepeatedMotion(ACTOR_LEMURIAN, 3);
  Actor_SetAttachedEffect(ACTOR_LEMURIAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
  Actor_SetAttachedEffect(ACTOR_SECOND_LEMURIAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  // "Leave our landing to me."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, -16);
  Event_Wait(10);
  // "You have my word that we shall create no problems during our stay..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_LEMURIAN, 2);
  Actor_RunRepeatedMotion(ACTOR_SECOND_LEMURIAN, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Piers, I know you are a man of your word, but the king's orders must..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You understand that we cannot simply ignore the king's orders, don't..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "What is King Hydros doing..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "So, you people really take your kings seriously, don't you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_Jump(ACTOR_LEMURIAN, 6, 15);
  Actor_Jump(ACTOR_LEMURIAN, 6, 23);
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Insolent pup! Do not dishonor the king of Lemuria! We are peaceful..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_Jump(ACTOR_KRADEN, 6, 15);
  Actor_Jump(ACTOR_KRADEN, 6, 23);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Pup? I must be twice your age! Who are you calling "pup"?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You wish to see Lemuria, and yet you know so little of us! I am far..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Pah! Older than me?! Inconceivable!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "Er... Actually, Kraden, these two are perhaps older than all of you..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  // "Babi must have spoken of this to you. In Lemuria, time passes quite..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Ah... Well... I suppose I recall something of that sort or another..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  // "But perhaps I was rude, and I hope you accept my apology..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "Well, just so long as we're clear on that."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
  Event_Wait(20);
  // "But if your fighting is as skilled as your wordplay, then perhaps you..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_FaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "I agree. Perhaps His Majesty will provide an exception in their case."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "King Hydros might be willing to reverse his order if we confer with..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "We shall speak with him at once!"
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "King Hydros has been seeking news from beyond the Sea of Time. He..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 55);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "Wait here."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_LEMURIAN, 0, -16);
  Actor_WalkByAndWait(ACTOR_SECOND_LEMURIAN, 0, -16);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // "How long do you think we'll be waiting?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Event_Wait(20);
  // "Good point. Time means little to most Lemurians. Mere moments to us..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_LEMURIAN, 3);
  Actor_SetAttachedEffect(ACTOR_LEMURIAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_SECOND_LEMURIAN, 3);
  Actor_SetAttachedEffect(ACTOR_SECOND_LEMURIAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 512, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "You seem young to me, and yet you are probably an old man among your..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 55);
  Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "King Hydros instructed us to be kind to the elderly at all times."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_FaceDirectionById(ACTOR_LEMURIAN, FACING_EAST, 0);
  Event_Wait(20);
  // "They seem trustworthy enough... Shall we let them in?"
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_WEST, 0);
  Event_Wait(20);
  // "I can see no reason not to. We won't get in trouble, will we?"
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_FELIX, 2);
  Actor_WalkByAndWait(ACTOR_FELIX, 8, -20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I will be with them! Now, please, let me walk the streets of my home..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  // "I beseech you!"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LEMURIAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_LEMURIAN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "Then we place our trust in each of you. You may enter. But...don't do..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "If you cause any trouble, we'll hunt you down and exact swift and..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LEMURIAN, 2);
  Event_Wait(10);
  // "Do we have your word?"
  FieldMessage_ShowAtActor(ACTOR_LEMURIAN, 0);
  if ((s8)gMainRoutineWork.weirdAnswers <= 4) {
    result = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
    if (result == 0) {
      Event_Wait(25);
      Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
      Event_Wait(30);
      Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
      Event_Wait(10);
      // "Ooo! Ooo! Finally! What could be waiting for me in Lemuria? I..."
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
      gFieldWork->message++;
    } else {
      Event_Wait(35);
      Actor_ShowEmote(ACTOR_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_SECOND_LEMURIAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
      Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
      Event_Wait(30);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      gFieldWork->message++;
      // "We've come a long way to see Lemuria. I won't have anything ruin..."
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
    }
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      Event_Wait(25);
      Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
      Actor_FaceDirectionAndRefreshById(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST);
      Event_Wait(10);
      Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
      Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
      Event_Wait(10);
      // "But thank you for speaking honestly. Sometimes, you can say the..."
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
      gFieldWork->message++;
    } else {
      Event_Wait(35);
      Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
      Actor_FaceDirectionAndRefreshById(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST);
      Event_Wait(10);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
      Actor_Jump(ACTOR_KRADEN, 6, 15);
      Actor_Jump(ACTOR_KRADEN, 6, 23);
      gFieldWork->message++;
      // "What!? Are you INSANE!? Or maybe you think you're funny? Because..."
      Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
    }
    Actor_FaceDirectionAndRefreshById(ACTOR_PIERS, FACING_NORTH);
    Event_Wait(10);
  }
  Actor_WalkByAndWait(ACTOR_PIERS, 0, -10);
  Event_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  // "Oh, hush. I'll keep a close eye on them. You have nothing to fear."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
  Event_Wait(10);
  // "Very well, Piers. We place our faith in you, then. Enter freely and..."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LEMURIAN, 2);
  Event_Wait(10);
  // "You must already be aware of this, but you are not permitted to enter..."
  Event_ShowMessageThenWait(ACTOR_LEMURIAN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_SHAKE_HEAD);
  // "If the king were to find out about this, we'd be in big trouble."
  Event_ShowMessageThenWait(ACTOR_SECOND_LEMURIAN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Don't worry! I won't let that happen."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationById(ACTOR_LEMURIAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_LEMURIAN, ANIM_NOD);
  Event_Wait(25);
  FieldCamera_FollowActor(ACTOR_SECOND_LEMURIAN, 1);
  Actor_WalkBy(ACTOR_SECOND_LEMURIAN, 0, -32);
  Event_Wait(10);
  Actor_WalkByAndWaitById(ACTOR_LEMURIAN, 16, 0);
  Actor_WalkBy(ACTOR_LEMURIAN, 0, -32);
  Actor_WaitForMoveById(ACTOR_SECOND_LEMURIAN);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetPosition(ACTOR_SECOND_LEMURIAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_LEMURIAN);
  Actor_SetPosition(ACTOR_LEMURIAN, 0, 0);
  Event_Wait(30);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 536, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(20);
  // "Is this Hydros really so fearsome?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Fearsome isn't the right word... King Hydros has lived longer than..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "While everyone respects him and holds him in great reverence... It..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(10);
  // "I certainly do not fear him. The king... He..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "He what?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "You will understand once you meet the king yourself."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, -32);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  // "But that does not matter now. Let's go!"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "To Lemuria... The city you've all been longing to see..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_FELIX, -8, -16);
  Actor_WalkBy(ACTOR_KRADEN, 0, -10);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -16);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AncientLemuria),
    SCENE_EXIT_UNLESS(1, AncientLemuria, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, LemuriaDock, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Lemuria, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AncientLemuria, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    152, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    136, 0, 504, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    200, 0, 504, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    {SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -toFIXED(56), 0,
     -toFIXED(56), FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -toFIXED(56), 0,
     -toFIXED(56), FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_RIME), MOVEMENT_TYPE_STAND,
     -toFIXED(56), 0, -toFIXED(56), FACING_SOUTH, TALK_FACING_PARTY, 0},
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    264, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(AncientLemuria_HeadForTheDock)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(AncientLemuria_KeepCameraSouth)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(AncientLemuria_LetCameraNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS,
     EVENT(AncientLemuria_JumpForward)},
    {TRIGGER_TALK, ACTOR_RIME, CONDITION_ALWAYS,
     EVENT(AncientLemuria_RimeJoins)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_BLUE_STATUE, FLAG_BLUE_STATUE_MOVED,
     (u32)(AncientLemuria_StartMovingStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BLUE_STATUE, FLAG_BLUE_STATUE_MOVED,
     EVENT(AncientLemuria_FinishMovingStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneRevealsRime)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneSecondWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneThirdWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneFourthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneFifthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 35,
     CONDITION_ALWAYS, EVENT(AncientLemuria_CycloneSixthWeeds)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_TREMOR, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RIMES_STATUE, IF_FLAG_SET(FLAG_RIME_IN_THE_STATUE),
     EVENT(AncientLemuria_StartTremorOnTheStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_RIMES_STATUE,
     IF_FLAG_SET(FLAG_RIME_IN_THE_STATUE),
     EVENT(AncientLemuria_TremorShakesOutRime)},
    {TRIGGER_TABLE_END},
};

// clang-format off
s16 gAncientLemuria_ShiningWaterColors[SHINING_WATER_COLORS] = {
    RGB(12, 24, 23), RGB(8, 22, 24), RGB(4, 20, 25),
    RGB(0, 18, 26),  RGB(0, 16, 26), RGB(0, 14, 26),
    RGB(0, 12, 26),  RGB(0, 14, 26), RGB(0, 16, 26),
    RGB(0, 18, 26),  RGB(4, 20, 25), RGB(8, 22, 24),
};
// clang-format on
