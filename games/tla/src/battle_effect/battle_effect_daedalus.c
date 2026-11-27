// The mirrored wall of Berserk, Spasm, Recovery and Sack, and the summon Daedalus with its second
// strike. Part of battle_effect.c.

#define MIRRORED_WALL_TICKS      48 // ticks of the wall
// ticks the wall's pictures are drawn
#define MIRRORED_WALL_DRAW_TICKS 28

// The wall's three 64x64 pictures, each a half wall and its mirror image, built at the start of the
// sprite sheet; EarthWallSheet decodes behind them, at PICTURE_OFFSET.
#define PICTURE_SLOT_SIZE        4096
#define PICTURE_OFFSET           16384

// EarthWallSheet's three half walls of flame, each taller than the last (earth_wall_11.png to
// _13.png): where each begins in the decoded sprite sheet, and its size. The Broken Seal's mirrored
// wall draws the same three pictures.
#define EARTH_WALL_LOW           2702
#define EARTH_WALL_LOW_WIDTH     17
#define EARTH_WALL_LOW_HEIGHT    49
#define EARTH_WALL_MIDDLE        3535
#define EARTH_WALL_MIDDLE_WIDTH  19
#define EARTH_WALL_MIDDLE_HEIGHT 55
#define EARTH_WALL_HIGH          4580
#define EARTH_WALL_HIGH_WIDTH    20
#define EARTH_WALL_HIGH_HEIGHT   64

// Copies half wall `half` into the bottom middle of a 64x64 picture, once as it is and once
// mirrored, so the picture is a whole wall.
#define MIRROR_HALF(picture, half)                                                                 \
    for (y = 0; y != half##_HEIGHT; y++)                                                           \
    {                                                                                              \
        for (x = 0; x != half##_WIDTH; x++)                                                        \
        {                                                                                          \
            state->sheet[(picture) * PICTURE_SLOT_SIZE + (y + 64 - half##_HEIGHT) * 64 + 32        \
                         - half##_WIDTH + x] =                                                     \
                state->sheet[PICTURE_OFFSET + half + y * half##_WIDTH + x];                        \
            state->sheet[(picture) * PICTURE_SLOT_SIZE + (y + 64 - half##_HEIGHT) * 64 + 31        \
                         + half##_WIDTH - x] =                                                     \
                state->sheet[PICTURE_OFFSET + half + y * half##_WIDTH + x];                        \
        }                                                                                          \
    }

// Raises a mirrored wall at the first target's feet while the target glows.
void btEffectRunMirroredWall(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
    BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEffect;
    MODEL_TEXTURE        texture;
    SCREEN_POINT         target;
    RENDER_MODEL        *quad;
    void                *vertices;
    Sint32               frame;
    Sint32               picture;
    Sint32               y;
    Sint32               x;

    // Build the three pictures from their halves.
    btEffectStartCanvasLayer(0);
    btResourceLoadAndDecompress(RESOURCE_EARTH_WALL_SHEET, state->sheet + PICTURE_OFFSET, 1, 1);
    syWordsClear(state->sheet, PICTURE_OFFSET);
    MIRROR_HALF(0, EARTH_WALL_LOW)
    MIRROR_HALF(1, EARTH_WALL_MIDDLE)
    MIRROR_HALF(2, EARTH_WALL_HIGH)
    if (mode == MIRRORED_WALL_SACK)
        syWordsCopy((void *)BG_PLTT,
                    syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
    else if (mode == MIRRORED_WALL_RECOVERY)
        syWordsCopy(
            (void *)BG_PLTT, syResourceGetTableEntry(RESOURCE_FIREBALL_SHEET), SHEET_PALETTE_BYTES);
    else
        syWordsCopy((void *)BG_PLTT,
                    syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                    SHEET_PALETTE_BYTES);

    // Centre the canvas on the target.
    btEffectPosProjectBattler(args->targets[0], &target);
    REG_BG2PA = Q_8_8(1);
    REG_BG2X  = (CANVAS_SIZE / 2 - target.x) << 8;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    sySetEvent((Sint32)btEffectTransferCanvas, EVENT_PASS_VBLANK);
    if (mode == MIRRORED_WALL_RECOVERY)
    {
        state->shakeFrame = 0;
        sySoundPlayStub(SE_POWER_UP);
    }
    else if (mode == MIRRORED_WALL_BERSERK || mode == MIRRORED_WALL_SACK)
    {
        state->shakeFrame = 8;
        sySoundPlayStub(SE_POWER_UP);
    }
    else
    {
        state->shakeFrame = 32;
    }

    for (frame = 0; frame != MIRRORED_WALL_TICKS; frame++)
    {
        if (frame == 0)
        {
            if (mode == MIRRORED_WALL_RECOVERY)
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 32);
            else
                btEffectTintBattler(args->targets[0], SPRITE_COLOR_RED_TINT, TINT_KEEP, 0, 32);
        }
        if (frame == 24)
            EFFECT_SHOW_DAMAGE(0);
        if (frame == 8 && mode == MIRRORED_WALL_SPASM)
            sySoundPlayStub(SE_RECOVERY);

        // Pictures 0, 1 and 2 four ticks each, then 1 and 2 by turns.
        if (frame < 32)
        {
            picture = frame / 4;
            if (picture > 2)
                picture = (picture & 1) + 1;
            if (frame < MIRRORED_WALL_DRAW_TICKS)
            {
                vertices          = syHeapAllocateEWRAM(32);
                quad              = btRenderAllocateModelList(1);
                texture.logWidth  = 6;
                texture.logHeight = 6;
                texture.pixels    = state->sheet + picture * PICTURE_SLOT_SIZE;
                quad->kind        = MODEL_SHADED_MAX;
                quad->mesh        = BattleEffectSquareMesh;
                quad->texture     = &texture;
                quad->shade       = MirroredWallShades[mode];
                quad->vertices    = vertices;
                syInitMatrix();
                syTranslate(0, INTtoFIXED(target.y - 58), 0);
                syMatrixScaleUniform(MirroredWallScales[mode] << 1);
                btRenderTransformVertices(BattleEffectStandingSquareCorners, vertices, 4);
                btRenderDrawModelList(quad);
                syHeapRelease(quad);
                syHeapRelease(vertices);
            }
        }

        if (mode == MIRRORED_WALL_SPASM)
            btCameraShake(4, 4);
        else
            btCameraShake(16, 16);
        EFFECT_END_TICK(state);
    }

    syCloseEvent((Sint32)btEffectTransferCanvas);
    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
    btEffectEndCanvasLayer();
}

// Packs YellowOrbSheet's six pictures to half width, keeping every second byte, the odd column's.
// stride: the source's bytes a row; the packed rows are half as long.
void btEffectPackYellowOrbs(Uint8 *src, Uint8 *dest, Sint32 stride)
{
    Sint32 row    = 0;
    Sint32 rows   = YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT;
    Sint32 offset = 0;

    for (; row != rows; row++)
    {
        Sint32 x;

        for (x = 0; x != YELLOW_ORB_WIDTH; x++)
        {
            dest[offset / 2 + x / 2] = src[offset + x];
        }
        offset += stride;
    }
}

// A sprite (SPR) as Daedalus reads it: its attribute 2 fields, scale, VRAM cache slot, rotation,
// size and drawing offset.
typedef struct DAEDALUS_SPR
{
    // The part's link and its attributes 0 and 1, which the effect leaves alone.
    Uint8  linkAndPos[8];
    Uint16 tile     : 10;
    Uint16 priority : 2;
    Uint16 palette  : 4;
    Uint8  unusedAfterPalette[2];
    FIXED  scale;
    Uint8  slot;
    Uint8  cacheFlags;
    Uint16 rotation;
    Uint8  size[2];
    Uint8  offsetX;
    Uint8  offsetY;
} DAEDALUS_SPR;

// The effect state's sprite slot `i`, read as Daedalus's sprite.
#define DAEDALUS_SPRITE(i) ((DAEDALUS_SPR *)state->sprites[i])

// A sprite's first ten bytes, its attribute 2 fields after eight others.
typedef struct SPR_ATTR
{
    Uint8  linkAndPos[8];
    Uint16 tile     : 10;
    Uint16 priority : 2;
    Uint16 palette  : 4;
} SPR_ATTR;

typedef void (*COPY_FUNC)(void *, const void *, Sint32);

// syWordsCopy reached through the linked IWRAM code's symbol, syIWRAMIRQMain, at its offset rather
// than at its fixed address: the same function, called as the trade's snapshot copies and the item
// menu call it.
#define IWRAM_COPY_WORDS_LINKED       ((COPY_FUNC)(syIWRAMIRQMain + syWordsCopyOffset))

// Where the effect's pictures are decoded before they are copied to OBJ VRAM.
#define DECODE_BUFFER                 RamMapMetatileBuffer
// The particles the sky scene, the fall and the second strike's sparks keep in EXTRA_PARTICLES.
#define DAEDALUS_PARTICLE_COUNT       (DAEDALUS_MISSILE_COUNT * DAEDALUS_TRAIL_LENGTH)

// The opening's mote copies: DAEDALUS_MOTE_COPIES copies of YellowGlowSheet's mote DAEDALUS_MOTE, 4
// pixels wide and 8 tall, in the battle sprite sheet's heap slot, the first DAEDALUS_MOTE_DIM_FIRST
// levels dimmer than the mote and each next DAEDALUS_MOTE_DIM_STEP more. Nothing in Daedalus draws
// them.
#define DAEDALUS_MOTE                 3
#define DAEDALUS_MOTE_BYTES           ((DAEDALUS_MOTE + 1) * (DAEDALUS_MOTE + 1) * 2)
#define DAEDALUS_MOTE_COPIES          10
#define DAEDALUS_MOTE_DIM_FIRST       40
#define DAEDALUS_MOTE_DIM_STEP        4

// ForestCanopyTiles: two rows of eight 32x32 pictures of treetops, in OBJ palette CANOPY_PALETTE.
// Each row is drawn by DAEDALUS_CANOPY_ROW sprites, the last showing the picture before the first,
// so the row wraps as it scrolls.
#define CANOPY_PALETTE                15
#define CANOPY_ROWS                   2
#define CANOPY_PICTURES               8
#define CANOPY_PICTURE_SIZE           32
#define CANOPY_ROW_BYTES              (CANOPY_PICTURES * OBJ_32X32_4BPP_BYTES)
// A whole number of rows of pictures, added to a picture's place before the remainder by
// CANOPY_PICTURES takes it away again.
#define CANOPY_WRAP                   (CANOPY_PICTURES * 8)
// The canopy's top in the sky scene. The opening shows it DAEDALUS_CLIMB pixels higher; as the
// camera climbs, the sky scene lowers the canopy by that much and raises the machine twice as far.
#define CANOPY_Y                      132
#define DAEDALUS_CLIMB                29

// RocketFlameSheet: six 12x24 flames, each dimmer than the last.
#define ROCKET_FLAME_WIDTH            12
#define ROCKET_FLAME_HEIGHT           24
#define ROCKET_FLAME_COUNT            6
// BirdFlockSheet's 16x16 birds, and the twelve steps of a wing beat (DaedalusWingBeatPictures).
#define BIRD_WIDTH                    16
#define BIRD_HEIGHT                   16
#define BIRD_WING_BEAT                12

// The sky scene's pictures: the rocket's flames, then the bursts, then the birds.
#define DAEDALUS_FLASH_BURST_PICTURES (ROCKET_FLAME_COUNT * PICTURE_BYTES(ROCKET_FLAME))
#define DAEDALUS_BIRD_FLOCK                                                                        \
    (DAEDALUS_FLASH_BURST_PICTURES + FLASH_BURST_PICTURE_COUNT * PICTURE_BYTES(FLASH_BURST))

// The fall's pictures: the bursts, then the rocket's flames.
#define DAEDALUS_ROCKET_FLAMES      (FLASH_BURST_PICTURE_COUNT * PICTURE_BYTES(FLASH_BURST))

// The sky scene's sprites: the canopy's two rows; the twenty-one 32x32 pieces of the machine
// (BronzeAutomatonTiles), among them its missiles' cover, its four hull plates and its spinning
// blade; the 32x64 rocket it launches; and the missile with fifteen copies. The state's first
// particles keep the canopy pictures' tile numbers in their variants, row by row as the sprites.
#define DAEDALUS_CANOPY_ROW         (CANOPY_PICTURES + 1)
#define DAEDALUS_MACHINE            18
#define DAEDALUS_MACHINE_PIECES     21
// BronzeAutomatonTiles' colours, OBJ palette 0 to 223. The first of them all green is the machine's
// core, which the sky scene recolours as it glows.
#define DAEDALUS_MACHINE_COLORS     224
// The core's colour: dark at first; from DAEDALUS_GLOW_START it brightens a level a tick towards
// orange (red at most, CORE_GLOW_GREEN and CORE_GLOW_BLUE) until DAEDALUS_GLOW_END, then cools
// towards a dark red (CORE_EMBER_RED).
#define CORE_DARK                   RGB(11, 8, 6)
#define CORE_GLOW_GREEN             20
#define CORE_GLOW_BLUE              6
#define CORE_EMBER_RED              20
#define DAEDALUS_GLOW_START         542
#define DAEDALUS_GLOW_END           558

// Until DAEDALUS_SWAY_STOP the machine's legs swing DAEDALUS_SWAY pixels to and fro, opposite ways,
// once every 256 ticks, and it bobs up twice a swing, never below its place. It lurches as its core
// glows and again as it fires, each lurch half a turn of a sine over 16 ticks.
#define DAEDALUS_SWAY               4
#define DAEDALUS_SWAY_STEP          (TRIG_TURN / 256)
#define DAEDALUS_SWAY_STOP          538
#define DAEDALUS_LURCH_STEP         (TRIG_TURN / 32)
#define DAEDALUS_RECOIL_START       560
#define DAEDALUS_RECOIL_END         576
// The tick the machine fires: a ring of fire and a ring of flames spread from it, and two ticks
// later the rocket leaves.
#define DAEDALUS_FIRE_TICK          568

// The rocket: where it rests in the machine and leaves from, how far its nozzle is from its middle,
// the pull of its flight, from which tick of it the camera would let it draw ahead, and its scale,
// a little over one so that it turns in a sprite box twice its size and keeps its fins.
#define DAEDALUS_ROCKET_X           152
#define DAEDALUS_ROCKET_Y           92
#define ROCKET_NOZZLE               27
#define ROCKET_GRAVITY              toFIXED(0.03431)
#define ROCKET_LEAD_START           230
#define ROCKET_SCALE                toFIXED(1.01561)

// The rocket's flight, in ticks from its launch. It sinks a little faster over its first
// ROCKET_SINK_TICKS and speeds forward from ROCKET_THRUST_START. It lifts from ROCKET_LIFT_START to
// ROCKET_LIFT_END, tipping its nose up from ROCKET_NOSE_UP_START to ROCKET_NOSE_UP_END. From
// ROCKET_TURN_START to ROCKET_TURN_END it turns its nose up as it climbs, the nose dipping a moment
// from ROCKET_DIP_START to ROCKET_DIP_END; it climbs more gently until ROCKET_SOAR_START and
// steeply after. Bursts rise from the foot of the view every fourth tick while its nose tips up,
// and again from ROCKET_BURSTS_START to ROCKET_BURSTS_END.
#define ROCKET_THRUST_START         30
#define ROCKET_LIFT_START           40
#define ROCKET_NOSE_UP_START        50
#define ROCKET_NOSE_UP_END          60
#define ROCKET_LIFT_END             68
#define ROCKET_SINK_TICKS           80
#define ROCKET_TURN_START           108
#define ROCKET_BURSTS_START         113
#define ROCKET_DIP_START            120
#define ROCKET_DIP_END              123
#define ROCKET_TURN_END             140
#define ROCKET_SOAR_START           160
#define ROCKET_BURSTS_END           200

// The ring of flames the machine sends out as it fires: FLAME_RING_COUNT flames round a ring
// centred on DaedalusRingX and FLAME_RING_Y, its radius spreading for FLAME_RING_TICKS ticks while
// each flame burns down through RocketFlameSheet's pictures, two ticks each.
#define FLAME_RING_COUNT            32
#define FLAME_RING_STEP             (TRIG_TURN / FLAME_RING_COUNT)
#define FLAME_RING_Y                60
#define FLAME_RING_TICKS            26

// From DAEDALUS_SPLASH_START, flames burst along four lines of six points over the hull
// (DaedalusSplashPoints), a point every two ticks, each burning down through RocketFlameSheet's
// pictures two ticks each; the hull falls away after them.
#define DAEDALUS_SPLASH_START       490

// The sky scene's ticks, unless A or B ends it sooner.
#define DAEDALUS_SKY_TICKS          800

// The fall's ticks, and its runs of EXTRA_PARTICLES: each missile's FALL_BURST_SLOTS, of which its
// two bursts use the first, and from FALL_TRAILS each missile's trail of FALL_TRAIL_LENGTH flames.
#define DAEDALUS_FALL_TICKS         109
#define FALL_BURST_SLOTS            8
#define FALL_TRAILS                 256
#define FALL_TRAIL_LENGTH           16
// The angle a missile's dive turns by for each unit of its velocityZ, a spread the fall leaves at
// 0.
#define FALL_DIVE_SPREAD            40

// The missiles fire in pairs, the first DAEDALUS_VOLLEY_START ticks after their cover flies off and
// the next every DAEDALUS_VOLLEY_STEP ticks.
#define DAEDALUS_VOLLEY_START       10
#define DAEDALUS_VOLLEY_STEP        6

// The machine's pieces, 32 pixels square, by their place in BronzeAutomatonTiles: the two of its
// top; its core; the five of its body behind the hull; the five of the leg on the right, whose
// thigh holds the front missiles behind a cover; the cover; the two of the leg on the left; the
// four hull plates; and the blade.
#define DAEDALUS_TOP                DAEDALUS_MACHINE
#define DAEDALUS_CORE               (DAEDALUS_MACHINE + 2)
#define DAEDALUS_BODY               (DAEDALUS_MACHINE + 3)
#define DAEDALUS_RIGHT_LEG          (DAEDALUS_MACHINE + 8)
#define DAEDALUS_COVER              (DAEDALUS_MACHINE + 13)
#define DAEDALUS_LEFT_LEG           (DAEDALUS_MACHINE + 14)
#define DAEDALUS_HULL               (DAEDALUS_MACHINE + 16)
#define DAEDALUS_BLADE              (DAEDALUS_MACHINE + 20)
#define DAEDALUS_PIECE_SIZE         32
#define DAEDALUS_ROCKET             (DAEDALUS_MACHINE + DAEDALUS_MACHINE_PIECES)
#define DAEDALUS_MISSILE            41
#define DAEDALUS_MISSILE_COPIES     15
#define DAEDALUS_MISSILE_COUNT      (DAEDALUS_MISSILE_COPIES + 1)
#define DAEDALUS_MISSILE_PALETTE    14

// Runs of EXTRA_PARTICLES in the sky scene: each missile's trail of DAEDALUS_TRAIL_LENGTH, the
// flames of the rocket's launch, its exhaust, the bursts that rise from the foot of the view as it
// climbs, and the birds that drift across. The later runs share the last missiles' trails.
#define DAEDALUS_TRAIL_LENGTH       32
#define DAEDALUS_LAUNCH_FLAMES      312
#define DAEDALUS_LAUNCH_FLAME_COUNT 16
#define DAEDALUS_EXHAUST            400
#define DAEDALUS_EXHAUST_FLAMES     32
#define DAEDALUS_BURSTS             432
#define DAEDALUS_BURST_COUNT        32
#define DAEDALUS_BIRDS              502
#define DAEDALUS_BIRD_COUNT         10

// In the sky scene the machine comes apart: the missiles' cover, which flies off and times the
// missiles, the four hull plates, the rocket and the blade each move by a particle of their own, as
// do the spreading ring of flames and the sixteen missiles.
#define DAEDALUS_COVER_MOTION       20
#define DAEDALUS_HULL_MOTION        21
#define DAEDALUS_ROCKET_MOTION      22
#define DAEDALUS_BLADE_MOTION       23
#define DAEDALUS_RING_MOTION        30
#define DAEDALUS_MISSILE_MOTIONS    32

// Daedalus: the battle flashes away to a cloudy blue sky over a forest. In the sky scene the bronze
// machine climbs into view among birds, throws off the cover of its missiles and its blade as the
// sixteen missiles fly, sheds its hull plates, and its core glows until it sends out a ring of fire
// and launches its rocket; A or B skips the scene. Then the battle comes back and the missiles dive
// onto the foes in bursts of fire. Its second strike (btEffectRunDaedalusStrike) comes at the end
// of the next turn.
//
// Abilities: Daedalus.
void btEffectRunDaedalus(BATTLE_EFFECT_PARM *args)
{
    Sint16                   targets[UNIT_MAX];
    SPR_PLACEMENT            canopyPlace;
    SPR_PLACEMENT            place;
    SPR_PLACEMENT            missilePlace;
    SCALE                    canopyScale;
    DRAW_RECTANGLE           blit[2];
    SCALE                    pieceScale;
    MODEL_TEXTURE            fireRingTexture;
    SCALE                    missileScale;
    Uint8                   *canvas;
    Sint32                   frame;
    Uint8                   *dimMotes;
    BATTLE_BACKGROUND_STATE *view;
    BATTLE_STATE            *battle;
    FIXED                    camera;
    FIXED                    prevCamera;
    FIXED                    canopyX;
    Sint32                   canopyScroll;
    // The camera's climb still to come, from -DAEDALUS_CLIMB pixels up to 0.
    FIXED                    climb;
    Sint32                   canopyPhase;
    Sint32                   savedScrollX;
    Sint32                   bgX;
    BATTLE_EFFECT_STATE     *state;
    Sint32                   k;
    Sint32                   i;
    Sint32                   j;
    // The mote copies' spacing, in halfwords.
    Sint32                   step = DAEDALUS_MOTE_BYTES / 2;
    // Reused: the sky scene's ring radius, and the 0 the fall stores.
    Sint32                   radius;

    canvas   = RamHeapSlots->battleCanvas;
    state    = RamHeapSlots->battleEffect;
    dimMotes = RamHeapSlots->battleSheet;
    view     = RamHeapSlots->battleBg;
    battle   = RamHeapSlots->battleState;
    btEffectStartCanvasLayer(0);
    btEffectConfigureEffectDisplay();
    btEffectFillWhiteBand();
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
    blit[0]             = RamHeapSlots->ramCode;
    state->transferMode = CANVAS_COPY;
    sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
    camera       = 0;
    canopyScroll = 0;
    climb        = -toFIXED(DAEDALUS_CLIMB);
    canopyPhase  = 0;
    savedScrollX = BgScrollWork[1].x;
    bgX          = -80;

    for (frame = 0; frame != SUMMON_OPENING_TICKS; frame++)
    {
        if (frame == SUMMON_OPENING_LOAD_TICK)
        {
            Uint16       *color;
            Sint32        n;
            Sint32        off;
            Uint8        *out;
            Sint32        k;
            Sint32        r, g, b;
            Sint32        v;
            const Uint8  *src;
            DAEDALUS_SPR *object;

            syVRAMCacheFreeEntry(battle->arrowTiles);
            ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
            targets[0]             = BATTLE_UNIT_LIST_END;
            btActorSpawnObjectsForListStub(targets, 0);
            btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);
            for (color = (Uint16 *)&BG_PLTT_COLOR(6, 0), i = 0; i != BATTLE_BACKGROUND_COLORS;
                 i++, color++)
            {
                Sint32 green;
                Sint32 red;

                // Each colour 4 levels darker, its red and green traded.
                red   = RGB_RED(*color);
                green = RGB_GREEN(*color);
                b     = RGB_BLUE(*color) - 4;
                g     = green - 4;
                r     = red - 4;
                if (g < 0)
                    g = 0;
                if (r < 0)
                    r = 0;
                if (b < 0)
                    b = 0;
                if (g > RGB_LEVEL_MAX)
                    g = RGB_LEVEL_MAX;
                if (r > RGB_LEVEL_MAX)
                    r = RGB_LEVEL_MAX;
                if (b > RGB_LEVEL_MAX)
                    b = RGB_LEVEL_MAX;
                *color = RGB(g, r, b);
            }
            state->scrollTimer    = 0;
            state->scrollInterval = 5;
            state->scrollStepX    = -1;
            state->scrollStepY    = 0;
            sySetEvent(btEffectAdvanceScrollOnInterval, EVENT_PASS_TICK);
            view->busy = 1;
            btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, state->sheet, 0, 0);
            // The dimmed mote copies, from the state's sprite sheet.
            off = offsetof(BATTLE_EFFECT_STATE, sheet);
            n   = DAEDALUS_MOTE;
            for (j = 0; j != DAEDALUS_MOTE_COPIES; j++)
            {
                k   = j * step;
                i   = 0;
                // v first holds the copy's byte offset.
                v   = k * 2;
                out = (Uint8 *)(v + (Sint32)dimMotes);
                do
                {
                    v = ((Uint8 *)state)[BattleEffectMotePictures[n] + i + off];
                    if (v != 0)
                    {
                        v = v - j * DAEDALUS_MOTE_DIM_STEP - DAEDALUS_MOTE_DIM_FIRST;
                        if (v <= 0)
                            v = 1;
                    }
                    *out++ = v;
                    // Mote picture n is n + 1 pixels wide and twice as tall.
                } while (++i != (n + 1) * (n + 1) * 2);
            }
            src = (const Uint8 *)syResourceGetTableEntry(RESOURCE_FOREST_CANOPY_TILES);
            syWordsCopy((void *)&OBJ_PLTT_COLOR(CANOPY_PALETTE, 0), src, PALETTE_BANK_BYTES);
            src += PALETTE_BANK_BYTES;
            syDecompressTaggedLZ(src, state->sheet);
            for (j = 0; j != CANOPY_ROWS; j++)
            {
                for (i = 0; i != CANOPY_PICTURES; i++)
                {
                    object = btEffectSpriteCreateWithTiles(
                        32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CANOPY_PALETTE));
                    state->sprites[j * DAEDALUS_CANOPY_ROW + i] = object;
                    object->priority                            = 1;
                    syWordsCopy((void *)(VramCacheEntries[object->slot].offset + OBJ_VRAM0),
                                &state->sheet[j * CANOPY_ROW_BYTES + i * OBJ_32X32_4BPP_BYTES],
                                OBJ_32X32_4BPP_BYTES);
                }
                object = btEffectSpriteAcquire(32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CANOPY_PALETTE));
                state->sprites[j * DAEDALUS_CANOPY_ROW + CANOPY_PICTURES] = object;
                syWordsCopy(object, state->sprites[j * DAEDALUS_CANOPY_ROW], offsetof(SPR, lastNo));
                for (i = 0; i != CANOPY_PICTURES; i++)
                    state->particles[j * DAEDALUS_CANOPY_ROW + i].variant =
                        DAEDALUS_SPRITE(j * DAEDALUS_CANOPY_ROW + i)->tile;
            }
            src = (const Uint8 *)syResourceGetTableEntry(RESOURCE_BRONZE_AUTOMATON_TILES);
            syWordsCopy((void *)OBJ_PLTT, src, DAEDALUS_MACHINE_COLORS * 2);
            src += DAEDALUS_MACHINE_COLORS * 2;
            syDecompressTaggedLZ(src, DECODE_BUFFER);
            for (i = 0; i != DAEDALUS_MACHINE_PIECES; i++)
            {
                object = btEffectSpriteCreateWithTiles(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
                object->priority                     = 3;
                state->sprites[DAEDALUS_MACHINE + i] = object;
                syWordsCopy((void *)(VramCacheEntries[object->slot].offset + OBJ_VRAM0),
                            DECODE_BUFFER + i * OBJ_32X32_8BPP_BYTES,
                            OBJ_32X32_8BPP_BYTES);
            }
            object = btEffectSpriteCreateWithTiles(32, 64, OBJ_SIZE_32X64 | OBJ_256_COLORS, 0);
            // FAKEMATCH(dead): the slot's store alone in a once-only block; as
            // * the plain store.
            do
            {
                state->sprites[DAEDALUS_MACHINE + i] = object;
            } while (0);
            object->priority  = 3;
            // FAKEMATCH(dead): updates of i that cancel out.
            i                ^= frame;
            i                ^= frame;
            syWordsCopy((void *)(VramCacheEntries[object->slot].offset + OBJ_VRAM0),
                        DECODE_BUFFER + i * OBJ_32X32_8BPP_BYTES,
                        32 * 64);
            src = (const Uint8 *)syResourceGetTableEntry(RESOURCE_MISSILE_TILES);
            syWordsCopy(
                (void *)&OBJ_PLTT_COLOR(DAEDALUS_MISSILE_PALETTE, 0), src, PALETTE_BANK_BYTES);
            src += PALETTE_BANK_BYTES;
            syDecompressTaggedLZ(src, DECODE_BUFFER);
            object = btEffectSpriteCreateWithTiles(
                16, 8, OBJ_SIZE_16X8, OBJ_PALETTE(DAEDALUS_MISSILE_PALETTE));
            object->priority                 = 1;
            state->sprites[DAEDALUS_MISSILE] = object;
            syWordsCopy(
                (void *)(VramCacheEntries[object->slot].offset + OBJ_VRAM0), DECODE_BUFFER, 256);
            ASM_NOTHING();
            for (i = 0; i != DAEDALUS_MISSILE_COPIES; i++)
            {
                state->sprites[DAEDALUS_MISSILE + 1 + i] =
                    btEffectSpriteAcquire(16, 8, OBJ_SIZE_16X8, OBJ_PALETTE(15));
                syWordsCopy(state->sprites[DAEDALUS_MISSILE + 1 + i],
                            state->sprites[DAEDALUS_MISSILE],
                            offsetof(SPR, lastNo));
                DAEDALUS_SPRITE(DAEDALUS_MISSILE + 1 + i)->tile =
                    DAEDALUS_SPRITE(DAEDALUS_MISSILE)->tile;
                // Missiles 8 to 15, the back row, behind the machine.
                if (i > 6)
                    DAEDALUS_SPRITE(DAEDALUS_MISSILE + 1 + i)->priority = 2;
            }
        }
        // The canopy scrolls by half a pixel a tick; each time it has moved a whole picture, every
        // sprite takes the picture before its own.
        if (frame > SUMMON_OPENING_LOAD_TICK)
        {
            // FAKEMATCH(temporary): canopyPlaceReference, a pointer standing
            // * for canopyPlace.
            SPR_PLACEMENT *canopyPlaceReference;
            Sint32         row;

            canopyScale               = DaedalusCanopyScale;
            canopyPlace.groundHeight  = 0;
            canopyPlace.height        = toFIXED(255);
            canopyScroll             += toFIXED(1) / 2;
            canopyX                   = canopyScroll + camera;
            if (canopyX > toFIXED(CANOPY_PICTURE_SIZE) - 1)
            {
                canopyPhase   = (canopyPhase + CANOPY_PICTURES - 1) % CANOPY_PICTURES;
                canopyX      -= toFIXED(CANOPY_PICTURE_SIZE);
                canopyScroll -= toFIXED(CANOPY_PICTURE_SIZE);
            }
            for (j = 0; j != CANOPY_ROWS; j++)
            {
                for (i = 0; i != DAEDALUS_CANOPY_ROW; i++)
                {
                    SPR_ATTR *object;
                    Sint32    v;

                    canopyPlaceReference = &canopyPlace;
                    {
                        Sint32 h;

                        k = toFIXED(i * CANOPY_PICTURE_SIZE) + canopyX;
                        h = j * CANOPY_PICTURE_SIZE + CANOPY_Y - DAEDALUS_CLIMB;
                        canopyPlaceReference->x = k;
                        canopyPlaceReference->y = toFIXED(h);
                    }
                    if (i == CANOPY_PICTURES)
                    {
                        object = state->sprites[j * DAEDALUS_CANOPY_ROW + CANOPY_PICTURES];
                        v      = state
                                ->particles[j * DAEDALUS_CANOPY_ROW
                                            + (canopyPhase + CANOPY_WRAP + CANOPY_PICTURES - 1)
                                                  % CANOPY_PICTURES]
                                .variant;
                    }
                    else
                    {
                        row     = j * DAEDALUS_CANOPY_ROW;
                        k       = row;
                        k      += i;
                        object  = state->sprites[k];
                        v       = state
                                ->particles[row + (canopyPhase + CANOPY_WRAP + i) % CANOPY_PICTURES]
                                .variant;
                    }
                    object->tile = v & OBJ_TILE_MASK;
                    // The ninth sprite wraps round to the left of the first.
                    if (canopyPlace.x > toFIXED(CANOPY_PICTURES * CANOPY_PICTURE_SIZE) - 1)
                        canopyPlace.x -= toFIXED(DAEDALUS_CANOPY_ROW * CANOPY_PICTURE_SIZE);
                    fdRenderApplyProjectedPlacementStub(
                        state->sprites[j * DAEDALUS_CANOPY_ROW + i], &canopyPlace, &canopyScale, 0);
                }
            }
        }
        bgX      += 4;
        REG_BG2X  = bgX << 8;
        EFFECT_PRESENT_CANVAS(state);
    }

    {
        DRAW_RECTANGLE *blits;
        Uint16         *palette;
        Sint32          swayX;
        Sint32          swayY;
        FIXED           bobX;
        FIXED           bobY;
        Sint32         *ringX;
        void           *pixels;

        syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
        syWordsClearEntry(canvas, CANVAS_VIEW_BYTES);
        syWordsClearEntry((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
        REG_BG2CNT =
            BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        REG_BG2PA = Q_8_8(0.5);
        REG_BG2X  = 0;
        blits     = blit;
        btEffectBuildCanvasBlitters(0, blits);
        state->scrollTimer    = 0;
        state->scrollInterval = 5;
        state->scrollStepX    = -1;
        state->scrollStepY    = 0;
        view->busy            = 1;
        btResourceLoadAndDecompress(RESOURCE_ROCKET_FLAME_SHEET, state->sheet, 0, 0);
        btResourceLoadAndDecompress(
            RESOURCE_FLASH_BURST_SHEET, state->sheet + DAEDALUS_FLASH_BURST_PICTURES, 1, 0);
        btResourceLoadAndDecompress(
            RESOURCE_BIRD_FLOCK_SHEET, state->sheet + DAEDALUS_BIRD_FLOCK, 0, 0);
        pixels = DECODE_BUFFER;
        btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pixels, 0, 0);
        syWordsCopy((void *)BG_PLTT,
                    (const void *)syResourceGetTableEntry(RESOURCE_SMOKE_SHEET),
                    SHEET_PALETTE_BYTES);
        EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        REG_BLDALPHA = EFFECT_BLEND_ADD;
        REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        // The core's colour: the machine's first colour all green.
        for (i = 0; i != DAEDALUS_MACHINE_COLORS; i++)
        {
            palette = (Uint16 *)OBJ_PLTT + i;
            if (RGB_GREEN(*palette) == RGB_LEVEL_MAX)
                break;
        }
        for (i = 0; i != DAEDALUS_PARTICLE_COUNT; i++)
            EXTRA_PARTICLES[i].variant = -1;

        frame = 0;
        if (!(PerWork.data & KEY_B) && !(PerWork.repeat & KEY_A))
        {
        skyTick:
            if (frame == 0)
            {
                REG_BG2PA                                          = Q_8_8(1);
                *palette                                           = CORE_DARK;
                climb                                              = -toFIXED(DAEDALUS_CLIMB);
                camera                                             = 0;
                DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation         = TRIG_TURN * 3 / 4;
                DAEDALUS_SPRITE(DAEDALUS_BLADE)->rotation          = 0;
                DAEDALUS_SPRITE(DAEDALUS_COVER)->rotation          = 0;
                state->particles[DAEDALUS_COVER_MOTION].x          = 0;
                state->particles[DAEDALUS_COVER_MOTION].y          = 0;
                state->particles[DAEDALUS_COVER_MOTION].velocityX  = -toFIXED(6);
                state->particles[DAEDALUS_COVER_MOTION].velocityY  = 0;
                state->particles[DAEDALUS_COVER_MOTION].variant    = 0;
                state->particles[DAEDALUS_BLADE_MOTION].x          = 0;
                state->particles[DAEDALUS_BLADE_MOTION].y          = 0;
                state->particles[DAEDALUS_BLADE_MOTION].velocityX  = -toFIXED(12);
                state->particles[DAEDALUS_BLADE_MOTION].velocityY  = 0;
                state->particles[DAEDALUS_BLADE_MOTION].variant    = 0;
                state->particles[DAEDALUS_HULL_MOTION].x           = 0;
                state->particles[DAEDALUS_HULL_MOTION].y           = 0;
                state->particles[DAEDALUS_HULL_MOTION].velocityX   = -toFIXED(1);
                state->particles[DAEDALUS_HULL_MOTION].velocityY   = 0;
                state->particles[DAEDALUS_HULL_MOTION].variant     = 0;
                state->particles[DAEDALUS_ROCKET_MOTION].x         = toFIXED(160);
                state->particles[DAEDALUS_ROCKET_MOTION].y         = 0;
                state->particles[DAEDALUS_ROCKET_MOTION].velocityX = 0;
                state->particles[DAEDALUS_ROCKET_MOTION].velocityY = 0;
                state->particles[DAEDALUS_ROCKET_MOTION].variant   = 0;
                // The front eight wait in the right thigh and the back eight behind the body, in
                // columns of three.
                for (i = 0; i != DAEDALUS_MISSILE_COUNT / 2; i++)
                {
                    PARTICLE *p;

                    p            = &state->particles[DAEDALUS_MISSILE_MOTIONS + i];
                    p->x         = toFIXED((i / 3) * 4) + toFIXED(176);
                    p->y         = toFIXED((i % 3) * 3 * 2) + toFIXED(74);
                    p->velocityX = 0;
                    p->velocityY = 0;
                    p->variant   = 0;
                }
                for (i = 0; i != DAEDALUS_MISSILE_COUNT / 2; i++)
                {
                    PARTICLE *p;

                    p = &state
                             ->particles[DAEDALUS_MISSILE_MOTIONS + DAEDALUS_MISSILE_COUNT / 2 + i];
                    p->x         = toFIXED((i / 3) * 4) + toFIXED(112);
                    p->y         = toFIXED((i % 3) * 3 * 2) + toFIXED(82);
                    p->velocityX = 0;
                    p->velocityY = 0;
                    p->variant   = 0;
                }
            }
            prevCamera = camera;
            if (frame >= DAEDALUS_GLOW_START && frame < DAEDALUS_GLOW_END)
            {
                Sint32 r, g, b;

                r = RGB_RED(*palette);
                g = RGB_GREEN(*palette);
                b = RGB_BLUE(*palette);
                if (r < RGB_LEVEL_MAX)
                    r++;
                if (g < CORE_GLOW_GREEN)
                    g++;
                if (b < CORE_GLOW_BLUE)
                    b++;
                *palette = RGB(r, g, b);
            }
            if (frame >= DAEDALUS_GLOW_END)
            {
                Sint32 r, g, b;

                r = RGB_RED(*palette);
                g = RGB_GREEN(*palette);
                b = RGB_BLUE(*palette);
                if (r > CORE_EMBER_RED)
                    r--;
                if (g > 0)
                    g--;
                if (b > 0)
                    b--;
                *palette = RGB(r, g, b);
            }
            // The cover flies off, and the missiles follow it two at a time.
            if (frame == 280)
            {
                syWordsCopy((void *)BG_PLTT,
                            (const void *)syResourceGetTableEntry(RESOURCE_SMOKE_SHEET),
                            SHEET_PALETTE_BYTES);
                state->particles[DAEDALUS_COVER_MOTION].variant = 1;
            }
            // The blade flies off, spinning.
            if (frame == 288)
                state->particles[DAEDALUS_BLADE_MOTION].variant = 1;
            if (frame == 279)
            {
                REG_BG2PA  = Q_8_8(0.5);
                REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
                REG_BG2PA  = Q_8_8(0.5);
            }
            // The hull falls away.
            if (frame == 506)
                state->particles[DAEDALUS_HULL_MOTION].variant = 1;
            if (frame == 489)
            {
                REG_BG2PA          = Q_8_8(0.5);
                REG_BG2CNT         = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
                state->scrollStepX = 0;
            }
            if (frame == 528)
            {
                REG_BG2PA  = Q_8_8(0.5);
                REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS
                           | BGCNT_SCREEN_BASE(7);
            }
            if (frame == 0)
            {
                EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
                EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
                for (i = 0; i != DAEDALUS_BIRD_COUNT; i++)
                {
                    PARTICLE *p;

                    p            = &EXTRA_PARTICLES[DAEDALUS_BIRDS + i];
                    p->x         = toFIXED(DaedalusBirdStarts[i][0]);
                    p->y         = toFIXED(DaedalusBirdStarts[i][1]);
                    p->velocityX = DaedalusBirdSpeeds[i][0];
                    p->velocityY = DaedalusBirdSpeeds[i][1];
                    p->variant   = syRandom() & 127;
                }
            }
            for (i = 0; i != DAEDALUS_BIRD_COUNT; i++)
            {
                PARTICLE *p;

                p = &EXTRA_PARTICLES[DAEDALUS_BIRDS + i];
                if (frame >= DaedalusBirdTicks[i])
                {
                    blit[0](
                        canvas,
                        &state->sheet[DaedalusWingBeatPictures[(p->variant / 4) % BIRD_WING_BEAT]
                                          * PICTURE_BYTES(BIRD)
                                      + DAEDALUS_BIRD_FLOCK],
                        FIXEDtoINT(p->x),
                        FIXEDtoINT(p->y),
                        BIRD_WIDTH,
                        BIRD_HEIGHT);
                    p->x += p->velocityX;
                    p->y += p->velocityY;
                    p->variant++;
                }
            }
            if (frame <= 473)
                canopyScroll += toFIXED(1) / 2;
            canopyX = canopyScroll + camera;
            if (canopyX >= toFIXED(CANOPY_PICTURE_SIZE))
            {
                canopyPhase   = (canopyPhase + CANOPY_PICTURES - 1) % CANOPY_PICTURES;
                canopyX      -= toFIXED(CANOPY_PICTURE_SIZE);
                canopyScroll -= toFIXED(CANOPY_PICTURE_SIZE);
            }
            if (climb < 0)
                climb += toFIXED(1) / 8;
            pieceScale = DaedalusMachineScale;
            bobX       = 0;
            bobY       = 0;
            if (frame >= DAEDALUS_SWAY_STOP)
            {
                swayX = syCos(DAEDALUS_SWAY_STOP * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
                swayY = sySin(DAEDALUS_SWAY_STOP * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
                if (frame >= DAEDALUS_GLOW_START && frame < DAEDALUS_GLOW_END)
                {
                    Sint32 v;

                    v    = frame * DAEDALUS_LURCH_STEP - DAEDALUS_GLOW_START * DAEDALUS_LURCH_STEP;
                    bobX = sySin(v) * 3;
                    bobY = sySin(v) * 2;
                }
                if (frame >= DAEDALUS_RECOIL_START && frame < DAEDALUS_RECOIL_END)
                {
                    Sint32 v;

                    v = frame * DAEDALUS_LURCH_STEP - DAEDALUS_RECOIL_START * DAEDALUS_LURCH_STEP;
                    bobX = sySin(v) * 2;
                    bobY = sySin(v) * 3;
                }
            }
            else
            {
                swayX = syCos(frame * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
                swayY = sySin(frame * DAEDALUS_SWAY_STEP) * DAEDALUS_SWAY;
            }
            if (swayY > 0)
                swayY = -swayY;
            swayY              -= climb * 2;
            place.groundHeight  = 0;
            place.height        = toFIXED(255);
            for (i = 0; i != 5; i++)
            {
                if (frame == DaedalusClatterTicks[i])
                {
                    state->shakeFrame = 23;
                    sySoundPlayStub(SE_CLATTER);
                }
            }
            if (frame == 510)
                sySoundPlayStub(SE_SOL_SANCTUM_LIGHT_UP);
            if (frame == DAEDALUS_FIRE_TICK)
            {
                state->shakeFrame = 10;
                sySoundPlayStub(SE_CRASH);
            }
            if (frame == 578)
                sySoundPlayStub(SE_WAVE_HIGH);
            for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++)
            {
                PARTICLE *p;

                p = &state->particles[DAEDALUS_MISSILE_MOTIONS + i];
                if (p->variant != 0)
                {
                    PARTICLE *q;

                    p->variant++;
                    if (p->x > -toFIXED(16))
                    {
                        p->x += p->velocityX;
                        p->y += p->velocityY;
                        if (p->velocityY > -toFIXED(1) && p->velocityY < toFIXED(1))
                            p->velocityY += p->velocityZ;
                    }
                    if (p->velocityX > -toFIXED(8))
                        p->velocityX -= toFIXED(1) / 2;
                    q            = &EXTRA_PARTICLES[i * DAEDALUS_TRAIL_LENGTH
                                         + (p->variant & (DAEDALUS_TRAIL_LENGTH - 1))];
                    q->variant   = 0;
                    q->x         = p->x / 2;
                    q->y         = p->y - toFIXED(20);
                    q->velocityX = 0;
                    q->velocityY = 0;
                }
                // Missiles i and i + 8 fire together.
                if (state->particles[DAEDALUS_COVER_MOTION].variant
                    == (i & 7) * DAEDALUS_VOLLEY_STEP + DAEDALUS_VOLLEY_START)
                {
                    sySoundPlayStub(SE_ITEM_BREAK);
                    p->variant    = 1;
                    p->x         += swayX;
                    p->y         += swayY - toFIXED(DaedalusShakeOffsets[state->shakeFrame]);
                    p->velocityY  = (syRandom() & TRIG_ANGLE_MASK) - toFIXED(0.5);
                    p->velocityZ  = (syRandom() & 0x3fff) - toFIXED(0.125);
                }
            }
            if (frame <= 489)
            {
                for (i = DAEDALUS_MISSILE_COUNT / 2; i != DAEDALUS_MISSILE_COUNT; i++)
                {
                    PARTICLE *p;

                    p       = &state->particles[DAEDALUS_MISSILE_MOTIONS + i];
                    place.x = p->x + camera;
                    place.y = p->y;
                    // BUG: the back eight are placed with the sprites of missiles 7 to 14, so
                    // missile 8 shares the front row's last sprite and the last copy, behind the
                    // machine, is never placed.
                    if (p->variant != 0)
                        fdRenderApplyProjectedPlacementStub(
#if FIX_DAEDALUS_MISSILE_SPRITES
                            state->sprites[DAEDALUS_MISSILE + i],
#else
                            state->sprites[DAEDALUS_MISSILE - 1 + i],
#endif
                            &place,
                            &pieceScale,
                            0);
                }
            }
            if (frame == 578)
            {
                state->scrollInterval = 1;
                state->scrollTimer    = 0;
            }
            if (frame >= 578 && frame < 594)
                state->scrollStepX = ~((frame - 578) / 8);
            for (i = 0; i != 2; i++)
            {
                place.x = camera - swayX + bobX + toFIXED(120);
                place.y = toFIXED(i * DAEDALUS_PIECE_SIZE - DaedalusShakeOffsets[state->shakeFrame])
                        + swayY + bobY + toFIXED(100);
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_LEFT_LEG + i], &place, &pieceScale, 0);
            }
            if (frame <= 316)
            {
                PARTICLE *p;

                p = &state->particles[DAEDALUS_BLADE_MOTION];
                if (p->variant != 0)
                {
                    p->variant++;
                    p->x                                      += p->velocityX;
                    p->y                                      += p->velocityY;
                    p->velocityY                              += toFIXED(1) / 8;
                    p->velocityX                               = p->velocityX * 48 / DAMPING_SCALE;
                    p->velocityY                               = p->velocityY * 63 / DAMPING_SCALE;
                    DAEDALUS_SPRITE(DAEDALUS_BLADE)->rotation += TRIG_TURN / 128;
                }
                place.x = p->x - swayX + camera + toFIXED(120);
                place.y = toFIXED(99 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + p->y;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_BLADE], &place, &pieceScale, 0);
            }
            {
                PARTICLE *p;
                Sint32    v;

                v       = camera + bobX;
                place.x = v + toFIXED(142);
                place.y = toFIXED(72 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_TOP], &place, &pieceScale, 0);
                place.x = v + toFIXED(174);
                place.y = toFIXED(72 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_TOP + 1], &place, &pieceScale, 0);
                v       += toFIXED(128);
                place.x  = v;
                place.y  = toFIXED(104 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_CORE], &place, &pieceScale, 0);
                p = &state->particles[DAEDALUS_ROCKET_MOTION];
                if (frame > 602)
                    btEffectStepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
                if (frame == DAEDALUS_FIRE_TICK + 2)
                {
                    p->x = toFIXED(DAEDALUS_ROCKET_X);
                    p->y = toFIXED(DAEDALUS_ROCKET_Y - DaedalusShakeOffsets[state->shakeFrame])
                         + swayY;
                    p->variant   = 1;
                    p->velocityX = -toFIXED(4);
                    p->velocityY = -toFIXED(5) / 16;
                    for (i = 0; i != DAEDALUS_LAUNCH_FLAME_COUNT; i++)
                    {
                        PARTICLE *q;

                        q            = &EXTRA_PARTICLES[DAEDALUS_LAUNCH_FLAMES + i];
                        q->x         = INTtoFIXED((syRandom() & 15) + 56);
                        q->y         = INTtoFIXED((syRandom() & 31) + 64);
                        q->velocityX = -((syRandom() & 31) + 8) << 12;
                        q->velocityY = -((syRandom() & 31) + 8) << 12;
                        q->variant   = 0;
                    }
                    for (i = 0; i != DAEDALUS_EXHAUST_FLAMES; i++)
                        EXTRA_PARTICLES[DAEDALUS_EXHAUST + i].variant = -1;
                    for (i = 0; i != DAEDALUS_BURST_COUNT; i++)
                        EXTRA_PARTICLES[DAEDALUS_BURSTS + i].variant = -1;
                }
                // The rocket's flight, its particle's variant counting its ticks: exhaust from its
                // tenth tick, bursts every fourth tick of two stretches, and the turns, climbs and
                // pulls of its course.
                if (p->variant != 0)
                {
                    if (p->variant > 9)
                    {
                        PARTICLE *q;

                        q            = &EXTRA_PARTICLES[DAEDALUS_EXHAUST
                                             + (p->variant & (DAEDALUS_EXHAUST_FLAMES - 1))];
                        q->variant   = 0;
                        q->velocityX = -(sySin(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 2);
                        q->velocityY = syCos(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * 2;
                        q->x         = (p->x + camera) / 2
                             - ((sySin(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * ROCKET_NOZZLE)
                                >> 1);
                        q->y = p->y
                             + syCos(DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation) * ROCKET_NOZZLE
                             - toFIXED(ROCKET_FLAME_HEIGHT / 2);
                    }
                    if (((p->variant >= ROCKET_NOSE_UP_START && p->variant < ROCKET_NOSE_UP_END)
                         || (p->variant >= ROCKET_BURSTS_START && p->variant < ROCKET_BURSTS_END))
                        && (p->variant & 3) == 0)
                    {
                        PARTICLE *q;

                        q            = &EXTRA_PARTICLES[DAEDALUS_BURSTS
                                             + (p->variant & (DAEDALUS_BURST_COUNT - 1))];
                        q->variant   = 0;
                        q->velocityX = ((syRandom() & 31) + 16) << 12;
                        q->velocityY = (-syRandom() | ~15) << 14;
                        q->x         = toFIXED(88);
                        q->y         = toFIXED(128);
                    }
                    prevCamera = camera;
                    {
                        PARTICLE *f;

                        // The camera follows the rocket, keeping it where it left the machine.
                        f = &state->particles[DAEDALUS_ROCKET_MOTION];
                        // DEAD: the lead of two pixels a tick from ROCKET_LEAD_START, which is 0 on
                        // the one tick that reaches it, the sky scene's last; the ROM keeps its
                        // test and its sum.
                        if (f->variant >= ROCKET_LEAD_START)
                            camera += (toFIXED(DAEDALUS_ROCKET_X)
                                       - (f->x + INTtoFIXED((f->variant - ROCKET_LEAD_START) << 1))
                                       - camera)
                                    / 4;
                        else if (f->variant > 0)
                            camera += (toFIXED(DAEDALUS_ROCKET_X) - f->x - camera) / 4;
                    }
                    if (p->variant > 1)
                    {
                        p->x += p->velocityX;
                        p->y += p->velocityY;
                    }
                    if (p->variant > 7)
                    {
                        p->velocityX = p->velocityX * 63 / DAMPING_SCALE;
                        p->velocityY = p->velocityY * 63 / DAMPING_SCALE + ROCKET_GRAVITY;
                    }
                    if (p->variant < ROCKET_SINK_TICKS)
                        p->velocityY += toFIXED(1) / 128;
                    if (p->variant >= ROCKET_THRUST_START)
                        p->velocityX -= toFIXED(1) / 4;
                    if (p->variant >= ROCKET_LIFT_START && p->variant < ROCKET_LIFT_END)
                        p->velocityY -= toFIXED(1) / 16;
                    if (p->variant >= ROCKET_NOSE_UP_START && p->variant < ROCKET_NOSE_UP_END)
                    {
                        DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation =
                            (p->variant << 7) - TRIG_TURN * 89 / 256;
                        p->velocityY -= toFIXED(1) / 16;
                    }
                    if (p->variant >= ROCKET_TURN_START && p->variant < ROCKET_TURN_END)
                    {
                        if (p->variant >= ROCKET_DIP_START && p->variant < ROCKET_DIP_END)
                            DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation -= TRIG_TURN * 3 / 512;
                        else
                            DAEDALUS_SPRITE(DAEDALUS_ROCKET)->rotation += TRIG_TURN / 256;
                        p->velocityY -= toFIXED(1) / 16;
                    }
                    if (p->variant >= ROCKET_TURN_END && p->variant < ROCKET_SOAR_START)
                        p->velocityY -= toFIXED(1) / 32;
                    if (p->variant >= ROCKET_SOAR_START)
                        p->velocityY -= toFIXED(1) / 8;
                    for (i = 0; i != DAEDALUS_EXHAUST_FLAMES; i++)
                    {
                        PARTICLE *q;

                        q = &EXTRA_PARTICLES[DAEDALUS_EXHAUST + i];
                        if ((Uint32)q->variant < ROCKET_FLAME_COUNT)
                        {
                            blits[1](canvas,
                                     &state->sheet[q->variant * PICTURE_BYTES(ROCKET_FLAME)],
                                     (FIXEDtoINT(q->x)) - 2,
                                     (FIXEDtoINT(q->y)) - ROCKET_FLAME_HEIGHT / 2,
                                     ROCKET_FLAME_WIDTH,
                                     ROCKET_FLAME_HEIGHT);
                            btParticleAdvanceWithGravity2D(q, PARTICLE_DAMPING_NONE, 0);
                            q->variant++;
                        }
                    }
                    for (i = 0; i != DAEDALUS_BURST_COUNT; i++)
                    {
                        PARTICLE *q;

                        q = &EXTRA_PARTICLES[DAEDALUS_BURSTS + i];
                        if ((Uint32)q->variant < FLASH_BURST_PICTURE_COUNT * 4)
                        {
                            blits[1](canvas,
                                     &state->sheet[q->variant / 4 * PICTURE_BYTES(FLASH_BURST)
                                                   + DAEDALUS_FLASH_BURST_PICTURES],
                                     (FIXEDtoINT(q->x)) - FLASH_BURST_WIDTH / 2,
                                     (FIXEDtoINT(q->y)) - FLASH_BURST_HEIGHT / 2,
                                     FLASH_BURST_WIDTH,
                                     FLASH_BURST_HEIGHT);
                            q->velocityX += toFIXED(1) / 16;
                            btParticleAdvanceWithGravity2D(q, PARTICLE_DAMPING_NONE, 0);
                            q->variant++;
                        }
                    }
                    p->variant++;
                }
                pieceScale.x = ROCKET_SCALE;
                pieceScale.y = ROCKET_SCALE;
                if (p->variant == 0)
                {
                    place.x = camera + bobX + toFIXED(DAEDALUS_ROCKET_X);
                    place.y = toFIXED(DAEDALUS_ROCKET_Y - DaedalusShakeOffsets[state->shakeFrame])
                            + swayY + bobY;
                }
                else
                {
                    place.x = p->x + camera;
                    place.y = p->y;
                }
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_ROCKET], &place, &pieceScale, 0);
            }
            DAEDALUS_SPRITE(DAEDALUS_ROCKET)->offsetX = 0;
            DAEDALUS_SPRITE(DAEDALUS_ROCKET)->offsetY = 32;
            // The ring of fire: FireRingTexture on a square laid nearly flat, growing by a quarter
            // a tick, bright for seven ticks and fading out over the next three.
            if (frame >= DAEDALUS_FIRE_TICK)
            {
                RENDER_MODEL *fireRing;
                void         *buf;
                Sint32        v;
                Sint32        z;

                buf                       = syHeapAllocateEWRAM(512);
                fireRing                  = btRenderAllocateModelList(1);
                fireRingTexture.logWidth  = 7;
                fireRingTexture.logHeight = 7;
                fireRingTexture.pixels    = pixels;
                fireRing->texture         = &fireRingTexture;
                fireRing->flags           = 0;
                fireRing->mesh            = BattleEffectLargeSquareMesh;
                fireRing->kind            = MODEL_SHADED_ADDITIVE;
                fireRing->vertices        = buf;
                v                         = frame - DAEDALUS_FIRE_TICK;
                z                         = 96 - (v << 4);
                if (z > 0)
                    z = 0;
                if (z > MODEL_SHADE_INVISIBLE)
                {
                    fireRing->shade = z;
                    syInitMatrix();
                    syTranslate(camera + toFIXED(2), toFIXED(8), 0);
                    syRotZ(TRIG_TURN * 67 / 256);
                    syRotX(-TRIG_TURN / 32);
                    syMatrixScaleUniform(v * (toFIXED(1) / 4) + toFIXED(131) / 512);
                    btRenderTransformVertices(BattleEffectGroundSquareCorners, buf, 4);
                    btRenderDrawModelList(fireRing);
                }
                syHeapRelease(fireRing);
                syHeapRelease(buf);
            }
            if (frame == DAEDALUS_FIRE_TICK)
            {
                state->particles[DAEDALUS_RING_MOTION].x         = 0;
                state->particles[DAEDALUS_RING_MOTION].velocityX = toFIXED(8);
            }
            for (j = 0; j != 1; j++)
            {
                ringX = &state->particles[DAEDALUS_RING_MOTION].x;
                if (frame >= DaedalusRingStarts[j]
                    && frame < DaedalusRingStarts[j] + FLAME_RING_TICKS)
                {
                    Sint32 n;

                    radius  = FIXEDtoINT(*ringX);
                    n       = (frame - DaedalusRingStarts[j]) / 2;
                    *ringX += state->particles[DAEDALUS_RING_MOTION].velocityX;
                    state->particles[DAEDALUS_RING_MOTION].velocityX =
                        state->particles[DAEDALUS_RING_MOTION].velocityX * 48 / DAMPING_SCALE;
                    if (n < ROCKET_FLAME_COUNT)
                    {
                        for (i = 0; i != FLAME_RING_COUNT; i++)
                        {
                            Sint32 v;

                            v = (FIXEDtoINT(sySin(i * FLAME_RING_STEP) * radius / 2))
                              + DaedalusRingX[j];
                            blit[0](canvas,
                                    &state->sheet[n * PICTURE_BYTES(ROCKET_FLAME)],
                                    v - ROCKET_FLAME_WIDTH / 2,
                                    (FIXEDtoINT(syCos(i * FLAME_RING_STEP) * radius * 2))
                                        + FLAME_RING_Y,
                                    ROCKET_FLAME_WIDTH,
                                    ROCKET_FLAME_HEIGHT);
                        }
                    }
                }
            }
            pieceScale.x                                 = toFIXED(1);
            pieceScale.y                                 = toFIXED(1);
            DAEDALUS_SPRITE(DAEDALUS_BODY)->priority     = 2;
            DAEDALUS_SPRITE(DAEDALUS_BODY + 1)->priority = 2;
            DAEDALUS_SPRITE(DAEDALUS_BODY + 2)->priority = 2;
            DAEDALUS_SPRITE(DAEDALUS_BODY + 3)->priority = 2;
            {
                Sint32 v;

                v       = prevCamera + bobX;
                place.x = v + toFIXED(160);
                place.y = toFIXED(104 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_BODY], &place, &pieceScale, 0);
                place.x = v + toFIXED(128);
                place.y = toFIXED(136 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_BODY + 1], &place, &pieceScale, 0);
                place.x = v + toFIXED(160);
                place.y = toFIXED(136 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_BODY + 2], &place, &pieceScale, 0);
                place.x = v + toFIXED(192);
                place.y = toFIXED(136 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_BODY + 3], &place, &pieceScale, 0);
                DAEDALUS_SPRITE(DAEDALUS_BODY + 4)->priority = 2;
                place.x                                      = v + toFIXED(128);
                place.y = toFIXED(104 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + bobY;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_BODY + 4], &place, &pieceScale, 0);
            }
            for (i = 0; i != 5; i++)
            {
                place.x = toFIXED(DaedalusRightLegPoints[i][0]) + swayX + prevCamera + bobX
                        + toFIXED(176);
                place.y =
                    toFIXED(DaedalusRightLegPoints[i][1] - DaedalusShakeOffsets[state->shakeFrame])
                    + swayY + bobY + toFIXED(84);
                DAEDALUS_SPRITE(DAEDALUS_RIGHT_LEG + i)->priority = 2;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_RIGHT_LEG + i], &place, &pieceScale, 0);
            }
            if (frame <= 489)
            {
                for (i = 0; i != DAEDALUS_MISSILE_COUNT / 2; i++)
                {
                    PARTICLE *p;

                    p       = &state->particles[DAEDALUS_MISSILE_MOTIONS + i];
                    place.x = p->x + camera;
                    place.y = p->y;
                    if (p->variant != 0)
                        fdRenderApplyProjectedPlacementStub(
                            state->sprites[DAEDALUS_MISSILE + i], &place, &pieceScale, 0);
                }
            }
            if (frame < 540)
            {
                PARTICLE *p;

                p = &state->particles[DAEDALUS_HULL_MOTION];
                if (p->variant == 1)
                {
                    p->x         += p->velocityX;
                    p->y         += p->velocityY;
                    p->velocityY += toFIXED(1) / 8;
                    p->velocityX  = p->velocityX * 60 / DAMPING_SCALE;
                    p->velocityY  = p->velocityY * 63 / DAMPING_SCALE;
                }
                for (i = 0; i != 4; i++)
                {
                    place.x = toFIXED((i & 1) * DAEDALUS_PIECE_SIZE) + p->x + camera + toFIXED(128);
                    place.y = toFIXED((i / 2) * DAEDALUS_PIECE_SIZE
                                      - DaedalusShakeOffsets[state->shakeFrame])
                            + swayY + p->y + toFIXED(104);
                    DAEDALUS_SPRITE(DAEDALUS_HULL + i)->priority = 1;
                    fdRenderApplyProjectedPlacementStub(
                        state->sprites[DAEDALUS_HULL + i], &place, &pieceScale, 0);
                }
            }
            if (frame <= 316)
            {
                PARTICLE *p;

                p = &state->particles[DAEDALUS_COVER_MOTION];
                if (p->variant != 0)
                {
                    p->variant++;
                    p->x                                      += p->velocityX;
                    p->y                                      += p->velocityY;
                    p->velocityY                              += toFIXED(1) / 8;
                    p->velocityX                               = p->velocityX * 48 / DAMPING_SCALE;
                    p->velocityY                               = p->velocityY * 63 / DAMPING_SCALE;
                    DAEDALUS_SPRITE(DAEDALUS_COVER)->rotation += TRIG_TURN / 256;
                }
                place.x = swayX + p->x + camera + toFIXED(184);
                place.y = toFIXED(92 - DaedalusShakeOffsets[state->shakeFrame]) + swayY + p->y;
                DAEDALUS_SPRITE(DAEDALUS_COVER)->priority = 1;
                fdRenderApplyProjectedPlacementStub(
                    state->sprites[DAEDALUS_COVER], &place, &pieceScale, 0);
            }
            {
                Sint32 row;

                for (j = 0; j != CANOPY_ROWS; j++)
                {
                    for (i = 0; i != DAEDALUS_CANOPY_ROW; i++)
                    {
                        DAEDALUS_SPR *object;
                        Sint32        v;
                        place.x = toFIXED(i * CANOPY_PICTURE_SIZE) + canopyX;
                        place.y = toFIXED(j * CANOPY_PICTURE_SIZE
                                          - DaedalusShakeOffsets[state->shakeFrame])
                                + climb + toFIXED(CANOPY_Y);
                        if (i == CANOPY_PICTURES)
                        {
                            DAEDALUS_SPRITE(j * DAEDALUS_CANOPY_ROW + i)->tile =
                                state
                                    ->particles[j * DAEDALUS_CANOPY_ROW
                                                + (canopyPhase + CANOPY_WRAP + CANOPY_PICTURES - 1)
                                                      % CANOPY_PICTURES]
                                    .variant
                                & OBJ_TILE_MASK;
                        }
                        else
                        {
                            row  = j * DAEDALUS_CANOPY_ROW;
                            k    = row;
                            k   += i;
                            DAEDALUS_SPRITE(k)->tile =
                                state
                                    ->particles[row
                                                + (canopyPhase + CANOPY_WRAP + i) % CANOPY_PICTURES]
                                    .variant
                                & OBJ_TILE_MASK;
                        }
                        if (place.x > toFIXED(CANOPY_PICTURES * CANOPY_PICTURE_SIZE) - 1)
                            place.x -= toFIXED(DAEDALUS_CANOPY_ROW * CANOPY_PICTURE_SIZE);
                        fdRenderApplyProjectedPlacementStub(
                            state->sprites[j * DAEDALUS_CANOPY_ROW + i], &place, &pieceScale, 0);
                    }
                }
            }
            for (j = 0; j != 4; j++)
            {
                for (i = 0; i != 6; i++)
                {
                    if (frame >= i * 2 + DAEDALUS_SPLASH_START
                        && frame < i * 2 + DAEDALUS_SPLASH_START + ROCKET_FLAME_COUNT * 2)
                    {
                        Sint32 x, y, n;

                        x = DaedalusSplashPoints[j][i][0];
                        n = (frame - (i * 2 + DAEDALUS_SPLASH_START)) / 2;
                        y = DaedalusSplashPoints[j][i][1] + (FIXEDtoINT(swayY))
                          - DaedalusShakeOffsets[state->shakeFrame];
                        blit[0](canvas,
                                &state->sheet[n * PICTURE_BYTES(ROCKET_FLAME)],
                                x - ROCKET_FLAME_WIDTH / 2,
                                y - ROCKET_FLAME_HEIGHT / 2,
                                ROCKET_FLAME_WIDTH,
                                ROCKET_FLAME_HEIGHT);
                    }
                }
            }
            // Every particle of the runs young enough to be a flame is drawn and moved here: the
            // missiles' trails, and with them the launch flames, which have no loop of their own,
            // and the young exhaust and bursts, which their own loops draw and move as well.
            for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++)
            {
                for (j = 0; j != DAEDALUS_TRAIL_LENGTH; j++)
                {
                    PARTICLE *q;

                    q = &EXTRA_PARTICLES[i * DAEDALUS_TRAIL_LENGTH + j];
                    if ((Uint32)q->variant < ROCKET_FLAME_COUNT)
                    {
                        blit[0](canvas,
                                &state->sheet[q->variant * PICTURE_BYTES(ROCKET_FLAME)],
                                (FIXEDtoINT(q->x)) - 2,
                                (FIXEDtoINT(q->y)) - ROCKET_FLAME_HEIGHT / 2,
                                ROCKET_FLAME_WIDTH,
                                ROCKET_FLAME_HEIGHT);
                        q->x         += q->velocityX;
                        q->y         += q->velocityY;
                        q->velocityY -= toFIXED(1) / 4;
                        q->variant++;
                    }
                }
            }
            {
                Sint32    *shake;
                Sint32     v;
                BG_SCROLL *bg;

                shake                  = &state->shakeFrame;
                bg                     = BgScrollWork;
                v                      = DaedalusShakeOffsets[*shake];
                bg[1].y                = v + BATTLE_BACKGROUND_SCROLL_Y;
                ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y - v;
                if (*shake > 0)
                    *shake -= 1;
            }
            EFFECT_END_TICK(state);
            frame++;
            if (frame != DAEDALUS_SKY_TICKS && !(PerWork.data & KEY_B) && !(PerWork.repeat & KEY_A))
                goto skyTick;
        }

        syCloseEvent(btEffectAdvanceScrollOnInterval);
        BgScrollWork[1].x = savedScrollX;
        view->busy        = 0;
        btEffectSetupBlendedDisplay();
        for (i = 0; i != DAEDALUS_MISSILE + 1 + DAEDALUS_MISSILE_COPIES; i++)
            fdSpriteDestroyStub(state->sprites[i]);
        battle->arrowTiles = syVRAMCacheAllocateEntry(128);
        syPaletteLoadBasicSpriteColors();
        syDMAXCopy((void *)OBJ_PLTT,
                   (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
                   DMA_COPY_HALFWORDS(PALETTE_COLORS),
                   REG_DMA3);
        BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
            BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
        EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
        REG_BG2X     = 0;
        REG_BLDCNT   = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BG2PA    = Q_8_8(0.5);
        REG_BG2CNT   = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        REG_BLDALPHA = EFFECT_BLEND_ADD;
        args->targets[args->targetCount] = BATTLE_UNIT_LIST_END;
        btActorSpawnObjectsForListStub(args->targets, 0);
        // The fall: sixteen missiles dive onto the foes.
        btEffectCreateSprites(1, SPRITE_DAEDALUS_MISSILE, 1);
        for (i = 0; i != DAEDALUS_MISSILE_COPIES; i++)
        {
            state->sprites[1 + i] =
                btEffectSpriteAcquire(32, 16, OBJ_SIZE_32X16 | OBJ_256_COLORS, 0);
            IWRAM_COPY_WORDS_LINKED(
                state->sprites[1 + i], state->sprites[0], offsetof(SPR, lastNo));
            DAEDALUS_SPRITE(1 + i)->offsetX = 32;
            DAEDALUS_SPRITE(1 + i)->offsetY = 8;
        }
        btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
        btResourceLoadAndDecompress(
            RESOURCE_ROCKET_FLAME_SHEET, state->sheet + DAEDALUS_ROCKET_FLAMES, 0, 0);
        {
            const void *colors = (const void *)syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B);
            COPY_FUNC   copy   = IWRAM_COPY_WORDS_LINKED;

            copy((void *)BG_PLTT, colors, SHEET_PALETTE_BYTES);
        }

        for (frame = 0; frame != DAEDALUS_FALL_TICKS; frame++)
        {
            // Each missile dives at its own angle, eight pixels a tick, from where it reaches its
            // point in 24 ticks.
            if (frame == 0)
            {
                Sint32    z;
                PARTICLE *p;

                i      = 0;
                radius = 0;
                for (; i != DAEDALUS_MISSILE_COUNT; i++)
                {
                    p          = &state->particles[i];
                    z          = i * -(TRIG_TURN / 256);
                    p->variant = radius;
                    syRandom();
                    p->velocityZ = radius;
                    // FAKEMATCH(dead): k = 0 under a test that is never true, a
                    // * store nothing reads.
                    if (state == NULL || p == NULL)
                        k = 0;
                    p->z         = z - (p->velocityZ * FALL_DIVE_SPREAD) - TRIG_TURN / 32;
                    p->velocityX = -syCos(p->z) * 8;
                    p->velocityY = -sySin(p->z) * 8;
                    p->x         = INTtoFIXED(syRandom() % 80 + 80) - p->velocityX * 24;
                    p->y         = INTtoFIXED((syRandom() & 7) + 96) - p->velocityY * 24;
                    DAEDALUS_SPRITE(i)->rotation = p->z;
                    DAEDALUS_SPRITE(i)->offsetX  = radius;
                    DAEDALUS_SPRITE(i)->offsetY  = 8;
                }
                for (i = 0; i != DAEDALUS_PARTICLE_COUNT; i++)
                    EXTRA_PARTICLES[i].variant = -1;
            }
            missileScale              = DaedalusMissileScale;
            missilePlace.groundHeight = 0;
            missilePlace.height       = toFIXED(255);
            for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++)
            {
                PARTICLE *p;

                p = &state->particles[i];
                if (frame >= DaedalusDiveStarts[i] && p->variant == 0)
                {
                    missilePlace.x = p->x;
                    missilePlace.y = p->y;
                    if (missilePlace.y > 0 && missilePlace.y < toFIXED(128))
                        fdRenderApplyProjectedPlacementStub(
                            state->sprites[i], &missilePlace, &missileScale, 0);
                    // A missile that reaches the ground bursts and strikes the foes; one still
                    // diving leaves a trail of flames.
                    if (p->y > toFIXED(112) - 1)
                    {
                        p->variant = 1;
                        for (j = 0; j != 2; j++)
                        {
                            Sint32    speed;
                            Sint32    angle;
                            PARTICLE *q;

                            q            = &EXTRA_PARTICLES[i * FALL_BURST_SLOTS + j];
                            speed        = (syRandom() & 31) + 32;
                            angle        = (syRandom() & 0x7fff) + DEGtoANG(90);
                            q->x         = p->x / 2;
                            q->y         = p->y;
                            q->velocityX = sySin(angle) * speed >> 4;
                            q->velocityY = syCos(angle) * speed >> 4;
                            q->variant   = 0;
                        }
                        sySoundPlayStub(SE_CLATTER);
                        for (j = 0; j != args->targetCount; j++)
                        {
                            btEffectStartActorMotion(args->targets[j], 1, 0, 0, toFIXED(2), 100);
                            btEffectTintBattler(
                                args->targets[j], SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 7);
                            state->shakeFrame = 4;
                        }
                    }
                    else
                    {
                        PARTICLE *q;

                        {
                            Sint32 m = (frame - DaedalusDiveStarts[i]) % FALL_TRAIL_LENGTH;
                            q        = &EXTRA_PARTICLES[i * FALL_TRAIL_LENGTH + m + FALL_TRAILS];
                        }
                        q->x         = p->x / 2;
                        q->y         = p->y - toFIXED(16);
                        q->velocityX = 0;
                        q->velocityY = 0;
                        q->variant   = 0;
                    }
                    DAEDALUS_SPRITE(i)->rotation = p->z;
                    btParticleAdvanceWithGravity3D(p, PARTICLE_DAMPING_NONE, 0);
                    p->velocityX             = -syCos(DAEDALUS_SPRITE(i)->rotation) * 8;
                    p->velocityY             = -sySin(DAEDALUS_SPRITE(i)->rotation) * 8;
                    DAEDALUS_SPRITE(i)->tile = DAEDALUS_SPRITE(0)->tile;
                }
            }
            // A trail's flame shows from its third tick.
            for (i = 0; i != DAEDALUS_MISSILE_COUNT * FALL_TRAIL_LENGTH; i++)
            {
                PARTICLE *q;

                q = &EXTRA_PARTICLES[FALL_TRAILS + i];
                if ((Uint32)q->variant < ROCKET_FLAME_COUNT + 2)
                {
                    Sint32 v;

                    v = q->variant - 2;
                    if (v >= 0)
                        blits[1](
                            canvas,
                            &state->sheet[v * PICTURE_BYTES(ROCKET_FLAME) + DAEDALUS_ROCKET_FLAMES],
                            (FIXEDtoINT(q->x)) - ROCKET_FLAME_WIDTH / 2,
                            (FIXEDtoINT(q->y)) - ROCKET_FLAME_HEIGHT / 2,
                            ROCKET_FLAME_WIDTH,
                            ROCKET_FLAME_HEIGHT);
                    q->variant++;
                }
            }
            // The bursts, three ticks a picture, every other one added.
            for (i = 0; i != DAEDALUS_MISSILE_COUNT * FALL_BURST_SLOTS; i++)
            {
                PARTICLE *q;

                q = &EXTRA_PARTICLES[i];
                if ((Uint32)q->variant < FLASH_BURST_PICTURE_COUNT * 3)
                {
                    Sint32 v;

                    v = q->variant / 3;
                    blits[i & 1](canvas,
                                 &state->sheet[v * PICTURE_BYTES(FLASH_BURST)],
                                 (FIXEDtoINT(q->x)) - FLASH_BURST_WIDTH / 2,
                                 (FIXEDtoINT(q->y)) - FLASH_BURST_HEIGHT * 3 / 4,
                                 FLASH_BURST_WIDTH,
                                 FLASH_BURST_HEIGHT);
                    btParticleAdvanceWithGravity2D(q, 56, -toFIXED(1) / 8);
                    q->variant++;
                }
            }
            btCameraShake(8, 8);
            EFFECT_END_TICK(state);
        }
        for (i = 0; i != DAEDALUS_MISSILE_COUNT; i++)
            fdSpriteDestroyStub(state->sprites[i]);
        EFFECT_FINISH_CANVAS();
    }
}

// The rocket's scale in the second strike: twice its size.
extern const SCALE DaedalusStrikeRocketScale;

#define DAEDALUS_STRIKE_TICKS       108
// The rocket dives in over the strike's first DAEDALUS_STRIKE_DIVE_TICKS.
#define DAEDALUS_STRIKE_DIVE_TICKS  92
// From DAEDALUS_STRIKE_GLOW_START a disc of light swells to DAEDALUS_STRIKE_DISC_SIZE beneath it
// and its rays of light spin out: six of the sixteen set up, each shown for
// DAEDALUS_STRIKE_RAY_LIFE steps, two a tick.
#define DAEDALUS_STRIKE_GLOW_START  56
#define DAEDALUS_STRIKE_DISC_SIZE   128
#define DAEDALUS_STRIKE_RAYS_SET_UP 16
#define DAEDALUS_STRIKE_RAYS        6
#define DAEDALUS_STRIKE_RAY_LIFE    48
// At DAEDALUS_STRIKE_SPARK_TICK the canvas flashes, and a burst of SPARKS_PER_BURST sparks scatters
// each of the next DAEDALUS_STRIKE_BURSTS ticks.
#define DAEDALUS_STRIKE_SPARK_TICK  80
#define DAEDALUS_STRIKE_BURSTS      8
#define SPARKS_PER_BURST            8
#define DAEDALUS_STRIKE_SPARKS      (DAEDALUS_STRIKE_BURSTS * SPARKS_PER_BURST)

// The rocket's display fields that the strike sets: a SPR from its rotation on.
typedef struct STRIKE_ROCKET
{
    Uint8  imageScaleAndSlot[18]; // SPR's fields before the rotation
    Uint16 rotation;
    Uint8  size[2];
    Uint8  offsetX;
    Uint8  offsetY;
} STRIKE_ROCKET;

// Where GlowBandTextureA is decoded in the effect's pictures.
#define DAEDALUS_STRIKE_GLOW_BAND 16384

// Daedalus's second strike, at the end of the turn after the summon: its rocket
// (SPRITE_DAEDALUS_ROCKET) dives from above the screen and settles while, from
// DAEDALUS_STRIKE_GLOW_START, a disc of light swells beneath it and rays of light spin out. At
// DAEDALUS_STRIKE_SPARK_TICK the canvas flashes and a burst of sparks scatters each tick of
// DAEDALUS_STRIKE_BURSTS, and at the ticks of DaedalusStrikeHitTicks every target is knocked back
// and flashes.
//
// Abilities: Daedalus (its second strike).
void btEffectRunDaedalusStrike(BATTLE_EFFECT_PARM *args)
{
    BATTLE_EFFECT_STATE *state;
    void                *canvas;
    DRAW_RECTANGLE       draw[2];
    MODEL_TEXTURE        texture;
    SPR_PLACEMENT        place;
    SCALE                rocketScale;
    Sint32               frame;
    Sint32               i;
    Sint32               j;
    FIXED                centerX;
    FIXED                centerY;
    Sint32               burst;
    PARTICLE            *spark;
    Sint32               mask;
    FIXED                rayX  = toFIXED(66);
    FIXED                discY = toFIXED(80);

    canvas                     = RamHeapSlots->battleCanvas;
    state                      = RamHeapSlots->battleEffect;
    btEffectConfigureEffectDisplay();
    REG_WIN0V = WIN_RANGE(16, 128);
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_CLR;
    btEffectStartCanvasLayer(0);
    REG_BLDALPHA = EFFECT_BLEND_ADD;
    btEffectCreateSprites(1, SPRITE_DAEDALUS_ROCKET, 3);
    btEffectShowLivingTargets(state->args);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
    draw[0] = RamHeapSlots->ramCode;
    btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
    EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    state->fadeFrame = BACKGROUND_DARKEN_TICKS;
    state->fadeStep  = 0;

    frame            = 0;
    // The first burst's sparks, counted from the strike's start.
    burst            = -DAEDALUS_STRIKE_SPARK_TICK * SPARKS_PER_BURST;
    do
    {
        if (frame == 0)
        {
            EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
            syWordsCopyEntry((void *)BG_PLTT,
                             syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                             SHEET_PALETTE_BYTES);
        }
        if (frame == DAEDALUS_STRIKE_GLOW_START)
        {
            sySoundPlayStub(SE_SPIN);
        }
        for (i = 0; i != 7; i++)
        {
            if (frame == DaedalusStrikeHitTicks[i])
            {
                for (j = 0; j != args->targetCount; j++)
                {
                    btEffectStartActorMotion(
                        args->targets[j], 1, toFIXED(2), toFIXED(8), toFIXED(2), 120);
                    btEffectTintBattler(args->targets[j], SPRITE_COLOR_WHITE, POSE_DAMAGE, j, 7);
                }
                state->shakeFrame = 4;
                sySoundPlayStub(SE_CRASH);
            }
        }

        // The flash, and a burst of sparks a tick.
        if (frame >= DAEDALUS_STRIKE_SPARK_TICK)
        {
            // One pointer to the sparks for the burst and the draw loop below.
            PARTICLE *sparks;

            if (frame == DAEDALUS_STRIKE_SPARK_TICK)
            {
                // UB: the lives are cleared through one Sint32 pointer stepped a whole step at a
                // time, past the step's scalar members.
                Sint32 *life = &EXTRA_PARTICLES[0].variant;

                for (i = 0; i != DAEDALUS_STRIKE_SPARKS; i++)
                {
                    *life  = -1;
                    life  += sizeof(PARTICLE) / sizeof(Sint32);
                }
                state->shakeFrame = 8;
                syWordsFillEntry(
                    canvas, CANVAS_VIEW_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
            }
            sparks = EXTRA_PARTICLES;
            if ((Uint32)(frame - DAEDALUS_STRIKE_SPARK_TICK) < DAEDALUS_STRIKE_BURSTS)
            {
                centerX = INTtoFIXED((syRandom() & 63) + 28);
                centerY = INTtoFIXED((syRandom() & 31) + 80);

                j       = 0;
                mask    = 15;
                spark   = sparks + burst;
                for (; j != SPARKS_PER_BURST; j++, spark++)
                {
                    Sint32 speed;
                    Sint32 angle;
                    speed             = 31;
                    speed            &= syRandom();
                    angle             = syRandom();

                    spark->x          = centerX + toFIXED((syRandom() & mask) - 8);
                    spark->y          = centerY + toFIXED((syRandom() & mask) - 8);
                    speed            += 32;
                    spark->velocityX  = (sySin(angle) * speed) >> 3;
                    spark->velocityY  = (syCos(angle) * speed) >> 3;
                    spark->variant    = 0;
                }
            }
            {
                PARTICLE *mote;

                for (i = 0, mote = sparks; i != DAEDALUS_STRIKE_SPARKS; i++, mote++)
                {
                    if ((Uint32)mote->variant < FLASH_BURST_PICTURE_COUNT * 3)
                    {
                        draw[0](canvas,
                                state->sheet + mote->variant / 3 * PICTURE_BYTES(FLASH_BURST),
                                FIXED_INTEGER_PART(mote->x) - FLASH_BURST_WIDTH / 2,
                                FIXED_INTEGER_PART(mote->y) - FLASH_BURST_HEIGHT * 3 / 4,
                                FLASH_BURST_WIDTH,
                                FLASH_BURST_HEIGHT);
                        btParticleAdvanceWithGravity2D(mote, 62, -toFIXED(1) / 32);
                        mote->variant++;
                    }
                }
            }
        }

        // The rocket dives two pixels a tick for 64 ticks, then sinks half a pixel a tick.
        if (frame < DAEDALUS_STRIKE_DIVE_TICKS)
        {
            STRIKE_ROCKET *rocket;

            rocketScale        = DaedalusStrikeRocketScale;
            rocket             = state->sprites[0];
            rocket->offsetY    = 32;
            rocket->offsetX    = 0;
            place.groundHeight = 0;
            rocket->rotation   = TRIG_TURN * 17 / 32;
            place.height       = toFIXED(255);
            if (frame <= 63)
            {
                place.x = toFIXED(152 - frame / 4);
                place.y = toFIXED(frame * 2) - toFIXED(64);
            }
            else if ((Uint32)(frame - 64) <= 63)
            {
                Sint32 sinkFrame = frame - 64;

                place.x          = toFIXED(136 - sinkFrame / 16);
                place.y          = toFIXED(sinkFrame / 2) + toFIXED(64);
            }
            else
            {
                place.x = toFIXED(136);
                place.y = toFIXED(64);
            }
            fdRenderApplyProjectedPlacementStub(state->sprites[0], &place, &rocketScale.x, 0);
        }

        // The disc and the rays, until the rocket settles.
        if ((Uint32)(frame - DAEDALUS_STRIKE_GLOW_START)
            < DAEDALUS_STRIKE_DIVE_TICKS - DAEDALUS_STRIKE_GLOW_START)
        {
            Sint32        size;
            Sint32        grow = frame * 2 - 104;
            RENDER_MODEL *model;
            void         *vertices;
            PARTICLE     *ray;
            Uint8        *cells;
            FIXED         discX = toFIXED(64);

            if (grow > DAEDALUS_STRIKE_DISC_SIZE / 2)
            {
                size = DAEDALUS_STRIKE_DISC_SIZE;
            }
            else
            {
                size  = grow * 2;
                cells = MapGridMetatileBuffer;
                btEffectDrawShadedCircles(cells, size, toFIXED(1));
            }
            cells = MapGridMetatileBuffer;
            btCanvasBlitMirroredQuarters(cells, FIXEDtoINT(discX), FIXEDtoINT(discY), size);
            // Each ray at a random angle, turning a random way, a little after the one before.
            if (frame == DAEDALUS_STRIKE_GLOW_START)
            {
                Sint32 delay;

                i     = 0;
                delay = 0;
                for (ray = state->particles; i != DAEDALUS_STRIKE_RAYS_SET_UP; i++, ray++)
                {
                    ray->x = syRandom() & TRIG_ANGLE_MASK;
                    ray->y = (syRandom() & 255) + 128;
                    if (syRandom() & 1)
                    {
                        ray->y = -ray->y;
                    }
                    ray->variant  = delay;
                    delay        -= 4;
                }
                btResourceLoadAndDecompress(
                    RESOURCE_GLOW_BAND_TEXTURE_A, state->sheet + DAEDALUS_STRIKE_GLOW_BAND, 0, 0);
            }
            vertices          = syHeapAllocateEWRAM(32);
            model             = btRenderAllocateModelList(1);
            model->shade      = 0;
            texture.logWidth  = 5;
            texture.logHeight = 2;
            model->vertices   = vertices;
            texture.pixels    = state->sheet + DAEDALUS_STRIKE_GLOW_BAND;
            model->kind       = MODEL_SHADED_ADDITIVE;
            model->texture    = &texture;
            model->mesh       = BattleEffectSmallSquareMesh;

            // A ray swells and narrows again over its life.
            for (i = 0; i != DAEDALUS_STRIKE_RAYS; i++)
            {
                ray           = &state->particles[i];
                ray->variant += 2;
                ray->x       += ray->y * 2;
                if ((Uint32)ray->variant < DAEDALUS_STRIKE_RAY_LIFE)
                {
                    Sint32 k;

                    syInitMatrix();
                    k = i & 3;
                    syTranslate(toFIXED(k * 2) + rayX - toFIXED(64),
                                discY - INTtoFIXED(k + 2) - toFIXED(64),
                                0);
                    syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
                    syRotZ(ray->x);
                    syScale(sySin(ray->variant * (TRIG_TURN / 2 / DAEDALUS_STRIKE_RAY_LIFE))
                                + toFIXED(1),
                            toFIXED(2),
                            toFIXED(1));
                    btRenderTransformVertices(BattleEffectTaperedBeamCorners, vertices, 4);
                    btRenderDrawModelList(model);
                }
            }
            syHeapRelease(model);
            syHeapRelease(vertices);
        }
        btCameraShake(8, 8);
        EFFECT_END_TICK(state);
        burst += SPARKS_PER_BURST;
        frame++;
    } while (frame != DAEDALUS_STRIKE_TICKS);
    syCloseEvent(btEffectTransferCanvas);
    syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
    fdSpriteDestroyStub(state->sprites[0]);
    btEffectEndCanvasLayer();
}

#undef MIRRORED_WALL_TICKS
#undef MIRRORED_WALL_DRAW_TICKS
#undef PICTURE_SLOT_SIZE
#undef PICTURE_OFFSET
#undef EARTH_WALL_LOW
#undef EARTH_WALL_LOW_WIDTH
#undef EARTH_WALL_LOW_HEIGHT
#undef EARTH_WALL_MIDDLE
#undef EARTH_WALL_MIDDLE_WIDTH
#undef EARTH_WALL_MIDDLE_HEIGHT
#undef EARTH_WALL_HIGH
#undef EARTH_WALL_HIGH_WIDTH
#undef EARTH_WALL_HIGH_HEIGHT
#undef MIRROR_HALF
#undef IWRAM_COPY_WORDS_LINKED
#undef DECODE_BUFFER
#undef DAEDALUS_STRIKE_TICKS
#undef DAEDALUS_STRIKE_DIVE_TICKS
#undef DAEDALUS_STRIKE_GLOW_START
#undef DAEDALUS_STRIKE_DISC_SIZE
#undef DAEDALUS_STRIKE_RAYS_SET_UP
#undef DAEDALUS_STRIKE_RAYS
#undef DAEDALUS_STRIKE_RAY_LIFE
#undef DAEDALUS_STRIKE_SPARK_TICK
#undef DAEDALUS_STRIKE_BURSTS
#undef SPARKS_PER_BURST
#undef DAEDALUS_STRIKE_SPARKS
#undef BIRD_HEIGHT
#undef BIRD_WIDTH
#undef BIRD_WING_BEAT
#undef CANOPY_PALETTE
#undef CANOPY_PICTURES
#undef CANOPY_PICTURE_SIZE
#undef CANOPY_ROWS
#undef CANOPY_ROW_BYTES
#undef CANOPY_WRAP
#undef CANOPY_Y
#undef CORE_DARK
#undef CORE_EMBER_RED
#undef CORE_GLOW_BLUE
#undef CORE_GLOW_GREEN
#undef FALL_BURST_SLOTS
#undef FALL_TRAILS
#undef FALL_DIVE_SPREAD
#undef FALL_TRAIL_LENGTH
#undef FLAME_RING_COUNT
#undef FLAME_RING_TICKS
#undef FLAME_RING_STEP
#undef FLAME_RING_Y
#undef ROCKET_FLAME_COUNT
#undef ROCKET_FLAME_HEIGHT
#undef ROCKET_FLAME_WIDTH
#undef ROCKET_GRAVITY
#undef ROCKET_LEAD_START
#undef ROCKET_NOZZLE
#undef ROCKET_SCALE
#undef ROCKET_THRUST_START
#undef ROCKET_LIFT_START
#undef ROCKET_NOSE_UP_START
#undef ROCKET_NOSE_UP_END
#undef ROCKET_LIFT_END
#undef ROCKET_SINK_TICKS
#undef ROCKET_TURN_START
#undef ROCKET_BURSTS_START
#undef ROCKET_DIP_START
#undef ROCKET_DIP_END
#undef ROCKET_TURN_END
#undef ROCKET_SOAR_START
#undef ROCKET_BURSTS_END
