/// @file
/// Scene code of overlay 0x686, in ROM order, from s160c's offset up to the next object the overlay links.

#include "ankohl_ruins.h"
#include "map.h"
#include "scene_tables.h"
#include "task.h"
#include "version.h"
#include "frame.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

extern struct AnkohlRuinsFloatWork Data_02004030;
struct AnkohlRuinsWaterRow {
  u8 level;
  u8 width;
  u8 height;
  u8 x;
  u8 z;
  u8 unk05[3];
};

extern struct AnkohlRuinsWaterRow gData_02002fa8[];

void Func_020015ac(void);
void Func_0200160c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  s16 i;
  s16 j;
  s16 m;
  s32 row;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  m = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Event_PlaySound(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 1);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (m == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 11, 68);
      AnkohlRuins_Call2dfc(10, 5, 1, 1, 11, 5);
    }
    if (m == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 11, 68);
  }
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782 = 0;
    Task_AddFrame(Func_020015ac, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, 5);
    Event_SetFlag(0x204);
    Event_ClearFlag(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gData_02002fa8[row].level != 99; row++) {
    if (i >= gData_02002fa8[row].level) {
      for (m = 0; m < gData_02002fa8[row].height; m++) {
        for (n = 0; n < gData_02002fa8[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, gData_02002fa8[row].x + n,
                               gData_02002fa8[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, gData_02002fa8[row].x + n,
                               gData_02002fa8[row].z + m + 64);
        }
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 7);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 7);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 5);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 5);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 71);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 71);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 69);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 69);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 8, 1, 1, 11, 5);
}

extern struct AnkohlRuinsWaterRow gData_02002ff0[];

void Func_020019a0(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  s32 level;
  s16 i;
  s16 j;
  s32 row;
  s16 m;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  level = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  Event_PlaySound(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 19);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 4, 86);
      AnkohlRuins_Call2dfc(4, 24, 1, 1, 4, 23);
    }
    if (level == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 4, 86);
  }
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782 = 0;
    Task_AddFrame(Func_020015ac, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, 23);
    Event_SetFlag(0x204);
    Event_ClearFlag(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 19);
  i = work->level >> 19;
  for (row = 0; gData_02002ff0[row].level != 99; row++) {
    if (i >= gData_02002ff0[row].level) {
      for (m = 0; m < gData_02002ff0[row].height; m++) {
        for (n = 0; n < gData_02002ff0[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, gData_02002ff0[row].x + n,
                               gData_02002ff0[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, gData_02002ff0[row].x + n,
                               gData_02002ff0[row].z + m + 64);
        }
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 27);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 13, 25);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 23);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 23);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 91);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 13, 89);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 87);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 87);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 7, 1, 1, 4, 23);
}

void Func_02001d34(void) {
  s32 done = AnkohlRuins_FlagCheck(0x202);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(584);
    FieldEvent_Wait(15);
    Event_PlaySound(202);
    MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gData_02003780 = 7;
    gData_02003782 = done;
    gData_02003784 = 200;
    gData_02003788 = 0x8000;
    gData_0200378c = 1;
    Event_SetFlag(381);
    Func_0200160c();
    Event_SetFlag(0x202);
  }
}

void Func_02001ddc(void) {
  if (AnkohlRuins_FlagCheck(0x9e2) == 0) {
    Event_SetFlag(0x9e2);
    Event_PlaySound(584);
    Func_02002e44();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_CopyCells(26, 70, 1, 3, 14, 65);
    FieldEvent_Wait(20);
    Event_PlaySound(181);
    MapGrid_CopyCells(26, 67, 1, 3, 14, 65);
    MapGrid_CopyCellAttributes(15, 5, 1, 1, 14, 5);
    Func_02002e4c();
  }
}

void Func_02001e54(void) {
  s32 done = AnkohlRuins_FlagCheck(0x203);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(584);
    FieldEvent_Wait(15);
    Event_PlaySound(202);
    AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gData_02003780 = 3;
    gData_02003782 = done;
    gData_02003784 = 100;
    gData_02003788 = 0x8000;
    gData_0200378c = 2;
    Event_SetFlag(381);
    Func_020019a0();
    Event_SetFlag(0x203);
  }
}

void Func_02001f04(void) {
  if (AnkohlRuins_FlagCheck(0x9e3) == 0) {
    Event_SetFlag(0x9e3);
    Event_PlaySound(584);
    Func_02002e44();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_CopyCells(26, 70, 1, 3, 2, 83);
    FieldEvent_Wait(20);
    Event_PlaySound(181);
    MapGrid_CopyCells(26, 67, 1, 3, 2, 83);
    MapGrid_CopyCellAttributes(1, 23, 1, 1, 2, 23);
    Func_02002e4c();
  }
}

/// Each frame, moves the raft (object 9) down by 0xcccc and carries the leader
/// and the event work's unk1cc object with it while the leader stands on it;
/// copies the cells it passes in rows 31 to 37 (flag 0x146), and past row 41
/// removes itself, stops the shaking, copies the landing cells and sets flag
/// 0x201.
void Func_02001f7c(void) {
  struct Actor *rider =
      ((struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork)->unk1cc;
  struct Actor *raft = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  s32 column;
  s32 row;

  if (GameFlag_Test(0x104) && work->unk154 != 40)
    return;
  raft->z += 0xcccc;
  column = leader->x >> 20;
  if (gMainRoutineWork.renderMode != 5 && column > 12 && column <= 15) {
    /* FAKEMATCH(temporary): a second pointer to the leader loads the raft's
     * z before the leader's in the second test; as leader, 16 of 167
     * instructions differ in tla-en. */
    struct Actor *player = leader;

    if (raft->z + 0x180000 >= player->z && player->z >= raft->z) {
      player->z = raft->z + 0x180000;
      rider->z = raft->z + 0x180000;
    }
    if (raft->z >= player->z && player->z >= raft->z - 0x100000) {
      player->z = raft->z - 0x100000;
      rider->z = raft->z - 0x100000;
    }
  }
  row = raft->z >> 20;
  if ((u16)(row - 31) <= 4 && !AnkohlRuins_FlagCheck(0x146)) {
    AnkohlRuins_Call2dfc(35, 83, 5, 4, 12, 96);
    Event_SetFlag(0x146);
  }
  if (row == 37 && AnkohlRuins_FlagCheck(0x146)) {
    AnkohlRuins_Call2dfc(40, 83, 5, 4, 12, 96);
    Event_ClearFlag(0x146);
  }
  if (row > 41) {
    FieldTask_Remove(Func_02001f7c);
    AnkohlRuins_Call2e24(-1, -1, 0xe666);
    AnkohlRuins_Call2dfc(41, 48, 3, 2, 13, 41);
    Event_SetFlag(0x201);
    Event_ClearFlag(0x17d);
    Event_ClearFlag(0x161);
    Event_PlaySound(208);
  }
}

void Func_020020fc(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    s32 found = Func_02002dcc(Func_02001f7c);

    if (found == -1) {
      Func_02002e44();
      FieldEvent_ResolvePendingActions(0);
      Event_PlaySound(207);
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(10);
      FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(392), 1);
      Func_02002eb4();
      Task_AddFrame(Func_02001f7c, TASK_PASS_VBLANK);
      FieldEvent_Wait(40);
      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Func_02002eb4();
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      Event_SetFlag(381);
      Event_SetFlag(353);
      Func_02002e4c();
    }
  }
}

void Func_020021d4(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    void (*func)(void) = Func_02001f7c;

    if (Func_02002dcc(func) == -1) {
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      Task_AddFrame(func, TASK_PASS_VBLANK);
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      Event_SetFlag(381);
      Event_SetFlag(353);
      Func_02002e4c();
    }
  }
}

struct AnkohlRuinsEffectParams {
  u8 unk00[0x18];
  u16 kind;
  u8 unk1a[0xe];
};

extern const u8 gData_02003790[];
extern const u8 gData_020037b4[];
extern const u8 gData_02003a78[];
extern const u8 gData_02003b98[];
extern const u8 gData_02003c94[];
extern const u8 gData_02003e38[];
extern const u8 gData_02003f4c[];
extern const u8 gData_02004018[];

void Func_0200015c(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct AnkohlRuinsEffectParams *params);
void Func_02002ee4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

void Func_02002260(void) {
  struct Actor *object = Actor_Get(9);

  if (object->x >> 20 == 26 && object->z >> 20 == 35) {
    Event_SetFlag(0x9db);
    Event_PlaySound(204);
    Func_02000b30(9, 0);
    Actor_SetPosition(9, 0, 0);
    Func_02002ee4();
    FieldEvent_SetExitRequest(20);
  }
#if !TLA_PATCH_1 /* The Japanese edition copies cells here. */
  Map_CopyCells(40, 98, 3, 5, 25, 98);
#endif
}

void Func_020022a8(void) {
#if TLA_PATCH_1 /* The Japanese edition does not touch flag 0x205. */
  Event_SetFlag(0x205);
#endif
  MapGrid_CopyCells(35, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(35, 98, 3, 3, 25, 98);
}

void Func_020022e0(void) {
#if TLA_PATCH_1
  Event_ClearFlag(0x205);
#endif
  MapGrid_CopyCells(40, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(40, 98, 3, 3, 25, 98);
}

void Func_02002318(void) { GameFlag_SetBit(0x205); }

void Func_02002328(void) { GameFlag_ClearBit(0x205); }

const u8 *Func_02002338(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    return gData_020037b4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    return gData_02003a78;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003b98;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    return gData_02003c94;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003e38;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003f4c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return gData_02004018;
  return gData_02003790;
}

void Func_020023d0(struct Actor *object) {
  struct AnkohlRuinsEffectParams params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      /* FAKEMATCH(temporary): the mask in a local, which the ROM builds before
       * the random draw; as a constant in the expression, 7 of 46 instructions
       * differ in tla-en. */
      s32 mask = 15;

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.kind = 673;
    Func_0200015c(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}
