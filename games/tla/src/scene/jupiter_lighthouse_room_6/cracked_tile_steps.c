/// @file
/// The hall below Jupiter Lighthouse's aerie: the cracked tiles' steps and
/// landings.

#include "../shared/tables.h"
#include "../shared/cracked_tile.h"
#include "types.h"
#include "cell_target.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"

void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_StepOffButton(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
void JupiterLighthouse_FinishMovingBlock(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

extern struct CellTarget gCrackedTiles;

void GameFlag_SetByte(s32 flag, s32 value);
void CrackedTile_Crack(struct CellTarget *work, struct CellTargetEntry *entry);
void CrackedTile_Fall(struct CellTarget *work, struct CellTargetEntry *entry);

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                        CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                CrackedTile_DropIn)
