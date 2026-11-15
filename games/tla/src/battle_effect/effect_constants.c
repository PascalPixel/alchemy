/// @file
/// The constants the battle effects copy into their locals, after the effect
/// code and in its order: scales, points and widths.

#include "fixed_math.h"
#include "types.h"
#include "battle/render_model.h"
#include "battle_effect/battle_effect.h"
#include "graphics/render_placement.h"

/// The unit scale (FIXED_ONE across and down) most effects start from.
#define UNIT_SCALE {FIXED_ONE, FIXED_ONE}

const struct Scale gCruelRuin_UnitScale = UNIT_SCALE;

const struct Scale gEclipse_Scales = UNIT_SCALE;

/// Unused: two more scales after Eclipse's, each word 0x55555556 (2^32 / 3
/// rounded up); nothing reads them.
const struct Scale gEclipse_UnusedScales[2] = {
    {0x55555556, 0x55555556},
    {0x55555556, 0x55555556},
};

/// The comets' unit scale.
const struct Scale gProcne_UnitScale = UNIT_SCALE;

/// Unused: three words after Procne's scale, 2^32 / 36 and twice 2^32 / 90, each
/// rounded up; nothing reads them.
const u32 gProcne_UnusedWords[3] = {0x071c71c8, 0x02d82d83, 0x02d82d83};

const struct Scale gZagan_UnitScale = UNIT_SCALE;

const struct Scale gBoreas_UnitScale = UNIT_SCALE;

/// Each of the four pillars' heights in pixels.
const struct RockWallHeights gRockWall_Heights = {{-32, -32, -32, -32}};

/// The three lines' end points, a pair of points each, in pixels.
const s16 gThor_Points[6][2] = {
    {8, 135}, {184, 135}, {128, 135}, {96, 32}, {96, 32}, {64, 135},
};

const struct Scale gGaia_UnitScale = UNIT_SCALE;

const struct SpriteScale gUlysses_ParticleScale = {{UNIT_SCALE}};

const struct Scale gUlysses_ShipScale = UNIT_SCALE;

const struct Scale gFormationObjects_Scale = UNIT_SCALE;

const struct Scale gFormationObjects_LargeScale = {
    FIXED_ONE * 5 / 4,
    FIXED_ONE * 5 / 4,
};

/// One to one.
const struct SpriteScale gRamses_UnitScale = {{UNIT_SCALE}};

const struct Scale gJudgment_UnitScale = UNIT_SCALE;

const struct SpriteScale gKirin_SweepScale = {{UNIT_SCALE}};

const struct SpriteScale gKirin_ChargeScale = {{UNIT_SCALE}};

/// The serpent's scale.
const struct Scale gNeptune_UnitScale = {FIXED_ONE * 3 / 2, FIXED_ONE * 3 / 2};

const struct Scale gMegaera_UnitScale = {FIXED_ONE * 3 / 2, FIXED_ONE * 3 / 2};

/// The hexagon's six corners, {z, y} in pixels.
const s8 gVoidBeam_Corners[12] = {
    40, -40, 0, 40, -40, -40, 40, 40, 0, -40, -40, 40,
};

const struct Scale gCharon_SwarmScale = UNIT_SCALE;

const struct Scale gCharon_PulseScale = UNIT_SCALE;

const struct Scale gCharon_RevealScale = UNIT_SCALE;

const struct Scale gCharon_FinaleScale = UNIT_SCALE;

const struct Scale gFlora_UnitScale = UNIT_SCALE;

const struct Scale gDaedalus_WaveScale = UNIT_SCALE;

const struct Scale gDaedalus_ShipScale = UNIT_SCALE;

const struct Scale gDaedalus_ParticleScale = {
    FIXED_ONE * 11 / 10,
    FIXED_ONE * 11 / 10,
};

const struct Scale gDaedalusAlt_SummonScale = {FIXED_ONE * 2, FIXED_ONE * 2};

const struct Scale gCoatlicue_UnitScale = UNIT_SCALE;

const struct Scale gCatastrophe_UnitScale = UNIT_SCALE;

const struct Scale gHaures_ChargeScale = UNIT_SCALE;

const struct Scale gHaures_OrbitScale = UNIT_SCALE;

const struct Scale gHaures_OpeningScale = UNIT_SCALE;

const struct Scale gTrident_UnitScale = UNIT_SCALE;

const struct Scale gMeteorBlow_DoubleScale = {FIXED_ONE * 2, FIXED_ONE * 2};

/// A mote's size by its number's low three bits.
const u8 gMoloch_MoteSizes[8] = {2, 3, 5, 8, 7, 6, 4, 0};

/// A pillar's six corners, {x, y, z} in pixels: three rows of two, 16 either
/// side of its centre, at y -16, 12 and 0.
const struct ModelVertex gIris_PillarVertices[6] = {
    {-16, -16, 0}, {16, -16, 0}, {-16, 12, 0},
    {16, 12, 0},   {-16, 0, 0},  {16, 0, 0},
};

const struct Scale gIris_UnitScale = UNIT_SCALE;

const struct FiveWidths gIris_RisingPillarWidths = {{2, 3, 4, 5, 6}};

const struct SevenWidths gIris_WideningPillarWidths = {{2, 3, 4, 5, 6, 7, 8}};

const struct SevenWidths gIris_NarrowingPillarWidths = {{8, 7, 6, 5, 4, 3, 2}};

const struct FourSpans gIris_LeftStripSpans = {
    {{112, 27}, {104, 28}, {92, 27}, {74, 27}},
};

const struct FourSpans gIris_RightStripSpans = {
    {{196, 44}, {176, 48}, {152, 45}, {116, 45}},
};
