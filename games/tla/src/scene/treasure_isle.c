// ---- games/tla/src/scene/treasure_isle/scene_tables.c ----
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
#include "module/scene/jupiter_lighthouse_room_4.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/scene/treasure_isle.h"
#include "scene_ids.h"
#include "module/field/event_commands.h"
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"
#include "scene_ids.h"

const u8 *Scene_GetPlacements(void) {
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

void Func_020003fc(struct TreasureIsleCell *cells, s16 index) {
  MapGrid_CopyCellsLayered(0, 0x64, 3, 4, cells[index].x + 0x3f,
                           cells[index].z - 2);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, cells[index].x, cells[index].z);
  Event_SetFlag(cells[index].flag);
}

#include "field_effect.h"

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CameraBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 13
   * of 163 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
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
  MapGrid_SetShake(0, 0x100000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
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
  Call2(Field_RequestBattleFromEncounterTable, 0x67, 0);
}

/* VIEW(scene_tables): variable gData_020016e4 as extern const u8 gData_020016e4[]; the definition has struct Trigger gData_020016e4[] = { { 1, 1, CONDITION_ALWAYS, 1 }, { 1, 2, CONDITION_ALWAYS, 2 }, { 1, 3, CONDITION_ALWAYS, 3 }, { 1, 4, CON...; tried as the definition's extern s16 gData_020016e4[], it does not compile in tla-ja: games/tla/src/scene/treasure_isle.c:715: conflicting types for `gData_020016e4'. */
extern const u8 gData_020016e4__scene_tables[] __asm__("gData_020016e4");

const u8 *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001780;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_0200184c;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_020018f4;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_02001a5c;
  return gData_020016e4__scene_tables;
}

#include "iwram_call.h"
#include "io_write_queue.h"

/* VIEW(scene_tables): variable gData_02001aa4 as extern s32 gData_02001aa4; the definition has s32 gData_02001aa4[] = { - 1, }; tried as the definition's extern s32 gData_02001aa4[], it does not compile in tla-ja: games/tla/src/scene/treasure_isle.c:422: `gData_02001aa4' has an incomplete type. */
extern s32 gData_02001aa4__scene_tables __asm__("gData_02001aa4");
extern volatile u16 gData_02001ac8[];

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
  if (gData_02001aa4__scene_tables == -1) {
    Call3((void (*)())Iwram_CopyWords, Data_02001aa8, (void *)0x05000060, 32);
    Iwram_CopyWords((void *)gData_02001ac8, Data_02001aa8, 32);
    Iwram_CopyWords((void *)Data_02001ae8, (void *)0x05000140, 32);
    Iwram_CopyWords(Data_02001b08, (void *)Data_02001ae8, 32);
    gData_02001aa4__scene_tables = 0;
  }
  /* FAKEMATCH(volatile): two of the colour arrays read and written as volatile,
   * which keeps the second read after the first store; as plain u16, 6 of 152
   * instructions differ in tla-en. */
  phase = gData_02001aa4__scene_tables / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gData_02001ac8[(phase + i) % 4 + 2] = Data_02001aa8[i + 2];
    Data_02001b08[(phase + i) % 4 + 2] = Data_02001ae8[i + 2];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)gData_02001ac8, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000140, (u32)Data_02001b08, 0x84000008);
  gData_02001aa4__scene_tables++;
}
#undef FIELD_AT

// ---- games/tla/src/scene/treasure_isle/s0cf4.c ----
// Scene code of overlay 0x6af, in ROM order, from s0cf4's offset to the end of
// its code, then the overlay's data.

#include "actor.h"
#include "task.h"

/* VIEW(s0cf4): tag struct TreasureIsleEventWork as struct TreasureIsleEventWork__s0cf4; treasure_isle.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/treasure_isle.c:454: structure has no member named `route'. */
struct TreasureIsleEventWork__s0cf4 {
  u8 unk000[0x1ac];
  s32 route;
};

/// Treasure Isle's setup for each of its four rooms: places the objects whose
/// flags are set, schedules Func_02000ba0 in rooms 2 and 3 and opens again
/// the cells already opened there (Func_020003fc), and in room 4 raises the
/// part priority of objects 9 to 16.
s32 Scene_Initialize(void) {
  s32 i;

  ((struct TreasureIsleEventWork__s0cf4 *)Ram_HeapSlots->eventWork)->route = 0x204;
  Func_0200105c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom1) {
    Event_SetFlag(0x300);
    Event_SetFlag(0x301);
    Event_SetFlag(0x302);
    if (Event_GetFlag(0x300))
      Actor_SetPosition(8, PIXELS(264), PIXELS(440));
    if (Event_GetFlag(0x301))
      Actor_SetPosition(9, PIXELS(328), PIXELS(440));
    if (Event_GetFlag(0x302))
      Actor_SetPosition(10, PIXELS(440), PIXELS(344));
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom2) {
    Task_AddFrame(Func_02000ba0, TASK_PASS_VBLANK);
    Func_02001064(0, 8, 9);
    if (Event_GetFlag(0x308))
      Actor_SetPosition(10, PIXELS(264), PIXELS(184));
    if (Event_GetFlag(0x303))
      Func_020003fc(gData_0200117c, 0);
    if (Event_GetFlag(0x304))
      Func_020003fc(gData_0200117c, 1);
    if (Event_GetFlag(0x305))
      Func_020003fc(gData_0200117c, 2);
    if (Event_GetFlag(0x306))
      Func_020003fc(gData_0200117c, 3);
    if (Event_GetFlag(0x307))
      Func_020003fc(gData_0200117c, 4);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom3) {
    Task_AddFrame(Func_02000ba0, TASK_PASS_VBLANK);
    Func_02001064(0, 8, 9);
    if (Event_GetFlag(0x309))
      Actor_SetPosition(10, PIXELS(664), PIXELS(248));
    if (Event_GetFlag(0x30a))
      Func_020003fc(gData_020011cc, 0);
    if (Event_GetFlag(0x30b))
      Func_020003fc(gData_020011cc, 1);
    if (Event_GetFlag(0x30c))
      Func_020003fc(gData_020011cc, 2);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom4) {
    if (Event_GetFlag(0x30d))
      Actor_SetPosition(8, PIXELS(696), PIXELS(600));
    /* FAKEMATCH(signature): Func_02001064 declared returning s32, though it
     * reaches Func_080d9e98, whose draft returns nothing; as void, which sets
     * r0 first for these calls, 4 of 241 instructions differ in tla-en. */
    Func_02001064(0, 17, 18);
    Func_02001064(1, 19, 20);
    Func_02001064(2, 21, 22);
    i = 0;
  next:
    {
      u8 *flags = (u8 *)Actor_Get(i + 9)->sprite + 37;

      *flags |= 12;
    }
    if (++i <= 7)
      goto next;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000f4c(void) { Func_0200102c(); }

#include "text/msg_ids.h"
#include "edition.h"

// The Rusty Sword's name; the Japanese edition holds message 0 in its place.
#if LANGUAGE == LANGUAGE_JAPANESE
#define RUSTY_SWORD_MESSAGE 0
#else
#define RUSTY_SWORD_MESSAGE MSGNO(MsgRustySword2)
#endif

void Func_02000684(void);
void Func_02000728(void);
void Func_02000940(void);
void Func_02000970(void);

// clang-format off
u32 gData_02001074[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020010b0[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020010ec[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001128[] = {
    39, 2, 0x80010000, 0, 1, 13,
    44, 0xc0010000, 0, 1, 2, 0,
    0, 0, 0, 1, 38, 17,
};

s32 gData_02001170[] = {
    (s32)gData_02001074,
    (s32)gData_020010b0,
    (s32)gData_020010ec,
};

struct TreasureIsleCell gData_0200117c[] = {
    {7, 21, 12, 0x303},
    {7, 17, 13, 0x304},
    {12, 15, 14, 0x305},
    {9, 11, 15, 0x306},
    {46, 38, 16, 0x307},
};

struct TreasureIsleCell gData_020011cc[] = {
    {41, 23, 11, 0x30a},
    {47, 23, 12, 0x30b},
    {53, 23, 13, 0x30c},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(298),
    SCENE_EXIT_IF(1, 2, 87, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 298, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 298, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 298, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 298, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 298, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 298, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 299, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(299),
    SCENE_EXIT_IF(1, 298, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 299, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 299, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 300, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 300, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(300),
    SCENE_EXIT_IF(1, 299, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 299, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 300, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 300, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 301, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(301),
    SCENE_EXIT_IF(1, 300, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 301, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 301, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 301, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 301, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 302, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(302),
    SCENE_EXIT_IF(1, 301, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 302, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 302, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gData_0200131c[] = {
    39, 1, 0, 1, 17,
};

struct ActorPlacement gData_02001330[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(440), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(440), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(344), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001390[] = {
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(600), 0, PIXELS(616),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(680), 0, PIXELS(616),
     FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(664), 0, PIXELS(664), FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001480[] = {
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(824), 0, PIXELS(232),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(824), 0, PIXELS(360),
     FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

u32 gData_020014e0[] = {
    22, 9, 0xcccc, 22, 10, 0xcccc,
    0, 1, 17,
};

struct ActorPlacement gData_02001504[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(600), FACING_SOUTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(328), 0, PIXELS(328),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(216), 0, PIXELS(312),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(104), 0, PIXELS(328),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(312), 0, PIXELS(216),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(216), 0, PIXELS(200),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(120), 0, PIXELS(216),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(744), 0, PIXELS(728),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gData_020014e0, PIXELS(424), 0, PIXELS(680),
     FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(360), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(360), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(152), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(152), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(72), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gData_0200131c, PIXELS(72), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {246, 0x7d, 1, PIXELS(88), 0, PIXELS(328), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200169c[] = {
    {169, 0x9bb, 1, PIXELS(792), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {243, 0x43, 1, PIXELS(200), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_020016e4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x300, EVENT(Func_02000760)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x301, EVENT(Func_02000760)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x302, EVENT(Func_02000760)},
    {TRIGGERS_END},
};

struct Trigger gData_02001780[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_020005ac)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02000728)},
    {0x602, 35, CONDITION_ALWAYS, EVENT(Func_02000f4c)},
    {0x50009805, 40, 0x1303, EVENT(Func_02000880)},
    {0x50009805, 41, 0x1304, EVENT(Func_02000880)},
    {0x50009805, 42, 0x1305, EVENT(Func_02000880)},
    {0x50009805, 43, 0x1306, EVENT(Func_02000880)},
    {0x50009805, 44, 0x1307, EVENT(Func_02000880)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x308, EVENT(Func_02000760)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {TRIGGERS_END},
};

struct Trigger gData_0200184c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {0xc602, 35, CONDITION_ALWAYS, EVENT(Func_02000f4c)},
    {0x602, 35, CONDITION_ALWAYS, EVENT(Func_02000f4c)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02000684)},
    {0x50009805, 50, 0x130a, EVENT(Func_02000880)},
    {0x50009805, 51, 0x130b, EVENT(Func_02000880)},
    {0x50009805, 52, 0x130c, EVENT(Func_02000880)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x309, EVENT(Func_02000760)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {TRIGGERS_END},
};

s32 gData_020018f4[] = {
    33,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0x8602,
    0x200001f,
    (s32)Func_02000f4c,
    0x602,
    0x2000020,
    (s32)Func_02000f4c,
    0x8602,
    0x2010021,
    (s32)Func_02000f4c,
    0x602,
    0x2010022,
    (s32)Func_02000f4c,
    0x8602,
    0x2020023,
    (s32)Func_02000f4c,
    0x602,
    0x2020024,
    (s32)Func_02000f4c,
    0x8602,
    0x2030025,
    (s32)Func_02000f4c,
    0x602,
    0x2030026,
    (s32)Func_02000f4c,
    0x8602,
    0x2040027,
    (s32)Func_02000f4c,
    0x602,
    0x2040028,
    (s32)Func_02000f4c,
    0x8602,
    0x2050029,
    (s32)Func_02000f4c,
    0x602,
    0x205002a,
    (s32)Func_02000f4c,
    RUSTY_SWORD_MESSAGE,
    (s32)0xffff0017,
    (s32)Func_02000864,
    0x8c15,
    0x30d0008,
    (s32)Func_02000760,
    9,
    -0x10000,
    (s32)Func_02000760,
    0x8f15,
    0x2000009,
    (s32)Func_02000914,
    0x8f15,
    0x201000a,
    (s32)Func_02000940,
    0x8f15,
    0x202000b,
    (s32)Func_02000970,
    0x8f15,
    0x203000c,
    (s32)Func_020009a0,
    0x8f15,
    0x204000d,
    (s32)Func_020009d0,
    0x8f15,
    0x205000e,
    (s32)Func_020009fc,
    0x8f15,
    0x206000f,
    (s32)Func_02000a2c,
    0x8f15,
    0x2070010,
    (s32)Func_02000a5c,
    -1,
    0,
    0,
};

struct Trigger gData_02001a5c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 42, 0x9bb, EVENT(Func_02000a8c)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000870)},
    {TRIGGERS_END},
};

s32 gData_02001aa4[] = {
    -1,
};
#undef FIELD_AT
#undef RUSTY_SWORD_MESSAGE
