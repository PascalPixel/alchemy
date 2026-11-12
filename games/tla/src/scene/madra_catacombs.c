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

extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_020010ec(s32 layer, s32 x, s32 z);
s32 Func_020011d4(struct Actor *object);
s32 Func_020011f4(s32 layer, s32 shape, s32 height);

// One-field views: the terminator's read and the shape store are memory of their
// own to sched2, which sinks them past the cell stores; plain reads and stores
// keep their order. A struct is padded to 4 bytes here, so the pointers stay plain.
struct ListedActor {
  u16 id;
};

struct ShapeCell {
  u8 shape;
};

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
void Func_02000038(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (((struct ListedActor *)list)->id != 0xffff) {
    struct Actor *object = Actor_GetById(*list);
    u8 *layer = &object->terrainId;
    s32 shape = Func_020010ec(*layer, object->x, object->z);
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

s32 Func_0200118c(s32 owner, s32 mode);
struct Actor *Func_0200120c(void);

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
    s32 id = Func_0200118c(gMainRoutineWork.currentOwner, 1);

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
      s32 kind = Func_020010ec(*layer, held->x, held->z);
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

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

// One-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell read; a plain store keeps its order.

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

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
  shape = Func_020010ec(*layer, held->x, held->z);
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

extern char SceneId_Ov65aRoom1;
extern char SceneId_Ov65aRoom2;
extern char SceneId_Ov65aRoom3;
extern char SceneId_Ov65aRoom4;
extern char SceneId_Ov65aRoom5;
extern char SceneId_Ov65aRoom6;
extern const u8 gData_020013d0[];
extern const u8 gData_02001400[];
extern const u8 gData_02001430[];
extern const u8 gData_020014a8[];
extern const u8 gData_0200159c[];
extern const u8 gData_020015b4[];
extern const u8 gData_0200165c[];
extern const u8 gData_020016bc[];
extern const u8 gData_020016ec[];
extern const u8 gData_02001914[];
extern const u8 gData_02001920[];
extern const u8 gData_020019f8[];
extern const u8 gData_02001a1c[];
extern const u8 gData_02001aac[];
extern const u8 gData_02001b54[];
extern const u8 gData_02001bcc[];

void Func_02000dcc(void);
void Func_02000f2c(void);
void Func_02000fbc(void);
void Func_02000fd8(void);
void Func_0200102c(void);

const u8 *Func_02000318(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom4)
    return gData_02001400;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65aRoom5)
    return gData_02001430;
  return gData_020013d0;
}

const u8 *Func_02000358(void) {
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
#include "scene_tables.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Func_02000388(void) {
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

const u8 *Func_020003ec(void) {
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

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

extern u8 MsgFieldDoorWontBudge;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];

void Func_0200110c(void);
void Func_02001114(void);
void Func_020011e4(const void *data);
void Func_020011cc(const void *data);
void Func_020011ec(void);

void Func_02000474(void) {
  struct EventWork15a *work = Ram_HeapSlots->eventWork;

  Func_0200110c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgFieldDoorWontBudge, 1);
  work->flag = 1;
  Func_02001114();
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
      Func_0200110c();
      Event_ResolvePendingActions(0);
      Func_020004a4(object);
      Map_CopyCellAttributeRect(41, 13, 2, 1, 39, z);
      GameFlag_SetBit(0x8c8);
      Func_02001114();
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

extern const u8 gData_02001264[];
extern const u8 gData_020012f4[];

void PartyInventory_Remove(s32 item);
void Func_02001174(void);
void Func_02000958(void);

void Func_02000568(void) {
  struct Actor *left = Actor_GetById(8);
  struct Actor *right = Actor_GetById(9);
  u32 i;

  Func_0200110c();
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
  PartyInventory_Remove(459);
  GameFlag_SetBit(0x8c9);
  Func_02001114();
}

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 flag;
};

struct AnimationRecord18 {
  u8 unk00[0x12];
  u16 frame;
};

extern u8 MsgFieldEyesOfTruth;
extern s16 gData_020013ca[];
extern const u8 gData_0200121c[];

void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200113c(s32 id, s32 value);
void Func_0200115c(s32 id);
s32 Func_02001184(s32 owner);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
void Func_020011c4(const void *data);
void Func_020011dc(const void *data);
s32 Func_020011fc(void);
void Func_02001204(void);
void Func_020008d0(struct Actor *object);
void Func_02000a28(void);
void Func_02000c94(void);
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
    Func_0200110c();
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
    Func_02001114();
  } else {
    Func_0200110c();
    Event_ResolvePendingActions(0);
    FieldMessage_Show((s32)&MsgFieldEyesOfTruth, 1);
    Func_02001114();
  }
}

void Func_02000874(void) {
  struct Actor *object;
  s32 x;

  if (Event_GetFlag(0x8cc) == 0) {
    object = Actor_GetById(64);
    x = object->x;
    if (x < (186 << 16)) {
      Func_0200110c();
      Event_ResolvePendingActions(0);
      Sound_Play(204);
      object->motionFlags = 3;
      wait(10);
      Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
      Event_SetFlag(0x8cc);
      Func_02001114();
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

struct EventWorkCbc {
  u8 unk000[0xcbc];
  void *active;
};

void Func_02000b38(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_0200093c(void) {
  struct EventWorkCbc *work = Ram_HeapSlots->eventWork;

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
    Func_0200113c(actor, target);
    target++;
    actor++;
  }
  MapGrid_CopyCellAttributes(0, 0, 2, 2, 9, 22);
  wait(1);
}

void Func_02000a28(void) {
  struct Actor *lever = Actor_GetById(8);
  struct Actor *rock = Actor_GetById(64);
  struct AnimationRecord18 *animation = rock->animation;

  animation->frame = ((struct AnimationRecord18 *)lever->animation)->frame;
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
  Func_0200110c();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(party->currentOwner, 0x8000, 0x4000);
  cell = object->x >> 16;
  Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                object->z);
  Script_WaitForEventTimeout(object);
  object->angle = FACING_NORTH;
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

  Func_0200110c();
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
    object->angle = FACING_NORTH;
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
  Func_0200110c();
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
  Func_0200110c();
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

s32 Func_02000d54(void) {
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
  Func_020011dc(gData_020013c0);
  if (GameFlag_Test(0x9ef) != 0)
    Actor_SetPosition(8, PIXELS(568), PIXELS(280));
  Actor_GetById(8)->priorityFlags &= 0xfe;
  Func_02000038(gData_020013ca);
  Func_020011c4(gData_020013c4);
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
