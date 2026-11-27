#ifndef ALCHEMY_VIEW_MOVE_H
#define ALCHEMY_VIEW_MOVE_H

/* @file */
/* The view's swing as Tremor and Grind shake the ground. Its state sits in */
/* HEAP_SLOT_BATTLE_EFFECT: fdViewMoveStart opens the map's view bounds and */
/* keeps the old ones, fdViewMoveSwing swings the view centre each frame, and */
/* fdViewMoveEnd puts the bounds back. fdViewMoveSaveNearbySprs gathers the */
/* nearby actors whose sprites rock with the view, and */
/* fdViewMoveRestoreNearbySprs sets them back. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

struct ACTOR;

/* Directions a view move swings in. */
#define VIEW_MOVE_X 1
#define VIEW_MOVE_Z 2

/* A view move's state: where the view and the map's view bounds stood when */
/* the move began, which edges it lies beside, and the actors it carries. */
typedef struct VIEW_MOVE_STATE {
	s32 x; /* the view centre's position when the move began */
	s32 y;
	s32 z;
	s32 savedMinX; /* the map's view bounds when the move began */
	s32 savedMaxX;
	s32 savedMinZ;
	s32 savedMaxZ;
	s16 edgeX; /* -1 beside the low edge, 1 beside the high edge, else 0 */
	s16 edgeZ;
	struct ACTOR *actors[8]; /* the actors near the caster it rocks */
	u16 sprRotations[8];	 /* each one's sprite rotation, given back after */
	s32 cnt;
} VIEW_MOVE_STATE;

STATIC_ASSERT(offsetof(VIEW_MOVE_STATE, edgeX) == 28);
STATIC_ASSERT(offsetof(VIEW_MOVE_STATE, actors) == 32);
STATIC_ASSERT(offsetof(VIEW_MOVE_STATE, cnt) == 80);

void fdViewMoveStart(s32 direction);
void fdViewMoveSwing(s32 direction, s32 phase, FIXED amplitude);
void fdViewMoveEnd(void);
void fdViewMoveSaveNearbySprs(void);
void fdViewMoveRestoreNearbySprs(void);

#endif
