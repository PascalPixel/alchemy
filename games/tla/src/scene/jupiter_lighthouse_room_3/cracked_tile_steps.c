/// @file
/// The room of gaps and logs in Jupiter Lighthouse: the cracked tiles' steps
/// and the fall to the floor below.

#include "../shared/cracked_tile.h"
#include "../shared/tables.h"
#include "types.h"
#include "cell_target.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "system.h"
#include "frame.h"
void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"

extern u16 gPushKinds[];

/// The cells CrackedTiles_Step watches, and the cell it copies over the one the
/// leader steps on.
extern struct CellTarget gCrackedTiles;

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void CrackedTile_Crack();
void CrackedTile_FallEvent();

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_FallEvent)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)
