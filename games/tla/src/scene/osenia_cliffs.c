/// @file
/// The Osenia Cliffs, across the Madra Drawbridge, where the wreck of a Champa
/// ship lies on the shore and three people from Madra look it over until Piers
/// has carried Gabomba's message.
///
/// The party hops across the wreckage to the exit on the east side. A rope
/// already stretches between two posts near the villagers; in the northeast,
/// two more posts stand bare until Felix ties a rope between them with Lash, on
/// the way to the Pirate's Sword.

#include "system/task.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "field/field_state.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "system/io_reg.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "system/frame.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

extern u32 gOseniaCliffs_PaletteCycleStep[];

#ifdef MODERN
void Rope_Tie(s32 a, s32 b, s32 c);
#endif

void Rope_Place(s32 index, s32 flag, s32 post, s32 otherPost);

#include "shared/field_rope_place.c"

void OseniaCliffs_CyclePalette(void);

/// Set once the party has lashed the second rope, which stays tied for the
/// visit.
#define FLAG_OSENIA_CLIFFS_ROPE_LASHED FLAG_SCENE_TEMP_03

/// The actors, numbering the rows of `gActorPlacements`.
enum OseniaCliffsActorId {
  ACTOR_VILLAGER_CHAMPA_BOAT = ACTOR_FIRST_PLACED,
  ACTOR_VILLAGER_NOT_CHAMPA,
  ACTOR_VILLAGER_WAVE_DAMAGE,
  /// The posts of the rope near the villagers, tied from the start.
  ACTOR_FIRST_ROPE_POST,
  ACTOR_SECOND_ROPE_POST,
  /// The posts in the northeast, low and high, that Lash ties a rope between.
  ACTOR_THIRD_ROPE_POST,
  ACTOR_FOURTH_ROPE_POST
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

#ifdef MODERN
void Rope_Lash(s32 a, s32 b, s32 c);
#endif

/// Felix casts Lash at the lower of the two bare rope posts in the northeast of
/// the Osenia Cliffs, the one on the wooden landing in the water, and a rope
/// flies from it up to the post on the clifftop to the north-east and ties
/// itself taut between them. Felix walks up it as a tightrope onto the
/// clifftop, on the way to the Pirate's Sword. It stays tied until the party
/// leaves.
void OseniaCliffs_LashRope(void) {
  Rope_Lash(1, ACTOR_THIRD_ROPE_POST, ACTOR_FOURTH_ROPE_POST);
  Event_SetFlag(FLAG_OSENIA_CLIFFS_ROPE_LASHED);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void Rope_Initialize(void);

s32 Scene_Initialize(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapGrid;

  map->wading = 1;
  gOseniaCliffs_PaletteCycleStep[0] = 0;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Add(OseniaCliffs_CyclePalette, TASK_PASS_FRAME);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

    leader->priorityFlags =
        ACTOR_PRIORITY_IGNORES_RISES | leader->priorityFlags;
  }
  Rope_Initialize();
  Rope_Tie(0, ACTOR_SECOND_ROPE_POST, ACTOR_FIRST_ROPE_POST);
  Actor_SetAnimation(ACTOR_SECOND_ROPE_POST, 4);
  Actor_SetAnimation(ACTOR_FIRST_ROPE_POST, 4);
  FieldRope_Place(1, FLAG_OSENIA_CLIFFS_ROPE_LASHED, ACTOR_THIRD_ROPE_POST,
                  ACTOR_FOURTH_ROPE_POST);
  return 0;
}

/// The palette cycle: the eight colours of one background palette, and the
/// four of another that show four of them in turn: the blues of the line where
/// the sea meets the foot of the cliffs, and of glints on the sea.
#define CYCLE_SOURCE_PALETTE 12
#define CYCLE_SOURCE_COLORS 8
#define SHORELINE_PALETTE 6

/// The sea shimmers where it meets the foot of the cliffs: every 16 frames,
/// four of `CYCLE_SOURCE_PALETTE`'s eight colours, starting one further each
/// time, are copied to colours 1 to 4 of `SHORELINE_PALETTE`.
void OseniaCliffs_CyclePalette(void) {
  if ((gFrameCount & 15) == 0) {
    volatile u16 *colors;
    s32 i;

    if (gOseniaCliffs_PaletteCycleStep[0] == CYCLE_SOURCE_COLORS)
      gOseniaCliffs_PaletteCycleStep[0] = 0;
    i = gOseniaCliffs_PaletteCycleStep[0]++;
    colors = (u16 *)&BG_PLTT_COLOR(CYCLE_SOURCE_PALETTE, 0);
    BG_PLTT_COLOR(SHORELINE_PALETTE, 1) = colors[i];
    BG_PLTT_COLOR(SHORELINE_PALETTE, 2) =
        colors[(i + 1) & (CYCLE_SOURCE_COLORS - 1)];
    BG_PLTT_COLOR(SHORELINE_PALETTE, 3) =
        colors[(i + 2) & (CYCLE_SOURCE_COLORS - 1)];
    BG_PLTT_COLOR(SHORELINE_PALETTE, 4) =
        colors[(i + 3) & (CYCLE_SOURCE_COLORS - 1)];
  }
}

s32 Scene_PrepareMap(void) { return 0; }

/// The sea's shimmer, its step through the source colours.
u32 gOseniaCliffs_PaletteCycleStep[] = {
    0,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(OseniaCliffs),
    SCENE_EXIT_UNLESS(1, WorldMap, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, WorldMap, 13, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR,
                    FLAG_GABOMBA_MESSAGE_CARRIED, MOVEMENT_TYPE_STAND, 200, 0,
                    226, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, FLAG_GABOMBA_MESSAGE_CARRIED,
                    MOVEMENT_TYPE_STAND, 259, 0, 312, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, FLAG_GABOMBA_MESSAGE_CARRIED,
                    MOVEMENT_TYPE_STAND, 242, 0, 313, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 240, 0, 184, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 424, 0, 104, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 472, 0, 104, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_VILLAGER_CHAMPA_BOAT, CONDITION_ALWAYS,
     MSGNO(MsgMadraChampaBoat)},
    {TRIGGER_TALK, ACTOR_VILLAGER_NOT_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraSailorNotChampa)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WAVE_DAMAGE, CONDITION_ALWAYS,
     MSGNO(MsgMadraWaveDamage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_CHAMPA_BOAT,
     CONDITION_ALWAYS, MSGNO(MsgMadraCannotEscapeBySea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NOT_CHAMPA,
     CONDITION_ALWAYS, MSGNO(MsgMadraBriggsNewShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WAVE_DAMAGE,
     CONDITION_ALWAYS, MSGNO(MsgMadraPirateAttackParties)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_THIRD_ROPE_POST,
     FLAG_OSENIA_CLIFFS_ROPE_LASHED, EVENT(OseniaCliffs_LashRope)},
    {TRIGGER_TABLE_END},
};
