// Draft, differ 210 of 193: advances Aqua Rock's two water streams one cell
// along the map: each stream follows the turn its cell's kind gives, leaves a
// splash on every open cell (255) it crosses, stops on a wall kind and counts
// the cells of the given kind it reaches; then it clears the splashes past the
// new count and opens the cells under those left.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct Splash {
  s32 x;
  s32 y;
  s32 z;
  u16 active;
  u16 direction;
  u32 sprite[3];
};

struct Stream {
  s32 x;
  s32 y;
  s32 z;
  u16 unk0c;
  u16 direction;
};

struct Splashes {
  u16 resource;
  u16 count;
  struct Stream streams[2];
  struct Splash splashes[32];
};

extern struct Splashes gData_02005970;
extern const s8 gData_0200534c[];

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);

s32 Func_020036b8(s32 first, s32 target) {
  s32 reached = 0;
  struct Splashes *state = &gData_02005970;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 count = state->count;
  struct Stream *stream = state->streams;
  s32 i = 1;
  struct Splash *splash;

  do {
    if (stream->x != 0) {
      s32 again;
      s32 turn;

      splash = &state->splashes[state->count];
      do {
        u8 *cells = FIELD_AT(map, u8 *, 312);
        s32 kind =
            cells[(((stream->x >> 20) + ((stream->z >> 20) << 7)) << 2) + 2];

        if (kind == 255) {
          splash->x = stream->x;
          splash->z = stream->z;
          again = 1;
          splash->y = stream->y;
          splash->active = 1;
          splash++;
          state->count++;
          turn = 0;
        } else if (kind >= first && kind <= first + 7) {
          turn = gData_0200534c[(stream->direction << 3) + kind - first];
          if (turn != 8)
            count = state->count;
          again = 0;
        } else if (kind == target) {
          count = state->count;
          turn = 8;
          reached++;
          again = 0;
        } else if (kind == 0) {
          turn = 8;
          again = 0;
        }
        if (turn != 8) {
          stream->direction = (stream->direction + turn) & 3;
          Vector_AddPolarOffsetFar(0x100000, stream->direction << 14,
                                   &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  } while (--i >= 0);
  if (count <= 31) {
    splash = &state->splashes[count];
    i = 32 - count;
    do {
      if (splash->active != 0)
        splash->active = 0;
      splash++;
    } while (--i != 0);
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
    do {
      if (splash->active != 0) {
        u8 *cell = FIELD_AT(map, u8 *, 312) +
                   (((splash->x >> 20) + ((splash->z >> 20) << 7)) << 2);

        cell[2] = 0;
        cell[3] |= 128;
      }
      splash++;
    } while (--i != 0);
  }
  return reached;
}
