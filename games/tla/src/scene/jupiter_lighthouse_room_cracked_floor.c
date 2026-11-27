// Rooms of Jupiter Lighthouse just below its aerie, on one map. Their stairs lead up to the aerie
// (later to the beacon's floor) and down to the rooms of the blue key and the red key. Standing on
// the button in the room of cracked tiles holds a stone door open; the pillar beside it, moved onto
// it (scJupiterLighthouseCheckPillarOnButton), keeps it open, a shortcut through the tower. The
// room's cracked tiles give way under a second step (scJupiterLighthouseStepOnCrackedTile),
// dropping the party into the hall of the four winged statues below, where a single cracked tile in
// a ring of studs (scJupiterLighthouseStepOnSingleCrackedTile) drops it into the room of the blue
// key.

// This unit declares its own form of fdMapGridGetMetatileBehavior, below.
#define OWN_fdMapGridGetMetatileBehavior

#include "field/field_mode.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "field/metatile_target.h"
#include "scene/lighthouse_palettes.h"

// The ROM's callers here are compiled with no declaration in scope, as against this form: no
// parameter list and an Sint32 result, where the definition takes three Sint32 values and returns a
// Uint8.
Sint32 fdMapGridGetMetatileBehavior();

// Set while the leader stands on the button by the door, which holds it open.
#define FLAG_DOOR_BUTTON_HELD FLAG_SCENE_TEMP_00

// The pillar the party pushes onto the button by the door, after the four winged statues in
// ActorPlacements.
enum
{
    ACTOR_SHORTCUT_PILLAR = ACTOR_FIRST_PLACED + 4
};

// The button's metatile, which the pillar is moved onto.
enum
{
    DOOR_BUTTON_METATILE_X = 4,
    DOOR_BUTTON_METATILE_Z = 37
};

// The step cues of the button: its metatile, and the three metatiles round it that the leader steps
// off it onto.
enum
{
    CUE_DOOR_BUTTON     = 20,
    CUE_OFF_DOOR_BUTTON = 21
};

// The metatile behavior of the single cracked tile in the hall below, which takes the next one up
// as it cracks; the room's own are METATILE_BEHAVIOR_CRACKED_TILE.
enum
{
    METATILE_BEHAVIOR_SINGLE_CRACKED_TILE = 43
};

// The entrances to the room of cracked tiles: up the stairs from the hall below, and down the
// stairs from the room above.
enum
{
    ENTRANCE_CRACKED_TILES_FROM_BELOW = 5,
    ENTRANCE_CRACKED_TILES_FROM_ABOVE
};

// How scJupiterLighthouseOpenDoor opens the door: at once, as the room is set up, or grinding open
// with a sound.
enum
{
    DOOR_OPEN_AT_ONCE,
    DOOR_GRINDS_OPEN
};

void                         scJupiterLighthouseOpenDoor(Sint32 how);
void                         scJupiterLighthouseCheckPillarOnButton(void);
void                         scCrackedTilePlaceLanding(Sint32 x, Sint32 z, Sint32 effect);
Sint32 WhirlwindRiding       BSS_DATA;
Uint16                       PaletteCycleColors[16] BSS_DATA;
// Palette 13's first two colours; it is copied whole, with the unlit colours after them.
Uint16                       PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16                       PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile       BSS_DATA;
Sint16 LightSpriteCount      BSS_DATA;
ACTOR                       *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM                    LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge     BSS_DATA;
Sint16 LightSpritePriority   BSS_DATA;
METATILE_TARGET CrackedTiles BSS_DATA;
void                         scJupiterLighthouseStepOffButton(void);
void                         scJupiterLighthouseFinishMovingBlock(void);

const SCENE_ENTRANCE        *scGetEntrances(void)
{
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    return ActorPlacements;
}

extern Sint32 PaletteCyclePausesInScripts;
void          scJupiterLighthouseCloseDoor(void);

// A cracked tile of the room under a grounded leader: as it gives way the party falls to the hall
// below.
void          scJupiterLighthouseStepOnCrackedTile(Sint32 value)
{
    if (fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed == 0 && scCrackedTileStep(value))
        syFieldScriptSetExitRequest(THROUGH_CRACKED_TILE);
}

// The single cracked tile of the hall below under a grounded leader: as it gives way the party
// falls into the room of the blue key.
void scJupiterLighthouseStepOnSingleCrackedTile(Sint32 value)
{
    if (fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed == 0 && scCrackedTileStep(value))
        syFieldScriptSetExitRequest(
            JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH);
}

// Opens the door, at once or grinding open through a tick between shut and open.
void scJupiterLighthouseOpenDoor(Sint32 how)
{
    if (how == DOOR_GRINDS_OPEN)
    {
        fdScriptPlaySound(SE_IMPACT);
        fdMapGridCopyMetatilesLayered(84, 36, 2, 3, 78, 40);
        syWait(6);
    }
    fdMapGridCopyMetatilesLayered(84, 39, 2, 3, 78, 40);
    fdMapGridCopyMetatileAttributes(20, 38, 2, 1, 14, 42);
}

// Closes the door with a thud.
void scJupiterLighthouseCloseDoor(void)
{
    fdScriptPlaySound(SE_HEAVY_IMPACT);
    fdMapGridCopyMetatilesLayered(84, 36, 2, 3, 78, 40);
    syWait(6);
    fdMapGridCopyMetatilesLayered(84, 42, 2, 3, 78, 40);
    fdMapGridCopyMetatileAttributes(20, 44, 2, 1, 14, 42);
}

// The shortcut pillar moved onto the button holds the door open for good.
void scJupiterLighthouseCheckPillarOnButton(void)
{
    ACTOR *actor = fdActorGet(ACTOR_SHORTCUT_PILLAR);

    if (!fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED)
        && FIELD_METATILE(actor->x.fixed) == DOOR_BUTTON_METATILE_X
        && FIELD_METATILE(actor->z.fixed) == DOOR_BUTTON_METATILE_Z)
    {
        fdScriptSetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
        scJupiterLighthouseOpenDoor(DOOR_GRINDS_OPEN);
    }
}

// Stepping on the button opens the door.
void scJupiterLighthouseStepOnButton(void)
{
    if (!fdScriptGetFlag(FLAG_DOOR_BUTTON_HELD))
    {
        fdScriptSetFlag(FLAG_DOOR_BUTTON_HELD);
        scJupiterLighthouseOpenDoor(DOOR_GRINDS_OPEN);
    }
}

// Stepping off the button closes the door, unless the shortcut pillar now holds it.
void scJupiterLighthouseStepOffButton(void)
{
    if (fdScriptGetFlag(FLAG_DOOR_BUTTON_HELD) == 1)
    {
        fdScriptClearFlag(FLAG_DOOR_BUTTON_HELD);
        scJupiterLighthouseCloseDoor();
    }
    scJupiterLighthouseCheckPillarOnButton();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const CUE *scGetCues(void)
{
    return SceneCues;
}

extern Sint32      PaletteCycleTick;

// Sets the palette cycle's tick (PaletteCycleTick).
static inline void scPaletteCycleSetTick(Sint32 value)
{
    PaletteCycleTick = value;
}

// Sets up the rooms as the party enters: the window in, the door as the pillar or the button leaves
// it, and the cracked tiles of the room entered: the room of cracked tiles by its stairs, or else
// the hall below, with the landing of a fall from above. Returns 0.
Sint32 scInit(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    state->transition  = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED))
    {
        fdActorSetPos(ACTOR_SHORTCUT_PILLAR, 72, 600);
        scJupiterLighthouseOpenDoor(DOOR_OPEN_AT_ONCE);
    }
    if (fdScriptGetFlag(FLAG_DOOR_BUTTON_HELD))
        scJupiterLighthouseOpenDoor(DOOR_OPEN_AT_ONCE);
    if (MainRoutineStateWork.entrance >= ENTRANCE_CRACKED_TILES_FROM_BELOW
        && MainRoutineStateWork.entrance <= ENTRANCE_CRACKED_TILES_FROM_ABOVE)
    {
        scCrackedTileFind(
            METATILE_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 20, 31);
    }
    else
    {
        scCrackedTileFind(
            METATILE_BEHAVIOR_SINGLE_CRACKED_TILE, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 21, 31);
        scCrackedTilePlaceLanding(
            21, -3, SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
    }
    return 0;
}

// Sets the scene's arrival flags and loads the lighthouse's palettes; the pillar goes back unless
// the party stays in place. Returns 0.
Sint32 scPrepareMap(void)
{
    scSetArrivalFlags(0);
    scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
    if (!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
        fdScriptClearFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
    return 0;
}

#include "shared/scene_set_arrival_flags.c"

void syGameFlagSetByte(Sint32 flag, Sint32 value);

#include "shared/palette_cycle_set_pauses_in_scripts.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

// Unused: holds an actor 32 pixels above its ground, without gravity, its metatile raised 6 steps
// over the terrain and made a gap to jump.
UNUSED void scJupiterLighthouseFloatAboveGround(Sint32 id)
{
    ACTOR *actor;
    Sint32 height;

    actor                 = fdActorGet(id);
    actor->motionFlags    = ACTOR_FLOATS;
    height                = actor->groundHeight;
    actor->bounce         = 0;
    actor->y.fixed        = height + toFIXED(32);
    actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
    height                = fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    fdMapGridSetMetatileHeight(
        actor->x.fixed, actor->z.fixed, 0, METATILE_HEIGHT_STEPS(height) + 6);
    fdMapGridSetMetatileAttributesAt(actor->x.fixed, actor->z.fixed, 0, MAP_METATILE_JUMP);
}

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern Sint32 WhirlwindEndScript[];

#include "shared/whirlwind_update.h"

void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

// Sets the priority the lit actors' sprites are drawn at.
void scLightSpritesSetPriority(Uint16 value)
{
    LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomCrackedFloor),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST),
    SCENE_EXIT(4, JupiterLighthouseRoomCrackedFloor, 5),
    SCENE_EXIT(5, JupiterLighthouseRoomCrackedFloor, 4),
    SCENE_EXIT(6, JupiterLighthouseRoomCrackedFloor, 7),
    SCENE_EXIT(7, JupiterLighthouseRoomCrackedFloor, 6),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST),
    SCENE_EXIT(11, JupiterLighthouseRoomCrackedFloor, 12),
    SCENE_EXIT(12, JupiterLighthouseRoomCrackedFloor, 11),
    SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomCrackedFloor, THROUGH_CRACKED_TILE),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_NORTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 456, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 632, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 632, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 264, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 264, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

CUE SceneCues[] = {
    // Once Isaac and Ivan have fallen, the stairs up lead to the beacon's floor.
    {CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A},
    {CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B},
    {CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST},
    {CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST},
    {CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_STEP, CUE_DOOR_BUTTON, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, SCRIPT_REF(scJupiterLighthouseStepOnButton)},
    {CUE_STEP, CUE_OFF_DOOR_BUTTON, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, SCRIPT_REF(scJupiterLighthouseStepOffButton)},
    {CUE_STEP, METATILE_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnCrackedTile)},
    {CUE_STEP, METATILE_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnCrackedTile)},
    {CUE_STEP, METATILE_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnCrackedTile)},
    {CUE_STEP, METATILE_BEHAVIOR_SINGLE_CRACKED_TILE, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnSingleCrackedTile)},
    {CUE_STEP, METATILE_BEHAVIOR_SINGLE_CRACKED_TILE + 1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnSingleCrackedTile)},
    {CUE_STEP, METATILE_BEHAVIOR_SINGLE_CRACKED_TILE + 2, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnSingleCrackedTile)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SHORTCUT_PILLAR, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, SCRIPT_REF(scJupiterLighthouseFinishMovingBlock)},
    {CUE_TABLE_END},
};

Sint32 PaletteCycleTick = -1;

Sint32 PaletteCyclePausesInScripts = 1;

Sint32 WhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

#include "../field/field_effect_move.c"

#undef FLAG_DOOR_BUTTON_HELD

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

struct METATILE_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void scCrackedTileCrack(METATILE_TARGET *state, METATILE_TARGET_ENTRY *entry);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
