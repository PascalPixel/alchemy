/// @file
/// Treasure Isle's first object: the shared actor helpers it links first.

#include "shared/actors.h"
#include "types.h"
#include "actor.h"
#include "field/event_inline.h"
#include "shared/tables.h"
#include "shared/map_cells.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "scene_ids.h"
#include "module/field/event_commands.h"
#include "field/cell_target.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "system/field_effect.h"
#include "field_event.h"
#include "call.h"
#include "system/iwram_call.h"
#include "system/io_write_queue.h"
#include "system/task.h"
#include "edition.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_ACTOR(TreasureIsle_CreateActor)

#include "../field/field_effect/move.c"

// @file
// Treasure Isle's copy of the field effect spawner.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

#undef EFFECT_SPAWN

// ---- scene/treasure_isle/treasure_isle.c ----

// Treasure Isle, the dungeon in the north-east of the world: buttons that
// raise spiky rocks Grind breaks, pillars Move pushes onto switches, rocks
// Lift raises, Gale the Jupiter Djinni, a Venus Djinni, and the guardian of
// water.

/// What The Lost Age's code declares for the files that use it, written once.

struct TreasureIsleCell;

extern struct ActorPlacement gTreasureIsle_Placements[];

extern struct ActorPlacement gTreasureIsle_Room2Placements[];

extern struct ActorPlacement gTreasureIsle_Room3Placements[];
extern struct ActorPlacement gTreasureIsle_Room4Placements[];
extern struct ActorPlacement gTreasureIsle_Room5Placements[];
extern u32 gTreasureIsle_RockDust[];

// Import stubs: Event_Start and Event_End, and the pending
// actions' resolution.
void Event_Start(void);

extern struct TreasureIsleCell gTreasureIsle_Room2SpikyRocks[];
extern struct TreasureIsleCell gTreasureIsle_Room3SpikyRocks[];
extern struct Trigger gTreasureIsle_Room2Triggers[];
extern struct Trigger gTreasureIsle_Room3Triggers[];
extern s32 gTreasureIsle_Room4Triggers[];
extern struct Trigger gTreasureIsle_Room5Triggers[];

void TreasureIsle_SetSpikyRock(struct TreasureIsleCell *cells, s16 index);

void TreasureIsle_CycleWaterColors(void);

void Tightrope_Initialize(void);

s32 Tightrope_Place(s32 a, s32 b, s32 c);

s32 Field_TryJumpForward();

void TreasureIsle_FightGale(void);

void TreasureIsle_VenusDjinniJoins(void);

void TreasureIsle_LiftFirstRock(void);

void TreasureIsle_LiftFourthRock(void);

void TreasureIsle_LiftFifthRock(void);

void TreasureIsle_LiftSixthRock(void);
void TreasureIsle_LiftSeventhRock(void);

void TreasureIsle_LiftEighthRock(void);

void TreasureIsle_MeetWaterGuardian(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern u32 gTreasureIsle_PostStill[];
extern u32 gTreasureIsle_LiftRockScale[];

void TreasureIsle_PressRoom2Button(void);

void FieldCamera_WaitForArrival(void);

#include "text/msg_ids.h"

extern u16 gTreasureIsle_SavedBgColors[];
extern volatile u16 gTreasureIsle_SavedObjColors[];
extern u16 gTreasureIsle_ObjColors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gTreasureIsle_Room2Placements;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gTreasureIsle_Room3Placements;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gTreasureIsle_Room4Placements;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gTreasureIsle_Room5Placements;
  return gTreasureIsle_Placements;
}

extern struct MapCell gMapCellBuffer[];

DEFINE_MAP_CELL_SWAP(TreasureIsle_SwapCells)

// Draws a spiky rock risen at its cell, blocks the cell and sets its flag.
void TreasureIsle_SetSpikyRock(struct TreasureIsleCell *cells, s16 index) {
  MapGrid_CopyCellsLayered(0, 0x64, 3, 4, cells[index].x + 0x3f,
                           cells[index].z - 2);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, cells[index].x, cells[index].z);
  Event_SetFlag(cells[index].flag);
}

// A spiky rock rises out of the ground at a listed cell: the camera goes to
// it, the ground shakes and dust bursts, and TreasureIsle_SetSpikyRock leaves
// it standing.
void TreasureIsle_RaiseSpikyRock(struct TreasureIsleCell *cells, s16 index) {
  struct TreasureIsleCell *cell;
  struct EffectOptions options;

  FieldCamera_SetSpeed(0x60000, 0xc000);
  // As &cells[index], the sum takes its operands the other way round.
  cell = (struct TreasureIsleCell *)((index << 4) + (s32)cells);
  FieldCamera_MoveTo((cell->x << 20) + 0x80000, -1, (cell->z << 20) + 0x80000,
                     1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(0, 0x100000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(567);
  options.startScaleX = 0x13333;
  options.startScaleY = 0x18000;
  options.priority = 1;
  options.type = 0x115;
  options.script = gTreasureIsle_RockDust;
  FieldEffect_Spawn((cell->x << 20) + 0x80000, 0, (cell->z << 20) + 0x140000, 0,
                    0, 0, 0x3a0000, &options);
  options.startScaleX = 0x9999;
  FieldEffect_Spawn((cell->x << 20) - 0x20000, 0, (cell->z << 20) + 0x80000, 0,
                    0, 0, 0x3a0000, &options);
  FieldEffect_Spawn((cell->x << 20) + 0x120000, 0, (cell->z << 20) + 0x80000, 0,
                    0, 0, 0x3a0000, &options);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsLayered(6, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsLayered(3, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  TreasureIsle_SetSpikyRock(cells, index);
}

// For each of cells 0 to 3 of gTreasureIsle_Room2SpikyRocks whose flag (0x303
// to 0x306) is still clear, calls TreasureIsle_RaiseSpikyRock and sets the
// flag; the event is begun before the first and ended after the last.
void TreasureIsle_PressRoom2Button(void) {
  s16 count = 0;

  if (!Event_GetFlag(0x303)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 0);
    Event_SetFlag(0x303);
    count = 1;
  }
  if (!Event_GetFlag(0x304)) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 1);
    Event_SetFlag(0x304);
    count++;
  }
  if (!Event_GetFlag(0x305)) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 2);
    Event_SetFlag(0x305);
    count++;
  }
  if (!Event_GetFlag(0x306)) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 3);
    Event_SetFlag(0x306);
    count++;
  }
  if (count != 0)
    Event_End();
}

// For each of cells 0 to 2 of gTreasureIsle_Room3SpikyRocks whose flag (0x30a
// to 0x30c) is still clear, calls TreasureIsle_RaiseSpikyRock and sets the
// flag; the event is begun before the first and ended after the last.
void TreasureIsle_PressRoom3Button(void) {
  s16 count = 0;

  if (!Event_GetFlag(0x30a)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room3SpikyRocks, 0);
    Event_SetFlag(0x30a);
    count = 1;
  }
  if (!Event_GetFlag(0x30b)) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room3SpikyRocks, 1);
    Event_SetFlag(0x30b);
    count++;
  }
  if (!Event_GetFlag(0x30c)) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room3SpikyRocks, 2);
    Event_SetFlag(0x30c);
    count++;
  }
  if (count != 0)
    Event_End();
}

void TreasureIsle_RaiseLastSpikyRock(void) {
  if (!Event_GetFlag(0x307)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 4);
    Event_SetFlag(0x307);
    Event_End();
  }
}

// Called through the scene table with an object id: in each of the first
// four rooms, sets a flag when object 8 to 11 is at a given x; object 11 at
// a given x and z in the second room runs TreasureIsle_RaiseLastSpikyRock
// instead.
void TreasureIsle_SettleMovedPillar(s32 unused, s32 id) {
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
      TreasureIsle_RaiseLastSpikyRock();
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

// Talking to Gale, the Jupiter Djinni, starts its battle.
void TreasureIsle_FightGale(void) { Field_RequestDjinnBattle(0x17, 0x4d); }

// The Venus Djinni waiting here joins the party without a fight.
void TreasureIsle_VenusDjinniJoins(void) { Djinn_JoinParty(9, 0, 0x13); }

// Grind on a spiky rock: it crumbles over three steps and its cell opens
// again (its flag cleared), the frame passed by the Psynergy.
void TreasureIsle_GrindSpikyRock(s32 unused, s32 frame, s32 id) {
  struct TreasureIsleCell *cells;

  if (id < 50) {
    cells = gTreasureIsle_Room2SpikyRocks;
    id -= 40;
  } else {
    cells = gTreasureIsle_Room3SpikyRocks;
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

// Lift raises one of Room4's rocks out of the way for good (flag n2).
#define DEFINE_LIFT_ROCK(name, n1, n2)                                         \
  void name(void) {                                                            \
    s32 t0;                                                                    \
                                                                               \
    t0 = ((s32)Actor_Get(n1));                                                 \
    FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =                                   \
        ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);                  \
    FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));      \
    Event_SetFlag(n2);                                                         \
  }

DEFINE_LIFT_ROCK(TreasureIsle_LiftFirstRock, 9, 0x200)

DEFINE_LIFT_ROCK(TreasureIsle_LiftSecondRock, 10, 0x201)

DEFINE_LIFT_ROCK(TreasureIsle_LiftThirdRock, 11, 0x202)

DEFINE_LIFT_ROCK(TreasureIsle_LiftFourthRock, 12, 0x203)

DEFINE_LIFT_ROCK(TreasureIsle_LiftFifthRock, 13, 0x204)

DEFINE_LIFT_ROCK(TreasureIsle_LiftSixthRock, 14, 0x205)

DEFINE_LIFT_ROCK(TreasureIsle_LiftSeventhRock, 15, 0x206)

DEFINE_LIFT_ROCK(TreasureIsle_LiftEighthRock, 16, 0x207)

// The guardian of water challenges the party to prove its worth.
// "I am the guardian of water. If you would claim my power, you must prove
// your worth."
void TreasureIsle_MeetWaterGuardian(void) {
  s32 t12;

  Event_Start();
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

extern struct Trigger gTreasureIsle_Room1Triggers[];

const u8 *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gTreasureIsle_Room2Triggers;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gTreasureIsle_Room3Triggers;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gTreasureIsle_Room4Triggers;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gTreasureIsle_Room5Triggers;
  return (const u8 *)gTreasureIsle_Room1Triggers;
}

// The water colours' cycle frame, -1 until the first frame saves the
// palettes.
extern s32 gTreasureIsle_ColorCycleFrame[];
extern volatile u16 gTreasureIsle_BgColors[];

// A task: cycles four colours of a background and an object palette every
// eight frames, saving both palettes the first time.
void TreasureIsle_CycleWaterColors(void) {
  struct TreasureIsleEventWork *work = Ram_HeapSlots->eventWork;
  s32 phase;
  s32 i;
  s32 busy;

  if (work->unk166 != 0)
    return;
  busy = (s8)work->unkca4;
  if (busy != 0)
    return;
  if (gTreasureIsle_ColorCycleFrame[0] == -1) {
    Call3((void (*)())Iwram_CopyWordsEntry, gTreasureIsle_SavedBgColors,
          (void *)0x05000060, 32);
    Iwram_CopyWordsEntry((void *)gTreasureIsle_BgColors,
                         gTreasureIsle_SavedBgColors, 32);
    Iwram_CopyWordsEntry((void *)gTreasureIsle_SavedObjColors,
                         (void *)0x05000140, 32);
    Iwram_CopyWordsEntry(gTreasureIsle_ObjColors,
                         (void *)gTreasureIsle_SavedObjColors, 32);
    gTreasureIsle_ColorCycleFrame[0] = 0;
  }
  /* FAKEMATCH(volatile): two of the colour arrays read and written as volatile,
   * which keeps the second read after the first store; as plain u16, 6 of 152
   * instructions differ in tla-en. */
  phase = gTreasureIsle_ColorCycleFrame[0] / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gTreasureIsle_BgColors[(phase + i) % 4 + 2] =
        gTreasureIsle_SavedBgColors[i + 2];
    gTreasureIsle_ObjColors[(phase + i) % 4 + 2] =
        gTreasureIsle_SavedObjColors[i + 2];
  }
  IO_WRITE_QUEUE_PUSH(0x05000060, (u32)gTreasureIsle_BgColors, 0x84000008);
  IO_WRITE_QUEUE_PUSH(0x05000140, (u32)gTreasureIsle_ObjColors, 0x84000008);
  gTreasureIsle_ColorCycleFrame[0]++;
}
#undef FIELD_AT

// its code, then the overlay's data.

/* VIEW(s0cf4): tag struct TreasureIsleEventWork as struct
 * TreasureIsleEventWork__s0cf4; treasure_isle.c has another definition; tried
 * with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/treasure_isle.c:473: structure has no member
 * named `route'. */
struct TreasureIsleEventWork__s0cf4 {
  u8 unk000[0x1ac];
  s32 route;
};

// Treasure Isle's setup for each of its four rooms: places the objects whose
// flags are set, schedules TreasureIsle_CycleWaterColors in rooms 2 and 3 and
// opens again the cells already opened there (TreasureIsle_SetSpikyRock), and
// in room 4 raises the part priority of objects 9 to 16.
s32 Scene_Initialize(void) {
  s32 i;

  ((struct TreasureIsleEventWork__s0cf4 *)Ram_HeapSlots->eventWork)->route =
      0x204;
  Tightrope_Initialize();
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
    Task_AddFrame(TreasureIsle_CycleWaterColors, TASK_PASS_VBLANK);
    Tightrope_Place(0, 8, 9);
    if (Event_GetFlag(0x308))
      Actor_SetPosition(10, PIXELS(264), PIXELS(184));
    if (Event_GetFlag(0x303))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 0);
    if (Event_GetFlag(0x304))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 1);
    if (Event_GetFlag(0x305))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 2);
    if (Event_GetFlag(0x306))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 3);
    if (Event_GetFlag(0x307))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 4);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom3) {
    Task_AddFrame(TreasureIsle_CycleWaterColors, TASK_PASS_VBLANK);
    Tightrope_Place(0, 8, 9);
    if (Event_GetFlag(0x309))
      Actor_SetPosition(10, PIXELS(664), PIXELS(248));
    if (Event_GetFlag(0x30a))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room3SpikyRocks, 0);
    if (Event_GetFlag(0x30b))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room3SpikyRocks, 1);
    if (Event_GetFlag(0x30c))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room3SpikyRocks, 2);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom4) {
    if (Event_GetFlag(0x30d))
      Actor_SetPosition(8, PIXELS(696), PIXELS(600));
    Tightrope_Place(0, 17, 18);
    Tightrope_Place(1, 19, 20);
    Tightrope_Place(2, 21, 22);
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

void TreasureIsle_JumpForward(void) { Field_TryJumpForward(); }

#include "text/msg_ids.h"

// The Rusty Sword's name; the Japanese edition holds message 0 in its place.
#if LANGUAGE == LANGUAGE_JAPANESE
#define RUSTY_SWORD_MESSAGE 0
#else
#define RUSTY_SWORD_MESSAGE MSGNO(MsgRustySword2)
#endif

void TreasureIsle_LiftSecondRock(void);
void TreasureIsle_LiftThirdRock(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gTreasureIsle_RockDust[] = {
    39, 2, 0x80010000, 0, 1, 13,
    44, 0xc0010000, 0, 1, 2, 0,
    0, 0, 0, 1, 38, 17,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct TreasureIsleCell gTreasureIsle_Room2SpikyRocks[] = {
    {7, 21, 12, 0x303},
    {7, 17, 13, 0x304},
    {12, 15, 14, 0x305},
    {9, 11, 15, 0x306},
    {46, 38, 16, 0x307},
};

struct TreasureIsleCell gTreasureIsle_Room3SpikyRocks[] = {
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

u32 gTreasureIsle_PostStill[] = {
    39, 1, 0, 1, 17,
};

DEFINE_G_DATA_02001330(gTreasureIsle_Placements, 272, 280, 440, 344, 440, 0, 424, 344)

struct ActorPlacement gTreasureIsle_Room2Placements[] = {
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(600), 0, PIXELS(616),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(680), 0, PIXELS(616),
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

struct ActorPlacement gTreasureIsle_Room3Placements[] = {
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(824), 0, PIXELS(232),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(824), 0, PIXELS(360),
     FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

u32 gTreasureIsle_LiftRockScale[] = {
    22, 9, 0xcccc, 22, 10, 0xcccc,
    0, 1, 17,
};

struct ActorPlacement gTreasureIsle_Room4Placements[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(600), FACING_SOUTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(328), 0, PIXELS(328),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(216), 0, PIXELS(312),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(104), 0, PIXELS(328),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(312), 0, PIXELS(216),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(216), 0, PIXELS(200),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(120), 0, PIXELS(216),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(744), 0, PIXELS(728),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, PIXELS(424), 0, PIXELS(680),
     FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(360), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(360), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(152), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(152), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(72), 0, PIXELS(216),
     FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, PIXELS(72), 0, PIXELS(296),
     FACING_SOUTH, 2, 0},
    {246, 0x7d, 1, PIXELS(88), 0, PIXELS(328), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTreasureIsle_Room5Placements[] = {
    {169, 0x9bb, 1, PIXELS(792), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {243, 0x43, 1, PIXELS(200), 0, PIXELS(136), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gTreasureIsle_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {9, 0, CONDITION_ALWAYS, EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x300,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x301,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x302,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTreasureIsle_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(TreasureIsle_PressRoom2Button)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS,
     EVENT(TreasureIsle_RaiseLastSpikyRock)},
    {0x602, 35, CONDITION_ALWAYS, EVENT(TreasureIsle_JumpForward)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 40, 0x1303,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 41, 0x1304,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 42, 0x1305,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 43, 0x1306,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 44, 0x1307,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x308,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTreasureIsle_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {0xc602, 35, CONDITION_ALWAYS, EVENT(TreasureIsle_JumpForward)},
    {0x602, 35, CONDITION_ALWAYS, EVENT(TreasureIsle_JumpForward)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(TreasureIsle_PressRoom3Button)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 50, 0x130a,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 51, 0x130b,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 52, 0x130c,
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x309,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_TABLE_END},
};

s32 gTreasureIsle_Room4Triggers[] = {
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
    (s32)TreasureIsle_JumpForward,
    0x602,
    0x2000020,
    (s32)TreasureIsle_JumpForward,
    0x8602,
    0x2010021,
    (s32)TreasureIsle_JumpForward,
    0x602,
    0x2010022,
    (s32)TreasureIsle_JumpForward,
    0x8602,
    0x2020023,
    (s32)TreasureIsle_JumpForward,
    0x602,
    0x2020024,
    (s32)TreasureIsle_JumpForward,
    0x8602,
    0x2030025,
    (s32)TreasureIsle_JumpForward,
    0x602,
    0x2030026,
    (s32)TreasureIsle_JumpForward,
    0x8602,
    0x2040027,
    (s32)TreasureIsle_JumpForward,
    0x602,
    0x2040028,
    (s32)TreasureIsle_JumpForward,
    0x8602,
    0x2050029,
    (s32)TreasureIsle_JumpForward,
    0x602,
    0x205002a,
    (s32)TreasureIsle_JumpForward,
    RUSTY_SWORD_MESSAGE,
    (s32)0xffff0017,
    (s32)TreasureIsle_FightGale,
    0x8c15,
    0x30d0008,
    (s32)TreasureIsle_SettleMovedPillar,
    9,
    -0x10000,
    (s32)TreasureIsle_SettleMovedPillar,
    0x8f15,
    0x2000009,
    (s32)TreasureIsle_LiftFirstRock,
    0x8f15,
    0x201000a,
    (s32)TreasureIsle_LiftSecondRock,
    0x8f15,
    0x202000b,
    (s32)TreasureIsle_LiftThirdRock,
    0x8f15,
    0x203000c,
    (s32)TreasureIsle_LiftFourthRock,
    0x8f15,
    0x204000d,
    (s32)TreasureIsle_LiftFifthRock,
    0x8f15,
    0x205000e,
    (s32)TreasureIsle_LiftSixthRock,
    0x8f15,
    0x206000f,
    (s32)TreasureIsle_LiftSeventhRock,
    0x8f15,
    0x2070010,
    (s32)TreasureIsle_LiftEighthRock,
    -1,
    0,
    0,
};

struct Trigger gTreasureIsle_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 42, 0x9bb, EVENT(TreasureIsle_MeetWaterGuardian)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TreasureIsle_VenusDjinniJoins)},
    {TRIGGER_TABLE_END},
};

s32 gTreasureIsle_ColorCycleFrame[] = {
    -1,
};
#undef FIELD_AT
#undef RUSTY_SWORD_MESSAGE
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
