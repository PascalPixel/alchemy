/// @file
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

extern s16 gData_0200312c[];

u8 *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_LemurianShip1)
    return gLemurianShip_Room1Entrances;
  if (scene == (s32)&SceneId_LemurianShip2)
    return gData_0200312c;
  return gLemurianShip_Entrances;
}

u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_LemurianShip1)
    return gData_02003234;
  if (scene == (s32)&SceneId_LemurianShip2)
    return gData_02003254;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

// object the overlay links.

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

// ---- this overlay

void Func_02000d10(s32 id);
void Func_02000e2c(void);
void Func_02001380(s32 id);
void Func_020013b8(s32 id);

extern s32 gData_02002e30[];
extern u32 gData_02002e6c[];
extern s32 gData_02002e9c[];
extern s16 gData_02003010[];
extern s16 gData_02003014[];
extern s16 gData_0200301e[];
extern s16 gData_02003034[];
extern s16 gData_0200304e[];

void Func_02001a04(void);
void LemurianShip_FindMonstersAboard(void);
void Actor_StartScript(struct Actor *object, const void *script);
void Actor_PlaceBlocks(const void *script);
extern u32 gData_02003ef8[];

extern struct ActorPlacement gData_02003560[];
extern struct ActorPlacement gData_02003590[];

struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip1) {
    switch (gMainRoutineWork.entrance) {
    case 3:
    case 4:
    case 5:
      return gData_02003398;
    case 25:
    case 26:
    case 27:
    case 28:
      return gData_0200b410;
    case 15:
    case 16:
    case 17:
    case 18:
      return gData_02003560;
    case 29:
    case 30:
      return gData_02003590;
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
      return gData_02003710;
    case 6:
    case 10:
      return gData_020037d0;
    default:
      return gData_02003848;
    }
  }
  return gLemurianShip_Placements;
}

void Func_0200021c(void) {
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
  entry = (u8 *)gData_02003904;
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
  MapGrid_CopyCellsTo(0, 0, gData_02003904[found].x, gData_02003904[found].z, 1,
                      2);
  MapGrid_AnimateCells(gData_02003904[found].value, gData_02003904[found].x,
                       gData_02003904[found].z + 60);
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

void Func_02000324(void) {
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
  entry = (u8 *)gData_0200397c;
  i = 0;
  offset = 0;
next2:
  if (work->stepTrigger == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 4)
    goto next2;
  if (found < 2 && Event_GetFlag(0x93f) && Event_GetFlag(0x9e4) == 0)
    found += 4;
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gData_0200397c[found].x, gData_0200397c[found].z, 1,
                      2);
  MapGrid_AnimateCells(gData_0200397c[found].value, gData_0200397c[found].x,
                       gData_0200397c[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_CenterOnColumn(gMainRoutineWork.currentOwner, 2, -4);
  Event_Wait(4);
  Import_EventSetExitRequest(work->stepTrigger);
}

void Func_02000438(void) {
  if (Event_GetFlag(0x982)) {
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
    Actor_SetScript(party->currentOwner, gData_02002de8);
    wait(12);
    Event_PlaySound(SE_MAP_EXIT);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->stepTrigger);
  }
}

extern struct Trigger gData_02003c10[];
extern struct Trigger gData_02003d9c[];
extern struct Trigger gData_02003dfc[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip1) {
    switch (gMainRoutineWork.entrance) {
    case 3:
    case 4:
    case 5:
      return gData_02003a84;
    case 25:
    case 26:
    case 27:
    case 28:
      return gData_02003ae4;
    case 15:
    case 16:
    case 17:
    case 18:
      return gData_02003c10;
    case 29:
    case 30:
      return gData_02003c7c;
    default:
      return gData_020039d0;
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
      return gData_02003d9c;
    case 6:
    case 10:
      return gData_02003dfc;
    default:
      return gData_02003ebc;
    }
  }
  return gLemurianShip_Triggers;
}

/// A switch that toggles flag 0x207 and its map edits once flag 0x983 is set;
/// before that, it does nothing. "There's no reaction... It doesn't seem to be
/// running."
void LemurianShip_ExamineEngine(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(0x983)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_07)) {
      Event_PlaySound(SE_CRUNCH);
      FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(15)),
                                           px(824), px(-32), px(72));
      ((struct Actor *)Actor_GetById(15))->terrainHeight = 0xffe00000;
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      Event_ClearFlag(FLAG_SCENE_TEMP_07);
    } else if (Event_GetFlag(FLAG_SCENE_TEMP_12)) {
      Event_PlaySound(SE_ITEM_REMOVE);
    } else {
      Event_PlaySound(SE_CRUNCH);
      Actor_SetPosition(15, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
      Event_SetFlag(FLAG_SCENE_TEMP_07);
    }
  } else {
    FieldMessage_Show((s32)MsgLemurianShip02, 1);
  }
  Event_End();
}

void Func_02000770(void) { Actor_LiftTargetBlock(gData_02003010); }

void Func_02000780(void) { Actor_PlaceMovedBlock(); }

void Func_02000788(void) { Actor_LiftTargetBlock(gData_02003010); }

void Func_02000798(void) {
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
    Event_SetFlag(FLAG_SCENE_TEMP_00);
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
    Event_End();
  }
}

void Func_02000818(void) { FlagBlock_ApplyFlags(gData_02003014); }

void Func_02000828(void) { FlagBlock_ApplyFlags(gData_0200301e); }

void Func_02000838(void) {
  MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
  Event_SetFlag(FLAG_SCENE_TEMP_12);
}

void Func_02000860(void) { FlagBlock_ApplyFlags(gData_02003034); }

void Func_02000870(void) {
  struct Actor *leader;
  struct Actor *helper;
  s32 opened;
  u8 *state;

  if (!Event_GetFlag(0x983)) {
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 840, 72);
    opened = Event_GetFlag(0x932);
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
      Event_ClearFlag(FLAG_SCENE_TEMP_01);
    }
    Event_End();
  }
}

void Func_02000af4(void) {
  struct Actor *leader;

  if (!Event_GetFlag(0x983)) {
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

#define DEFINE_FUNC_02000BF0(name, n1)                                         \
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
    GameFlag_Set(id + n1);                                                     \
  }

DEFINE_FUNC_02000BF0(Func_02000bf0, 0x2f8)

DEFINE_FUNC_02000BF0(Func_02000c50, 0x2f9)

DEFINE_FUNC_02000BF0(Func_02000cb0, 0x2ff)

void Func_02000d10(s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  Actor_SetSpeed(id, 3.2);
  object->gravity = px(1) / 2;
  object->velocityY = px(8);
  Actor_SetDestination(id, 120, 120);
}

void Func_02000d44(void) {
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
  Func_02000d10(9);
  Func_02000d10(10);
  Func_02000d10(11);
  Func_02000d10(12);
  Func_02000d10(13);
  Actor_WaitForMoveById(13);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Event_Wait(20);
  Event_PlaySound(MUS_052);
  Actor_RunScriptAndWait(8, gData_02002e9c);

  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination((s32)&SceneId_LemurianShip2, 15);
  Field_RequestBattleFromEncounterTable(11, 1);
  Event_SetFlag(FLAG_AREA_TEMP_0F);
}

/// When actor 8 or 9 stands on its switch cell (column 35 or 29, row 9), lowers
/// it, edits the map and sets flag 0x202 or 0x203; once both are set, the last
/// map edit sets flag 0x982.
void Func_02000e2c(void) {
  struct Actor *object;
  struct Actor *leader;
  s32 column;
  s32 row;
  u32 i;
  s32 done;

  done = Event_GetFlag(FLAG_SCENE_TEMP_02);
  if (!done && Event_GetFlag(FLAG_SCENE_TEMP_04)) {
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
        Event_SetFlag(FLAG_SCENE_TEMP_02);
        if (object->z > leader->z) {
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  done = Event_GetFlag(FLAG_SCENE_TEMP_03);
  if (!done && Event_GetFlag(FLAG_SCENE_TEMP_05)) {
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
        Event_SetFlag(FLAG_SCENE_TEMP_03);
        if (object->z > leader->z) {
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_02) && Event_GetFlag(FLAG_SCENE_TEMP_03)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(SE_THUMP);
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_Wait(20);
    Event_SetFlag(0x982);
    Event_End();
  }
}

void Func_020010bc(void) { Actor_LiftTargetBlock(gData_0200304e); }

void Func_020010cc(void) {
  Actor_PlaceMovedBlock();
  Func_02000e2c();
}

#define DEFINE_FUNC_020010D8(name, n1, n2)                                     \
  void name(void) {                                                            \
    Sound_Play(SE_DRIP);                                                       \
    Actor_SetModeById(n1, 2);                                                  \
    GameFlag_Set(n2);                                                          \
    Func_02000e2c();                                                           \
  }

DEFINE_FUNC_020010D8(Func_020010d8, 8, 0x204)

DEFINE_FUNC_020010D8(Func_020010f8, 9, 0x205)

void Func_02001118(void) {
  FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(10)), 0);
}

void Func_02001128(void) {
  FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(11)), 0);
}

#define DEFINE_FUNC_02001138(name, n1, n2, n3, n4)                             \
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
    GameFlag_Clear(0x982);                                                     \
    Event_Wait(20);                                                            \
    Event_End();                                                               \
  }

DEFINE_FUNC_02001138(Func_02001138, 8, 0x202, 0x204, 35)

DEFINE_FUNC_02001138(Func_020011f4, 9, 0x203, 0x205, 29)

/// Places the leader and object 64 304 pixels back along z
/// (Actor_SetHorizontalPositionWithTerrain), lets the camera follow the leader,
/// starts a blend and sets flag 0x9e4.
void Func_020012b4(void) {
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
  Event_SetFlag(0x9e4);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  Event_End();
}

void Func_02001368(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_ResolvePendingActions(0);
  Event_End();
}

#define DEFINE_FUNC_02001380(name, n1)                                         \
  void name(s32 id) {                                                          \
    struct Actor *object = ((struct Actor *)Actor_GetById(id));                \
                                                                               \
    ((struct Actor *)Actor_GetById(id))->priorityFlags |= 0x20;                \
    Actor_SetSpritePriority(id, n1);                                           \
    object->priorityFlags &= ~2;                                               \
    object->riseCounter = 1;                                                   \
  }

DEFINE_FUNC_02001380(Func_02001380, 1)

// From patch 1; in the Japanese release actors 10 and 11 also use action 1.
#if TLA_PATCH_1
DEFINE_FUNC_02001380(Func_020013b8, 2)
#endif

void Func_020013f0(void) {
  ((struct Actor *)Actor_GetById(15))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(15, 3);
  ((struct Actor *)Actor_GetById(15))->priorityFlags &= ~2;
  Func_02001380(16);
  Func_02001380(17);
  Func_02001380(18);
  Func_02001380(19);
  Func_02001380(20);
}

void Func_0200143c(void) {
  Func_02001380(16);
  Func_02001380(17);
  Func_02001380(18);
  Func_02001380(19);
  Func_02001380(20);
  Func_02001380(22);
}

void Func_02001464(void) {
#if !TLA_PATCH_1
  Func_02001380(10);
  Func_02001380(11);
#else
  Func_020013b8(10);
  Func_020013b8(11);
#endif
}

void Func_02001474(void) {
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

void Func_0200155c(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;

  if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    object = ((struct Actor *)Actor_GetById(10));
    if ((object->x | object->z) == 0) {
      work->raisedTrigger = 200;
      Actor_SetAnimation(8, 1);
    }
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_03)) {
    object = ((struct Actor *)Actor_GetById(11));
    if ((object->x | object->z) == 0) {
      work->raisedTrigger = 201;
      Actor_SetAnimation(9, 1);
    }
  }
}

void Func_020015c8(void) {
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
    Actor_StartScript(object, gData_02003ef8);
  }
}

/// The scene that clears the ship and sets its flags.
/// "How did all these monsters get on board?"
void LemurianShip_FindMonstersAboard(void) {
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
  Actor_SetScript(6, gData_02002f44);
  Actor_SetScript(5, gData_02002f88);
  Actor_SetScript(8, gData_02002fcc);
  Actor_RunScriptAndWait(7, gData_02002f1c);
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
  Task_Add(Func_020015c8, 0x480);
  Event_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(20);
  Task_Remove(Func_020015c8);
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
  PartyInventory_Remove(242);
  Event_SetFlag(0x983);
  Event_SetFlag(0x930);
  Event_SetFlag(0x931);
  Event_SetFlag(0x933);
  Event_SetFlag(0x934);
  Event_SetFlag(0x935);
  Event_SetFlag(0x936);
  Event_SetFlag(0x937);
  Event_SetFlag(0x938);
  Event_SetFlag(0x939);
  Event_SetFlag(0x93a);
  Event_SetFlag(0x93b);
  Event_SetFlag(0x93c);
  Event_SetFlag(0x93e);
  Event_SetFlag(0x932);
  Event_SetFlag(0x93f);
  gMainRoutineWork.ownerOverride = 1;
  Import_EventSetExitRequest(90);
}

void Func_02001944(s32 actor, s32 id) {
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
  Actor_SetScript(actor, gData_02002e30);
  Actor_RunScriptAndWait(id, gData_02002e6c);
  Event_Wait(20);
  Event_End();
}

void Func_020019ac(void) {
  if (Event_GetFlag(0x932) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    Func_02001a04();
    Event_End();
  }
}

void Func_020019e0(void) {
  if (Event_GetFlag(0x932) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Func_02001a04();
    Event_End();
  }
}

void Func_02001a04(void) {
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
  Event_SetFlag(FLAG_SCENE_TEMP_01);
}

void Func_02001a9c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_SCATTER);
  Event_Wait(120);
  Event_End();
}

void Func_02001c88(void);

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x209;
  if (Event_GetFlag(0x9b4))
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineWork.scene == (s32)&SceneId_LemurianShip1) {
    switch (gMainRoutineWork.entrance) {
    case 3:
    case 4:
    case 5:
      Func_02001c88();
      break;
    case 6:
    case 7:
    case 8:
      Func_020027e4();
      break;
    case 25:
    case 26:
    case 27:
    case 28:
      Func_02001e0c();
      break;
    case 15:
    case 16:
    case 17:
    case 18:
      Func_0200241c();
      break;
    case 29:
    case 30:
      Func_02002498();
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
      Func_02002820();
      break;
    case 6:
    case 10:
      Func_02002970();
      break;
    case 9:
      Func_02002b98();
      break;
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  u32 i;

  i = 0;
next:
  if (Event_GetFlag(i + 0x300)) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->sequenceResult == 1) {
      Event_SetFlag(i + 0x930);
      Event_SetFlag(i + 0x220);
    }
  }
  Event_ClearFlag(i + 0x300);
  i++;
  if (i < 16)
    goto next;
  return 0;
}

/// Places actors 10 and 11 as flags 0x930 and 0x931 say, then runs the pending
/// step of flag 0x220 or 0x221.
void Func_02001c88(void) {
  s32 flag;

  if (Event_GetFlag(0x930)) {
    flag = Event_GetFlag(FLAG_SCENE_TEMP_20);
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
  if (Event_GetFlag(0x931)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_21))
      Actor_SetSpritePriority(9, 1);
    else
      Actor_SetPosition(9, 0, 0);
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(11)),
                                         px(104), px(-32), px(536));
    ((struct Actor *)Actor_GetById(11))->terrainHeight = 0xffe00000;
    Actor_SetSpritePriority(11, 1);
    wait(1);
  }
  FlagBlock_PlaceAll(gData_02003014);
  if (Event_GetFlag(FLAG_SCENE_TEMP_20)) {
    Func_02001944(8, 10);
    Event_ClearFlag(FLAG_SCENE_TEMP_20);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_21)) {
    Func_02001944(9, 11);
    Event_ClearFlag(FLAG_SCENE_TEMP_21);
  }
}

void Func_02001dc8(s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  ((struct Actor *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y = -px(32);
  object->terrainHeight = -px(32);
}

/// Places a platform object at its raised position.
#define RAISE(id, x, z)                                                        \
  {                                                                            \
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(id)),  \
                                         x, px(-32), z);                       \
    ((struct Actor *)Actor_GetById(id))->terrainHeight = -px(32);              \
  }

void Func_02001e0c(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gData_02003010);
  ((struct Actor *)Actor_GetById(8))->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  ((struct Actor *)Actor_GetById(9))->priorityFlags |= 32;
  if (Event_GetFlag(0x932)) {
    struct Actor *object = ((struct Actor *)Actor_GetById(15));

    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(15)),
                                         206 << 18, px(-32), 144 << 15);
    object->terrainHeight = -px(32);
    if (Event_GetFlag(FLAG_SCENE_TEMP_12))
      MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
    else
      MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_22))
      Actor_SetPosition(9, 0, 0);
  }
  if (Event_GetFlag(0x983)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_07)) {
      Actor_SetPosition(15, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
    } else {
      RAISE(15, 206 << 18, 144 << 15)
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      if (Event_GetFlag(FLAG_SCENE_TEMP_12))
        MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
      else
        MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    }
  }
  if (Event_GetFlag(0x933)) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 46, 5);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 45, 5);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 5);
    RAISE(16, 186 << 18, 176 << 15)
    if (!Event_GetFlag(FLAG_SCENE_TEMP_23))
      Actor_SetPosition(10, 0, 0);
  } else {
    Func_02001dc8(10);
  }
  if (Event_GetFlag(0x934)) {
    struct Actor *block = ((struct Actor *)Actor_GetById(8));

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 47, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 46, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 10);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 8);
    RAISE(17, 190 << 18, 152 << 16)
    if (block->x >> 20 == 47 && block->z >> 20 == 8)
      Actor_PlaceBlocks(gData_02003010);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_24))
      Actor_SetPosition(11, 0, 0);
  } else {
    Func_02001dc8(11);
  }
  if (Event_GetFlag(0x935)) {
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
      Actor_PlaceBlocks(gData_02003010);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_25))
      Actor_SetPosition(12, 0, 0);
  } else {
    Func_02001dc8(12);
  }
  if (Event_GetFlag(0x936)) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 7);
    RAISE(19, 206 << 18, 240 << 15)
    if (!Event_GetFlag(FLAG_SCENE_TEMP_26))
      Actor_SetPosition(13, 0, 0);
  } else {
    Func_02001dc8(13);
  }
  if (Event_GetFlag(0x937)) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 51, 8);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 52, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 51, 10);
    RAISE(20, 206 << 18, 152 << 16)
    if (!Event_GetFlag(FLAG_SCENE_TEMP_27))
      Actor_SetPosition(14, 0, 0);
  } else {
    Func_02001dc8(14);
  }
  FlagBlock_PlaceAll(gData_0200301e);
  Task_Add(Func_020013f0, 0x480);
  if (Event_GetFlag(FLAG_SCENE_TEMP_22)) {
    Func_02001944(9, 15);
    Event_ClearFlag(FLAG_SCENE_TEMP_22);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_23)) {
    Func_02001944(10, 16);
    Event_ClearFlag(FLAG_SCENE_TEMP_23);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_24)) {
    Func_02001944(11, 17);
    Event_ClearFlag(FLAG_SCENE_TEMP_24);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_25)) {
    Func_02001944(12, 18);
    Event_ClearFlag(FLAG_SCENE_TEMP_25);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_26)) {
    Func_02001944(13, 19);
    Event_ClearFlag(FLAG_SCENE_TEMP_26);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_27)) {
    Func_02001944(14, 20);
    Event_ClearFlag(FLAG_SCENE_TEMP_27);
  }
  if (gMainRoutineWork.entrance == 26 &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Func_020019ac();
}

void Func_020023f8(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner));

  Actor_SetSpritePriority(8, ((struct FieldSprite *)leader->sprite)->priority);
}

void Func_0200241c(void) {
  if (Event_GetFlag(8))
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
  Actor_PlaceBlocks(gData_02003010);
  LEMURIAN_SHIP_TASK_ADD(Func_020023f8, 0x480);
}

void Func_0200245c(s32 id) {
  struct Actor *object = ((struct Actor *)Actor_GetById(id));

  ((struct Actor *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y = 0;
  object->terrainHeight = 0;
}

/// Restores six of the scene's map edits and actors 16 to 22 from flags 0x938
/// to 0x93e, then runs the pending step of one of flags 0x228 to 0x22e.
void Func_02002498(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gData_02003010);
  ((struct Actor *)Actor_GetById(8))->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  if (Event_GetFlag(0x938)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 32);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(16)),
                                    px(776), 0, px(520));
    ((struct Actor *)Actor_GetById(16))->terrainHeight = 0;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_28))
      Actor_SetPosition(9, 0, 0);
  } else {
    Func_0200245c(9);
  }
  if (Event_GetFlag(0x939)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 53, 32);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(17)),
                                    px(856), 0, px(520));
    ((struct Actor *)Actor_GetById(17))->terrainHeight = 0;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_29))
      Actor_SetPosition(10, 0, 0);
  } else {
    Func_0200245c(10);
  }
  if (Event_GetFlag(0x93a)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 43, 37);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(18)),
                                    px(696), 0, px(600));
    ((struct Actor *)Actor_GetById(18))->terrainHeight = 0;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_2A))
      Actor_SetPosition(11, 0, 0);
  } else {
    Func_0200245c(11);
  }
  if (Event_GetFlag(0x93b)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 37);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(19)),
                                    px(776), 0, px(600));
    ((struct Actor *)Actor_GetById(19))->terrainHeight = 0;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_2B))
      Actor_SetPosition(12, 0, 0);
  } else {
    Func_0200245c(12);
  }
  if (Event_GetFlag(0x93c)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 50, 37);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(20)),
                                    px(808), 0, px(600));
    ((struct Actor *)Actor_GetById(20))->terrainHeight = 0;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_2C))
      Actor_SetPosition(13, 0, 0);
  } else {
    Func_0200245c(13);
  }
  if (Event_GetFlag(0x93e)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 43);
    Actor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(22)),
                                    px(776), 0, px(696));
    ((struct Actor *)Actor_GetById(22))->terrainHeight = 0;
    if (!Event_GetFlag(FLAG_SCENE_TEMP_2E))
      Actor_SetPosition(15, 0, 0);
  } else {
    Func_0200245c(15);
  }
  FlagBlock_PlaceAll(gData_02003034);
  FieldTask_AddFrame(Func_0200143c, 0x480);
  if (Event_GetFlag(FLAG_SCENE_TEMP_28)) {
    Func_02001944(9, 16);
    Event_ClearFlag(FLAG_SCENE_TEMP_28);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_29)) {
    Func_02001944(10, 17);
    Event_ClearFlag(FLAG_SCENE_TEMP_29);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_2A)) {
    Func_02001944(11, 18);
    Event_ClearFlag(FLAG_SCENE_TEMP_2A);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_2B)) {
    Func_02001944(12, 19);
    Event_ClearFlag(FLAG_SCENE_TEMP_2B);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_2C)) {
    Func_02001944(13, 20);
    Event_ClearFlag(FLAG_SCENE_TEMP_2C);
  } else if (Event_GetFlag(FLAG_SCENE_TEMP_2E)) {
    Func_02001944(15, 22);
    Event_ClearFlag(FLAG_SCENE_TEMP_2E);
  }
}

void Func_020027e4(void) {
  if (Event_GetFlag(0x983)) {
    MapGrid_CopyCellsTo(17, 125, 27, 63, 3, 3);
    MapGrid_CopyCellAttributes(25, 0, 5, 3, 25, 4);
  }
}

/// Sets up the scene by flags 0x93f, 0x9e4 and 0x22f: actor 64, actors 8 to 13
/// and the camera.
void Func_02002820(void) {
  struct Actor *object;

  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  LEMURIAN_SHIP_TASK_ADD(Func_02001474, 0x480);
  if (Event_GetFlag(0x93f)) {
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
  if (Event_GetFlag(0x9e4)) {
    struct Actor *marker = ((struct Actor *)Actor_GetById(64));

    Actor_SetPositionFixed(64, marker->x, marker->z + 0xfed00000);
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(64)), 0);
    MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
    MapGrid_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Map_CenterOnFollowedPoint();
    wait(1);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_2F)) {
    Func_02001a9c();
    Event_ClearFlag(FLAG_SCENE_TEMP_2F);
  }
}

/// Restores the scene from its flags: two map edits, actors 8 to 11 and two
/// scheduled tasks.
void Func_02002970(void) {
  struct Actor *object;

  ((struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Actor_PlaceBlocks(gData_0200304e);
  if (Event_GetFlag(0x982)) {
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_SetFlag(FLAG_SCENE_TEMP_02);
    Event_SetFlag(FLAG_SCENE_TEMP_03);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(8)), 0);
  if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    object = ((struct Actor *)Actor_GetById(8));
    MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 35, 9);
    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(object, px(568), px(-16), px(152));
    Actor_SetSpritePriority(8, 3);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Actor_SetPosition(10, 568, 152);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_04)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_02))
      Actor_SetAnimation(8, 3);
    else
      Actor_SetAnimation(8, 2);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(9)), 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Event_SetFlag(FLAG_SCENE_TEMP_03);
  if (Event_GetFlag(FLAG_SCENE_TEMP_03)) {
    object = ((struct Actor *)Actor_GetById(9));
    MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 29, 9);
    object->motionFlags = 0;
    object->y = 0xfff00000;
    Actor_SetSpritePriority(9, 3);
    Event_SetFlag(FLAG_SCENE_TEMP_05);
    Actor_SetPosition(11, 472, 152);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_05)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_03))
      Actor_SetAnimation(9, 3);
    else
      Actor_SetAnimation(9, 2);
  }
  wait(1);
  FlagBlock_PlaceAll(gData_02003014);
  FieldTask_AddFrame(Func_02001464, 0x480);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_10))
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(10)), 15);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_11))
    FieldActor_SetSpriteColor(((struct Actor *)Actor_GetById(11)), 15);
  LEMURIAN_SHIP_TASK_ADD(Func_0200155c, 0x480);
}

void Func_02002b98(void) {
  if (Event_GetFlag(0x983) == 0)
    LemurianShip_FindMonstersAboard();
}

#include "text/msg_ids.h"

// clang-format off
s32 gData_02002de8[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -1024),                                               
      SC_ADD(SW_SCALE_Y, -1024),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

s32 gData_02002e30[] = {
    23, 9, -512, 23, 10, -1024,
    0, 1, 13, 60, 0, 3,
    0, 1, 17,
};

u32 gData_02002e6c[] = {
    SC_ADD(SW_SCALE_X, 0x400),
    SC_ADD(SW_SCALE_Y, 0x400),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_END,
};

s32 gData_02002e9c[] = {
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

u32 gData_02002f1c[] =                                                                 
      {                                                                        
          SC_ARG(2), SC_MOVE(px(808), 0, px(168)), SC_SYNC, SC_ARG(1),   
          SC_END,                                                              
  };

u32 gData_02002f44[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(px(784), 0, px(196)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gData_02002f88[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(px(832), 0, px(196)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

u32 gData_02002fcc[] = {                                                               
      SC_ARG(2),   SC_MOVE(px(792), 0, px(216)),                         
      SC_SYNC,     SC_ARG(1),                                                  
      SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),              
      SC_WAIT(10), SC_END,                                                     
  };

s16 gData_02003010[] = {
    8, -1,
};

s16 gData_02003014[] = {
    10, 528, 11, 529, -1,
};

s16 gData_0200301e[] = {
    16, 531, 17, 532, 18, 533, 19, 534,
    20, 535, -1,
};

s16 gData_02003034[] = {
    16, 536, 17, 537, 18, 538, 19, 539,
    20, 540, 22, 542, -1,
};

s16 gData_0200304e[] = {
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

s16 gData_0200312c[] = {
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

u32 gData_02003234[] = {
    0xffec01d2, 0x1da0244, 0x24cfff4, 0x13ffff, 0xffff, 0,
    0, 0,
};

u32 gData_02003254[] = {
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
    SCENE_EXIT_UNLESS(8, LemurianShip2, 1, 0x93f),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 11, 0x9e4),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemurianShip2, 2, 0x93f),
    SCENE_EXIT_UNLESS(9, LemurianShip2, 12, 0x9e4),
    SCENE_EXIT_UNLESS(9, LemurianShip2, 17, EXIT_FLAG_NONE),
    SCENE_EXIT(10, LemurianShip1, 25),
    SCENE_EXIT(11, LemurianShip1, 26),
    SCENE_EXIT(12, LemurianShip1, 29),
    SCENE_EXIT(13, LemurianShip1, 5),
    SCENE_EXIT_UNLESS(14, LemurianShip2, 3, 0x93f),
    SCENE_EXIT_UNLESS(14, LemurianShip2, 13, 0x9e4),
    SCENE_EXIT_UNLESS(14, LemurianShip2, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, LemurianShip2, 4, 0x93f),
    SCENE_EXIT_UNLESS(15, LemurianShip2, 14, 0x9e4),
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
    SCENE_EXIT_UNLESS(8, LemurianShip2, 9, 0x983),
    SCENE_EXIT_UNLESS(8, LemurianShip2, 19, EXIT_FLAG_NONE),
    SCENE_EXIT(9, LemurianShip2, 8),
    SCENE_EXIT(10, LemurianShip1, 20),
    SCENE_EXIT(90, WorldMap, 69),
    SCENE_EXITS_END,
};

struct ActorPlacement gLemurianShip_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003398[] = {                                             
      {248, CONDITION_ALWAYS, 1, px(104), 0, px(424), FACING_SOUTH, 0,   
       0},                                                                     
      {248, CONDITION_ALWAYS, 1, px(104), 0, px(536), FACING_SOUTH, 0,   
       0},                                                                     
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},               
      {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},               
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gData_0200b410[] = {
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

struct ActorPlacement gData_02003560[] = {
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003590[] = {
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

struct ActorPlacement gData_02003710[] = {
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(248, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(7, 0x0, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020037d0[] = {                                             
      {314, CONDITION_ALWAYS, 1, px(584), 0, px(168), FACING_SOUTH, 2,   
       0},                                                                     
      {314, CONDITION_ALWAYS, 1, px(472), 0, px(152), FACING_SOUTH, 2,   
       0},                                                                     
      {290, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},               
      {290, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},               
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gData_02003848[] = {                                             
      {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {SCENE_TABLE_END},                                                       
  };

s16 gData_020038c0[] = {
    0, 126, 1, 2, 2, 1, 126, 1,
    2, 4, 2, 126, 1, 2, 4, -1,
};

s16 gData_020038e0[] = {
    10, 126, 1, 2, 4, 11, 126, 1,
    2, 4, -1, 4, 126, 2, 2, 2,
    -1, 0,
};

struct LemurianShipWarp gData_02003904[] = {
    {2, (s32)gData_020038e0, 4, 3},
    {14, (s32)gData_020038c0, 35, 11},
    {15, (s32)gData_020038c0, 6, 45},
    {16, (s32)gData_020038c0, 11, 45},
    {17, (s32)gData_020038c0, 21, 45},
    {18, (s32)gData_020038c0, 26, 46},
    {20, (s32)gData_020038c0, 29, 28},
    {25, (s32)gData_020038c0, 48, 2},
    {26, (s32)gData_020038c0, 54, 2},
    {29, (s32)gData_020038c0, 41, 29},
};

struct LemurianShipWarp gData_0200397c[] = {
    {1, (s32)gData_020038c0, 4, 2},
    {2, (s32)gData_020038c0, 10, 2},
    {6, (s32)gData_020038c0, 31, 3},
    {8, (s32)gData_020038c0, 41, 51},
    {11, (s32)gData_020038c0, 4, 21},
    {12, (s32)gData_020038c0, 10, 21},
};

struct Trigger gLemurianShip_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020039d0[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShip03))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShip04))},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003a84[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000bf0)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000bf0)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x210, EVENT(Func_02000818)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x211, EVENT(Func_02000818)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003ae4[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000c50)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02000c50)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_02000c50)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_02000c50)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_02000c50)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(Func_02000c50)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 15, 0x212, EVENT(Func_02000838)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x213, EVENT(Func_02000828)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, 0x214, EVENT(Func_02000828)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, 0x215, EVENT(Func_02000828)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, 0x216, EVENT(Func_02000828)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x217, EVENT(Func_02000828)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(LemurianShip_ExamineEngine)},
    {TRIGGER_STEP, 41, 0x212, EVENT(Func_02000870)},
    {TRIGGER_STEP, 43, 0x212, EVENT(Func_02000af4)},
    {TRIGGER_STEP, 42, 0x201, EVENT(Func_020019e0)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c10[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02000788)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(Func_02000798)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02000788)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_02000798)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c7c[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TALK_TRIGGER_FACING(FACING_EAST), 9, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_WEST), 10, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_EAST), 15, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_WEST), 15, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x218, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, 0x219, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, 0x21a, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, 0x21b, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x21c, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 22, 0x21e, EVENT(Func_02000860)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003d9c[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Func_02000324)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(Func_02000324)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000d44)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_FIRST), 255, CONDITION_ALWAYS, EVENT(Func_02001368)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 255, CONDITION_ALWAYS, EVENT(Func_020012b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003dfc[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x202, EVENT(Func_020010bc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x202, EVENT(Func_020010cc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x203, EVENT(Func_020010bc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x203, EVENT(Func_020010cc)},
    {TRIGGER_PUSH, 0, 0x982, EVENT(Func_020010bc)},
    {TRIGGER_PUSHED, 0, 0x982, EVENT(Func_020010cc)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0x204, EVENT(Func_020010d8)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x205, EVENT(Func_020010f8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), 10, 0x210, EVENT(Func_02001118)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), 11, 0x211, EVENT(Func_02001128)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, 0x210, EVENT(Func_02000818)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x211, EVENT(Func_02000818)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02001138)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(Func_020011f4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003ebc[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};

u32 gData_02003ef8[] = {
    0, 10, 1, 3, 0, 1,
    38,
};
// clang-format on
#undef RAISE
