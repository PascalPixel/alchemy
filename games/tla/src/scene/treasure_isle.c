/// @file
/// Treasure Isle's first object: the shared actor helpers it links first.
#include "djinn_ids.h"
#include "edition.h"
#include "fixed_math.h"
#include "flags.h"
#include "scene_ids.h"
#include "types.h"
#include "version.h"
#include "macros.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/cell_target.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/field_effect.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "text/msg_ids.h"

#include "shared/tables.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

// Treasure Isle's copy of the field effect spawner.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#undef EFFECT_SPAWN

// Treasure Isle, the dungeon in the north-east of the world: buttons that
// raise spiky rocks Grind breaks, pillars Move pushes onto switches, rocks
// Lift raises, Gale the Jupiter Djinni, a Venus Djinni, and the guardian of
// water.

/// Set as Move pushes the entrance room's pillar n (0 to 2) onto its switch.
#define FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(n) (FLAG_AREA_TEMP_00 + (n))
/// Set as spiky rock n (0 to 4) of the chests' room rises, cleared as Grind
/// breaks it (gTreasureIsle_Room2SpikyRocks).
#define FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(n) (FLAG_AREA_TEMP_03 + (n))
/// Set as Move pushes the chests' room's stone pillar onto its switch.
#define FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED FLAG_AREA_TEMP_08
/// Set as Move pushes the Jester's Armlet room's stone pillar onto its switch.
#define FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED FLAG_AREA_TEMP_09
/// Set as spiky rock n (0 to 2) of the Jester's Armlet room rises, cleared as
/// Grind breaks it (gTreasureIsle_Room3SpikyRocks).
#define FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(n) (FLAG_AREA_TEMP_0A + (n))
/// Set as Move pushes the stone pillar of Gale's room onto its switch.
#define FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED FLAG_AREA_TEMP_0D
/// Set as Lift raises boulder n (0 to 7) of Gale's room; while it is clear the
/// boulder stands and the party can jump onto it.
#define FLAG_TREASURE_ISLE_BOULDER_LIFTED(n) (FLAG_SCENE_TEMP_00 + (n))
/// Set once the Star Magician, the guardian of water, has been beaten; it is
/// gone.
#define FLAG_STAR_MAGICIAN_BEATEN 0x9bb

/// Gale answers from any side in the Japanese edition; from the international
/// editions on, only while the leader faces east.
#if TLA_PATCH_1
#define GALE_TALK_TRIGGER TALK_TRIGGER_FACING(FACING_EAST)
#else
#define GALE_TALK_TRIGGER TRIGGER_TALK
#endif

/// The first room's three pillars, set where Move pushed them.
enum {
  ACTOR_PUSHED_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_PUSHED_PILLAR_B,
  ACTOR_PUSHED_PILLAR_C
};

/// The second and third rooms' tightrope posts and the stone pillar Move
/// pushes; the second room has a second pillar.
enum {
  ACTOR_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_POST_B,
  ACTOR_STONE_PILLAR,
  ACTOR_SECOND_STONE_PILLAR
};

/// Gale's room (gTreasureIsle_Room4Placements): its stone pillar, the eight
/// boulders Lift raises, the posts of its three tightropes, and Gale.
enum TreasureIsleGaleRoomActorId {
  ACTOR_GALE_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_BOULDER,
  ACTOR_LAST_BOULDER = ACTOR_FIRST_BOULDER + 7,
  ACTOR_GALE_ROOM_POST_A,
  ACTOR_GALE_ROOM_POST_B,
  ACTOR_GALE_ROOM_POST_C,
  ACTOR_GALE_ROOM_POST_D,
  ACTOR_GALE_ROOM_POST_E,
  ACTOR_GALE_ROOM_POST_F,
  ACTOR_GALE
};

/// The Star Magician, the guardian of water, in the last room
/// (gTreasureIsle_Room5Placements), and the Venus Djinni Earth19 waiting
/// there.
enum { ACTOR_STAR_MAGICIAN = ACTOR_FIRST_PLACED, ACTOR_EARTH19 };

struct TreasureIsleCell;

extern struct ActorPlacement gTreasureIsle_Placements[];

extern struct ActorPlacement gTreasureIsle_Room2Placements[];

extern struct ActorPlacement gTreasureIsle_Room3Placements[];
extern struct ActorPlacement gTreasureIsle_Room4Placements[];
extern struct ActorPlacement gTreasureIsle_Room5Placements[];
extern u32 gTreasureIsle_RockDust[];

/// Import stubs: Event_Start and Event_End, and the pending
/// actions' resolution.

extern struct TreasureIsleCell gTreasureIsle_Room2SpikyRocks[];
extern struct TreasureIsleCell gTreasureIsle_Room3SpikyRocks[];
extern struct Trigger gTreasureIsle_Room2Triggers[];
extern struct Trigger gTreasureIsle_Room3Triggers[];
extern struct Trigger gTreasureIsle_Room4Triggers[];
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

void TreasureIsle_MeetStarMagician(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern u32 gTreasureIsle_PostStill[];
extern u32 gTreasureIsle_LiftRockScale[];

void TreasureIsle_PressRoom2Button(void);

/// One palette's sixteen colours.
struct Palette {
  u16 colors[PALETTE_COLORS];
};
struct Palette gTreasureIsle_SavedBgColors BSS_DATA;
struct Palette gTreasureIsle_BgColors BSS_DATA;
struct Palette gTreasureIsle_SavedObjColors BSS_DATA;
struct Palette gTreasureIsle_ObjColors BSS_DATA;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const u8 *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_TREASURE_ISLE_CHESTS)
    return gTreasureIsle_Room2Placements;
  if (scene == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST)
    return gTreasureIsle_Room3Placements;
  if (scene == SCENE_TREASURE_ISLE_GALE)
    return gTreasureIsle_Room4Placements;
  if (scene == SCENE_TREASURE_ISLE_GUARDIAN)
    return gTreasureIsle_Room5Placements;
  return gTreasureIsle_Placements;
}

extern struct MapCell gMapGridCellBuffer[];

/// Unused: stores value as the collision code and attribute of the cell at x, y
/// of a map layer (the cell buffer above layer 2) and returns what it held.
UNUSED s32 TreasureIsle_SwapCells(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}

/// Draws a spiky rock risen at its cell, blocks the cell and sets its flag.
void TreasureIsle_SetSpikyRock(struct TreasureIsleCell *cells, s16 index) {
  MapGrid_CopyCellsLayered(0, 100, 3, 4, cells[index].x + 63,
                           cells[index].z - 2);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, cells[index].x, cells[index].z);
  Event_SetFlag(cells[index].flag);
}

/// A spiky rock rises out of the ground at a listed cell: the camera goes to
/// it, the ground shakes and dust bursts, and TreasureIsle_SetSpikyRock leaves
/// it standing.
void TreasureIsle_RaiseSpikyRock(struct TreasureIsleCell *cells, s16 index) {
  struct TreasureIsleCell *cell;
  struct EffectOptions options;

  FieldCamera_SetSpeed(6);
  // As &cells[index], the sum takes its operands the other way round.
  cell = (struct TreasureIsleCell *)((index << 4) + (s32)cells);
  FieldCamera_MoveToFX(FIELD_CELL_CENTER(cell->x), -1,
                       FIELD_CELL_CENTER(cell->z), 1);
  FieldCamera_WaitForArrival();
  MapGrid_SetShake(0, 16, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SE_POUND);
  options.startScaleX = FIXED_ONE * 12 / 10;
  options.startScaleY = FIXED_ONE * 3 / 2;
  options.priority = 1;
  options.type = SPRITE_WATER_SPRAY;
  options.script = gTreasureIsle_RockDust;
  FieldEffect_Spawn(FIELD_CELL_CENTER(cell->x), 0,
                    (cell->z << 20) + toFIXED(20), 0, 0, 0,
                    EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                    &options);
  options.startScaleX = FIXED_ONE * 6 / 10;
  FieldEffect_Spawn((cell->x << 20) - toFIXED(2), 0, FIELD_CELL_CENTER(cell->z),
                    0, 0, 0,
                    EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                    &options);
  FieldEffect_Spawn((cell->x << 20) + toFIXED(18), 0,
                    FIELD_CELL_CENTER(cell->z), 0, 0, 0,
                    EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                    &options);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsLayered(6, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsLayered(3, 100, 3, 4, cell->x + 63, cell->z - 2);
  FieldEvent_Wait(10);
  TreasureIsle_SetSpikyRock(cells, index);
}

/// The chests' room's button: each of its first four spiky rocks not yet risen
/// rises in turn (TreasureIsle_RaiseSpikyRock), all in one event, and its flag
/// is set.
void TreasureIsle_PressRoom2Button(void) {
  s16 count = 0;

  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0))) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 0);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0));
    count = 1;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1))) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 1);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1));
    count++;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2))) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 2);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2));
    count++;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3))) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 3);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3));
    count++;
  }
  if (count != 0)
    Event_End();
}

/// The Jester's Armlet room's button: each of its three spiky rocks not yet
/// risen rises in turn (TreasureIsle_RaiseSpikyRock), all in one event, and its
/// flag is set.
void TreasureIsle_PressRoom3Button(void) {
  s16 count = 0;

  if (!Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0))) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room3SpikyRocks, 0);
    Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0));
    count = 1;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1))) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room3SpikyRocks, 1);
    Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1));
    count++;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2))) {
    if (count == 0)
      Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room3SpikyRocks, 2);
    Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2));
    count++;
  }
  if (count != 0)
    Event_End();
}

/// The chests' room's fifth spiky rock rises, once, as Move pushes the second
/// stone pillar onto its switch.
void TreasureIsle_RaiseLastSpikyRock(void) {
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4))) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsle_Room2SpikyRocks, 4);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4));
    Event_End();
  }
}

/// After Move or a push moves a stone pillar: a pillar that has reached its
/// switch sets its flag, and the chests' room's second pillar on its switch
/// raises the last spiky rock (TreasureIsle_RaiseLastSpikyRock).
void TreasureIsle_SettleMovedPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_ENTRANCE) {
    if (id == ACTOR_PUSHED_PILLAR_A && FIELD_CELL(object->x.fixed) == 16)
      Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0));
    if (id == ACTOR_PUSHED_PILLAR_B && FIELD_CELL(object->x.fixed) == 20)
      Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1));
    if (id == ACTOR_PUSHED_PILLAR_C && FIELD_CELL(object->x.fixed) == 27)
      Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2));
  }
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_CHESTS) {
    if (id == ACTOR_STONE_PILLAR && object->x.fixed >> 19 == 33)
      Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED);
    if (id == ACTOR_SECOND_STONE_PILLAR && object->x.fixed >> 19 == 85 &&
        object->z.fixed >> 19 == 83)
      TreasureIsle_RaiseLastSpikyRock();
  }
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST) {
    if (id == ACTOR_STONE_PILLAR && object->x.fixed >> 19 == 83)
      Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED);
  }
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_GALE) {
    if (id == ACTOR_GALE_ROOM_PILLAR && object->x.fixed >> 19 == 87)
      Event_SetFlag(FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED);
  }
}
/// Talking to Gale, the Jupiter Djinni, starts its battle.
void TreasureIsle_FightGale(void) {
  Field_RequestDjinnBattle(ACTOR_GALE, DJINNI_GALE);
}

/// The Venus Djinni waiting here joins the party without a fight.
void TreasureIsle_VenusDjinniJoins(void) {
  Djinn_JoinParty(ACTOR_EARTH19, DJINNI_EARTH19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_EARTH19));
}

/// Grind on a spiky rock: it crumbles over three steps and its cell opens
/// again (its flag cleared), the frame passed by the Psynergy.
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

/// Lift raises boulder n of Gale's room out of the way for good: its sprite is
/// drawn at priority 1 from now on, no longer taken from the map, and its flag
/// is set.
#define DEFINE_LIFT_ROCK(name, n)                                              \
  void name(void) {                                                            \
    struct Actor *boulder = Actor_Get(ACTOR_FIRST_BOULDER + (n));              \
                                                                               \
    ((struct FieldSprite *)boulder->sprite)->priority = 1;                     \
    boulder->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;                       \
    Event_SetFlag(FLAG_TREASURE_ISLE_BOULDER_LIFTED(n));                       \
  }

DEFINE_LIFT_ROCK(TreasureIsle_LiftFirstRock, 0)

DEFINE_LIFT_ROCK(TreasureIsle_LiftSecondRock, 1)

DEFINE_LIFT_ROCK(TreasureIsle_LiftThirdRock, 2)

DEFINE_LIFT_ROCK(TreasureIsle_LiftFourthRock, 3)

DEFINE_LIFT_ROCK(TreasureIsle_LiftFifthRock, 4)

DEFINE_LIFT_ROCK(TreasureIsle_LiftSixthRock, 5)

DEFINE_LIFT_ROCK(TreasureIsle_LiftSeventhRock, 6)

DEFINE_LIFT_ROCK(TreasureIsle_LiftEighthRock, 7)

/// The Star Magician, the guardian of water, glows and speaks ("I am the
/// guardian of water. If you would claim my power, you must prove your
/// worth."), turns to the leader and starts its battle, which cannot be fled.
void TreasureIsle_MeetStarMagician(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_StartRepeatedMotion(ACTOR_STAR_MAGICIAN, 2);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(FIXED_ONE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_Interpolate(60);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgIAmTheGuardian, 0, 6);
  PaletteFade_ApplyTarget(FIXED_ONE, 0);
  PaletteFade_Interpolate(60);
  FieldEvent_Wait(60);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_STAR_MAGICIAN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldEvent_Wait(20);
  gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_STAR_MAGICIAN_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_GUARDIANS, 0);
}

extern struct Trigger gTreasureIsle_Room1Triggers[];

const u8 *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_TREASURE_ISLE_CHESTS)
    return gTreasureIsle_Room2Triggers;
  if (scene == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST)
    return gTreasureIsle_Room3Triggers;
  if (scene == SCENE_TREASURE_ISLE_GALE)
    return gTreasureIsle_Room4Triggers;
  if (scene == SCENE_TREASURE_ISLE_GUARDIAN)
    return gTreasureIsle_Room5Triggers;
  return (const u8 *)gTreasureIsle_Room1Triggers;
}

/// The water colours' cycle frame, -1 until the first frame saves the
/// palettes.
extern s32 gTreasureIsle_ColorCycleFrame[];

/// A task: cycles four colours of a background and an object palette every
/// eight frames, saving both palettes the first time.
void TreasureIsle_CycleWaterColors(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 phase;
  s32 i;
  s32 busy;

  if (work->psynergy != 0)
    return;
  busy = (s8)work->revealActive;
  if (busy != 0)
    return;
  if (gTreasureIsle_ColorCycleFrame[0] == -1) {
    Iwram_CopyWords(gTreasureIsle_SavedBgColors.colors,
                    (void *)&BG_PLTT_COLOR(3, 0), sizeof(struct Palette));
    Iwram_CopyWordsEntry(gTreasureIsle_BgColors.colors,
                         gTreasureIsle_SavedBgColors.colors,
                         sizeof(struct Palette));
    Iwram_CopyWordsEntry(gTreasureIsle_SavedObjColors.colors,
                         (void *)&BG_PLTT_COLOR(10, 0), sizeof(struct Palette));
    Iwram_CopyWordsEntry(gTreasureIsle_ObjColors.colors,
                         gTreasureIsle_SavedObjColors.colors,
                         sizeof(struct Palette));
    gTreasureIsle_ColorCycleFrame[0] = 0;
  }
  phase = gTreasureIsle_ColorCycleFrame[0] / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gTreasureIsle_BgColors.colors[(phase + i) % 4 + 2] =
        gTreasureIsle_SavedBgColors.colors[i + 2];
    gTreasureIsle_ObjColors.colors[(phase + i) % 4 + 2] =
        gTreasureIsle_SavedObjColors.colors[i + 2];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(3, 0),
                      (u32)gTreasureIsle_BgColors.colors,
                      DMA_COPY_WORDS(PALETTE_COLORS / 2));
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(10, 0),
                      (u32)gTreasureIsle_ObjColors.colors,
                      DMA_COPY_WORDS(PALETTE_COLORS / 2));
  gTreasureIsle_ColorCycleFrame[0]++;
}

/// Treasure Isle's setup for each of its rooms: places the pillars already
/// pushed onto their switches, cycles the water colours in the chests' and the
/// Jester's Armlet rooms and stands their risen spiky rocks again, and in
/// Gale's room strings the tightropes and draws the boulders' shadows at the
/// lowest priority.
s32 Scene_Initialize(void) {
  s32 i;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Tightrope_Initialize();
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_ENTRANCE) {
    Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0));
    Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1));
    Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2));
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0)))
      Actor_SetPosition(ACTOR_PUSHED_PILLAR_A, 264, 440);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1)))
      Actor_SetPosition(ACTOR_PUSHED_PILLAR_B, 328, 440);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2)))
      Actor_SetPosition(ACTOR_PUSHED_PILLAR_C, 440, 344);
  }
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_CHESTS) {
    Task_AddFrame(TreasureIsle_CycleWaterColors, TASK_PASS_VBLANK);
    Tightrope_Place(0, ACTOR_POST_A, ACTOR_POST_B);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_STONE_PILLAR, 264, 184);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 0);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 1);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 2);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 3);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room2SpikyRocks, 4);
  }
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST) {
    Task_AddFrame(TreasureIsle_CycleWaterColors, TASK_PASS_VBLANK);
    Tightrope_Place(0, ACTOR_POST_A, ACTOR_POST_B);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_STONE_PILLAR, 664, 248);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room3SpikyRocks, 0);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room3SpikyRocks, 1);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)))
      TreasureIsle_SetSpikyRock(gTreasureIsle_Room3SpikyRocks, 2);
  }
  if (gMainRoutineWork.next == SCENE_TREASURE_ISLE_GALE) {
    if (Event_GetFlag(FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_GALE_ROOM_PILLAR, 696, 600);
    Tightrope_Place(0, ACTOR_GALE_ROOM_POST_A, ACTOR_GALE_ROOM_POST_B);
    Tightrope_Place(1, ACTOR_GALE_ROOM_POST_C, ACTOR_GALE_ROOM_POST_D);
    Tightrope_Place(2, ACTOR_GALE_ROOM_POST_E, ACTOR_GALE_ROOM_POST_F);
    for (i = 0; i <= 7; i++)
      ((struct FieldSprite *)Actor_Get(i + ACTOR_FIRST_BOULDER)->sprite)
          ->partPriority = 3;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Steps onto a pillar top, a post or a boulder: the leader jumps ahead if it
/// can land.
void TreasureIsle_JumpForward(void) { Field_TryJumpForward(); }

void TreasureIsle_LiftSecondRock(void);
void TreasureIsle_LiftThirdRock(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
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
    {7, 21, 12, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)},
    {7, 17, 13, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)},
    {12, 15, 14, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)},
    {9, 11, 15, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)},
    {46, 38, 16, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)},
};

struct TreasureIsleCell gTreasureIsle_Room3SpikyRocks[] = {
    {41, 23, 11, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)},
    {47, 23, 12, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)},
    {53, 23, 13, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(TreasureIsleEntrance),
    SCENE_EXIT_UNLESS(1, WorldMap, 87, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleEntrance, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleEntrance, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleEntrance, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TreasureIsleEntrance, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TreasureIsleEntrance, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TreasureIsleEntrance, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TreasureIsleChests, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleChests),
    SCENE_EXIT_UNLESS(1, TreasureIsleEntrance, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleChests, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleChests, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleJestersArmletChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TreasureIsleJestersArmletChest, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleJestersArmletChest),
    SCENE_EXIT_UNLESS(1, TreasureIsleChests, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleChests, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleJestersArmletChest, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleJestersArmletChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TreasureIsleGale, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleGale),
    SCENE_EXIT_UNLESS(1, TreasureIsleJestersArmletChest, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleGale, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleGale, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleGale, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TreasureIsleGale, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TreasureIsleGuardian, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleGuardian),
    SCENE_EXIT_UNLESS(1, TreasureIsleGale, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleGuardian, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleGuardian, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gTreasureIsle_PostStill[] = {
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gTreasureIsle_Placements[] = {                                             
      {272, CONDITION_ALWAYS, 1, toFIXED(280), 0, toFIXED(440), FACING_SOUTH, 2,    
       0},                                                                     
      {272, CONDITION_ALWAYS, 1, toFIXED(344), 0, toFIXED(440), FACING_SOUTH, 2,    
       0},                                                                    
      {272, CONDITION_ALWAYS, 1, toFIXED(424), 0, toFIXED(344), FACING_SOUTH, 2,    
       0},                                                                    
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gTreasureIsle_Room2Placements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 600, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 680, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTreasureIsle_Room3Placements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 824, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 824, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

u32 gTreasureIsle_LiftRockScale[] = {
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gTreasureIsle_Room4Placements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 328, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 216, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 104, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 312, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 216, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 120, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 744, 0, 728, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, (s32)gTreasureIsle_LiftRockScale, 424, 0, 680, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 360, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 360, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 152, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 152, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 72, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 72, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GALE), MOVEMENT_TYPE_STAND, 88, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTreasureIsle_Room5Placements[] = {
    ACTOR_PLACEMENT(169, FLAG_STAR_MAGICIAN_BEATEN, MOVEMENT_TYPE_STAND, 792, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_EARTH19), MOVEMENT_TYPE_STAND, 200, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR_A,
     FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0),
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR_B,
     FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1),
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR_C,
     FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2),
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
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS,
     EVENT(TreasureIsle_JumpForward)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 40,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 41,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 42,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 43,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 44,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_PILLAR,
     FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_STONE_PILLAR,
     CONDITION_ALWAYS, EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTreasureIsle_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS,
     EVENT(TreasureIsle_JumpForward)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(TreasureIsle_PressRoom3Button)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 50,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 51,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS), 52,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_PILLAR,
     FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_TABLE_END},
};

/// Gale's room: its exits, the jumps between the pillar tops and boulders
/// while each boulder stands, Gale, the stone pillar Move pushes and the
/// boulders Lift raises.
struct Trigger gTreasureIsle_Room4Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, FLAG_SCENE_TEMP_00,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 32, FLAG_SCENE_TEMP_00,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 33, FLAG_SCENE_TEMP_01,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 34, FLAG_SCENE_TEMP_01,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 35, FLAG_SCENE_TEMP_02,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 36, FLAG_SCENE_TEMP_02,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 37, FLAG_SCENE_TEMP_03,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 38, FLAG_SCENE_TEMP_03,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 39, FLAG_SCENE_TEMP_04,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 40, FLAG_SCENE_TEMP_04,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 41, FLAG_SCENE_TEMP_05,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 42, FLAG_SCENE_TEMP_05,
     EVENT(TreasureIsle_JumpForward)},
    {GALE_TALK_TRIGGER, ACTOR_GALE, CONDITION_ALWAYS,
     EVENT(TreasureIsle_FightGale)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GALE_ROOM_PILLAR,
     FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(0), EVENT(TreasureIsle_LiftFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 1,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(1), EVENT(TreasureIsle_LiftSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 2,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(2), EVENT(TreasureIsle_LiftThirdRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 3,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(3), EVENT(TreasureIsle_LiftFourthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 4,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(4), EVENT(TreasureIsle_LiftFifthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 5,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(5), EVENT(TreasureIsle_LiftSixthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FIRST_BOULDER + 6,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(6), EVENT(TreasureIsle_LiftSeventhRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_LAST_BOULDER,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(7), EVENT(TreasureIsle_LiftEighthRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTreasureIsle_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 42, FLAG_STAR_MAGICIAN_BEATEN,
     EVENT(TreasureIsle_MeetStarMagician)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TreasureIsle_VenusDjinniJoins)},
    {TRIGGER_TABLE_END},
};

s32 gTreasureIsle_ColorCycleFrame[] = {
    -1,
};

#undef GALE_TALK_TRIGGER
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
