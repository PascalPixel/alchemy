/// @file
/// Treasure Isle, the island cave in the north-east corner of the world, its
/// five rooms one overlay. It can be entered early, but finishing it needs
/// Grind and Isaac's Lift.
///
/// Past the entrance room's three pillars, the chests' room is a lake of small
/// islands joined by jumps and a tightrope: a red button on one island raises
/// spiky rocks out of the water, which Grind breaks again, and a pillar Moved
/// onto a switch in its east part raises one more. The Jester's Armlet room
/// beyond has another button and three spiky rocks in a row. In Gale's room,
/// boulders stand in the way; Lift raises them into the air for good, and Gale,
/// the Jupiter Djinni, waits on the west side. In the last rooms, a Venus
/// Djinni from The Broken Seal that the party lacks waits in the west room,
/// and in the east chamber the Star Magician, the guardian of water, fights
/// for the Azul tablet behind it.
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
#include "field/field_coordinate.h"
#include "field/field_event.h"
#include "field/field_sequence.h"
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
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "text/msg_ids.h"

#include "shared/tables.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/// Set as Move pushes the entrance room's pillar n (0 to 2) aside from its
/// doorway. Scene_Initialize sets all three on entry, so the doorways always
/// stand open.
#define FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(n) (FLAG_AREA_TEMP_00 + (n))
/// Set as spiky rock n (0 to 4) of the chests' room rises, cleared as Grind
/// breaks it (gTreasureIsleChestsSpikyRocks).
#define FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(n) (FLAG_AREA_TEMP_03 + (n))
/// Set as Move pushes the chests' room's stone pillar to its place.
#define FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED FLAG_AREA_TEMP_08
/// Set as Move pushes the Jester's Armlet room's stone pillar to its place.
#define FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED FLAG_AREA_TEMP_09
/// Set as spiky rock n (0 to 2) of the Jester's Armlet room rises, cleared as
/// Grind breaks it (gTreasureIsleArmletSpikyRocks).
#define FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(n) (FLAG_AREA_TEMP_0A + (n))
/// Set as Move pushes the stone pillar of Gale's room to its place.
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

/// The entrance room's three stone pillars, one beside each doorway they
/// blocked (gTreasureIsleEntrancePlacements).
enum {
  ACTOR_ENTRANCE_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_ENTRANCE_PILLAR_B,
  ACTOR_ENTRANCE_PILLAR_C
};

/// The chests' and the Jester's Armlet rooms' tightrope posts and the stone
/// pillar Move pushes; the chests' room has a second pillar.
enum {
  ACTOR_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPE_POST_B,
  ACTOR_STONE_PILLAR,
  ACTOR_SECOND_STONE_PILLAR
};

/// Gale's room (gTreasureIsleGalePlacements): its stone pillar, the eight
/// boulders Lift raises, the posts of its three tightropes, and Gale.
enum TreasureIsleGaleRoomActorId {
  ACTOR_GALE_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_BOULDER,
  ACTOR_LAST_BOULDER = ACTOR_FIRST_BOULDER + 7,
  ACTOR_GALE_ROOM_TIGHTROPE_POST_A,
  ACTOR_GALE_ROOM_TIGHTROPE_POST_B,
  ACTOR_GALE_ROOM_TIGHTROPE_POST_C,
  ACTOR_GALE_ROOM_TIGHTROPE_POST_D,
  ACTOR_GALE_ROOM_TIGHTROPE_POST_E,
  ACTOR_GALE_ROOM_TIGHTROPE_POST_F,
  ACTOR_GALE
};

/// The Star Magician, the guardian of water, in the last room
/// (gTreasureIsleGuardianPlacements), and the Venus Djinni waiting there: the
/// spare DJINNI_EARTH19 stands for one of The Broken Seal's Venus Djinn the
/// party lacks (SCENE_STATE_CHOSEN_DJINN), and with none lacking it is not
/// placed.
enum { ACTOR_STAR_MAGICIAN = ACTOR_FIRST_PLACED, ACTOR_VENUS_DJINNI };

/// Where Move leaves each stone pillar, in pixels: Scene_Initialize puts a
/// pillar whose flag is set back there.
#define ENTRANCE_PILLAR_A_MOVED_X 264
#define ENTRANCE_PILLAR_A_MOVED_Z 440
#define ENTRANCE_PILLAR_B_MOVED_X 328
#define ENTRANCE_PILLAR_B_MOVED_Z 440
#define ENTRANCE_PILLAR_C_MOVED_X 440
#define ENTRANCE_PILLAR_C_MOVED_Z 344
#define CHESTS_PILLAR_MOVED_X 264
#define CHESTS_PILLAR_MOVED_Z 184
#define ARMLET_PILLAR_MOVED_X 664
#define ARMLET_PILLAR_MOVED_Z 248
#define GALE_ROOM_PILLAR_MOVED_X 696
#define GALE_ROOM_PILLAR_MOVED_Z 600
/// The place of the chests' room's second pillar that raises the last spiky
/// rock.
#define CHESTS_SECOND_PILLAR_MOVED_X 680
#define CHESTS_SECOND_PILLAR_MOVED_Z 664

/// What the rooms' map cells do when Felix steps on them or casts at them,
/// beside their exits.
enum {
  /// The red button in the chests' and the Jester's Armlet rooms.
  TRIGGER_BUTTON = 30,
  /// The switch in the chests' room's east part, cell (42, 41).
  TRIGGER_SWITCH = 31,
  /// A cell Felix jumps two cells on from, over a gap in the water.
  TRIGGER_JUMP = 35,
  /// In Gale's room, the cells beside its first six boulders Felix jumps
  /// from, west and east in turn, two to a boulder.
  TRIGGER_FIRST_BOULDER_JUMP = 31,
  /// The cells before the Star Magician in the last chamber.
  TRIGGER_BEFORE_STAR_MAGICIAN = 42
};

/// The cells Grind breaks each spiky rock from, the chests' room's from
/// CHESTS_FIRST_ROCK_CELL and the Jester's Armlet room's from
/// ARMLET_FIRST_ROCK_CELL, in the order of their rooms' lists.
#define CHESTS_FIRST_ROCK_CELL 40
#define ARMLET_FIRST_ROCK_CELL 50

/// The two background palettes of water whose colours cycle.
#define WATER_PALETTE_A 3
#define WATER_PALETTE_B 10

struct TreasureIsleCell;

extern struct ActorPlacement gTreasureIsleEntrancePlacements[];

extern struct ActorPlacement gTreasureIsleChestsPlacements[];

extern struct ActorPlacement gTreasureIsleArmletPlacements[];
extern struct ActorPlacement gTreasureIsleGalePlacements[];
extern struct ActorPlacement gTreasureIsleGuardianPlacements[];
extern u32 gTreasureIsle_RockDust[];

extern struct TreasureIsleCell gTreasureIsleChestsSpikyRocks[];
extern struct TreasureIsleCell gTreasureIsleArmletSpikyRocks[];
extern struct Trigger gTreasureIsleChestsTriggers[];
extern struct Trigger gTreasureIsleArmletTriggers[];
extern struct Trigger gTreasureIsleGaleTriggers[];
extern struct Trigger gTreasureIsleGuardianTriggers[];

void TreasureIsle_SetSpikyRock(struct TreasureIsleCell *cells, s16 index);

void TreasureIsle_CycleWaterColors(void);

void Tightrope_Initialize(void);

s32 Tightrope_Place(s32 slotNo, s32 firstId, s32 secondId);

s32 Field_TryJumpForward();

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern u32 gTreasureIsle_PostStill[];
extern u32 gTreasureIsle_LiftRockScale[];

/// One palette's sixteen colours.
struct Palette {
  u16 colors[PALETTE_COLORS];
};
struct Palette gTreasureIsle_SavedWaterColorsA BSS_DATA;
struct Palette gTreasureIsle_WaterColorsA BSS_DATA;
struct Palette gTreasureIsle_SavedWaterColorsB BSS_DATA;
struct Palette gTreasureIsle_WaterColorsB BSS_DATA;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The actors of the room the party enters.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_TREASURE_ISLE_CHESTS)
    return gTreasureIsleChestsPlacements;
  if (scene == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST)
    return gTreasureIsleArmletPlacements;
  if (scene == SCENE_TREASURE_ISLE_GALE)
    return gTreasureIsleGalePlacements;
  if (scene == SCENE_TREASURE_ISLE_GUARDIAN)
    return gTreasureIsleGuardianPlacements;
  return gTreasureIsleEntrancePlacements;
}

extern struct MapCell gMapGridCellBuffer[];

#include "shared/map_cell_set.c"

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
                    FIELD_CELL_ORIGIN(cell->z) + toFIXED(20), 0, 0, 0,
                    EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                    &options);
  options.startScaleX = FIXED_ONE * 6 / 10;
  FieldEffect_Spawn(FIELD_CELL_ORIGIN(cell->x) - toFIXED(2), 0,
                    FIELD_CELL_CENTER(cell->z), 0, 0, 0,
                    EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                    &options);
  FieldEffect_Spawn(FIELD_CELL_ORIGIN(cell->x) + toFIXED(18), 0,
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

/// Stepping on the red button on a small island in the chests' room's water,
/// cell (9, 15), raises the room's first four spiky rocks out of the water one
/// after another, the camera going to each as it bursts up in a spray: at
/// cells (7, 21), (7, 17), (12, 15) and (9, 11). A rock Grind has broken rises
/// again; one still standing is left as it is.
void TreasureIsle_PressChestsRoomButton(void) {
  s16 count = 0;

  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0))) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleChestsSpikyRocks, 0);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0));
    count = 1;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1))) {
    if (count == 0)
      Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleChestsSpikyRocks, 1);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1));
    count++;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2))) {
    if (count == 0)
      Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleChestsSpikyRocks, 2);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2));
    count++;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3))) {
    if (count == 0)
      Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleChestsSpikyRocks, 3);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3));
    count++;
  }
  if (count != 0)
    Event_End();
}

/// Stepping on the red button in the Jester's Armlet room, cell (47, 27),
/// raises the room's three spiky rocks out of the water in a row, one after
/// another, at cells (41, 23), (47, 23) and (53, 23). A rock Grind has broken
/// rises again.
void TreasureIsle_PressArmletRoomButton(void) {
  s16 count = 0;

  if (!Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0))) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleArmletSpikyRocks, 0);
    Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0));
    count = 1;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1))) {
    if (count == 0)
      Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleArmletSpikyRocks, 1);
    Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1));
    count++;
  }
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2))) {
    if (count == 0)
      Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleArmletSpikyRocks, 2);
    Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2));
    count++;
  }
  if (count != 0)
    Event_End();
}

/// Stepping on the switch in the chests' room's east part, cell (42, 41), or
/// pushing its stone pillar onto it, raises a fifth spiky rock out of the
/// water at cell (46, 38), once.
void TreasureIsle_RaiseLastSpikyRock(void) {
  if (!Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4))) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    TreasureIsle_RaiseSpikyRock(gTreasureIsleChestsSpikyRocks, 4);
    Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4));
    Event_End();
  }
}

/// Move or a push leaving one of the stone pillars where it belongs sets its
/// flag, so the pillar is put back there next time; nothing more is seen. The
/// pillar in the chests' room's east part, left on its switch, cell
/// (42, 41), raises the fifth spiky rock (TreasureIsle_RaiseLastSpikyRock).
void TreasureIsle_SettleMovedPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_ENTRANCE) {
    if (id == ACTOR_ENTRANCE_PILLAR_A &&
        FIELD_CELL(actor->x.fixed) ==
            FIELD_CELL(toFIXED(ENTRANCE_PILLAR_A_MOVED_X)))
      Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0));
    if (id == ACTOR_ENTRANCE_PILLAR_B &&
        FIELD_CELL(actor->x.fixed) ==
            FIELD_CELL(toFIXED(ENTRANCE_PILLAR_B_MOVED_X)))
      Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1));
    if (id == ACTOR_ENTRANCE_PILLAR_C &&
        FIELD_CELL(actor->x.fixed) ==
            FIELD_CELL(toFIXED(ENTRANCE_PILLAR_C_MOVED_X)))
      Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2));
  }
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_CHESTS) {
    if (id == ACTOR_STONE_PILLAR &&
        FIELD_HALF_CELL(actor->x.fixed) ==
            FIELD_HALF_CELL(toFIXED(CHESTS_PILLAR_MOVED_X)))
      Event_SetFlag(FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED);
    if (id == ACTOR_SECOND_STONE_PILLAR &&
        FIELD_HALF_CELL(actor->x.fixed) ==
            FIELD_HALF_CELL(toFIXED(CHESTS_SECOND_PILLAR_MOVED_X)) &&
        FIELD_HALF_CELL(actor->z.fixed) ==
            FIELD_HALF_CELL(toFIXED(CHESTS_SECOND_PILLAR_MOVED_Z)))
      TreasureIsle_RaiseLastSpikyRock();
  }
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST) {
    if (id == ACTOR_STONE_PILLAR &&
        FIELD_HALF_CELL(actor->x.fixed) ==
            FIELD_HALF_CELL(toFIXED(ARMLET_PILLAR_MOVED_X)))
      Event_SetFlag(FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED);
  }
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_GALE) {
    if (id == ACTOR_GALE_ROOM_PILLAR &&
        FIELD_HALF_CELL(actor->x.fixed) ==
            FIELD_HALF_CELL(toFIXED(GALE_ROOM_PILLAR_MOVED_X)))
      Event_SetFlag(FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED);
  }
}

/// Talking to Gale, the Jupiter Djinni, starts its battle.
void TreasureIsle_FightGale(void) {
  Field_RequestDjinnBattle(ACTOR_GALE, DJINNI_GALE);
}

/// Talking to the Venus Djinni waiting in the west room before the Star
/// Magician's chamber, cell (12, 8), makes it join the party without a fight.
/// It is one of The Broken Seal's Venus Djinn the party lacks, and is not
/// there when none is lacking.
void TreasureIsle_VenusDjinniJoins(void) {
  Djinn_JoinParty(ACTOR_VENUS_DJINNI, DJINNI_EARTH19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_EARTH19));
}

/// Grind on a spiky rock in the chests' or the Jester's Armlet room crushes it
/// back into the water in three steps, clearing the way. Grind calls this as
/// it plays, with how far it has got in `frame`, and `id` says which rock.
void TreasureIsle_GrindSpikyRock(s32 unused, s32 frame, s32 id) {
  struct TreasureIsleCell *cells;

  if (id < ARMLET_FIRST_ROCK_CELL) {
    cells = gTreasureIsleChestsSpikyRocks;
    id -= CHESTS_FIRST_ROCK_CELL;
  } else {
    cells = gTreasureIsleArmletSpikyRocks;
    id -= ARMLET_FIRST_ROCK_CELL;
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

/// Lift on the east boulder of the south row in the west of Gale's room, cell
/// (20, 20), raises it into the air out of the way for good. While it stands,
/// Felix can jump across it from the cells either side.
DEFINE_LIFT_ROCK(TreasureIsle_LiftFirstRock, 0)

/// Lift on the middle boulder of the south row in the west of Gale's room, cell
/// (13, 19), raises it into the air out of the way for good. While it stands,
/// Felix can jump across it.
DEFINE_LIFT_ROCK(TreasureIsle_LiftSecondRock, 1)

/// Lift on the west boulder of the south row in the west of Gale's room, cell
/// (6, 20), raises it into the air out of the way for good. While it stands,
/// Felix can jump across it.
DEFINE_LIFT_ROCK(TreasureIsle_LiftThirdRock, 2)

/// Lift on the east boulder of the north row in the west of Gale's room, cell
/// (19, 13), raises it into the air out of the way for good. While it stands,
/// Felix can jump across it.
DEFINE_LIFT_ROCK(TreasureIsle_LiftFourthRock, 3)

/// Lift on the middle boulder of the north row in the west of Gale's room, cell
/// (13, 12), raises it into the air out of the way for good. While it stands,
/// Felix can jump across it.
DEFINE_LIFT_ROCK(TreasureIsle_LiftFifthRock, 4)

/// Lift on the west boulder of the north row in the west of Gale's room, cell
/// (7, 13), raises it into the air out of the way for good. While it stands,
/// Felix can jump across it.
DEFINE_LIFT_ROCK(TreasureIsle_LiftSixthRock, 5)

/// Lift on the boulder blocking the narrow passage north in the south-east of
/// Gale's room, cell (46, 45), raises it into the air above the passage for
/// good.
DEFINE_LIFT_ROCK(TreasureIsle_LiftSeventhRock, 6)

/// Lift on the boulder in the south of Gale's room, cell (26, 42), raises it
/// into the air out of the way for good.
DEFINE_LIFT_ROCK(TreasureIsle_LiftEighthRock, 7)

/// Walking up the steps to the Star Magician in the last chamber, cells (48,
/// 10) to (50, 10), until it is beaten: it stirs, the screen glows warm, and it
/// says "I am the guardian of water. If you would claim my power, you must
/// prove your worth." Then it moves at Felix and the battle with it and its
/// balls starts, with no escape.
void TreasureIsle_MeetStarMagician(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
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
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_STAR_MAGICIAN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldEvent_Wait(20);
  gMainRoutineState.pendingDjinni = IF_FLAG_SET(FLAG_STAR_MAGICIAN_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_HIDDEN_BOSSES,
                                        HIDDEN_BOSS_TREASURE_ISLE);
}

extern struct Trigger gTreasureIsleEntranceTriggers[];

/// The triggers of the room the party enters.
const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_TREASURE_ISLE_CHESTS)
    return gTreasureIsleChestsTriggers;
  if (scene == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST)
    return gTreasureIsleArmletTriggers;
  if (scene == SCENE_TREASURE_ISLE_GALE)
    return gTreasureIsleGaleTriggers;
  if (scene == SCENE_TREASURE_ISLE_GUARDIAN)
    return gTreasureIsleGuardianTriggers;
  return gTreasureIsleEntranceTriggers;
}

/// The water colours' cycle frame, -1 until the first frame saves the
/// palettes.
extern s32 gTreasureIsle_ColorCycleFrame[];

/// A task: cycles four colours of the two water palettes every eight frames,
/// saving both palettes the first time.
void TreasureIsle_CycleWaterColors(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 phase;
  s32 i;
  s32 busy;

  if (state->psynergy != 0)
    return;
  busy = (s8)state->revealActive;
  if (busy != 0)
    return;
  if (gTreasureIsle_ColorCycleFrame[0] == -1) {
    Iwram_CopyWords(gTreasureIsle_SavedWaterColorsA.colors,
                    (void *)&BG_PLTT_COLOR(WATER_PALETTE_A, 0),
                    sizeof(struct Palette));
    Iwram_CopyWordsEntry(gTreasureIsle_WaterColorsA.colors,
                         gTreasureIsle_SavedWaterColorsA.colors,
                         sizeof(struct Palette));
    Iwram_CopyWordsEntry(gTreasureIsle_SavedWaterColorsB.colors,
                         (void *)&BG_PLTT_COLOR(WATER_PALETTE_B, 0),
                         sizeof(struct Palette));
    Iwram_CopyWordsEntry(gTreasureIsle_WaterColorsB.colors,
                         gTreasureIsle_SavedWaterColorsB.colors,
                         sizeof(struct Palette));
    gTreasureIsle_ColorCycleFrame[0] = 0;
  }
  phase = gTreasureIsle_ColorCycleFrame[0] / 8 % 4;
  for (i = 0; i <= 3; i++) {
    gTreasureIsle_WaterColorsA.colors[(phase + i) % 4 + 2] =
        gTreasureIsle_SavedWaterColorsA.colors[i + 2];
    gTreasureIsle_WaterColorsB.colors[(phase + i) % 4 + 2] =
        gTreasureIsle_SavedWaterColorsB.colors[i + 2];
  }
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(WATER_PALETTE_A, 0),
                      (u32)gTreasureIsle_WaterColorsA.colors,
                      DMA_COPY_WORDS(PALETTE_COLORS / 2));
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(WATER_PALETTE_B, 0),
                      (u32)gTreasureIsle_WaterColorsB.colors,
                      DMA_COPY_WORDS(PALETTE_COLORS / 2));
  gTreasureIsle_ColorCycleFrame[0]++;
}

/// Treasure Isle's setup for each of its rooms: places the pillars already
/// pushed to their places, cycles the water colours in the chests' and the
/// Jester's Armlet rooms and stands their risen spiky rocks again, and in
/// Gale's room strings the tightropes and draws the boulders' shadows at the
/// lowest priority.
s32 Scene_Initialize(void) {
  s32 i;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Tightrope_Initialize();
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_ENTRANCE) {
    Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0));
    Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1));
    Event_SetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2));
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0)))
      Actor_SetPosition(ACTOR_ENTRANCE_PILLAR_A, ENTRANCE_PILLAR_A_MOVED_X,
                        ENTRANCE_PILLAR_A_MOVED_Z);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1)))
      Actor_SetPosition(ACTOR_ENTRANCE_PILLAR_B, ENTRANCE_PILLAR_B_MOVED_X,
                        ENTRANCE_PILLAR_B_MOVED_Z);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2)))
      Actor_SetPosition(ACTOR_ENTRANCE_PILLAR_C, ENTRANCE_PILLAR_C_MOVED_X,
                        ENTRANCE_PILLAR_C_MOVED_Z);
  }
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_CHESTS) {
    Task_AddFrame(TreasureIsle_CycleWaterColors, TASK_PASS_FRAME);
    Tightrope_Place(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_STONE_PILLAR, CHESTS_PILLAR_MOVED_X,
                        CHESTS_PILLAR_MOVED_Z);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)))
      TreasureIsle_SetSpikyRock(gTreasureIsleChestsSpikyRocks, 0);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)))
      TreasureIsle_SetSpikyRock(gTreasureIsleChestsSpikyRocks, 1);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)))
      TreasureIsle_SetSpikyRock(gTreasureIsleChestsSpikyRocks, 2);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)))
      TreasureIsle_SetSpikyRock(gTreasureIsleChestsSpikyRocks, 3);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)))
      TreasureIsle_SetSpikyRock(gTreasureIsleChestsSpikyRocks, 4);
  }
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_JESTERS_ARMLET_CHEST) {
    Task_AddFrame(TreasureIsle_CycleWaterColors, TASK_PASS_FRAME);
    Tightrope_Place(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_STONE_PILLAR, ARMLET_PILLAR_MOVED_X,
                        ARMLET_PILLAR_MOVED_Z);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)))
      TreasureIsle_SetSpikyRock(gTreasureIsleArmletSpikyRocks, 0);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)))
      TreasureIsle_SetSpikyRock(gTreasureIsleArmletSpikyRocks, 1);
    if (Event_GetFlag(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)))
      TreasureIsle_SetSpikyRock(gTreasureIsleArmletSpikyRocks, 2);
  }
  if (gMainRoutineState.next == SCENE_TREASURE_ISLE_GALE) {
    if (Event_GetFlag(FLAG_TREASURE_ISLE_GALE_ROOM_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_GALE_ROOM_PILLAR, GALE_ROOM_PILLAR_MOVED_X,
                        GALE_ROOM_PILLAR_MOVED_Z);
    Tightrope_Place(0, ACTOR_GALE_ROOM_TIGHTROPE_POST_A,
                    ACTOR_GALE_ROOM_TIGHTROPE_POST_B);
    Tightrope_Place(1, ACTOR_GALE_ROOM_TIGHTROPE_POST_C,
                    ACTOR_GALE_ROOM_TIGHTROPE_POST_D);
    Tightrope_Place(2, ACTOR_GALE_ROOM_TIGHTROPE_POST_E,
                    ACTOR_GALE_ROOM_TIGHTROPE_POST_F);
    for (i = 0; i <= 7; i++)
      ((struct FieldSprite *)Actor_Get(i + ACTOR_FIRST_BOULDER)->sprite)
          ->partPriority = 3;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Walking towards a gap from one of its jump cells, Felix jumps two cells
/// across it if there is ground to land on: in the chests' room from cell (18,
/// 11) east, in the Jester's Armlet room from cell (43, 15) north or east, and
/// in Gale's room across a standing boulder.
void TreasureIsle_JumpForward(void) { Field_TryJumpForward(); }

// clang-format off
#include "shared/field_effect_duration_scripts.c"

u32 gTreasureIsle_RockDust[] = {
    SC_ANIMATION(2),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_LOOP(44, SC_TO(1)),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The spiky rocks that rise in the chests' room and the Armlet's room: each
/// rock's cell, a number no code reads (in the chests' room, one of its five
/// sea rocks' actor ids), and the flag set while it stands.
struct TreasureIsleCell gTreasureIsleChestsSpikyRocks[] = {
    {7, 21, 12, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)},
    {7, 17, 13, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)},
    {12, 15, 14, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)},
    {9, 11, 15, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)},
    {46, 38, 16, FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(4)},
};
struct TreasureIsleCell gTreasureIsleArmletSpikyRocks[] = {
    {41, 23, 11, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)},
    {47, 23, 12, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)},
    {53, 23, 13, FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(2)},
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(TreasureIsleEntrance),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TREASURE_ISLE_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleEntrance, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleEntrance, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleEntrance, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TreasureIsleEntrance, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TreasureIsleEntrance, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TreasureIsleEntrance, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_ENTRANCE_TO_TREASURE_ISLE_CHESTS, TreasureIsleChests, TREASURE_ISLE_CHESTS_FROM_TREASURE_ISLE_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleChests),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_ENTRANCE, TreasureIsleEntrance, TREASURE_ISLE_ENTRANCE_FROM_TREASURE_ISLE_CHESTS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleChests, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleChests, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_EAST, TreasureIsleJestersArmletChest, TREASURE_ISLE_JESTERS_ARMLET_CHEST_FROM_TREASURE_ISLE_CHESTS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_WEST, TreasureIsleJestersArmletChest, TREASURE_ISLE_JESTERS_ARMLET_CHEST_FROM_TREASURE_ISLE_CHESTS_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleJestersArmletChest),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_EAST, TreasureIsleChests, TREASURE_ISLE_CHESTS_FROM_TREASURE_ISLE_JESTERS_ARMLET_CHEST_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_WEST, TreasureIsleChests, TREASURE_ISLE_CHESTS_FROM_TREASURE_ISLE_JESTERS_ARMLET_CHEST_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleJestersArmletChest, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleJestersArmletChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_GALE, TreasureIsleGale, TREASURE_ISLE_GALE_FROM_TREASURE_ISLE_JESTERS_ARMLET_CHEST, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleGale),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_GALE_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST, TreasureIsleJestersArmletChest, TREASURE_ISLE_JESTERS_ARMLET_CHEST_FROM_TREASURE_ISLE_GALE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleGale, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleGale, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TreasureIsleGale, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TreasureIsleGale, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_GALE_TO_TREASURE_ISLE_GUARDIAN, TreasureIsleGuardian, TREASURE_ISLE_GUARDIAN_FROM_TREASURE_ISLE_GALE, EXIT_FLAG_NONE),
    SCENE_EXITS(TreasureIsleGuardian),
    SCENE_EXIT_UNLESS(TREASURE_ISLE_GUARDIAN_TO_TREASURE_ISLE_GALE, TreasureIsleGale, TREASURE_ISLE_GALE_FROM_TREASURE_ISLE_GUARDIAN, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TreasureIsleGuardian, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TreasureIsleGuardian, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gTreasureIsle_PostStill[] = {
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gTreasureIsleEntrancePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTreasureIsleChestsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 600, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 680, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTreasureIsleArmletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 824, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gTreasureIsle_PostStill, 824, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

u32 gTreasureIsle_LiftRockScale[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE * 4 / 5),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 4 / 5),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gTreasureIsleGalePlacements[] = {
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

struct ActorPlacement gTreasureIsleGuardianPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAR_MAGICIAN, FLAG_STAR_MAGICIAN_BEATEN, MOVEMENT_TYPE_STAND, 792, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_EARTH19), MOVEMENT_TYPE_STAND, 200, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gTreasureIsleEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, TREASURE_ISLE_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS,
     TREASURE_ISLE_ENTRANCE_TO_TREASURE_ISLE_CHESTS},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR_A,
     FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(0),
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR_B,
     FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(1),
     EVENT(TreasureIsle_SettleMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENTRANCE_PILLAR_C,
     FLAG_TREASURE_ISLE_ENTRANCE_PILLAR_MOVED(2),
     EVENT(TreasureIsle_SettleMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTreasureIsleChestsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_ENTRANCE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_EAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     TREASURE_ISLE_CHESTS_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST_WEST},
    {TRIGGER_STEP, TRIGGER_BUTTON, CONDITION_ALWAYS,
     EVENT(TreasureIsle_PressChestsRoomButton)},
    {TRIGGER_STEP, TRIGGER_SWITCH, CONDITION_ALWAYS,
     EVENT(TreasureIsle_RaiseLastSpikyRock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_JUMP, CONDITION_ALWAYS,
     EVENT(TreasureIsle_JumpForward)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     CHESTS_FIRST_ROCK_CELL,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(0)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     CHESTS_FIRST_ROCK_CELL + 1,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(1)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     CHESTS_FIRST_ROCK_CELL + 2,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(2)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     CHESTS_FIRST_ROCK_CELL + 3,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_CHESTS_ROCK_RISEN(3)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     CHESTS_FIRST_ROCK_CELL + 4,
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

struct Trigger gTreasureIsleArmletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_EAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_CHESTS_WEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS,
     TREASURE_ISLE_JESTERS_ARMLET_CHEST_TO_TREASURE_ISLE_GALE},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TRIGGER_JUMP, CONDITION_ALWAYS,
     EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_JUMP, CONDITION_ALWAYS,
     EVENT(TreasureIsle_JumpForward)},
    {TRIGGER_STEP, TRIGGER_BUTTON, CONDITION_ALWAYS,
     EVENT(TreasureIsle_PressArmletRoomButton)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     ARMLET_FIRST_ROCK_CELL,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(0)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     ARMLET_FIRST_ROCK_CELL + 1,
     IF_FLAG_SET(FLAG_TREASURE_ISLE_ARMLET_ROCK_RISEN(1)),
     EVENT(TreasureIsle_GrindSpikyRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_GRIND, PSYNERGY_PHASE_ACTS),
     ARMLET_FIRST_ROCK_CELL + 2,
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
struct Trigger gTreasureIsleGaleTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
     TREASURE_ISLE_GALE_TO_TREASURE_ISLE_JESTERS_ARMLET_CHEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS,
     TREASURE_ISLE_GALE_TO_TREASURE_ISLE_GUARDIAN},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_FIRST_BOULDER_JUMP,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(0), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_FIRST_BOULDER_JUMP + 1,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(0), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_FIRST_BOULDER_JUMP + 2,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(1), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_FIRST_BOULDER_JUMP + 3,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(1), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_FIRST_BOULDER_JUMP + 4,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(2), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_FIRST_BOULDER_JUMP + 5,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(2), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_FIRST_BOULDER_JUMP + 6,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(3), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_FIRST_BOULDER_JUMP + 7,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(3), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_FIRST_BOULDER_JUMP + 8,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(4), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_FIRST_BOULDER_JUMP + 9,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(4), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_FIRST_BOULDER_JUMP + 10,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(5), EVENT(TreasureIsle_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), TRIGGER_FIRST_BOULDER_JUMP + 11,
     FLAG_TREASURE_ISLE_BOULDER_LIFTED(5), EVENT(TreasureIsle_JumpForward)},
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

struct Trigger gTreasureIsleGuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     TREASURE_ISLE_GUARDIAN_TO_TREASURE_ISLE_GALE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, TRIGGER_BEFORE_STAR_MAGICIAN, FLAG_STAR_MAGICIAN_BEATEN,
     EVENT(TreasureIsle_MeetStarMagician)},
    {TRIGGER_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS,
     EVENT(TreasureIsle_VenusDjinniJoins)},
    {TRIGGER_TABLE_END},
};

s32 gTreasureIsle_ColorCycleFrame[] = {
    -1,
};

#undef GALE_TALK_TRIGGER
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
