// Each tick, raises or lowers each geyser on its timer, writes its height to the collision and
// metatile maps, and carries its platform with a bob.
void scGeyserUpdate(void)
{
    GEYSER_WORK  *state        = GEYSER_STATE;
    MAP_STATE    *map          = RamHeapSlots->mapGrid;
    GEYSER       *entry        = state->entries;
    Sint32        firstTerrain = state->firstTerrain;
    Uint8        *terrain      = map->layers[0].terrainIds;
    MAP_METATILE *metatiles    = map->layers[0].metatiles;
    Sint32        i            = 0;
    Sint32       *collision;

    for (collision = (Sint32 *)RamMapCollision + firstTerrain; i < GEYSER_ENTRIES;
         collision++, entry++, i++)
    {
        ACTOR        *actor;
        ACTOR        *rider;
        MAP_METATILE *metatile;
        Sint32        index;

        if (entry->id == 0)
            continue;
        actor = fdActorGetById(entry->id);
        if (actor->x.fixed == 0 && actor->z.fixed == 0)
            continue;
        if (entry->timer == entry->riseAt)
        {
            sySoundPlay(SE_CURSE_FALL);
            entry->step = 4;
        }
        if (entry->timer == entry->fallAt)
        {
            sySoundPlay(SOUND_FADE_OUT_EXIT_SE);
            entry->step = -8;
        }
        if (entry->step != 0)
        {
            entry->height += entry->step;
            if (entry->height >= entry->max)
            {
                entry->height = entry->max;
                entry->step   = 0;
                entry->phase  = 0;
            }
            if (entry->height <= entry->min)
            {
                entry->height = entry->min;
                entry->step   = 0;
                entry->phase  = 0;
            }
        }
        if (!syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS))
        {
            if (++entry->timer >= entry->period)
                entry->timer = 0;
        }
        index = FIELD_METATILE(actor->x.fixed) + FIELD_METATILE(actor->z.fixed) * MAPGRID_SIZE;
        terrain[index] = (Uint8)state->firstTerrain + i;
        *collision     = ((actor->y.part[0].integer + entry->height) / 8) << 8;
        metatile       = &metatiles[index];
        if (entry->step != 0)
            metatile->attributeB = 0;
        else
            metatile->attributeB |= MAP_METATILE_JUMP;
        if (entry->height != 0)
            metatile->attributeB |= MAP_METATILE_WATER;
        else
            metatile->attributeB = 0;
        rider = entry->rider;
        if (rider == NULL)
            continue;
        if (entry->height == 0)
        {
            rider->y.fixed = actor->y.fixed + -toFIXED(16);
        }
        else
        {
            FIXED wave;

            entry->phase   += TRIG_TURN / 16;
            wave            = syMulFX(toFIXED(2), sySin(entry->phase)) - toFIXED(14);
            rider->y.fixed  = actor->y.fixed + toFIXED(entry->height) + wave;
        }
        rider->x.fixed = actor->x.fixed;
        rider->z.fixed = actor->z.fixed;
    }
}
