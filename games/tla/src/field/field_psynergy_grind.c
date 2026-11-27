// Grind: the screen shakes up and down while spray rises round the target, a sea rock is ground
// away, and the plain sway Grind makes at nothing. Part of field_psynergy.c.

// The splashes of spray Grind keeps.
#define GRIND_SPLASH_COUNT       64
// The largest picture Grind grinds down, in bytes, a byte a pixel.
#define GRIND_PICTURE_SIZE       2048
// The tiles of each frame of the splash picture (RESOURCE_WATER_SPLASH), 16 by 16 pixels in 256
// colours.
#define GRIND_SPLASH_FRAME_TILES SPRITE_TILES_8BPP(16, 16)
// How far in front of or behind the target the spray is drawn.
#define GRIND_SPRAY_DEPTH        15

// Grind's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the splash picture and the splash particles.
typedef struct GRIND_STATE
{
    Uint8                   unused[32]; // no code names it
    Uint8                   picture[GRIND_PICTURE_SIZE];
    FIELD_PSYNERGY_PARTICLE particles[GRIND_SPLASH_COUNT];
    EFFECT_SPR              sprites[GRIND_SPLASH_COUNT];
} GRIND_STATE;

// What Grind does.
typedef enum GRIND_MODE
{
    GRIND_ON_TARGET =
        // the target's Grind script runs as the shaking peaks
    0,
    GRIND_SEA_ROCK   = 1, // a sea rock is ground down row by row
    GRIND_ON_NOTHING = 2  // spray rises before the caster
} GRIND_MODE;

// Grind ("Pulverize large objects."): the screen shakes harder and harder while spray rises round
// the target, or round the spot 32 pixels before the caster. A sea rock (SPRITE_SEA_ROCK) is ground
// down row by row; otherwise the place's Grind script runs while the shaking is at its height.
void fdPsynergyGrindOnTarget(void)
{
    GRIND_STATE        *state  = syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, sizeof(GRIND_STATE));
    FIELD_EFFECT_STATE *scene  = RamHeapSlots->fieldEffectState;
    ACTOR              *caster = scene->caster;
    ACTOR              *child  = scene->target;
    SPR                *animation = child->sprite;
    EFFECT_SPR         *sprite;
    FIELD_PSYNERGY_PARTICLE *particle;
    void                    *src;
    Uint8                   *dest;
    Sint32                   angle;
    Sint32                   i;
    Sint32                   entry;
    Sint32                   tile;
    Sint32                   phase;
    Sint32                   timer;
    Sint32                   shifts;
    Sint32                   animating;
    Sint32                   spraying;
    Sint32                   next;
    Sint32                   isSeaRock;
    Sint32                   mode;
    FIXED                    radius;
    Sint32                   selector;
    CUE                     *pending;
    Uint8                   *picture;
    Sint32                   counter;
    FIXED                    amplitude;

    shifts    = 0;
    radius    = toFIXED(14);
    animating = 0;
    spraying  = 0;
    next      = 0;
    mode      = GRIND_ON_TARGET;
    selector  = fdSelectPsynergyTarget(scene->fieldPsynergy);
    pending = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy, selector);
    if (child != NULL)
    {
        if (animation != NULL)
        {
            if (animation->width * animation->height <= GRIND_PICTURE_SIZE)
            {
            }
            else
                goto end;
        }
    }
    else
    {
        scene->x = caster->x.fixed;
        scene->z = caster->z.fixed + toFIXED(8);
        scene->y = caster->y.fixed;
        syVectorAddPolarOffset(toFIXED(32), scene->angle, &scene->x);
    }
    isSeaRock = 0;
    if (fdActorGetSpriteIdStub(child) == SPRITE_SEA_ROCK)
    {
        isSeaRock = 1;
        mode      = GRIND_SEA_ROCK;
    }
    if (child == NULL)
        mode = GRIND_ON_NOTHING;
    if (mode == GRIND_SEA_ROCK)
        radius = toFIXED(14);
    if (mode == GRIND_ON_NOTHING)
        radius = toFIXED(21);

    fdPsynergyFadeToCastColor();
    src     = syResourceGetTableEntry(RESOURCE_WATER_SPLASH);
    picture = state->picture;
    syDecompressTaggedLZ(src, picture);
    entry    = syVRAMCacheFindFreeEntry();
    tile     = syVRAMCacheLoad(entry, 1024, picture);
    particle = state->particles;
    for (i = 0; i < GRIND_SPLASH_COUNT; i++)
    {
        sprite = &state->sprites[i];
        fdEffectSpriteInit(sprite, 8, 8, OBJ_SIZE_16X16, tile);
        sprite->oam.fullColor = 1;
        sprite->oam.palette   = 0;
        sprite->oam.priority  = 0;
        sprite->depth         = OAM_DEPTH_WINDOW;
        particle->timer       = -1;
        particle++;
    }
    if (child != NULL && animation != NULL && isSeaRock == 1)
    {
        fdSpriteSetAnimationSpeedStub(animation, 0);
        fdPsynergyDecodePicture(
            (PICTURE_SOURCE *)animation->entries[0], picture, animation->width * animation->height);
    }
    fdViewMoveStart(VIEW_MOVE_Z);
    sySynch(1);
    sySoundPlayStub(SE_PUSH_STEP);
    counter   = 0;
    amplitude = toFIXED(2);
    phase     = SWAY_GROW;
    timer     = 0;
    do
    {
        if (spraying)
        {
            if (mode == GRIND_SEA_ROCK)
            {
                if (radius > toFIXED(5))
                    radius += -(toFIXED(1) / 25);
            }
            else if (radius > toFIXED(5))
            {
                radius += -(toFIXED(2) / 25);
            }
        }
        for (i = 0; i < spraying; i++)
        {
            angle = syRandom();
            if (mode == GRIND_ON_NOTHING)
                angle = ((syRandom() * 10) >> 4) - TRIG_TURN / 16;
            sprite   = &state->sprites[next];
            particle = &state->particles[next];
            if (child != NULL)
            {
                sprite->oam.priority = fdActorGetSpritePriority(child);
                if (angle < TRIG_TURN / 2)
                    sprite->depth = fdActorGetSpriteDepth(child) + GRIND_SPRAY_DEPTH;
                else
                    sprite->depth = fdActorGetSpriteDepth(child) - GRIND_SPRAY_DEPTH;
            }
            particle->timer = 0;
            SET_POSITION(particle, scene->x, scene->y + toFIXED(5), scene->z);
            if (radius > toFIXED(5))
                syVectorAddPolarOffset(radius + syRandom() * 5, angle, &particle->x);
            else
                syVectorAddPolarOffset(radius - (syRandom() << 2), angle, &particle->x);
            next++;
            next %= GRIND_SPLASH_COUNT;
        }
        particle = state->particles;
        for (i = 0; i < GRIND_SPLASH_COUNT; i++, particle++)
        {
            sprite = &state->sprites[i];
            if ((Uint32)particle->timer <= 23)
            {
                if (particle->timer == 0)
                    sprite->oam.index = tile;
                if (particle->timer == 18)
                    sprite->oam.index = tile + GRIND_SPLASH_FRAME_TILES;
                if (particle->timer == 20)
                    sprite->oam.index = tile + 2 * GRIND_SPLASH_FRAME_TILES;
                if (particle->timer == 22)
                    sprite->oam.index = tile + 3 * GRIND_SPLASH_FRAME_TILES;
                fdEffectSpriteDrawAtWorldPos(sprite, particle);
                particle->timer++;
            }
        }
        if (child != NULL && animation != NULL && animating)
        {
            Sint32 size = animation->width * animation->height;
            dest =
                (Uint8 *)OBJ_VRAM0 + syVRAMCacheLoad(animation->slot, size, NULL) * TILE_SIZE_4BPP;
            if (shifts % 3 == 0)
                fdBattleEffectShiftRowsDown(picture, animation->width, animation->height);
            shifts++;
            ((void (*)(void *, Sint32, Sint32, void *))RamHeapSlots->pictureUploader)(
                picture, animation->width, animation->height, dest);
        }
        fdViewMoveSwing(VIEW_MOVE_Z, counter++, amplitude);
        switch (phase)
        {
            case SWAY_GROW:
                if (timer == 40 && mode != GRIND_ON_TARGET)
                    spraying = 1;
                amplitude += toFIXED(1) / 10;
                if (amplitude > toFIXED(12) - 1)
                {
                    phase++;
                    timer = -1;
                }
                break;
            case SWAY_HOLD:
                if (isSeaRock == 0)
                {
                    if (timer == 50)
                    {
                        phase = SWAY_SHRINK;
                        timer = -1;
                    }
                    if (pending != NULL)
                        ((void (*)(Sint32, Sint32, Sint32))pending->value)(0, timer, selector);
                }
                else
                {
                    if (timer == animation->height * 3)
                    {
                        phase = SWAY_SHRINK;
                        timer = -1;
                    }
                    animating = 1;
                }
                break;
            case SWAY_SHRINK:
                if (isSeaRock == 1)
                    animating = 0;
                if (timer == 5 && mode != GRIND_ON_TARGET)
                    spraying = 0;
                amplitude += -(toFIXED(3) / 20);
                if (amplitude <= toFIXED(2))
                {
                    phase++;
                    timer = -1;
                }
                break;
            case SWAY_SETTLE:
                amplitude = 0;
                if (timer == 0)
                    phase = SWAY_DONE;
                break;
        }
        sySynch(1);
        timer++;
    } while (phase != SWAY_DONE);
    sySoundPlayStub(SOUND_FADE_OUT_ACTION_SE);
    fdViewMoveEnd();
    fdPsynergyFadeFromCastColor();
    syVRAMCacheFreeEntry(entry);
end:
    syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

// FIELD_PSYNERGY_GRIND as fdPsynergyRun casts it: the view sways.

// Sways the view up and down the screen: the swing grows from SWAY_START to SWAY_PEAK, holds for
// SWAY_HOLD_TICKS, shrinks back and stops; then the map's view bounds come back.
void fdPsynergyGrind(void)
{
    FIELD_EFFECT_STATE *scene;
    Sint32              phase;
    Sint32              amplitude;
    Sint32              state;
    Sint32              ticks;

    // Grind's state's size, though the sway holds only the view move.
    syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, sizeof(GRIND_STATE));
    scene = RamHeapSlots->fieldEffectState;
    fdPsynergyFadeToCastColor();
    fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
    fdViewMoveStart(VIEW_MOVE_Z);
    sySynch(10);

    phase     = 0;
    amplitude = SWAY_START;
    state     = SWAY_GROW;
    ticks     = 0;
    do
    {
        fdViewMoveSwing(VIEW_MOVE_Z, phase++, amplitude);
        switch (state)
        {
            case SWAY_GROW:
                amplitude += SWAY_RAMP;
                if (amplitude >= SWAY_PEAK)
                {
                    state = SWAY_HOLD;
                    ticks = -1;
                }
                break;
            case SWAY_HOLD:
                if (ticks == SWAY_HOLD_TICKS)
                {
                    state = SWAY_SHRINK;
                    ticks = -1;
                }
                break;
            case SWAY_SHRINK:
                amplitude -= SWAY_RAMP;
                if (amplitude <= SWAY_START)
                {
                    state = SWAY_SETTLE;
                    ticks = -1;
                }
                break;
            case SWAY_SETTLE:
                amplitude = 0;
                if (ticks == 0)
                    state = SWAY_DONE;
                break;
        }
        sySynch(1);
        ticks++;
    } while (state != SWAY_DONE);

    fdViewMoveEnd();
    fdPsynergyFadeFromCastColor();
    syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}
