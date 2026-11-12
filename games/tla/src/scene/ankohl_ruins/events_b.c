/// @file
/// Scene code of overlay 0x686, in ROM order, from events_b's offset up to the next object the overlay links.

#include "ankohl_ruins.h"
#include "version.h"
#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "call.h"
#include "field.h"

s32 Func_020005c0(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->priority = 2;
  object->priorityFlags &= 0xfe;
  object->motionFlags = 0;
  object->y -= 0x40000;
  return 0;
}

extern const u8 gData_020032a0[];
extern const u8 gData_020032b8[];
extern const u8 gData_020034f8[];
extern const u8 gData_02003570[];
extern const u8 gData_020035d0[];
extern const u8 gData_02003708[];
extern const u8 gData_02003750[];
extern const u8 gData_02003768[];

const u8 *Func_02000600(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    return gData_020032b8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    return gData_020034f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003570;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    return gData_020035d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003750;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return gData_02003768;
  return gData_020032a0;
}

void Func_02000698(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020006b4(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 0;
}

/// Copies the cells under the object with the given id in the rooms that have
/// them (rooms 1, 3, 4 and 5); in room 5 the object with id 9 also copies the
/// rows that flag 0x205 opens (always, in Japanese).
void Func_020006d0(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 4, 69);
      AnkohlRuins_Call2dfc(3, 68, 1, 1, 3, 70);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 19) {
      AnkohlRuins_Call2dfc(35, 72, 1, 1, 38, 72);
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 37, 73);
    }
    if (id == 8)
      AnkohlRuins_Call2dfc(2, 113, 1, 1, 3, 113);
    if (id == 18)
      AnkohlRuins_Call2dfc(39, 93, 1, 1, 40, 93);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(32, 69, 1, 1, 32, 70);
      AnkohlRuins_Call2dfc(33, 69, 1, 1, 33, 71);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5) {
    if (id == 8) {
      MapGrid_CopyCellAttributes(17, 85, 1, 1, 17, 83);
      AnkohlRuins_Call2dfc(19, 84, 1, 1, 18, 82);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 19, 83);
    }
#if !TLA_PATCH_1
    if (id == 9)
#else
    if (id == 9 && Event_GetFlag(0x205))
#endif
      AnkohlRuins_Call2df4(35, 98, 3, 5, 25, 98);
  }
}

/// Copies the cells the object with the given id has pushed open in rooms 1,
/// 3, 4 and 5, and sets flags 0x9c6 to 0x9cb when it stands at the end of its
/// track.
void Func_02000850(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1) {
    if (object->x >> 20 == 4) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 2, 69);
      Event_SetFlag(0x9c6);
    }
    AnkohlRuins_Call2dfc(3, 68, 1, 1, 4, 69);
    AnkohlRuins_Call2dfc(3, 69, 1, 1, 3, 70);
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3) {
    MapGrid_CopyCellAttributes(32, 71, 1, 1, 33, 71);
    AnkohlRuins_Call2dfc(31, 70, 1, 1, 32, 70);
    if (object->x >> 20 == 32) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
      Event_SetFlag(0x9c7);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(3, 114, 1, 1, 3, 113);
      if (object->x >> 20 == 2)
        Event_SetFlag(0x9c8);
    }
    if (id == 18) {
      AnkohlRuins_Call2dfc(40, 94, 1, 1, 40, 93);
      if (object->x >> 20 == 39)
        Event_SetFlag(0x9c9);
    }
    if (id == 19) {
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(37, 74, 1, 1, 37, 73);
      if (object->x >> 20 == 38) {
        MapGrid_CopyCellAttributes(37, 70, 1, 1, 37, 71);
        Event_SetFlag(0x9ca);
      }
    }
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5) {
    MapGrid_CopyCellAttributes(16, 83, 1, 1, 17, 83);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 18, 82);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 19, 83);
    if (object->x >> 20 == 17) {
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
      Event_SetFlag(0x9cb);
    }
  }
}

void Func_02000a68(s32 unused, s32 objectId) {
  s32 id = objectId;
  struct Actor *object = Actor_Get(id);
  s16 frames = 60;

  if (object->z >> 20 == 31) {
    object->motionFlags = 3;
    Func_02002e34(object, 3);
    Event_PlaySound(204);
    while (frames != 0) {
      System_Wait(1);
      if (object->velocityY == 0)
        break;
      frames--;
    }
    Actor_SetPosition(id, 0, 0);
    if (id == 10)
      Event_SetFlag(0x9d2);
    if (id == 11)
      Event_SetFlag(0x9d3);
    if (id == 12)
      Event_SetFlag(0x9d4);
    if (id == 13)
      Event_SetFlag(0x9d5);
    if (id == 14)
      Event_SetFlag(0x9d6);
    if (id == 15)
      Event_SetFlag(0x9d7);
    if (id == 16)
      Event_SetFlag(0x9d8);
  }
}

/// Runs the cue, if any, and sets the object's motion flags to 3 while it
/// moves, for at most 60 frames until its vertical speed is 0, then clears
/// them ten frames later; Func_02000100 of Gaia Rock is the same function.
void Func_02000b30(s32 id, s32 cue) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the flags' address built in r2 with 85 in it first; as a
   * plain pointer, 17 of 32 instructions differ in tla-en. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter in r6 and the object in r5; as a plain
   * counter, 12 of 32 instructions differ in tla-en. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): keeps 85 in a register, so it is added to the object
   * rather than the object copied and 85 added; without it, 4 of 32
   * instructions differ in tla-en. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): the address copied to r8 for the calls after the
   * counter starts; as one variable, 9 of 32 instructions differ in tla-en. */
  held = flags;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  *held = 0;
}

// Places count effects in a row, rows times, each row two pixels further down, waiting between rows.
struct AnkohlRuinsEffectParams {
  u32 unk00;
  s32 unk04;
  s32 unk08;
  s32 unk0c;
  u8 unk10[0x8];
  u16 kind;
  u8 unk1a[0xe];
};

void Func_0200015c(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct AnkohlRuinsEffectParams *params);

/// Places count effects in a row, rows times, each row two pixels further down,
/// waiting between rows.
void Func_02000b78(s32 rows, s32 frames, s32 x, s32 y, s32 startZ, s32 count) {
  struct AnkohlRuinsEffectParams params;
  s32 row;
  s32 left;
  s32 dx;
  s32 dy;
  s32 z;

  params.unk04 = 1;
  params.unk08 = 0xc000;
  params.unk0c = 0xc000;
  row = 0;
  if (row < rows) {
    z = startZ;
  next_row:
    for (left = 0; left < count; left++) {
      {
        s32 r = Random16Far();
        dx = 15;
        dx &= r;
      }
      dy = Random16Far() & 31;
      Func_0200015c(((x + left * 16) << 16) + ((dx - 8) << 16), (y + dy) << 16, z << 16, 0, 0,
                    0x100, 0x80001, &params);
    }
    FieldEvent_Wait(frames);
    z += 2;
    row++;
    if (row < rows)
      goto next_row;
  }
}

/// When the objects with ids 9 and 10 stand on cells (12, 45) and (14, 45),
/// each sets its flag (0x9d9, 0x9da); once both are set, shakes the screen,
/// copies the cells of rows 2 to 6 and 13 in two columns and has Func_02000b78
/// place five objects between them, as Func_02000e60 does for object 8.
void Func_02000c24(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 toColumn;
  s32 lowRow;
  s32 highRow;
  s32 rise;
  s32 kind;

  if (id == 9 && column == 12 && row == 45) {
    Event_SetFlag(0x9d9);
    Call2((void (*)())Func_02000b30, 9, 181);
  }
  if (id == 10 && column == 14 && row == 45) {
    Event_SetFlag(0x9da);
    Call2((void (*)())Func_02000b30, 10, 181);
  }
  if (!GameFlag_Test(0x9da) || !GameFlag_Test(0x9d9))
    return;
  Func_02002e44();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(770), 1);
  FieldEvent_Wait(15);
  Event_PlaySound(202);
  toColumn = 12;
  highRow = 111;
  MapGrid_CopyCellsLayered(12, 91, 3, 1, toColumn, highRow);
  FieldEvent_Wait(60);
  Event_PlaySound(0x23f);
  lowRow = 47;
  MapGrid_CopyCells(16, 27, 3, 2, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 2, toColumn, highRow);
  rise = -40;
  // The last argument is also kept for the later calls, as the native keeps
  // the 3 it stores.
  Func_02000b78(3, 15, 200, rise, 0x2e8, kind = 3);
  MapGrid_CopyCells(16, 27, 3, 3, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 3, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x2f8, kind);
  MapGrid_CopyCells(16, 27, 3, 4, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 4, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x308, kind);
  MapGrid_CopyCells(16, 27, 3, 5, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 5, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x318, kind);
  MapGrid_CopyCells(16, 27, 3, 6, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 6, toColumn, highRow);
  MapGrid_SetShake(-1, -1, 0xe666);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x328, kind);
  MapGrid_CopyCells(16, 27, 3, 13, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 13, toColumn, highRow);
  Func_02002e4c();
}

/// When the object with id 8 stands on cell (12, 5): sets flag 0x9dc, shakes
/// the screen, copies the cells of rows 2 to 6 and 16 in two columns and has
/// Func_02000b78 place four objects between them.
void Func_02000e60(s32 unused, s32 id) {
  struct Actor *pillar = Actor_GetById(id);
  s32 column = pillar->x >> 20;
  s32 row = pillar->z >> 20;

  if (id != 8 || column != 12 || row != 5)
    return;
  Event_SetFlag(0x9dc);
  Func_02000b30(8, 181);
  Func_02002e44();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(15);
  Event_PlaySound(202);
  AnkohlRuins_Call2df4(29, 70, 3, 1, 12, 71);
  AnkohlRuins_CamBounds(PIXELS(216), -1, PIXELS(120), 1);
  AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(60);
  Event_PlaySound(0x23f);
  MapGrid_CopyCells(35, 6, 7, 2, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 2, 10, 71);
  Func_02000b78(6, 10, 200, -40, 124, 3);
  MapGrid_CopyCells(35, 6, 7, 3, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 3, 10, 71);
  Func_02000b78(3, 10, 184, -40, 140, 5);
  MapGrid_CopyCells(35, 6, 7, 4, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 4, 10, 71);
  Func_02000b78(3, 10, 184, -40, 156, 5);
  MapGrid_CopyCells(35, 6, 7, 5, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 5, 10, 71);
  Func_02000b78(3, 10, 184, -40, 172, 5);
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  MapGrid_CopyCells(35, 6, 7, 6, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 6, 10, 71);
  FieldEvent_Wait(15);
  MapGrid_CopyCells(35, 6, 7, 16, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 16, 10, 71);
  Func_02002e4c();
}

void Func_0200104c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (id == 10 && x == 15 && z == 24) {
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
    object->y -= 0x20000;
    FieldEvent_Wait(1);
    object->y -= 0x20000;
    FieldEvent_Wait(1);
    object->y -= 0x40000;
    FieldEvent_Wait(1);
    object->y -= 0x80000;
    FieldEvent_Wait(1);
    Actor_SetAnimation(9, 3);
    Actor_SetPosition(10, 0, 0);
    Event_PlaySound(188);
    FieldEvent_Wait(45);
    Actor_SetAnimation(9, 4);
    Event_PlaySound(564);
    Event_SetFlag(0x9dd);
  }
}
