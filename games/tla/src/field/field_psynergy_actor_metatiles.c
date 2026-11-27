// Actors standing as blocks in the map: their metatiles tagged and the tags cleared, the actor at a
// position, and effect sprites filled and queued. Part of field_psynergy.c.

// A 16.16 height in the 8-pixel steps a shape entry's height counts.
#define HEIGHT_STEPS(height) (FIXEDtoINT(height) >> 3)
// A block's height, 32 pixels, in those steps.
#define BLOCK_HEIGHT_STEPS   4

// Makes each listed actor (ids, ended by ACTOR_LIST_END) a block in the map: its metatile's shape
// is tagged BLOCK_HEIGHT_STEPS above the ground, so the party can stand on it, its metatile is
// marked MAP_METATILE_JUMP, a gap to jump onto from another block (the old mark kept in the actor),
// and the overhead layer's metatile is a wall. A leader below the new ground is lifted onto it.
// Scenes call it as they start, for the blocks Move pushes.
void fdActorPlaceBlocks(Uint16 *actors)
{
    MAP_STATE    *map    = RamHeapSlots->mapGrid;
    MAP_METATILE *blocks = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
    Uint8        *grid   = MapShapeGrid;
    ACTOR        *leader;
    Sint32        height;

    for (; *actors != ACTOR_LIST_END; actors++)
    {
        ACTOR *actor = fdActorGetById(*actors);
        Uint32 key   = fdMapGridGetTerrainKindStub(actor->layer, actor->x.fixed, actor->z.fixed);
        Sint32 level = HEIGHT_STEPS(
            fdMapGridGetTerrainHeightStub(actor->layer, actor->x.fixed, actor->z.fixed));
        Sint32        slot      = fdActorGetMetatileIndex(actor);
        MAP_METATILE *metatiles = map->layers[actor->layer].metatiles;
        // The layer's row of the shape grid, as an integer sum with its index first.
        Uint8        *shapes =
            (Uint8 *)((((Sint32)metatiles - (Sint32)RamMapMetatileBuffer) >> 2) + (Sint32)grid);
        MAP_METATILE *metatile;
        Sint32        tag;
        Uint8         flags;

        actor->motionFlags    = 0;
        actor->collisionFlags = 0;
        fdActorSetSpriteFlagsStub(actor, 0);
        shapes               += slot;
        tag                   = fdMapShapeFindOrAdd(actor->layer, key, level + BLOCK_HEIGHT_STEPS);
        metatile              = &metatiles[slot];
        flags                 = metatile->attributeB;
        actor->sceneWord      = flags;
        metatile->attributeB  = flags | MAP_METATILE_JUMP;
        *shapes               = tag;
        metatile              = &blocks[slot];
        metatile->collisionCode = METATILE_BEHAVIOR_WALL;
    }
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    height = fdMapGridGetTerrainHeightStub(leader->layer, leader->x.fixed, leader->z.fixed);
    if (leader->y.fixed < height)
        leader->y.fixed = leader->groundHeight = height;
}

// Takes the block about to move out of the map: the field effect's target while one runs, else the
// actor ahead of the leader, if it is one of the listed actors. Its metatile goes back to the
// ground and its old mark, and the script state remembers it for fdActorPlaceMovedBlock.
void fdActorLiftTargetBlock(Uint16 *actors)
{
    FIELD_STATE        *event = RamHeapSlots->fieldState;
    FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffectState;
    ACTOR              *target;

    if (state != NULL && event->psynergy != 0)
        target = state->target;
    else
        target = fdLeaderFindActorAhead();
    event->movedBlock = NULL;

    for (; *actors != ACTOR_LIST_END; actors++)
    {
        if (target == fdActorGetById(*actors))
        {
            MAP_STATE    *map       = RamHeapSlots->mapGrid;
            Uint8        *grid      = MapShapeGrid;
            MAP_METATILE *metatiles = map->layers[target->layer].metatiles;
            MAP_METATILE *blocks    = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
            Uint8        *shapes = &grid[((Sint32)metatiles - (Sint32)RamMapMetatileBuffer) >> 2];
            Uint32        key =
                fdMapGridGetTerrainKindStub(target->layer, target->x.fixed, target->z.fixed);
            Sint32 level =
                fdMapGridGetTerrainHeightStub(target->layer, target->x.fixed, target->z.fixed);
            Sint32 slot = fdActorGetMetatileIndex(target);
            Sint32 tag;

            shapes += slot;
            tag = fdMapShapeFindOrAdd(target->layer, key, HEIGHT_STEPS(level) - BLOCK_HEIGHT_STEPS);
            metatiles                         += slot;
            blocks                            += slot;
            ((SHAPE_METATILE *)shapes)->shape  = tag;
            metatiles->attributeB              = target->sceneWord;
            blocks->collisionCode              = 0;
            event->movedBlock                  = target;
        }
    }
}

// Puts the block fdActorLiftTargetBlock remembered back into the map at the metatile it has moved
// to, as fdActorPlaceBlocks places each.
void fdActorPlaceMovedBlock(void)
{
    ACTOR *target = FieldStateWork->movedBlock;

    if (target != NULL)
    {
        MAP_STATE    *map       = RamHeapSlots->mapGrid;
        Uint8        *grid      = RamMapShapeGrid;
        MAP_METATILE *metatiles = map->layers[target->layer].metatiles;
        MAP_METATILE *blocks    = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
        Uint8        *shapes    = &grid[((Sint32)metatiles - (Sint32)RamMapMetatileBuffer) >> 2];
        Uint32 key = fdMapGridGetTerrainKindStub(target->layer, target->x.fixed, target->z.fixed);
        Sint32 level =
            fdMapGridGetTerrainHeightStub(target->layer, target->x.fixed, target->z.fixed);
        Sint32 slot = fdActorGetMetatileIndex(target);
        Sint32 tag;

        shapes += slot;
        tag     = fdMapShapeFindOrAdd(target->layer, key, HEIGHT_STEPS(level) + BLOCK_HEIGHT_STEPS);
        ((SHAPE_METATILE *)shapes)->shape  = tag;
        metatiles                         += slot;
        target->sceneWord                  = metatiles->attributeB;
        metatiles->attributeB             |= MAP_METATILE_JUMP;
        blocks                            += slot;
        blocks->collisionCode              = METATILE_BEHAVIOR_WALL;
    }
}

// Returns the actor one metatile, or failing that two metatiles, ahead of the leader at the
// leader's height, or NULL.
ACTOR *fdLeaderFindActorAhead(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    FIXED  pos[3];
    // The leader's facing rounded to the nearest quarter turn, in sixteenths.
    Sint32 angle = (leader->facing / FACING_STEP + 2) & 0xc;
    ACTOR *found;

    pos[0] = leader->x.fixed;
    pos[1] = leader->y.fixed;
    pos[2] = leader->z.fixed;
    syVectorAddPolarOffset(toFIXED(16), angle * FACING_STEP, pos);
    found = fdActorFindAtMetatile(pos, 1);
    if (found != NULL)
        return found;
    pos[0] = leader->x.fixed;
    pos[1] = leader->y.fixed;
    pos[2] = leader->z.fixed;
    syVectorAddPolarOffset(toFIXED(32), angle * FACING_STEP, pos);
    found = fdActorFindAtMetatile(pos, 1);
    if (found != NULL)
        return found;
    return NULL;
}

// Returns the first of the placed actors (not the treasure spots) on position's metatile, at its
// height too when checkY is set, or NULL.
ACTOR *fdActorFindAtMetatile(Sint32 *pos, Sint32 checkY)
{
    Uint32 i;

    for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++)
    {
        FIELD_STATE *event = RamHeapSlots->fieldState;
        ACTOR       *actor = event->actors[i];

        if (checkY)
        {
            if (FIELD_METATILE(pos[0]) == FIELD_METATILE(actor->x.fixed)
                && pos[1] / toFIXED(1) == actor->y.fixed / toFIXED(1)
                && FIELD_METATILE(pos[2]) == FIELD_METATILE(actor->z.fixed))
                return actor;
        }
        else
        {
            if (FIELD_METATILE(pos[0]) == FIELD_METATILE(actor->x.fixed)
                && FIELD_METATILE(pos[2]) == FIELD_METATILE(actor->z.fixed))
                return actor;
        }
    }
    return NULL;
}

// The width and height of each OAM shape and size, in pixels, by the shape times 4 plus the size.
extern const Uint8 OamShapeSizes[][2];
// Where an attr01 word holds a sprite's size (attribute 1's top two bits) and shape (attribute
// 0's).
#define OBJ_SIZE_SHIFT  30
#define OBJ_SHAPE_SHIFT 14

// Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix, 256 colours), the
// tiles, unscaled and unrotated at depth 255, and the picture's size from its shape, pivoted at
// (pivotX, pivotY).
void fdEffectSpriteInit(
    EFFECT_SPR *sprite, Sint32 pivotX, Sint32 pivotY, Uint32 attributes, Sint32 tile)
{
    Uint32 *words;
    Sint32  shape;

    if (sprite == NULL)
        return;
    syWordsFillEntry(sprite, sizeof(*sprite), 0);
    sprite->scaleX = toFIXED(1);
    sprite->scaleY = toFIXED(1);
    sprite->depth  = OAM_DEPTH_FRONT;
    sprite->pivotX = pivotX;
    sprite->pivotY = pivotY;
    shape          = ((attributes >> OBJ_SIZE_SHIFT) | (attributes >> (OBJ_SHAPE_SHIFT - 2))) & 0xf;
    sprite->width  = OamShapeSizes[shape][0];
    sprite->height = OamShapeSizes[shape][1];
    words          = (Uint32 *)&sprite->oam;
    *words++       = attributes;
    *words         = 0;
    // Spelt through the OAM's address, as the two words above are written.
    (&sprite->oam)->fullColor = 1;
    sprite->oam.index         = tile;
}

// Draws a field effect's sprite. An unscaled, unrotated sprite needs no affine matrix; any other
// takes one, and one scaled past 1 is drawn in a double-size box. The picture's pivot is turned and
// scaled with it, so the pivot stays on the sprite's position. A sprite wholly off screen is not
// queued; the rest join the OAM bucket of their depth.
void fdEffectSpriteDraw(EFFECT_SPR *sprite)
{
    Sint32           affineMode;
    Sint32           matrix;
    FIXED            x      = sprite->x;
    FIXED            y      = sprite->y;
    Sint32           scaleX = sprite->scaleX;
    Sint32           scaleY = sprite->scaleY;
    Sint32           width  = sprite->width;
    Sint32           height = sprite->height;
    Sint32           left;
    Sint32           top;
    Sint32           screenX;
    Sint32           screenY;
    Sint32           magnitude;
    AFFINE_TRANSFORM transform;

    if (sprite->depth == EFFECT_SPRITE_HIDDEN || scaleX == 0 || scaleY == 0)
        return;

    // The affine matrix, unless the sprite is drawn as it is.
    if (scaleX == toFIXED(1) && scaleY == scaleX && sprite->angle == 0)
    {
        matrix     = 0;
        affineMode = OAM_AFFINE_OFF;
    }
    else
    {
        transform.angle  = sprite->angle;
        transform.scaleX = scaleX >> 8;
        transform.scaleY = scaleY >> 8;
        matrix           = syOAMMatrixAllocateFromTransform(&transform);
        affineMode       = OAM_AFFINE_ON;
    }

    // Grown past its own size, it needs the double-size box.
    magnitude = scaleX;
    if (magnitude < 0)
        magnitude = -magnitude;
    if (magnitude > toFIXED(1)
        || (magnitude = scaleY < 0 ? -scaleY : scaleY, magnitude > toFIXED(1)))
    {
        affineMode  = OAM_AFFINE_DOUBLE_SIZE;
        width      *= 2;
        height     *= 2;
    }

    // Where the pivot sits in the drawn box, from the box's top left.
    if (sprite->angle == 0)
    {
        if (scaleX == toFIXED(1) && scaleY == scaleX)
        {
            left = sprite->pivotX - (sprite->width >> 1) + width / 2;
            top  = sprite->pivotY - (sprite->height >> 1) + height / 2;
        }
        else
        {
            left = FIXEDtoINT(scaleX * (sprite->pivotX - (sprite->width >> 1)) + (toFIXED(1) - 1))
                 + width / 2;
            top = FIXEDtoINT(scaleY * (sprite->pivotY - (sprite->height >> 1)) + (toFIXED(1) - 1))
                + height / 2;
        }
    }
    else
    {
        FIXED  sin        = sySin(sprite->angle);
        FIXED  cos        = syCos(sprite->angle);
        Sint32 halfWidth  = sprite->width * scaleX / 2;
        Sint32 halfHeight = sprite->height * scaleY / 2;
        FIXED  cornerX    = syMulFX(cos, halfWidth) - syMulFX(sin, halfWidth);
        FIXED  cornerY    = syMulFX(cos, halfHeight) + syMulFX(sin, halfHeight);
        Sint32 pivotX     = scaleX * sprite->pivotX;
        Sint32 pivotY     = sprite->pivotY * scaleY;
        Sint32 turnedX    = syMulFX(cos, pivotX) - syMulFX(sin, pivotY);
        Sint32 turnedY    = syMulFX(cos, pivotY) + syMulFX(sin, pivotX);

        left              = width / 2 + FIXEDtoINT(turnedX - cornerX);
        top               = height / 2 + FIXEDtoINT(turnedY - cornerY);
    }

    // The box on screen; skip it when none of it shows.
    screenX = FIXEDtoINT(x) - left;
    screenY = FIXEDtoINT(y) - top;
    if (screenX + width < 0 || screenX >= DISPLAY_WIDTH || screenY + height <= 0
        || screenY >= DISPLAY_HEIGHT)
        return;

    sprite->oam.x          = screenX;
    sprite->oam.y          = screenY;
    sprite->oam.affineMode = affineMode;
    sprite->oam.matrix     = matrix;
    syOAMAddSpriteAtDepthClamped(sprite, sprite->depth);
}

// Places sprite at the screen position of world position and draws it.
void fdEffectSpriteDrawAtWorldPos(EFFECT_SPR *sprite, Sint32 *pos)
{
    Sint32 screen[3];

    if (sprite != 0)
    {

        screen[0] = pos[0];
        screen[1] = pos[1];
        screen[2] = pos[2];
        fdCameraWorldToScreen(screen);
        sprite->x = screen[0];
        sprite->y = screen[2];
        fdEffectSpriteDraw(sprite);
    }
}

#undef HEIGHT_STEPS
#undef BLOCK_HEIGHT_STEPS
#undef OBJ_SIZE_SHIFT
#undef OBJ_SHAPE_SHIFT
