#ifndef ALCHEMY_BATTLE_EFFECT_TABLE_H
#define ALCHEMY_BATTLE_EFFECT_TABLE_H

// The rows and columns of the battle effects' tables, which effect_table.c fills and the effects
// read.

#include "types.h"

// The projectile volley's modes (btEffectRunProjectileVolley), each named for the effect that fires
// it; the casting impacts fire some of them.
typedef enum PROJECTILE_VOLLEY_MODE
{
	PROJECTILE_VOLLEY_SLASH,         // comets, picked by the variant
	PROJECTILE_VOLLEY_SHURIKEN,      // spinning shurikens
	PROJECTILE_VOLLEY_CRAZY_VOICE,   // bouncing musical notes
	PROJECTILE_VOLLEY_WING_FLUTTER,  // yellow sparks
	PROJECTILE_VOLLEY_WING_BEAT,     // small yellow sparks
	PROJECTILE_VOLLEY_CUTTING_EDGE,  // one large comet in ice colours
	PROJECTILE_VOLLEY_HEAT_WAVE,     // fireballs trailing embers
	PROJECTILE_VOLLEY_SLEET,         // ice chips falling from high above
	PROJECTILE_VOLLEY_IMPACT_COMETS, // comets, after a casting impact
	PROJECTILE_VOLLEY_BLIZZARD,      // scattered ice chips
	PROJECTILE_VOLLEY_ICE_BLESSING,  // ice chips streaming one a tick
	PROJECTILE_VOLLEY_WING_STROKE,   // large yellow sparks, spread up and down
	PROJECTILE_VOLLEY_BLAZE_RUSH,
	PROJECTILE_VOLLEY_HYDRO_SLASH,
	PROJECTILE_VOLLEY_DIAMOND_DUST,
	PROJECTILE_VOLLEY_BEAST_NEEDLE,
	PROJECTILE_VOLLEY_POISON_STING,
	PROJECTILE_VOLLEY_STUN_STING,
	PROJECTILE_VOLLEY_CHILL_BREATH
} PROJECTILE_VOLLEY_MODE;

// The bytes of a ProjectileVolleyModes row.
typedef enum VOLLEY_FIELD
{
	PROJECTILE_VOLLEY_SHOTS,   // shots at each target
	PROJECTILE_VOLLEY_STEPS,   // ticks a shot takes to reach its target
	PROJECTILE_VOLLEY_STAGGER, // ticks between shots
	PROJECTILE_VOLLEY_DELAY,   // ticks between targets
	PROJECTILE_VOLLEY_SPARKS,  // sparks thrown on each hit
	PROJECTILE_VOLLEY_COLUMNS
} VOLLEY_FIELD;

// How each element card's sparks move (ElementCardsSparkMotions).
typedef struct SPARK_MOTION
{
	Sint32 gravity; // added to the y speed each step, 16.16 pixels
	Sint32 damping; // the speed kept each step, in sixty-fourths
	Sint32 steps;   // steps a tick, the spark drawn at each
} SPARK_MOTION;

// The columns of FumeLevels, one row per Fume level (effect->variant): the plume's width, the tick
// its tail starts, its arc's height, the sparks' spread, count and lift.
typedef enum FUME_LEVEL_COLUMN
{
	FUME_WIDTH,
	FUME_TAIL_START,
	FUME_HEIGHT,
	FUME_SPREAD,
	FUME_SPARKS,
	FUME_LIFT,
	FUME_COLUMNS
} FUME_LEVEL_COLUMN;

// Each Froth variant: its motes and ticks.
enum
{
	FROTH_BUBBLE_COUNT, // bubbles, at most 30 (battle_effect_vortices.c)
	FROTH_TICKS         // the effect's ticks; the damage shows 16 before
};

// Each Whirlwind variant: its tornadoes, their sway and the screen shake.
enum
{
	WHIRLWIND_TORNADOES, // tornadoes drawn
	WHIRLWIND_SWAY,      // how far each sways across, in pixels
	WHIRLWIND_SHAKE      // btCameraShake's range
};

// The spark groups' variants past the three strengths (EFFECT_VARIANT), which their callers set
// before the run: rows 3 to 5 of SparkGroupsShapes.
enum
{
	SPARK_GROUPS_SEVERE_BLOW = 3, // 340 sparks in one burst: Severe Blow
	SPARK_GROUPS_THRASH,          // 32 sparks: Thrash
	SPARK_GROUPS_MIGHTY_PRESS     // 200 sparks in one burst: Mighty Press
};

#endif                            // ALCHEMY_BATTLE_EFFECT_TABLE_H
