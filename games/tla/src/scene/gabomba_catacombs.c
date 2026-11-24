/// @file
/// The Gabomba Catacombs, the caves below the Gabomba Statue, which Felix
/// reaches by the ladder down from inside the statue once Piers can cast
/// Frost. Each cave is a room of this overlay.
///
/// The ladder comes down at the north-east of the first cave, where three
/// stones lie hidden under the weeds. Cyclone on one uncovers it, and a
/// whirlwind carries Felix down into the cave below, each stone to its own
/// spot. There the Venus Djinni Mud waits in plain sight and fights
/// (GabombaCatacombs_FightMud); stairs lead back up to the first cave, and a
/// way on leads into the cave of the hidden wall. In that cave Cyclone on the
/// big patch of weeds in the east uncovers a Mint. In the west it scatters a
/// cross of five weeds from a puddle, and Frost freezes the puddle into a
/// pillar Felix hops across to a dead end ringed by six more weeds over
/// puddles; Reveal there shows the passage on to the stairs down. In the
/// stream cave below, the shallow water pushes Felix south as he wades, and
/// Cyclone and Frost make a second pillar on the way to the stairs on the
/// rock in the middle. They climb to the last cave, where Cyclone on the
/// weeds wakes the first Mad Plant, and where the Great Gabomba, speaking
/// through the idol at the top, gives the party the Tomegathericon
/// (GabombaCatacombs_GabombaBestowsPower).

#include "flags.h"
#include "djinn_ids.h"
#include "shared/tables.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "types.h"
#include "graphics/palette_buffers.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/scene_tables.h"
#include "system/frame.h"
#include "system/scene.h"
#include "field/event_inline.h"
#include "scene/event_layer.h"
#include "field/field.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "version.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/event_commands.h"

/// Set once the Great Gabomba has given the party its power, the
/// Tomegathericon (GabombaCatacombs_GabombaBestowsPower).
#define FLAG_GABOMBA_BESTOWED_POWER 0x9ce

void Actor_StartScript(struct Actor *actor, const void *script);
s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);
void Event_End(void);

extern u32 gGabombaCatacombs_SplashScript[];

/// A Mad Plant of the given sprite leaps out at Felix from (x, z) and attacks.
static __inline__ void GabombaCatacombs_MadPlantLeaps(s32 sprite, s32 x,
                                                      s32 z) {
  MadPlant_LeapAtLeader(sprite, x, z);
}

extern u32 gFieldEffect_MediumScript[];
extern struct SceneRegion gGabombaCatacombs_TeleportersExitRegions[];
extern struct ActorPlacement gGabombaCatacombs_Placements[];
extern struct ActorPlacement gGabombaCatacombs_TeleportersActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_MudActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_HiddenWallActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_TomegathericonActorPlacements[];
extern struct ActorPlacement gGabombaCatacombs_StreamActorPlacements[];
extern struct Trigger gGabombaCatacombs_TeleportersTriggers[];
extern struct Trigger gGabombaCatacombs_MudTriggers[];
extern struct Trigger gGabombaCatacombs_HiddenWallTriggers[];
extern struct Trigger gGabombaCatacombs_TomegathericonTriggers[];
extern struct Trigger gGabombaCatacombs_StreamTriggers[];

void Actor_DepartInRing(s32 id);

#include "text/msg_ids.h"

/// Whether the leader stands on the cell (cellX, cellZ).
#define LEADER_ON_CELL(leader, cellX, cellZ)                                   \
  (FIELD_CELL((leader)->x.fixed) == (cellX) &&                                 \
   FIELD_CELL((leader)->z.fixed) == (cellZ))

/// Takes control from the player and starts Felix's hop off a square.
#define LEADER_HOP()                                                           \
  do {                                                                         \
    Event_Start();                                                             \
    Event_EndLastingPsynergy(0);                                               \
    FieldEffect_SetAttached(gMainRoutineState.leaderActorId,                   \
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);  \
  } while (0)

/// Mud, the Djinni waiting in the cave below the teleport stones.
enum { ACTOR_MUD = ACTOR_FIRST_PLACED };

/// The hidden wall's cave's seven puddles under the weeds Cyclone scatters
/// (the first under the cross of five weeds in the west, the rest in the ring
/// of six at the dead end below the hidden wall), the stream cave's puddle,
/// and the last cave's seated idol the Great Gabomba speaks through, and
/// Kraden.
enum {
  ACTOR_FIRST_PUDDLE = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  ACTOR_FOURTH_PUDDLE,
  ACTOR_FIFTH_PUDDLE,
  ACTOR_SIXTH_PUDDLE,
  ACTOR_SEVENTH_PUDDLE
};
/// The stream cave's puddle.
enum { ACTOR_STREAM_PUDDLE = ACTOR_FIRST_PLACED };
/// The last cave's seated idol, and Kraden.
enum { ACTOR_SEATED_IDOL = ACTOR_FIRST_PLACED, ACTOR_KRADEN };

/// Each puddle Frost has frozen into a pillar, by the puddles above.
#define FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
#define FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
#define FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_04
#define FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_05
#define FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_06
#define FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN FLAG_SCENE_TEMP_00

/// The map cells the caves' trigger rows answer, by what lies on them.
enum GabombaCatacombsCellBehavior {
  /// The first cave: its weeds, and the three teleport stones hidden in its
  /// west, east and south patches of weeds.
  CELL_BEHAVIOR_TELEPORTERS_WEEDS = 41,
  CELL_BEHAVIOR_WEST_TELEPORT_STONE = 43,
  CELL_BEHAVIOR_EAST_TELEPORT_STONE = 44,
  CELL_BEHAVIOR_SOUTH_TELEPORT_STONE = 45,
  /// The hidden wall's cave: the big patch of weeds in the east and the
  /// square in it where the Mint lies, the cross of five weeds in the west
  /// and the puddle under it, and the six puddles of the ring at the dead
  /// end.
  CELL_BEHAVIOR_MINT_PATCH_WEEDS = 40,
  CELL_BEHAVIOR_CROSS_WEEDS = 41,
  CELL_BEHAVIOR_FIRST_PUDDLE = 42,
  CELL_BEHAVIOR_SECOND_PUDDLE,
  CELL_BEHAVIOR_THIRD_PUDDLE,
  CELL_BEHAVIOR_FOURTH_PUDDLE,
  CELL_BEHAVIOR_FIFTH_PUDDLE,
  CELL_BEHAVIOR_SIXTH_PUDDLE,
  CELL_BEHAVIOR_SEVENTH_PUDDLE,
  CELL_BEHAVIOR_MINT = 49,
  /// The stream cave: the weeds by the stairs up, the column of weeds over
  /// the puddle in the west and the puddle itself, the weeds in the north,
  /// and the water of the stream.
  CELL_BEHAVIOR_STAIRS_WEEDS = 40,
  CELL_BEHAVIOR_PUDDLE_WEEDS = 41,
  CELL_BEHAVIOR_STREAM_PUDDLE = 42,
  CELL_BEHAVIOR_NORTH_WEEDS = 43,
  CELL_BEHAVIOR_STREAM = 50,
  /// The last cave: its clump of weeds and the square in it where the Mad
  /// Plant hides, and the square in front of the idol.
  CELL_BEHAVIOR_MAD_PLANT_WEEDS = 30,
  CELL_BEHAVIOR_MAD_PLANT = 31,
  CELL_BEHAVIOR_BEFORE_THE_IDOL = 40
};

/// Talking to Mud, the Venus Djinni waiting in plain sight in the cave below
/// the teleport stones, starts its battle.
void GabombaCatacombs_FightMud(void) {
  Field_RequestDjinnBattle(ACTOR_MUD, DJINNI_MUD);
}

/// Cyclone scatters the weeds of the first cave, below the ladder.
void GabombaCatacombs_ScatterTeleportersWeeds(void) {
  FieldCyclone_CopyTouchedCells(7, 63, 1, 16, 16, 57, 63);
}

/// Cyclone scatters the big patch of weeds in the east of the hidden wall's
/// cave, around cell (24, 17).
void GabombaCatacombs_ScatterHiddenWallWeeds(void) {
  FieldCyclone_CopyTouchedCells(22, 15, 0, 5, 5, 63, 52);
}

/// Cyclone scatters the cross of five weeds in the west of the hidden wall's
/// cave, around cell (8, 23).
void GabombaCatacombs_ScatterHiddenWallSmallWeeds(void) {
  FieldCyclone_CopyTouchedCells(7, 22, 0, 3, 3, 56, 58);
}

/// Cyclone scatters the weeds by the stairs up from the stream cave, around
/// cell (46, 26).
void GabombaCatacombs_ScatterStreamWeeds(void) {
  FieldCyclone_CopyTouchedCells(44, 25, 0, 4, 3, 66, 57);
}

/// Cyclone scatters the column of weeds over the puddle in the west of the
/// stream cave, cells (16, 14) to (19, 20).
void GabombaCatacombs_ScatterStreamWideWeeds(void) {
  FieldCyclone_CopyTouchedCells(16, 13, 0, 4, 8, 61, 52);
}

/// Cyclone scatters the weeds in the north of the stream cave, at cell
/// (22, 10).
void GabombaCatacombs_ScatterStreamSmallWeeds(void) {
  FieldCyclone_CopyTouchedCells(21, 9, 0, 2, 2, 57, 52);
}

/// Cyclone on the big patch of weeds in the east of the hidden wall's cave
/// uncovers the Mint hidden at cell (24, 21), in the middle of the patch's
/// south edge. Until the Mint is taken, its treasure is put on that square, and
/// if Felix stands there he hops a square east off it.
void GabombaCatacombs_UncoverMint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(22, 15, 0, 5, 5, 63, 52);
  if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_MINT_TAKEN) == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 392, 344);
    if (LEADER_ON_CELL(leader, 24, 21)) {
      LEADER_HOP();
      Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
      leader->velocityY = toFIXED(6);
      Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone on the patch of weeds in the west of the first cave uncovers the
/// teleport stone at cell (10, 15), and a whirlwind carries Felix down into
/// the cave below, west of Mud.
void GabombaCatacombs_UncoverWestTeleportStone(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 9, 65);
  Import_EventSetExitRequest(
      GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_WEST);
}

/// Cyclone on the patch of weeds in the east of the first cave uncovers the
/// teleport stone at cell (19, 16), and a whirlwind carries Felix down into
/// the cave below, to its east.
void GabombaCatacombs_UncoverEastTeleportStone(void) {
  MapGrid_CopyCellsLayered(51, 83, 3, 1, 18, 66);
  Import_EventSetExitRequest(
      GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_EAST);
}

/// Cyclone on the teleport stone in the patch of weeds in the south of the
/// first cave, cell (13, 24), whirls Felix down into the cave below, to its
/// south. Unlike the other two stones, this one is never hidden: it lies bare
/// among the weeds from the start, so no picture is redrawn here. The cast
/// scatters leaves, the whirlwind carries Felix onto the stone and spins him
/// down through it, and he lands on the path above the ladder in the cave
/// below; the weeds around the stone stay.
void GabombaCatacombs_TakeSouthTeleportStone(void) {
  Import_EventSetExitRequest(
      GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_SOUTH);
}

/// Cyclone on the middle of the cross of five weeds in the west of the hidden
/// wall's cave, cell (8, 23), shows the puddle under it. If Felix stands on
/// the puddle's square, he hops a square east off it.
void GabombaCatacombs_UncoverFirstPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(7, 22, 0, 3, 3, 56, 58);
  Actor_SetPosition(ACTOR_FIRST_PUDDLE, 136, 376);
  if (LEADER_ON_CELL(leader, 8, 23)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds in the north-east of the ring of six weeds at the dead
/// end below the hidden wall, cell (8, 15), shows the puddle under them. If
/// Felix stands on the puddle's square, he hops a square east off it.
void GabombaCatacombs_UncoverSecondPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_SECOND_PUDDLE, 136, 248);
  if (LEADER_ON_CELL(leader, 8, 15)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds in the east of the ring of six weeds at the dead end
/// below the hidden wall, cell (9, 16), shows the puddle under them. If Felix
/// stands on the puddle's square, he hops a square west off it.
void GabombaCatacombs_UncoverThirdPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_THIRD_PUDDLE, 152, 264);
  if (LEADER_ON_CELL(leader, 9, 16)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, -16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds in the south-east of the ring of six weeds at the dead
/// end below the hidden wall, cell (8, 17), shows the puddle under them. If
/// Felix stands on the puddle's square, he hops a square east off it.
void GabombaCatacombs_UncoverFourthPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_FOURTH_PUDDLE, 136, 280);
  if (LEADER_ON_CELL(leader, 8, 17)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds in the south-west of the ring of six weeds at the dead
/// end below the hidden wall, cell (6, 17), shows the puddle under them. If
/// Felix stands on the puddle's square, he hops a square east off it.
void GabombaCatacombs_UncoverFifthPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_FIFTH_PUDDLE, 104, 280);
  if (LEADER_ON_CELL(leader, 6, 17)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds in the west of the ring of six weeds at the dead end
/// below the hidden wall, cell (5, 16), shows the puddle under them. If Felix
/// stands on the puddle's square, he hops a square east off it.
void GabombaCatacombs_UncoverSixthPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_SIXTH_PUDDLE, 88, 264);
  if (LEADER_ON_CELL(leader, 5, 16)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the weeds in the north-west of the ring of six weeds at the dead
/// end below the hidden wall, cell (6, 15), shows the puddle under them. If
/// Felix stands on the puddle's square, he hops a square east off it.
void GabombaCatacombs_UncoverSeventhPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(5, 13, 0, 5, 3, 56, 52);
  Actor_SetPosition(ACTOR_SEVENTH_PUDDLE, 104, 248);
  if (LEADER_ON_CELL(leader, 6, 15)) {
    LEADER_HOP();
    Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, 16, 0);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone on the column of weeds in the west of the stream cave shows the
/// puddle under them at cell (17, 18). If Felix stands on the puddle's
/// square, he hops a square north off it.
void GabombaCatacombs_UncoverStreamPuddle(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  FieldCyclone_CopyTouchedCells(16, 13, 0, 4, 8, 61, 52);
  Actor_SetPosition(ACTOR_STREAM_PUDDLE, 280, 296);
  if (LEADER_ON_CELL(leader, 17, 18)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 0, -16);
    leader->velocityY = toFIXED(6);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Event_End();
  }
}

/// Cyclone scatters the clump of weeds in the south-west of the last cave.
void GabombaCatacombs_ScatterTomegathericonWeeds(void) {
  FieldCyclone_CopyTouchedCells(7, 23, 0, 4, 5, 60, 56);
}

/// Cyclone on the clump of weeds in the south-west of the last cave, at
/// cell (10, 26), wakes the first Mad Plant, which leaps out at Felix and
/// fights; the party cannot run. Once it is beaten it does not come back.
void GabombaCatacombs_UncoverMadPlant(void) {
  FieldCyclone_CopyTouchedCells(7, 23, 0, 4, 5, 60, 56);
  if (GameFlag_Get(FLAG_MAD_PLANT(0)) == 0) {
    GabombaCatacombs_MadPlantLeaps(SPRITE_MAD_PLANT, 168, 424);
    gMainRoutineState.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(0));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS, 0);
  }
}

/// Reveal at the dead end below the hidden wall shows a passage through the
/// wall north of the ring of weeds, cells (7, 11) to (7, 14), on to the stairs
/// down to the stream cave.
void GabombaCatacombs_RevealWall(void) {
  MapGrid_CopyCellTilesCellsRect(0, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 4, 1, 1, 7, 14);
}

/// As Reveal wears off, the wall north of the ring of weeds closes again.
void GabombaCatacombs_HideWall(void) {
  MapGrid_CopyCellTilesCellsRect(1, 0, 1, 2, 7, 11);
  Map_CopyCellAttributeRect(2, 5, 1, 1, 7, 14);
}

/// Frost on the puddle in the cross of weeds in the west of the hidden wall's
/// cave, cell (8, 23), freezes it into an ice pillar. Its square becomes one
/// Felix hops onto, like the squares on either side, so he can hop across
/// to the dead end below the hidden wall.
void GabombaCatacombs_FreezeFirstPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FIRST_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
}

/// Frost on the puddle in the north-east of the ring of six weeds at the dead
/// end below the hidden wall, cell (8, 15), freezes it into an ice pillar. The
/// ring's pillars, unlike the cross's, do not become squares to hop onto: each
/// takes the attributes of cell (0, 4), plain ground 48 pixels up, 16 above the
/// ring's floor. The ring's six cells, (6, 15), (8, 15), (9, 16), (8, 17),
/// (6, 17) and (5, 16), lie on that floor and not over a gap, so its pillars
/// bridge nothing.
void GabombaCatacombs_FreezeSecondPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SECOND_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
}

/// Frost on the puddle in the east of the ring of six weeds at the dead end
/// below the hidden wall, cell (9, 16), freezes it into an ice pillar.
void GabombaCatacombs_FreezeThirdPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_THIRD_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 16);
}

/// Frost on the puddle in the south-east of the ring of six weeds at the dead
/// end below the hidden wall, cell (8, 17), freezes it into an ice pillar.
void GabombaCatacombs_FreezeFourthPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FOURTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
}

/// Frost on the puddle in the south-west of the ring of six weeds at the dead
/// end below the hidden wall, cell (6, 17), freezes it into an ice pillar.
void GabombaCatacombs_FreezeFifthPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FIFTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
}

/// Frost on the puddle in the west of the ring of six weeds at the dead end
/// below the hidden wall, cell (5, 16), freezes it into an ice pillar.
void GabombaCatacombs_FreezeSixthPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SIXTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 16);
}

/// Frost on the puddle in the north-west of the ring of six weeds at the dead
/// end below the hidden wall, cell (6, 15), freezes it into an ice pillar.
void GabombaCatacombs_FreezeSeventhPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SEVENTH_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
}

/// Frost on the puddle in the west of the stream cave, cell (17, 18), freezes
/// it into an ice pillar. Its square becomes one Felix hops onto, like the
/// squares on either side, on the way to the stairs on the rock in the
/// middle.
void GabombaCatacombs_FreezeStreamPuddle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_STREAM_PUDDLE);

  GameFlag_Set(FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN);
  actor->motionFlags = 0;
  actor->priorityFlags = 0;
  MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
}

/// The stream carries Felix, and the view with him, dz further south, unless
/// a wall or post lies just south of him or diagonally ahead.
void GabombaCatacombs_DriftWithStream(struct Actor *actor, s32 dz) {
  struct Actor *camera;
  struct FieldPosition position;

  camera = gFieldState->camera;

  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed + dz;
  MapGrid_CheckCollision(actor, &position);
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed + toFIXED(8);
  if (MapGrid_CheckCollision(actor, &position) <= 0) {
    position.x = actor->x.fixed + toFIXED(57) / 10;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed + toFIXED(57) / 10;
    if (MapGrid_CheckCollision(actor, &position) <= 0) {
      position.x = actor->x.fixed - toFIXED(57) / 10;
      position.y = actor->y.fixed;
      position.z = actor->z.fixed + toFIXED(57) / 10;
      if (MapGrid_CheckCollision(actor, &position) <= 0) {
        camera->z.fixed += dz;
        actor->z.fixed += dz;
      }
    }
  }
}

/// Wading in the shallow stream that runs south through the stream cave,
/// Felix splashes, with the sound of wading, every eighth frame, and the
/// water pushes him slowly south.
void GabombaCatacombs_WadeStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *splash;

  if ((gFrameCount & 7) == 0) {
    splash = ((struct Actor *)Import_ActorCreate(
        SPRITE_WADING_SPLASH, leader->x.fixed, leader->y.fixed + toFIXED(6),
        leader->z.fixed));
    if (splash) {
      Actor_SetSpriteFlags(splash, 0);
      Actor_SelectAnimation(splash, 1);
      Actor_StartScript(splash, gGabombaCatacombs_SplashScript);
    }
    Sound_Play(SE_WADE);
  }
  GabombaCatacombs_DriftWithStream(leader, toFIXED(17) / 20);
}

/// The flash of light about the Tomegathericon as it appears on the idol's
/// lap: each frame it shrinks by a twentieth of its full size.
s32 GabombaCatacombs_ShrinkGlow(struct Actor *actor) {
  actor->scaleX -= FIXED_ONE / 20;
  actor->scaleY -= FIXED_ONE / 20;
  return 0;
}

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void Actor_PlaceOnActor(s32 actorId, s32 target);

/// Shows the conversation's next line, centred.
#define TALK()                                                                 \
  EventMessage_ShowCentered((s16)((union EventLine *)&state->message)->line++, \
                            0, -3)

/// When Felix steps in front of the idol at the top of the last cave, between
/// the two candlesticks, the cave darkens and the Great Gabomba speaks
/// through the idol: "By reaching this place, you have proven your power."
/// The Tomegathericon appears in a flash on the idol's lap. Kraden steps out
/// beside Felix and worries that they have wronged Akafubu, and the Gabomba
/// gives the power on the condition that they never tell Akafubu. Kraden
/// agrees, the Gabomba goes back to sleep, and the light returns with the
/// book still on the idol's lap.
void GabombaCatacombs_GabombaBestowsPower(void) {
  struct Actor *kraden = Actor_GetById(ACTOR_KRADEN);
  struct Actor *treasure = Actor_GetById(FIELD_TREASURE_ACTOR);
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *glow, *leader;
  s32 speed;

  Event_Start();
  Event_EndLastingPsynergy(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 158, 1);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  Sound_Play(SE_STATUE_STIRS);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgByReachingThis);
  TALK();
  Actor_SetPosition(FIELD_TREASURE_ACTOR, 168, 152);
  treasure->motionFlags = ACTOR_FALLS | ACTOR_FLOATS;
  treasure->groundHeight += toFIXED(16);
  Sound_Play(SE_SHIMMER);
  glow = ((struct Actor *)Import_ActorCreate(SPRITE_FIREBALL, 0, 0, 0));
  Actor_StartScript(glow, gFieldEffect_MediumScript);
  Actor_SetSpriteColorMode(glow, 1);
  glow->motionFlags = 0;
  glow->x.fixed = treasure->x.fixed;
  glow->y.fixed = treasure->y.fixed + toFIXED(8);
  glow->z.fixed = treasure->z.fixed;
  glow->groundHeight = treasure->groundHeight;
  glow->update = GabombaCatacombs_ShrinkGlow;
  Event_Wait(15);
  glow->x.fixed = 0;
  glow->y.fixed = 0;
  glow->z.fixed = 0;
  glow->update = 0;
  TALK();
  Actor_SetDestinationById(gMainRoutineState.leaderActorId, 166, 188);
  Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  speed = toFIXED(1);
  kraden->speedLimit = speed;
  Actor_SetDestinationById(ACTOR_KRADEN, 184, 188);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  // "I am grateful for your promise...But I am worried that we have done..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  TALK();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "Then what are we to do? Does this mean we won't receive your magic?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  TALK();
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(30);
  TALK();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_Wait(30);
  // "Wh-What kind of conditions?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  TALK();
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_WEST, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  TALK();
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  TALK();
  Event_Wait(40);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
  Event_Wait(40);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  // "Of course!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  TALK();
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 4);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  GameFlag_Set(FLAG_GABOMBA_BESTOWED_POWER);
  // The cave's music and its light come back.
  Scene_PlayMusic();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(60);
  FieldCamera_MoveToActorById(gMainRoutineState.leaderActorId, 1);
  Event_Wait(60);
  Event_End();
}

/// Talking to the idol at the top of the last cave, once the Tomegathericon
/// lies on its lap, takes the book.
void GabombaCatacombs_TalkToStatue(void) {
  struct Actor *actor = Actor_GetById(FIELD_TREASURE_ACTOR);

  if (actor != NULL && actor->x.fixed != 0)
    Scene_TalkToActor(FIELD_TALK_SILENT | FIELD_TREASURE_ACTOR);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return gGabombaCatacombs_TeleportersExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct Trigger gGabombaCatacombs_Triggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return gGabombaCatacombs_TeleportersActorPlacements;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_MUD)
    return gGabombaCatacombs_MudActorPlacements;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
    return gGabombaCatacombs_HiddenWallActorPlacements;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
    return gGabombaCatacombs_TomegathericonActorPlacements;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_STREAM)
    return gGabombaCatacombs_StreamActorPlacements;
  return gGabombaCatacombs_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS)
    return gGabombaCatacombs_TeleportersTriggers;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_MUD)
    return gGabombaCatacombs_MudTriggers;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL)
    return gGabombaCatacombs_HiddenWallTriggers;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON)
    return gGabombaCatacombs_TomegathericonTriggers;
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_STREAM)
    return gGabombaCatacombs_StreamTriggers;
  return gGabombaCatacombs_Triggers;
}

/// The transition the catacombs open with: the window's shape until patch 1, an
/// interlaced wipe from it.
#if !TLA_PATCH_1
#define CATACOMBS_SCREEN_EFFECT                                                \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#define CATACOMBS_SCREEN_EFFECT                                                \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

extern char SceneId_GabombaStatuePinion, SceneId_GabombaStatueChamberIdol;
void FieldEffect_StartBlankOverlay(s32 mode);

/// Sets up the room the party comes into: coming down the ladder into the first
/// cave, where a retreat leads back to in the statue above; landing from a
/// teleport stone beside Mud, in a ring of light; the puddles frozen into
/// pillars on this visit; and the Tomegathericon on the idol's lap until it is
/// taken.
s32 Scene_Initialize(void) {
  s32 entrance;
  struct Actor *treasure;

  gFieldState->transition = CATACOMBS_SCREEN_EFFECT;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TELEPORTERS &&
      (entrance = gMainRoutineState.entrance) ==
          GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR) {
    if (GameFlag_Get(FLAG_GABOMBA_RETREAT_TO_PINION)) {
      gMainRoutineState.retreatScene = SCENE_GABOMBA_STATUE_PINION;
      gMainRoutineState.retreatEntrance = entrance;
    } else if (GameFlag_Get(FLAG_GABOMBA_RETREAT_TO_IDOL)) {
      gMainRoutineState.retreatScene = SCENE_GABOMBA_STATUE_CHAMBER_IDOL;
      gMainRoutineState.retreatEntrance =
          GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS;
    }
  }
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_MUD &&
      gMainRoutineState.entrance >
          GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL)
    Actor_DepartInRing(gMainRoutineState.leaderActorId);
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_HIDDEN_WALL) {
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(6, 23, 1, 1, 8, 23);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 15);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 9, 16);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 8, 17);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 17);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 5, 16);
    if (GameFlag_Get(FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(0, 4, 1, 1, 6, 15);
  }
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_STREAM &&
      GameFlag_Get(FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(15, 18, 1, 1, 17, 18);
  if (gMainRoutineState.next == SCENE_GABOMBA_CATACOMBS_TOMEGATHERICON &&
      GameFlag_Get(FLAG_GABOMBA_BESTOWED_POWER) &&
      !GameFlag_Get(FLAG_TOMEGATHERICON_TAKEN) &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    treasure = Actor_GetById(FIELD_TREASURE_ACTOR);
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 168, 152);
    treasure->motionFlags = ACTOR_FALLS | ACTOR_FLOATS;
    treasure->groundHeight += toFIXED(16);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
u32 gGabombaCatacombs_SplashScript[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1), SC_LOOP(44, SC_TOP),
    SC_WAIT(1), SC_POSITION(0, 0, 0),
    SC_WAIT(1), SC_RELEASE,
    SC_END,
};

#include "shared/scene_entrances_default.c"

struct SceneRegion gGabombaCatacombs_TeleportersExitRegions[] = {
    {388, 46, 276, 396, 54, 284, CONDITION_ALWAYS, 1},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GabombaCatacombsTeleporters),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, GabombaStatueChamberWitchDoctor, GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_CATACOMBS_TELEPORTERS),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_WEST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_WEST),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_EAST, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_EAST),
    SCENE_EXIT(GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_SOUTH, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_TELEPORTERS_SOUTH),
    SCENE_EXITS(GabombaCatacombsMud),
    SCENE_EXIT(GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST, GabombaCatacombsTeleporters, GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_CATACOMBS_MUD_NORTHEAST),
    SCENE_EXIT(GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL, GabombaCatacombsHiddenWall, GABOMBA_CATACOMBS_HIDDEN_WALL_FROM_GABOMBA_CATACOMBS_MUD),
    SCENE_EXITS(GabombaCatacombsHiddenWall),
    SCENE_EXIT(GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD, GabombaCatacombsMud, GABOMBA_CATACOMBS_MUD_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL),
    SCENE_EXIT(GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM, GabombaCatacombsStream, GABOMBA_CATACOMBS_STREAM_FROM_GABOMBA_CATACOMBS_HIDDEN_WALL),
    SCENE_EXITS(GabombaCatacombsTomegathericon),
    SCENE_EXIT(GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM, GabombaCatacombsStream, GABOMBA_CATACOMBS_STREAM_FROM_GABOMBA_CATACOMBS_TOMEGATHERICON),
    SCENE_EXITS(GabombaCatacombsStream),
    SCENE_EXIT(GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL, GabombaCatacombsHiddenWall, GABOMBA_CATACOMBS_HIDDEN_WALL_FROM_GABOMBA_CATACOMBS_STREAM),
    SCENE_EXIT(GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON, GabombaCatacombsTomegathericon, GABOMBA_CATACOMBS_TOMEGATHERICON_FROM_GABOMBA_CATACOMBS_STREAM),
    SCENE_EXITS_END,
};

struct ActorPlacement gGabombaCatacombs_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_TeleportersActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_MudActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MUD), MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_HiddenWallActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_TomegathericonActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIT_CANDLESTICK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaCatacombs_StreamActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gGabombaCatacombs_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_TeleportersTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST,
     CONDITION_ALWAYS,
     GABOMBA_CATACOMBS_TELEPORTERS_TO_GABOMBA_CATACOMBS_MUD_NORTHEAST},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_TELEPORTERS_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterTeleportersWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_WEST_TELEPORT_STONE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverWestTeleportStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_EAST_TELEPORT_STONE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverEastTeleportStone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SOUTH_TELEPORT_STONE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_TakeSouthTeleportStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_MudTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST,
     CONDITION_ALWAYS,
     GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_TELEPORTERS_NORTHEAST},
    {TRIGGER_EXIT, GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL,
     CONDITION_ALWAYS, GABOMBA_CATACOMBS_MUD_TO_GABOMBA_CATACOMBS_HIDDEN_WALL},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_MUD, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_FightMud)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_HiddenWallTriggers[] = {
    {TRIGGER_EXIT, GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD,
     CONDITION_ALWAYS, GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_MUD},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM,
     CONDITION_ALWAYS,
     GABOMBA_CATACOMBS_HIDDEN_WALL_TO_GABOMBA_CATACOMBS_STREAM},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_RevealWall)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(GabombaCatacombs_HideWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MINT_PATCH_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterHiddenWallWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_CROSS_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterHiddenWallSmallWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FIRST_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverFirstPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SECOND_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverSecondPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_THIRD_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverThirdPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FOURTH_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverFourthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FIFTH_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverFifthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SIXTH_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverSixthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SEVENTH_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverSeventhPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MINT, CONDITION_ALWAYS, EVENT(GabombaCatacombs_UncoverMint)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_FIRST_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_SECOND_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_THIRD_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeThirdPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FOURTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_FOURTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeFourthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_FIFTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeFifthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIXTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_SIXTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeSixthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SEVENTH_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_SEVENTH_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeSeventhPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_TomegathericonTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM,
     CONDITION_ALWAYS,
     GABOMBA_CATACOMBS_TOMEGATHERICON_TO_GABOMBA_CATACOMBS_STREAM},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_THE_IDOL, FLAG_GABOMBA_BESTOWED_POWER,
     EVENT(GabombaCatacombs_GabombaBestowsPower)},
    {TRIGGER_TALK, ACTOR_SEATED_IDOL, FLAG_TOMEGATHERICON_TAKEN,
     EVENT(GabombaCatacombs_TalkToStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAD_PLANT_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterTomegathericonWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAD_PLANT, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverMadPlant)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaCatacombs_StreamTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL,
     CONDITION_ALWAYS,
     GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_HIDDEN_WALL},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON,
     CONDITION_ALWAYS,
     GABOMBA_CATACOMBS_STREAM_TO_GABOMBA_CATACOMBS_TOMEGATHERICON},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_STAIRS_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterStreamWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_PUDDLE_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterStreamWideWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_NORTH_WEEDS, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_ScatterStreamSmallWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_STREAM_PUDDLE, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_UncoverStreamPuddle)},
    {TRIGGER_STEP, CELL_BEHAVIOR_STREAM, CONDITION_ALWAYS,
     EVENT(GabombaCatacombs_WadeStream)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_STREAM_PUDDLE,
     FLAG_GABOMBA_CATACOMBS_STREAM_PUDDLE_FROZEN,
     EVENT(GabombaCatacombs_FreezeStreamPuddle)},
    {TRIGGER_TABLE_END},
};
