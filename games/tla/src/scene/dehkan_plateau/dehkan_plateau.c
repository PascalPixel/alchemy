/// @file
/// Scene code of overlay 0x655, in ROM order.

// ---- games/tla/src/scene/dehkan_plateau.c ----

#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_work.h"
#include "event.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_call.h"

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02002180(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Func_0200118c(void) {
  GameFlag_SetBit(0x209);
  Field_RequestDjinnBattle(16, 47);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_020045e0[];
extern const u8 gData_020045f8[];
extern const u8 gData_020047d8[];
extern const u8 gData_02004ac0[];
extern const u8 gData_02004d78[];
extern const u8 gData_02004dd8[];
extern const u8 gData_02004e20[];
extern const u8 gData_02004f10[];
extern const u8 gData_02004f1c[];
extern const u8 gData_02005078[];
extern const u8 gData_020051bc[];
extern const u8 gData_020053b4[];
extern const u8 gData_02005450[];
extern const u8 gData_020055b8[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    return gData_020045f8;
  if (scene == (s32)&SceneId_Ov655Room2)
    return gData_020047d8;
  if (scene == (s32)&SceneId_Ov655Room3)
    return gData_02004ac0;
  if (scene == (s32)&SceneId_Ov655Room4)
    return gData_02004d78;
  if (scene == (s32)&SceneId_Ov655Room5)
    return gData_02004dd8;
  if (scene == (s32)&SceneId_Ov655Room6)
    return gData_02004e20;
  return gData_020045e0;
}

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    return gData_02004f1c;
  if (scene == (s32)&SceneId_Ov655Room2)
    return gData_02005078;
  if (scene == (s32)&SceneId_Ov655Room3)
    return gData_020051bc;
  if (scene == (s32)&SceneId_Ov655Room4)
    return gData_020053b4;
  if (scene == (s32)&SceneId_Ov655Room5)
    return gData_02005450;
  if (scene == (s32)&SceneId_Ov655Room6)
    return gData_020055b8;
  return gData_02004f10;
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_020012c8(void) { Func_020002f8(gData_02003f44); }

void Func_020012d8(void) {
  if (GameFlag_Test(0x213) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 9, 18);
  if (GameFlag_Test(0x215) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 11, 18);
  if (GameFlag_Test(0x221) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 14, 28);
  if (GameFlag_Test(0x223) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 16, 28);
}

void Func_02001368(void) {
  s32 y;

  Func_02000708(gData_02003f52);
  Func_020012d8();
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  if (y < 0x80000) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_020013b0(void) {
  if (GameFlag_Test(0x213) == 0)
    Map_CopyAttributes(63, 0, 1, 1, 50, 20);
  if (GameFlag_Test(0x215) == 0)
    Map_CopyAttributes(63, 0, 1, 1, 52, 20);
}

void Func_020013fc(void) {
  Func_020002f8(gData_02003f80);
  Func_020013b0();
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -0x180000) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}

void Func_02001444(void) {
  s32 y;

  Func_020002f8(gData_02003f92);
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  if (y < 0x180000)
    Func_02002180();
}

void Func_02001514(s32 show, s32 first, u32 count);

void Func_02001474(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    Func_02001514(show, 13, 14);
  else if (scene == (s32)&SceneId_Ov655Room2)
    Func_02001514(show, 25, 13);
  else if (scene == (s32)&SceneId_Ov655Room3)
    Func_02001514(show, 26, 10);
  else if (scene == (s32)&SceneId_Ov655Room4)
    Func_02001514(show, 8, 3);
  else if (scene == (s32)&SceneId_Ov655Room6)
    Func_02001514(show, 10, 6);
}

void Func_020014ec(void) {
  Func_02001474(1);
  GameFlag_SetBit(0x200);
}

void Func_02001500(void) {
  Func_02001474(0);
  GameFlag_ClearBit(0x200);
}

DEFINE_FUNC_02001514(Func_02001514)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02001554(s32 a, s32 b) { Func_020000a4(gData_02003f98, a, b); }

void Func_02001568(s32 a, s32 b) { Func_020000a4(gData_02003fa0, a, b); }

void Func_0200157c(s32 a, s32 b) { Func_020000a4(gData_02003fa8, a, b); }

void Func_02001590(s32 a, s32 b) { Func_020000a4(gData_02003fb0, a, b); }

void Func_020015a4(s32 a, s32 b) { Func_020000a4(gData_02003fb8, a, b); }

DEFINE_FUNC_020004A4(Func_020015b8)

void Func_020015e0(void) {
  struct Actor *object = Actor_GetById(12);
  s32 x;

  x = object->x >> 20;
  if (x == 56 && (object->z >> 20) == 32) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(54, 32, 2, 1, x, 64);
    GameFlag_SetBit(0x301);
    Event_End();
  }
}

void Func_02001630(void) { Func_02003dc0(gData_02003fc0); }

void Func_02001640(void) {
  struct Actor *object = Actor_GetById(23);
  s32 x = object->x >> 20;

  Func_02003dc8();
  if (x == 52) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(51, 7, 1, 1, x, 7);
    wait(1);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(50, 7, 1, 1, x, 7);
    GameFlag_SetBit(0x840);
    Event_End();
  }
}

void Func_020016a0(void) { Func_02003dc0(gData_02003fc4); }

void Func_020016b0(void) {
  struct Actor *object = Actor_GetById(19);
  s32 x = object->x >> 20;

  Func_02003dc8();
  if (x == 47) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(47, 12, 1, 1, x, 13);
    wait(1);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(50, 16, 1, 1, x, 13);
    Actor_SetSpritePriority(19, 2);
    Event_End();
  } else if (x == 46) {
    Map_CopyAttributes(44, 13, 1, 1, 48, 13);
  }
}

static __inline__ void Actor_PlaceOnTerrain(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_0200172c(void) {
  Actor_PlaceOnTerrain(20, 150 << 18, 134 << 18);
  Map_CopyAttributes(38, 32, 1, 1, 37, 33);
  Map_CopyAttributes(30, 32, 1, 2, 38, 32);
  Map_CopyAttributes(40, 64, 4, 3, 36, 64);
  GameFlag_SetBit(0x841);
}

void Func_02001788(void) {
  if ((Actor_GetById(20)->x >> 20) == 37)
    Func_0200172c();
}

void Func_020017a0(void) {
  u8 *flags;

  if ((Actor_GetById(20)->x >> 20) == 38) {
    flags = &Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags;
    Event_Start();
    Event_ResolvePendingActions(0);
    *flags = 0;
    wait(1);
    Func_02003de8();
    Func_0200172c();
    wait(1);
    *flags = 3;
    Event_End();
  }
}

void Func_020017f4(void) { GameFlag_ClearBit(0x201); }

void Func_02001804(void) {
  s32 flag;

  flag = 0x320;
loop:
  GameFlag_ClearBit(flag);
  if (flag != 0x35f) {
    flag++;
    goto loop;
  }
}

DEFINE_FUNC_02001820(Func_02001820, gData_02004066, 13, gData_02003fd2)

extern const u16 gData_02003fda[];
extern const struct CellRecord gData_0200410e[];

DEFINE_FUNC_02001820(Func_020018a8, gData_0200410e, 7, gData_02003fda)

extern const u16 gData_02003fe2[];
extern const struct CellRecord gData_0200416e[];

DEFINE_FUNC_02001820(Func_02001930, gData_0200416e, 5, gData_02003fe2)

void Func_020019b8(void) {
  u16 *record = gData_020041b6;
  s32 flag = 0x240;
  u32 i;

  for (i = 0; i <= 15; flag++, i++) {
    if (GameFlag_Test(0x109)) {
      if (GameFlag_Test(flag)) {
        s32 x;
        s32 y;

        record = &gData_020041b8[i * 6];
        x = *record++;
        y = *record++;
        Map_CopyCellPictureIndicesRect(gData_02003fea[0], gData_02003fea[1], x,
                                       y, 1, 1);
      }
    } else {
      if (*record != 0)
        GameFlag_SetBit(flag);
      record += 6;
    }
  }
}

void Func_020006ec(struct Actor *object, struct Actor *other);
void Actor_StartScript(struct Actor *object, const void *script);
void Actor_MoveToObject(s32 id, s32 target);

extern struct Actor *gData_02004420;
extern s32 gData_02005678;

#define SCENE(name) ((s32)&SceneId_##name)

// Steps onto a listed cell in rooms 1, 2, 3 and 5 (SceneId_Ov655Room1 to
// Room5): either falls through to exit work->exit or opens the cell and sets
// its flag. cellX is reused for the doubled index and the fall's frame count.
void Func_02001a30(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 cellX = ((leader->x >> 20) << 1) + 1;
  s32 cellZ = ((leader->z >> 20) << 1) + 1;
  s32 n, flag;
  u16 *record, *next;

  if (gMainRoutineWork.scene == SCENE(Ov655Room1)) {
    n = work->exit - 11;
    flag = work->exit + 0x235;
    record = &gData_02004068[n * 6];
  } else if (gMainRoutineWork.scene == SCENE(Ov655Room2)) {
    n = work->exit - 14;
    flag = work->exit + 0x232;
    record = &gData_02004110[n * 6];
  } else if (gMainRoutineWork.scene == SCENE(Ov655Room3)) {
    n = work->exit - 12;
    flag = work->exit + 0x234;
    record = &gData_02004170[n * 6];
  } else if (gMainRoutineWork.scene == SCENE(Ov655Room5)) {
    n = work->exit - 18;
    flag = work->exit + 0x22e;
    record = &gData_020041b8[n * 6];
    if (GameFlag_Test(0x33a))
      gData_020041b8[26] = 3;
  } else {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, cellX << 3,
                          cellZ << 3);
  Actor_SetPositionAndResetMotion(leader, cellX << 19, leader->y, cellZ << 19);
  Event_Wait(4);
  FieldCamera_GetActor()->y += -0x80000;
  next = record + 1;
  if (GameFlag_Test(flag)) {
    s32 x, y, index, toX, toZ;

    Sound_Play(229);
    x = *record;
    record = next;
    {
      s16 scene = gMainRoutineWork.scene;
      y = *record++;
      if (scene == SCENE(Ov655Room1))
        Map_CopyCellPictureIndicesRect(gData_02003fd2[2], gData_02003fd2[3], x,
                                       y, 1, 1);
      else if (scene == SCENE(Ov655Room2))
        Map_CopyCellPictureIndicesRect(gData_02003fda[2], gData_02003fda[3], x,
                                       y, 1, 1);
      else if (scene == SCENE(Ov655Room3))
        Map_CopyCellPictureIndicesRect(gData_02003fe2[2], gData_02003fe2[3], x,
                                       y, 1, 1);
      else if (scene == SCENE(Ov655Room5))
        Map_CopyCellPictureIndicesRect(gData_02003fea[2], gData_02003fea[3], x,
                                       y, 1, 1);
    }
    index = *record++;
    cellX = index * 2;
    toX = record[0];
    toZ = record[1];
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room1)) {
        {
          s32 srcX = gData_02003ff2[cellX], srcY = gData_02003ff2[cellX + 1],
              width = 1, height = 2;
          Map_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width, height);
        }
      } else if (scene == SCENE(Ov655Room2)) {
        {
          s32 srcX = gData_02004012[cellX], srcY = gData_02004012[cellX + 1],
              width = 1, height = 2;
          Map_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width, height);
        }
      } else if (scene == SCENE(Ov655Room3)) {
        {
          s32 srcX = gData_02004032[cellX], srcY = gData_02004032[cellX + 1],
              width = 1, height = 2;
          Map_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width, height);
        }
      } else if (scene == SCENE(Ov655Room5)) {
        if ((n == 0 && GameFlag_Test(0x241)) ||
            (n == 11 && GameFlag_Test(0x24c))) {
          s32 srcX = gData_02004052[8], srcY = gData_02004052[9], width = 1,
              height = 2;
          Map_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width, height);
        } else {
          s32 srcX = gData_02004052[cellX], srcY = gData_02004052[cellX + 1],
              width = 1, height = 2;
          Map_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width, height);
        }
      }
    }
    Event_Wait(12);
    leader->facing = 0x4000;
    leader->motionFlags = 0;
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room1)) {
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
      } else if (scene == SCENE(Ov655Room2) || scene == SCENE(Ov655Room3)) {
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      } else if (scene == SCENE(Ov655Room5)) {
        if (gData_02004420 != NULL)
          ((struct FieldSprite *)gData_02004420->sprite)->priority = 3;
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    Event_Wait(16);
    for (cellX = 0; (u32)cellX <= 39; cellX++) {
      if (cellX == 5)
        Sound_Play(204);
      leader->scaleX += -0x400;
      leader->scaleY += -0x300;
      leader->y += -0x9999;
      wait(1);
    }
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->exit);
  } else {
    s32 x, y;

    Sound_Play(161);
    y = *next;
    x = *record;
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room1))
        Map_CopyCellPictureIndicesRect(gData_02003fd2[0], gData_02003fd2[1], x,
                                       y, 1, 1);
      else if (scene == SCENE(Ov655Room2))
        Map_CopyCellPictureIndicesRect(gData_02003fda[0], gData_02003fda[1], x,
                                       y, 1, 1);
      else if (scene == SCENE(Ov655Room3))
        Map_CopyCellPictureIndicesRect(gData_02003fe2[0], gData_02003fe2[1], x,
                                       y, 1, 1);
      else if (scene == SCENE(Ov655Room5))
        Map_CopyCellPictureIndicesRect(gData_02003fea[0], gData_02003fea[1], x,
                                       y, 1, 1);
    }
    Event_Wait(12);
    GameFlag_SetBit(flag);
    GameFlag_SetBit(0x201);
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room5) || scene == SCENE(Ov655Room3)) {
        gData_0200567c = leader->x >> 20;
        gData_02005678 = leader->z >> 20;
      }
    }
    Event_End();
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

void Func_02001ee8(void) {
  struct Actor *object;

  if (Flag_Test(0x201) != 0) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (gData_0200567c != (object->x >> 20) ||
        gData_02005678 != (object->z >> 20))
      GameFlag_ClearBit(0x201);
  }
}

DEFINE_FUNC_02001F38(Func_02001f38, Func_02003cc0)

void Func_02001f68(void) {
  if (GameFlag_Test(0x109) != 0) {
    if (GameFlag_Test(0x200) == 0)
      Func_02001500();
  } else {
    GameFlag_SetBit(0x200);
  }
}

DEFINE_FUNC_02001F94(Func_02001f94, 13, Func_02001f38, 13, Func_02001f68)

DEFINE_FUNC_02001F94(Func_02001fac, 12, Func_02001f38, 25, Func_02001f68)

DEFINE_FUNC_02001F94(Func_02001fc4, 9, Func_02001f38, 26, Func_02001f68)

DEFINE_FUNC_02001F94(Func_02001fdc, 2, Func_02001f38, 8, Func_02001f68)

DEFINE_FUNC_02001F94(Func_02001ff4, 5, Func_02001f38, 10, Func_02001f68)

void Func_02000e9c(struct Actor *object);

void Func_0200200c(struct Actor *object) {
  s32 x;
  u8 *cell;

  Func_02000e9c(object);
  x = object->x >> 20;
  if (x == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(0, 32, 1, 1, x, 11);
    wait(1);
    Func_020015b8(object);
    Map_CopyCellAttributeRect(1, 32, 1, 1, x, 11);
    Event_End();
  } else if (x == 18) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Func_02003db0(object) * 4;
    cell[3] &= 0x7f;
  } else if (x == 54) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Func_02003db0(object) * 4;
    cell[3] &= 0x7f;
    cell[2] = 11;
  } else if (x == 55) {
    Map_CopyAttributes(53, 20, 1, 1, 54, 20);
    Map_CopyCellAttributeRect(57, 20, 1, 1, 56, 20);
  }
}

void Func_02000d78(const void *data);
extern const u8 gData_02003fc8[];

DEFINE_FUNC_020008F4(Func_020020dc, Func_02003de0, Func_02003d10, Func_02000d78,
                     gData_02003fc8, Func_02003de8, Func_0200200c)

void Func_02002114(void) { Func_02000d78(gData_02003fc8); }

void Func_02002124(void) {
  struct EventWorkCbc *work = Ram_HeapSlots->eventWork;
  struct Actor *active;

  active = work->active;
  if (active != 0)
    Func_0200200c(active);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02002140(void) {
  s32 set = GameFlag_Test(0x207);

  if (set == 0) {
    Actor_GetById(8)->motionFlags = set;
    Map_CopyAttributes(51, 12, 1, 1, 47, 12);
    GameFlag_SetBit(0x202);
  }
}

void Func_02002180(void) {
  if (GameFlag_Test(0x207) == 0) {
    Map_CopyAttributes(48, 11, 1, 1, 47, 12);
    GameFlag_ClearBit(0x202);
  }
}

void Func_020021b4(void) {
  struct Actor *object = FieldCamera_GetActor();
  u32 i;

  object->y += -0x80000;
  wait(1);
  for (i = 0; i <= 7; i++) {
    object->y += 0x10000;
    wait(1);
  }
}
#undef SCENE

// ---- games/tla/src/scene/dehkan_plateau/events_g.c ----
// Scene code of overlay 0x655, in ROM order, from events_g's offset up to the
// next object the overlay links.

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02002180(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                    s32 width, s32 height);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

/* VIEW(events_g): routine Actor_SetHorizontalPositionAndAngleWithTerrain as
 * void Actor_SetHorizontalPositionAndAngleWithTerrain(u32 id, s32 x, s32 z, s16
 * angle); event.h has void Actor_SetHorizontalPositionAndAngleWithTerrain(s32
 * actorId, s32 x, s32 z, s32 angle); tried as the definition's void
 * Actor_SetHorizontalPositionAndAngleWithTerrain(u32 objectId, s32 x, s32 z,
 * s16 angle), it does not compile in tla-ja:
 * games/tla/src/scene/dehkan_plateau.c:832: conflicting types for
 * `Actor_SetHorizontalPositionAndAngleWithTerrain'. */
extern void Actor_SetHorizontalPositionAndAngleWithTerrain__events_g(
    u32 id, s32 x, s32 z,
    s16 angle) __asm__("Actor_SetHorizontalPositionAndAngleWithTerrain");
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
/* VIEW(events_g): routine Task_Add as void Task_Add(void(*func) (void), s32
 * priority); task.h has s32 Task_Add(TaskFunc func, s32 order); tried as the
 * definition's s32 Task_Add(TaskFunc func, s32 order), which the object already
 * declares, Func_02002604 moves: 1 of 20 instructions in tla-ja. */
extern void Task_Add__events_g(void (*func)(void),
                               s32 priority) __asm__("Task_Add");
void Task_RemoveFar(void (*func)(void));

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_MoveTo(s32 id, s32 x, s32 z) {
  Actor_WalkToAndWaitById(id, x, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain__events_g(id, x, z, angle);
}

void Func_020021e8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  Camera_MoveWithinMapBounds(658 << 16, -1, 164 << 16, 1);
  Func_02003cf0();
  Actor_FaceActorById(24, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(24, 0x100, 40);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 664, 136);
  Map_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
  Sound_Play(161);
  Func_020021b4();
  GameFlag_SetBit(0x247);
  Actor_MoveTo(24, 664, 104);
  Map_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(24, 6, 16);
  Actor_PlaceFacing(24, 222 << 18, 168 << 16, FACING_SOUTH);
  Event_Wait(20);
  GameFlag_SetBit(0x86c);
  Event_End();
}

void Func_020022e0(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(245 << 18, -1, 168 << 16, 1);
  Func_02003cf0();
  Actor_FaceActorById(24, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(24, 0x100, 40);
  Actor_GetById(24)->motionFlags = 0;
  Actor_SetSpritePriority(24, 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 940, 168);
  Actor_MoveTo(24, 984, 152);
  Actor_MoveTo(24, 1040, 152);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x86d);
  Event_End();
}

void Func_02002390(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(144 << 16, -1, 214 << 15, 1);
  Func_02003cf0();
  Actor_FaceActorById(25, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(25, 0x100, 40);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWaitById(25, 168, 104);
  Event_Wait(10);
  Sound_Play(153);
  Actor_GetById(25)->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWaitById(25, 200, 104);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Actor_MoveTo(25, 294, 104);
  Actor_PlaceFacing(25, 212 << 17, 208 << 15, FACING_SOUTH);
  GameFlag_SetBit(0x86e);
  Event_End();
}

void Func_02002458(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  FieldCamera_GetActor()->motionFlags = 0;
  Camera_SetBounds(212 << 17, -1, 208 << 15, 1);
  Func_02003cf0();
  Actor_FaceActorById(25, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(25, 0x100, 20);
  Camera_MoveWithinMapBounds(212 << 17, -1, 152 << 16, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_MoveTo(25, 424, 152);
  Actor_ArmCallback(25, FACING_EAST, 20);
  Func_02002604();
  Actor_MoveTo(25, 440, 152);
  Actor_MoveTo(25, 456, 168);
  Actor_MoveTo(25, 456, 184);
  GameFlag_SetBit(0x241);
  Map_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
  Sound_Play(161);
  Func_020021b4();
  Actor_MoveTo(25, 456, 216);
  Actor_MoveTo(25, 584, 216);
  Actor_PlaceFacing(25, 154 << 18, 140 << 17, FACING_SOUTH);
  GameFlag_SetBit(0x86f);
  Event_End();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02002574(void) {
  struct EffectWork7c4 *work = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(18);

  if (work->timer != -1 && work->timer <= 7)
    object->scaleY += -0x1f00;
  if (work->active != 0) {
    Map_CopyAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    ((u8 *)object)[89] &= 0xfe;
    object->scaleY = 0x10000;
    GameFlag_SetBit(0x205);
  }
}

void Func_02002604(void) {
  BattleEffect_LoadActionEffectResources(134, 1);
  Task_Add__events_g(Func_02002574, 0x480);
  FieldPsynergy_SetupActorPair(25, 18);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_RemoveFar(Func_02002574);
  BattleEffect_CleanupSceneObjects();
}
