// The ground particles, the rising particles and the floating motes. Part of script_commands.c.

// mote.png's six pictures, four tiles each: two of each size, the motes shrinking as they rise and
// flickering between the two.
#define MOTE_PICTURE_TILES SPRITE_TILES(16, 16)
#define MOTE_LARGE         0
#define MOTE_MEDIUM        2
#define MOTE_SMALL         4

// Unused: nothing calls fdGroundParticlesStartStub. The Broken Seal's rain, kept whole where this
// game has its own fdWeatherStartRain: 64 sprites placed at the leader's position on the terrain,
// their tiles decoded into VRAM, the blend set, and fdGroundParticlesUpdate scheduled to move them.
void fdGroundParticlesStart(void)
{
    WEATHER_STATE    *state;
    Sint32           *leader;
    WEATHER_PARTICLE *particle;
    // words is the decode buffer, then each particle's attribute cursor.
    Uint32           *words;
    Uint32            i;
    Uint32            fill = 0;

    state                  = syHeapSlotAllocateEWRAM(HEAP_SLOT_WEATHER, sizeof(WEATHER_STATE));
    leader                 = ((MAP_STATE *)RamHeapSlots->mapGrid)->origin;
    fdQueueMusic(SE_AMBIENT_WATER);
    particle = state->particles;
    DMA_FILL32(fill, state, sizeof(WEATHER_STATE));
    words = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
    syDecompressByteLZ(WeatherParticleTiles, words);
    state->entry    = syVRAMCacheFindFreeEntry();
    state->tileBase = syVRAMCacheLoad(state->entry, 6 * WEATHER_PICTURE_BYTES, words);
    syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
    for (i = 0; i < WEATHER_PARTICLE_COUNT; i++)
    {
        Sint32 x;
        Sint32 z;

        words           = (Uint32 *)particle;
        *words++        = 0;
        *words++        = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        *words          = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
        x               = leader[0];
        z               = leader[2];
        particle->posX  = x;
        particle->posZ  = z;
        particle->posY  = fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, z);
        particle->timer = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
        particle++;
    }
    syIOWrite16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
    syIOWrite16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
    syIOWrite16(0, &REG_BLDY);
    sySetEvent(fdGroundParticlesUpdate, EVENT_PASS_TICK);
}

// Event of fdRisingParticlesStart: updates the 64 rising particles; each starts on the ground at a
// random point of the strip for 120 ticks, drifts right by a random amount each tick, rises with
// its age, and is drawn at priority 3, its tile stepping back by 8 at 90 and 60 ticks left and
// flickering between two tiles every 8 ticks.
void fdRisingParticlesUpdate(void)
{
    RISING_PARTICLE_STATE *state    = RamHeapSlots->weather;
    RISING_PARTICLE       *particle = state->particles;
    FIELD_COORDINATE       screen[3];
    Uint32                 i;
    Sint32                 x;
    Sint32                 y;

    for (i = 0; i < ARRAY_COUNT(state->particles); i++, particle++)
    {
        if (particle->timer == 0)
        {
            x               = syRandom() * 200 + toFIXED(288);
            y               = toFIXED(80) - syRandom() * 20;
            particle->posX  = x;
            particle->posZ  = y;
            particle->posY  = fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, y);
            particle->timer = 120;
            particle->drift = 0;
        }
        screen[0].fixed = particle->posX + particle->drift;
        screen[1].fixed = particle->posY;
        screen[2].fixed = particle->posZ;
        fdCameraWorldToScreen(screen);
        x                = screen[0].part[0].integer;
        y                = screen[2].part[0].integer - particle->timer - 8;
        particle->drift += syRandom() + toFIXED(1) / 2;
        if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT)
        {
            if (particle->timer < 60)
                particle->sprite.attributes.tile = state->tile + MOTE_SMALL * MOTE_PICTURE_TILES;
            else if (particle->timer < 90)
                particle->sprite.attributes.tile = state->tile + MOTE_MEDIUM * MOTE_PICTURE_TILES;
            else
                particle->sprite.attributes.tile = state->tile + MOTE_LARGE * MOTE_PICTURE_TILES;
            particle->sprite.attributes.tile =
                ((TickCount >> 3) & 1) * MOTE_PICTURE_TILES + particle->sprite.attributes.tile;
            particle->sprite.attributes.x          = x;
            particle->sprite.attributes.shape      = OBJ_SHAPE_SQUARE;
            particle->sprite.attributes.size       = OAM_SIZE_16X16;
            particle->sprite.attributes.priority   = 3;
            particle->sprite.attributes.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
            particle->sprite.attributes.y          = y;
            syOAMAddSpriteAtDepthClamped(particle, OAM_DEPTH_WINDOW);
        }
        particle->timer--;
    }
}

// Starts the rising particles, as the Northern Reaches does: allocates and clears heap slot
// HEAP_SLOT_WEATHER, decodes the particle tiles into VRAM, sets each of the 64 particles' sprite
// words and places it on the ground at a random point of the strip (x 288 to 488 pixels, z 60 to
// 80), staggering their timers by 2 ticks, and schedules the update.
void fdRisingParticlesStart(void)
{
    RISING_PARTICLE_STATE *state =
        syHeapSlotAllocateEWRAM(HEAP_SLOT_WEATHER, sizeof(WEATHER_STATE));
    RISING_PARTICLE *particle = state->particles;
    volatile Uint32  fill;
    Uint8           *tiles;
    Uint32           i;
    FIXED            z;

    // FAKEMATCH(temporary): z, holding the fill's 0 before it is a particle's
    // * z; as 0 in the fill.
    z = 0;
    DMA_FILL32_CELL(fill, z, state, sizeof(WEATHER_STATE));
    tiles = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
    syDecompressByteLZ(FieldEffectMoteTiles, tiles);
    state->vramEntry = syVRAMCacheFindFreeEntry();
    state->tile      = syVRAMCacheLoad(state->vramEntry, 6 * WEATHER_PICTURE_BYTES, tiles);
    syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
    for (i = 0; i < ARRAY_COUNT(state->particles); i++, particle++)
    {
        Uint32 *sprite = particle->sprite.words;
        Sint32  x;

        *sprite++       = 0;
        *sprite++       = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        *sprite         = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
        x               = syRandom() * 200 + toFIXED(288);
        z               = toFIXED(80) - syRandom() * 20;
        particle->posX  = x;
        particle->posZ  = z;
        particle->posY  = fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, z);
        particle->drift = 0;
        particle->timer = i * 2;
    }
    sySetEvent(fdRisingParticlesUpdate, EVENT_PASS_TICK);
}

// Event of fdMotesStart: moves and draws the 64 motes in HEAP_SLOT_WEATHER. Up to eight a tick
// start within 128 pixels of the point the view follows and live 120 ticks, rising, shrinking at 90
// and 60 ticks left and flickering every 8 ticks; FLAG_WEATHER_HELD holds them where they are.
void fdMotesUpdate(void)
{
    WEATHER_STATE    *state   = RamHeapSlots->weather;
    MAP_STATE        *map     = RamHeapSlots->mapGrid;
    Uint32            spawned = 0;
    Uint32            i;
    WEATHER_PARTICLE *mote;

    for (i = 0, mote = state->particles; i < WEATHER_PARTICLE_COUNT; i++, mote++)
    {
        Sint32 x;
        Sint32 y;

        if (--mote->timer != GROUND_PARTICLE_FREED)
        {
            Sint32 cameraX = syMapStateView(map)[0];
            Sint32 cameraZ = syMapStateView(map)[1];
            Sint16 age     = mote->timer;
            Uint32 firstJitterDraw;
            Uint32 secondJitterDraw;

            if (syGameFlagGet(FLAG_WEATHER_HELD))
            {
                mote->timer++;
                mote->delay--;
            }
            // x jitters by -1 or 0, and y is the ticks left in pixels above the spot; a quarter of
            // the delay, which builds up as it ages, lifts it further.
            firstJitterDraw  = syRandom();
            secondJitterDraw = syRandom();
            x                = FIXEDtoINT(mote->posX - cameraX)
              + (((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
            y = FIXEDtoINT(mote->posZ - mote->posY - cameraZ) - (Uint16)age;
            if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT)
            {
                if (mote->timer < 60)
                {
                    mote->tile   = state->tileBase + MOTE_SMALL * MOTE_PICTURE_TILES;
                    mote->delay += 3;
                }
                else if (mote->timer < 90)
                {
                    mote->tile = state->tileBase + MOTE_MEDIUM * MOTE_PICTURE_TILES;
                    mote->delay++;
                }
                else
                {
                    mote->tile = state->tileBase + MOTE_LARGE * MOTE_PICTURE_TILES;
                }
                if ((TickCount >> 3) & 1)
                    mote->tile += MOTE_PICTURE_TILES;
                mote->x     = x;
                mote->y     = y - (mote->delay >> 2);
                mote->shape = OBJ_SHAPE_SQUARE;
                mote->size  = OAM_SIZE_16X16;
                syOAMAddSpriteAtDepthClamped(mote, OAM_DEPTH_WINDOW);
            }
            else
            {
                mote->timer = 0;
            }
        }
        if (spawned < 8 && mote->timer == 0)
        {
            Sint32 *leader = map->origin;

            x              = leader[0] + (syRandom() << 8) - toFIXED(128);
            y              = leader[2] + (syRandom() << 8) - toFIXED(128);
            mote->posX     = x;
            mote->posZ     = y;
            mote->posY     = fdMapGridGetTerrainHeightStub(MAPGRID_LAYER_GROUND, x, y);
            mote->timer    = 120;
            mote->delay    = 0;
            spawned++;
        }
    }
}

#undef MOTE_PICTURE_TILES
#undef MOTE_LARGE
#undef MOTE_MEDIUM
#undef MOTE_SMALL
