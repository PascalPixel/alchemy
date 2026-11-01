// Draft, differ 183 of 193, skeleton gap 61 (tla-en, 2026-11-01)
// Advances Aqua Rock's two water streams one cell along the map: each stream
// follows the turn its cell's kind gives, leaves a splash on every open cell
// (255) it crosses, stops on a wall kind and counts the cells of the given kind
// it reaches; then it clears the splashes past the new count and opens the
// cells under those left.
//
// Left is mostly allocation: the listing has state in r4 (saved round the call)
// and again and turn in r10 and r9; here again takes r4 and state r8, which
// changes the mov pairs and turns the 3 into a pool load. Measuring state's
// priority against again is the next step. Tried without gain: separate
// counters or splash pointers per loop, narrower scopes, direct global access.
// Also left: the first read of stream->x in the do body. PRE reuses the if's
// load and inserts one on the back edge, which adds a b.n, makes the loop
// multi-entry for loop.c and lengthens the outer branch to blt plus b; the
// listing reloads. Reading through state->streams[i] stops it, but i is then no
// longer only a counter and the outer loop does not reverse. With a memory
// clobber before the do, in a scratch copy only, the reload appears and the gap
// barely moves, so it is not the main difference.
// The two tail loops are label and goto: the for form lets loop.c hoist the 0
// and 128 out, which the listing keeps inside; the reversed counters are written
// out to match. Whether the source really had such loops is open.
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
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Splashes *state = &gData_02005970;
  s32 count = state->count;
  s32 again = 0;
  s32 turn;
  s32 i;
  struct Stream *stream = state->streams;
  struct Splash *splash;

  for (i = 0; i < 2; i++) {
    if (stream->x != 0) {
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
  }
  splash = &state->splashes[count];
  if (count < 32) {
    i = 32 - count;
  clear:
    if (splash->active != 0)
      splash->active = 0;
    splash++;
    if (--i != 0)
      goto clear;
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
  open:
    if (splash->active != 0) {
      u8 *cell = FIELD_AT(map, u8 *, 312) +
                 (((splash->x >> 20) + ((splash->z >> 20) << 7)) << 2);

      cell[2] = 0;
      cell[3] |= 128;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
