/// @file
/// The Lemurian Ship below deck, at the East Indra Shore, in two scenes
/// (SceneId_LemurianShipCabins and SceneId_LemurianShipAquaHydra) whose rooms
/// are told apart by the entrance used. Aqua Jellies fill the cabins: each one
/// beaten melts into a puddle Frost turns to ice to stand on, and crates are
/// pushed between them. A frightened jelly flees down the lift; the last one
/// calls the others together into the Aqua Hydra, whose fall floods the hold
/// until Parch dries it. Douse fills the two pillars that sink on their droplet
/// tiles and open the way to the engine room, where Piers sets the Black
/// Crystal and the ship sails.
#include "system/task.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "field/field_work.h"
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

/// The Lemurian Ship's room of two jellies, by the rows of
/// gLemurianShip_TwoJellyRoomPlacements.
enum LemurianShipTwoJellyRoomActorId {
  ACTOR_TWO_JELLY_ROOM_JELLY_A = ACTOR_FIRST_PLACED,
  ACTOR_TWO_JELLY_ROOM_JELLY_B,
  ACTOR_TWO_JELLY_ROOM_PUDDLE_A,
  ACTOR_TWO_JELLY_ROOM_PUDDLE_B
};

/// The Lemurian Ship's room of six jellies and the lift, by the rows of
/// gLemurianShip_SixJellyRoomPlacements.
enum LemurianShipSixJellyRoomActorId {
  ACTOR_SIX_JELLY_ROOM_CRATE = ACTOR_FIRST_PLACED,
  ACTOR_SIX_JELLY_ROOM_JELLY_A,
  ACTOR_SIX_JELLY_ROOM_JELLY_B,
  ACTOR_SIX_JELLY_ROOM_JELLY_C,
  ACTOR_SIX_JELLY_ROOM_JELLY_D,
  ACTOR_SIX_JELLY_ROOM_JELLY_E,
  ACTOR_SIX_JELLY_ROOM_JELLY_F,
  ACTOR_LIFT_PUDDLE,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_A,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_B,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_C,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_D,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_E
};

/// The Lemurian Ship's room of the crate on the ledge, by the rows of
/// gLemurianShip_LedgeRoomPlacements.
enum LemurianShipLedgeRoomActorId { ACTOR_LEDGE_CRATE = ACTOR_FIRST_PLACED };

/// The Lemurian Ship's room of seven jellies, by the rows of
/// gLemurianShip_SevenJellyRoomPlacements.
enum LemurianShipSevenJellyRoomActorId {
  ACTOR_SEVEN_JELLY_ROOM_CRATE = ACTOR_FIRST_PLACED,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_A,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_B,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_C,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_D,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_E,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_F,
  ACTOR_SEVEN_JELLY_ROOM_JELLY_G,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_A,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_B,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_C,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_D,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_E,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_F,
  ACTOR_SEVEN_JELLY_ROOM_PUDDLE_G
};

/// The Lemurian Ship's hold, where the jellies gather into the Aqua Hydra, by
/// the rows of gLemurianShip_HoldPlacements; the party's rows go to their
/// slots.
enum LemurianShipHoldActorId {
  ACTOR_HOLD_JELLY_A = ACTOR_FIRST_PLACED,
  ACTOR_HOLD_JELLY_B,
  ACTOR_HOLD_JELLY_C,
  ACTOR_HOLD_JELLY_D,
  ACTOR_HOLD_JELLY_E,
  ACTOR_HOLD_JELLY_F
};

/// The Lemurian Ship's room of the two pillars Douse fills, by the rows of
/// gLemurianShip_PillarRoomPlacements; the party's rows go to their slots.
enum LemurianShipPillarRoomActorId {
  ACTOR_RIGHT_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_LEFT_PILLAR,
  ACTOR_RIGHT_PILLAR_PUDDLE,
  ACTOR_LEFT_PILLAR_PUDDLE,
  ACTOR_PILLAR_ROOM_KRADEN
};

/// The Lemurian Ship's engine room, by the rows of
/// gLemurianShip_EngineRoomPlacements; the party's rows go to their slots.
enum LemurianShipEngineRoomActorId {
  ACTOR_ENGINE_ROOM_KRADEN = ACTOR_FIRST_PLACED
};
#include "scene_ids.h"
#include "scene/lemurian_ship.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "version.h"
#include "macros.h"
#include "field/actor_script.h"
#include "item_ids.h"

#define FLAG_JELLIES_BEATEN 0x930 ///< the first of sixteen, by jelly number
#define FLAG_AQUA_HYDRA_BEATEN (FLAG_JELLIES_BEATEN + 15)
#define FLAG_PILLAR_WAY_OPEN 0x982     ///< both pillars sank, the way opened
#define FLAG_SHIP_ENGINE_RUNNING 0x983 ///< the Black Crystal is set
#define FLAG_HOLD_PARCHED 0x9e4        ///< Parch dried the flooded hold
#define FLAG_CRATE_DROPPED FLAG_SCENE_TEMP_00
#define FLAG_JELLY_FLED FLAG_SCENE_TEMP_01 ///< the frightened jelly ran
#define FLAG_RIGHT_PILLAR_SUNK FLAG_SCENE_TEMP_02
#define FLAG_LEFT_PILLAR_SUNK FLAG_SCENE_TEMP_03
#define FLAG_RIGHT_PILLAR_FULL FLAG_SCENE_TEMP_04 ///< Douse filled it
#define FLAG_LEFT_PILLAR_FULL FLAG_SCENE_TEMP_05
#define FLAG_LIFT_LOWERED FLAG_SCENE_TEMP_07
/// The first of sixteen: Frost froze jelly n's puddle.
#define FLAG_PUDDLE_FROZEN FLAG_SCENE_TEMP_10
/// The first of sixteen: jelly n was just beaten and melts on arrival.
#define FLAG_JELLY_MELTING FLAG_SCENE_TEMP_20
/// The first of sixteen: the battle with jelly n is on.
#define FLAG_FIGHTING_JELLY FLAG_AREA_TEMP_00

extern s16 gLemurianShip_AquaHydraEntrances[];

u8 *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_LEMURIAN_SHIP_CABINS)
    return gLemurianShip_CabinsEntrances;
  if (scene == SCENE_LEMURIAN_SHIP_AQUA_HYDRA)
    return gLemurianShip_AquaHydraEntrances;
  return gLemurianShip_Entrances;
}

u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_LEMURIAN_SHIP_CABINS)
    return gLemurianShip_CabinsRegions;
  if (scene == SCENE_LEMURIAN_SHIP_AQUA_HYDRA)
    return gLemurianShip_AquaHydraRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

void Event_Start(void);
void Event_End(void);
void FlagBlock_ApplyFlags(const void *script);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldPsynergy_RunHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void FlagBlock_PlaceAll(const void *script);
void Event_StartScreenTransition(void);

void Actor_MoveToObject(s32 actor, s32 target);

#define LEMURIAN_SHIP_TASK_ADD(func, priority) Task_Add((func), (priority))

void MapGrid_CenterOnFollowedPoint(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Actor_LiftTargetBlock(const void *script);
void Actor_PlaceMovedBlock(void);

void LemurianShip_LeapIntoHydra(s32 id);
void LemurianShip_CheckPillarsOnDroplets(void);
void LemurianShip_SetUpPuddle(s32 id);
void LemurianShip_SetUpLowPuddle(s32 id);

extern s32 gLemurianShip_JellyMelts[];
extern u32 gLemurianShip_PuddleSpreads[];
extern s32 gLemurianShip_HydraSwells[];
extern s16 gLemurianShip_Crate[];
extern s16 gLemurianShip_TwoJellyRoomPuddles[];
extern s16 gLemurianShip_SixJellyRoomPuddles[];
extern s16 gLemurianShip_SevenJellyRoomPuddles[];
extern s16 gLemurianShip_Pillars[];

void LemurianShip_JellyFlees(void);
void LemurianShip_SetBlackCrystal(void);
void Actor_StartScript(struct Actor *object, const void *script);
void Actor_PlaceBlocks(const void *script);
extern u32 gLemurianShip_LightToOrb[];

extern struct ActorPlacement gLemurianShip_LedgeRoomPlacements[];
extern struct ActorPlacement gLemurianShip_SevenJellyRoomPlacements[];

struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
    switch (gMainRoutineWork.entrance) {
    case 3:
    case 4:
    case 5:
      return gLemurianShip_TwoJellyRoomPlacements;
    case 25:
    case 26:
    case 27:
    case 28:
      return gLemurianShip_SixJellyRoomPlacements;
    case 15:
    case 16:
    case 17:
    case 18:
      return gLemurianShip_LedgeRoomPlacements;
    case 29:
    case 30:
      return gLemurianShip_SevenJellyRoomPlacements;
    }
  } else if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
      return gLemurianShip_HoldPlacements;
    case 6:
    case 10:
      return gLemurianShip_PillarRoomPlacements;
    default:
      return gLemurianShip_EngineRoomPlacements;
    }
  }
  return gLemurianShip_Placements;
}

void LemurianShip_OpenDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  u8 *entry;
  s32 offset;
  u32 found;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  found = 0;
  i = 8;
next:
  object = ((struct Actor *)Actor_GetById(i));
  if (object != NULL)
    object->motionFlags = 0;
  i++;
  if (i < 64)
    goto next;
  entry = (u8 *)gLemurianShip_Doors;
  i = 0;
  offset = 0;
next2:
  if (work->stepTrigger == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 10)
    goto next2;
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  MapGrid_CopyCellsTo(0, 0, gLemurianShip_Doors[found].x,
                      gLemurianShip_Doors[found].z, 1, 2);
  MapGrid_AnimateCells(gLemurianShip_Doors[found].value,
                       gLemurianShip_Doors[found].x,
                       gLemurianShip_Doors[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  {
    s32 owner = gMainRoutineWork.leaderActorId;

    if (found == 12)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  Event_Wait(4);
  Import_EventSetExitRequest(work->stepTrigger);
}

void LemurianShip_OpenLowerDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  u8 *entry;
  s32 offset;
  u32 found;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  found = 0;
  i = 8;
next:
  object = ((struct Actor *)Actor_GetById(i));
  if (object != NULL)
    object->motionFlags = 0;
  i++;
  if (i < 64)
    goto next;
  entry = (u8 *)gLemurianShip_LowerDoors;
  i = 0;
  offset = 0;
next2:
  if (work->stepTrigger == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 4)
    goto next2;
  if (found < 2 && Event_GetFlag(FLAG_AQUA_HYDRA_BEATEN) &&
      Event_GetFlag(FLAG_HOLD_PARCHED) == 0)
    found += 4;
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  MapGrid_CopyCellsTo(0, 0, gLemurianShip_LowerDoors[found].x,
                      gLemurianShip_LowerDoors[found].z, 1, 2);
  MapGrid_AnimateCells(gLemurianShip_LowerDoors[found].value,
                       gLemurianShip_LowerDoors[found].x,
                       gLemurianShip_LowerDoors[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_CenterOnColumn(gMainRoutineWork.leaderActorId, 2, -4);
  Event_Wait(4);
  Import_EventSetExitRequest(work->stepTrigger);
}

void LemurianShip_WalkIntoPassage(void) {
  if (Event_GetFlag(FLAG_PILLAR_WAY_OPEN)) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    struct MainRoutineWork *party = &gMainRoutineWork;
    struct Actor *leader;
    s32 x;
    s32 cell;

    leader = ((struct Actor *)Actor_GetById(party->leaderActorId));
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
    Actor_SetSpeed(party->leaderActorId, 0.5);
    x = leader->x.fixed;
    cell = x >> 16;
    Actor_SetMoveTarget(leader, ((8 - cell % 16) << 16) + x, leader->y.fixed,
                        leader->z.fixed);
    FieldActor_WaitForMove(leader);
    leader->facing = FACING_NORTH;
    Actor_SetScript(party->leaderActorId, gLemurianShip_IntoPassage);
    wait(12);
    Event_PlaySound(SE_MAP_EXIT);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->stepTrigger);
  }
}

extern struct Trigger gLemurianShip_LedgeRoomTriggers[];
extern struct Trigger gLemurianShip_HoldTriggers[];
extern struct Trigger gLemurianShip_PillarRoomTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
    switch (gMainRoutineWork.entrance) {
    case 3:
    case 4:
    case 5:
      return gLemurianShip_TwoJellyRoomTriggers;
    case 25:
    case 26:
    case 27:
    case 28:
      return gLemurianShip_SixJellyRoomTriggers;
    case 15:
    case 16:
    case 17:
    case 18:
      return gLemurianShip_LedgeRoomTriggers;
    case 29:
    case 30:
      return gLemurianShip_SevenJellyRoomTriggers;
    default:
      return gLemurianShip_CabinTriggers;
    }
  } else if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
      return gLemurianShip_HoldTriggers;
    case 6:
    case 10:
      return gLemurianShip_PillarRoomTriggers;
    default:
      return gLemurianShip_LowerDeckTriggers;
    }
  }
  return gLemurianShip_Triggers;
}

/// The lift's switch: once the engine runs it raises or lowers the lift; before
/// that, "There's no reaction... It doesn't seem to be running."
void LemurianShip_ExamineLiftSwitch(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    if (Event_GetFlag(FLAG_LIFT_LOWERED)) {
      Event_PlaySound(SE_CRUNCH);
      FieldActor_SetPositionAndResetMotion(
          ((struct Actor *)Actor_GetById(ACTOR_LIFT_PUDDLE)), px(824), px(-32),
          px(72));
      ((struct Actor *)Actor_GetById(ACTOR_LIFT_PUDDLE))->groundHeight =
          0xffe00000;
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      Event_ClearFlag(FLAG_LIFT_LOWERED);
    } else if (Event_GetFlag((FLAG_PUDDLE_FROZEN + 2))) {
      Event_PlaySound(SE_ITEM_REMOVE);
    } else {
      Event_PlaySound(SE_CRUNCH);
      Actor_SetPosition(ACTOR_LIFT_PUDDLE, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
      Event_SetFlag(FLAG_LIFT_LOWERED);
    }
  } else {
    // "There's no reaction... It doesn't seem to be running."
    FieldMessage_Show((s32)MsgLemurianShipTheresNoReactionItDoesnt, 1);
  }
  Event_End();
}

void LemurianShip_LiftCrate(void) {
  Actor_LiftTargetBlock(gLemurianShip_Crate);
}

void LemurianShip_SetDownCrate(void) { Actor_PlaceMovedBlock(); }

void LemurianShip_LiftLedgeCrate(void) {
  Actor_LiftTargetBlock(gLemurianShip_Crate);
}

void LemurianShip_PushCrateOffLedge(void) {
  struct Actor *object;
  s32 x;
  u8 *flags;
  s32 moving;

  Actor_PlaceMovedBlock();
  object = ((struct Actor *)Actor_GetById(ACTOR_LEDGE_CRATE));
  x = FIELD_CELL(object->x.fixed);
  if (x == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    MapGrid_CopyCellAttributes(16, 47, 1, 1, x, 46);
    wait(1);
    flags = &object->motionFlags;
    *flags = 3;
    do {
      wait(1);
      moving = object->velocityY;
    } while (moving != 0);
    Event_PlaySound(SE_THUMP);
    wait(10);
    *flags = moving;
    Event_SetFlag(FLAG_CRATE_DROPPED);
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
    Event_End();
  }
}

void LemurianShip_FreezeTwoJellyRoomPuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShip_TwoJellyRoomPuddles);
}

void LemurianShip_FreezeSixJellyRoomPuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShip_SixJellyRoomPuddles);
}

void LemurianShip_FreezeLiftPuddle(void) {
  MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
  Event_SetFlag((FLAG_PUDDLE_FROZEN + 2));
}

void LemurianShip_FreezeSevenJellyRoomPuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShip_SevenJellyRoomPuddles);
}

void LemurianShip_StepOnLift(void) {
  struct Actor *leader;
  struct Actor *helper;
  s32 opened;
  u8 *state;

  if (!Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 840, 72);
    opened = Event_GetFlag((FLAG_JELLIES_BEATEN + 2));
    state = &leader->layer;
    if (opened) {
      Event_PlaySound(SE_LEMURIAN_SHIP_LIFT);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      MapGrid_SetShake(0, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      *state = 2;
      Event_Wait(20);
      Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 840, 88);
      Event_Wait(20);
      Event_PlaySound(SE_CRUNCH);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      *state = 1;
    } else {
      helper = ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A));
      Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2);
      Actor_SetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2.4);
      ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
          ->steeringFlags &= ~1;
      Actor_WalkToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 824, 88);
      Event_Wait(1);
      {
        /* FAKEMATCH(temporary): the bit in a local the byte is or'ed into. */
        u8 *bits =
            &((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
                 ->steeringFlags;
        s32 value = 1;

        value |= *bits;
        *bits = value;
      }
      Event_PlaySound(SE_LEMURIAN_SHIP_LIFT);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      MapGrid_SetShake(0, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      {
        s32 two = 2;
        *state = two;
        Event_Wait(20);
        Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST,
                            20);
        Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
        Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
        Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 840, 88);
        Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
        Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 9);
        helper->gravity = px(1);
        helper->layer = two;
      }
      Actor_Jump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 4, 0);
      Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 832, 74);
      Actor_Jump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 4, 0);
      Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 856, 88);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(20);
      Event_PlaySound(SE_CRUNCH);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      Actor_Stop(gMainRoutineWork.leaderActorId);
      *state = 1;
      helper->layer = 0;
      wait(1);
      Event_ClearFlag(FLAG_JELLY_FLED);
    }
    Event_End();
  }
}

void LemurianShip_StepOnLiftWestSide(void) {
  struct Actor *leader;

  if (!Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 812, 72);
    Event_PlaySound(SE_LEMURIAN_SHIP_LIFT);
    Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    MapGrid_SetShake(0, 2, 1);
    Event_Wait(20);
    MapGrid_SetShake(0, 0, 0);
    MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
    leader->layer = 2;
    Event_Wait(20);
    Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 3.2);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 812, 88);
    Event_Wait(20);
    Event_PlaySound(SE_CRUNCH);
    MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
    Event_Wait(20);
    leader->layer = 1;
  }
}

#define DEFINE_JELLY_BATTLE(name, firstJelly, firstActor)                      \
  void name(s32 id) {                                                          \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Actor_FaceActorById(id, gMainRoutineWork.leaderActorId, 0);                \
    Actor_StartRepeatedMotion(id, 2);                                          \
    Actor_SetAttachedEffect(id,                                                \
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);  \
    Event_Wait(20);                                                            \
    gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;                       \
    Field_RequestBattleFromEncounterTable(11, 0);                              \
    GameFlag_Set(id + (FLAG_FIGHTING_JELLY + (firstJelly) - (firstActor)));    \
  }

DEFINE_JELLY_BATTLE(LemurianShip_FightTwoJellyRoomJelly, 0, 8)

DEFINE_JELLY_BATTLE(LemurianShip_FightSixJellyRoomJelly, 2, 9)

DEFINE_JELLY_BATTLE(LemurianShip_FightSevenJellyRoomJelly, 8, 9)

void LemurianShip_LeapIntoHydra(s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  Actor_SetSpeed(id, 3.2);
  object->gravity = px(1) / 2;
  object->velocityY = px(8);
  Actor_SetDestination(id, 120, 120);
}

void LemurianShip_FightAquaHydra(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(MUS_SILENCE);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_B, 2);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_C, 2);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_D, 2);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_E, 2);
  Actor_RunRepeatedMotion(ACTOR_HOLD_JELLY_F, 2);
  Event_Wait(20);
  Event_PlaySound(SE_BEAST_CRY);
  LemurianShip_LeapIntoHydra(9);
  LemurianShip_LeapIntoHydra(10);
  LemurianShip_LeapIntoHydra(11);
  LemurianShip_LeapIntoHydra(12);
  LemurianShip_LeapIntoHydra(13);
  Actor_WaitForMoveById(ACTOR_HOLD_JELLY_F);
  Actor_SetPosition(ACTOR_HOLD_JELLY_B, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_C, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_D, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_E, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_F, 0, 0);
  Event_Wait(20);
  Event_PlaySound(MUS_AQUA_HYDRA);
  Actor_RunScriptAndWait(ACTOR_HOLD_JELLY_A, gLemurianShip_HydraSwells);

  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination(SCENE_LEMURIAN_SHIP_AQUA_HYDRA, 15);
  Field_RequestBattleFromEncounterTable(11, 1);
  Event_SetFlag((FLAG_FIGHTING_JELLY + 15));
}

/// A pillar full of water pushed onto its droplet tile sinks into the floor;
/// with both sunk, the way to the engine room opens.
void LemurianShip_CheckPillarsOnDroplets(void) {
  struct Actor *object;
  struct Actor *leader;
  s32 column;
  s32 row;
  u32 i;
  s32 done;

  done = Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK);
  if (!done && Event_GetFlag(FLAG_RIGHT_PILLAR_FULL)) {
    object = ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR));
    column = FIELD_CELL(object->x.fixed);
    if (column == 35) {
      row = FIELD_CELL(object->z.fixed);
      if (row == 9) {
        leader =
            ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(SE_STONE_RUMBLE);
        MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
        MapGrid_CopyCellAttributes(35, 0, 1, 1, column, row);
        object->motionFlags = done;
        Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, 3);
        if (object->z.fixed > leader->z.fixed)
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
        for (i = 0; i < 128; i++) {
          object->y.fixed -= px(1) / 8;
          wait(1);
        }
        Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 3);
        Actor_SetPosition(ACTOR_RIGHT_PILLAR_PUDDLE, 568, 152);
        Event_Wait(20);
        Event_SetFlag(FLAG_RIGHT_PILLAR_SUNK);
        if (object->z.fixed > leader->z.fixed) {
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  done = Event_GetFlag(FLAG_LEFT_PILLAR_SUNK);
  if (!done && Event_GetFlag(FLAG_LEFT_PILLAR_FULL)) {
    object = ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR));
    column = FIELD_CELL(object->x.fixed);
    if (column == 29) {
      row = FIELD_CELL(object->z.fixed);
      if (row == 9) {
        leader =
            ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(SE_STONE_RUMBLE);
        MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
        MapGrid_CopyCellAttributes(35, 0, 1, 1, column, row);
        object->motionFlags = done;
        Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, 3);
        if (object->z.fixed > leader->z.fixed)
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
        for (i = 0; i < 128; i++) {
          object->y.fixed -= px(1) / 8;
          wait(1);
        }
        Actor_SetAnimation(ACTOR_LEFT_PILLAR, 3);
        Actor_SetPosition(ACTOR_LEFT_PILLAR_PUDDLE, 472, 152);
        Event_Wait(20);
        Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
        if (object->z.fixed > leader->z.fixed) {
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK) &&
      Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(SE_THUMP);
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_Wait(20);
    Event_SetFlag(FLAG_PILLAR_WAY_OPEN);
    Event_End();
  }
}

void LemurianShip_LiftPillar(void) {
  Actor_LiftTargetBlock(gLemurianShip_Pillars);
}

void LemurianShip_SetDownPillar(void) {
  Actor_PlaceMovedBlock();
  LemurianShip_CheckPillarsOnDroplets();
}

#define DEFINE_DOUSE_PILLAR(name, n1, n2)                                      \
  void name(void) {                                                            \
    Sound_Play(SE_DRIP);                                                       \
    Actor_SetAnimationById(n1, 2);                                             \
    GameFlag_Set(n2);                                                          \
    LemurianShip_CheckPillarsOnDroplets();                                     \
  }

DEFINE_DOUSE_PILLAR(LemurianShip_DouseRightPillar, 8, FLAG_RIGHT_PILLAR_FULL)

DEFINE_DOUSE_PILLAR(LemurianShip_DouseLeftPillar, 9, FLAG_LEFT_PILLAR_FULL)

void LemurianShip_ShowRightPillarPuddle(void) {
  FieldActor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR_PUDDLE)), 0);
}

void LemurianShip_ShowLeftPillarPuddle(void) {
  FieldActor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR_PUDDLE)), 0);
}

#define DEFINE_RAISE_PILLAR(name, n1, n2, n3, n4)                              \
  void name(void) {                                                            \
    struct FieldWork *work = Ram_HeapSlots->fieldWork;                         \
    struct Actor *object = ((struct Actor *)Actor_GetById(n1));                \
    u32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    work->raisedTrigger = 0;                                                   \
    GameFlag_Clear(n2);                                                        \
    GameFlag_Clear(n3);                                                        \
    Event_Wait(20);                                                            \
    Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);                                       \
    for (i = 0; i <= 63; i++) {                                                \
      object->y.fixed += px(1) / 4;                                            \
      wait(1);                                                                 \
    }                                                                          \
    Actor_SetSpritePriority(n1, 2);                                            \
    MapGrid_CopyCellPictureIndicesRect(4, 123, n4, 69, 1, 1);                  \
    MapGrid_CopyCellAttributes(36, 0, 1, 1, n4, 9);                            \
    Sound_Play(SE_THUMP);                                                      \
    MapGrid_CopyCellsTo(7, 124, 31, 64, 3, 2);                                 \
    GameFlag_Clear(FLAG_PILLAR_WAY_OPEN);                                      \
    Event_Wait(20);                                                            \
    Event_End();                                                               \
  }

DEFINE_RAISE_PILLAR(LemurianShip_RaiseRightPillar, 8, FLAG_RIGHT_PILLAR_SUNK,
                    FLAG_RIGHT_PILLAR_FULL, 35)

DEFINE_RAISE_PILLAR(LemurianShip_RaiseLeftPillar, 9, FLAG_LEFT_PILLAR_SUNK,
                    FLAG_LEFT_PILLAR_FULL, 29)

/// Parch dries the flooded hold: the party and the chest the flood carried off
/// are set back where the floor is dry.
void LemurianShip_ParchHold(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Field_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
  Actor_SetPositionFixed(gMainRoutineWork.leaderActorId, leader->x.fixed,
                         leader->z.fixed - px(304));
  object = ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR));
  Actor_SetPositionFixed(FIELD_TREASURE_ACTOR, object->x.fixed,
                         object->z.fixed - px(304));
  FieldActor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), 0);
  wait(10);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
  wait(1);
  MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_SetFlag(FLAG_HOLD_PARCHED);
  PaletteFade_SetTarget(0x10000, 0);
  Event_End();
}

void LemurianShip_BeginParch(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_ResolvePendingActions(0);
  Event_End();
}

#define DEFINE_SET_UP_PUDDLE(name, n1)                                         \
  void name(s32 id) {                                                          \
    struct Actor *object = ((struct Actor *)Actor_GetById(id));                \
                                                                               \
    ((struct Actor *)Actor_GetById(id))->priorityFlags |= 0x20;                \
    Actor_SetSpritePriority(id, n1);                                           \
    object->priorityFlags &= ~2;                                               \
    object->riseCounter = 1;                                                   \
  }

DEFINE_SET_UP_PUDDLE(LemurianShip_SetUpPuddle, 1)

// From patch 1; in the Japanese release actors 10 and 11 also use action 1.
#if TLA_PATCH_1
DEFINE_SET_UP_PUDDLE(LemurianShip_SetUpLowPuddle, 2)
#endif

void LemurianShip_KeepSixJellyRoomPuddles(void) {
  ((struct Actor *)Actor_GetById(ACTOR_LIFT_PUDDLE))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(ACTOR_LIFT_PUDDLE, 3);
  ((struct Actor *)Actor_GetById(ACTOR_LIFT_PUDDLE))->priorityFlags &= ~2;
  LemurianShip_SetUpPuddle(16);
  LemurianShip_SetUpPuddle(17);
  LemurianShip_SetUpPuddle(18);
  LemurianShip_SetUpPuddle(19);
  LemurianShip_SetUpPuddle(20);
}

void LemurianShip_KeepSevenJellyRoomPuddles(void) {
  LemurianShip_SetUpPuddle(16);
  LemurianShip_SetUpPuddle(17);
  LemurianShip_SetUpPuddle(18);
  LemurianShip_SetUpPuddle(19);
  LemurianShip_SetUpPuddle(20);
  LemurianShip_SetUpPuddle(22);
}

void LemurianShip_KeepPillarRoomPuddles(void) {
#if !TLA_PATCH_1
  LemurianShip_SetUpPuddle(10);
  LemurianShip_SetUpPuddle(11);
#else
  LemurianShip_SetUpLowPuddle(10);
  LemurianShip_SetUpLowPuddle(11);
#endif
}

void LemurianShip_RippleAroundLeader(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  MapGrid_CopyCellsTo(74, 105, 4, 85, 8, 6);
  if (z == 25) {
    if (x == 4)
      MapGrid_CopyCellsTo(18, 126, 4, 85, 1, 2);
  } else if (z == 26) {
    if (x == 7)
      MapGrid_CopyCellsTo(18, 126, 7, 86, 1, 1);
  } else if (z == 27) {
    if (x == 4)
      MapGrid_CopyCellsTo(18, 126, 4, 87, 1, 2);
    else if (x == 6)
      MapGrid_CopyCellsTo(18, 126, 6, 87, 1, 2);
    else if (x == 7)
      MapGrid_CopyCellsTo(18, 126, 7, 87, 1, 2);
    else if (x == 9)
      MapGrid_CopyCellsTo(18, 126, 9, 87, 1, 2);
  } else if (z == 29) {
    if (x == 6)
      MapGrid_CopyCellsTo(18, 126, 6, 89, 1, 2);
  }
}

void LemurianShip_WatchPillarPuddles(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;

  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR_PUDDLE));
    if ((object->x.fixed | object->z.fixed) == 0) {
      work->raisedTrigger = 200;
      Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 1);
    }
  }
  if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR_PUDDLE));
    if ((object->x.fixed | object->z.fixed) == 0) {
      work->raisedTrigger = 201;
      Actor_SetAnimation(ACTOR_LEFT_PILLAR, 1);
    }
  }
}

void LemurianShip_DrawLightToOrb(void) {
  s32 position[3];
  struct Actor *object;

  SET_VECTOR(position, px(808), px(8), px(140));
  Vector_AddPolarOffset(0x200000, Random16(), position);
  object = ((struct Actor *)Import_ActorCreate(0x2a2, position[0], position[1],
                                               position[2]));
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 0);
    FieldActor_SetMoveTarget(object, 808, 8, 140);
    Actor_StartScript(object, gLemurianShip_LightToOrb);
  }
}

/// In the engine room Piers sets the Black Crystal in its pedestal and the
/// engine starts: the monsters are gone and the ship sails out.
/// "How did all these monsters get on board?"
void LemurianShip_SetBlackCrystal(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemurianShipHowDidAllTheseMonsters);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 808, 208);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 176, 1);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_ENGINE_ROOM_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_ENGINE_ROOM_KRADEN, 0.8);
  Actor_SetScript(ACTOR_SHEBA, gLemurianShip_ShebaStepsUp);
  Actor_SetScript(ACTOR_JENNA, gLemurianShip_JennaStepsUp);
  Actor_SetScript(ACTOR_ENGINE_ROOM_KRADEN, gLemurianShip_KradenStepsUp);
  Actor_RunScriptAndWait(ACTOR_PIERS, gLemurianShip_PiersToPedestal);
  Event_Wait(20);
  // "How did all these monsters get on board? I could swear I locked the..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  // "It doesn't matter. Once we get this ship moving, I'm sure they'll..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  // "OK, now, I must set the black orb in this pedestal."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PIERS, 808, 148);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Actor_SetSpritePriority(ACTOR_PIERS, 1);
  Event_PlaySound(SE_SHIMMER);
  Task_Add(LemurianShip_DrawLightToOrb, TASK_PASS_VBLANK);
  Event_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(20);
  Task_Remove(LemurianShip_DrawLightToOrb);
  Event_PlaySound(SE_BLACK_CRYSTAL_SET);
  Field_SetCameraBounds(0x2b00000, 0, 0x3a00000, 0x2200000);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 448, 0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 808, 480);
  Actor_SetPosition(ACTOR_PIERS, 808, 420);
  Actor_SetPosition(ACTOR_SHEBA, 784, 468);
  Actor_SetPosition(ACTOR_JENNA, 832, 468);
  Actor_SetPosition(ACTOR_ENGINE_ROOM_KRADEN, 792, 488);
  MapGrid_CenterOnFollowedPoint();
  Event_Wait(20);
  Event_PlaySound(SE_GLITTER);
  Event_Wait(80);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  PartyInventory_Remove(ITEM_BLACK_CRYSTAL);
  Event_SetFlag(FLAG_SHIP_ENGINE_RUNNING);
  Event_SetFlag(FLAG_JELLIES_BEATEN);
  Event_SetFlag((FLAG_JELLIES_BEATEN + 1));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 3));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 4));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 5));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 6));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 7));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 8));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 9));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 10));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 11));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 12));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 14));
  Event_SetFlag((FLAG_JELLIES_BEATEN + 2));
  Event_SetFlag(FLAG_AQUA_HYDRA_BEATEN);
  gMainRoutineWork.ownerOverride = 1;
  Import_EventSetExitRequest(90);
}

void LemurianShip_MeltJellyIntoPuddle(s32 actor, s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  Event_Start();
  Event_ResolvePendingActions(0);
  object->scaleX = Q16_ONE / 16;
  object->scaleY = Q16_ONE / 16;
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Event_PlaySound(SE_CURSE);
  Actor_SetScript(actor, gLemurianShip_JellyMelts);
  Actor_RunScriptAndWait(id, gLemurianShip_PuddleSpreads);
  Event_Wait(20);
  Event_End();
}

void LemurianShip_ScareJellyOnArrival(void) {
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 2)) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    LemurianShip_JellyFlees();
    Event_End();
  }
}

void LemurianShip_ScareJelly(void) {
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 2)) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    LemurianShip_JellyFlees();
    Event_End();
  }
}

void LemurianShip_JellyFlees(void) {
  struct Actor *object =
      ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A));

  FieldActor_ShowEmote(ACTOR_SIX_JELLY_ROOM_JELLY_A,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2);
  object->gravity = px(1) / 2;
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 1);
  Actor_SetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 0.8);
  Actor_Jump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 6, 0);
  Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 832, 88);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 1.6);
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 3);
  Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 824, 72);
  Actor_FaceDirection(ACTOR_SIX_JELLY_ROOM_JELLY_A, FACING_SOUTH_SOUTHEAST, 40);
  Event_SetFlag(FLAG_JELLY_FLED);
}

void LemurianShip_FloodHold(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_SCATTER);
  Event_Wait(120);
  Event_End();
}

void LemurianShip_SetUpTwoJellyRoom(void);

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (Event_GetFlag(0x9b4))
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
    switch (gMainRoutineWork.entrance) {
    case 3:
    case 4:
    case 5:
      LemurianShip_SetUpTwoJellyRoom();
      break;
    case 6:
    case 7:
    case 8:
      LemurianShip_SetUpStairRoom();
      break;
    case 25:
    case 26:
    case 27:
    case 28:
      LemurianShip_SetUpSixJellyRoom();
      break;
    case 15:
    case 16:
    case 17:
    case 18:
      LemurianShip_SetUpLedgeRoom();
      break;
    case 29:
    case 30:
      LemurianShip_SetUpSevenJellyRoom();
      break;
    }
  } else if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
      LemurianShip_SetUpHold();
      break;
    case 6:
    case 10:
      LemurianShip_SetUpPillarRoom();
      break;
    case 9:
      LemurianShip_SetUpEngineRoom();
      break;
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  u32 i;

  i = 0;
next:
  if (Event_GetFlag(FLAG_FIGHTING_JELLY + i)) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->sequenceResult == 1) {
      Event_SetFlag(FLAG_JELLIES_BEATEN + i);
      Event_SetFlag(FLAG_JELLY_MELTING + i);
    }
  }
  Event_ClearFlag(FLAG_FIGHTING_JELLY + i);
  i++;
  if (i < 16)
    goto next;
  return 0;
}

/// The first room below deck: its two jellies, or the puddles they left, and a
/// jelly just beaten melting into its puddle.
void LemurianShip_SetUpTwoJellyRoom(void) {
  s32 flag;

  if (Event_GetFlag(FLAG_JELLIES_BEATEN)) {
    flag = Event_GetFlag(FLAG_JELLY_MELTING);
    if (flag != 0) {
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      Actor_SetPositionAndResetMotion(
          ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A)),
          px(104), 0, px(424));
      ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A))
          ->groundHeight = 0;
    } else {
      Actor_SetPosition(ACTOR_TWO_JELLY_ROOM_JELLY_A, 0, 0);
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      FieldActor_SetPositionAndResetMotion(
          ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A)),
          px(104), 0, px(424));
      ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A))
          ->groundHeight = 0;
    }
    wait(1);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 1))) {
    if (Event_GetFlag((FLAG_JELLY_MELTING + 1)))
      Actor_SetSpritePriority(ACTOR_TWO_JELLY_ROOM_JELLY_B, 1);
    else
      Actor_SetPosition(ACTOR_TWO_JELLY_ROOM_JELLY_B, 0, 0);
    FieldActor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_B)), px(104),
        px(-32), px(536));
    ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_B))
        ->groundHeight = -px(32);
    Actor_SetSpritePriority(ACTOR_TWO_JELLY_ROOM_PUDDLE_B, 1);
    wait(1);
  }
  FlagBlock_PlaceAll(gLemurianShip_TwoJellyRoomPuddles);
  if (Event_GetFlag(FLAG_JELLY_MELTING)) {
    LemurianShip_MeltJellyIntoPuddle(8, 10);
    Event_ClearFlag(FLAG_JELLY_MELTING);
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 1))) {
    LemurianShip_MeltJellyIntoPuddle(9, 11);
    Event_ClearFlag((FLAG_JELLY_MELTING + 1));
  }
}

void LemurianShip_SetUpRaisedJelly(s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  ((struct Actor *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y.fixed = -px(32);
  object->groundHeight = -px(32);
}

/// Places a puddle up on the raised floor, where its jelly sat.
#define RAISE(id, x, z)                                                        \
  {                                                                            \
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(id)),  \
                                         x, px(-32), z);                       \
    ((struct Actor *)Actor_GetById(id))->groundHeight = -px(32);               \
  }

void LemurianShip_SetUpSixJellyRoom(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gLemurianShip_Crate);
  ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_CRATE))->priorityFlags |=
      32;
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_CRATE, 1);
  ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
      ->priorityFlags |= 32;
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 2))) {
    struct Actor *object = ((struct Actor *)Actor_GetById(ACTOR_LIFT_PUDDLE));

    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_LIFT_PUDDLE)), px(824), px(-32),
        px(72));
    object->groundHeight = -px(32);
    if (Event_GetFlag((FLAG_PUDDLE_FROZEN + 2)))
      MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
    else
      MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 2)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_A, 0, 0);
  }
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    if (Event_GetFlag(FLAG_LIFT_LOWERED)) {
      Actor_SetPosition(ACTOR_LIFT_PUDDLE, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
    } else {
      RAISE(15, 206 << 18, 144 << 15)
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      if (Event_GetFlag((FLAG_PUDDLE_FROZEN + 2)))
        MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
      else
        MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    }
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 3))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 46, 5);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 45, 5);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 5);
    RAISE(16, 186 << 18, 176 << 15)
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 3)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_B, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(10);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 4))) {
    struct Actor *block =
        ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_CRATE));

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 47, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 46, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 10);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 8);
    RAISE(17, 190 << 18, 152 << 16)
    if (FIELD_CELL(block->x.fixed) == 47 && FIELD_CELL(block->z.fixed) == 8)
      Actor_PlaceBlocks(gLemurianShip_Crate);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 4)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_C, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(11);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 5))) {
    struct Actor *block =
        ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_CRATE));
    s32 x = FIELD_CELL(block->x.fixed);
    s32 z = FIELD_CELL(block->z.fixed);

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 49, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 6);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 8);
    RAISE(18, 198 << 18, 240 << 15)
    if ((x == 48 && z == 7) || (x == 49 && z == 8))
      Actor_PlaceBlocks(gLemurianShip_Crate);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 5)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_D, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(12);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 6))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 7);
    RAISE(19, 206 << 18, 240 << 15)
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 6)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_E, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(13);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 7))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 51, 8);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 52, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 51, 10);
    RAISE(20, 206 << 18, 152 << 16)
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 7)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_F, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(14);
  }
  FlagBlock_PlaceAll(gLemurianShip_SixJellyRoomPuddles);
  Task_Add(LemurianShip_KeepSixJellyRoomPuddles, TASK_PASS_VBLANK);
  if (Event_GetFlag((FLAG_JELLY_MELTING + 2))) {
    LemurianShip_MeltJellyIntoPuddle(9, 15);
    Event_ClearFlag((FLAG_JELLY_MELTING + 2));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 3))) {
    LemurianShip_MeltJellyIntoPuddle(10, 16);
    Event_ClearFlag((FLAG_JELLY_MELTING + 3));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 4))) {
    LemurianShip_MeltJellyIntoPuddle(11, 17);
    Event_ClearFlag((FLAG_JELLY_MELTING + 4));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 5))) {
    LemurianShip_MeltJellyIntoPuddle(12, 18);
    Event_ClearFlag((FLAG_JELLY_MELTING + 5));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 6))) {
    LemurianShip_MeltJellyIntoPuddle(13, 19);
    Event_ClearFlag((FLAG_JELLY_MELTING + 6));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 7))) {
    LemurianShip_MeltJellyIntoPuddle(14, 20);
    Event_ClearFlag((FLAG_JELLY_MELTING + 7));
  }
  if (gMainRoutineWork.entrance == 26 &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    LemurianShip_ScareJellyOnArrival();
}

void LemurianShip_MatchCratePriority(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));

  Actor_SetSpritePriority(ACTOR_LEDGE_CRATE,
                          ((struct FieldSprite *)leader->sprite)->priority);
}

void LemurianShip_SetUpLedgeRoom(void) {
  // XXX: flag 8 is tested where the drop sets FLAG_CRATE_DROPPED; whether it is
  // a slip for that flag would be settled by returning from a battle here.
  if (Event_GetFlag(8))
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
  Actor_PlaceBlocks(gLemurianShip_Crate);
  LEMURIAN_SHIP_TASK_ADD(LemurianShip_MatchCratePriority, 0x480);
}

void LemurianShip_SetUpJelly(s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  ((struct Actor *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y.fixed = 0;
  object->groundHeight = 0;
}

/// The room of seven jellies: each beaten one leaves a puddle and opens its
/// cell.
void LemurianShip_SetUpSevenJellyRoom(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gLemurianShip_Crate);
  ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_CRATE))
      ->priorityFlags |= 32;
  Actor_SetSpritePriority(ACTOR_SEVEN_JELLY_ROOM_CRATE, 1);
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 8))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 32);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_A)),
        px(776), 0, px(520));
    ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_A))
        ->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 8)))
      Actor_SetPosition(ACTOR_SEVEN_JELLY_ROOM_JELLY_A, 0, 0);
  } else {
    LemurianShip_SetUpJelly(9);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 9))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 53, 32);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_B)),
        px(856), 0, px(520));
    ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_B))
        ->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 9)))
      Actor_SetPosition(ACTOR_SEVEN_JELLY_ROOM_JELLY_B, 0, 0);
  } else {
    LemurianShip_SetUpJelly(10);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 10))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 43, 37);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_C)),
        px(696), 0, px(600));
    ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_C))
        ->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 10)))
      Actor_SetPosition(ACTOR_SEVEN_JELLY_ROOM_JELLY_C, 0, 0);
  } else {
    LemurianShip_SetUpJelly(11);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 11))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 37);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_D)),
        px(776), 0, px(600));
    ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_D))
        ->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 11)))
      Actor_SetPosition(ACTOR_SEVEN_JELLY_ROOM_JELLY_D, 0, 0);
  } else {
    LemurianShip_SetUpJelly(12);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 12))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 50, 37);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_E)),
        px(808), 0, px(600));
    ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_E))
        ->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 12)))
      Actor_SetPosition(ACTOR_SEVEN_JELLY_ROOM_JELLY_E, 0, 0);
  } else {
    LemurianShip_SetUpJelly(13);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 14))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 43);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_G)),
        px(776), 0, px(696));
    ((struct Actor *)Actor_GetById(ACTOR_SEVEN_JELLY_ROOM_PUDDLE_G))
        ->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 14)))
      Actor_SetPosition(ACTOR_SEVEN_JELLY_ROOM_JELLY_G, 0, 0);
  } else {
    LemurianShip_SetUpJelly(15);
  }
  FlagBlock_PlaceAll(gLemurianShip_SevenJellyRoomPuddles);
  FieldTask_AddFrame(LemurianShip_KeepSevenJellyRoomPuddles, TASK_PASS_VBLANK);
  if (Event_GetFlag((FLAG_JELLY_MELTING + 8))) {
    LemurianShip_MeltJellyIntoPuddle(9, 16);
    Event_ClearFlag((FLAG_JELLY_MELTING + 8));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 9))) {
    LemurianShip_MeltJellyIntoPuddle(10, 17);
    Event_ClearFlag((FLAG_JELLY_MELTING + 9));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 10))) {
    LemurianShip_MeltJellyIntoPuddle(11, 18);
    Event_ClearFlag((FLAG_JELLY_MELTING + 10));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 11))) {
    LemurianShip_MeltJellyIntoPuddle(12, 19);
    Event_ClearFlag((FLAG_JELLY_MELTING + 11));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 12))) {
    LemurianShip_MeltJellyIntoPuddle(13, 20);
    Event_ClearFlag((FLAG_JELLY_MELTING + 12));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + 14))) {
    LemurianShip_MeltJellyIntoPuddle(15, 22);
    Event_ClearFlag((FLAG_JELLY_MELTING + 14));
  }
}

void LemurianShip_SetUpStairRoom(void) {
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    MapGrid_CopyCellsTo(17, 125, 27, 63, 3, 3);
    MapGrid_CopyCellAttributes(25, 0, 5, 3, 25, 4);
  }
}

/// The hold of the Aqua Hydra: once it is beaten the hold is flooded, and once
/// Parch has dried it the chest lies on dry floor.
void LemurianShip_SetUpHold(void) {
  struct Actor *object;

  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= 32;
  LEMURIAN_SHIP_TASK_ADD(LemurianShip_RippleAroundLeader, 0x480);
  if (Event_GetFlag(FLAG_AQUA_HYDRA_BEATEN)) {
    object = ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR));
    object->motionFlags = 0;
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), px(136), px(-32),
        px(392));
    object->groundHeight = -px(16);
    Actor_SetSpritePriority(FIELD_TREASURE_ACTOR, 3);
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), 1);
    Actor_SetPosition(ACTOR_HOLD_JELLY_A, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_B, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_C, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_D, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_E, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_F, 0, 0);
    wait(1);
  }
  if (Event_GetFlag(FLAG_HOLD_PARCHED)) {
    struct Actor *marker =
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR));

    Actor_SetPositionFixed(FIELD_TREASURE_ACTOR, marker->x.fixed,
                           marker->z.fixed - px(304));
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), 0);
    MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
    MapGrid_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
    FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  if (Event_GetFlag((FLAG_JELLY_MELTING + 15))) {
    LemurianShip_FloodHold();
    Event_ClearFlag((FLAG_JELLY_MELTING + 15));
  }
}

/// The room of the two pillars Douse fills, as the player left them.
void LemurianShip_SetUpPillarRoom(void) {
  struct Actor *object;

  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gLemurianShip_Pillars);
  if (Event_GetFlag(FLAG_PILLAR_WAY_OPEN)) {
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_SetFlag(FLAG_RIGHT_PILLAR_SUNK);
    Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR)), 0);
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR));
    MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 35, 9);
    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(object, px(568), px(-16), px(152));
    Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, 3);
    Event_SetFlag(FLAG_RIGHT_PILLAR_FULL);
    Actor_SetPosition(ACTOR_RIGHT_PILLAR_PUDDLE, 568, 152);
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_FULL)) {
    if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK))
      Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 3);
    else
      Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 2);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR)), 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
  if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR));
    MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 29, 9);
    object->motionFlags = 0;
    object->y.fixed = -px(16);
    Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, 3);
    Event_SetFlag(FLAG_LEFT_PILLAR_FULL);
    Actor_SetPosition(ACTOR_LEFT_PILLAR_PUDDLE, 472, 152);
  }
  if (Event_GetFlag(FLAG_LEFT_PILLAR_FULL)) {
    if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK))
      Actor_SetAnimation(ACTOR_LEFT_PILLAR, 3);
    else
      Actor_SetAnimation(ACTOR_LEFT_PILLAR, 2);
  }
  wait(1);
  FlagBlock_PlaceAll(gLemurianShip_TwoJellyRoomPuddles);
  FieldTask_AddFrame(LemurianShip_KeepPillarRoomPuddles, TASK_PASS_VBLANK);
  if (!Event_GetFlag(FLAG_PUDDLE_FROZEN))
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR_PUDDLE)), 15);
  if (!Event_GetFlag((FLAG_PUDDLE_FROZEN + 1)))
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR_PUDDLE)), 15);
  LEMURIAN_SHIP_TASK_ADD(LemurianShip_WatchPillarPuddles, 0x480);
}

void LemurianShip_SetUpEngineRoom(void) {
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING) == 0)
    LemurianShip_SetBlackCrystal();
}

#include "text/msg_ids.h"

// clang-format off
s32 gLemurianShip_IntoPassage[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -Q16_ONE / 64),                                               
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

s32 gLemurianShip_JellyMelts[] = {
    23, 9, -512, 23, 10, -1024,
    0, 1, 13, 60, 0, 3,
    0, 1, 17,
};

u32 gLemurianShip_PuddleSpreads[] = {
    SC_ADD(SW_SCALE_X, 0x400),
    SC_ADD(SW_SCALE_Y, 0x400),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_END,
};

s32 gLemurianShip_HydraSwells[] = {
    SC_ADD(SW_SCALE_X, 0x3000),
    SC_ADD(SW_SCALE_Y, 0x3000),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x1800),
    SC_ADD(SW_SCALE_Y, -0x1800),
    SC_WAIT(4),
    SC_LOOP(8, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -Q16_ONE / 128),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 128),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

u32 gLemurianShip_PiersToPedestal[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(808, 0, 168), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gLemurianShip_ShebaStepsUp[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(784, 0, 196),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gLemurianShip_JennaStepsUp[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(832, 0, 196),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gLemurianShip_KradenStepsUp[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(792, 0, 216),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(10), SC_END,                                                     
  };

s16 gLemurianShip_Crate[] = {
    8, -1,
};

s16 gLemurianShip_TwoJellyRoomPuddles[] = {
    10, FLAG_PUDDLE_FROZEN, 11, FLAG_PUDDLE_FROZEN + 1, -1,
};

s16 gLemurianShip_SixJellyRoomPuddles[] = {
    16, FLAG_PUDDLE_FROZEN + 3, 17, FLAG_PUDDLE_FROZEN + 4, 18, FLAG_PUDDLE_FROZEN + 5, 19, FLAG_PUDDLE_FROZEN + 6,
    20, FLAG_PUDDLE_FROZEN + 7, -1,
};

s16 gLemurianShip_SevenJellyRoomPuddles[] = {
    16, FLAG_PUDDLE_FROZEN + 8, 17, FLAG_PUDDLE_FROZEN + 9, 18, FLAG_PUDDLE_FROZEN + 10, 19, FLAG_PUDDLE_FROZEN + 11,
    20, FLAG_PUDDLE_FROZEN + 12, 22, FLAG_PUDDLE_FROZEN + 14, -1,
};

s16 gLemurianShip_Pillars[] = {
    8, 9, -1,
};

s32 gLemurianShip_Entrances[] = {
    -0x10000, 172, 0x40000095, -0x10000, -1, 0xffff,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gLemurianShip_CabinsEntrances[] = {
    0xffff0009, 406, 0xc0000178, 0x1500000, 0x2680108, 424,
    0xffff000a, 550, 0xc0000179, 0x1500000, 0x2680108, 424,
    0xffff000b, 566, 0xc0000379, 0x1e80000, 0x2d80308, 936,
    0xffff000c, 648, 0xc000037e, 0x1e80000, 0x2d80308, 936,
    0xffff000d, 440, 216, 0x1800000, 0x2700080, 288,
    0xffff000e, 567, 0x400000d7, 0x1800000, 0x2700080, 288,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gLemurianShip_AquaHydraEntrances[] = {
    0, -1, 72, 0, 72, 0x4000, 0, -1,
    -1, -1, -1, 0, 1, -1, 72, 0,
    72, 0x4000, 0, 8, 16, 248, 256, 0,
    2, -1, 168, 0, 72, 0x4000, 0, 8,
    16, 248, 256, 0, 3, -1, 72, 0,
    216, 0xc000, 0, 8, 16, 248, 256, 0,
    4, -1, 184, 0, 216, 0xc000, 0, 8,
    16, 248, 256, 0, 11, -1, 72, 0,
    376, 0x4000, 0, 8, 320, 248, 560, 0,
    12, -1, 168, 0, 376, 0x4000, 0, 8,
    320, 248, 560, 0, 13, -1, 72, 0,
    512, 0xc000, 0, 8, 320, 248, 560, 0,
    14, -1, 184, 0, 512, 0xc000, 0, 8,
    320, 248, 560, 0, 15, -1, 120, 0,
    440, 0xc000, 0, 8, 320, 248, 560, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

u32 gLemurianShip_CabinsRegions[] = {
    0xffec01d2, 0x1da0244, 0x24cfff4, 0x13ffff, 0xffff, 0,
    0, 0,
};

u32 gLemurianShip_AquaHydraRegions[] = {
    0x100200, 0x21000be, 0xce0020, 0xaffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemurianShipCabins),
    SCENE_EXIT_UNLESS(1, LemurianShipDeck2, 1, 0x907),
    SCENE_EXIT_UNLESS(1, LemuriaDock, 3, 0x9b4),
    SCENE_EXIT_UNLESS(1, AttekaInlet, 4, EXIT_FLAG_NONE),
    SCENE_EXIT(2, LemurianShipCabins, 3),
    SCENE_EXIT(3, LemurianShipCabins, 2),
    SCENE_EXIT(4, LemurianShipCabins, 6),
    SCENE_EXIT(5, LemurianShipCabins, 13),
    SCENE_EXIT(6, LemurianShipCabins, 4),
    SCENE_EXIT(7, LemurianShipCabins, 19),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, 1, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, 11, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemurianShipAquaHydra, 2, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(9, LemurianShipAquaHydra, 12, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(9, LemurianShipAquaHydra, 17, EXIT_FLAG_NONE),
    SCENE_EXIT(10, LemurianShipCabins, 25),
    SCENE_EXIT(11, LemurianShipCabins, 26),
    SCENE_EXIT(12, LemurianShipCabins, 29),
    SCENE_EXIT(13, LemurianShipCabins, 5),
    SCENE_EXIT_UNLESS(14, LemurianShipAquaHydra, 3, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(14, LemurianShipAquaHydra, 13, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(14, LemurianShipAquaHydra, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemurianShipAquaHydra, 4, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(15, LemurianShipAquaHydra, 14, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(15, LemurianShipAquaHydra, 20, EXIT_FLAG_NONE),
    SCENE_EXIT(16, LemurianShipCabins, 27),
    SCENE_EXIT(17, LemurianShipCabins, 28),
    SCENE_EXIT(18, LemurianShipCabins, 30),
    SCENE_EXIT(19, LemurianShipAquaHydra, 10),
    SCENE_EXIT(20, LemurianShipCabins, 7),
    SCENE_EXIT(25, LemurianShipCabins, 10),
    SCENE_EXIT(26, LemurianShipCabins, 11),
    SCENE_EXIT(27, LemurianShipCabins, 16),
    SCENE_EXIT(28, LemurianShipCabins, 17),
    SCENE_EXIT(29, LemurianShipCabins, 12),
    SCENE_EXIT(30, LemurianShipCabins, 18),
    SCENE_EXITS(LemurianShipAquaHydra),
    SCENE_EXIT(1, LemurianShipCabins, 8),
    SCENE_EXIT(2, LemurianShipCabins, 9),
    SCENE_EXIT(3, LemurianShipCabins, 14),
    SCENE_EXIT(4, LemurianShipCabins, 15),
    SCENE_EXIT(6, LemurianShipAquaHydra, 7),
    SCENE_EXIT(7, LemurianShipAquaHydra, 6),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, 9, FLAG_SHIP_ENGINE_RUNNING),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, 19, EXIT_FLAG_NONE),
    SCENE_EXIT(9, LemurianShipAquaHydra, 8),
    SCENE_EXIT(10, LemurianShipCabins, 20),
    SCENE_EXIT(90, WorldMap, 69),
    SCENE_EXITS_END,
};

struct ActorPlacement gLemurianShip_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_TwoJellyRoomPlacements[] = {                                             
      {248, CONDITION_ALWAYS, 1, px(104), 0, px(424), FACING_SOUTH, 0,   
       0},                                                                     
      {248, CONDITION_ALWAYS, 1, px(104), 0, px(536), FACING_SOUTH, 0,   
       0},                                                                     
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},               
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},               
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gLemurianShip_SixJellyRoomPlacements[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_LedgeRoomPlacements[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_SevenJellyRoomPlacements[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_HoldPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIERS, 0x0, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_PillarRoomPlacements[] = {                                             
      {314, CONDITION_ALWAYS, 1, px(584), 0, px(168), FACING_SOUTH, 2,   
       0},                                                                     
      {314, CONDITION_ALWAYS, 1, px(472), 0, px(152), FACING_SOUTH, 2,   
       0},                                                                     
      {290, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},               
      {290, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},               
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gLemurianShip_EngineRoomPlacements[] = {                                             
      {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {SCENE_TABLE_END},                                                       
  };

s16 gLemurianShip_DoorOpening[] = {
    0, 126, 1, 2, 2, 1, 126, 1,
    2, 4, 2, 126, 1, 2, 4, -1,
};

s16 gLemurianShip_FirstDoorOpening[] = {
    10, 126, 1, 2, 4, 11, 126, 1,
    2, 4, -1, 4, 126, 2, 2, 2,
    -1, 0,
};

struct LemurianShipWarp gLemurianShip_Doors[] = {
    {2, (s32)gLemurianShip_FirstDoorOpening, 4, 3},
    {14, (s32)gLemurianShip_DoorOpening, 35, 11},
    {15, (s32)gLemurianShip_DoorOpening, 6, 45},
    {16, (s32)gLemurianShip_DoorOpening, 11, 45},
    {17, (s32)gLemurianShip_DoorOpening, 21, 45},
    {18, (s32)gLemurianShip_DoorOpening, 26, 46},
    {20, (s32)gLemurianShip_DoorOpening, 29, 28},
    {25, (s32)gLemurianShip_DoorOpening, 48, 2},
    {26, (s32)gLemurianShip_DoorOpening, 54, 2},
    {29, (s32)gLemurianShip_DoorOpening, 41, 29},
};

struct LemurianShipWarp gLemurianShip_LowerDoors[] = {
    {1, (s32)gLemurianShip_DoorOpening, 4, 2},
    {2, (s32)gLemurianShip_DoorOpening, 10, 2},
    {6, (s32)gLemurianShip_DoorOpening, 31, 3},
    {8, (s32)gLemurianShip_DoorOpening, 41, 51},
    {11, (s32)gLemurianShip_DoorOpening, 4, 21},
    {12, (s32)gLemurianShip_DoorOpening, 10, 21},
};

struct Trigger gLemurianShip_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_CabinTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShipItLooksLikeTheresCaptains))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShipItsPleasantlyCool))},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_TwoJellyRoomTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TALK, ACTOR_TWO_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightTwoJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_TWO_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightTwoJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TWO_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TWO_JELLY_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + 1), EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_SixJellyRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_C, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_D, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_E, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_F, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_LIFT_PUDDLE, (FLAG_PUDDLE_FROZEN + 2), EVENT(LemurianShip_FreezeLiftPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_A, (FLAG_PUDDLE_FROZEN + 3), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + 4), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_C, (FLAG_PUDDLE_FROZEN + 5), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_D, (FLAG_PUDDLE_FROZEN + 6), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_E, (FLAG_PUDDLE_FROZEN + 7), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SIX_JELLY_ROOM_CRATE, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(LemurianShip_ExamineLiftSwitch)},
    {TRIGGER_STEP, 41, (FLAG_PUDDLE_FROZEN + 2), EVENT(LemurianShip_StepOnLift)},
    {TRIGGER_STEP, 43, (FLAG_PUDDLE_FROZEN + 2), EVENT(LemurianShip_StepOnLiftWestSide)},
    {TRIGGER_STEP, 42, FLAG_JELLY_FLED, EVENT(LemurianShip_ScareJelly)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_LedgeRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftLedgeCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_CRATE, CONDITION_ALWAYS, EVENT(LemurianShip_PushCrateOffLedge)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftLedgeCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_PushCrateOffLedge)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_SevenJellyRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_SEVEN_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_SEVEN_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_SEVEN_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_SEVEN_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SEVEN_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SEVEN_JELLY_ROOM_JELLY_C, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SEVEN_JELLY_ROOM_JELLY_D, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SEVEN_JELLY_ROOM_JELLY_E, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_SEVEN_JELLY_ROOM_JELLY_G, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_SEVEN_JELLY_ROOM_JELLY_G, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SEVEN_JELLY_ROOM_JELLY_G, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVEN_JELLY_ROOM_PUDDLE_A, (FLAG_PUDDLE_FROZEN + 8), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVEN_JELLY_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + 9), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVEN_JELLY_ROOM_PUDDLE_C, (FLAG_PUDDLE_FROZEN + 10), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVEN_JELLY_ROOM_PUDDLE_D, (FLAG_PUDDLE_FROZEN + 11), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVEN_JELLY_ROOM_PUDDLE_E, (FLAG_PUDDLE_FROZEN + 12), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVEN_JELLY_ROOM_PUDDLE_G, (FLAG_PUDDLE_FROZEN + 14), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SEVEN_JELLY_ROOM_CRATE, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_HoldTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(LemurianShip_OpenLowerDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(LemurianShip_OpenLowerDoor)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, ACTOR_HOLD_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightAquaHydra)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_FIRST), 255, CONDITION_ALWAYS, EVENT(LemurianShip_BeginParch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 255, CONDITION_ALWAYS, EVENT(LemurianShip_ParchHold)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_PillarRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(LemurianShip_WalkIntoPassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, FLAG_RIGHT_PILLAR_SUNK, EVENT(LemurianShip_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_SUNK, EVENT(LemurianShip_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, FLAG_LEFT_PILLAR_SUNK, EVENT(LemurianShip_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_SUNK, EVENT(LemurianShip_SetDownPillar)},
    {TRIGGER_PUSH, 0, FLAG_PILLAR_WAY_OPEN, EVENT(LemurianShip_LiftPillar)},
    {TRIGGER_PUSHED, 0, FLAG_PILLAR_WAY_OPEN, EVENT(LemurianShip_SetDownPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_FULL, EVENT(LemurianShip_DouseRightPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_FULL, EVENT(LemurianShip_DouseLeftPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), 10, FLAG_PUDDLE_FROZEN, EVENT(LemurianShip_ShowRightPillarPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), 11, (FLAG_PUDDLE_FROZEN + 1), EVENT(LemurianShip_ShowLeftPillarPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_RIGHT_PILLAR_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_LEFT_PILLAR_PUDDLE, (FLAG_PUDDLE_FROZEN + 1), EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(LemurianShip_RaiseRightPillar)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(LemurianShip_RaiseLeftPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_LowerDeckTriggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(LemurianShip_WalkIntoPassage)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};

u32 gLemurianShip_LightToOrb[] = {
    0, 10, 1, 3, 0, 1,
    38,
};
// clang-format on
#undef RAISE
