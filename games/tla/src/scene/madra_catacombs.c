/// @file
/// The scene code of overlay 0x65a, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "field_sprite.h"
#include "field.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/madra_catacombs.h"
#include "module/scene/scene.h"
#include "module/tla.h"

/* VIEW(madra_catacombs): routine Func_020010ec as s32 Func_020010ec(s32 layer, s32 x, s32 z); module/scene/aqua_rock_inside.h has void Func_020010ec(void). */
extern s32 Func_020010ec__madra_catacombs(s32 layer, s32 x, s32 z) __asm__("Func_020010ec");
s32 Func_020011d4(struct Actor *object);

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
void Func_02000038(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (((struct ListedActor *)list)->id != 0xffff) {
    struct Actor *object = Actor_GetById(*list);
    u8 *layer = &object->terrainId;
    s32 shape = Func_020010ec__madra_catacombs(*layer, object->x, object->z);
    s32 level = Map_GetTerrainHeight(*layer, object->x, object->z);
    s32 index;
    u8 *cells;
    u8 *shapes;
    u8 newShape;
    u8 *grid;

    level >>= 19;
    index = Func_020011d4(object);
    cells = (u8 *)map->layers[*layer].cells;
    grid = Ram_MapShapeGrid;
    shapes =
        grid + (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    level += 4;
    newShape = Func_020011f4(*layer, shape, level);
    shapes += index;
    index <<= 2;
    ((struct ShapeCell *)shapes)->shape = newShape;
    cells += index;
    ((struct MapCell *)cells)->attributeB |= 192;
    cells += 0x200;
    ((struct MapCell *)cells)->attributeB |= 64;
    list++;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

// A one-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell's first read (as a plain store, 4 of 131 instructions differ
// in tla-en). A struct is padded to 4 bytes here, so the pointer stays plain.

/// Lowers the cells under the held actor, or the one the leader carries, for
/// each listed actor it is: the shape grid takes the shape for its new height
/// and the cell flags under it are cleared; the actor is kept at 0xcbc of the
/// event work. Overlay 655's Func_02000d78 and overlay 676's copy.
void Func_0200014c(u16 *list) {
  struct Actor *held = Func_0200120c();
  void *work = Ram_HeapSlots->eventWork;
  u8 *grid;

  if (held == NULL) {
    s32 id = BattleEffect_FindNearestObject(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  grid = Ram_MapShapeGrid;
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      u8 *cell = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shape =
          grid + ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind = Func_020010ec__madra_catacombs(*layer, held->x, held->z);
      s32 height = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Func_020011d4(held);
      s32 result = Func_020011f4(*layer, kind, (height >> 19) - 4);

      shape += index;
      /* FAKEMATCH(temporary): index also holds its cell offset; as cell +=
       * index * 4, 35 of 131 instructions differ in tla-en. */
      index <<= 2;
      ((struct ShapeCell *)shape)->shape = result;
      cell += index;
      ((struct MapCell *)cell)->attributeB &= 63;
      cell += 0x200;
      ((struct MapCell *)cell)->attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}

/* VIEW(madra_catacombs): routine ObjectVisual_CopyAttributes as void ObjectVisual_CopyAttributes(s32 id, s32 value); module/field/object.h has void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId). */
extern void ObjectVisual_CopyAttributes__madra_catacombs(s32 id, s32 value) __asm__("ObjectVisual_CopyAttributes");
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02003c00(s32 a, s32 b, s32 c);
/* VIEW(madra_catacombs): routine Func_02003d80 as s32 Func_02003d80(void *sprite, s32 frame); module/scene/mars_lighthouse_room_f.h has void Func_02003d80(void). */
extern s32 Func_02003d80__madra_catacombs(void *sprite, s32 frame) __asm__("Func_02003d80");
/* VIEW(madra_catacombs): routine Effect_AdvanceMotionAtLeaderPriority as void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object); module/field/field_effect.h has void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object). */
extern void Effect_AdvanceMotionAtLeaderPriority__madra_catacombs(struct Actor *object) __asm__("Effect_AdvanceMotionAtLeaderPriority");

// One-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell read; a plain store keeps its order.

void Func_02000270(void) {
  struct Actor *held = HELD_OBJECT(Ram_HeapSlots->eventWork);
  u8 *layer;
  u8 *cells;
  u8 *shapes;
  s32 shape;
  s32 level;
  s32 index;
  u8 newShape;

  if (held == NULL)
    return;
  layer = &held->terrainId;
  cells = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
  shapes = Ram_MapShapeGrid +
           (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
  shape = Func_020010ec__madra_catacombs(*layer, held->x, held->z);
  level = Map_GetTerrainHeight(*layer, held->x, held->z);
  index = Func_020011d4(held);
  level >>= 19;
  level += 4;
  newShape = Func_020011f4(*layer, shape, level);
  shapes += index;
  index <<= 2;
  /* FAKEMATCH(signature): the grid byte stored through a one-field struct
   * view (struct ShapeCell), whose alias set lets sched2 sink it past the
   * cell read; as *shapes = newShape, 6 of 76 instructions differ in tla-en. */
  ((struct ShapeCell *)shapes)->shape = newShape;
  cells += index;
  ((struct MapCell *)cells)->attributeB |= 192;
  cells += 0x200;
  ((struct MapCell *)cells)->attributeB |= 64;
}

struct SceneWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

extern struct Trigger gData_02001914[];

void Func_02000fbc(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom4)
    return gData_02001400;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom5)
    return gData_02001430;
  return gData_020013d0;
}

const s16 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65aRoom2)
    return gData_020014a8;
  return 0;
}

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom1)
    return gData_020015b4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom3)
    return gData_0200165c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom5)
    return gData_020016bc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom6)
    return gData_020016ec;
  return gData_0200159c;
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom1)
    return gData_02001920;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom2)
    return gData_020019f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom3)
    return gData_02001a1c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom4)
    return gData_02001aac;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom5)
    return gData_02001b54;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom6)
    return gData_02001bcc;
  return gData_02001914;
}

extern u8 MsgFieldDoorWontBudge[];

void Func_020011e4(const void *data);
void Func_020011cc(const void *data);

void Func_02000474(void) {
  struct EventWork15a *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgFieldDoorWontBudge, 1);
  work->flag = 1;
  Event_End();
}

void Func_020004a4(struct Actor *object) {
  u8 *flags = &object->motionFlags;

  *flags = 3;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(188);
  wait(10);
  *flags = 0;
}

void Func_020004cc(void) {
  struct Actor *object = Actor_GetById(11);
  s32 z;

  if ((object->x >> 20) == 40) {
    z = object->z >> 20;
    if (z == 13) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Func_020004a4(object);
      Map_CopyCellAttributeRect(41, 13, 2, 1, 39, z);
      GameFlag_SetBit(0x8c8);
      Event_End();
    }
  }
}

void Func_0200051c(void) { Func_020011e4(gData_020013c0); }

void Func_0200052c(void) {
  struct Actor *object = Actor_GetById(12);

  Func_020011ec();
  if ((object->x >> 20) == 44 && (object->z >> 20) == 26)
    GameFlag_SetBit(0x8ca);
}

void Func_02000558(void) { Func_020011cc(gData_020013c4); }

/* VIEW(madra_catacombs): routine PartyInventory_Remove as void PartyInventory_Remove(s32 item); module/game/game.h has s32 PartyInventory_Remove(s32 item_id). */
extern void PartyInventory_Remove__madra_catacombs(s32 item) __asm__("PartyInventory_Remove");
void Func_02001174(void);
void Func_02000958(void);

void Func_02000568(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(183);
  for (i = 0; i < 120; i++) {
    left->x += -0x1999;
    /* FAKEMATCH(signature): read and written as long, another alias set than
     * the left door's int store, so the load need not wait for that store; as
     * int, 2 of 175 instructions differ in tla-en. */
    *(long *)&right->x += 0x1999;
    wait(1);
  }
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(160), -1, PIXELS(240), 1);
  Func_02001174();
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(160), -1, PIXELS(186), 1);
  Sound_Play(168);
  Actor_SetPosition(14, PIXELS(136), PIXELS(258));
  Actor_SetPosition(19, PIXELS(184), PIXELS(258));
  Actor_SetMovementTypeById(14, gData_02001264);
  Actor_SetMovementTypeById(19, gData_02001264);
  Event_Wait(40);
  Sound_Play(168);
  Actor_SetPosition(13, PIXELS(120), PIXELS(226));
  Actor_SetPosition(18, PIXELS(200), PIXELS(226));
  Actor_SetMovementTypeById(13, gData_02001264);
  Actor_SetMovementTypeById(18, gData_02001264);
  Event_Wait(40);
  Sound_Play(168);
  Actor_SetPosition(12, PIXELS(120), PIXELS(194));
  Actor_SetPosition(17, PIXELS(200), PIXELS(194));
  Actor_SetMovementTypeById(12, gData_02001264);
  Actor_SetMovementTypeById(17, gData_02001264);
  Event_Wait(60);
  Sound_Play(229);
  Actor_SetPosition(11, PIXELS(136), PIXELS(162));
  Actor_SetPosition(16, PIXELS(184), PIXELS(162));
  Actor_SetPosition(10, PIXELS(136), PIXELS(138));
  Actor_SetPosition(15, PIXELS(184), PIXELS(138));
  Actor_SetMovementTypeById(11, gData_020012f4);
  Actor_SetMovementTypeById(16, gData_020012f4);
  Actor_SetMovementTypeById(10, gData_020012f4);
  Actor_SetMovementTypeById(15, gData_020012f4);
  Event_Wait(80);
  Func_02000958();
  PartyInventory_Remove__madra_catacombs(459);
  GameFlag_SetBit(0x8c9);
  Event_End();
}

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 flag;
};

extern u8 MsgFieldEyesOfTruth[];

void Actor_MoveToObject(s32 id, s32 value);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
/* VIEW(madra_catacombs): routine Func_020011c4 as void Func_020011c4(const void *data); module/scene/mars_lighthouse_room_m.h has void Func_020011c4(void). */
extern void Func_020011c4__madra_catacombs(const void *data) __asm__("Func_020011c4");
/* VIEW(madra_catacombs): routine Func_020011dc as void Func_020011dc(const void *data); module/scene/trial_road.h has void Func_020011dc(void). */
extern void Func_020011dc__madra_catacombs(const void *data) __asm__("Func_020011dc");
void Func_020008d0(struct Actor *object);
void Func_02000cf4(void);

void Func_0200072c(void) {
  if (GameFlag_Test(0x8cb) != 0)
    MapGrid_CopyCellsTo(72, 63, 36, 38, 1, 2);
  else
    MapGrid_CopyCellsTo(71, 63, 36, 38, 1, 2);
  GameFlag_SetBit(0x203);
}

void Func_02000778(void) {
  MapGrid_CopyCellsTo(70, 63, 36, 38, 1, 2);
  GameFlag_ClearBit(0x203);
}

void Func_020007a0(void) {
  if (GameFlag_Test(0x203) != 0 && Event_GetFlag(0x8cb) == 0) {
    Event_Start();
    Event_ResolvePendingActions(1);
    Sound_Play(161);
    Map_CopyCellPictureIndicesRect(72, 63, 36, 38, 1, 2);
    Event_Wait(40);
    Sound_Play(188);
    Map_CopyCellPictureIndicesRect(71, 66, 97, 39, 2, 2);
    wait(2);
    Map_CopyCellPictureIndicesRect(73, 66, 97, 39, 2, 2);
    wait(2);
    Map_CopyCellPictureIndicesRect(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
    GameFlag_SetBit(0x8cb);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldMessage_Show((s32)&MsgFieldEyesOfTruth, 1);
    Event_End();
  }
}

void Func_02000874(void) {
  struct Actor *object;
  s32 x;

  if (Event_GetFlag(0x8cc) == 0) {
    object = Actor_GetById(64);
    x = object->x;
    if (x < (186 << 16)) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Sound_Play(204);
      object->motionFlags = 3;
      wait(10);
      Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
      Event_SetFlag(0x8cc);
      Event_End();
    }
  }
}

void Func_020008d0(struct Actor *object) {
  Func_02000270();
  if ((object->x >> 20) == 35 && (object->z >> 20) == 17)
    GameFlag_SetBit(0x9ef);
}

void Func_020008f4(void) {
  struct Actor *object;

  if (Func_020011fc() != 0) {
    object = Actor_GetById(Func_02001184(gMainRoutineWork.currentOwner));
    Func_0200014c(gData_020013ca);
    Func_02001204();
    Func_020008d0(object);
  }
}

void Func_0200092c(void) { Func_0200014c(gData_020013ca); }

/* VIEW(madra_catacombs): tag struct EventWorkCbc as struct EventWorkCbc__madra_catacombs; module/scene/scene.h has another definition. */
struct EventWorkCbc__madra_catacombs {
  u8 unk000[0xcbc];
  void *active;
};

void Func_02000b38(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_0200093c(void) {
  struct EventWorkCbc__madra_catacombs *work = Ram_HeapSlots->eventWork;

  if (work->active != 0)
    Func_020008d0(work->active);
}

/// Places actors 10 to 19 in two columns, then sends actors 20 to 29 to them
/// one each.
void Func_02000958(void) {
  u32 i;
  /* FAKEMATCH(pin): the actor pinned to r6, so the counter takes r5 as the
   * native has them (a do-while or a counter-derived actor measured 14 to
   * 29); as a plain local the two swap, 14 of 89 instructions differ in
   * tla-en. */
  register s32 actor __asm__("r6");
  s32 target;

  Actor_SetPosition(14, PIXELS(136), PIXELS(258));
  Actor_SetPosition(19, PIXELS(184), PIXELS(258));
  Actor_SetPosition(13, PIXELS(120), PIXELS(226));
  Actor_SetPosition(18, PIXELS(200), PIXELS(226));
  Actor_SetPosition(12, PIXELS(120), PIXELS(194));
  Actor_SetPosition(17, PIXELS(200), PIXELS(194));
  Actor_SetPosition(11, PIXELS(136), PIXELS(162));
  Actor_SetPosition(16, PIXELS(184), PIXELS(162));
  Actor_SetPosition(10, PIXELS(136), PIXELS(138));
  Actor_SetPosition(15, PIXELS(184), PIXELS(138));
  target = 10;
  actor = 20;
  for (i = 0; i < 10; i++) {
    Actor_SetModeById(actor, 0);
    Actor_MoveToObject(actor, target);
    target++;
    actor++;
  }
  MapGrid_CopyCellAttributes(0, 0, 2, 2, 9, 22);
  wait(1);
}

void Func_02000a28(void) {
  struct Actor *lever = Actor_GetById(8);
  struct Actor *rock = Actor_GetById(64);
  struct AnimationRecord18 *animation = rock->sprite;

  animation->frame = ((struct AnimationRecord18 *)lever->sprite)->frame;
  if (Event_GetFlag(0x204) != 0 && animation->frame == 0) {
    Sound_Play(0x217);
    rock->x += PIXELS(-2);
    rock->velocityY = PIXELS(2);
    GameFlag_ClearBit(0x204);
  }
  if (animation->frame != 0)
    GameFlag_SetBit(0x204);
}

void Func_02000a88(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 cell;

  object = Actor_GetById(party->currentOwner);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(party->currentOwner, 0x8000, 0x4000);
  cell = object->x >> 16;
  Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z);
  Script_WaitForEventTimeout(object);
  object->facing = FACING_NORTH;
  Actor_SetMovementTypeById(party->currentOwner, gData_0200121c);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->returnEntrance);
}

void Func_02000b38(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 cell;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Sound_Play(188);
  else
    Sound_Play(158);
  Map_CopyCellPictureIndicesRect(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                        object->z);
    Script_WaitForEventTimeout(object);
    object->facing = FACING_NORTH;
  }
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_0200121c);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->returnEntrance);
}

void Func_02000c24(void) { Func_02000b38(78, 29, 78, 31, 2, 2); }

void Func_02000c40(void) { Func_02000b38(109, 1, 109, 3, 1, 2); }

void Func_02000c5c(void) { Func_02000b38(118, 1, 118, 3, 1, 2); }

void Func_02000c78(void) { Func_02000b38(92, 29, 92, 31, 1, 2); }

void Func_02000c94(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Import_EventSetExitRequest(3);
}

void Func_02000cf4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Import_EventSetExitRequest(8);
}

s32 Scene_Initialize(void) {
  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom1)
    Func_02000dcc();
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom3)
    Func_02000f2c();
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom4)
    Func_02000fbc();
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom5)
    Func_02000fd8();
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom6)
    Func_0200102c();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000dcc(void) {
  s32 a;
  s32 b;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *object;

  GameFlag_ClearBit(0x203);
  Actor_SetSpritePriority(64, 3);
  Func_020011b4();
  b = 0x102;
  a = 1;
  /* FAKEMATCH(barrier): a and b held at an empty asm, so the first argument is
   * set before the second is finished; as no asm, 2 of 142 instructions
   * differ in tla-en. */
  __asm__("" : "+r"(a), "+r"(b));
  Func_020011bc(a, b + 255, 9, 10);
  Func_0200115c(11);
  if (GameFlag_Test(0x8c8) != 0) {
    MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, 13);
    object = Actor_GetById(11);
    x = 162;
    z = 216;
    x <<= 18;
    y = 0;
    /* FAKEMATCH(barrier): x, y and z held at an empty asm, the second of the
     * pair above, so y is cleared before z is shifted; as no asm, 2 of 142
     * instructions differ in tla-en. */
    __asm__("" : "+r"(x), "+r"(y), "+r"(z));
    Actor_SetPositionAndResetMotion(object, x, y, z << 16);
  }
  if (GameFlag_Test(0x8ca) != 0)
    Actor_SetPosition(12, PIXELS(712), PIXELS(424));
  Func_020011dc__madra_catacombs(gData_020013c0);
  if (GameFlag_Test(0x9ef) != 0)
    Actor_SetPosition(8, PIXELS(568), PIXELS(280));
  Actor_GetById(8)->priorityFlags &= 0xfe;
  Func_02000038(gData_020013ca);
  Func_020011c4__madra_catacombs(gData_020013c4);
  if (GameFlag_Test(0x8cb) != 0) {
    Map_CopyCellPictureIndicesRect(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
  }
  if (GameFlag_Test(0x8ab) != 0) {
    MapGrid_CopyCellsTo(82, 74, 116, 16, 5, 3);
    MapGrid_CopyCellAttributes(52, 11, 5, 1, 52, 18);
    if (gMainRoutineWork.entrance == 3)
      Func_02000c94();
    else
      wait(10);
  }
}

void Func_02000f2c(void) {
  Func_020011b4();
  Func_020011bc(0, 0x200, 8, 9);
  if (GameFlag_Test(0x8ab) != 0) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    MapGrid_CopyCellsTo(68, 70, 113, 11, 5, 3);
    MapGrid_CopyCellsTo(50, 74, 50, 75, 2, 1);
    MapGrid_CopyCellAttributes(49, 9, 5, 1, 49, 13);
    if (gMainRoutineWork.entrance == 8)
      Func_02000cf4();
    else
      wait(1);
  }
}

void Func_02000fbc(void) {
  if (GameFlag_Test(0x8cc) == 0)
    Actor_SetHorizontalPositionWithTerrain(65, 0, 0);
}

void Func_02000fd8(void) {
  struct Actor *object;

  if (GameFlag_Test(0x8cc) != 0) {
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  } else {
    object = Actor_GetById(64);
    object->collisionFlags = 8;
    object->motionFlags = 2;
    object->y = PIXELS(32);
    object->terrainHeight = PIXELS(32);
    object->update = Func_02000a28;
    Actor_GetById(8)->scaleY = 0xcccc;
  }
}

/// Mirrors actors 8 and 9 and puts them on the ground; once flag 0x8c9 is set,
/// each steps 12 pixels outward unless flag 0x109 is set too, and Func_02000958
/// runs.
void Func_0200102c(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);

  left->scaleX = 0x14000;
  right->scaleX = -0x14000;
  left->motionFlags = 0;
  right->motionFlags = 0;
  left->y = 0;
  right->y = 0;
  if (GameFlag_Test(0x8c9) != 0) {
    if (GameFlag_Test(0x109) == 0) {
      left->x += PIXELS(-12);
      right->x += 0xc0000;
    }
    Func_02000958();
  }
}

#include "text/msg_ids.h"
#include "objcmd.h"

// clang-format off
s32 gData_0200121c[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -0x8000),
    SC_ADD(SW_SCALE_X, -1024),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

u32 gData_02001264[] = {
    SC_SET(SW_SCALE_X, 0x1000),
    SC_SET(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s32 gData_020012f4[] = {
    SC_SET(SW_SCALE_X, 0x1000),
    SC_SET(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x8000),
    SC_ADD(SW_SCALE_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, 0x6000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(3)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s16 gData_020013c0[] = {
    12, -1,
};

s16 gData_020013c4[] = {
    13, 514, -1,
};

s16 gData_020013ca[] = {
    8, -1, 0,
};

struct SceneEntrance gData_020013d0[] = {
    {0, -1, 172, 0, 149, 0x4000, 0, -1, -1, -1, -1, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct SceneEntrance gData_02001400[] = {
    {3, -1, 512, 0, 224, 0xc000, 0, 392, 40, 632, 280, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct SceneEntrance gData_02001430[] = {
    {5, -1, 728, 0, 88, 0x4000, 0, 680, 32, 920, 192, 0},
    {6, -1, 872, 0, 88, 0x4000, 0, 680, 32, 920, 192, 0},
    {7, -1, 872, 0, 120, 0, 0, 680, 32, 920, 192, 0},
    {8, -1, 792, 0, 152, 0xc000, 0, 680, 32, 920, 192, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

s16 gData_020014a8[] = {
    112, 48, 144, 128, 64, 160, -1, 1,
    336, 48, 144, 352, 64, 160, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(47),
    SCENE_EXIT(1, 51, 8),
    SCENE_EXIT(2, 50, 7),
    SCENE_EXIT(3, 49, 1),
    SCENE_EXIT(4, 50, 13),
    SCENE_EXIT(5, 49, 6),
    SCENE_EXITS(48),
    SCENE_EXIT(1, 44, 15),
    SCENE_EXIT(2, 49, 7),
    SCENE_EXIT(3, 44, 16),
    SCENE_EXIT(4, 49, 4),
    SCENE_EXITS(49),
    SCENE_EXIT(1, 47, 3),
    SCENE_EXIT(2, 49, 3),
    SCENE_EXIT(3, 49, 2),
    SCENE_EXIT(4, 48, 4),
    SCENE_EXIT(5, 49, 8),
    SCENE_EXIT(6, 47, 5),
    SCENE_EXIT(7, 48, 2),
    SCENE_EXIT(8, 49, 5),
    SCENE_EXITS(50),
    SCENE_EXIT(1, 51, 1),
    SCENE_EXIT(2, 50, 11),
    SCENE_EXIT(3, 50, 9),
    SCENE_EXIT(4, 51, 2),
    SCENE_EXIT(5, 52, 4),
    SCENE_EXIT(6, 52, 1),
    SCENE_EXIT(7, 47, 2),
    SCENE_EXIT(8, 50, 12),
    SCENE_EXIT(9, 50, 3),
    SCENE_EXIT(10, 51, 7),
    SCENE_EXIT(11, 50, 2),
    SCENE_EXIT(12, 50, 8),
    SCENE_EXIT(13, 47, 4),
    SCENE_EXITS(51),
    SCENE_EXIT(1, 50, 1),
    SCENE_EXIT(2, 50, 4),
    SCENE_EXIT(3, 51, 5),
    SCENE_EXIT(4, 51, 6),
    SCENE_EXIT(5, 51, 3),
    SCENE_EXIT(6, 51, 4),
    SCENE_EXIT(7, 50, 10),
    SCENE_EXIT(8, 47, 1),
    SCENE_EXITS(52),
    SCENE_EXIT(1, 50, 6),
    SCENE_EXIT(2, 52, 3),
    SCENE_EXIT(3, 52, 2),
    SCENE_EXIT(4, 50, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_0200159c[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020015b4[] = {
    {250, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(360), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200165c[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(232), FACING_SOUTH, 2, 1},
    {71, CONDITION_ALWAYS, 1, PIXELS(794), 0, PIXELS(236), FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020016bc[] = {
    {679, CONDITION_ALWAYS, 1, PIXELS(202), 0, PIXELS(408), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020016ec[] = {
    {330, CONDITION_ALWAYS, 7, PIXELS(150), 0, PIXELS(366), FACING_SOUTH, 2, 0},
    {330, CONDITION_ALWAYS, 7, PIXELS(170), 0, PIXELS(366), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001914[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02001920[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 9, 0x201, 0},
    {9, 10, 0x8c8, EVENT(Func_020004cc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x8c8, EVENT(Func_020004cc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, 0x8ca, EVENT(Func_0200051c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x8ca, EVENT(Func_0200052c)},
    {0x602, 11, 0x9ef, EVENT(Func_020008f4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x9ef, EVENT(Func_0200092c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9ef, EVENT(Func_0200093c)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x202, EVENT(Func_02000558)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_0200072c)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {0xc403, 10, CONDITION_ALWAYS, EVENT(Func_020007a0)},
    {TRIGGERS_END},
};

struct Trigger gData_020019f8[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02001a1c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x200, 0},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldCaveFoundByChampa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFieldWhoDugCave)},
    {TRIGGERS_END},
};

struct Trigger gData_02001aac[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0xc602, 9, CONDITION_ALWAYS, EVENT(Func_02000c24)},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGERS_END},
};

struct Trigger gData_02001b54[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000c40)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02000c5c)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 8, CONDITION_ALWAYS, EVENT(Func_02000874)},
    {TRIGGERS_END},
};

struct Trigger gData_02001bcc[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000c78)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x1cb04, 10, CONDITION_ALWAYS, EVENT(Func_02000568)},
    {TRIGGER_EXAMINE, 10, 0x8c9, EVENT(Func_02000474)},
    {TRIGGERS_END},
};
