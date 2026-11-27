/* The constants the battle effects copy into their locals, in the order of the
   effects that read them: mostly the scales their sprites are placed at, and a
   few corners, points and widths. */

#include "fixed_math.h"
#include "types.h"
#include "battle/render_model.h"
#include "battle_effect/battle_effect.h"
#include "graphics/render_placement.h"

/* A sprite drawn at its own size, which most effects place theirs at. */
#define UNIT_SCALE {toFIXED(1), toFIXED(1)}

/* Cruel Ruin's scale for the summon, copied each tick before it is placed. */
const SCALE CruelRuinUnitScale = UNIT_SCALE;

/* Eclipse's scale for the dragon's sprites, copied each tick before they are
   placed. */
const SCALE EclipseDragonScale = UNIT_SCALE;

/* Unused: two more scales after Eclipse's, each word 0x55555556 (2^32 / 3
   rounded up); nothing reads them. */
const SCALE EclipseUnusedScales[2] = {
	{0x55555556, 0x55555556},
	{0x55555556, 0x55555556},
};

/* The comets' scale. */
const SCALE ProcneUnitScale = UNIT_SCALE;

/* Unused: three words after Procne's scale, 2^32 / 36 and twice 2^32 / 90, each
   rounded up; nothing reads them. */
const Uint32 ProcneUnusedWords[3] = {0x071c71c8, 0x02d82d83, 0x02d82d83};

/* Zagan's scale, copied each tick before he is placed. */
const SCALE ZaganUnitScale = UNIT_SCALE;

/* Boreas's scale, copied each tick before his sprites are placed. */
const SCALE BoreasUnitScale = UNIT_SCALE;

/* Each of the four pillars' heights in pixels. */
const ROCK_WALL_HEIGHTS RockWallHeights = {{-32, -32, -32, -32}};

/* The three lines' end points, a pair of points each, in pixels. */
const Sint16 ThorPoints[6][2] = {
	{8, 135},
	{184, 135},
	{128, 135},
	{96, 32},
	{96, 32},
	{64, 135},
};

/* The scale each of Gaia's rocks starts from before it grows. */
const SCALE GaiaUnitScale = UNIT_SCALE;

/* The white flake's scale as it flutters away at Ulysses's end. */
const SPR_SCALE UlyssesParticleScale = {{UNIT_SCALE}};

/* The mage's sprite scale in Ulysses, copied each tick the mage stands. */
const SCALE UlyssesMageScale = UNIT_SCALE;

/* The scale of the formation's objects that are drawn at their size. */
const SCALE SummonFormationScale = UNIT_SCALE;

/* The scale of the formation's objects drawn a quarter larger. */
const SCALE SummonFormationLargeScale = {
	toFIXED(5) / 4,
	toFIXED(5) / 4,
};

/* The statue's scale in Ramses. */
const SPR_SCALE RamsesUnitScale = {{UNIT_SCALE}};

/* The formation objects' scale in Judgment. */
const SCALE JudgmentUnitScale = UNIT_SCALE;

/* Kirin's scale as he bounds in and lifts away. */
const SPR_SCALE KirinSweepScale = {{UNIT_SCALE}};

/* Kirin's scale while he charges, before it is changed by tick. */
const SPR_SCALE KirinChargeScale = {{UNIT_SCALE}};

/* The serpent's scale. */
const SCALE NeptuneUnitScale = {toFIXED(3) / 2, toFIXED(3) / 2};

/* Megaera's scale as she falls, one and a half. */
const SCALE MegaeraUnitScale = {toFIXED(3) / 2, toFIXED(3) / 2};

/* The hexagon's six corners, {z, y} in pixels. */
const Sint8 VoidBeamCorners[6][2] = {
	{40, -40},
	{0, 40},
	{-40, -40},
	{40, 40},
	{0, -40},
	{-40, 40},
};

/* Charon's swarm scale, before it is set to one and a quarter. */
const SCALE CharonSwarmScale = UNIT_SCALE;

/* Charon's pulse scale, before the pulse sets it from a sine. */
const SCALE CharonPulseScale = UNIT_SCALE;

/* Charon's scale while he is revealed. */
const SCALE CharonRevealScale = UNIT_SCALE;

/* Charon's scale in the finale. */
const SCALE CharonFinaleScale = UNIT_SCALE;

/* Flora's scale. */
const SCALE FloraUnitScale = UNIT_SCALE;

/* The scale of the forest canopy Daedalus flies over. */
const SCALE DaedalusCanopyScale = UNIT_SCALE;

/* The scale of the pieces of Daedalus's machine. */
const SCALE DaedalusMachineScale = UNIT_SCALE;

/* The scale of Daedalus's missiles, a tenth larger than their pictures. */
const SCALE DaedalusMissileScale = {
	toFIXED(11) / 10,
	toFIXED(11) / 10,
};

/* The rocket's scale in Daedalus's second strike, twice its size. */
const SCALE DaedalusStrikeRocketScale = {toFIXED(2), toFIXED(2)};

/* Coatlicue's scale. */
const SCALE CoatlicueUnitScale = UNIT_SCALE;

/* Catastrophe's scale for the summon. */
const SCALE CatastropheUnitScale = UNIT_SCALE;

/* Haures's scale while he charges. */
const SCALE HauresChargeScale = UNIT_SCALE;

/* Haures's scale while he circles. */
const SCALE HauresOrbitScale = UNIT_SCALE;

/* Haures's scale in the opening, before it is flipped. */
const SCALE HauresOpeningScale = UNIT_SCALE;

/* The scale of Trident's four prongs. */
const SCALE TridentUnitScale = UNIT_SCALE;

/* The summon's scale in Meteor Blow, twice its size. */
const SCALE MeteorBlowDoubleScale = {toFIXED(2), toFIXED(2)};

/* A mote's size by its number's low three bits. */
const Uint8 MolochMoteSizes[8] = {2, 3, 5, 8, 7, 6, 4, 0};

/* A pillar's six corners, {x, y, z} in pixels: three rows of two, 16 either
   side of its centre, at y -16, 12 and 0. */
const MODEL_VERTEX IrisPillarVertices[6] = {
	{-16, -16, 0},
	{16, -16, 0},
	{-16, 12, 0},
	{16, 12, 0},
	{-16, 0, 0},
	{16, 0, 0},
};

/* Iris's scale while she rises. */
const SCALE IrisUnitScale = UNIT_SCALE;

/* The rising pillars' widths, left to right. */
const FIVE_WIDTHS IrisRisingPillarWidths = {{2, 3, 4, 5, 6}};

/* The widening pillars' widths, left to right. */
const SEVEN_WIDTHS IrisWideningPillarWidths = {{2, 3, 4, 5, 6, 7, 8}};

/* The narrowing pillars' widths, left to right. */
const SEVEN_WIDTHS IrisNarrowingPillarWidths = {{8, 7, 6, 5, 4, 3, 2}};

/* The left strip's four spans where petals start, {left, width} in pixels. */
const FOUR_SPANS IrisLeftStripSpans = {
	{{112, 27}, {104, 28}, {92, 27}, {74, 27}},
};

/* The right strip's four spans where petals start, {left, width} in pixels. */
const FOUR_SPANS IrisRightStripSpans = {
	{{196, 44}, {176, 48}, {152, 45}, {116, 45}},
};
