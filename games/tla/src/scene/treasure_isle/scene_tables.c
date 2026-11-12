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

#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_02001330[];
extern const u8 gData_02001390[];
extern const u8 gData_02001480[];
extern const u8 gData_02001504[];
extern const u8 gData_0200169c[];

const u8 *Func_02000350(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001390;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_02001480;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_02001504;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_0200169c;
  return gData_02001330;
}

#include "ram_buffer.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

s32 Func_020003b4(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}

#include "event_inline.h"

/// One entry of the cell lists Func_020005ac walks.
struct TreasureIsleCell {
  s32 x; ///< in 16-pixel cells
  s32 z; ///< in 16-pixel cells
  s32 unk08;
  u16 flag; ///< set once the cell has been opened (Func_020003fc)
  u16 unk0e;
};

void Func_020003fc(struct TreasureIsleCell *cells, s16 index) {
  MapGrid_CopyCellsLayered(0, 0x64, 3, 4, cells[index].x + 0x3f,
                           cells[index].z - 2);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, cells[index].x, cells[index].z);
  Event_SetFlag(cells[index].flag);
}

#include "field_effect.h"

extern const s32 gData_02001128[];

void Func_02001014(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CameraBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 13
   * of 163 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetFade(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 11
   * of 163 instructions differ in tla-en. */
  Map_SetScreenShake(a, b, c);
}

static __inline__ void SetCells(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 24
   * of 163 instructions differ in tla-en. */
  Func_0802b450(a, b, c, d, x, z);
}

/// Called by Func_020005ac for each listed cell whose flag is still clear:
/// points the camera at the cell, bursts three effects there, changes its
/// cell pictures twice and hands over to Func_020003fc, which sets its flag.
void Func_0200043c(struct TreasureIsleCell *cells, s16 index) {
  struct TreasureIsleCell *cell;
  struct EffectOptions options;

  FieldCamera_SetSpeed(0x60000, 0xc000);
  // As &cells[index], the sum takes its operands the other way round.
  cell = (struct TreasureIsleCell *)((index << 4) + (s32)cells);
  CameraBounds((cell->x << 20) + 0x80000, -1, (cell->z << 20) + 0x80000, 1);
  Func_02001014();
  SetFade(0, 0x100000, 0x10000);
  SetFade(-1, -1, 0xe666);
  Event_PlaySound(567);
  options.startScaleX = 0x13333;
  options.startScaleY = 0x18000;
  options.priority = 1;
  options.type = 0x115;
  options.script = gData_02001128;
  Func_0200015c((cell->x << 20) + 0x80000, 0, (cell->z << 20) + 0x140000, 0, 0,
                0, 0x3a0000, &options);
  options.startScaleX = 0x9999;
  Func_0200015c((cell->x << 20) - 0x20000, 0, (cell->z << 20) + 0x80000, 0, 0,
                0, 0x3a0000, &options);
  Func_0200015c((cell->x << 20) + 0x120000, 0, (cell->z << 20) + 0x80000, 0, 0,
                0, 0x3a0000, &options);
  FieldEvent_Wait(10);
  SetCells(6, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  SetCells(3, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  Func_020003fc(cells, index);
}

#include "field_event.h"

// Import stubs: Event_Start and Event_End, and the pending
// actions' resolution.
void Func_02000fcc(void);
void Func_02000fd4(void);

extern struct TreasureIsleCell gData_0200117c[];
extern struct TreasureIsleCell gData_020011cc[];

/// For each of cells 0 to 3 of gData_0200117c whose flag (0x303 to 0x306) is
/// still clear, calls Func_0200043c and sets the flag; the event is
/// begun before the first and ended after the last.
void Func_020005ac(void) {
  s16 count = 0;

  if (!Event_GetFlag(0x303)) {
    Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 0);
    Event_SetFlag(0x303);
    count = 1;
  }
  if (!Event_GetFlag(0x304)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 1);
    Event_SetFlag(0x304);
    count++;
  }
  if (!Event_GetFlag(0x305)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 2);
    Event_SetFlag(0x305);
    count++;
  }
  if (!Event_GetFlag(0x306)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 3);
    Event_SetFlag(0x306);
    count++;
  }
  if (count != 0)
    Func_02000fd4();
}

/// For each of cells 0 to 2 of gData_020011cc whose flag (0x30a to 0x30c) is
/// still clear, calls Func_0200043c and sets the flag; the event is
/// begun before the first and ended after the last.
void Func_02000684(void) {
  s16 count = 0;

  if (!Event_GetFlag(0x30a)) {
    Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_020011cc, 0);
    Event_SetFlag(0x30a);
    count = 1;
  }
  if (!Event_GetFlag(0x30b)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_020011cc, 1);
    Event_SetFlag(0x30b);
    count++;
  }
  if (!Event_GetFlag(0x30c)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_020011cc, 2);
    Event_SetFlag(0x30c);
    count++;
  }
  if (count != 0)
    Func_02000fd4();
}

void Func_02000728(void) {
  if (!Event_GetFlag(0x307)) {
    Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 4);
    Event_SetFlag(0x307);
    Func_02000fd4();
  }
}

/// Called through the scene table with an object id: in each of the first
/// four rooms, sets a flag when object 8 to 11 is at a given x; object 11 at
/// a given x and z in the second room runs Func_02000728 instead.
void Func_02000760(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom1) {
    if (id == 8 && object->x >> 20 == 16)
      Event_SetFlag(0x300);
    if (id == 9 && object->x >> 20 == 20)
      Event_SetFlag(0x301);
    if (id == 10 && object->x >> 20 == 27)
      Event_SetFlag(0x302);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom2) {
    if (id == 10 && object->x >> 19 == 33)
      Event_SetFlag(0x308);
    if (id == 11 && object->x >> 19 == 85 && object->z >> 19 == 83)
      Func_02000728();
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom3) {
    if (id == 10 && object->x >> 19 == 83)
      Event_SetFlag(0x309);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom4) {
    if (id == 8 && object->x >> 19 == 87)
      Event_SetFlag(0x30d);
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000864(void) { Field_RequestDjinnBattle(0x17, 0x4d); }

void Func_02000870(void) { Djinn_JoinParty(9, 0, 0x13); }

void Func_02000880(s32 unused, s32 frame, s32 id) {
  struct TreasureIsleCell *cells;

  if (id < 50) {
    cells = gData_0200117c;
    id -= 40;
  } else {
    cells = gData_020011cc;
    id -= 50;
  }
  if (frame == 15) {
    MapGrid_CopyCellsLayered(3, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
  } else if (frame == 30) {
    MapGrid_CopyCellsLayered(6, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
  } else if (frame == 45) {
    MapGrid_CopyCellsLayered(9, 100, 3, 4, cells[id].x + 63, cells[id].z - 2);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, cells[id].x, cells[id].z);
    Event_ClearFlag(cells[id].flag);
  }
}

void Func_02000914(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x200);
}

void Func_02000940(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(10));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x201);
}

void Func_02000970(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(11));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x202);
}

void Func_020009a0(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(12));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x203);
}

void Func_020009d0(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(13));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x204);
}

void Func_020009fc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(14));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x205);
}

void Func_02000a2c(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(15));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x206);
}

void Func_02000a5c(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(16));
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  Event_SetFlag(0x207);
}

#include "call.h"
s32 Func_0200101c();
extern u8 MsgIAmTheGuardian[];

void Func_02000a8c(void) {
  s32 t12;

  Func_02000fcc();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_StartRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgIAmTheGuardian, 0, 6);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  t12 = Actor_Get(gMainRoutineWork.currentOwner);
  if (t12 != 0) {
    Actor_SetDestination(8, FIELD_AT(t12, s16, 10), FIELD_AT(t12, s16, 18));
  }
  FieldEvent_Wait(20);
  gMainRoutineWork.pendingDjinnEvent = 0x19bb;
  gMainRoutineWork.battleRule24b = 2;
  Call2(Func_0200101c, 0x67, 0);
}

extern const u8 gData_020016e4[];
extern const u8 gData_02001780[];
extern const u8 gData_0200184c[];
extern const u8 gData_020018f4[];
extern const u8 gData_02001a5c[];

const u8 *Func_02000b3c(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001780;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_0200184c;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_020018f4;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_02001a5c;
  return gData_020016e4;
}

#include "iwram_call.h"
#include "io_write_queue.h"

struct TreasureIsleEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

extern s32 gData_02001aa4;
extern u16 Data_02001aa8[];
extern volatile u16 gData_02001ac8[];
extern volatile u16 Data_02001ae8[];
extern u16 Data_02001b08[];

void Func_02000ba0(void) {
  struct TreasureIsleEventWork *work = Ram_HeapSlots->eventWork;
  s32 phase;
  s32 i;
  s32 busy;

  if (work->unk166 != 0)
    return;
  busy = (s8)work->unkca4;
  if (busy != 0)
    return;
  if (gData_02001aa4 == -1) {
    Call3((void (*)())Iwram_CopyWords, Data_02001aa8, (void *)0x05000060, 32);
    Iwram_CopyWords((void *)gData_02001ac8, Data_02001aa8, 32);
    Iwram_CopyWords((void *)Data_02001ae8, (void *)0x05000140, 32);
    Iwram_CopyWords(Data_02001b08, (void *)Data_02001ae8, 32);
    gData_02001aa4 = 0;
  }
  /* FAKEMATCH(volatile): two of the colour arrays read and written as volatile,
   * which keeps the second read after the first store; as plain u16, 6 of 152
   * instructions differ in tla-en. */
  phase = gData_02001aa4 / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gData_02001ac8[(phase + i) % 4 + 2] = Data_02001aa8[i + 2];
    Data_02001b08[(phase + i) % 4 + 2] = Data_02001ae8[i + 2];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)gData_02001ac8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000140, (u32)Data_02001b08, 0x84000008);
  gData_02001aa4++;
}
