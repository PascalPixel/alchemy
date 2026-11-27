// Tremor: the view swayed from side to side. Part of field_psynergy.c.

// Tremor cast at a chosen object (fdPsynergyDispatch): fdPsynergyTremor's sideways sway, stronger,
// with the object's picture held in the state.

// The sprites of RESOURCE_SHAKE_TILES' two tiles the state sets up.
#define SHAKE_SPRITE_COUNT           64
// The OBJ palette the shake sprites take.
#define SHAKE_SPRITE_PALETTE         14
#define PICTURE_SIZE                 4096
#define PICTURE_MAX_PIXELS           2048

// How far round the nearby objects' shaking goes each tick: 3/32 of a turn.
#define SHAKE_STEP                   (TRIG_TURN * 3 / 32)
// The objects shake as far as a whole turn for every STRONG_SHAKE_PIXELS_PER_TURN pixels the view
// sways past SWAY_SHAKE.
#define STRONG_SHAKE_PIXELS_PER_TURN 128

// The stronger sway at an object (16.16 pixels): from SWAY_START up to STRONG_SWAY_PEAK, ramping
// more slowly and holding longer.
#define STRONG_SWAY_PEAK             toFIXED(12)
#define STRONG_SWAY_RAMP             (toFIXED(1) / 10) // 0.1 pixels a tick
#define SWAY_SHAKE                   toFIXED(4)        // sprites shake only past this
#define STRONG_SWAY_HOLD_TICKS       40

// The stages of a sway of the view (Tremor's and Grind's): it grows, holds, shrinks and settles,
// and SWAY_DONE ends it.
enum
{
    SWAY_GROW,
    SWAY_HOLD,
    SWAY_SHRINK,
    SWAY_SETTLE,
    SWAY_DONE = 999,
};

// Tremor's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the view's sway, the picture of the shaken
// actor and its falling pieces.
typedef struct TREMOR_STATE
{
    VIEW_MOVE_STATE         view;
    Uint8                   picture[PICTURE_SIZE];
    EFFECT_SPR              sprites[SHAKE_SPRITE_COUNT];
    FIELD_PSYNERGY_PARTICLE particles[SHAKE_SPRITE_COUNT];
} TREMOR_STATE;

// Sets the step of each of a sprite's layers: 0 stops its animation, ANIM_STEP_NORMAL runs it at
// normal speed.
void fdSpriteSetAnimationSpeedStub(void *sprite, Sint32 value);

// Tremor ("Shake objects left and right.") on the chosen object: sways the view from side to side,
// as fdPsynergyTremor does with no object: the swing grows from SWAY_START to STRONG_SWAY_PEAK,
// holds STRONG_SWAY_HOLD_TICKS, shrinks back and stops. Past SWAY_SHAKE the nearby objects' sprites
// shake with it. The state also takes the object's picture and SHAKE_SPRITE_COUNT sprites, which
// nothing here draws. An object whose picture is larger than PICTURE_MAX_PIXELS gets no sway.
void fdPsynergyTremorOnTarget(void)
{
    TREMOR_STATE *tremorState = syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, sizeof(*tremorState));
    FIELD_EFFECT_STATE      *scene        = RamHeapSlots->fieldEffectState;
    FIELD_STATE             *field        = RamHeapSlots->fieldState;
    ACTOR                   *target       = scene->target;
    SPR                     *targetSprite = target->sprite;
    Sint32                   tiles;
    Sint32                   shown;
    Sint32                   width;
    Sint32                   showing;
    Sint32                   frameCount;
    Sint32                   phase;
    Sint32                   i;
    Sint32                   tile;
    Sint32                   amplitude;
    Sint32                   state;
    Sint32                   ticks;
    FIELD_PSYNERGY_PARTICLE *particle;

    if (target == NULL || targetSprite == NULL
        || targetSprite->width * targetSprite->height <= PICTURE_MAX_PIXELS)
    {
        fdPsynergyFadeToCastColor();
        syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_SHAKE_TILES), tremorState->picture);
        tiles = syVRAMCacheFindFreeEntry();
        tile  = syVRAMCacheLoad(tiles, 64, tremorState->picture);
        for (i = 0, particle = tremorState->particles; i < SHAKE_SPRITE_COUNT; i++, particle++)
        {
            EFFECT_SPR *sprite = &tremorState->sprites[i];

            fdEffectSpriteInit(sprite, 4, 4, 0, tile + (i & 1));
            sprite->oam.fullColor = 0;
            sprite->oam.palette   = SHAKE_SPRITE_PALETTE;
            sprite->oam.priority  = 0;
            sprite->depth         = OAM_DEPTH_WINDOW;
            particle->timer       = -1;
        }
        fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
        if (target != NULL && targetSprite != NULL)
        {
            fdSpriteSetAnimationSpeedStub(targetSprite, 0);
            fdPsynergyDecodePicture((PICTURE_SOURCE *)targetSprite->entries[0],
                                    tremorState->picture,
                                    targetSprite->width * targetSprite->height);
        }
        fdViewMoveStart(VIEW_MOVE_X);
        fdViewMoveSaveNearbySprites();
        sySynch(10);

        shown      = -1;
        width      = targetSprite->width;
        showing    = 0;
        phase      = 0;
        frameCount = 0;
        amplitude  = SWAY_START;
        state      = SWAY_GROW;
        ticks      = 0;
        do
        {
            if ((frameCount & 15) == 15)
                sySoundPlayStub(SE_JUMP);

            // Shake the gathered objects' sprites with the view.
            if (field->mode == ACTOR_SYSTEM_MAP)
            {
                for (i = 0; i < tremorState->view.count; i++)
                {
                    SPR   *sprite   = tremorState->view.actors[i]->sprite;
                    Sint32 rotation = tremorState->view.spriteRotations[i];

                    if (amplitude > SWAY_SHAKE)
                        sprite->rotation =
                            rotation
                            + syMulFX(sySin(phase * SHAKE_STEP),
                                      (amplitude - SWAY_SHAKE) / STRONG_SHAKE_PIXELS_PER_TURN);
                    else
                        sprite->rotation = rotation;
                }
            }

            fdViewMoveSwing(VIEW_MOVE_X, phase++, amplitude);
            switch (state)
            {
                case SWAY_GROW:
                    if (ticks == 40)
                        showing = 1;
                    amplitude += STRONG_SWAY_RAMP;
                    if (amplitude >= STRONG_SWAY_PEAK)
                    {
                        state = SWAY_HOLD;
                        ticks = -1;
                    }
                    break;
                case SWAY_HOLD:
                    if (ticks == STRONG_SWAY_HOLD_TICKS)
                    {
                        state = SWAY_SHRINK;
                        ticks = -1;
                    }
                    break;
                case SWAY_SHRINK:
                    if (ticks == 50)
                        showing = -1;
                    amplitude -= STRONG_SWAY_RAMP;
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

            // A count up to the picture's width and back that nothing reads.
            if (showing == 1 && shown <= width)
                shown++;
            if (showing == -1 && shown >= 0)
                shown -= 2;

            sySynch(1);
            ticks++;
            frameCount++;
        } while (state != SWAY_DONE);

        if (target != NULL && targetSprite != NULL)
        {
            fdSpriteSetAnimationSpeedStub(targetSprite, ANIM_STEP_NORMAL);
            ((SPR *)target->sprite)->dirty = 1;
        }
        fdViewMoveEnd();
        fdViewMoveRestoreNearbySprites();
        syVRAMCacheFreeEntry(tiles);
        fdPsynergyFadeFromCastColor();
    }
    syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

#undef STRONG_SWAY_PEAK
#undef STRONG_SWAY_RAMP
#undef STRONG_SWAY_HOLD_TICKS
#undef STRONG_SHAKE_PIXELS_PER_TURN

// Tremor as fdPsynergyRun casts it, at no object: the view sways.

// The sway's hold while nearby objects shake with it, in place of SWAY_HOLD_TICKS.
#define SWAY_SHAKE_TICKS      50
// The objects shake as far as a whole turn for every SHAKE_PIXELS_PER_TURN pixels the view sways.
#define SHAKE_PIXELS_PER_TURN 384

// Sways the view from side to side, with a sound every 16 ticks: the swing grows from SWAY_START to
// SWAY_PEAK, holds, shrinks back and stops. On a town or dungeon map the nearby objects' sprites
// shake with it; when there are any, the hold lasts SWAY_SHAKE_TICKS instead of SWAY_HOLD_TICKS.
// Then the view bounds and the sprites come back.
void fdPsynergyTremor(void)
{
    // Tremor's state's size, though the sway holds only the view move.
    VIEW_MOVE_STATE *viewMoveState =
        syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, sizeof(TREMOR_STATE));
    FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
    FIELD_STATE        *field = RamHeapSlots->fieldState;
    Sint32              frameCount;
    Sint32              phase;
    Sint32              i;
    Sint32              amplitude;
    Sint32              state;
    Sint32              ticks;

    fdPsynergyFadeToCastColor();
    fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
    fdViewMoveStart(VIEW_MOVE_X);
    fdViewMoveSaveNearbySprites();
    sySynch(10);

    phase      = 0;
    frameCount = 0;
    amplitude  = SWAY_START;
    state      = SWAY_GROW;
    ticks      = 0;
    do
    {
        if ((frameCount & 15) == 15)
            sySoundPlayStub(SE_JUMP);

        // Shake the gathered objects' sprites with the view.
        if (field->mode == ACTOR_SYSTEM_MAP)
        {
            for (i = 0; i < viewMoveState->count; i++)
            {
                SPR   *sprite   = viewMoveState->actors[i]->sprite;
                Sint32 rotation = viewMoveState->spriteRotations[i];

                if (amplitude > 0)
                    sprite->rotation =
                        rotation
                        + syMulFX(sySin(phase * SHAKE_STEP), amplitude / SHAKE_PIXELS_PER_TURN);
                else
                    sprite->rotation = rotation;
            }
        }

        fdViewMoveSwing(VIEW_MOVE_X, phase++, amplitude);
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
                if (viewMoveState->count != 0)
                {
                    if (ticks == SWAY_SHAKE_TICKS)
                    {
                        state = SWAY_SHRINK;
                        ticks = -1;
                    }
                }
                else if (ticks == SWAY_HOLD_TICKS)
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
        frameCount++;
    } while (state != SWAY_DONE);

    fdViewMoveEnd();
    fdViewMoveRestoreNearbySprites();
    fdPsynergyFadeFromCastColor();
    syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

// Moves rows 1 to rows - 2 of a buffer down one row by DMA, dropping the last, then clears row 0
// (stride bytes per row).
void fdBattleEffectShiftRowsDown(Uint8 *base, Sint32 stride, Sint32 rows)
{
    Uint8 *src  = base + (rows - 2) * stride;
    Uint8 *dest = base + (rows - 1) * stride;
    Sint32 i;

    for (i = rows - 1; i != 1; i--)
    {
        syDMAXCopy(src, dest, DMA_COPY_WORDS(stride / 4), REG_DMA3);
        src  -= stride;
        dest -= stride;
    }
    syWordsFillEntry(base, stride, 0);
}

#undef SHAKE_SPRITE_COUNT
#undef SHAKE_SPRITE_PALETTE
#undef PICTURE_SIZE
#undef PICTURE_MAX_PIXELS
#undef SWAY_SHAKE
#undef SWAY_SHAKE_TICKS
#undef SHAKE_STEP
#undef SHAKE_PIXELS_PER_TURN
