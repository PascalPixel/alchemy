/// @file
/// The scene code of overlay 0x6ad, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/mars_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_room_f.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_ids.h"

void Actor_DisableById(s32 id);
void FieldEffect_StartBlankOverlay(s32 a0);
void Tightrope_Initialize(void);
void Tightrope_Place(s32 a0, s32 a1, s32 a2);
void Actor_DepartInRing(s32 id);
void Actor_ArriveInRing(s32 id);
void TileMap_ConvertBuffer(s32 scene, s32 a1);
extern u16 gData_02007088[];
extern u16 gData_0200708c[];
void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();
extern s32 Data_02007064;
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "mars_lighthouse_room_f.h"

extern const u8 gMarsLighthouseRoomF_Placements[];
extern const u8 gMarsLighthouseRoomF_Room4Placements[];
extern const u8 gMarsLighthouseRoomF_Room1Placements[];
extern const u8 gMarsLighthouseRoomF_Room3Placements[];
extern const u8 gMarsLighthouseRoomF_Room2Placements[];

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom4)
    return gMarsLighthouseRoomF_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom2)
    return gMarsLighthouseRoomF_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom1)
    return gMarsLighthouseRoomF_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom3)
    return gMarsLighthouseRoomF_Room3Placements;
  return gMarsLighthouseRoomF_Placements;
}

void Func_020000bc(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_020000d0(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u32 facing = object->facing;

  if (facing == 0 || facing == 0x8000) {
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (facing != 0)
      x--;
    else
      x++;
    object = Actor_Get(10);
    if (object->x >> 20 != x || object->z >> 20 != z) {
      object = Actor_Get(11);
      if (object->x >> 20 != x || object->z >> 20 != z) {
        object = Actor_Get(12);
        if (object->x >> 20 != x || object->z >> 20 != z) {
          Field_TryJumpForward();
          return;
        }
      }
    }
  }
  if (Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner) != -1)
    Leader_PushObjectAhead();
}

void Func_02000168(void) { Field_TryJumpForward(); }

#include "version.h"

#if TLA_PATCH_1

void Func_02000170(void) {
  s32 position[3];
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = (object->y + 0x40000) & 0xfff80000;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  object = Actor_FindAtCell(position, 1);
  if (object != NULL) {
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    FieldVector_AddPolarOffset(0x100000, angle, position);
    if (Actor_FindAtCell(position, 1) != NULL)
      return;
  }
  Leader_PushObjectAhead();
}

#else

void Func_02000170(void) { Leader_PushObjectAhead(); }

#endif

extern s16 gData_02005b5c[];

void Func_020001f4(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 50) {
    Event_SetFlag(0xa38);
    object->motionFlags = 3;
  }
}

void Func_0200021c(void) {
  struct Actor *object = Actor_Get(16);

  Leader_PushObjectAhead();
  if (object->x >> 20 == 50) {
    Event_SetFlag(0xa38);
    object->motionFlags = 3;
  }
}

void Func_02000248(s32 unused, s32 id) {
  if (id == 10)
    Actor_Get(10)->motionFlags = 0;
}

void Func_0200025c(s32 unused, s32 id) {
  if (id == 16) {
    Func_020001f4(unused, 16);
  } else if (id == 10) {
    Actor_Get(10)->motionFlags = 3;
#if TLA_PATCH_1 /* From patch 1. */
    Func_020002ec();
#endif
  }
}

void Func_02000280(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 kind;

  kind = gData_02005b5c[id - 18];
  Actor_SetPosition(kind, S16_AT(object, 100) << 16,
                    (S16_AT(object, 102) << 16) - PIXELS(8));
}

void Func_020002b8(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  const s16 *table;
  s32 offset;
  s32 x;
  s32 kind;

  table = gData_02005b5c;
  x = object->x;
  offset = (id - 18) * 2;
  kind = *(s16 *)((u8 *)table + offset);
  if (x >> 20 == object->timer >> 4)
    Actor_SetPosition(kind, 0, 0);
}

#if TLA_PATCH_1 /* From patch 1. */
void Func_020002ec(void) {
  struct Actor *object;
  s32 id;
  s32 x;
  s32 z;

  MapGrid_CopyCellAttributes(53, 72, 1, 1, 38, 72);
  MapGrid_CopyCellAttributes(53, 72, 1, 1, 43, 72);
  MapGrid_CopyCellAttributes(53, 8, 1, 1, 47, 8);
  for (id = 10; id <= 12; id++) {
    object = Actor_Get(id);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 38 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 38, 72);
  }
  for (id = 10; id <= 12; id++) {
    object = Actor_Get(id);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 43 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 43, 72);
  }
  object = Actor_Get(10);
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 47 && z == 8)
    MapGrid_CopyCellAttributes(54, 8, 1, 1, 47, 8);
}
#endif

void Func_020003b4(void) {
  if (Actor_Get(15)->z >> 20 == 14) {
    Func_020027ac(0, 65);
    Func_020027ac(2, 19);
  } else {
    Func_020027ac(0, 17);
    Func_020027ac(2, 67);
  }
  if (Actor_Get(16)->x >> 20 == 6) {
    Func_020027ac(1, 65);
    Func_020027ac(3, 3);
  } else {
    Func_020027ac(1, 49);
    Func_020027ac(3, 67);
  }
}

void Func_02000418(void) {
  struct PushTarget box;

  if (PushTarget_FindSlide(&box)) {
    Func_02004d10(box);
    Func_020003b4();
  }
}

#define DEFINE_FUNC_02000444(name, n1, n2, n3)                                 \
  void name(s32 state, s32 unused) {                                           \
    if (state == 1) {                                                          \
      MapGrid_CopyCells(29, 56, 3, 4, n1, 50);                                 \
      MapGrid_CopyCellsLayered(93, 56, 3, 4, n2, 50);                          \
      Event_SetFlag(n3);                                                       \
    }                                                                          \
  }

DEFINE_FUNC_02000444(Func_02000444, 10, 74, 0xa44)

DEFINE_FUNC_02000444(Func_02000480, 18, 82, 0xa45)

DEFINE_FUNC_02000444(Func_020004bc, 14, 78, 0xa46)

void Func_020004f8(void) {
  Event_PlaySound(0x248);
  Func_020027ac(0, 3);
  Event_SetFlag(0x220);
}

void Func_02000514(void) {
  struct Actor *object = Actor_Get(9);

  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(9, S16_AT(object, 100) << 16, S16_AT(object, 102) << 16);
  Event_SetFlag(0xa42);
  Event_PlaySound(136);
}

void Func_02000554(void) { Func_02001448(); }

#include "iwram_call.h"

extern const u32 gData_02005b68[];
extern const u32 gData_02005bac[];

void Func_0200055c(s32 id) {
  struct Actor *origin = Actor_Get(id);
  s32 i;

  for (i = 11; i >= 0; i--) {
    s32 a = Random16();
    s32 x = origin->x + (a << 4) - (Random16() << 4);
    s32 b = Random16();
    s32 c = Random16();
    struct Actor *object =
        FieldActor_Create(0x149, x, (b << 3) + origin->y, (c << 4) + origin->z);

    if (object != NULL) {
      s32 angle = (Random16() >> 2) + 0xe000;
      s32 speed = Random16() << 3;
      s32 scale = (Random16() >> 1) + 0x4000;

      {
        u32 pick = Random16();

        if (pick <= 0x7fff)
          Actor_StartScript(object, gData_02005b68);
        else
          Actor_StartScript(object, gData_02005bac);
      }
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = (angle << 2) + 0x20000;
      object->velocityZ = Iwram_MulQ16(speed, Trig_Cos(angle));
      {
        s32 vx = Iwram_MulQ16(speed, Math_Sin(angle));

        object->acceleration = 0;
        object->scaleX = scale;
        object->scaleY = scale;
        object->gravity = 0x547a;
        object->velocityX = vx;
        object->bounce = 0x8000;
      }
      Actor_SetSpriteColorMode(object, 1);
    }
  }
}

void Func_02000670(void) {
  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk166 = 0;
  Event_ClearFlag(0x106);
  Event_ClearFlag(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(712), 1);
  FieldCamera_WaitForArrival();
  System_Wait(20);
  FieldCamera_SetSpeed(0x50000, 0xa000);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(872), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void Func_020006f4(s32 burst) {
  if (!Flag_IsSet(0xa46)) {
    MapGrid_CopyCells(29, 60, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 60, 3, 4, 78, 50);
    if (burst && !Flag_IsSet(0xa43))
      Func_0200055c(14);
    Flag_Set(0xa43);
  }
}

void Func_02000754(void) { Import_EventSetExitRequest(2); }

void Func_02000760(void) { Import_EventSetExitRequest(3); }

void Func_0200076c(void) { Import_EventSetExitRequest(4); }

void Func_02000778(void) { Import_EventSetExitRequest(5); }

void Func_02000784(void) { Import_EventSetExitRequest(6); }

void Func_02000790(void) { Import_EventSetExitRequest(7); }

void Func_0200079c(void) { Import_EventSetExitRequest(8); }

void Func_020007a8(void) { Import_EventSetExitRequest(9); }

void Func_020007b4(void) { Import_EventSetExitRequest(10); }

void Func_020007c0(void) { Import_EventSetExitRequest(11); }

void Func_020007cc(void) { Import_EventSetExitRequest(12); }

void Func_020007d8(void) { Import_EventSetExitRequest(13); }

void Func_020007e4(void) { Import_EventSetExitRequest(14); }

void Func_020007f0(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 13) {
    object->motionFlags = 3;
    if (object->terrainHeight !=
        Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z)) {
      s32 frames;

      System_Wait(2);
      for (frames = 0; frames < 30 && object->y > object->terrainHeight;
           frames++)
        System_Wait(1);
      Event_PlaySound(133);
    }
    MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
    MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
    MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
    Event_PlaySound(167);
    Actor_SetPosition(id, 0, 0);
    Flag_Set(0x358);
  }
}

void Func_020008a0(s32 unused, s32 id) {
  if (Actor_Get(id)->x >> 20 == 7)
    Flag_Set(0xa37);
}

void Func_020008bc(void) {
  MapGrid_CopyCells(17, 24, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 24, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 20, 1, 2, 54, 20);
}

void Func_02000900(void) {
  MapGrid_CopyCells(17, 25, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 25, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 22, 1, 2, 54, 20);
}

void Func_02000944(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 frame;

  Leader_StopMoving();
  Actor_SetSpriteFlags(object, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  frame = 0;
step:
  if (frame == 5)
    Event_PlaySound(204);
  object->motionFlags = 0;
  object->scaleX += (-0x400);
  object->scaleY += -0x300;
  object->y += -0x9999;
  System_Wait(1);
  if (++frame <= 39)
    goto step;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(16);
}

#include "input.h"
#include "macros.h"

void Func_020009ec(void) {
  Func_020027ac(2, 65);
  Func_020027ac(1, 65);
  Func_020027ac(0, 65);
  Flag_Clear(0x350);
  if (Actor_Get(10)->z >> 20 != 15 && Actor_Get(9)->z >> 20 != 15) {
    Func_020027ac(1, 33);
    return;
  }
  if (Actor_Get(9)->z >> 20 != 13) {
    Func_020027ac(0, 17);
    return;
  }
  if (Actor_Get(8)->x >> 20 != 37) {
    Func_020027ac(2, 49);
    return;
  }
  Flag_Set(0x350);
  if (!Flag_IsSet(0x351)) {
    FieldCamera_SetSpeed(0x20000, 0x4000);
    FieldCamera_MoveTo(PIXELS(616), -1, PIXELS(24), 1);
    FieldEvent_Wait(60);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(9);
  }
}

void Func_02000aac(void) {
  Func_020027ac(3, 3);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(560), 1);
}

void Func_02000ad8(void) {
  struct SceneWork *work = Ram_HeapSlots->eventWork;

  Flag_Set(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  work->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  FieldEvent_Wait(60);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(744), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Event_End();
}

void Func_02000b40(void) {
  struct PushTarget box;

  if (PushTarget_FindSlide(&box)) {
    Func_02004d10(box);
    Func_020009ec();
    if (Func_020027dc(0) != 64 && Flag_IsSet(0x221)) {
      struct Actor *object = Actor_Get(12);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(12, 6);
      Flag_Clear(0x221);
      Func_02000cb8(12);
    }
    if (Func_020027dc(2) != 64 && Flag_IsSet(0x220)) {
      struct Actor *object = Actor_Get(11);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(11, 6);
      Flag_Clear(0x220);
      Func_02000cb8(11);
    }
  } else {
    s32 angle;
    const s16 *table;
    u32 *keys;

    Actor_Get(gMainRoutineWork.currentOwner);
    keys = &gInput.held;
    table = gData_02005f18;
    angle = *(s16 *)((u8 *)table + (((*keys >> 4) & 15) << 1));
    if (angle != -1) {
      struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
      struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
      s32 position[3];
      s32 x;
      s32 z;
      u8 *cells;

      SET_VECTOR(position, (leader->x & 0xfff00000) + 0x80000, leader->y,
                 (leader->z & 0xfff00000) + 0x80000);
      FieldVector_AddPolarOffset(0x100000, angle, position);
      x = position[0] >> 20;
      z = position[2] >> 20;
      cells = Ram_MapCellBuffer;
      if (map != NULL)
        cells = map->layers[2].cells;
      cells += (x + (z << 7)) << 2;
      if (cells[2] != 255)
        Field_TryJumpForward();
    }
  }
}

extern const u8 gMarsLighthouseRoomF_Triggers[];
extern const u8 gMarsLighthouseRoomF_Room4Triggers[];
extern const u8 gMarsLighthouseRoomF_Room1Triggers[];
extern const u8 gMarsLighthouseRoomF_Room3Triggers[];
extern const u8 gMarsLighthouseRoomF_Room2Triggers[];
void MarsLighthouseRoomF_ContinueSlide(void);
void Func_020043ec(void);

void Func_02000c80(s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  s32 row = (object->z >> 20) << 7;
  s32 x = object->x;
  u8 *cell = map->layers[2].cells;
  s32 offset = (x >> 20) + row;

  cell += offset << 2;
  cell[2] = 255;
  Actor_Get(id)->priorityFlags = 0;
}

void Func_02000cb8(s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  s32 row = (object->z >> 20) << 7;
  s32 x = object->x;
  u8 *cell = map->layers[2].cells;
  s32 offset = (x >> 20) + row;

  cell += offset << 2;
  cell[2] = 0;
  {
    u8 *flags = &Actor_Get(id)->priorityFlags;
    s32 value = 2;

    value |= *flags;
    *flags = value;
  }
}

#define DEFINE_FUNC_02000CF4(name, n1, n2)                                     \
  void name(s32 unused, s32 id) {                                              \
    Func_02000c80(id);                                                         \
    Flag_Set(n1);                                                              \
    if (Func_020027dc(n2) != 64) {                                             \
      struct Actor *object = Actor_Get(id);                                    \
                                                                               \
      WORD_AT(object, 108) = 0;                                                \
      Actor_SetSpriteColorMode(object, 0);                                     \
      Actor_SetAnimation(id, 6);                                               \
      Flag_Clear(n1);                                                          \
      Func_02000cb8(id);                                                       \
    }                                                                          \
  }

DEFINE_FUNC_02000CF4(Func_02000cf4, 0x220, 2)

DEFINE_FUNC_02000CF4(Func_02000d38, 0x221, 0)

void Func_02000d80(void) {
  Flag_Set(0xa40);
  MapGrid_CopyCellAttributes(24, 38, 3, 9, 10, 38);
  MapGrid_CopyCellsLayered(64, 64, 3, 9, 74, 36);
  MapGrid_CopyCellsLayered(64, 64, 3, 9, 10, 100);
  MapGrid_CopyCellAttributes(64, 64, 3, 9, 74, 38);
  MapGrid_CopyCellAttributes(64, 64, 3, 9, 10, 102);
}

void Func_02000df8(void) {
  Func_020043ec();
  MarsLighthouseRoomF_ContinueSlide();
}

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom4)
    return gMarsLighthouseRoomF_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom2)
    return gMarsLighthouseRoomF_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom1)
    return gMarsLighthouseRoomF_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom3)
    return gMarsLighthouseRoomF_Room3Triggers;
  return gMarsLighthouseRoomF_Triggers;
}

void Func_02000e68(void) {
  if (Func_02002cfc())
    Func_02004700(1);
}

void JupiterLighthouse_LoadPalettesZero(s32 mode);
extern const u16 gData_02005bf0[];
extern const u16 gData_02005bf6[];
extern const u16 gData_02005bfc[];
extern const u16 gData_02005c0e[];
extern const u16 gData_02005c14[];
extern const u16 gData_02005c26[];
extern const u16 gData_02005c2c[];
extern const u16 gData_02005c32[];
extern const u16 gData_02005c38[];
extern const u16 gData_02005c46[];
extern const u16 gData_02005c4c[];
extern const u16 gData_02005c5a[];
extern const u16 gData_02005c60[];
extern const u16 gData_02005c6e[];

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  s32 entrance = work->entrance;
  struct HeapSlots *heap = Ram_HeapSlots;
  s32 value;
  u8 *flags;
  s32 bits;
  s32 other;

  /* FAKEMATCH(temporary): the first flag update through other; as *flags |=
   * bits, 49 of 559 instructions differ in tla-en. */

  ((struct SceneWork *)heap->eventWork)->unk1ac = 516;
  value = 516;
  Flag_Set(0x112);
  Func_020025b0();
  if (scene == (s32)&SceneId_Ov6adRoom1) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    if (entrance - 1 <= 1U) {
      Func_02002624(gData_02005bf0, gData_02005bf6, Func_02000514, 88);
      if (Flag_IsSet(0xa42))
        Actor_SetPosition(10, 0, 0);
      else
        Actor_SetPosition(9, 0, 0);
      if (Flag_IsSet(0xa43))
        Func_020006f4(0);
      if (Flag_IsSet(0xa44))
        Func_02000444(1, 0);
      if (Flag_IsSet(0xa45))
        Func_02000480(1, 0);
      if (Flag_IsSet(0xa46))
        Func_020004bc(1, 0);
    } else {
      Func_02002624(gData_02005bfc, gData_02005c0e, 0, 88);
      Actor_DisableById(15);
      Actor_DisableById(16);
      Func_02004938(15);
      Func_02004938(16);
      Func_020003b4();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom2) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_02002624(gData_02005c14, gData_02005c26, Func_02001468, 88);
    Func_020027cc(3, 1);
    if (entrance == 2)
      Func_02002604();
    if (entrance == 9) {
      Flag_Set(0x351);
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      Actor_SetPosition(work->currentOwner, 0, 0);
      Actor_SetPosition(17, 0, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(30);
      Actor_SetPosition(18, 0, 0);
      Actor_SetPosition(17, 0x1080000, 0x2080000);
      Event_PlaySound(136);
      Event_Wait(60);
      Flag_Set(0x109);
      return ((s32 (*)())Import_EventSetExitRequest)(3);
    }
    if (Flag_IsSet(0x350)) {
      Actor_SetPosition(8, 0x2580000, 0x900000);
      Actor_SetPosition(9, 0x2200000, 0xd00000);
      Actor_SetPosition(10, 0x2200000, 0xf00000);
      Flag_Set(0x220);
      Flag_Set(0x221);
    }
    Actor_DisableById(8);
    Actor_DisableById(9);
    Actor_DisableById(10);
    Func_02004938(8);
    Func_02004938(9);
    Func_02004938(10);
    flags = &Actor_GetById(11)->priorityFlags;
    bits = 2;
    other = *flags;
    other |= bits;
    *flags = other;
    flags = &Actor_GetById(12)->priorityFlags;
    bits |= *flags;
    *flags = bits;
    if (Flag_IsSet(0x220))
      Func_02000c80(11);
    if (Flag_IsSet(0x221))
      Func_02000c80(12);
    Func_020009ec();
    if (Flag_IsSet(0x350))
      Actor_SetPosition(18, 0, 0);
    else
      Actor_SetPosition(17, 0, 0);
    if (Flag_IsSet(0xa40))
      Func_02000d80();
    Task_Add(Func_02000e68, 0x480);
  } else if (scene == (s32)&SceneId_Ov6adRoom3) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    JupiterLighthouse_LoadPalettesZero(1);
    LightSprites_InitB();
    Func_020053bc(2);
    Func_0200538c(11, 6);
    if (Flag_IsSet(0x358)) {
      MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
      MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
      MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
      Actor_SetPosition(11, 0, 0);
    }
    if (Flag_IsSet(0xa37))
      Actor_SetPosition(12, 0x780000, 0x1280000);
    if (entrance - 14 <= 1U) {
      Func_02002624(gData_02005c2c, gData_02005c32, Func_02001490, 88);
      Func_020027cc(0, 1);
    } else {
      Func_02002624(gData_02005c38, gData_02005c46, 0, 88);
    }
    if (!Flag_IsSet(0x109)) {
      if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 7 ||
          entrance == 8 || entrance == 10 || entrance == 11 || entrance == 13)
        Actor_DepartInRing(gMainRoutineWork.currentOwner);
      if (entrance == 3 || entrance == 5 || entrance == 9 || entrance == 12 ||
          entrance == 14)
        Actor_ArriveInRing(gMainRoutineWork.currentOwner);
      if (entrance == 16 && !Flag_IsSet(0x109))
        Func_02003d80();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom4) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Tightrope_Initialize();
    Tightrope_Place(0, 8, 9);
    BYTE_AT(Actor_GetById(8), 98) = 1;
    BYTE_AT(Actor_GetById(9), 98) = 1;
    if (entrance - 5 <= 1U || entrance == 8) {
      Func_02002624(gData_02005c4c, gData_02005c5a, 0, 88);
#if TLA_PATCH_1
      Func_020002ec();
#endif
    } else if (entrance == 11 || entrance == 13 || entrance == 14 ||
               entrance == 1 || entrance == 15) {
      Func_02002624(gData_02005c60, gData_02005c6e, 0, 88);
    }
    if (!Flag_IsSet(0x109)) {
      if (entrance == 15)
        Actor_SetPosition(23, 0x2980000, 0x2c80000);
      if (entrance == 11)
        Actor_SetPosition(17, 0x2180000, 0x2280000);
    }
    Actor_GetById(16)->motionFlags = 0;
    if (Flag_IsSet(0xa38)) {
      Actor_SetPosition(16, 0x3280000, 0x1380000);
      Actor_GetById(16)->y = 0;
      Actor_GetById(16)->terrainHeight = 0;
    }
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Actor_GetById(10)->priorityFlags |= 192;
    Actor_GetById(11)->priorityFlags |= 128;
    Actor_GetById(12)->priorityFlags |= 128;
  }
  return 0;
}

extern const u8 gData_02005c84[];
extern const u8 gData_02005ce0[];
extern const u8 gData_02005d3c[];

s32 Func_0200328c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  s32 flag = 0xa30;

  party = &gMainRoutineWork;
  if (Event_GetFlag(flag))
    TileMap_ConvertBuffer(
        party->map + ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
        0x5000000);
  return 0;
}

void Func_02001448(void) { Func_0200328c(11, 12, 13, 0x4000, gData_02005c84); }

void Func_02001468(void) {
  Func_0200328c(19, 21, 22, 0x4000, gData_02005ce0);
  Func_020027ac(3, 131);
}

void Func_02001490(void) { Func_0200328c(14, 15, 16, 0x4000, gData_02005d3c); }

#include "shared/scene_set_arrival_flags.h"

extern s32 gData_02007040;

void Func_020014dc(s32 value) { gData_02007040 = value; }

#include "shared/jupiter_lighthouse_aerie.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_0200703c;
extern u16 gData_02007068[];

DEFINE_PALETTE_CYCLE_STEP(Func_020014e8, gData_0200703c, gData_02007040,
                          gData_02007068)

#include "ram_buffer.h"
#include "field_event.h"

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES_ZERO(JupiterLighthouse_LoadPalettesZero,
                                             gData_02007040, gData_02007068,
                                             gData_02007088, gData_0200703c,
                                             Func_020014e8, gData_0200708c)

#include "event_inline.h"

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"

/* VIEW(s167c): tag struct EventWork as struct EventWork__s167c; event_work.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:977: structure has no member
 * named `unk_d46'. */
struct EventWork__s167c {
  u8 unk_00[0x154];
  u16 unk_154;
  u8 unk_156[0xd44 - 0x156];
  u16 unk_d44;
  u16 unk_d46;
};

void Func_0200167c(void) {
  struct EventWork__s167c *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *motion;
  u8 *flag;
  s32 x;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unk_d46 = 0;
  motion = &actor->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(actor, 0);
  flag = (u8 *)linked + 0x63;
  while (*flag != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unk_d44 = 1;
  work->unk_d46 = 1;
  x = (linked->x >> 19) & ~1;
  facing = linked->facing;
  facing >>= 13;
  actor->x = (x + facing - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  // Through a pointer: the native loads this 1 from the pool.
  *(u8 *)&actor->priorityFlags = 1;
  *motion = 3;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  // Through a pointer: the native loads this 0 from the pool.
  *(u8 *)&shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
  fall:
    System_Wait(1);
    i++;
    if (i <= 59 && actor->y > actor->terrainHeight + 0x100000)
      goto fall;
  }
  Event_PlaySound(127);
  i = 0;
  if (actor->velocityY != 0) {
  settle:
    System_Wait(1);
    i++;
    if (i <= 59 && actor->velocityY != 0)
      goto settle;
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unk_d46 = 1;
  work->unk_154 = 0;
  Event_End();
}

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

#include "field_sprite.h"

extern const s32 gData_02007044[];

DEFINE_WHIRLWIND_UPDATE(Func_02001894, MapGrid_GetCellBehavior,
                        Position_FlatDistance, gData_02007044, Data_02007064)

#include "field_effect.h"

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): MulQ16 forwarded inline; as direct calls to
   * Iwram_MulQ16, 69 of 92 instructions differ in tla-en. */
  return Iwram_MulQ16(a, b);
}

struct GustObject *Func_02001a54(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + MulQ16(0x80000, Trig_Cos(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = (struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                  position.z);
  Func_02005480(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001894;
  object->speedLimit = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_020035b8(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

DEFINE_ACTOR_EMIT_RISING_SPARKS_B(Actor_EmitRisingSparksB, Func_020035b8)

#include "call.h"
#include "field_coordinate.h"

void Func_02001bcc(void) {
  struct PositionedActor *actor9 = Actor_GetById(9);
  struct PositionedActor *actor10 = Actor_GetById(10);
  struct PositionedActor *actor23 = Actor_GetById(23);
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(9), 4);
    Actor_SetSpriteColorMode(Actor_GetById(11), 4);
  }
  if (Event_GetFlag(0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(10), 4);
    Actor_SetSpriteColorMode(Actor_GetById(12), 4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

DEFINE_ACTOR_IS_TOUCHING(Func_02001d5c)

DEFINE_ACTOR_IS_BESIDE(Func_02001dc0)

#define MapLayer MapLayer__s1e20
#define SceneWork SceneWork__s1e20
#define Camera_SetBounds Camera_SetBounds__s1e20
#include "shared/mars_lighthouse_entrance_room.h"
#undef Camera_SetBounds
/* VIEW(s1e20): static Camera_SetBounds as Camera_SetBounds__s1e20; room.h has
 * static inline void FieldCamera_MoveTo(s32 x, s32 y, s32 z, s32 use_setter) {
 * FieldCamera_MoveWithinMapBounds(x, y, z, use_setter); }; not tried:
 * Camera_SetBounds__s1e20 was not found. */
#undef SceneWork
/* VIEW(s1e20): tag struct SceneWork as struct SceneWork__s1e20; room.h has
 * another definition; not tried: struct SceneWork__s1e20 was not found. */
#undef MapLayer
/* VIEW(s1e20): tag struct MapLayer as struct MapLayer__s1e20; room.h has
 * another definition; not tried: struct MapLayer__s1e20 was not found. */
#include "field/event_work.h"

struct RideSlot {
  u16 id;
  u16 blocked;
};

struct RideWork {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

DEFINE_RIDE_UPDATE_CARRIED(Func_02001e20, RideWork, Func_02001d5c,
                           Func_02001dc0)

extern const u8 gData_02005d8c[];

DEFINE_ACTOR_RESET_SCRIPTED_FOR(Func_02001f84, gData_02005d8c)

DEFINE_RIDE_SPAWN_FRONT(Func_02001fb4, Func_02005498_, Func_02001f84, 2,
                        Func_02001e20, Func_02005480)

#include "shared/mars_lighthouse_entrance_room.h"

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1ffc): tag struct RideWork as struct RideWork__s1ffc;
 * mars_lighthouse_room_f.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1426: request for member `unk02'
 * in something not a structure or union. */
struct RideWork__s1ffc {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

DEFINE_RIDE_UPDATE_LANDING(Func_02001ffc, RideWork__s1ffc, Func_02001d5c,
                           Func_02001dc0)

#define ABS(value) ((value) < 0 ? -(value) : (value))

extern const u8 gData_02005d8c[];

void Func_020022d0(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02005d8c; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
DEFINE_ACTOR_SETUP_SCRIPTED_PART(Func_0200215c, gData_02005d8c)

DEFINE_RIDE_SPAWN_BACK(Func_020021a0, Func_02005498_, Func_0200215c,
                       Func_02001e20, Func_02005480)

DEFINE_RIDE_SPAWN_FRONT(Func_020021e8, Func_02005498_, Func_0200215c, 3,
                        Func_02001ffc, Func_02005480)

DEFINE_RIDE_SPAWN_BACK(Func_02002230, Func_02005498_, Func_0200215c,
                       Func_02001ffc, Func_02005480)

DEFINE_ACTOR_IS_ABOVE(Func_02002278)

DEFINE_RIDE_UPDATE_DRIFT(Func_020022d0, Func_02002278)

DEFINE_ACTOR_SETUP_SCRIPTED_FOR(Func_0200236c, Func_02005480, gData_02005d8c)

DEFINE_RIDE_SPAWN_SPARK(Func_020023ac, Func_02005498_, Func_0200236c,
                        Func_020022d0)

/* VIEW(s242c): tag struct Actor as struct Actor__s242c; actor.h has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1695:
 * structure has no member named `flag5b'. */
typedef struct Actor__s242c {
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

/* VIEW(s242c): routine Func_020023ac as void Func_020023ac(); the parts before
 * it has another declaration; tried as the definition's void Func_020023ac(s32
 * *position, s32 speed), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1730: too many
 * arguments to function `Func_020023ac'. */
extern void Func_020023ac__s242c() __asm__("Func_020023ac");

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

DEFINE_BEAM_DRAW_TARGETS(Func_0200242c, Func_020023ac__s242c, Func_020021a0,
                         Func_02002230, Func_020021e8, Func_02001fb4)
#undef IWRAM

void Func_020025b0(void) {
  struct MarsLighthouseRoomFRideWork *work =
      (struct MarsLighthouseRoomFRideWork *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomFAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Func_02005480(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

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

/* VIEW(s25b0): tag struct RideWork as struct RideWork__s25b0;
 * mars_lighthouse_room_f.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1798: structure has no member
 * named `handle'. */
struct RideWork__s25b0 {
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

void Func_0200236c(struct Actor *object, s32 unused);

void Func_02002604(void) {
  struct RideWork__s25b0 *work =
      (struct RideWork__s25b0 *)(gInventorySnapshot + 0x188);

  Actor_Release(work->handle);
  work->handle = NULL;
}

void Func_02002618(s16 value) {
  ((struct RideWork__s25b0 *)(gInventorySnapshot + 0x188))->value0e = value;
}

DEFINE_BEAM_START(Func_02002624, Func_0200242c)
