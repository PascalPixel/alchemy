/// @file
/// Tables several scenes' overlays each keep a copy of, written once: a
/// scene expands the macro where its copy sits. The placeholder names are the
/// first copy's.
#ifndef ALCHEMY_SCENE_SHARED_TABLES_H
#define ALCHEMY_SCENE_SHARED_TABLES_H

/// 8 scenes: islets/data, jupiter_lighthouse_room_3/s1f50,
/// jupiter_lighthouse_room_8/s1dc8, magma_rock/s34ac, magma_rock_inside/s46e8,
/// mars_lighthouse_room_f/s3d80, venus_lighthouse/floor,
/// yampi_desert_cave/data.
#define DEFINE_G_DATA_0200198C(name)                                           \
  s16 name[] = {                                                               \
      0, 16,  0, 16,  0, 16,  16,  0, 16,  0, 16,  0, 16,  0, 0, -16,          \
      0, -16, 0, -16, 0, -16, -16, 0, -16, 0, -16, 0, -16, 0, 0, 16,           \
  };

/// 2 scenes: gaia_rock, gaia_rock_inside/s4d00.
#define DEFINE_G_DATA_020016CC(name)                                           \
  s32 name[] = {                                                               \
      SC_ADD(SW_SCALE_X, 0x8000),                                              \
      SC_ADD(SW_SCALE_Y, 0x8000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, -0x4000),                                             \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x2000),                                             \
      SC_ADD(SW_SCALE_Y, -0x2000),                                             \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x2000),                                             \
      SC_ADD(SW_SCALE_Y, -0x2000),                                             \
      SC_WAIT(4),                                                              \
      SC_ADD(SW_SCALE_X, 0x8000),                                              \
      SC_ADD(SW_SCALE_Y, 0x8000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, -0x4000),                                             \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x2000),                                             \
      SC_ADD(SW_SCALE_Y, -0x2000),                                             \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x2000),                                             \
      SC_ADD(SW_SCALE_Y, -0x2000),                                             \
      SC_WAIT(4),                                                              \
      SC_ADD(SW_SCALE_X, 0x8000),                                              \
      SC_ADD(SW_SCALE_Y, 0x8000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, -0x4000),                                             \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x2000),                                             \
      SC_ADD(SW_SCALE_Y, -0x2000),                                             \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, -0x2000),                                             \
      SC_ADD(SW_SCALE_Y, -0x2000),                                             \
      SC_WAIT(4),                                                              \
      SC_WAIT(20),                                                             \
      SC_LOOP(3, SC_TOP),                                                      \
      SC_END,                                                                  \
  };

/// 1 scenes: naribwe/data.
#define DEFINE_G_DATA_020006CC(name)                                           \
  struct ActorPlacement name[] = {                                             \
      {365, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(246), FACING_SOUTH, 0, \
       0},                                                                     \
      {304, CONDITION_ALWAYS, (s32)gData_02000604, PIXELS(316), 0,             \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {304, CONDITION_ALWAYS, (s32)gData_02000604, PIXELS(372), 0,             \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {410, CONDITION_ALWAYS, (s32)gData_02000610, PIXELS(316), 0,             \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {410, CONDITION_ALWAYS, (s32)gData_02000610, PIXELS(372), 0,             \
       PIXELS(132), FACING_SOUTH, 0, 0},                                       \
      {115, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(344), FACING_SOUTH, 1, \
       0},                                                                     \
      {116, CONDITION_ALWAYS, 3, PIXELS(200), 0, PIXELS(296), FACING_EAST, 0,  \
       0},                                                                     \
      {114, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(152), FACING_SOUTH, 1, \
       0},                                                                     \
      {128, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(376), FACING_NORTHEAST, \
       0, 0},                                                                  \
      {112, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(88), FACING_SOUTHEAST, \
       1, 0},                                                                  \
      {113, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(224),                  \
       FACING_SOUTHEAST, 1, 0},                                                \
      {136, CONDITION_ALWAYS, 2, PIXELS(200), 0, PIXELS(200),                  \
       FACING_NORTHEAST, 0, 0},                                                \
      {147, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(232),                  \
       FACING_SOUTHWEST, 1, 0},                                                \
      {112, CONDITION_ALWAYS, 2, PIXELS(56), 0, PIXELS(232), FACING_EAST, 0,   \
       0},                                                                     \
      {110, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(360), FACING_EAST, 1,  \
       0},                                                                     \
      {307, CONDITION_ALWAYS, (s32)gData_02000634, PIXELS(40), 0, PIXELS(168), \
       FACING_SOUTH, 2, 0},                                                    \
      {407, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(376), FACING_SOUTH, 2, \
       0},                                                                     \
      {489, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(376), FACING_SOUTH, 2, \
       0},                                                                     \
      {201, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(168), FACING_SOUTH, 1, \
       0},                                                                     \
      {328, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(104), FACING_SOUTH, 1, \
       0},                                                                     \
      {SCENE_TABLE_END},                                                       \
  };

/// 1 scenes: izumo.
#define DEFINE_G_DATA_02000E14(name)                                           \
  u32 name[] = {                                                               \
      SC_SET(SW_COLLISION, 129),                                               \
      SC_SET(SW_ACCEL, 0x3333),                                                \
      SC_SET(SW_SPEED, 0x9999),                                                \
      SC_SET(SW_STEERING, 2),                                                  \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_LABEL(1),                                                             \
      SC_WAIT(30),                                                             \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(60),                                                             \
      SC_WAIT(30),                                                             \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(60),                                                             \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
      SC_END,                                                                  \
  };

/// 1 scenes: izumo.
#define DEFINE_G_DATA_02000F14(name)                                           \
  u32 name[] = {                                                               \
      SC_SET(SW_COLLISION, 129),                                               \
      SC_SET(SW_ACCEL, 0x3333),                                                \
      SC_SET(SW_SPEED, 0x9999),                                                \
      SC_SET(SW_STEERING, 2),                                                  \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_LABEL(1),                                                             \
      SC_WAIT(30),                                                             \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(60),                                                             \
      SC_WAIT(30),                                                             \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(60),                                                             \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
      SC_END,                                                                  \
  };

/// 4 scenes: islets/data, kibombo/s2f34, lemurian_ship, madra_catacombs.
#define DEFINE_G_DATA_0200121C(name)                                           \
  s32 name[] = {                                                               \
      SC_SET(SW_MOTION, 0),                                                    \
      SC_ADD(SW_Z, -0x8000),                                                   \
      SC_ADD(SW_SCALE_X, -1024),                                               \
      SC_ADD(SW_SCALE_Y, -1024),                                               \
      SC_WAIT(1),                                                              \
      SC_LOOP(24, SC_TOP),                                                     \
      SC_END,                                                                  \
  };

/// 2 scenes: apojii_islands, aqua_rock/data2.
#define DEFINE_G_DATA_02001F18(name)                                           \
  s16 name[] = {                                                               \
      -8,  -12,  20,  0,   1,    -8,  -12,  20,  0,    1,   -8,   -12,         \
      20,  0,    1,   -8,  -12,  16,  0,    1,   -8,   -12, 16,   0,           \
      1,   -8,   -12, 12,  0,    1,   -8,   -12, 8,    0,   1,    0,           \
      -32, 0,    2,   2,   8,    -48, 0,    2,   2,    16,  -64,  0,           \
      2,   2,    24,  -80, 0,    2,   2,    32,  -96,  0,   2,    2,           \
      40,  -112, 0,   2,   2,    48,  -128, 0,   2,    2,   56,   -144,        \
      0,   2,    2,   64,  -160, 0,   2,    2,   -2,   12,  -4,   12,          \
      -6,  12,   -8,  8,   -10,  8,   -12,  8,   -14,  8,   -16,  8,           \
      -19, 4,    -21, 4,   -24,  4,   -27,  4,   -30,  4,   -33,  4,           \
      -36, 4,    -40, 4,   -44,  4,   -48,  0,   -52,  0,   -56,  0,           \
      -60, 0,    -64, 0,   -68,  0,   -72,  0,   -76,  0,   -80,  0,           \
      -85, 0,    -90, 0,   -95,  0,   -100, 0,   -110, 0,   -120, 0,           \
  };

/// 2 scenes: jupiter_lighthouse_room_5/s2450, mars_lighthouse_aerie/s3370.
#define DEFINE_G_DATA_02003214(name)                                           \
  u32 name[] = {                                                               \
      SC_SET(SW_SCALE_X, 0xcccc),                                              \
      SC_SET(SW_SCALE_Y, 0xcccc),                                              \
      SC_ARG(1),                                                               \
      SC_LABEL(1),                                                             \
      SC_ADD(SW_Y, PIXELS(1)),                                                 \
      SC_WAIT(1),                                                              \
      SC_LOOP(16, SC_TO(1)),                                                   \
      SC_POSITION(0, 0, 0),                                                    \
      SC_WAIT(1),                                                              \
      SC_FREE,                                                                 \
  };

/// 2 scenes: jupiter_lighthouse_aerie_beacon/data2,
/// mars_lighthouse_aerie/s3370.
#define DEFINE_G_DATA_02005B54(name)                                           \
  s32 name[] = {                                                               \
      SC_LABEL(1),                                                             \
      SC_ADD(SW_SCALE_X, -1536),                                               \
      SC_WAIT(1),                                                              \
      SC_LOOP(4, SC_TO(1)),                                                    \
      SC_LABEL(2),                                                             \
      SC_ADD(SW_SCALE_X, 0x600),                                               \
      SC_WAIT(1),                                                              \
      SC_LOOP(4, SC_TO(2)),                                                    \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

/// 2 scenes: airs_rock_inside/s505c, aqua_rock_inside/s3844.
#define DEFINE_G_DATA_02004014(name, call1, call2, call3)                      \
  s32 name[] = {                                                               \
      46, (s32)call1, 46, (s32)call2, 46, (s32)call3, 38, 17,                  \
  };

/// 1 scenes: mars_lighthouse_aerie/s3370.
#define DEFINE_G_DATA_02005EC4(name)                                           \
  u32 name[] =                                                                 \
      {                                                                        \
          SC_ARG(2),   SC_MOVE(PIXELS(560), 0, PIXELS(280)),                   \
          SC_SYNC,     SC_ARG(1),                                              \
          SC_WAIT(10), SC_SET(SW_FACING, FACING_WEST),                         \
          SC_WAIT(10), SC_END,                                                 \
  };

/// 2 scenes: gaia_rock_inside/s4d00, magma_rock_inside/s46e8.
#define DEFINE_G_DATA_02005244(name, call1, call2, call3)                      \
  s32 name[] = {                                                               \
      SC_CALL(call1), SC_CALL(call2), SC_CALL(call3), SC_FREE, SC_END,         \
  };

/// 2 scenes: gaia_rock_inside/s4d00, magma_rock_inside/s46e8.
#define DEFINE_G_DATA_02005218(name)                                           \
  u32 name[] = {                                                               \
      SC_MOVE_BY(0, PIXELS(160), 0),                                           \
      SC_SYNC,                                                                 \
      SC_FREE,                                                                 \
      SC_END,                                                                  \
  };

/// 2 scenes: settlements/s1d30, tundaria_tower/s29d0.
#define DEFINE_G_DATA_02002276(name)                                           \
  s16 name[] = {                                                               \
      -1,     0,      0x8000, -1,     0xc000, 0xe000, 0xa000, 0xc000,          \
      0x4000, 0x2000, 0x6000, 0x4000, -1,     0,      0x8000, -1,              \
      -1,     0,      0x8000, -1,     0xc000, -1,     -1,     0xc000,          \
      0x4000, -1,     -1,     0x4000, -1,     0,      0x8000, -1,              \
  };

#endif
