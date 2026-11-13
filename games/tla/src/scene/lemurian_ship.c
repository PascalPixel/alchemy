/// @file
/// The scene code of overlay 0x656, in ROM order.

#include "shared/tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "module/scene/lemurian_ship.h"
#include "module/scene/scene.h"

/* VIEW(scene_tables): variable gData_0200312c as extern u8 gData_0200312c[];
 * the definition has s16 gData_0200312c[] = { 0, - 1, 72, 0, 72, 0x4000, 0, -
 * 1, - 1, - 1, - 1, 0, 1, - 1, 72, 0, 72, 0x4000, 0, 8, 16, 248, 256, 0, 2, -
 * 1, 168,...; tried as the definition's extern u32 gData_0200312c[], it does
 * not compile in tla-ja: games/tla/src/scene/lemurian_ship.c:1671: conflicting
 * types for `gData_0200312c'. */
extern u8 gData_0200312c__scene_tables[] __asm__("gData_0200312c");

u8 *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov656Room1)
    return gLemurianShip_Room1Entrances;
  if (scene == (s32)&SceneId_Ov656Room2)
    return gData_0200312c__scene_tables;
  return gLemurianShip_Entrances;
}

u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov656Room1)
    return gData_02003234;
  if (scene == (s32)&SceneId_Ov656Room2)
    return gData_02003254;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

// object the overlay links.

/* VIEW(s00b8): routine Actor_SetSpriteFlags as void
 * Actor_SetSpriteFlags(struct LemurianShipObject *object, u32 value);
 * field.h has void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
 * tried as the 35 declarations' void Actor_SetSpriteFlags(struct
 * FieldEffect *effect, s32 flags), it does not compile in tla-ja:
 * games/tla/src/scene/lemurian_ship/lemurian_ship.h:72: conflicting types for
 * `Actor_SetSpriteFlags'. */
struct LemurianShipObject;
extern void
Actor_SetSingleChildFlags__s00b8(struct LemurianShipObject *object,
                                 u32 value) __asm__("Actor_SetSpriteFlags");
#define Actor_SetSpriteFlags Actor_SetSingleChildFlags__s00b8
/* VIEW(s00b8): routine Actor_SetBlendMode as void
 * Actor_SetBlendMode(struct LemurianShipObject *object, s32 flags);
 * field.h has void Actor_SetBlendMode(struct Actor *object, u32 value);
 * tried as the definition's void Actor_SetBlendMode(struct Actor *object,
 * u32 value), which the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/lemurian_ship/lemurian_ship.h:73: conflicting types for
 * `Actor_SetBlendMode'. */
struct LemurianShipObject;
extern void
Animation_SetStateFlags__s00b8(struct LemurianShipObject *object,
                               s32 flags) __asm__("Actor_SetBlendMode");
#define Actor_SetBlendMode Animation_SetStateFlags__s00b8
#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"

struct LemurianShipObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[2];
  u8 field22;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct LemurianShipEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[2];
  u16 field16a;
  u8 unk16c[0x40];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

void Event_Start(void);
void Event_End(void);
void Func_02002dc0(const void *script);
void Actor_SelectAnimation(struct LemurianShipObject *object, s32 mode);
void Actor_SetSpriteFlags(struct LemurianShipObject *object, u32 value);
void Actor_SetBlendMode(struct LemurianShipObject *object, s32 flags);
void FieldPsynergy_RunHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02002db8(const void *script);
void Event_StartScreenTransition(void);

void Actor_MoveToObject(s32 actor, s32 target);

#define LEMURIAN_SHIP_TASK_ADD(func, priority) Task_Add((func), (priority))

void Map_CenterOnFollowedPoint(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Func_02002dd0(const void *script);
void Func_02002dd8(void);

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
void Actor_StartScript(struct LemurianShipObject *object, const void *script);
void Func_02002dc8(const void *script);
extern u32 gData_02003ef8[];
#undef Actor_SetBlendMode
#undef Actor_SetSpriteFlags
#include "event_inline.h"
#include "version.h"
#include "macros.h"

extern struct ActorPlacement gData_02003560[];
extern struct ActorPlacement gData_02003590[];

struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov656Room1) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov656Room2) {
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
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object;
  u8 *entry;
  s32 offset;
  u32 found;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  found = 0;
  i = 8;
next:
  object = ((struct LemurianShipObject *)Actor_GetById(i));
  if (object != NULL)
    object->motionFlags = 0;
  i++;
  if (i < 64)
    goto next;
  entry = (u8 *)gData_02003904;
  i = 0;
  offset = 0;
next2:
  if (work->exit == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 10)
    goto next2;
  Event_PlaySound(158);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gData_02003904[found].x, gData_02003904[found].z, 1,
                      2);
  MapGrid_AnimateCells(gData_02003904[found].value, gData_02003904[found].x,
                       gData_02003904[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (found == 12)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  Event_Wait(4);
  Import_EventSetExitRequest(work->exit);
}

void Func_02000324(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object;
  u8 *entry;
  s32 offset;
  u32 found;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  found = 0;
  i = 8;
next:
  object = ((struct LemurianShipObject *)Actor_GetById(i));
  if (object != NULL)
    object->motionFlags = 0;
  i++;
  if (i < 64)
    goto next;
  entry = (u8 *)gData_0200397c;
  i = 0;
  offset = 0;
next2:
  if (work->exit == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 4)
    goto next2;
  if (found < 2 && Event_GetFlag(0x93f) && Event_GetFlag(0x9e4) == 0)
    found += 4;
  Event_PlaySound(158);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gData_0200397c[found].x, gData_0200397c[found].z, 1,
                      2);
  MapGrid_AnimateCells(gData_0200397c[found].value, gData_0200397c[found].x,
                       gData_0200397c[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_CenterOnColumn(gMainRoutineWork.currentOwner, 2, -4);
  Event_Wait(4);
  Import_EventSetExitRequest(work->exit);
}

void Func_02000438(void) {
  if (Event_GetFlag(0x982)) {
    struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
    struct MainRoutineWork *party = &gMainRoutineWork;
    struct LemurianShipObject *leader;
    s32 x;
    s32 cell;

    leader = ((struct LemurianShipObject *)Actor_GetById(party->currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Actor_SetAnimation(party->currentOwner, 2);
    Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
    x = leader->x;
    cell = x >> 16;
    Actor_SetMoveTarget(leader, ((8 - cell % 16) << 16) + x, leader->y,
                        leader->z);
    FieldActor_WaitForMove(leader);
    leader->facing = 0xc000;
    Actor_SetScript(party->currentOwner, gData_02002de8);
    wait(12);
    Event_PlaySound(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->exit);
  }
}

extern struct Trigger gData_02003c10[];
extern struct Trigger gData_02003d9c[];
extern struct Trigger gData_02003dfc[];

struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov656Room1) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov656Room2) {
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
    if (Event_GetFlag(0x207)) {
      Event_PlaySound(232);
      FieldActor_SetPositionAndResetMotion(
          ((struct LemurianShipObject *)Actor_GetById(15)), PIXELS(824),
          PIXELS(-32), PIXELS(72));
      ((struct LemurianShipObject *)Actor_GetById(15))->height = 0xffe00000;
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      Event_ClearFlag(0x207);
    } else if (Event_GetFlag(0x212)) {
      Event_PlaySound(117);
    } else {
      Event_PlaySound(232);
      Actor_SetPosition(15, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
      Event_SetFlag(0x207);
    }
  } else {
    FieldMessage_Show((s32)MsgLemurianShip02, 1);
  }
  Event_End();
}

void Func_02000770(void) { Func_02002dd0(gData_02003010); }

void Func_02000780(void) { Func_02002dd8(); }

void Func_02000788(void) { Func_02002dd0(gData_02003010); }

void Func_02000798(void) {
  struct LemurianShipObject *object;
  s32 x;
  u8 *flags;
  s32 moving;

  Func_02002dd8();
  object = ((struct LemurianShipObject *)Actor_GetById(8));
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
    Event_PlaySound(188);
    wait(10);
    *flags = moving;
    Event_SetFlag(0x200);
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
    Event_End();
  }
}

void Func_02000818(void) { Func_02002dc0(gData_02003014); }

void Func_02000828(void) { Func_02002dc0(gData_0200301e); }

void Func_02000838(void) {
  MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
  Event_SetFlag(0x212);
}

void Func_02000860(void) { Func_02002dc0(gData_02003034); }

void Func_02000870(void) {
  struct LemurianShipObject *leader;
  struct LemurianShipObject *helper;
  s32 opened;
  u8 *state;

  if (!Event_GetFlag(0x983)) {
    leader = ((struct LemurianShipObject *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 840, 72);
    opened = Event_GetFlag(0x932);
    state = &leader->field22;
    if (opened) {
      Event_PlaySound(243);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      MapGrid_SetShake(0, 0x20000, 0x10000);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      *state = 2;
      Event_Wait(20);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 840, 88);
      Event_Wait(20);
      Event_PlaySound(232);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      *state = 1;
    } else {
      helper = ((struct LemurianShipObject *)Actor_GetById(9));
      Actor_SetSpritePriority(9, 2);
      Actor_SetSpeed(9, 0x26666, 0x13333);
      ((struct LemurianShipObject *)Actor_GetById(9))->unk5a[0] &= ~1;
      Actor_WalkToAndWait(9, 824, 88);
      Event_Wait(1);
      {
        /* FAKEMATCH(temporary): the bit in a local the byte is or'ed into; as
         * unk5a[0] |= 1, 47 of 260 instructions differ in tla-en. */
        u8 *bits = &((struct LemurianShipObject *)Actor_GetById(9))->unk5a[0];
        s32 value = 1;

        value |= *bits;
        *bits = value;
      }
      Event_PlaySound(243);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      MapGrid_SetShake(0, 0x20000, 0x10000);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      {
        s32 two = 2;
        *state = two;
        Event_Wait(20);
        Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x6000, 20);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
        Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 840, 88);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 9);
        helper->field48 = 0x10000;
        helper->field22 = two;
      }
      Actor_Jump(9, 4, 0);
      Actor_MoveToAndWait(9, 832, 74);
      Actor_Jump(9, 4, 0);
      Actor_MoveToAndWait(9, 856, 88);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Event_Wait(20);
      Event_PlaySound(232);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      Actor_Stop(gMainRoutineWork.currentOwner);
      *state = 1;
      helper->field22 = 0;
      wait(1);
      Event_ClearFlag(0x201);
    }
    Event_End();
  }
}

void Func_02000af4(void) {
  struct LemurianShipObject *leader;

  if (!Event_GetFlag(0x983)) {
    leader = ((struct LemurianShipObject *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 812, 72);
    Event_PlaySound(243);
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
    MapGrid_SetShake(0, 0x20000, 0x10000);
    Event_Wait(20);
    MapGrid_SetShake(0, 0, 0);
    MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
    leader->field22 = 2;
    Event_Wait(20);
    Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x33333, 0x19999);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 812, 88);
    Event_Wait(20);
    Event_PlaySound(232);
    MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
    Event_Wait(20);
    leader->field22 = 1;
  }
}

#define DEFINE_FUNC_02000BF0(name, n1)                                         \
  void name(s32 id) {                                                          \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Actor_FaceActorById(id, gMainRoutineWork.currentOwner, 0);                 \
    Actor_StartRepeatedMotion(id, 2);                                          \
    Actor_SetAttachedEffect(id, 0x102);                                        \
    Event_Wait(20);                                                            \
    gMainRoutineWork.battleRule24b = 2;                                        \
    Field_RequestBattleFromEncounterTable(11, 0);                              \
    GameFlag_SetBit(id + n1);                                                  \
  }

DEFINE_FUNC_02000BF0(Func_02000bf0, 0x2f8)

DEFINE_FUNC_02000BF0(Func_02000c50, 0x2f9)

DEFINE_FUNC_02000BF0(Func_02000cb0, 0x2ff)

void Func_02000d10(s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  Actor_SetSpeed(id, 0x33333, 0x19999);
  object->field48 = 0x8000;
  object->velocityY = 0x80000;
  Actor_SetDestination(id, 120, 120);
}

void Func_02000d44(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(0);
  Actor_StartRepeatedMotion(9, 2);
  Actor_StartRepeatedMotion(10, 2);
  Actor_StartRepeatedMotion(11, 2);
  Actor_StartRepeatedMotion(12, 2);
  Actor_RunRepeatedMotion(13, 2);
  Event_Wait(20);
  Event_PlaySound(147);
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
  Event_PlaySound(52);
  Actor_RunScriptAndWait(8, gData_02002e9c);

  gMainRoutineWork.battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_Ov656Room2, 15);
  Field_RequestBattleFromEncounterTable(11, 1);
  Event_SetFlag(0x30f);
}

/// When actor 8 or 9 stands on its switch cell (column 35 or 29, row 9), lowers
/// it, edits the map and sets flag 0x202 or 0x203; once both are set, the last
/// map edit sets flag 0x982.
void Func_02000e2c(void) {
  struct LemurianShipObject *object;
  struct LemurianShipObject *leader;
  s32 column;
  s32 row;
  u32 i;
  s32 done;

  done = Event_GetFlag(0x202);
  if (!done && Event_GetFlag(0x204)) {
    object = ((struct LemurianShipObject *)Actor_GetById(8));
    column = object->x >> 20;
    if (column == 35) {
      row = object->z >> 20;
      if (row == 9) {
        leader = ((struct LemurianShipObject *)Actor_GetById(
            gMainRoutineWork.currentOwner));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(183);
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
        Actor_SetPosition(10, PIXELS(568), PIXELS(152));
        Event_Wait(20);
        Event_SetFlag(0x202);
        if (object->z > leader->z) {
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
          ((struct LemurianShipObject *)Actor_GetById(
               gMainRoutineWork.currentOwner))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  done = Event_GetFlag(0x203);
  if (!done && Event_GetFlag(0x205)) {
    object = ((struct LemurianShipObject *)Actor_GetById(9));
    column = object->x >> 20;
    if (column == 29) {
      row = object->z >> 20;
      if (row == 9) {
        leader = ((struct LemurianShipObject *)Actor_GetById(
            gMainRoutineWork.currentOwner));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(183);
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
        Actor_SetPosition(11, PIXELS(472), PIXELS(152));
        Event_Wait(20);
        Event_SetFlag(0x203);
        if (object->z > leader->z) {
          Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
          ((struct LemurianShipObject *)Actor_GetById(
               gMainRoutineWork.currentOwner))
              ->priorityFlags |= 1;
        }
        Event_End();
      }
    }
  }
  if (Event_GetFlag(0x202) && Event_GetFlag(0x203)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(188);
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_Wait(20);
    Event_SetFlag(0x982);
    Event_End();
  }
}

void Func_020010bc(void) { Func_02002dd0(gData_0200304e); }

void Func_020010cc(void) {
  Func_02002dd8();
  Func_02000e2c();
}

#define DEFINE_FUNC_020010D8(name, n1, n2)                                     \
  void name(void) {                                                            \
    Sound_Play(106);                                                           \
    Actor_SetModeById(n1, 2);                                                  \
    GameFlag_SetBit(n2);                                                       \
    Func_02000e2c();                                                           \
  }

DEFINE_FUNC_020010D8(Func_020010d8, 8, 0x204)

DEFINE_FUNC_020010D8(Func_020010f8, 9, 0x205)

void Func_02001118(void) {
  FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(10)),
                            0);
}

void Func_02001128(void) {
  FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(11)),
                            0);
}

#define DEFINE_FUNC_02001138(name, n1, n2, n3, n4)                             \
  void name(void) {                                                            \
    struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;             \
    struct LemurianShipObject *object =                                        \
        ((struct LemurianShipObject *)Actor_GetById(n1));                      \
    u32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    work->field16a = 0;                                                        \
    GameFlag_ClearBit(n2);                                                     \
    GameFlag_ClearBit(n3);                                                     \
    Event_Wait(20);                                                            \
    Sound_Play(186);                                                           \
    for (i = 0; i <= 63; i++) {                                                \
      object->y += 0x4000;                                                     \
      wait(1);                                                                 \
    }                                                                          \
    Actor_SetSpritePriority(n1, 2);                                            \
    MapGrid_CopyCellPictureIndicesRect(4, 123, n4, 69, 1, 1);                  \
    MapGrid_CopyCellAttributes(36, 0, 1, 1, n4, 9);                            \
    Sound_Play(188);                                                           \
    MapGrid_CopyCellsTo(7, 124, 31, 64, 3, 2);                                 \
    GameFlag_ClearBit(0x982);                                                  \
    Event_Wait(20);                                                            \
    Event_End();                                                               \
  }

DEFINE_FUNC_02001138(Func_02001138, 8, 0x202, 0x204, 35)

DEFINE_FUNC_02001138(Func_020011f4, 9, 0x203, 0x205, 29)

/// Places the leader and object 64 304 pixels back along z
/// (Actor_SetHorizontalPositionWithTerrain), lets the camera follow the leader,
/// starts a blend and sets flag 0x9e4.
void Func_020012b4(void) {
  struct LemurianShipObject *leader = ((
      struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct LemurianShipObject *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Field_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
  Actor_SetPosition(gMainRoutineWork.currentOwner, leader->x,
                    leader->z + 0xfed00000);
  object = ((struct LemurianShipObject *)Actor_GetById(64));
  Actor_SetPosition(64, object->x, object->z + 0xfed00000);
  FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(64)),
                            0);
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
    struct LemurianShipObject *object =                                        \
        ((struct LemurianShipObject *)Actor_GetById(id));                      \
                                                                               \
    ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;   \
    Actor_SetSpritePriority(id, n1);                                           \
    object->priorityFlags &= ~2;                                               \
    object->timer = 1;                                                         \
  }

DEFINE_FUNC_02001380(Func_02001380, 1)

// From patch 1; in the Japanese release actors 10 and 11 also use action 1.
#if TLA_PATCH_1
DEFINE_FUNC_02001380(Func_020013b8, 2)
#endif

void Func_020013f0(void) {
  ((struct LemurianShipObject *)Actor_GetById(15))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(15, 3);
  ((struct LemurianShipObject *)Actor_GetById(15))->priorityFlags &= ~2;
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
  struct LemurianShipObject *leader = ((
      struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));
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
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object;

  if (Event_GetFlag(0x202)) {
    object = ((struct LemurianShipObject *)Actor_GetById(10));
    if ((object->x | object->z) == 0) {
      work->field16a = 200;
      Actor_SetAnimation(8, 1);
    }
  }
  if (Event_GetFlag(0x203)) {
    object = ((struct LemurianShipObject *)Actor_GetById(11));
    if ((object->x | object->z) == 0) {
      work->field16a = 201;
      Actor_SetAnimation(9, 1);
    }
  }
}

void Func_020015c8(void) {
  s32 position[3];
  struct LemurianShipObject *object;

  SET_VECTOR(position, 0x3280000, 0x80000, 0x8c0000);
  Vector_AddPolarOffsetFar(0x200000, Random16Far(), position);
  object = ((struct LemurianShipObject *)Import_ActorCreate(
      0x2a2, position[0], position[1], position[2]));
  if (object != NULL) {
    object->motionFlags = 0;
    *(s32 *)((u8 *)object + 0x30) = 0x20000;
    *(s32 *)((u8 *)object + 0x34) = 0x8000;
    Actor_SetSingleChildFlags__s00b8(object, 0);
    Actor_SelectAnimation(object, 0);
    Actor_SetMoveTarget(object, PIXELS(808), PIXELS(8), PIXELS(140));
    Actor_StartScript(object, gData_02003ef8);
  }
}

/// The scene that clears the ship and sets its flags.
/// "How did all these monsters get on board?"
void LemurianShip_FindMonstersAboard(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemurianShip01);
  Event_PlaySound(78);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 808, 208);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(176), 1);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
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
  Event_PlaySound(220);
  Task_Add(Func_020015c8, 0x480);
  Event_Wait(60);
  Event_PlaySound(390);
  Event_Wait(20);
  Task_RemoveFar(Func_020015c8);
  Event_PlaySound(251);
  Field_SetCameraBounds(0x2b00000, 0, 0x3a00000, 0x2200000);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(448), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(808), PIXELS(480));
  Actor_SetPosition(7, PIXELS(808), PIXELS(420));
  Actor_SetPosition(6, PIXELS(784), PIXELS(468));
  Actor_SetPosition(5, PIXELS(832), PIXELS(468));
  Actor_SetPosition(8, PIXELS(792), PIXELS(488));
  Map_CenterOnFollowedPoint();
  Event_Wait(20);
  Event_PlaySound(246);
  Event_Wait(80);
  ((struct LemurianShipEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      513;
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
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  Event_Start();
  Event_ResolvePendingActions(0);
  object->scaleX = 0x1000;
  object->scaleY = 0x1000;
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Event_PlaySound(103);
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
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(9));

  FieldActor_ShowEmote(9, 0x100, 0);
  Actor_RunRepeatedMotion(9, 2);
  object->field48 = 0x8000;
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_Jump(9, 6, 0);
  Actor_MoveToAndWait(9, 832, 88);
  Event_Wait(20);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  Actor_SetSpritePriority(9, 3);
  Actor_MoveToAndWait(9, 824, 72);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 40);
  Event_SetFlag(0x201);
}

void Func_02001a9c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(164);
  Event_Wait(120);
  Event_End();
}

void Func_02001c88(void);

s32 Scene_Initialize(void) {
  ((struct LemurianShipEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  if (Event_GetFlag(0x9b4))
    Event_SetFlag(0x144);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov656Room1) {
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
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov656Room2) {
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
    flag = Event_GetFlag(0x220);
    if (flag != 0) {
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      Actor_SetPositionAndResetMotion(
          ((struct LemurianShipObject *)Actor_GetById(10)), PIXELS(104), 0,
          PIXELS(424));
      ((struct LemurianShipObject *)Actor_GetById(10))->height = 0;
    } else {
      Actor_SetPosition(8, 0, 0);
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      FieldActor_SetPositionAndResetMotion(
          ((struct LemurianShipObject *)Actor_GetById(10)), PIXELS(104), 0,
          PIXELS(424));
      ((struct LemurianShipObject *)Actor_GetById(10))->height = 0;
    }
    wait(1);
  }
  if (Event_GetFlag(0x931)) {
    if (Event_GetFlag(0x221))
      Actor_SetSpritePriority(9, 1);
    else
      Actor_SetPosition(9, 0, 0);
    FieldActor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(11)), PIXELS(104),
        PIXELS(-32), PIXELS(536));
    ((struct LemurianShipObject *)Actor_GetById(11))->height = 0xffe00000;
    Actor_SetSpritePriority(11, 1);
    wait(1);
  }
  Func_02002db8(gData_02003014);
  if (Event_GetFlag(0x220)) {
    Func_02001944(8, 10);
    Event_ClearFlag(0x220);
  } else if (Event_GetFlag(0x221)) {
    Func_02001944(9, 11);
    Event_ClearFlag(0x221);
  }
}

void Func_02001dc8(s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y = -0x200000;
  object->height = -0x200000;
}

/// Places a platform object at its raised position.
#define RAISE(id, x, z)                                                        \
  {                                                                            \
    FieldActor_SetPositionAndResetMotion(                                      \
        ((struct LemurianShipObject *)Actor_GetById(id)), x, PIXELS(-32), z);  \
    ((struct LemurianShipObject *)Actor_GetById(id))->height = -0x200000;      \
  }

void Func_02001e0c(void) {
  ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Func_02002dc8(gData_02003010);
  ((struct LemurianShipObject *)Actor_GetById(8))->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  ((struct LemurianShipObject *)Actor_GetById(9))->priorityFlags |= 32;
  if (Event_GetFlag(0x932)) {
    struct LemurianShipObject *object =
        ((struct LemurianShipObject *)Actor_GetById(15));

    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(15)), 206 << 18,
        PIXELS(-32), 144 << 15);
    object->height = -0x200000;
    if (Event_GetFlag(0x212))
      MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
    else
      MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    if (!Event_GetFlag(0x222))
      Actor_SetPosition(9, 0, 0);
  }
  if (Event_GetFlag(0x983)) {
    if (Event_GetFlag(0x207)) {
      Actor_SetPosition(15, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
    } else {
      RAISE(15, 206 << 18, 144 << 15)
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      if (Event_GetFlag(0x212))
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
    if (!Event_GetFlag(0x223))
      Actor_SetPosition(10, 0, 0);
  } else {
    Func_02001dc8(10);
  }
  if (Event_GetFlag(0x934)) {
    struct LemurianShipObject *block =
        ((struct LemurianShipObject *)Actor_GetById(8));

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 47, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 46, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 10);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 8);
    RAISE(17, 190 << 18, 152 << 16)
    if (block->x >> 20 == 47 && block->z >> 20 == 8)
      Func_02002dc8(gData_02003010);
    if (!Event_GetFlag(0x224))
      Actor_SetPosition(11, 0, 0);
  } else {
    Func_02001dc8(11);
  }
  if (Event_GetFlag(0x935)) {
    struct LemurianShipObject *block =
        ((struct LemurianShipObject *)Actor_GetById(8));
    s32 x = block->x >> 20;
    s32 z = block->z >> 20;

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 49, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 6);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 8);
    RAISE(18, 198 << 18, 240 << 15)
    if ((x == 48 && z == 7) || (x == 49 && z == 8))
      Func_02002dc8(gData_02003010);
    if (!Event_GetFlag(0x225))
      Actor_SetPosition(12, 0, 0);
  } else {
    Func_02001dc8(12);
  }
  if (Event_GetFlag(0x936)) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 7);
    RAISE(19, 206 << 18, 240 << 15)
    if (!Event_GetFlag(0x226))
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
    if (!Event_GetFlag(0x227))
      Actor_SetPosition(14, 0, 0);
  } else {
    Func_02001dc8(14);
  }
  Func_02002db8(gData_0200301e);
  Task_Add(Func_020013f0, 0x480);
  if (Event_GetFlag(0x222)) {
    Func_02001944(9, 15);
    Event_ClearFlag(0x222);
  } else if (Event_GetFlag(0x223)) {
    Func_02001944(10, 16);
    Event_ClearFlag(0x223);
  } else if (Event_GetFlag(0x224)) {
    Func_02001944(11, 17);
    Event_ClearFlag(0x224);
  } else if (Event_GetFlag(0x225)) {
    Func_02001944(12, 18);
    Event_ClearFlag(0x225);
  } else if (Event_GetFlag(0x226)) {
    Func_02001944(13, 19);
    Event_ClearFlag(0x226);
  } else if (Event_GetFlag(0x227)) {
    Func_02001944(14, 20);
    Event_ClearFlag(0x227);
  }
  if (gMainRoutineWork.entrance == 26 && !Event_GetFlag(0x109))
    Func_020019ac();
}

void Func_020023f8(void) {
  struct LemurianShipObject *leader = ((
      struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));

  Actor_SetSpritePriority(8, leader->sprite->priority);
}

void Func_0200241c(void) {
  if (Event_GetFlag(8))
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
  Func_02002dc8(gData_02003010);
  LEMURIAN_SHIP_TASK_ADD(Func_020023f8, 0x480);
}

void Func_0200245c(s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;
  Actor_SetSpritePriority(id, 1);
  object->motionFlags = 0;
  object->y = 0;
  object->height = 0;
}

/// Restores six of the scene's map edits and actors 16 to 22 from flags 0x938
/// to 0x93e, then runs the pending step of one of flags 0x228 to 0x22e.
void Func_02002498(void) {
  ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Func_02002dc8(gData_02003010);
  ((struct LemurianShipObject *)Actor_GetById(8))->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  if (Event_GetFlag(0x938)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 32);
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(16)), PIXELS(776), 0,
        PIXELS(520));
    ((struct LemurianShipObject *)Actor_GetById(16))->height = 0;
    if (!Event_GetFlag(0x228))
      Actor_SetPosition(9, 0, 0);
  } else {
    Func_0200245c(9);
  }
  if (Event_GetFlag(0x939)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 53, 32);
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(17)), PIXELS(856), 0,
        PIXELS(520));
    ((struct LemurianShipObject *)Actor_GetById(17))->height = 0;
    if (!Event_GetFlag(0x229))
      Actor_SetPosition(10, 0, 0);
  } else {
    Func_0200245c(10);
  }
  if (Event_GetFlag(0x93a)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 43, 37);
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(18)), PIXELS(696), 0,
        PIXELS(600));
    ((struct LemurianShipObject *)Actor_GetById(18))->height = 0;
    if (!Event_GetFlag(0x22a))
      Actor_SetPosition(11, 0, 0);
  } else {
    Func_0200245c(11);
  }
  if (Event_GetFlag(0x93b)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 37);
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(19)), PIXELS(776), 0,
        PIXELS(600));
    ((struct LemurianShipObject *)Actor_GetById(19))->height = 0;
    if (!Event_GetFlag(0x22b))
      Actor_SetPosition(12, 0, 0);
  } else {
    Func_0200245c(12);
  }
  if (Event_GetFlag(0x93c)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 50, 37);
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(20)), PIXELS(808), 0,
        PIXELS(600));
    ((struct LemurianShipObject *)Actor_GetById(20))->height = 0;
    if (!Event_GetFlag(0x22c))
      Actor_SetPosition(13, 0, 0);
  } else {
    Func_0200245c(13);
  }
  if (Event_GetFlag(0x93e)) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 43);
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(22)), PIXELS(776), 0,
        PIXELS(696));
    ((struct LemurianShipObject *)Actor_GetById(22))->height = 0;
    if (!Event_GetFlag(0x22e))
      Actor_SetPosition(15, 0, 0);
  } else {
    Func_0200245c(15);
  }
  Func_02002db8(gData_02003034);
  FieldTask_AddFrame(Func_0200143c, 0x480);
  if (Event_GetFlag(0x228)) {
    Func_02001944(9, 16);
    Event_ClearFlag(0x228);
  } else if (Event_GetFlag(0x229)) {
    Func_02001944(10, 17);
    Event_ClearFlag(0x229);
  } else if (Event_GetFlag(0x22a)) {
    Func_02001944(11, 18);
    Event_ClearFlag(0x22a);
  } else if (Event_GetFlag(0x22b)) {
    Func_02001944(12, 19);
    Event_ClearFlag(0x22b);
  } else if (Event_GetFlag(0x22c)) {
    Func_02001944(13, 20);
    Event_ClearFlag(0x22c);
  } else if (Event_GetFlag(0x22e)) {
    Func_02001944(15, 22);
    Event_ClearFlag(0x22e);
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
  struct LemurianShipObject *object;

  ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  LEMURIAN_SHIP_TASK_ADD(Func_02001474, 0x480);
  if (Event_GetFlag(0x93f)) {
    object = ((struct LemurianShipObject *)Actor_GetById(64));
    object->motionFlags = 0;
    Actor_SetPositionAndResetMotion(
        ((struct LemurianShipObject *)Actor_GetById(64)), PIXELS(136),
        PIXELS(-32), PIXELS(392));
    object->height = 0xfff00000;
    Actor_SetSpritePriority(64, 3);
    FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(64)),
                              1);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    wait(1);
  }
  if (Event_GetFlag(0x9e4)) {
    struct LemurianShipObject *marker =
        ((struct LemurianShipObject *)Actor_GetById(64));

    Actor_SetPosition(64, marker->x, marker->z + 0xfed00000);
    FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(64)),
                              0);
    MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
    MapGrid_SetCameraBounds(0x80000, 0x100000, 0xf80000, 0x1000000);
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Map_CenterOnFollowedPoint();
    wait(1);
  }
  if (Event_GetFlag(0x22f)) {
    Func_02001a9c();
    Event_ClearFlag(0x22f);
  }
}

/// Restores the scene from its flags: two map edits, actors 8 to 11 and two
/// scheduled tasks.
void Func_02002970(void) {
  struct LemurianShipObject *object;

  ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner))
      ->priorityFlags |= 32;
  Func_02002dc8(gData_0200304e);
  if (Event_GetFlag(0x982)) {
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_SetFlag(0x202);
    Event_SetFlag(0x203);
  }
  Actor_SetSingleChildFlags__s00b8(
      ((struct LemurianShipObject *)Actor_GetById(8)), 0);
  if (Event_GetFlag(0x202)) {
    object = ((struct LemurianShipObject *)Actor_GetById(8));
    MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 35, 9);
    object->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(object, PIXELS(568), PIXELS(-16),
                                         PIXELS(152));
    Actor_SetSpritePriority(8, 3);
    Event_SetFlag(0x204);
    Actor_SetPosition(10, PIXELS(568), PIXELS(152));
  }
  if (Event_GetFlag(0x204)) {
    if (Event_GetFlag(0x202))
      Actor_SetAnimation(8, 3);
    else
      Actor_SetAnimation(8, 2);
  }
  Actor_SetSingleChildFlags__s00b8(
      ((struct LemurianShipObject *)Actor_GetById(9)), 0);
  if (!Event_GetFlag(0x109))
    Event_SetFlag(0x203);
  if (Event_GetFlag(0x203)) {
    object = ((struct LemurianShipObject *)Actor_GetById(9));
    MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 29, 9);
    object->motionFlags = 0;
    object->y = 0xfff00000;
    Actor_SetSpritePriority(9, 3);
    Event_SetFlag(0x205);
    Actor_SetPosition(11, PIXELS(472), PIXELS(152));
  }
  if (Event_GetFlag(0x205)) {
    if (Event_GetFlag(0x203))
      Actor_SetAnimation(9, 3);
    else
      Actor_SetAnimation(9, 2);
  }
  wait(1);
  Func_02002db8(gData_02003014);
  FieldTask_AddFrame(Func_02001464, 0x480);
  if (!Event_GetFlag(0x210))
    FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(10)),
                              15);
  if (!Event_GetFlag(0x211))
    FieldActor_SetSpriteColor(((struct LemurianShipObject *)Actor_GetById(11)),
                              15);
  LEMURIAN_SHIP_TASK_ADD(Func_0200155c, 0x480);
}

void Func_02002b98(void) {
  if (Event_GetFlag(0x983) == 0)
    LemurianShip_FindMonstersAboard();
}

#include "text/msg_ids.h"
#include "objcmd.h"

// clang-format off
DEFINE_G_DATA_0200121C(gData_02002de8)

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

DEFINE_G_DATA_02002F1C(gData_02002f1c, 808, 168)

DEFINE_G_DATA_02002F44(gData_02002f44, 784, 196)

DEFINE_G_DATA_02002F88(gData_02002f88, 832, 196, 10)

DEFINE_G_DATA_02002FCC(gData_02002fcc, 792, 216, 10)

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
    SCENE_EXITS(40),
    SCENE_EXIT_IF(1, 43, 1, 0x907),
    SCENE_EXIT_IF(1, 200, 3, 0x9b4),
    SCENE_EXIT_IF(1, 234, 4, -1),
    SCENE_EXIT(2, 40, 3),
    SCENE_EXIT(3, 40, 2),
    SCENE_EXIT(4, 40, 6),
    SCENE_EXIT(5, 40, 13),
    SCENE_EXIT(6, 40, 4),
    SCENE_EXIT(7, 40, 19),
    SCENE_EXIT_IF(8, 41, 1, 0x93f),
    SCENE_EXIT_IF(8, 41, 11, 0x9e4),
    SCENE_EXIT_IF(8, 41, 16, -1),
    SCENE_EXIT_IF(9, 41, 2, 0x93f),
    SCENE_EXIT_IF(9, 41, 12, 0x9e4),
    SCENE_EXIT_IF(9, 41, 17, -1),
    SCENE_EXIT(10, 40, 25),
    SCENE_EXIT(11, 40, 26),
    SCENE_EXIT(12, 40, 29),
    SCENE_EXIT(13, 40, 5),
    SCENE_EXIT_IF(14, 41, 3, 0x93f),
    SCENE_EXIT_IF(14, 41, 13, 0x9e4),
    SCENE_EXIT_IF(14, 41, 18, -1),
    SCENE_EXIT_IF(15, 41, 4, 0x93f),
    SCENE_EXIT_IF(15, 41, 14, 0x9e4),
    SCENE_EXIT_IF(15, 41, 20, -1),
    SCENE_EXIT(16, 40, 27),
    SCENE_EXIT(17, 40, 28),
    SCENE_EXIT(18, 40, 30),
    SCENE_EXIT(19, 41, 10),
    SCENE_EXIT(20, 40, 7),
    SCENE_EXIT(25, 40, 10),
    SCENE_EXIT(26, 40, 11),
    SCENE_EXIT(27, 40, 16),
    SCENE_EXIT(28, 40, 17),
    SCENE_EXIT(29, 40, 12),
    SCENE_EXIT(30, 40, 18),
    SCENE_EXITS(41),
    SCENE_EXIT(1, 40, 8),
    SCENE_EXIT(2, 40, 9),
    SCENE_EXIT(3, 40, 14),
    SCENE_EXIT(4, 40, 15),
    SCENE_EXIT(6, 41, 7),
    SCENE_EXIT(7, 41, 6),
    SCENE_EXIT_IF(8, 41, 9, 0x983),
    SCENE_EXIT_IF(8, 41, 19, -1),
    SCENE_EXIT(9, 41, 8),
    SCENE_EXIT(10, 40, 20),
    SCENE_EXIT(90, 2, 69),
    SCENE_EXITS_END,
};

struct ActorPlacement gLemurianShip_Placements[] = {
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02003398(gData_02003398, 248, 104, 424, 0, 104, 536, 1)

struct ActorPlacement gData_0200b410[] = {
    {287, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(120), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(856), 0, PIXELS(88), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(88), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(152), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(120), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(120), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(152), FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003560[] = {
    {287, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(744), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003590[] = {
    {287, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(552), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(520), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(856), 0, PIXELS(520), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(600), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(600), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(600), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(696), FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003710[] = {
    {248, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(120), FACING_SOUTH, 0, 0},
    {248, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(88), FACING_SOUTH, 0, 1},
    {248, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(120), FACING_SOUTH, 0, 1},
    {248, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(152), FACING_SOUTH, 0, 1},
    {248, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(88), FACING_SOUTH, 0, 1},
    {248, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(152), FACING_SOUTH, 0, 1},
    {7, 0x0, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02003398(gData_020037d0, 314, 584, 168, 2, 472, 152, 7)

DEFINE_G_DATA_02003690(gData_02003848, 7, 0, 0, 6, 5, 0, 56, 0)

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
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {0xc602, 14, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {0xc602, 20, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {0xf3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShip03))},
    {0x1c3, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShip04))},
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
    {0xc602, 25, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {0xc602, 26, CONDITION_ALWAYS, EVENT(Func_0200021c)},
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
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(LemurianShip_ExamineEngine)},
    {TRIGGER_STEP, 41, 0x212, EVENT(Func_02000870)},
    {TRIGGER_STEP, 43, 0x212, EVENT(Func_02000af4)},
    {TRIGGER_STEP, 42, 0x201, EVENT(Func_020019e0)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c10[] = {
    {0xc602, 15, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {0xc602, 16, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {0xc602, 17, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {0xc602, 18, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02000788)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(Func_02000798)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000788)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000798)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c7c[] = {
    {0xc602, 29, CONDITION_ALWAYS, EVENT(Func_0200021c)},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {0x400, 9, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0x4400, 9, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0x8400, 10, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0x4400, 10, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0xc400, 10, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0xc400, 13, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0x400, 15, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0x8400, 15, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {0xc400, 15, CONDITION_ALWAYS, EVENT(Func_02000cb0)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x218, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, 0x219, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, 0x21a, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, 0x21b, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x21c, EVENT(Func_02000860)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 22, 0x21e, EVENT(Func_02000860)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000770)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000780)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003d9c[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000324)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000324)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000d44)},
    {0x80008a05, 255, CONDITION_ALWAYS, EVENT(Func_02001368)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 255, CONDITION_ALWAYS, EVENT(Func_020012b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003dfc[] = {
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x202, EVENT(Func_020010bc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x202, EVENT(Func_020010cc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x203, EVENT(Func_020010bc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x203, EVENT(Func_020010cc)},
    {8, 0, 0x982, EVENT(Func_020010bc)},
    {9, 0, 0x982, EVENT(Func_020010cc)},
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
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_02000438)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};

u32 gData_02003ef8[] = {
    0, 10, 1, 3, 0, 1,
    38,
};
// clang-format on
#undef RAISE
