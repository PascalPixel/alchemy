#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

/// @file
/// The records and columns the battle effect tables are written in.

#include "types.h"

/// How each element card's sparks move: the kernel's gravity and damping, and
/// how many steps (each drawn) a frame.
struct SparkMotion {
  s32 gravity;
  s32 damping;
  s32 steps;
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

#endif // ALCHEMY_BATTLE_EFFECT_TABLE_H
