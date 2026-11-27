// Helpers the field Psynergy kinds share: map metatiles, spawned actors and particles, and the
// field's tint. Part of script_commands.c.

#include "field/field_effect.h"

// If the scene's sprite is active, stops its update callback (fdPsynergyDrawCanvasSprite) and frees
// its VRAM cache entry.
void fdPsynergyReleaseSceneSprite(void)
{
    FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffectState;

    if (effectState->spriteActive != 0)
    {
        effectState->spriteActive = 0;
        syCloseEvent(fdPsynergyDrawCanvasSprite);
        syVRAMCacheFreeEntry(effectState->spriteVramSlot);
    }
}

// Returns the map metatile at position x, z in the given layer, read from the world map's blocks or
// the metatile buffer by the script state's mode.
MAP_METATILE *fdMapGridGetMetatileAt(Sint32 x, Sint32 z, Sint32 layer)
{
    FIELD_STATE  *event = RamHeapSlots->fieldState;
    MAP_STATE    *map   = RamHeapSlots->mapGrid;
    MAP_METATILE *metatiles;

    if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        metatiles = &((MAP_METATILE *)syRamMapBlocks)[((x / toFIXED(WORLD_MAP_METATILE_PIXELS))
                                                       & (WORLD_MAP_VIEW_METATILES - 1))
                                                      + ((z / toFIXED(WORLD_MAP_METATILE_PIXELS))
                                                         & (WORLD_MAP_VIEW_METATILES - 1))
                                                            * WORLD_MAP_VIEW_METATILES];
    }
    else
    {
        if (layer <= MAPGRID_LAYER_OVERHEAD)
            metatiles = map->layers[layer].metatiles;
        else
            metatiles = (MAP_METATILE *)RamMapMetatileBuffer;
        metatiles = &metatiles[x / toFIXED(MAPGRID_METATILE_PIXELS)
                               + (z / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE];
    }
    return metatiles;
}

// Whether the Scoop can dig at (x, z): on the world map, sand; elsewhere not on a Cyclone metatile,
// water or raised ground, and then on sand, or on a MAP_METATILE_SCOOP metatile (any other, where
// FIELD_STATE.sceneAttributeLow is set).
Sint32 fdMapGridCanScoop(Sint32 x, Sint32 z, Sint32 layer)
{
    FIELD_STATE *event = RamHeapSlots->fieldState;

    if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
        return fdMapGridIsSandMetatile(x, z, layer);
    if (fdMapGridGetCycloneMetatileKind(x, z, layer) != CYCLONE_METATILE_NONE
        || fdMapGridIsWaterMetatile(x, z, layer) != 0
        || fdMapGridGetTerrainKindAtStub(layer, x, z) != 0)
        return 0;
    if (fdMapGridIsSandMetatile(x, z, layer) != 0)
        return 1;
    if (event->sceneAttributeLow == 0)
        return fdMapGridGetMetatileAt(x, z, layer)->attributeB & MAP_METATILE_SCOOP;
    return ~fdMapGridGetMetatileAt(x, z, layer)->attributeB & MAP_METATILE_SCOOP;
}

// Whether Cloak can hide the leader on the metatile at (x, z): the overhead layer's collision code
// is METATILE_BEHAVIOR_CLOAK, whatever layer is passed.
Sint32 fdMapGridIsCloakMetatile(Sint32 x, Sint32 z, Sint32 layer)
{
    return fdMapGridGetMetatileAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode
        == METATILE_BEHAVIOR_CLOAK;
}

// Whether Hover holds up over the metatile at (x, z): the overhead layer's collision code is
// METATILE_BEHAVIOR_HOVER, whatever layer is passed.
Sint32 fdMapGridIsHoverMetatile(Sint32 x, Sint32 z, Sint32 layer)
{
    return fdMapGridGetMetatileAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode
        == METATILE_BEHAVIOR_HOVER;
}

// Whether the leader can move as sand on the metatile at (x, z): the overhead layer's
// METATILE_BEHAVIOR_SAND, or on the world map collision WORLD_MAP_COLLISION_WALKABLE_FIRST or
// shallow water; FIELD_STATE.sceneAttributeHigh turns the answer round.
Sint32 fdMapGridIsSandMetatile(Sint32 x, Sint32 z, Sint32 layer)
{
    FIELD_STATE *event  = RamHeapSlots->fieldState;
    Sint32       column = x;
    if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        Sint32 pos[3];

        SET_VECTOR(pos, column, 0, z);
        column = (Uint32)(fdWorldMapGetCollisionStub(pos) - WORLD_MAP_COLLISION_WALKABLE_FIRST)
              <= WORLD_MAP_COLLISION_SHALLOW_WATER - WORLD_MAP_COLLISION_WALKABLE_FIRST;
    }
    else
    {
        Sint32 code = fdMapGridGetMetatileAt(column, z, MAPGRID_LAYER_OVERHEAD)->collisionCode;

        column      = code == METATILE_BEHAVIOR_SAND;
    }
    if (event->sceneAttributeHigh != 0)
        column = column == 0;
    return column;
}

// Whether the metatile at (x, z) can be climbed (MAP_METATILE_CLIMBABLE): Sand starts up a wall
// face only there (fdActorMoveAsSand) and ends its climb off one.
Uint8 fdMapGridIsClimbableMetatile(Sint32 x, Sint32 z, Sint32 layer)
{
    return fdMapGridGetMetatileAt(x, z, layer)->attributeB & MAP_METATILE_CLIMBABLE;
}

// Whether the metatile at (x, z) is water: MAP_METATILE_WATER, or on the world map collision
// WORLD_MAP_COLLISION_WATER_FIRST to WORLD_MAP_COLLISION_SEA_LAST.
Sint32 fdMapGridIsWaterMetatile(Sint32 x, Sint32 z, Sint32 layer)
{
    FIELD_STATE *event = RamHeapSlots->fieldState;

    if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    {
        Sint32 pos[3];
        Sint32 collision;

        pos[1]    = 0;
        pos[0]    = x;
        pos[2]    = z;
        collision = fdWorldMapGetCollisionStub(pos);
        return (Uint32)(collision - WORLD_MAP_COLLISION_WATER_FIRST)
            <= WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_WATER_FIRST;
    }
    return (Uint8)(fdMapGridGetMetatileAt(x, z, layer)->attributeB & MAP_METATILE_WATER);
}

// The Cyclone's kind of the metatile at (x, z), from the overhead layer's collision code
// (CYCLONE_METATILE_KIND). fdPsynergyGatherNearbyMetatiles lists them.
Sint32 fdMapGridGetCycloneMetatileKind(Sint32 x, Sint32 z, Sint32 layer)
{
    Uint8 code = fdMapGridGetMetatileAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode;

    if (code == METATILE_BEHAVIOR_CYCLONE)
        return CYCLONE_METATILE_BURST;
    if (code == METATILE_BEHAVIOR_CYCLONE_CARRY)
        return CYCLONE_METATILE_CARRY;
    if (code == METATILE_BEHAVIOR_CYCLONE_LIFT)
        return CYCLONE_METATILE_LIFT;
    return CYCLONE_METATILE_NONE;
}

// Returns the behaviour (collision code) of the map metatile at x, z in layer.
Uint8 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer)
{
    return fdMapGridGetMetatileAt(x, z, layer)->collisionCode;
}

// Sets the metatile behavior of the map metatile at x, z in layer, and returns it.
Uint8 fdMapGridSetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer, Sint32 value)
{
    fdMapGridGetMetatileAt(x, z, layer)->collisionCode = value;
    return value;
}

// Returns the attribute byte of the map metatile at x, z in layer.
Sint32 fdMapGridGetMetatileAttributesAt(Sint32 x, Sint32 z, Sint32 layer)
{
    return fdMapGridGetMetatileAt(x, z, layer)->attributeB;
}

// Sets the attribute byte of the map metatile at x, z in layer.
Uint8 fdMapGridSetMetatileAttributesAt(Sint32 x, Sint32 z, Sint32 layer, Sint32 value)
{
    fdMapGridGetMetatileAt(x, z, layer)->attributeB = value;
    return value;
}

// Sets bits in the attribute byte of the map metatile at x, z in layer.
Uint8 fdMapGridSetMetatileAttributeBitsAt(Sint32 x, Sint32 z, Sint32 layer, Sint32 bits)
{
    Sint32        flags     = fdMapGridGetMetatileAttributesAt(x, z, layer);
    MAP_METATILE *metatile  = fdMapGridGetMetatileAt(x, z, layer);

    flags                  |= bits;
    metatile->attributeB    = flags;
    return flags;
}

// Clears bits in the attribute byte of the map metatile at x, z in layer.
Uint8 fdMapGridClearAttributeBBitsAt(Sint32 x, Sint32 z, Sint32 layer, Sint32 bits)
{
    Sint32        flags     = fdMapGridGetMetatileAttributesAt(x, z, layer);
    MAP_METATILE *metatile  = fdMapGridGetMetatileAt(x, z, layer);

    flags                  &= ~bits;
    metatile->attributeB    = flags;
    return flags;
}

// Returns 1 when two upright cylinders overlap, their centres closer on the ground than the sum of
// radiusA and radiusB; else -1.
Sint32 fdPsynergyCheckOverlap(Sint32 *a, Sint32 radiusA, Sint32 *b, Sint32 radiusB)
{
    Sint32 dx     = FIXEDtoINT(*a++ - *b++);
    Sint32 dy     = FIXEDtoINT(*a++ - *b++);
    Sint32 dz     = FIXEDtoINT(*a - *b);
    Sint32 radius = radiusA + radiusB;

    // The box and height bounds are 16.16 against whole-pixel differences, so on a map they never
    // reject a pair and the ground circle alone decides.
    if (dx > toFIXED(64) || dz > toFIXED(64))
        return -1;
    if (dy > toFIXED(radius))
        return -1;
    if (dx * dx + dz * dz < radius * radius)
        return 1;
    return -1;
}

// Moves the actor from 16 pixels above the caster to (x, y, z), growing it from a quarter to full
// size.
void fdPsynergyGrowFromCaster(ACTOR *actor, Sint32 x, Sint32 y, Sint32 z)
{
    FIELD_EFFECT_STATE *scene     = RamHeapSlots->fieldEffectState;
    ACTOR              *mainActor = scene->caster;
    FIXED               start[3];
    Sint32              i = 0;

    if (scene->angle != TRIG_TURN * 3 / 4)
        fdActorSetSpritePriorityDirectStub(actor, 0);
    start[0] = mainActor->x.fixed;
    start[1] = mainActor->y.fixed + toFIXED(16);
    start[2] = mainActor->z.fixed;
    for (; i <= GROW_STEPS; i++)
    {
        SET_ACTOR_POSITION(actor,
                           start[0] + (x - start[0]) * i / GROW_STEPS,
                           start[1] + (y - start[1]) * i / GROW_STEPS,
                           start[2] + (z - start[2]) * i / GROW_STEPS);
        actor->scaleY = actor->scaleX = i * (toFIXED(3) / 4) / GROW_STEPS + toFIXED(1) / 4;
        sySynch(1);
    }
}

// fdPsynergyGrowFromCaster backwards: moves the actor from (x, y, z) to 16 pixels above the caster,
// shrinking it from full size to a quarter.
void fdPsynergyShrinkToCaster(ACTOR *actor, Sint32 x, Sint32 y, Sint32 z)
{
    ACTOR *mainActor = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->caster;
    FIXED  start[3];
    Sint32 i;

    i        = 0;
    start[0] = mainActor->x.fixed;
    start[1] = mainActor->y.fixed + toFIXED(16);
    start[2] = mainActor->z.fixed;
    for (; i <= GROW_STEPS; i++)
    {
        SET_ACTOR_POSITION(actor,
                           x + (start[0] - x) * i / GROW_STEPS,
                           y + (start[1] - y) * i / GROW_STEPS,
                           z + (start[2] - z) * i / GROW_STEPS);
        actor->scaleY = actor->scaleX = i * -(toFIXED(3) / 4) / GROW_STEPS + toFIXED(1);
        sySynch(1);
    }
}

// Spawns a sparkle at x, y, z that hops off in a random direction 8 to 32 pixels away at 1 to 2
// pixels a tick.
ACTOR *fdBattleEffectSpawnScatterParticle(Sint32 x, Sint32 y, Sint32 z)
{
    ACTOR *actor = fdActorSpawn(SPRITE_SPARKLE, x, y, z);
    Sint32 magnitude;

    if (actor != NULL)
    {
        fdActorStartScriptStub(actor, FieldEffectFragmentScript);
        actor->speedLimit    = syRandom() + toFIXED(1);
        actor->acceleration  = toFIXED(1);
        actor->motionFlags   = ACTOR_FALLS;
        actor->gravity       = toFIXED(1) / 25;
        magnitude            = syRandom();
        magnitude           -= syRandom();
        actor->velocityY     = magnitude;
        fdActorSetTargetPosFromMagnitudeAngle(actor, syRandom() * 24 + toFIXED(8), syRandom());
    }
    return actor;
}

// Counts the free blocks of the sprite VRAM cache.
Sint32 fdResourceTableCountFreeBlocks(void)
{
    Uint8 *marker = VramCacheBlockOwners;
    Sint32 freeCount;
    Sint32 remaining;

    freeCount = 0;
    remaining = VRAM_BLOCK_COUNT;
    do
    {
        if (*marker++ == VRAM_BLOCK_OWNER_FREE)
        {
            freeCount++;
        }
        remaining--;
    } while (remaining != 0);
    return freeCount;
}

// Makes srcSprite share sharedSprite's VRAM block, releasing its own; with no sharedSprite it keeps
// its block.
void *fdSpriteShareVRAMCacheEntry(void *srcSprite, void *sharedSprite)
{
    SPR *shared;
    SPR *sprite;

    sprite = srcSprite;
    shared = NULL;
    if (sprite != NULL)
    {
        if (sharedSprite == NULL)
        {
            sprite->retainCacheEntry = 1;
        }
        else
        {
            syVRAMCacheFreeEntry(sprite->slot);
            sprite->slot             = ((SPR *)sharedSprite)->slot;
            sprite->retainCacheEntry = 1;
            sprite                   = sharedSprite;
        }
        shared = sprite;
    }
    return shared;
}

#define fdActorSetAnimationSpeedStub ((void (*)(ACTOR *, Sint32))fdActorSetAnimationSpeedStub)

// Creates an actor for a field Psynergy, at the caster's ground height, with sprite priority 0 and
// animation 1.
ACTOR *fdActorSpawn(Sint32 kind, Sint32 x, Sint32 y, Sint32 z)
{
    FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
    ACTOR              *actor;

    actor = fdActorCreateStub(kind, x, y, z);
    if (actor != NULL)
    {
        if (actor->spriteStorageKind == ACTOR_SPRITE_NONE)
        {
            fdActorReleaseStub(actor);
            return NULL;
        }
        actor->groundHeight                    = scene->caster->groundHeight;
        actor->motionFlags                     = ACTOR_FLOATS;
        actor->priorityFlags                   = ACTOR_PRIORITY_OVERHEAD;
        ((SPR *)actor->sprite)->image.priority = 0;
        fdActorSetSpriteFlagsStub(actor, 0);
        fdActorSelectAnimationStub(actor, 1);
    }
    return actor;
}

// Pauses or resumes every actor but the Halted one and excluded, and sets their animation speed, as
// Sand stills the field around the leader.
void fdActorSetOthersPaused(ACTOR *excluded, Sint32 paused, Sint32 animationSpeed)
{
    Sint16 *haltedId;
    ACTOR  *actor;
    Sint32  actorId;

    actorId  = 0;
    haltedId = &MainRoutineStateWork.haltedActor;
    do
    {
        actor = fdActorLookup(actorId);
        if (actorId != *haltedId && actor != NULL && actor != excluded)
        {
            actor->paused = paused;
            fdActorSetAnimationSpeedStub(actor, animationSpeed);
        }
        actorId++;
        // BUG: one past the actor table: fdActorLookup(FIELD_ACTOR_COUNT) reads the step cue and
        // push metatile requests as an actor.
#if FIX_ACTOR_LOOP_ONE_PAST_END
    } while (actorId < FIELD_ACTOR_COUNT);
#else
    } while (actorId <= FIELD_ACTOR_COUNT);
#endif
}

// The channels of those colours, three a colour, before the OBJ colours'.
#define BG_CHANNELS (PALETTE_FADE_BG_COLORS * 3)

// Fades the palettes to color over ticks ticks, keeping the field's colours for
// fdStartPaletteTransition; FLAG_PSYNERGY_TINTED is set while it holds.
void fdStartColorTransition(Sint32 ticks, Sint32 color)
{
    FIELD_STATE  *field = RamHeapSlots->fieldState;
    PALETTE_FADE *state = RamHeapSlots->paletteFade;

    // The OBJ colours are kept always, the BG colours only while Reveal has not greyed them.
    syDMAXCopy(&state->target[BG_CHANNELS],
               &field->savedFadeTarget[BG_CHANNELS],
               DMA_COPY_WORDS((PALETTE_FADE_CHANNELS - BG_CHANNELS) / 2),
               REG_DMA3);
    if (field->revealActive == 0)
        syDMAXCopy(
            state->target, field->savedFadeTarget, DMA_COPY_WORDS(BG_CHANNELS / 2), REG_DMA3);
    syDMAXCopy(state->target, state->current, DMA_COPY_WORDS(sizeof state->current / 4), REG_DMA3);
    fdPaletteFadeSetTarget(PALETTE_TINT_CUSTOM | color, PALETTES_BG);
    syGameFlagSet(FLAG_PSYNERGY_TINTED);
    fdPaletteFadeStartInterpolation(ticks);
}

// Fades the palettes back from fdStartColorTransition's colour over ticks ticks, the BG half to
// grey while Reveal is active; clears FLAG_PSYNERGY_TINTED.
void fdStartPaletteTransition(Sint32 ticks)
{
    FIELD_STATE *state         = RamHeapSlots->fieldState;
    Sint32       savedPalettes = (Sint32)state->savedFadeTarget;

    fdPaletteFadeSetTarget(savedPalettes, PALETTES_OBJECTS);
    if (state->revealActive != 0)
        fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
    else
        fdPaletteFadeSetTarget(savedPalettes, PALETTES_BG);
    syGameFlagClear(FLAG_PSYNERGY_TINTED);
    fdPaletteFadeStartInterpolation(ticks);
}

// Tints the field for a Psynergy animation: a tint chosen by the tick counter (so each cast looks a
// little different), fixed for some animations, and forced by the tint flags; then fades towards it
// over eight ticks. The palettes are copied by fdStartColorTransition.
void fdPsynergyFadeToCastColor(void)
{
    FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
    Sint32              tint;
    Sint32              animation;

    tint      = TickCount & 7;
    animation = scene->fieldPsynergy;
    if (animation == FIELD_PSYNERGY_POUND)
        tint = PSYNERGY_TINT_BLUE;
    if (animation == FIELD_PSYNERGY_BLAZE)
        tint = PSYNERGY_TINT_RED;
    if (animation == FIELD_PSYNERGY_SAND)
        tint = PSYNERGY_TINT_GREY;
    if (animation == FIELD_PSYNERGY_CYCLONE)
        tint = PSYNERGY_TINT_BLUE;
    if (animation == FIELD_PSYNERGY_SCOOP)
        tint = PSYNERGY_TINT_YELLOW;
    if (animation == FIELD_PSYNERGY_ARROW)
        tint = PSYNERGY_TINT_BLUE;
    if (animation == FIELD_PSYNERGY_PARCH)
        tint = PSYNERGY_TINT_GREY;
    if (animation == FIELD_PSYNERGY_TREMOR)
        tint = PSYNERGY_TINT_DARK_GREY;
    if (animation == FIELD_PSYNERGY_GRIND)
        tint = PSYNERGY_TINT_BLUE;
    if (animation == FIELD_PSYNERGY_BURST)
        tint = PSYNERGY_TINT_GREY;
    if (scene->fieldPsynergy == FIELD_PSYNERGY_TELEPORT)
        tint = PSYNERGY_TINT_DARK_GREY;

    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_BLUE)))
        tint = PSYNERGY_TINT_BLUE;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_GREEN)))
        tint = PSYNERGY_TINT_GREEN;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_RED)))
        tint = PSYNERGY_TINT_RED;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_CYAN)))
        tint = PSYNERGY_TINT_CYAN;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_MAGENTA)))
        tint = PSYNERGY_TINT_MAGENTA;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_YELLOW)))
        tint = PSYNERGY_TINT_YELLOW;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_DARK_GREY)))
        tint = PSYNERGY_TINT_DARK_GREY;
    if (syGameFlagGet(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_GREY)))
        tint = PSYNERGY_TINT_GREY;
    fdStartColorTransition(8, FieldPsynergyCastColors[tint]);
}

#undef fdActorSetAnimationSpeedStub
#undef BG_CHANNELS
