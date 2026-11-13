/// @file
/// The scene code of overlay 0x6aa, in ROM order.

#include "shared/map_cells.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "map.h"
#include "shared/mars_lighthouse.h"
#include "shared/actors.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "field_event.h"
#include "system/field_effect.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/io_reg.h"
#include "version.h"
#include "call.h"
#include "field/event_work.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "module/system/system.h"
#include "field_sprite.h"
#include "macros.h"
#include "system/input.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "shared/effects.h"
#include "module/field/field_effect.h"
#include "system/dma.h"
#include "objcmd.h"
#ifndef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#define TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H

#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

extern u8 gInventorySnapshot[];

#define RIDE_WORK ((struct RideWork *)(gInventorySnapshot + 0x188))

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#endif

s32 Func_02000038(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

void Func_02000044(void) { Field_RequestDjinnBattle(11, 57); }

void Func_02000050(void) { Field_RequestDjinnBattle(17, 36); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/* VIEW(first): routine gMarsLighthouseEntrance_Placements as this part declared
 * it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Placements");
#define gMarsLighthouseEntrance_Placements                                     \
  gMarsLighthouseEntrance_Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room1Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room1Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room1Placements");
#define gMarsLighthouseEntrance_Room1Placements                                \
  gMarsLighthouseEntrance_Room1Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room2Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room2Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room2Placements");
#define gMarsLighthouseEntrance_Room2Placements                                \
  gMarsLighthouseEntrance_Room2Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room3Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room3Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room3Placements");
#define gMarsLighthouseEntrance_Room3Placements                                \
  gMarsLighthouseEntrance_Room3Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room4Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room4Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room4Placements");
#define gMarsLighthouseEntrance_Room4Placements                                \
  gMarsLighthouseEntrance_Room4Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room5Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room5Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room5Placements");
#define gMarsLighthouseEntrance_Room5Placements                                \
  gMarsLighthouseEntrance_Room5Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room6Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room6Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room6Placements");
#define gMarsLighthouseEntrance_Room6Placements                                \
  gMarsLighthouseEntrance_Room6Placements__first
/* VIEW(first): routine gMarsLighthouseEntrance_Room7Placements as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room7Placements__first[] __asm__(
    "gMarsLighthouseEntrance_Room7Placements");
#define gMarsLighthouseEntrance_Room7Placements                                \
  gMarsLighthouseEntrance_Room7Placements__first

s32 Func_02000070(void) {
  Func_0200246c(0, 65);
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gMarsLighthouseEntrance_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gMarsLighthouseEntrance_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gMarsLighthouseEntrance_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gMarsLighthouseEntrance_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gMarsLighthouseEntrance_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gMarsLighthouseEntrance_Room6Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gMarsLighthouseEntrance_Room7Placements;
  return gMarsLighthouseEntrance_Placements;
}

void Func_02000118(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[0xd38] = 1;
}

extern struct MapCell gMapCellBuffer[];

DEFINE_MAP_CELL_GET(Func_0200012c)

DEFINE_MAP_CELL_SWAP(Func_02000168)

#include "../field/object/set_sprite_priority.c"

// ---- scene/mars_lighthouse_entrance/mars_lighthouse_entrance.c ----

/// Scene code of overlay 0x6aa, in ROM order.

#include "text/msg_ids.h"

void MapGrid_SetCellHeight();
void Teleport_PlayWhirl(void);
void Teleport_PlaySparks(void);
extern u16 gData_020068a2;
extern u32 Data_020068a4[3];
extern s16 gData_020068a0;
void TileMap_ConvertBuffer(s32 scene, s32 a1);
s32 FieldEffect_StartBlankOverlay();
s32 Weather_StartRain();
extern u8 Data_02006878[];

s32 Func_0200012c();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

void MarsLighthouseEntrance_SetIceConeCells(struct Actor *object) {
  s32 t1, t2, t3, t4, v10, v6, v7, v8, v9;
  struct Actor *v5;
  u8 *v11;

  v5 = object;
  v9 = (v5->x >> 20);
  v10 = (v5->z >> 20);
  t1 = Func_0200012c(0, v9, v10);
  v8 = t1;
  t2 = Func_0200012c(2, v9, v10);
  v6 = t2;
  v11 = &v5->terrainId;
  t3 = Map_GetTerrainHeight(v5->terrainId, v5->x, v5->z);
  v7 = (t3 >> 19);
  t4 = Event_GetFlag(v5->timer);
  if (t4 == 0)
    goto L_02008268;
  v6 = (v6 & -0xff01);
  v8 = (v8 & -129);
  if ((v5->terrainHeight >> 19) == v7)
    goto L_02008282;
  v7 = (v7 + -4);
  goto L_02008282;
L_02008268:
  v6 = (v6 & 255);
  v6 = (v6 | 0xff00);
  v8 = (v8 | 128);
  v7 = (v7 + 4);
  Scene_SetActorSpritePriority(v5, 2);
L_02008282:
  Func_02000168(0, v9, v10, v8);
  Func_02000168(2, v9, v10, v6);
  MapGrid_SetCellHeight((v9 << 20), (v10 << 20), *v11, v7);
}

void Func_02004bc0();

/// Each row: an ice cone's actor, its first animation and the flag set once
/// Pound has flattened it.
void MarsLighthouseEntrance_PlaceIceCones(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = (struct FieldActor *)Actor_Get(row[0]);
  animation = row[1];
  flag = row[2];

  Actor_SetSpriteFlags(object, 0);
  Func_02004bc0(object, (u16)animation + Event_GetFlag((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  MarsLighthouseEntrance_SetIceConeCells(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

/* VIEW(mars_lighthouse_entrance): routine Func_02003b30 as this part declared
 * it; the joined file has another. */
void Func_02003b30__mars_lighthouse_entrance(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("Func_02003b30");
#define Func_02003b30 Func_02003b30__mars_lighthouse_entrance

/// While index is at most 7, spawns effect type 0x117 index steps below the
/// object the effect work names and shrinks the object a little; mode 1
/// restores its size, sets its flag, hands it to
/// MarsLighthouseEntrance_SetIceConeCells and clears its position.
void MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index) {
  struct EffectHolder *holder = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_Get(holder->objectId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = 0x117;
    options.startScaleX = options.startScaleY = 0x10000;
    options.targetScaleX = options.targetScaleY = 0x8000;
    options.rotation = (Random16() & 15) << 12;
    Func_02003b30(object->x, object->y - index * 3 * 0x10000 + 0x100000,
                  object->z, ((Random16() & 15) - 8) << 13, 0, 0, 0x5a0000,
                  &options);
    object->scaleY -= 0x1f00;
  }
  if (mode == 1) {
    object->scaleY = 0x10000;
    Event_SetFlag(object->timer);
    MarsLighthouseEntrance_SetIceConeCells(Actor_Get(holder->objectId));
    object->x = object->y = object->z = 0;
  }
}

/* VIEW(mars_lighthouse_entrance): routine MarsLighthouseEntrance_PoundIceCone
 * as void MarsLighthouseEntrance_PoundIceCone(void); the definition has void
 * MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index); not tried:
 * MarsLighthouseEntrance_PoundIceCone's bound declaration was not found. */
extern void Func_02000338__mars_lighthouse_entrance(void) __asm__(
    "MarsLighthouseEntrance_PoundIceCone");

void MarsLighthouseEntrance_StartPoundIceCone(void) {
  Func_02000338__mars_lighthouse_entrance();
}

/* VIEW(mars_lighthouse_entrance): routine gData_02005394 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005394__mars_lighthouse_entrance[] __asm__("gData_02005394");
#define gData_02005394 gData_02005394__mars_lighthouse_entrance

void Func_02000414(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16() << 4) - (Random16() << 4);
    struct Actor *object =
        Func_02004bd8_(0x145, x, (Random16() << 3) + source->y + 0x40000,
                       source->z + (Random16() << 4) - (Random16() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16();
      radius = Random16();
      scale = (Random16() >> 1) + 0x4000;
      Actor_StartScript(object, gData_02005394);
      Actor_SetSpriteFlags(object, 0);
      /* FAKEMATCH(dead): an empty once-only block holds the order around it; as
       * no block, 2 of 113 instructions differ in tla-en. */
      do {
      } while (0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

/// Grind on the ice at the dead end, actor 10: the crack runs to the ice sheet
/// a floor below.
void MarsLighthouseEntrance_GrindIce(void) {
  s32 x;
  s32 count;

  Event_PlaySound(0x23c);
  x = 32;
  count = 6;
  // A goto loop, which GCC does not treat as a loop, so the stack arguments
  // are built inside it.
step:
  Func_02004c30_(x, 125, 3, 3, 39, 85);
  Func_02000414(10);
  count--;
  System_Wait(10);
  x += 3;
  if (count >= 0)
    goto step;
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(20);
}

/// ★ Named from the game: " checked the ice." (MsgCheckedTheIce)
void MarsLighthouseEntrance_CheckIce(void) {
  FieldMessage_Show((s32)MsgCheckedTheIce, 1);
  FieldMessage_Show((s32)MsgCheckedTheIce + 1, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
}

/// Teleport on the circle: the first time the camera follows the leader across;
/// after, the leader leaves at once.
void MarsLighthouseEntrance_Teleport(void) {
  if (!Flag_IsSet(0xa36)) {
    Func_020046b4();
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Teleport_PlaySparks();
    WORD_AT(Actor_Get(gMainRoutineWork.currentOwner), 108) = 0;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
    Func_02004cd8_(gMainRoutineWork.currentOwner, PIXELS(688), PIXELS(360));
    FieldCamera_MoveTo(PIXELS(688), -1, PIXELS(360), 1);
    FieldCamera_WaitForArrival();
    Teleport_PlayWhirl();
    FieldEvent_Wait(40);
    Func_02004718();
    FieldPsynergy_FadeFromCastColor();
    System_Wait(10);
    Event_End();
    Flag_Set(0xa36);
  } else {
    Func_020046b4();
    Teleport_PlaySparks();
    {
      u8 *party = (u8 *)&gMainRoutineWork;

      party[0x2c2] = 2;
    }
    FieldEvent_SetExitRequest(18);
    FieldEvent_Wait(40);
    Func_02004718();
  }
}

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000648, Func_020046b4, Teleport_PlaySparks,
                           17, Func_02004718)

/// Burst on the cracked ice sheet, actor 18: a flash, the sheet gone, chips
/// flying (flag 0xa3f).
void MarsLighthouseEntrance_BurstIceSheet(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(0x23e);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02004c30_(47, 94, 10, 6, 47, 100);
    Func_02004c28_(110, 40, 12, 4, 46, 40);
    Actor_SetPosition(18, 0, 0);
    Flag_Set(0xa3f);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    Func_02000414(18);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

void Func_02000710(void) { Func_02001a00(); }

void Func_02000718(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(168), 0, PIXELS(664), 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  Camera_SetBounds(PIXELS(472), 0, PIXELS(664), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(90);
  Event_End();
}

void Func_02000798(void) {
  MapGrid_CopyCellAttributes(14, 55, 14, 3, 14, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 14, 104);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 14, 105);
}

void Func_02000808(void) {
  struct Actor *object = Actor_Get(20);

  if (!Flag_IsSet(0xa47)) {
    Event_PlaySound(0x23d);
    Flag_Set(0x231);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(0xa48);
    Func_02000798();
  }
}

#define DEFINE_FUNC_0200085C(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)         \
  void name(s32 state, s32 frame) {                                            \
    if (state == 1) {                                                          \
      Event_PlaySound(0x23e);                                                  \
      PaletteFade_ApplyTarget(0x10000, 0);                                     \
      PaletteFade_Interpolate(60);                                             \
      MapGrid_CopyCellsLayered(n1, n2, 8, n3, n1, n4);                         \
      MapGrid_CopyCellAttributes(n5, n6, 8, n7, n1, n6);                       \
      Actor_SetPosition(n8, 0, 0);                                             \
      Flag_Set(n9);                                                            \
    }                                                                          \
    if (frame > 96 && frame < 272 && frame % 30 == 0)                          \
      Func_02000414(n8);                                                       \
    if (frame == 276) {                                                        \
      PaletteFade_ApplyTarget(0x7fff, 0);                                      \
      PaletteFade_Interpolate(20);                                             \
    }                                                                          \
  }

DEFINE_FUNC_0200085C(Func_0200085c, 28, 92, 8, 100, 92, 38, 8, 20, 0xa47)

/// Move on the statue, actor 8, to cell x 20: it blocks the last flame thrower
/// (flag 0xa4f).
void MarsLighthouseEntrance_PushStatueIntoFlames(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 20) {
      FieldEvent_Wait(5);
      object->z += PIXELS(1);
      BYTE_AT(object, 98) = 1;
      Flag_Set(0xa4f);
    }
  }
}

void Func_02000930(void) { GameFlag_Set(0x144); }

DEFINE_SCENE_LEAVE_BY_EXIT(Func_0200093c, Func_020046b4, Teleport_PlaySparks,
                           14, Func_02004718)

/// Sends object id from column 11 to x 152, then from column 10 to x 200,
/// both at z 792, through Actor_SetMoveTarget.
void Func_0200096c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 11) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    Actor_SetMoveTarget(object, PIXELS(152), y, PIXELS(792));
  }
  if (object->x >> 20 == 10) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    {
      /* FAKEMATCH(pin): the actor pinned to r0 to keep the argument order; as
       * an unpinned local, 2 of 38 instructions differ in tla-en. */
      register struct Actor *self asm("r0") = object;
      s32 x = 0xc80000;
      /* FAKEMATCH(pin): z pinned to r3 to keep the argument order; as an
       * unpinned local, 2 of 38 instructions differ in tla-en. */
      register s32 z asm("r3") = 0x3180000;

      Actor_SetMoveTarget(self, x, y, z);
    }
  }
}

void Func_020009c0(void) {
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(88), -1, PIXELS(696), 1);
  Func_020019c4();
}

void Func_020009e8(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void Func_02000a24(void) {
  struct Actor *object = Actor_Get(12);

  if (!Flag_IsSet(0xa4d)) {
    Event_PlaySound(0x23d);
    Flag_Set(0x232);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(0xa4c);
  }
}

void Func_02000a74(void) { Func_020019e0(); }

void Func_02000a7c(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(872), -1, PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(264), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000af4, Func_020046b4, Teleport_PlaySparks,
                           14, Func_02004718)

void Func_02000b24(void) {
  MapGrid_CopyCells(53, 30, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 30, 4, 3, 117, 48);
}

void Func_02000b54(void) {
  MapGrid_CopyCells(53, 40, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 45, 4, 3, 117, 48);
}

DEFINE_FUNC_0200085C(Func_02000b84, 12, 95, 6, 101, 76, 40, 5, 12, 0xa4d)

void Func_02000c1c(void) {
  MapGrid_CopyCellAttributes(35, 11, 3, 6, 53, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 117, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 53, 75);
  Flag_Set(0xa49);
}

void Func_02000c68(void) { MapGrid_CopyCellAttributes(25, 9, 11, 3, 26, 9); }

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000c84, Func_020046b4, Teleport_PlaySparks,
                           12, Func_02004718)

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000cb4, Func_020046b4, Teleport_PlaySparks,
                           13, Func_02004718)

void Func_02000ce4(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 5) {
      FieldEvent_Wait(5);
      object->z += PIXELS(1);
      Flag_Set(0xa4e);
    }
  }
}

#define Oam_AddSpriteAtDepthClamped Oam_AddSpriteAtDepthClamped

void Func_02000d18(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 x = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 originY = map->origin[1];
  s32 tile = gResourceTableEntries[gData_020068a2].offset >> 5;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 y, z, depth, sx, sy;
  u32 *word;

  x = ((leader->x >> 20) << 20) - x;
  y = Map_GetTerrainHeight(0, leader->x, leader->z) - originY;
  z = ((leader->z >> 20) << 20) - cameraZ - originY;
  sy = z - y;
  sx = x >> 16;
  sy = (sy >> 16) + 16;
  depth = ((y + z) >> 16) + 58;
  if ((u32)(sx + 15) > 334 || sy < -15 || sy > 239)
    return;
  sx &= 0x1ff;
  sy &= 0xff;
  word = Data_020068a4;
  *word++ = 0;
  *word++ = sy | (sx << 16) | 0x80008800;
  *word = tile | 0x400;
  Oam_AddSpriteAtDepthClamped(Data_020068a4, depth);
}

/// Fills 0x100 bytes with colour 1, loads them into a VRAM block of their own
/// and starts Func_02000d18.
void Func_02000df0(void) {
  u32 *buffer = Heap_Alloc(0x100);
  u32 *word = buffer;
  s32 count = 63;

  // A goto loop: as a for loop loop.c moves the fill word out of it.
fill:
  *word++ = 0x11111111;
  if (--count >= 0)
    goto fill;
  gData_020068a2 = VramCache_FindFreeEntry();
  Vram_Load(gData_020068a2, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000d18, TASK_PASS_VBLANK);
}

// A once-only block with a local that holds one constant, as the native
// builds the value.
#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = 0xf1f;                                                        \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

/// The leader shrinks and sinks over 40 frames, then the screen transition
/// ends.
void Func_02000e3c(s32 arg) {
  struct Actor *actor;
  struct MapWork *map;
  struct MapCell *cells;
  struct MapCell *buffer;
  s32 x, z;
  s32 i;

  actor = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  actor->update = NULL;
  {
    s32 value = 0x100;

    FieldActor_ShowEmote(gMainRoutineWork.currentOwner, value, 0);
  }
  Func_02004bc0(actor, 27);
  Actor_SetAnimationSpeed(actor, 48);
  System_Wait(30);
  actor->facing = FACING_SOUTH;
  actor->motionFlags = 0;
  buffer = gMapCellBuffer;
  x = actor->x >> 20;
  z = actor->z >> 20;
  map = Ram_HeapSlots->mapWork;
  if (map != NULL)
    cells = map->layers[actor->terrainId & 3].cells;
  else
    cells = buffer;
  cells += x + (z << 7) + 128;
  if (!(cells->attributeB & 0x40)) {
    SET_SPRITE_WINDOW_LAYERS();
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(204);
  actor->scaleX -= 0x400;
  actor->scaleY -= 0x300;
  actor->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct Overlay6aaEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(arg);
}

/* VIEW(mars_lighthouse_entrance): routine gData_0200503c as this part declared
 * it; the joined file has another. */
extern const u8
    gData_0200503c__mars_lighthouse_entrance[] __asm__("gData_0200503c");
#define gData_0200503c gData_0200503c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200547c as this part declared
 * it; the joined file has another. */
extern const u8
    gData_0200547c__mars_lighthouse_entrance[] __asm__("gData_0200547c");
#define gData_0200547c gData_0200547c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020054cc as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020054cc__mars_lighthouse_entrance[] __asm__("gData_020054cc");
#define gData_020054cc gData_020054cc__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005528 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005528__mars_lighthouse_entrance[] __asm__("gData_02005528");
#define gData_02005528 gData_02005528__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005578 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005578__mars_lighthouse_entrance[] __asm__("gData_02005578");
#define gData_02005578 gData_02005578__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020055d4 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020055d4__mars_lighthouse_entrance[] __asm__("gData_020055d4");
#define gData_020055d4 gData_020055d4__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005644 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005644__mars_lighthouse_entrance[] __asm__("gData_02005644");
#define gData_02005644 gData_02005644__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Triggers as
 * this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Triggers");
#define gMarsLighthouseEntrance_Triggers                                       \
  gMarsLighthouseEntrance_Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room1Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room1Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room1Triggers");
#define gMarsLighthouseEntrance_Room1Triggers                                  \
  gMarsLighthouseEntrance_Room1Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room2Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room2Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room2Triggers");
#define gMarsLighthouseEntrance_Room2Triggers                                  \
  gMarsLighthouseEntrance_Room2Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room3Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room3Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room3Triggers");
#define gMarsLighthouseEntrance_Room3Triggers                                  \
  gMarsLighthouseEntrance_Room3Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room4Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room4Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room4Triggers");
#define gMarsLighthouseEntrance_Room4Triggers                                  \
  gMarsLighthouseEntrance_Room4Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room5Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room5Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room5Triggers");
#define gMarsLighthouseEntrance_Room5Triggers                                  \
  gMarsLighthouseEntrance_Room5Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room6Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room6Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room6Triggers");
#define gMarsLighthouseEntrance_Room6Triggers                                  \
  gMarsLighthouseEntrance_Room6Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room7Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room7Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room7Triggers");
#define gMarsLighthouseEntrance_Room7Triggers                                  \
  gMarsLighthouseEntrance_Room7Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02006868 as this part declared
 * it; the joined file has another. */
extern s16 gData_02006868__mars_lighthouse_entrance __asm__("gData_02006868");
#define gData_02006868 gData_02006868__mars_lighthouse_entrance

/* VIEW(mars_lighthouse_entrance): routine Func_02002c2c as void
 * Func_02002c2c(void); scene/mars_lighthouse_entrance/s263c has void
 * Func_02002c2c(struct Actor *object) {; tried as the definition's
 * DEFINE_EMBERS_SPAWN(Func_02002b4c, gData_02005714) void Func_02002c2c(struct
 * Actor *object), which the object already declares, it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1868: conflicting
 * types for `Func_02002c2c'. */
void Func_02002c2c__mars_lighthouse_entrance(void) __asm__("Func_02002c2c");
#define Func_02002c2c Func_02002c2c__mars_lighthouse_entrance

void Func_02000fa4(void) {
  Event_PlaySound(0x248);
  Flag_Set(0x17d);
  Actor_SetScript(11, gData_0200503c);
  Func_0200246c(0, 3);
  Flag_Set(0x234);
}

void Func_02000fd4(void) {
  Func_02003564();
  Func_020029a0();
}

void GameFlag_SetByte(s32 flag, s32 value);

void Func_02000fe0(const struct MapCell *cell) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  // The leader's map column and, later, the frame count share one variable,
  // as the native's register shows.
  s32 i = leader->x >> 20;

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, z >> 20);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  leader->update = 0;
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Func_02004bc0(leader, 27);
  Actor_SetAnimationSpeed(leader, 48);
  System_Wait(30);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  if ((cell->attributeB & 64) == 0) {
    {
      u16 winout = 0xf1f;

      REG_WINOUT = winout;
    }
    REG_DISPCNT |= 0x8000;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(204);
  leader->scaleX -= 0x400;
  leader->scaleY -= 0x300;
  leader->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

void Func_0200112c(void) {
  Field_TryJumpForward();
  Func_02003884();
}

void Func_02001138(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 x;
  s32 z;
  struct MapCell *cell;

  object = Actor_Get(party->currentOwner);
  x = object->x >> 20;
  z = object->z >> 20;
  cell = gMapCellBuffer;
  if (map != NULL)
    cell = map->layers[object->terrainId & 3].cells;
  cell += x + (z << 7);
  if (cell->attributeB & 64)
    Func_02000fe0(cell + 128);
}

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gMarsLighthouseEntrance_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gMarsLighthouseEntrance_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gMarsLighthouseEntrance_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gMarsLighthouseEntrance_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gMarsLighthouseEntrance_Room5Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gMarsLighthouseEntrance_Room6Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gMarsLighthouseEntrance_Room7Triggers;
  return gMarsLighthouseEntrance_Triggers;
}

DEFINE_ACTOR_FREEZE(Func_02001234)
#undef SET_SPRITE_WINDOW_LAYERS

/* VIEW(mars_lighthouse_entrance): routine gData_02006868 as this part declared
 * it; the joined file has another. */
extern s16 gData_02006868__mars_lighthouse_entrance __asm__("gData_02006868");
#define gData_02006868 gData_02006868__mars_lighthouse_entrance

void Func_02001254(void) {
  s16 *state = &gData_02006868;

  if (gData_020068a0 < 60) {
    if (*state == 0) {
      Func_0200246c(0, 3);
      *state = 1;
    }
  } else {
    // Read through a struct, so PRE does not merge it with the first arm's
    // read.
    if (((struct HalfwordField *)state)->value == 1) {
      Func_0200246c(0, 67);
      *state = 0;
    }
  }
  if (++gData_020068a0 > 300)
    gData_020068a0 = 0;
}

DEFINE_ACTOR_BLINK(Func_020012bc, 0, 6, 10)

void Func_020012e4(struct Actor *actor) {
  struct Sprite020012e4 *sprite = actor->sprite;

  sprite->unk12 += 0x800;
  if (++actor->timer == 8)
    actor->velocityY = 0x40000;
  if (actor->timer > 9) {
    actor->x += 0x40000;
    actor->scaleX -= 0x800;
    actor->scaleY -= 0x800;
  }
  if (actor->timer == 32) {
    actor->x = 0;
    actor->z = 0;
    actor->update = NULL;
  }
}

/* VIEW(mars_lighthouse_entrance): routine gData_0200503c as this part declared
 * it; the joined file has another. */
extern const u8
    gData_0200503c__mars_lighthouse_entrance[] __asm__("gData_0200503c");
#define gData_0200503c gData_0200503c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200547c as this part declared
 * it; the joined file has another. */
extern const u8
    gData_0200547c__mars_lighthouse_entrance[] __asm__("gData_0200547c");
#define gData_0200547c gData_0200547c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020054cc as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020054cc__mars_lighthouse_entrance[] __asm__("gData_020054cc");
#define gData_020054cc gData_020054cc__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005528 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005528__mars_lighthouse_entrance[] __asm__("gData_02005528");
#define gData_02005528 gData_02005528__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005578 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005578__mars_lighthouse_entrance[] __asm__("gData_02005578");
#define gData_02005578 gData_02005578__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020055d4 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020055d4__mars_lighthouse_entrance[] __asm__("gData_020055d4");
#define gData_020055d4 gData_020055d4__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005644 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005644__mars_lighthouse_entrance[] __asm__("gData_02005644");
#define gData_02005644 gData_02005644__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Triggers as
 * this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Triggers");
#define gMarsLighthouseEntrance_Triggers                                       \
  gMarsLighthouseEntrance_Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room1Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room1Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room1Triggers");
#define gMarsLighthouseEntrance_Room1Triggers                                  \
  gMarsLighthouseEntrance_Room1Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room2Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room2Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room2Triggers");
#define gMarsLighthouseEntrance_Room2Triggers                                  \
  gMarsLighthouseEntrance_Room2Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room3Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room3Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room3Triggers");
#define gMarsLighthouseEntrance_Room3Triggers                                  \
  gMarsLighthouseEntrance_Room3Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room4Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room4Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room4Triggers");
#define gMarsLighthouseEntrance_Room4Triggers                                  \
  gMarsLighthouseEntrance_Room4Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room5Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room5Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room5Triggers");
#define gMarsLighthouseEntrance_Room5Triggers                                  \
  gMarsLighthouseEntrance_Room5Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room6Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room6Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room6Triggers");
#define gMarsLighthouseEntrance_Room6Triggers                                  \
  gMarsLighthouseEntrance_Room6Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room7Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room7Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room7Triggers");
#define gMarsLighthouseEntrance_Room7Triggers                                  \
  gMarsLighthouseEntrance_Room7Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02006868 as this part declared
 * it; the joined file has another. */
extern s16 gData_02006868__mars_lighthouse_entrance __asm__("gData_02006868");
#define gData_02006868 gData_02006868__mars_lighthouse_entrance

void Func_02002c2c(void);

void Func_02001344(struct Actor *object) {
  if (ABS(object->x - object->watchedObject->x) > 0x200000)
    return;
  object->timer = 0;
  object->update = (void (*)(void))Func_020012e4;
}

void Func_02001378(void) {
  if (Func_020029bc())
    Func_02003878(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
/* VIEW(mars_lighthouse_entrance): routine Func_020022e4 as s32 Func_020022e4();
 * scene/mars_lighthouse_entrance/s2270 has DEFINE_BEAM_START(Func_020022e4,
 * Func_020020ec) s32 Func_02002418(s32 index) {; tried as the one declaration
 * s32 Func_020022e4(), it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1047: `Func_020022e4'
 * undeclared (first use in this function). */
s32 Func_020022e4__mars_lighthouse_entrance() __asm__("Func_020022e4");
#define Func_020022e4 Func_020022e4__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine Func_0200248c as s32 Func_0200248c();
 * scene/mars_lighthouse_entrance/s2270 has void Func_0200248c(s32 index, s16
 * value) {; tried as the definition's void Func_0200248c(s32 index, s16 value),
 * which the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1145: `Func_0200248c'
 * undeclared (first use in this function). */
s32 Func_0200248c__mars_lighthouse_entrance() __asm__("Func_0200248c");
#define Func_0200248c Func_0200248c__mars_lighthouse_entrance
/* VIEW(first): routine Func_020045f8 as this part declared it; the joined file
 * has another. */
s32 Func_020045f8__first() __asm__("Func_020045f8");
#define Func_020045f8 Func_020045f8__first
void Func_0200196c(void);
/* VIEW(mars_lighthouse_entrance): routine gData_02005314 as this part declared
 * it; the joined file has another. */
extern u8 gData_02005314__mars_lighthouse_entrance[] __asm__("gData_02005314");
#define gData_02005314 gData_02005314__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020053d8 as this part declared
 * it; the joined file has another. */
extern u8 gData_020053d8__mars_lighthouse_entrance[] __asm__("gData_020053d8");
#define gData_020053d8 gData_020053d8__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020053e6 as this part declared
 * it; the joined file has another. */
extern u8 gData_020053e6__mars_lighthouse_entrance[] __asm__("gData_020053e6");
#define gData_020053e6 gData_020053e6__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020053f8 as this part declared
 * it; the joined file has another. */
extern u8 gData_020053f8__mars_lighthouse_entrance[] __asm__("gData_020053f8");
#define gData_020053f8 gData_020053f8__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020053fe as this part declared
 * it; the joined file has another. */
extern u8 gData_020053fe__mars_lighthouse_entrance[] __asm__("gData_020053fe");
#define gData_020053fe gData_020053fe__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200540c as this part declared
 * it; the joined file has another. */
extern u8 gData_0200540c__mars_lighthouse_entrance[] __asm__("gData_0200540c");
#define gData_0200540c gData_0200540c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005412 as this part declared
 * it; the joined file has another. */
extern u8 gData_02005412__mars_lighthouse_entrance[] __asm__("gData_02005412");
#define gData_02005412 gData_02005412__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005418 as this part declared
 * it; the joined file has another. */
extern u8 gData_02005418__mars_lighthouse_entrance[] __asm__("gData_02005418");
#define gData_02005418 gData_02005418__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005438 as this part declared
 * it; the joined file has another. */
extern u8 gData_02005438__mars_lighthouse_entrance[] __asm__("gData_02005438");
#define gData_02005438 gData_02005438__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200543e as this part declared
 * it; the joined file has another. */
extern u8 gData_0200543e__mars_lighthouse_entrance[] __asm__("gData_0200543e");
#define gData_0200543e gData_0200543e__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005448 as this part declared
 * it; the joined file has another. */
extern u8 gData_02005448__mars_lighthouse_entrance[] __asm__("gData_02005448");
#define gData_02005448 gData_02005448__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200544e as this part declared
 * it; the joined file has another. */
extern u8 gData_0200544e__mars_lighthouse_entrance[] __asm__("gData_0200544e");
#define gData_0200544e gData_0200544e__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005454 as this part declared
 * it; the joined file has another. */
extern u8 gData_02005454__mars_lighthouse_entrance[] __asm__("gData_02005454");
#define gData_02005454 gData_02005454__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200586c as this part declared
 * it; the joined file has another. */
extern u8 gData_0200586c__mars_lighthouse_entrance[] __asm__("gData_0200586c");
#define gData_0200586c gData_0200586c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200587c as this part declared
 * it; the joined file has another. */
extern u8 gData_0200587c__mars_lighthouse_entrance[] __asm__("gData_0200587c");
#define gData_0200587c gData_0200587c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020058a8 as this part declared
 * it; the joined file has another. */
extern u8 gData_020058a8__mars_lighthouse_entrance[] __asm__("gData_020058a8");
#define gData_020058a8 gData_020058a8__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020058d4 as this part declared
 * it; the joined file has another. */
extern u8 gData_020058d4__mars_lighthouse_entrance[] __asm__("gData_020058d4");
#define gData_020058d4 gData_020058d4__mars_lighthouse_entrance
/* VIEW(s138c): variable gMainRoutineWork as extern u8 gMainRoutineWork[];
 * main_routine_work.h has extern struct MainRoutineWork gMainRoutineWork; tried
 * as the 4 declarations' extern u8 gMainRoutineWork[], it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:995: conflicting types
 * for `gMainRoutineWork'. */
extern u8 gMainRoutineWork__s138c[] __asm__("gMainRoutineWork");

s32 Scene_Initialize(void) {
  s32 v_r8, v_r7, v_r5, t0, t1, t2, t8, t12, t17, t21;
  s32 t23, t24, t26, t27, t29, t34, t38, t40, t41, t48;
  s32 t51, t52, t55, t56, t57, t72, t73, t74, t78, t81;
  s32 t93, t96, t97, t98, t99, t100, t101, t102, t103, t104;
  s32 t105, t106, t107, t108, t109, t110, t111;

  t0 = FIELD_AT((s32)gMainRoutineWork__s138c, s16, 480);
  t1 = FIELD_AT((s32)gMainRoutineWork__s138c, s16, 482);
  *(s32 *)(s32)Data_02006878 = 0;
  t2 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t2, s32, 428) = 0x204;
  Event_SetFlag(0x112);
  Func_02002270();
  v_r7 = t1;
  v_r8 = t0;
  if (v_r8 != (s32)&SceneId_Ov6aaRoom1)
    goto L_020093dc;
  Call1((void (*)())Weather_StartRain, 1);
  goto L_020098be;
L_020093dc:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom2)
    goto L_0200944c;
  Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
#if !TLA_PATCH_1
  FieldActor_SetSpritePriority(11, 3);
#endif
  Call1((void (*)())Func_02001234, 10);
  t8 = Event_GetFlag(0xa30);
  if (t8 == 0)
    goto L_02009420;
  MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
  MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
L_02009420:
  Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones, (s32)gData_020053d8);
  t12 = Event_GetFlag(0xa3e);
  if (t12 != 0)
    goto L_02009436;
  goto L_020098be;
L_02009436:
  MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  goto L_020098be;
L_0200944c:
  if (v_r8 == (s32)&SceneId_Ov6aaRoom3)
    goto L_02009454;
  goto L_02009680;
L_02009454:
  Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
  Call4((void (*)())Func_020022e4, (s32)gData_020053e6, (s32)gData_020053f8, 0,
        88);
  Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones, (s32)gData_020053fe);
  t17 = Event_GetFlag(0xa3f);
  if (t17 == 0)
    goto L_020094ae;
  MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
  MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
  Actor_SetPosition(18, 0, 0);
  goto L_020094e0;
L_020094ae:
  t21 = Event_GetFlag(0xa3e);
  if (t21 == 0)
    goto L_020094d6;
  Event_SetFlag(0x230);
  t23 = (s32)Actor_Get(18);
  FIELD_AT(t23, u8, 35) |= 2;
  goto L_020094e0;
L_020094d6:
  Actor_SetPosition(18, 0, 0);
L_020094e0:
  t26 = (s32)Actor_Get(20);
  FIELD_AT(t26, u8, 35) |= 2;
  FieldActor_SetSpritePriority(20, 2);
  t29 = Event_GetFlag(0xa47);
  if (t29 == 0)
    goto L_0200953c;
  MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
  MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
  Func_02000798();
  Actor_SetPosition(20, 0, 0);
  goto L_02009564;
L_0200953c:
  t34 = Event_GetFlag(0xa48);
  if (t34 == 0)
    goto L_0200955a;
  Func_02000798();
  Event_SetFlag(0x231);
  goto L_02009564;
L_0200955a:
  Actor_SetPosition(20, 0, 0);
L_02009564:
  t38 = Event_GetFlag(0xa4f);
  if (t38 == 0)
    goto L_0200958c;
  Actor_SetPosition(8, 0x1480000, 0x780000);
  t40 = (s32)Actor_Get(8);
  FIELD_AT(t40, u8, 98) = 1;
L_0200958c:
  t41 = Event_GetFlag(0xa30);
  if (t41 == 0)
    goto L_020095a4;
  Actor_SetPosition(19, 0, 0);
  goto L_020095ae;
L_020095a4:
  Actor_SetPosition(15, 0, 0);
L_020095ae:
  if (v_r7 == 20)
    goto L_020095b4;
  goto L_020098be;
L_020095b4:
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_SetShake(0, 0x20000, 0x10000);
  t48 = FIELD_AT((s32)gMainRoutineWork__s138c, s32, 532);
  Actor_SetPosition(t48, 0, 0);
  Actor_SetPosition(18, 0, 0);
  t51 = (s32)Actor_Get(18);
  FIELD_AT(t51, u8, 35) |= 2;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t55 = (s32)Actor_Get(18);
  t56 = FIELD_AT(t55, s16, 100);
  t57 = FIELD_AT(t55, s16, 102);
  Actor_SetPosition(18, (t56 << 16), (t57 << 16));
  Event_PlaySound(0x23d);
  v_r5 = 2;
L_0200963c:
  Call1((void (*)())Func_02000414, 18);
  System_Wait(1);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_0200963c;
  Event_SetFlag(0xa3e);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldEvent_Wait(120);
  FieldEvent_SetExitRequest(20);
  Event_SetFlag(0x109);
  goto L_020098be;
L_02009680:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom4)
    goto L_020096b0;
  Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
  if (v_r7 == 10)
    goto L_02009694;
  if (v_r7 != 1)
    goto L_020096a8;
L_02009694:
  Call4((void (*)())Func_020022e4, (s32)gData_0200540c, (s32)gData_02005412,
        (s32)Func_020019a4, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
L_020096a8:
  Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones, (s32)gData_02005418);
  goto L_020098be;
L_020096b0:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom5)
    goto L_02009784;
  Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
  t72 = (s32)Actor_Get(12);
  FIELD_AT(t72, u8, 35) |= 2;
  t74 = Event_GetFlag(0xa4d);
  if (t74 == 0)
    goto L_0200970c;
  MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
  MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
  Actor_SetPosition(12, 0, 0);
  goto L_0200976e;
L_0200970c:
  t78 = Event_GetFlag(0xa4c);
  if (t78 == 0)
    goto L_02009764;
  Event_SetFlag(0x232);
  goto L_0200976e;
L_02009764:
  Actor_SetPosition(12, 0, 0);
L_0200976e:
  t81 = Event_GetFlag(0xa49);
  if (t81 != 0)
    goto L_0200977e;
  goto L_020098be;
L_0200977e:
  Func_02000c1c();
  goto L_020098be;
L_02009784:
  if (v_r8 == (s32)&SceneId_Ov6aaRoom6)
    goto L_0200978c;
  goto L_020098b2;
L_0200978c:
  Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
  if (v_r7 == 2)
    goto L_020097a2;
  if (v_r7 == 4)
    goto L_020097a2;
  if (v_r7 == 6)
    goto L_020097a2;
  if (v_r7 != 13)
    goto L_020097b6;
L_020097a2:
  Call4((void (*)())Func_020022e4, (s32)gData_02005438, (s32)gData_0200543e,
        (s32)Func_0200196c, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
L_020097b6:
  if ((u32)(v_r7 - 7) <= (u32)1)
    goto L_020097c0;
  if (v_r7 != 11)
    goto L_02009884;
L_020097c0:
  Call4((void (*)())Func_020022e4, (s32)gData_02005448, (s32)gData_0200544e,
        (s32)Func_02001988, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
  if (v_r7 != 11)
    goto L_020097fa;
  Actor_SetScript(11, gData_02005314);
  Func_0200246c(0, 3);
  Task_AddFrame(Func_02001254, 0x480);
  Event_SetFlag(0x17d);
L_020097fa:
  Task_AddFrame(Func_02001378, 0x480);
  t93 = (s32)Actor_Get(11);
  FIELD_AT(t93, s32, 108) = (s32)Func_020012bc;
  Func_020022d8(1);
  Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones, (s32)gData_02005454);
  t96 = (s32)Actor_Get(20);
  t97 = (s32)Actor_Get(13);
  FIELD_AT(t97, s32, 108) = (s32)Func_02001344;
  t98 = (s32)Actor_Get(14);
  FIELD_AT(t98, s32, 108) = (s32)Func_02001344;
  t99 = (s32)Actor_Get(15);
  FIELD_AT(t99, s32, 108) = (s32)Func_02001344;
  t100 = (s32)Actor_Get(16);
  FIELD_AT(t100, s32, 108) = (s32)Func_02001344;
  t101 = (s32)Actor_Get(17);
  FIELD_AT(t101, s32, 108) = (s32)Func_02001344;
  t102 = (s32)Actor_Get(18);
  FIELD_AT(t102, s32, 108) = (s32)Func_02001344;
  t103 = (s32)Actor_Get(13);
  FIELD_AT(t103, s32, 104) = t96;
  t104 = (s32)Actor_Get(14);
  FIELD_AT(t104, s32, 104) = t96;
  t105 = (s32)Actor_Get(15);
  FIELD_AT(t105, s32, 104) = t96;
  t106 = (s32)Actor_Get(16);
  FIELD_AT(t106, s32, 104) = t96;
  t107 = (s32)Actor_Get(17);
  FIELD_AT(t107, s32, 104) = t96;
  t108 = (s32)Actor_Get(18);
  FIELD_AT(t108, s32, 104) = t96;
L_02009884:
  t109 = (s32)Actor_Get(9);
  FIELD_AT(t109, u8, 35) |= 2;
  t111 = Event_GetFlag(0xa4e);
  if (t111 == 0)
    goto L_020098be;
  Actor_SetPosition(8, 0x580000, 0x2b80000);
  goto L_020098be;
L_020098b2:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom7)
    goto L_020098be;
  Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
L_020098be:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom6)
    goto L_020098d0;
  if (v_r7 != 13)
    goto L_020098d0;
  Event_ClearFlag(0x104);
L_020098d0:
  Call4((void (*)())Func_020045f8, (s32)gData_0200586c, (s32)gData_0200587c,
        (s32)gData_020058a8, (s32)gData_020058d4);
  return 0;
}
#undef FIELD_AT

/* VIEW(mars_lighthouse_entrance): routine gData_0200503c as this part declared
 * it; the joined file has another. */
extern const u8
    gData_0200503c__mars_lighthouse_entrance[] __asm__("gData_0200503c");
#define gData_0200503c gData_0200503c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_0200547c as this part declared
 * it; the joined file has another. */
extern const u8
    gData_0200547c__mars_lighthouse_entrance[] __asm__("gData_0200547c");
#define gData_0200547c gData_0200547c__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020054cc as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020054cc__mars_lighthouse_entrance[] __asm__("gData_020054cc");
#define gData_020054cc gData_020054cc__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005528 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005528__mars_lighthouse_entrance[] __asm__("gData_02005528");
#define gData_02005528 gData_02005528__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005578 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005578__mars_lighthouse_entrance[] __asm__("gData_02005578");
#define gData_02005578 gData_02005578__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_020055d4 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020055d4__mars_lighthouse_entrance[] __asm__("gData_020055d4");
#define gData_020055d4 gData_020055d4__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02005644 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_02005644__mars_lighthouse_entrance[] __asm__("gData_02005644");
#define gData_02005644 gData_02005644__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Triggers as
 * this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Triggers");
#define gMarsLighthouseEntrance_Triggers                                       \
  gMarsLighthouseEntrance_Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room1Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room1Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room1Triggers");
#define gMarsLighthouseEntrance_Room1Triggers                                  \
  gMarsLighthouseEntrance_Room1Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room2Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room2Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room2Triggers");
#define gMarsLighthouseEntrance_Room2Triggers                                  \
  gMarsLighthouseEntrance_Room2Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room3Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room3Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room3Triggers");
#define gMarsLighthouseEntrance_Room3Triggers                                  \
  gMarsLighthouseEntrance_Room3Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room4Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room4Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room4Triggers");
#define gMarsLighthouseEntrance_Room4Triggers                                  \
  gMarsLighthouseEntrance_Room4Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room5Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room5Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room5Triggers");
#define gMarsLighthouseEntrance_Room5Triggers                                  \
  gMarsLighthouseEntrance_Room5Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room6Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room6Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room6Triggers");
#define gMarsLighthouseEntrance_Room6Triggers                                  \
  gMarsLighthouseEntrance_Room6Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gMarsLighthouseEntrance_Room7Triggers
 * as this part declared it; the joined file has another. */
extern const u8
    gMarsLighthouseEntrance_Room7Triggers__mars_lighthouse_entrance[] __asm__(
        "gMarsLighthouseEntrance_Room7Triggers");
#define gMarsLighthouseEntrance_Room7Triggers                                  \
  gMarsLighthouseEntrance_Room7Triggers__mars_lighthouse_entrance
/* VIEW(mars_lighthouse_entrance): routine gData_02006868 as this part declared
 * it; the joined file has another. */
extern s16 gData_02006868__mars_lighthouse_entrance __asm__("gData_02006868");
#define gData_02006868 gData_02006868__mars_lighthouse_entrance

/* VIEW(s1930): routine Func_02002f4c as s32 Func_02002f4c(s32 id, s32 a1, s32
 * a2, s32 a3, const void *script); s1344 has void Func_02002f4c(s32 id, s32 a1,
 * s32 a2, s32 a3, const void *script); tried as the rest of the object declares
 * it, Func_0200196c moves: 1 of 11 instructions, Func_02001988 moves: 1 of 11
 * instructions, Func_020019a4 moves: 1 of 12 instructions, 3 more in tla-ja. */
extern s32 Func_02002f4c__s1930(s32 id, s32 a1, s32 a2, s32 a3,
                                const void *script) __asm__("Func_02002f4c");
void Func_02002c2c(void);

s32 Scene_PrepareMap(void) {
  if (Flag_IsSet(0xa30))
    TileMap_ConvertBuffer(gMainRoutineWork.map + ((s32)&SceneId_Ov6ACRoom3 -
                                                  (s32)&SceneId_Ov6a8Room2),
                          0x5000000);
  return 0;
}

void Func_0200196c(void) {
  Func_02002f4c__s1930(10, 19, 20, 0, gData_0200547c);
}

void Func_02001988(void) {
  Func_02002f4c__s1930(12, 19, 20, 0, gData_020054cc);
}

void Func_020019a4(void) {
  Func_02002f4c__s1930(9, 18, 20, 0x4000, gData_02005528);
}

void Func_020019c4(void) { Func_02002f4c__s1930(9, 13, 14, 0, gData_02005578); }

void Func_020019e0(void) {
  Func_02002f4c__s1930(11, 13, 14, 0x4000, gData_020055d4);
}

void Func_02001a00(void) {
  Func_02002f4c__s1930(13, 21, 22, 0, gData_02005644);
}

/* VIEW(mars_lighthouse_entrance): routine gData_020056a0 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020056a0__mars_lighthouse_entrance[] __asm__("gData_020056a0");
#define gData_020056a0 gData_020056a0__mars_lighthouse_entrance

void Func_02001f90(struct Actor *object);

DEFINE_ACTOR_IS_TOUCHING(Func_02001a1c)

DEFINE_ACTOR_IS_BESIDE(Func_02001a80)

/* VIEW(s1930): tag struct RideSlot as struct RideSlot__s1930; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1348: structure has no member
 * named `id'. */
struct RideSlot__s1930 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1930): tag struct RideWork as struct RideWork__s1930; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1339: structure has no member
 * named `value68'. */
struct RideWork__s1930 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1930 slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

DEFINE_RIDE_UPDATE_CARRIED(Func_02001ae0, RideWork__s1930, Func_02001a1c,
                           Func_02001a80)

/* VIEW(mars_lighthouse_entrance): routine gData_020056a0 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020056a0__mars_lighthouse_entrance[] __asm__("gData_020056a0");
#define gData_020056a0 gData_020056a0__mars_lighthouse_entrance

void Func_02001f90(struct Actor *object);

DEFINE_ACTOR_RESET_SCRIPTED_FOR(Func_02001c44, gData_020056a0)

DEFINE_RIDE_SPAWN_FRONT(Func_02001c74, Func_02004bd8_, Func_02001c44, 2,
                        Func_02001ae0, Func_02004bc0)

/* VIEW(s1c44): tag struct RideSlot as struct RideSlot__s1c44; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1384: structure has no member
 * named `id'. */
struct RideSlot__s1c44 {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1c44): tag struct RideWork as struct RideWork__s1c44; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1376: structure has no member
 * named `direction'. */
struct RideWork__s1c44 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1c44 slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

DEFINE_RIDE_UPDATE_LANDING(Func_02001cbc, RideWork__s1c44, Func_02001a1c,
                           Func_02001a80)

/* VIEW(mars_lighthouse_entrance): routine gData_020056a0 as this part declared
 * it; the joined file has another. */
extern const u8
    gData_020056a0__mars_lighthouse_entrance[] __asm__("gData_020056a0");
#define gData_020056a0 gData_020056a0__mars_lighthouse_entrance

void Func_02001f90(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020056a0. Overlays 6ab, 6ac and 6ad have the same function.
DEFINE_ACTOR_SETUP_SCRIPTED_PART(Func_02001e1c, gData_020056a0)

DEFINE_RIDE_SPAWN_BACK(Func_02001e60, Func_02004bd8_, Func_02001e1c,
                       Func_02001ae0, Func_02004bc0)

DEFINE_RIDE_SPAWN_FRONT(Func_02001ea8, Func_02004bd8_, Func_02001e1c, 3,
                        Func_02001cbc, Func_02004bc0)

DEFINE_RIDE_SPAWN_BACK(Func_02001ef0, Func_02004bd8_, Func_02001e1c,
                       Func_02001cbc, Func_02004bc0)

DEFINE_ACTOR_IS_ABOVE(Func_02001f38)

DEFINE_RIDE_UPDATE_DRIFT(Func_02001f90, Func_02001f38)

DEFINE_ACTOR_SETUP_SCRIPTED_FOR(Func_0200202c, Func_02004bc0, gData_020056a0)

DEFINE_RIDE_SPAWN_SPARK(Func_0200206c, Func_02004bd8_, Func_0200202c,
                        Func_02001f90)

/* VIEW(s20ec): tag struct Actor as struct Actor__s20ec; actor.h has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1463:
 * structure has no member named `flag5b'. */
typedef struct Actor__s20ec {
  u8 pad0[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[91 - 20];
  u8 flag5b;
} Actor;

typedef struct Entry {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk_6;
  Actor *actor;
  s32 unk_c;
} Entry;

typedef struct State {
  u8 pad0[6];
  s16 unk_6;
  s16 unk_8;
  u8 pada[4];
  s16 unk_e;
  void (*callback)();
  Entry entries[4];
} State;

/* VIEW(s20ec): routine Func_0200206c as void Func_0200206c(); the parts before
 * it has another declaration; tried as the one declaration void
 * Func_0200206c(), it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1470: too many arguments to
 * function `Func_0200206c'. */
extern void Func_0200206c__s20ec() __asm__("Func_0200206c");

typedef struct Position {
  s32 x;
  s32 z;
} Position;

typedef struct Camera {
  u8 pad0[228];
  Position position;
} Camera;

typedef struct IwramGlobals {
  u8 pad0[32];
  Camera *camera;
  u8 pad24[108 - 36];
  union {
    s32 word;
    void *pointer;
  } unk_6c;
} IwramGlobals;

#define IWRAM ((IwramGlobals *)Ram_HeapSlots)

DEFINE_BEAM_DRAW_TARGETS(Func_020020ec, Func_0200206c__s20ec, Func_02001e60,
                         Func_02001ef0, Func_02001ea8, Func_02001c74)
#undef IWRAM

#undef Func_0200248c
#undef Oam_AddSpriteAtDepthClamped
#undef Func_02002c2c
#undef Func_020022e4
// ---- scene/mars_lighthouse_entrance/s2270.c ----

/// Scene code of overlay 0x6aa, in ROM order.

void Func_02002270(void) {
  struct MarsLighthouseEntranceRideHead *work =
      (struct MarsLighthouseEntranceRideHead *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseEntranceAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02004bc0(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

void Func_020022c4(void) {
  struct RideWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}

void Func_020022d8(s16 value) { RIDE_WORK->value0e = value; }

void Func_020020ec(void);

DEFINE_BEAM_START(Func_020022e4, Func_020020ec)

s32 Func_02002418(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Func_02002428(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Func_02002438(void) { return RIDE_WORK->counter; }

s32 Func_02002444(s32 limit) {
  struct RideWork *work = RIDE_WORK;

  if (!Event_GetFlag(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

/* VIEW(s2270): routine Func_0200246c as void Func_0200246c(s32 index, s32
 * value); scene/shared/mars_lighthouse_entrance_room.h has s32
 * Func_0200246c(s32 index, s32 value); tried as the definition's void
 * Func_0200246c(s32 index, s32 value), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1547: conflicting types for
 * `Func_0200246c'. */
void Func_0200246c__s2270(s32 index, s32 value) __asm__("Func_0200246c");
#define Func_0200246c Func_0200246c__s2270
void Func_0200246c(s32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}

void Func_0200248c(s32 index, s16 value) {
  RIDE_WORK->entries[index].c = value;
}

s32 Func_0200249c(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

void Func_020024ac(u32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].value = value;
}

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

/* VIEW(s2270): routine Unit_Get as struct BattleUnit *Unit_Get(s32 owner);
 * item_icon.h has void *Unit_Get(s32 owner); tried as the definition's void
 * *Unit_Get(s32 owner), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1584: request for
 * member `hp' in something not a structure or union. */
struct BattleUnit *Unit_Get__s2270(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__s2270

DEFINE_PARTY_TAKE_FALL_DAMAGE(Func_020024c0)

#include "../field/field_effect/advance_spin.c"

DEFINE_ACTOR_BLINK(Func_02002614, 10, 4, 0)

#undef Func_020022e4
#undef Unit_Get
#undef Func_0200246c
// ---- scene/mars_lighthouse_entrance/s263c.c ----

/// Scene code of overlay 0x6aa, in ROM order.

void Func_020024c0();
void Func_02004bc0(struct Actor *actor, s32 a);
s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
/* VIEW(s263c): routine Func_02002614 as void Func_02002614(void);
 * scene/mars_lighthouse_entrance/s2270 has DEFINE_ACTOR_BLINK(Func_02002614,
 * 10, 4, 0) #undef Func_020022e4 #undef Unit_Get // ----
 * scene/mars_lighthouse_entrance/s263c.c ---- /// Scene code of overlay 0x6aa,
 * in ROM order. #include "shared/mars_lighthouse.h" #include "version.h"
 * #include "types.h" #include "call.h" #include "actor.h" #include
 * "main_routine_work.h" #include "ram_buffer.h" #include "vector3.h" #include
 * "fixed_math.h" #include "event_inline.h" #include "display_transition.h"
 * #include "event_work.h" #include "field.h" #include "item_icon.h" #include
 * "module/field/field.h" #include "module/scene/mars_lighthouse_entrance.h"
 * #include "module/system/system.h" #include "scene_ids.h" void
 * Func_020024c0(); tried as the rest of the object declares it, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1651:
 * conflicting types for `Func_02002614'. */
void Func_02002614__s263c(void) __asm__("Func_02002614");
#define Func_02002614 Func_02002614__s263c
extern u8 gInventorySnapshot[];

#define SHARED_F02002148_NAME Func_0200263c
#define SHARED_F02002148_ID2 Func_020024c0
#define SHARED_F02002148_ID3 Func_02002614
#define SHARED_F02002148_ID4 Func_02004bc0
#define SHARED_F02002148_ID5 MapGrid_GetCellAttributeLowNibbleOrBump
#define SHARED_F02002148_ID6 MapGrid_CheckCollision
#define SHARED_F02002148_ID7 Func_02004c78
#include "shared/func_02002148.c"
#undef SHARED_F02002148_NAME
#undef SHARED_F02002148_ID2
#undef SHARED_F02002148_ID3
#undef SHARED_F02002148_ID4
#undef SHARED_F02002148_ID5
#undef SHARED_F02002148_ID6
#undef SHARED_F02002148_ID7

void MarsLighthouseEntrance_ContinueSlide(void) {
  if (RIDE_WORK->active)
    Func_0200263c();
}

/* VIEW(s2860): routine Func_020024c0 as s32 Func_020024c0(s32 a, s32 b); s263c
 * has void Func_020024c0(); tried as the rest of the object declares it, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1711: too many arguments to
 * function `Func_020024c0'. */
extern s32 Func_020024c0__s2860(s32 a, s32 b) __asm__("Func_020024c0");
void Func_02002614(void);

struct RideEvent {
  u8 unk000[0x15a];
  u16 field15a;
  u16 field15c;
  u16 field15e;
  u8 unk160[4];
  u16 field164;
  u16 field166;
  u8 unk168[0x44];
  s32 field1ac;
  u8 unk1b0[2];
  u16 started;
  s32 field1b4;
};

struct RideCar {
  u8 unk00[0x64];
  s16 stopped;
  u8 unk66[0xa];
  s32 stepX;
  s32 unk74;
  s32 stepZ;
};

struct RideState {
  s32 counter;
  s16 active;
  u8 unk06[0x66];
  struct RideCar *car;
};

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
int MarsLighthouseEntrance_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  s32 frame[3];

  /* FAKEMATCH(signature): typed as returning int, though it returns nothing;
   * the return register then waits on the leader's copy into r0, which the
   * scheduler issues before the update hook is cleared; as void, 2 of 138
   * instructions differ in tla-en. */
  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Flag_Clear(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  /* FAKEMATCH(signature): Func_020024c0 declared returning s32, though it
   * returns nothing, so the -10 call sets r1 before negating r0; as void, 2 of
   * 138 instructions differ in tla-en. */
  Func_020024c0__s2860(-10, 0);
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Flag_Set(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(0x106);
  leader->update = Func_02002614;
  leader->facing = 0x4000;
  Func_02004bc0(leader, 49);
  while (ride->car->stopped == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z) != 7) {
      leader->x += ride->car->stepX;
      leader->z += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (ride->car->stepZ != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
}

s32 Func_020029a0(void) {
  if (RIDE_WORK->active) {
    MarsLighthouseEntrance_SlideOnIce();
    return 1;
  }
  return 0;
}

s32 Func_020029bc(void) { return RIDE_WORK->active; }

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 unk0a[2];
  s32 scale; ///< 16.16, as FieldSprite (field_sprite.h) has it
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 unk11 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

/* VIEW(s263c): routine gData_020056d0 as this part declared it; the joined file
 * has another. */
extern u8 gData_020056d0__s263c[] __asm__("gData_020056d0");
#define gData_020056d0 gData_020056d0__s263c

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_020029c8(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16 - b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + 0x40000;
    object = (struct Actor *)Import_ActorCreate(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16();
      radius = Random16() * 8;
      sum = Random16() + Random16();
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gData_020056d0);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->acceleration = 0;
      object->bounce = 0x8000;
      sprite = object->sprite;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;
      sprite->tile = source->tile;
    }
  }
}

/* VIEW(s263c): routine gData_02005714 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02005714__s263c[] __asm__("gData_02005714");
#define gData_02005714 gData_02005714__s263c

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Func_02002b4c, gData_02005714)

void Func_02002c2c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2860): tag struct Work020023c4 as struct Work020023c4__s2860; s263c.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1877: structure has no member
 * named `unk6c'. */
struct Work020023c4__s2860 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Func_02002c40, Work020023c4__s2860,
                              MapGrid_GetCellAttributeLowNibbleOrBump,
                              Func_02002b4c, Func_020029c8)
#undef WORK
#undef ABS

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
/* VIEW(s263c): routine gData_0200503c as this part declared it; the joined file
 * has another. */
extern const u8 gData_0200503c__s263c[] __asm__("gData_0200503c");
#define gData_0200503c gData_0200503c__s263c
/* VIEW(s263c): routine gData_0200547c as this part declared it; the joined file
 * has another. */
extern const u8 gData_0200547c__s263c[] __asm__("gData_0200547c");
#define gData_0200547c gData_0200547c__s263c
/* VIEW(s263c): routine gData_020054cc as this part declared it; the joined file
 * has another. */
extern const u8 gData_020054cc__s263c[] __asm__("gData_020054cc");
#define gData_020054cc gData_020054cc__s263c
/* VIEW(s263c): routine gData_02005528 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02005528__s263c[] __asm__("gData_02005528");
#define gData_02005528 gData_02005528__s263c
/* VIEW(s263c): routine gData_02005578 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02005578__s263c[] __asm__("gData_02005578");
#define gData_02005578 gData_02005578__s263c
/* VIEW(s263c): routine gData_020055d4 as this part declared it; the joined file
 * has another. */
extern const u8 gData_020055d4__s263c[] __asm__("gData_020055d4");
#define gData_020055d4 gData_020055d4__s263c
/* VIEW(s263c): routine gData_02005644 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02005644__s263c[] __asm__("gData_02005644");
#define gData_02005644 gData_02005644__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Triggers as this part declared
 * it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Triggers");
#define gMarsLighthouseEntrance_Triggers gMarsLighthouseEntrance_Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room1Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room1Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room1Triggers");
#define gMarsLighthouseEntrance_Room1Triggers                                  \
  gMarsLighthouseEntrance_Room1Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room2Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room2Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room2Triggers");
#define gMarsLighthouseEntrance_Room2Triggers                                  \
  gMarsLighthouseEntrance_Room2Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room3Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room3Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room3Triggers");
#define gMarsLighthouseEntrance_Room3Triggers                                  \
  gMarsLighthouseEntrance_Room3Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room4Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room4Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room4Triggers");
#define gMarsLighthouseEntrance_Room4Triggers                                  \
  gMarsLighthouseEntrance_Room4Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room5Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room5Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room5Triggers");
#define gMarsLighthouseEntrance_Room5Triggers                                  \
  gMarsLighthouseEntrance_Room5Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room6Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room6Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room6Triggers");
#define gMarsLighthouseEntrance_Room6Triggers                                  \
  gMarsLighthouseEntrance_Room6Triggers__s263c
/* VIEW(s263c): routine gMarsLighthouseEntrance_Room7Triggers as this part
 * declared it; the joined file has another. */
extern const u8 gMarsLighthouseEntrance_Room7Triggers__s263c[] __asm__(
    "gMarsLighthouseEntrance_Room7Triggers");
#define gMarsLighthouseEntrance_Room7Triggers                                  \
  gMarsLighthouseEntrance_Room7Triggers__s263c
/* VIEW(s263c): routine gData_02006868 as this part declared it; the joined file
 * has another. */
extern s16 gData_02006868__s263c __asm__("gData_02006868");
#define gData_02006868 gData_02006868__s263c
extern s16 gData_020068a0;
extern u16 gData_020068a2;

void TileMap_ConvertBuffer(s32 scene, s32 a1);

DEFINE_ACTOR_ATTACH(Func_02002f4c, Func_02004bc0, Func_02002c2c)

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_02002c40 as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
DEFINE_ACTOR_LAUNCH_AHEAD(Func_02002fc4, Func_02002c40)

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

DEFINE_ACTOR_POP_UP_SLOW(Func_0200308c, Func_02004bc0)

DEFINE_ACTOR_POP_UP(Func_02003138, Func_02004bc0)

DEFINE_ACTOR_PARK(Func_020031dc, Func_02002fc4)

/* VIEW(s263c): routine gData_02005758 as this part declared it; the joined file
 * has another. */
extern const s16 gData_02005758__s263c[] __asm__("gData_02005758");
#define gData_02005758 gData_02005758__s263c

DEFINE_ACTOR_STEER_BY_PAD(Func_020031f8, gData_02005758, Func_02004bc0)

DEFINE_LEADER_STOP_IF_PENDING(Func_02003284)

/* VIEW(s263c): tag struct Position as struct Position__s263c;
 * scene/mars_lighthouse_entrance/mars_lighthouse_entrance has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1955:
 * redefinition of `struct Position'. */
#define Position Position__s263c
/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_02004bc0(struct Actor *actor, s32 mode);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
/* VIEW(s263c): routine gData_02005798 as this part declared it; the joined file
 * has another. */
extern s16 gData_02005798__s263c[] __asm__("gData_02005798");
#define gData_02005798 gData_02005798__s263c

DEFINE_LEADER_SLIDE_ON_ICE(Func_020032b8, Func_02003284, gData_02005798,
                           Func_02004bc0, Func_020031f8, Func_02004c78)

extern volatile s16 gData_020068b0;
/* VIEW(s263c): routine gData_02005798 as this part declared it; the joined file
 * has another. */
extern s16 gData_02005798__s263c[] __asm__("gData_02005798");
#define gData_02005798 gData_02005798__s263c
void Func_020031f8(struct Actor *object);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004bc0(struct Actor *object, s32 mode);

#define SHARED_F02003070_NAME Func_02003564
#define SHARED_F02003070_ID2 gData_020068b0
#define SHARED_F02003070_ID3 Func_02003284
#define SHARED_F02003070_ID4 gData_02005798
#define SHARED_F02003070_ID5 Func_02004bc0
#define SHARED_F02003070_ID6 Actor_HasArrived
#define SHARED_F02003070_ID7 Func_020031f8
#define SHARED_F02003070_ID8 Func_02004c78
#include "shared/func_02003070.c"
#undef SHARED_F02003070_NAME
#undef SHARED_F02003070_ID2
#undef SHARED_F02003070_ID3
#undef SHARED_F02003070_ID4
#undef SHARED_F02003070_ID5
#undef SHARED_F02003070_ID6
#undef SHARED_F02003070_ID7
#undef SHARED_F02003070_ID8

void Func_02003878(s32 value) { gData_020068b0 = value; }

#undef Func_02002614
#undef Position
// ---- scene/mars_lighthouse_entrance/s3884.c ----

/// Scene code of overlay 0x6aa, in ROM order.

/* VIEW(s3884): tag struct Position as struct Position__s3884;
 * scene/mars_lighthouse_entrance/mars_lighthouse_entrance has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:2016:
 * redefinition of `struct Position'. */
#define Position Position__s3884
/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Func_02003884, Func_02003284, Func_02004bc0,
                                 Func_020031f8, Func_02004c78)

#include "../field/object/set_sprite_priority_b.c"

#undef Position
#include "../field/field_effect/move.c"

#undef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#undef RIDE_WORK
#undef DEFINE_FUNC_0200085C
#undef Oam_AddSpriteAtDepthClamped
#undef SET_SPRITE_WINDOW_LAYERS
#undef FIELD_AT
#undef IWRAM
#undef SHARED_F02002148_NAME
#undef SHARED_F02002148_ID2
#undef SHARED_F02002148_ID3
#undef SHARED_F02002148_ID4
#undef SHARED_F02002148_ID5
#undef SHARED_F02002148_ID6
#undef SHARED_F02002148_ID7
#undef WORK
#undef ABS
#undef SHARED_F02003070_NAME
#undef SHARED_F02003070_ID2
#undef SHARED_F02003070_ID3
#undef SHARED_F02003070_ID4
#undef SHARED_F02003070_ID5
#undef SHARED_F02003070_ID6
#undef SHARED_F02003070_ID7
#undef SHARED_F02003070_ID8
#undef gMarsLighthouseEntrance_Placements
#undef gMarsLighthouseEntrance_Room1Placements
#undef gMarsLighthouseEntrance_Room2Placements
#undef gMarsLighthouseEntrance_Room3Placements
#undef gMarsLighthouseEntrance_Room4Placements
#undef gMarsLighthouseEntrance_Room5Placements
#undef gMarsLighthouseEntrance_Room6Placements
#undef gMarsLighthouseEntrance_Room7Placements
#undef Func_02003b30
#undef gData_02005394
#undef gData_0200503c
#undef gData_0200547c
#undef gData_020054cc
#undef gData_02005528
#undef gData_02005578
#undef gData_020055d4
#undef gData_02005644
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef gData_02006868
#undef Func_02002c2c
#undef gData_02006868
#undef gData_0200503c
#undef gData_0200547c
#undef gData_020054cc
#undef gData_02005528
#undef gData_02005578
#undef gData_020055d4
#undef gData_02005644
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef gData_02006868
#undef Func_020022e4
#undef Func_0200248c
#undef gData_02005314
#undef gData_020053d8
#undef gData_020053e6
#undef gData_020053f8
#undef gData_020053fe
#undef gData_0200540c
#undef gData_02005412
#undef gData_02005418
#undef gData_02005438
#undef gData_0200543e
#undef gData_02005448
#undef gData_0200544e
#undef gData_02005454
#undef gData_0200586c
#undef gData_0200587c
#undef gData_020058a8
#undef gData_020058d4
#undef gData_0200503c
#undef gData_0200547c
#undef gData_020054cc
#undef gData_02005528
#undef gData_02005578
#undef gData_020055d4
#undef gData_02005644
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef gData_02006868
#undef gData_020056a0
#undef gData_020056a0
#undef gData_020056a0
#undef Func_0200246c
#undef Unit_Get
#undef Func_02002614
#undef gData_020056d0
#undef gData_02005714
#undef gData_0200503c
#undef gData_0200547c
#undef gData_020054cc
#undef gData_02005528
#undef gData_02005578
#undef gData_020055d4
#undef gData_02005644
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef gData_02006868
#undef gData_02005758
#undef Position
#undef gData_02005798
#undef gData_02005798
#undef Position
#undef Func_020045f8
// @file
// Scene code of overlay 0x6aa, in ROM order.

/* VIEW(s3b30): routine gData_0200686c as this part declared it; the joined file
 * has another. */
extern const s32 *gData_0200686c__s3b30[] __asm__("gData_0200686c");
#define gData_0200686c gData_0200686c__s3b30

/* VIEW(s3b30): routine Actor_StartScript as this part declared it; the joined
 * file has another. */
void Actor_StartScript__s3b30(struct FieldEffect *effect,
                              const s32 *script) __asm__("Actor_StartScript");
#define Actor_StartScript Actor_StartScript__s3b30
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
/* VIEW(s3b30): routine Actor_SetSpritePriorityB as this part declared it; the
 * joined file has another. */
void Actor_SetSpritePriorityB__s3b30(
    struct FieldEffect *effect,
    s32 priority) __asm__("Actor_SetSpritePriorityB");
#define Actor_SetSpritePriorityB Actor_SetSpritePriorityB__s3b30

void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02004bc0(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_0200686c[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Actor_SetSpritePriorityB(effect,
                           ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Actor_SetSpritePriorityB(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_0200686c[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_02004bc0(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE Data_020068b4
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(Func_02003e24, Data_020068b4, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(Func_020040f0, Leader_CenterInCell)

void Func_02004160(struct PushEvent *event) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x;
  s32 z;
  s32 mask;

  mask = 64;
  x = object->x >> 20;
  do {
  } while (0);
  x |= (s16)(event->x & mask);
  z = object->z >> 20;
  z |= (s16)(event->z & mask);
  Leader_CenterInCell();
  Event_PlaySound(229);
  MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {
    u32 zero = 0;

    object->facing = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  {
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Event_PlaySound(204);
    object->scaleX -= 0x400;
    object->scaleY -= 0x300;
    object->y -= 0x9999;
    System_Wait(1);
  }
  do {
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef gData_0200686c
#undef Actor_StartScript
#undef Actor_SetSpritePriorityB
// @file
// Scene code of overlay 0x6aa, in ROM order.

void Teleport_PlayWhirl(void);

/* VIEW(s42f8): routine Func_02003b30 as this part declared it; the joined file
 * has another. */
void Func_02003b30__s42f8(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("Func_02003b30");
#define Func_02003b30 Func_02003b30__s42f8

void Func_020042f8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  SCENE_WORK->unk1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
next:
  {
    s32 angle = i << 12;

    velocity.x = Trig_Cos(angle);
    velocity.y = 0;
    velocity.z = Math_Sin(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16() * 3 << 13) >> 16) - 0x3000;
    Func_02003b30(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                  velocity.z, 0x1090001, &options);
  }
  if (++i <= 16)
    goto next;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s42f8): routine Func_02004160 as this part declared it; the joined file
 * has another. */
void Func_02004160__s42f8(
    struct CellTarget *work,
    struct CellTargetEntry *entry) __asm__("Func_02004160");
#define Func_02004160 Func_02004160__s42f8

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, Data_020068b4, Func_020040f0,
                          Func_02004160)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, Data_020068b4,
                                  Func_020042f8)

extern u16 *gData_0200e88c;
extern u16 *gData_02006890;

// Records the four palette lists, writes each listed palette entry from its
// red, green and blue, then copies two palettes from block32 and starts the
// blend.
DEFINE_PALETTE_SET_COLORS_FADE(Func_020045f8, gData_0200e88c, gData_02006890,
                               Data_02006894, Data_02006880, Func_020048e8)

extern u16 gData_02006898;

DEFINE_PALETTE_CYCLE_START(Func_020046b4, gData_0200689c, gData_02006898,
                           Func_02004794, gData_0200e88c, gData_0200e888, 15,
                           gData_02006884, 16, gData_0200687c, Func_020047b8)

void Func_02004718(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_020047b8);
}

DEFINE_PALETTE_CYCLE_START(Func_02004738, gData_0200689c, gData_02006898,
                           Func_02004794, gData_0200e88c, gData_0200e888, 2,
                           gData_02006884, 2, gData_0200687c, Func_020047b8)

DEFINE_LIST_COUNT_ENTRIES(Func_02004794)

DEFINE_PALETTE_CYCLE_FADE(Func_020047b8, gData_02006884, gData_0200e888,
                          Data_02006894, gData_02006898, Data_02006880,
                          gData_0200689c, gData_0200e88c, gData_0200687c,
                          gData_02006890)

void Func_020048e8(void) {
  struct MarsLighthouseEntranceEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct MarsLighthouseEntranceEventWork *)Ram_HeapSlots->eventWork)
      ->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004738();
  Teleport_PlayWhirl();
  System_Wait(40);
  Func_02004718();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

DEFINE_ACTORS_SET_CELLS_OPEN(Actors_SetCellsOpen, Func_02004a60)

// Finds the current target's table row and shrinks it.
void Func_020049dc(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  /* FAKEMATCH(dead): the entry jump in a once-only block; as the plain
   * goto, 5 of 61 instructions differ in tla-en. */
  do {
    goto test;
  } while (0);
  do {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Func_02004a60(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}

DEFINE_ACTOR_SET_CELL_OPEN(Func_02004a60, Actor_GetCellIndex, Func_02004bc0)

/* VIEW(s42f8): routine Func_02000038 as this part declared it; the joined file
 * has another. */
void Func_02000038__s42f8(void) __asm__("Func_02000038");
#define Func_02000038 Func_02000038__s42f8
/* VIEW(s42f8): routine Func_02000070 as this part declared it; the joined file
 * has another. */
void Func_02000070__s42f8(void) __asm__("Func_02000070");
#define Func_02000070 Func_02000070__s42f8
/* VIEW(s42f8): routine Func_0200308c as this part declared it; the joined file
 * has another. */
void Func_0200308c__s42f8(void) __asm__("Func_0200308c");
#define Func_0200308c Func_0200308c__s42f8
/* VIEW(s42f8): routine Func_02003138 as this part declared it; the joined file
 * has another. */
void Func_02003138__s42f8(void) __asm__("Func_02003138");
#define Func_02003138 Func_02003138__s42f8
/* VIEW(s42f8): routine Func_020031dc as this part declared it; the joined file
 * has another. */
void Func_020031dc__s42f8(void) __asm__("Func_020031dc");
#define Func_020031dc Func_020031dc__s42f8

// clang-format off
DEFINE_G_DATA_02004DE8(gData_02004de8, 200, 104)

DEFINE_G_DATA_02004DE8(gData_02004e50, 232, 296)

DEFINE_G_DATA_02004EB8(gData_02004eb8, 48, 904, 528, 840)

u32 gData_02004f44[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_WAIT(10),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(30),
    SC_LABEL(1),
    SC_MOVE(PIXELS(152), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

DEFINE_G_DATA_02004FF0(gData_02004ff0, 0)

s32 gData_0200503c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(808), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(808), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(60),
    SC_CALL(Func_02000070),
    SC_END,
};

u32 gData_02005314[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, PIXELS(56)),
    SC_SET(SW_Z, PIXELS(128)),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

u32 gData_02005394[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_020053d8[] = {
    8, 7, 512, 9, 7, 513, -1,
};

u16 gData_020053e6[] = {
    9, 3, 10, 3, 11, 3, 12, 3,
    0,
};

u16 gData_020053f8[] = {
    8, 0, 0,
};

s16 gData_020053fe[] = {
    16, 7, 514, 17, 7, 515, -1,
};

u16 gData_0200540c[] = {
    8, 3, 0,
};

u16 gData_02005412[] = {
    9, 1, 0,
};

s16 gData_02005418[] = {
    12, 7, 516, 13, 7, 517, 14, 7,
    518, 15, 7, 519, 16, 7, 520, -1,
};

u16 gData_02005438[] = {
    9, 3, 0,
};

u16 gData_0200543e[] = {
    8, 0, 10, 1, 0,
};

u16 gData_02005448[] = {
    11, 67, 0,
};

u16 gData_0200544e[] = {
    12, 1, 0,
};

s16 gData_02005454[] = {
    13, 7, 528, 14, 7, 529, 15, 7,
    530, 16, 7, 531, 17, 7, 532, 18,
    7, 533, -1, 0,
};

DEFINE_G_DATA_0200547C(gData_0200547c, Func_0200308c, 112, 728, 22, 128, 728,
    Func_020031dc)

DEFINE_G_DATA_020054CC(gData_020054cc, Func_0200308c, 0x500000, 0x200000,
    0xa80000, 0x160000, 0x600000, 0xa80000, Func_02000c68, Func_020031dc)

DEFINE_G_DATA_0200547C(gData_02005528, Func_0200308c, 872, 576, 30, 872, 584,
    Func_020031dc)

DEFINE_G_DATA_020054CC(gData_02005578, Func_02003138, 0x600000, 0x200000,
    0x2a80000, 0x160000, 0x700000, 0x2a80000, Func_02000a24, Func_020031dc)

s32 gData_020055d4[] = {
    46,
    (s32)Func_02003138,
    4,
    0x3580000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x980000,
    1,
    22,
    37,
    (s32)Func_02000c1c,
    46,
    (s32)Func_020031dc,
    17,
};

DEFINE_G_DATA_020054CC(gData_02005644, Func_02003138, 0x700000, 0x200000,
    0x2a80000, 0x160000, 0x800000, 0x2a80000, Func_02000808, Func_020031dc)

DEFINE_G_DATA_02001540(gData_020056a0, 30)

u32 gData_020056d0[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005714[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_02005758[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000, 0xffff, -0x8000, -0x4000, (s32)0xc000ffff,
    -0xc000, 0x4000ffff, 0xffff, -0x8000,
};

s32 gData_02005798[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gData_020057b8, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gData_020057f4, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gData_02005830, 126)

s16 gData_0200586c[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gData_0200587c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020058a8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020058d4[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gData_0200591c[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(279),
    SCENE_EXIT(1, 2, 84),
    SCENE_EXIT(2, 281, 2),
    SCENE_EXITS(280),
    SCENE_EXIT(1, 281, 1),
    SCENE_EXIT(2, 287, 2),
    SCENE_EXIT(3, 280, 4),
    SCENE_EXIT(4, 280, 3),
    SCENE_EXIT(5, 282, 5),
    SCENE_EXIT(6, 282, 6),
    SCENE_EXIT(7, 280, 8),
    SCENE_EXIT(8, 280, 7),
    SCENE_EXIT(9, 280, 10),
    SCENE_EXIT(10, 280, 9),
    SCENE_EXIT(20, 281, 20),
    SCENE_EXITS(281),
    SCENE_EXIT(1, 280, 1),
    SCENE_EXIT(2, 279, 2),
    SCENE_EXIT(3, 282, 3),
    SCENE_EXIT(4, 282, 4),
    SCENE_EXIT(5, 283, 5),
    SCENE_EXIT(6, 281, 7),
    SCENE_EXIT(7, 281, 6),
    SCENE_EXIT(8, 281, 9),
    SCENE_EXIT(9, 281, 8),
    SCENE_EXIT(10, 281, 11),
    SCENE_EXIT(11, 281, 10),
    SCENE_EXIT(12, 281, 13),
    SCENE_EXIT(13, 281, 12),
    SCENE_EXIT(14, 281, 15),
    SCENE_EXIT(15, 281, 14),
    SCENE_EXIT(16, 282, 16),
    SCENE_EXIT(17, 281, 18),
    SCENE_EXIT(18, 281, 17),
    SCENE_EXIT(20, 280, 3),
    SCENE_EXITS(282),
    SCENE_EXIT(1, 283, 1),
    SCENE_EXIT(2, 283, 2),
    SCENE_EXIT(3, 281, 3),
    SCENE_EXIT(4, 281, 4),
    SCENE_EXIT(5, 280, 5),
    SCENE_EXIT(6, 280, 6),
    SCENE_EXIT(7, 284, 7),
    SCENE_EXIT(8, 282, 9),
    SCENE_EXIT(9, 282, 8),
    SCENE_EXIT(10, 282, 11),
    SCENE_EXIT(11, 282, 10),
    SCENE_EXIT(14, 283, 14),
    SCENE_EXIT(16, 281, 16),
    SCENE_EXITS(283),
    SCENE_EXIT(1, 282, 1),
    SCENE_EXIT(2, 282, 2),
    SCENE_EXIT(3, 283, 4),
    SCENE_EXIT(4, 283, 3),
    SCENE_EXIT(5, 281, 5),
    SCENE_EXIT(6, 283, 7),
    SCENE_EXIT(7, 283, 6),
    SCENE_EXIT(8, 283, 9),
    SCENE_EXIT(9, 283, 8),
    SCENE_EXIT(10, 283, 11),
    SCENE_EXIT(11, 283, 10),
    SCENE_EXIT(12, 283, 13),
    SCENE_EXIT(13, 283, 12),
    SCENE_EXIT(14, 282, 14),
    SCENE_EXITS(284),
    SCENE_EXIT(1, 285, 1),
    SCENE_EXIT(2, 284, 3),
    SCENE_EXIT(3, 284, 2),
    SCENE_EXIT(4, 284, 5),
    SCENE_EXIT(5, 284, 4),
    SCENE_EXIT(6, 286, 6),
    SCENE_EXIT(7, 282, 7),
    SCENE_EXIT(8, 286, 8),
    SCENE_EXIT(9, 286, 9),
    SCENE_EXIT(10, 284, 11),
    SCENE_EXIT(11, 284, 10),
    SCENE_EXIT(12, 284, 13),
    SCENE_EXIT(13, 284, 12),
    SCENE_EXIT(14, 284, 15),
    SCENE_EXIT(15, 284, 14),
    SCENE_EXIT(16, 286, 10),
    SCENE_EXITS(285),
    SCENE_EXIT(1, 284, 1),
    SCENE_EXIT(2, 285, 3),
    SCENE_EXIT(3, 285, 2),
    SCENE_EXIT(4, 285, 6),
    SCENE_EXIT(5, 286, 5),
    SCENE_EXIT(6, 285, 4),
    SCENE_EXIT(7, 286, 7),
    SCENE_EXIT(11, 286, 11),
    SCENE_EXITS(286),
    SCENE_EXIT(1, 286, 2),
    SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4),
    SCENE_EXIT(4, 286, 3),
    SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6),
    SCENE_EXIT(7, 285, 7),
    SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

u32 gData_02005ae8[] = {
    39, 7, 17,
};

struct ActorPlacement gMarsLighthouseEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseEntrance_Room1Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(280), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(280), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gMarsLighthouseEntrance_Room2Placements[] = {
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(136), 0, PIXELS(424),
     FACING_SOUTH, 2, 0},
    {489, 800, 1, PIXELS(648), 0, PIXELS(376), FACING_SOUTH, 2, 0},
#if !TLA_PATCH_1
    {245, 105, 1, PIXELS(664), 0, PIXELS(424), FACING_SOUTH, 0, 0},
#else
    {245, 105, 1, PIXELS(728), 0, PIXELS(408), FACING_NORTHWEST, 0, 0},
#endif
    {SCENE_TABLE_END},
};

void Func_02000044(void);
void Func_02000050(void);
void MarsLighthouseEntrance_GrindIce(void);
void Func_02000648(void);
/* VIEW(s42f8): routine MarsLighthouseEntrance_BurstIceSheet as this part
 * declared it; the joined file has another. */
void MarsLighthouseEntrance_BurstIceSheet__s42f8(void) __asm__(
    "MarsLighthouseEntrance_BurstIceSheet");
#define MarsLighthouseEntrance_BurstIceSheet                                   \
  MarsLighthouseEntrance_BurstIceSheet__s42f8
void Func_02000718(void);
/* VIEW(s42f8): routine Func_0200085c as this part declared it; the joined file
 * has another. */
void Func_0200085c__s42f8(void) __asm__("Func_0200085c");
#define Func_0200085c Func_0200085c__s42f8
/* VIEW(s42f8): routine MarsLighthouseEntrance_PushStatueIntoFlames as this part
 * declared it; the joined file has another. */
void MarsLighthouseEntrance_PushStatueIntoFlames__s42f8(void) __asm__(
    "MarsLighthouseEntrance_PushStatueIntoFlames");
#define MarsLighthouseEntrance_PushStatueIntoFlames                            \
  MarsLighthouseEntrance_PushStatueIntoFlames__s42f8
/* VIEW(s42f8): routine Func_0200096c as this part declared it; the joined file
 * has another. */
void Func_0200096c__s42f8(void) __asm__("Func_0200096c");
#define Func_0200096c Func_0200096c__s42f8
/* VIEW(s42f8): routine Func_02000b84 as this part declared it; the joined file
 * has another. */
void Func_02000b84__s42f8(void) __asm__("Func_02000b84");
#define Func_02000b84 Func_02000b84__s42f8
/* VIEW(s42f8): routine Func_02000ce4 as this part declared it; the joined file
 * has another. */
void Func_02000ce4__s42f8(void) __asm__("Func_02000ce4");
#define Func_02000ce4 Func_02000ce4__s42f8
/* VIEW(s42f8): routine Func_02000e3c as this part declared it; the joined file
 * has another. */
void Func_02000e3c__s42f8(void) __asm__("Func_02000e3c");
#define Func_02000e3c Func_02000e3c__s42f8
void Func_0200112c(void);
void Func_02001138(void);

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room3Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004de8, PIXELS(104), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004e50, PIXELS(280), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(552), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(552), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(840), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(480), 0, PIXELS(704), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room4Placements[] = {
    {414, CONDITION_ALWAYS, (s32)gData_02004eb8, PIXELS(840), 0, PIXELS(528), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(872), 0, PIXELS(568), FACING_SOUTH, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(792), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(792), FACING_WEST, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(840), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(904), 0, PIXELS(744), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(840), 0, PIXELS(808), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(936), 0, PIXELS(824), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(904), 0, PIXELS(872), FACING_SOUTH, 2, 1},
    {244, 0x54, 1, PIXELS(88), 0, PIXELS(904), FACING_SOUTH, 0, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room5Placements[] = {
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(72), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(72), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(904), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(856), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(240), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room6Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004f44, PIXELS(120), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004ff0, PIXELS(792), 0, PIXELS(128), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(56), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(200), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(328), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(360), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(424), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(664), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(760), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room7Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(152), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(152), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gMarsLighthouseEntrance_Room2Triggers[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {0x8400, 11, CONDITION_ALWAYS, EVENT(Func_02000044)},
    {0xc400, 11, CONDITION_ALWAYS, EVENT(Func_02000044)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 10, 0xa3e,
     EVENT(MarsLighthouseEntrance_GrindIce)},
    {TRIGGER_TALK, 10, 0xa3e, EVENT(MarsLighthouseEntrance_CheckIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room3Triggers[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x51, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {0x51, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x31, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Func_0200263c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x202,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x203,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 17, CONDITION_ALWAYS, EVENT(Func_02000648)},
    {0x9c05, 18, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_Teleport)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa4f,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {9, 0, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80, 0x1230,
     EVENT(MarsLighthouseEntrance_BurstIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 81, 0x1231,
     EVENT(Func_0200085c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, 0},
    {0x60009a15, 13, CONDITION_ALWAYS, EVENT(Func_02000710)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 13,
     CONDITION_ALWAYS, EVENT(Func_02000718)},
    {TRIGGER_TALK, 64, CONDITION_ALWAYS, EVENT(Func_02000930)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x51, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x51, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x21, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020032b8)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Func_02000050)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Func_0200096c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x204,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x205,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x206,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x207,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x208,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 14, CONDITION_ALWAYS, EVENT(Func_0200093c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room5Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020032b8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, 0},
    {0x60009a15, 9, CONDITION_ALWAYS, EVENT(Func_020009c0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9,
     CONDITION_ALWAYS, EVENT(Func_020009e8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, 0},
    {0x60009a15, 11, CONDITION_ALWAYS, EVENT(Func_02000a74)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11,
     CONDITION_ALWAYS, EVENT(Func_02000a7c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80, 0x1232,
     EVENT(Func_02000b84)},
    {0x9c05, 14, CONDITION_ALWAYS, EVENT(Func_02000af4)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_02000b24)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_02000b54)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x51, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 35, 0x234, EVENT(Func_02000fa4)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02000fd4)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(Func_020029a0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x210,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x211,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x212,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x213,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x214,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, 0x215,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 12, CONDITION_ALWAYS, EVENT(Func_02000c84)},
    {0x9c05, 13, CONDITION_ALWAYS, EVENT(Func_02000cb4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa4e, EVENT(Func_02000ce4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room7Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020032b8)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02001138)},
    {0x202, 35, CONDITION_ALWAYS, EVENT(Func_0200112c)},
    {TRIGGER_TABLE_END},
};

u32 gData_02006868[] = {
    0,
};

s32 gData_0200686c[] = {
    (s32)gData_020057b8,
    (s32)gData_020057f4,
    (s32)gData_02005830,
};
#undef Func_02003b30
#undef Func_02004160
#undef Func_020045f8
#undef Func_02000038
#undef Func_02000070
#undef Func_0200308c
#undef Func_02003138
#undef Func_020031dc
#undef MarsLighthouseEntrance_BurstIceSheet
#undef Func_0200085c
#undef MarsLighthouseEntrance_PushStatueIntoFlames
#undef Func_0200096c
#undef Func_02000b84
#undef Func_02000ce4
#undef Func_02000e3c
