// Five of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone Isle.
//
// Each opens off one of the nine floors the cave descends through (crossbone_isle.c) through two
// doors, east and west. The floor's east door leads in at the room's `_EAST` entrance
// (CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST in the first) and its west
// door at the `_WEST` one. The exits of the same names lead back out.
//
// Each is a puzzle with four chests, and all five take the area name Crossbone Isle Cave. In the
// order the party meets them:
//
// - SceneIdCrossboneIsleSteppingStones, off the first floor: raised squares
//       over a pit, with four stone pillars on them. Walking into a pillar
// jumps Isaac across it where there is a square to land on, and otherwise pushes it
// (scCrossboneIsleSteppingStonesJumpOrPushPillar), so the pillars are
//       moved until a line of squares leads to each chest and to the far door.
//
// - SceneIdCrossboneIsleIcePillars, off the second floor: columns lying
//       north to south and east to west, pushed by walking into them until the
//       way to the Mystery Blade's chest is clear.
//
// - SceneIdCrossboneIsleFloodgate, off the fifth floor: a cave with a stream
//       and a floodgate across it, and logs to roll.
//
//       Searching the switch beside the floodgate
//       (scCrossboneIsleSteppingStonesFlipWaterSwitch) closes it, so the water
//       backs up and the logs float higher, or opens it to drain them back
// down. The water level lasts only while the party stays
//       (FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED, kept through a battle).
//
// - SceneIdCrossboneIsleStackedPillars, off the seventh floor, a cave of
//       cliffs, ladders and brown pits. Its square pillars are shifted with
// Move and lifted onto ledges with Carry, and one can stand on another; each
//       settles after every push (scCrossboneIsleSteppingStonesMovePillar).
// Pushed off a cliff, a pillar drops out of play.
//
// - SceneIdCrossboneIsleDragonStatue, off the eighth floor, below a stone
//       dragon: two logs, a small boulder and three puddles that Frost freezes
//       into pillars of ice.
//
//       The puddles are by the west ladder, by the east ladder, and in front of
//       the dragon under a white platform. Frost on that one lifts the platform
//       on a pillar of ice, and the dragon breathes fire on the puddle by the
//       east ladder, melting its ice. It freezes only if Isaac was not standing
//       in it.
//
//       Move pushes the small boulder below the logs east until it settles into
//       the floor two steps along, for good.
//
// The other three side rooms are in crossbone_isle_keys.c, and the ninth floor's in
// crossbone_isle.c.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

extern Sint32         CrossboneIsleSteppingStonesIceShineScript[];
extern Uint16         CrossboneIsleSteppingStonesSwitchCloseSteps[];
extern Uint16         CrossboneIsleSteppingStonesSwitchOpenSteps[];
extern Uint16         CrossboneIsleSteppingStonesFloodgateCloseSteps[];
extern Uint16         CrossboneIsleSteppingStonesFloodgateOpenSteps[];
extern Uint16         CrossboneIsleSteppingStonesFloodgateFootSteps[];
extern SCENE_ENTRANCE CrossboneIsleSteppingStonesEntrances[];
extern SCENE_ENTRANCE CrossboneIsleIcePillarsEntrances[];
extern SCENE_ENTRANCE CrossboneIsleFloodgateEntrances[];
extern SCENE_ENTRANCE CrossboneIsleStackedPillarsEntrances[];
extern SCENE_ENTRANCE CrossboneIsleDragonStatueEntrances[];
extern Uint32         CrossboneIsleSteppingStonesExits[];
extern ACTOR_PLACE    CrossboneIsleSteppingStonesActorPlacements[];
extern ACTOR_PLACE    CrossboneIsleIcePillarsActorPlacements[];
extern ACTOR_PLACE    CrossboneIsleFloodgateActorPlacements[];
extern ACTOR_PLACE    CrossboneIsleSteppingStonesActorPlacementsOther[];
extern ACTOR_PLACE    CrossboneIsleDragonStatueActorPlacements[];
extern CUE            CrossboneIsleSteppingStonesCuesOther[];
extern CUE            CrossboneIsleSteppingStonesCues[];
extern CUE            CrossboneIsleIcePillarsCues[];
extern CUE            CrossboneIsleFloodgateCues[];
extern CUE            CrossboneIsleStackedPillarsCues[];
extern CUE            CrossboneIsleDragonStatueCues[];

// The placed actors, numbered in the order of each room's actor placements (scGetActorPlacements).
typedef enum CROSSBONE_ISLE_STEPPING_STONES_ACTOR_ID
{
    // The pillars of the first floor's side room (four, SPRITE_STONE_PILLAR), the second floor's
    // side room (five) and the seventh floor's side room (four, SPRITE_SPIRAL_PILLAR), the only
    // actors those rooms place.
    ACTOR_FIRST_PILLAR    = ACTOR_FIRST_PLACED,
    // The logs, SPRITE_LOG_NORTH_SOUTH and SPRITE_LOG_WEST_EAST as Kolima Forest's. There are eight
    // in the floodgate room, where the seven after the first float on the raised water, and two in
    // the dragon statue's room.
    ACTOR_FIRST_LOG       = ACTOR_FIRST_PLACED,
    // The dragon statue's room's boulder for Move, SPRITE_TURNING_ROCK, which sinks into place at
    // SINKING_BOULDER_COLUMN.
    ACTOR_SINKING_BOULDER = ACTOR_FIRST_LOG + 2,
    // The dragon statue's room's three puddles, SPRITE_PUDDLE, which Frost freezes into pillars of
    // ice: by the west ladder, by the east ladder, and in front of the stone dragon.
    ACTOR_FIRST_PUDDLE,
    ACTOR_SECOND_PUDDLE,
    ACTOR_THIRD_PUDDLE,
    // The white platform in the puddle in front of the stone dragon, SPRITE_WHITE_PLATFORM, which a
    // pillar of ice lifts two steps as Frost freezes the puddle.
    ACTOR_RISING_PILLAR,
    // The floodgate room's water, SPRITE_SPLASH, raised behind the closed floodgate.
    ACTOR_WATER = ACTOR_FIRST_LOG + 8
} CROSSBONE_ISLE_STEPPING_STONES_ACTOR_ID;

// The rooms' chests, named for what each holds; each room numbers its own.
enum
{
    TREASURE_SPOT_STEPPING_STONES_ANTIDOTE    = TREASURE_SPOT_FIRST_ID,
    TREASURE_SPOT_STEPPING_STONES_NUT         = TREASURE_SPOT_FIRST_ID + 1,
    TREASURE_SPOT_STEPPING_STONES_111_COINS   = TREASURE_SPOT_FIRST_ID + 2,
    TREASURE_SPOT_STEPPING_STONES_HARD_NUT    = TREASURE_SPOT_FIRST_ID + 3,
    TREASURE_SPOT_ICE_PILLARS_LUCKY_PEPPER    = TREASURE_SPOT_FIRST_ID,
    TREASURE_SPOT_ICE_PILLARS_MYSTERY_BLADE   = TREASURE_SPOT_FIRST_ID + 1,
    TREASURE_SPOT_ICE_PILLARS_222_COINS       = TREASURE_SPOT_FIRST_ID + 2,
    TREASURE_SPOT_ICE_PILLARS_ELIXIR          = TREASURE_SPOT_FIRST_ID + 3,
    TREASURE_SPOT_FLOODGATE_555_COINS         = TREASURE_SPOT_FIRST_ID,
    TREASURE_SPOT_FLOODGATE_LUCKY_MEDAL       = TREASURE_SPOT_FIRST_ID + 1,
    TREASURE_SPOT_FLOODGATE_POTION            = TREASURE_SPOT_FIRST_ID + 2,
    TREASURE_SPOT_FLOODGATE_NINJA_GARB        = TREASURE_SPOT_FIRST_ID + 3,
    TREASURE_SPOT_STACKED_PILLARS_777_COINS   = TREASURE_SPOT_FIRST_ID,
    TREASURE_SPOT_STACKED_PILLARS_LUCKY_MEDAL = TREASURE_SPOT_FIRST_ID + 1,
    TREASURE_SPOT_STACKED_PILLARS_PSY_CRYSTAL = TREASURE_SPOT_FIRST_ID + 2,
    TREASURE_SPOT_STACKED_PILLARS_WICKED_MACE = TREASURE_SPOT_FIRST_ID + 3,
    TREASURE_SPOT_DRAGON_STATUE_888_COINS     = TREASURE_SPOT_FIRST_ID,
    TREASURE_SPOT_DRAGON_STATUE_POTION        = TREASURE_SPOT_FIRST_ID + 1,
    TREASURE_SPOT_DRAGON_STATUE_CLERICS_RING  = TREASURE_SPOT_FIRST_ID + 2,
    TREASURE_SPOT_DRAGON_STATUE_SMOKE_BOMB    = TREASURE_SPOT_FIRST_ID + 3
};

// The floodgate room: the water has been raised.
#define FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED                     FLAG_SCENE_TEMP_00
// The seventh floor's side room: the first pillar has another pushed on top of it, so Move and
// Carry leave it be. The four pillars' flags follow from this one.
#define FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER FLAG_SCENE_TEMP_00
// The dragon statue's room: the first puddle is frozen. The second's and third's flags follow from
// this one.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN          FLAG_SCENE_TEMP_00
// The dragon statue's room: the leader stood on the third puddle when Frost began, so it stays
// unfrozen.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE       FLAG_SCENE_TEMP_03
// The dragon statue's room: the boulder Move pushes has sunk into place at SINKING_BOULDER_COLUMN.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK                 FLAG_SCENE_TEMP_04

// The map column where the dragon statue's room's boulder sinks into place.
#define SINKING_BOULDER_COLUMN                                         16

// The seventh floor's side room's own metatile behaviors, on layer 2: where a pillar pushed onto
// one falls down the cliff, and where it sinks into the floor. Either way it then leaves play.
enum
{
    METATILE_BEHAVIOR_STACKED_PILLARS_DROP = 50,
    METATILE_BEHAVIOR_STACKED_PILLARS_SINK = 51
};

// The seventh floor's side room's metatiles that run scripts as Isaac walks: among the pillars, at
// the ladders, on the lowest floor and where he leaves it.
enum
{
    METATILE_BEHAVIOR_STACKED_PILLARS_PILLARS            = 30,
    METATILE_BEHAVIOR_STACKED_PILLARS_LADDER             = 31,
    METATILE_BEHAVIOR_STACKED_PILLARS_LOWEST_FLOOR       = 40,
    METATILE_BEHAVIOR_STACKED_PILLARS_LEAVE_LOWEST_FLOOR = 41
};

// SPRITE_TURNING_ROCK's round boulder (turning_rock.tsv's picture 1), as the dragon statue's room's
// boulder is drawn.
#define TURNING_ROCK_ANIMATION_BOULDER 2

// The metatiles beside the pillars, columns, logs and boulder of the first, second, fifth and
// eighth floors' side rooms, from which walking into one pushes it.
enum
{
    METATILE_BEHAVIOR_BESIDE_PUSHED_ACTOR = 10
};

// The floodgate room's switch, searched to flip it.
enum
{
    METATILE_BEHAVIOR_FLOODGATE_SWITCH = 20
};

// While the floodgate room's water changes level, the background maps hold the stream in its other
// level twelve metatile rows below where they show it (scMapGridDrawMetatilePanel's rows 19 and
// 31), and the top WATER_DISSOLVE_LINES lines of the screen show it scrolled down to there, line by
// line, more often each tick.
#define WATER_PANEL_SCROLL_Y (12 * MAPGRID_METATILE_PIXELS)

// The floodgate room's map: its layers 1 and 2 begin at the map's columns 32 and 64, and each water
// level is kept below the room as those two layers side by side, the raised water from row 32 and
// the drained from row 64. The stream the level changes in is the 16 by 5 metatiles from (10, 19).
enum
{
    FLOODGATE_LAYER_1_COLUMN = 32,
    FLOODGATE_LAYER_2_COLUMN = 64,
    RAISED_WATER_ROW         = 32,
    DRAINED_WATER_ROW        = 64,
    STREAM_COLUMN            = 10,
    STREAM_ROW               = 19,
    // Where the stream at its other level is drawn, WATER_PANEL_SCROLL_Y below.
    WATER_PANEL_ROW          = STREAM_ROW + 12
};

#define WATER_DISSOLVE_LINES 48

// A damped particle loses a PARTICLE_DRAG-th of its speed across the ground each tick, and a
// PARTICLE_DRAG_Y-th of its speed up or down.
enum
{
    PARTICLE_DRAG   = 10,
    PARTICLE_DRAG_Y = 3
};

// A copied map metatile, also read in the two flag pairs the pillar redraw keeps.
typedef union CROSSBONE_ISLE_PILLAR_METATILE
{
    MAP_GRID_METATILE metatile;

    struct CROSSBONE_ISLE_PILLAR_METATILE_BITS
    {
        Uint32 metatileTiles : 12;
        // bits 13..12 of MAP_GRID_METATILE.metatileTilesAndFlags
        Uint32 lowFlags      : 2;
        // bits 15..14, read by fdMapGridGetMetatileHighFlags
        Uint8  highFlags     : 2;
        Uint8  behavior;
        Uint8  shape;
    } bits;
} CROSSBONE_ISLE_PILLAR_METATILE;

STATIC_ASSERT(sizeof(CROSSBONE_ISLE_PILLAR_METATILE) == sizeof(MAP_GRID_METATILE));
STATIC_ASSERT(offsetof(CROSSBONE_ISLE_PILLAR_METATILE, bits.behavior) == 2);
STATIC_ASSERT(offsetof(CROSSBONE_ISLE_PILLAR_METATILE, bits.shape) == 3);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"

#include "shared/map_grid_panel.c"

// Waits up to sixty ticks for landing, then stops vertical movement on the ground.
static void scCrossboneIsleSteppingStonesSettleActorOnGround(ACTOR *actor)
{
    Sint32 remainingTicks = 60;

    for (;;)
    {
        if (remainingTicks == 0)
        {
            break;
        }
        sySynch(1);
        if (actor->y.fixed == actor->groundHeight)
        {
            break;
        }
        remainingTicks--;
    }

    actor->velocityY = 0;
    actor->targetY   = ACTOR_NO_TARGET;
    actor->y.fixed   = actor->groundHeight;
}

// Waits until the actor reaches the ground or height limit, then stops its descent.
static void scCrossboneIsleSteppingStonesStopAtGroundOrHeight(ACTOR *actor, Sint32 limit)
{
    Sint32 remainingTicks = 60;

    for (;;)
    {
        if (remainingTicks == 0)
        {
            break;
        }
        sySynch(1);
        if (actor->y.fixed <= actor->groundHeight)
        {
            break;
        }
        if (actor->y.fixed <= limit)
        {
            break;
        }
        remainingTicks--;
    }

    actor->velocityY = 0;
    actor->targetY   = ACTOR_NO_TARGET;
}

// Finds the metatiles the actor covers: their width and depth, the top-left one, and the metatile
// where the map grid's layer 1 starts.
//
// The actor's edges are its row of PushedActorRectangles. An actor whose animation is none of the
// six kinds gets PUSHED_ACTOR_NO_RECTANGLE, past them, and is refused.
static Sint32 scCrossboneIsleSteppingStonesQueryCoveredMetatiles(Sint32              id,
                                                                 Uint32             *width,
                                                                 Uint32             *depth,
                                                                 PUSHED_ACTOR_PROBE *probe,
                                                                 Sint32             *layerOriginX,
                                                                 Sint32             *layerOriginZ)
{
    MAP_GRID_SCROLL_STATE *grid  = MapGridState[0];
    ACTOR                 *actor = fdActorGet(id);
    Uint32                 i;
    Sint32                 coordinate;
    Sint32                 farExtent;

    for (i = 0; i <= 5; i++)
    {
        if (actor->sprite->entries[0]->animationId == PushedActorRectangleKinds[i])
        {
            probe->rectangleIndex = i;
            break;
        }
        probe->rectangleIndex = PUSHED_ACTOR_NO_RECTANGLE;
    }
    if (probe->rectangleIndex < 0 || probe->rectangleIndex > 6)
    {
        return 0;
    }
    // Bounds are pixel offsets; dimensions and origins below are 16-pixel metatiles.
    probe->pos.x = actor->x.fixed;
    probe->pos.y = actor->y.fixed;
    probe->pos.z = actor->z.fixed;
    coordinate   = PushedActorRectangles[probe->rectangleIndex].z0;
    if (coordinate < 0)
    {
        coordinate = -coordinate;
    }
    farExtent = PushedActorRectangles[probe->rectangleIndex].z1;
    if (farExtent < 0)
    {
        farExtent = -farExtent;
    }
    *depth     = (coordinate + farExtent) >> 4;
    coordinate = PushedActorRectangles[probe->rectangleIndex].x0;
    if (coordinate < 0)
    {
        coordinate = -coordinate;
    }
    farExtent = PushedActorRectangles[probe->rectangleIndex].x1;
    if (farExtent < 0)
    {
        farExtent = -farExtent;
    }
    *width        = (coordinate + farExtent) >> 4;
    // Move from the actor's center to the top-left metatile it covers.
    probe->pos.x += toFIXED(PushedActorRectangles[probe->rectangleIndex].x0);
    {
        Sint32 offsetZ  = toFIXED(PushedActorRectangles[probe->rectangleIndex].z0);

        coordinate      = probe->pos.z;
        coordinate     += offsetZ;
        coordinate      = FIELD_METATILE(coordinate);
        probe->pos.z    = coordinate;
    }
    probe->pos.x  = FIELD_METATILE(probe->pos.x);
    *layerOriginX = FIELD_METATILE(grid->layers[1].offsetX);
    *layerOriginZ = FIELD_METATILE(grid->layers[1].offsetY);
    return 1;
}

// Floats one of the floodgate room's logs on the raised water: it bobs under the leader's feet, and
// its metatiles are redrawn as a floating log lying across or along the room as it does.
static Bool scCrossboneIsleSteppingStonesFloatLog(Sint32 actorId)
{
    ACTOR             *actor = fdActorGet(actorId);
    Uint32             width, depth;
    Sint32             layerOriginX, layerOriginZ;
    PUSHED_ACTOR_PROBE probe;
    Sint32             metatileX, metatileZ;

    if (scCrossboneIsleSteppingStonesQueryCoveredMetatiles(
            actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ)
        == 0)
    {
        return FALSE;
    }

    metatileX = probe.pos.x;
    metatileZ = probe.pos.z;
    fdMapGridCopyMetatileAttributes(2, 2, width, depth, metatileX, metatileZ);

    fdActorSelectAnimation(actor, LOG_ANIMATION_FLOAT);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

    if (width > depth)
    {
        fdMapGridCopyMetatilesTo(70, 40, probe.pos.x + 32, probe.pos.z + 2, width, depth);
    }
    else
    {
        fdMapGridCopyMetatilesTo(68, 40, probe.pos.x + 32, probe.pos.z + 2, width, depth);
    }

    return TRUE;
}

// Floats the floodgate room's seven logs on the raised water.
static inline void scCrossboneIsleSteppingStonesFloatLogs(void)
{
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 1);
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 2);
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 3);
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 4);
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 5);
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 6);
    scCrossboneIsleSteppingStonesFloatLog(ACTOR_FIRST_LOG + 7);
}

// Strands one of the floodgate room's logs on the floor as the water drains: the metatiles under it
// are restored from the map and walled off, and it lies still.
static Bool scCrossboneIsleSteppingStonesStrandLog(Sint32 actorId)
{
    ACTOR             *actor = fdActorGet(actorId);
    Uint32             width, depth;
    Sint32             layerOriginX, layerOriginZ;
    PUSHED_ACTOR_PROBE probe;

    if (scCrossboneIsleSteppingStonesQueryCoveredMetatiles(
            actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ)
        == 0)
    {
        return FALSE;
    }

    {
        Sint32 srcMetatileX = layerOriginX + probe.pos.x;
        Sint32 srcMetatileZ = layerOriginZ + probe.pos.z;

        fdMapGridCopyMetatileAttributes(
            srcMetatileX, srcMetatileZ, width, depth, probe.pos.x, probe.pos.z);
        scMapGridFillCollisionRectangle(
            0, probe.pos.x, probe.pos.z, width, depth, METATILE_BEHAVIOR_WALL);
    }

    fdActorSelectAnimation(actor, LOG_ANIMATION_AT_REST);
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;

    return TRUE;
}

// Reads one map metatile through DMA, waiting until the transfer finishes.
static void scCrossboneIsleSteppingStonesReadMapGridMetatile(Sint32                          layer,
                                                             Sint32                          x,
                                                             Sint32                          y,
                                                             CROSSBONE_ISLE_PILLAR_METATILE *dest)
{
    MAP_GRID_SCROLL_STATE *grid = MapGridState[0];

    if (grid != NULL)
    {
        MAP_GRID_METATILE *metatile  = grid->layers[layer].metatiles;

        metatile                    += x + (y << MAPGRID_ROW_SHIFT);
        syDMAXCopy(metatile, dest, DMA_COPY_WORDS(sizeof(*metatile) / sizeof(Uint32)), REG_DMA3);
        syDMAWait(REG_DMA3);
    }
}

// Copies the metatile's flags and metatile behavior without replacing its metatile tiles.
static void scCrossboneIsleSteppingStonesSetMetatileAttributes(
    Sint32 layer, Sint32 x, Sint32 y, const CROSSBONE_ISLE_PILLAR_METATILE *src)
{
    MAP_GRID_SCROLL_STATE *grid = MapGridState[0];

    if (grid != NULL)
    {
        struct CROSSBONE_ISLE_PILLAR_METATILE_BITS *metatile =
            (struct CROSSBONE_ISLE_PILLAR_METATILE_BITS *)grid->layers[layer].metatiles;

        metatile            += x + (y << MAPGRID_ROW_SHIFT);
        metatile->lowFlags   = src->bits.lowFlags;
        metatile->highFlags  = src->bits.highFlags;
        metatile->behavior   = src->bits.behavior;
        metatile->shape      = src->bits.shape;
    }
}

// Lets the actor fall down the cliff south of it to the next ledge, then takes it out of play, to
// the map's origin. Returns whether a ledge was found.
static Sint32 scCrossboneIsleSteppingStonesLowerActorToLedge(Sint32 id, Sint32 far)
{
    ACTOR *actor = fdActorGet(id);
    Sint32 found;
    ACTOR *temporaryActor;
    FIXED  height;
    Sint32 level;
    Sint32 top;
    Uint32 row;
    Uint32 rows;
    Sint32 x;
    Sint32 y;
    FIXED  z;

    found          = 0;
    temporaryActor = NULL;
    fdActorSetSpriteFlags(actor, 0);
    height = fdMapGridGetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
    level  = height / toFIXED(16);
    rows   = level;
    if (level < 0)
    {
        rows = -level;
    }
    rows++;
    for (row = 0; row <= rows; row++)
    {
        top = fdMapGridGetTerrainHeight(
            actor->layer, actor->x.fixed, FIELD_METATILE_ORIGIN(row) + actor->z.fixed);
        top /= toFIXED(16);
        if (level < top)
        {
            x = FIELD_METATILE_CENTER(FIELD_METATILE(actor->x.fixed));
            // A black, see-through crate or capped pillar stands at the ledge until the actor has
            // stopped.
            if (far == 0)
            {
                row += 2;
                z    = FIELD_METATILE_ORIGIN(row + FIELD_METATILE(actor->z.fixed)) + toFIXED(2);
                y    = FIELD_METATILE_ORIGIN(top);
                temporaryActor = scActorPrepareFX(x, y, z, SPRITE_CRATE);
                height         = actor->z.fixed - z + y;
            }
            else
            {
                row += 3;
                z    = FIELD_METATILE_ORIGIN(row + FIELD_METATILE(actor->z.fixed)) - toFIXED(2);
                y    = FIELD_METATILE_ORIGIN(top);
                temporaryActor = scActorPrepareFX(x, y, z, SPRITE_CAPPED_PILLAR);
                height         = actor->z.fixed - z + y;
            }
            found = 1;
            break;
        }
    }
    scCrossboneIsleSteppingStonesStopAtGroundOrHeight(actor, height);
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    if (temporaryActor != NULL)
    {
        fdActorRelease(temporaryActor);
    }
    return found;
}

// Sinks the pillar into the floor, a tenth of its height and 0.8 pixels a tick, until a tenth of it
// is left (at most 32 ticks).
static void scCrossboneIsleSteppingStonesSinkPillar(Sint32 index)
{
    ACTOR *actor = fdActorGet(index);
    Uint32 elapsedTicks;

    actor->motionFlags = 0;

    elapsedTicks       = 0;
    for (;;)
    {
        if (elapsedTicks > 31)
            return;
        sySynch(1);
        actor->scaleY  -= toFIXED(1) / 10;
        actor->y.fixed -= toFIXED(4) / 5;
        elapsedTicks++;
        if (actor->scaleY <= toFIXED(1) / 10 - 1)
        {
            actor->scaleY = toFIXED(1) / 10;
            return;
        }
    }
}

// Unused: spawns a particle near the leader once every four ticks; nothing calls it.
UNUSED static void scCrossboneIsleSteppingStonesSpawnUnusedLeaderParticle(void)
{
    ACTOR         *leader;
    Sint32         cycleFrame;
    FIXED          x;
    FIXED          z;
    Sint32         velocityY;
    EFFECT_OPTIONS options;

    leader     = fdActorGet(ACTOR_PARTY_LEADER);
    cycleFrame = TickCount & 3;
    if (cycleFrame == 0)
    {
        options.palette     = SPRITE_COLOR_RED_TINT;
        options.startScaleX = toFIXED(7) / 10;
        options.startScaleY = toFIXED(7) / 10;
        x                   = leader->x.fixed + toFIXED(syRandomBelow(17) - 8);
        z                   = leader->z.fixed + toFIXED(syRandomBelow(17) - 8);
        velocityY           = syMathDivide(toFIXED(syRandomBelow(5)) + toFIXED(3), 10);
        fdEffectSpawn(x,
                      leader->y.fixed,
                      z,
                      0,
                      velocityY,
                      cycleFrame,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
    }
}

// Jumps two metatiles forward when both the intervening and landing metatiles permit it.
static Bool scCrossboneIsleSteppingStonesTryJumpAcrossPillar(void)
{
    ACTOR *leader;
    Sint32 savedMotionFlags;
    Sint32 jumpDirection;
#if !JUMP_LANDS_ON_CELL_CENTER
    Sint32 step;
#endif
    FIELD_POS dest;

    leader           = fdActorGet(ACTOR_PARTY_LEADER);
    savedMotionFlags = leader->motionFlags;
#if JUMP_LANDS_ON_CELL_CENTER
    jumpDirection = FACING_NEAREST_CARDINAL(leader->facing);
#endif
    if (MainRoutineStateWork.movementMode != LEADER_MOVEMENT_WALK)
    {
        return FALSE;
    }
#if JUMP_LANDS_ON_CELL_CENTER
    dest.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    dest.y = leader->y.fixed;
    dest.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    syVectorAddPolarOffset(toFIXED(16), jumpDirection, (FIXED *)&dest);
#else
    jumpDirection = FACING_INDEX(leader->facing);
    fdActorStepFrom(&dest, leader, PushedActorDirectionSteps[jumpDirection]);
#endif
    if (syFieldActorCheckMovementCollision(leader, &dest) != 1
        && scPushedActorFindAtTile(&dest, leader) == 0)
    {
#if JUMP_LANDS_ON_CELL_CENTER
        dest.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        dest.y = leader->y.fixed;
        dest.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        syVectorAddPolarOffset(toFIXED(32), jumpDirection, (FIXED *)&dest);
#else
        // Two steps on.
        step   = PushedActorDirectionSteps[jumpDirection];
        dest.x = leader->x.fixed + (DIRECTION_STEP_X_FIXED(step) * 2);
        dest.y = leader->y.fixed;
        step   = DIRECTION_STEP_Z_FIXED(step) * 2;
        dest.z = leader->z.fixed + step;
#endif
        if (scPushedActorFindAtTile(&dest, leader) == 0
            && syFieldActorCheckMovementCollision(leader, &dest) == 0)
        {
            fdScriptStart();
            fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
            sySynch(6);
            sySoundPlay(SE_JUMP);
            fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
            leader->speedLimit    = toFIXED(3);
            leader->acceleration  = toFIXED(2);
            leader->velocityY     = toFIXED(4);
            leader->motionFlags  &= ACTOR_JUMP_KEEPS_MOTION;
            fdActorSetSpriteFlags(leader, 0);
            fdActorMoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(dest.x), FIXEDtoINT(dest.z));
            fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
            fdActorSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
            leader->motionFlags = savedMotionFlags;
            fdScriptEnd();
            return TRUE;
        }
    }
    return FALSE;
}

// Moves the particle while damping its velocity, growing it, and advancing rotation.
static void scCrossboneIsleSteppingStonesUpdateDampedParticle(ACTOR *actor)
{
    FIELD_EFFECT *effect = (FIELD_EFFECT *)actor;
    Sint32        velocityX;
    Sint32        velocityY;
    Sint32        velocityZ;

    effect->x                += effect->velocity.x;
    effect->y                += effect->velocity.y;
    effect->z                += effect->velocity.z;
    velocityX                 = effect->velocity.x;
    velocityY                 = effect->velocity.y;
    velocityZ                 = effect->velocity.z;
    effect->velocity.x        = velocityX - syMathDivide(velocityX, PARTICLE_DRAG);
    effect->velocity.y        = velocityY - syMathDivide(velocityY, PARTICLE_DRAG_Y);
    effect->velocity.z        = velocityZ - syMathDivide(velocityZ, PARTICLE_DRAG);
    effect->scaleX           += effect->scaleRateX;
    effect->scaleY           += effect->scaleRateY;
    effect->sprite->rotation += effect->spin;
}

// Sets the actor's sprite to the colour mode (SPR_COL_MODE) in the low four bits of its timer.
static Sint32 scCrossboneIsleSteppingStonesSetSpriteColorFromTimer(ACTOR *actor)
{
    fdActorSetSpriteColor(actor, actor->timer & 15);
    return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_CROSSBONE_ISLE_STEPPING_STONES)
    {
        return CrossboneIsleSteppingStonesEntrances;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_ICE_PILLARS)
    {
        return CrossboneIsleIcePillarsEntrances;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_FLOODGATE)
    {
        return CrossboneIsleFloodgateEntrances;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_STACKED_PILLARS)
    {
        return CrossboneIsleStackedPillarsEntrances;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_DRAGON_STATUE)
    {
        return CrossboneIsleDragonStatueEntrances;
    }
    return SceneFallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return 0;
}

// Returns the exits the five rooms share.
const Uint32 *scGetExits(void)
{
    return CrossboneIsleSteppingStonesExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_CROSSBONE_ISLE_STEPPING_STONES)
    {
        return CrossboneIsleSteppingStonesActorPlacements;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_ICE_PILLARS)
    {
        return CrossboneIsleIcePillarsActorPlacements;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_FLOODGATE)
    {
        return CrossboneIsleFloodgateActorPlacements;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_DRAGON_STATUE)
    {
        return CrossboneIsleDragonStatueActorPlacements;
    }
    return CrossboneIsleSteppingStonesActorPlacementsOther;
}

// Isaac walks into one of the stone pillars on the raised squares of the first floor's side room:
// he jumps across it if there is a square beyond to land on, and otherwise pushes it a step.
static void scCrossboneIsleSteppingStonesJumpOrPushPillar(void)
{
    fdScriptStart();
    if (scCrossboneIsleSteppingStonesTryJumpAcrossPillar() == 0)
        scPushedActorAdvancePair();
    fdScriptEnd();
}

// Isaac walks into one of the lying columns of the second floor's side room, or a log in the fifth
// floor's room of the floodgate, and pushes it if the way beyond is clear.
static void scCrossboneIsleSteppingStonesPushPillar(void)
{
    PUSHED_ACTOR_PROBE out;

    fdScriptStart();
    if (scPushedActorFindClearPos(&out) != 0)
        scPushedActorRoll(out);
    fdScriptEnd();
}

// A pillar slot's place and ground metatile, without its flag. It holds one slot while two are
// swapped.
typedef struct PILLAR_PLACEMENT
{
    Sint32                         x; // 16-pixel map metatiles
    Sint32                         y; // 16-pixel map metatiles
    Sint32                         z; // 16-pixel map metatiles
    CROSSBONE_ISLE_PILLAR_METATILE metatile;
} PILLAR_PLACEMENT;

// Where one of the seventh floor's side room's pillars stands, the ground metatile saved under it
// and its flag of standing under another pillar.
typedef struct PILLAR_SLOT
{
    Sint32                         x; // 16-pixel map metatiles
    Sint32                         y; // 16-pixel map metatiles
    Sint32                         z; // 16-pixel map metatiles
    CROSSBONE_ISLE_PILLAR_METATILE metatile;
    Sint32                         flags;
} PILLAR_SLOT;

// The side room's four pillars.
static PILLAR_SLOT PillarSlots[4] ALIGNED(16);
// Where a moved pillar stands, tested against the map's collision.
static FIELD_POS   ProbePosition;
// BG1 to BG3's scroll moved down to the stream's other level
// (scCrossboneIsleSteppingStonesPrepareWaterScroll).
static BG_SCROLL   OtherWaterScroll[3];
// The percent chance that a line of the top WATER_DISSOLVE_LINES shows the other water level: 0
// when the water starts to change, then a point higher each tick until every line shows it.
static Sint32      OtherWaterChance;

// HBlank handler while the water changes level: the next line, if one of the top
// WATER_DISSOLVE_LINES, shows the other water level with OtherWaterChance, else the backgrounds'
// own scroll.
static void        scCrossboneIsleSteppingStonesJitterBackgroundScroll(void)
{
    Uint32           line = REG_VCOUNT;
    Uint32          *src  = &((Uint32 *)BgScrollWork)[1];
    volatile Uint32 *reg  = (volatile Uint32 *)&REG_BG1HOFS;

    if (line == DISPLAY_TOTAL_LINES - 1 || line <= WATER_DISSOLVE_LINES - 2)
    {
        if (syRandomBelow(100) < OtherWaterChance)
        {
            src = ((Uint32 *)OtherWaterScroll);
        }
    }
    *reg   = *src++;
    reg    = (volatile Uint32 *)&REG_BG2HOFS;
    *reg++ = *src++;
    *reg   = *src;
}

// Copies BG1 to BG3's scroll and moves it down WATER_PANEL_SCROLL_Y, to the stream in its other
// level. Event, each tick while the water changes level.
static void scCrossboneIsleSteppingStonesPrepareWaterScroll(void)
{
    Uint32       *dest;
    const Uint32 *src;
    BG_SCROLL    *other;

    src         = (const Uint32 *)&((Uint32 *)BgScrollWork)[1];
    dest        = (Uint32 *)OtherWaterScroll;
    *dest++     = *src++;
    *dest++     = *src++;
    *dest       = *src;
    other       = OtherWaterScroll;
    other[0].y += WATER_PANEL_SCROLL_Y;
    other[1].y += WATER_PANEL_SCROLL_Y;
    other[2].y += WATER_PANEL_SCROLL_Y;
}

// The floodgate room's water changes level: over 100 ticks, more and more of the top lines show the
// stream at its other level, then the map takes it.
static void scCrossboneIsleSteppingStonesAnimateWaterLevel(void)
{
    Sint32 waterRaised;

    waterRaised = gmFlagGet(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
    if (waterRaised == 0)
    {
        scMapGridDrawMetatilePanel(
            STREAM_COLUMN, STREAM_ROW, 16, 5, waterRaised, STREAM_COLUMN, WATER_PANEL_ROW);
        scMapGridDrawMetatilePanel(
            STREAM_COLUMN, RAISED_WATER_ROW + STREAM_ROW, 16, 5, 1, STREAM_COLUMN, WATER_PANEL_ROW);
        scMapGridDrawMetatilePanel(32 + STREAM_COLUMN,
                                   RAISED_WATER_ROW + STREAM_ROW,
                                   16,
                                   5,
                                   2,
                                   STREAM_COLUMN,
                                   WATER_PANEL_ROW);
    }
    else
    {
        scMapGridDrawMetatilePanel(
            STREAM_COLUMN, STREAM_ROW, 16, 5, 0, STREAM_COLUMN, WATER_PANEL_ROW);
        scMapGridDrawMetatilePanel(STREAM_COLUMN,
                                   DRAINED_WATER_ROW + STREAM_ROW,
                                   16,
                                   5,
                                   1,
                                   STREAM_COLUMN,
                                   WATER_PANEL_ROW);
        scMapGridDrawMetatilePanel(32 + STREAM_COLUMN,
                                   DRAINED_WATER_ROW + STREAM_ROW,
                                   16,
                                   5,
                                   2,
                                   STREAM_COLUMN,
                                   WATER_PANEL_ROW);
    }
    OtherWaterChance = 0;
    sySetTickEvent(scCrossboneIsleSteppingStonesPrepareWaterScroll, EVENT_PASS_TICK);
    sySynch(1);
    syIntFunction(IRQ_HBLANK, 0, scCrossboneIsleSteppingStonesJitterBackgroundScroll);
    sySoundPlay(SE_WATER_LEVEL);
    for (OtherWaterChance = 0; OtherWaterChance <= 100; OtherWaterChance++)
    {
        sySynch(1);
    }
    // The map takes the new level for good.
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    if (gmFlagGet(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) == 0)
    {
        fdMapGridCopyMetatilesTo(0, RAISED_WATER_ROW, FLOODGATE_LAYER_1_COLUMN, 0, 32, 32);
        fdMapGridCopyMetatilesTo(32, RAISED_WATER_ROW, FLOODGATE_LAYER_2_COLUMN, 0, 32, 32);
    }
    else
    {
        fdMapGridCopyMetatilesTo(0, DRAINED_WATER_ROW, FLOODGATE_LAYER_1_COLUMN, 0, 32, 32);
        fdMapGridCopyMetatilesTo(32, DRAINED_WATER_ROW, FLOODGATE_LAYER_2_COLUMN, 0, 32, 32);
    }
    sySynch(1);
    syIntFunction(IRQ_HBLANK, 0, 0);
    sySynch(1);
    syCloseEvent(scCrossboneIsleSteppingStonesPrepareWaterScroll);
    fdMapGridCenterOnFollowedPoint();
    sySynch(30);
}

// Isaac searches the switch beside the floodgate in the fifth floor's side room: "Isaac flipped the
// switch." With the gate open it shuts, spray bursts at its foot, the water rises behind it and the
// logs float up; with it shut, it opens, the water drains and the logs settle on the stream bed.
static void scCrossboneIsleSteppingStonesFlipWaterSwitch(void)
{
    Sint32 waterRaised;

    fdScriptStart();
    fdCameraSetSpeed(1);
    fdCameraMoveTo(281, CAMERA_KEEP, 432, 1);
    fdCameraWaitForMove();
    mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
    waterRaised = fdScriptGetFlag(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
    if (waterRaised == 0)
    {
        sySoundPlay(SE_CRUNCH);
        fdMapGridAnimateMetatiles(CrossboneIsleSteppingStonesSwitchCloseSteps, 84, 24);
        fdScriptWait(30);
        sySoundPlay(SE_LOG_FALL);
        fdActorSetSpritePriority(ACTOR_WATER, 1);
        fdActorGet(ACTOR_WATER)->motionFlags = waterRaised;
        fdActorGet(ACTOR_WATER)->y.fixed     = -toFIXED(32);
        fdActorSetPos(ACTOR_WATER, 272, 416);
        fdActorSetAnimation(ACTOR_WATER, SPLASH_ANIMATION_BURST);
        fdMapGridAnimateMetatiles(CrossboneIsleSteppingStonesFloodgateCloseSteps, 80, 24);
        fdMapGridAnimateMetatiles(CrossboneIsleSteppingStonesFloodgateFootSteps, 80, 28);
        fdMapGridCopyMetatilesTo(65, 40, 16, 27, 2, 4);
        scCrossboneIsleSteppingStonesAnimateWaterLevel();
        scCrossboneIsleSteppingStonesFloatLogs();
        fdMapGridCopyMetatileAttributes(24, 3, 1, 1, 24, 8);
        gmFlagSet(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
    }
    else
    {
        sySoundPlay(SE_CRUNCH);
        fdMapGridAnimateMetatiles(CrossboneIsleSteppingStonesSwitchOpenSteps, 84, 24);
        fdScriptWait(30);
        sySoundPlay(SE_WATER_DRAIN);
        fdActorGet(ACTOR_WATER)->motionFlags = 0;
        fdActorGet(ACTOR_WATER)->y.fixed     = -toFIXED(32);
        fdActorSetPos(ACTOR_WATER, 272, 436);
        fdActorSetAnimation(ACTOR_WATER, SPLASH_ANIMATION_CHURN);
        fdMapGridCopyMetatilesTo(65, 45, 16, 27, 2, 4);
        fdMapGridAnimateMetatiles(CrossboneIsleSteppingStonesFloodgateOpenSteps, 80, 24);
        scCrossboneIsleSteppingStonesAnimateWaterLevel();
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 1);
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 2);
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 3);
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 4);
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 5);
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 6);
        scCrossboneIsleSteppingStonesStrandLog(ACTOR_FIRST_LOG + 7);
        fdMapGridCopyMetatileAttributes(24, 4, 1, 1, 24, 8);
        gmFlagClear(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
    }
    fdScriptEnd();
}

// When actor stands just behind front, up to two metatiles north of it, within a metatile east or
// west and at the same height, it is drawn no nearer the viewer than front: it takes front's sprite
// priority (the background layers it is drawn over) if its own is nearer. Returns whether it stands
// there.
static Sint32 scCrossboneIsleSteppingStonesSyncPriorityIfBehind(ACTOR *actor, ACTOR *front)
{
    Sint32 result = 0;

    if (front->x.fixed == actor->x.fixed && front->y.fixed == actor->y.fixed
        && front->z.fixed == actor->z.fixed)
    {
        return result;
    }
    if (front->x.fixed - toFIXED(16) < actor->x.fixed
        && actor->x.fixed < front->x.fixed + toFIXED(16)
        && front->y.fixed / toFIXED(1) == actor->y.fixed / toFIXED(1)
        && front->z.fixed > actor->z.fixed && front->z.fixed - toFIXED(32) < actor->z.fixed)
    {
        if (actor->sprite->image.priority < front->sprite->image.priority)
        {
            actor->priorityFlags           &= ~ACTOR_PRIORITY_AUTOMATIC;
            actor->sprite->image.priority   = front->sprite->image.priority;
            actor->sprite->shadow.priority  = front->sprite->shadow.priority;
        }
        result = 1;
    }
    return result;
}

// The same test from the front: when actor stands just behind front, it takes front's sprite
// priority if its own is nearer the viewer. Returns whether it stands there.
static Sint32 scCrossboneIsleSteppingStonesSyncPriorityIfAhead(ACTOR *front, ACTOR *actor)
{
    Sint32 result = 0;

    if (actor->x.fixed == front->x.fixed && actor->y.fixed == front->y.fixed
        && actor->z.fixed == front->z.fixed)
    {
        return result;
    }
    if (front->x.fixed - toFIXED(16) < actor->x.fixed
        && actor->x.fixed < front->x.fixed + toFIXED(16)
        && actor->y.fixed / toFIXED(1) == front->y.fixed / toFIXED(1)
        && front->z.fixed > actor->z.fixed && front->z.fixed - toFIXED(32) < actor->z.fixed)
    {
        if (front->sprite->image.priority > actor->sprite->image.priority)
        {
            actor->priorityFlags           &= ~ACTOR_PRIORITY_AUTOMATIC;
            actor->sprite->image.priority   = front->sprite->image.priority;
            actor->sprite->shadow.priority  = front->sprite->shadow.priority;
        }
        result = 1;
    }
    return result;
}

// A stacked pillar's update function, each tick. While Psynergy is cast it is solid and drawn on
// Isaac's layers. Otherwise a pillar just behind Isaac or another pillar is drawn no nearer than
// it, and then the pillars just behind it no nearer than it in turn; a pillar whose top is below
// Isaac's feet is walked over, drawn under him and not solid. A pillar none of this touches goes
// back to the priority its place gives it. XXX: what the player would see without it; a recording
// of Isaac walking round the stacked pillars, with this function and with it returning at once,
// would settle it.
static UNREAD_RESULT scCrossboneIsleSteppingStonesUpdateActorPriority(ACTOR *actor)
{
    ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
    Sint32 hit;
    Uint32 id;

    if (FieldStateWork->castStarted == 1)
    {
        actor->sprite->image.priority  = leader->sprite->image.priority;
        actor->collisionFlags         |= ACTOR_SOLID;
        RETURN_UNREAD(0);
    }
    hit = scCrossboneIsleSteppingStonesSyncPriorityIfBehind(actor, leader);
    for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++)
    {
        hit += scCrossboneIsleSteppingStonesSyncPriorityIfBehind(actor, fdActorGet(id));
    }
    if (hit != 0)
    {
        for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++)
        {
            scCrossboneIsleSteppingStonesSyncPriorityIfAhead(actor, fdActorGet(id));
        }
    }
    if (actor->y.fixed < leader->y.fixed)
    {
        actor->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags &= ~ACTOR_SOLID;
        if (actor->sprite->image.priority < leader->sprite->image.priority)
        {
            actor->priorityFlags           &= ~ACTOR_PRIORITY_AUTOMATIC;
            actor->sprite->image.priority   = leader->sprite->image.priority;
            actor->sprite->shadow.priority  = leader->sprite->shadow.priority;
            hit                             = 1;
        }
    }
    else
    {
        actor->priorityFlags  &= ~ACTOR_PRIORITY_UNDERFOOT;
        actor->collisionFlags |= ACTOR_SOLID;
    }
    if (hit == 0)
    {
        actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    }
    RETURN_UNREAD(0);
}

// Sorts the four pillars by height and depth, carrying their saved metatile state with them.
static void scCrossboneIsleSteppingStonesSortPillarActors(void)
{
    Uint32 i;
    Uint32 j;
    ACTOR *firstActor;
    ACTOR *secondActor;
    ACTOR  savedActor;
#if FIX_PILLAR_SORT_SCRATCH_OVERRUN
    PILLAR_PLACEMENT savedPlacement;
#else
    // UB: the original 12-byte scratch receives a 16-byte DMA transfer.
    Sint32 savedPlacement[3];
#endif

    for (i = 0; i <= 2; i++)
    {
        firstActor = fdActorGet(i + ACTOR_FIRST_PILLAR);
        for (j = i; j <= 3; j++)
        {
            secondActor = fdActorGet(j + ACTOR_FIRST_PILLAR);
            if (firstActor->y.fixed <= secondActor->y.fixed
                && firstActor->z.fixed < secondActor->z.fixed)
            {
                continue;
            }
            syDMAXCopy(secondActor,
                       &savedActor,
                       DMA_COPY_WORDS(sizeof(savedActor) / sizeof(Uint32)),
                       REG_DMA3);
            syDMAWait(REG_DMA3);
            syDMAXCopy(firstActor,
                       secondActor,
                       DMA_COPY_WORDS(sizeof(savedActor) / sizeof(Uint32)),
                       REG_DMA3);
            syDMAWait(REG_DMA3);
            syDMAXCopy(&savedActor,
                       firstActor,
                       DMA_COPY_WORDS(sizeof(savedActor) / sizeof(Uint32)),
                       REG_DMA3);
            syDMAWait(REG_DMA3);
            syDMAXCopy(&PillarSlots[j],
                       &savedPlacement,
                       DMA_COPY_WORDS(sizeof(PILLAR_PLACEMENT) / sizeof(Uint32)),
                       REG_DMA3);
            syDMAWait(REG_DMA3);
            syDMAXCopy(&PillarSlots[i],
                       &PillarSlots[j],
                       DMA_COPY_WORDS(sizeof(PILLAR_PLACEMENT) / sizeof(Uint32)),
                       REG_DMA3);
            syDMAWait(REG_DMA3);
            syDMAXCopy(&savedPlacement,
                       &PillarSlots[i],
                       DMA_COPY_WORDS(sizeof(PILLAR_PLACEMENT) / sizeof(Uint32)),
                       REG_DMA3);
            syDMAWait(REG_DMA3);
            if (gmFlagGet(PillarSlots[i].flags) && !gmFlagGet(PillarSlots[j].flags))
            {
                gmFlagClear(PillarSlots[i].flags);
                gmFlagSet(PillarSlots[j].flags);
            }
            else if (!gmFlagGet(PillarSlots[i].flags) && gmFlagGet(PillarSlots[j].flags))
            {
                gmFlagSet(PillarSlots[i].flags);
                gmFlagClear(PillarSlots[j].flags);
            }
        }
    }
}

// Isaac casts Move or Carry on one of the square pillars of the seventh floor's side room, and
// after each push there: each pillar settles where it now stands, on the floor, on a ledge or on
// another pillar, or drops off a cliff edge and is gone.
//
// The metatiles under each pillar are saved and redrawn as it moves, so the pillar tops can be
// walked and jumped on.
static void scCrossboneIsleSteppingStonesMovePillar(void)
{
    Uint32                         id;
    ACTOR                         *actor;
    Uint32                         i;
    Uint32                         k;
    ACTOR                         *other;
    CROSSBONE_ISLE_PILLAR_METATILE metatile;

    for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++)
    {
        actor        = fdActorGet(id);
        actor->layer = 2;
        i            = id - ACTOR_FIRST_PILLAR;
        if (FIELD_METATILE(actor->x.fixed) == PillarSlots[i].x
            && (FIELD_METATILE(actor->z.fixed)) == PillarSlots[i].z && actor->velocityY == 0)
        {
            continue;
        }
        syDMAXCopy(&actor->x,
                   &ProbePosition,
                   DMA_COPY_WORDS(sizeof(ProbePosition) / sizeof(Uint32)),
                   REG_DMA3);
        syDMAWait(REG_DMA3);
        if (syFieldActorCheckMovementCollision(actor, &ProbePosition) == -1)
        {
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        }
        scCrossboneIsleSteppingStonesSetMetatileAttributes(
            0, PillarSlots[i].x, PillarSlots[i].z, &PillarSlots[i].metatile);
        scCrossboneIsleSteppingStonesSetMetatileAttributes(
            2, PillarSlots[i].x, PillarSlots[i].z, &PillarSlots[i].metatile);
        // Falling pillars either reach a ledge, shrink away, or settle on ground.
        if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN)
        {
            if (fdMapGridGetMetatileBehaviorAt(2, actor->x.fixed, actor->z.fixed)
                == METATILE_BEHAVIOR_STACKED_PILLARS_DROP)
            {
                sySoundPlay(SE_STONE_GRIND);
                actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
                scCrossboneIsleSteppingStonesLowerActorToLedge(id, 1);
                actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
            }
            else if (fdMapGridGetMetatileBehaviorAt(2, actor->x.fixed, actor->z.fixed)
                     == METATILE_BEHAVIOR_STACKED_PILLARS_SINK)
            {
                scCrossboneIsleSteppingStonesStopAtGroundOrHeight(actor, 0);
                sySoundPlay(SE_STONE_GRIND);
                actor->y.fixed        = 0;
                actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
                scCrossboneIsleSteppingStonesSinkPillar(id);
                actor->x.fixed        = 0;
                actor->y.fixed        = 0;
                actor->z.fixed        = 0;
                actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
            }
            else
            {
                scCrossboneIsleSteppingStonesSettleActorOnGround(actor);
            }
            actor->motionFlags = 0;
        }
        scCrossboneIsleSteppingStonesReadMapGridMetatile(0,
                                                         FIELD_METATILE(actor->x.fixed),
                                                         FIELD_METATILE(actor->z.fixed),
                                                         &PillarSlots[i].metatile);
        if (actor->y.fixed >= 0)
        {
            scCrossboneIsleSteppingStonesReadMapGridMetatile(
                0, 27, FIELD_METATILE(actor->y.fixed) + 6, &metatile);
            scCrossboneIsleSteppingStonesSetMetatileAttributes(
                0, FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed), &metatile);
            metatile.bits.highFlags = PillarSlots[i].metatile.bits.highFlags;
            scCrossboneIsleSteppingStonesSetMetatileAttributes(
                2, FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed), &metatile);
        }
        PillarSlots[i].x = FIELD_METATILE(actor->x.fixed);
        PillarSlots[i].y = FIELD_METATILE(actor->y.fixed);
        PillarSlots[i].z = FIELD_METATILE(actor->z.fixed);
        for (k = 0; k <= 3; k++)
        {
            if (k == i)
            {
                continue;
            }
            gmFlagClear(PillarSlots[k].flags);
            other = fdActorGet(k + ACTOR_FIRST_PILLAR);
            if (FIELD_METATILE(actor->x.fixed) == FIELD_METATILE(other->x.fixed)
                && (FIELD_METATILE(actor->z.fixed)) == FIELD_METATILE(other->z.fixed)
                && actor->y.fixed > other->y.fixed)
            {
                gmFlagSet(PillarSlots[k].flags);
            }
        }
    }
    scCrossboneIsleSteppingStonesSortPillarActors();
}

// Isaac walks into a square pillar in the seventh floor's side room: he jumps across it if there is
// somewhere to land, and otherwise pushes it, and the pillars settle.
static void scCrossboneIsleSteppingStonesJumpOrPushStackedPillar(void)
{
    fdScriptStart();
#if LEADER_SNAPPED_TO_GROUND_AT_STACKED_PILLAR
    fdActorGet(ACTOR_PARTY_LEADER)->y.fixed =
        fdMapGridGetTerrainHeight(fdActorGet(ACTOR_PARTY_LEADER)->layer,
                                  fdActorGet(ACTOR_PARTY_LEADER)->x.fixed,
                                  fdActorGet(ACTOR_PARTY_LEADER)->z.fixed);
#endif
    if (scCrossboneIsleSteppingStonesTryJumpAcrossPillar() == 0)
    {
        fdActorGet(ACTOR_PARTY_LEADER)->motionFlags   &= ~ACTOR_FOLLOWS_TERRAIN;
        fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        scPushedActorAdvancePair();
        scCrossboneIsleSteppingStonesMovePillar();
        fdActorGet(ACTOR_PARTY_LEADER)->motionFlags   |= ACTOR_FOLLOWS_TERRAIN;
        fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    }
    fdScriptEnd();
}

// Isaac walks north into a ladder in the seventh floor's side room and climbs up it.
static void scCrossboneIsleSteppingStonesClimbUp(void)
{
    fdClimbUp();
}

// Isaac walks south at the top of a ladder in the seventh floor's side room: he pushes a pillar
// standing below him, jumps across one, or climbs down.
static void scCrossboneIsleSteppingStonesPushOrClimbDown(void)
{
    ACTOR    *leader = fdActorGet(ACTOR_PARTY_LEADER);
    FIELD_POS pos;

    pos.x = leader->x.fixed;
    pos.y = leader->y.fixed;
    pos.z = leader->z.fixed + toFIXED(16);
    if (scPushedActorFindAtTile(&pos, leader) != NULL)
    {
        scCrossboneIsleSteppingStonesJumpOrPushStackedPillar();
    }
    else if (scCrossboneIsleSteppingStonesTryJumpAcrossPillar() == 0)
    {
        fdClimbDown();
    }
}

// Nothing visible happens when Isaac walks onto the lowest floor of the seventh floor's side room,
// among the brown pits, for example metatile (54, 15) in its north east. The game starts the ground
// mask on him and sets his `sceneCount`. From then on, walking on the brown floor along the raised
// pink ground south of it, he shows only from the waist up: the ground mask covers the rest in the
// colour of the raised ground, as if he stood behind it.
static void scCrossboneIsleSteppingStonesStartRising(void)
{
    ACTOR *leader              = fdActorGet(ACTOR_PARTY_LEADER);
    GroundMaskStateWork->actor = leader;
    leader->sceneCount         = 1;
}

// Nothing visible happens when Isaac leaves the lowest floor of the seventh floor's side room at
// metatile (33, 27), below the west ladders, or metatile (54, 14) in the north east. The ground
// mask lets him go and his `sceneCount` is cleared.
static void scCrossboneIsleSteppingStonesStopRising(void)
{
    ACTOR *leader              = fdActorGet(ACTOR_PARTY_LEADER);

    GroundMaskStateWork->actor = NULL;
    leader->sceneCount         = 0;
}

// Puts the frozen puddle under the leader when it is behind and below the leader.
static UNREAD_RESULT scCrossboneIsleSteppingStonesUpdateFrozenPuddlePriority(ACTOR *actor)
{
    ACTOR *reference = fdActorGet(ACTOR_PARTY_LEADER);

    if (actor->z.fixed > reference->z.fixed)
    {
        actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
        if (actor->y.fixed < reference->y.fixed)
            actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }

    RETURN_UNREAD(0);
}

// A puddle in the dragon statue's room has frozen into a pillar of ice: it shines, and its metatile
// becomes a pillar top Isaac can jump onto (CrossboneIsleSteppingStonesIceShineScript).
static void scCrossboneIsleSteppingStonesFreezePuddle(Sint32 id)
{
    ACTOR *actor;

    actor = fdActorGet(id);
    fdScriptStart();
#if FIX_FUNCTION_SIGNATURES
    actor->update = scCrossboneIsleSteppingStonesUpdateFrozenPuddlePriority;
#else
    actor->update = (void (*)(ACTOR *))scCrossboneIsleSteppingStonesUpdateFrozenPuddlePriority;
#endif
    fdMapGridCopyMetatileAttributes(
        20, 14, 1, 1, FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
    gmFlagSet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + id - ACTOR_FIRST_PUDDLE);
    fdActorSetScript(id, CrossboneIsleSteppingStonesIceShineScript);
    fdScriptEnd();
}

// Isaac casts Frost on the puddle by the west ladder of the dragon statue's room, metatile (8, 13):
// it freezes into a pillar of ice.
static void scCrossboneIsleSteppingStonesFreezeFirstPuddle(void)
{
    scCrossboneIsleSteppingStonesFreezePuddle(ACTOR_FIRST_PUDDLE);
}

// Isaac casts Frost on the puddle by the east ladder of the dragon statue's room, metatile (18,
// 13): it freezes into a pillar of ice, until the dragon's fire melts it.
static void scCrossboneIsleSteppingStonesFreezeSecondPuddle(void)
{
    scCrossboneIsleSteppingStonesFreezePuddle(ACTOR_SECOND_PUDDLE);
}

// Isaac starts casting Frost on the puddle under the white platform in front of the stone dragon.
// Nothing visible happens yet; the game notes whether he is standing in the puddle, which then
// stays unfrozen.
static void scCrossboneIsleSteppingStonesFrostStartsOnThirdPuddle(void)
{
    ACTOR              *leader;
    ACTOR              *puddle;
    FIELD_EFFECT_STATE *effectState;

    leader      = fdActorGet(ACTOR_PARTY_LEADER);
    puddle      = fdActorGet(ACTOR_THIRD_PUDDLE);
    effectState = FieldEffectStateWork;
    if (FIELD_METATILE(puddle->x.fixed) == FIELD_METATILE(leader->x.fixed)
        && FIELD_METATILE(puddle->z.fixed) == FIELD_METATILE(leader->z.fixed))
    {
        gmFlagSet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
        effectState->leaveTarget = 1;
    }
    else
    {
        gmFlagClear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
    }
}

// As Frost takes hold of the puddle in front of the stone dragon, a pillar of ice grows in it and
// lifts the white platform two steps up.
static void scCrossboneIsleSteppingStonesFrostRaisesPillar(void)
{
    ACTOR *pillar;

    pillar                                                   = fdActorGet(ACTOR_RISING_PILLAR);
    fdActorGet(ACTOR_THIRD_PUDDLE)->scaleX                   = toFIXED(1);
    fdActorGet(ACTOR_THIRD_PUDDLE)->scaleY                   = toFIXED(1);
    (fdActorGet(ACTOR_THIRD_PUDDLE)->sprite)->image.priority = 2;
    pillar->sprite->image.priority                           = 2;
    pillar->acceleration                                     = toFIXED(2) / 5;
    pillar->speedLimit                                       = toFIXED(4) / 5;
    syFieldActorSetMoveTargetFX(pillar, pillar->x.fixed, toFIXED(32), pillar->z.fixed);
    fdActorWaitForMoveById(ACTOR_RISING_PILLAR);
    fdMapGridCopyMetatileAttributes(20, 14, 1, 1, 22, 16);
}

// The stone dragon's fire, each tick while it breathes: flames burst from beside it towards the
// puddle by the east ladder, with a whoosh.
static void scCrossboneIsleSteppingStonesSpawnDragonFire(void)
{
    EFFECT_OPTIONS options;
    Sint32         velocityX;
    FIXED          velocityY;
    Uint32         angle;

    if ((TickCount & 2) != 0)
        return;
    if ((TickCount & 7) == 0)
        sySoundPlay(SE_WHOOSH);

    options.palette       = SPRITE_COLOR_RED_TINT;
    options.startScaleX   = toFIXED(1) / 2;
    options.startScaleY   = toFIXED(1) / 2;
    options.targetScaleX  = toFIXED(8) / 5;
    options.targetScaleY  = toFIXED(8) / 5;
    // A random angle, of which options.rotation keeps the top four bits: a random sixteenth of a
    // turn.
    angle                 = syRandomNext() & 0x0ffff000;
    options.rotation      = angle;
    options.update        = scCrossboneIsleSteppingStonesUpdateDampedParticle;

    velocityX             = -(toFIXED(syRandomBelow(5)) + toFIXED(6));
    velocityX            /= 2;
    velocityY             = -(toFIXED(syRandomBelow(5)) + toFIXED(5));

    fdEffectSpawn(toFIXED(324),
                  toFIXED(48),
                  toFIXED(228),
                  velocityX,
                  velocityY,
                  0,
                  EFFECT_USE_UPDATE | EFFECT_USE_ROTATION | EFFECT_USE_START_SCALE
                      | EFFECT_SCALE_TO_TARGET | EFFECT_USE_PALETTE,
                  &options);
}

// Steam rises from the melting ice by the east ladder: a white puff every eight ticks.
static UNREAD_RESULT scCrossboneIsleSteppingStonesSpawnMeltParticle(ACTOR *actor)
{
    EFFECT_OPTIONS options;
    Uint32         phase = TickCount & 7;
    FIXED          x;
    FIXED          y;
    FIXED          z;
    Sint32         velocityY;

    if (phase != 0)
        RETURN_UNREAD(0);

    options.palette     = SPRITE_COLOR_WHITE;
    options.startScaleX = toFIXED(7) / 10;
    options.startScaleY = toFIXED(7) / 10;

    x                   = actor->x.fixed + toFIXED(syRandomBelow(17) - 8);
    y                   = actor->y.fixed + toFIXED(syRandomBelow(17));
    z                   = actor->z.fixed + toFIXED(syRandomBelow(17) - 8);
    velocityY           = syMathDivide(toFIXED(syRandomBelow(5)) + toFIXED(3), 10);

    fdEffectSpawn(x,
                  y,
                  z,
                  0,
                  velocityY,
                  phase,
                  EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                  &options);
    RETURN_UNREAD(0);
}

// Isaac's Frost freezes the puddle in front of the stone dragon, unless he stood in it. The view
// moves up to the dragon, which breathes fire at the puddle by the east ladder; if that puddle is
// frozen, its ice melts away in steam, so it can be frozen again.
static void scCrossboneIsleSteppingStonesFreezeThirdPuddle(void)
{
    Sint32 set;

    set = gmFlagGet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
    if (set == 0)
    {
        gmFlagSet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2);
        fdScriptStart();
        fdCameraSetSpeed(0.6);
        fdCameraMoveTo(312, CAMERA_KEEP, 184, 1);
        fdCameraWaitForMove();
        fdScriptWait(20);
        fdMapGridCopyMetatilesTo(73, 10, 60, 10, 1, 2);
        fdScriptWait(20);
        // The third puddle freezes while the frozen second puddle melts.
        sySetTickEvent(scCrossboneIsleSteppingStonesSpawnDragonFire, EVENT_PASS_TICK);
        fdScriptWait(40);
        if (fdScriptGetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1) != 0)
        {
#if FIX_FUNCTION_SIGNATURES
            fdActorGet(ACTOR_SECOND_PUDDLE)->update =
                scCrossboneIsleSteppingStonesSpawnMeltParticle;
#else
            fdActorGet(ACTOR_SECOND_PUDDLE)->update =
                (void (*)(ACTOR *))scCrossboneIsleSteppingStonesSpawnMeltParticle;
#endif
            fdActorSetAnimation(ACTOR_SECOND_PUDDLE, PUDDLE_ANIMATION_MELT);
            fdActorWaitForAnimationChange(ACTOR_SECOND_PUDDLE);
            fdActorGet(ACTOR_SECOND_PUDDLE)->update = (void (*)(ACTOR *))set;
            fdMapGridCopyMetatileAttributes(17, 13, 1, 1, 18, 13);
            gmFlagClear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1);
            fdActorSetSpriteColorById(ACTOR_SECOND_PUDDLE, SPRITE_COLOR_NORMAL);
            fdActorSetMovementTypeById(ACTOR_SECOND_PUDDLE, MOVEMENT_TYPE_STAND);
        }
        else
        {
            fdScriptWait(60);
        }
        syCloseEvent(scCrossboneIsleSteppingStonesSpawnDragonFire);
        fdScriptWait(20);
        fdMapGridCopyMetatilesTo(72, 10, 60, 10, 1, 2);
        fdScriptWait(20);
        fdScriptEnd();
    }
}

// Nothing visible happens as Isaac starts casting Move on the small boulder below the logs in the
// dragon statue's room; the game clears the boulder's metatile so that it can move.
static void scCrossboneIsleSteppingStonesStartMovingSinkingBoulder(void)
{
    ACTOR *boulder = fdActorGet(ACTOR_SINKING_BOULDER);
    Sint32 x;
    Sint32 z;

    fdScriptStart();

    x = FIELD_METATILE(boulder->x.fixed);
    z = FIELD_METATILE(boulder->z.fixed);

    scMapGridFillCollisionRectangle(2, x, z, 1, 1, METATILE_BEHAVIOR_NORMAL);
    fdScriptEnd();
}

// Isaac moves the small boulder below the logs in the dragon statue's room a step, with Move or by
// walking into it. Two steps east of where it starts, in column SINKING_BOULDER_COLUMN, it settles
// into the floor with a thud and stays there for good.
static void scCrossboneIsleSteppingStonesMoveSinkingBoulder(void)
{
    ACTOR *boulder;
    Sint32 alreadyLowered;

    boulder = fdActorGet(ACTOR_SINKING_BOULDER);
    fdScriptStart();
    scMapGridFillCollisionRectangle(2,
                                    FIELD_METATILE(boulder->x.fixed),
                                    (FIELD_METATILE(boulder->z.fixed)),
                                    1,
                                    1,
                                    METATILE_BEHAVIOR_WALL);
    if (FIELD_METATILE(boulder->x.fixed) == SINKING_BOULDER_COLUMN)
    {
        alreadyLowered = fdScriptGetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
        if (alreadyLowered == 0)
        {
            fdScriptWait(10);
            sySoundPlay(SE_DOOR_CLOSE);
            boulder->motionFlags  = alreadyLowered;
            boulder->groundHeight = -toFIXED(2);
            boulder->y.fixed      = -toFIXED(2);
            gmFlagSet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
        }
    }
    fdScriptEnd();
}

// Isaac walks into the small boulder below the logs in the dragon statue's room and pushes it a
// step if the way beyond is clear.
static void scCrossboneIsleSteppingStonesPushSinkingBoulder(void)
{
    PUSHED_ACTOR_PROBE probe;

    fdScriptStart();
    if (scPushedActorFindClearPos(&probe) != 0)
    {
        scPushedActorRoll(probe);
    }
    else
    {
        scCrossboneIsleSteppingStonesStartMovingSinkingBoulder();
        scPushedActorAdvancePair();
        scCrossboneIsleSteppingStonesMoveSinkingBoulder();
    }
    fdScriptEnd();
}

const CUE *scGetCues(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_CROSSBONE_ISLE_STEPPING_STONES)
    {
        return CrossboneIsleSteppingStonesCues;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_ICE_PILLARS)
    {
        return CrossboneIsleIcePillarsCues;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_FLOODGATE)
    {
        return CrossboneIsleFloodgateCues;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_STACKED_PILLARS)
    {
        return CrossboneIsleStackedPillarsCues;
    }
    if (sceneId == SCENE_CROSSBONE_ISLE_DRAGON_STATUE)
    {
        return CrossboneIsleDragonStatueCues;
    }
    return CrossboneIsleSteppingStonesCuesOther;
}

// Lays an actor in the floor below the leader: not solid, drawn under his feet at priority 2, and
// without a shadow.
static void scCrossboneIsleSteppingStonesSetBelowLeader(Sint32 id)
{
    ACTOR *actor           = fdActorGet(id);

    actor->collisionFlags &= ~ACTOR_SOLID;
    actor->priorityFlags  |= ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags     = 0;
    fdActorSetSpriteFlags(actor, 0);
    actor->sprite->image.priority = 2;
}

// A puddle Frost has frozen stands as its pillar of ice again, with its metatile, its priority
// function and its shine.
static void scCrossboneIsleSteppingStonesRestoreFrozenPuddle(Sint32 actorId)
{
    ACTOR *actor;

    actor = fdActorGet(actorId);
    if (gmFlagGet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + actorId
                  - ACTOR_FIRST_PUDDLE)
        != 0)
    {
        fdActorSelectAnimation(actor, PUDDLE_ANIMATION_PILLAR);
#if FIX_FUNCTION_SIGNATURES
        actor->update = scCrossboneIsleSteppingStonesUpdateFrozenPuddlePriority;
#else
        actor->update = (void (*)(ACTOR *))scCrossboneIsleSteppingStonesUpdateFrozenPuddlePriority;
#endif
        fdMapGridCopyMetatileAttributes(
            20, 14, 1, 1, FIELD_METATILE(actor->x.fixed), FIELD_METATILE(actor->z.fixed));
        fdActorSetScript(actorId, CrossboneIsleSteppingStonesIceShineScript);
    }
}

// Uses layer 2 and installs the moving pillar's priority update function.
static void scCrossboneIsleSteppingStonesInitPillarPriority(Sint32 id)
{
    ACTOR *actor       = fdActorGet(id);

    actor->layer       = 2;
    actor->motionFlags = 0;
    actor->update      = (void (*)(ACTOR *))scCrossboneIsleSteppingStonesUpdateActorPriority;
}

// Matches the leader's draw priority to a nearby pillar in front.
static void scCrossboneIsleSteppingStonesUpdateLeaderPriority(void)
{
    ACTOR *hero = fdActorGet(ACTOR_PARTY_LEADER);
    Uint32 selector;
    ACTOR *actor;

    for (selector = ACTOR_FIRST_PILLAR; selector <= ACTOR_FIRST_PILLAR + 3; selector++)
    {
        actor = fdActorGet(selector);
        if (hero->y.fixed / toFIXED(1) == actor->y.fixed / toFIXED(1)
            && hero->z.fixed <= actor->z.fixed - toFIXED(8)
            && hero->z.fixed > actor->z.fixed - toFIXED(24))
        {
            if (hero->x.fixed - toFIXED(16) <= actor->x.fixed
                && actor->x.fixed < hero->x.fixed + toFIXED(16))
            {
                fdActorSetSpritePriority(ACTOR_PARTY_LEADER, actor->sprite->image.priority);
                return;
            }
        }
        else
        {
            fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
        }
    }
}

// Sets up the room: the metatiles under its pillars and logs, and the floodgate's water and the
// puzzles as a battle left them.
Sint32 scInit(void)
{
    ACTOR *actor;
    Uint32 i;
    Uint32 id;
    Sint32 x;
    Sint32 z;

    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_ICE_PILLARS)
    {
        scPushedActorBlockFootprint(ACTOR_FIRST_PILLAR);
        scPushedActorBlockFootprint(ACTOR_FIRST_PILLAR + 1);
        scPushedActorBlockFootprint(ACTOR_FIRST_PILLAR + 2);
        scPushedActorBlockFootprint(ACTOR_FIRST_PILLAR + 3);
        scPushedActorBlockFootprint(ACTOR_FIRST_PILLAR + 4);
    }
    else if (MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_FLOODGATE)
    {
        fdMapGridCopyMetatiles(32, 0, 64, 32, 0, 64);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 1);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 2);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 3);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 4);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 5);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 6);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 7);
        if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0
            && gmFlagGet(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) != 0)
        {
            fdMapGridCopyMetatilesTo(79, 34, 84, 24, 1, 2);
            fdMapGridCopyMetatilesTo(0, RAISED_WATER_ROW, FLOODGATE_LAYER_1_COLUMN, 0, 32, 32);
            fdMapGridCopyMetatilesTo(32, RAISED_WATER_ROW, FLOODGATE_LAYER_2_COLUMN, 0, 32, 32);
            scCrossboneIsleSteppingStonesFloatLogs();
            fdMapGridCopyMetatileAttributes(24, 3, 1, 1, 24, 8);
        }
    }
    else if (MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_STACKED_PILLARS)
    {
        fdActorCreateConfigured(584, 0, 200, SPRITE_CRATE);
        if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        {
            fdActorGet(ACTOR_PARTY_LEADER)->sceneCount = 1;
        }
        fdEffectStartGroundMask(0);
        if (fdActorGet(ACTOR_PARTY_LEADER)->sceneCount == 0)
        {
            scCrossboneIsleSteppingStonesStopRising();
        }
        scCrossboneIsleSteppingStonesInitPillarPriority(ACTOR_FIRST_PILLAR);
        scCrossboneIsleSteppingStonesInitPillarPriority(ACTOR_FIRST_PILLAR + 1);
        scCrossboneIsleSteppingStonesInitPillarPriority(ACTOR_FIRST_PILLAR + 2);
        scCrossboneIsleSteppingStonesInitPillarPriority(ACTOR_FIRST_PILLAR + 3);
        for (i = 0; i < 4; i++)
        {
            PillarSlots[i].x = 0;
            PillarSlots[i].y = 0;
            PillarSlots[i].z = 0;
            PillarSlots[i].flags =
                i + FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER;
        }
        scCrossboneIsleSteppingStonesMovePillar();
        fdScriptWait(1);
        sySetTickEvent(scCrossboneIsleSteppingStonesUpdateLeaderPriority, EVENT_PASS_TICK);
        if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
        {
            for (id = ACTOR_FIRST_PILLAR; id < ACTOR_FIRST_PILLAR + 4; id++)
            {
                actor = fdActorGet(id);
                x     = FIELD_METATILE(actor->x.fixed);
                if (x == 37)
                {
                    z = FIELD_METATILE(actor->z.fixed);
                    if (z == 9)
                    {
                        fdMapGridCopyMetatileAttributes(27, 8, 1, 1, x, z);
                        break;
                    }
                }
            }
        }
    }
    else if (MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_DRAGON_STATUE)
    {
        fdActorSetAnimation(ACTOR_SINKING_BOULDER, TURNING_ROCK_ANIMATION_BOULDER);
        fdActorSetSpriteColorById(ACTOR_SINKING_BOULDER, SPRITE_COLOR_GREY);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG);
        scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 1);
#if DRAGON_STATUE_LOGS_STAY_PUT
        fdActorGet(ACTOR_FIRST_LOG)->motionFlags     = 0;
        fdActorGet(ACTOR_FIRST_LOG + 1)->motionFlags = 0;
#endif
        scCrossboneIsleSteppingStonesMoveSinkingBoulder();
        scCrossboneIsleSteppingStonesSetBelowLeader(ACTOR_FIRST_PUDDLE);
        scCrossboneIsleSteppingStonesSetBelowLeader(ACTOR_SECOND_PUDDLE);
        scCrossboneIsleSteppingStonesSetBelowLeader(ACTOR_THIRD_PUDDLE);
        scCrossboneIsleSteppingStonesRestoreFrozenPuddle(ACTOR_FIRST_PUDDLE);
        scCrossboneIsleSteppingStonesRestoreFrozenPuddle(ACTOR_SECOND_PUDDLE);
        scCrossboneIsleSteppingStonesRestoreFrozenPuddle(ACTOR_THIRD_PUDDLE);
        fdActorGet(ACTOR_THIRD_PUDDLE)->update = 0;
        scCrossboneIsleSteppingStonesSetBelowLeader(ACTOR_RISING_PILLAR);
        fdActorGet(ACTOR_RISING_PILLAR)->collisionFlags |= ACTOR_NOT_TARGETED;
        if (gmFlagGet(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2) == 0)
        {
            fdActorGet(ACTOR_THIRD_PUDDLE)->scaleX                  = toFIXED(3) / 2;
            fdActorGet(ACTOR_THIRD_PUDDLE)->scaleY                  = toFIXED(3) / 2;
            fdActorGet(ACTOR_THIRD_PUDDLE)->sprite->image.priority  = 3;
            fdActorGet(ACTOR_RISING_PILLAR)->sprite->image.priority = 3;
            fdMapGridCopyMetatileAttributes(26, 12, 1, 1, 22, 16);
        }
    }
    return 0;
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

// The shine on a pillar of ice Frost has made of a puddle in the dragon statue's room.
//
// The timer carries a colour mode to scCrossboneIsleSteppingStonesSetSpriteColorFromTimer, with green and blue swapped for two ticks in every eight.
Sint32 CrossboneIsleSteppingStonesIceShineScript[] = {
    SC_SET(SW_TIMER, SPRITE_COLOR_RBG),
    SC_CALL(scCrossboneIsleSteppingStonesSetSpriteColorFromTimer),
    SC_WAIT(2),
    SC_SET(SW_TIMER, SPRITE_COLOR_NORMAL),
    SC_CALL(scCrossboneIsleSteppingStonesSetSpriteColorFromTimer),
    SC_WAIT(6),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The floodgate's switch flipping as scCrossboneIsleSteppingStonesFlipWaterSwitch closes the gate, and flipping back as it opens it.
Uint16 CrossboneIsleSteppingStonesSwitchCloseSteps[] = {
    MAPGRID_ANIMATION_STEP(76, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(77, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(78, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(79, 34, 1, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 CrossboneIsleSteppingStonesSwitchOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(79, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(78, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(77, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(76, 34, 1, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

// The floodgate closing, and opening again.
Uint16 CrossboneIsleSteppingStonesFloodgateCloseSteps[] = {
    MAPGRID_ANIMATION_STEP(67, 34, 2, 5, 6), MAPGRID_ANIMATION_STEP(69, 34, 2, 5, 6), MAPGRID_ANIMATION_STEP(71, 34, 2, 5, 6), MAPGRID_ANIMATION_STEP(73, 34, 2, 5, 6), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 CrossboneIsleSteppingStonesFloodgateOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(71, 50, 2, 5, 6), MAPGRID_ANIMATION_STEP(69, 50, 2, 5, 6), MAPGRID_ANIMATION_STEP(67, 50, 2, 5, 6), MAPGRID_ANIMATION_STEP(65, 50, 2, 5, 6), MAPGRID_ANIMATION_STEPS_END,
};

// The two metatiles at the floodgate's foot as it shuts.
//
// They show three pictures of white spray splashing up and falling, then the stream bed below the gate, left almost dry (scCrossboneIsleSteppingStonesFlipWaterSwitch's render).
Uint16 CrossboneIsleSteppingStonesFloodgateFootSteps[] = {
    MAPGRID_ANIMATION_STEP(75, 38, 2, 1, 4), MAPGRID_ANIMATION_STEP(77, 38, 2, 1, 6), MAPGRID_ANIMATION_STEP(79, 38, 2, 1, 8), MAPGRID_ANIMATION_STEP(65, 53, 2, 1, 1), MAPGRID_ANIMATION_STEPS_END,
};

#include "shared/fallback_entrances.c"

SCENE_ENTRANCE CrossboneIsleSteppingStonesEntrances[] = {
    { CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsleIcePillarsEntrances[] = {
    { CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsleFloodgateEntrances[] = {
    { CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsleStackedPillarsEntrances[] = {
    { CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsleDragonStatueEntrances[] = {
    { CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { SCENE_TABLE_END },
};

Uint32 CrossboneIsleSteppingStonesExits[] = {
    SCENE_EXITS(CrossboneIsleSteppingStones),
    SCENE_EXIT(CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST),
    SCENE_EXITS(CrossboneIsleIcePillars),
    SCENE_EXIT(CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_ICE_PILLARS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_ICE_PILLARS_WEST),
    SCENE_EXITS(CrossboneIsleFloodgate),
    SCENE_EXIT(CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FLOODGATE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FLOODGATE_WEST),
    SCENE_EXITS(CrossboneIsleStackedPillars),
    SCENE_EXIT(CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_STACKED_PILLARS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_STACKED_PILLARS_WEST),
    SCENE_EXITS(CrossboneIsleDragonStatue),
    SCENE_EXIT(CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_DRAGON_STATUE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_DRAGON_STATUE_WEST),
    SCENE_EXITS_END,
};

// Unused: an empty actor placement table nothing returns.
ACTOR_PLACE CrossboneIsleSteppingStonesNoActorPlacements[] = {
    { SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleSteppingStonesActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        744, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        696, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        776, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleIcePillarsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        144, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        256, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        224, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        184, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleFloodgateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        144, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        208, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        272, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        352, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleSteppingStonesActorPlacementsOther[] = {
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        680, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleDragonStatueActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        208, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        224, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        296, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

CUE CrossboneIsleSteppingStonesCuesOther[] = {
    { CUE_TABLE_END },
};

CUE CrossboneIsleSteppingStonesCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST },
    { CUE_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST },
    { CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesJumpOrPushPillar) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_ANTIDOTE, FLAG_CROSSBONE_ISLE_STEPPING_STONES_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_NUT, FLAG_CROSSBONE_ISLE_STEPPING_STONES_NUT, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_111_COINS, FLAG_CROSSBONE_ISLE_STEPPING_STONES_111_COINS, FIND_COINS(111) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_HARD_NUT, FLAG_CROSSBONE_ISLE_STEPPING_STONES_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { CUE_TABLE_END },
};

CUE CrossboneIsleIcePillarsCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST },
    { CUE_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST },
    { CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesPushPillar) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_LUCKY_PEPPER, FLAG_CROSSBONE_ISLE_ICE_PILLARS_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_MYSTERY_BLADE, FLAG_CROSSBONE_ISLE_ICE_PILLARS_MYSTERY_BLADE, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_222_COINS, FLAG_CROSSBONE_ISLE_ICE_PILLARS_222_COINS, FIND_COINS(222) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_ELIXIR, FLAG_CROSSBONE_ISLE_ICE_PILLARS_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { CUE_TABLE_END },
};

CUE CrossboneIsleFloodgateCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST },
    { CUE_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST },
    { CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesPushPillar) },
    { EXAMINE_CHEST_FACING(FACING_EAST), TREASURE_SPOT_FLOODGATE_555_COINS, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), TREASURE_SPOT_FLOODGATE_555_COINS, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { EXAMINE_CHEST_FACING(FACING_EAST), TREASURE_SPOT_FLOODGATE_LUCKY_MEDAL, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CHEST_FACING(FACING_NORTHEAST), TREASURE_SPOT_FLOODGATE_LUCKY_MEDAL, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_FLOODGATE_POTION, FLAG_CROSSBONE_ISLE_FLOODGATE_POTION, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_FLOODGATE_NINJA_GARB, FLAG_CROSSBONE_ISLE_FLOODGATE_NINJA_GARB, FIND_ITEM(ITEM_NINJA_GARB) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FLOODGATE_SWITCH, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesFlipWaterSwitch) },
    { CUE_TABLE_END },
};

CUE CrossboneIsleStackedPillarsCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST },
    { CUE_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST },
    { CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_STACKED_PILLARS_PILLARS, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesJumpOrPushStackedPillar) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_STACKED_PILLARS_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesClimbUp) },
    { STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_STACKED_PILLARS_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesPushOrClimbDown) },
    { CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_STACKED_PILLARS_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesJumpOrPushStackedPillar) },
    { CUE_STEP, METATILE_BEHAVIOR_STACKED_PILLARS_LOWEST_FLOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesStartRising) },
    { CUE_STEP, METATILE_BEHAVIOR_STACKED_PILLARS_LEAVE_LOWEST_FLOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleSteppingStonesStopRising) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STACKED_PILLARS_777_COINS, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_777_COINS, FIND_COINS(777) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STACKED_PILLARS_LUCKY_MEDAL, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CHEST_FACING(FACING_EAST), TREASURE_SPOT_STACKED_PILLARS_PSY_CRYSTAL, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STACKED_PILLARS_WICKED_MACE, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_WICKED_MACE, FIND_ITEM(ITEM_WICKED_MACE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, SCRIPT_REF(scCrossboneIsleSteppingStonesMovePillar) },
    { CUE_TABLE_END },
};
// clang-format on

CUE CrossboneIsleDragonStatueCues[] = {
    {CUE_EXIT,
     1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST},
    {CUE_EXIT,
     2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST},
    {CUE_STEP | STEP_CUE_FLAG_WALKING,
     METATILE_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS,
     SCRIPT_REF(scCrossboneIsleSteppingStonesPushSinkingBoulder)},
    {EXAMINE_CUE(EXAMINE_CHEST),
     TREASURE_SPOT_DRAGON_STATUE_888_COINS, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_888_COINS,
     FIND_COINS(888)},
    {EXAMINE_CUE(EXAMINE_CHEST),
     TREASURE_SPOT_DRAGON_STATUE_POTION, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_POTION,
     FIND_ITEM(ITEM_POTION)},
    {EXAMINE_CUE(EXAMINE_CHEST),
     TREASURE_SPOT_DRAGON_STATUE_CLERICS_RING, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_CLERICS_RING,
     FIND_ITEM(ITEM_CLERICS_RING)},
    {EXAMINE_CUE(EXAMINE_CHEST),
     TREASURE_SPOT_DRAGON_STATUE_SMOKE_BOMB, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_SMOKE_BOMB,
     FIND_ITEM(ITEM_SMOKE_BOMB)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SINKING_BOULDER, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     SCRIPT_REF(scCrossboneIsleSteppingStonesStartMovingSinkingBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_SINKING_BOULDER, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     SCRIPT_REF(scCrossboneIsleSteppingStonesMoveSinkingBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_FIRST_PUDDLE, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN,
     SCRIPT_REF(scCrossboneIsleSteppingStonesFreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_SECOND_PUDDLE, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1,
     SCRIPT_REF(scCrossboneIsleSteppingStonesFreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_THIRD_PUDDLE, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     SCRIPT_REF(scCrossboneIsleSteppingStonesFrostStartsOnThirdPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS),
     ACTOR_THIRD_PUDDLE, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     SCRIPT_REF(scCrossboneIsleSteppingStonesFrostRaisesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_THIRD_PUDDLE, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     SCRIPT_REF(scCrossboneIsleSteppingStonesFreezeThirdPuddle)},
    {CUE_TABLE_END},
};
