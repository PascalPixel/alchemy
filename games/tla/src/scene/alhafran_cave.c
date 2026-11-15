/// @file
/// The Alhafran Cave under the mayor's manor, in two scenes that share this
/// overlay: the cave behind the rear entrance (SceneId_AlhafranCave), which the
/// hungry boy's father lets the party into, and the town's jail
/// (SceneId_AlhafraJail). Once Briggs and his crew are jailed, his wife Chaucha
/// and their son Eoleo visit him, and the boy shows Psynergy of his own
/// (AlhafranCave_ChauchaAndEoleoVisit). Pound sinks a pillar
/// (AlhafranCave_PoundPillar), Frost freezes a puddle
/// (AlhafranCave_FreezePuddle), and the way down to the jail opens as the
/// leader stands on the ledge above it (AlhafranCave_OpenWayToJail).

#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "version.h"
#include "system/task.h"
#include "field/map_grid.h"
#include "field/call.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "fixed_math.h"

void Pillars_Place(const u16 *table);
void AlhafranCave_TakeLeadersLayer(void);
void Field_StartColorTransition(s32 frames, s32 color);
extern s32 gAlhafranCave_GuardStepsAsideScript[];
extern u32 gAlhafranCave_VisitorsLeaveScript[];
void AlhafranCave_OpenWayToJail(void);
void AlhafranCave_CloseWayToJail(void);
void AlhafranCave_CastSand(void);
void AlhafranCave_StartMovingCrate(void);
void AlhafranCave_FinishMovingCrate(void);
void FlagBlock_PlaceAllMarkingEdges(const void *data);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
extern s32 gAlhafranCave_ClearedWord;

/// Set while the way down to the jail is open, as the leader stands on the
/// ledge above it.
#define FLAG_WAY_TO_JAIL_OPEN 0x204
/// Set once Lash has tied the jail's rope.
#define FLAG_JAIL_ROPE_TIED FLAG_SCENE_TEMP_00
/// Set once Pound has sunk the jail's stake.
#define FLAG_JAIL_STAKE_POUNDED FLAG_SCENE_TEMP_01
/// Set once Frost has frozen the cave's puddle.
#define FLAG_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
/// The cave's actors (SceneId_AlhafranCave), numbering the rows of
/// gAlhafranCave_CaveActorPlacements.
enum AlhafranCaveActorId {
  /// Three pillars standing twelve pixels up.
  ACTOR_CAVE_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_CAVE_SECOND_PILLAR,
  ACTOR_CAVE_THIRD_PILLAR,
  /// A pillar's two pieces (sprite 371, in its animations 2 and 3) and the
  /// pillar leaning across them, turned a sixteenth of a turn.
  ACTOR_CAVE_FIRST_PILLAR_PIECE,
  ACTOR_CAVE_SECOND_PILLAR_PIECE,
  ACTOR_CAVE_LEANING_PILLAR,
  /// The two pillars Move can push.
  ACTOR_CAVE_FIRST_PUSHED_PILLAR,
  ACTOR_CAVE_SECOND_PUSHED_PILLAR,
  /// The puddle Frost freezes into a pillar of ice.
  ACTOR_CAVE_PUDDLE
};

/// The jail's actors (SceneId_AlhafraJail), numbering the rows of
/// gAlhafranCave_JailActorPlacements.
enum AlhafraJailActorId {
  /// The crate Move pushes.
  ACTOR_CRATE = ACTOR_FIRST_PLACED,
  /// The stake Pound sinks.
  ACTOR_JAIL_STAKE,
  /// The poles of the rope Lash ties.
  ACTOR_JAIL_ROPE_POLE,
  ACTOR_JAIL_ROPE_OTHER_POLE,
  /// A pillar's two pieces and the pillar leaning across them, as the cave's.
  ACTOR_JAIL_FIRST_PILLAR_PIECE,
  ACTOR_JAIL_SECOND_PILLAR_PIECE,
  ACTOR_JAIL_LEANING_PILLAR,
  /// XXX: what it shows (sprite 683, by the cells); a render of the jail with
  /// Briggs's crew inside would settle it.
  ACTOR_JAIL_CELL_SPRITE,
  /// Briggs and his crew in their cell, once Briggs is caught.
  /// "I hate prison. The food is terrible..."
  ACTOR_CREWMAN_WHO_HATES_PRISON,
  /// "It sounds like the mayor of Alhafra lost interest in fixing that boat."
  ACTOR_CREWMAN_ON_THE_BOAT,
  ACTOR_BRIGGS,
  /// "Everything would have been fine if it hadn't been for you meddling
  /// kids..."
  ACTOR_CREWMAN_BLAMING_THE_PARTY,
  /// "I saw you staring at Eoleo..."
  ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
  ACTOR_EOLEO,
  ACTOR_CHAUCHA,
  /// "Everything's so boring here... Why doesn't anybody do anything...?"
  ACTOR_BORED_GUARD,
  ACTOR_JAILER
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

struct AlhafranCaveMapCell {
  u8 unk0[2];
  u8 state;
  u8 unk3;
};

struct AlhafranCaveMapLayer {
  struct AlhafranCaveMapCell *cells;
  u8 unk04[0x34];
};

struct AlhafranCaveMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
  u8 unk018[0x120];
  struct AlhafranCaveMapLayer layers[1];
};

struct AlhafranCaveSwitch {
  u16 id;
  s16 kind;
  s16 flag;
};

void Actor_ApplyRandomChildValues(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void FlagBlock_ApplyFlags(const void *data);
void Actor_TakeLeaderLayer(void);
void BattleEffect_DrawSpriteAtWorldPosition(const void *data, s32 x, s32 y,
                                            s32 z);

void AlhafranCave_SetUpCave(void);
void AlhafranCave_SetUpJail(void);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

extern s16 gAlhafranCave_Puddles[];
extern s16 gAlhafranCave_Pillars[];
extern struct ActorPlacement gAlhafranCave_CaveActorPlacements[];
extern struct ActorPlacement gAlhafranCave_JailActorPlacements[];
extern struct Trigger gAlhafranCave_CaveTriggers[];
extern struct Trigger gAlhafranCave_JailTriggers[];
extern u8 gAlhafranCave_CrateSprite[];

/// Sets the height of the map cell under a 16.16 position.
static __inline__ void MapGrid_SetCellHeight_(s32 x, s32 z, s32 a, s32 b) {
  MapGrid_SetCellHeight(x, z, a, b);
}

/// Sets the behavior of the map cell under a 16.16 position.
static __inline__ void MapGrid_SetCellBehavior_(s32 x, s32 z, s32 a, s32 b) {
  MapGrid_SetCellBehavior(x, z, a, b);
}

/// The current scene's actors: the cave's or the jail's.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_ALHAFRAN_CAVE)
    return gAlhafranCave_CaveActorPlacements;
  if (scene == SCENE_ALHAFRA_JAIL)
    return gAlhafranCave_JailActorPlacements;
  return gAlhafranCave_CaveActorPlacements;
}

/// Pound sinks the jail's stake.
void AlhafranCave_PoundPillar(s32 mode, s32 frame) {
  Pound_SinkTarget(gAlhafranCave_Pillars, mode, frame);
}

/// Move lifts the crate; nothing else happens.
void AlhafranCave_StartMovingCrate(void) {}

/// Move sets the crate down; nothing else happens.
void AlhafranCave_FinishMovingCrate(void) {}

/// Briggs asks whether the party has stopped by for a visit, and answers yes or
/// no.
void AlhafranCave_TalkToBriggs(s32 id) {
  FieldEvent_SetMessageId((s32)MsgOhStoppedBy);
  // "Oh, stopped by for a visit, did you?"
  Event_OpenMessage(id, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)MsgOhStoppedBy + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)MsgOhStoppedBy + 2);
  }
  Event_ShowMessage(id, 0);
}

/// Frost freezes the cave's puddle into a pillar of ice.
void AlhafranCave_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gAlhafranCave_Puddles);
}

/// Opens the way down to the jail: the cells by the ledge drop to the lower
/// level.
void AlhafranCave_OpenWayToJail(void) {
  Event_SetFlag(FLAG_WAY_TO_JAIL_OPEN);
  MapGrid_SetCellHeight_(px(400), px(304), 0, 4);
  MapGrid_SetCellHeight_(px(416), px(304), 0, 4);
  MapGrid_SetCellHeight(px(432), px(304), 0, 4);
  MapGrid_SetCellBehavior_(px(384), px(288), 0, 255);
  MapGrid_SetCellHeight_(px(288), px(384), 0, 4);
  MapGrid_SetCellHeight(px(336), px(384), 0, 4);
}

/// Closes the way down to the jail as the leader steps off the ledge.
void AlhafranCave_CloseWayToJail(void) {
  Event_ClearFlag(FLAG_WAY_TO_JAIL_OPEN);
  MapGrid_SetCellHeight_(px(400), px(304), 0, 0);
  MapGrid_SetCellHeight_(px(416), px(304), 0, 0);
  MapGrid_SetCellHeight(px(432), px(304), 0, 0);
  MapGrid_SetCellBehavior_(px(384), px(288), 0, 0);
  MapGrid_SetCellHeight_(px(288), px(384), 0, 0);
  MapGrid_SetCellHeight(px(336), px(384), 0, 0);
}

/// Sand sinks the leader into the sand's layer.
void AlhafranCave_CastSand(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->unkd4a = 2;
}

/// The current scene's triggers: the cave's or the jail's.
const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_ALHAFRAN_CAVE)
    return gAlhafranCave_CaveTriggers;
  if (scene == SCENE_ALHAFRA_JAIL)
    return gAlhafranCave_JailTriggers;
  return gAlhafranCave_CaveTriggers;
}

/// Notes whether the leader stands on the cave's upper level.
void AlhafranCave_TrackLeadersLevel(void) {
  s32 y = Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed;

  if (y < 0x80000)
    gMainRoutineWork.unk2c3 = 0;
  else
    gMainRoutineWork.unk2c3 = 1;
}

/// The jail's task: draws the crate's sprite where the crate stands.
s32 AlhafranCave_DrawCrateSprite(void) {
  struct Actor *object = Actor_Get(ACTOR_CRATE);

  BattleEffect_DrawSpriteAtWorldPosition(
      gAlhafranCave_CrateSprite, object->x.fixed, object->y.fixed + px(28),
      object->z.fixed);
  return 0;
}

/// Sets up the cave or the jail.
s32 Scene_Initialize(void) {
#if !TLA_PATCH_1 /* The Japanese edition does not check the entrance. */
  gFieldWork->transition = 0x201;
  if (gMainRoutineWork.next == SCENE_ALHAFRAN_CAVE)
    AlhafranCave_SetUpCave();
  else if (gMainRoutineWork.next == SCENE_ALHAFRA_JAIL)
    AlhafranCave_SetUpJail();
  return 0;
#else
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_ALHAFRAN_CAVE) {
    gFieldWork->transition = 0x201;
    AlhafranCave_SetUpCave();
  } else if (scene == SCENE_ALHAFRA_JAIL) {
    if (gMainRoutineWork.entrance == 8 || gMainRoutineWork.entrance == 9)
      gFieldWork->transition = 0x209;
    else
      gFieldWork->transition = 0x201;
    AlhafranCave_SetUpJail();
  }
  return 0;
#endif
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

/// Sets up the cave: its pillars, the pushed pillars, the puddle, and the way
/// to the jail if it is open.
void AlhafranCave_SetUpCave(void) {
  struct AlhafranCaveMapWork *map =
      (struct AlhafranCaveMapWork *)Ram_HeapSlots->mapGrid;
  struct Actor *object;

  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 32;
  map->mode17 = 1;
  Task_AddFrame(AlhafranCave_TrackLeadersLevel, TASK_PASS_VBLANK);
  if (Event_GetFlag(FLAG_WAY_TO_JAIL_OPEN))
    AlhafranCave_OpenWayToJail();
  object = Actor_Get(ACTOR_CAVE_FIRST_PILLAR);
  object->motionFlags = 0;
  object->y.fixed = px(12);
  object = Actor_Get(ACTOR_CAVE_SECOND_PILLAR);
  object->motionFlags = 0;
  object->y.fixed = px(12);
  object = Actor_Get(ACTOR_CAVE_THIRD_PILLAR);
  object->motionFlags = 0;
  object->y.fixed = px(12);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_FIRST_PILLAR), 6);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_SECOND_PILLAR), 6);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_THIRD_PILLAR), 6);
  object = Actor_Get(ACTOR_CAVE_SECOND_PILLAR_PIECE);
  object->motionFlags = 0;
  object->y.fixed = px(16);
  object = Actor_Get(ACTOR_CAVE_FIRST_PILLAR_PIECE);
  object->motionFlags = 0;
  object->y.fixed = px(16);
  FieldActor_SetSpriteRotationById(ACTOR_CAVE_LEANING_PILLAR, 0x1000);
  Actor_SetAnimation(ACTOR_CAVE_FIRST_PILLAR_PIECE, 2);
  Actor_SetAnimation(ACTOR_CAVE_SECOND_PILLAR_PIECE, 3);
  Actor_SetAnimation(ACTOR_CAVE_LEANING_PILLAR, 2);
  FieldActor_SetSpritePriority(ACTOR_CAVE_FIRST_PILLAR_PIECE, 2);
  FieldActor_SetSpritePriority(ACTOR_CAVE_SECOND_PILLAR_PIECE, 2);
  FieldActor_SetSpritePriority(ACTOR_CAVE_LEANING_PILLAR, 3);
  Actor_Get(ACTOR_CAVE_FIRST_PILLAR_PIECE)->priorityFlags |= 2;
  Actor_Get(ACTOR_CAVE_SECOND_PILLAR_PIECE)->priorityFlags |= 2;
  Actor_Get(ACTOR_CAVE_LEANING_PILLAR)->priorityFlags |= 2;
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_FIRST_PILLAR_PIECE), 6);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_SECOND_PILLAR_PIECE), 6);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_LEANING_PILLAR), 6);
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(ACTOR_CAVE_FIRST_PUSHED_PILLAR, 200, 136);
    Actor_SetPosition(ACTOR_CAVE_SECOND_PUSHED_PILLAR, 392, 360);
  }
  object = Actor_Get(ACTOR_CAVE_FIRST_PUSHED_PILLAR);
  object->motionFlags = 0;
  object->groundHeight = 0;
  object->y.fixed = 0;
  MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, 4);
  object = Actor_Get(ACTOR_CAVE_SECOND_PUSHED_PILLAR);
  object->motionFlags = 0;
  object->groundHeight = 0;
  object->y.fixed = 0;
  MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, 4);
  FlagBlock_PlaceAllMarkingEdges(gAlhafranCave_Puddles);
  Actor_SetMovementType(Actor_Get(ACTOR_CAVE_PUDDLE), 9);
  if (!Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    /* FAKEMATCH(wrapper): the three calls forwarded through
     * MapGrid_CopyCellPictureCellsRect_. */
    MapGrid_CopyCellsLayered(0, 58, 12, 7, 13, 58);
    MapGrid_CopyCellsLayered(0, 58, 2, 9, 25, 66);
    MapGrid_CopyCellsLayered(0, 58, 10, 5, 15, 74);
  }
}

void FieldEffect_StartBlankOverlay(s32 value);
void EffectSprite_InitFromSlot94(const void *data, s32 a, s32 b, s32 c);

/// Sets up the jail and the cave: who is held and who visits, by how far the
/// story has gone (Briggs caught, his family's visit, the mast cleared), and
/// the hungry boy's father stepping in as the party comes in by entrance 9.
void AlhafranCave_SetUpJail(void) {
  const void *data;
  struct FieldWork *work;

  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 32;
  Actor_Get(ACTOR_CRATE)->priorityFlags |= 32;
  FieldEffect_StartBlankOverlay(0);
  data = gAlhafranCave_CrateSprite;
  BattleEffect_FillOverlayTiles(0);
  Call4((void (*)())EffectSprite_InitFromSlot94, (s32)data, 8, 15, 0x40000000);
  FieldTask_AddFrame(AlhafranCave_DrawCrateSprite, TASK_PASS_VBLANK);
  if ((u16)(gMainRoutineWork.entrance - 8) <= 1) {
    work = Ram_HeapSlots->fieldWork;

    work->unkd4c = 3;
  }
  Rope_Initialize();
  Rope_Place(0, FLAG_JAIL_ROPE_TIED, ACTOR_JAIL_ROPE_POLE,
             ACTOR_JAIL_ROPE_OTHER_POLE);
  Actor_Get(ACTOR_JAIL_ROPE_POLE)->priorityFlags |= 32;
  Actor_Get(ACTOR_JAIL_ROPE_OTHER_POLE)->priorityFlags |= 32;
  Pillars_Place(gAlhafranCave_Pillars);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JAIL_STAKE), 0);
  FieldActor_SetSpriteRotationById(ACTOR_JAIL_LEANING_PILLAR, 0x1000);
  Actor_SetAnimation(ACTOR_JAIL_FIRST_PILLAR_PIECE, 0);
  Actor_SetAnimation(ACTOR_JAIL_SECOND_PILLAR_PIECE, 1);
  Actor_SetAnimation(ACTOR_JAIL_LEANING_PILLAR, 2);
  FieldActor_SetSpritePriority(ACTOR_JAIL_FIRST_PILLAR_PIECE, 1);
  FieldActor_SetSpritePriority(ACTOR_JAIL_SECOND_PILLAR_PIECE, 1);
  Actor_Get(ACTOR_JAIL_FIRST_PILLAR_PIECE)->priorityFlags |= 2;
  Actor_Get(ACTOR_JAIL_SECOND_PILLAR_PIECE)->priorityFlags |= 2;
  Actor_Get(ACTOR_JAIL_LEANING_PILLAR)->priorityFlags |= 2;
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_JAIL_FIRST_PILLAR_PIECE), 6);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_JAIL_SECOND_PILLAR_PIECE), 6);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_JAIL_LEANING_PILLAR), 6);
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(ACTOR_JAIL_CELL_SPRITE, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_WHO_HATES_PRISON, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_ON_THE_BOAT, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_BLAMING_THE_PARTY, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_JAILER, 0, 0);
    Actor_Get(ACTOR_BORED_GUARD)->update = AlhafranCave_TakeLeadersLayer;
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(ACTOR_BORED_GUARD, 0, 0);
    FieldActor_SetSpritePriority(ACTOR_BRIGGS, 2);
    FieldActor_SetSpritePriority(ACTOR_CREWMAN_WHO_HATES_PRISON, 2);
    FieldActor_SetSpritePriority(ACTOR_CREWMAN_ON_THE_BOAT, 2);
    FieldActor_SetSpritePriority(ACTOR_CREWMAN_BLAMING_THE_PARTY, 2);
    FieldActor_SetSpritePriority(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, 2);
    if (!Event_GetFlag(FLAG_FAMILY_VISITED_BRIGGS)) {
      Actor_SetPosition(ACTOR_CREWMAN_ON_THE_BOAT, 712, 216);
      Actor_SetPosition(ACTOR_CREWMAN_BLAMING_THE_PARTY, 744, 216);
    } else {
      Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
      Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    }
  } else {
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_WHO_HATES_PRISON, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_ON_THE_BOAT, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_BLAMING_THE_PARTY, 0, 0);
    Actor_SetPosition(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_JAILER, 0, 0);
    Actor_Get(ACTOR_BORED_GUARD)->update = AlhafranCave_TakeLeadersLayer;
  }
  if (gMainRoutineWork.entrance == 9 &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetSpeed(ACTOR_CRATE, 0.3);
    Actor_SetDestinationOffset(ACTOR_CRATE, 0, 16);
    FieldActor_WaitForMoveById(ACTOR_CRATE);
  }
}

/// Once the mast is cleared, the jail's cells stand open and empty.
s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_ALHAFRA_JAIL &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCellsLayered(23, 46, 4, 5, 21, 14);
    MapGrid_CopyCellsLayered(86, 44, 5, 6, 84, 12);
    MapGrid_CopyCellAttributes(22, 45, 5, 6, 20, 13);
  }
  return 0;
}

void AlhafranCave_IdleTask(void) {}

extern u8 MsgYouHereTo[];

void AlhafranCave_ShakeUnderEoleosPower(void);

/// Briggs's wife Chaucha and their son Eoleo visit him in the jail, and Eoleo
/// shows Psynergy of his own.
void AlhafranCave_ChauchaAndEoleoVisit(void) {
  struct Actor *actor21;
  struct Actor *actor15;
  struct Actor *effect;
  s32 position[3];

  /* FAKEMATCH(wrapper): 79 calls forwarded through call.h's Call2, Call3, Call4
   * and Value3. */
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT) == 0)
    return;
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED) == 1)
    return;
  Event_SetFlag(FLAG_FAMILY_VISITED_BRIGGS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1);
  Actor_SetSpeed(ACTOR_EOLEO, 1);
  Actor_SetSpeed(ACTOR_CREWMAN_ON_THE_BOAT, 1);
  Actor_SetSpeed(ACTOR_CREWMAN_BLAMING_THE_PARTY, 1);
  Actor_SetSpeed(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, 1);
  FieldEvent_SetMessageId((s32)MsgYouHereTo);
  // "You here to see Briggs?"
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 800, 288);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Chaucha... It's good to see you."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "And you, too, Eoleo."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_EOLEO, 6, 15);
  FieldActor_Jump(ACTOR_EOLEO, 6, 23);
  // "Goo. Goo goo ga."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "How...is your cell? Is there anything I can do to make this easier..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_CREWMAN_WHO_HATES_PRISON, 2);
  FieldEvent_Wait(10);
  // "I'm fine, Chaucha. My only complaint is that the food here is terrible."
  FieldEvent_ShowMessageThenWait(ACTOR_CREWMAN_WHO_HATES_PRISON, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "What!? Are they forcing you to eat this disgusting gruel?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  (void *)Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 312, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_EOLEO, ANIM_NOD);
  FieldActor_StartRepeatedMotion(ACTOR_JAILER, 3);
  Actor_SetAttachedEffect(ACTOR_JAILER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  EventCamera_FollowActor(ACTOR_JAILER, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_JAILER, 1.5);
  Actor_SetScript(ACTOR_JAILER, gAlhafranCave_GuardStepsAsideScript);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_JAILER);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JAILER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Now, wait a second..."
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JAILER, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  // "First of all, it's not disgusting. It's nutritious! And besides..."
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_FaceDirection(ACTOR_JAILER, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(ACTOR_JAILER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "You're IN JAIL! What did you expect, gourmet cuisine?"
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  actor21 = Actor_Get(ACTOR_EOLEO);
  actor15 = Actor_Get(ACTOR_JAIL_CELL_SPRITE);
  Call2((void (*)())Field_StartColorTransition, 8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 2);
  Event_PlaySound(SE_ASCEND);
  FieldEvent_Wait(40);
  effect = FieldEffect_SpawnMoveHand(actor21);
  position[0] = actor15->x.fixed;
  effect->watchedActor = actor15;
  position[1] = actor15->y.fixed + px(16);
  position[2] = actor15->z.fixed;
  FieldVector_AddPolarOffset(px(16), 0xc000, position);
  FieldActor_SetMoveTargetFixed(effect, position[0], position[1], position[2]);
  FieldEffect_SnapScaleToFull(effect);
  Actor_SelectAnimation(effect, 4);
  actor15->update = AlhafranCave_ShakeUnderEoleosPower;
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_CREWMAN_ON_THE_BOAT, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_BLAMING_THE_PARTY, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_CREWMAN_ON_THE_BOAT, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_CREWMAN_BLAMING_THE_PARTY, 3);
  Actor_FaceDirection(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 288, 1);
  FieldCamera_WaitForArrival();
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_ShowEmote(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, -16, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(40);
  FieldActor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  // "Eoleo!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
  Actor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  actor15->update = 0;
  Actor_SetSpriteColorMode(actor15, 0);
  UpdateRisingParticleBurst(effect);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_WHO_HATES_PRISON, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JAILER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_EOLEO, 3);
  Actor_SetAttachedEffect(ACTOR_EOLEO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_CREWMAN_WHO_HATES_PRISON, FACING_SOUTH_SOUTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_CREWMAN_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JAILER, FACING_WEST, 0);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Field_StartColorTransition(8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 2);
  Event_PlaySound(SE_ASCEND);
  FieldEvent_Wait(40);
  // "Eoleo!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  Actor_FaceDirection(ACTOR_CREWMAN_WHO_HATES_PRISON, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JAILER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "Hooo..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  (void *)Actor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_RunRepeatedMotion(ACTOR_EOLEO, 2);
  FieldEvent_Wait(20);
  // "Ahhh waaah..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_FaceDirection(ACTOR_CREWMAN_WHO_HATES_PRISON, FACING_SOUTH_SOUTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_CREWMAN_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JAILER, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_EOLEO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // "WAHHH!"
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 20);
  gAlhafranCave_ClearedWord = 0;
  Task_AddFrame(AlhafranCave_IdleTask, TASK_PASS_VBLANK);
  Actor_ShowEmote(ACTOR_JAILER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I don't know why he's crying! You're his mother! Can't you do..."
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 8);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "There there, Eoleo... What's the matter?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EOLEO, 2);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(ACTOR_EOLEO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  // "Waa waa waa!"
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Did the mean old guard frighten you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Waa waa..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Don't let him bother you... He's just a big bully! Come on! Let's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Waa waa..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "We'll be back again later. Will that be all right?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JAILER, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Yeah, yeah. That's fine."
  FieldEvent_ShowMessageThenWait(ACTOR_JAILER, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "We'll come back soon, dear."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_JAILER, 21);
  Actor_WalkBy(ACTOR_CHAUCHA, 0, -16);
  Actor_WalkByAndWait(ACTOR_EOLEO, 0, -16);
  Actor_WalkBy(ACTOR_CHAUCHA, 52, 0);
  Actor_WalkByAndWait(ACTOR_EOLEO, 52, 0);
  FieldEvent_Wait(15);
  FieldActor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, 21);
  FieldActor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  FieldActor_SetSpritePriority(ACTOR_EOLEO, 2);
  Actor_SetScript(ACTOR_CHAUCHA, gAlhafranCave_VisitorsLeaveScript);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_EOLEO, gAlhafranCave_VisitorsLeaveScript);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, 16, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkBy(ACTOR_CREWMAN_ON_THE_BOAT, 0, 16);
  Actor_WalkByAndWait(ACTOR_CREWMAN_BLAMING_THE_PARTY, 0, 16);
  FieldEvent_Wait(5);
  Actor_FaceDirection(ACTOR_CREWMAN_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CREWMAN_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_EOLEO);
  FieldActor_Stop(ACTOR_JAILER);
  FieldActor_Stop(ACTOR_FELIX);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Actor_SetPosition(ACTOR_EOLEO, 0, 0);
  Actor_SetMovementType(Actor_Get(ACTOR_JAILER), 9);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Event_End();
}

void AlhafranCave_TakeLeadersLayer(void) { Actor_TakeLeaderLayer(); }

/// While Eoleo's Psynergy runs, the actors shake.
void AlhafranCave_ShakeUnderEoleosPower(void) {
  Actor_ApplyRandomChildValues();
}

#include "shared/pillars_place.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

// clang-format off
s16 gAlhafranCave_Puddles[] = {
    ACTOR_CAVE_PUDDLE, FLAG_PUDDLE_FROZEN, -1, 0,
};

s32 gAlhafranCave_GuardStepsAsideScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(px(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -px(60)),
    SC_SYNC,
    SC_MOVE_BY(-px(4), 0, 0),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gAlhafranCave_VisitorsLeaveScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(px(780), 0, px(288)),
    SC_SYNC,
    SC_MOVE_BY(0, 0, px(12)),
    SC_SYNC,
    SC_MOVE_BY(px(64), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(px(32), 0, px(32)),
    SC_SYNC,
    SC_MOVE_BY(0, 0, px(54)),
    SC_SYNC,
    SC_END,
};

s16 gAlhafranCave_Pillars[] = {
    ACTOR_JAIL_STAKE, 1, FLAG_JAIL_STAKE_POUNDED, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AlhafranCave),
    SCENE_EXIT_UNLESS(1, Alhafra, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AlhafranCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AlhafranCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AlhafraJail, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AlhafraJail, 7, EXIT_FLAG_NONE),
    SCENE_EXITS(AlhafraJail),
    SCENE_EXIT_UNLESS(6, AlhafranCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AlhafranCave, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Alhafra, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AlhafraJail, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AlhafraJail, 9, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAlhafranCave_CaveActorPlacements[] = {
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, 7, 120, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, 7, 472, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, 7, 424, 0, 312, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(371, CONDITION_ALWAYS, 7, 296, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(371, CONDITION_ALWAYS, 7, 344, 0, 392, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, 7, 324, 0, 416, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafranCave_JailActorPlacements[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(371, CONDITION_ALWAYS, 7, 184, 0, 284, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(371, CONDITION_ALWAYS, 7, 232, 0, 284, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, 7, 196, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(683, CONDITION_ALWAYS, 7, 720, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(16, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(38, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 304, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(37, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 296, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 744, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 736, 0, 360, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAlhafranCave_CaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(AlhafranCave_CloseWayToJail)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(AlhafranCave_OpenWayToJail)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CAVE_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT(AlhafranCave_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AlhafranCave_CastSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafranCave_JailTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 20, FLAG_FAMILY_VISITED_BRIGGS, EVENT(AlhafranCave_ChauchaAndEoleoVisit)},
    {TRIGGER_TALK, ACTOR_BORED_GUARD, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgEverythingSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BORED_GUARD, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgIDonTKnow)},
    {TRIGGER_TALK, ACTOR_BORED_GUARD, CONDITION_ALWAYS, MSGNO(MsgBeforeIKnew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BORED_GUARD, CONDITION_ALWAYS, MSGNO(MsgNoMatterHow)},
    {TRIGGER_TALK, ACTOR_JAILER, CONDITION_ALWAYS, MSGNO(MsgIVeBeen)},
    {TRIGGER_TALK, ACTOR_BRIGGS, CONDITION_ALWAYS, EVENT(AlhafranCave_TalkToBriggs)},
    {TRIGGER_TALK, ACTOR_CREWMAN_WHO_HATES_PRISON, CONDITION_ALWAYS, MSGNO(MsgIHatePrison)},
    {TRIGGER_TALK, ACTOR_CREWMAN_ON_THE_BOAT, CONDITION_ALWAYS, MSGNO(MsgItSoundsLike)},
    {TRIGGER_TALK, ACTOR_CREWMAN_BLAMING_THE_PARTY, CONDITION_ALWAYS, MSGNO(MsgEverythingWouldHave)},
    {TRIGGER_TALK, ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, CONDITION_ALWAYS, MSGNO(MsgISawYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JAILER, CONDITION_ALWAYS, MSGNO(MsgItSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgHasStrangePowers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CREWMAN_WHO_HATES_PRISON, CONDITION_ALWAYS, MSGNO(MsgIMNotStaying)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CREWMAN_ON_THE_BOAT, CONDITION_ALWAYS, MSGNO(MsgIfTheyDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CREWMAN_BLAMING_THE_PARTY, CONDITION_ALWAYS, MSGNO(MsgOhWhatS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CREWMAN_GUARDING_EOLEOS_SECRET, CONDITION_ALWAYS, MSGNO(MsgIHearSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_JAIL_ROPE_POLE, FLAG_JAIL_ROPE_TIED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_JAIL_STAKE, FLAG_JAIL_STAKE_POUNDED, EVENT(AlhafranCave_PoundPillar)},
    // The cave's Frost row, repeated in the jail's table.
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CAVE_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT(AlhafranCave_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CRATE, CONDITION_ALWAYS, EVENT(AlhafranCave_StartMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, CONDITION_ALWAYS, EVENT(AlhafranCave_FinishMovingCrate)},
    {TRIGGER_TABLE_END},
};
