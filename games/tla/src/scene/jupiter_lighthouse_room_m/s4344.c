/// @file
/// Scene code of overlay 0x6a3, in ROM order.

#include "../shared/tables.h"
#include "../shared/cracked_tile.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_m.h"
#include "module/scene/scene.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

#include "scene_tables.h"
#include "objcmd.h"

// clang-format off
u32 gData_020049cc[] = {
    1, 39, 1, 17,
};

u32 gData_020049dc[] = {
    1, 39, 1, 17,
};

u32 gData_020049ec[] = {
    1, 39, 1, 17,
};

u32 gData_020049fc[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

u32 gData_02004a18[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_END,
};

u32 gData_02004a34[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_END,
};

u32 gData_02004a50[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gData_02004a6c[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004a90[] = {
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1),
    SC_END,
};

DEFINE_G_DATA_02004AB4(gData_02004ab4, 699)

DEFINE_G_DATA_02004AB4(gData_02004ae8, 679)

DEFINE_G_DATA_02004AB4(gData_02004b1c, 659)
// clang-format on
