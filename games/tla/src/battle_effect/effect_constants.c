/// @file
/// The constants the battle effects copy into their locals, in the order of
/// the effects that read them: mostly the scales their sprites are placed at,
/// and a few corners, points and widths.

#include "fixed_math.h"
#include "types.h"
#include "battle/render_model.h"
#include "battle_effect/battle_effect.h"
#include "graphics/render_placement.h"

/// A sprite drawn at its own size, which most effects place theirs at.
#define UNIT_SCALE {FIXED_ONE, FIXED_ONE}

/// Cruel Ruin's scale for the summon, copied each frame before it is placed.
const struct Scale gCruelRuin_UnitScale = UNIT_SCALE;

/// Eclipse's scale for the dragon's sprites, copied each frame before they are
/// placed.
const struct Scale gEclipse_Scales = UNIT_SCALE;

/// Unused: two more scales after Eclipse's, each word 0x55555556 (2^32 / 3
/// rounded up); nothing reads them.
const struct Scale gEclipse_UnusedScales[2] = {
    {0x55555556, 0x55555556},
    {0x55555556, 0x55555556},
};

/// The comets' scale.
const struct Scale gProcne_UnitScale = UNIT_SCALE;

/// Unused: three words after Procne's scale, 2^32 / 36 and twice 2^32 / 90,
/// each rounded up; nothing reads them.
const u32 gProcne_UnusedWords[3] = {0x071c71c8, 0x02d82d83, 0x02d82d83};

/// Zagan's scale, copied each frame before he is placed.
const struct Scale gZagan_UnitScale = UNIT_SCALE;

/// Boreas's scale, copied each frame before his sprites are placed.
const struct Scale gBoreas_UnitScale = UNIT_SCALE;

/// Each of the four pillars' heights in pixels.
const struct RockWallHeights gRockWall_Heights = {{-32, -32, -32, -32}};

/// The three lines' end points, a pair of points each, in pixels.
const s16 gThor_Points[6][2] = {
    {8, 135}, {184, 135}, {128, 135}, {96, 32}, {96, 32}, {64, 135},
};

/// The scale each of Gaia's rocks starts from before it grows.
const struct Scale gGaia_UnitScale = UNIT_SCALE;

/// The white flake's scale as it flutters away at Ulysses's end.
const struct SpriteScale gUlysses_ParticleScale = {{UNIT_SCALE}};

/// The ship's scale in Ulysses, copied each frame it sails.
const struct Scale gUlysses_ShipScale = UNIT_SCALE;

/// The scale of the formation's objects that are drawn at their size.
const struct Scale gFormationObjects_Scale = UNIT_SCALE;

/// The scale of the formation's objects drawn a quarter larger.
const struct Scale gFormationObjects_LargeScale = {
    FIXED_ONE * 5 / 4,
    FIXED_ONE * 5 / 4,
};

/// The statue's scale in Ramses.
const struct SpriteScale gRamses_UnitScale = {{UNIT_SCALE}};

/// The formation objects' scale in Judgment.
const struct Scale gJudgment_UnitScale = UNIT_SCALE;

/// Kirin's scale as he bounds in and lifts away.
const struct SpriteScale gKirin_SweepScale = {{UNIT_SCALE}};

/// Kirin's scale while he charges, before it is changed by frame.
const struct SpriteScale gKirin_ChargeScale = {{UNIT_SCALE}};

/// The serpent's scale.
const struct Scale gNeptune_UnitScale = {FIXED_ONE * 3 / 2, FIXED_ONE * 3 / 2};

/// Megaera's scale as she falls, one and a half.
const struct Scale gMegaera_UnitScale = {FIXED_ONE * 3 / 2, FIXED_ONE * 3 / 2};

/// The hexagon's six corners, {z, y} in pixels.
const s8 gVoidBeam_Corners[6][2] = {
    {40, -40}, {0, 40}, {-40, -40}, {40, 40}, {0, -40}, {-40, 40},
};

/// Charon's swarm scale, before it is set to one and a quarter.
const struct Scale gCharon_SwarmScale = UNIT_SCALE;

/// Charon's pulse scale, before the pulse sets it from a sine.
const struct Scale gCharon_PulseScale = UNIT_SCALE;

/// Charon's scale while he is revealed.
const struct Scale gCharon_RevealScale = UNIT_SCALE;

/// Charon's scale in the finale.
const struct Scale gCharon_FinaleScale = UNIT_SCALE;

/// Flora's scale.
const struct Scale gFlora_UnitScale = UNIT_SCALE;

/// The scale of the forest canopy Daedalus flies over.
const struct Scale gDaedalus_CanopyScale = UNIT_SCALE;

/// The scale of the pieces of Daedalus's machine.
const struct Scale gDaedalus_MachineScale = UNIT_SCALE;

/// The scale of Daedalus's missiles, a tenth larger than their pictures.
const struct Scale gDaedalus_MissileScale = {
    FIXED_ONE * 11 / 10,
    FIXED_ONE * 11 / 10,
};

/// The rocket's scale in Daedalus's second strike, twice its size.
const struct Scale gDaedalusStrike_RocketScale = {FIXED_ONE * 2, FIXED_ONE * 2};

/// Coatlicue's scale.
const struct Scale gCoatlicue_UnitScale = UNIT_SCALE;

/// Catastrophe's scale for the summon.
const struct Scale gCatastrophe_UnitScale = UNIT_SCALE;

/// Haures's scale while he charges.
const struct Scale gHaures_ChargeScale = UNIT_SCALE;

/// Haures's scale while he circles.
const struct Scale gHaures_OrbitScale = UNIT_SCALE;

/// Haures's scale in the opening, before it is flipped.
const struct Scale gHaures_OpeningScale = UNIT_SCALE;

/// The scale of Trident's four prongs.
const struct Scale gTrident_UnitScale = UNIT_SCALE;

/// The summon's scale in Meteor Blow, twice its size.
const struct Scale gMeteorBlow_DoubleScale = {FIXED_ONE * 2, FIXED_ONE * 2};

/// A mote's size by its number's low three bits.
const u8 gMoloch_MoteSizes[8] = {2, 3, 5, 8, 7, 6, 4, 0};

/// A pillar's six corners, {x, y, z} in pixels: three rows of two, 16 either
/// side of its centre, at y -16, 12 and 0.
const struct ModelVertex gIris_PillarVertices[6] = {
    {-16, -16, 0}, {16, -16, 0}, {-16, 12, 0},
    {16, 12, 0},   {-16, 0, 0},  {16, 0, 0},
};

/// Iris's scale while she rises.
const struct Scale gIris_UnitScale = UNIT_SCALE;

/// The rising pillars' widths, left to right.
const struct FiveWidths gIris_RisingPillarWidths = {{2, 3, 4, 5, 6}};

/// The widening pillars' widths, left to right.
const struct SevenWidths gIris_WideningPillarWidths = {{2, 3, 4, 5, 6, 7, 8}};

/// The narrowing pillars' widths, left to right.
const struct SevenWidths gIris_NarrowingPillarWidths = {{8, 7, 6, 5, 4, 3, 2}};

/// The left strip's four spans where petals start, {left, width} in pixels.
const struct FourSpans gIris_LeftStripSpans = {
    {{112, 27}, {104, 28}, {92, 27}, {74, 27}},
};

/// The right strip's four spans where petals start, {left, width} in pixels.
const struct FourSpans gIris_RightStripSpans = {
    {{196, 44}, {176, 48}, {152, 45}, {116, 45}},
};
