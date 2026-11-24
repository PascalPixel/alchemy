#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

/// @file
/// The rows and columns of the battle effects' tables, which effect_table.c
/// fills and the effects read.

#include "types.h"

/// How each element card's sparks move (gElementCards_SparkMotions).
struct SparkMotion {
  s32 gravity; ///< added to the y speed each step, 16.16 pixels
  s32 damping; ///< the speed kept each step, in sixty-fourths
  s32 steps;   ///< steps a frame, the spark drawn at each
};

/// The columns of gFume_Levels, one row per Fume level (effect->variant): the
/// plume's width, the frame its tail starts, its arc's height, the sparks'
/// spread, count and lift.
enum FumeLevelColumn {
  FUME_WIDTH,
  FUME_TAIL_START,
  FUME_HEIGHT,
  FUME_SPREAD,
  FUME_SPARKS,
  FUME_LIFT,
  FUME_COLUMNS
};

/// Each Froth variant: its motes and frames.
enum {
  FROTH_BUBBLE_COUNT, ///< bubbles, at most 30 (battle_effect_vortices.c)
  FROTH_FRAMES        ///< the effect's frames; the damage shows 16 before
};

/// Each Whirlwind variant: its tornadoes, their sway and the screen shake.
enum {
  WHIRLWIND_TORNADOES, ///< tornadoes drawn
  WHIRLWIND_SWAY,      ///< how far each sways across, in pixels
  WHIRLWIND_SHAKE      ///< BattleCamera_Shake's range
};

/// The spark groups' variants past the three strengths (enum EffectVariant),
/// which their callers set before the run: rows 3 to 5 of gSparkGroups_Shapes.
enum {
  SPARK_GROUPS_SEVERE_BLOW = 3, ///< 340 sparks in one burst: Severe Blow
  SPARK_GROUPS_THRASH,          ///< 32 sparks: Thrash
  SPARK_GROUPS_MIGHTY_PRESS     ///< 200 sparks in one burst: Mighty Press
};

#endif // ALCHEMY_BATTLE_EFFECT_TABLE_H
