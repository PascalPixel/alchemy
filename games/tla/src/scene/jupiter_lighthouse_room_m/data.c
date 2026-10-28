#include "types.h"
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

u32 gData_02004ab4[] = {
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(699), PIXELS(170)),
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004ae8[] = {
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(679), PIXELS(170)),
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};

u32 gData_02004b1c[] = {
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(659), PIXELS(170)),
    SC_SYNC,
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_ARG(1),
    SC_END,
};
// clang-format on
