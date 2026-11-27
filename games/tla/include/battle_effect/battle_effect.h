#ifndef ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_H

// The battle effects as the rest of the battle calls them: a runner for each Psynergy, Djinni,
// summon and monster ability, and the canvas, palette, particle and model helpers they share.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/particle.h"
#include "battle/battle_effect.h"
#include "battle/battle_unit.h"
#include "battle_effect/battle_effect_ids.h"
#include "system.h"

struct BATTLE_EFFECT_PARM;

// The battle-effect module's functions in the order of its stub table (src/battle_effect/stubs.s).
// Writes a drawing function for the canvas into heap slot `slot`, built from BlitterTemplate for
// one clip, mirroring and blend mode.
Sint32
btEffectBuildBlitter(Sint32 slot, Sint32 widthShift, Sint32 heightShift, Sint32 flags, Uint32 mode);
// Runs a battle effect from the second range of kinds: the casting impacts, the plain attacks'
// beams and their critical hits' ring and sparks, and the canvas burst.
void   btEffectRunByIdRange(struct BATTLE_EFFECT_PARM *args);
// Runs one of the large battle effects, the summons and the scenes of a few abilities: takes the
// effect's sprite sheet, state and canvas, runs the effect of args->kind (SUMMON_EFFECT_ID) and
// frees them again.
void   btEffectRunSummon(struct BATTLE_EFFECT_PARM *args);
// Runs an ordinary battle effect from BattleEffectHandlers, with the sprite sheet, state and canvas
// it lends.
void   btEffectRunFromTable(struct BATTLE_EFFECT_PARM *args);
// A pillar of light in an element's colours stands at a unit's feet while motes spiral up around
// it. The battle's element glow and the auto battle run it, and it sets up its own heap blocks.
void   btEffectRunElementPillar(struct BATTLE_EFFECT_PARM *args);
// As the battle with the Doom Dragon opens over the Serpent's stars, the screen turns white and
// breaks into pieces that fly apart and fall, while a grey disc grows over it and the battle
// background's colours return.
void   btEffectRunShatteringOpening(struct BATTLE_EFFECT_PARM *args);
// Sparks in an element's colours flare at four corners around a unit and motes spiral in, then a
// burst of light plays over it. The battle's element glow runs it, and it sets up its own heap
// blocks.
void   btEffectRunElementBurst(struct BATTLE_EFFECT_PARM *args);
// Starts the element streaks, which then draw by themselves while the battle runs on.
void   btEffectStartElementStreaks(Sint32 kind);
// Ends the element streaks: removes their three events, clears the canvas in VRAM and frees what
// they took.
void   btEffectEndElementStreaks(void);
// btEffectBuildBlitter for a 128 by 128 canvas, its mode and flags packed as mode << 4 | flags.
Sint32 btEffectLoadBlitter(Sint32 slot, Sint32 modeAndFlags);
// The Doom Dragon's change of form, played over the dragon once a form is beaten. A glow widens
// over it while rays of light spin out from it, and it flashes white; at FORM_CHANGE_BURST the
// screen shakes, flashes burst upward and four columns of light rise from the ground one after
// another, spinning. After its first and second forms (FORMATION_DOOM_DRAGON_A and B) the glow
// stands to its right and to its left, and the battle swaps in the next form at FORM_CHANGE_SWAP;
// after its last (FORMATION_DOOM_DRAGON_C) the glow stands over it, the screen whitens, and it puts
// the battle's view back itself.
void   btEffectRunFormChange(struct BATTLE_EFFECT_PARM *args, Sint32 kind);

struct SCREEN_POINT;
struct PARTICLE;
struct MESH_TRIANGLE;
struct MESH_VERTEX;
struct RENDER_MODEL;
struct STRIP_QUAD;

// The summons' sprite formations (btEffectPlaceFormationSprites and btEffectRunImpactBurst), each
// named for the summon that keeps it.
typedef enum SUMMON_FORMATION
{
    SUMMON_FORMATION_THOR,    // nine in a 3x3 grid at 32-pixel spacing
    SUMMON_FORMATION_NEREID,  // twelve in rows of 1, 3, 4 and 4
    SUMMON_FORMATION_CYBELE,  // eight in rows of 3, 2 and 3
    SUMMON_FORMATION_TIAMAT,  // eight drawn at 1.25 times their size
    SUMMON_FORMATION_MEGAERA, // eleven from signed columns
    SUMMON_FORMATION_MOLOCH,  // twelve in a 4x3 grid
    // Tiamat's eight when the first target's animation kind is 2.
    SUMMON_FORMATION_TIAMAT_ALTERNATE
} SUMMON_FORMATION;

typedef DRAW_RECTANGLE BLITTER_PAIR[2];

// A texture as the render jobs take it: 1 << logWidth by 1 << logHeight.
typedef struct TEXTURE
{
    Uint8  logWidth;
    Uint8  logHeight;
    Uint16 padding;
    void  *pixels;
} TEXTURE;

// A point or a velocity, 16.16 pixels.
typedef struct POINT2_D
{
    FIXED x;
    FIXED y;
} POINT2_D;

// A sprite's scale across and down, 16.16, as an effect keeps it in a local (SPR_SCALE is the same
// pair as a union with an array).
typedef struct SCALE
{
    FIXED x;
    FIXED y;
} SCALE;

// An effect's variant (BATTLE_EFFECT_PARM.variant): abilities of one family that share an effect,
// such as Quake, Earthquake and Quake Sphere, and a double or triple blow adds one or two. A runner
// that sets its own variants names them beside it.
typedef enum EFFECT_VARIANT
{
    EFFECT_VARIANT_BASIC,    // Quake, Douse, Bolt
    EFFECT_VARIANT_STRONGER, // Earthquake, Drench, Flash Bolt
    EFFECT_VARIANT_STRONGEST // Quake Sphere, Deluge, Blue Bolt
} EFFECT_VARIANT;

// btMotionApplyKnockback's knockbacks, the columns of the BattleKnockback* tables. Each is named by
// how far and how high it sends the unit, as percent of its spot's distance from the centre and its
// hop in pixels a tick; all but KNOCKBACK_HURL and KNOCKBACK_HURL_FAR accelerate by 1 pixel a tick
// to at most 4.
typedef enum BATTLE_KNOCKBACK
{
    KNOCKBACK_NUDGE,  // 140 percent, a hop of 3
    KNOCKBACK_HOP,    // 140 percent, a hop of 6
    KNOCKBACK_LEAP,   // 150 percent, a hop of 7
    KNOCKBACK_LAUNCH, // 200 percent, a hop of 10
    // The hardest: 200 percent at 8 pixels a tick more each tick, to at most 20, with a hop of 5.
    KNOCKBACK_HURL,
    // 150 percent, a hop of 2. From here on a unit already in the air takes the new hop too.
    KNOCKBACK_SKID,
    KNOCKBACK_BOUNCE, // the spot itself, a hop of 3
    KNOCKBACK_JOLT,   // the spot itself, a hop of 5.5
    // Unused: the spot itself, a hop of 10; no knockback names it.
    KNOCKBACK_LIFT,
    // Unused: 300 percent, as KNOCKBACK_HURL otherwise; no knockback names it.
    KNOCKBACK_HURL_FAR
} BATTLE_KNOCKBACK;

// Rebuilds the background palette's first ramp as a grey whose channels sway with the tick, each at
// its own phase, so the effect shimmers through colours.
void btEffectUpdatePhasePalette(Sint32 frame, Sint32 redPhase, Sint32 greenPhase, Sint32 bluePhase);
// Opens the canvas layer over the battle, darkening the battle background behind it.
void btEffectStartCanvasLayer(Sint32 bgControl);
// Closes the canvas layer and brings the battle background's colours back.
void btEffectEndCanvasLayer(void);
// Counts down each target's flash timer and gives it its own palette back when one runs out.
void btEffectCountDownBattlerTints(void);
// Tints a unit's battle object, sets its pose and starts its flash timer.
void btEffectTintBattler(
    Sint32 actor, Sint32 palette, Sint32 pose, Sint32 timerSlot, Sint32 timerTicks);
// Draws the upper left quarter of a shaded disc into `map`, its rings darker outward by a cosine
// falloff scaled by `scale`.
void btEffectDrawShadedCircles(Uint8 *map, Sint32 size, FIXED scale);
// Copies a width by height picture into one twice its size, mirrored into all four quadrants.
void btPictureMirrorIntoQuadrants(Uint8 *src, Uint8 *dest, Sint32 width, Sint32 height);
// A Djinni gathers out of sparks at an anchor, sways and bursts into motes (DJINNI_ENTRANCE packs
// which, its side and where); the anchor's screen and world points are returned.
void btEffectRunDjinniEntrance(struct BATTLE_EFFECT_PARM *effect,
                               Uint32                     packed,
                               Sint32                    *screen,
                               Sint32                    *world);
// Draws a quarter picture four times into the canvas, mirrored about (x, y), keeping the brighter
// pixel: a whole glow centred there.
void btCanvasBlitMirroredQuarters(void *src, Sint32 x, Sint32 y, Sint32 size);
// Allocates `count` model entries and a cleared one that ends the list.
struct RENDER_MODEL *btRenderAllocateModelList(Sint32 count);
// Runs `count` packed vertices through the current transform into screen coordinates, with the ARM
// kernel copied into the heap.
void                 btRenderTransformVertices(const void *corners, void *vertices, Sint32 count);
// Projects a unit's place onto the screen, 16 pixels higher.
void                 btEffectPosProjectBattler(Sint32 id, SCREEN_POINT *pos);
// Creates `count` sprites of `spriteId` in the effect's state, the nth showing its animation n.
void                 btEffectCreateSprites(Sint32 count, Sint32 spriteId, Uint32 priority);
// Shows the battle objects of every target with HP left.
void                 btEffectShowLivingTargets(struct BATTLE_EFFECT_PARM *args);
// Knocks a unit back in its damage pose with the caller's acceleration, top speed, hop and reach in
// percent, as btMotionApplyKnockback does from its tables.
void                 btEffectStartActorMotion(Sint32 id,
                                              Sint32 force,
                                              Sint32 acceleration,
                                              Sint32 speedLimit,
                                              Sint32 velocityY,
                                              Sint32 percent);
// Builds the two usual canvas blitters, mirrored when `flip` is set: out[0] keeps the brighter
// level, out[1] adds the levels.
void                 btEffectBuildCanvasBlitters(Sint32 flip, DRAW_RECTANGLE *out);
// Shakes the battle view by random offsets in the two ranges, powers of two.
void                 btCameraShake(Sint32 rangeX, Uint32 rangeY);
// Moves a particle one tick on the screen: its velocity scaled by `damping` sixty-fourths, then
// `gravity` added to its y speed.
void                 btParticleAdvanceWithGravity2D(PARTICLE *step, Sint32 damping, Sint32 gravity);
// Builds `count` quads of a vertex strip, each width by height texels, and ends the list with an
// empty triangle.
void btRenderBuildQuadStrip(struct STRIP_QUAD *quad, Sint32 width, Sint32 height, Sint32 count);
// Moves a particle one tick in three dimensions: its velocity scaled by `damping` sixty-fourths,
// then `gravity` added to its y speed.
void btParticleAdvanceWithGravity3D(PARTICLE *step, Sint32 damping, Sint32 gravity);
// Opens the canvas layer with the canvas repeated across a wrapping map.
void btEffectStartTiledCanvas(Sint32 bgControl);
// As btEffectStartTiledCanvas, the rest of each row showing the canvas's top right tile.
void btEffectStartTiledCanvasFilled(Sint32 bgControl);
// Fills the canvas with a white band, its edges fading, that the summons sweep in from the right to
// white the battle out.
void btEffectFillWhiteBand(void);
// Scrolls BG1 a step every few frames.
void btEffectAdvanceScrollOnInterval(void);
// Brightens the battle background's colours by a step in each channel, or darkens them by a
// negative one.
void btPaletteBrightenBattleBg(Sint32 redDelta, Sint32 greenDelta, Sint32 blueDelta);
// As btPaletteBrightenBattleBg, from the background's own colours as the battle keeps them.
void btPaletteBrightenSavedBattleBg(Sint32 redDelta, Sint32 greenDelta, Sint32 blueDelta);
// Builds the mesh of an open cylinder of `segments` sides, the band Coatlicue shows.
void btMeshBuildCylinderBand(struct MESH_VERTEX   *vertices,
                             struct MESH_TRIANGLE *triangles,
                             Sint32                segments);
// Loads a grey ramp into the background colours from colour 1.
void btGraphicsLoadGreyRamp(void);
// Fills a grid of columns by rows vertices with two triangles a cell, each cellWidth by cellHeight
// texels, and ends the list.
void btMeshBuildGrid(struct MESH_TRIANGLE *triangles,
                     Sint32                columns,
                     Sint32                rows,
                     Sint32                cellWidth,
                     Sint32                cellHeight);
// Brightens background colours 1 to 63 by a step in each channel, or darkens them by a negative
// one.
void btPaletteBrightenBgEntries(Sint32 blueDelta, Sint32 greenDelta, Sint32 redDelta);
// Puts the battle screen back into the display state every effect starts from and ends with.
void btEffectConfigureEffectDisplay(void);
// Clears every target's flash timer.
void btEffectClearTintTimers(void);
// Draws a line on the canvas, keeping the brighter colour where it crosses what is drawn.
void btEffectDrawClippedCanvasLine(Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 color);
// Sets the registers that show the canvas on BG2.
void btEffectSetupCanvasWindows(void);
// Draws the whole disc btEffectDrawShadedCircles draws a quarter of.
void btEffectDrawShadedCirclesMirrored(Uint8 *map, Sint32 size, Sint32 scale);

// The effects with a runner of their own: the summons, Daedalus's second strike, and Trident, Cruel
// Ruin and Meteor Blow.
void btEffectRunRamses(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunNereid(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunKirin(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunAtalanta(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunNeptune(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunProcne(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunJudgment(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunBoreas(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunThor(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunMegaera(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunFlora(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunMoloch(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunUlysses(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunHaures(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunEclipse(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunCoatlicue(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunDaedalus(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAzul(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunCatastrophe(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunCharon(struct BATTLE_EFFECT_PARM *arg);
void btEffectRunIris(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunDaedalusStrike(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunTrident(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunCruelRuin(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunMeteorBlow(struct BATTLE_EFFECT_PARM *effect);

// Effects several abilities share, those that take a `mode` told apart by it.
void btEffectRunDjinnBeams(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunLobbedShells(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
void btEffectRunImpair(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDull(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGatheringLights(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// Arms the HBlank DMA that scrolls BG1 line by line.
void btEffectArmBg1HBlankDMA(void);
// Reverses the first 64 background colours, keeping the backdrop colour.
void btPaletteMirrorBgColors(void);
// Shows the battle background, as dark as the canvas's fade left it, behind a cleared canvas, and
// fades out the action's sound effects.
void btEffectSetupBlendedDisplay(void);
// Fireballs rain on the targets and burn where they land, then a blast of fire sweeps across them.
void btEffectRunDragonFire(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// Arranges the effect's sprites in a formation (SUMMON_FORMATION) about (x, y), 16.16 pixels.
void btEffectPlaceFormationSprites(Sint32 formation, Sint32 x, Sint32 y);
// Opens an impact flash and scatters sparks and fragments, keeping the effect's sprites in
// formation for the first 36 ticks.
void btEffectRunImpactBurst(Sint32 formation, Sint32 x, FIXED y);
// Fires streaks at each target in turn from above the user.
void btEffectRunStreakVolley(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// Motes burst out of the user and float upwards, and each target reacts.
void btEffectRunMoteFountain(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// The screen point midway between the first and last targets.
void btEffectGetTargetsMidpoint(SCREEN_POINT *midpoint);
// The revival of Balm, Tinder and Phoenix: comets or feathers tumble round the targets, and every
// target flashes.
void btEffectRunTumblingQuads(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// The user strikes each target with the extras its mode's style chooses.
void btEffectRunTargetStrike(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// Moves a unit's object `percent` percent of the way to the target's in `ticks` ticks.
void btEffectSetApproachMotion(Sint32 actor, Sint32 target, Sint32 ticks, Sint32 percent);
// Unused: a turning cloud swirl on BG2 until A is pressed.
void btEffectRunCloudSwirl(struct BATTLE_EFFECT_PARM *effect);
// Turns the battle camera's yaw by the effect's delta, at least half of it every VBlank.
void btCameraApplyPhasedDelta(void);

// The window btEffectBuildCenteredIndexList builds a list of indices around: its value, radius and
// limit, and the values it lists. Nothing calls it, so nothing says what the first and last three
// words hold.
typedef struct BATTLE_EFFECT_INDEX_WINDOW
{
    Sint32 unusedBefore[3];
    Sint32 currentValue;
    Sint32 indexRadius;
    Sint32 entryLimit;
    Sint32 unusedAfter[3];
    Sint16 values[1];
} BATTLE_EFFECT_INDEX_WINDOW;

// What btEffectRunDjinniEntrance takes added to its element; the element alone plays the 55-tick
// entrance.
typedef enum DJINNI_ENTRANCE_LENGTH
{
    // The long entrance: 84 ticks instead of 55.
    DJINNI_ENTRANCE_LONG = ELEMENT_COUNT
} DJINNI_ENTRANCE_LENGTH;

// Where btEffectRunDjinniEntrance's Djinni appears; the first five anchors stand it 60 pixels up.
typedef enum DJINNI_ENTRANCE_ANCHOR
{
    DJINNI_ANCHOR_OVER_TARGET, // over the first target
    DJINNI_ANCHOR_TARGET_X,    // at the first target's x, at depth 0
    DJINNI_ANCHOR_OVER_USER,   // over the user
    DJINNI_ANCHOR_USER_X,      // at the user's x, at depth 0
    DJINNI_ANCHOR_CENTRE,      // in the centre of the field
    DJINNI_ANCHOR_HIGH_USER_X, // at the user's x, 80 pixels up
    // In the centre: the entrance takes 40 ticks with no fade, and the backdrop brightens from
    // tick 24.
    DJINNI_ANCHOR_CENTRE_BRIGHTENING,
    DJINNI_ANCHOR_LOW_CENTRE, // in the centre, 20 pixels up
    // Over the first target: the entrance takes 54 ticks.
    DJINNI_ANCHOR_OVER_TARGET_SHORT,
    DJINNI_ANCHOR_HIGH_TARGET_X, // at the first target's x, 120 pixels up
    DJINNI_ANCHOR_LOW_USER_X     // at the user's x, 30 pixels up
} DJINNI_ENTRANCE_ANCHOR;

// btEffectRunDjinniEntrance's packed argument: which Djinni (an element, plus DJINNI_ENTRANCE_LONG
// for the longer entrance), the side whose way it faces, and where it appears
// (DJINNI_ENTRANCE_ANCHOR).
#define DJINNI_ENTRANCE(element, side, anchor) ((element) | (side) << 4 | (anchor) << 5)
#define DJINNI_ENTRANCE_ELEMENT(packed)        ((packed) & 7)
#define DJINNI_ENTRANCE_SIDE(packed)           (((packed) >> 4) & 1)
#define DJINNI_ENTRANCE_ANCHOR_OF(packed)      (((packed) >> 5) & 0xf)

// A row of QuakeTimings, one for each variant.
enum
{
    QUAKE_TIMING_TICKS,      // the whole effect
    QUAKE_TIMING_RISE_TICKS, // how long each pillar stands
    QUAKE_TIMING_SHAKE       // the camera shake
};

// The four rock pillars' heights, in pixels.
typedef struct ROCK_WALL_HEIGHTS
{
    Sint32 column[4]; // each pillar's height in pixels
} ROCK_WALL_HEIGHTS;

// The five widths a rising pillar takes.
typedef struct FIVE_WIDTHS
{
    Sint32 widths[5];
} FIVE_WIDTHS;

// The seven widths a widening pillar takes.
typedef struct SEVEN_WIDTHS
{
    Sint32 widths[7];
} SEVEN_WIDTHS;

// Where a strip's petals start across, by quarter of its rise: the left edge and the width they
// spread over.
typedef struct FOUR_SPANS
{
    struct
    {
        Sint32 left;
        Sint32 width;
    } spans[4];
} FOUR_SPANS;

// A row of FrostTimings, one for each variant.
enum
{
    FROST_TIMING_MOTES,     // the rising motes
    FROST_TIMING_COLUMNS,   // the pillars
    FROST_TIMING_BURST_TICK // the tick they burst
};

// A row of EruptionCounts, one for each variant.
enum
{
    ERUPTION_COUNT_MOTES,
    ERUPTION_COUNT_PUFFS,
    ERUPTION_COUNT_TICKS
};

// A growing column, laid over PARTICLE in state->particles.
typedef struct DUAL_COLUMN
{
    Sint32 x;                    // the root on the canvas
    Sint32 y;                    // the root on the canvas
    Uint8  unused[8];            // no code names it
    Sint32 height;               // the full length
    Uint8  unusedAfterHeight[4]; // no code names it
    Sint32 start;                // the tick it starts to grow
} DUAL_COLUMN;

// Steps background colours 0 to 63 one level towards a palette resource.
void               btEffectStepPaletteToResource(Sint32 resourceId);
// Wipes the canvas across or down, leaving it as `fill` says (enum CANVAS_FILL).
void               btEffectWipeCanvas(Sint32 direction, Sint32 fill);
// Projects a unit's sprite onto the screen, raised by half its height and 16 pixels more.
void               btEffectPosProjectSprite(Sint32 id, SCREEN_POINT *pos);
// Builds window 0's right edge for every line and restarts the DMA that writes it.
void               btEffectBuildWindowEdgeTable(void);
// Motes burst out of each target in turn and are pulled into the user, who glows white as they
// arrive.
void               btEffectRunDrainMotes(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// Boreas's scale, copied each tick before his sprites are placed.
extern const SCALE BoreasUnitScale;
// Flares of light swirl in to each target in turn and burst, and the mode's picture plays over it.
void               btEffectRunStatBoost(struct BATTLE_EFFECT_PARM *effect, Uint32 mode);
// Each of the four rock pillars' heights in pixels.
extern const ROCK_WALL_HEIGHTS RockWallHeights;
// A fountain of motes and smoke bursts up beside the targets and falls back on them one after
// another.
void                           btEffectRunEruption(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
// Vines or green pillars grow one after another, alternately mirrored, each flaring at its root.
void btEffectRunGrowingColumns(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);

// The effect handlers, in the order the handler table (BattleEffectHandlers) first lists them.

void btEffectRunQuake(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSpire(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGaia(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGrowth(struct BATTLE_EFFECT_PARM *args);
void btEffectRunThorn(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFrost(struct BATTLE_EFFECT_PARM *args);
void btEffectRunIce(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPrism(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDouse(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFroth(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFlare(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFire(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVolcano(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBlast(struct BATTLE_EFFECT_PARM *args);
void btEffectRunNova(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBolt(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRay(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPlasma(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSlash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWhirlwind(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCure(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPly(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWish(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCurePoison(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRevive(struct BATTLE_EFFECT_PARM *args);
void btEffectRunImpact(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDull(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGuard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunImpair(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWeaken(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTaint(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDelude(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedOrbitingSpirals(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeartrender(struct BATTLE_EFFECT_PARM *args);
void btEffectRunParalyze(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSleep(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBind(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHaunt(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCondemn(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCurse(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDrain(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBreak(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDragonCloud(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDemonNight(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHelmSplitter(struct BATTLE_EFFECT_PARM *args);
void btEffectRunQuickStrike(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDeathPlunge(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShuriken(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAnnihilation(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPunji(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCuttingEdge(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRagnarok(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeatWave(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPlanetDiver(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTitanBlade(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShiningStar(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBlizzard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDemonFire(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVorpalSlash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDrown(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedTriangleStrike(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAstralBlast(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeartbreak(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAura(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFume(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBeam(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRagingHeat(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCool(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSabreDance(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBackstab(struct BATTLE_EFFECT_PARM *args);
void btEffectRunJuggle(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFlameCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWhiplash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonFlow(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFirePuppet(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDiamondDust(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDiamondBerg(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPlumeEdge(struct BATTLE_EFFECT_PARM *args);
void btEffectRunThunderMine(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFrostCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBrambleCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunThunderCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBaffleCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSwordCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSleepCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDeathCard(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStoneJustice(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeavyDivide(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWyrdCurse(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAcheronsGrief(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHurricane(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDreamtide(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLetheAlbion(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRisingDragon(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBoostHack(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRadiantFire(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFireDance(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBlazeRush(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShred(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPurgatory(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSoulShatter(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAgingGas(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMadZephyr(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLunarSlash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunNirvana(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLightSurge(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVoidBeam(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLegend(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLiquifier(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMegiddo(struct BATTLE_EFFECT_PARM *args);
void btEffectRunOdyssey(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVengeance(struct BATTLE_EFFECT_PARM *args);
void btEffectRunThornyGrave(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPlanetary(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLifeShear(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCrucibleFire(struct BATTLE_EFFECT_PARM *args);
void btEffectRunReverseStar(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGranite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVine(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGround(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSleet(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHail(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTonic(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDew(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPsyCrystal(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFlash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTorch(struct BATTLE_EFFECT_PARM *args);
void btEffectRunZephyr(struct BATTLE_EFFECT_PARM *args);
void btEffectRunKite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGust(struct BATTLE_EFFECT_PARM *args);
void btEffectRunIron(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSteam(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShade(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCoal(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSalt(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSpring(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunEther(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBalm(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTinder(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWhorl(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGale(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWaft(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCore(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMud(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHaze(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPetra(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRime(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLull(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGasp(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFugue(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCrystal(struct BATTLE_EFFECT_PARM *args);
void btEffectRunEddy(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCannon(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAroma(struct BATTLE_EFFECT_PARM *args);
void btEffectRunReflux(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDjinnCounter(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMold(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMeld(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShine(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGeode(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSerac(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSpark(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFury(struct BATTLE_EFFECT_PARM *args);
void btEffectRunChar(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVenus(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMercury(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMars(struct BATTLE_EFFECT_PARM *args);
void btEffectRunJupiter(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSonicWave(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShriek(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBansheeHowl(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCrazyVoice(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWarCry(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWickedHowl(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWingBeat(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWingFlutter(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRumble(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBoneChiller(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBoneCharge(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMysticFlame(struct BATTLE_EFFECT_PARM *args);
void btEffectRunNumbingSting(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBruteForce(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStickyGoo(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCannibalFang(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBearClaw(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonousBite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFlyingAttack(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUndeadSword(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRansack(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStickyPoison(struct BATTLE_EFFECT_PARM *args);
void btEffectRunElectricBite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonTail(struct BATTLE_EFFECT_PARM *args);
void btEffectRunOnslaught(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVampiricFang(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBacteriaRush(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRottenBlood(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDoubleFang(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMortalBlow(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFreebiteRush(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTwinBeaks(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRabidFang(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAcidBite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDynamite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeadbutt(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonInk(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSoothingStar(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSpiderWeb(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMadSpatter(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSpasm(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSleepStar(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDecompose(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHaunting(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBerserk(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSmokeBomb(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWingStroke(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonBeat(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSpinningBeat(struct BATTLE_EFFECT_PARM *args);
void btEffectRunOuterSpace(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDragonDriver(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDrainFang(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSevereBlow(struct BATTLE_EFFECT_PARM *args);
void btEffectRunThrash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRecovery(struct BATTLE_EFFECT_PARM *args);
void btEffectRunEchoCut(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWateryGrave(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRagingFlood(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHydroSlash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMightyPress(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSlaver(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCounterRush(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDjinnfest(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeatKiss(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAngleSpike(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFireBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunWaterBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunIceBlessing(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDarkBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunAcidBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStormBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunEvilBlessing(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDeadlyGas(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSandBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDesertGasp(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBlackIce(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTyphoonBlow(struct BATTLE_EFFECT_PARM *args);
void btEffectRunToxicBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGravelBlow(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBlastBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDarksolGasp(struct BATTLE_EFFECT_PARM *args);
void btEffectRunManeater(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUrFlash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCrusherGrip(struct BATTLE_EFFECT_PARM *args);
void btEffectRunVanishClaw(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTwinShear(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTripleChomp(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDoublestep(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonGel(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRisingVenom(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSack(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFireDanceUnleash(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStunMuscle(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRollingFlame(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCage(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPhoenix(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHumanHunt(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedHeavyBite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedFlashBite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunKillSting(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBeastNeedle(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPoisonSting(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStunSting(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDemonEye(struct BATTLE_EFFECT_PARM *args);
void btEffectRunEmu(struct BATTLE_EFFECT_PARM *args);
void btEffectRunOceanFist(struct BATTLE_EFFECT_PARM *args);
void btEffectRunCallDullahan(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGhostSoldier(struct BATTLE_EFFECT_PARM *args);
void btEffectRunRisingDragonEnemy(struct BATTLE_EFFECT_PARM *args);
void btEffectRunTrueCollide(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStarMine(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFormicAcid(struct BATTLE_EFFECT_PARM *args);
void btEffectRunClawAttack(struct BATTLE_EFFECT_PARM *args);
void btEffectRunBeatDance(struct BATTLE_EFFECT_PARM *args);
void btEffectRunUnusedSparkBite(struct BATTLE_EFFECT_PARM *args);
void btEffectRunLivingArmor(struct BATTLE_EFFECT_PARM *args);
void btEffectRunHeatStun(struct BATTLE_EFFECT_PARM *args);
void btEffectRunEarthForce(struct BATTLE_EFFECT_PARM *args);
void btEffectRunGuardAura(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDjinnBlast(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDjinnStorm(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMineBall(struct BATTLE_EFFECT_PARM *args);
void btEffectRunElementSwap(struct BATTLE_EFFECT_PARM *args);
void btEffectRunDjinnStun(struct BATTLE_EFFECT_PARM *args);
void btEffectRunMysticCall(struct BATTLE_EFFECT_PARM *args);
void btEffectRunClarionCry(struct BATTLE_EFFECT_PARM *args);
void btEffectRunPsyBoost(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStrongHit(struct BATTLE_EFFECT_PARM *args);
void btEffectRunChillBreath(struct BATTLE_EFFECT_PARM *args);
void btEffectRunStormBreathStrong(struct BATTLE_EFFECT_PARM *args);

#endif
