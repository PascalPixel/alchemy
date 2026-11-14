/// @file
/// The Lemurian Ship below deck, at the East Indra Shore, in two scenes
/// (SceneId_LemurianShip1 and SceneId_LemurianShip2) whose rooms are told apart
/// by the entrance used. Aqua Jellies fill the cabins: each one beaten melts
/// into a puddle Frost turns to ice to stand on, and crates are pushed between
/// them. A frightened jelly flees down the lift; the last one calls the others
/// together into the Aqua Hydra, whose fall floods the hold until Parch dries
/// it. Douse fills the two pillars that sink on their droplet tiles and open
/// the way to the engine room, where Piers sets the Black Crystal and the
/// ship sails.
#include "shared/tables.h"
#include "field/field_work.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "module/scene/lemurian_ship.h"
#include "module/scene/scene.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "version.h"
#include "macros.h"
#include "objcmd.h"
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

extern s16 gLemurianShip_Room2Entrances[];

u8 *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_LemurianShip1)
    return gLemurianShip_Room1Entrances;
  if (scene == (s32)&SceneId_LemurianShip2)
    return gLemurianShip_Room2Entrances;
  return gLemurianShip_Entrances;
}

u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_LemurianShip1)
    return gLemurianShip_Room1Regions;
  if (scene == (s32)&SceneId_LemurianShip2)
    return gLemurianShip_Room2Regions;
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

void Map_CenterOnFollowedPoint(void);
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

struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip1) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip2) {
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
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gLemurianShip_Doors[found].x,
                      gLemurianShip_Doors[found].z, 1, 2);
  MapGrid_AnimateCells(gLemurianShip_Doors[found].value,
                       gLemurianShip_Doors[found].x,
                       gLemurianShip_Doors[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  {
    s32 owner = gMainRoutineWork.currentOwner;

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
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gLemurianShip_LowerDoors[found].x,
                      gLemurianShip_LowerDoors[found].z, 1, 2);
  MapGrid_AnimateCells(gLemurianShip_LowerDoors[found].value,
                       gLemurianShip_LowerDoors[found].x,
                       gLemurianShip_LowerDoors[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_CenterOnColumn(gMainRoutineWork.currentOwner, 2, -4);
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

    leader = ((struct Actor *)Actor_GetById(party->currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetAnimation(party->currentOwner, 2);
    Actor_SetSpeed(party->currentOwner, 0.5);
    x = leader->x;
    cell = x >> 16;
    Actor_SetMoveTarget(leader, ((8 - cell % 16) << 16) + x, leader->y,
                        leader->z);
    FieldActor_WaitForMove(leader);
    leader->facing = 0xc000;
    Actor_SetScript(party->currentOwner, gLemurianShip_IntoPassage);
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

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip1) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip2) {
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
      FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(15)),
                                           px(824), px(-32), px(72));
      ((struct Actor *)Actor_GetById(15))->terrainHeight = 0xffe00000;
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      Event_ClearFlag(FLAG_LIFT_LOWERED);
    } else if (Event_GetFlag((FLAG_PUDDLE_FROZEN + 2))) {
      Event_PlaySound(SE_ITEM_REMOVE);
    } else {
      Event_PlaySound(SE_CRUNCH);
      Actor_SetPosition(15, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
      Event_SetFlag(FLAG_LIFT_LOWERED);
    }
  } else {
    FieldMessage_Show((s32)MsgLemurianShip02, 1);
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
  object = ((struct Actor *)Actor_GetById(8));
  x = object->x >> 20;
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
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 840, 72);
    opened = Event_GetFlag((FLAG_JELLIES_BEATEN + 2));
    state = &leader->terrainId;
    if (opened) {
      Event_PlaySound(SE_243);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      MapGrid_SetShake(0, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      *state = 2;
      Event_Wait(20);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 840, 88);
      Event_Wait(20);
      Event_PlaySound(SE_CRUNCH);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      *state = 1;
    } else {
      helper = ((struct Actor *)Actor_GetById(9));
      Actor_SetSpritePriority(9, 2);
      Actor_SetSpeed(9, 2.4);
      ((struct Actor *)Actor_GetById(9))->actionFlags &= ~1;
      Actor_WalkToAndWait(9, 824, 88);
      Event_Wait(1);
      {
        /* FAKEMATCH(temporary): the bit in a local the byte is or'ed into. */
        u8 *bits = &((struct Actor *)Actor_GetById(9))->actionFlags;
        s32 value = 1;

        value |= *bits;
        *bits = value;
      }
      Event_PlaySound(SE_243);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      MapGrid_SetShake(0, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      {
        s32 two = 2;
        *state = two;
        Event_Wait(20);
        Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST,
                            20);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
        Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 840, 88);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_WEST);
        Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 9);
        helper->gravity = px(1);
        helper->terrainId = two;
      }
      Actor_Jump(9, 4, 0);
      Actor_MoveToAndWait(9, 832, 74);
      Actor_Jump(9, 4, 0);
      Actor_MoveToAndWait(9, 856, 88);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(20);
      Event_PlaySound(SE_CRUNCH);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      Actor_Stop(gMainRoutineWork.currentOwner);
      *state = 1;
      helper->terrainId = 0;
      wait(1);
      Event_ClearFlag(FLAG_JELLY_FLED);
    }
    Event_End();
  }
}

void LemurianShip_StepOnLiftWestSide(void) {
  struct Actor *leader;

  if (!Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 812, 72);
    Event_PlaySound(SE_243);
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    MapGrid_SetShake(0, 2, 1);
    Event_Wait(20);
    MapGrid_SetShake(0, 0, 0);
    MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
    leader->terrainId = 2;
    Event_Wait(20);
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 3.2);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 812, 88);
    Event_Wait(20);
    Event_PlaySound(SE_CRUNCH);
    MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
    Event_Wait(20);
    leader->terrainId = 1;
  }
}

#define DEFINE_JELLY_BATTLE(name, firstJelly, firstActor)                      \
  void name(s32 id) {                                                          \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Actor_FaceActorById(id, gMainRoutineWork.currentOwner, 0);                 \
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
  Event_PlaySound(MUS_000);
  Actor_StartRepeatedMotion(9, 2);
  Actor_StartRepeatedMotion(10, 2);
  Actor_StartRepeatedMotion(11, 2);
  Actor_StartRepeatedMotion(12, 2);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(20);
  Event_PlaySound(SE_BEAST_CRY);
  LemurianShip_LeapIntoHydra(9);
  LemurianShip_LeapIntoHydra(10);
  LemurianShip_LeapIntoHydra(11);
  LemurianShip_LeapIntoHydra(12);
  LemurianShip_LeapIntoHydra(13);
  Actor_WaitForMoveById(13);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Event_Wait(20);
  Event_PlaySound(MUS_052);
  Actor_RunScriptAndWait(8, gLemurianShip_HydraSwells);

  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination((s32)&SceneId_LemurianShip2, 15);
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
    object = ((struct Actor *)Actor_GetById(8));
    column = object->x >> 20;
    if (column == 35) {
      row = object->z >> 20;
      if (row == 9) {
        leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(SE_STONE_RUMBLE);
        MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
        MapGrid_CopyCellAttributes(35, 0, 1, 1, column, row);
        object->motionFlags = done;
        Actor_SetSpritePriority(8, 3);
        if (object->z > leader->z)
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
        for (i = 0; i < 128; i++) {
          object->y += 0xffffe000;
          wait(1);
        }
        Actor_SetAnimation(8, 3);
        Actor_SetPosition(10, 568, 152);
        Event_Wait(20);
        Event_SetFlag(FLAG_RIGHT_PILLAR_SUNK);
        if (object->z > leader->z) {
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  done = Event_GetFlag(FLAG_LEFT_PILLAR_SUNK);
  if (!done && Event_GetFlag(FLAG_LEFT_PILLAR_FULL)) {
    object = ((struct Actor *)Actor_GetById(9));
    column = object->x >> 20;
    if (column == 29) {
      row = object->z >> 20;
      if (row == 9) {
        leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(SE_STONE_RUMBLE);
        MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
        MapGrid_CopyCellAttributes(35, 0, 1, 1, column, row);
        object->motionFlags = done;
        Actor_SetSpritePriority(9, 3);
        if (object->z > leader->z)
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
        for (i = 0; i < 128; i++) {
          object->y += 0xffffe000;
          wait(1);
        }
        Actor_SetAnimation(9, 3);
        Actor_SetPosition(11, 472, 152);
        Event_Wait(20);
        Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
        if (object->z > leader->z) {
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
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
    Actor_SetModeById(n1, 2);                                                  \
    GameFlag_Set(n2);                                                          \
    LemurianShip_CheckPillarsOnDroplets();                                     \
  }

DEFINE_DOUSE_PILLAR(LemurianShip_DouseRightPillar, 8, FLAG_RIGHT_PILLAR_FULL)

DEFINE_DOUSE_PILLAR(LemurianShip_DouseLeftPillar, 9, FLAG_LEFT_PILLAR_FULL)

void LemurianShip_ShowRightPillarPuddle(void) {
  FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(10)), 0);
}

void LemurianShip_ShowLeftPillarPuddle(void) {
  FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(11)), 0);
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
      object->y += 0x4000;                                                     \
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
      ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Field_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
  Actor_SetPositionFixed(gMainRoutineWork.currentOwner, leader->x,
                         leader->z + 0xfed00000);
  object = ((struct Actor *)Actor_GetById(64));
  Actor_SetPositionFixed(64, object->x, object->z + 0xfed00000);
  FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(64)), 0);
  wait(10);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  wait(1);
  MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
  Map_CenterOnFollowedPoint();
  wait(1);
  Event_SetFlag(FLAG_HOLD_PARCHED);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
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
  ((struct Actor *)Actor_GetById(15))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(15, 3);
  ((struct Actor *)Actor_GetById(15))->priorityFlags &= ~2;
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
      ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

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
    object = ((struct Actor *)Actor_GetById(10));
    if ((object->x | object->z) == 0) {
      work->raisedTrigger = 200;
      Actor_SetAnimation(8, 1);
    }
  }
  if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(11));
    if ((object->x | object->z) == 0) {
      work->raisedTrigger = 201;
      Actor_SetAnimation(9, 1);
    }
  }
}

void LemurianShip_DrawLightToOrb(void) {
  s32 position[3];
  struct Actor *object;

  SET_VECTOR(position, 0x3280000, 0x80000, 0x8c0000);
  Vector_AddPolarOffset(0x200000, Random16(), position);
  object = ((struct Actor *)Import_ActorCreate(0x2a2, position[0], position[1],
                                               position[2]));
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 0);
    Actor_SetMoveTarget(object, px(808), px(8), px(140));
    Actor_StartScript(object, gLemurianShip_LightToOrb);
  }
}

/// In the engine room Piers sets the Black Crystal in its pedestal and the
/// engine starts: the monsters are gone and the ship sails out.
/// "How did all these monsters get on board?"
void LemurianShip_SetBlackCrystal(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemurianShip01);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 808, 208);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 176, 1);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(7, 0.8);
  Actor_SetSpeed(6, 0.8);
  Actor_SetSpeed(5, 0.8);
  Actor_SetSpeed(8, 0.8);
  Actor_SetScript(6, gLemurianShip_ShebaStepsUp);
  Actor_SetScript(5, gLemurianShip_JennaStepsUp);
  Actor_SetScript(8, gLemurianShip_KradenStepsUp);
  Actor_RunScriptAndWait(7, gLemurianShip_PiersToPedestal);
  Event_Wait(20);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_WalkToAndWait(7, 808, 148);
  Actor_RunRepeatedMotion(7, 2);
  Actor_SetSpritePriority(7, 1);
  Event_PlaySound(SE_SHIMMER);
  Task_Add(LemurianShip_DrawLightToOrb, 0x480);
  Event_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(20);
  Task_Remove(LemurianShip_DrawLightToOrb);
  Event_PlaySound(SE_251);
  Field_SetCameraBounds(0x2b00000, 0, 0x3a00000, 0x2200000);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 448, 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 808, 480);
  Actor_SetPosition(7, 808, 420);
  Actor_SetPosition(6, 784, 468);
  Actor_SetPosition(5, 832, 468);
  Actor_SetPosition(8, 792, 488);
  Map_CenterOnFollowedPoint();
  Event_Wait(20);
  Event_PlaySound(SE_GLITTER);
  Event_Wait(80);
  gFieldWork->transition = 513;
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
  struct Actor *object = ((struct Actor *)Actor_GetById(9));

  FieldActor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(9, 2);
  object->gravity = px(1) / 2;
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpeed(9, 0.8);
  Actor_Jump(9, 6, 0);
  Actor_MoveToAndWait(9, 832, 88);
  Event_Wait(20);
  Actor_SetSpeed(9, 1.6);
  Actor_SetSpritePriority(9, 3);
  Actor_MoveToAndWait(9, 824, 72);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 40);
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
  gFieldWork->transition = 0x209;
  if (Event_GetFlag(0x9b4))
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip1) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip2) {
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
      Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(10)),
                                      px(104), 0, px(424));
      ((struct Actor *)Actor_GetById(10))->terrainHeight = 0;
    } else {
      Actor_SetPosition(8, 0, 0);
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(10)),
                                           px(104), 0, px(424));
      ((struct Actor *)Actor_GetById(10))->terrainHeight = 0;
    }
    wait(1);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 1))) {
    if (Event_GetFlag((FLAG_JELLY_MELTING + 1)))
      Actor_SetSpritePriority(9, 1);
    else
      Actor_SetPosition(9, 0, 0);
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(11)),
                                         px(104), px(-32), px(536));
    ((struct Actor *)Actor_GetById(11))->terrainHeight = 0xffe00000;
    Actor_SetSpritePriority(11, 1);
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
  object->y = -px(32);
  object->terrainHeight = -px(32);
}

/// Places a puddle up on the raised floor, where its jelly sat.
#define RAISE(id, x, z)                                                        \
  {                                                                            \
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(id)),  \
                                         x, px(-32), z);                       \
    ((struct Actor *)Actor_GetById(id))->terrainHeight = -px(32);              \
  }

void LemurianShip_SetUpSixJellyRoom(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gLemurianShip_Crate);
  ((struct Actor *)Actor_GetById(8))->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  ((struct Actor *)Actor_GetById(9))->priorityFlags |= 32;
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 2))) {
    struct Actor *object = ((struct Actor *)Actor_GetById(15));

    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(15)),
                                         206 << 18, px(-32), 144 << 15);
    object->terrainHeight = -px(32);
    if (Event_GetFlag((FLAG_PUDDLE_FROZEN + 2)))
      MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
    else
      MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 2)))
      Actor_SetPosition(9, 0, 0);
  }
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    if (Event_GetFlag(FLAG_LIFT_LOWERED)) {
      Actor_SetPosition(15, 0, 0);
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
      Actor_SetPosition(10, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(10);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 4))) {
    struct Actor *block = ((struct Actor *)Actor_GetById(8));

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 47, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 46, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 10);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 8);
    RAISE(17, 190 << 18, 152 << 16)
    if (block->x >> 20 == 47 && block->z >> 20 == 8)
      Actor_PlaceBlocks(gLemurianShip_Crate);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 4)))
      Actor_SetPosition(11, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(11);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 5))) {
    struct Actor *block = ((struct Actor *)Actor_GetById(8));
    s32 x = block->x >> 20;
    s32 z = block->z >> 20;

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 49, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 6);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 8);
    RAISE(18, 198 << 18, 240 << 15)
    if ((x == 48 && z == 7) || (x == 49 && z == 8))
      Actor_PlaceBlocks(gLemurianShip_Crate);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 5)))
      Actor_SetPosition(12, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(12);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 6))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 7);
    RAISE(19, 206 << 18, 240 << 15)
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 6)))
      Actor_SetPosition(13, 0, 0);
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
      Actor_SetPosition(14, 0, 0);
  } else {
    LemurianShip_SetUpRaisedJelly(14);
  }
  FlagBlock_PlaceAll(gLemurianShip_SixJellyRoomPuddles);
  Task_Add(LemurianShip_KeepSixJellyRoomPuddles, 0x480);
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
      ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));

  Actor_SetSpritePriority(8, ((struct FieldSprite *)leader->sprite)->priority);
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
  object->y = 0;
  object->terrainHeight = 0;
}

/// The room of seven jellies: each beaten one leaves a puddle and opens its
/// cell.
void LemurianShip_SetUpSevenJellyRoom(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gLemurianShip_Crate);
  ((struct Actor *)Actor_GetById(8))->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 8))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 32);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(16)),
                                    px(776), 0, px(520));
    ((struct Actor *)Actor_GetById(16))->terrainHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 8)))
      Actor_SetPosition(9, 0, 0);
  } else {
    LemurianShip_SetUpJelly(9);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 9))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 53, 32);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(17)),
                                    px(856), 0, px(520));
    ((struct Actor *)Actor_GetById(17))->terrainHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 9)))
      Actor_SetPosition(10, 0, 0);
  } else {
    LemurianShip_SetUpJelly(10);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 10))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 43, 37);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(18)),
                                    px(696), 0, px(600));
    ((struct Actor *)Actor_GetById(18))->terrainHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 10)))
      Actor_SetPosition(11, 0, 0);
  } else {
    LemurianShip_SetUpJelly(11);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 11))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 37);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(19)),
                                    px(776), 0, px(600));
    ((struct Actor *)Actor_GetById(19))->terrainHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 11)))
      Actor_SetPosition(12, 0, 0);
  } else {
    LemurianShip_SetUpJelly(12);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 12))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 50, 37);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(20)),
                                    px(808), 0, px(600));
    ((struct Actor *)Actor_GetById(20))->terrainHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 12)))
      Actor_SetPosition(13, 0, 0);
  } else {
    LemurianShip_SetUpJelly(13);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + 14))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 43);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(22)),
                                    px(776), 0, px(696));
    ((struct Actor *)Actor_GetById(22))->terrainHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + 14)))
      Actor_SetPosition(15, 0, 0);
  } else {
    LemurianShip_SetUpJelly(15);
  }
  FlagBlock_PlaceAll(gLemurianShip_SevenJellyRoomPuddles);
  FieldTask_AddFrame(LemurianShip_KeepSevenJellyRoomPuddles, 0x480);
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

  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  LEMURIAN_SHIP_TASK_ADD(LemurianShip_RippleAroundLeader, 0x480);
  if (Event_GetFlag(FLAG_AQUA_HYDRA_BEATEN)) {
    object = ((struct Actor *)Actor_GetById(64));
    object->motionFlags = 0;
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(64)),
                                    px(136), px(-32), px(392));
    object->terrainHeight = 0xfff00000;
    Actor_SetSpritePriority(64, 3);
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(64)), 1);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    wait(1);
  }
  if (Event_GetFlag(FLAG_HOLD_PARCHED)) {
    struct Actor *marker = ((struct Actor *)Actor_GetById(64));

    Actor_SetPositionFixed(64, marker->x, marker->z + 0xfed00000);
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(64)), 0);
    MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
    MapGrid_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Map_CenterOnFollowedPoint();
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

  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gLemurianShip_Pillars);
  if (Event_GetFlag(FLAG_PILLAR_WAY_OPEN)) {
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_SetFlag(FLAG_RIGHT_PILLAR_SUNK);
    Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(8)), 0);
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(8));
    MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 35, 9);
    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(object, px(568), px(-16), px(152));
    Actor_SetSpritePriority(8, 3);
    Event_SetFlag(FLAG_RIGHT_PILLAR_FULL);
    Actor_SetPosition(10, 568, 152);
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_FULL)) {
    if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK))
      Actor_SetAnimation(8, 3);
    else
      Actor_SetAnimation(8, 2);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(9)), 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
  if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    object = ((struct Actor *)Actor_GetById(9));
    MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 29, 9);
    object->motionFlags = 0;
    object->y = 0xfff00000;
    Actor_SetSpritePriority(9, 3);
    Event_SetFlag(FLAG_LEFT_PILLAR_FULL);
    Actor_SetPosition(11, 472, 152);
  }
  if (Event_GetFlag(FLAG_LEFT_PILLAR_FULL)) {
    if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK))
      Actor_SetAnimation(9, 3);
    else
      Actor_SetAnimation(9, 2);
  }
  wait(1);
  FlagBlock_PlaceAll(gLemurianShip_TwoJellyRoomPuddles);
  FieldTask_AddFrame(LemurianShip_KeepPillarRoomPuddles, 0x480);
  if (!Event_GetFlag(FLAG_PUDDLE_FROZEN))
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(10)), 15);
  if (!Event_GetFlag((FLAG_PUDDLE_FROZEN + 1)))
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(11)), 15);
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
      SC_ADD(SW_SCALE_X, -1024),                                               
      SC_ADD(SW_SCALE_Y, -1024),                                               
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
    SC_ADD(SW_SCALE_X, -512),
    SC_ADD(SW_SCALE_Y, -512),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

u32 gLemurianShip_PiersToPedestal[] =                                                                 
      {                                                                        
          SC_ARG(2), SC_MOVE(px(808), 0, px(168)), SC_SYNC, SC_ARG(1),   
          SC_END,                                                              
  };

u32 gLemurianShip_ShebaStepsUp[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(px(784), 0, px(196)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gLemurianShip_JennaStepsUp[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(px(832), 0, px(196)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gLemurianShip_KradenStepsUp[] = {                                                               
      SC_ARG(2),   SC_MOVE(px(792), 0, px(216)),                         
      SC_SYNC,     SC_ARG(1),                                                  
      SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),              
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

u32 gLemurianShip_Room1Entrances[] = {
    0xffff0009, 406, 0xc0000178, 0x1500000, 0x2680108, 424,
    0xffff000a, 550, 0xc0000179, 0x1500000, 0x2680108, 424,
    0xffff000b, 566, 0xc0000379, 0x1e80000, 0x2d80308, 936,
    0xffff000c, 648, 0xc000037e, 0x1e80000, 0x2d80308, 936,
    0xffff000d, 440, 216, 0x1800000, 0x2700080, 288,
    0xffff000e, 567, 0x400000d7, 0x1800000, 0x2700080, 288,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gLemurianShip_Room2Entrances[] = {
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

u32 gLemurianShip_Room1Regions[] = {
    0xffec01d2, 0x1da0244, 0x24cfff4, 0x13ffff, 0xffff, 0,
    0, 0,
};

u32 gLemurianShip_Room2Regions[] = {
    0x100200, 0x21000be, 0xce0020, 0xaffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemurianShip1),
    SCENE_EXIT_UNLESS(1, LemurianShipDeck2, 1, 0x907),
    SCENE_EXIT_UNLESS(1, LemuriaDock, 3, 0x9b4),
    SCENE_EXIT_UNLESS(1, AttekaInlet1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT(2, LemurianShip1, 3),
    SCENE_EXIT(3, LemurianShip1, 2),
    SCENE_EXIT(4, LemurianShip1, 6),
    SCENE_EXIT(5, LemurianShip1, 13),
    SCENE_EXIT(6, LemurianShip1, 4),
    SCENE_EXIT(7, LemurianShip1, 19),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 1, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 11, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemurianShip2, 2, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(9, LemurianShip2, 12, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(9, LemurianShip2, 17, EXIT_FLAG_NONE),
    SCENE_EXIT(10, LemurianShip1, 25),
    SCENE_EXIT(11, LemurianShip1, 26),
    SCENE_EXIT(12, LemurianShip1, 29),
    SCENE_EXIT(13, LemurianShip1, 5),
    SCENE_EXIT_UNLESS(14, LemurianShip2, 3, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(14, LemurianShip2, 13, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(14, LemurianShip2, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemurianShip2, 4, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(15, LemurianShip2, 14, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(15, LemurianShip2, 20, EXIT_FLAG_NONE),
    SCENE_EXIT(16, LemurianShip1, 27),
    SCENE_EXIT(17, LemurianShip1, 28),
    SCENE_EXIT(18, LemurianShip1, 30),
    SCENE_EXIT(19, LemurianShip2, 10),
    SCENE_EXIT(20, LemurianShip1, 7),
    SCENE_EXIT(25, LemurianShip1, 10),
    SCENE_EXIT(26, LemurianShip1, 11),
    SCENE_EXIT(27, LemurianShip1, 16),
    SCENE_EXIT(28, LemurianShip1, 17),
    SCENE_EXIT(29, LemurianShip1, 12),
    SCENE_EXIT(30, LemurianShip1, 18),
    SCENE_EXITS(LemurianShip2),
    SCENE_EXIT(1, LemurianShip1, 8),
    SCENE_EXIT(2, LemurianShip1, 9),
    SCENE_EXIT(3, LemurianShip1, 14),
    SCENE_EXIT(4, LemurianShip1, 15),
    SCENE_EXIT(6, LemurianShip2, 7),
    SCENE_EXIT(7, LemurianShip2, 6),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 9, FLAG_SHIP_ENGINE_RUNNING),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 19, EXIT_FLAG_NONE),
    SCENE_EXIT(9, LemurianShip2, 8),
    SCENE_EXIT(10, LemurianShip1, 20),
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
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_LedgeRoomPlacements[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_SevenJellyRoomPlacements[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gLemurianShip_HoldPlacements[] = {
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(7, 0x0, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShip03))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShip04))},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_TwoJellyRoomTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(LemurianShip_FightTwoJellyRoomJelly)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(LemurianShip_FightTwoJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, FLAG_PUDDLE_FROZEN, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, (FLAG_PUDDLE_FROZEN + 1), EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_SixJellyRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 15, (FLAG_PUDDLE_FROZEN + 2), EVENT(LemurianShip_FreezeLiftPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, (FLAG_PUDDLE_FROZEN + 3), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, (FLAG_PUDDLE_FROZEN + 4), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, (FLAG_PUDDLE_FROZEN + 5), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, (FLAG_PUDDLE_FROZEN + 6), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, (FLAG_PUDDLE_FROZEN + 7), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(LemurianShip_PushCrateOffLedge)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftLedgeCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_PushCrateOffLedge)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_SevenJellyRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TALK_TRIGGER_FACING(FACING_EAST), 9, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_WEST), 10, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_EAST), 15, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_WEST), 15, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(LemurianShip_FightSevenJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, (FLAG_PUDDLE_FROZEN + 8), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, (FLAG_PUDDLE_FROZEN + 9), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, (FLAG_PUDDLE_FROZEN + 10), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, (FLAG_PUDDLE_FROZEN + 11), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, (FLAG_PUDDLE_FROZEN + 12), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 22, (FLAG_PUDDLE_FROZEN + 14), EVENT(LemurianShip_FreezeSevenJellyRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_HoldTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(LemurianShip_OpenLowerDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(LemurianShip_OpenLowerDoor)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(LemurianShip_FightAquaHydra)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_FIRST), 255, CONDITION_ALWAYS, EVENT(LemurianShip_BeginParch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 255, CONDITION_ALWAYS, EVENT(LemurianShip_ParchHold)},
    {TRIGGER_TABLE_END},
};

struct Trigger gLemurianShip_PillarRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(LemurianShip_WalkIntoPassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, FLAG_RIGHT_PILLAR_SUNK, EVENT(LemurianShip_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_RIGHT_PILLAR_SUNK, EVENT(LemurianShip_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, FLAG_LEFT_PILLAR_SUNK, EVENT(LemurianShip_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_LEFT_PILLAR_SUNK, EVENT(LemurianShip_SetDownPillar)},
    {TRIGGER_PUSH, 0, FLAG_PILLAR_WAY_OPEN, EVENT(LemurianShip_LiftPillar)},
    {TRIGGER_PUSHED, 0, FLAG_PILLAR_WAY_OPEN, EVENT(LemurianShip_SetDownPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, FLAG_RIGHT_PILLAR_FULL, EVENT(LemurianShip_DouseRightPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, FLAG_LEFT_PILLAR_FULL, EVENT(LemurianShip_DouseLeftPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), 10, FLAG_PUDDLE_FROZEN, EVENT(LemurianShip_ShowRightPillarPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), 11, (FLAG_PUDDLE_FROZEN + 1), EVENT(LemurianShip_ShowLeftPillarPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, FLAG_PUDDLE_FROZEN, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, (FLAG_PUDDLE_FROZEN + 1), EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
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
