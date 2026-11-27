// The summons that spend one Djinni: Mercury, Jupiter, Venus and Mars. Each shows its Djinni washed
// bright, Mercury's white and the others in their element's colour, while what it throws bursts
// over every target. Part of battle_effect.c.

// Copies a byte picture turned 90 degrees clockwise.
void btEffectTransposeCopy(const Uint8 *src, Uint8 *dest, Sint32 w, Sint32 h)
{
    Sint32 x;
    Sint32 y;

    for (y = 0; y != h; y++)
    {
        for (x = 0; x != w; x++)
            dest[x * h + (h - 1 - y)] = src[y * w + x];
    }
}

// The tick each Djinni's attack shows its damage.
#define DJINN_ATTACK_DAMAGE_TICK 56

#define MERCURY_TICKS            96
// The blue flame sprite sheet's flame, after its shaft; Mercury turns both on their side, and Venus
// draws the flame as its rising columns.
#define BLUE_FLAME_SHEET_FLAME   PICTURE_BYTES(BLUE_FLAME_COLUMN)

// Mercury's sprite sheet: its Djinni, then the blue flame sprite sheet's two pictures turned on
// their side.
typedef enum MERCURY_SHEET_PICTURE
{
    // The shaft turned; never drawn.
    MERCURY_TURNED_SHAFT = SMALL_DJINNI_WIDTH * MERCURY_SMALL_DJINNI_HEIGHT,
    // The flame turned to fly left.
    MERCURY_TURNED_FLAME = MERCURY_TURNED_SHAFT + BLUE_FLAME_COLUMN_HEIGHT * BLUE_FLAME_COLUMN_WIDTH
} MERCURY_SHEET_PICTURE;

#define MERCURY_FLAMES                  5 // flames thrown
// The particle slots Mercury's flames throw their motes into.
#define MERCURY_MOTE_SLOTS              256
// In each summon what bursts throws its motes into its own slots of PARTICLES; a mote falls, slows
// and shrinks a size for each DJINN_ATTACKS_MOTE_SHRINK_TICKS of its life it loses.
#define DJINN_ATTACKS_MOTES_PER_FLAME   32 // slots for each flame's motes
#define DJINN_ATTACKS_MOTE_SLOTS        512
#define DJINN_ATTACKS_MOTE_SHRINK_TICKS 16

// The Mercury Djinni rises from below on the caster's side and whirls there, washed white, firing
// five blue flames to the left; trailing like comets, each bursts into a spray of motes over the
// targets and hits them all.
//
// Abilities: Mercury.
void btEffectRunMercury(BATTLE_EFFECT_PARM *args)
{
    HEAP_SLOTS          *heap;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    Uint8               *sheet;
    BLITTER_PAIR         blit;
    PARTICLE            *flame;
    PARTICLE            *mote;
    Uint8               *cells;
    Sint32               i;
    Sint32               j;

    heap   = RamHeapSlots;
    state  = (BATTLE_EFFECT_STATE *)heap->battleEffect;
    canvas = heap->battleCanvas;
    sheet  = heap->battleSheet;
    btEffectStartCanvasLayer(0);
    btEffectBuildCanvasBlitters(0, blit);

    cells = RamMapMetatileBuffer;
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET, state->sheet, 1, 0);
    btResourceLoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, cells, 1, 1);
    btEffectTransposeCopy(cells,
                          state->sheet + MERCURY_TURNED_SHAFT,
                          BLUE_FLAME_COLUMN_WIDTH,
                          BLUE_FLAME_COLUMN_HEIGHT);
    btEffectTransposeCopy(cells + BLUE_FLAME_SHEET_FLAME,
                          state->sheet + MERCURY_TURNED_FLAME,
                          BLUE_FLAME_SPLASH_WIDTH,
                          BLUE_FLAME_SPLASH_HEIGHT);
    // The canvas fades by a quarter each tick, so each flame trails like a comet.
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    EFFECT_FREE_PARTICLES(i, state->particles, 8);
    EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
    sySoundPlayStub(SE_WAVE);
    for (frame = 0; frame != MERCURY_TICKS; frame++)
    {
        if (frame == DJINN_ATTACK_DAMAGE_TICK)
            EFFECT_SHOW_DAMAGE(SE_IMPACT);

        // Flames fly left and burst on their 5th tick.
        for (i = 0; i != MERCURY_FLAMES; i++)
        {
            flame = &state->particles[i];
            if (flame->variant != -1)
            {
                blit[0](canvas,
                        state->sheet + MERCURY_TURNED_FLAME,
                        flame->x - 16,
                        flame->y - BLUE_FLAME_SPLASH_WIDTH / 2,
                        BLUE_FLAME_SPLASH_HEIGHT,
                        BLUE_FLAME_SPLASH_WIDTH);
                flame->x -= 12;
                flame->variant++;
                if (flame->variant == 5)
                {
                    sySoundPlayStub(SE_IMPACT);
                    state->shakeFrame = 4;
                    for (j = 0; j != DJINN_ATTACKS_MOTES_PER_FLAME; j++)
                    {
                        Sint32 angle;
                        Sint32 speed;

                        mote            = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
                        angle           = syRandom() & TRIG_ANGLE_MASK;
                        speed           = (syRandom() & 0x1ff) + 256;
                        mote->x         = toFIXED(flame->x);
                        mote->y         = toFIXED(flame->y);
                        mote->velocityX = (sySin(angle) * speed) >> 8;
                        mote->velocityY = (syCos(angle) * speed) >> 7;
                        // lives 32 to 47 ticks
                        mote->variant   = (syRandom() & 15) + 32;
                    }
                }
            }
        }

        // DEAD: always true, as the loop ends at MERCURY_TICKS; the ROM keeps the compare.
        if (frame <= MERCURY_TICKS - 1)
        {
            Sint32 x = (FIXEDtoINT(sySin(frame * (TRIG_TURN / 32)) * (64 - frame * 2)) >> 1) + 96;
            Sint32 y = FIXEDtoINT(syCos(frame * (TRIG_TURN / 32)) * (64 - frame * 2)) + 60;

            blit[1](canvas,
                    state->sheet,
                    x - SMALL_DJINNI_WIDTH / 2,
                    y - MERCURY_SMALL_DJINNI_HEIGHT / 2,
                    SMALL_DJINNI_WIDTH,
                    MERCURY_SMALL_DJINNI_HEIGHT);
            for (i = 0; i != MERCURY_FLAMES; i++)
            {
                if (frame == DjinnAttacksLaunchTicks[i])
                {
                    PARTICLE *shot = &state->particles[i];

                    if (shot->variant == -1)
                    {
                        shot->x         = x - 8;
                        // DEAD: velocityX keeps the launch x, which nothing reads; the ROM keeps
                        // the store.
                        shot->velocityX = x;
                        shot->y         = y;
                        shot->variant   = 0;
                        break;
                    }
                }
                if (frame == DjinnAttacksLaunchTicks[i] + 6)
                {
                    EFFECT_FOR_EACH_TARGET(j, args)
                    {
                        EFFECT_STRIKE_TARGET(args, j, 6, KNOCKBACK_BOUNCE);
                    }
                }
            }
        }

        for (i = 0; i != MERCURY_MOTE_SLOTS; i++)
        {
            mote = &PARTICLES[i];
            if (mote->variant != -1)
            {
                Sint32 size = mote->variant / DJINN_ATTACKS_MOTE_SHRINK_TICKS + 2;

                EFFECT_DRAW_MOTE(blit[1],
                                 canvas,
                                 sheet,
                                 FIXED_INTEGER_PART(mote->x),
                                 FIXED_INTEGER_PART(mote->y),
                                 size);
                btParticleAdvanceWithGravity2D(mote, 62, toFIXED(1) / 8);
                mote->variant--;
            }
        }
        btCameraShake(4, 4);
        EFFECT_END_TICK(state);
    }
    EFFECT_FINISH_CANVAS();
}

#define JUPITER_TICKS           96
#define JUPITER_FLAMEBALLS      8 // flameballs that fall
// The flameballs given a place to start from, of which only JUPITER_FLAMEBALLS fall.
#define JUPITER_FLAMEBALL_SLOTS 32
#define JUPITER_BURST_Y         95 // a flameball bursts once below this line
#define JUPITER_FLAMEBALL       PICTURE_BYTES(SMALL_DJINNI) // after the Djinni

// The Jupiter Djinni whirls by the caster, trailing pink, while eight pink flameballs fall from the
// top right onto the targets, 4 ticks apart; each bursts into 32 motes and hits every target.
//
// Abilities: Jupiter.
void btEffectRunJupiter(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    Uint8               *sheet;
    BLITTER_PAIR         blit;
    PARTICLE            *ball;
    PARTICLE            *mote;
    Sint32               i;
    Sint32               j;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    sheet  = RamHeapSlots->battleSheet;
    btEffectStartCanvasLayer(0);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    btEffectBuildCanvasBlitters(0, blit);
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET, state->sheet, 1, 1);
    btResourceLoadAndDecompress(RESOURCE_FLAMEBALL_SHEET, state->sheet + JUPITER_FLAMEBALL, 1, 0);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

    for (i = 0; i != JUPITER_FLAMEBALL_SLOTS; i++)
    {
        ball    = &state->particles[i];
        ball->x = (syRandom() & 63) + 64;
        ball->y = (syRandom() & 63) - 80;
    }
    EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
    sySoundPlayStub(SE_THUNDER);
    frame = 0;
    {
        Sint32 angle = DEGtoANG(180);

        for (; frame != JUPITER_TICKS; frame++)
        {
            if (frame == DJINN_ATTACK_DAMAGE_TICK)
                EFFECT_SHOW_DAMAGE(SE_IMPACT);

            // DEAD: always true, as the loop ends at JUPITER_TICKS; the ROM keeps the compare.
            if (frame <= JUPITER_TICKS - 1)
            {
                Sint32 x = FIXEDtoINT(sySin(angle) * (64 - frame * 2)) >> 1;
                Sint32 y = FIXEDtoINT(syCos(angle) * (64 - frame * 2));

                blit[0](
                    canvas, state->sheet, x + 86, y + 28, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
            }

            // Flameballs fall from the top right, one every 4 ticks.
            for (i = 0; i != JUPITER_FLAMEBALLS; i++)
            {
                ball = &state->particles[i];
                if (frame >= i * 4 + 8 && ball->y <= JUPITER_BURST_Y)
                {
                    blit[0](canvas,
                            state->sheet + JUPITER_FLAMEBALL,
                            ball->x - FLAMEBALL_WIDTH / 2,
                            ball->y - FLAMEBALL_HEIGHT / 2,
                            FLAMEBALL_WIDTH,
                            FLAMEBALL_HEIGHT);
                    ball->x -= 6;
                    ball->y += 12;
                    if (ball->y > JUPITER_BURST_Y)
                    {
                        for (j = 0; j != DJINN_ATTACKS_MOTES_PER_FLAME; j++)
                        {
                            Sint32 direction;
                            Sint32 speed;

                            mote            = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
                            direction       = syRandom() & TRIG_ANGLE_MASK;
                            speed           = (syRandom() & 0x1ff) + 256;
                            mote->x         = toFIXED(ball->x);
                            mote->y         = toFIXED(ball->y);
                            mote->velocityX = (sySin(direction) * speed) >> 7;
                            mote->velocityY = (syCos(direction) * speed) >> 6;
                            // lives 32 to 47 ticks
                            mote->variant   = (syRandom() & 15) + 32;
                        }
                        sySoundPlayStub(SE_IMPACT);
                        state->shakeFrame = 4;
                        EFFECT_FOR_EACH_TARGET(j, args)
                        {
                            EFFECT_STRIKE_TARGET(args, j, 6, KNOCKBACK_BOUNCE);
                        }
                    }
                }
            }

            for (i = 0; i != DJINN_ATTACKS_MOTE_SLOTS; i++)
            {
                mote = &PARTICLES[i];
                if (mote->variant != -1)
                {
                    Sint32 size = mote->variant / DJINN_ATTACKS_MOTE_SHRINK_TICKS + 1;

                    EFFECT_DRAW_MOTE(blit[1],
                                     canvas,
                                     sheet,
                                     FIXED_INTEGER_PART(mote->x),
                                     FIXED_INTEGER_PART(mote->y),
                                     size);
                    btParticleAdvanceWithGravity2D(mote, 62, toFIXED(1) / 8);
                    mote->variant--;
                }
            }
            btCameraShake(4, 4);
            EFFECT_END_TICK(state);
            angle -= TRIG_TURN / 32;
        }
    }
    EFFECT_FINISH_CANVAS();
}

// Decompresses a resource that may open with a palette.
void btResourceLoadAndDecompress(Sint32 id, void *dest, Sint32 skipPalette, Sint32 copyPalette)
{
    Uint8 *resource = syResourceGetTableEntry(id);

    if (copyPalette != 0)
        syWordsCopy((void *)BG_PLTT, resource, SHEET_PALETTE_BYTES);
    if (skipPalette != 0)
        resource += SHEET_PALETTE_BYTES;
    syDecompressTaggedLZ(resource, dest);
}

#define VENUS_TICKS             96
#define VENUS_COLUMNS           10 // columns of flame that rise
#define VENUS_FIRST_COLUMN_TICK 16 // the tick the first column rises
// The columns given a place to rise from, of which only VENUS_COLUMNS rise.
#define VENUS_COLUMN_SLOTS      32
#define VENUS_MOTES_PER_COLUMN  16
// How far the Djinni sways to either side as it drops.
#define VENUS_DJINNI_SWAY       24

// Venus's sprite sheet: its Djinni, then the blue flame sprite sheet.
typedef enum VENUS_SHEET_PICTURE
{
    VENUS_BLUE_FLAME_SHEET = SMALL_DJINNI_WIDTH * VENUS_SMALL_DJINNI_HEIGHT,
    // 34x65, the flame, drawn upside down as a rising column.
    VENUS_COLUMN           = VENUS_BLUE_FLAME_SHEET + BLUE_FLAME_SHEET_FLAME
} VENUS_SHEET_PICTURE;

// The Venus Djinni drops in from above the targets, swaying and trailing yellow, while ten columns
// of yellow fire shoot up from the ground beneath them one after another, each bursting into motes
// and hitting every target.
//
// Abilities: Venus.
void btEffectRunVenus(BATTLE_EFFECT_PARM *args)
{
    HEAP_SLOTS          *heap;
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Sint32               frame;
    BLITTER_PAIR         draw;
    Uint8               *sheet;
    PARTICLE            *column;
    PARTICLE            *mote;
    Sint32               i;
    Sint32               j;
    Sint32               start;
    Sint32               angle;

    heap   = RamHeapSlots;
    state  = (BATTLE_EFFECT_STATE *)heap->battleEffect;
    canvas = heap->battleCanvas;
    sheet  = heap->battleSheet;
    btEffectStartCanvasLayer(0);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
    draw[0] = heap->ramCode;
    draw[1] = heap->ramCodePaired;
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET, state->sheet, 1, 1);
    btResourceLoadAndDecompress(
        RESOURCE_BLUE_FLAME_SHEET, state->sheet + VENUS_BLUE_FLAME_SHEET, 1, 0);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
    for (i = 0; i != VENUS_COLUMN_SLOTS; i++)
    {
        column    = &state->particles[i];
        column->x = syRandom() & 63;
        column->y = 104;
    }
    EFFECT_FREE_PARTICLES(i, PARTICLES, DJINN_ATTACKS_MOTE_SLOTS);
    sySoundPlayStub(SE_RUMBLE);
    frame = 0;
    angle = DEGtoANG(180);
    for (; frame != VENUS_TICKS; frame++)
    {
        // The Djinni sways from side to side as it drops.
        if (frame <= 79)
        {
            Sint32 x = FIXEDtoINT(sySin(angle) * VENUS_DJINNI_SWAY);
            Sint32 y = FIXEDtoINT(syCos(angle) * (64 - frame * 2));

            draw[1](canvas,
                    state->sheet,
                    x + 22,
                    y + 29,
                    SMALL_DJINNI_WIDTH,
                    VENUS_SMALL_DJINNI_HEIGHT);
        }
        if (frame == DJINN_ATTACK_DAMAGE_TICK)
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
        // Ten flame columns rise, one every 4 ticks.
        for (i = 0, start = VENUS_FIRST_COLUMN_TICK; i != VENUS_COLUMNS; start += 4, i++)
        {
            column = &state->particles[i];
            if (frame >= start)
            {
                draw[0](canvas,
                        state->sheet + VENUS_COLUMN,
                        column->x - BLUE_FLAME_SPLASH_WIDTH / 2,
                        column->y - BLUE_FLAME_SPLASH_HEIGHT / 2,
                        BLUE_FLAME_SPLASH_WIDTH,
                        BLUE_FLAME_SPLASH_HEIGHT);
                if (frame == start)
                {
                    for (j = 0; j != VENUS_MOTES_PER_COLUMN; j++)
                    {
                        Sint32 direction;
                        Sint32 speed;

                        mote            = &PARTICLES[i * DJINN_ATTACKS_MOTES_PER_FLAME + j];
                        direction       = (syRandom() & 0x7fff) + DEGtoANG(90);
                        speed           = (syRandom() & 0x1ff) + 256;
                        mote->x         = toFIXED(column->x);
                        mote->y         = INTtoFIXED(column->y + 16);
                        mote->velocityX = (sySin(direction) * speed) >> 7;
                        mote->velocityY = (syCos(direction) * speed) >> 6;
                        mote->variant   = (syRandom() & 15) + 32;
                    }
                    if (i & 1)
                        sySoundPlayStub(SE_IMPACT);
                    state->shakeFrame = 4;
                    EFFECT_FOR_EACH_TARGET(j, args)
                    {
                        EFFECT_STRIKE_TARGET(args, j, 6, KNOCKBACK_BOUNCE);
                    }
                }
                column->y -= 12;
            }
        }
        for (i = 0; i != DJINN_ATTACKS_MOTE_SLOTS; i++)
        {
            mote = &PARTICLES[i];
            if (mote->variant != -1)
            {
                Sint32 size = mote->variant / DJINN_ATTACKS_MOTE_SHRINK_TICKS + 2;

                EFFECT_DRAW_MOTE(draw[1],
                                 canvas,
                                 sheet,
                                 FIXED_INTEGER_PART(mote->x),
                                 FIXED_INTEGER_PART(mote->y),
                                 size);
                btParticleAdvanceWithGravity2D(mote, 62, toFIXED(1) / 8);
                mote->variant--;
            }
        }
        btCameraShake(4, 4);
        EFFECT_END_TICK(state);
        angle -= TRIG_TURN / 32;
    }
    EFFECT_FINISH_CANVAS();
}

#define MARS_TICKS            112
#define MARS_FLAMES           9 // flames along the arc
// The embers each flame throws off.
#define MARS_EMBERS_PER_FLAME 16
// The row of flames: from the canvas's x MARS_ROW_X out to MARS_ROW_REACH to either side, along
// line MARS_ROW_Y bowed four pixels.
#define MARS_ROW_X            32
#define MARS_ROW_Y            52
#define MARS_ROW_REACH        24
// The ticks a flame burns: eight to each of its six pictures.
#define MARS_FLAME_LIFE       48
#define MARS_GLOW_ORB         PICTURE_BYTES(SMALL_DJINNI) // six pictures, after the Djinni

// A fireball blooms high over the targets and burns out, the Mars Djinni appears where it was and
// dives onto them, nine flames erupt in a row along the ground beneath them and embers stream up.
//
// Abilities: Mars.
void btEffectRunMars(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    Uint8               *sheet;
    BLITTER_PAIR         blitters;
    PARTICLE            *particle;
    PARTICLE            *mote;
    Sint32               index;
    Sint32               frame;
    Sint32               i;

    state  = RamHeapSlots->battleEffect;
    canvas = RamHeapSlots->battleCanvas;
    sheet  = RamHeapSlots->battleSheet;
    btEffectStartCanvasLayer(0);
    btEffectBuildCanvasBlitters(0, blitters);
    btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
    btResourceLoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET, state->sheet, 1, 0);
    btResourceLoadAndDecompress(RESOURCE_GLOW_ORB_SHEET, state->sheet + MARS_GLOW_ORB, 1, 1);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

    // Nine flames spread out along the ground, alternately left and right.
    index    = 0;
    particle = state->particles;
    for (; index != MARS_FLAMES; index++)
    {
        Sint32 angle = index * (TRIG_TURN / 32);

        particle->x  = FIXEDtoINT(sySin(angle) * MARS_ROW_REACH);
        particle->y  = FIXEDtoINT(syCos(angle) * 4) + MARS_ROW_Y;
        if (index & 1)
            particle->x = MARS_ROW_X - particle->x;
        else
            particle->x += MARS_ROW_X;
        particle->variant = -(index * 2);
        for (i = 0; i != MARS_EMBERS_PER_FLAME; i++)
        {
            mote            = &PARTICLES[index * MARS_EMBERS_PER_FLAME + i];
            mote->x         = toFIXED(((syRandom() & 15) + particle->x) - 8);
            mote->y         = INTtoFIXED((syRandom() & 7) + 96);
            mote->velocityX = ((syRandom() & 127) - 64) << 11;
            mote->velocityY = ((syRandom() & 127) - 64) << 10;
            mote->z         = syRandom() & TRIG_ANGLE_MASK;
            // DEAD: nothing reads velocityZ; the ROM keeps the store.
            mote->velocityZ = syRandom() & TRIG_ANGLE_MASK;
        }
        particle++;
    }

    sySoundPlayStub(SE_WHOOSH);
    for (frame = 0; frame != MARS_TICKS; frame++)
    {
        if (frame == DJINN_ATTACK_DAMAGE_TICK)
            EFFECT_SHOW_DAMAGE(SE_IMPACT);
        // A glowing orb flares, then the Djinni shows in it and dives down to the left.
        if (frame <= 23)
            blitters[0](canvas,
                        state->sheet + MARS_GLOW_ORB + (frame / 4) * PICTURE_BYTES(GLOW_ORB),
                        40,
                        20,
                        GLOW_ORB_WIDTH,
                        GLOW_ORB_HEIGHT);
        if (frame == 20)
            syWordsCopy((void *)BG_PLTT,
                        syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
                        SHEET_PALETTE_BYTES);
        if (frame >= 20 && frame <= 31)
        {
            if (frame > 23)
                blitters[1](canvas,
                            state->sheet,
                            146 - frame * 4,
                            frame * 8 - 172,
                            SMALL_DJINNI_WIDTH,
                            SMALL_DJINNI_HEIGHT);
            else
                blitters[0](canvas, state->sheet, 50, 20, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        }
        // It strikes: the screen shakes and the flames erupt.
        if (frame == 32)
        {
            sySoundPlayStub(SE_CRASH);
            state->shakeFrame = 8;
            btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
        }
        if (frame > 31)
        {
            for (index = 0; index != MARS_FLAMES; index++)
            {
                particle = &state->particles[index];
                if (particle->variant >= 0 && particle->variant < MARS_FLAME_LIFE)
                {
                    Sint32 picture = particle->variant / 8;
                    Uint32 width;

                    // UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
                    width = BattleEffectEmberStreakPictureWidths[picture];
                    blitters[0](canvas,
                                state->sheet + BattleEffectEmberStreakPictures[picture],
                                particle->x - (width >> 1),
                                particle->y + BattleEffectEmberStreakPictureBiasY[picture],
                                width,
                                BattleEffectEmberStreakPictureHeights[picture]);
#else
                    blitters[0](
                        canvas,
                        state->sheet + BattleEffectEmberStreakPictures[picture],
                        particle->x
                            - ((width = BattleEffectEmberStreakPictureWidths[picture]) >> 1),
                        particle->y + BattleEffectEmberStreakPictureBiasY[picture],
                        width,
                        BattleEffectEmberStreakPictureHeights[picture]);
#endif
                }
                particle->variant++;
            }
        }
        // Embers rise from the flames, swaying.
        for (index = 0; index != MARS_FLAMES * MARS_EMBERS_PER_FLAME; index++)
        {
            particle = &PARTICLES[index];
            if (frame >= (index / MARS_EMBERS_PER_FLAME) * 2 + 40)
            {
                Sint32 size;
                Sint32 x;

                size = (index & 1) + 3;
                x    = FIXED_INTEGER_PART(particle->x) + FIXEDtoINT(sySin(particle->z) * 4);
                EFFECT_DRAW_MOTE(
                    blitters[1], canvas, sheet, x, FIXED_INTEGER_PART(particle->y), size);
                btParticleAdvanceWithGravity2D(particle, PARTICLE_DAMPING_NONE, -toFIXED(1) / 8);
                particle->z += TRIG_TURN / 32;
                if (particle->z > TRIG_ANGLE_MASK)
                    particle->z -= TRIG_TURN - 1;
            }
        }

        if (frame == 38)
        {
            EFFECT_FOR_EACH_TARGET(index, args)
            {
                EFFECT_STRIKE_TARGET(args, index, 16, KNOCKBACK_BOUNCE);
            }
        }
        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
    }
    EFFECT_FINISH_CANVAS();
}

#undef MERCURY_TICKS
#undef MERCURY_FLAMES
#undef DJINN_ATTACKS_MOTES_PER_FLAME
#undef DJINN_ATTACKS_MOTE_SLOTS
#undef DJINN_ATTACKS_MOTE_SHRINK_TICKS
#undef JUPITER_FLAMEBALL_SLOTS
#undef VENUS_COLUMN_SLOTS
#undef VENUS_MOTES_PER_COLUMN
#undef VENUS_DJINNI_SWAY
#undef MARS_ROW_X
#undef MARS_ROW_Y
#undef MARS_ROW_REACH
#undef MARS_FLAME_LIFE
#undef JUPITER_TICKS
#undef JUPITER_FLAMEBALLS
#undef JUPITER_BURST_Y
#undef VENUS_TICKS
#undef VENUS_COLUMNS
#undef VENUS_FIRST_COLUMN_TICK
#undef MARS_TICKS
#undef MARS_FLAMES
#undef MARS_EMBERS_PER_FLAME
#undef BLUE_FLAME_SHEET_FLAME
#undef JUPITER_FLAMEBALL
#undef MARS_GLOW_ORB
#undef MERCURY_MOTE_SLOTS
