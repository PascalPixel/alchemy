/// @file
/// Tables several scenes' overlays each keep a copy of, written once: a
/// scene expands the macro where its copy sits. The placeholder names are the
/// first copy's.
#ifndef ALCHEMY_SCENE_SHARED_TABLES_H
#define ALCHEMY_SCENE_SHARED_TABLES_H

/// One step of the leader as he pushes: x pixels in the high half, z in the
/// low half, as the push routines read it.
#define PUSH_STEP(x, z) ((x) * 0x10000 + ((z) & 0xffff))

/// The leader's step as he pushes, 16 pixels on one axis, for each of the 16
/// directions he can face.
#define DEFINE_PUSH_LEADER_STEPS(name)                                         \
  s32 name[] = {                                                               \
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  \
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  \
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 \
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 \
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 \
      PUSH_STEP(16, 0),                                                        \
  };

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

/// An actor that spins on the spot every second and a half, from facing
/// south; the east and north forms start from those headings.
#define DEFINE_SPIN_FROM_SOUTH_SCRIPT(name)                                    \
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

#define DEFINE_SPIN_FROM_EAST_SCRIPT(name)                                     \
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

#define DEFINE_G_DATA_02004014(name, call1, call2, call3)                      \
  s32 name[] = {                                                               \
      46, (s32)call1, 46, (s32)call2, 46, (s32)call3, 38, 17,                  \
  };

#define DEFINE_G_DATA_02005EC4(name)                                           \
  u32 name[] =                                                                 \
      {                                                                        \
          SC_ARG(2),   SC_MOVE(PIXELS(560), 0, PIXELS(280)),                   \
          SC_SYNC,     SC_ARG(1),                                              \
          SC_WAIT(10), SC_SET(SW_FACING, FACING_WEST),                         \
          SC_WAIT(10), SC_END,                                                 \
  };

#define DEFINE_G_DATA_02005244(name, call1, call2, call3)                      \
  s32 name[] = {                                                               \
      SC_CALL(call1), SC_CALL(call2), SC_CALL(call3), SC_FREE, SC_END,         \
  };

#define DEFINE_G_DATA_02005218(name)                                           \
  u32 name[] = {                                                               \
      SC_MOVE_BY(0, PIXELS(160), 0),                                           \
      SC_SYNC,                                                                 \
      SC_FREE,                                                                 \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02002276(name)                                           \
  s16 name[] = {                                                               \
      -1,     0,      0x8000, -1,     0xc000, 0xe000, 0xa000, 0xc000,          \
      0x4000, 0x2000, 0x6000, 0x4000, -1,     0,      0x8000, -1,              \
      -1,     0,      0x8000, -1,     0xc000, -1,     -1,     0xc000,          \
      0x4000, -1,     -1,     0x4000, -1,     0,      0x8000, -1,              \
  };

/// A field effect's script that keeps it for `frames` frames, then frees it;
/// each overlay's effect spawner picks one of three by its flags.
#define DEFINE_FIELD_EFFECT_DURATION_SCRIPT(name, frames)                          \
  u32 name[] = {                                                               \
      SC_WAIT(1), SC_LOOP(frames, SC_TOP),                                     \
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        \
      SC_WAIT(1), SC_FREE,                                                     \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02002324(name, call1, n1, n2, n3, n4)                    \
  s32 name[] = {                                                               \
      22,                                                                      \
      30,                                                                      \
      129,                                                                     \
      22,                                                                      \
      16,                                                                      \
      0x3333,                                                                  \
      22,                                                                      \
      21,                                                                      \
      0x6666,                                                                  \
      22,                                                                      \
      15,                                                                      \
      0x9999,                                                                  \
      22,                                                                      \
      31,                                                                      \
      2,                                                                       \
      22,                                                                      \
      3,                                                                       \
      0x4000,                                                                  \
      (s32)0x80010000,                                                         \
      46,                                                                      \
      (s32)call1,                                                              \
      4,                                                                       \
      n1,                                                                      \
      0,                                                                       \
      0x2700000,                                                               \
      1,                                                                       \
      4,                                                                       \
      n1,                                                                      \
      0,                                                                       \
      0x2580000,                                                               \
      1,                                                                       \
      4,                                                                       \
      n2,                                                                      \
      0,                                                                       \
      0x2580000,                                                               \
      1,                                                                       \
      4,                                                                       \
      n2,                                                                      \
      0,                                                                       \
      0x2700000,                                                               \
      1,                                                                       \
      22,                                                                      \
      3,                                                                       \
      n3,                                                                      \
      0,                                                                       \
      5,                                                                       \
      22,                                                                      \
      3,                                                                       \
      0xc000,                                                                  \
      0,                                                                       \
      5,                                                                       \
      22,                                                                      \
      3,                                                                       \
      n4,                                                                      \
      0,                                                                       \
      5,                                                                       \
      22,                                                                      \
      3,                                                                       \
      0x4000,                                                                  \
      0,                                                                       \
      5,                                                                       \
      23,                                                                      \
      6,                                                                       \
      0x40000,                                                                 \
      0,                                                                       \
      30,                                                                      \
      4,                                                                       \
      n1,                                                                      \
      0,                                                                       \
      0x2700000,                                                               \
      1,                                                                       \
      4,                                                                       \
      n2,                                                                      \
      0,                                                                       \
      0x2580000,                                                               \
      1,                                                                       \
      4,                                                                       \
      n1,                                                                      \
      0,                                                                       \
      0x2580000,                                                               \
      1,                                                                       \
      4,                                                                       \
      n2,                                                                      \
      0,                                                                       \
      0x2700000,                                                               \
      1,                                                                       \
      22,                                                                      \
      3,                                                                       \
      n3,                                                                      \
      0,                                                                       \
      5,                                                                       \
      22,                                                                      \
      3,                                                                       \
      0xc000,                                                                  \
      0,                                                                       \
      5,                                                                       \
      22,                                                                      \
      3,                                                                       \
      n4,                                                                      \
      0,                                                                       \
      5,                                                                       \
      22,                                                                      \
      3,                                                                       \
      0x4000,                                                                  \
      0,                                                                       \
      5,                                                                       \
      23,                                                                      \
      6,                                                                       \
      0x40000,                                                                 \
      0,                                                                       \
      30,                                                                      \
      13,                                                                      \
      0xffff,                                                                  \
      (s32)0xc0010000,                                                         \
      -1,                                                                      \
  };

#define DEFINE_G_DATA_020054CC(name, call1, n1, n2, n3, n4, n5, n6, call2,     \
                               call3)                                          \
  s32 name[] = {                                                               \
      46, (s32)call1, 4,  n1, n2, n3, 1,  4,          n1, n4,         n3, 1,   \
      4,  n5,         n4, n6, 1,  22, 37, (s32)call2, 46, (s32)call3, 17,      \
  };

#define DEFINE_G_DATA_02000850(name, n1, n2, n3, n4)                           \
  s32 name[] = {                                                               \
      SC_SET(SW_COLLISION, 129),                                               \
      SC_SET(SW_ACCEL, 0x3333),                                                \
      SC_SET(SW_SPEED, n1),                                                    \
      SC_LABEL(1),                                                             \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n2), 0, PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_WAIT(20), /* A half turn; FACING_WEST names a heading, not a turn. */ \
      SC_ADD(SW_FACING, -0x8000),                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, 0x2000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x2000),                                              \
      SC_ADD(SW_SCALE_Y, -4096),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(4),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, 0x2000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x2000),                                              \
      SC_ADD(SW_SCALE_Y, -4096),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(4),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, 0x2000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x2000),                                              \
      SC_ADD(SW_SCALE_Y, -4096),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(4),                                                              \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n4), 0, PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_WAIT(20), /* A half turn; FACING_WEST names a heading, not a turn. */ \
      SC_ADD(SW_FACING, -0x8000),                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, 0x2000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x2000),                                              \
      SC_ADD(SW_SCALE_Y, -4096),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(4),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, 0x2000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x2000),                                              \
      SC_ADD(SW_SCALE_Y, -4096),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(4),                                                              \
      SC_ADD(SW_SCALE_X, -0x4000),                                             \
      SC_ADD(SW_SCALE_Y, 0x2000),                                              \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x2000),                                              \
      SC_ADD(SW_SCALE_Y, -4096),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(2),                                                              \
      SC_ADD(SW_SCALE_X, 0x1000),                                              \
      SC_ADD(SW_SCALE_Y, -2048),                                               \
      SC_WAIT(4),                                                              \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02002F1C(name, n1, n2)                                   \
  u32 name[] =                                                                 \
      {                                                                        \
          SC_ARG(2), SC_MOVE(PIXELS(n1), 0, PIXELS(n2)), SC_SYNC, SC_ARG(1),   \
          SC_END,                                                              \
  };

#define DEFINE_G_DATA_02002F88(name, n1, n2, n3)                               \
  u32 name[] =                                                                 \
      {                                                                        \
          SC_ARG(2),   SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                     \
          SC_SYNC,     SC_ARG(1),                                              \
          SC_WAIT(n3), SC_SET(SW_FACING, FACING_NORTHWEST),                    \
          SC_WAIT(n3), SC_END,                                                 \
  };

#define DEFINE_G_DATA_02003534(name, n1, n2, n3)                               \
  u32 name[] = {                                                               \
      SC_ARG(2),   SC_MOVE(PIXELS(n1), 0, PIXELS(n2)), SC_SYNC,     SC_ARG(1), \
      SC_WAIT(n3), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(n3), SC_END,    \
  };

#define DEFINE_G_DATA_02005074(name, n1, n2, n3, n4, n5, n6, n7, n8)           \
  s16 name[] = {                                                               \
      4, 0,  0, n1, 0, 16, 0, n2, 1,  0,  4,  0,  0, n3, 0,  16,               \
      0, n4, 1, 0,  4, 0,  0, n5, 0,  16, 0,  n6, 1, 0,  4,  0,                \
      0, n7, 0, 16, 0, n8, 1, 0,  13, 0,  -1, 0,  0, 0,  17, 0,                \
  };

#define DEFINE_G_DATA_02001540(name, n1)                                       \
  u32 name[] = {                                                               \
      SC_WAIT(n1), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       \
      SC_FREE,                                                                 \
  };

#define DEFINE_G_DATA_02001334(name, n1, n2, n3)                               \
  u32 name[] = {                                                               \
      SC_SET(SW_SPEED, 0x8000),                                                \
      SC_SET(SW_ACCEL, 0x1999),                                                \
      SC_SET(SW_COLLISION, 129),                                               \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                                      \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_WAIT(3),                                                              \
      SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),                       \
      SC_WAIT(30),                                                             \
      SC_ARG(4),                                                               \
      SC_WAIT(90),                                                             \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n3), 0, PIXELS(n2)),                                      \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_WAIT(3),                                                              \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(30),                                                             \
      SC_ARG(4),                                                               \
      SC_WAIT(90),                                                             \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

#define DEFINE_G_DATA_02006088(name, n1, n2, n3, n4, n5)                       \
  s32 name[] = {                                                               \
      SC_ARG(2),                                                               \
      SC_SET(SW_SPEED, 0x38000),                                               \
      SC_SET(SW_ACCEL, 0x10000),                                               \
      SC_MOVE(PIXELS(n1), PIXELS(32), PIXELS(312)),                            \
      SC_SYNC,                                                                 \
      SC_WAIT(10),                                                             \
      SC_MOVE(PIXELS(n2), PIXELS(32), PIXELS(312)),                            \
      SC_SET(SW_MOTION, 2),                                                    \
      SC_SET(SW_VELOCITY_Y, PIXELS(5)),                                        \
      SC_CALL(Func_02000038),                                                  \
      SC_SYNC,                                                                 \
      SC_CALL(Func_02000050),                                                  \
      SC_WAIT(20),                                                             \
      SC_MOVE(PIXELS(n3), PIXELS(32), PIXELS(312)),                            \
      SC_SET(SW_MOTION, 2),                                                    \
      SC_SET(SW_VELOCITY_Y, PIXELS(5)),                                        \
      SC_WAIT(10),                                                             \
      SC_CALL(Func_02000038),                                                  \
      SC_SYNC,                                                                 \
      SC_CALL(Func_02000050),                                                  \
      SC_WAIT(20),                                                             \
      SC_MOVE(PIXELS(n4), PIXELS(32), PIXELS(312)),                            \
      SC_SET(SW_MOTION, 2),                                                    \
      SC_SET(SW_VELOCITY_Y, PIXELS(5)),                                        \
      SC_CALL(Func_02000038),                                                  \
      SC_SYNC,                                                                 \
      SC_CALL(Func_02000050),                                                  \
      SC_WAIT(20),                                                             \
      SC_MOVE(PIXELS(n4), PIXELS(32), PIXELS(n5)),                             \
      SC_SET(SW_MOTION, 2),                                                    \
      SC_SET(SW_VELOCITY_Y, PIXELS(5)),                                        \
      SC_CALL(Func_02000038),                                                  \
      SC_SYNC,                                                                 \
      SC_CALL(Func_02000050),                                                  \
      SC_ARG(1),                                                               \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02002F44(name, n1, n2)                                   \
  u32 name[] =                                                                 \
      {                                                                        \
          SC_ARG(2),   SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                     \
          SC_SYNC,     SC_ARG(1),                                              \
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    \
          SC_WAIT(10), SC_END,                                                 \
  };

#define DEFINE_SPIN_FROM_NORTH_SCRIPT(name, n1)                                \
  u32 name[] = {                                                               \
      SC_SET(SW_COLLISION, 129),                                               \
      SC_SET(SW_ACCEL, n1),                                                    \
      SC_SET(SW_SPEED, 0x9999),                                                \
      SC_SET(SW_STEERING, 2),                                                  \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_LABEL(1),                                                             \
      SC_WAIT(30),                                                             \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(60),                                                             \
      SC_WAIT(30),                                                             \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_SOUTH),                                         \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(5),                                                              \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(60),                                                             \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02002634(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)       \
  u32 name[] = {                                                               \
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 4,  n1, 0,  n2,     1,  4,      \
      n3, 0,  n2, 1,  4,       n4, 0,  n5,     1,  4,  n6, 0,      n7, 1,      \
      4,  n8, 0,  n7, 1,       4,  n9, 0,      n5, 1,  13, 0xffff, 0,          \
  };

#define DEFINE_G_DATA_02003C88(name, n1, data1, n2, n3, n4, n5, n6, n7, n8)    \
  s32 name[] = {                                                               \
      (s32)n1, (s32)data1, n2, 0, 0, n3, (s32)0xffff0110, n4, n5, 0, 0, n3,    \
      (s32)n6, n4,         n7, 0, 0, n8, 0xffff,          0,  0,  0, 0, 0,     \
  };

#define DEFINE_G_DATA_02003584(name, call1, n1, n2)                            \
  s32 name[] = {                                                               \
      46,                                                                      \
      (s32)call1,                                                              \
      n1,                                                                      \
      n2,                                                                      \
  };

#define DEFINE_G_DATA_020033C8(name, n1, n2, n3, n4, n5)                       \
  u32 name[] = {                                                               \
      SC_ARG(2), SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                           \
      SC_SYNC,   SC_MOVE(PIXELS(n1), 0, PIXELS(n3)),                           \
      SC_SYNC,   SC_MOVE(PIXELS(n4), 0, PIXELS(n5)),                           \
      SC_SYNC,   SC_ARG(1),                                                    \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_0200547C(name, call1, n1, n2, n3, n4, n5, call2)         \
  s32 name[] = {                                                               \
      SC_CALL(call1), SC_MOVE(PIXELS(n1), PIXELS(32), PIXELS(n2)),             \
      SC_SYNC,        SC_MOVE(PIXELS(n1), PIXELS(n3), PIXELS(n2)),             \
      SC_SYNC,        SC_MOVE(PIXELS(n4), PIXELS(n3), PIXELS(n5)),             \
      SC_SYNC,        SC_CALL(call2),                                          \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02000654(name, n1, n2)                                   \
  u32 name[] = {                                                               \
      SC_SET(SW_COLLISION, 129),                                               \
      SC_SET(SW_ACCEL, 0x1999),                                                \
      SC_SET(SW_SPEED, 0x4ccc),                                                \
      SC_LABEL(1),                                                             \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n1), 0, PIXELS(88)),                                      \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_ARG(1),                                                               \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(20),                                                             \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n2), 0, PIXELS(104)),                                     \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_ARG(1),                                                               \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_WAIT(20),                                                             \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_020013B4(name, n1, n2, n3, n4, n5, n6)                   \
  u16 name[] = {                                                               \
      n1, n2, n3, n4, n5, n6, n6, n1, n2, n3, n4, n5, n5, n6, n1, n2, n3, n4,  \
      n4, n5, n6, n1, n2, n3, n3, n4, n5, n6, n1, n2, n2, n3, n4, n5, n6, n1,  \
  };

#define DEFINE_G_DATA_020052AC(name, n1, n2)                                   \
  s32 name[] = {                                                               \
      SC_SET(SW_MOTION, 0),                                                    \
      SC_MOVE_BY(0, PIXELS(-n1), 0),                                           \
      SC_SYNC,                                                                 \
      SC_SET(SW_MOTION, n2),                                                   \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02002FCC(name, n1, n2, n3)                               \
  u32 name[] = {                                                               \
      SC_ARG(2),   SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                         \
      SC_SYNC,     SC_ARG(1),                                                  \
      SC_WAIT(n3), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),              \
      SC_WAIT(n3), SC_END,                                                     \
  };

#define DEFINE_G_SCENE_EXITS(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)          \
  u32 gSceneExits[] = {                                                        \
      SCENE_EXITS(n1),                                                         \
      SCENE_EXIT_IF(1, n2, n3, CONDITION_ALWAYS),                              \
      SCENE_EXIT_IF(2, n4, n5, CONDITION_ALWAYS),                              \
      SCENE_EXIT_IF(3, n6, n7, CONDITION_ALWAYS),                              \
      SCENE_EXIT_IF(4, n6, n8, CONDITION_ALWAYS),                              \
      SCENE_EXIT_IF(5, n9, n10, CONDITION_ALWAYS),                             \
      SCENE_EXITS_END,                                                         \
  };

#define DEFINE_G_DATA_0200064C(name, data1, n1, data2, n2)                     \
  s32 name[] = {                                                               \
      (s32)data1,                                                              \
      n1,                                                                      \
      (s32)data2,                                                              \
      n2,                                                                      \
  };

#define DEFINE_G_DATA_0200200C(name, n1, n2, n3, n4)                           \
  u32 name[] = {                                                               \
      SC_ARG(2), SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                           \
      SC_SYNC,   SC_MOVE(PIXELS(n3), 0, PIXELS(n4)),                           \
      SC_SYNC,   SC_ARG(1),                                                    \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02005264(name, n1)                                       \
  u32 name[] = {                                                               \
      SC_MOVE_BY(0, PIXELS(n1), 0),                                            \
      SC_SYNC,                                                                 \
      SC_SET(SW_MOTION, 4),                                                    \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02004AB4(name, n1)                                       \
  u32 name[] = {                                                               \
      SC_SYNC,    SC_MOVE_XZ(PIXELS(n1), PIXELS(170)), SC_SYNC,                \
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTHEAST), SC_ARG(1),              \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_020047AC(name, n1, n2, n3, n4, n5, n6, n7, n8)           \
  u32 name[] = {                                                               \
      39, 2,  22, 15, 0x10000, 22, 16, 0x6666, 39, 2,  4,  n1,     0,          \
      n2, 1,  39, 2,  4,       n3, 0,  n4,     1,  39, 2,  4,      n5,         \
      0,  n6, 1,  39, 2,       4,  n7, 0,      n8, 1,  13, 0xffff, 0,          \
  };

#define DEFINE_G_DATA_02001FC4(name, n1)                                       \
  u32 name[] = {                                                               \
      SC_ARG(2),                                                               \
      SC_POSITION(PIXELS(327), 0, PIXELS(71)),                                 \
      SC_SET(SW_FACING, FACING_EAST),                                          \
      SC_MOVE(PIXELS(352), 0, PIXELS(72)),                                     \
      SC_SYNC,                                                                 \
      SC_MOVE_XZ(PIXELS(373), PIXELS(n1)),                                     \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_SET(SW_FACING, FACING_NORTH),                                         \
      SC_WAIT(10),                                                             \
      SC_SET(SW_FACING, FACING_SOUTHWEST),                                     \
      SC_WAIT(10),                                                             \
      SC_SET(SW_FACING, FACING_WEST),                                          \
      SC_WAIT(10),                                                             \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_020034CC(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)       \
  u32 name[] = {                                                               \
      39, 2,  4, n1, 0, n2, 1,        4, n3, 0, n4, 1, 4,  n5, 0,      n6, 1,  \
      4,  n7, 0, n8, 1, 4,  0xf80000, 0, n9, 1, 39, 1, 22, 3,  0xd000, 17,     \
  };

#define DEFINE_G_SCENE_EXITS_2(n1, n2, n3, n4, n5, n6, n7, n8, n9)             \
  u32 gSceneExits[] = {                                                        \
      SCENE_EXITS(n1),       SCENE_EXIT(1, n2, n3), SCENE_EXIT(2, n4, n5),     \
      SCENE_EXIT(3, n6, n7), SCENE_EXIT(4, n8, n9), SCENE_EXITS_END,           \
  };

#define DEFINE_G_SCENE_EXITS_3(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)        \
  u32 gSceneExits[] = {                                                        \
      SCENE_EXITS(n1),         SCENE_EXIT(1, n2, 1),   SCENE_EXIT(2, n1, 3),   \
      SCENE_EXIT(3, n1, 2),    SCENE_EXIT(4, n1, 5),   SCENE_EXIT(5, n1, 4),   \
      SCENE_EXIT(6, n3, n4),   SCENE_EXIT(7, n3, n5),  SCENE_EXIT(8, n6, 8),   \
      SCENE_EXIT(9, n3, n7),   SCENE_EXIT(10, n3, n8), SCENE_EXIT(11, n3, n9), \
      SCENE_EXIT(12, n10, 11), SCENE_EXITS_END,                                \
  };

#define DEFINE_G_DATA_020057D0(name, n1, n2, n3)                               \
  u32 name[] = {                                                               \
      SC_ARG(n1),                                                              \
      SC_MOVE_XZ(PIXELS(n2), PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),                           \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02001D84(name, n1, n2, n3, n4)                           \
  u32 name[] = {                                                               \
      SC_ARG(2),                                                               \
      SC_SET(SW_SPEED, 0x10000),                                               \
      SC_SET(SW_ACCEL, 0x10000),                                               \
      SC_LABEL(1),                                                             \
      SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                                      \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n1), 0, PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n4), 0, PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n4), 0, PIXELS(n2)),                                      \
      SC_SYNC,                                                                 \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
  };

#define DEFINE_G_DATA_02004EB8(name, n1, n2, n3, n4)                           \
  u32 name[] = {                                                               \
      SC_SET(SW_STEERING, 0),                                                  \
      SC_SET(SW_MOTION, 2),                                                    \
      SC_SET(SW_Y, PIXELS(n1)),                                                \
      SC_SET(SW_GROUND, PIXELS(n1)),                                           \
      SC_SET(SW_SPEED, 0x8000),                                                \
      SC_SET(SW_ACCEL, 0x800),                                                 \
      SC_MOVE(PIXELS(n2), 0, PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_MOVE(PIXELS(n4), 0, PIXELS(n3)),                                      \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

#define DEFINE_G_DATA_02005C74(name, n1)                                       \
  s32 name[] = {                                                               \
      22, 35, 0xc000, 22, 36, n1, 22, 40, 0, 46, (s32)Func_0200028c,           \
  };

#define DEFINE_G_DATA_020056D4(name, n1, n2)                                   \
  u32 name[] = {                                                               \
      SC_SET(SW_STEERING, 0),                                                  \
      SC_SET(SW_MOTION, 2),                                                    \
      SC_SET(SW_SPEED, 0x8000),                                                \
      SC_SET(SW_ACCEL, 0x800),                                                 \
      SC_MOVE(PIXELS(n1), 0, PIXELS(304)),                                     \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_MOVE(PIXELS(n2), 0, PIXELS(304)),                                     \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

#define DEFINE_G_DATA_020056C4(name, n1, n2)                                   \
  s32 name[] = {                                                               \
      SC_SET(SW_SPEED, 0x20000),                                               \
      SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                                      \
      SC_SET(SW_VELOCITY_Y, PIXELS(8)),                                        \
      SC_SYNC,                                                                 \
      SC_CALL(Func_020007f4),                                                  \
      SC_WAIT(2),                                                              \
      SC_POSITION(0, 0, 0),                                                    \
      SC_LABEL(1),                                                             \
      SC_WAIT(1),                                                              \
      SC_LOOP(SC_FOREVER, SC_TO(1)),                                           \
  };

#define DEFINE_G_DATA_020057C0(name, n1, n2, n3, n4, n5, n6, n7, n8)           \
  struct ActorPlacement name[] = {                                             \
      {n1, CONDITION_ALWAYS, 1, n2, 0, 0, FACING_SOUTH, 2, n3},                \
      {n1, CONDITION_ALWAYS, 1, n3, 0, 0, FACING_SOUTH, 2, n4},                \
      {n1, CONDITION_ALWAYS, 1, n4, 0, 0, FACING_SOUTH, 2, n4},                \
      {n1, CONDITION_ALWAYS, 1, n5, 0, 0, FACING_SOUTH, 2, n4},                \
      {n6, CONDITION_ALWAYS, 1, n7, 0, 0, FACING_SOUTH, 2, n3},                \
      {n8, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  \
      {n8, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  \
      {n8, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  \
      {n8, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  \
      {SCENE_TABLE_END},                                                       \
  };

#define DEFINE_G_DATA_020050D4(name, n1, n2, n3, n4, n5, n6, n7, n8)           \
  u32 name[] = {                                                               \
      SC_MOVE(PIXELS(n1), PIXELS(16), PIXELS(n2)),                             \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n3), PIXELS(16), PIXELS(n4)),                             \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n5), PIXELS(16), PIXELS(n6)),                             \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n7), PIXELS(16), PIXELS(n8)),                             \
      SC_SYNC,                                                                 \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02004DE8(name, n1, n2)                                   \
  u32 name[] = {                                                               \
      SC_SET(SW_STEERING, 0),                                                  \
      SC_SET(SW_SPEED, 0x8000),                                                \
      SC_SET(SW_ACCEL, 0x800),                                                 \
      SC_MOVE(PIXELS(n1), 0, PIXELS(112)),                                     \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_MOVE(PIXELS(n2), 0, PIXELS(112)),                                     \
      SC_SYNC,                                                                 \
      SC_WAIT(20),                                                             \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

#define DEFINE_G_DATA_0200218C(name, n1, n2, n3, n4)                           \
  u32 name[] = {                                                               \
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(n1),                        \
      SC_SET(SW_FACING, FACING_SOUTHWEST), SC_WAIT(n2),                        \
      SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(n3),                        \
      SC_SET(SW_FACING, FACING_SOUTHEAST), SC_WAIT(n4),                        \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

#define DEFINE_G_SCENE_EXITS_4(n1, n2, n3, n4, n5)                             \
  u32 gSceneExits[] = {                                                        \
      SCENE_EXITS(n1),                                                         \
      SCENE_EXIT_IF(1, n2, 1, EXIT_FLAG_NONE),                                 \
      SCENE_EXIT_IF(2, n2, 2, EXIT_FLAG_NONE),                                 \
      SCENE_EXIT_IF(3, n2, 3, EXIT_FLAG_NONE),                                 \
      SCENE_EXIT_IF(4, n2, 4, EXIT_FLAG_NONE),                                 \
      SCENE_EXIT_IF(5, n2, 5, EXIT_FLAG_NONE),                                 \
      SCENE_EXIT_IF(6, n3, n4, EXIT_FLAG_NONE),                                \
      SCENE_EXIT_IF(7, n3, n5, EXIT_FLAG_NONE),                                \
      SCENE_EXITS_END,                                                         \
  };

#define DEFINE_G_DATA_020013D8(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)       \
  struct ActorPlacement name[] = {                                             \
      {n1, CONDITION_ALWAYS, 1, n2, 0, 0, FACING_SOUTH, 2, n3},                \
      {n1, CONDITION_ALWAYS, 1, n3, 0, 0, FACING_SOUTH, 2, n3},                \
      {n1, CONDITION_ALWAYS, 1, n4, 0, 0, FACING_SOUTH, 2, n3},                \
      {n5, CONDITION_ALWAYS, 1, n6, 0, 0, FACING_SOUTH, 2, n3},                \
      {n5, CONDITION_ALWAYS, 1, n7, 0, 0, FACING_SOUTH, 2, n3},                \
      {n5, CONDITION_ALWAYS, 1, n8, 0, 0, FACING_SOUTH, 2, n3},                \
      {n5, CONDITION_ALWAYS, 1, n9, 0, 0, FACING_SOUTH, 2, n3},                \
      {SCENE_TABLE_END},                                                       \
  };

#define DEFINE_G_DATA_02005B1C(name, n1)                                       \
  u32 name[] = {                                                               \
      SC_ARG(2),  SC_MOVE(PIXELS(n1), 0, PIXELS(176)), SC_SYNC,    SC_ARG(1),  \
      SC_WAIT(1), SC_SET(SW_FACING, FACING_SOUTH),     SC_WAIT(1), SC_END,     \
  };

#define DEFINE_G_DATA_02005118(name, n1, n2)                                   \
  s32 name[] = {                                                               \
      SC_MOVE_BY(0, 0, PIXELS(n1)),                                            \
      SC_SYNC,                                                                 \
      SC_MOVE_BY(0, 0, PIXELS(-n2)),                                           \
      SC_SYNC,                                                                 \
      SC_MOVE_BY(0, 0, PIXELS(n1)),                                            \
      SC_SYNC,                                                                 \
      SC_WAIT(2),                                                              \
      SC_LOOP(SC_FOREVER, SC_TOP),                                             \
  };

#define DEFINE_G_DATA_02004508(name, n1, n2, n3, n4)                           \
  u32 name[] = {                                                               \
      SC_MOVE(PIXELS(248), 0, PIXELS(n1)),                                     \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(248), 0, PIXELS(n2)),                                     \
      SC_SYNC,                                                                 \
      SC_MOVE(PIXELS(n3), 0, PIXELS(n4)),                                      \
      SC_SYNC,                                                                 \
      SC_SET(SW_BYTE_63, 1),                                                   \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02004FF0(name, n1)                                       \
  u32 name[] = {                                                               \
      SC_SET(SW_STEERING, 0),                                                  \
      SC_SET(SW_MOTION, n1),                                                   \
      SC_SET(SW_Y, PIXELS(32)),                                                \
      SC_SET(SW_GROUND, PIXELS(32)),                                           \
      SC_SET(SW_SPEED, 0x8000),                                                \
      SC_SET(SW_ACCEL, 0x800),                                                 \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02005800(name, n1, n2)                                   \
  u32 name[] = {                                                               \
      SC_ARG(n1),                                                              \
      SC_MOVE_XZ(PIXELS(n2), PIXELS(312)),                                     \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),                       \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_020017A4(name, n1, n2)                                   \
  u32 name[] = {                                                               \
      SC_SET(SW_SPEED, 0xe666),                                                \
      SC_ARG(2),                                                               \
      SC_MOVE(PIXELS(n1), 0, PIXELS(n2)),                                      \
      SC_SYNC,                                                                 \
      SC_ARG(1),                                                               \
      SC_END,                                                                  \
  };

#define DEFINE_G_DATA_02003398(name, n1, n2, n3, n4, n5, n6, n7)               \
  struct ActorPlacement name[] = {                                             \
      {n1, CONDITION_ALWAYS, 1, PIXELS(n2), 0, PIXELS(n3), FACING_SOUTH, n4,   \
       0},                                                                     \
      {n1, CONDITION_ALWAYS, 1, PIXELS(n5), 0, PIXELS(n6), FACING_SOUTH, n4,   \
       0},                                                                     \
      {290, CONDITION_ALWAYS, n7, 0, 0, 0, FACING_SOUTH, n4, 0},               \
      {290, CONDITION_ALWAYS, n7, 0, 0, 0, FACING_SOUTH, n4, 0},               \
      {SCENE_TABLE_END},                                                       \
  };

#define DEFINE_G_DATA_02003690(name, n1, n2, n3, n4, n5, n6, n7, n8)           \
  struct ActorPlacement name[] = {                                             \
      {n1, CONDITION_ALWAYS, 1, n2, 0, 0, FACING_SOUTH, 2, n3},                \
      {n4, CONDITION_ALWAYS, 1, n3, 0, 0, FACING_SOUTH, 2, n3},                \
      {n5, CONDITION_ALWAYS, 1, n6, 0, 0, FACING_SOUTH, 2, n3},                \
      {n7, CONDITION_ALWAYS, 1, n8, 0, 0, FACING_SOUTH, 2, n3},                \
      {SCENE_TABLE_END},                                                       \
  };

#define DEFINE_G_DATA_0200116C(name, n1, n2, n3, n4, n5, n6, n7)               \
  s32 name[] = {                                                               \
      11, n1, -1, n2,         -1, n3, -1, n4, -1,  n5,                         \
      -1, n6, -1, 0x10703085, -1, 12, n7, -1, 511,                             \
  };

#define DEFINE_G_DATA_02002F94(name, n1, n2)                                   \
  s32 name[] = {                                                               \
      SC_ADD(SW_SCALE_X, -n1),                                                 \
      SC_WAIT(1),                                                              \
      SC_LOOP(n2, SC_TOP),                                                     \
      SC_END,                                                                  \
  };

#define DEFINE_G_SCENE_EXITS_5(n1, n2, n3, n4)                                 \
  u32 gSceneExits[] = {                                                        \
      SCENE_EXITS(n1),                                                         \
      SCENE_EXIT_IF(1, 2, n2, EXIT_FLAG_NONE),                                 \
      SCENE_EXIT_IF(n3, 2, n4, EXIT_FLAG_NONE),                                \
      SCENE_EXITS_END,                                                         \
  };

#define DEFINE_G_DATA_02001330(name, n1, n2, n3, n4, n5, n6, n7, n8)           \
  struct ActorPlacement name[] = {                                             \
      {n1, CONDITION_ALWAYS, 1, PIXELS(n2), 0, PIXELS(n3), FACING_SOUTH, 2,    \
       0},                                                                     \
      {n1, CONDITION_ALWAYS, 1, PIXELS(n4), 0, PIXELS(n5), FACING_SOUTH, 2,    \
       n6},                                                                    \
      {n1, CONDITION_ALWAYS, 1, PIXELS(n7), 0, PIXELS(n8), FACING_SOUTH, 2,    \
       n6},                                                                    \
      {SCENE_TABLE_END},                                                       \
  };

#define DEFINE_G_DATA_02003434(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)  \
  u32 name[] = {                                                               \
      39,        2, 4,  n1, 0,   n2, 1,  4,         n3, 0, n4,     1,  4,      \
      0x1980000, 0, n5, 1,  4,   n6, 0,  0x2a80000, 1,  4, n7,     0,  n8,     \
      1,         4, n9, 0,  n10, 1,  39, 1,         22, 3, 0xd000, 17,         \
  };

#define DEFINE_G_DATA_020011AC(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)  \
  u32 name[] = {                                                               \
      0xffff014d, 7,  0x580000,  0, 0x1780000, 0x24000,                        \
      0xffff014d, 7,  0xd80000,  0, 0x1480000, 0x1024000,                      \
      0xffff014d, 7,  0xa80000,  0, 0xb80000,  0x1024000,                      \
      0xffff014d, 7,  0x1e80000, 0, 0x1980000, 0x1024000,                      \
      0xffff014d, 7,  0x2280000, 0, 0x1a80000, 0x1024000,                      \
      0xffff0073, 1,  n1,        0, n2,        0x12000,                        \
      0xffff0073, 1,  n3,        0, n4,        0x1c000,                        \
      0xffff0074, 1,  n5,        0, n6,        0x18000,                        \
      0xffff006a, n7, n8,        0, n9,        n10,                            \
      0x190a01a4, 1,  0x1d00000, 0, 0x1080000, 0x20000,                        \
      0xffff0038, 1,  0,         0, 0,         0x4000,                         \
      0xffff,     0,  0,         0, 0,         0,                              \
  };

#define DEFINE_G_SCENE_EXITS_6(n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11,   \
                               n12)                                            \
  u32 gSceneExits[] = {                                                        \
      SCENE_EXITS(n1),         SCENE_EXIT(1, n2, 1),    SCENE_EXIT(n3, n4, 3), \
      SCENE_EXIT(n5, n4, n6),  SCENE_EXIT(5, n7, 5),    SCENE_EXIT(6, n7, 6),  \
      SCENE_EXIT(7, n8, n9),   SCENE_EXIT(8, n10, n11), SCENE_EXIT(9, n12, 9), \
      SCENE_EXIT(10, n12, 10), SCENE_EXITS_END,                                \
  };

#endif
