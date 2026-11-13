// ---- games/tla/src/scene/mars_lighthouse_room_f.c ----
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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "mars_lighthouse_room_f/room.h"

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;
extern const u8 gData_02006518[];
extern const u8 gData_02006530[];
extern const u8 gData_02006710[];
extern const u8 gData_02006890[];
extern const u8 gData_02006a0c[];

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom4)
    return gData_02006530;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom2)
    return gData_02006a0c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom1)
    return gData_02006710;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom3)
    return gData_02006890;
  return gData_02006518;
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
          Func_02005600();
          return;
        }
      }
    }
  }
  if (Func_020055f8(gMainRoutineWork.currentOwner) != -1)
    Func_02005688();
}

void Func_02000168(void) { Func_02005600(); }

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
  Vector_AddPolar(0x100000, angle, position);
  object = Func_02005680(position, 1);
  if (object != NULL) {
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    Vector_AddPolar(0x100000, angle, position);
    if (Func_02005680(position, 1) != NULL)
      return;
  }
  Func_02005688();
}

#else

void Func_02000170(void) { Func_02005688(); }

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

  Func_02005688();
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
  Func_020054e8_(53, 8, 1, 1, 47, 8);
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

  if (Func_02004a8c(&box)) {
    Func_02004d10(box);
    Func_020003b4();
  }
}

void Func_02000444(s32 state, s32 unused) {
  if (state == 1) {
    MapGrid_CopyCells(29, 56, 3, 4, 10, 50);
    MapGrid_CopyCellsLayered(93, 56, 3, 4, 74, 50);
    Event_SetFlag(0xa44);
  }
}

void Func_02000480(s32 state, s32 unused) {
  if (state == 1) {
    MapGrid_CopyCells(29, 56, 3, 4, 18, 50);
    MapGrid_CopyCellsLayered(93, 56, 3, 4, 82, 50);
    Event_SetFlag(0xa45);
  }
}

void Func_020004bc(s32 state, s32 unused) {
  if (state == 1) {
    MapGrid_CopyCells(29, 56, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 56, 3, 4, 78, 50);
    Event_SetFlag(0xa46);
  }
}

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
    s32 a = Random16Far();
    s32 x = origin->x + (a << 4) - (Random16Far() << 4);
    s32 b = Random16Far();
    s32 c = Random16Far();
    struct Actor *object =
        FieldActor_Create(0x149, x, (b << 3) + origin->y, (c << 4) + origin->z);

    if (object != NULL) {
      s32 angle = (Random16Far() >> 2) + 0xe000;
      s32 speed = Random16Far() << 3;
      s32 scale = (Random16Far() >> 1) + 0x4000;

      {
        u32 pick = Random16Far();

        if (pick <= 0x7fff)
          Actor_StartScript(object, gData_02005b68);
        else
          Actor_StartScript(object, gData_02005bac);
      }
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = (angle << 2) + 0x20000;
      object->velocityZ = Iwram_MulQ16(speed, Math_Cosine(angle));
      {
        s32 vx = Iwram_MulQ16(speed, Math_Sin(angle));

        object->acceleration = 0;
        object->scaleX = scale;
        object->scaleY = scale;
        object->gravity = 0x547a;
        object->velocityX = vx;
        object->bounce = 0x8000;
      }
      Animation_ApplyChildValues(object, 1);
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
  Camera_SetBounds(PIXELS(248), -1, PIXELS(712), 1);
  Func_020055e0();
  System_Wait(20);
  FieldCamera_SetSpeed(0x50000, 0xa000);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(872), 1);
  Func_020055e0();
  FieldEvent_Wait(120);
  Event_End();
}

void Func_020006f4(s32 burst) {
  if (!Flag_IsSet(0xa46)) {
    Func_020054e0_(29, 60, 3, 4, 14, 50);
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
  Func_020054e0_(17, 24, 3, 1, 17, 13);
  Func_020054e8_(20, 24, 1, 1, 18, 77);
  Func_020054e0_(62, 20, 1, 2, 54, 20);
}

void Func_02000900(void) {
  Func_020054e0_(17, 25, 3, 1, 17, 13);
  Func_020054e8_(20, 25, 1, 1, 18, 77);
  Func_020054e0_(62, 22, 1, 2, 54, 20);
}

void Func_02000944(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 frame;

  Leader_StopMoving();
  Actor_SetSingleChildFlags(object, 0);
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

extern s16 gData_02005f18[];

void Func_02000cb8(s32 id);

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
  Func_020055e0();
  FieldEvent_Wait(60);
  Event_End();
}

void Func_02000b40(void) {
  struct PushTarget box;

  if (Func_02004a8c(&box)) {
    Func_02004d10(box);
    Func_020009ec();
    if (Func_020027dc(0) != 64 && Flag_IsSet(0x221)) {
      struct Actor *object = Actor_Get(12);

      object->update = NULL;
      Animation_ApplyChildValues(object, 0);
      Actor_SetAnimation(12, 6);
      Flag_Clear(0x221);
      Func_02000cb8(12);
    }
    if (Func_020027dc(2) != 64 && Flag_IsSet(0x220)) {
      struct Actor *object = Actor_Get(11);

      object->update = NULL;
      Animation_ApplyChildValues(object, 0);
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
        Func_02005600();
    }
  }
}

extern const u8 gData_02006b8c[];
extern const u8 gData_02006b98[];
extern const u8 gData_02006d78[];
extern const u8 gData_02006e38[];
extern const u8 gData_02006f64[];
s32 Func_02002cfc(void);
void Func_02002ce0(void);
void Func_020043ec(void);
void Func_02004700(s32 a0);

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

void Func_02000cf4(s32 unused, s32 id) {
  Func_02000c80(id);
  Flag_Set(0x220);
  if (Func_020027dc(2) != 64) {
    struct Actor *object = Actor_Get(id);

    WORD_AT(object, 108) = 0;
    Animation_ApplyChildValues(object, 0);
    Actor_SetAnimation(id, 6);
    Flag_Clear(0x220);
    Func_02000cb8(id);
  }
}

void Func_02000d38(s32 unused, s32 id) {
  Func_02000c80(id);
  Flag_Set(0x221);
  if (Func_020027dc(0) != 64) {
    struct Actor *object = Actor_Get(id);

    WORD_AT(object, 108) = 0;
    Animation_ApplyChildValues(object, 0);
    Actor_SetAnimation(id, 6);
    Flag_Clear(0x221);
    Func_02000cb8(id);
  }
}

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
  Func_02002ce0();
}

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom4)
    return gData_02006b98;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom2)
    return gData_02006f64;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom1)
    return gData_02006d78;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6adRoom3)
    return gData_02006e38;
  return gData_02006b8c;
}

void Func_02000e68(void) {
  if (Func_02002cfc())
    Func_02004700(1);
}

void Func_020015a4(s32 mode);
void Func_020025b0(void);
void Func_02002604(void);
/* VIEW(mars_lighthouse_room_f): routine Func_02002624 as s32 Func_02002624(const void *targets, const void *stops, void *a2, s32 a3); the definition has void Func_02002624(u16 *targets, u16 *stops, s32 a2, s32 a3). */
extern s32 Func_02002624__mars_lighthouse_room_f(const void *targets, const void *stops, void *a2, s32 a3) __asm__("Func_02002624");
void Func_020027cc(s32 index, s16 value);
void Func_02003d80(void);
s32 Func_02004938(s32 id);
void Func_02005284(void);
void Func_0200538c(s32 id, s32 value);
void Func_020053bc(s32 value);
void Func_020055b8(s32 id);
void Func_02005638(s32 a0);
void Func_02005670(void);
void Func_02005678(s32 a0, s32 a1, s32 a2);
void Func_020056b0(s32 id);
void Func_020056b8(s32 id);
void Func_02001468(void);
void Func_02001490(void);
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
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    if (entrance - 1 <= 1U) {
      Func_02002624__mars_lighthouse_room_f(gData_02005bf0, gData_02005bf6, Func_02000514, 88);
      if (Flag_IsSet(0xa42))
        Func_02005598_(10, 0, 0);
      else
        Func_02005598_(9, 0, 0);
      if (Flag_IsSet(0xa43))
        Func_020006f4(0);
      if (Flag_IsSet(0xa44))
        Func_02000444(1, 0);
      if (Flag_IsSet(0xa45))
        Func_02000480(1, 0);
      if (Flag_IsSet(0xa46))
        Func_020004bc(1, 0);
    } else {
      Func_02002624__mars_lighthouse_room_f(gData_02005bfc, gData_02005c0e, 0, 88);
      Func_020055b8(15);
      Func_020055b8(16);
      Func_02004938(15);
      Func_02004938(16);
      Func_020003b4();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom2) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_02002624__mars_lighthouse_room_f(gData_02005c14, gData_02005c26, Func_02001468, 88);
    Func_020027cc(3, 1);
    if (entrance == 2)
      Func_02002604();
    if (entrance == 9) {
      Flag_Set(0x351);
      Event_Start();
      Event_ResolvePendingActions(0);
      Camera_SetBounds(-1, -1, -1, 0);
      Func_02005598_(work->currentOwner, 0, 0);
      Func_02005598_(17, 0, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(30);
      Func_02005598_(18, 0, 0);
      Func_02005598_(17, 0x1080000, 0x2080000);
      Sound_Play(136);
      Event_Wait(60);
      Flag_Set(0x109);
      return ((s32 (*)())Import_EventSetExitRequest)(3);
    }
    if (Flag_IsSet(0x350)) {
      Func_02005598_(8, 0x2580000, 0x900000);
      Func_02005598_(9, 0x2200000, 0xd00000);
      Func_02005598_(10, 0x2200000, 0xf00000);
      Flag_Set(0x220);
      Flag_Set(0x221);
    }
    Func_020055b8(8);
    Func_020055b8(9);
    Func_020055b8(10);
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
      Func_02005598_(18, 0, 0);
    else
      Func_02005598_(17, 0, 0);
    if (Flag_IsSet(0xa40))
      Func_02000d80();
    Task_Add(Func_02000e68, 0x480);
  } else if (scene == (s32)&SceneId_Ov6adRoom3) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_020015a4(1);
    Func_02005284();
    Func_020053bc(2);
    Func_0200538c(11, 6);
    if (Flag_IsSet(0x358)) {
      Func_020054f0_(71, 24, 7, 1, 71, 5);
      Func_020054e8_(7, 24, 7, 1, 7, 7);
      Func_020054e8_(7, 88, 7, 1, 7, 71);
      Func_02005598_(11, 0, 0);
    }
    if (Flag_IsSet(0xa37))
      Func_02005598_(12, 0x780000, 0x1280000);
    if (entrance - 14 <= 1U) {
      Func_02002624__mars_lighthouse_room_f(gData_02005c2c, gData_02005c32, Func_02001490, 88);
      Func_020027cc(0, 1);
    } else {
      Func_02002624__mars_lighthouse_room_f(gData_02005c38, gData_02005c46, 0, 88);
    }
    if (!Flag_IsSet(0x109)) {
      if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 7 ||
          entrance == 8 || entrance == 10 || entrance == 11 || entrance == 13)
        Func_020056b0(gMainRoutineWork.currentOwner);
      if (entrance == 3 || entrance == 5 || entrance == 9 || entrance == 12 ||
          entrance == 14)
        Func_020056b8(gMainRoutineWork.currentOwner);
      if (entrance == 16 && !Flag_IsSet(0x109))
        Func_02003d80();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom4) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_02005670();
    Func_02005678(0, 8, 9);
    BYTE_AT(Actor_GetById(8), 98) = 1;
    BYTE_AT(Actor_GetById(9), 98) = 1;
    if (entrance - 5 <= 1U || entrance == 8) {
      Func_02002624__mars_lighthouse_room_f(gData_02005c4c, gData_02005c5a, 0, 88);
#if TLA_PATCH_1
      Func_020002ec();
#endif
    } else if (entrance == 11 || entrance == 13 || entrance == 14 ||
               entrance == 1 || entrance == 15) {
      Func_02002624__mars_lighthouse_room_f(gData_02005c60, gData_02005c6e, 0, 88);
    }
    if (!Flag_IsSet(0x109)) {
      if (entrance == 15)
        Func_02005598_(23, 0x2980000, 0x2c80000);
      if (entrance == 11)
        Func_02005598_(17, 0x2180000, 0x2280000);
    }
    Actor_GetById(16)->motionFlags = 0;
    if (Flag_IsSet(0xa38)) {
      Func_02005598_(16, 0x3280000, 0x1380000);
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

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern const u8 gData_02005c84[];
extern const u8 gData_02005ce0[];
extern const u8 gData_02005d3c[];

void Func_02005548(s32 scene, s32 a1);
s32 Func_0200328c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  s32 flag = 0xa30;

  party = &gMainRoutineWork;
  if (Event_GetFlag(flag))
    Func_02005548(party->map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
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

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_0200703c;
extern u16 gData_02007068[];

void Func_020014e8(void) {
  u16 colors[16];
  s32 phase = (gData_0200703c / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02007040 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    /* FAKEMATCH(temporary): the slot and the table address held in locals;
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02007068 + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02007068;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_0200703c++;
}

#include "ram_buffer.h"
#include "field_event.h"

extern u16 gData_02007088[];
extern u16 gData_0200708c[];

void Func_02005540();
void Func_020056a8();

void Func_020015a4(s32 mode) {
  gData_02007040 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02007068, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02007088, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (GameFlag_Test(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): the zero held in a local, so it is built in r2 and
     * the counter's address in r3; as a plain store of 0, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_0200703c = zero;
    Task_Add(Func_020014e8, 1152);
  } else {
    Dma_Set(gData_0200708c, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

#include "event_inline.h"

void Func_02001628(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02005540(object->x, object->z, 0, (height >> 19) + 6);
  Func_020056a8(object->x, object->z, 0, 128);
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s167c.c ----
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"

void Event_Start(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Event_End(void);

/* VIEW(s167c): tag struct EventWork as struct EventWork__s167c; event_work.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1017: structure has no member named `unk_d46'. */
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
  Actor_SetSingleChildFlags(actor, 0);
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
  Actor_SetSingleChildFlags(actor, 1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  // Through a pointer: the native loads this 0 from the pool.
  *(u8 *)&shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
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
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unk_d46 = 1;
  work->unk_154 = 0;
  Event_End();
}


s32 Func_02001868(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

#include "field_sprite.h"

extern const s32 gData_02007044[];
extern s32 Data_02007064;

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_020056a0(s32 x, s32 z, s32 layer);
void Camera_WorldToScreen(s32 *position);
s32 Math_Cosine(s32 angle);
void Actor_StartScript(struct Actor *object, const void *script);

void Func_02001894(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_020056a0(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_02001868(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Math_Cosine(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gData_02007044);
    Event_PlaySound(228);
    Data_02007064 = zero;
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s1a54.c ----
#include "field_effect.h"

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

s32 Math_Cosine(s32 angle);
void Func_02005480(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Func_02001894(struct Actor *self);
void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const struct EffectOptions *options);

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
  position.x = source->x + MulQ16(0x80000, Math_Cosine(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = (struct GustObject *)FieldActor_Create(280, position.x, source->y, position.z);
  Func_02005480(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001894;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_020035b8(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}


s32 Func_02001b2c(struct Actor *object) {
  s32 params[10];

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params[0] = 3;
    params[2] = 0xb333;
    params[3] = 0xb333;
    params[1] = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    Func_020035b8(x, y, object->z, 0,
                  (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, params);
  }
  return 0;
}

#include "call.h"
#include "field_coordinate.h"

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02001bcc(void) {
  struct PositionedActor *actor9 = Actor_GetById(9);
  struct PositionedActor *actor10 = Actor_GetById(10);
  struct PositionedActor *actor23 = Actor_GetById(23);
  struct FieldSprite *sprite = actor23->animation;

  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 24, 0x2000000, 0xf80000);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 23, 0x2000000, 0xf80000);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, 0x1d80000, 0x1000000);
  Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (GameFlag_Test(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_GetById(9), 4);
    Animation_ApplyChildValues(Actor_GetById(11), 4);
  }
  if (Value1((s32 (*)())GameFlag_Test, 0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_GetById(10), 4);
    Animation_ApplyChildValues(Actor_GetById(12), 4);
  }
  if (GameFlag_Test(0xa0a) && GameFlag_Test(0xa0b)) {
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

s32 Func_02001d5c(struct Actor *a, struct Actor *b) {
  if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&
      ABS(a->z - b->z) < 0x80000)
    return 1;
  return 0;
}

s32 Func_02001dc0(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)
      return 1;
  }
  return 0;
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s1e20.c ----
#define MapLayer MapLayer__s1e20
#define SceneWork SceneWork__s1e20
#define Camera_SetBounds Camera_SetBounds__s1e20
#include "mars_lighthouse_entrance/room.h"
#undef Camera_SetBounds
/* VIEW(s1e20): static Camera_SetBounds as Camera_SetBounds__s1e20; room.h has static inline void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) { Camera_MoveWithinMapBounds(x, y, z, use_setter); }; not tried: Camera_SetBounds__s1e20 was not found. */
#undef SceneWork
/* VIEW(s1e20): tag struct SceneWork as struct SceneWork__s1e20; room.h has another definition; not tried: struct SceneWork__s1e20 was not found. */
#undef MapLayer
/* VIEW(s1e20): tag struct MapLayer as struct MapLayer__s1e20; room.h has another definition; not tried: struct MapLayer__s1e20 was not found. */
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

extern u8 gData_020023c4[];

s32 Func_02001d5c(struct Actor *a, struct Actor *b);
s32 Func_02001dc0(struct Actor *a, struct Actor *b);

void Func_02001e20(struct Actor *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  s32 direction;
  s32 id;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (actor->y == actor->terrainHeight && Func_02001d5c(object, actor) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value68 = 0;
      {
        u16 *cell = &work->direction;

        if (object->unk4c > 0) {
          direction = 1;
          *cell = direction;
        } else {
          direction = 0xffff;
          *cell = direction;
        }
      }
    }
    object->priorityFlags = 2;
    {
      s32 speed = object->unk4c;

      object->unk4c = 0;
      object->z -= speed;
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_Get(id);

    if (actor->y == actor->terrainHeight && Func_02001dc0(object, actor)) {
      if (work->slots[i].blocked == 0) {
        s32 speed;

        object->priorityFlags = 2;
        speed = object->unk4c;
        object->unk4c = 0;
        object->z -= speed;
        object->gravity += speed >> 2;
      } else {
        work->blocked = 1;
      }
    }
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s1f84.c ----
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

extern const u8 gData_02005d8c[];

void Func_02001e20(struct Actor *object);
void Func_02001ffc(struct Actor *object);
void Func_02001f84(struct Actor *object, s32 unused) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_02005d8c);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001fb4(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_02001f84(object, 2);
    object->update = Func_02001e20;
    if (mode)
      object->unk5c[0] = 2;
    Func_02005480(object, 2);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s1ffc.c ----
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"


struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1ffc): tag struct RideWork as struct RideWork__s1ffc; mars_lighthouse_room_f.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1531: request for member `unk02' in something not a structure or union. */
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

extern u8 gData_020023c4[];

s32 Func_02001d5c(struct Actor *a, struct Actor *b);
s32 Func_02001dc0(struct Actor *a, struct Actor *b);

void Func_02001ffc(struct Actor *object) {
  struct RideWork__s1ffc *work = (struct RideWork__s1ffc *)(gData_020023c4 + 0x188);
  struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  struct RideDirection *direction;

  object->z += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->x += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (other->y == other->terrainHeight && Func_02001d5c(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.unk02 = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->unk00 = 1;
      else
        direction->unk00 = 0xffff;
    }
    object->priorityFlags = 2;
    object->z -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->unk4c = 0;
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y == other->terrainHeight && Func_02001dc0(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = 2;
        object->z -= object->unk4c;
        object->gravity += object->unk4c >> 2;
        object->unk4c = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s21a0.c ----
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

#define ABS(value) ((value) < 0 ? -(value) : (value))


extern const u8 gData_02005d8c[];

void Func_02001e20(struct Actor *object);
void Func_02001ffc(struct Actor *object);
void Func_020022d0(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02005d8c; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_0200215c(struct Actor *object, s32 part) {
  /* FAKEMATCH(temporary): one zero for every cleared field; as six stores of
   * 0, 29 of 30 instructions differ in tla-en. */
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_02005d8c);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_020021a0(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_0200215c(object, 3);
    object->update = Func_02001e20;
    if (mode)
      object->unk5c[0] = 2;
    Func_02005480(object, 2);
  }
}

void Func_020021e8(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_0200215c(object, 3);
    object->update = Func_02001ffc;
    if (mode)
      object->unk5c[0] = 2;
    Func_02005480(object, 2);
  }
}

void Func_02002230(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_0200215c(object, 3);
    object->update = Func_02001ffc;
    if (mode)
      object->unk5c[0] = 2;
    Func_02005480(object, 2);
  }
}

s32 Func_02002278(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)
      return 1;
  }
  return 0;
}

void Func_020022d0(struct Actor *object) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 dx = object->bounce;
  s32 dz;
  struct SpriteParts *parts;
  s32 rate;
  s32 scale;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  parts->unk12 += object->timer;
  if (leader->y == leader->terrainHeight && Func_02002278(object, leader)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}

void Func_0200236c(struct Actor *object, s32 unused) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_02005480(object, 2);
  Actor_StartScript(object, gData_02005d8c);
  Actor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_020023ac(s32 *position, s32 speed) {
  struct Actor *object =
      Func_02005498_(285, position[0], position[1], position[2] + 0xfffe0000);
  s32 spin;

  if (object != NULL) {
    object->bounce = Math_Cosine((u16)(Random16Far() * 3 >> 4) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + speed;
    spin = (u16)Random16Far() + 0xffff8000;
    object->timer = spin;
    Func_0200236c(object, 2);
    object->update = (void (*)(void))Func_020022d0;
    Animation_SetStateFlags(object, 1);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s242c.c ----
/* VIEW(s242c): tag struct Actor as struct Actor__s242c; actor.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1811: structure has no member named `flag5b'. */
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

void Func_02001fb4();
void Func_020021a0();
void Func_020021e8();
void Func_02002230();
/* VIEW(s242c): routine Func_020023ac as void Func_020023ac(); the parts before it has another declaration; tried as the definition's void Func_020023ac(s32 *position, s32 speed), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1846: too many arguments to function `Func_020023ac'. */
extern void Func_020023ac__s242c() __asm__("Func_020023ac");
extern u8 gData_020023c4[];

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

void Func_0200242c(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  Camera *camera;
  Position *position;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)(gData_020023c4 + 0x188);
  camera = IWRAM->camera;
  entry = state->entries;
  position = &camera->position;
  cameraX = position->x;
  cameraX &= 0xffff0000;
  cameraZ = position->z & 0xffff0000;
  saved[1] = (*(s32 **)camera)[1];
  saved[0] = IWRAM->unk_6c.word;
  if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)
    state->callback();
  v = (u16)state->unk_6;
  state->unk_8 = v;
  state->unk_6 = 0;
  for (i = 3; i >= 0; i--, entry++) {
    Actor *actor;
    s32 flag, x, y, unk6, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Test(0x106);
    if (*(s16 *)(saved[0] + 358) != 0)
      flag |= 1;
    mark = &actor->flag5b;
    *mark = flag;
    if (state->unk_e != 0)
      *mark = GameFlag_Test(0x106);
    dx = actor->x;
    dz = actor->z;
    dx -= cameraX;
    /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank; as
     * no asm, 94 of 182 instructions differ in tla-en. */
    asm("" : : "r"(dx));
    y = actor->y;
    dz -= cameraZ;
    y = dz - y;
    unk6 = entry->unk_6;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (unk6 == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 240;
      switch (y) {
      case 128:
        Func_020023ac__s242c(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 48:
        Func_020021a0(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 32:
        Func_02002230(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 16:
        Func_020021e8(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 0:
        Func_02001fb4(&entry->actor->x, entry->unk_c, unk6);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}
#undef IWRAM

// ---- games/tla/src/scene/mars_lighthouse_room_f/s25b0.c ----
struct MarsLighthouseRoomFAnimation {
  u8 unk00[0x10];
  u8 value10;
  u8 flag11 : 1;
};

struct MarsLighthouseRoomFRideWork {
  u8 unk00[0x0c];
  u16 value0c;
  u8 unk0e[0x62];
  struct Actor *handle;
};

extern u8 gData_020023c4[];

void Func_020025b0(void) {
  struct MarsLighthouseRoomFRideWork *work =
      (struct MarsLighthouseRoomFRideWork *)(gData_020023c4 + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomFAnimation *animation;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWords(work, 116);
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


/* VIEW(s25b0): tag struct RideWork as struct RideWork__s25b0; mars_lighthouse_room_f.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1936: structure has no member named `handle'. */
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

extern u16 gData_02004cf0[];
void Func_020054a0(void *handle);
void Func_02002148(void);
void Func_0200236c(struct Actor *object, s32 unused);
void Func_02002ad0(struct Actor *object);

void Func_02002604(void) {
  struct RideWork__s25b0 *work = (struct RideWork__s25b0 *)(gData_020023c4 + 0x188);

  Func_020054a0(work->handle);
  work->handle = NULL;
}

void Func_02002618(s16 value) {
  ((struct RideWork__s25b0 *)(gData_020023c4 + 0x188))->value0e = value;
}


/// One object the beam reaches, as Func_02002624 records it.
struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  struct {
    u16 unk00;
    u16 unk02;
  } stops[4];
};

void Func_0200242c(void);

void Func_02002624(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;
  s32 value;
  s32 one;
  s32 flags;
  s32 height;
  struct Actor *object;
  s32 alpha;
  s32 control;

  if (Event_GetFlag(0x109) == 0) {
    for (i = 0; i < 4 && (id = *targets++) > 0; i++) {
      flags = *targets;
      height = flags & 15;
      flags &= 0xf0;
      object = Actor_Get(id);
      targets++;
      if (object != NULL) {
        Actor_SetSingleChildFlags(object, 0);
        ((u8 *)object)[98] = one = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = one;
        target->flags = flags;
        target->object = Actor_Get(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i < 4; i++) {
        value = *stops++;
        beam->stops[i].unk00 = value;
        if (value <= 0)
          break;
        beam->stops[i].unk02 = *stops++;
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    alpha = 0xc08;
    REG_BLDALPHA = alpha;
    control = 0x3f10;
    REG_BLDCNT = control;
  }
  Task_AddFrame(Func_0200242c, 0x480);
}
