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

const u8 *Func_02000058(void) {
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
  u32 facing = object->angle;

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

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;
extern const u8 gData_02006518[];
extern const u8 gData_02006530[];
extern const u8 gData_02006710[];
extern const u8 gData_02006890[];
extern const u8 gData_02006a0c[];

void Func_02000170(void) {
  s32 position[3];
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 angle = object->angle >> 12;

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
  if (x >> 20 == object->action >> 4)
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
          Func_02005490(object, gData_02005b68);
        else
          Func_02005490(object, gData_02005bac);
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
  Func_02005560();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  Camera_SetBounds(PIXELS(248), -1, PIXELS(712), 1);
  Func_020055e0();
  System_Wait(20);
  FieldCamera_SetSpeed(0x50000, 0xa000);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(872), 1);
  Func_020055e0();
  FieldEvent_Wait(120);
  Func_02005568();
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

  Func_02005608();
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
    Func_02005660();
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
  Func_02005560();
  FieldEvent_ResolvePendingActions(0);
  work->unk166 = 0;
  Flag_Clear(0x106);
  Flag_Clear(0x104);
  FieldEvent_Wait(60);
  FieldCamera_SetSpeed(0x30000, 0x6000);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(744), 1);
  Func_020055e0();
  FieldEvent_Wait(60);
  Func_02005568();
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

const u8 *Func_02000e04(void) {
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
s32 Func_02002624(const void *targets, const void *stops, void *a2, s32 a3);
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
void Event_StartScreenTransition(void);
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

s32 Func_02000e7c(void) {
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
      Func_02002624(gData_02005bf0, gData_02005bf6, Func_02000514, 88);
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
      Func_02002624(gData_02005bfc, gData_02005c0e, 0, 88);
      Func_020055b8(15);
      Func_020055b8(16);
      Func_02004938(15);
      Func_02004938(16);
      Func_020003b4();
    }
  } else if (scene == (s32)&SceneId_Ov6adRoom2) {
    Func_02005638(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Func_02002624(gData_02005c14, gData_02005c26, Func_02001468, 88);
    Func_020027cc(3, 1);
    if (entrance == 2)
      Func_02002604();
    if (entrance == 9) {
      Flag_Set(0x351);
      Func_02005560();
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
      Func_02002624(gData_02005c2c, gData_02005c32, Func_02001490, 88);
      Func_020027cc(0, 1);
    } else {
      Func_02002624(gData_02005c38, gData_02005c46, 0, 88);
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

s32 Func_0200140c(void) {
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
